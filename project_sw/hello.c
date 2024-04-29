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

void countdownExplosions(void) {

    for (int x = 0; x < MAP_SIZE; x++) {
        for (int y = 0; y < MAP_SIZE; y++) {

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

    for (int x = 0; x < MAP_SIZE; x++) {
        for (int y = 0; y < MAP_SIZE; y++) {

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
                case TERRAIN_GROUND:
                    explodeTile(x - i, y, explosion);
                    break;
            }
        }
    }
}

void explodeTile(int8_t x, int8_t y, Explosion *explosion) {

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
    if (terrain_grid[x][y] != TERRAIN_WALL_BREAKABLE) {
        items_grid[x][y] = ITEM_EMPTY;
    } else {
        items_grid[x][y] = generateRandomItem();
    }

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
            drawPlayerIcons();
            drawPlayerScores();
        }
    }
}

void move(Player *player, direction dir) {

    /* Check for out of bounds movement */
    if (walkingOutOfBounds(player, dir)) {
        dir = DIRECTION_IDLE;
    }

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
            clamp(&temp_x);
            clamp(&temp_y);

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
        clamp(&temp_x);
        clamp(&temp_y);

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
    clamp(&temp_x);
    clamp(&temp_y);

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
        clamp(&temp_x);
        clamp(&temp_y);

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

    if (x < 0 || y < 0 || x >= MAP_SIZE || y >= MAP_SIZE) { return false; }
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
            result->y = TILE_SIZE; 
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
    } else if (pos->x >= MAP_SIZE) {
        pos->x = MAP_SIZE - 1;
    }

    if (pos->y < 0) {
        pos->y = 0;
    } else if (pos->y >= MAP_SIZE) {
        pos->y = MAP_SIZE - 1;
    }
}

/* Safe way of setting player screen_coordinates */
void setPlayerPosition(Player* player, int32_t x, int32_t y) {

    /* Clamp coordinates */
    clamp(&x);
    clamp(&y);

    player->screen_position.x = x;
    player->screen_position.y = y;
}

void clamp(int32_t* i) {
    if (*i < 0) {
        *i = 0; 
    } else if (*i > (MAP_SIZE - 1) * TILE_SIZE) { 
        *i = (MAP_SIZE - 1) * TILE_SIZE; 
    }
}

bool walkingOutOfBounds(Player* player, direction dir) {
    if (player->tile_position.x == 0 && dir == DIRECTION_LEFT) {
        return true;
    }
    if (player->tile_position.y == 0 && dir == DIRECTION_UP) {
        return true;
    }
    if (player->screen_position.x >= (MAP_SIZE - 1) * TILE_SIZE - player->move_speed && dir == DIRECTION_RIGHT) {
        player->screen_position.x = (MAP_SIZE - 1) * TILE_SIZE;
        return true;
    }
    if (player->screen_position.y >= (MAP_SIZE - 1) * TILE_SIZE - player->move_speed && dir == DIRECTION_DOWN) {
        player->screen_position.y = (MAP_SIZE - 1) * TILE_SIZE;
        return true;
    }
    return false;
}



int main()
{
  vga_ball_arg_t vla;
 
  
}
