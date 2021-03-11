cmake_minimum_required(VERSION 3.17)
project(learnopengl)
set(CMAKE_CXX_STANDARD 20)

# find_package(OpenGL REQUIRED)
# include_directories(${OPENGL_INCLUDE_DIRS})

# add_library(
#     glad
#     thirdparty/glad/include/glad/glad.h
#     thirdparty/glad/src/glad.c
# )
# target_include_directories(glad PUBLIC thirdparty/glad/include)

add_executable(
    learnopengl
    src/main.cc
)

# glfw
add_subdirectory(thirdparty/glfw-3.3.3)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
include_directories(${GLFW_INCLUDE_DIR})

# glad
set(GLAD_DIR "thirdparty/glad")
add_library(glad "${GLAD_DIR}/src/glad.c")
target_include_directories(glad PRIVATE "${GLAD_DIR}/include")
target_include_directories(learnopengl PRIVATE "${GLAD_DIR}/include")

# target_link_libraries(learnopengl ${OPENGL_LIBRARIES} glfw)
target_link_libraries(learnopengl glfw glad)
