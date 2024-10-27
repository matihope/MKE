#pragma once

#include "MKE/WorldEntity.hpp"

namespace mk {

	class Camera2D: public View2D, public WorldEntity2D {
		// onUpdate() game.getRenderWindow.setView(myView);
	};

	class Camera3D: public View3D, public WorldEntity3D {
		// onUpdate() game.getRenderWindow.setView(myView);
	};
}
