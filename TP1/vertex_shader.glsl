#version 330 core

// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec3 vertices_position_modelspace;
layout(location = 1) in vec2 uv;

//TODO create uniform transformations matrices Model View Projection
// Values that stay constant for the whole mesh.
uniform mat4 MVP;

out float height;

uniform sampler2D heightmap;

out vec2 o_uv0;

void main(){
        o_uv0=uv;
        //vec3 pos=vec3(vertices_position_modelspace.x, texture(heightmap, uv).r*10-5, vertices_position_modelspace.z);
        vec3 pos=vec3(vertices_position_modelspace.x, vertices_position_modelspace.y, vertices_position_modelspace.z);
        gl_Position = MVP * vec4(pos,1);
        height = pos.y;
}

