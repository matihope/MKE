#include "MKE/Color.hpp"
#include "MKE/Input.hpp"
#include "MKE/Math/Vector.hpp"
#include "MKE/Nodes/2d/RectShape.hpp"
#include "MKE/Nodes/GUI/Alignments.hpp"
#include "MKE/Nodes/GUI/Label.hpp"
#include "MKE/Primitives/2d/RectPrimitive.hpp"
#include "MKE/Random.hpp"
#include "MKE/WorldEntity.hpp"
#include "MKE/Game.hpp"

constexpr auto HEAD     = mk::Color(0, 204, 146);
constexpr auto BODY     = mk::Color(138, 162, 158);
constexpr auto FRUIT    = mk::Color(219, 84, 97);
constexpr auto DEATH    = mk::Color(241, 237, 238);
constexpr auto BG_COLOR = mk::Color(104, 105, 99);

class Player: public mk::WorldEntity2D {
	usize ticks_per_second = 6;
	float acc              = 0.f;
	bool  dead             = false;

	mk::gui::Label* death_text;

	mk::math::Vector2i dir;

	std::list<std::unique_ptr<mk::RectPrimitive>> rects;

	const float size;

	void readInput(mk::Game& game) {
		auto input_x = (int) game.isKeyPressed(mk::input::KEY::D)
		             - (int) game.isKeyPressed(mk::input::KEY::A);
		auto input_y = (int) game.isKeyPressed(mk::input::KEY::S)
		             - (int) game.isKeyPressed(mk::input::KEY::W);
		if (input_x && dir.x != -input_x) dir = { input_x, 0 };
		if (input_y && dir.y != -input_y) dir = { 0, input_y };
	}

	auto getHead() { return rects.front()->getPosition2D(); }

	void handleDeath() {
		dead = true;
		death_text->show();
	}

	void tick(mk::Game& game) {
		if (dir == 0) return;

	auto       head_pos = getHead();
		auto       new_pos  = (dir * size).type<float>() + head_pos;
		const auto m        = game.getWindowSize().x;
		new_pos += m;
		auto next_position = (new_pos.type<int>() % m).type<float>();
		if (checkCollisions((next_position), true)) {
			handleDeath();
		} else {
			rects.front()->setColor(BODY);

			auto new_head_block = std::move(rects.back());
			rects.pop_back();
			new_head_block->setColor(HEAD);
			new_head_block->setPosition(next_position);
			rects.push_front(std::move(new_head_block));
		}
	}

public:
	Player(float size): size(size) {}

	bool checkCollisions(mk::math::Vector2f pos, bool drop_back = false) {
		for (const auto& seg: rects | std::views::reverse | std::views::drop(drop_back))
			if (seg->getPosition2D() == pos) return true;
		return false;
	}

	void onReady(mk::Game& game) override {
		dir = mk::math::Vector2i(0, 0);
		rects.push_back(std::make_unique<mk::RectPrimitive>(size));
		rects.back()->setColor(HEAD);

		death_text = addChild<mk::gui::Label>(game, game.getDefaultFont(), "you died :C");
		death_text->setAlignment(mk::gui::HAlignment::MIDDLE, mk::gui::VAlignment::CENTER);
		death_text->setPosition(game.getWindowSize().type<float>() / 2.f);
		death_text->setTextSize(64);
		death_text->setColor(DEATH);
		death_text->hide();
	}

	void grow() {
		rects.push_back(std::make_unique<mk::RectPrimitive>(size));
		rects.back()->setPosition(getHead());
	}

	void onUpdate(mk::Game& game, float dt) override {
		acc += dt;
		readInput(game);

		if (!dead && acc >= 1.f / ticks_per_second) {
			acc -= 1.f / ticks_per_second;
			tick(game);
		}
	}

	void onDraw(
		[[maybe_unused]] mk::RenderTarget& target,
		[[maybe_unused]] mk::DrawContext   context,
		[[maybe_unused]] const mk::Game&   game
	) const override {
		for (const auto& segment: rects | std::views::reverse) target.render(*segment, context);
	}
};

class Fruit: public mk::RectShape {
	const float size;
	Player*     player;

	void respawn(mk::Game& game) {
		mk::math::Vector2f new_pos;
		do {
			new_pos
				= mk::math::Vector2i(
					  mk::Random::getInt(0, static_cast<int>(game.getWindowSize().x / size) - 1),
					  mk::Random::getInt(0, static_cast<int>(game.getWindowSize().y / size) - 1)
				  )
			          .type<float>()
			    * size;
		} while (player->checkCollisions(new_pos));

		setPosition(new_pos);
	}

public:
	Fruit(usize size, Player* player): mk::RectShape(FRUIT, size), size(size), player(player) {}

	void onReady(mk::Game& game) override { respawn(game); }

	void onUpdate(mk::Game& game, float) override {
		if (player->checkCollisions(getPosition2D())) {
			respawn(game);
			player->grow();
		}
	}
};

class World: public mk::WorldEntity2D {
public:
	void onReady(mk::Game& game) override {
		auto size = game.getWindowSize().x / 20.f;
		addChild<Fruit>(game, size, addChild<Player>(game, size));
	}
};

int main() {
	mk::Game game("settings.json");
	game.setClearColor(BG_COLOR);
	mk::Random::initRandom();
	game.addScene<World>();
	game.run();
}
