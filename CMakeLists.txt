cmake_minimum_required(VERSION 3.14)
project(MyGLFWProject)

# Enable C++17 standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add the deps and src directories
add_subdirectory(deps)
add_subdirectory(src)