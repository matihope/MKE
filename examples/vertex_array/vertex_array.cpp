#include <MKE/Shader.hpp>
#include <MKE/Window.hpp>
#include "MKE/Event.hpp"
#include "MKE/Math/Vector2.hpp"
#include "MKE/RenderWindow.hpp"
#include "MKE/Vertex.hpp"
#include "MKE/VertexArray.hpp"
#include "MKE/Texture.hpp"

int main() {
	mk::RenderWindow window(800, 600, "VertexArray");

	mk::Vertex2D vertices[] = {
		// positions          // colors           // texture coords
		mk::Vertex2D({ 0.5f, 0.5f }, { 255, 0, 0 }, { 1.0f, 1.0f }),    // top right
		mk::Vertex2D({ 0.5f, -0.5f }, { 0, 255, 0 }, { 1.0f, 0.0f }),   // bottom right
		mk::Vertex2D({ -0.5f, -0.5f }, { 0, 0, 255 }, { 0.0f, 0.0f }),  // bottom left
		mk::Vertex2D({ -0.5f, 0.5f }, { 255, 255, 0 }, { 0.0f, 1.0f })    // top left
	};
	u32 indices[] = {
		// note that we start from 0!
		0, 1, 3,  // first triangle
		1, 2, 3   // second triangle
	};

	mk::VertexArray2D vertex_array(true);
	vertex_array.setSize(4);
	vertex_array.setVertexBuffer(vertices, 4);
	vertex_array.setElementBuffer(indices, sizeof(indices));

	bool run = true;

	mk::Texture texture;
	texture.loadFromFile(mk::ResPath::example("arrow.png"));
	texture.setSmooth(false);

	while (run) {
		mk::Event event;
		while (window.pollEvent(event))
			if (event.type == mk::EventType::WindowClose) run = false;
		window.clear(mk::Colors::DARK);
		window.render(vertex_array, texture);
		window.display();
	}
}
