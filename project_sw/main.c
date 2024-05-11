
#include "main.h"


struct controller_list open_controllers();

int vga_ball_fd;

unsigned short coords;
unsigned short coords2;

vga_ball_color_t color = {0};

pthread_t p1b, p2b;

// TODO : add a 0(ground), 1(unbreakable), 2(breakable) map to this
int map[30][40] = {
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    };

Terrain terrain_grid[MAP_SIZE_H][MAP_SIZE_V];
Bomb bomb_grid[MAP_SIZE_H][MAP_SIZE_V];
//item_type items_grid[MAP_SIZE_H][MAP_SIZE_V];
Explosion explosion_grid[MAP_SIZE_H][MAP_SIZE_V];
bool changed_tiles[MAP_SIZE_H][MAP_SIZE_V];
Player players[PLAYER_NUM];

struct controller_list devices;
struct controller_pkt controller1, controller2;
int fields1, fields2;
int size1 = sizeof(controller1);
int size2 = sizeof(controller2);

player_id getWinner(void) {
    for (int i = 0; i < PLAYER_NUM; i++) {
        if (players[i].alive) {
            return players[i].id;
        }
    }
    return PLAYER_NONE;
}



int main(){


    vga_ball_arg_t vla;
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
  devices = open_controllers();
  size1 = sizeof(controller1);
  size2 = sizeof(controller2);


  # define COLORS 9

  printf("Tanks Game Userspace program started\n");

  if ( (vga_ball_fd = open(filename, O_RDWR)) == -1) {
    fprintf(stderr, "could not open %s\n", filename);
    return -1;
  }


    initialisePlayers();
    setupRound();

    while(ready_screen){
        runGame();

   }
/*
    for(;;){
        runGame();
    }
    
*/
}

void runGame(void){

    //initialisePlayers();
    //setupRound();
    player_id winner = playRound();
   /*
    while(ready_screen()){
        player_id winner = playRound();

        for(int j = 0; j < PLAYER_NUM; j++) {
            if(winner == players[j].id) {
                players[j].number_of_wins++;
            }
        }

    }
    */

}


void initialisePlayers(void) {
    for (int i = 0; i < PLAYER_NUM; i++) {
        players[i].id = i;
        players[i].number_of_wins = 0;
    }
}

bool readyScreen(void) {
    
    //drawReadyScreen();
    //renderFrame();
    bool player1_ready = false;
    bool player2_ready = false;

    while(!(player1_ready && player2_ready)) {
        updateControls();
        uint8_t p1_ab = controller1.ab;
        uint8_t p2_ab = controller2.ab;
        
        if(!player1_ready && (p1_ab == 79 || p1_ab == 95 || p1_ab == 207 || p1_ab == 223)) {
            //drawPlayerReady(PLAYER_ONE);
            player1_ready = true;
            //snd_play_pickup();
        }
        if(!player2_ready && (p2_ab == 79 || p1_ab == 95 || p1_ab == 207 || p1_ab == 223)) {
            //drawPlayerReady(PLAYER_TWO);
            player2_ready = true;
            //snd_play_pickup();
        }
        //update_sound();
    }


    while(player1_ready && player2_ready) {
        updateControls();
        return ture;
        /*
        if(player1_ready && controller_signals[CONTROLLER_ONE_IND_TRIGGER] == 0) {
            //drawPlayerReady(PLAYER_ONE);
            player1_ready = false;
        }
        if(player2_ready && controller_signals[CONTROLLER_TWO_IND_TRIGGER] == 0) {
            //drawPlayerReady(PLAYER_TWO);
            player2_ready = false;
        }
        //update_sound();
        */
    }
    //return true;

}

void updateControls(void) {

    libusb_interrupt_transfer(devices.device1, devices.device1_addr, (unsigned char *) &controller1, size1, &fields1, 0);
    libusb_interrupt_transfer(devices.device2, devices.device2_addr, (unsigned char *) &controller2, size2, &fields2, 0);
    applyPlayerInput(&players[PLAYER_ONE], &controller1);
    applyPlayerInput(&players[PLAYER_TWO], &controller2);
}

void setupRound(void) {

    setupGrids();
    setupPlayers();
    //drawInitialState();
    drawPlayers();
    //drawSidebar();
    //drawPlayerIcons();
    //drawPlayerScores();
    //drawFullTimebar();
    //renderFrame();
}

