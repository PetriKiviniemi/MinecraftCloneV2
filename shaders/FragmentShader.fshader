#version 330 core
out vec4 FragColor;
in vec3 customColor;
in vec2 textureCoordinates; 

uniform sampler2D ourTexture;

void main()
{
    FragColor = texture(ourTexture, textureCoordinates);
} 
