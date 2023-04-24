/*
Copyright © 2021-2023 Alfredo Mungo <alfredo.mungo@protonmail.ch>

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the “Software”), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
of the Software, and to permit persons to whom the Software is furnished to do
so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/
#include <memory.h>
#include <zlib.h>
#include <gcf/gcf.h>
#include <gcf/internal/memory.h>

static void *zlib_alloc(voidpf opaque, uInt items, uInt size) {
    gcf_read_ctx * const ctx = opaque;
    size_t const total_alloc_size = items * size;

    return memory_alloc(ctx, total_alloc_size);
}

static void zlib_free(voidpf opaque, voidpf address) {
    gcf_read_ctx * const ctx = opaque;

    memory_free(ctx, address);
}

static gcf_result decompress_zlib_or_deflate(
    gcf_read_ctx * const GCF_RESTRICT ctx,
    gcf_resource_descriptor const * const GCF_RESTRICT descriptor,
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
    gcf_read_ctx * const GCF_RESTRICT ctx,
    gcf_resource_descriptor const * const GCF_RESTRICT descriptor,
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
