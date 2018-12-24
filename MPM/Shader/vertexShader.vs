#version 430 core

layout (location = 0) in vec2 aPos;
layout (location = 1) in vec2 aColor;

uniform mat4 transform;

out vec2 ourColor;

void main(){
	gl_Position = transform * vec4(2 * aPos.x, 2 * aPos.y, 0.0f, 1.0f);
	gl_PointSize = 3;
	ourColor = aColor;
}