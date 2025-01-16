//
// Created by Mateusz Kolpa on 15/01/2025.
//

#include "VoxelVertex.hpp"
#include "MKE/VertexArray.hpp"

void VoxelVertex::configureVertexAttribute() {
	glVertexAttribPointer(
		0,
		sizeof(position) / sizeof(float),
		GL_INT,
		GL_FALSE,
		sizeof(VoxelVertex),
		static_cast<void*>(nullptr)
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
