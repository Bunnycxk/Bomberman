#pragma once

#include "Object.h"
#include "Bomb.h"

/*
    角色
*/

class Role: public Object
{
private:
    /* data */
    int move_dx, move_dy, move_speed;     // 移动相关 
    uint max_bomb_num;                    // 可以放置的最大炸弹数
    uint now_bomb_num;                    // 当前已经放置的炸弹数
    uint bomb_len;                        // 炸弹长度
    int health_points, max_health_points; // 生命值
    std::shared_ptr<Item> item;          
    action_type act_type;           // 当前动画类型  
    uint frame_now;                 // 当前帧
    uint speed_cnt;                 // 速度计数

public:
    Role(object_type type, int x, int y, std::shared_ptr<Item> init_item, uint len);
    ~Role();

    void draw(uint *cell);
    void move(uint *cell);

    void set_action_type(action_type type);
    void set_move(int dx, int dy, int speed);
    std::shared_ptr<Bomb> set_bomb(std::shared_ptr<Map> mp, std::shared_ptr<Item> bomb);
};
