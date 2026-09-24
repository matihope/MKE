#include "MKE/Color.hpp"
#include "MKE/Input.hpp"
#include "MKE/Math/Vector.hpp"
#include "MKE/Nodes/2d/RectShape.hpp"
#include "MKE/Nodes/GUI/Button.hpp"
#include "MKE/Primitives/2d/RectPrimitive.hpp"
#include "MKE/Random.hpp"
#include "MKE/ResourceManager.hpp"
#include "MKE/WorldEntity.hpp"
#include <MKE/Game.hpp>

class Fruit;

class Player: public mk::WorldEntity2D {
	usize ticks_per_second = 3;
	float acc              = 0.f;

	mk::math::Vector2i dir;

	std::list<std::unique_ptr<mk::RectPrimitive>> rects;

	const float size;

public:
	Player(float size): size(size) {}

	void onReady(mk::Game&) override {
		dir = mk::math::Vector2i(0, 0);
		rects.push_back(std::make_unique<mk::RectPrimitive>(size));
	}

	auto getHead() {
		return rects.front()->getPosition2D();
	}

	void grow() {
		rects.push_back(std::make_unique<mk::RectPrimitive>(size));
		rects.back()->setPosition(getHead());
	}

	void onUpdate(mk::Game& game, float dt) override {
		acc += dt;

		auto input_x = (int) game.isKeyPressed(mk::input::KEY::D)
		             - (int) game.isKeyPressed(mk::input::KEY::A);
		auto input_y = (int) game.isKeyPressed(mk::input::KEY::S)
		             - (int) game.isKeyPressed(mk::input::KEY::W);
		if (input_x) dir = { input_x, 0 };
		if (input_y) dir = { 0, input_y };

		if (acc >= 1.f / ticks_per_second) {
			acc -= 1.f / ticks_per_second;
			auto head = getHead();
			auto back = std::move(rects.back());
			rects.pop_back();
			auto new_pos = (dir * size).type<float>() + head;
			const auto m = game.getWindowSize().x;
			new_pos += m;
			back->setPosition((new_pos.type<int>() % m).type<float>());
			rects.push_front(std::move(back));
		}
	}

	void onDraw(
		[[maybe_unused]] mk::RenderTarget& target,
		[[maybe_unused]] mk::DrawContext   context,
		[[maybe_unused]] const mk::Game&   game
	) const override {
		for (const auto& segment: rects) target.render(*segment, context);
	}
};

class Fruit: public mk::RectShape {
	const float size;
public:
	Fruit(usize size): mk::RectShape(mk::Colors::BLUE, size), size(size) {}

	void onReady(mk::Game& game) override {

	}
};

class World: public mk::WorldEntity2D {
	Player*             player;
	std::vector<Fruit*> fruits;

public:
	void onReady(mk::Game& game) override {
		player = addChild<Player>(game, game.getWindowSize().x / 20.f);
		fruits.push_back(addChild<Fruit>(game, game.getWindowSize().x / 20.f));
	}

	void onUpdate(mk::Game& game, float) override {
		if (game.isKeyJustPressed(mk::input::KEY::SPACE)) player->grow();
	}

	void fruitEaten(Fruit& fruit) {}
};

int main() {
	mk::Game game("settings.json");
	game.addScene<World>();
	game.run();
}
