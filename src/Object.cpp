#include "../include/Object.h"

uint object_num = 0;

Object::Object(object_type type, Item *config_item){
    obj_type = type;
    switch (type)
    {
    case TYPE_BACKGROUD:
        id = object_num ++;
        priority = 0;
        postion_x = postion_y = 0;
        time_to_live = UINT32_MAX;
        item = config_item;
        act_type = ACTION_STOP;
        break;
    default:
        break;
    }

}

Object::Object(object_type type, Item *config_item, uint x, uint y){
    obj_type = type;
    switch (type)
    {
    case TYPE_ROLE:
        id = object_num ++;
        priority = 1;
        postion_x = x, postion_y = y;
        time_to_live = UINT32_MAX;
        item = config_item;
        act_type = ACTION_STOP;
        break;
    default:
        break;
    }

}

Object::~Object()
{
}

void Object::set_action_type(action_type type){
    act_type = type;
    item->clear_cnt();
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
        if (x < MAP_LEFT || x > MAP_RIGHT || y < MAP_TOP || y > MAP_BOTTOM) return;
        postion_x = x, postion_y = y;
    }
}

void Object::draw(){
    item->draw(postion_x, postion_y, act_type);
}

void Object::print_info(){
    printf("obj_id = %u\n", id);
}