add_executable(raytracer main.cpp vector.cpp)

#target_link_libraries(raytracer PRIVATE -static)
set_property(TARGET raytracer PROPERTY CXX_STANDARD 20)
