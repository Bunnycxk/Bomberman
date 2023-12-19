#pragma once

#include "Object.h"
#include "Map.h"

class Bomb : public Object
{
private:
    /* data */
    std::shared_ptr<Item> item;          
    action_type act_type;                   // 当前动画类型    
    uint frame_now;                         // 当前帧
    uint speed_cnt;                         // 速度计数
    uint bomb_len;                          // 炸弹长度
    uint* bomb_num;                         // 
    std::shared_ptr<Map> mp;               
public:
    Bomb(object_type type, int x, int y, std::shared_ptr<Item> init_item, std::shared_ptr<Map> mp, uint *bomb_num, uint len);

    ~Bomb();
    
    object_status draw(uint *cell);
};


