
#include "main.h"


struct controller_list open_controllers();

int vga_ball_fd;

unsigned short coords;
unsigned short coords2;

vga_ball_color_t color = {0};

pthread_t p1b, p2b;

// TODO : add a 0(ground), 1(unbreakable), 2(breakable) map to this
/*
int map[40][30] = {
    {0, 1, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0},
    {0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 1, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0},
    {0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0},
    {0, 0, 2, 0, 0, 0, 0, 0, 0, 2, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0},
    {0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
};
*/

Terrain terrain_grid[MAP_SIZE_H][MAP_SIZE_V];
Bomb bomb_grid[MAP_SIZE_H][MAP_SIZE_V];
//item_type items_grid[MAP_SIZE_H][MAP_SIZE_V];
Explosion explosion_grid[MAP_SIZE_H][MAP_SIZE_V];
bool changed_tiles[MAP_SIZE_H][MAP_SIZE_V];
Player players[PLAYER_NUM];

int map[MAP_SIZE_H][MAP_SIZE_V];


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



    float ratio0 = 0.5, ratio1 = 0.3, ratio2 = 0.2; // 0 for ground, 1 for unbreakable wall, 2 for breakable wall

    generateMap(map, ratio0, ratio1, ratio2);

    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            map[i][j] = 0;
        }
        
    }
    for (int i = MAP_SIZE_H-3; i < MAP_SIZE_H; i++)
    {
        for (int j = MAP_SIZE_V-3; j < MAP_SIZE_V; j++)
        {
            map[i][j] = 0;
        }
        
    }
    

    initialisemap();
    initialisePlayers();
    setupRound();

    

    for(;;){
        runGame();
    }
    

}


void generateMap(int matrix[MAP_SIZE_H][MAP_SIZE_V], float ratio0, float ratio1, float ratio2) {
    int totalElements = MAP_SIZE_H * MAP_SIZE_V;
    int count0 = totalElements * ratio0;
    int count1 = totalElements * ratio1;
    int count2 = totalElements * ratio2;

    // Create an array with the correct number of 0s, 1s, and 2s
    int *array = (int *)malloc(totalElements * sizeof(int));
    int index = 0;
    
    for (int i = 0; i < count0; i++) array[index++] = 0;
    for (int i = 0; i < count1; i++) array[index++] = 1;
    for (int i = 0; i < count2; i++) array[index++] = 2;

    // Shuffle the array
    srand(time(NULL));
    for (int i = totalElements - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        int temp = array[i];
        array[i] = array[j];
        array[j] = temp;
    }

    // Fill the matrix with the shuffled array
    index = 0;
    for (int i = 0; i < MAP_SIZE_H; i++) {
        for (int j = 0; j < MAP_SIZE_V; j++) {
            matrix[i][j] = array[index++];
        }
    }

    // Free the allocated memory
    free(array);
}



