#include "MKE/Color.hpp"
#include "MKE/Input.hpp"
#include "MKE/Math/Vector.hpp"
#include "MKE/Nodes/2d/RectShape.hpp"
#include "MKE/Nodes/GUI/Button.hpp"
#include "MKE/Random.hpp"
#include "MKE/ResourceManager.hpp"
#include "MKE/WorldEntity.hpp"
#include <MKE/Game.hpp>

class Player: public mk::RectShape {
public:
	Player(): mk::RectShape(mk::Colors::WHITE, { 50.f, 50.f }) {}

	void onReady(mk::Game& game) override {
		setOrigin(25.f, 25.f);
		texture = game.resources().getTexture("arrow.png");
	}

	void onUpdate(mk::Game& game, float dt) override {
		bool right = game.isKeyPressed(mk::input::KEY::ARROW_RIGHT);
		bool left  = game.isKeyPressed(mk::input::KEY::ARROW_LEFT);
		bool up    = game.isKeyPressed(mk::input::KEY::ARROW_UP);
		bool down  = game.isKeyPressed(mk::input::KEY::ARROW_DOWN);

		mk::math::Vector2f move_vec(right - left, down - up);
		move_vec = move_vec.normalizeOrZero();
		move_vec *= 500.f * dt;

		bool rr = game.isKeyPressed(mk::input::KEY::E);
		bool rl = game.isKeyPressed(mk::input::KEY::Q);
		rotate(mk::math::Vector3f((float) rr - rl, 0, 0) * 2 * dt);

		move(move_vec);
	}

	void onDraw(mk::RenderTarget& target, mk::DrawContext context, const mk::Game& game)
		const override {
		context.texture = texture;
		mk::RectShape::onDraw(target, context, game);
	}

	const mk::Texture* texture = nullptr;
};

class World: public mk::WorldEntity2D {
public:
	void onReady(mk::Game& game) override {
		player = addChild<Player>(game);

		button = addChild<mk::gui::Button>(game, game.getDefaultFont(), "Randomize");
		button->setMinSpaceBetween({ 10.f, 10.f });
		button->setPosition(300, 300);
	}

	void onUpdate(mk::Game& game, float) override {
		if (button->isPressed()) {
			auto [w, h] = game.getWindowSize().bind();
			player->setPosition({ mk::Random::getReal<float>(0, w),
			                      mk::Random::getReal<float>(0, h) });
		}
	}

	Player*          player = nullptr;
	mk::gui::Button* button = nullptr;
};

int main() {
	mk::Game game("settings.json");
	game.addScene<World>();
	game.run();
}
