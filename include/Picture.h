#pragma once
#include "Object.h"

class Picture : public Object
{
private:
    /* data */
    std::shared_ptr<Item> item;          
    action_type act_type;                   // 当前动画类型    
    uint frame_now;                         // 当前帧
    uint speed_cnt;                         // 速度计数

public:
    Picture(object_type type, int x, int y, std::shared_ptr<Item> init_item);
    ~Picture();
};



