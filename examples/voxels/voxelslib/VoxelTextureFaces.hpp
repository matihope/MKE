#pragma once

#include "VoxelVertex.hpp"
#include "chunk_utils.hpp"
#include "MKE/Camera.hpp"
#include "MKE/DrawContext.hpp"
#include "MKE/Drawable.hpp"
#include "MKE/Game.hpp"
#include "MKE/VertexArray.hpp"

#include <array>

using VoxelFaceArray = mk::VertexArray<VoxelVertex>;

class VoxelTextureFaces final: public mk::Drawable {
public:
	VoxelTextureFaces(const VoxelType type, mk::Camera3D* camera): type(type), camera(camera) {}

	~VoxelTextureFaces() override = default;

	void clearFaces();
	void addFace(mk::math::Vector3i bottom_left, FaceDir dir, usize size_x, usize size_y);
	void save();

private:
	void draw(mk::RenderTarget& target, mk::DrawContext context) const override;

	std::array<VoxelFaceArray, 6> faces{};
	VoxelType                     type;
	mk::Camera3D*                 camera;
};
