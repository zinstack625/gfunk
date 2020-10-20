cmake_minimum_required(VERSION 3.18)

project(gfunk)
set(CMAKE_CXX_STANDARD 17)

find_package(PkgConfig)
pkg_check_modules(GTKMM gtkmm-3.0)

add_subdirectory(sources)
