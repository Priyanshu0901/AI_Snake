/*
 * AI.c
 *
 *  Created on: 04-Jan-2026
 *      Author: rayv_mini_pc
 */

#include "AI.h"
#include "stdlib.h"

typedef struct {
    bool up, down, left, right;
    bool visited;
} MST_Node_t;

static MST_Node_t supergrid[4][4];

// Reset Grid
void init_supergrid() {
    for (int y = 0; y < 4; y++) {
        for (int x = 0; x < 4; x++) {
            supergrid[y][x] = (MST_Node_t){false, false, false, false, false};
        }
    }
}

void visit_node(int x, int y) {
    supergrid[y][x].visited = true;

    // Directions: 0=Up, 1=Down, 2=Left, 3=Right
    int dirs[] = {0, 1, 2, 3};

    // Shuffle directions for randomness
    for (int i = 0; i < 4; i++) {
        int r = rand() % 4;
        int temp = dirs[r];
        dirs[r] = dirs[i];
        dirs[i] = temp;
    }

    for (int i = 0; i < 4; i++) {
        int nx = x, ny = y;
        if (dirs[i] == 0) ny--;      // Up
        else if (dirs[i] == 1) ny++; // Down
        else if (dirs[i] == 2) nx--; // Left
        else if (dirs[i] == 3) nx++; // Right

        // Check bounds and if the neighbor was already visited
        if (nx >= 0 && nx < 4 && ny >= 0 && ny < 4 && !supergrid[ny][nx].visited) {
            // Open the wall between current and neighbor
            if (dirs[i] == 0) { supergrid[y][x].up = true;    supergrid[ny][nx].down = true; }
            else if (dirs[i] == 1) { supergrid[y][x].down = true;  supergrid[ny][nx].up = true; }
            else if (dirs[i] == 2) { supergrid[y][x].left = true;  supergrid[ny][nx].right = true; }
            else if (dirs[i] == 3) { supergrid[y][x].right = true; supergrid[ny][nx].left = true; }

            // Recurse into the neighbor
            visit_node(nx, ny);
        }
    }
}

void generate_mst_4x4() {
    init_supergrid();
    // Start generating from a random corner or (0,0)
    visit_node(rand() % 4, rand() % 4);
}

void expand_mst_to_hamiltonian(AI_t *me) {
    int x = 0, y = 0;
    for (int i = 0; i < 64; i++) {
        me->ham_path[i].x = x;
        me->ham_path[i].y = y;

        int sx = x / 2; // Current super-cell X
        int sy = y / 2; // Current super-cell Y

        // Determine next step based on which corner of the 2x2 we are in:
        if (x % 2 == 0 && y % 2 == 0) { // Top-Left
            if (supergrid[sy][sx].up) y--;    // Move to super-cell above
            else x++;                         // Move to Top-Right
        } else if (x % 2 == 1 && y % 2 == 0) { // Top-Right
            if (supergrid[sy][sx].right) x++; // Move to super-cell right
            else y++;                         // Move to Bottom-Right
        } else if (x % 2 == 1 && y % 2 == 1) { // Bottom-Right
            if (supergrid[sy][sx].down) y++;  // Move to super-cell below
            else x--;                         // Move to Bottom-Left
        } else { // Bottom-Left
            if (supergrid[sy][sx].left) x--;  // Move to super-cell left
            else y--;                         // Move to Top-Left
        }
    }
}

void AI_ctor(AI_t * const me, GAME_Engine_t * game_state){
	me->game_state = game_state;
	AI_reset(me);
}

void AI_reset(AI_t * const me) {
    log_message("AI", LOG_INFO, "Generating Hamiltonian Cycle...");

    // 1. Build a Randomized Spanning Tree on a 4x4 grid
    // (A simple DFS-based maze generator works perfectly for this)
    generate_mst_4x4();

    // 2. Convert that 4x4 tree into the 8x8 ham_path
    // This fills me->ham_path[0...63]
    expand_mst_to_hamiltonian(me);

    me->game_state->game_over = false;
    log_message("AI", LOG_INFO, "Path Generated. Ready to play.");
}

key_action_e AI_get_action(AI_t * const me) {
    C_COORDINATES_t head = me->game_state->body[0];
    int current_idx = -1;

    // 1. Find where we are in the path
    for (int i = 0; i < 64; i++) {
        if (me->ham_path[i].x == head.x && me->ham_path[i].y == head.y) {
            current_idx = i;
            break;
        }
    }

    // 2. Get the next coordinate (looping at 63 back to 0)
    C_COORDINATES_t next = me->ham_path[(current_idx + 1) % 64];

    // 3. Convert coordinates to Action
    if (next.x > head.x) return ACTION_RIGHT;
    if (next.x < head.x) return ACTION_LEFT;
    if (next.y > head.y) return ACTION_DOWN;
    if (next.y < head.y) return ACTION_UP;

    return ACTION_NONE;
}
