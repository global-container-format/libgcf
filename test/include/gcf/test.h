#ifndef GCF_TEST_H
#define GCF_TEST_H

#include <stdlib.h>
#include <stdio.h>
#include <gcf/gcf.h>
#include <gcf/test-defs.h>

#define GCF_TEST_SUCCEED() exit(EXIT_SUCCESS)
#define GCF_TEST_FAIL() exit(EXIT_FAILURE)
#define GCF_TEST_FAIL2(msg) do { puts((msg)); GCF_TEST_FAIL(); } while(0)
#define GCF_ASSERT(x) if(!(x)) { GCF_TEST_FAIL2(GCF_QUOTE(x)); }
#define GCF_ASSERT_RESULT(r) GCF_ASSERT((r) == GCF_RESULT_SUCCESS)

uint32_t gcf_test_read_clbk(
    void * const destination,
    size_t const n_bytes,
    void * const user_data
);

void* gcf_test_alloc_clbk(
    size_t size,
    void *user_data
);

void gcf_test_free_clbk(
    void * const ptr,
    void *user_data
);

void gcf_test_init_read_context(
    gcf_read_ctx * const ctx,
    char const * const resource_path
);

#endif /* !GCF_TEST_H */
