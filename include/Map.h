#pragma once
#include "Object.h"



class Map : public Object
{
private:
    /* data */    
    std::shared_ptr<Item> item;          
    std::shared_ptr<Item> icon;          
    action_type act_type;                   // 当前动画类型    
    uint frame_now;                         // 当前帧
    uint speed_cnt;     
    
public:
    uint *cell;
    Map(object_type type, int x, int y, std::shared_ptr<Item> icon, uint *cell);
    
    ~Map();
    void set_empty(int x, int y);
    void set_probs(int x, int y, uint type);

    void set_map_cell(int x, int y, fb_image *icon_img);
    uint get_type(int x, int y);
    void set_type(int x, int y, uint type);

    object_status draw(uint *cell);
};


