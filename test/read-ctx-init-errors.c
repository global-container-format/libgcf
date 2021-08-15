#include <stdio.h>
#include <gcf/test.h>

int main(int argc, char **argv) {
    gcf_read_ctx ctx;

    gcf_result result = gcf_read_context_initialize(&ctx, NULL, gcf_test_alloc_clbk, gcf_test_free_clbk, gcf_decompress, NULL);
    GCF_ASSERT(result == GCF_RESULT_INVALID_CALLBACK);

    result = gcf_read_context_initialize(&ctx, gcf_test_read_clbk, NULL, gcf_test_free_clbk, gcf_decompress, NULL);
    GCF_ASSERT(result == GCF_RESULT_INVALID_CALLBACK);

    result = gcf_read_context_initialize(&ctx, gcf_test_read_clbk, gcf_test_alloc_clbk, NULL, gcf_decompress, NULL);
    GCF_ASSERT(result == GCF_RESULT_INVALID_CALLBACK);

    result = gcf_read_context_initialize(&ctx, gcf_test_read_clbk, gcf_test_alloc_clbk, gcf_test_free_clbk, NULL, NULL);
    GCF_ASSERT(result == GCF_RESULT_INVALID_CALLBACK);

    FILE * const empty_file = fopen(GCF_TEST_RESOURCE_IMAGE_MIPMAPS, "rb");
    GCF_ASSERT(empty_file);
    fseek(empty_file, 0, SEEK_END);

    result = gcf_read_context_initialize(
        &ctx,
        gcf_test_read_clbk,
        gcf_test_alloc_clbk,
        gcf_test_free_clbk,
        gcf_decompress,
        empty_file
    );
    GCF_ASSERT(result == GCF_RESULT_READ_ERROR);
    fclose(empty_file);

    FILE * const wrong_magic_file = fopen(GCF_TEST_RESOURCE_IMAGE_MIPMAPS, "rb");
    GCF_ASSERT(wrong_magic_file);
    fseek(wrong_magic_file, 1, SEEK_SET);

    result = gcf_read_context_initialize(
        &ctx,
        gcf_test_read_clbk,
        gcf_test_alloc_clbk,
        gcf_test_free_clbk,
        gcf_decompress,
        wrong_magic_file
    );
    GCF_ASSERT(result == GCF_RESULT_INVALID_MAGIC);
    fclose(wrong_magic_file);

    GCF_TEST_SUCCEED();
}
