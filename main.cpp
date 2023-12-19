/* Lab1 ~ Lab3测试程序 */

#include <cstdio>

#include "include/Game.h"
int main(int argc, char *argv[]){
	Game game;
	game.init(GAME_START);
	game.run();
	return 0;
}

