include_guard()

function(add_module_tests module_name tests_var coverage_files_var)
    foreach(test_src IN LISTS ARGN)
        cmake_path(GET test_src STEM test_src_name)

        set(test_name ${module_name}-${test_src_name})
        set(test_coverage_file ${COVERAGE_DIR}/${test_name}.profraw)

        add_executable(${test_name} ${test_src})
        target_link_libraries(${test_name} PRIVATE gcf gcf_test_utils)
        target_compile_definitions(${test_name} PRIVATE _CRT_SECURE_NO_WARNINGS)

        set_target_properties(
            ${test_name}
            PROPERTIES
            RUNTIME_OUTPUT_DIRECTORY tests/${module_name}
            RUNTIME_OUTPUT_NAME ${test_src_name}
        )

        add_test(
            NAME ${test_name}
            COMMAND ${test_name}
            WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
        )

        list(APPEND test_name_list ${test_name})
        list(APPEND test_coverage_file_list ${test_coverage_file})

        if(GCF_ENABLE_COVERAGE)
            set_tests_properties(
                ${test_name}
                PROPERTIES
                    ENVIRONMENT "LLVM_PROFILE_FILE=${test_coverage_file}"
            )
            target_link_options(
                ${test_name}
                PRIVATE
                    --coverage
            )
        endif()
    endforeach()

    set(${tests_var} ${test_name_list} PARENT_SCOPE)
    set(${coverage_files_var} ${test_coverage_file_list} PARENT_SCOPE)
endfunction()
