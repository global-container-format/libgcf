#include <gcf/gcf.h>

void * memory_alloc(gcf_read_ctx * const ctx, size_t const size) {
    return ctx->alloc_clbk(size, ctx->user_data);
}

void memory_free(gcf_read_ctx * const ctx, void * const address) {
    ctx->free_clbk(address, ctx->user_data);
}

size_t align_size(size_t const orig_size, size_t const alignment) {
    size_t const mask = alignment - 1;

    return (orig_size + mask) & ~mask;
}
