#include "MKE/Color.hpp"
#include "MKE/Input.hpp"
#include "MKE/Ints.hpp"
#include "MKE/Math/Base.hpp"
#include "MKE/Math/Rect.hpp"
#include "MKE/Math/Vector.hpp"
#include "MKE/Random.hpp"
#include "MKE/WorldEntity.hpp"
#include "MKE/Nodes/2d/RectShape.hpp"
#include <MKE/Game.hpp>
#include <list>

namespace game {
	using namespace mk;

	namespace physics {

		class PhysicsEngine;

		class RectBody {
			friend class PhysicsEngine;

		private:
			PhysicsEngine& engine;
			u64            id;
			bool           dying = false;

			RectBody(PhysicsEngine& engine, u64 id, math::RectF rect):
				  engine(engine),
				  id(id),
				  my_rect(rect) {}

			math::RectF my_rect;

		public:
			math::Vector2f getPosition() const { return my_rect.getPosition(); }

			math::Vector2f getSize() const { return my_rect.getSize(); }

			void queueFree() { dying = true; }

			bool canMoveTo(math::RectF rect) const;

			/**
			 * @brief Moves an object along delta. Returns how much was moved. It's quite stable,
			 * although there's a looped float addition...
			 */
			math::Vector2f moveAndSlide(math::Vector2f delta) {
				const math::RectF initial = my_rect;

				math::RectF    helper       = my_rect;
				helper.left += delta.x;
				if (!canMoveTo(helper)) {
					auto lmb = [&](float mid) {
						helper.left   = initial.left + mid;
						bool can_move = canMoveTo(helper);
						if (math::sign(mid)) return can_move;
						return !can_move;
					};
					// auto diff = my_rect.getPosition().x - initial.getPosition().x;
					// delta.x   = std::min(std::abs(delta.x), std::abs(diff)) *
					// math::sign(delta.x);
					delta.x = math::binsearch(0.f, delta.x, lmb);
				}
				helper.left = initial.left + delta.x;
				helper.top += delta.y;
				if (!canMoveTo(helper)) {
					auto lmb = [&](float mid) {
						helper.top    = initial.top + mid;
						bool can_move = canMoveTo(helper);
						// return !can_move;
						if (math::sign(mid)) return can_move;
						return !can_move;
					};
					delta.y = math::binsearch(0.f, delta.y, lmb);
				}
				my_rect.left = initial.left + delta.x;
				my_rect.top  = initial.top + delta.y;
				return delta;
			}

			bool isOnFloor(math::Vector2f up) { return !getCollidingBodies(-up).empty(); }

			std::vector<const RectBody*> getCollidingBodies(math::Vector2f direction);
		};

		class PhysicsEngine {
		public:
			PhysicsEngine() = default;
			std::list<RectBody> bodies;
			u64                 body_counter = 0;

			void update() {
				for (auto it = bodies.begin(); it != bodies.end(); it++)
					if (it->dying) it = bodies.erase(it);
			}

			RectBody& requestRectBody(math::RectF init_rect) {
				bodies.push_back(RectBody(*this, body_counter++, init_rect));
				return bodies.back();
			}

			bool tryMoveTo(RectBody& body, math::RectF new_rect) {
				if (!canMoveTo(body, new_rect)) return false;
				body.my_rect = new_rect;
				return true;
			}

			bool canMoveTo(const RectBody& body, math::RectF new_rect) {
				for (auto& b: bodies) {
					if (b.id == body.id) continue;
					if (b.my_rect.overlaps(new_rect)) return false;
				}
				return true;
			}

			std::vector<const RectBody*>
				getCollidingBodies(RectBody& body, math::Vector2f direction) {
				// Direction shall be a unit vector

				std::vector<const RectBody*> bods;
				math::RectF                  test = body.my_rect;
				direction *= 1e-4;
				test.top += direction.y;
				test.left += direction.x;

				for (auto& b: bodies) {
					if (b.id == body.id) continue;
					if (b.my_rect.overlaps(test)) bods.push_back(&b);
				}
				return bods;
			}
		};

		bool RectBody::canMoveTo(math::RectF rect) const { return engine.canMoveTo(*this, rect); }

		std::vector<const RectBody*> RectBody::getCollidingBodies(math::Vector2f direction) {
			return engine.getCollidingBodies(*this, direction);
		}
	}

	Color getRandColor() {
		return { Random::getReal(0., 1.), Random::getReal(0., 1.), Random::getReal(0., 1.) };
	}

	class Level;

	class Player: public mk::WorldEntity2D {
	public:
		Player(physics::PhysicsEngine& engine, Level& level):
			  level(level),
			  my_body(engine.requestRectBody({ 50.f, 50.f, 15.f, 15.f })) {}

