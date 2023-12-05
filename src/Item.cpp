#include "../include/Item.h"

std::map<std::string, action_type>action_map={
    {"stop",ACTION_STOP},
    {"move",ACTION_MOVE},
    {"destructible",ACTION_DESTRUTIBLE},
    {"indestructible",ACTION_INDESTRUTIBLE},
    {"props",ACTION_PROPS},
    {"bomb_center",ACTION_BOMB_CENTER},
    {"bomb_left",ACTION_BOMB_LEFT},
    {"bomb_right",ACTION_BOMB_RIGHT},
    {"bomb_up",ACTION_BOMB_UP},
    {"bomb_down",ACTION_BOMB_DOWN},
    {"bomb_left_end",ACTION_BOMB_LEFT_END},
    {"bomb_right_end",ACTION_BOMB_RIGHT_END},
    {"bomb_up_end",ACTION_BOMB_UP_END},
    {"bomb_down_end",ACTION_BOMB_DOWN_END}
};
 
Item::Item(Json::Value &value){
    auto img_url = value["image_url"].asString();
    auto img_type = value["image_type"].asString();
    if (img_type == "png") {
        img = fb_read_png_image(img_url.c_str());
    }
    Json::Value &action_value = value["action"];
    auto action_num = action_value.size();
    for (uint i = 0; i < action_num; i++){
        auto &now_value = action_value[i];
        auto action_type = action_map[now_value["action_type"].asString()];
        Action &now_action = action[action_type];
        now_action.type = action_type;
        now_action.speed = now_value["speed"].asUInt();

        now_action.frame_num = now_value["frame_num"].asUInt();
        for (uint j = 0; j <now_action.frame_num; j++){
            now_action.postion.push_back(
                Postion{
                    now_value["x"][j].asUInt(),    now_value["y"][j].asUInt(),
                    now_value["width"][j].asUInt() ,now_value["height"][j].asUInt()
                }
            );
        }
    }
}

Item::~Item(){
    fb_free_image(img);
}

void Item::print_info(){
    for (int i = 0; i < ACTION_COUNT; i++){
        auto &now = action[i];
        if (now.type == i) {
            printf("speed = %u, frame_num = %d\n",now.speed, now.frame_num);
            for (auto pos : now.postion){
                printf("x = %u y = %u w = %u h = %u\n",pos.x, pos.y, pos.w, pos.h);
            }
        }
    }
    printf("\n");
}