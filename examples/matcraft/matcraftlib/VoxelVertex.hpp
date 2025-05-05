#pragma once
#include "chunk_utils.hpp"
#include "MKE/Color.hpp"
#include "MKE/Math/Vector.hpp"

struct VoxelVertex {
	i32 data = 0;

	void encodeData(mk::math::Vector3i position, FaceDir dir, GameItem type);
	void encodeData(i32 x, i32 y, i32 z, FaceDir dir, GameItem type);

	// 31 30 29 28 27 26 25 24 23 22 21 20 19 18 17 16 15 14 13 12 11 10 9 8 7 6 5 4 3 2 1 0
	//	  				 25[----type-21[-dir--18[-------X-------12[----Y-------6[----Z-----0

	static void configureVertexAttribute();
};
