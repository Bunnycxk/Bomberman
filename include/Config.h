#pragma once
#include <string>
#include <iostream>
#include <fstream>
#include <memory>
#include <jsoncpp/json/json.h>
#include <map>
#include "../common/common.h"
#include "Item.h"



class Config
{
private:
    /* data */

public:
    std::map<std::string,std::shared_ptr<Item>>item_map;

    // std::shared_ptr<Item> gaming_background;
    // std::shared_ptr<Item> gamestart_background;
    // std::shared_ptr<Item> win_background;
    // std::shared_ptr<Item> lose_background;
    // std::shared_ptr<Item> createroom_background;
    // std::shared_ptr<Item> joinroom_background;
    // std::shared_ptr<Item> gamemenu_background;
    // std::shared_ptr<Item> icon;
    // std::shared_ptr<Item> bomb;
    // std::shared_ptr<Item> role;
    std::string fb_filename;
    std::string font_filename;
    std::string gamepad_filename;
    std::string map_filename;
    std::string touch_filename;
    std::string room_name;
    uint cell[MAP_ROW][MAP_COLUMN];
    Config(/* args */);
    ~Config();
    void loadmap();
};