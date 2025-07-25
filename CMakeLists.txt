cmake_minimum_required(VERSION 3.14)
project(fileops)

set(CMAKE_CXX_STANDARD 17)

# pybind11 path
add_subdirectory(backend/pybind11)

# Find Python and include directories
find_package(Python3 REQUIRED COMPONENTS Interpreter Development)

include_directories(${Python3_INCLUDE_DIRS})
link_libraries(${Python3_LIBRARIES})

# Create Python module using pybind11
pybind11_add_module(fileops
    backend/bindings.cpp
    backend/file_operations.cpp
    backend/settings.cpp
)
