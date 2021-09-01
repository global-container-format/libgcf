include_guard()
include(test-defs.cmake)

if(GCF_ENABLE_TESTS)
    add_custom_command(
        OUTPUT
            ${GCF_TEST_RESOURCE_IMAGE_MIPMAPS}
            ${GCF_TEST_RESOURCE_IMAGE_MIPMAPS_UNPADDED}
            ${GCF_TEST_RESOURCE_IMAGE_COMPRESSED_ZLIB}
            ${GCF_TEST_RESOURCE_IMAGE_UNCOMPRESSED}
            ${GCF_TEST_RESOURCE_IMAGE_COMPRESSED_TEST}
        COMMAND ${Python_EXECUTABLE} -mgcfconv generate mandelbrot --width 256 --height 256 --mip-levels 5 ${GCF_TEST_RESOURCE_IMAGE_MIPMAPS}
        COMMAND ${Python_EXECUTABLE} -mgcfconv generate mandelbrot --width 256 --height 256 --mip-levels 5 --no-padding ${GCF_TEST_RESOURCE_IMAGE_MIPMAPS_UNPADDED}
        COMMAND ${Python_EXECUTABLE} -mgcfconv generate mandelbrot --width 256 --height 100 --supercompression ZLib ${GCF_TEST_RESOURCE_IMAGE_COMPRESSED_ZLIB}
        COMMAND ${Python_EXECUTABLE} -mgcfconv generate mandelbrot --width 256 --height 100 ${GCF_TEST_RESOURCE_IMAGE_UNCOMPRESSED}
        COMMAND ${Python_EXECUTABLE} -mgcfconv generate mandelbrot --width 256 --height 100 --supercompression Test ${GCF_TEST_RESOURCE_IMAGE_COMPRESSED_TEST}
        COMMENT "Generating test resources"
    )

    add_custom_target(
        generate-test-resources
        DEPENDS
            ${GCF_TEST_RESOURCE_IMAGE_MIPMAPS}
            ${GCF_TEST_RESOURCE_IMAGE_MIPMAPS_UNPADDED}
            ${GCF_TEST_RESOURCE_IMAGE_COMPRESSED_ZLIB}
            ${GCF_TEST_RESOURCE_IMAGE_UNCOMPRESSED}
            ${GCF_TEST_RESOURCE_IMAGE_COMPRESSED_TEST}
    )

    set_target_properties(
        generate-test-resources
        PROPERTIES
            ADDITIONAL_CLEAN_FILES ${GCF_TEST_RESOURCE_FOLDER}
    )
endif()
