cmake_minimum_required(VERSION 3.15)
project(PackageTest C)

find_package(libgcf CONFIG REQUIRED)

add_executable(example src/example.c)
target_link_libraries(example libgcf::libgcf)
