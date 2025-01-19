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

		void setPositionBase(math::Vector3f position) override;
		// no need for getPosition() since WorldEntity3D::getPosition() == view.getPosition()

		void           setDirection(math::Vector3f direction);
		math::Vector3f getDirection() const;

		void lookAt(math::Vector3f at);


		void           setPitchYawRoll(math::Vector3f pitch_yaw_roll);
		math::Vector3f getPitchYawRoll() const;

		void  setFov(float fov);
		float getFovH() const;
		float getFovV() const;

		void  setAspect(float aspect);
		float getAspect() const;
		void  setNear(float near);
		void  setFar(float far);


	private:
		View3D view;
	};
}
