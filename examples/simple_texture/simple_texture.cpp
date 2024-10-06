#include <MKE/Shader.hpp>
#include <MKE/Window.hpp>
#include "MKE/Event.hpp"
#include "MKE/ResPath.hpp"
#include "MKE/Texture.hpp"
#include "glad/glad.h"
#include <stb_image.h>

int main() {
	mk::Window window(800, 600, "Simple game");

	mk::Shader shader("shader.vs", "shader.fs");

	float vertices[] = {
		// positions          // colors           // texture coords
		0.5f,  0.5f,  0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,  // top right
		0.5f,  -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,  // bottom right
		-0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,  // bottom left
		-0.5f, 0.5f,  0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f   // top left
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

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*) 0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*) (3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*) (6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	mk::Texture texture;
	texture.loadFromFile("arrow.png");
	texture.setSmooth(false);

	while (!window.isExitRequested()) {
		window.clear(mk::Colors::DARK);

		mk::Shader::use(&shader);
		mk::Texture::bind(&texture);

		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		window.display();
	}
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
}
