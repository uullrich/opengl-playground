include(FetchContent)

# Fetch and configure GLM
FetchContent_Declare(glm
    GIT_REPOSITORY https://github.com/g-truc/glm.git
    GIT_TAG "1.0.1"
    GIT_SHALLOW ON
)
FetchContent_MakeAvailable(glm)

# Fetch and configure STB Image
FetchContent_Declare(stb
    GIT_REPOSITORY https://github.com/nothings/stb.git
    GIT_TAG master
    GIT_SHALLOW ON
)
FetchContent_MakeAvailable(stb)

add_library(stb_image INTERFACE)
target_include_directories(stb_image INTERFACE ${stb_SOURCE_DIR})
target_link_libraries(stb_image INTERFACE)

# Fetch glfw
FetchContent_Declare(glfw
    GIT_REPOSITORY "https://github.com/glfw/glfw"
    GIT_TAG "3.4"
    GIT_SHALLOW ON
)
FetchContent_MakeAvailable(glfw)