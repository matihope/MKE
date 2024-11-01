#pragma once
#include "MKE/Collisions/CollisionComponent/CollisionComponent.hpp"
#include "MKE/Updatable.hpp"

namespace mk {
	class Clickable: public Updatable {
	public:
		enum class ClickMode { PressOnRelease, PressOnClick };

		virtual ~Clickable() = 0;
		[[nodiscard]]
		bool isPressed() const;
		void update(Game& game, float dt) override;
		void setClickCollisionShape(CollisionComponent* collision_shape);
		void setClickMode(ClickMode new_mode);

	protected:
		virtual void onHover() {}  // hovering mouse over m_collision_shape

		virtual void onStopHover() {}

		virtual void onHold() {}     // function called during holding

		virtual void onRelease() {}  // mouse is released over the m_collision_shape

		virtual void onPressed() {}  // callback

	private:
		ClickMode           click_mode      = ClickMode::PressOnRelease;
		CollisionComponent* collision_shape = nullptr;

		bool is_help{};
		bool is_pressable{};
		bool was_help_prev{};
		bool is_pressed{};

		void makePress();
	};
}  // namespace mk
