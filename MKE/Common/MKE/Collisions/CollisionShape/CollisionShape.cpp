#include "CollisionShape.hpp"
#include "MKE/DrawContext.hpp"
#include "MKE/Primitives/2d/RectPrimitive.hpp"

namespace mk {

	// CircleCollision::CircleCollision(WorldEntity* parent): CollisionComponent(parent) {
	// 	setRadius(5.f);
	// }

	// CircleCollision::CircleCollision(WorldEntity* parent, const float radius):
	// 	  CollisionComponent(parent) {
	// 	setRadius(radius);
	// }

	// void CircleCollision::setRadius(const float radius) { m_radius = radius; }

	// float CircleCollision::getRadius() const { return m_radius; }

	// void CircleCollision::draw(RenderTarget& target, DrawContext context) const {
	// 	CollisionComponent::draw(target, context);

	// 	sf::CircleShape shape(m_radius);
	// 	shape.setFillColor(m_fill_color);
	// 	shape.setOutlineColor(m_outline_color);
	// 	target.draw(shape, states);
	// }

	// bool CircleCollision::contains(const math::Vector2f& point) const {
	// 	auto diff = math::Vector2f(getPosition()) - point;
	// 	return diff.lengthSquared() <= m_radius * m_radius;
	// }

	// RECT

	RectCollision::RectCollision(WorldEntity* parent): CollisionComponent(parent) {
		m_parent = parent;
		setSize(5.f, 5.f);
	}

	RectCollision::RectCollision(WorldEntity* parent, const float width, const float height):
		  CollisionComponent(parent) {
		setSize(width, height);
	}

	void RectCollision::setSize(const float width, const float height) {
		m_size.x = width;
		m_size.y = height;
	}

	const math::Vector2f& RectCollision::getSize() const { return m_size; }

	void RectCollision::draw(RenderTarget& target, DrawContext context) const {
		if (!shouldDraw()) return;
		context.transform *= getTransform();
		RectPrimitive shape(m_size);
		shape.setColor(m_fill_color);
		target.render(shape, context);
	}

	bool RectCollision::contains(const math::Vector2f& point) const {
		auto pos = m_parent->getGlobalTransform() ^ getPosition();
		if (point.x < pos.x) return false;
		if (point.x > pos.x + m_size.x) return false;
		if (point.y < pos.y) return false;
		if (point.y > pos.y + m_size.y) return false;
		return true;
	}
}  // namespace mk
