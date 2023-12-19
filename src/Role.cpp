#include "../include/Role.h"

const int direction_dx[] = {0, 1,  0, -1, 0};
const int direction_dy[] = {1, 0, -1,  0, 0};

Role::Role(object_type type, int x, int y, std::shared_ptr<Item> init_item, uint len): Object(type, x, y){
    item = init_item;
    set_priority(100);
    set_TTL(UINT32_MAX);
    max_bomb_num = 100, now_bomb_num = 0;
    bomb_len = len;
    health_points = max_health_points = 100;
    act_type = ACTION_STOP;
    frame_now = speed_cnt = 0;
    no_attack_time = 0;
}

void Role::set_action_type(action_type type){
    act_type = type;
    frame_now = speed_cnt = 0;
}

void Role::set_move(int dx, int dy, int speed){
    move_dx = dx, move_dy = dy, move_speed = speed;
    if (dx || dy){
        set_action_type(ACTION_MOVE);
    }else {
        set_action_type(ACTION_STOP);
    }
}

std::shared_ptr<Bomb> Role::set_bomb(std::shared_ptr<Map> mp, std::shared_ptr<Item> bomb){
    std::shared_ptr<Bomb> bomb_ptr(nullptr);
    int x = get_map_x(get_x()), y = get_map_y(get_y());
    // 不是空格子不放
    if (mp->get_type(y, x) != MAP_EMPTY) return bomb_ptr;
    // 已经达到最大放置数
    if (now_bomb_num == max_bomb_num) return bomb_ptr;
    mp->set_type(y, x, MAP_BOMB);
    now_bomb_num ++;
    bomb_ptr = std::make_shared<Bomb>(TYPE_BOMB, get_pixel_x(x), get_pixel_y(y), bomb, mp, &now_bomb_num, bomb_len);
    return bomb_ptr;
}

Role::~Role()
{
}

void Role::move(uint *cell){
    if (move_dx || move_dy){
        int postion_x = get_x(), postion_y = get_y();
        int x = postion_x + move_dx * move_speed, y = postion_y + move_dy * move_speed;
        // 超出地图边界，直接不走
        if (x < MAP_LEFT + MAP_CELL_HALF || x > MAP_RIGHT - MAP_CELL_HALF || y < MAP_TOP + MAP_CELL_HALF || y > MAP_BOTTOM - MAP_CELL_HALF) return;
        int type, check_x, check_y;

        check_x = get_map_y(y + move_dy * (MAP_CELL_HALF - 2));
        check_y = get_map_x(x + move_dx * (MAP_CELL_HALF - 2));
        // 前进方向的边界是碰撞物
        type = *(cell + check_x * MAP_COLUMN + check_y);
        if (type == MAP_DESTRUCTIBLE || type == MAP_INDESTRUCTIBLE) return;
        
        // 所在格子是炸弹
        if (type == MAP_BOMB && (get_map_x(postion_x) != get_map_x(x + move_dx * (MAP_CELL_HALF - 2)) || 
                                 get_map_y(postion_y) != get_map_y(y + move_dy * (MAP_CELL_HALF - 2)) )) return;
        
        if (move_dx) {
            // 所进行方向的斜下角，作位置修正
            check_x = get_map_y(y + (MAP_CELL_HALF - 2));
            check_y = get_map_x(x + move_dx * (MAP_CELL_HALF - 2));
            type = *(cell + check_x * MAP_COLUMN + check_y);
            if (type == MAP_DESTRUCTIBLE || type == MAP_INDESTRUCTIBLE || type == MAP_BOMB) {
                y = get_pixel_y(get_map_y(y)) + MAP_CELL_HALF; 
            }

            // 所进行方向的斜上角，作位置修正
            check_x = get_map_y(y - (MAP_CELL_HALF - 2));
            check_y = get_map_x(x + move_dx * (MAP_CELL_HALF - 2));
            type = *(cell + check_x * MAP_COLUMN + check_y);
            if (type == MAP_DESTRUCTIBLE || type == MAP_INDESTRUCTIBLE || type == MAP_BOMB) {
                y = get_pixel_y(get_map_y(y)) + MAP_CELL_HALF;
            }

        }

        if (move_dy) {
            // 所进行方向的斜左方，作位置修正
            check_x = get_map_y(y + move_dy * (MAP_CELL_HALF - 2));
            check_y = get_map_x(x - (MAP_CELL_HALF - 2));
            type = *(cell + check_x * MAP_COLUMN + check_y);
            if (type == MAP_DESTRUCTIBLE || type == MAP_INDESTRUCTIBLE || type == MAP_BOMB) {
                x = get_pixel_x(get_map_x(x)) + MAP_CELL_HALF;
            }
            // 所进行方向的斜右方，作位置修正
            check_x = get_map_y(y + move_dy * (MAP_CELL_HALF - 2));
            check_y = get_map_x(x + (MAP_CELL_HALF - 2));
            type = *(cell + check_x * MAP_COLUMN + check_y);
            if (type == MAP_DESTRUCTIBLE || type == MAP_INDESTRUCTIBLE || type == MAP_BOMB) {
                x = get_pixel_x(get_map_x(x)) + MAP_CELL_HALF;
            }   
        }
        set_x(x),set_y(y);
    }
}

object_status Role::update_health(uint *cell){
    printf("attack!!!\n");
    if (no_attack_time) {
        no_attack_time--;
        return NORMAL;
    }
    printf("health\n");
    int x = get_x(), y = get_y(), damage = 0;
    // 检查上下左右是否碰到了炸弹。
    for (int i = 0; i < 5; i++){
        int check_x = get_map_y(y + direction_dy[i] * (MAP_CELL_HALF - 2));
        int check_y = get_map_x(x + direction_dx[i] * (MAP_CELL_HALF - 2));
        if (*(cell + check_x * MAP_COLUMN + check_y) == MAP_EXPLOSION){
            damage = 1;
            break;
        }
    }
    printf("damage = %d\n",damage);
    if (damage) {
        health_points -= 20;
        if (health_points <=0) return DELETE;
        no_attack_time = FPS;
    }
    return NORMAL;
}

// void Role::update_probs(uint *cell){
//     int x = get_map_x(get_x()), y = get_map_y(get_y());
//     if (*(cell + y * MAP_COLUMN + x) == )
// }

object_status Role::draw(uint *cell){
    move(cell);
    auto status = update_health(cell);
    //update_probs(cell);
    item->draw(get_x() - MAP_CELL_HALF, get_y() - MAP_CELL_HALF, act_type, frame_now, speed_cnt);
    char health_str[20];
    sprintf(health_str, "%d/%d", health_points, max_health_points);
    fb_draw_text(STATUS_LEFT, STATUS_TOP - 4, health_str, 20, COLOR_WHITE);
    return status;
}