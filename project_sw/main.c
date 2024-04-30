
#include "main.h"


struct controller_list open_controllers();

int vga_ball_fd;

unsigned short coords;
unsigned short coords2;

vga_ball_color_t color;

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
        {1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0},
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




    

    for(;;){
        runGame();
    }
    

}

void runGame(void){

    initialisePlayers();
    setupRound();

    while(ready_screen()){
        player_id winner = playRound();

        for(int j = 0; j < PLAYER_NUM; j++) {
            if(winner == players[j].id) {
                players[j].number_of_wins++;
            }
        }

    }


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

    while(player1_ready || player2_ready) {
        updateControls();
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
    return true;

}

void updateControls(void) {

    libusb_interrupt_transfer(devices.device1, devices.device1_addr, (unsigned char *) &controller1, size1, &fields1, 0);
    libusb_interrupt_transfer(devices.device2, devices.device2_addr, (unsigned char *) &controller2, size2, &fields2, 0);
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

void drawPlayers(void) {
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
                break;
        
        if (i==0){
            color.p1_coordinate = coordinate;
            color.p1_state = state;
            set_background_color(&color);
        }
        else{
            color.p2_coordinate = coordinate;
            color.p2_state = state;
            set_background_color(&color);
        }
        }
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
    setupRound(); 

    int frames = 0;
    int players_alive = PLAYER_NUM;
    int final_countdown_count = 0;
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
        players_alive = countPlayers();
        //frames++;
        //update_sound();
    }

    return getWinner();
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
  
    if (controller1.ab == 47 | controller1.ab == 63 | controller1.ab == 175 | controller1.ab == 191) {
        players[PLAYER_TWO].plant_bomb = true;
    }
  
    if (controller1.dir_x == 255) {
        move(&players[PLAYER_TWO], DIRECTION_RIGHT);
    } else if (controller1.dir_y == 0) {
        move(&players[PLAYER_TWO], DIRECTION_UP);
    } else if (controller1.dir_x == 0) {
        move(&players[PLAYER_TWO], DIRECTION_LEFT);
    } else if (controller1.dir_y == 255) {
        move(&players[PLAYER_TWO], DIRECTION_DOWN);
    } else {
        move(&players[PLAYER_TWO], DIRECTION_IDLE);
    }
}

void render(void) {

    /* Render the tiles that have changed */
    for (int i = 0; i < MAP_SIZE_H; i++) {
        for (int j = 0; j < MAP_SIZE_V; j++) {
            if (changed_tiles[i][j]) {
                redrawTile(i, j);
                changed_tiles[i][j] = false;
            }
        }
    }

    drawPlayers();

    /* Call renderFrame at the end of each frame to draw results on screen */
    //renderFrame();
}

void redrawTile(uint32_t x, uint32_t y) {
    /* Draw terrain */
    if (x >= MAP_SIZE_H || y >= MAP_SIZE_V) { return; }

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

