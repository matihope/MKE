#include "MKE/Math/Geometry.hpp"
#include "MKE/Math/Matrix.hpp"
#include "MKE/Transformable.hpp"
#include "MKE/Math/Vector.hpp"
#include <list>
#include <variant>
#include <MKE/VariantMatch.hpp>

namespace physics {

	namespace collision {
		// class CollisionShape {
		// public:
		// 	virtual const std::vector<mk::math::Vector2f>& getVertices() const = 0;
		// 	virtual ~CollisionShape()                                          = default;
		// };

		struct Rectangle {
			mk::math::Vector2f size;
		};

		struct Circle {
			float radius;
		};

		struct CollisionShape2D {
			std::variant<Rectangle, Circle> shape = Rectangle({ 1.f });

			std::optional<mk::math::CollisionInfo> collidesWith(
				const mk::math::Matrix4f& me_transform,
				const CollisionShape2D&   he,
				const mk::math::Matrix4f& he_transform
			) const {
				if (&he == this) return std::nullopt;
				variant_match(shape) {
					variant_case(Rectangle, me_rect) {
						variant_match(he.shape) {
							variant_case(Rectangle, he_rect) {
								std::vector<mk::math::Vector2f> me_shape = {
									me_transform ^ mk::math::Vector2f{ 0 },
									me_transform ^ mk::math::Vector2f{ me_rect.size.x, 0 },
									me_transform
									    ^ mk::math::Vector2f{ me_rect.size.x, me_rect.size.y },
									me_transform ^ mk::math::Vector2f{ 0, me_rect.size.y },
								};
								std::vector<mk::math::Vector2f> he_shape = {
									he_transform ^ mk::math::Vector2f{ 0 },
									he_transform ^ mk::math::Vector2f{ he_rect.size.x, 0 },
									he_transform
									    ^ mk::math::Vector2f{ he_rect.size.x, he_rect.size.y },
									he_transform ^ mk::math::Vector2f{ 0, he_rect.size.y },
								};
								return mk::math::getCollision(me_shape, he_shape);
							}
							variant_case(Circle, he_circle) { MK_PANIC("Not handled yet!"); }
						}
					}
					variant_case(Circle, me_circle) {
						variant_match(he.shape) {
							variant_case(Rectangle, he_rect) { MK_PANIC("Not handled yet!"); }
							variant_case(Circle, he_circle) {
								const auto me_at = me_transform ^ mk::math::Vector2f{ 0 };
								const auto he_at = he_transform ^ mk::math::Vector2f{ 0 };
								const mk::math::Vector2f diff = he_at - me_at;
								const float total_radius      = me_circle.radius + he_circle.radius;
								if (diff.lengthSquared() > total_radius * total_radius)
									return std::nullopt;
								return { { (me_at - he_at).normalizeOrZero(),
								           total_radius - diff.length() } };
							}
						}
					}
				}
			}
		};
	}

	namespace body {

		class PhysicsBody2D: public mk::Transformable {
			bool should_free = false;


			collision::CollisionShape2D collision_shape;

		public:
			PhysicsBody2D(collision::CollisionShape2D collision_shape):
				  collision_shape(std::move(collision_shape)) {}

			void queueFree() { should_free = true; }

			bool shouldFree() const { return should_free; }

			const collision::CollisionShape2D& getCollisionShape() const { return collision_shape; }
		};

		class StaticBody2D: public PhysicsBody2D {
		public:
			using PhysicsBody2D::PhysicsBody2D;
		};

		class KinematicBody2D: public PhysicsBody2D {
		public:
			using PhysicsBody2D::PhysicsBody2D;

			mk::math::Vector2f velocity;
		};

		/**
		 * @brief Manually stepped version of KinematicBody by "move_and_slide"
		 */
		class CharacterBody: public KinematicBody2D {};
	}

	class World {
		std::list<std::unique_ptr<body::StaticBody2D>>    static_bodies;
		std::list<std::unique_ptr<body::KinematicBody2D>> kinematic_bodies;

	public:
		template<class T>
		T* addBody(std::unique_ptr<T> body) {
			if constexpr (std::is_same_v<T, body::StaticBody2D>) {
				static_bodies.push_back(std::move(body));
				return static_bodies.back().get();
			} else if constexpr (std::is_same_v<T, body::KinematicBody2D>) {
				kinematic_bodies.push_back(std::move(body));
				return kinematic_bodies.back().get();
			} else {
				static_assert(false, "Not yet supported");
			}
		}

		void step(float dt, usize sub_steps = 4) {
			for (std::unique_ptr<body::KinematicBody2D>& kinematic_body: kinematic_bodies)
				for (usize i = 0; i < sub_steps; i++) {
					kinematic_body->move(kinematic_body->velocity / sub_steps);
					for (const auto& static_body: static_bodies)
						if (auto collision_info = kinematic_body->getCollisionShape().collidesWith(
								kinematic_body->getTransform(),
								static_body->getCollisionShape(),
								static_body->getTransform()
							)) {
							kinematic_body->move(collision_info->normal * collision_info->depth);
							kinematic_body->velocity
							    -= collision_info->normal
							     * mk::math::dotProduct(
									   collision_info->normal, kinematic_body->velocity
								 );
						}
				}
		}
	};
}
