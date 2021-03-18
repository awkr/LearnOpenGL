#pragma once

#include <glm/glm.hpp>

enum Movement { FORWARD, BACKWARD, LEFT, RIGHT };

// default camera values
const float YAW = -90.0f; // initialized to -90.0 degrees since a yaw of 0.0 results in a direction vector pointing to
                          // the right so we initially rotate a bit to the left.
const float PITCH = 0.0f;
const float SPEED = 2.5f;
const float SENSITIVITY = 0.1f;
const float ZOOM = 45.0f;

// an abstract camera class that processes input and calculates the corresponding Euler Angles, Vectors and Matrices for
// use in OpenGL
class Camera {
public:
  float zoom;

  Camera(glm::vec3 pos = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW,
         float pitch = PITCH)
      : position(pos), worldUp(up), yaw(yaw), pitch(pitch), front(glm::vec3(0.0f, 0.0f, -1.0f)), movementSpeed(SPEED),
        mouseSensitivity(SENSITIVITY), zoom(ZOOM) {
    update();
  }

  // return the view matrix calculated using Euler Angles and the LookAt Matrix
  glm::mat4 getViewMatrix() { return glm::lookAt(position, position + front, up); }

  // process input received from any keyboard-like input system
  void processKeyboard(Movement direction, float deltaTime) {
    float velocity = movementSpeed * deltaTime;
    switch (direction) {
    case FORWARD:
      position += front * velocity;
      break;
    case BACKWARD:
      position -= front * velocity;
      break;
    case LEFT:
      position -= right * velocity;
      break;
    case RIGHT:
      position += right * velocity;
      break;
    }
  }

  // process input received from a mouse input system. expects the offset value in both the x and y direction.
  void processMouseMovement(float xOffset, float yOffset, GLboolean constrainPitch = true) {
    xOffset *= mouseSensitivity;
    yOffset *= mouseSensitivity;

    yaw += xOffset;
    pitch += yOffset;

    // make sure that when pitch is out of bounds, screen doesn't get flipped
    if (constrainPitch) {
      if (pitch > 89.0f)
        pitch = 89.0f;
      if (pitch < -89.0f)
        pitch = -89.0f;
    }

    update();
  }

  // process input received from a mouse scroll-wheel event. only requires input on the vertical wheel-axis.
  void processMouseScroll(float yOffset) {
    if (zoom -= (float)yOffset; zoom < 1.0f)
      zoom = 1.0f;
    if (zoom > 45.0f)
      zoom = 45.0f;
  }

private:
  // attributes
  glm::vec3 position;
  glm::vec3 front;
  glm::vec3 up;
  glm::vec3 right;
  glm::vec3 worldUp;
  // euler angles
  float yaw;
  float pitch;
  // options
  float movementSpeed;
  float mouseSensitivity;

  // calculate the front, right, up vectors from the camera's Euler Angles
  void update() {
    glm::vec3 dir;
    dir.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    dir.y = sin(glm::radians(pitch));
    dir.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    front = glm::normalize(dir);

    right = glm::normalize(glm::cross(front, worldUp));
    up = glm::normalize(glm::cross(right, front));
  }
};
