#include <memory.h>
#include <zlib.h>
#include <gcf/gcf.h>

static bool is_magic_valid(uint32_t const actual_magic) {
    static char const str_magic[] = GCF_MAGIC_VALUE;

    uint32_t const magic_value = (
        (uint32_t)str_magic[0]
        | ((uint32_t)str_magic[1] << 8)
        | ((uint32_t)str_magic[2] << 16)
        | ((uint32_t)str_magic[3] << 24)
    );

    return actual_magic == magic_value;
}

static uint32_t read_data(
    gcf_read_ctx * const ctx,
    void * const destination,
    size_t const data_size
) {
    return ctx->read_clbk(destination, data_size, ctx->user_data);
}

static inline size_t align_size(size_t const orig_size, size_t const alignment) {
    size_t const mask = alignment - 1;

    return (orig_size + mask) & ~mask;
}

static inline gcf_result read_generic_data(
    gcf_read_ctx * const restrict ctx,
    uint32_t const data_size,
    void * const out_data
) {
    if(read_data(ctx, out_data, data_size) != data_size) {
        return GCF_RESULT_READ_ERROR;
    }

    return GCF_RESULT_SUCCESS;
}

static inline void * memory_alloc(gcf_read_ctx * const ctx, size_t const size) {
    return ctx->alloc_clbk(size, ctx->user_data);
}

static inline void memory_free(gcf_read_ctx * const ctx, void * const address) {
    ctx->free_clbk(address, ctx->user_data);
}

static void *zlib_alloc(voidpf opaque, uInt items, uInt size) {
    gcf_read_ctx * const ctx = opaque;
    size_t const total_alloc_size = items * size;

    return memory_alloc(ctx, total_alloc_size);
}

static void zlib_free(voidpf opaque, voidpf address) {
    gcf_read_ctx * const ctx = opaque;

    memory_free(ctx, address);
}

GCFATTR gcf_result GCFAPI gcf_read_context_initialize(
    gcf_read_ctx * const ctx,
    gcf_read_clbk const read_clbk,
    gcf_alloc_clbk const alloc_clbk,
    gcf_free_clbk const free_clbk,
    gcf_decompress_clbk const decompress_clbk,
    void * const user_data
) {
    if(!read_clbk) {
        return GCF_RESULT_INVALID_CALLBACK;
    }

    if(!alloc_clbk) {
        return GCF_RESULT_INVALID_CALLBACK;
    }

    if(!free_clbk) {
        return GCF_RESULT_INVALID_CALLBACK;
    }

    if(!decompress_clbk) {
        return GCF_RESULT_INVALID_CALLBACK;
    }

    memset(ctx, 0, sizeof(gcf_read_ctx));

    ctx->read_clbk = read_clbk;
    ctx->alloc_clbk = alloc_clbk;
    ctx->free_clbk = free_clbk;
    ctx->decompress_clbk = decompress_clbk;
    ctx->user_data = user_data;

    if(read_data(ctx, &ctx->header, sizeof(gcf_header)) != sizeof(gcf_header)) {
        return GCF_RESULT_READ_ERROR;
    }

    if(!is_magic_valid(ctx->header.magic)) {
        return GCF_RESULT_INVALID_MAGIC;
    }

    return GCF_RESULT_SUCCESS;
}

GCFATTR void GCFAPI gcf_read_context_destroy(
    gcf_read_ctx const * const ctx
) {
    ((void)0);
}

GCFATTR gcf_result GCFAPI gcf_read_resource_descriptor(
    gcf_read_ctx * const restrict ctx,
    gcf_resource_descriptor * const restrict out_descriptor
) {
    if(ctx->current_resource_index >= ctx->header.resource_count) {
        return GCF_RESULT_END;
    }

    ctx->current_resource_index++;

    if(
        read_data(ctx, out_descriptor, sizeof(gcf_resource_descriptor))
        != sizeof(gcf_resource_descriptor)
    ) {
        return GCF_RESULT_READ_ERROR;
    }

    return GCF_RESULT_SUCCESS;
}

GCFATTR gcf_result GCFAPI gcf_read_resource_content(
    gcf_read_ctx * const restrict ctx,
    gcf_resource_descriptor const * const restrict descriptor,
    void * const restrict out_content
) {
    if(
        read_data(ctx, out_content, descriptor->size) != descriptor->size
    ) {
        return GCF_RESULT_READ_ERROR;
    }

    /*
     * Skip padding.
     *
     * Since all the format structure sizes are multiple of 64 bits, the only
     * thing that can be misaligned is the actual content data.
     */
    if(~ctx->header.flags & GCF_CONTAINER_FLAG_BIT_UNPADDED) {
        size_t const aligned_size = align_size(descriptor->size, sizeof(uint64_t));
        size_t const padding_size = aligned_size - descriptor->size;

        read_data(ctx, NULL, padding_size);
    }

    return GCF_RESULT_SUCCESS;
}

