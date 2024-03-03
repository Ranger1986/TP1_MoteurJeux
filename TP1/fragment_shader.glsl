#version 330 core

// Ouput data
out vec4 FragColor;
out vec3 color;

in vec2 uv;
in float height;

uniform sampler2D grass;
uniform sampler2D rock;
uniform sampler2D snow;

void main(){
    if (height > 0.3){
        FragColor=texture(snow,uv);
    } else if (height > 0){
        FragColor=texture(rock,uv);
    } else {
        FragColor=texture(grass,uv);
    }
}
