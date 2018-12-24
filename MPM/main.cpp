#include "main.h"

int main() {
	//Create main window
	initialGLFW();
	GLFWwindow* main_window = initialWindow(WIN_WIDTH, WIN_HEIGHT, "MPM");
	initialGLAD();//GLAD must be initialized after creating window
	glViewport(0, 0, WIN_WIDTH, WIN_HEIGHT);

	Shader shader("Shader/vertexShader.vs", "Shader/fragmentShader.fs");
	shader.use();
	int position_loc = glGetAttribLocation(shader.ID, "aPos");
	int color_loc = glGetAttribLocation(shader.ID, "aColor");
	
	Scene scene;
	scene.addSphere(Vector2f(0.1, 0.65), 0.1, Vector2f(40.0, 0.0), 100.0);
	scene.addSphere(Vector2f(0.9, 0.50), 0.1, Vector2f(-40.0, 0.0), 100.0);
	//scene.addRectangle(Vector2f(0.25, 0.05), Vector2f(0.75, 0.4));
	MPM_Simulator simulator(scene, TIMESTEP);

	int particleNum = scene.getParticleNum();
	std::vector<float> vertices(particleNum * 4);
	//simulator.createVAO(vertices);
	
	unsigned int VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	unsigned int VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//glBufferData(GL_ARRAY_BUFFER, particleNum * sizeof(GL_FLOAT) * 2, &(vertices[0]), GL_STREAM_DRAW);

	//glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GL_FLOAT), (void*)0);
	glVertexAttribPointer(position_loc, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GL_FLOAT), (void*)0);
	glEnableVertexAttribArray(position_loc);
	glVertexAttribPointer(color_loc, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GL_FLOAT), (void*)(2 * sizeof(GL_FLOAT)));
	glEnableVertexAttribArray(color_loc);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	glm::mat4 view;
	view = glm::translate(view, glm::vec3(-1.0f, -1.0f, 0.0f));

	/*-----time-----*/
	auto previous = system_clock::now(), current = system_clock::now();
	auto duration = duration_cast<microseconds>(current - previous);

	frame_count = 0;

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	
	//Render Loop
	while (!glfwWindowShouldClose(main_window))
	{
		// update particles
		shader.setMat4("transform", view);

		previous = system_clock::now();

		simulator.advance();
		simulator.createVAO(vertices);

		glClear(GL_COLOR_BUFFER_BIT);
		glEnable(GL_POINT_SIZE);

		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, particleNum * sizeof(GL_FLOAT) * 4, &(vertices[0]), GL_STREAM_DRAW);// GL_STREAM_DRAW: 数据每次绘制时都会改变

		glDrawArrays(GL_POINTS, 0, particleNum);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);

		current = system_clock::now();

		duration = duration_cast<microseconds>(current - previous);
		
		std::cout << "Frame : " << ++frame_count << std::endl;
		std::cout << "cost " << double(duration.count()) * microseconds::period::num / microseconds::period::den << " seconds." << std::endl;

		glfwSwapBuffers(main_window);
		glfwPollEvents();
	}
	
	glfwTerminate();
	return 0;
}