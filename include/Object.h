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
    Item *item;                     // 
    uint frame_now;                 // 当前帧
    uint speed_cnt;                 // 速度计数
    action_type act_type;           // 当前动画类型    

    /* player 相关*/
    int move_dx, move_dy, move_speed;   // 移动相关 
    uint max_bomb_num;                   // 可以放置的最大炸弹数
    uint now_bomb_num;                   // 当前已经放置的炸弹数

    /* bomb 相关*/
    uint *player_bomb_num;               // 指向放置这个炸弹的用户的now_bomb_num


public:
    uint postion_x, postion_y;      // role类为中心位置坐标，其他为左上角位置的像素坐标
    
    uint time_to_live;              // 剩余存活帧数      
    object_type obj_type;           // 
public:
    Object(object_type type, Item *config_item, uint x, uint y, uint *bomb_num);
    
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
    std::shared_ptr<Object> set_bomb(std::shared_ptr<Map> mp, Item *bomb);
};


 

