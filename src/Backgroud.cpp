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
    printf("Backgroud delete\n");
}


object_status Backgroud::draw(uint *cell){
    //item->print_info();
    item->draw(get_x(), get_y(), act_type, frame_now, speed_cnt);
    return NORMAL;
}

void Backgroud::draw_track(int x1, int y1, int x2, int y2, int color){
    fb_draw_track(x1, y1, x2, y2, color, (int *)item->img->content);
}

void Backgroud::draw_track(int x1, int y1, int color){
    fb_draw_15_circle(x1, y1, color, (int *)item->img->content);
}

void Backgroud::draw_rect(int x1, int y1, int w, int h, int color){
    fb_draw_rect(x1, y1, w, h, color, (int *)item->img->content);
}