void setupGrids(void) {

    /* Set up default values */
    for (int i = 0; i < MAP_SIZE_H; i++) {
        for (int j = 0; j < MAP_SIZE_V; j++) {
            switch (map[i][j])
            {
            case 0:
                terrain_grid[i][j] = TERRAIN_GROUND;
                break;
            case 1:
                terrain_grid[i][j] = TERRAIN_WALL_BREAKABLE;
                break;
            }
            

            bomb_grid[i][j].type = BOMB_EMPTY;

            bomb_grid[i][j].timer = 0;
            bomb_grid[i][j].range = 0;

            explosion_grid[i][j].type = EXPLOSION_EMPTY;
            explosion_grid[i][j].timer = 0;

            changed_tiles[i][j] = false;

            //items_grid[i][j] = ITEM_EMPTY;
        }
    }
}

void setupPlayers(void) {

    for (int i = 0; i < PLAYER_NUM; i++) {
        if (i == 0)
        {
            players[i].screen_position.x = 0;
            players[i].screen_position.y = 0;
        }
        else
        {
            players[i].screen_position.x = (MAP_SIZE_H - 1) * TILE_SIZE + 1;
            players[i].screen_position.y = (MAP_SIZE_V - 1) * TILE_SIZE + 1;
        }
        
        
        players[i].alive = true;

        players[i].bomb.owner = players[i].id;
        players[i].bomb.timer = DEFAULT_BOMB_TIMER;
        players[i].bomb.range = DEFAULT_BOMB_RANGE;
        players[i].bomb.type  = DEFAULT_BOMB_TYPE;
        players[i].bomb.current_frame  = 0;
        players[i].bomb.explosion.type = EXPLOSION_TYPE_NORMAL;
        players[i].bomb.explosion.timer = 40;

        players[i].moving          = false;
        players[i].move_speed      = DEFAULT_MOVE_SPEED;
        players[i].move_direction  = DEFAULT_DIRECTION;

        players[i].max_bomb_number     = DEFAULT_MAX_BOMBS;
        players[i].current_bomb_number = 0;

        //players[i].current_frame = A_MOVE_DOWN_FRAMES * i;
    }

	
}

void set_background_color(const vga_ball_color_t *c)
{
  vga_ball_arg_t vla;
  vla.game_state = *c;
  if (ioctl(vga_ball_fd, VGA_BALL_WRITE_STATE, &vla)) {
      perror("ioctl(VGA_BALL_SET_BACKGROUND) failed");
      return;
  }
}


void drawPlayers(void){
    for (int i = 0; i < PLAYER_NUM; i++) {
        
        if (!(players[i].alive)) {
            continue;
        }
        uint32_t coordinate = 0;
        uint32_t state = 0;
        //uint8_t* player_sprite = NULL;
        //uint8_t num_of_frames  = 0;

        switch (players[i].move_direction) {
            case DIRECTION_UP:
                coordinate = ((uint32_t) players[i].screen_position.y << 16) | players[i].screen_position.x;
                state |= ((players[i].move_direction & 0x3) | (players[i].alive << 2)); 
                break;
            case DIRECTION_DOWN:
                coordinate = ((uint32_t) players[i].screen_position.y << 16) | players[i].screen_position.x;
                state |= ((players[i].move_direction & 0x3) | (players[i].alive << 2));
                break;
            case DIRECTION_LEFT:
                coordinate = ((uint32_t) players[i].screen_position.y << 16) | players[i].screen_position.x;
                state |= ((players[i].move_direction & 0x3) | (players[i].alive << 2));
                break;
            case DIRECTION_RIGHT:
                coordinate = ((uint32_t) players[i].screen_position.y << 16) | players[i].screen_position.x;
                state |= ((players[i].move_direction & 0x3) | (players[i].alive << 2));
                break;
            case DIRECTION_IDLE:
		coordinate = ((uint32_t) players[i].screen_position.y << 16) | players[i].screen_position.x;
                state |= ((0x0 & 0x3) | (players[i].alive << 2));
                break;
        }

        if (i==0){
            color.p1_coordinate = coordinate;
            color.p1_state = state;
            //set_background_color(&color);
        }
        else{
            color.p2_coordinate = coordinate;
            color.p2_state = state;
            //set_background_color(&color);
        }
        set_background_color(&color);
        usleep(300000);

        printf("player %d, x position = %d, y position = %d, dir = %u\n", i, players[i].screen_position.x, players[i].screen_position.y, players[i].move_direction);
	
        /*
        uint8_t anim_frame_offset = num_of_frames * i;

        if (players[i].moving) {
            players[i].current_frame = ((players[i].current_frame + 1) % num_of_frames) + anim_frame_offset;
        } else {
            players[i].current_frame = anim_frame_offset;
        }
        drawAnimFrame(players[i].screen_position.x  + OFFSET_X, players[i].screen_position.y + OFFSET_Y, player_sprite, players[i].current_frame);
        */
    }
}


