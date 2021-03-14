#version 330 core

layout (location = 0) in vec3 pos;
  
uniform mat4 MVP;
uniform vec3 zpos;
uniform vec3 size;

out vec3 colpos;

void main()
{
    gl_Position = MVP * vec4(pos.x, pos.y, pos.z, 1.0f);

    float z1 = (pos.x - zpos.x) / size.z;
    float z2 = (pos.y - zpos.y) / size.z;
    float z3 = (pos.z - zpos.z) / size.z;

	float r = max(min(z1, 1), 0);
    float g = max(min(1-z2, 1), 0);
    float b = max(min(z3, 1), 0);

    colpos = vec3(r, g, b);
} 