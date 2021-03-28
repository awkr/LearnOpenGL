add_executable(stencil-testing main.cc)
target_include_directories(
        stencil-testing
        PUBLIC
        ${PROJECT_SOURCE_DIR}/src
)
target_link_libraries(
        stencil-testing
        PRIVATE
        base
        glfw
        glm
        glad
        assimp
)
