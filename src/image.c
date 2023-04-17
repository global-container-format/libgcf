#include <gcf/gcf.h>
#include <gcf/internal/memory.h>
#include <gcf/internal/io.h>

GCFATTR gcf_result GCFAPI gcf_read_image_mip_level(
    gcf_read_ctx * const restrict ctx,
    gcf_resource_descriptor const * const restrict res_descriptor,
    gcf_image_mip_level_descriptor const * const restrict mip_descriptor,
    void * const out_data
) {
    void * const compressed_data = memory_alloc(ctx, mip_descriptor->compressed_size);
    gcf_result result = GCF_RESULT_MEMORY_ERROR;

    if(compressed_data) {
        result = read_data(ctx, mip_descriptor->compressed_size, compressed_data);

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

GCFATTR gcf_result GCFAPI gcf_read_image_mip_level_descriptor(
    gcf_read_ctx * const restrict ctx,
    gcf_resource_descriptor const * const restrict descriptor,
    gcf_image_mip_level_descriptor * const out_level_descriptor
) {
    return read_data(
        ctx,
        sizeof(gcf_image_mip_level_descriptor),
        out_level_descriptor
    );
}
