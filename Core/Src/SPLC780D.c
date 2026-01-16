/*
 * SPLC780D.c
 *
 *  Created on: 16-Jan-2026
 *      Author: rayv_mini_pc
 */

#include "SPLC780D.h"

#define SPLC780D_PCF8574_ADDRESS (PCF8574_DEFAULT_ADDRESS | 0x05)

#define SPLC780D_CLEAR_CMD	0x01

void SPLC780D_Toggle_Latch(SPLC780D_t * const me){
	HAL_GPIO_TogglePin(me->E_Port, me->E_Pin);
}

void SPLC780D_Write_CMD(SPLC780D_t * const me, uint16_t cmd){

}

void SPLC780D_ctor(SPLC780D_t * const me, I2C_HandleTypeDef *i2chandle){

	me->driver.address = SPLC780D_PCF8574_ADDRESS;
	PCF8574_ctor(&(me->driver), i2cHandle);


}

