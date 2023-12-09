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
    frame_now = speed_cnt = 0;
    
    auto img_url = value["image_url"].asCString();
    auto img_type = value["image_type"].asString();
    if (img_type == "png") {
        img = fb_read_png_image(img_url);
    }else
    if (img_type == "jpg"){
        img = fb_read_jpeg_image(img_url);
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

        for (uint j = 0; j < now_action.frame_num; j++){
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

void Item::set(std::shared_ptr<Map> mp, Item *icon){
    
    for (int i = 0; i < MAP_ROW; i++)
    for (int j = 0; j < MAP_COLUMN; j++){     
        
        int icon_type = rand();
        action_type cell_type = ACTION_STOP;
        switch (mp->cell[i][j])
        {
        case MAP_DESTRUCTIBLE:
            cell_type = ACTION_DESTRUTIBLE;
            break;
        case MAP_INDESTRUCTIBLE:
            cell_type = ACTION_INDESTRUTIBLE;
            break;
        case MAP_PROPS:
            cell_type = ACTION_PROPS;
            break;
        case MAP_BOMB:
            cell_type = ACTION_BOMB_CENTER;
            break;
        default:
            continue;
            break;
        }
        Action &icon_action = icon->action[cell_type];
        uint size = icon_action.postion.size();
        icon_type %= size;
        Postion pos = icon_action.postion[icon_type];
        //printf("%d %d\n",i,j);
        //printf("%d %d %d %d %d\n",pos.x, pos.y, pos.w, pos.h,icon->img->line_byte);
        
        fb_image *icon_img = fb_get_sub_image(icon->img, pos.x, pos.y, pos.w, pos.h);
        fb_image *bg_img = fb_get_sub_image(img, MAP_LEFT + j * MAP_CELL_SIZE, MAP_TOP + i * MAP_CELL_SIZE, pos.w, pos.h);
        char *bg_ptr = bg_img->content;
        char *icon_ptr = icon_img->content;
        
        for (int y = 0; y < MAP_CELL_SIZE; y++){
            for (int x = 0; x < MAP_CELL_SIZE; x++)
                fb_mix_pixel(bg_ptr + (x << 2), icon_ptr + (x << 2));
                // *(bg_ptr + x) = *(icon_ptr + x);
            bg_ptr += bg_img->line_byte;
            icon_ptr += icon_img->line_byte;
        }
        fb_free_image(icon_img);
        fb_free_image(bg_img);
        //fb_mix_pixel(MAP_LEFT + j * MAP_CELL_SIZE, MAP_TOP + i * MAP_CELL_SIZE, sub_img, 0);
        //printf("??\n");
    }
}

void Item::clear_cnt(){
    frame_now = speed_cnt = 0;
}

void Item::draw(int x, int y, action_type type){
    Action &now_action = action[type];
    Postion &pos = now_action.postion[frame_now];
    fb_image *sub_img = fb_get_sub_image(img, pos.x, pos.y, pos.w, pos.h);
    fb_draw_image(x, y, sub_img, 0);
    if (++speed_cnt > FPS / now_action.speed) {
        speed_cnt = 0;
        if (++frame_now >= now_action.frame_num) frame_now = 0;
    }
    printf("%d %d\n",type, frame_now);
    free(sub_img);
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