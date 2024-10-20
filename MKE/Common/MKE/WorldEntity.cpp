#include "WorldEntity.hpp"
#include "MKE/DrawContext.hpp"
#include "MKE/RenderTarget.hpp"

namespace mk {
	EntityID detail::id_counter() {
		static EntityID newest_id{};
		return newest_id++;
	}

	void WorldEntity2D::draw2d(const RenderTarget2D& target, DrawContext context) const {
		if (m_show) {
			DrawContext copied_context(context);
			copied_context.transform *= getTransform();
			onDraw2d(target, context);

			for (const auto& layer: m_entity_pool)
				for (auto& entity: layer.second) target.render2dContext(*entity, copied_context);
		}
	}

	void WorldEntity3D::draw3d(const RenderTarget3D& target, DrawContext context) const {
		if (m_show) {
			DrawContext copied_context(context);
			copied_context.transform *= getTransform();
			onDraw3d(target, context);

			for (const auto& layer: m_entity_pool)
				for (auto& entity: layer.second) target.render3dContext(*entity, copied_context);
		}
	}
}  // namespace mk
