cmake_minimum_required(VERSION 3.17)
project(learnopengl)
set(CMAKE_CXX_STANDARD 17)

add_subdirectory(src)
add_subdirectory(example/light)
add_subdirectory(example/model-loading)

# glfw
add_subdirectory(thirdparty/glfw-3.3.3)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

# glad
set(GLAD_DIR "thirdparty/glad")
add_library(glad "${GLAD_DIR}/src/glad.c")
target_include_directories(glad PUBLIC "${GLAD_DIR}/include")

# stb_image
target_include_directories(light PUBLIC "thirdparty/stb")
target_include_directories(model-loading PUBLIC "thirdparty/stb")

# glm
add_subdirectory("thirdparty/glm")

# assimp
add_subdirectory(thirdparty/assimp-5.0.1)
