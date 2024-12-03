#include "MKE/Input.hpp"
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

		void onEvent(mk::Game&, const mk::Event& event) override {
			if (auto ev = event.get<mk::Event::KeyPressed>(); ev) {
				if (ev->key == mk::input::KEY::SPACE) speed.y = -10.f;
			}
		}

		void onPhysicsUpdate(mk::Game& game, float dt) override {
			float hsp = (i32) (game.isKeyPressed(mk::input::KEY::D))
			          - game.isKeyPressed(mk::input::KEY::A);
			hsp *= 5.f;

			speed.x = math::lerp(speed.x, hsp, dt * 10.f);

			speed.y += gravity;
			if (game.isKeyJustPressed(mk::input::KEY::SPACE)) speed.y = -10.f;

			move(speed);
		}

		math::Vector2f speed;
		float          gravity = 0.4f;

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
