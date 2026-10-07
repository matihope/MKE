#include "MKE/Clock.hpp"
#include "MKE/Color.hpp"
#include "MKE/Event.hpp"
#include "MKE/Game.hpp"
#include "MKE/Input.hpp"
#include "MKE/Nodes/2d/RectShape.hpp"
#include "MKE/Primitives/2d/CirclePrimitive.hpp"
#include "MKE/Primitives/2d/RectPrimitive.hpp"
#include "MKE/RenderWindow.hpp"
#include "MKE/Transformable.hpp"
#include <MKE/Math/Vector.hpp>
#include <MKE/Math/Math.hpp>
#include <iostream>
#include <list>
#include <variant>
#include "MKE/WorldEntity.hpp"
#include "physics/physics.hpp"

namespace theory {
	struct Wall {
		enum class Kind { Rock, Forest } kind = Kind::Rock;
	};

	struct Ground {
		enum class Kind { Dirt, Rock, Water, Grass } kind = Kind::Rock;
	};

	using Material = std::variant<Wall, Ground>;

	class Tank {};

	class World {
		mk::math::Vector2u    size;
		float                 unit_length;
		std::vector<Material> world;

	public:
		World(mk::math::Vector2u size): size(size) {
			world.resize(size.x * size.y, Ground(Ground::Kind::Grass));
		}

		const Material& getMaterial(mk::math::Vector2u pos) const {
			MK_ASSERT(pos.x < size.x);
			MK_ASSERT(pos.y < size.y);
			return world[pos.y * size.x + pos.x];
		}

		Material& getMaterial(mk::math::Vector2u pos) {
			MK_ASSERT(pos.x < size.x);
			MK_ASSERT(pos.y < size.y);
			return world[pos.y * size.x + pos.x];
		}

		void print(std::ostream& out) {
			out << "+";
			for (u32 x = 0; x < size.x * 2 + 1; x++) out << "-";
			out << "+\n";
			for (u32 y = 0; y < size.y; y++) {
				out << "|";
				for (u32 x = 0; x < size.x; x++) {
					if (x == 0) out << " ";
					const Material& m = getMaterial({ x, y });
					if (const Wall* wall = std::get_if<Wall>(&m)) {
						switch (wall->kind) {
						case Wall::Kind::Rock:
							out << "R";
							break;
						case Wall::Kind::Forest:
							out << "F";
							break;
						}
					} else if (const Ground* ground = std::get_if<Ground>(&m)) {
						switch (ground->kind) {
						case Ground::Kind::Dirt:
							out << "d";
							break;
						case Ground::Kind::Rock:
							out << "r";
							break;
						case Ground::Kind::Water:
							out << "w";
							break;
						case Ground::Kind::Grass:
							out << "g";
							break;
						}
					}
					out << " ";
				}

				out << "|\n";
			}
			out << "+";
			for (u32 x = 0; x < size.x * 2 + 1; x++) out << "-";
			out << "+\n";
		}
	};
}

class Tank: public mk::WorldEntity2D {
	physics::World&                 world;
	physics::body::KinematicBody2D* physics_body;
	mk::RectShape*                  visual_body;
	mk::Color                       color;
	mk::math::Vector2f              size;

	mk::input::KEY forward;
	mk::input::KEY backward;
	mk::input::KEY right;
	mk::input::KEY left;

public:
	Tank(
		physics::World&    world,
		mk::Color          color,
		mk::math::Vector2f size,
		mk::input::KEY     forward,
		mk::input::KEY     backward,
		mk::input::KEY     right,
		mk::input::KEY     left
	):
		  world(world),
		  color(color),
		  size(size),
		  forward(forward),
		  backward(backward),
		  right(right),
		  left(left) {}

	void onReady(mk::Game& game) override {
		visual_body = addChild<mk::RectShape>(game, color, size);
		visual_body->setOrigin(size / 2.f);
		physics_body = world.addBody(
			std::make_unique<physics::body::KinematicBody2D>(
				physics::collision::CollisionShape2D(physics::collision::Rectangle(size))
			)
		);
		physics_body->getCollisionShape().setOrigin(size / 2);
		physics_body->setPosition(400, 300);
	}

	void onUpdate(mk::Game&, float) override {
		setPosition(physics_body->getPosition());
		setRotation(physics_body->getRotation());
	}

	void onPhysicsUpdate(mk::Game& game, float) override {
		auto fw = static_cast<int>(game.isKeyPressed(forward)) - game.isKeyPressed(backward);
		auto rl = static_cast<int>(game.isKeyPressed(right)) - game.isKeyPressed(left);

		physics_body->velocity = mk::math::Vector2f(rl, fw).normalizeOrZero() * 100;
	}
};

class World: public mk::WorldEntity2D {
	std::vector<Tank*> tanks;

	physics::World world;

public:
	void onReady(mk::Game& game) override {
		tanks.push_back(
			addChild<Tank>(
				game,
				world,
				mk::Colors::WHITE,
				mk::math::Vector2f{ 50.f, 70.f },
				mk::input::KEY::W,
				mk::input::KEY::S,
				mk::input::KEY::D,
				mk::input::KEY::A
			)
		);
		tanks.back()->setPosition({ 100.f, 100.f });
		tanks.push_back(
			addChild<Tank>(
				game,
				world,
				mk::Colors::RED,
				mk::math::Vector2f{ 50.f, 70.f },
				mk::input::KEY::ARROW_UP,
				mk::input::KEY::ARROW_DOWN,
				mk::input::KEY::ARROW_RIGHT,
				mk::input::KEY::ARROW_LEFT
			)
		);
		tanks.back()->setPosition({ 200.f, 200.f });
	}

	void onPhysicsUpdate(mk::Game&, float dt) override { world.step(dt); }
};

int main() {
	// auto w = theory::World({ 20, 20 });
	// w.print(std::cout);

	mk::Game game("settings.json");
	game.addScene<World>();
	game.run();
}
