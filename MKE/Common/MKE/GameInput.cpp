#include "GameInput.hpp"
#include "MKE/Input.hpp"

using namespace mk;

bool GameInput::isKeyPressed(mk::input::KEY key) const { return pressed_key[(usize) key]; }

bool GameInput::isKeyJustPressed(input::KEY key) const {
	usize idx = (usize) key;
	return pressed_key[idx] && !prev_pressed_key[idx];
}

bool GameInput::isMousePressed(input::MOUSE key) const { return pressed_mouse[(usize) key]; }

bool GameInput::isMouseJustPressed(input::MOUSE key) const {
	usize idx = (usize) key;
	return pressed_mouse[idx] && !prev_pressed_mouse[idx];
}

void mk::GameInput::tick() {
	prev_pressed_key = pressed_key;
	prev_pressed_mouse = pressed_mouse;
}

void mk::GameInput::handleEvent(const Event& event) {
	if (auto ev = event.get<Event::KeyPressed>(); ev)
		pressed_key[(usize) ev->key] = true;
	else if (auto ev = event.get<Event::KeyReleased>(); ev)
		pressed_key[(usize) ev->key] = false;
	else if (auto ev = event.get<Event::MouseButtonPressed>(); ev)
		pressed_mouse[(usize) ev->button] = true;
	else if (auto ev = event.get<Event::MouseButtonReleased>(); ev)
		pressed_mouse[(usize) ev->button] = false;
}

bool mk::GameInput::isKeyJustReleased(input::KEY key) const {
	usize idx = (usize) key;
	return !pressed_key[idx] && prev_pressed_key[idx];
}

bool mk::GameInput::isMouseJustReleased(input::MOUSE key) const {
	usize idx = (usize) key;
	return !pressed_mouse[idx] && prev_pressed_mouse[idx];
}
