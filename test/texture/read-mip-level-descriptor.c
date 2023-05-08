#include <memory.h>
#include <gcf/test.h>

int main(int argc, char **argv) {
    gcf_read_ctx ctx;
    gcf_resource_descriptor resource_descriptor;
    gcf_texture_extended_resource_descriptor tex_descriptor;
    gcf_texture_mip_level_descriptor cm_mip_level_descriptor;
    gcf_test_init_read_context(&ctx, GCF_TEST_RESOURCE_TEXTURE_MIPMAPS);

    GCF_ASSERT(ctx.header.resource_count >= 1);

    GCF_ASSERT_RESULT(
        gcf_read_resource_descriptor(&ctx, &resource_descriptor)
    );

    GCF_ASSERT_RESULT(
        gcf_read_texture_extended_resource_descriptor(&ctx, &tex_descriptor)
    );

    GCF_ASSERT_RESULT(
        gcf_read_texture_mip_level_descriptor(&ctx, &resource_descriptor, &cm_mip_level_descriptor)
    );

    GCF_ASSERT(cm_mip_level_descriptor.compressed_size == 262144);
    GCF_ASSERT(cm_mip_level_descriptor.uncompressed_size == 262144);
    GCF_ASSERT(cm_mip_level_descriptor.row_stride == 1024);
    GCF_ASSERT(cm_mip_level_descriptor.slice_stride == 262144);
    GCF_ASSERT(cm_mip_level_descriptor.layer_stride == 262144);
    GCF_ASSERT(cm_mip_level_descriptor.rsvd == 0);

    gcf_read_context_destroy(&ctx);
    GCF_TEST_SUCCEED();
}