player_id playRound(void) {
    //setupRound(); 

    int frames = 0;
    int players_alive = PLAYER_NUM;
    int final_countdown_count = 0;
    color.p1_bomb = 0x100;

    
    while(players_alive > 1) {

        updateControls();
    /*
        if (frames >= FRAMES_IN_A_ROUND) {
            if ((frames % FINAL_COUNTDOWN_RATE) == 0) {
                finalCountdown(final_countdown_count);
                final_countdown_count++;
            }
        } else {
            updateTimebar((double) ((double) frames / (double) FRAMES_IN_A_ROUND));
        }
    */
	set_background_color(&color);
        applyPlayerInput();
        //applyAIMovement();
        /*
        if (applyItems()) {
            snd_play_pickup();
        }
    */
   /*
        countdownExplosions();
        if (plantBombs()) {
            snd_play_plant();
        }
        */
        /*
        if (countdownBombs()) {
            snd_play_explosion();
        }
        */

        render();
        //players_alive = countPlayers();
        //frames++;
        //update_sound();
    }

    //return getWinner();
}


void applyPlayerInput(void) {

    if (controller1.ab == 47 | controller1.ab == 63 | controller1.ab == 175 | controller1.ab == 191) {
        players[PLAYER_ONE].plant_bomb = true;
    }
  
    if (controller1.dir_x == 255) {
        move(&players[PLAYER_ONE], DIRECTION_RIGHT);
    } else if (controller1.dir_y == 0) {
        move(&players[PLAYER_ONE], DIRECTION_UP);
    } else if (controller1.dir_x == 0) {
        move(&players[PLAYER_ONE], DIRECTION_LEFT);
    } else if (controller1.dir_y == 255) {
        move(&players[PLAYER_ONE], DIRECTION_DOWN);
    } else {
        move(&players[PLAYER_ONE], DIRECTION_IDLE);
    }
  
    if (controller2.ab == 47 | controller2.ab == 63 | controller2.ab == 175 | controller2.ab == 191) {
        players[PLAYER_TWO].plant_bomb = true;
    }
  
    if (controller2.dir_x == 255) {
        move(&players[PLAYER_TWO], DIRECTION_RIGHT);
    } else if (controller2.dir_y == 0) {
        move(&players[PLAYER_TWO], DIRECTION_UP);
    } else if (controller2.dir_x == 0) {
        move(&players[PLAYER_TWO], DIRECTION_LEFT);
    } else if (controller2.dir_y == 255) {
        move(&players[PLAYER_TWO], DIRECTION_DOWN);
    } else {
        move(&players[PLAYER_TWO], DIRECTION_IDLE);
    }

    
}

void render(void) {

    /* Render the tiles that have changed */
    for (int i = 0; i < MAP_SIZE_H; i++) {
        for (int j = 0; j < MAP_SIZE_V; j++) {
		redrawTile(i, j);
	/*
            if (changed_tiles[i][j]) {
                redrawTile(i, j);
                changed_tiles[i][j] = false;
            }
		*/
        }
    }
    
    drawPlayers();

    /* Call renderFrame at the end of each frame to draw results on screen */
    //renderFrame();
}

