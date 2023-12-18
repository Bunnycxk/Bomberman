#include "../include/Game.h"
#include "task.h"

Game::Game(/* args */){

}

Game::~Game(){
}

void Game::init(){
	
    fb_init(config.fb_filename.c_str());
	font_init(config.font_filename.c_str());

	// create a map
	config.loadmap();
	mp = std::make_shared<Map>(TYPE_MAP, MAP_LEFT, MAP_TOP, config.icon, &config.cell[0][0]);
	object.insert(mp);
	// mp = std::make_shared<Object>(config.map_filename.c_str());
	// mp_obj = std::make_shared<Object>(TYPE_MAP, config.icon, MAP_LEFT, MAP_TOP, mp);
	// mp->set_item(mp_obj->item);
	// object.insert(mp_obj);
	
	// create a backgroud
	backgroud = std::make_shared<Backgroud>(TYPE_BACKGROUD, 0, 0, config.game_backgroud);
	object.insert(backgroud);

	// printf("%d\n",object.size());
	// for (auto obj : object){
	// 	obj->print_info();
	// 	//obj->draw();
	// }

	// create a role
	player = std::make_shared<Role>(TYPE_ROLE, MAP_LEFT + MAP_CELL_HALF, MAP_TOP + MAP_CELL_HALF, config.role, 1);
	object.insert(player);

	// bomb = std::make_shared<Object>(TYPE_BOMB, config.bomb, MAP_LEFT, MAP_TOP);
	// object.insert(bomb);

	// 游戏手柄初始化
    gamepad_fd = gamepad_init(config.gamepad_filename.c_str()); 
    task_add_file(gamepad_fd, this, &gamepad_event_cb);

	// 绑定绘图
    task_add_timer(1000/FPS, this, &draw); /*每一帧调用一次绘图*/

	fb_draw_rect(0,0,SCREEN_WIDTH,SCREEN_HEIGHT,FB_COLOR(0,0,0));
	fb_update();
}

// void Game::create_map(){
// 	config.game_backgroud->set(mp, config.icon);
// }

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
	printf("---a new frame---\n");
	myTime start_time = task_get_time();

	for (auto it = game->object.begin(); it != game->object.end();){
		(*it)->print_info();
		// if (obj->obj_type == TYPE_ROLE) obj->move(game->mp);
		auto status = (*it)->draw(&game->config.cell[0][0]);
		switch (status)
		{
		case DELETE:
			it = game->object.erase(it);
			break;
		
		default:
			it++;
			break;
		}
	}

	// for (auto it = game->object.begin(); it != game->object.end();){
	// 	if ((*it)->get_TTL() == 0){
	// 		switch ((*it)->obj_type)
	// 		{
	// 		case TYPE_BOMB:
	// 			{
	// 				uint x = get_map_x((*it)->postion_x), y = get_map_y((*it)->postion_y); 
	// 				game->mp->cell[y][x] = 0;
	// 				if ((*it)->act_type == ACTION_STOP){
	// 					(*it)->time_to_live = 2 * FPS/10;  // 爆炸 0.2s
	// 					(*it)->set_action_type(ACTION_BOMB_CENTER);
	// 				}else {
	// 					it = game->object.erase(it);
	// 				}
	// 			}
	// 			break;
				
	// 		default:
	// 			break;
	// 		}
	// 	}else {
	// 		it++;
	// 	}
	// }
	fb_update();
	myTime end_time = task_get_time();
	printf("cost time: %d\n", end_time - start_time);
	printf("------the frame end---------\n\n");
}

void gamepad_event_cb(int fd, Game *game){
	int type;
	type = gamepad_read(fd);
	printf("%d\n",type);
	switch (type)
	{
	case GAMEPAD_LEFT_BUTTON_TOUCH:
		printf("left touch !\n");
		game->player->set_move(-1, 0, 8);
		//role_set_direction(man, -1, 0);
		break;
	case GAMEPAD_RIGHT_BUTTON_TOUCH:
		printf("right touch !\n");
		game->player->set_move(1, 0, 8);
		//role_set_direction(man, 1, 0);
		break;
	case GAMEPAD_LEFT_BUTTON_RELEASE:
		printf("left release !\n");
		game->player->set_move(0, 0, 8);
		//role_set_direction(man, 0, 0);
		break;
	case GAMEPAD_RIGHT_BUTTON_RELEASE:
		printf("right release !\n");
		game->player->set_move(0, 0, 8);
		//role_set_direction(man, 0, 0);
		break;
	case GAMEPAD_UP_BUTTON_TOUCH:
		printf("up touch !\n");
		game->player->set_move(0, -1, 8);
		//role_set_direction(man, 0, -1);
		break;
	case GAMEPAD_DOWN_BUTTON_TOUCH:
		printf("down touch !\n");
		game->player->set_move(0, 1, 8);
		//role_set_direction(man, 0, 1);
		break;
	case GAMEPAD_UP_BUTTON_RELEASE:
		printf("up release !\n");
		game->player->set_move(0, 0, 8);
		//role_set_direction(man, 0, 0);
		break;
	case GAMEPAD_DOWN_BUTTON_RELEASE:
		printf("down release !\n");
		game->player->set_move(0, 0, 8);
		//role_set_direction(man, 0, 0);
		break;
	case GAMEPAD_ERROR:
		printf("close GAMEPAD fd\n");
		close(fd);
		task_delete_file(fd);
		break;
	case GAMEPAD_A_TOUCH:
		printf("button A touch!\n");
		{
			auto new_bomb = game->player->set_bomb(game->mp, game->config.bomb);
			if (new_bomb){	// 成功放置
				game->object.insert(new_bomb);
			}
		}
		break;
	default:
		break;
	}
	return;
}