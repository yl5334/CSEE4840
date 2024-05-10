#ifndef _VGA_BALL_H
#define _VGA_BALL_H
#define MAX_BOMBS 2 
#define PLAYER1_COORD_ADDR        0x00
#define PLAYER2_COORD_ADDR        0x04
#define PLAYER1_STATE_ADDR        0x08
#define PLAYER2_STATE_ADDR        0x0C
#define PLAYER1_BOMB_ADDR         0x10
#define PLAYER2_BOMB_ADDR         0x14
#define PLAYER1_FIRE_CENTER_ADDR  0x18
#define PLAYER2_FIRE_CENTER_ADDR  0x1C
#define PLAYER1_FIRE_UP_ADDR      0x20
#define PLAYER2_FIRE_UP_ADDR      0x24
#define PLAYER1_FIRE_DOWN_ADDR    0x28
#define PLAYER2_FIRE_DOWN_ADDR    0x2C
#define PLAYER1_FIRE_LEFT_ADDR    0x30
#define PLAYER2_FIRE_LEFT_ADDR    0x34
#define PLAYER1_FIRE_RIGHT_ADDR   0x38
#define PLAYER2_FIRE_RIGHT_ADDR   0x3C

#include <linux/ioctl.h>


typedef struct {
    unsigned char x, y; //position
    unsigned char alive; //0 = dead, 1 = alive
} player_t;

typedef struct {
    unsigned char active; //0 = inactive, 1 = active
    unsigned char x, y;   
    unsigned char timer;  //time before explosion
    unsigned char exploded; //0 = not exploded, 1 = exploded
    unsigned char fire_center;
    unsigned char fire_up, fire_down, fire_left, fire_right;
} bomb_t;

typedef struct {
    unsigned char x, y;
    unsigned char active;
} fire_t;


typedef struct {
    player_t player1, player2;
    bomb_t bombs[MAX_BOMBS];
} game_state_t;

typedef struct {
    game_state_t state;
} vga_ball_arg_t;


#define VGA_BALL_MAGIC 'q'

/* ioctls and their arguments */
#define VGA_BALL_WRITE_STATE _IOW(VGA_BALL_MAGIC, 1, vga_ball_arg_t *)
#define VGA_BALL_READ_STATE  _IOR(VGA_BALL_MAGIC, 2, vga_ball_arg_t *)

#endif
