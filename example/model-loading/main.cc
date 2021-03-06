#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>

#include <base/camera.h>
#include <base/model.h>
#include <base/shader.h>

// settings
const int WIN_WIDTH = 800;
const int WIN_HEIGHT = 600;

// camera
Camera camera(glm::vec3(.0f, 10.0f, 30.0f));
bool firstMouse = true;
double lastX = .0;
double lastY = .0;

// timing
float deltaTime = .0f; // time between current frame and last frame
float lastFrame = .0f;

void framebufferSizeCallback(GLFWwindow *window, int width, int height);
void mouseCallback(GLFWwindow *window, double x, double y);
void scrollCallback(GLFWwindow *window, double xOffset, double yOffset);
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
  glfwSetCursorPosCallback(window, mouseCallback);
  glfwSetScrollCallback(window, scrollCallback);
  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

  // load all OpenGL function pointers
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::cerr << "failed to initialize GLAD" << std::endl;
    return EXIT_FAILURE;
  }

  int attrCount;
  glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &attrCount);
  std::cout << "GL_MAX_VERTEX_ATTRIBS: " << attrCount << std::endl;
  std::cout << "size of GLfloat: " << sizeof(GLfloat) << " byte(s)" << std::endl;

  // configure global opengl state
  glad_glEnable(GL_DEPTH_TEST);

  // build and compile shaders
  Shader shader("shaders/model_loading.vert", "shaders/model_loading.frag");

  // load model
  Model ourModel("nanosuit/nanosuit.obj");

  while (!glfwWindowShouldClose(window)) {
    double currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;

    processInput(window);

    // render
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    shader.use();

    // view/projection transformations
    glm::mat4 projection =
        glm::perspective(glm::radians(camera.zoom), (float)WIN_WIDTH / (float)WIN_HEIGHT, 0.1f, 100.0f);
    glm::mat4 view = camera.getViewMatrix();
    shader.setMat4("projection", projection);
    shader.setMat4("view", view);

    // render the loaded model
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f)); // translate it down so it's at the center of the scene
    model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));     // it's a bit too big for our scene, so scale it down
    shader.setMat4("model", model);

    ourModel.draw(shader);

    /* Swap front and back buffers */
    glfwSwapBuffers(window);

    /* Poll for and process events */
    glfwPollEvents();
  }

  glfwTerminate();

  return EXIT_SUCCESS;
}

void mouseCallback(GLFWwindow *window, double x, double y) {
  if (firstMouse) {
    lastX = x;
    lastY = y;
    firstMouse = false;
  }

  auto xOffset = lastX - x;
  auto yOffset = y - lastY;
  lastX = x;
  lastY = y;

  camera.processMouseMovement(xOffset, yOffset);
}

void scrollCallback(GLFWwindow *window, double xOffset, double yOffset) { camera.processMouseScroll(yOffset); }

// glfw: whenever the window size changed, this callback function executes
void framebufferSizeCallback(GLFWwindow *window, int width, int height) {
  // make sure the viewport matches the new window dimensions; note that width and height will be significantly larger
  // than specified on retina displays
  glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(window, true);

  if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
    camera.processKeyboard(FORWARD, deltaTime);
  } else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
    camera.processKeyboard(BACKWARD, deltaTime);
  } else if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
    camera.processKeyboard(LEFT, deltaTime);
  } else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
    camera.processKeyboard(RIGHT, deltaTime);
  }
}
