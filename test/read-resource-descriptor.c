#include <gcf/test.h>

#ifndef VK_FORMAT_R8G8B8A8_UINT
    #define VK_FORMAT_R8G8B8A8_UINT 41
#endif /* VK_FORMAT_R8G8B8A8_UINT */

int main(int argc, char **argv) {
    gcf_read_ctx ctx;
    gcf_resource_descriptor resource_descriptor;
    gcf_test_init_read_context(&ctx, GCF_TEST_RESOURCE_IMAGE_MIPMAPS);

    GCF_ASSERT(ctx.header.resource_count >= 1);

    GCF_ASSERT_RESULT(
        gcf_read_resource_descriptor(&ctx, &resource_descriptor)
    );

    GCF_ASSERT(resource_descriptor.resource_type == GCF_RESOURCE_TYPE_IMAGE);
    GCF_ASSERT(resource_descriptor.format == VK_FORMAT_R8G8B8A8_UINT);
    GCF_ASSERT(resource_descriptor.size == 349344);
    GCF_ASSERT(resource_descriptor.supercompression_scheme == GCF_SUPERCOMPRESSION_SCHEME_NONE);
    GCF_ASSERT(resource_descriptor.type_data.image.width == 256);
    GCF_ASSERT(resource_descriptor.type_data.image.height == 256);
    GCF_ASSERT(resource_descriptor.type_data.image.depth == 1);
    GCF_ASSERT(resource_descriptor.type_data.image.layer_count == 1);
    GCF_ASSERT(resource_descriptor.type_data.image.mip_level_count == 5);
    GCF_ASSERT(resource_descriptor.type_data.image.flags == GCF_COLOR_MAP_FLAG_IMAGE_BIT_IMAGE_2D);
    GCF_ASSERT(resource_descriptor.type_data.image.rsvd1 == 0);
    GCF_ASSERT(resource_descriptor.type_data.image.rsvd2 == 0);

    GCF_ASSERT(
        gcf_read_resource_descriptor(&ctx, &resource_descriptor)
        == GCF_RESULT_END
    );

    gcf_read_context_destroy(&ctx);
    GCF_TEST_SUCCEED();
}
