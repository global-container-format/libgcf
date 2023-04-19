/*
Copyright © 2021 Alfredo Mungo <alfredo.mungo@protonmail.ch>

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the “Software”), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
of the Software, and to permit persons to whom the Software is furnished to do
so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/
#ifndef GCF_CONTAINER_H
#define GCF_CONTAINER_H

#include <stdbool.h>
#include <stdint.h>
#include <gcf/version.h>
#include <gcf/api.h>

#define GCF_VALUE(value) value
#define GCF_QUOTE_LITERAL(value) #value
#define GCF_QUOTE(value) GCF_QUOTE_LITERAL(value)

#if GCF_VERSION_MAJOR < 9
    #define GCF_MAGIC_PREFIX "GC0"
#else
    #define GCF_MAGIC_PREFIX "GC"
#endif /* GCF_VERSION_MAJOR > 9 */

/**
 * Magic value.
 */
#define GCF_MAGIC_VALUE GCF_MAGIC_PREFIX GCF_QUOTE(GCF_VERSION_MAJOR)

/**
 * Check whether a result is successful or not. Return from
 * the current function with the result value if the result value
 * is not successful.
 *
 * @param result The result expression to test.
 */
#define GCF_CHECK(result) do { gcf_result const _gcf_check_r = (result); if(_gcf_check_r != GCF_RESULT_SUCCESS) { return _gcf_check_r; } } while(0)

typedef struct gcf_read_ctx gcf_read_ctx;

/**
 * Header of a GCF sequence.
 */
typedef struct gcf_header {
    /**
     * The magic value. Must always be `GCF_MAGIC_VALUE`.
     */
    uint32_t magic;

    /**
     * The number of resources stored in the container.
     */
    uint16_t resource_count;

    /**
     * The container flags.
     *
     * @see gcf_container_flag_bits
     */
    uint16_t flags;
} gcf_header;

/**
 * The container flags.
 */
typedef enum gcf_container_flag_bits {
    /**
     * No padding between resources.
     */
    GCF_CONTAINER_FLAG_BIT_UNPADDED = 0x0001
} gcf_container_flag_bits;

/**
 * Image type-specific data.
 */
typedef struct gcf_image_resource_descriptor {
    /**
     * Width.
     */
    uint16_t width;

    /**
     * Height.
     */
    uint16_t height;

    /**
     * Depth.
     */
    uint16_t depth;

    /**
     * Layer count.
     */
    uint8_t layer_count;

    /**
     * Mip level count.
     */
    uint8_t mip_level_count;

    /**
     * Resource-specific flags.
     */
    uint16_t flags;

    /**
     * Reserved.
     */
    uint16_t rsvd1;

    /**
     * Reserved.
     */
    uint32_t rsvd2;
} gcf_image_resource_descriptor;

/**
 * Blob type-specific data.
 */
typedef struct gcf_blob_resource_descriptor {
    /**
     * Uncompressed resource size.
     */
    uint64_t uncompressed_size;

    /**
     * Reserved.
     */
    uint64_t rsvd;
} gcf_blob_resource_descriptor;

/**
 * Type-specific data.
 */
typedef union gcf_resource_descriptor_type_data {
    /**
     * Blob type-specific data.
     */
    gcf_blob_resource_descriptor blob;

    /**
     * Image type-specific data.
     */
    gcf_image_resource_descriptor image;
} gcf_resource_descriptor_type_data;

/**
 * Resource descriptor.
 */
typedef struct gcf_resource_descriptor {
    /**
     * Resource type.
     *
     * @see gcf_resource_type
     */
    uint32_t resource_type;

    /**
     * Data format value compatible with Vulkan's `VkFormat`.
     *
     * @see
     * https://www.khronos.org/registry/vulkan/specs/1.2-extensions/html/chap43.html#VkFormat
     */
    uint32_t format;

    /**
     * Resource size, in bytes, excluding padding.
     */
    uint32_t size;

    /**
     * Supercompression scheme.
     *
     * @see gcf_supercompression_scheme
     */
    uint16_t supercompression_scheme;

    /**
     * Reserved.
     */
    uint16_t rsvd;

    /**
     * Type-specific descriptor data.
     */
    gcf_resource_descriptor_type_data type_data;
} gcf_resource_descriptor;

