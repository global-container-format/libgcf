#ifndef GCF_TEST_H
#define GCF_TEST_H

#include <stdlib.h>
#include <gcf/gcf.h>

#define GCF_TEST_RESOURCE_FOLDER "test-assets"
#define GCF_TEST_RESOURCE_IMAGE_MIPMAPS GCF_TEST_RESOURCE_FOLDER "/image-mipmaps.gcf"
#define GCF_TEST_RESOURCE_IMAGE_MIPMAPS_UNPADDED GCF_TEST_RESOURCE_FOLDER "/image-mipmaps-unpadded.gcf"
#define GCF_TEST_RESOURCE_IMAGE_COMPRESSED_ZLIB GCF_TEST_RESOURCE_FOLDER "/image-compressed-zlib.gcf"
#define GCF_TEST_RESOURCE_IMAGE_UNCOMPRESSED GCF_TEST_RESOURCE_FOLDER "/image-uncompressed.gcf"
#define GCF_TEST_RESOURCE_IMAGE_COMPRESSED_TEST GCF_TEST_RESOURCE_FOLDER "/image-compressed-test.gcf"

#define GCF_TEST_SUCCEED() exit(EXIT_SUCCESS)
#define GCF_TEST_FAIL() exit(EXIT_FAILURE)
#define GCF_ASSERT(x) if(!(x)) { GCF_TEST_FAIL(); }
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
