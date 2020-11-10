#version 330 core

layout (location = 0) in vec3 vcrd;
in float vcomp;
out vec4 vColor;

void main() {
  vColor = vec4(1.0/(1.0 - vcrd.z), 1.0/(1.0 - vcrd.z), 0.0, 1.0);
  gl_Position = vec4(vcrd.x, vcrd.y, vcrd.z/vcomp, 1.0);
}
