#include "game_state.h"



Terrain terrain_grid[MAP_SIZE_H][MAP_SIZE_V];
Bomb bomb_grid[MAP_SIZE_H][MAP_SIZE_V];
item_type items_grid[MAP_SIZE_H][MAP_SIZE_V];
Explosion explosion_grid[MAP_SIZE_H][MAP_SIZE_V];
bool changed_tiles[MAP_SIZE_H][MAP_SIZE_V];
Player players[PLAYER_NUM];