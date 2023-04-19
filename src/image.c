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
