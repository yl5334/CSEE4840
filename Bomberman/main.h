#include <stdio.h>
#include "vga_ball.h"
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include "controller.h"
#include <libusb-1.0/libusb.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdbool.h>
#include <stdint.h>
#include "game_state.h"
#include "position.h"
#include "bomb.h"
#include "direction.h"


/*game_state.h*/
#define MAP_SIZE_H 40
#define MAP_SIZE_V 30
#define TILE_SIZE 16
#define PLAYER_NUM 2
//#define CONTROLLER_SIGNALS_NUM 10

extern Terrain terrain_grid[MAP_SIZE_H][MAP_SIZE_V];
extern Bomb bomb_grid[MAP_SIZE_H][MAP_SIZE];
extern item_type items_grid[MAP_SIZE_H][MAP_SIZE_V];
extern Explosion explosion_grid[MAP_SIZE_H][MAP_SIZE];
extern bool changed_tiles[MAP_SIZE_H][MAP_SIZE];
extern Player players[PLAYER_NUM];

typedef enum {
    TERRAIN_GROUND,
    TERRAIN_WALL_BREAKABLE,
    TERRAIN_WALL_UNBREAKABLE,
} Terrain;



#define DEFAULT_MOVE_SPEED 2
#define DEFAULT_DIRECTION DIRECTION_DOWN
#define DEFAULT_MAX_BOMBS 1

typedef enum {
    PLAYER_ONE   = 0,
    PLAYER_TWO   = 1,
    PLAYER_THREE = 2,
    PLAYER_FOUR  = 3,
    PLAYER_NONE
} player_id;

typedef struct {
    player_id id;
    bool alive;
    Position tile_position;
    Position screen_position;
    Bomb bomb;

    bool moving;
    int8_t move_speed;
    direction move_direction;

    bool plant_bomb;
    int8_t max_bomb_number;
    int8_t current_bomb_number;

    uint8_t current_frame;

    uint8_t number_of_wins;
} Player;

/*position.h*/


typedef struct {
    int16_t x;
    int16_t y;
} Position;


/*direction.h*/
typedef enum {
    DIRECTION_UP,
    DIRECTION_DOWN,
    DIRECTION_LEFT,
    DIRECTION_RIGHT,
    DIRECTION_IDLE
} direction;


