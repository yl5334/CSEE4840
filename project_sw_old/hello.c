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


void print_game_state() {
    vga_ball_arg_t vla;
  
    if (ioctl(vga_ball_fd, VGA_BALL_READ_STATE, &vla)) {
        perror("ioctl(VGA_BALL_READ_STATE) failed");
        return;
    }
    printf("Player 1: x=%u, y=%u\n", vla.game_state.player1.x, vla.game_state.player1.y);
    printf("Player 2: x=%u, y=%u\n", vla.game_state.player2.x, vla.game_state.player2.y);
    printf("Bomb 1: x=%u, y=%u, active=%u\n", vla.game_state.bomb1[0].x, vla.game_state.bomb1[0].y, vla.game_state.bomb1[0].active);
    printf("Bomb 2: x=%u, y=%u, active=%u\n", vla.game_state.bomb2[1].x, vla.game_state.bomb2[1].y, vla.game_state.bomb2[1].active);
}


void set_game_state(const vga_ball_game_state_t *state)
{
    vga_ball_arg_t vla;
    vla.background = *state;
    if (ioctl(vga_ball_fd, VGA_BALL_WRITE_STATE, &vla)) {
        perror("ioctl(VGA_BALL_SET_BACKGROUND) failed");
        return;
    }
}

// place a bomb in an empty spot.
void plant_bomb(vga_ball_arg_t *vla, int player_index) {
    vga_ball_arg_t vla;
    for (int i = 0; i < MAX_BOMBS; i++) {
        if (vla->game_state.bombs[i].active == 0) {  
            vla->game_state.bombs[i].active = 1;
            if (player_index == 1) {
                vla->game_state.bombs[i].x = vla->game_state.player1.x;
                vla->game_state.bombs[i].y = vla->game_state.player1.y;
            } else if (player_index == 2) {
                vla->game_state.bombs[i].x = vla->game_state.player2.x;
                vla->game_state.bombs[i].y = vla->game_state.player2.y;
            }
            vla->game_state.bombs[i].timer = 1; 
            vla->game_state.bombs[i].exploded = 0;
            break;
        }
    }
    ioctl(vga_ball_fd, VGA_BALL_WRITE_STATE, &vla);
}


void handle_controller_input(struct controllers ctrls) {
    struct controller_packet packet1, packet2;
    int transferred;

    //controller input
    libusb_interrupt_transfer(ctrls.controller1, ctrls.endpoint1, (unsigned char *)&packet1, sizeof(packet1), &transferred, 0);
    libusb_interrupt_transfer(ctrls.controller2, ctrls.endpoint2, (unsigned char *)&packet2, sizeof(packet2), &transferred, 0);

    vga_ball_arg_t vla;
    ioctl(vga_ball_fd, VGA_BALL_READ_STATE, &vla);
    if (packet1.buttons & BUTTON_UP)
        vla.game_state.player1.y--;
    if (packet1.buttons & BUTTON_DOWN)
        vla.game_state.player1.y++;
    if (packet1.buttons & BUTTON_LEFT)
        vla.game_state.player1.x--;
    if (packet1.buttons & BUTTON_RIGHT)
        vla.game_state.player1.x++;
    if (packet1.buttons & BUTTON_A)
        plant_bomb(1);

  
    if (packet2.buttons & BUTTON_UP)
        vla.game_state.player2.y--;
    if (packet2.buttons & BUTTON_DOWN)
        vla.game_state.player2.y++;
    if (packet2.buttons & BUTTON_LEFT)
        vla.game_state.player2.x--;
    if (packet2.buttons & BUTTON_RIGHT)
        vla.game_state.player2.x++;
    if (packet2.buttons & BUTTON_A)
        plant_bomb(2);
    ioctl(vga_ball_fd, VGA_BALL_WRITE_STATE, &vla);
}

void check_bombs(vga_ball_arg_t *vla) {
    int i;
    for (i = 0; i < MAX_BOMBS; i++) {
        bomb_t *bomb = &vla->game_state.bombs[i];
        if (bomb->active && !bomb->exploded) {
            bomb->timer--;
            if (bomb->timer == 0) {
                bomb->exploded = 1;
                bomb->fire_center = 1;  // The center of the bomb explosion

                // Array to store possible expansion directions
                const int directions[4][2] = {{0, -1}, {0, 1}, {-1, 0}, {1, 0}};
                unsigned char *fires[4] = {&bomb->fire_up, &bomb->fire_down, &bomb->fire_left, &bomb->fire_right};

                // Check all four directions for wall collisions
                for (int dir = 0; dir < 4; dir++) {
                    int nx = bomb->x + directions[dir][0];
                    int ny = bomb->y + directions[dir][1];

                    // Check bounds and if there's a wall at the new location
                    if (nx >= 0 && nx < MAP_WIDTH && ny >= 0 && ny < MAP_HEIGHT && !map[ny][nx]) {
                        *(fires[dir]) = 1;
                    } else {
                        *(fires[dir]) = 0;
                    }
                }

                // Check if any players are hit by the explosion
                player_t *players[2] = {&vla->game_state.player1, &vla->game_state.player2};
		int p; 
                for (p = 0; p < 2; p++) {
                    if (players[p]->x == bomb->x && players[p]->y == bomb->y) {
                        players[p]->alive = 0;  // Player is at the center of the explosion
                    }
                    for (int dir = 0; dir < 4; dir++) {
                        int ex = bomb->x + directions[dir][0];
                        int ey = bomb->y + directions[dir][1];
                        if (*(fires[dir]) && players[p]->x == ex && players[p]->y == ey) {
                            players[p]->alive = 0;  // Player is in the line of fire
                        }
                    }
                }
            }
        }
    }
}


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

    printf("Game Userspace program started\n");

    if ( (vga_ball_fd = open(filename, O_RDWR)) == -1) {
        fprintf(stderr, "could not open %s\n", filename);
        return -1;
    }

    ctrls = opencontrollers();  
    set_game_state(&state);
    print_game_state();
    
    while (1) {
	handle_controller_input(ctrls);  
        vga_ball_arg_t vla;
        

        ioctl(vga_ball_fd, VGA_BALL_READ_STATE, &vla);
        
        check_bombs(&vla); 
	update_bombs(&vla);
        ioctl(vga_ball_fd, VGA_BALL_WRITE_STATE, &vla);
        sleep(0.1);  
    }

    libusb_close(ctrls.controller1);
    libusb_close(ctrls.controller2);
    libusb_exit(NULL);
    close(vga_ball_fd);
   
    printf("Game Userspace program terminating\n");
    return 0;
}
