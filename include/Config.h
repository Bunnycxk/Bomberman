#pragma once
#include <string>
#include <iostream>
#include <fstream>
#include <jsoncpp/json/json.h>
#include "../common/common.h"
#include "Item.h"

class Config
{
private:
    /* data */

public:

    Item* game_backgroud;
    Item* icon;
    Item* bomb;

    Config(/* args */);
    ~Config();
};