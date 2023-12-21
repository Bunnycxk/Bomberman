#include <cstdlib>
#include <thread>
#include <map>
#include "../include/Game.h"
#include "task.h"

std::string init_text[] = {"1 Player", "Create Room", "Join Room", "Game Over", "Press SELECT to menu"};

const int init_player_x[] = {MAP_LEFT + MAP_CELL_HALF, MAP_RIGHT - MAP_CELL_HALF};
const int init_player_y[] = {MAP_TOP + MAP_CELL_HALF, MAP_TOP + MAP_CELL_HALF};
Game::Game(/* args */){
	system("bluetoothctl --timeout 5 scan on &");
	fb_init(config.fb_filename.c_str());
	font_init(config.font_filename.c_str());

	// 游戏手柄初始化
    gamepad_fd = gamepad_init(config.gamepad_filename.c_str()); 
    task_add_file(gamepad_fd, this, &gamepad_event_cb);
	
	touch_fd = touch_init(config.touch_filename.c_str());
	task_add_file(touch_fd, this, &touch_event_cb);
	// 绑定绘图
    task_add_timer(1000/FPS, this, &draw); /*每一帧调用一次绘图*/

	fb_draw_rect(0,0,SCREEN_WIDTH,SCREEN_HEIGHT, COLOR_BLACK, NULL);
	fb_update();
	is_server = 0;
	player_num = 1 , max_player_num = 2;
	my_id = 0;

}

Game::~Game(){
}




/*
	场景初始化
*/
void Game::init(screen_type screen){

	this->screen = screen;
	// 清空物品堆
	object.clear();
	switch (screen)
	{
	case GAMING:
		{

			if (is_server) {
				task_delete_file(500);	 	// 不再监听文件。
				for (int i = 1; i < player_num; i++){
					bluetooth.send(bluetooth_fd[i], ("start : " + std::to_string(player_num) + '\n').c_str());
				}
					
			}
			// create a map
			config.loadmap();
			mp = std::make_shared<Map>(TYPE_MAP, MAP_LEFT, MAP_TOP, config.item_map["icon"], &config.cell[0][0]);
			object.insert(mp);
			
			// create a backgroud
			backgroud = std::make_shared<Backgroud>(TYPE_BACKGROUD, 0, 0, config.item_map["gaming_backgroud"]);
			object.insert(backgroud);
			printf("num = %d \n" , player_num);
			// create a role
			for (int i = 0; i < player_num; i++){
				player[i] = std::make_shared<Role>(TYPE_ROLE, init_player_x[i], init_player_y[i], config.item_map["role"], mp, 1, i);
				object.insert(player[i]);
			}


			if (is_server) task_add_timer(5000, this, &game_generate_probs); // 每 5s 生成一个道具

			time_to_over = FPS;
		}
		break;
	case GAME_START:
		{
			backgroud = std::make_shared<Backgroud>(TYPE_BACKGROUD, 0, 0, config.item_map["gamestart_backgroud"]);
			//backgroud->draw(NULL);
			object.insert(backgroud);

		}
		break;
	case GAME_MENU:
		{
			backgroud = std::make_shared<Backgroud>(TYPE_BACKGROUD, 0, 0, config.item_map["gamemenu_backgroud"]);
			object.insert(backgroud); 
			//backgroud->draw(NULL);
	

			backgroud->draw_rect(MENU_MOVE_LEFT - MAP_CELL_HALF, MENU_MOVE_TOP - MAP_CELL_HALF, MAP_CELL_SIZE, MAP_CELL_SIZE, COLOR_WHITE);
			backgroud->draw_rect(MENU_STOP_LEFT - MAP_CELL_HALF, MENU_STOP_TOP - MAP_CELL_HALF, MAP_CELL_SIZE, MAP_CELL_SIZE, COLOR_WHITE);
			auto temp_obj = std::make_shared<Role>(TYPE_ROLE, MENU_MOVE_LEFT, MENU_MOVE_TOP, config.item_map["role"], nullptr, 1, 0);
			temp_obj->set_action_type(ACTION_MOVE);
			object.insert(temp_obj);
			temp_obj = std::make_shared<Role>(TYPE_ROLE, MENU_STOP_LEFT, MENU_STOP_TOP, config.item_map["role"], nullptr, 1, 0);
			object.insert(temp_obj);
			

			object.insert(std::make_shared<Text>(TYPE_TEXT, MENU_CREATE_LEFT + 32, MENU_CREATE_TOP + 40,  "创建", 30, COLOR_WHITE, false));
			object.insert(std::make_shared<Text>(TYPE_TEXT, MENU_ROOM_LEFT   + 70, MENU_ROOM_LIST_TOP   + 8,  "房间列表", 30, COLOR_WHITE, false));
			object.insert(std::make_shared<Text>(TYPE_TEXT, MENU_ROOM_FLUSH_LEFT + 32, MENU_ROOM_FLUSH_TOP + 40,  "刷新", 30, COLOR_WHITE, false));

			room_flush();
			// fb_draw_rect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, COLOR_WHITE);
			// fb_update();
			// Create some text
			// int text_x = 400, text_y = 150;
			// for (int i = 0; i < TEXT_COUNT; i++){
			// 	auto now_text = std::make_shared<Text>(TYPE_TEXT, text_x, text_y, init_text[i], 50, COLOR_BLACK, i == 0);
			// 	text_y += 100;
			// 	all_text.push_back(now_text);
			// 	object.insert(now_text);
			// }
			// text_cur = 0;
		}
		break;
	case GAME_ROOM:
		{
			for (int i = 0; i < max_player_num; i++)
				join_player[i] = false;
			backgroud = std::make_shared<Backgroud>(TYPE_BACKGROUD, 0, 0, config.item_map["room_backgroud"]);
			object.insert(backgroud); 
			object.insert(std::make_shared<Text>(TYPE_TEXT, MENU_START_LEFT + 1, MENU_START_TOP + 8,  "开始游戏", 30, COLOR_WHITE, false));
		}
		break;
	case GAME_OVER:
		{
			backgroud = std::make_shared<Backgroud>(TYPE_BACKGROUD, 0, 0, config.item_map["lose_backgroud"]);
			object.insert(backgroud);
			system("ps -ef | grep 'rfcomm -r' | grep -v grep | awk '{print $2}' | xargs kill -9");

			// int text_x = 400, text_y = 250;
			// for (int i = TEXT_COUNT; i < TEXT_COUNT + 2; i++){
			// 	auto now_text = std::make_shared<Text>(TYPE_TEXT, text_x, text_y, init_text[i], 50, COLOR_BLACK, i == 0);
			// 	text_y += 100;
			// 	text_x -= 100;
			// 	object.insert(now_text);
			// }
		}
		break;
	default:
		break;
	}
    
	// bomb = std::make_shared<Object>(TYPE_BOMB, config.bomb, MAP_LEFT, MAP_TOP);
	// object.insert(bomb);
}

