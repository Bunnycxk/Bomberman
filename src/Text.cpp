#include "../include/Text.h"


Text::Text(object_type type, int x, int y, std::string &content, int font_size, int color, bool flag):Object(type, x, y){
    printf("TEXT NOW CREATE %s\n",content.c_str());
    set_priority(1);
    set_TTL(UINT32_MAX);
    act_type = flag?ACTION_TEXT_ON:ACTION_TEXT_OFF;
    item = std::make_shared<Item>(act_type, 0, 0);
    this->content = content;
    this->font_size = font_size;
    this->color = color;    
    frame_now = speed_cnt = 0;
}

Text::~Text()
{
    printf("Text delete!\n");
}


void Text::set_on_flag(bool flag){
    act_type = flag?ACTION_TEXT_ON:ACTION_TEXT_OFF;
}


object_status Text::draw(uint *cell){
    if (act_type == ACTION_TEXT_OFF){
        item->draw(get_x(), get_y(), content.c_str(), font_size, color);
    }else {
        item->draw(get_x(), get_y(), ("> "+content).c_str(), font_size, color);
    }
    return NORMAL;
}