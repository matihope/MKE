#pragma once

namespace mk {
	class RenderTarget;
	struct DrawContext;

	class Drawable {
		friend class RenderTarget;


		virtual void draw(RenderTarget& target, DrawContext context) const = 0;
	public:
		virtual ~Drawable() = 0;
	};

	inline Drawable::~Drawable() {}
}
