from conanfile import ConanFile
from conanfile import CMake

class StbImageConan(ConanFile):
    name = "stb_image"
    version = "2.26"
    license = "MIT"
    author = "nothings stb"
    url = "https://github.com/nothings/stb"
    description = "STB single-header image library"
    topics = ("conan", "image", "stb")
    homepage = "https://github.com/nothings/stb"
    no_copy_source = True

    def source(self):
        self.run("git clone --branch v%s https://github.com/nothings/stb.git" % self.version)

    def build(self):
        cmake = CMake(self)
        cmake.definitions["STB_IMAGE_IMPLEMENTATION"] = "ON"
        cmake.configure(source_folder="stb")
        cmake.build()

    def package(self):
        self.copy("stb_image.h", dst="include", src="stb")
        self.copy("stb_image_write.h", dst="include", src="stb")
        self.copy("*.lib", dst="lib", keep_path=False)
        self.copy("*.a", dst="lib", keep_path=False)

    def package_info(self):
        self.cpp_info.libs = ["stb_image"]
