#ifndef GCF_MEMORY_H
#define GCF_MEMORY_H

#include <gcf/gcf.h>

size_t align_size(size_t const orig_size, size_t const alignment);

void * memory_alloc(gcf_read_ctx * const ctx, size_t const size);
void memory_free(gcf_read_ctx * const ctx, void * const address);

#endif /* !GCF_MEMORY_H */
