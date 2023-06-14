import os

from conan import ConanFile
from conan.tools.cmake import CMake, CMakeToolchain, cmake_layout
from conan.tools.build import can_run
from conan.tools.files import copy


class AddpConan(ConanFile):
    name = "addp"
    version = "1.0.2"
    settings = "os", "compiler", "build_type", "arch"
    generators = "CMakeDeps"
    exports_sources = "CMakeLists.txt", "src/addp/*", "src/unittest/*"
    requires = "boost/1.80.0"

    def generate(self):
        tc = CMakeToolchain(self)
        tc.variables["ADDP_CLIENT_APP"] = False
        tc.generate()

    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()
        if can_run(self):
            cmake.test()

    def layout(self):
        cmake_layout(self)

    def package(self):
        copy(self, "*.h", os.path.join(self.source_folder, "src", "addp"), os.path.join(self.package_folder, "include", "addp"))
        copy(self, "*.a", self.build_folder, os.path.join(self.package_folder, "lib"), keep_path=False)

    def package_info(self):
        self.cpp_info.libs = ["addp"]
        if self.settings.os == "Windows":
            self.cpp_info.system_libs.append("ws2_32")

