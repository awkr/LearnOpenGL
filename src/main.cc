#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/glad.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>

#include "shader.h"

const int WIN_WIDTH = 640;
const int WIN_HEIGHT = 480;

// how much we're seeing of either texture
float mixValue = 0.2f;

void framebufferSizeCallback(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow *window);

int main(int argc, char **argv) {
  if (!glfwInit()) {
    std::cerr << "failed to init GLFW" << std::endl;
    return EXIT_FAILURE;
  }

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

  GLFWwindow *window;
  // create a windowed mode window and its OpenGL context
  if (window = glfwCreateWindow(WIN_WIDTH, WIN_HEIGHT, "Hello CG", nullptr, nullptr); !window) {
    glfwTerminate();
    return EXIT_FAILURE;
  }

  glfwMakeContextCurrent(window); // make the window's context current
  glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::cerr << "failed to initialize GLAD" << std::endl;
    return EXIT_FAILURE;
  }

  int attrCount;
  glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &attrCount);
  std::cout << "GL_MAX_VERTEX_ATTRIBS: " << attrCount << std::endl;
  std::cout << "size of GLfloat: " << sizeof(GLfloat) << " byte(s)" << std::endl;

  // build and compile shader program
  Shader shader("shaders/shader.vert", "shaders/shader.frag");

  // set up vertex data

  float vertices[] = {
      // position   color                   tex
      0.5f,  0.5f,  0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, // 右上
      0.5f,  -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, // 右下
      -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // 左下
      -0.5f, 0.5f,  0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f  // 左上
  };
  unsigned int indices[] = {
      0, 1, 3, // first triangle
      1, 2, 3, // second triangle
  };

  unsigned int vao, vbo, ebo;
  glGenVertexArrays(1, &vao);
  glGenBuffers(1, &vbo);
  glGenBuffers(1, &ebo);

  // bind the vertex array object first, then bind and set vertex buffers, and
  // then configure vertex attributes
  glBindVertexArray(vao);

  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

  // position attribute
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GL_FLOAT), nullptr);
  glEnableVertexAttribArray(0);
  // color attribute
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GL_FLOAT), (void *)(3 * sizeof(GL_FLOAT)));
  glEnableVertexAttribArray(1);
  // texture coord attribute
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GL_FLOAT), (void *)(6 * sizeof(GL_FLOAT)));
  glEnableVertexAttribArray(2);

  // load and create a texture
  // texture 1
  GLuint texture0;
  glad_glGenTextures(1, &texture0);
  glad_glBindTexture(GL_TEXTURE_2D,
                     texture0); // all upcoming GL_TEXTURE_2D operations now have effected on this texture object
  // set the texture wrapping parameters
  glad_glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // default wrapping method
  glad_glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  // set texture filtering parameters
  glad_glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glad_glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  // load image, create texture and generate mipmaps
  int width, height, channels;
  unsigned char *data = stbi_load("textures/container.jpg", &width, &height, &channels, 0);
  if (!data) {
    std::cerr << "load texture error" << std::endl;
    return EXIT_FAILURE;
  }
  glad_glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
  glad_glGenerateMipmap(GL_TEXTURE_2D);
  stbi_image_free(data);

  // texture 2
  GLuint texture1;
  glad_glGenTextures(1, &texture1);
  glad_glBindTexture(GL_TEXTURE_2D, texture1);

  glad_glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glad_glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

  glad_glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glad_glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  stbi_set_flip_vertically_on_load(true);
  if (data = stbi_load("textures/awesomeface.png", &width, &height, &channels, 0); !data) {
    std::cerr << "load texture error" << std::endl;
    return EXIT_FAILURE;
  }
  glad_glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
  glad_glGenerateMipmap(GL_TEXTURE_2D);
  stbi_image_free(data);

  shader.use();
  shader.setInt("texture0", 0);
  shader.setInt("texture1", 1);

  // glm::mat4 trans(1.0f);
  // // 先绕Z轴逆时针旋转90°，再缩放0.5倍
  // trans = glm::rotate(trans, glm::radians(90.0f), glm::vec3(.0, .0, 1.0));
  // trans = glm::scale(trans, glm::vec3(.5, .5, .5));

  // shader.setMat4("transform", glm::value_ptr(trans));

  // create transformations

  glm::mat4 model = glm::mat4(1.0f);
  glm::mat4 view = glm::mat4(1.0f);
  glm::mat4 projection = glm::mat4(1.0f);
  model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));
  view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
  projection = glm::perspective(glm::radians(45.0f), (float)WIN_WIDTH / (float)WIN_HEIGHT, 0.1f, 100.0f);

  shader.set_mat4fv("model", model);
  shader.set_mat4fv("view", view);
  shader.set_mat4fv("projection", projection);

  while (!glfwWindowShouldClose(window)) {
    processInput(window);

    // render
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // bind textures on corresponding texture units
    glad_glActiveTexture(GL_TEXTURE0);
    glad_glBindTexture(GL_TEXTURE_2D, texture0);
    glad_glActiveTexture(GL_TEXTURE1);
    glad_glBindTexture(GL_TEXTURE_2D, texture1);

    // set the texture mix value in the shader
    shader.setFloat("mixValue", mixValue);

    // create transformations

    // glm::mat4 trans(1.0);
    // // trans = glm::translate(trans, glm::vec3(0.5f, -0.5f, 0.0f));
    // trans = glm::rotate(trans, (float)glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f));
    // shader.setMat4("transform", glm::value_ptr(trans));

    // shader.use();

    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    // glDrawArrays(GL_TRIANGLES, 0, 6);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    /* Swap front and back buffers */
    glfwSwapBuffers(window);

    /* Poll for and process events */
    glfwPollEvents();
  }

  glfwTerminate();

  return EXIT_SUCCESS;
}

// glfw: whenever the window size changed, this callback function executes
void framebufferSizeCallback(GLFWwindow *window, int width, int height) {
  // make sure the viewport matches the new window dimensions; note that width and height will be significantly larger
  // than specified on retina displays
  glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(window, true);

  if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
    if (mixValue += 0.004f; mixValue > 1.0f)
      mixValue = 1.0f;
  } else if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
    if (mixValue -= 0.004f; mixValue < .0f)
      mixValue = .0f;
  }
}
