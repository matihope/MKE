#pragma once

#include "MKE/Math/Matrix.hpp"
#include "MKE/Shader.hpp"
#include "MKE/Texture.hpp"
#include <functional>

namespace mk {
	class RenderTarget;

	// A shader-specification class - for a specific shader specification (input variables)
	class DrawContext {
		friend class RenderTarget;

	public:
		DrawContext() = default;

		DrawContext(const Shader* shader): shader(shader) {}

		void setShader(const Shader* shader) { this->shader = shader; }

		virtual ~DrawContext() = 0;

		virtual void bind() {
			Shader::use(shader);
		}

	protected:
		const Shader* shader = nullptr;
	};

	inline DrawContext::~DrawContext() {}

	class DrawContext2D: public DrawContext {
	public:
		DrawContext2D();

		~DrawContext2D() = default;

		math::Matrix4f                                 transform;
		std::optional<std::reference_wrapper<Texture>> texture;
	};
}
