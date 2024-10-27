#include "MKE/Init.hpp"
#include "MKE/RenderWindow.hpp"
#include <MKE/Game.hpp>

int main() {
	mk::init::init();
	mk::Game game("settings.json");
	game.run();
}
