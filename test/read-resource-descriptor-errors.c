#include <stdio.h>
#include <gcf/test.h>

int main(int argc, char **argv) {
    gcf_read_ctx ctx;
    gcf_resource_descriptor resource_descriptor;
    gcf_test_init_read_context(&ctx, GCF_TEST_RESOURCE_TEXTURE_MIPMAPS);

    GCF_ASSERT(ctx.header.resource_count >= 1);

    FILE * const f = ctx.user_data;
    fseek(f, 0, SEEK_END);

    GCF_ASSERT(
        gcf_read_resource_descriptor(&ctx, &resource_descriptor)
        == GCF_RESULT_READ_ERROR
    );

    gcf_read_context_destroy(&ctx);
    GCF_TEST_SUCCEED();
}
