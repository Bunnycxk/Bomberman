#include "../include/Config.h"

Config::Config(/* args */){
    std::ifstream ifs;
    ifs.open("./config/config.json");
    assert(ifs.is_open());
    Json::Reader reader;
	Json::Value value;
    Json::Value::Members mem;
    assert(reader.parse(ifs, value, false));

    /*
        file_config
    */
    mem = value["file_config"].getMemberNames();
    for (auto it = mem.begin();it != mem.end(); it++){
        printf("%s :\n", (*it).c_str());
        if ((*it) == "fb_file"){
            fb_filename = value["file_config"][*it].asString();
        }else
        if ((*it) == "font_file"){
            font_filename = value["file_config"][*it].asString();
        }else
        if ((*it) == "gamepad_file"){
            gamepad_filename = value["file_config"][*it].asString();
        }else
        if ((*it) == "map_file"){
            map_filename = value["file_config"][*it].asCString();
        }
    }

    /*
        action_config
    */
   
    mem = value["action_config"].getMemberNames();
    for (auto it = mem.begin();it != mem.end(); it++){
        printf("%s :\n", (*it).c_str());
        if ((*it) == "game_backgroud"){
            game_backgroud = new Item(value["action_config"][*it]);
            game_backgroud->print_info();
        }else
        if ((*it) == "icon"){
            icon = new Item(value["action_config"][*it]);
            icon->print_info();
        }else
        if ((*it) == "bomb"){
            bomb = new Item(value["action_config"][*it]);
            bomb->print_info();
        }else
        if ((*it) == "role"){
            role = new Item(value["action_config"][*it]);
            role->print_info();
        }
    }

}

Config::~Config(){
    delete game_backgroud;
    delete icon;
    delete bomb;
    delete role;
}

