from conans import ConanFile, CMake, tools


class AddpConan(ConanFile):
    name = "addp"
    version = "1.0.0"
    license = "<Put the package license here>"
    author = "<Put your name here> <And your email here>"
    url = "<Package recipe repository url here, for issues about the package>"
    description = "<Description of Addp here>"
    topics = ("<Put some tag here>", "<here>", "<and here>")
    settings = "os", "compiler", "build_type", "arch"
    options = {"shared": [True, False]}
    default_options = {"shared": False}
    generators = "cmake"
    exports_sources = "CMakeLists.txt", "src/addp/*", "src/unittest/*"
    requires = "boost/1.73.0"

    def build(self):
        cmake = CMake(self)
        cmake.definitions["ADDP_CLIENT_APP"] = "OFF"
        cmake.definitions["ADDP_SERVER"] = "OFF"
        cmake.configure(source_folder=".")
        cmake.build()
        if not tools.cross_building(self.settings):
            cmake.test()

    def package(self):
        self.copy("*.h", dst="include/addp", src="src/addp")
        self.copy("*.lib", dst="lib", keep_path=False)
        self.copy("*.dll", dst="bin", keep_path=False)
        self.copy("*.dylib*", dst="lib", keep_path=False)
        self.copy("*.so", dst="lib", keep_path=False)
        self.copy("*.a", dst="lib", keep_path=False)

    def package_info(self):
        self.cpp_info.libs = ["addp"]
        if self.settings.os == "Windows":
            if not self.options.shared:
                self.cpp_info.libs.append("ws2_32")

