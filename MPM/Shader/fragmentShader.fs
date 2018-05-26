#version 330 core
out vec4 FragColor;

void main(){
	if (length(gl_PointCoord - vec2(0.5)) > 0.5) {  
        discard;  
    } 
	FragColor = vec4(0.9f, 0.9f, 0.9f, 1.0f);
}