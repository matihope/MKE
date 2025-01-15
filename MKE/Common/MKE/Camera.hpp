#pragma once

#include "MKE/Math/Vector.hpp"
#include "MKE/View.hpp"
#include "MKE/WorldEntity.hpp"

namespace mk {

	class Camera2D: public WorldEntity2D {
	public:
		void update(Game& game, float dt) override;

	private:
		View2D view;
	};

	class Camera3D: public WorldEntity3D {
	public:
		void update(Game& game, float dt) override;

		void event(Game& game, const Event& event) override;

		void setPosition(math::Vector3f position);
		void setDirection(math::Vector3f direction);
		void lookAt(math::Vector3f at);

	private:
		View3D view;
	};
}
