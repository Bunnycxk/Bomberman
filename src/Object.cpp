#include "../include/Object.h"

const int direction_dx[] = {0, 1,  0, -1};
const int direction_dy[] = {1, 0, -1,  0};
const action_type direction_type[2][4] = {
    {ACTION_BOMB_DOWN, ACTION_BOMB_RIGHT, ACTION_BOMB_UP, ACTION_BOMB_LEFT},
    {ACTION_BOMB_DOWN_END, ACTION_BOMB_RIGHT_END, ACTION_BOMB_UP_END, ACTION_BOMB_LEFT_END}
};
uint object_num = 0;

Object::Object(object_type type, Item *config_item, uint x, uint y, uint *bomb_num, uint len){
    id = object_num ++;
    frame_now = speed_cnt = 0;
    obj_type = type;
    item = config_item;
    switch (type)
    {
    case TYPE_BACKGROUD:
        priority = 0;
        postion_x = x, postion_y = y;
        time_to_live = UINT32_MAX;
        act_type = ACTION_STOP;
        break;
    case TYPE_ROLE: 
        priority = 2;
        postion_x = x, postion_y = y;
        time_to_live = UINT32_MAX;
        act_type = ACTION_STOP;
        max_bomb_num = 1, now_bomb_num = 0;
        bomb_len = len;
        break;
    case TYPE_BOMB:
        priority = 1;
        postion_x = x, postion_y = y;
        time_to_live = FPS; // 可以存活 1 s
        act_type = ACTION_STOP;
        player_bomb_num = bomb_num;
        bomb_len = len; 
        break;
    default:
        break;
    }

}

Object::~Object()
{
    printf("a object clean : \n");
    print_info();
    if (obj_type == TYPE_BOMB){
        (*player_bomb_num) --;
    }
}

void Object::set_action_type(action_type type){
    act_type = type;
    frame_now = speed_cnt = 0;
}

void Object::set_move(int dx, int dy, int speed){
    move_dx = dx, move_dy = dy, move_speed = speed;
    if (dx || dy){
        set_action_type(ACTION_MOVE);
    }else {
        set_action_type(ACTION_STOP);
    }
}

void Object::move(std::shared_ptr<Map> mp){
    if (move_dx || move_dy){
        uint x = postion_x + move_dx * move_speed, y = postion_y + move_dy * move_speed;
        // 超出地图边界，直接不走
        if (x < MAP_LEFT + MAP_CELL_HALF || x > MAP_RIGHT - MAP_CELL_HALF || y < MAP_TOP + MAP_CELL_HALF || y > MAP_BOTTOM - MAP_CELL_HALF) return;
        uint type;
        
        // 前进方向是碰撞物
        type = mp->cell[get_map_y(y + move_dy * (MAP_CELL_HALF - 2))][get_map_x(x + move_dx * (MAP_CELL_HALF - 2))];
        if (type == MAP_DESTRUCTIBLE || type == MAP_INDESTRUCTIBLE) return;
        
        if (move_dx) {
            // 所进行方向的斜下角，作位置修正
            type = mp->cell[get_map_y(y + (MAP_CELL_HALF - 2))][get_map_x(x + move_dx * (MAP_CELL_HALF - 2))];
            if (type == MAP_DESTRUCTIBLE || type == MAP_INDESTRUCTIBLE) {
                y = get_pixel_y(get_map_y(y)) + MAP_CELL_HALF; 
            }
            // 所进行方向的斜上角，作位置修正
            type = mp->cell[get_map_y(y - (MAP_CELL_HALF - 2))][get_map_x(x + move_dx * (MAP_CELL_HALF - 2))];
            if (type == MAP_DESTRUCTIBLE || type == MAP_INDESTRUCTIBLE) {
                y = get_pixel_y(get_map_y(y)) + MAP_CELL_HALF;
            }   
        }

        if (move_dy) {
            // 所进行方向的斜左方，作位置修正
            type = mp->cell[get_map_y(y + move_dy * (MAP_CELL_HALF - 2))][get_map_x(x - (MAP_CELL_HALF - 2))];
            if (type == MAP_DESTRUCTIBLE || type == MAP_INDESTRUCTIBLE) {
                x = get_pixel_x(get_map_x(x)) + MAP_CELL_HALF;
            }
            // 所进行方向的斜右方，作位置修正
            type = mp->cell[get_map_y(y + move_dy * (MAP_CELL_HALF - 2))][get_map_x(x + (MAP_CELL_HALF - 2))];
            if (type == MAP_DESTRUCTIBLE || type == MAP_INDESTRUCTIBLE) {
                x = get_pixel_x(get_map_x(x)) + MAP_CELL_HALF;
            }   
        }
        postion_x = x, postion_y = y;
    }
}

std::shared_ptr<Object> Object::set_bomb(std::shared_ptr<Map> mp, Item *bomb){
    std::shared_ptr<Object> bomb_ptr(nullptr);
    // 不是角色不放
    if (obj_type != TYPE_ROLE) return bomb_ptr;
    int x = get_map_x(postion_x), y = get_map_y(postion_y);
    // 不是空格子不放
    if (mp->cell[y][x] != MAP_EMPTY) return bomb_ptr;
    // 已经达到最大放置数
    if (now_bomb_num == max_bomb_num) return bomb_ptr;
    mp->cell[y][x] = MAP_BOMB;
    now_bomb_num ++;
    bomb_ptr = std::make_shared<Object>(TYPE_BOMB, bomb, get_pixel_x(x), get_pixel_y(y), &now_bomb_num, bomb_len);
    return bomb_ptr;
}

void Object::draw(std::shared_ptr<Map> mp){
    switch (obj_type)
    {
    case TYPE_ROLE:
        item->draw(postion_x - MAP_CELL_HALF, postion_y - MAP_CELL_HALF, act_type, frame_now, speed_cnt);
        break;
    case TYPE_BACKGROUD:
        item->draw(postion_x, postion_y, act_type, frame_now, speed_cnt);
        break;
    case TYPE_BOMB:
        time_to_live--;
        switch (act_type)
        {
        case ACTION_STOP:
            item->draw(postion_x, postion_y, act_type, frame_now, speed_cnt);
            break;
        case ACTION_BOMB_CENTER:
            {
                item->draw(postion_x, postion_y, ACTION_BOMB_CENTER, frame_now, speed_cnt);
                int x = get_map_x(postion_x), y = get_map_y(postion_y);
                printf("map_row = %d map_column = %d\n",y,x);
                for (int j = bomb_len; j > 0; j--){
                    for (int i = 0; i < 4; i++){
                        int new_x = x + direction_dx[i], new_y = y + direction_dy[i];
                        if (new_x < 0 || new_x >= MAP_COLUMN || new_y < 0 || new_y >= MAP_ROW) continue;
                        if (mp->cell[new_y][new_x] == MAP_INDESTRUCTIBLE) continue;
                        item->draw(get_pixel_x(new_x), get_pixel_y(new_y), direction_type[j == 1][i], frame_now, speed_cnt);
                    }
                }

            }
            break;
        default:
            break;
        }
        
        break;
    default:
        break;
    }   
}

void Object::print_info(){
    switch (obj_type)
    {
    case TYPE_ROLE:
        printf("role :");
        break;
    case TYPE_BACKGROUD:
        printf("backgroud :");
        break;
    case TYPE_BOMB:
        printf("bomb :");
    default:
        break;
    }
    printf("obj_id = %u x = %u y = %u TTL = %u \n", id, postion_x, postion_y, time_to_live);
}