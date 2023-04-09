import os.path as path
import re
from conan import ConanFile
from conan.tools.cmake import CMake, CMakeDeps, CMakeToolchain, cmake_layout
from conan.tools.files import load


class LibGcfRecipe(ConanFile):
    name = "libgcf"
    description = "Reference C implementation for a GCF file reader."
    settings = "os", "compiler", "build_type", "arch"

    requires = (
        "zlib/1.2.13"
    )

    tool_requires = (
        "ninja/[>=1.11.1]",
        "cmake/[>=3.22.6]"
    )

    def set_version(self):
        cmake_version_file = load(self, path.join(self.recipe_folder, "version.cmake"))
        major_version_match = re.search(r"set\s*\(\s*GCF_VERSION_MAJOR\s*(\d+)\s*\)", cmake_version_file)
        minor_version_match = re.search(r"set\s*\(\s*GCF_VERSION_MINOR\s*(\d+)\s*\)", cmake_version_file)
        patch_version_match = re.search(r"set\s*\(\s*GCF_VERSION_PATCH\s*(\d+)\s*\)", cmake_version_file)

        if (
            not major_version_match
            or not minor_version_match
            or not patch_version_match
        ):
            raise RuntimeError("Unable to determine libgcf version.")

        major_version = major_version_match.group(1)
        minor_version = minor_version_match.group(1)
        patch_version = patch_version_match.group(1)

        self.version = f"{major_version}.{minor_version}.{patch_version}"

    def generate(self):
        tc = CMakeToolchain(self)
        tc.generate()

        deps = CMakeDeps(self)
        deps.generate()

    def layout(self):
        cmake_layout(self)

    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()
        cmake.test()
        # cmake.install()
