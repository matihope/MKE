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
			bool           queue_free = false;

			RectBody(PhysicsEngine& engine, u64 id, math::RectF rect):
				  engine(engine),
				  id(id),
				  my_rect(rect) {}

			math::RectF my_rect;

		public:
			math::Vector2f getPosition() const { return my_rect.getPosition(); }

			math::Vector2f getSize() const { return my_rect.getSize(); }

			void queueFree() { queue_free = true; }

			bool tryRect(math::RectF rect);

			math::Vector2f moveAndSlide(const math::Vector2f delta) {
				math::RectF initial = my_rect;

				math::RectF    helper    = my_rect;
				math::Vector2f new_delta = delta;
				helper.left += new_delta.x;
				if (!tryRect(helper)) {
					helper.left = my_rect.left;
					while (!math::isZero(new_delta.x)) {
						helper.left = my_rect.left + new_delta.x;
						tryRect(helper);
						new_delta.x /= 2;
					}
				}
				helper = my_rect;
				helper.top += new_delta.y;
				if (!tryRect(helper)) {
					helper.top = my_rect.top;
					while (!math::isZero(new_delta.y)) {
						helper.top = my_rect.top + new_delta.y;
						tryRect(helper);
						new_delta.y /= 2;
					}
				}
				auto diff = my_rect.getPosition() - initial.getPosition();
				diff.x    = std::min(std::abs(delta.x), std::abs(diff.x)) * math::sign(delta.x);
				diff.y    = std::min(std::abs(delta.y), std::abs(diff.y)) * math::sign(delta.y);
				return diff;
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
					if (it->queue_free) it = bodies.erase(it);
			}

			RectBody& requestRectBody(math::RectF init_rect) {
				bodies.push_back(RectBody(*this, body_counter++, init_rect));
				return bodies.back();
			}

			bool tryMoveTo(RectBody& body, math::RectF new_rect) {
				for (auto& b: bodies) {
					if (b.id == body.id) continue;
					if (b.my_rect.overlaps(new_rect)) return false;
				}
				body.my_rect = new_rect;
				return true;
			}

			std::vector<const RectBody*>
				getCollidingBodies(RectBody& body, math::Vector2f direction) {
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

		bool RectBody::tryRect(math::RectF rect) { return engine.tryMoveTo(*this, rect); }

		std::vector<const RectBody*> RectBody::getCollidingBodies(math::Vector2f direction) {
			return engine.getCollidingBodies(*this, direction);
		}
	}

	class Level;

	class Player: public mk::WorldEntity2D {
	public:
		Player(physics::PhysicsEngine& engine, Level& level):
			  level(level),
			  my_body(engine.requestRectBody({ 0.f, 0.f, 10.f, 10.f })) {}

		void onReady(mk::Game& game) override {
			my_body.moveAndSlide({ 50.f, 50.f });
			setPosition(my_body.getPosition());
			sprite = addChild<RectShape>(game, Colors::GREEN, my_body.getSize());
		}

		void onPhysicsUpdate(mk::Game& game, float dt) override;

		math::Vector2f moveAndSlide(math::Vector2f delta) {
			auto moved = my_body.moveAndSlide(delta);
			setPosition(my_body.getPosition());
			return moved;
		}

	private:
		math::Vector2f speed;
		float          gravity    = 0.4f;
		float          jump_force = -10.f;

		RectShape*         sprite = nullptr;
		Level&             level;
		physics::RectBody& my_body;
	};

	class Block: public WorldEntity3D {
	public:
		Block(math::Vector2f position, usize size, Color color, physics::PhysicsEngine& engine):
			  pos(position.x, position.y, size, size),
			  color(color),
			  body(engine.requestRectBody(pos)) {}

		void onReady(mk::Game& game) override {
			sprite = addChild<RectShape>(game, color, pos.getSize());
			sprite->setPosition(pos.getPosition());
		}

		math::RectF pos;
		Color       color;

		RectShape*         sprite = nullptr;
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

		void touchedBodies(const std::vector<const physics::RectBody*>& bodies) {
			for (Block* b: blocks) {
				for (auto bod: bodies) {
					if (&b->body == bod) {
						auto color = getRandColor();
						b->color   = color;
						b->sprite->setColor(color);
					}
				}
			}
		}

		Color getRandColor() {
			return { Random::getReal(0., 1.), Random::getReal(0., 1.), Random::getReal(0., 1.) };
		}

		Player*                player = nullptr;
		physics::PhysicsEngine engine;
		std::vector<Block*>    blocks;
	};

	void Player::onPhysicsUpdate(mk::Game& game, float dt) {
		float hsp
			= (i32) (game.isKeyPressed(mk::input::KEY::D)) - game.isKeyPressed(mk::input::KEY::A);
		hsp *= 5.f;

		speed.x = math::lerp(speed.x, hsp, dt * 10.f);

		bool falling = speed.y > 0.f;
		speed.y += gravity;

		bool jumped = false;
		if (game.isKeyPressed(mk::input::KEY::SPACE) && my_body.isOnFloor({ 0.f, -1.f })) {
			speed.y = jump_force;
			jumped  = true;
		}

		bool spd_grv = math::isZero(speed.y - gravity);

		auto colliding_bodies = my_body.getCollidingBodies({ 0.f, 1.f });
		speed                 = moveAndSlide(speed);

		if ((falling && jumped) || (!spd_grv && math::isZero(speed.y)))
			level.touchedBodies(std::move(colliding_bodies));
	}
}

int main() {
	mk::Game game("settings.json");
	game.addScene<game::Level>();
	game.run();
}
