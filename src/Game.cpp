#include "../include/Game.h"
#include "../include/task.h"
Game::Game(/* args */){
}

Game::~Game(){
}

void Game::init(){
    
    fb_init("/dev/fb0");
	font_init("./resource/font.ttc");

	// 初始化 Object
	object.insert(Object(TYPE_BACKGROUD, config.game_backgroud));

	// 游戏手柄初始化
    gamepad_fd = gamepad_init("/dev/input/event3"); 
    task_add_file(gamepad_fd, this, &gamepad_event_cb);

	// 绑定绘图
    task_add_timer(1000/FPS, this, &draw); /*每一帧调用一次绘图*/

	
}



void Game::run(){
    task_loop();
}



/*
    画图函数，负责将所有Object画出
	由于静态成员函数不能访问非静态成员，所以把Game*传给这个函数，所以需要魔改一下task.c

    另外 draw 其实不需要参数fd，但是 c 和 c++ 不一样
    c 没有重载函数，所以一个函数名对应一个地址
    当给定的函数的形参和函数指针定义时的不一样时不会出错
    但是 c++ 可以重载，因此必须给一个没有用的形参给他，主要是懒得改底层给的task.c了
*/

void draw(int fb, Game *game){  
    printf("draw!\n");

	for (auto obj : game->object){
		// to-do
		obj.print_info();
	}

}

void gamepad_event_cb(int fd, Game *game){
	int type;
	type = gamepad_read(fd);
	printf("%d\n",type);
	switch (type)
	{
	case GAMEPAD_LEFT_BUTTON_TOUCH:
		printf("left touch !\n");
		//role_set_direction(man, -1, 0);
		break;
	case GAMEPAD_RIGHT_BUTTON_TOUCH:
		printf("right touch !\n");
		//role_set_direction(man, 1, 0);
		break;
	case GAMEPAD_LEFT_BUTTON_RELEASE:
		printf("left release !\n");
		//role_set_direction(man, 0, 0);
		break;
	case GAMEPAD_RIGHT_BUTTON_RELEASE:
		printf("right release !\n");
		//role_set_direction(man, 0, 0);
		break;
	case GAMEPAD_UP_BUTTON_TOUCH:
		printf("up touch !\n");
		//role_set_direction(man, 0, -1);
		break;
	case GAMEPAD_DOWN_BUTTON_TOUCH:
		printf("down touch !\n");
		//role_set_direction(man, 0, 1);
		break;
	case GAMEPAD_UP_BUTTON_RELEASE:
		printf("up release !\n");
		//role_set_direction(man, 0, 0);
		break;
	case GAMEPAD_DOWN_BUTTON_RELEASE:
		printf("down release !\n");
		//role_set_direction(man, 0, 0);
		break;
	case GAMEPAD_ERROR:
		printf("close GAMEPAD fd\n");
		close(fd);
		task_delete_file(fd);
		break;
	case GAMEPAD_A_TOUCH:
		printf("button A touch!\n");
		//set_bomb(man);
		break;
	default:
		break;
	}
	return;
}