GCFATTR gcf_result GCFAPI gcf_read_image_mip_level_descriptor(
    gcf_read_ctx * const restrict ctx,
    gcf_resource_descriptor const * const restrict descriptor,
    gcf_image_mip_level_descriptor * const out_level_descriptor
) {
    return read_generic_data(
        ctx,
        sizeof(gcf_image_mip_level_descriptor),
        out_level_descriptor
    );
}

static gcf_result decompress_zlib_or_deflate(
    gcf_read_ctx * const restrict ctx,
    gcf_resource_descriptor const * const restrict descriptor,
    uint32_t const compressed_data_size,
    void * const compressed_data,
    uint32_t const decompressed_data_size,
    void * const out_decompressed_data
) {
    z_stream stream = {
        .next_in = compressed_data,
        .next_out = out_decompressed_data,
        .avail_in = compressed_data_size,
        .avail_out = decompressed_data_size,
        .zalloc = zlib_alloc,
        .zfree = zlib_free,
        .opaque = ctx
    };

    if(
        inflateInit2(
            &stream,
            descriptor->supercompression_scheme == GCF_SUPERCOMPRESSION_SCHEME_DEFLATE
            ? -15
            : 15
        ) != Z_OK
    ) {
        return GCF_RESULT_DECOMPRESS_ERROR;
    }

    int status;

    do {
        if(stream.avail_out == 0 || stream.avail_in == 0) {
            return GCF_RESULT_DECOMPRESS_ERROR;
        }

        status = inflate(&stream, Z_FINISH);

        if(status > Z_STREAM_END || status < Z_OK) {
            return GCF_RESULT_DECOMPRESS_ERROR;
        }
    } while(status != Z_STREAM_END);

    if(inflateEnd(&stream) != Z_OK) {
        return GCF_RESULT_DECOMPRESS_ERROR;
    }

    return GCF_RESULT_SUCCESS;
}

static gcf_result decompress_identity(
    uint32_t const compressed_data_size,
    void * const compressed_data,
    uint32_t const decompressed_data_size,
    void * const out_decompressed_data
) {
    memcpy(out_decompressed_data, compressed_data, decompressed_data_size);

    return GCF_RESULT_SUCCESS;
}

GCFATTR gcf_result GCFAPI gcf_decompress(
    gcf_read_ctx * const restrict ctx,
    gcf_resource_descriptor const * const restrict descriptor,
    uint32_t const compressed_data_size,
    void * const compressed_data,
    uint32_t const decompressed_data_size,
    void * const out_decompressed_data
) {
    switch(descriptor->supercompression_scheme) {
        case GCF_SUPERCOMPRESSION_SCHEME_NONE:
            return decompress_identity(
                compressed_data_size,
                compressed_data,
                decompressed_data_size,
                out_decompressed_data
            );

        case GCF_SUPERCOMPRESSION_SCHEME_DEFLATE:
        case GCF_SUPERCOMPRESSION_SCHEME_ZLIB:
            return decompress_zlib_or_deflate(
                ctx,
                descriptor,
                compressed_data_size,
                compressed_data,
                decompressed_data_size,
                out_decompressed_data
            );

        default:
            return GCF_RESULT_DECOMPRESS_ERROR;
    }
}

GCFATTR gcf_result GCFAPI gcf_read_image_mip_level(
    gcf_read_ctx * const restrict ctx,
    gcf_resource_descriptor const * const restrict res_descriptor,
    gcf_image_mip_level_descriptor const * const restrict mip_descriptor,
    void * const out_data
) {
    void * const compressed_data = memory_alloc(ctx, mip_descriptor->compressed_size);
    gcf_result result = GCF_RESULT_MEMORY_ERROR;

    if(compressed_data) {
        result = read_generic_data(ctx, mip_descriptor->compressed_size, compressed_data);

        if(result == GCF_RESULT_SUCCESS) {
            result = ctx->decompress_clbk(
                ctx,
                res_descriptor,
                mip_descriptor->compressed_size,
                compressed_data,
                mip_descriptor->uncompressed_size,
                out_data
            );
        }

        memory_free(ctx, compressed_data);
    }

    return result;
}
