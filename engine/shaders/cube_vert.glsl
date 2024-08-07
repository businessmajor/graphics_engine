#version 330 core
layout(location=0) in vec3 position;

out vec3 v_texCoord;

uniform mat4 view;
uniform mat4 projection;

void main(){
    v_texCoord = position;

    gl_Position = projection * view * vec4(position, 1.0);
    // gl_Position = vec4(position, 1.0);
}
