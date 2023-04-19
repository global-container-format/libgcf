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
#include <gcf/gcf.h>
#include <gcf/internal/memory.h>
#include <gcf/internal/io.h>

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

    GCF_CHECK(read_data(ctx, sizeof(gcf_header), &ctx->header));

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

    GCF_CHECK(read_data(ctx, sizeof(gcf_resource_descriptor), out_descriptor));

    return GCF_RESULT_SUCCESS;
}

GCFATTR gcf_result GCFAPI gcf_read_resource_content(
    gcf_read_ctx * const restrict ctx,
    gcf_resource_descriptor const * const restrict descriptor,
    void * const restrict out_content
) {
    GCF_CHECK(read_data(ctx, descriptor->size, out_content));

    /*
     * Skip padding.
     *
     * Since all the format structure sizes are multiple of 64 bits, the only
     * thing that can be misaligned is the actual content data.
     */
    if(~ctx->header.flags & GCF_CONTAINER_FLAG_BIT_UNPADDED) {
        size_t const aligned_size = align_size(descriptor->size, sizeof(uint64_t));
        size_t const padding_size = aligned_size - descriptor->size;

        read_data(ctx, padding_size, NULL);
    }

    return GCF_RESULT_SUCCESS;
}
