cmake_minimum_required(VERSION 3.17)
project(learnopengl)
set(CMAKE_CXX_STANDARD 17)

add_executable(
    learnopengl
    src/main.cc
)

# glfw
add_subdirectory(thirdparty/glfw-3.3.3)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
target_include_directories(learnopengl PUBLIC ${GLFW_INCLUDE_DIR})

# glad
set(GLAD_DIR "thirdparty/glad")
add_library(glad "${GLAD_DIR}/src/glad.c")
target_include_directories(glad PUBLIC "${GLAD_DIR}/include")
target_include_directories(learnopengl PUBLIC "${GLAD_DIR}/include")

# stb_image
target_include_directories(learnopengl PUBLIC "thirdparty/stb")

# glm
add_subdirectory("thirdparty/glm")
target_link_libraries(learnopengl glm)

target_link_libraries(learnopengl glfw glad)