void redrawTile(uint32_t x, uint32_t y) {
    /* Draw terrain */
    if (x >= MAP_SIZE_H || y >= MAP_SIZE_V) { return; }

    uint32_t bomb_coordinate = 0;
    /* Draw ground regardless */
    //drawAnimFrame(x * TILE_SIZE + OFFSET_X, y * TILE_SIZE + OFFSET_Y, a_terrain, (x+y)%2);

    switch (terrain_grid[x][y]) {
        case TERRAIN_GROUND:
            break;
        case TERRAIN_WALL_BREAKABLE:

            break;
        case TERRAIN_WALL_UNBREAKABLE:
            //drawAnimFrame(x * TILE_SIZE + OFFSET_X, y * TILE_SIZE + OFFSET_Y, a_terrain, 2);
            break;
    }
/*
    switch (bomb_grid[x][y].type) {
	case BOMB_TYPE_NORMAL:
		bomb_coordinate = ((y << 10) | x);
		break;
	case BOMB_EMPTY:
		break;
	}
	color.p1_bomb = bomb_coordinate;
        set_background_color(&color);
*/
    /* Draw items */
    /*
    switch (items_grid[x][y]) {
        case ITEM_SPEED_UP:
            drawAnimFrame(x * TILE_SIZE + OFFSET_X, y * TILE_SIZE + OFFSET_Y, a_item_icons, ITEM_SPEED_UP);
            break;
        case ITEM_SPEED_DOWN:
            drawAnimFrame(x * TILE_SIZE + OFFSET_X, y * TILE_SIZE + OFFSET_Y, a_item_icons, ITEM_SPEED_DOWN);
            break;
        case ITEM_RANGE_UP:
            drawAnimFrame(x * TILE_SIZE + OFFSET_X, y * TILE_SIZE + OFFSET_Y, a_item_icons, ITEM_RANGE_UP);
            break;
        case ITEM_RANGE_DOWN:
            drawAnimFrame(x * TILE_SIZE + OFFSET_X, y * TILE_SIZE + OFFSET_Y, a_item_icons, ITEM_RANGE_DOWN);
            break;
        case ITEM_BOMBS_UP:
            drawAnimFrame(x * TILE_SIZE + OFFSET_X, y * TILE_SIZE + OFFSET_Y, a_item_icons, ITEM_BOMBS_UP);
            break;
        case ITEM_BOMBS_DOWN:
            drawAnimFrame(x * TILE_SIZE + OFFSET_X, y * TILE_SIZE + OFFSET_Y, a_item_icons, ITEM_BOMBS_DOWN);
            break;
        
        default:
            / This shouldn't happen /
            break;
    }
    */
    

    /* Draw bombs */
    int32_t timer = bomb_grid[x][y].timer;
    //uint8_t* bomb_sprite = a_bomb_neutral;
    /*
    if (timer <= BOMB_EXPLODING_THRESHOLD) {
        bomb_sprite = a_bomb_exploding;
    }
    */

    switch (bomb_grid[x][y].type) {
        case BOMB_EMPTY:
            break;
        case BOMB_TYPE_NORMAL:
            //drawAnimFrame(x * TILE_SIZE + OFFSET_X, y * TILE_SIZE + OFFSET_Y, bomb_sprite, bomb_grid[x][y].current_frame);
            break;
        
        default:
            /* This shouldn't happen */
            break;
    }

    /* Draw explosions */
    switch (explosion_grid[x][y].type) {
        case EXPLOSION_EMPTY:
            break;
        case EXPLOSION_TYPE_NORMAL:
            //drawSprite(x * TILE_SIZE + OFFSET_X, y * TILE_SIZE + OFFSET_Y, a_explosion);
            break;

        case EXPLOSION_TYPE_PERMANENT:
            //drawSprite(x * TILE_SIZE + OFFSET_X, y * TILE_SIZE + OFFSET_Y, a_explosion);
            break;
        
        default:
            /* This shouldn't happen */
            break;
    }
}

