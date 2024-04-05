/*
 * Userspace program that communicates with the vga_ball device driver
 * through ioctls
 *
 * Hongkuan Yu (hy2819), Yelin Mao (ym3000), Yuqi Lin (yl5334)
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

/* Read and print the background color */
void print_background_color() {
  vga_ball_arg_t vla;
  
  if (ioctl(vga_ball_fd, VGA_BALL_READ_BACKGROUND, &vla)) {
      perror("ioctl(VGA_BALL_READ_BACKGROUND) failed");
      return;
  }
  printf("%02x %02x %02x\n",
	 vla.background.red, vla.background.green, vla.background.blue);
}

/* Set the background color */
void set_background_color(const vga_ball_color_t *c)
{
  vga_ball_arg_t vla;
  vla.background = *c;
  if (ioctl(vga_ball_fd, VGA_BALL_WRITE_BACKGROUND, &vla)) {
      perror("ioctl(VGA_BALL_SET_BACKGROUND) failed");
      return;
  }
}


void print_position() {
  vga_ball_arg_t vla;
  
  if (ioctl(vga_ball_fd, VGA_BALL_READ_POSITION, &vla)) {
      perror("ioctl(VGA_BALL_READ_BACKGROUND) failed");
      return;
  }
}

void set_circle(const vga_ball_position_t *pos)
{
  if (ioctl(vga_ball_fd, VGA_BALL_WRITE_POSITION, pos)) {
      perror("ioctl(VGA_BALL_SET_BACKGROUND) failed");
      return;
  }
}

void set_ball_circle(int x, int y)
{
  vga_ball_position_t pos = { .x=x, .y=y};
  if (ioctl(vga_ball_fd, VGA_BALL_WRITE_POSITION, &pos)) {
      perror("ioctl(VGA_BALL_SET_POSITION) failed");
      return;
  }
}

int main()
{
  vga_ball_arg_t vla;
  int i;
  static const char filename[] = "/dev/vga_ball";

  static const vga_ball_color_t colors[] = {
    { 0xff, 0x00, 0x00 }, /* Red */
    { 0x00, 0xff, 0x00 }, /* Green */
    { 0x00, 0x00, 0xff }, /* Blue */
    { 0xff, 0xff, 0x00 }, /* Yellow */
    { 0x00, 0xff, 0xff }, /* Cyan */
    { 0xff, 0x00, 0xff }, /* Magenta */
    { 0x80, 0x80, 0x80 }, /* Gray */
    { 0x00, 0x00, 0x00 }, /* Black */
    { 0xff, 0xff, 0xff }  /* White */
  };

  static vga_ball_position_t position;
 

# define COLORS 9

  printf("VGA ball Userspace program started\n");

  if ( (vga_ball_fd = open(filename, O_RDWR)) == -1) {
    fprintf(stderr, "could not open %s\n", filename);
    return -1;
  }

  printf("initial state: ");
  print_background_color();

    int x=100, y =100;
    position.x = x;
    position.y = y;
    int dx = 1;
    int dy = 1;
    const int maxX = 640, maxY = 480; //Resolution restrictions
    
    for (;;) {
        //Update current coordinates
        x += dx;
        y += dy;

        //Horizontal bounce
        if (x <= 15) {
            dx = 1; //Bounce to right
            x = 15; //The radius we set is 15, so we need to make the ball fully show up instead of immersing into the side of the screen
        } else if (x >= maxX - 15) {
            dx = -1; //Bounce to left
            x = maxX -15;
        }

        //Vertical bounce
        if (y <= 15) {
            dy = 1; //Bounce down
            y = 15;
        } else if (y >= maxY -15) {
            dy = -1; //Bounce up
            y = maxY -15;
        }
     
	position.x = x;
    	position.y = y;
	set_circle(&position);
        print_position();
	printf("%04d %04d\n", position.x, position.y);
        usleep(30000);

    }
  
	
  
  printf("VGA BALL Userspace program terminating\n");
  return 0;
}
