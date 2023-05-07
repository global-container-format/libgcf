#include <gcf/test.h>

int main(int argc, char **argv) {
    gcf_read_ctx ctx;
    gcf_test_init_read_context(&ctx, GCF_TEST_RESOURCE_TEXTURE_MIPMAPS);

    GCF_ASSERT(ctx.header.flags == 0);
    GCF_ASSERT(ctx.header.resource_count == 1);

    gcf_read_context_destroy(&ctx);
    GCF_TEST_SUCCEED();
}
