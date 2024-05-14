#ifndef _MAIN_H
#define _MAIN_H

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



/*game_state.h*/
#define MAP_SIZE_H 40
#define MAP_SIZE_V 30
#define TILE_SIZE 16
#define PLAYER_NUM 2
//#define CONTROLLER_SIGNALS_NUM 10



typedef enum {
    TERRAIN_GROUND,
    TERRAIN_WALL_BREAKABLE,
    TERRAIN_WALL_UNBREAKABLE,
} Terrain;



#define DEFAULT_MOVE_SPEED 1
#define DEFAULT_DIRECTION DIRECTION_RIGHT
#define DEFAULT_MAX_BOMBS 1

typedef enum {
    PLAYER_ONE   = 0,
    PLAYER_TWO   = 1,
    PLAYER_THREE = 2,
    PLAYER_FOUR  = 3,
    PLAYER_NONE
} player_id;

player_id playRound(void);

/*position.h*/


typedef struct {
    int16_t x;
    int16_t y;
} Position;


/*direction.h*/
typedef enum {
    DIRECTION_DOWN,
    DIRECTION_UP,
    DIRECTION_LEFT,
    DIRECTION_RIGHT,
    DIRECTION_IDLE
} direction;


/*Bomb.h*/
#define DEFAULT_BOMB_TIMER 100
#define DEFAULT_BOMB_RANGE 1
#define DEFAULT_BOMB_TYPE BOMB_TYPE_NORMAL

typedef enum {
    BOMB_EMPTY,
    BOMB_TYPE_NORMAL
} bomb_type;

/*explosition.h*/
#define DEFAULT_EXPLOSION_TIMER 100
typedef enum {
    EXPLOSION_EMPTY,
    EXPLOSION_TYPE_NORMAL,
    EXPLOSION_TYPE_CENTER,
    EXPLOSION_TYPE_LEFT,
    EXPLOSION_TYPE_RIGHT,
    EXPLOSION_TYPE_UP,
    EXPLOSION_TYPE_DOWN,
    EXPLOSION_TYPE_PERMANENT,
} explosion_type;


typedef struct {
    explosion_type type;
    int32_t timer;
    int owner;       // todo make a player_id
    bool up;
    bool down;
    bool right;
    bool left;
} Explosion;


typedef struct {
    int owner;       // todo make a player_id
    int32_t timer;   // 0 when out of time
    int8_t range;    // number of grid units for the explosion
    bomb_type type;
    Position position;
    Explosion explosion;
    bool used;

    bool current_frame;
} Bomb;


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


extern Terrain terrain_grid[MAP_SIZE_H][MAP_SIZE_V];
extern Bomb bomb_grid[MAP_SIZE_H][MAP_SIZE_V];
//extern item_type items_grid[MAP_SIZE_H][MAP_SIZE_V];
extern Explosion explosion_grid[MAP_SIZE_H][MAP_SIZE_V];
extern bool changed_tiles[MAP_SIZE_H][MAP_SIZE_V];
extern Player players[PLAYER_NUM];

/*movelogic.h*/
void move(Player *player, direction dir);
bool checkWalkable(int32_t x, int32_t y);
bool readyScreen(void);

void getOccupiedTiles(Player* player, Position* tile1, Position* tile2);
void screenToTile(Position *pos);
bool isAligned(int32_t x, int32_t y);
void setPlayerTilePos(Player* player);
void setPlayerPosition(Player* player, int32_t x, int32_t y);
bool snapToGrid(Player* player, direction dir);
void clamp_x(int32_t* i);
void clamp_y(int32_t* i);
bool walkingOutOfBounds(Player* player, direction dir);

void getPixelOffset(Position* result, direction dir);
void getNextStep(Position* pos, direction dir);
void getOffsets(Position* next_pos, Position* pixel_offset, Player* player, direction dir);


/*bomb_logic.h*/
#define BOMB_ANIMATION_CYCLES 5

void countdownExplosions(void);

bool plantBombs(void);

bool countdownBombs(void);

void plantBomb(Player *player);

void explodeBomb(Bomb *bomb);

void explodeTile(int8_t x, int8_t y, Explosion *explosion);

void killPlayersInExplosion(void);


player_id playRound(void);

#endif
