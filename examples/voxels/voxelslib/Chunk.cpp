#include "Chunk.hpp"

#include "World.hpp"
#include "MKE/Random.hpp"
#include "MKE/Primitives/3d/CubePrimitive.hpp"
#include "MKE/Shaders/SimpleShader.hpp"

namespace {
	bool anyOf(auto voxels, GameItem type) {
		for (auto& val: voxels)
			if (val == type) return true;
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
		MK_ASSERT(x >= 0 && x < CHUNK_SIZE && y >= 0 && y < CHUNK_SIZE && z >= 0 && z < CHUNK_SIZE);
		return x * CHUNK_SIZE * CHUNK_SIZE + y * CHUNK_SIZE + z;
	}

	constexpr usize getIdx(const mk::math::Vector3i pos) { return getIdx(pos.x, pos.y, pos.z); }
}

void Chunk::onReady(mk::Game& game) {}

void Chunk::generateTerrain(mk::Game& game, World& world) {
	for (i32 x = 0; x < CHUNK_SIZE; ++x) {
		for (i32 z = 0; z < CHUNK_SIZE; ++z) {
			i32 TOP_BLOCK = world.getChunkGenHeight(
				x + int_position.x * CHUNK_SIZE, z + int_position.z * CHUNK_SIZE
			);
			for (i32 y = 0; y < CHUNK_SIZE; ++y) {
				const mk::math::Vector3i voxel_world_pos
					= mk::math::Vector3i(x, y, z) + int_position * CHUNK_SIZE;

				if (voxel_world_pos.y == 0)
					setBlock(x, y, z, GameItem::BEDROCK, false);
				else if (TOP_BLOCK < 10) {
					if (voxel_world_pos.y <= 10) setBlock(x, y, z, GameItem::WATER, false);
				} else {
					if (voxel_world_pos.y == TOP_BLOCK)
						setBlock(x, y, z, GameItem::GRASS, false);
					else if (TOP_BLOCK >= voxel_world_pos.y && voxel_world_pos.y >= TOP_BLOCK - 2)
						setBlock(x, y, z, GameItem::DIRT, false);
					else if (voxel_world_pos.y < TOP_BLOCK) {
						if (mk::Random::getInt(0, 120) == 0)
							setBlock(x, y, z, GameItem::DIAMOND_ORE, false);
						else if (mk::Random::getInt(0, 80) == 0)
							setBlock(x, y, z, GameItem::GOLD_ORE, false);
						else
							setBlock(x, y, z, GameItem::STONE, false);
					}
				}
			}
		}
	}
}

void Chunk::generateTrees(mk::Game& game, World& world) {
	constexpr i32 LEAF_BUFF = 2;
	for (i32 x = LEAF_BUFF; x < CHUNK_SIZE - LEAF_BUFF - 1; ++x) {
		for (i32 z = LEAF_BUFF; z < CHUNK_SIZE - LEAF_BUFF - 1; ++z) {
			i32 y = 31 - LEAF_BUFF;
			while (y) {
				const i32 TREE_HEIGHT = mk::Random::getInt(2 + LEAF_BUFF + 1, 6 + LEAF_BUFF + 1);
				if (i32 height = 32 - y; height >= TREE_HEIGHT
				                         && y + TREE_HEIGHT + LEAF_BUFF < CHUNK_SIZE
				                         && voxels[getIdx(x, y - 1, z)] != GameItem::AIR
				                         && voxels[getIdx(x, y, z)] == GameItem::AIR) {
					if (mk::Random::getInt(0, CHUNK_SIZE * CHUNK_SIZE * 3) == 0) {
						for (i32 t_x = -2; t_x <= 2; t_x++) {
							for (i32 t_y = -2; t_y <= 2; t_y++)
								for (i32 t_z = -2; t_z <= 2; t_z++) {
									if (t_x * t_x + t_y * t_y + t_z * t_z >= 6) continue;
									setBlock(
										x + t_x,
										y + TREE_HEIGHT - t_y - 1,
										z + t_z,
										GameItem::LEAF,
										false
									);
								}
						}
						for (i32 t_y = 0; t_y < TREE_HEIGHT; ++t_y)
							setBlock(x, y + t_y, z, GameItem::LOG, false);
					}
					break;
				}
				y--;
			}
		}
	}
	buildMeshes();
}

