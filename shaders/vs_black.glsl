#version 330 core

layout (location = 0) in vec3 pos;
  
uniform mat4 MVP;
uniform vec3 zpos;
uniform vec3 size;

out vec3 colpos;

void main()
{
    gl_Position = MVP * vec4(pos.x, pos.y, pos.z, 1.0f);
    colpos = vec3(0.0, 0.0, 0.0);
} 