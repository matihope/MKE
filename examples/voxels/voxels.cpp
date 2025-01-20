#include "Menu.hpp"
#include <MKE/Game.hpp>

/*
 * TODO:
 * - Add Trees
 *
 * -
 * - Player UI (better inventory)
 * - Improve drawing speed
 *
 * - Fix translucency?
 * - Infinite world
 * - menu to adjust FOV, RENDER DISTANCE, ETC.
 */


int main() {
	mk::Game game("settings.json");
	initTextures(game);
	game.addScene<Menu>();
	game.run();
}