		void onReady(mk::Game& game) override {
			setPosition(my_body.getPosition());
			color  = getRandColor();
			sprite = addChild<RectShape>(game, color, my_body.getSize());
			game.resources().setTextureSmooth("face.png", false);
			sprite->setTexture(game.resources().getTexture("face.png"));
		}

		void onPhysicsUpdate(mk::Game& game, float dt) override;

		math::Vector2f moveAndSlide(math::Vector2f delta) {
			auto moved = my_body.moveAndSlide(delta);
			setPosition(my_body.getPosition());
			return moved;
		}

		void updateColor() {
			clock.restart();
			color = getRandColor();
			if (sprite) sprite->setColor(color);
		}

		void stepUpdateColor() {
			if (clock.getElapsedTime() > sec_reset_color) updateColor();
		}

	private:
		math::Vector2f speed;
		float          gravity    = 0.4f;
		float          jump_force = -10.f;

		RectShape*         sprite = nullptr;
		Level&             level;
		physics::RectBody& my_body;

		Color color;
		float sec_reset_color = 2.0f;
		Clock clock;
	};

	class Block: public WorldEntity3D {
		math::RectF pos;
		Color       color;

		RectShape* sprite = nullptr;

	public:
		Block(math::Vector2f position, usize size, Color color, physics::PhysicsEngine& engine):
			  pos(position.x, position.y, size, size),
			  color(color),
			  body(engine.requestRectBody(pos)) {}

		void onReady(mk::Game& game) override {
			sprite = addChild<RectShape>(game, color, pos.getSize());
			sprite->setPosition(pos.getPosition());
			sprite->setTexture(game.resources().getTexture("face.png"));
		}

		void setColor(Color new_color) {
			color = new_color;
			if (sprite) sprite->setColor(color);
		}

		physics::RectBody& body;
	};

	class Level: public WorldEntityUI {
	public:
		void onReady(mk::Game& game) override {
			player = addChild<Player>(game, engine, *this);

			initBlocks(game);
		}

		void initBlocks(mk::Game& game) {
			usize block_size = 25;
			for (int i = 0; i < game.getWindowSize().x / block_size; i++)
				blocks.push_back(addChild<Block>(
					game,
					math::Vector2f(i * block_size, game.getWindowSize().y - block_size),
					block_size,
					getRandColor(),
					engine
				));
			for (int i = 2; i < game.getWindowSize().x / block_size; i += 2)
				blocks.push_back(addChild<Block>(
					game,
					math::Vector2f(
						i * block_size, game.getWindowSize().y - block_size - i * block_size
					),
					block_size,
					getRandColor(),
					engine
				));
		}

		void touchedBodies(
			const std::vector<const physics::RectBody*>& bodies, Color new_color = getRandColor()
		) const {
			for (Block* b: blocks) {
				for (const auto bod: bodies)
					if (&b->body == bod) b->setColor(new_color);
			}
		}

		Player*                player = nullptr;
		physics::PhysicsEngine engine;
		std::vector<Block*>    blocks;
	};

	void Player::onPhysicsUpdate(mk::Game& game, float dt) {
		stepUpdateColor();

		float hsp
			= (i32) (game.isKeyPressed(mk::input::KEY::D)) - game.isKeyPressed(mk::input::KEY::A);
		hsp *= 5.f;

		speed.x = math::lerp(speed.x, hsp, dt * 10.f);

		bool falling  = speed.y > 0.f;
		bool going_up = speed.y < 0.f;
		speed.y += gravity;

		bool jumped       = false;
		auto bodies_below = my_body.getCollidingBodies({ 0.f, 1.f });
		if (game.isKeyPressed(mk::input::KEY::SPACE) && !bodies_below.empty()) {
			speed.y = jump_force;
			jumped  = true;
		}

		bool  spd_grv    = math::isZero(speed.y - gravity);
		float spd_before = speed.y;

		speed = moveAndSlide(speed);

		if ((falling && jumped)
		    || (falling && spd_before > speed.y && !spd_grv && spd_before > gravity)) {
			// This list of bodies is from a moment before `moveAndSlide`,
			// in particular it might be empty.
			level.touchedBodies(std::move(bodies_below), color);

			if (bodies_below.empty()) {
				auto&& new_bodies = my_body.getCollidingBodies({ 0.f, 10.f });
				level.touchedBodies(new_bodies, color);
			}
			if (!jumped) speed.y = -spd_before / 2.f;  // bounce
		}

		if (going_up && math::isZero(speed.y)) {
			auto&& bodies_up = my_body.getCollidingBodies({ 0.f, -1.f });
			level.touchedBodies(bodies_up, Colors::TRANSPARENT);
			updateColor();
		}
	}
}

int main() {
	mk::Game game("settings.json");
	game.addScene<game::Level>();
	game.run();
}
