/*
 * Game.h
 *
 *  Created on: 02-Jan-2026
 *      Author: rayv_mini_pc
 */

#ifndef INC_GAME_H_
#define INC_GAME_H_

#include "Input.h"
#include "Canvas.h"

#define REFRESH_RATE 120
#define INPUT_RATE 30
#define TICK_RATE 1

typedef struct {
	CANVAS_t * canvas;
	INPUT_t * input;

	//---------------------------Player ----------------/
	C_COORDINATES_t player_pos;
	PIXEL_t player_color;
}GAME_Engine_t;

void GAME_ctor(GAME_Engine_t * const me, CANVAS_t * canvas, INPUT_t * input);

void GAME_update(GAME_Engine_t * const me);
void GAME_render(GAME_Engine_t * const me);

#endif /* INC_GAME_H_ */
