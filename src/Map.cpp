#include "../include/Map.h"



Map::Map(object_type type, int x, int y, std::shared_ptr<Item> icon, uint *cell):Object(type, x, y){
    item = std::make_shared<Item>(ACTION_STOP, MAP_RIGHT - MAP_LEFT, MAP_BOTTOM - MAP_TOP);
    this->icon = icon;
    set_priority(1);
    set_TTL(UINT32_MAX);
    act_type = ACTION_STOP;
    frame_now = speed_cnt = 0;
    this->cell = cell;

    for (int i = 0; i < MAP_ROW; i++)
    for (int j = 0; j < MAP_COLUMN; j++){
        int icon_type = rand();
        uint cell_type = 0;
        switch (*(cell + i * MAP_COLUMN + j))
        {
        case MAP_DESTRUCTIBLE:
            cell_type = ACTION_DESTRUTIBLE;
            break;
        case MAP_INDESTRUCTIBLE:
            cell_type = ACTION_INDESTRUTIBLE;
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
        fb_image *icon_img = fb_get_sub_image(icon->img, pos.x, pos.y, pos.w, pos.h);
        set_map_cell(i, j, icon_img);
        fb_free_image(icon_img);
    }
}



uint Map::get_type(int x, int y){
    return *(cell + x * MAP_COLUMN + y);
}


void Map::set_type(int x, int y, uint type){
    *(cell + x * MAP_COLUMN + y) = type;
}

void Map::set_probs(int x, int y, uint type){
    *(cell + x * MAP_COLUMN + y) = type + MAP_PROPS_SPEED;
    Postion pos = icon->action[ACTION_PROPS].postion[type];
    fb_image *icon_img = fb_get_sub_image(icon->img, pos.x, pos.y, pos.w, pos.h);
    this->set_map_cell(x, y, icon_img);
    fb_free_image(icon_img);
}

/*
    把地图格子 (x, y) 涂成 透明
*/
void Map::set_empty(int x, int y){
    *(cell + x * MAP_COLUMN + y) = MAP_EMPTY;
    this->set_map_cell(x, y, NULL);
}

/*
    把地图格子 (x, y) 涂成 icon_img 
*/
void Map::set_map_cell(int x, int y, fb_image *icon_img){

    int *map_ptr =(int *) (item->img->content + (x * MAP_CELL_SIZE) * item->img->line_byte + (y * MAP_CELL_SIZE << 2)); 
    int *icon_ptr = NULL;   // 空图标设置为透明
    if (icon_img != NULL) { 
        icon_ptr = (int *) (icon_img->content);
    }

    for (int i = 0; i < MAP_CELL_SIZE; i++){
        for (int j = 0; j < MAP_CELL_SIZE; j++){
            if (icon_ptr == NULL) {
                *(map_ptr + j) = 0;
            }else {
                *(map_ptr + j) = *(icon_ptr + j);
            }
        }
        map_ptr += item->img->line_byte >> 2;
        if (icon_ptr !=NULL) icon_ptr += icon_img->line_byte >> 2;
    }
}

object_status Map::draw(uint *cell){
    //fb_draw_image(MAP_LEFT, MAP_TOP, img, 0);
    item->draw(get_x(), get_y(), act_type, frame_now, speed_cnt);
    return NORMAL;
}

Map::~Map(){
}