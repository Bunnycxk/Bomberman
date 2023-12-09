#include "../include/Map.h"

Map::Map(const char *filename){
    FILE *map_file = fopen(filename, "r");
    for (int i = 0; i < MAP_ROW; i++)
    for (int j = 0; j < MAP_COLUMN; j++)
        fscanf(map_file, "%u", &cell[i][j]);
    
    for (int i = 0; i < MAP_ROW; i++){    
        for (int j = 0; j < MAP_COLUMN; j++)
        printf("%d ",cell[i][j]);
        printf("\n");
    }

    fclose(map_file);
}

Map::~Map()
{
}