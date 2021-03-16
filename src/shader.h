#pragma once

#include <fstream>
#include <glad/glad.h>
#include <iostream>
#include <sstream>
#include <string>

class Shader {
private:
  GLuint id;
  // utility function for checking shader compilation/linking errors
  void checkError(unsigned int shader, std::string type);

public:
  // constructor generates the shader on the fly
  Shader(const GLchar *vertexPath, const GLchar *fragmentPath);
  ~Shader() { glad_glDeleteProgram(id); };
  GLuint get_id() const { return id; };
  void use() { glad_glUseProgram(id); }
  // utility uniform functions
  void setBool(const std::string &name, bool value) const {
    glad_glUniform1i(glad_glGetUniformLocation(id, name.c_str()), (int)value);
  }
  void setInt(const std::string &name, int value) const {
    glad_glUniform1i(glad_glGetUniformLocation(id, name.c_str()), value);
  }
  void setFloat(const std::string &name, float value) const {
    glad_glUniform1f(glad_glGetUniformLocation(id, name.c_str()), value);
  }
};

Shader::Shader(const GLchar *vertexPath, const GLchar *fragmentPath) {
  try {
    // 1 retrieve the vertex/fragment source code from file path

    std::ifstream vShaderFile;
    std::ifstream fShaderFile;

    // open files
    vShaderFile.open(vertexPath);
    fShaderFile.open(fragmentPath);

    // read file's buffer contents into streams
    std::stringstream vShaderStream, fShaderStream;
    vShaderStream << vShaderFile.rdbuf();
    fShaderStream << fShaderFile.rdbuf();

    // close file handlers
    vShaderFile.close();
    fShaderFile.close();

    // convert stream into string
    std::string vertexCode = vShaderStream.str();
    std::string fragmentCode = fShaderStream.str();

    // 2 compile shaders
    // vertex shader
    GLuint vertex = glad_glCreateShader(GL_VERTEX_SHADER);
    const char *vShaderCode = vertexCode.c_str();
    glad_glShaderSource(vertex, 1, &vShaderCode, nullptr);
    glad_glCompileShader(vertex);
    checkError(vertex, "VERTEX");
    // fragment shader
    GLuint fragment = glad_glCreateShader(GL_FRAGMENT_SHADER);
    const char *fShaderCode = fragmentCode.c_str();
    glad_glShaderSource(fragment, 1, &fShaderCode, nullptr);
    glad_glCompileShader(fragment);
    checkError(fragment, "FRAGMENT");
    // link shaders
    id = glad_glCreateProgram();
    glad_glAttachShader(id, vertex);
    glad_glAttachShader(id, fragment);
    glad_glLinkProgram(id);
    checkError(id, "PROGRAM");
    // delete the shaders as they're linked into our program now and no longer
    // necessary
    glad_glDeleteShader(vertex);
    glad_glDeleteShader(fragment);
  } catch (std::ifstream::failure &e) {
    std::cerr << "read shader file error: " << e.what() << std::endl;
  }
}

void Shader::checkError(unsigned int shader, std::string type) {
  GLint success;
  char infolog[1024];
  if (type != "PROGRAM") {
    if (glad_glGetShaderiv(shader, GL_COMPILE_STATUS, &success); !success) {
      glad_glGetShaderInfoLog(shader, 1024, nullptr, infolog);
      std::cerr << type << ": compile shader error: " << infolog << std::endl;
    }
  } else {
    if (glad_glGetProgramiv(shader, GL_LINK_STATUS, &success); !success) {
      glad_glGetProgramInfoLog(shader, 1024, nullptr, infolog);
      std::cerr << "link program error: " << infolog << std::endl;
    }
  }
}