// void Game::create_map(){
// 	config.game_backgroud->set(mp, config.icon);
// }

screen_type Game::get_screen(){
	return screen;
}

int Game::get_room_cur(){
	return room_cur;
}

int Game::get_room_num(){
	return room_num;
}

void Game::set_room_cur(int data){
	room_cur = data;
}
void Game::run(){
    task_loop();
}


void Game::room_flush(){
	for (auto obj:all_room){
		object.erase(obj);
	}
	all_room.clear();
	bluetooth.scan();
	int text_x = MENU_ROOM_LEFT, text_y = MENU_ROOM_TOP + 32;
	room_num = 0;
	for (auto info: bluetooth.room){
		auto now_room = std::make_shared<Text>(TYPE_TEXT, text_x + 10, text_y + 8,  info.name, 30, COLOR_WHITE, false);
		all_room.push_back(now_room);
		object.insert(now_room);
		text_y += 64;
		room_num ++;
		if (room_num == 4) break;
	}
}	
// void Game::switch_to_text(direction_type type){
// 	all_text[text_cur]->set_on_flag(false);
// 	text_cur = (text_cur + type + TEXT_COUNT) % TEXT_COUNT;
// 	all_text[text_cur]->set_on_flag(true);
// }


void Game::switch_screen(screen_type scr){
	switch (screen)
	{
	case GAMING:
		/* code */
		if (is_server) task_delete_timer(5000);
		break;
	case GAME_MENU:
		if (is_server){
			bluetooth.listen(max_player_num - 1, "Bunny");
			task_add_timer(500, this, &listen_rfcomm_file);
		}else {
			bluetooth.connect(room_cur);
			bluetooth_fd[0] = open("/dev/rfcomm0", O_RDWR|O_NOCTTY|O_NONBLOCK); /*非阻塞模式*/
			while (bluetooth_fd[0] < 0){
				bluetooth_fd[0] = open("/dev/rfcomm0", O_RDWR|O_NOCTTY|O_NONBLOCK);
			}
			task_add_file(bluetooth_fd[0], this, &bluetooth_event_cb);
			//myWrite_nonblock(fd, "hello from client\n", 6);
		}
		break;
	case GAME_ROOM:
		all_room.clear();
		break;
	default:
		break;
	}
	if (screen == GAMING) {
		
	}
	backgroud = nullptr;
	init(scr);
}

