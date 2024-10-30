#pragma once

namespace mk {
	class RenderTarget;
	struct DrawContext;

	class Drawable {
	public:
		virtual ~Drawable() = 0;

		virtual void draw(RenderTarget& target, DrawContext context) const = 0;
	};

	inline Drawable::~Drawable() {}
}
