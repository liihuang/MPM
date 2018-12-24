#version 330 core

in vec2 ourColor;

out vec4 FragColor;

void main(){
	if (length(gl_PointCoord - vec2(0.5)) > 0.5) {  
        discard;  
    } 
	FragColor = vec4(ourColor.x, 0.0f, ourColor.y, 1.0f);
	//FragColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);
}