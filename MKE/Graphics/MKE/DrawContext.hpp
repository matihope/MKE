#pragma once

#include "MKE/Math/Matrix.hpp"
#include "MKE/Shader.hpp"
#include "MKE/Texture.hpp"

namespace mk {
	struct DrawContext {
		DrawContext() = default;

		explicit DrawContext(const Shader* shader);
		explicit DrawContext(const Texture* texture);

		explicit DrawContext(const math::Matrix4f& camera);

		virtual void bind();

		math::Matrix4f transform{};
		math::Matrix4f camera{};

		const Shader*  shader  = nullptr;
		const Texture* texture = nullptr;
	};
}
