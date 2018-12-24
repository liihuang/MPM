#ifndef MAIN_H
#define MAIN_H

#include <glad\glad.h>
#include <GLFW\glfw3.h>

#include <chrono>
#include <stdlib.h>
#include <time.h>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Shader.h"
#include "Scene.h"
#include "Util.h"
#include "SimConstant.h"
#include "MPM_Simulator.h"
#include "Vector2f.h"
#include "Matrix2f.h"


using namespace std::chrono;


int frame_count;

void initialGLFW() {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
}

GLFWwindow* initialWindow(int width, int height, const char* title) {
	GLFWwindow* window = glfwCreateWindow(width, height, title, NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return NULL;
	}
	glfwMakeContextCurrent(window);
	return window;
}

int initialGLAD() {
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return 0;
	}
	return 1;
}

#endif