void runGame(void){


    while (readyScreen()) {
        player_id winner = playRound();
   }
    //initialisePlayers();
    //setupRound();
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


void initialisemap(void){
    uint32_t map_info;
    uint32_t map_address;
    for (int i = 0; i < MAP_SIZE_H; i++) {
        for (int j = 0; j < MAP_SIZE_V; j++) {
            color.map_info = 0x0;
            map_info = 0x0;
            map_info |= 0x80000000;
            map_info |= 0x40000000;
            map_info |= map[i][j];
            map_address = i + j*40;
            map_info |= (map_address << 19);
            color.map_info |= map_info;
            set_background_color(&color);
        }
    }
    map_info &= 0x80000000;

}


void initialisePlayers(void) {
    for (int i = 0; i < PLAYER_NUM; i++) {
        players[i].id = i;
        players[i].number_of_wins = 0;
	    players[i].alive = true;
	    players[i].plant_bomb = false;

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
        uint32_t mask = 1 << 31;
        
        if(!player1_ready && (p1_ab == 79 || p1_ab == 95 || p1_ab == 207 || p1_ab == 223)) {
            //drawPlayerReady(PLAYER_ONE);
            color.p1_state |= mask;
            player1_ready = true;
            //snd_play_pickup();
            set_background_color(&color);
        }
        if(!player2_ready && (p2_ab == 79 || p1_ab == 95 || p1_ab == 207 || p1_ab == 223)) {
            //drawPlayerReady(PLAYER_TWO);
            color.p2_state |= mask;
            player2_ready = true;
            set_background_color(&color);
            //snd_play_pickup();
        }
        //update_sound();
    }

/*
    while(player1_ready || player2_ready) {
        updateControls();
        
        if(player1_ready && controller_signals[CONTROLLER_ONE_IND_TRIGGER] == 0) {
            //drawPlayerReady(PLAYER_ONE);
            player1_ready = false;
        }
        if(player2_ready && controller_signals[CONTROLLER_TWO_IND_TRIGGER] == 0) {
            //drawPlayerReady(PLAYER_TWO);
            player2_ready = false;
        }
        //update_sound();
        
    }
    */
    return true;

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
                terrain_grid[i][j] = TERRAIN_WALL_UNBREAKABLE;
                break;
            case 2:
                terrain_grid[i][j] = TERRAIN_WALL_BREAKABLE;
            }
            

            bomb_grid[i][j].type = BOMB_EMPTY;

            bomb_grid[i][j].timer = 0;
            bomb_grid[i][i].used = 0;
            bomb_grid[i][j].range = 0;

            explosion_grid[i][j].type = EXPLOSION_EMPTY;
            explosion_grid[i][j].timer = 0;
            explosion_grid[i][j].up = 0;
            explosion_grid[i][j].down = 0;
            explosion_grid[i][j].left = 0;
            explosion_grid[i][j].right = 0;

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
	    players[i].id = 0;
        }
        else
        {
            players[i].screen_position.x = (MAP_SIZE_H - 1) * TILE_SIZE + 1;
            players[i].screen_position.y = (MAP_SIZE_V - 1) * TILE_SIZE + 1;
	    players[i].id = 1;
        }
        
        
        players[i].alive = true;

        players[i].bomb.owner = players[i].id;
        players[i].bomb.timer = DEFAULT_BOMB_TIMER;
        players[i].bomb.range = DEFAULT_BOMB_RANGE;
        players[i].bomb.type  = DEFAULT_BOMB_TYPE;
	    players[i].bomb.owner = players[i].id;
        players[i].bomb.current_frame  = 0;
        players[i].bomb.explosion.type = EXPLOSION_TYPE_NORMAL;
        players[i].bomb.explosion.timer = 0;
        players[i].bomb.explosion.up = 0;
        players[i].bomb.explosion.down = 0;
        players[i].bomb.explosion.right = 0;
        players[i].bomb.explosion.left = 0;

        players[i].bomb.used = 0;

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
            color.p1_state &= ~0x7;
            color.p1_coordinate = coordinate;
            color.p1_state |= state;
            //set_background_color(&color);
        }
        else{
            color.p2_state &= ~0x7;
            color.p2_coordinate = coordinate;
            color.p2_state |= state;
            //set_background_color(&color);
        }
        set_background_color(&color);
        usleep(30000);

        //printf("player %d, x position = %d, y position = %d, dir = %u\n", i, players[i].screen_position.x, players[i].screen_position.y, players[i].move_direction);
	
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
    //color.p1_bomb = 0x100;

    
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
   

    
        if (plantBombs()) {
            //snd_play_plant();
        }

        
        if (countdownBombs()) {
            //snd_play_explosion();
        }

	countdownExplosions();
        

        render();
        //players_alive = countPlayers();
        //frames++;
        //update_sound();
    }

    exit(1);
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
    uint32_t map_info = 0;
    uint32_t map_address = 0;
    /* Draw ground regardless */
    //drawAnimFrame(x * TILE_SIZE + OFFSET_X, y * TILE_SIZE + OFFSET_Y, a_terrain, (x+y)%2);

    switch (terrain_grid[x][y]) {
        case TERRAIN_GROUND:
            color.map_info &= 0x80000000;

            //color.map_info = 0;
            //set_background_color(&color);
            /*
            if (x== && y==2)
            {
            
            }
            */
            //printf("in the ground: map_info = %d, x= %d, y = %d\n", color.map_info, x, y);
            break;
        case TERRAIN_WALL_BREAKABLE:

            break;
        case TERRAIN_WALL_UNBREAKABLE:
            //drawAnimFrame(x * TILE_SIZE + OFFSET_X, y * TILE_SIZE + OFFSET_Y, a_terrain, 2);
            break;
        case TERRAIN_WALL_BREAKABLE_B:
            map_info |= 0x80000000;
            map_info |= 0x40000000;
            map_info |= 0x0;
            map_address = x + y*40;
            map_info |= (map_address << 19);
            color.map_info |= map_info;
            set_background_color(&color);
            
            terrain_grid[x][y] = TERRAIN_GROUND;
            printf("breakable: map_info = %d, x= %d, y = %d\n", color.map_info, x, y);
            usleep(20);

            break;
    }

    switch (bomb_grid[x][y].type) {
	case BOMB_TYPE_NORMAL:
		//bomb_coordinate = (((y << 4 - y) << 10) | (x << 4 - x));
		bomb_coordinate = ((y * TILE_SIZE) << 10 | x * TILE_SIZE );
        if((!bomb_grid[x][y].used) && (bomb_grid[x][y].owner == 0) && ((color.p1_state & 0x8) != 0x8 )){
            color.p1_bomb = bomb_coordinate;
            color.p1_state |= 0x8;
            bomb_grid[x][y].used = 1;
            set_background_color(&color);
        }
        else if((!bomb_grid[x][y].used) && (bomb_grid[x][y].owner == 1) && ((color.p2_state & 0x8) != 0x8 ))
            {
            color.p2_bomb = bomb_coordinate;
            color.p2_state |= 0x8;
            bomb_grid[x][y].used = 1;
            set_background_color(&color);
        }
        
		break;
	case BOMB_EMPTY:
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
    
    uint32_t explosion_coordinate = 0;

    switch (explosion_grid[x][y].type) {
	    //printf("explode position = %d\n", explosion_coordinate);
	    //explosion_coordinate = ((y * TILE_SIZE) << 10 | x * TILE_SIZE );
        case EXPLOSION_EMPTY:
            break;
        case EXPLOSION_TYPE_NORMAL:
	    if (explosion_grid[x][y].owner == 0) {
		  explosion_coordinate = ((y * TILE_SIZE) << 10 | x * TILE_SIZE );
                    color.p1_firecenter = explosion_coordinate;
                    color.p1_state |= 0x10;
                    if (explosion_grid[x][y].up == 1)
                    {   

                        explosion_coordinate = (((y-1) * TILE_SIZE) << 10 | x * TILE_SIZE );
                        if (explosion_grid[x][y-1].type != EXPLOSION_TYPE_NORMAL)
                        {
			                explosion_grid[x][y-1].type = EXPLOSION_TYPE_UP;
                        }
                        
			//printf("down cor = %d\n", explosion_coordinate);
			//printf("%d, %d\n", x, y);
                        color.p1_fireup = explosion_coordinate;
                        color.p1_state |= 0x20;
                    }
                    if (explosion_grid[x][y].down == 1)
                    {   
                        explosion_coordinate = (((y+1) * TILE_SIZE) << 10 | x * TILE_SIZE );
			            
                        if (explosion_grid[x][y+1].type != EXPLOSION_TYPE_NORMAL)
                        {
                            explosion_grid[x][y+1].type = EXPLOSION_TYPE_DOWN;        
                        }
			//printf("down cor = %d\n", explosion_coordinate);
			//printf("%d, %d\n", x, y);
                        color.p1_firedown = explosion_coordinate;
                        color.p1_state |= 0x40;
                    }
                    if (explosion_grid[x][y].left == 1)
                    {   
                        explosion_coordinate = ((y * TILE_SIZE) << 10 | (x-1) * TILE_SIZE );
                        if (explosion_grid[x-1][y].type != EXPLOSION_TYPE_NORMAL)
                        {
			                explosion_grid[x-1][y].type = EXPLOSION_TYPE_LEFT;
                        }
                        color.p1_fireleft = explosion_coordinate;
                        color.p1_state |= 0x80;
                    }
                    if (explosion_grid[x][y].right == 1)
                    {   
                        explosion_coordinate = ((y * TILE_SIZE) << 10 | (x+1) * TILE_SIZE );
                        if (explosion_grid[x+1][y].type != EXPLOSION_TYPE_NORMAL)
                        {
			                explosion_grid[x+1][y].type = EXPLOSION_TYPE_RIGHT;
                        }
                        color.p1_fireright = explosion_coordinate;
                        color.p1_state |= 0x100;
                    }
                        set_background_color(&color);
                } else if (explosion_grid[x][y].owner == 1)
                {
		    explosion_coordinate = ((y * TILE_SIZE) << 10 | x * TILE_SIZE );
                    color.p2_firecenter = explosion_coordinate;
                    color.p2_state |= 0x10;
                    if (explosion_grid[x][y].up == 1)
                    {   
                        explosion_coordinate = (((y-1) * TILE_SIZE) << 10 | x * TILE_SIZE );
			explosion_grid[x][y-1].type = EXPLOSION_TYPE_UP;
                        color.p2_fireup = explosion_coordinate;
                        color.p2_state |= 0x20;
                    }
                    if (explosion_grid[x][y].down == 1)
                    {   
                        explosion_coordinate = (((y+1) * TILE_SIZE) << 10 | x * TILE_SIZE );
			explosion_grid[x][y+1].type = EXPLOSION_TYPE_DOWN;
                        color.p2_firedown = explosion_coordinate;
			//printf("player 2\n");
			//printf("%d\n", explosion_grid[x][y].left);
                        color.p2_state |= 0x40;
                    }
                    if (explosion_grid[x][y].left == 1)
                    {   
			//printf(" in the switch");
                        explosion_coordinate = ((y * TILE_SIZE) << 10 | (x-1) * TILE_SIZE );
			explosion_grid[x-1][y].type = EXPLOSION_TYPE_LEFT;
                        color.p2_fireleft = explosion_coordinate;
                        color.p2_state |= 0x80;
			//printf("%d",color.p2_state);
                    }
                    if (explosion_grid[x][y].right == 1)
                    {   
                        explosion_coordinate = ((y * TILE_SIZE) << 10 | (x+1) * TILE_SIZE );
			explosion_grid[x+1][y].type = EXPLOSION_TYPE_RIGHT;
                        color.p2_fireright = explosion_coordinate;
                        color.p2_state |= 0x100;
                    }
                        set_background_color(&color);
                }
            break;
    }

    
    /* Draw explosions */
    /*
    switch (explosion_grid[x][y].type) {
	explosion_coordinate = ((y * TILE_SIZE) << 10 | x * TILE_SIZE );
        case EXPLOSION_EMPTY:
            break;
        case EXPLOSION_TYPE_NORMAL:
	    if (bomb_grid[x][y].owner == PLAYER_ONE) {
                    color.p1_firecenter = explosion_coordinate;
                    color.p1_state |= 0x16;
		    set_background_color(&color);
                } else {
            color.p2_firecenter = explosion_coordinate;
             color.p2_state |= 0x32;

		    set_background_color(&color);
                }
            break;

        case EXPLOSION_TYPE_UP:
            if (bomb_grid[x][y+1].owner == PLAYER_ONE) {
                    color.p1_fireup = explosion_coordinate;
                    color.p1_state |= 0x;
		    set_background_color(&color);
                } else {
                    color.p2_fireup = explosion_coordinate;
                    color.p2_state |= 0x32;
		    set_background_color(&color);
                }
            break;
	
	case EXPLOSION_TYPE_DOWN:
            if (bomb_grid[x][y-1].owner == PLAYER_ONE) {
                    color.p1_firedown = explosion_coordinate;
                    color.p1_state |= 0x64;
		    set_background_color(&color);
                } else {
                    color.p2_firedown = explosion_coordinate;
                    color.p2_state |= 0x64;
		    set_background_color(&color);
                }
            break;

	case EXPLOSION_TYPE_LEFT:
            if (bomb_grid[x+1][y].owner == PLAYER_ONE) {
                    color.p1_fireleft = explosion_coordinate;
                    color.p1_state |= 0x128;

		    set_background_color(&color);
                } else {
                    color.p2_fireleft = explosion_coordinate;
                    color.p2_state |= 0x128;

		    set_background_color(&color);
                }
            break;

	case EXPLOSION_TYPE_RIGHT:
            if (bomb_grid[x-1][y].owner == PLAYER_ONE) {
                    color.p1_fireright = explosion_coordinate;
                    color.p1_state |= 0x256;
		    set_background_color(&color);
                } else {
                    color.p2_fireright = explosion_coordinate;
                     color.p2_state |= 0x256;

		    set_background_color(&color);
                }
            break;
        
        default:
            break;

    }
    */
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
	    //printf("explosion owner = %d\n", explosion->owner);
	    //printf("explosion timer = %d\n", explosion->timer);
		

        if (explosion->timer == 0) {
            explosion_grid[x][y].type = EXPLOSION_EMPTY;
		    //printf("%d\n", explosion_grid[x][y].type);
		    if(explosion_grid[x][y].up == 1){
			explosion_grid[x][y-1].type = EXPLOSION_EMPTY;
            terrain_grid[x][y-1] = TERRAIN_GROUND;
			//printf("%d\n", explosion_grid[x][y-1].type);
			explosion_grid[x][y].up = 0;
		    }
		    if(explosion_grid[x][y].down == 1){
			explosion_grid[x][y+1].type = EXPLOSION_EMPTY;
            terrain_grid[x][y+1] = TERRAIN_GROUND;

			//printf("%d\n", explosion_grid[x][y+1].type);
			explosion_grid[x][y].down = 0;
		    }
		    if(explosion_grid[x][y].left == 1){
			explosion_grid[x-1][y].type = EXPLOSION_EMPTY;
            terrain_grid[x-1][y] = TERRAIN_GROUND;
			//printf("%d\n", explosion_grid[x-1][y].type);
			explosion_grid[x][y].left = 0;
		    }
		    if(explosion_grid[x][y].right == 1){
			explosion_grid[x+1][y].type = EXPLOSION_EMPTY;
            terrain_grid[x+1][y] = TERRAIN_GROUND;
			//printf("%d\n", explosion_grid[x+1][y].type);
			explosion_grid[x][y].right = 0;
		    }
		  
		  //  printf("explosion owner = %d\n", explosion->owner);
		   int mask = 0xFFFFFE0F;
		   if(explosion->owner == 0){
			color.p1_state &= mask;
		    }
		   else if (explosion->owner == 1){
			color.p2_state &= mask;
			//printf("explosion disapper\n");
		    }
 

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
    //printf("bomb x position = %d, bomb y position = %d, owner = %d\n", x, y, bomb.owner);
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
    //printf("bomb position x = %d, y = %d", x, y);

    
    Explosion *explosion = &(bomb->explosion);
    explosion->owner = bomb->owner;
    explosion->timer = DEFAULT_EXPLOSION_TIMER;
    
    //printf("owner = %d", explosion->owner);
    /*
    Explosion *explosion_L = &(bomb->explosion);
    Explosion *explosion_R = &(bomb->explosion);
    Explosion *explosion_D = &(bomb->explosion);
    Explosion *explosion_U = &(bomb->explosion);

    explosion_L->type = EXPLOSION_TYPE_LEFT;
    explosion_R->type = EXPLOSION_TYPE_RIGHT;
    explosion_U->type = EXPLOSION_TYPE_UP;
    explosion_D->type = EXPLOSION_TYPE_DOWN;
    */


    
    /* Decrement corresponding player bomb count */
    players[bomb->owner].current_bomb_number--;

    /* Remove bomb and explode tile */
    bomb_grid[x][y].type = BOMB_EMPTY;
    unsigned int mask = ~(1 << 3); 
    if (bomb_grid[x][y].owner == 0)
    {
        color.p1_bomb = 0x0;
        color.p1_state &= mask;
        bomb_grid[x][y].used = 0;
        set_background_color(&color);
    }
    else if (bomb_grid[x][y].owner == 1)
    {
        color.p2_bomb = 0x0;
        color.p2_state &= mask;
        bomb_grid[x][y].used = 0;
        set_background_color(&color);
    }
    

    explosion_grid[x][y] = *explosion;
    //printf("bomb owner = %d, explosion owner = %d", bomb_grid[x][y].owner, explosion_grid[x][y].owner); 
    //explodeTile(x, y, explosion);

    //printf("explode position x = %d, y = %d\n", x, y);

    /* Explode tiles on the horizontal and vertical that are in range but
     * don't explode past an unbreakable wall */

    bool left_blocked  = false;
    bool right_blocked = false;
    bool up_blocked    = false;
    bool down_blocked  = false;

    for (int i = 1; i <= range; i++) {

        if (!(up_blocked)) {
            switch (terrain_grid[x][y - i]) {
                case TERRAIN_WALL_UNBREAKABLE:
		            explosion_grid[x][y].up = 0;
                    up_blocked = true;
                    break;
                case TERRAIN_WALL_BREAKABLE:
                    explosion_grid[x][y].up = 1;
                    terrain_grid[x][y-i] = TERRAIN_WALL_BREAKABLE_B;
                    printf("up");

                    //explodeTile(x, y + i, explosion);
                    up_blocked = true;
                    break;
                case TERRAIN_GROUND:
                    //explodeTile(x, y + i, explosion);
                    if (bomb_grid[x][y-i].type == BOMB_TYPE_NORMAL || explosion_grid[x][y-i].type == EXPLOSION_TYPE_NORMAL)
                    {
                        explosion_grid[x][y].up = 0;
                        up_blocked = true;
                    }
                    else{
                        explosion_grid[x][y].up = 1;

                    }
		    
                    break;
            }
        }

        if (!(down_blocked)) {
            switch (terrain_grid[x][y + i]) {
                case TERRAIN_WALL_UNBREAKABLE:
		    explosion_grid[x][y].down = 0;
                    down_blocked = true;
                    break;
                case TERRAIN_WALL_BREAKABLE:
                    explosion_grid[x][y].down = 1;
                    terrain_grid[x][y+i] = TERRAIN_WALL_BREAKABLE_B;
                    printf("down");
                    //explodeTile(x, y - i, explosion);
			        //explosion->down = 0;
                    down_blocked = true;
                    break;
                case TERRAIN_GROUND:
                    //explodeTile(x, y - i, explosion);
                    //printf("explosition type = %d\n", explosion_grid[x][y-i].type);
                    if (bomb_grid[x][y+i].type == BOMB_TYPE_NORMAL || explosion_grid[x][y+i].type == EXPLOSION_TYPE_NORMAL)
                    {
                         explosion_grid[x][y].down = 0;
                        down_blocked = true;
                    }
                    else{
                        explosion_grid[x][y].down = 1;
                    }
                    
                    break;
            }
        }

	if (!(left_blocked)) {
            switch (terrain_grid[x - i][y]) {
                case TERRAIN_WALL_UNBREAKABLE:
                    explosion_grid[x][y].left = 0;
                    left_blocked = true;
                    break;
                case TERRAIN_WALL_BREAKABLE:
                    explosion_grid[x][y].left = 1;
                    terrain_grid[x-i][y] = TERRAIN_WALL_BREAKABLE_B;
                    //explodeTile(x, y + i, explosion);
                    left_blocked = true;
                    break;
                case TERRAIN_GROUND:
                    //explodeTile(x - i, y, explosion);
                    if (bomb_grid[x - i][y].type == BOMB_TYPE_NORMAL || explosion_grid[x - i][y].type == EXPLOSION_TYPE_NORMAL)
                    {
                         explosion_grid[x][y].left = 0;
                        left_blocked = true;
                    }
                    else{
                        explosion_grid[x][y].left = 1;
                    }
		    
		    //printf("left is not block, %d\n", explosion_grid[x][y].left);
                    break;
            }
        }

        if (!(right_blocked)) {
            switch (terrain_grid[x + i][y]) {
                case TERRAIN_WALL_UNBREAKABLE:
		        explosion_grid[x][y].right = 0;
                    right_blocked = true;
                    break;
                case TERRAIN_WALL_BREAKABLE:
                    explosion_grid[x][y].right = 1;
                    terrain_grid[x+i][y] = TERRAIN_WALL_BREAKABLE_B;
                    //explodeTile(x, y + i, explosion);
                    up_blocked = true;
                    right_blocked = true;
                    break;
                case TERRAIN_GROUND:
                    //explodeTile(x + i, y, explosion);
                    //printf("explosition type = %d\n", explosion_grid[x+i][y].type);
		        if (bomb_grid[x + i][y].type == BOMB_TYPE_NORMAL || explosion_grid[x + i][y].type == EXPLOSION_TYPE_NORMAL)
                    {
                        explosion_grid[x][y].right = 0;
                        right_blocked = true;
                    }
                    else{
                        explosion_grid[x][y].right = 1;
                    }
                    break;
            }
        }

        
    }
    changed_tiles[x][y] = true;
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


}

void killPlayersInExplosion(void) {
    
    Position pos_1 = {0, 0};
    Position pos_2 = {0, 0};

    for (int i = 0; i < PLAYER_NUM; i++) {
        if (explosion_grid[players[i].tile_position.x][players[i].tile_position.y].type != EXPLOSION_EMPTY) {
            players[i].alive = false;
            uint32_t mask = (1 << 31);    // Set the 32nd bit
            mask |= (0x1F << 4);          // Set the 5th to 9th bits (0x1F = 00011111 in binary)
            if (i==0)
            {
                color.p1_state &= mask;
                set_background_color(&color);
            }
            else {
                color.p2_state &= mask;
                set_background_color(&color);
            }

            printf("players die\n");

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




