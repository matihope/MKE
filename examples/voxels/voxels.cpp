#include "World.hpp"


#include <MKE/Game.hpp>

/*
 * TODO:
 * - Collisions
 * - Player UI (better inventor)
 * - Equipment/Survival
 * - Fog of war
 * - Print coordinates
 * - better terrain generation
 *
 * TODO 2:
 * - Add sky/color (DONE)
 */


int main() {
	mk::Game game("settings.json");
	game.setClearColor(mk::Color(182,242,243));
	initTextures(game);
	game.addScene<World>();
	game.run();
}
