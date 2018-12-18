#include "main.h"

int main() {
	//Create main window
	initialGLFW();
	GLFWwindow* main_window = initialWindow(WinWidth, WinHeight, "MPM");
	initialGLAD();//GLAD must be initialized after creating window
	glViewport(0, 0, WinWidth, WinHeight);

	Shader shader("Shader/vertexShader.vs", "Shader/fragmentShader.fs");
	shader.use();
	int position_loc = glGetAttribLocation(shader.ID, "aPos");

	//std::shared_ptr<Object> snowball = makeSnowball(Vector2f(0.205, 0.795), 0.1, Vector2f(20.0, -5.0));
	std::vector<LargrangianParticle> particles;
	//makeSnowball(particles, Vector2f(0.15, 0.85), 0.1, Vector2f(20.0, -10.0));

	/*--------------*/
	makeSnowball(particles, Vector2f(0.205, 0.65), 0.1, Vector2f(20.0, 0.0));
	makeSnowball(particles, Vector2f(0.795, 0.50), 0.1, Vector2f(-20.0, 0.0));
	
	/*--------------*/
	/*makeSnowball(particles, Vector2f(0.5, 0.16), 0.15, Vector2f(0.0, 0.0));
	makeSnowball(particles, Vector2f(0.5, 0.4), 0.1, Vector2f(0.0, 0.0));
	makeSnowball(particles, Vector2f(0.5, 0.57), 0.07, Vector2f(0.0, 0.0));
	makeSnowball(particles, Vector2f(0.795, 0.5), 0.08, Vector2f(-20.0, -5.0));*/


	int particleNum = particles.size();
	std::shared_ptr<Object> scene(std::make_shared<Object>(std::make_shared<std::vector<LargrangianParticle>>(particles)));
	MPM_Simulator simulator(scene);
	
	std::vector<float> vertices(particleNum * 2);
	simulator.createVAO(vertices);
	//auto vertices = snowball->createVAO();
	
	unsigned int VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	unsigned int VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, particleNum * sizeof(GL_FLOAT) * 2, &(vertices[0]), GL_STREAM_DRAW);

	//glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GL_FLOAT), (void*)0);
	glVertexAttribPointer(position_loc, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glEnableVertexAttribArray(position_loc);
	
	glm::mat4 view;
	view = glm::translate(view, glm::vec3(-0.5f, 0.0f, 0.0f));

	/*-----time-----*/
	auto previous = system_clock::now(), current = system_clock::now();
	auto duration = duration_cast<microseconds>(current - previous);

	frame_count = 0;

	glClearColor(0.05f, 0.1f, 0.1f, 1.0f);
	//Render Loop
	while (!glfwWindowShouldClose(main_window))
	{
		glClear(GL_COLOR_BUFFER_BIT);
		
		glEnable(GL_PROGRAM_POINT_SIZE);
		
		shader.setMat4("transform", view);

		previous = system_clock::now();

		simulator.update();
		simulator.createVAO(vertices);

		//glBindVertexArray(VAO);
		glBufferData(GL_ARRAY_BUFFER, particleNum * sizeof(GL_FLOAT) * 2, &(vertices[0]), GL_STREAM_DRAW);

		glDrawArrays(GL_POINTS, 0, particleNum);

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