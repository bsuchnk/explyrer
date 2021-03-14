#version 330 core

out vec4 FragColor;

in vec3 colpos;
  
void main()
{
    FragColor = vec4(colpos, 1.0);
    //FragColor = vec4(1.0, 1.0, 1.0, 1.0);
    //FragColor = vec4(0.0, 0.0, 0.0, 1.0);
}