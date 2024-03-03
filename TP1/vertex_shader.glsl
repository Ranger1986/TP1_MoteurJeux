#version 330 core

// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec3 vertices_position_modelspace;

//TODO create uniform transformations matrices Model View Projection
// Values that stay constant for the whole mesh.
uniform mat4 MVP;

out vec2 uv;
out float height;

uniform sampler2D heightmap;

void main(){
        uv= vec2(vertices_position_modelspace.x,vertices_position_modelspace.z);
        vec3 pos=vec3(vertices_position_modelspace.x, texture(heightmap, uv).w, vertices_position_modelspace.z);
        gl_Position = MVP * vec4(vertices_position_modelspace,1);
        height = vertices_position_modelspace.y;
}

