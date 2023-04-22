include_guard()
include(${CMAKE_CURRENT_LIST_DIR}/test-defs.cmake)

if(GCF_ENABLE_TESTS)
    add_custom_command(
        OUTPUT
            ${GCF_TEST_RESOURCE_FOLDER}/image-mipmaps-256x256-level0.bin
            ${GCF_TEST_RESOURCE_FOLDER}/image-mipmaps-256x256-level1.bin
            ${GCF_TEST_RESOURCE_FOLDER}/image-mipmaps-256x256-level2.bin
            ${GCF_TEST_RESOURCE_FOLDER}/image-mipmaps-256x256-level3.bin
            ${GCF_TEST_RESOURCE_FOLDER}/image-mipmaps-256x256-level4.bin
            ${GCF_TEST_RESOURCE_FOLDER}/image-no-mipmaps-256x100-level0.bin
        DEPENDS
            ${GCF_TEST_RESOURCE_IMAGE_MIPMAPS_DEF}
            ${GCF_TEST_RESOURCE_IMAGE_MIPMAPS_UNPADDED_DEF}
            ${GCF_TEST_RESOURCE_IMAGE_COMPRESSED_ZLIB_DEF}
            ${GCF_TEST_RESOURCE_IMAGE_UNCOMPRESSED_DEF}
            ${GCF_TEST_RESOURCE_IMAGE_COMPRESSED_TEST_DEF}
        COMMAND ${Python_EXECUTABLE} ${CMAKE_CURRENT_LIST_DIR}/generate-assets.py "${CMAKE_CURRENT_BINARY_DIR}"
        COMMENT "Generating test resource image data files"
    )

    add_custom_command(
        OUTPUT
            ${GCF_TEST_RESOURCE_IMAGE_MIPMAPS}
            ${GCF_TEST_RESOURCE_IMAGE_MIPMAPS_UNPADDED}
            ${GCF_TEST_RESOURCE_IMAGE_COMPRESSED_ZLIB}
            ${GCF_TEST_RESOURCE_IMAGE_UNCOMPRESSED}
            ${GCF_TEST_RESOURCE_IMAGE_COMPRESSED_TEST}
        DEPENDS
            ${GCF_TEST_RESOURCE_FOLDER}/image-mipmaps-256x256-level0.bin
            ${GCF_TEST_RESOURCE_FOLDER}/image-mipmaps-256x256-level1.bin
            ${GCF_TEST_RESOURCE_FOLDER}/image-mipmaps-256x256-level2.bin
            ${GCF_TEST_RESOURCE_FOLDER}/image-mipmaps-256x256-level3.bin
            ${GCF_TEST_RESOURCE_FOLDER}/image-mipmaps-256x256-level4.bin
            ${GCF_TEST_RESOURCE_FOLDER}/image-no-mipmaps-256x100-level0.bin
        COMMAND ${Python_EXECUTABLE} -mgcfpack create -i "${GCF_TEST_RESOURCE_IMAGE_MIPMAPS_DEF}" -o "${GCF_TEST_RESOURCE_IMAGE_MIPMAPS}"
        COMMAND ${Python_EXECUTABLE} -mgcfpack create -i "${GCF_TEST_RESOURCE_IMAGE_MIPMAPS_UNPADDED_DEF}" -o "${GCF_TEST_RESOURCE_IMAGE_MIPMAPS_UNPADDED}"
        COMMAND ${Python_EXECUTABLE} -mgcfpack create -i "${GCF_TEST_RESOURCE_IMAGE_COMPRESSED_ZLIB_DEF}" -o "${GCF_TEST_RESOURCE_IMAGE_COMPRESSED_ZLIB}"
        COMMAND ${Python_EXECUTABLE} -mgcfpack create -i "${GCF_TEST_RESOURCE_IMAGE_UNCOMPRESSED_DEF}" -o "${GCF_TEST_RESOURCE_IMAGE_UNCOMPRESSED}"
        COMMAND ${Python_EXECUTABLE} -mgcfpack create -i "${GCF_TEST_RESOURCE_IMAGE_COMPRESSED_TEST_DEF}" -o "${GCF_TEST_RESOURCE_IMAGE_COMPRESSED_TEST}"
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
