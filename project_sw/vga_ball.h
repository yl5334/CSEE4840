#ifndef _VGA_BALL_H
#define _VGA_BALL_H

#include <linux/ioctl.h>


typedef struct {
    unsigned long p1_coordinate, p1_state, p1_bomb, p1_firecenter, p1_fireup, p1_firedown, p1_fireleft, p1_fireright, p2_coordinate, p2_state, p2_bomb, p2_firecenter, p2_fireup, p2_firedown, p2_fireleft, p2_fireright, map_info;
} vga_ball_color_t;

typedef struct {
    vga_ball_color_t game_state;
} vga_ball_arg_t;


#define VGA_BALL_MAGIC 'q'

/* ioctls and their arguments */
#define VGA_BALL_WRITE_STATE _IOW(VGA_BALL_MAGIC, 1, vga_ball_arg_t *)
#define VGA_BALL_READ_STATE  _IOR(VGA_BALL_MAGIC, 2, vga_ball_arg_t *)



#endif
