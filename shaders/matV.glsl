#version 410 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNorm;

uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProj;

out vec3 iPos;
out vec3 iNorm;

void main() {
    iPos = aPos;
    iNorm = aNorm;
    gl_Position = uProj * uView * uModel * vec4(aPos, 1.0);
}
