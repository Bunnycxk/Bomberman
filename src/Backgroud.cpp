#include "../include/Backgroud.h"

Backgroud::Backgroud(object_type type, int x, int y, std::shared_ptr<Item> init_item):Object(type, x, y){
    item = init_item;
    set_priority(0);
    set_TTL(UINT32_MAX);
    act_type = ACTION_STOP;
    frame_now = speed_cnt = 0;
}

Backgroud::~Backgroud()
{
    printf("bg??\n");
}


void Backgroud::draw(uint *cell){
    printf("??\n");
    item->print_info();
    item->draw(get_x(), get_y(), act_type, frame_now, speed_cnt);
    
}