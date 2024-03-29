#include <stdio.h>
#include <gcf/test.h>

int main(int argc, char **argv) {
    gcf_read_ctx ctx;
    gcf_resource_descriptor resource_descriptor;
    gcf_test_init_read_context(&ctx, GCF_TEST_RESOURCE_TEXTURE_MIPMAPS_UNPADDED);

    GCF_ASSERT(ctx.header.resource_count >= 1);

    GCF_ASSERT_RESULT(
        gcf_read_resource_descriptor(&ctx, &resource_descriptor)
    );

    void * const content = malloc(resource_descriptor.content_size);
    FILE * const f = ctx.user_data;
    long const pos = ftell(f);

    GCF_ASSERT_RESULT(gcf_read_resource_content(&ctx, &resource_descriptor, content));

    long const pos_end = ftell(f);
    GCF_ASSERT(pos_end == pos + resource_descriptor.content_size);

    GCF_ASSERT(
        gcf_read_resource_content(&ctx, &resource_descriptor, content)
        == GCF_RESULT_READ_ERROR
    );

    gcf_read_context_destroy(&ctx);
    GCF_TEST_SUCCEED();
}
