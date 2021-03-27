add_executable(model-loading main.cc)
target_include_directories(
        model-loading
        PUBLIC
        ${PROJECT_SOURCE_DIR}/src
)
target_link_libraries(
        model-loading
        PRIVATE
        base
        glfw
        glm
        glad
        assimp
)
