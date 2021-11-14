include_guard()

function(add_module_tests module_name tests_var coverage_files_var)
    foreach(test_src IN LISTS ARGN)
        cmake_path(GET test_src STEM test_src_name)

        set(test_name ${module_name}-${test_src_name})
        set(test_coverage_file ${COVERAGE_DIR}/${test_name}.profraw)

        add_executable(${test_name} ${test_src})
        target_link_libraries(${test_name} PRIVATE gcf gcf_test_utils)
        target_compile_definitions(${test_name} PRIVATE _CRT_SECURE_NO_WARNINGS)

        add_dependencies(${test_name} generate-test-resources)

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

# Ensure a python module is available
function(check_python_module module_name)
    set(var_name Python_module_${module_name}_FOUND)

    if(${ARGC} EQUAL 2)
        if(${ARGV1} STREQUAL REQUIRED)
            set(is_required TRUE)
        else()
            message(FATAL_ERROR "Invalid argument. Must be REQUIRED or not specified.")
        endif()
    endif()

    if(DEFINED CACHE{${var_name}})
        set(module_is_available $CACHE{${var_name}})
    else()
        execute_process(
            COMMAND ${_Python_EXECUTABLE} -c "import ${module_name}"
            RESULTS_VARIABLE lookup_command_result
            OUTPUT_QUIET
            ERROR_QUIET
        )

        if(NOT ${lookup_command_result} EQUAL 0)
            set(module_is_available FALSE)
        else()
            set(module_is_available TRUE)
        endif()

        set(${var_name} ${module_is_available} CACHE BOOL "Module ${module_name} is available.")
    endif()

    if(module_is_available)
        set(${var_name} ${module_name} PARENT_SCOPE)
    else()
        if(NOT is_required)
            set(${var_name} NOTFOUND PARENT_SCOPE)
        else()
            message(FATAL_ERROR "Required Python module \"${module_name}\" not found.")
        endif()
    endif()
endfunction()
