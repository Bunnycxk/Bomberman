#include "../include/Config.h"

Config::Config(/* args */){
    std::ifstream ifs;
    ifs.open("./config/config.json");
    assert(ifs.is_open());
    Json::Reader reader;
	Json::Value value;
    assert(reader.parse(ifs, value, false));
    Json::Value::Members mem = value.getMemberNames();
    for (auto it = mem.begin();it != mem.end(); it++){
        printf("%s :\n", (*it).c_str());
        if ((*it) == "game_backgroud"){
            game_backgroud = new Item(value[*it]);
            game_backgroud->print_info();
        }else
        if ((*it) == "icon"){
            icon = new Item(value[*it]);
            icon->print_info();
        }else
        if ((*it) == "bomb"){
            bomb = new Item(value[*it]);
            bomb->print_info();
        }
    }

}

Config::~Config(){
    delete game_backgroud;
    delete icon;
    delete bomb;
}

