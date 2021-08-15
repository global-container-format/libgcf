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
#ifndef GCF_COLOR_MAP_H
#define GCF_COLOR_MAP_H

#include <stdint.h>
#include <gcf/container.h>

/**
 * Resource descriptor flags for color map resource type.
 */
typedef enum gcf_color_map_flag_bits {
    GCF_COLOR_MAP_FLAG_IMAGE_BIT_IMAGE_1D = 0x0001,
    GCF_COLOR_MAP_FLAG_IMAGE_BIT_IMAGE_2D = 0x0003,
    GCF_COLOR_MAP_FLAG_IMAGE_BIT_IMAGE_3D = 0x0007
} gcf_color_map_flag_bits;

/**
 * Descriptor for a single mip level.
 */
typedef struct gcf_color_map_mip_level_descriptor {
    /**
     * Size of the compressed data, in bytes.
     */
    uint32_t compressed_size;

    /**
     * Expected size of the uncompressed data, in bytes.
     */
    uint32_t uncompressed_size;

    /**
     * Row stride.
     */
    uint32_t row_stride;

    /**
     * Depth stride for 3D images.
     */
    uint32_t depth_stride;

    /**
     * Layer stride for array images.
     */
    uint32_t layer_stride;

    /**
     * Reserved.
     */
    uint32_t rsvd;

    /**
     * Reserved.
     */
    uint64_t rsvd2;
} gcf_color_map_mip_level_descriptor;

/**
 * Read a color map mip level descriptor.
 *
 * @param ctx The context.
 * @param descriptor The color map resource descriptor.
 * @param out_level_descriptor The output mip map level descriptor.
 *
 * @return `GCF_RESULT_SUCCESS` on success, `GCF_RESULT_READ_ERROR` on error.
 */
GCFATTR gcf_result GCFAPI gcf_read_color_map_mip_level_descriptor(
    gcf_read_ctx * const restrict ctx,
    gcf_resource_descriptor const * const restrict descriptor,
    gcf_color_map_mip_level_descriptor * const out_level_descriptor
);

/**
 * Read a color map mip level.
 *
 * @param ctx The context.
 * @param res_descriptor The resource descriptor.
 * @param mip_descriptor The mip map level descriptor.
 * @param out_data The output mip map level data buffer. This must be
 *  at least `gcf_color_map_mip_level_descriptor::uncompressed_size` bytes long and
 *  will contain the uncompressed mip map level data on success.
 *
 * @return `GCF_RESULT_SUCCESS` on success, `GCF_RESULT_READ_ERROR` if an error occurred during
 *  a read, `GCF_RESULT_MEMORY_ERROR` if an error occurred during memory allocation or
 *  `GCF_RESULT_DECOMPRESS_ERROR` if an error occurred during resource decompression.
 */
GCFATTR gcf_result GCFAPI gcf_read_color_map_mip_level(
    gcf_read_ctx * const restrict ctx,
    gcf_resource_descriptor const * const restrict res_descriptor,
    gcf_color_map_mip_level_descriptor const * const restrict mip_descriptor,
    void * const out_data
);

#endif /* !GCF_COLOR_MAP_H */
