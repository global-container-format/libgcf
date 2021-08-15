#include <stdio.h>
#include <memory.h>
#include <gcf/gcf.h>
#include <gcf/test.h>

void gcf_test_init_read_context(
    gcf_read_ctx * const ctx,
    char const * const resource_path
) {
    FILE * const f = fopen(resource_path, "rb");

    if(!f) {
        GCF_TEST_FAIL();
    }

    gcf_result result = gcf_read_context_initialize(
        ctx,
        gcf_test_read_clbk,
        gcf_test_alloc_clbk,
        gcf_test_free_clbk,
        gcf_decompress,
        f
    );

    GCF_ASSERT_RESULT(result);
}
