#pragma once
#include <jsoncpp/json/json.h>
#include <map>
#include <vector>
#include "../common/common.h"


enum action_type {
                ACTION_STOP, ACTION_MOVE, 
                ACTION_DESTRUTIBLE, ACTION_INDESTRUTIBLE, ACTION_PROPS, 
                ACTION_BOMB_CENTER, ACTION_BOMB_LEFT, ACTION_BOMB_RIGHT, ACTION_BOMB_UP, ACTION_BOMB_DOWN,
                ACTION_BOMB_LEFT_END, ACTION_BOMB_RIGHT_END, ACTION_BOMB_UP_END, ACTION_BOMB_DOWN_END,
                ACTION_COUNT};

struct Postion{
    uint x,y;
    uint w,h;
};

struct Action{
    action_type type;
    uint speed;
    uint frame_num;
    std::vector<Postion>postion;
};

class Item
{
private:
    /* data */
    fb_image *img;
    Action action[ACTION_COUNT];

public:
    Item(Json::Value &value);
    ~Item();
    void print_info();
};

