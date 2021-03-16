#version 410 core
out vec4 color;

in vec3 ourColor;
in vec2 ourTexCoord;

uniform float mixValue;
uniform sampler2D texture0;
uniform sampler2D texture1;

void main() {
    // color = texture(texture0, ourTexCoord) * vec4(ourColor, 1.0);
    color = mix(texture(texture0, ourTexCoord), texture(texture1, ourTexCoord), mixValue);
}
