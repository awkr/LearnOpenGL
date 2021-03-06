#version 410 core
layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 color;
layout (location = 2) in vec2 texCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 ourColor;
out vec2 ourTexCoord;

void main() {
    gl_Position = projection * view * model * vec4(pos, 1.0);
    ourColor = color;
    ourTexCoord = texCoord;
}
