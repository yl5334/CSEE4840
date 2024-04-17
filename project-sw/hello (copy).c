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
#include <controller.h> 


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

// Function to check if there is a free slot for a new bomb and plant it if possible
void plant_bomb(vga_ball_arg_t *vla, int player_index) {
    for (int i = 0; i < MAX_BOMBS; i++) {
        if (vla->game_state.bombs[i].active == 0) {  // Find an inactive bomb slot
            vla->game_state.bombs[i].active = 1;
            if (player_index == 1) {
                vla->game_state.bombs[i].x = vla->game_state.player1.x;
                vla->game_state.bombs[i].y = vla->game_state.player1.y;
            } else if (player_index == 2) {
                vla->game_state.bombs[i].x = vla->game_state.player2.x;
                vla->game_state.bombs[i].y = vla->game_state.player2.y;
            }
            vla->game_state.bombs[i].timer = 1; // Set the timer for the bomb
            vla->game_state.bombs[i].exploded = 0;
            break;
        }
    }
}

// Modify handle_controller_input to include bomb planting
void handle_controller_input(controller_button buttons, int player_index) {
    vga_ball_arg_t vla;
    ioctl(vga_ball_fd, VGA_BALL_READ_STATE, &vla);

    // Handling direction buttons
    if (buttons & dir_U) vla.game_state.players[player_index].y--;
    if (buttons & dir_D) vla.game_state.players[player_index].y++;
    if (buttons & dir_L) vla.game_state.players[player_index].x--;
    if (buttons & dir_R) vla.game_state.players[player_index].x++;

    // Planting a bomb
    if (buttons & button_A) {
        plant_bomb(&vla, player_index);
    }

    ioctl(vga_ball_fd, VGA_BALL_WRITE_STATE, &vla);
}

void check_bombs(vga_ball_arg_t *vla) {
    int i;
    for (i = 0; i < MAX_BOMBS; i++) {
        if (vla->game_state.bombs[i].active && !vla->game_state.bombs[i].exploded) {
            vla->game_state.bombs[i].timer--;
            if (vla->game_state.bombs[i].timer == 0) {
                vla->game_state.bombs[i].exploded = 1;
                // Set explosion effect
                vla->game_state.bombs[i].fire_center = 1; // Center fire
                vla->game_state.bombs[i].fire_up = 1;    // Explosion extends upwards
                vla->game_state.bombs[i].fire_down = 1;  // Explosion extends downwards
                vla->game_state.bombs[i].fire_left = 1;  // Explosion extends left
                vla->game_state.bombs[i].fire_right = 1; // Explosion extends right

                // Check and affect players
		player_t *players[2] = {&vla->game_state.player1, &vla->game_state.player2};
		int p;
                for (p = 0; p < 2; p++) {
                    if (vla->game_state.players[p].x == vla->game_state.bombs[i].x &&
                        vla->game_state.players[p].y == vla->game_state.bombs[i].y) {
                        vla->game_state.players[p].alive = 0; // Affects player at the center
                    }
                }
            }
        }
    }
}

// Update the timer for all bombs
static void update_bombs(vga_ball_arg_t *state) {
    for (int i = 0; i < MAX_BOMBS; i++) {
        bomb_t *bomb = &state->bombs[i];
        if (bomb->active && bomb->timer > 0) {
            bomb->timer--;
        }
    }
}

int main()
{

    printf("VGA ball Userspace program started\n");

    if ( (vga_ball_fd = open(filename, O_RDWR)) == -1) {
        fprintf(stderr, "could not open %s\n", filename);
        return -1;
    }


    set_game_state(&state);
    print_game_state();
    controller_init();

    while (1) {
        vga_ball_arg_t vla;
        

        ioctl(vga_ball_fd, VGA_BALL_READ_STATE, &vla);
        
        check_bombs(&vla); // Check for bomb explosions
	update_bombs(&vla);
        ioctl(vga_ball_fd, VGA_BALL_WRITE_STATE, &vla);
        sleep(1); // Game loop runs every second
    }

    controller_destroy();
    close(vga_ball_fd);
   
    printf("VGA BALL Userspace program terminating\n");
    return 0;
}