/**
 * Resource type.
 */
typedef enum gcf_resource_type {

    /**
     * A generic resource.
     */
    GCF_RESOURCE_TYPE_BLOB,

    /**
     * An image resource.
     */
    GCF_RESOURCE_TYPE_IMAGE,

    /**
     * First available user-private resource ID.
     */
    GCF_RESOURCE_TYPE_USER = 0x70000000,

    /**
     * Test resource type. DO NOT USE.
     */
    GCF_RESOURCE_TYPE_TEST = 0xffffffff
} gcf_resource_type;

/**
 * Supercompression scheme.
 */
typedef enum gcf_supercompression_scheme {
    /**
     * No compression.
     */
    GCF_SUPERCOMPRESSION_SCHEME_NONE,

    /**
     * ZLib compression.
     */
    GCF_SUPERCOMPRESSION_SCHEME_ZLIB,

    /**
     * DEFLATE compression.
     */
    GCF_SUPERCOMPRESSION_SCHEME_DEFLATE,

    /**
     * First available user-private supercompression scheme.
     */
    GCF_SUPERCOMPRESSION_SCHEME_USER = 0x7000,

    /**
     * Test supercompression scheme. DO NOT USE.
     */
    GCF_SUPERCOMPRESSION_SCHEME_TEST = 0xffff
} gcf_supercompression_scheme;

/**
 * Operation result.
 */
typedef enum gcf_result {
    /**
     * Operation completed successfully.
     */
    GCF_RESULT_SUCCESS,

    /**
     * Invalid magic value.
     * Most probably the data being read is not GCF.
     */
    GCF_RESULT_INVALID_MAGIC,

    /**
     * Any data read error.
     */
    GCF_RESULT_READ_ERROR,

    /**
     * The provided callback is invalid.
     */
    GCF_RESULT_INVALID_CALLBACK,

    /**
     * No more resources available for reading.
     */
    GCF_RESULT_END,

    /**
     * Error during resource decompression.
     */
    GCF_RESULT_DECOMPRESS_ERROR,

    /**
     * Error during memory allocation.
     */
    GCF_RESULT_MEMORY_ERROR
} gcf_result;

/**
 * Memory read callback.
 *
 * @param destination Pointer to the destination buffer where the read content
 *  will be stored. If `NULL`, skip the next `n_bytes` bytes.
 * @param n_bytes The number of bytes to read. Reading 0 bytes must be allowed.
 * @param user_data The same user data passed to the context constructor.
 *
 * @return The number of bytes read/skipped or UINT32_MAX in case of error.
 */
typedef uint32_t (*gcf_read_clbk)(
    void * const destination,
    size_t const n_bytes,
    void * const user_data
);

/**
 * Data allocation callback.
 *
 * @param size The size of the memory chunk to allocate.
 * @param user_data The same user data passed to the context constructor.
 *
 * @return A pointer to a chunk of memory of at least `size` bytes or NULL on failure.
 */
typedef void* (*gcf_alloc_clbk)(
    size_t size,
    void *user_data
);

/**
 * Memory deallocation callback.
 *
 * @param ptr A pointer to the memory chunk to deallocate. Valid values for `ptr` are only
 *  those returned by calls to `gcf_alloc_clbk()`.
 * @param user_data The same user data passed to the context constructor.
 */
typedef void (*gcf_free_clbk)(
    void * const ptr,
    void *user_data
);

/**
 * Data decompress callback.
 *
 * @param ctx The read context.
 * @param descriptor The resource descriptor of the resource to decompress.
 * @param compressed_data_size The compressed data size.
 * @param compressed_data The compressed data.
 * @param decompressed_data_size The decompressed data size.
 * @param out_decompressed_data The output decompressed data.
 *
 * @return `GCF_RESULT_SUCCESS` on success, `GCF_RESULT_DECOMPRESS_ERROR` on error,
 *  including an unrecognized supercompression scheme.
 */
