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

class World: public mk::WorldEntity2D {
	mk::RectShape*                  rect1;
	physics::body::StaticBody2D*    body1;
	mk::RectShape*                  rect2;
	physics::body::KinematicBody2D* body2;

	physics::World world;

public:
	void onReady(mk::Game& game) override {
		rect1 = addChild<mk::RectShape>(game, mk::Colors::WHITE, mk::math::Vector2f{ 50.f, 50.f });
		body1 = world.addBody(
			std::make_unique<physics::body::StaticBody2D>(
				physics::collision::CollisionShape2D(physics::collision::Rectangle({ 50.f, 50.f }))
			)
		);
		body1->setPosition(0, 0);

		rect2 = addChild<mk::RectShape>(game, mk::Colors::RED, mk::math::Vector2f{ 50.f, 50.f });
		body2 = world.addBody(
			std::make_unique<physics::body::KinematicBody2D>(
				physics::collision::CollisionShape2D(physics::collision::Rectangle({ 50.f, 50.f }))
			)
		);
		body2->setPosition(0, 25);
	}

	void onPhysicsUpdate(mk::Game& game, float dt) override {
		auto x = static_cast<int>(game.isKeyPressed(mk::input::KEY::D))
		       - game.isKeyPressed(mk::input::KEY::A);
		auto y = static_cast<int>(game.isKeyPressed(mk::input::KEY::S))
		       - game.isKeyPressed(mk::input::KEY::W);

		auto r = static_cast<int>(game.isKeyPressed(mk::input::KEY::E))
		       - game.isKeyPressed(mk::input::KEY::Q);

		body2->velocity = mk::math::Vector2f(x, y).normalizeOrZero() * 50 * dt;
		body2->rotate(r * dt * 10);
		world.step(dt);
		rect1->setPosition(body1->getPosition());
		rect2->setPosition(body2->getPosition());
		rect2->setRotation(body2.getRotation());
	}
};

int main() {
	// auto w = theory::World({ 20, 20 });
	// w.print(std::cout);

	mk::Game game("settings.json");
	game.addScene<World>();
	game.run();
}
