cmake_minimum_required(VERSION 3.10)

include(CMake/compiler.cmake)
include(CMake/qt.cmake)
include(CMake/conan.cmake)
include(CMake/source.cmake)
include(CMake/project.cmake)

compiler()
set_project()
qt()
conan()
source()