void move(Player *player, direction dir) {

    /* Check for out of bounds movement */
    
    if (walkingOutOfBounds(player, dir)) {
        dir = DIRECTION_IDLE;
    }
	

    //player.move_direction = dir;
    Position pixel_offset = {0, 0};
    Position next_pos = {0, 0};
    getOffsets(&next_pos, &pixel_offset, player, dir);

    bool aligned = isAligned(player->screen_position.x, player->screen_position.y);

    Position tile1 = {0, 0};
    Position tile2 = {0, 0};
    getOccupiedTiles(player, &tile1, &tile2);

    /* Mark tiles for rendering */
    changed_tiles[tile1.x][tile1.y] = true;
    changed_tiles[tile2.x][tile2.y] = true;

    /* When joystick is not moving but player is still mid movement OR when trying to change directions */
    if (dir == DIRECTION_IDLE || dir != player->move_direction) {
        if (!aligned) {
            bool snappedToGrid = snapToGrid(player, dir);
            if (snappedToGrid) {
                return;
            }

            /* Move normally if not */
            int32_t temp_x = player->screen_position.x + next_pos.x * player->move_speed;
            int32_t temp_y = player->screen_position.y + next_pos.y * player->move_speed;
            clamp_x(&temp_x);
            clamp_y(&temp_y);

            setPlayerPosition(player, temp_x, temp_y);
        } else {
            /* We are grid-aligned -> can change directions now (or stop moving) */
            if (dir != player->move_direction && dir != DIRECTION_IDLE) {
                player->move_direction = dir;
            } else {
                player->moving = false;
            }
        }
        setPlayerTilePos(player);
        return;
    }

    /* Check if we're stepping onto another tile */
    for (int i = 0; i <= player->move_speed; i++) {
        int32_t temp_x = player->screen_position.x + next_pos.x * i + pixel_offset.x;
        int32_t temp_y = player->screen_position.y + next_pos.y * i + pixel_offset.y;
        clamp_x(&temp_x);
        clamp_y(&temp_y);

        if (isAligned(temp_x, temp_y)) {

            /* We found another tile */
            Position temp;
            temp.x = temp_x;
            temp.y = temp_y;
            screenToTile(&temp);
            
            /* Stop if we can't walk on it */
            if (!checkWalkable(temp.x, temp.y)) {
                setPlayerPosition(player, player->screen_position.x + next_pos.x * i, player->screen_position.y + next_pos.y * i);
                player->moving = false;
                player->move_direction = dir;
                setPlayerTilePos(player);
                return;
            }

            break;
        }
    }

    /* No other case, so just keep moving */
    int32_t temp_x = player->screen_position.x + next_pos.x * player->move_speed;
    int32_t temp_y = player->screen_position.y + next_pos.y * player->move_speed;
    clamp_x(&temp_x);
    clamp_y(&temp_y);

    setPlayerPosition(player, temp_x, temp_y);

    player->moving = true;
    player->move_direction = dir;
    setPlayerTilePos(player);
}

/* Returns the next set of coordinates w.r.t. the given direction, in terms of 
 * -1 and +1 values */
void getNextStep(Position* pos, direction dir) {
    switch (dir) {
        case DIRECTION_UP:
            pos->y = -1;
            break;
        case DIRECTION_DOWN:
            pos->y = 1;
            break;
        case DIRECTION_LEFT:
            pos->x = -1;
            break;
        case DIRECTION_RIGHT:
            pos->x = 1;
            break;
        default:
            break;
    }
}

void getOffsets(Position* next_pos, Position* pixel_offset, Player* player, direction dir) {
    /* Calculate offset for the 'front' of the character */
    getPixelOffset(pixel_offset, dir);

    /* Get pixel values of the next step */
    getNextStep(next_pos, player->move_direction);

}

/* Snaps the player screen position to the grid. Call this function if
 * (1) The player direction is changing OR
 * (2) The player is stopping 
 * Returns whether the snapping was successful within this frame */
bool snapToGrid(Player* player, direction dir) {

    Position pixel_offset = {0, 0};
    Position next_pos = {0, 0};
    getOffsets(&next_pos, &pixel_offset, player, dir);

    for (int i = 0; i <= player->move_speed; i++) {
        int32_t temp_x = player->screen_position.x + next_pos.x * i + pixel_offset.x;
        int32_t temp_y = player->screen_position.y + next_pos.y * i + pixel_offset.y;
        clamp_x(&temp_x);
        clamp_y(&temp_y);

        if (isAligned(temp_x, temp_y)) {
            setPlayerPosition(player, player->screen_position.x + next_pos.x * i, player->screen_position.y + next_pos.y * i);

            /* If the direction is changing, change the player attribute */
            if (dir != player->move_direction && dir != DIRECTION_IDLE) {
                player->move_direction = dir;
            } else {
                /* Otherwise we're stopping */
                player->moving = false;
            }
            setPlayerTilePos(player);
            return true;
        }
    }
    return false;
}

