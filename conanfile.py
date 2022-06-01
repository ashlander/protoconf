from conans import ConanFile, CMake, tools
import os


class ProtoconfConan(ConanFile):
    name = "protoconf"
    version = "0.1.0"
    license = "BSD 2-Clause License"
    author = "Andrii Zhuk <andrewzhuk@gmail.com>"
    url = "<Package recipe repository url here, for issues about the package>"
    description = "Protobuf configuration"
    topics = ("bsw", "c++", "protobuf", "configuration")
    settings = "os", "compiler", "build_type", "arch"
    options = {"codecov": [True, False]
            , "clangcheck": [True, False]
            , "asancheck": [True, False]
            , "tsancheck": [True, False]
            , "usancheck": [True, False]
            , "tags": [True,False]}
    default_options = "codecov=False" , "clangcheck=False" , "asancheck=False" , "tsancheck=False" , "usancheck=False", "tags=False"
    generators = "cmake"
    build_requires = "boost/1.79.0", "gtest/1.11.0", "protobuf/3.21.1"

    # TODO
    #def source(self):

    def build(self):
        cmake = CMake(self)

        if self.options.codecov == "True":
            cmake.definitions["WITH_CODECOV"] = "ON"

        if self.options.asancheck == "True":
            cmake.definitions["WITH_ASAN"] = "ON"

        if self.options.tsancheck == "True":
            cmake.definitions["WITH_TSAN"] = "ON"

        if self.options.usancheck == "True":
            cmake.definitions["WITH_USAN"] = "ON"

        if self.options.tags == "True":
            cmake.definitions["WITH_TAGS"] = "ON"

        cmake.configure(source_folder="src")

        cmake.build()

    def package(self):
        self.copy("*.h", dst="include", src="src/includex")

    def package_info(self):
        self.cpp_info.libs = ["protoconf"]

