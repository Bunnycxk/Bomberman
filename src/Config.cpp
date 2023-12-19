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
        if ((*it) == "gaming_backgroud"){
            gaming_backgroud = std::make_shared<Item>(value["action_config"][*it]);
            gaming_backgroud->print_info();
        }else
        if ((*it) == "gamestart_backgroud"){
            gamestart_backgroud = std::make_shared<Item>(value["action_config"][*it]);
            gamestart_backgroud->print_info();
        }else
        if ((*it) == "icon"){
            icon = std::make_shared<Item>(value["action_config"][*it]);
            icon->print_info();
        }else
        if ((*it) == "bomb"){
            bomb = std::make_shared<Item>(value["action_config"][*it]);
            bomb->print_info();
        }else
        if ((*it) == "role"){
            role = std::make_shared<Item>(value["action_config"][*it]);
            role->print_info();
        }
    }

}

Config::~Config(){
}

void Config::loadmap(){
    FILE *map_file = fopen(map_filename.c_str(), "r");
    for (int i = 0; i < MAP_ROW; i++)
    for (int j = 0; j < MAP_COLUMN; j++)
        fscanf(map_file, "%u", &cell[i][j]);
     for (int i = 0; i < MAP_ROW; i++){
        for (int j = 0; j < MAP_COLUMN; j++)
        printf("%u ",cell[i][j]);
        printf("\n");
     }
    
    fclose(map_file);
    
}