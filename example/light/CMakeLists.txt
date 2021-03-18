add_executable(light main.cc)
target_include_directories(
        light
        PUBLIC
        ${PROJECT_SOURCE_DIR}/src
)
target_link_libraries(
        light
        PRIVATE
        base
        glfw
        glm
        glad
)
