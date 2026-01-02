/*
 * Game.c
 *
 *  Created on: 02-Jan-2026
 *      Author: rayv_mini_pc
 */

#include "Game.h"
#include <stdlib.h>
#include <string.h>

void GAME_ctor(GAME_Engine_t *const me, CANVAS_t *canvas, INPUT_t *input) {
	me->canvas = canvas;
	me->input = input;
	GAME_reset(me);
}

void move_snake(GAME_Engine_t *me) {
	// 1. Shift the body: Start from the tail, move each segment to the position of the one before it
	for (int i = me->length - 1; i > 0; i--) {
		me->body[i] = me->body[i - 1];
	}

	// 2. Move the Head based on current direction
	switch (me->current_dir) {
	case ACTION_UP:
		me->body[0].y--;
		break;
	case ACTION_DOWN:
		me->body[0].y++;
		break;
	case ACTION_LEFT:
		me->body[0].x--;
		break;
	case ACTION_RIGHT:
		me->body[0].x++;
		break;
	default:
		break;
	}
}

void spawn_food(GAME_Engine_t *const me) {
    bool on_snake;
    do {
        on_snake = false;
        me->food.x = rand() % 8;
        me->food.y = rand() % 8;

        // Make sure food didn't spawn on the snake body
        for (int i = 0; i < me->length; i++) {
            if (me->food.x == me->body[i].x && me->food.y == me->body[i].y) {
                on_snake = true;
                break;
            }
        }
    } while (on_snake); // Keep trying until we find a clear spot
}

void check_collisions(GAME_Engine_t *const me) {
    C_COORDINATES_t head = me->body[0];

    // 1. Wall Collision (Checking >= 8 handles wrap-around for uint8_t)
    if (head.x >= me->canvas->display->size_x || head.y >= me->canvas->display->size_y) {
        GAME_reset(me);
        return;
    }

    // 2. Self Collision (Start checking from index 1 to length)
    for (int i = 1; i < me->length; i++) {
        if (head.x == me->body[i].x && head.y == me->body[i].y) {
            GAME_reset(me);
            return;
        }
    }

    // 3. Food Collision
    if (head.x == me->food.x && head.y == me->food.y) {
        if (me->length < MAX_SNAKE_LEN) {
            me->length++; // Grow the snake
        }
        spawn_food(me); // Place new food
    }
}

void GAME_update(GAME_Engine_t *const me) {
	key_action_e new_action = INPUT_get_action(me->input);

	// Filter input to prevent 180-degree turns
	if (new_action == ACTION_UP && me->current_dir != ACTION_DOWN)
		me->current_dir = ACTION_UP;
	if (new_action == ACTION_DOWN && me->current_dir != ACTION_UP)
		me->current_dir = ACTION_DOWN;
	if (new_action == ACTION_LEFT && me->current_dir != ACTION_RIGHT)
		me->current_dir = ACTION_LEFT;
	if (new_action == ACTION_RIGHT && me->current_dir != ACTION_LEFT)
		me->current_dir = ACTION_RIGHT;

	// Control the "Speed" (Snake should only move every ~200ms, not every frame)
	if (HAL_GetTick() - me->move_timer > 1000 / TICK_RATE) {
		me->move_timer = HAL_GetTick();
		move_snake(me);
		check_collisions(me);
	}
}

void GAME_render(GAME_Engine_t *const me) {
    CANVAS_clear(me->canvas);

    // 1. Draw Food (Red)
    PIXEL_t food_color = {.pixels = {0, 255, 0}};
    CANVAS_draw_point(me->canvas, me->food, food_color);

    // 2. Draw Snake (Green)
    PIXEL_t snake_color = {.pixels = {255, 0, 0}};
    for (int i = 0; i < me->length; i++) {
        CANVAS_draw_point(me->canvas, me->body[i], snake_color);
    }
}

void GAME_reset(GAME_Engine_t *const me) {
    memset(me->body, 0, MAX_SNAKE_LEN * sizeof(C_COORDINATES_t));

    // Start in the center
    me->body[0].x = 3;
    me->body[0].y = 3;
    me->length = 1;

    me->current_dir = ACTION_NONE; // Wait for player input to start
    me->move_timer = HAL_GetTick();

    spawn_food(me);
}
