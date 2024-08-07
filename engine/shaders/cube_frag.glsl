#version 330 core

in vec3 v_texCoord;
out vec4 color;
uniform samplerCube u_skybox;


void main(){

    color = texture(u_skybox, v_texCoord); 
}
