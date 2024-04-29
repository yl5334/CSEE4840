#ifndef _VGA_BALL_H
#define _VGA_BALL_H

#include <linux/ioctl.h>

#include <stdint.h>
#include <stdbool.h>

#ifndef _VGA_BALL_H
#define _VGA_BALL_H
#define MAX_BOMBS 2 



typedef struct {
    unsigned short p1_coordinate, p1_state, p1_bomb, p1_firecenter, p1_fireup, p1_firedown, p1_fireleft, p1_fireright, p2_coordinate, p2_state, p2_bomb, p2_firecenter, p2_fireup, p2_firedown, p2_fireleft, p2_fireright;
} vga_ball_color_t;

typedef struct {
    vga_ball_color_t game_state;
} vga_ball_arg_t;


#define VGA_BALL_MAGIC 'q'

/* ioctls and their arguments */
#define VGA_BALL_WRITE_STATE _IOW(VGA_BALL_MAGIC, 1, vga_ball_arg_t *)
#define VGA_BALL_READ_STATE  _IOR(VGA_BALL_MAGIC, 2, vga_ball_arg_t *)



//Bomb
#define DEFAULT_BOMB_TIMER 100
#define DEFAULT_BOMB_RANGE 1
#define DEFAULT_BOMB_TYPE BOMB_TYPE_NORMAL

typedef enum {
    BOMB_EMPTY,
    BOMB_TYPE_NORMAL
} bomb_type;

typedef struct {
    int32_t timer;   // 0 when out of time
    int8_t range;    // number of grid units for the explosion
    bomb_type type;
    Position position;
    Explosion explosion;

    bool current_frame;
} Bomb;

//Position
typedef struct {
    int32_t x;
    int32_t y;
} Position;

//Explosion
typedef enum {
    EXPLOSION_EMPTY,
    EXPLOSION_TYPE_NORMAL,
    EXPLOSION_TYPE_PERMANENT,
} explosion_type;

typedef struct {
    explosion_type type;
    int32_t timer;
    unsigned char fire_center;
    unsigned char fire_up, fire_down, fire_left, fire_right;
} Explosion;

//Direction
typedef enum {
    DIRECTION_IDLE,
    DIRECTION_UP,
    DIRECTION_DOWN,
    DIRECTION_LEFT,
    DIRECTION_RIGHT
} direction;

//Player
#define DEFAULT_MOVE_SPEED 2
#define DEFAULT_DIRECTION DIRECTION_DOWN
#define DEFAULT_MAX_BOMBS 1

typedef enum {
    PLAYER_ONE   = 0,
    PLAYER_TWO   = 1,
    PLAYER_NONE
} player_id;

typedef struct {
    player_id id;
    bool alive;
    Position tile_position;
    Position screen_position;
    Bomb bomb;
    Explosion explosion

    bool moving;
    direction move_direction;

    bool plant_bomb;
    int8_t max_bomb_number;

    uint8_t current_frame;

} Player;

//Terrain
typedef enum {
    TERRAIN_GROUND,
    TERRAIN_WALL_UNBREAKABLE,
} Terrain;

//game_state
#define MAP_SIZE 13
#define TILE_SIZE 16
#define PLAYER_NUM 2

extern Terrain terrain_grid[MAP_SIZE][MAP_SIZE];
extern Bomb bomb_grid[MAP_SIZE][MAP_SIZE];
extern item_type items_grid[MAP_SIZE][MAP_SIZE];
extern Explosion explosion_grid[MAP_SIZE][MAP_SIZE];
extern bool changed_tiles[MAP_SIZE][MAP_SIZE];
extern Player players[PLAYER_NUM];


#endif
