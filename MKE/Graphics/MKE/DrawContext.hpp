#pragma once

#include "MKE/Math/Matrix.hpp"
#include "MKE/Shader.hpp"
#include "MKE/Texture.hpp"

namespace mk {
	class RenderTarget;

	// A shader-specification class - for a specific shader specification (input variables)
	class DrawContext {
		friend class RenderTarget;

	public:
		DrawContext() = default;

		DrawContext(const Shader* shader): shader(shader) {}

		virtual void setShader(const Shader* shader) { this->shader = shader; }

		virtual ~DrawContext() = 0;

		virtual void bind() { Shader::use(shader); }

	protected:
		const Shader* shader = nullptr;
	};

	inline DrawContext::~DrawContext() {}

	class DrawContext2D final: public DrawContext {
	public:
		DrawContext2D();
		DrawContext2D(const Texture& texture);

		~DrawContext2D() = default;

		math::Matrix4f transform;

		void setShader(const Shader* shader) override;

		void bind() override {
			DrawContext::bind();
			Texture::bind(texture);
			shader->setMatrix4f("transform", transform);
		}

	private:
		const Texture* texture = nullptr;
	};
}
