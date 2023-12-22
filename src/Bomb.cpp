#include "../include/Bomb.h"

const int direction_dx[] = {0, 1,  0, -1};
const int direction_dy[] = {1, 0, -1,  0};
const action_type direction_type[2][4] = {
    {ACTION_BOMB_DOWN, ACTION_BOMB_RIGHT, ACTION_BOMB_UP, ACTION_BOMB_LEFT},
    {ACTION_BOMB_DOWN_END, ACTION_BOMB_RIGHT_END, ACTION_BOMB_UP_END, ACTION_BOMB_LEFT_END}
};

Bomb::Bomb(object_type type, int x, int y, std::shared_ptr<Item> init_item, std::shared_ptr<Map> mp, uint *bomb_num, uint len):Object(type, x, y){
    item = init_item;
    this->mp = mp;
    set_priority(3);
    set_TTL(FPS * 3);       // 炸弹 3s 后爆炸
    act_type = ACTION_STOP;
    frame_now = speed_cnt = 0;
    bomb_len = len;
    this->bomb_num = bomb_num;
}

Bomb::~Bomb()
{
    (*bomb_num)--;
    printf("Bomb delete!\n");
}

object_status Bomb::draw(uint *cell){
    set_TTL(get_TTL() - 1);
    switch (act_type)
    {
    case ACTION_STOP:
        {
            item->draw(get_x(), get_y(), act_type, frame_now, speed_cnt);
            if (get_TTL() == 0){
                uint x = get_map_x(get_x()), y = get_map_y(get_y()); 
                mp->set_type(y, x, MAP_EMPTY);
                set_TTL(2 * FPS / 10);
                act_type = ACTION_BOMB_CENTER;
            }
        }
        break;
    case ACTION_BOMB_CENTER:
        {
            item->draw(get_x(), get_y(), ACTION_BOMB_CENTER, frame_now, speed_cnt);
            int x = get_map_x(get_x()), y = get_map_y(get_y());
            printf("map_row = %d map_column = %d\n",y,x);
            if (get_TTL()) mp->set_type(y, x, MAP_EXPLOSION); else mp->set_type(y, x, MAP_EMPTY);
            int v[4] = {0};
            for (uint j = 1; j <= bomb_len; j++){
                for (int i = 0; i < 4; i++)
                if (!v[i]){
                    int new_x = x + j * direction_dx[i], new_y = y + j * direction_dy[i];
                    if (new_x < 0 || new_x >= MAP_COLUMN || new_y < 0 || new_y >= MAP_ROW) {
                        v[i] = 1;
                        continue;
                    }
                    if (*(cell + new_y * MAP_COLUMN + new_x) == MAP_INDESTRUCTIBLE) {
                        v[i] = 1;
                        continue;
                    }
                    if (*(cell + new_y * MAP_COLUMN + new_x) == MAP_DESTRUCTIBLE) {
                        mp->set_empty(new_y, new_x);
                    }
                    if (get_TTL()) mp->set_type(new_y, new_x, MAP_EXPLOSION); else mp->set_type(new_y, new_x, MAP_EMPTY);
                    item->draw(get_pixel_x(new_x), get_pixel_y(new_y), direction_type[j == bomb_len][i], frame_now, speed_cnt);
                }
            }
            if (!get_TTL()) return DELETE;
        }
        break;
    default:
        break;
    }
    return NORMAL;
}