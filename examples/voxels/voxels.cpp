#include "World.hpp"


#include <MKE/Game.hpp>

/*
 * TODO:
 * - Infinite world
 * - menu to adjust FOV, RENDER DISTANCE, ETC.
 * - better terrain generation (like from a 3d function)
 * - Add menu
 * - Add trees
 * - Player UI (better inventory)
 * - Equipment/Survival
 * - Print coordinates
 */


int main() {
	mk::Game game("settings.json");
	initTextures(game);
	game.addScene<World>();
	game.run();
}