typedef gcf_result (*gcf_decompress_clbk)(
    gcf_read_ctx * const restrict ctx,
    gcf_resource_descriptor const * const restrict descriptor,
    uint32_t const compressed_data_size,
    void * const compressed_data,
    uint32_t const decompressed_data_size,
    void * const out_decompressed_data
);

/**
 * A GCF read context.
 */
struct gcf_read_ctx {
    /**
     * The data reading callback.
     */
    gcf_read_clbk read_clbk;

    /**
     * User data.
     */
    void * user_data;

    /**
     * The GCF context.
     */
    gcf_header header;

    /**
     * The last resource index.
     */
    uint16_t current_resource_index;

    /**
     * The memory allocation callback.
     */
    gcf_alloc_clbk alloc_clbk;

    /**
     * The memory deallocation callback.
     */
    gcf_free_clbk free_clbk;

    /**
     * The data decompression callback.
     * You can set this value to `gcf_decompress` if using one of the standard
     * supercompression schemes.
     */
    gcf_decompress_clbk decompress_clbk;
};

/**
 * Initialize a GCF reading context.
 *
 * @param ctx The context object to initialize.
 * @param read_clbk The data reading callback.
 * @param alloc_clbk The memory allocation callback.
 * @param free_clbk The memory deallocation callback.
 * @param gcf_decompress_clbk The data decompression callback.
 * @param user_data Pointer to some user-specific data for the callbacks.
 */
GCFATTR gcf_result GCFAPI gcf_read_context_initialize(
    gcf_read_ctx * const ctx,
    gcf_read_clbk const read_clbk,
    gcf_alloc_clbk const alloc_clbk,
    gcf_free_clbk const free_clbk,
    gcf_decompress_clbk const decompress_clbk,
    void * const user_data
);

/**
 * Destroy a GCF reading context.
 *
 * @param ctx The context object to initialize.
 */
GCFATTR void GCFAPI gcf_read_context_destroy(
    gcf_read_ctx const * const ctx
);

/**
 * Read the next resource descriptor.
 *
 * @param ctx The read context.
 * @param out_descriptor A pointer to the descriptor that will receive the data.
 *
 * @return The result of the operation.
 *
 * @note
 *  This function must only be called after:
 *  - `gcf_read_context_initialize()`
 *  - `gcf_read_resource_content()`
 *
 *  Any other use will result in undefined behaviour.
 */
GCFATTR gcf_result GCFAPI gcf_read_resource_descriptor(
    gcf_read_ctx * const restrict ctx,
    gcf_resource_descriptor * const restrict out_descriptor
);

/**
 * Read or skip the resource content for the current resource.
 *
 * @param ctx The read context.
 * @param descriptor The resource descriptor.
 * @param out_content Pointer to a buffer of at least `descriptor->size`
 *  bytes where the resource content will be stored. Set to `NULL` to
 *  skip the resource.
 *
 * @return The result.
 *
 * @note This function can only be called once per resource.
 */
GCFATTR gcf_result GCFAPI gcf_read_resource_content(
    gcf_read_ctx * const restrict ctx,
    gcf_resource_descriptor const * const restrict descriptor,
    void * const restrict out_content
);

/**
 * Decompress data.
 *
 * @param ctx The read context.
 * @param descriptor The resource descriptor.
 * @param compressed_data_size The compressed data size.
 * @param compressed_data The compressed data.
 * @param decompressed_data_size The decompressed data size.
 * @param out_decompressed_data The output decompressed data.
 *
 * @return `GCF_RESULT_SUCCESS` on success, `GCF_RESULT_DECOMPRESS_ERROR` on error.
 *
 * @note Calling this function for user-defined super-compression schemes will result in
 *  an error.
 */
GCFATTR gcf_result GCFAPI gcf_decompress(
    gcf_read_ctx * const restrict ctx,
    gcf_resource_descriptor const * const restrict descriptor,
    uint32_t const compressed_data_size,
    void * const compressed_data,
    uint32_t const decompressed_data_size,
    void * const out_decompressed_data
);

#endif /* !GCF_CONTAINER_H */