/* Returns whether tile at given *tile* position can be walked on */
bool checkWalkable(int32_t x, int32_t y) {

    if (x < 0 || y < 0 || x >= MAP_SIZE_H || y >= MAP_SIZE_V) { return false; }
    if (bomb_grid[x][y].type != BOMB_EMPTY) { return false; } 
    if (terrain_grid[x][y] != TERRAIN_GROUND) { return false; }

    return true;
}

/* Returns whether given *screen* position is aligned with the tile grid */
bool isAligned(int32_t x, int32_t y) {
    return (x % TILE_SIZE == 0) && (y % TILE_SIZE == 0);
}

/* Gets tile coordinates of tiles that are occupied by a player. If the player is 
 * grid-aligned, tile1 will have the same coordinates as tile2 */
void getOccupiedTiles(Player* player, Position* tile1, Position* tile2) {

    if (isAligned(player->screen_position.x, player->screen_position.y)) {
        /* Aligned with grid */
        tile1->x = player->screen_position.x;
        tile1->y = player->screen_position.y;
        screenToTile(tile1);

        tile2->x = tile1->x;
        tile2->y = tile1->y;
    } else {
        int32_t offset_x = player->screen_position.x % TILE_SIZE;
        int32_t offset_y = player->screen_position.y % TILE_SIZE;

        /* First move backwards */
        tile1->x = player->screen_position.x - offset_x;
        tile1->y = player->screen_position.y - offset_y;

        screenToTile(tile1);

        /* Then forwards */
        if (offset_x == 0) {
            tile2->x = player->screen_position.x;
            tile2->y = player->screen_position.y + TILE_SIZE - offset_y;
        } else {
            tile2->y = player->screen_position.y;
            tile2->x = player->screen_position.x + TILE_SIZE - offset_x;
        }

        screenToTile(tile2);
    }
}

/* Gets offset in screen position when trying to check 'in front' */
void getPixelOffset(Position* result, direction dir) {
    switch (dir) {
        case DIRECTION_UP:
            result->x = 0;
            result->y = -TILE_SIZE;
            break;
        case DIRECTION_LEFT:
            result->x = -TILE_SIZE;
            result->y = 0;
            break;
        case DIRECTION_IDLE:
            result->x = 0;
            result->y = 0;
            break;
        case DIRECTION_RIGHT:
            result->x = TILE_SIZE;
            result->y = 0;
            break;
        case DIRECTION_DOWN:
            result->x = 0;
            result->y = +TILE_SIZE; 
            break;
    }
}

/* Sets the player tile_position attribute to the closest tile
 * PRE: The player is always aligned in at least one dimension */
void setPlayerTilePos(Player* player) {
    int32_t offset_x = player->screen_position.x % TILE_SIZE;
    int32_t offset_y = player->screen_position.y % TILE_SIZE;

    Position result;

    if (offset_x == 0) {
        result.x = player->screen_position.x;

        /* Get the closer tile */
        if (offset_y < TILE_SIZE / 2) {
            result.y = player->screen_position.y - offset_y;
        } else {
            result.y = player->screen_position.y + TILE_SIZE - offset_y;
        }
    } else {
        result.y = player->screen_position.y;

        /* Get the closer tile */
        if (offset_x < TILE_SIZE / 2) {
            result.x = player->screen_position.x - offset_x;
        } else {
            result.x = player->screen_position.x + TILE_SIZE - offset_x;
        }
    }

    screenToTile(&result);

    player->tile_position.x = result.x;
    player->tile_position.y = result.y;
}

/* Helper function to convert from screen position to tile position */
void screenToTile(Position *pos) {
    pos->x /= TILE_SIZE;
    pos->y /= TILE_SIZE;

    if (pos->x < 0) {
        pos->x = 0;
    } else if (pos->x >= MAP_SIZE_H) {
        pos->x = MAP_SIZE_H - 1;
    }

    if (pos->y < 0) {
        pos->y = 0;
    } else if (pos->y >= MAP_SIZE_V) {
        pos->y = MAP_SIZE_V - 1;
    }
}

/* Safe way of setting player screen_coordinates */
void setPlayerPosition(Player* player, int32_t x, int32_t y) {

    /* Clamp coordinates */
    clamp_x(&x);
    clamp_y(&y);

    player->screen_position.x = x;
    player->screen_position.y = y;
}

