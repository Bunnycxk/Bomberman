#pragma once 
#include <string>
#include <memory>
#include "../common/common.h"
#include "Item.h"

enum object_type {TYPE_BACKGROUD, TYPE_ROLE, TYPE_BOMB, TYPE_MAP, TYPE_HEALTH};

class Object
{
private:
    /* data */
    object_type obj_type;           // 
    uint id;                        // 唯一标识符
    uint priority;                  // 显示优先级
    int postion_x, postion_y;       // 左上角的像素坐标，Role类为中心的像素坐标
    uint time_to_live;              // 存活帧数
public:
    Object(object_type obj_type, int postion_x, int postion_y);
    ~Object();

    void set_priority(uint priority);
    
    uint get_TTL();
    void set_TTL(uint TTL);

    int get_x();
    void set_x(int x);
    int get_y();
    void set_y(int y);

    

    void print_info();
    friend bool operator <(const std::shared_ptr<Object>& left, const std::shared_ptr<Object>& right){
        if (left->priority != right->priority){
            return left->priority < right->priority;
        }
        return left->id < right->id;
    }
    
    virtual void draw(uint *cell)=0;
    //virtual void set_move(int dx, int dy, int speed)=0;
};


