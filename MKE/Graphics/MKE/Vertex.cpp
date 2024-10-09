#include "Vertex.hpp"
#include "glad/glad.h"

void mk::Vertex2D::configureVertexAttribute() {
	glVertexAttribPointer(
		0, sizeof(position) / sizeof(float), GL_FLOAT, GL_FALSE, sizeof(Vertex2D), (void*) 0
	);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(
		1,
		sizeof(color) / sizeof(float),
		GL_FLOAT,
		GL_FALSE,
		sizeof(Vertex2D),
		(void*) (sizeof(position))
	);
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(
		2,
		sizeof(tex_coords) / sizeof(float),
		GL_FLOAT,
		GL_FALSE,
		sizeof(Vertex2D),
		(void*) (sizeof(position) + sizeof(color))
	);
	glEnableVertexAttribArray(2);
}

void mk::Vertex2DNoColor::configureVertexAttribute() {
	glVertexAttribPointer(
		0, sizeof(position) / sizeof(float), GL_FLOAT, GL_FALSE, sizeof(Vertex2DNoColor), (void*) 0
	);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(
		1,
		sizeof(tex_coords) / sizeof(float),
		GL_FLOAT,
		GL_FALSE,
		sizeof(Vertex2DNoColor),
		(void*) (sizeof(position))
	);
	glEnableVertexAttribArray(1);
}

void mk::Vertex2DNoTexture::configureVertexAttribute() {
	glVertexAttribPointer(
		0,
		sizeof(position) / sizeof(float),
		GL_FLOAT,
		GL_FALSE,
		sizeof(Vertex2DNoTexture),
		(void*) 0
	);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(
		1,
		sizeof(color) / sizeof(float),
		GL_FLOAT,
		GL_FALSE,
		sizeof(Vertex2DNoTexture),
		(void*) (sizeof(position))
	);
	glEnableVertexAttribArray(1);
}

void mk::Vertex3D::configureVertexAttribute() {
	glVertexAttribPointer(
		0, sizeof(position) / sizeof(float), GL_FLOAT, GL_FALSE, sizeof(Vertex3D), (void*) 0
	);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(
		1,
		sizeof(color) / sizeof(float),
		GL_FLOAT,
		GL_FALSE,
		sizeof(Vertex3D),
		(void*) (sizeof(position))
	);
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(
		2,
		sizeof(tex_coords) / sizeof(float),
		GL_FLOAT,
		GL_FALSE,
		sizeof(Vertex3D),
		(void*) (sizeof(position) + sizeof(color))
	);
	glEnableVertexAttribArray(2);
}