void clamp_x(int32_t* i) {
    if (*i < 0) {
        *i = 0; 
    } else if (*i > (MAP_SIZE_H - 1) * TILE_SIZE) { 
        *i = (MAP_SIZE_H - 1) * TILE_SIZE; 
    }
}

void clamp_y(int32_t* i) {
    if (*i < 0) {
        *i = 0; 
    } else if (*i > (MAP_SIZE_V - 1) * TILE_SIZE) { 
        *i = (MAP_SIZE_V - 1) * TILE_SIZE; 
    }
}

bool walkingOutOfBounds(Player* player, direction dir) {
    if (player->tile_position.x == 0 && dir == DIRECTION_LEFT) {
        return true;
    }
    if (player->tile_position.y == 0 && dir == DIRECTION_UP) {
        return true;
    }
    if (player->screen_position.x >= (MAP_SIZE_H - 1) * TILE_SIZE - player->move_speed && dir == DIRECTION_RIGHT) {
        player->screen_position.x = (MAP_SIZE_H - 1) * TILE_SIZE;
        return true;
    }
    if (player->screen_position.y >= (MAP_SIZE_V - 1) * TILE_SIZE - player->move_speed && dir == DIRECTION_DOWN) {
        player->screen_position.y = (MAP_SIZE_V - 1) * TILE_SIZE;
        return true;
    }
    return false;
}

void countdownExplosions(void) {

    for (int x = 0; x < MAP_SIZE_H; x++) {
        for (int y = 0; y < MAP_SIZE_V; y++) {

            Explosion *explosion = &explosion_grid[x][y];

            if (explosion->type == EXPLOSION_TYPE_NORMAL) {
                explosion->timer--;

                if (explosion->timer == 0) {
                    explosion_grid[x][y].type = EXPLOSION_EMPTY; 

                    /* Set the terrain on this tile to be ground - destroy breakable wall */
                    terrain_grid[x][y] = TERRAIN_GROUND;


                    /* Update the changed tiles so explosion is no longer displayed */
                    changed_tiles[x][y] = true;
                }
            }
        }
    }

    killPlayersInExplosion();

}

bool plantBombs(void) {

    bool bomb_planted = false;
    for (int i = 0; i < PLAYER_NUM; i++) {
        if (players[i].plant_bomb) {
            players[i].plant_bomb = false;
            plantBomb(&(players[i]));
            bomb_planted = true;
        }
    }

    return bomb_planted;
}


bool countdownBombs(void) {
    bool bomb_exploded = false;

    for (int x = 0; x < MAP_SIZE_H; x++) {
        for (int y = 0; y < MAP_SIZE_V; y++) {

            Bomb *bomb = &bomb_grid[x][y];

            if (bomb->type != BOMB_EMPTY) {
                bomb->timer--;

                if ((bomb->timer) == 0) {
                    explodeBomb(bomb);
                    bomb_exploded = true;
                } else if ((bomb->timer) % BOMB_ANIMATION_CYCLES == 0) {
                    /* Update the changed tiles so bomb is rendered
                     * to indicate imminient explostion */
                    bomb->current_frame = !(bomb->current_frame);
                    changed_tiles[x][y] = true;
                }
            }
        }
    }

    killPlayersInExplosion();
    return bomb_exploded;
}

void plantBomb(Player *player) {

    /* Get the coordinates for convenience */
    int8_t x = player->tile_position.x;
    int8_t y = player->tile_position.y;

    /* Check player is alive, has enough bombs and is not standing on a bomb */
    if ((!(player->alive)) || 
        (player->current_bomb_number >= player->max_bomb_number) ||
        (bomb_grid[x][y].type != BOMB_EMPTY)) { return; }

    /* Copy the player's bomb template into the bomb grid and set its position */
    Bomb bomb = player->bomb;
    bomb.position = player->tile_position;
    bomb_grid[x][y] = bomb;

    /* Update the changed tiles so bomb is rendered */
    changed_tiles[x][y] = true;

    player->current_bomb_number++;
}

