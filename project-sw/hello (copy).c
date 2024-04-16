/*
 * Userspace program that communicates with the vga_ball device driver
 * through ioctls
 *
 * Stephen A. Edwards
 * Columbia University
 */

#include <stdio.h>
#include "vga_ball.h"
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>

int vga_ball_fd;

/* Read and print the game state */
void print_game_state() {
  vga_ball_arg_t vla;
  
  if (ioctl(vga_ball_fd, VGA_BALL_READ_STATE, &vla)) {
      perror("ioctl(VGA_BALL_READ_STATE) failed");
      return;
  }
  printf("Player 1: x=%u, y=%u\n", vla.game_state.player1.x, vla.game_state.player1.y);
  printf("Player 2: x=%u, y=%u\n", vla.game_state.player2.x, vla.game_state.player2.y);
  printf("Bomb 1: x=%u, y=%u, active=%u\n", vla.game_state.bomb1.x, vla.game_state.bomb1.y, vla.game_state.bomb1.active);
  printf("Bomb 2: x=%u, y=%u, active=%u\n", vla.game_state.bomb2.x, vla.game_state.bomb2.y, vla.game_state.bomb2.active);
}

/* Set the game state */
void set_game_state(const vga_ball_game_state_t *state)
{
  vga_ball_arg_t vla;
  vla.background = *state;
  if (ioctl(vga_ball_fd, VGA_BALL_WRITE_STATE, &vla)) {
      perror("ioctl(VGA_BALL_SET_BACKGROUND) failed");
      return;
  }
}



int main()
{

  printf("VGA ball Userspace program started\n");

  if ( (vga_ball_fd = open(filename, O_RDWR)) == -1) {
    fprintf(stderr, "could not open %s\n", filename);
    return -1;
  }

  vga_ball_game_state_t state = {
        .player1 = {15, 465}, // Bottom-left corner (considering a small margin)
        .player2 = {625, 15}, // Top-right corner
        .bomb1 = {0, 0, 0}, // Initially no bomb placed
        .bomb2 = {0, 0, 0}  // Initially no bomb placed
   };

   set_game_state(&state);
   print_game_state();

   close(vga_ball_fd);
   
  printf("VGA BALL Userspace program terminating\n");
  return 0;
}
