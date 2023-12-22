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
        }else
        if ((*it) == "touch_file"){
            touch_filename = value["file_config"][*it].asCString();
        }else
        if ((*it) == "room_name"){
            room_name = value["file_config"][*it].asCString();
        }
    }

    /*
        action_config
    */
   
    mem = value["action_config"].getMemberNames();
    for (auto it = mem.begin();it != mem.end(); it++){
        printf("%s :\n", (*it).c_str());
        item_map[*it] = std::make_shared<Item>(value["action_config"][*it]);
        item_map[*it]->print_info();
        // if ((*it) == "gaming_background"){
        //     mp[]
        //     gaming_background = std::make_shared<Item>(value["action_config"][*it]);
        //     gaming_background->print_info();
        // }else
        // if ((*it) == "gamestart_background"){
        //     gamestart_background = std::make_shared<Item>(value["action_config"][*it]);
        //     gamestart_background->print_info();
        // }else
        // if ((*it) == "win_background"){
        //     win_background = std::make_shared<Item>(value["action_config"][*it]);
        //     win_background->print_info();
        // }else
        // if ((*it) == "lose_background"){
        //     lose_background = std::make_shared<Item>(value["action_config"][*it]);
        //     lose_background->print_info();
        // }else
        // if ((*it) == "createroom_background"){
        //     createroom_background = std::make_shared<Item>(value["action_config"][*it]);
        //     createroom_background->print_info();
        // }else 
        // if ((*it) == "joinroom_background"){
        //     joinroom_background = std::make_shared<Item>(value["action_config"][*it]);
        //     joinroom_background->print_info();
        // }else 
        // if ((*it) == "icon"){
        //     icon = std::make_shared<Item>(value["action_config"][*it]);
        //     icon->print_info();
        // }else
        // if ((*it) == "bomb"){
        //     bomb = std::make_shared<Item>(value["action_config"][*it]);
        //     bomb->print_info();
        // }else
        // if ((*it) == "role"){
        //     role = std::make_shared<Item>(value["action_config"][*it]);
        //     role->print_info();
        // }
    }

}

Config::~Config(){
}

void Config::loadmap(){
    FILE *map_file = fopen(map_filename.c_str(), "r");
    for (int i = 0; i < MAP_ROW; i++)
    for (int j = 0; j < MAP_COLUMN; j++)
        fscanf(map_file, "%u", &cell[i][j]);
    
    // for (int i = 0; i < MAP_ROW; i++){
    //     for (int j = 0; j < MAP_COLUMN; j++)
    //     printf("%u ",cell[i][j]);
    //     printf("\n");
    // }
    
    fclose(map_file);
    
}