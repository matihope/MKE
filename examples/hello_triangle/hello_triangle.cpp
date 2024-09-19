#include "MKE/Panic.hpp"
#include "glad/glad.h"
#include <MKE/Window.hpp>

int main() {
	mk::Window window(800, 600, "Hello window!");

	float vertices[] = { -0.5f, -0.5f, 0.0f, 0.5f, -0.5f, 0.0f, 0.0f, 0.5f, 0.0f };

	// Create a buffer
	u32 VBO;
	glGenBuffers(1, &VBO);

	// Say it is a Vertex Buffer (by loading VBO to GL_ARRAY_BUFFER place thingy)
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	// Copy vertices into GL_ARRAY_BUFFER, which is VBO.
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	const char* vertexShaderSource
		= "#version 330 core\n"
		  "layout (location = 0) in vec3 aPos;\n"
		  "void main()\n"
		  "{\n"
		  "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
		  "}\0";

	u32 vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);

	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);

	int  success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		MKE_PANIC("ERROR::SHADER::VERTEX::COMPILATION_FAILED\n", infoLog);
	}

	bool run = true;
	while (run) {
		mk::Event event;
		while (window.pollEvent(event))
			if (event.type == mk::EventType::WindowClose) run = false;

		window.clear(mk::Colors::DARK);
		window.display();
	}
}
