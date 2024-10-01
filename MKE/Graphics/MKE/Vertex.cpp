#include "Vertex.hpp"
#include "glad/glad.h"

void mk::Vertex2D::configureVertexAttribute() {
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex2D), (void*) 0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex2D), (void*) (sizeof(position)));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(
		2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex2D), (void*) (sizeof(position) + sizeof(color))
	);
	glEnableVertexAttribArray(2);
}
