/*
 * Game.c
 *
 *  Created on: 02-Jan-2026
 *      Author: rayv_mini_pc
 */

#include "Game.h"
#include <stdlib.h>

void GAME_ctor(GAME_Engine_t *const me, CANVAS_t *canvas, INPUT_t *input) {
	me->canvas = canvas;
	me->input = input;

	me->player_pos.x = 3;
	me->player_pos.y = 3;

	me->player_color.pixels.red = 0;
	me->player_color.pixels.green = 255;
	me->player_color.pixels.blue = 0;
}

void GAME_update(GAME_Engine_t *const me) {
	key_action_e action = INPUT_get_action(me->input);
	if (action != ACTION_NONE) {
		switch (action) {
		case ACTION_UP:
			if (me->player_pos.y > 0)
				me->player_pos.y--;
			break;
		case ACTION_DOWN:
			if (me->player_pos.y < (me->canvas->display->size_y - 1))
				me->player_pos.y++;
			break;
		case ACTION_LEFT:
			if (me->player_pos.x > 0)
				me->player_pos.x--;
			break;
		case ACTION_RIGHT:
			if (me->player_pos.x < (me->canvas->display->size_x - 1))
				me->player_pos.x++;
			break;
		case ACTION_CONFIRM:
		default:
			break;
		}
	}
}

void GAME_render(GAME_Engine_t *const me) {
	// 1. Wipe the canvas clean (erases the point's previous position)
	CANVAS_clear(me->canvas);

	// 2. Draw the point at its current state position
	CANVAS_draw_point(me->canvas, me->player_pos, me->player_color);
}
