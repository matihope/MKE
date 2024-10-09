#pragma once

#include "MKE/Math/Matrix.hpp"
#include "MKE/Shader.hpp"
#include "MKE/Texture.hpp"

namespace mk {
	struct DrawContext {
		DrawContext() = default;

		DrawContext(Shader* shader);

		DrawContext(Texture* texture);

		virtual void bind();

		math::Matrix4f transform{};
		math::Matrix4f camera{};

		Shader*  shader  = nullptr;
		Texture* texture = nullptr;
	};
}
