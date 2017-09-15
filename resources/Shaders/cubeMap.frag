#version 450 

out vec4 FragColor;

in vec3 textureDir;
layout(location = 3) uniform samplerCube cubemap;

void main()
{             
    FragColor = texture(cubemap, textureDir);
}  