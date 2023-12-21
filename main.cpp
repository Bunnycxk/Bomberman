/* Lab1 ~ Lab3测试程序 */

#include <cstdio>
#include <csignal>
#include "include/Game.h"

void signalHandler(int signum) {
    if (signum == SIGINT) {
        std::cout << "Received Ctrl+C - Hello!" << std::endl;
        // 可以在這裡添加額外的清理工作
		system("ps -ef | grep 'rfcomm -r' | grep -v grep | awk '{print $2}' | xargs kill -9");
        exit(signum);  // 結束程序
    }
}

int main(int argc, char *argv[]){
	signal(SIGINT, signalHandler);
	Game game;
	game.init(GAME_START);
	game.run();
	return 0;
}

