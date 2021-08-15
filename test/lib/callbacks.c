#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

uint32_t gcf_test_read_clbk(
    void * const destination,
    size_t const n_bytes,
    void * const user_data
) {
    FILE * const f = user_data;

    if(f) {
        if(destination) {
            return fread(destination, 1, n_bytes, f);
        } else {
            return fseek(f, n_bytes, SEEK_CUR);
        }
    }

    return UINT32_MAX;
}

void* gcf_test_alloc_clbk(
    size_t size,
    void *user_data
) {
    ((void)user_data);

    return malloc(size);
}

void gcf_test_free_clbk(
    void * const ptr,
    void *user_data
) {
    ((void)user_data);

    return free(ptr);
}
