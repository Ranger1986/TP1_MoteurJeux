#version 330 core

// Ouput data
out vec3 color;

// in vec2 uv;
in float height;

void main(){
    if (height <= 0){
        color =vec3(0.1, 0.1,0.1);
    } else {
        color=vec3(0.9,0.9,0.9);
    }
    // if(height<0.5f){
    //     color =vec3(0.1, 0.1,0.1);
    // }
    // else{
    //     color =vec3(0.9, 0.9,0.9);
    // }
}
