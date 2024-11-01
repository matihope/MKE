#pragma once

#include "MKE/Primitives/3d/CubePrimitive.hpp"
#include "MKE/WorldEntity.hpp"

namespace mk {
	class CubeShape: public WorldEntity3D {
	public:
		CubeShape() = default;

		~CubeShape() = default;

		void onDraw(RenderTarget& target, DrawContext context, const Game& game) const override;

	private:
		CubePrimitive cube;
	};
}
