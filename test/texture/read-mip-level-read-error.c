#include <memory.h>
#include <gcf/test.h>

int main(int argc, char **argv) {
    gcf_read_ctx ctx;
    gcf_resource_descriptor resource_descriptor;
    gcf_texture_mip_level_descriptor cm_mip_level_descriptor;
    gcf_test_init_read_context(&ctx, GCF_TEST_RESOURCE_TEXTURE_COMPRESSED_TEST);

    GCF_ASSERT(ctx.header.resource_count >= 1);

    GCF_ASSERT_RESULT(
        gcf_read_resource_descriptor(&ctx, &resource_descriptor)
    );

    GCF_ASSERT_RESULT(
        gcf_read_texture_mip_level_descriptor(&ctx, &resource_descriptor, &cm_mip_level_descriptor)
    );

    void * const data = malloc(cm_mip_level_descriptor.uncompressed_size);

    ctx.user_data = NULL;

    GCF_ASSERT(
        gcf_read_texture_mip_level(&ctx, &resource_descriptor, &cm_mip_level_descriptor, data)
        == GCF_RESULT_READ_ERROR
    );

    free(data);
    gcf_read_context_destroy(&ctx);
    GCF_TEST_SUCCEED();
}
