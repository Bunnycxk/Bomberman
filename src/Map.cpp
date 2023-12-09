#include "../include/Map.h"



Map::Map(const char *filename, Item *icon){
    item = icon;
    img = fb_new_image(FB_COLOR_RGBA_8888, MAP_RIGHT - MAP_LEFT, MAP_BOTTOM - MAP_TOP, 0);
    memset(img->content, 0, img->line_byte * img->pixel_h);

    FILE *map_file = fopen(filename, "r");
    for (int i = 0; i < MAP_ROW; i++)
    for (int j = 0; j < MAP_COLUMN; j++){
        printf("%d %d\n",i,j);
        fscanf(map_file, "%u", &cell[i][j]);
        int icon_type = rand();
        uint cell_type = 0;
        switch (cell[i][j])
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
        set_map_cell(i, j, icon_img);
        fb_free_image(icon_img);

        //fb_mix_pixel(MAP_LEFT + j * MAP_CELL_SIZE, MAP_TOP + i * MAP_CELL_SIZE, sub_img, 0);
        //printf("??\n");
    }
    
    for (int i = 0; i < MAP_ROW; i++){    
        for (int j = 0; j < MAP_COLUMN; j++)
        printf("%d ",cell[i][j]);
        printf("\n");
    }

    fclose(map_file);
}

/*
    把地图格子 (x, y) 涂成 透明
*/
void Map::set_empty(int x, int y){
    set_map_cell(x, y, 0);
}

/*
    把地图格子 (x, y) 涂成 icon_img 
*/
void Map::set_map_cell(int x, int y, fb_image *icon_img){

    int *map_ptr =(int *) (img->content + (x * MAP_CELL_SIZE) * img->line_byte + (y * MAP_CELL_SIZE << 2)); 
    int *icon_ptr = NULL;   // 空图标设置为透明
    printf("??\n");
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
        map_ptr += img->line_byte >> 2;
        if (icon_ptr !=NULL) icon_ptr += icon_img->line_byte >> 2;
    }
}

void Map::draw(){
    fb_draw_image(MAP_LEFT, MAP_TOP, img, 0);
}

Map::~Map(){
    fb_free_image(img);
}