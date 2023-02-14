
#version 460 core
layout(location = 0) in vec3 Position;
layout(location = 1) in vec2 Texcoord;

out vec2 texcoord0;

void main(){

gl_Position = vec4(Position, 1.0);
texcoord0 = Texcoord; 
}