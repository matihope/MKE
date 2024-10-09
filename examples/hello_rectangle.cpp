#include "MKE/Panic.hpp"
#include "glad/glad.h"
#include <MKE/Window.hpp>
#include <GLFW/glfw3.h>

int main() {
	mk::Window window(800, 600, "Hello rectangle!");

	const char* vertexShaderSource
		= "#version 410 core\n"
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
		MK_PANIC("Vertex shader compilation has failed:\n", infoLog);
	}

	const char* fragmentShaderSource
		= "#version 410 core\n"
		  "out vec4 FragColor;\n"
		  "uniform vec4 ourColor;\n"
		  "void main()\n"
		  "{\n"
		  "   FragColor = ourColor;\n"
		  "}\0";

	u32 fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);

	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		MK_PANIC("Fragment shader compilation has failed:\n", infoLog);
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
		MK_PANIC("Shader program compilation has failed:\n", infoLog);
	}

	glUseProgram(shaderProgram);

	float vertices[] = {
		0.5f,  0.5f,  0.0f,  // top right
		0.5f,  -0.5f, 0.0f,  // bottom right
		-0.5f, -0.5f, 0.0f,  // bottom left
		-0.5f, 0.5f,  0.0f   // top left
	};
	u32 indices[] = {
		// note that we start from 0!
		0, 1, 3,  // first triangle
		1, 2, 3   // second triangle
	};

	// Create a Vertex Array Object to automate some steps of drawing
	u32 VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	// Create a buffer
	u32 VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// Create element buffer
	u32 EBO;
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// Specify how data is layed out in vertex input
	// index = 0, because location = 0 in vertex shader
	// size - size of one vertex attribute
	// pointer - pointer to the buffer where data starts
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*) 0);
	glEnableVertexAttribArray(0);

	// Enable wireframe
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	int vertexColorLocation = glGetUniformLocation(shaderProgram, "ourColor");

	while (!window.isExitRequested()) {
		window.clear(mk::Colors::DARK);

		float timeValue  = glfwGetTime();
		float greenValue = (sin(timeValue) / 2.0f) + 0.5f;

		glUseProgram(shaderProgram);
		glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f);

		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		window.display();
	}
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteProgram(shaderProgram);
}
