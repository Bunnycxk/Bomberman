#include "../include/Object.h"

uint object_num = 0;

Object::Object(object_type type,Item *config_item){
    if (type == TYPE_BACKGROUD){
        id = object_num ++;
        priority = 0;
        postion_x = postion_y = 0;
        time_to_live = UINT32_MAX;
        item = config_item;
    }
}

Object::~Object()
{
}

void Object::print_info(){
    printf("obj_id = %u\n", id);
}