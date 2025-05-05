#include "VoxelVertex.hpp"
#include "glad/glad.h"

void VoxelVertex::encodeData(mk::math::Vector3i position, FaceDir dir, GameItem type) {
	encodeData(position.x, position.y, position.z, dir, type);
}

void VoxelVertex::encodeData(i32 x, i32 y, i32 z, FaceDir dir, GameItem type) {
	i32 new_data = 0;
	new_data |= (z & 63) << 0;
	new_data |= (y & 63) << 6;
	new_data |= (x & 63) << 12;
	new_data |= static_cast<int>(dir) << 18;
	new_data |= static_cast<int>(type) << 21;
	data = new_data;
}

void VoxelVertex::configureVertexAttribute() {
	glVertexAttribIPointer(
		0,
		sizeof(i32),
		GL_INT,
		sizeof(VoxelVertex),
		static_cast<void*>(nullptr)
	);
	glEnableVertexAttribArray(0);
}
