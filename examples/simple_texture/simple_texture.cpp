#include <MKE/Shader.hpp>
#include <MKE/Window.hpp>
#include "MKE/Event.hpp"
#include "MKE/Input.hpp"
#include "MKE/Math/Matrix.hpp"
#include "MKE/Math/Vector2.hpp"
#include "MKE/ResPath.hpp"
#include "glad/glad.h"

int main() {
	mk::Window window(800, 600, "Simple game");

	mk::Shader shader(mk::ResPath::example("shader.vs"), mk::ResPath::example("shader.fs"));

	float vertices[] = {
		// positions         // colors
		0.0f,  0.0f,  0.0f, 1.0f, 0.0f, 0.0f,  // top left
		0.0f,  10.0f, 0.0f, 0.0f, 1.0f, 0.0f,  // bottom left
		10.0f, 10.0f, 0.0f, 0.0f, 0.0f, 1.0f,  // bottom right
		10.0f, 0.0f,  0.0f, 0.0f, 0.0f, 1.0f   // top right
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

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*) 0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*) (3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	bool run = true;

	mk::math::Vector2f pos   = { 0, 0 };
	mk::math::Vector2f scale = { 50., 50. };
	scale /= window.getSize().as<mk::math::Vector2f>();

	bool enabled = true;

	while (run) {
		mk::Event event;
		while (window.pollEvent(event)) {
			if (event.type == mk::EventType::WindowClose) run = false;
			if (event.type == mk::EventType::KeyPressed
			    && event.key_pressed.key == mk::input::KEY::SPACE) {
				window.enableVerticalSync(enabled = !enabled);
			}
		}
		window.clear(mk::Colors::DARK);

		pos.x += ((int) window.isKeyPressed(mk::input::KEY::D)
		          - window.isKeyPressed(mk::input::KEY::A))
		       * 0.01;

		pos.y += ((int) window.isKeyPressed(mk::input::KEY::W)
		          - window.isKeyPressed(mk::input::KEY::S))
		       * 0.01;

		mk::math::Matrix4f transform = {
			std::array{ scale.x, 0.f, 0.f, pos.x },
			std::array{ 0.f, scale.y, 0.f, pos.y },
			std::array{ 0.f, 0.f, 1.f, 0.f },
			std::array{ 0.f, 0.f, 0.f, 1.f },
		};

		shader.use();
		shader.setMatrix4f("transform", transform);
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		window.display();
	}
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
}