void explodeBomb(Bomb *bomb) {

    /* Check bomb is not empty */
    if (bomb->type == BOMB_EMPTY) { return; }

    /* Get the coordinates, range and explosion for convenience */
    int8_t x = bomb->position.x;
    int8_t y = bomb->position.y;
    int8_t range = bomb->range;
    Explosion *explosion = &(bomb->explosion);
    
    /* Decrement corresponding player bomb count */
    //players[bomb->owner].current_bomb_number--;

    /* Remove bomb and explode tile */
    bomb_grid[x][y].type = BOMB_EMPTY;
    explodeTile(x, y, explosion);

    /* Explode tiles on the horizontal and vertical that are in range but
     * don't explode past an unbreakable wall */

    bool left_blocked  = false;
    bool right_blocked = false;
    bool up_blocked    = false;
    bool down_blocked  = false;

    for (int i = 1; i <= range; i++) {

        if (!(up_blocked)) {
            switch (terrain_grid[x][y + i]) {
                case TERRAIN_WALL_UNBREAKABLE:
                    up_blocked = true;
                    break;
                case TERRAIN_WALL_BREAKABLE:
                    explodeTile(x, y + i, explosion);
                    up_blocked = true;
                    break;
                case TERRAIN_GROUND:
                    explodeTile(x, y + i, explosion);
                    break;
            }
        }

        if (!(down_blocked)) {
            switch (terrain_grid[x][y - i]) {
                case TERRAIN_WALL_UNBREAKABLE:
                    down_blocked = true;
                    break;
                case TERRAIN_WALL_BREAKABLE:
                    explodeTile(x, y - i, explosion);
                    down_blocked = true;
                    break;
                case TERRAIN_GROUND:
                    explodeTile(x, y - i, explosion);
                    break;
            }
        }

        if (!(right_blocked)) {
            switch (terrain_grid[x + i][y]) {
                case TERRAIN_WALL_UNBREAKABLE:
                    right_blocked = true;
                    break;
                case TERRAIN_WALL_BREAKABLE:
                    explodeTile(x + i, y, explosion);
                    right_blocked = true;
                    break;
                case TERRAIN_GROUND:
                    explodeTile(x + i, y, explosion);
                    break;
            }
        }

        if (!(left_blocked)) {
            switch (terrain_grid[x - i][y]) {
                case TERRAIN_WALL_UNBREAKABLE:
                    left_blocked = true;
                    break;
                case TERRAIN_WALL_BREAKABLE:
                    explodeTile(x - i, y, explosion);
                    left_blocked = true;
                    break;
                case TERRAIN_GROUND:
                    explodeTile(x - i, y, explosion);
                    break;
            }
        }
    }
}

void explodeTile(int8_t x, int8_t y, Explosion *explosion) {
    bool isCoordinateInRange(int8_t x, int8_t y) {
    	return ((x >= 0) && (x < MAP_SIZE_H)  &&  (y >= 0) && (y < MAP_SIZE_V));
    }

    /* Check position is valid, the explosion type is not empty and there is not
     * an unbreakable wall */
    if ((!isCoordinateInRange(x, y)) ||
        (explosion->type == EXPLOSION_EMPTY) ||
        (terrain_grid[x][y] == TERRAIN_WALL_UNBREAKABLE)) { return; }

    /* Set the explosion, overwriting any existing explosions */
    if (explosion_grid[x][y].type != EXPLOSION_TYPE_PERMANENT) {
        explosion_grid[x][y] = *explosion;
    }

    /* Set any bombs on this tile to explode on the next round */
    bomb_grid[x][y].timer = 1;

    /* Destroy any item on this tile, unless covered by a breakable wall */
    /*
    if (terrain_grid[x][y] != TERRAIN_WALL_BREAKABLE) {
        items_grid[x][y] = ITEM_EMPTY;
    } else {
        items_grid[x][y] = generateRandomItem();
    }
    */
    /* Update the changed tiles so changes are rendered */
    changed_tiles[x][y] = true;

}

void killPlayersInExplosion(void) {
    
    Position pos_1 = {0, 0};
    Position pos_2 = {0, 0};

    for (int i = 0; i < PLAYER_NUM; i++) {
        if (explosion_grid[players[i].tile_position.x][players[i].tile_position.y].type != EXPLOSION_EMPTY) {
            players[i].alive = false;

            getOccupiedTiles(&(players[i]), &pos_1, &pos_2);

            /* Render both tiles the player occupied */
            changed_tiles[pos_1.x][pos_1.y] = true;
            changed_tiles[pos_2.x][pos_2.y] = true;

            /* Render changes in sidebar */
	    /*
            drawPlayerIcons();
            drawPlayerScores();
	    */
        }
    }
}




