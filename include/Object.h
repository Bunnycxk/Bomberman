#include <string>

#include "../common/common.h"
#include "Item.h"

enum object_type {TYPE_BACKGROUD, TYPE_ROLE, TYPE_BOMB};

class Object
{
private:
    /* data */
    uint id;                        // 唯一标识符
    object_type type;
    uint priority;                  // 显示优先级
    uint postion_x, postion_y;      // 左上角位置的像素坐标
    uint time_to_live;              // 剩余存活帧数      
    Item *item;                     // 
    
public:
    Object(object_type type, Item *config_item);
    ~Object();
    bool operator < (const Object& object) const {
        if (this->priority != object.priority) {
            return this->priority < object.priority;
        }
        return this->id < object.id;
    }
    void print_info();
};



