# Emil Gedda 2020
cmake_minimum_required(VERSION 3.12)
project(raytracer CXX)

include("cmake/CMakeLists.txt") # Don't create a new scope
add_subdirectory("src")
