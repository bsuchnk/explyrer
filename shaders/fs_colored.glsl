#version 330 core

out vec4 FragColor;

in vec3 colpos;
  
void main()
{
    FragColor = vec4(colpos, 1.0);
}