void Game::generate_probs(){
	// TO-DO 可以优化
	int num = 0, probs_num = 0;
	for (int i = 0, id = 0; i < MAP_ROW; i++)
	for (int j = 0; j < MAP_COLUMN; j++, id++)
		if (mp->cell[id] == MAP_EMPTY) num++;else
		if (mp->cell[id] >= MAP_PROBS_HEALTH && mp->cell[id] <= MAP_PROBS_ATTACK) probs_num++;
	
	if (num == 0 || probs_num >= 5) return;
	int rand_num = (rand() % num) + 1;
	for (int i = 0, id = 0; i < MAP_ROW; i++)
	for (int j = 0; j < MAP_COLUMN; j++, id++)
	if (mp->cell[id] == MAP_EMPTY && --rand_num == 0) {
		int type = rand() % 100;
		if (type <= 30) type = 0;else // 30% 是加速
		if (type <= 60) type = 1;else // 30% 是回复HP
		if (type <= 80) type = 2;else // 20% 是加一个炸弹
		if (type <= 90) type = 3;else // 10% 是 push
			type = 4;				  // 10% 是 len++

		mp->set_probs(i, j, type);
		for (int k = 1; k < player_num; k++)
		bluetooth.send(bluetooth_fd[k],("probs : " + std::to_string(i) + " " + std::to_string(j) + " " + std::to_string(type)).c_str());
		return;
	}
	return;
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

	if (game->get_screen() == GAMING){
		int alive_num = game->player_num;
		for (int i = 0; i < game->player_num; i++)
		if (game->player[i] == nullptr) alive_num --;
		if (alive_num <= 1 && alive_num < game->player_num){
			if (--game->time_to_over == 0){
				game->switch_screen(GAME_OVER);
				return;
			}
		}
	}

	
	//printf("---a new frame---\n");
	myTime start_time = task_get_time();

	for (auto it = game->object.begin(); it != game->object.end();){
		//(*it)->print_info();
		// if (obj->obj_type == TYPE_ROLE) obj->move(game->mp);
		auto status = (*it)->draw(&game->config.cell[0][0]);
		switch (status)
		{
		case DELETE:
			if ((*it)->get_obj_type() == TYPE_ROLE){
				for (int i = 0; i < game->player_num; i++)
					if (game->player[i]->get_obj_id() == (*it)->get_obj_id()) {
						game->player[i] = nullptr;
					}
			}
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
	// 					it = game->object.erase(ibombt);
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
	// printf("cost time: %d\n", end_time - start_time);
	// printf("------the frame end---------\n\n");
}


static int lastx[5],lasty[5];
void touch_event_cb(int fd, Game *game){
	
	int type,x,y,finger;
	type = touch_read(fd, &x,&y,&finger);
	switch (game->get_screen()){
		case GAME_START:
			game->switch_screen(GAME_MENU);
			break;
		case GAME_MENU:
			switch(type){
			case TOUCH_PRESS:
				printf("TOUCH_PRESS：x=%d,y=%d,finger=%d\n",x,y,finger);
				
				if (MENU_ICON_FLUSH_LEFT <= x && x<= MENU_ICON_FLUSH_RIGHT && MENU_ICON_FLUSH_TOP <= y && y <= MENU_ICON_FLUSH_BOTTOM){
					game->backgroud->draw_rect(MENU_ICON_LEFT, MENU_ICON_TOP, MENU_ICON_RIGHT - MENU_ICON_LEFT, MENU_ICON_BOTTTOM - MENU_ICON_TOP, COLOR_WHITE);
					fb_update();
				}else 
				if (MENU_ICON_LEFT < x - 7 && x + 7 < MENU_ICON_RIGHT && MENU_ICON_TOP < y - 7 && y + 7 < MENU_ICON_BOTTTOM){
					lastx[finger] = x, lasty[finger] = y;
					game->backgroud->draw_track(x, y, COLOR_BLACK);
					fb_update();
				}else
				if (MENU_CREATE_LEFT <= x && x<= MENU_CREATE_RIGHT && MENU_CREATE_TOP <= y && y <= MENU_CREATE_BOTTOM){
					game->is_server = true;
					game->my_id = 0;
					game->switch_screen(GAME_ROOM);		
				}else
				if (MENU_ROOM_FLUSH_LEFT <= x && x <= MENU_ROOM_FLUSH_RIGHT && MENU_ROOM_FLUSH_TOP <= y && y <= MENU_ROOM_FLUSH_BOTTOM){
					game->room_flush();
					//printf("here!\n");
				}else				 
				if (MENU_ROOM_LEFT <= x && x<= MENU_ROOM_LEFT + MENU_ROOM_WIDTH && MENU_ROOM_TOP <= y && y <= MENU_ROOM_TOP + game->get_room_num() * MENU_ROOM_HEIGHT){
					game->is_server = false;
					game->set_room_cur((y - MENU_ROOM_TOP) / MENU_ROOM_HEIGHT);
					printf("room cur = %d\n",game->get_room_cur());
					game->switch_screen(GAME_ROOM);
				}
				break;
			case TOUCH_MOVE:
				printf("TOUCH_MOVE：x=%d,y=%d,finger=%d\n",x,y,finger);
				if ((MENU_ICON_LEFT < lastx[finger] - 7 && lastx[finger] + 7 < MENU_ICON_RIGHT && MENU_ICON_TOP < lasty[finger] - 7 && lasty[finger] + 7 < MENU_ICON_BOTTTOM) 
				&&  (MENU_ICON_LEFT < x - 7 && x + 7 < MENU_ICON_RIGHT && MENU_ICON_TOP < y - 7 && y + 7 < MENU_ICON_BOTTTOM)){
					game->backgroud->draw_track(x, y, lastx[finger], lasty[finger], COLOR_BLACK);
					fb_update();
					lastx[finger] = x, lasty[finger] = y;
				}
				
				break;
			case TOUCH_RELEASE:
				printf("TOUCH_RELEASE：x=%d,y=%d,finger=%d\n",x,y,finger);
				break;
			case TOUCH_ERROR:
				printf("close touch fd\n");
				close(fd);
				task_delete_file(fd);
				break;
			default:
				return;
			}
		break;
	case GAME_ROOM:
		switch (type)
		{
		case TOUCH_PRESS:
			
			if (MENU_START_LEFT <= x && x<= MENU_START_RIGHT && MENU_START_TOP <= y && y <= MENU_START_BOTTOM){
				if (game->is_server) game->switch_screen(GAMING);else return;
			}
			break;
		
		default:
			break;
		}
		break;
	default:
		break;
	}
	
	return;
}

void gamepad_event_cb(int fd, Game *game){
	int type;
	type = gamepad_read(fd);
	printf("%d\n",type);
	switch (game->get_screen())
	{
	case GAME_START:
		{
			switch (type)
			{
			default:
				game->switch_screen(GAME_MENU);
				break;
			}
		}
		break;
	case GAME_MENU:
		/* code */
		{
			// switch (type)
			// {
			// case GAMEPAD_UP_BUTTON_TOUCH:
			// 	/* code */
			// 	game->switch_to_text(DIRECTION_UP);
			// 	break;
			// case GAMEPAD_DOWN_BUTTON_TOUCH:
			// 	game->switch_to_text(DIRECTION_DOWN);
			// 	break;
			// case GAMEPAD_SELECT_TOUCH:
			// 	switch (game->get_text_cur()){
			// 		case ONE_PLAYER:
			// 			game->switch_screen(GAMING);
			// 			break;
			// 		case CREATE_ROOM:
			// 			game->switch_screen(GAME_CREATE_ROOM);
			// 			break;
			// 		case JOIN_ROOM:
			// 			game->switch_screen(GAME_JOIN_ROOM);
			// 			break;
			// 		default:
			// 			break;
			// 	}
			// 	break;
			// default:
			// 	break;
			// }
		}

		break;
	
	case GAMING:
		if (game->player[game->my_id] == nullptr) return;
		switch (type)
		{
		case GAMEPAD_LEFT_BUTTON_TOUCH:
			printf("left touch !\n");
			game->player[game->my_id]->set_move(-1, 0);
			if (game->is_server) {
				for (int i = 1; i < game->player_num; i++)
					game->bluetooth.send(game->bluetooth_fd[i], ("mvleft : " + std::to_string(game->my_id)).c_str());
			}else{
				game->bluetooth.send(game->bluetooth_fd[0], ("mvleft : " + std::to_string(game->my_id)).c_str());
			}
			//role_set_direction(man, -1, 0);
			break;
		case GAMEPAD_RIGHT_BUTTON_TOUCH:
			printf("right touch !\n");
			game->player[game->my_id]->set_move(1, 0);
			if (game->is_server) {
				for (int i = 1; i < game->player_num; i++)
					game->bluetooth.send(game->bluetooth_fd[i], ("mvright : " + std::to_string(game->my_id)).c_str());
			}else{
				game->bluetooth.send(game->bluetooth_fd[0], ("mvright : " + std::to_string(game->my_id)).c_str());
			}
			//role_set_direction(man, 1, 0);
			break;
		case GAMEPAD_LEFT_BUTTON_RELEASE:
			printf("left release !\n");
			game->player[game->my_id]->set_move(0, 0);
			if (game->is_server) {
				for (int i = 1; i < game->player_num; i++)
					game->bluetooth.send(game->bluetooth_fd[i], ("stop : " + std::to_string(game->my_id)).c_str());
			}else{
				game->bluetooth.send(game->bluetooth_fd[0], ("stop : " + std::to_string(game->my_id)).c_str());
			}
			//role_set_direction(man, 0, 0);
			break;
		case GAMEPAD_RIGHT_BUTTON_RELEASE:
			printf("right release !\n");
			game->player[game->my_id]->set_move(0, 0);
			if (game->is_server) {
				for (int i = 1; i < game->player_num; i++)
					game->bluetooth.send(game->bluetooth_fd[i], ("stop : " + std::to_string(game->my_id)).c_str());
			}else{
				game->bluetooth.send(game->bluetooth_fd[0], ("stop : " + std::to_string(game->my_id)).c_str());
			}
			//role_set_direction(man, 0, 0);
			break;
		case GAMEPAD_UP_BUTTON_TOUCH:
			printf("up touch !\n");
			game->player[game->my_id]->set_move(0, -1);
			if (game->is_server) {
				for (int i = 1; i < game->player_num; i++)
					game->bluetooth.send(game->bluetooth_fd[i], ("mvup : " + std::to_string(game->my_id)).c_str());
			}else{
				game->bluetooth.send(game->bluetooth_fd[0], ("mvup : " + std::to_string(game->my_id)).c_str());
			}
			//role_set_direction(man, 0, -1);
			break;
		case GAMEPAD_DOWN_BUTTON_TOUCH:
			printf("down touch !\n");
			game->player[game->my_id]->set_move(0, 1);
			if (game->is_server) {
				for (int i = 1; i < game->player_num; i++)
					game->bluetooth.send(game->bluetooth_fd[i], ("mvdown : " + std::to_string(game->my_id)).c_str());
			}else{
				game->bluetooth.send(game->bluetooth_fd[0], ("mvdown : " + std::to_string(game->my_id)).c_str());
			}
			//role_set_direction(man, 0, 1);
			break;
		case GAMEPAD_UP_BUTTON_RELEASE:
			printf("up release !\n");
			game->player[game->my_id]->set_move(0, 0);
			if (game->is_server) {
				for (int i = 1; i < game->player_num; i++)
					game->bluetooth.send(game->bluetooth_fd[i], ("stop : " + std::to_string(game->my_id)).c_str());
			}else{
				game->bluetooth.send(game->bluetooth_fd[0], ("stop : " + std::to_string(game->my_id)).c_str());
			}
			//role_set_direction(man, 0, 0);
			break;
		case GAMEPAD_DOWN_BUTTON_RELEASE:
			printf("down release !\n");
			game->player[game->my_id]->set_move(0, 0);
			if (game->is_server) {
				for (int i = 1; i < game->player_num; i++)
					game->bluetooth.send(game->bluetooth_fd[i], ("stop : " + std::to_string(game->my_id)).c_str());				
			}else{
				game->bluetooth.send(game->bluetooth_fd[0], ("stop : " + std::to_string(game->my_id)).c_str());

			}
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
				auto new_bomb = game->player[game->my_id]->set_bomb(game->mp, game->config.item_map["bomb"]);
				if (new_bomb){	// 成功放置
					game->object.insert(new_bomb);
					if (game->is_server) {
						for (int i = 1; i < game->player_num; i++)
							game->bluetooth.send(game->bluetooth_fd[i], ("bomb : " + std::to_string(game->my_id)).c_str());
					}else{
						game->bluetooth.send(game->bluetooth_fd[0], ("bomb : " + std::to_string(game->my_id)).c_str());
					}
				}
				
			}
			break;
		default:
			break;
		}
		break;
	case GAME_OVER:
		switch (type)
		{		
		default:
			game->switch_screen(GAME_START);
			break;
		}
		break;
	default:
		break;
	}
	
	return;
}

void game_generate_probs(int fd, Game *game){
	game->generate_probs();
}

void listen_rfcomm_file(int fd, Game *game){
	// to-do
	for (int i = 0; i < game->max_player_num - 1; i++)
	if (game->join_player[i] == false){
		std::string url = "/dev/rfcomm" + std::to_string(i);
		int fd = open(url.c_str(), O_RDWR|O_NOCTTY|O_NONBLOCK);
		if(fd < 0){
			continue;
		}else {
			
			task_add_file(fd, game, &bluetooth_event_cb);
			game->bluetooth.send(fd, ("id : " + std::to_string(game->player_num)).c_str());
			game->bluetooth_fd[game->player_num++] = fd;
			game->join_player[i] = true;
			printf("listen here and find rfcomm\n");
		}
	}
}

void bluetooth_event_cb(int fd, Game *game){
	char buf[128];
	int n;
	n = myRead_nonblock(fd, buf, sizeof(buf)-1);
	// TO-DO， 没想到好的解析方法，可能要考虑使用JSON，先暴力解析。
	if(n <= 0) {
		printf("close bluetooth tty fd\n");
		//task_delete_file(fd);
		//close(fd);
		exit(0);
		return;
	}
	buf[n] = '\0';
	printf("bluetooth tty receive \"%s\"\n", buf);
	if (buf[0] == 'i' && buf[1] == 'd'){	//id : id
		game->my_id = buf[5] - 48;
		printf("my_id = %d\n", game->my_id);
	}else 
	if (buf[0] == 's' && buf[1] == 't' && buf[2] == 'a'){ //start : num
		game->player_num = buf[8] - 48;
		printf("num = %d\n", game->player_num);
		if (!game->is_server) game->switch_screen(GAMING);
	}else								 
	if (buf[0] == 'm' && buf[1] == 'v'){ //mvleft : id
		int i = 0;
		if (buf[2] == 'l'){
			i = buf[9] - 48;			
			game->player[i]->set_move(-1, 0);
		}else
		if (buf[2] == 'r'){//mvright : id
			i = buf[10] - 48;
			game->player[i]->set_move(1, 0);
		}else
		if (buf[2] == 'u'){//mvup : id
			i = buf[7] - 48;
			game->player[i]->set_move(0, -1);
		}else
		if (buf[2] == 'd'){
			i = buf[9] - 48;
			game->player[i]->set_move(0, 1);
		}
		if (game->is_server){
			for (int j = 1; j < game->player_num; j++)
			if (i!=j){	
				game->bluetooth.send(game->bluetooth_fd[j], buf);
			}
		}
	}else 
	if (buf[0] == 's' && buf[1] == 't'){ //stop : 
		int i = buf[7] - 48;
		game->player[i]->set_move(0, 0);
	}else
	if (buf[0] == 'b' && buf[1] == 'o'){//bomb : id
		int i = buf[7] - 48;
		auto new_bomb = game->player[i]->set_bomb(game->mp, game->config.item_map["bomb"]);
		if (new_bomb){	// 成功放置
			game->object.insert(new_bomb);
		}
	}else
	if (buf[0] == 'p' && buf[1] == 'r'){ // probs: i j type
		int i, j, type;
		sscanf(buf, "probs : %d %d %d\n", &i, &j, &type);
		game->mp->set_probs(i, j, type);
	}
	
//	fb_draw_text(2, pen_y, buf, 24, COLOR_TEXT); fb_update();
//	pen_y += 30;
	return;
}