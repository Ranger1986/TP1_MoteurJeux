#version 330 core

// Ouput data
//out vec4 FragColor;
out vec3 color;

in float height;
in vec2 o_uv0;
in vec3 DAcolor;

void main(){
    color = DAcolor;
}
