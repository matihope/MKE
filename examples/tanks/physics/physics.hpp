#include "MKE/Transformable.hpp"
#include "MKE/Math/Vector.hpp"
#include <list>

namespace physics {

	namespace collision {
		class CollisionShape {};

		struct Rectangle: public CollisionShape {
			mk::math::Vector2f size;
		};

		struct Circle: public CollisionShape {
			float radius;
		};
	}

	namespace body {

		class PhysicsBody: public mk::Transformable {
			bool should_free = false;

			void queueFree() { should_free = true; }

			bool shouldFree() const { return should_free; }

			std::unique_ptr<collision::CollisionShape> collision_shape;

            collision::CollisionShape& getCollisionShape() { return *collision_shape; }
            const collision::CollisionShape& getCollisionShape() const { return *collision_shape; }
		public:
            PhysicsBody(std::unique_ptr<collision::CollisionShape> collision_shape):
				  collision_shape(std::move(collision_shape)) {}
		};

		class StaticBody: public PhysicsBody {
		public:
		};

		class KinematicBody: public PhysicsBody {
			mk::math::Vector3f velocity;

		public:
		};
	}

	class World {
		std::list<std::unique_ptr<body::StaticBody>>    static_bodies;
		std::list<std::unique_ptr<body::KinematicBody>> kinematic_bodies;
        public:
	};
}
