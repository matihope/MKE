#include "World.hpp"


#include <MKE/Game.hpp>

/*
 * TODO:
 * - Add sky/color
 * - Collisions
 * - Equipment/Survival
 * - better terrain generation
 */


int main() {
	mk::Game game("settings.json");
	initTextures(game);
	game.addScene<World>();
	game.run();
}
