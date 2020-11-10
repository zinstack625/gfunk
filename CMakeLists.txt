cmake_minimum_required(VERSION 3.18)

project(gfunk)
set(CMAKE_CXX_STANDARD 17)

find_package(PkgConfig REQUIRED)
pkg_check_modules(GTKMM gtkmm-3.0)
find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)

add_subdirectory(sources)
