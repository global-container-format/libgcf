#include <gcf/test.h>
#include <gcf/gcf.h>

int main(int argc, char **argv) {
    gcf_read_ctx ctx;
    gcf_resource_descriptor resource_descriptor;
    gcf_texture_extended_resource_descriptor tex_descriptor;
    gcf_test_init_read_context(&ctx, GCF_TEST_RESOURCE_TEXTURE_MIPMAPS);

    GCF_ASSERT(ctx.header.resource_count >= 1);

    GCF_ASSERT_RESULT(
        gcf_read_resource_descriptor(&ctx, &resource_descriptor)
    );

    GCF_ASSERT(resource_descriptor.extension_size == sizeof(gcf_texture_extended_resource_descriptor));

    GCF_ASSERT_RESULT(
        gcf_read_texture_extended_resource_descriptor(&ctx, &tex_descriptor)
    );

    GCF_ASSERT(resource_descriptor.resource_type == GCF_RESOURCE_TYPE_TEXTURE);
    GCF_ASSERT(resource_descriptor.format == GCF_FORMAT_R8G8B8A8_UINT);
    GCF_ASSERT(resource_descriptor.content_size == 349304);
    GCF_ASSERT(resource_descriptor.extension_size == sizeof(gcf_texture_extended_resource_descriptor));
    GCF_ASSERT(resource_descriptor.supercompression_scheme == GCF_SUPERCOMPRESSION_SCHEME_NONE);
    GCF_ASSERT(tex_descriptor.base_width == 256);
    GCF_ASSERT(tex_descriptor.base_height == 256);
    GCF_ASSERT(tex_descriptor.base_depth == 1);
    GCF_ASSERT(tex_descriptor.layer_count == 1);
    GCF_ASSERT(tex_descriptor.mip_level_count == 5);
    GCF_ASSERT(tex_descriptor.flags == GCF_TEXTURE_FLAG_BIT_2D);
    GCF_ASSERT(tex_descriptor.texture_group == 123);
    GCF_ASSERT(tex_descriptor.rsvd == 0);

    GCF_ASSERT(
        gcf_read_resource_descriptor(&ctx, &resource_descriptor)
        == GCF_RESULT_END
    );

    gcf_read_context_destroy(&ctx);
    GCF_TEST_SUCCEED();
}
