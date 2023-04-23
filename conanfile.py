import os.path as path
import re
from conan import ConanFile
from conan.tools.cmake import CMake, CMakeDeps, CMakeToolchain, cmake_layout
from conan.tools.files import load


class LibGcfRecipe(ConanFile):
    name = "libgcf"
    description = "Reference C implementation for a GCF file reader."
    url = "https://github.com/global-container-format/libgcf"
    topics = "media", "container", "format", "gcf", "texture", "game", "engine"
    settings = "os", "build_type", "arch"

    requires = "zlib/1.2.13",

    tool_requires = (
        "ninja/[>=1.11.1]",
        "cmake/[>=3.22.6]"
    )

    exports = "version.cmake", "LICENSE", "README.md"

    exports_sources = (
        "CMakeLists.txt",
        "version.cmake",
        "scripts/*",
        "src/*",
        "test/*",
        "assets/*",
        "Pipfile",
        "Pipfile.lock"
    )

    options = {"shared": [True, False], "fPIC": [True, False]}
    default_options = {"shared": False, "fPIC": True}

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
        tc.cache_variables["BUILD_SHARED_LIBS"] = self.options.shared
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

    def package(self):
        cmake = CMake(self)
        cmake.install()

    def package_info(self):
        self.cpp_info.libs = ["gcf"]

    def config_options(self):
        if self.settings.os == "Windows":
            self.options.rm_safe("fPIC")

    def configure(self):
        if not self.options.shared:
            self.options.rm_safe("fPIC")
