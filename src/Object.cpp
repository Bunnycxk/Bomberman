#include "../include/Object.h"

uint object_num = 0;

Object::Object(object_type obj_type, int postion_x, int postion_y){
    this->id = object_num ++;
    this->obj_type = obj_type;
    this->postion_x = postion_x;
    this->postion_y = postion_y;
}

Object::~Object()
{
    printf("obj delete??\n");
}

void Object::set_priority(uint priority){
    this->priority = priority;
}

void Object::set_TTL(uint ttl){
    time_to_live = ttl;
}

int Object::get_x(){
    return postion_x;
}
void Object::set_x(int x){
    postion_x = x;
}
int Object::get_y(){
    return postion_y;
}
void Object::set_y(int y){
    postion_y = y;
}

uint Object::get_TTL(){
    return time_to_live;
}

void Object::print_info(){
    switch (obj_type)
    {
    case TYPE_ROLE:
        printf("role :");
        break;
    case TYPE_BACKGROUD:
        printf("backgroud :");
        break;
    case TYPE_BOMB:
        printf("bomb :");
    default:
        break;
    }
    printf("obj_id = %u x = %u y = %u TTL = %u \n", id, postion_x, postion_y, time_to_live);
}