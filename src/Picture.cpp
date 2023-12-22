#include "../include/Picture.h"


Picture::Picture(object_type type, int x, int y, ):Object(type, x, y){
    item = std::make_shared<Item>(ACTION_STOP,);
    set_priority(1);
    set_TTL(UINT32_MAX);
    act_type = ACTION_STOP;
    frame_now = speed_cnt = 0;
}

Picture::~Picture()
{
}