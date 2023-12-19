#pragma once

#include "Object.h"

class Text : public Object
{
private:

    /* data */
    std::shared_ptr<Item> item;          
    action_type act_type;                   // 当前动画类型    
    uint frame_now;                         // 当前帧
    uint speed_cnt;                         // 速度计数
    std::string content;                    // 显示内容
    int font_size;                          // 字体大小
    int color;                              // 字体颜色
public:
    Text(object_type type, int x, int y, std::string &content, int font_size, int color, bool flag = 0);
    ~Text();

    void set_on_flag(bool flag);

    object_status draw(uint *cell);
};

