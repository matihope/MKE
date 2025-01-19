#include "Menu.hpp"
#include "World.hpp"


#include <MKE/Game.hpp>

/*
 * TODO:
 * - Player UI (better inventory)
 * - better terrain generation (like from a 3d function)
 * - Add Trees
 * - Improve drawing speed
 *
 * - Fix translucency?
 * - Equipment/Survival
 * - Infinite world
 * - Add menu
 * - Add trees
 * - menu to adjust FOV, RENDER DISTANCE, ETC.
 */


int main() {
	mk::Game game("settings.json");
	initTextures(game);
	game.addScene<Menu>();
	game.run();
}
