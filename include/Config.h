#pragma once
#include <string>
#include <iostream>
#include <fstream>
#include <memory>
#include <jsoncpp/json/json.h>
#include "../common/common.h"
#include "Item.h"

class Config
{
private:
    /* data */

public:
    std::shared_ptr<Item> gaming_backgroud;
    std::shared_ptr<Item> gamestart_backgroud;
    std::shared_ptr<Item> icon;
    std::shared_ptr<Item> bomb;
    std::shared_ptr<Item> role;
    std::string fb_filename;
    std::string font_filename;
    std::string gamepad_filename;
    std::string map_filename;

    uint cell[MAP_ROW][MAP_COLUMN];
    Config(/* args */);
    ~Config();
    void loadmap();
};