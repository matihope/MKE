#include "MKE/Clock.hpp"
#include "MKE/Event.hpp"
#include "MKE/Input.hpp"
#include "MKE/Primitives/2d/CirclePrimitive.hpp"
#include "MKE/Primitives/2d/RectPrimitive.hpp"
#include "MKE/RenderWindow.hpp"
#include "MKE/Transformable.hpp"
#include <MKE/Math/Vector.hpp>
#include <MKE/Math/Math.hpp>
#include <iostream>
#include <list>
#include <variant>


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

int main() {
	// auto w = theory::World({ 20, 20 });
	// w.print(std::cout);
    mk::RenderWindow window(800, 600, "Test");
    window.enableVerticalSync(true);

    mk::RectPrimitive rect(mk::math::Vector2f{50.f, 50.f});
    rect.setPosition(50, 80);
    mk::CirclePrimitive circle(50.f);
    circle.setPosition(400, 300);

    mk::Clock fps_clock;
    float     fps_sum   = 0.f;
    int       fps_count = 0;

    while(!window.isExitRequested()) {
        auto x = static_cast<int>(window.isKeyPressed(mk::input::KEY::D)) - window.isKeyPressed(mk::input::KEY::A);
        auto y = static_cast<int>(window.isKeyPressed(mk::input::KEY::S)) - window.isKeyPressed(mk::input::KEY::W);
        circle.move(mk::math::Vector2f(x, y).normalizeOrZero() * 5);
        window.clear(mk::Color(21, 21, 21));
        window.render(rect, mk::DrawContext(window.getCurrentView2D().getTransform()));
        window.render(circle, mk::DrawContext(window.getCurrentView2D().getTransform()));
        window.display();

        float dt = fps_clock.restart();
        ++fps_count;
        fps_sum += dt;
        if (fps_sum >= 1.f) {
            std::cout << "FPS: " << fps_count << '\n';
            fps_count = 0;
            fps_sum   = 0.f;
        }
    }
}
