#pragma once

namespace mk {
	class RenderTarget;
	class DrawContext2D;

	// Interface drawable, very nice
	class Drawable {
	public:
		virtual ~Drawable() = 0;

		virtual void beginDraw(const RenderTarget& target) const = 0;
	};

	inline Drawable::~Drawable() {}

	class Drawable2D: public Drawable {
	public:
		virtual ~Drawable2D() = 0;

		void beginDraw(const RenderTarget& target) const override;

		virtual void draw(const RenderTarget& target, DrawContext2D context) const = 0;
	};
}
