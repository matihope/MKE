#include "MKE/WorldEntity.hpp"
#include "MKE/Nodes/2d/RectShape.hpp"
#include <MKE/Game.hpp>

namespace game {
	using namespace mk;

	class Player: public mk::WorldEntity2D {
	public:
		void onReady(mk::Game& game) override {
			setPosition(50.f, 50.f);
			body = addChild<RectShape>(game, Colors::GREEN, math::Vector2f(50.f, 50.f));
		}

		void onPhysicsUpdate(mk::Game& game, float dt) override {
			float hsp
				= (i32)(game.isKeyPressed(mk::input::KEY::D)) - game.isKeyPressed(mk::input::KEY::A);

			if(false) {

				float time = 0.1f;
				speed.x = ((1.f - dt / time) * speed.x + hsp * dt / time);
				if(abs(speed.x - hsp) < 1e-3) speed.x = hsp;
			} else {
				speed.x = speed.x + hsp * dt / time);
				if(abs(speed.x - hsp) < 1e-3) speed.x = hsp;
			}

				std::cout << speed.x << '\n';
			move(speed);
		}

		math::Vector2f speed;
		float          gravity = -0.4f;

		RectShape* body = nullptr;
	};

	class Level: public WorldEntityUI {
	public:
		void onReady(mk::Game& game) override { player = addChild<Player>(game); }

		Player* player = nullptr;
	};

}

int main() {
	mk::Game game("settings.json");
	game.addScene<game::Level>();
	game.run();
}
