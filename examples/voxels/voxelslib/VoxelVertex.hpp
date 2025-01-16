//
// Created by Mateusz Kolpa on 15/01/2025.
//

#pragma once
#include "MKE/Color.hpp"
#include "MKE/Math/Vector.hpp"

struct VoxelVertex {
	mk::math::Vector3i position{};                 // 12 bytes
	mk::math::Vector2f tex_coords{};               // 8 bytes
	mk::Color          color = mk::Colors::WHITE;  // 16 bytes

	static void configureVertexAttribute();
};
