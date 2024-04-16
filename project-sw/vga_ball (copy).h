#ifndef _VGA_BALL_H
#define _VGA_BALL_H
#define MAX_BOMBS 2 




#include <linux/ioctl.h>


typedef struct {
    unsigned char x, y; // Player position
    unsigned char alive; // Player life status: 0 = dead, 1 = alive
} player_t;

typedef struct {
    unsigned char active; // Bomb status: 0 = inactive, 1 = active
    unsigned char x, y;   // Bomb position
    unsigned char timer;  // Countdown to explosion
    unsigned char exploded; // 0 = not exploded, 1 = exploded
    unsigned char fire_center;
    unsigned char fire_up, fire_down, fire_left, fire_right;
} bomb_t;

typedef struct {
    unsigned char x, y;
    unsigned char active;
} fire_t;


typedef struct {
    player_t player1 player2;
    bomb_t bombs[MAX_BOMBS];
} game_state_t;

typedef struct {
    game_state_t game_state;
} vga_ball_arg_t;


#define VGA_BALL_MAGIC 'q'

/* ioctls and their arguments */
#define VGA_BALL_WRITE_STATE _IOW(VGA_BALL_MAGIC, 1, vga_ball_arg_t *)
#define VGA_BALL_READ_STATE  _IOR(VGA_BALL_MAGIC, 2, vga_ball_arg_t *)

#endif
