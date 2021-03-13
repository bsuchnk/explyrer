#version 330 core

layout (location = 0) in vec3 pos;
  
uniform mat4 MVP;
uniform vec3 zpos;
uniform vec3 size;

out vec3 colpos;

void main()
{
    gl_Position = MVP * vec4(pos.x, pos.y, pos.z, 1.0f);

    float z = (pos.z - zpos.z) / size.z * 4;

    float r = max(min(-2+z, 1), 0);
    float g = max(2-abs(2-z), 0);
    float b = max(min(2-z, 1), 0);

    colpos = vec3(r, g, b);
} 