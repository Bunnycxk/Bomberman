#pragma once

#include "Object.h"


class Backgroud : public Object
{
private:
    /* data */
    std::shared_ptr<Item> item;          
    action_type act_type;                   // 当前动画类型    
    uint frame_now;                         // 当前帧
    uint speed_cnt;                         // 速度计数

public:
    Backgroud(object_type type, int x, int y, std::shared_ptr<Item> init_item);
    ~Backgroud();
    object_status draw(uint *cell);
};

