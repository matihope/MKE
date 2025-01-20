#include "VoxelVertex.hpp"
#include "glad/glad.h"

void VoxelVertex::configureVertexAttribute() {
	glVertexAttribIPointer(
		0,
		sizeof(position) / sizeof(i32),
		GL_INT,
		sizeof(VoxelVertex),
		(void*)(0)
	);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(
		1,
		sizeof(tex_coords) / sizeof(float),
		GL_FLOAT,
		GL_FALSE,
		sizeof(VoxelVertex),
		(void*) (sizeof(position))
	);
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(
		2,
		sizeof(color) / sizeof(float),
		GL_FLOAT,
		GL_FALSE,
		sizeof(VoxelVertex),
		(void*) (sizeof(position) + sizeof(tex_coords))
	);
	glEnableVertexAttribArray(2);
}
