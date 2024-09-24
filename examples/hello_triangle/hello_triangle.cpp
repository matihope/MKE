#include "MKE/Panic.hpp"
#include "glad/glad.h"
#include <MKE/Window.hpp>

int main() {
	mk::Window window(800, 600, "Hello triangle!");

	const char* vertexShaderSource
		= "#version 330 core\n"
		  "layout (location = 0) in vec3 aPos;\n"
		  "void main()\n"
		  "{\n"
		  "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
		  "}\0";

	u32 vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);

	int  success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		MKE_PANIC("Vertex shader compilation has failed:\n", infoLog);
	}

	const char* fragmentShaderSource
		= "#version 330 core\n"
		  "out vec4 FragColor;\n"
		  "void main()\n"
		  "{\n"
		  "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
		  "}\0";

	u32 fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);

	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		MKE_PANIC("Fragment shader compilation has failed:\n", infoLog);
	}

	u32 shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		MKE_PANIC("Shader program compilation has failed:\n", infoLog);
	}

	glUseProgram(shaderProgram);

	float vertices[] = { -0.5f, -0.5f, 0.0f, 0.5f, -0.5f, 0.0f, 0.0f, 0.5f, 0.0f };


	// Create a Vertex Array Object to automate some steps of drawing
	u32 VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	// Create a buffer
	u32 VBO;
	glGenBuffers(1, &VBO);

	// Say it is a Vertex Buffer (by loading VBO to GL_ARRAY_BUFFER place thingy)
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	// Copy vertices into GL_ARRAY_BUFFER, which is VBO.
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// Specify how data is layed out in vertex input
	// index = 0, because location = 0 in vertex shader
	// size - size of one vertex attribute
	// pointer - pointer to the buffer where data starts
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*) 0);
	glEnableVertexAttribArray(0);

	bool run = true;
	while (run) {
		mk::Event event;
		while (window.pollEvent(event))
			if (event.type == mk::EventType::WindowClose) run = false;
		window.clear(mk::Colors::DARK);

		glUseProgram(shaderProgram);
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		window.display();
	}
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteProgram(shaderProgram);
}
