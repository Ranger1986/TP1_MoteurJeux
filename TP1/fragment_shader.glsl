#version 330 core

// Ouput data
out vec4 FragColor;
out vec3 color;

in float height;
in vec2 o_uv0;

uniform sampler2D heightmap;
uniform sampler2D grass;
uniform sampler2D rock;
uniform sampler2D snow;

void main(){
    if (height > 8){
        FragColor=texture(snow,o_uv0);
    } else if (height > 5){
        FragColor=texture(rock,o_uv0);
    } else {
        FragColor=texture(grass,o_uv0);
    }
}
