#pragma once
#include <string>

#include "../common/common.h"
#include "Item.h"

enum object_type {TYPE_BACKGROUD, TYPE_ROLE, TYPE_BOMB};

class Object
{
private:
    /* data */
    uint id;                        // 唯一标识符
    uint priority;                  // 显示优先级
    uint postion_x, postion_y;      // 左上角位置的像素坐标
    int move_dx, move_dy, move_speed;         // 
    uint time_to_live;              // 剩余存活帧数      
    Item *item;                     // 
    action_type act_type;           // 当前动画类型
public:
    object_type obj_type;           // 
public:
    Object(object_type type, Item *config_item);
    Object(object_type type, Item *config_item, uint x, uint y);
    
    ~Object();
    // bool operator < (const Object& object) const {
    //     if (this->priority != object.priority) {
    //         return this->priority < object.priority;
    //     }
    //     return this->id < object.id;
    // }
    
    friend bool operator <(const std::shared_ptr<Object>& left, const std::shared_ptr<Object>& right){
        if (left->priority != right->priority){
            return left->priority < right->priority;
        }
        return left->id < right->id;
    }

    void draw();
    void move(std::shared_ptr<Map> mp);
    void print_info();
    void set_action_type(action_type type);
    void set_move(int dx, int dy, int speed);
};


 

