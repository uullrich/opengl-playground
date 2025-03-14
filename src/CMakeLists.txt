# Add the executable
add_executable(MyGLFWApp main.cpp)

find_package(GLEW REQUIRED)

if(NOT GLEW_FOUND)
    message(WARNING "GLEW not found!")
else()
    message(STATUS "GLEW found: ${GLEW_INCLUDE_DIRS}")
endif()

# Link glfw to the executable
target_link_libraries(MyGLFWApp PRIVATE GLEW::GLEW glfw stb_image ${OpenGL_LIBRARY})

# Include glfw headers
target_include_directories(MyGLFWApp PRIVATE ${glfw_SOURCE_DIR}/include ${glm_SOURCE_DIR} ${glew_SOURCE_DIR}/include)

# Enable STB Image implementation
target_compile_definitions(MyGLFWApp PRIVATE STB_IMAGE_IMPLEMENTATION)

add_custom_command(TARGET MyGLFWApp POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/shaders $<TARGET_FILE_DIR:MyGLFWApp>/shaders)

add_custom_command(TARGET MyGLFWApp POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/textures $<TARGET_FILE_DIR:MyGLFWApp>/textures)