void Chunk::onDraw(mk::RenderTarget& target, mk::DrawContext context, const mk::Game&) const {
	context.transform *= getTransform();
	for (const auto& [tp, arr]: faces) {
		const bool is_translucent = IS_TRANSLUCENT[static_cast<usize>(tp)];
		if ((chunk_draw_mode == ChunkDrawMode::ONLY_TRANSLUCENT && is_translucent)
		    || (chunk_draw_mode == ChunkDrawMode::ONLY_OPAQUE && !is_translucent)) {
			target.render(arr, context);
		}
	}
}

void Chunk::setBlock(
	const usize x, const usize y, const usize z, GameItem type, const bool rebuild
) {
	MK_ASSERT(x < CHUNK_SIZE && y < CHUNK_SIZE && z < CHUNK_SIZE);
	const auto idx  = getIdx(x, y, z);
	auto       prev = voxels[idx];
	if (prev == type) return;

	voxels[idx] = type;

	if (rebuild) buildMeshes();
}

GameItem Chunk::getBlockType(usize x, usize y, usize z) const { return voxels[getIdx(x, y, z)]; }

GameItem Chunk::getBlockType(const mk::math::Vector3u pos) const {
	return getBlockType(pos.x, pos.y, pos.z);
}

mk::math::Vector3i Chunk::getIntPosition() const { return int_position; }

void Chunk::clearFacesOf(const GameItem type) {
	if (const auto it = faces.find(type); it != faces.end()) faces.erase(it);
}

VoxelTextureFaces& Chunk::getFacesOf(const GameItem type) {
	auto [obj, _] = faces.try_emplace(type, type, camera);
	return obj->second;
}

constexpr bool isFaceVisible(
	GameItem                                                          pos_type,
	const std::array<GameItem, CHUNK_SIZE * CHUNK_SIZE * CHUNK_SIZE>& voxels,
	const mk::math::Vector3i                                          pos,
	const FaceDir                                                     face
) {
	if (voxels[getIdx(pos)] != pos_type) return false;
	if (const auto new_pos = pos + getDirVec(face); isValid(new_pos)) {
		if (pos_type == GameItem::WATER && voxels[getIdx(new_pos)] == GameItem::WATER) return false;
		return IS_TRANSLUCENT[static_cast<usize>(voxels[getIdx(new_pos)])];
	}
	return true;
}

void buildFaceArray(
	GameItem                                                          type,
	const std::array<GameItem, CHUNK_SIZE * CHUNK_SIZE * CHUNK_SIZE>& voxels,
	const FaceDir                                                     dir,
	VoxelTextureFaces&                                                face_array
) {
	std::array<bool, CHUNK_SIZE * CHUNK_SIZE * CHUNK_SIZE> faces_to_draw{};
	std::array<bool, CHUNK_SIZE * CHUNK_SIZE * CHUNK_SIZE> checked{};

	for (i32 x = 0; x < CHUNK_SIZE; x++) {
		for (i32 y = 0; y < CHUNK_SIZE; y++)
			for (i32 z = 0; z < CHUNK_SIZE; z++)
				faces_to_draw[getIdx(x, y, z)] = isFaceVisible(type, voxels, { x, y, z }, dir);
	}

	const auto bottom_left           = getBottomLeftFor(dir);
	const auto [grow_up, grow_right] = getGrowDirVec(dir);
	const auto grow_into             = -getDirVec(dir);

	auto curr_into = bottom_left * (CHUNK_SIZE - 1);
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
						const auto exp_idx = getIdx(exp_curr);
						if (!checked[exp_idx] && faces_to_draw[exp_idx]) {
							size_up_now++;
							checked[exp_idx] = true;
						} else {
							break;
						}
						exp_curr += grow_up;
					} while (isValid(exp_curr) && size_up_now < size_up);

					if (size_up_now == 0) break;

					// If this is the first iteration....
					if (size_right == 0) {
						size_up    = size_up_now;
						size_right = 1;
					} else if (size_up == size_up_now) {
						size_right++;
					} else {
						// gotta roll back them faces
						auto exp_curr_rollback = exp_right;
						while (size_up_now--) {
							const auto exp_idx = getIdx(exp_curr_rollback);
							checked[exp_idx]   = false;
							exp_curr_rollback += grow_up;
						}
						break;
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
		const auto voxel_type = static_cast<GameItem>(type_index);
		if (anyOf(voxels, voxel_type)) {
			auto& voxel_faces = getFacesOf(voxel_type);
			// Greedy meshing here.
			voxel_faces.clearFaces();
			for (usize dir_id = 0; dir_id < FACE_DIRS; dir_id++)
				buildFaceArray(voxel_type, voxels, static_cast<FaceDir>(dir_id), voxel_faces);
			voxel_faces.save();
		} else {
			clearFacesOf(voxel_type);
		}
	}
}
