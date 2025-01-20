#pragma once

#include "VoxelVertex.hpp"
#include "chunk_utils.hpp"
#include "MKE/DrawContext.hpp"
#include "MKE/Drawable.hpp"
#include "MKE/Game.hpp"
#include "MKE/VertexArray.hpp"

using VoxelFaceArray = mk::VertexArray<VoxelVertex>;

class VoxelTextureFaces final: public mk::Drawable {
public:
	~VoxelTextureFaces() override = default;

	void clearFaces();
	void addFace(mk::math::Vector3i bottom_left, FaceDir dir, usize size_x, usize size_y, GameItem type);
	void save();

private:
	void draw(mk::RenderTarget& target, mk::DrawContext context) const override;

	VoxelFaceArray faces{};
};
