#pragma once

#include "VoxelVertex.hpp"
#include "MKE/DrawContext.hpp"
#include "MKE/Drawable.hpp"
#include "MKE/Game.hpp"
#include "MKE/VertexArray.hpp"

#include <array>

enum class VoxelType : u8 { EMPTY, DIRT };
constexpr u8 VOXEL_TYPES = 2;

enum class FaceDir : u8 { NORTH, EAST, SOUTH, WEST, UP, DOWN };
constexpr u8 FACE_DIRS = 6;


using VoxelFaceArray = mk::VertexArray<VoxelVertex>;

void initTextures(mk::Game& game);

class VoxelTextureFaces final: public mk::Drawable {
public:
	explicit VoxelTextureFaces(const VoxelType type): type(type) {}

	~VoxelTextureFaces() override = default;

	void addFace(mk::math::Vector3i bottom_left, FaceDir dir, usize size_x, usize size_y);
	void save();

private:
	void draw(mk::RenderTarget& target, mk::DrawContext context) const override;

	std::array<VoxelFaceArray, 6> faces{};
	VoxelType                     type;
};
