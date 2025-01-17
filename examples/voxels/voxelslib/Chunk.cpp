#include "Chunk.hpp"

namespace {
	bool anyOf(auto voxels, VoxelType type) {
		for (auto& val: voxels[static_cast<usize>(type)])
			if (val) return true;
		return false;
	}

	constexpr bool isValid(const mk::math::Vector3i pos) {
		for (const auto d: pos.vec_data)
			if (d == -1 || d == CHUNK_SIZE) return false;
		return true;
	}

	constexpr bool isValidMove(mk::math::Vector3i pos, const mk::math::Vector3i dir) {
		pos += dir;
		return isValid(pos);
	}

	constexpr usize getIdx(auto x, auto y, auto z) {
		return x * CHUNK_SIZE * CHUNK_SIZE + y * CHUNK_SIZE + z;
	}

	constexpr usize getIdx(const mk::math::Vector3i pos) { return getIdx(pos.x, pos.y, pos.z); }
}

void Chunk::onReady(mk::Game& game) {
	for (usize x = 0; x < CHUNK_SIZE; ++x) {
		for (usize y = 0; y < CHUNK_SIZE; ++y) {
			for (usize z = 0; z < CHUNK_SIZE; ++z)
				if (y == CHUNK_SIZE - 1)
					setBlock(x, y, z, VoxelType::GRASS, false);
				else if (y >= CHUNK_SIZE - 4)
					setBlock(x, y, z, VoxelType::DIRT, false);
				else
					setBlock(x, y, z, VoxelType::STONE, false);
		}
	}
	buildMeshes();
	shader.load(mk::ResPath("voxel.vert"), mk::ResPath("voxel.frag"));
}

void Chunk::onDraw(mk::RenderTarget& target, mk::DrawContext context, const mk::Game&) const {
	context.transform *= getTransform();
	context.shader = &shader;
	// Here we should not draw chunks that player cannot see.
	for (const auto& arr: faces | std::views::values) target.render(arr, context);
}

void Chunk::setBlock(usize x, usize y, usize z, VoxelType type, const bool rebuild) {
	auto idx  = getIdx(x, y, z);
	auto prev = filled_blocks[idx];
	if (prev == type) return;

	voxels[static_cast<usize>(prev)][idx] = false;
	voxels[static_cast<usize>(type)][idx] = true;
	filled_blocks[idx]                    = type;

	if (rebuild) buildMeshes();
}

void Chunk::clearFacesOf(const VoxelType type) {
	if (const auto it = faces.find(type); it != faces.end()) faces.erase(it);
}

VoxelTextureFaces& Chunk::getFacesOf(const VoxelType type) {
	auto [obj, _] = faces.try_emplace(type, type, camera);
	return obj->second;
}

constexpr bool isFaceVisible(
	const std::array<VoxelType, CHUNK_SIZE * CHUNK_SIZE * CHUNK_SIZE>& filled,
	const mk::math::Vector3i                                           pos,
	const FaceDir                                                      face
) {
	if (const auto new_pos = pos + getDirVec(face); isValid(new_pos)) {
		auto [x, y, z] = new_pos.vec_data;
		return filled[x * CHUNK_SIZE * CHUNK_SIZE + y * CHUNK_SIZE + z] == VoxelType::AIR;
	}
	return true;
}

void buildFaceArray(
	const std::array<bool, CHUNK_SIZE * CHUNK_SIZE * CHUNK_SIZE>&      voxels,
	const std::array<VoxelType, CHUNK_SIZE * CHUNK_SIZE * CHUNK_SIZE>& filled,
	const FaceDir                                                      dir,
	VoxelTextureFaces&                                                 face_array
) {
	std::array<bool, CHUNK_SIZE * CHUNK_SIZE * CHUNK_SIZE> faces_to_draw{};
	std::array<bool, CHUNK_SIZE * CHUNK_SIZE * CHUNK_SIZE> checked{};

	for (usize x = 0; x < CHUNK_SIZE; x++) {
		for (usize y = 0; y < CHUNK_SIZE; y++) {
			for (usize z = 0; z < CHUNK_SIZE; z++) {
				const auto  pos_vec = mk::math::Vector3i(x, y, z);
				const usize pos     = getIdx(x, y, z);
				faces_to_draw[pos]  = voxels[pos] && isFaceVisible(filled, pos_vec, dir);
			}
		}
	}

	const auto bottom_left           = getBottomLeftFor(dir);
	const auto [grow_up, grow_right] = getGrowDirVec(dir);
	const auto grow_into             = -getDirVec(dir);

	auto curr_into = bottom_left * 31;
	for (usize k = 0; k < CHUNK_SIZE; k++, curr_into += grow_into) {
		auto curr_up = curr_into;
		for (usize i = 0; i < CHUNK_SIZE; i++, curr_up += grow_up) {
			auto curr_pos = curr_up;
			for (usize j = 0; j < CHUNK_SIZE; j++, curr_pos += grow_right) {
				const auto idx = getIdx(curr_pos);
				if (checked[idx] || !faces_to_draw[idx]) continue;

				// Here we try to expand right and up
				usize size_up
					= CHUNK_SIZE;  // To avoid some logic on first iter, initial = CHUNK_SIZE
				usize size_right = 0;
				auto  exp_right  = curr_pos;
				do {
					usize size_up_now = 0;
					auto  exp_curr    = exp_right;
					// Look up
					do {
						auto exp_idx = getIdx(exp_curr);
						if (!checked[exp_idx] && faces_to_draw[exp_idx] && size_up_now < size_up) {
							size_up_now++;
							checked[exp_idx] = true;
						} else {
							break;
						}
						exp_curr += grow_up;
					} while (isValid(exp_curr));

					if (!size_up_now) break;

					// If this is the first iteration....
					if (size_right == 0) {
						size_up    = size_up_now;
						size_right = 1;
					} else if (size_up == size_up_now) {
						size_right++;
					}
					exp_right += grow_right;
				} while (isValid(exp_right));
				// Add face here!
				if (size_right) face_array.addFace(curr_pos, dir, size_right, size_up);
			}
		}
	}
}

void Chunk::buildMeshes() {
	for (usize type_index = 1; type_index < VOXEL_TYPES; ++type_index) {
		const auto voxel_type = static_cast<VoxelType>(type_index);
		if (anyOf(voxels, voxel_type)) {
			auto& voxel_faces = getFacesOf(voxel_type);
			// Greedy meshing here.
			for (usize dir_id = 0; dir_id < FACE_DIRS; dir_id++) {
				buildFaceArray(
					voxels[type_index], filled_blocks, static_cast<FaceDir>(dir_id), voxel_faces
				);
			}
			voxel_faces.save();
		} else {
			clearFacesOf(voxel_type);
		}
	}
}
