/**
 * @file    PAJ7620.h
 * @brief   Library for the PAJ7620 breakout board
 * @author  Yayi
 * @Website rtrobot.org
 * @Version 1.0.0
 * @Copyright	This code is public domain but you buy me a beer if you use
 * this and we meet someday (Beerware license).
 */
 

#ifndef __PAJ7620_H__
#define __PAJ7620_H__
#include <stdint.h>
// DEVICE ID
#define PAJ7620_ID  0x73

#define PAJ7620_ADDR_BASE				0xEF

// PAJ7620_REGITER_BANK_SEL
#define PAJ7620_BANK0 0
#define PAJ7620_BANK1 1

#define GES_RIGHT_FLAG				1<<0
#define GES_LEFT_FLAG				1<<1
#define GES_UP_FLAG					1<<2
#define GES_DOWN_FLAG				1<<3
#define GES_FORWARD_FLAG			1<<4
#define GES_BACKWARD_FLAG			1<<5
#define GES_CLOCKWISE_FLAG			1<<6
#define GES_COUNT_CLOCKWISE_FLAG	1<<7
#define GES_WAVE_FLAG				1<<0


uint8_t PAJ7620Init(void);
uint8_t PAJ7620WriteReg(uint8_t addr, uint8_t cmd);
uint8_t PAJ7620ReadReg(uint8_t addr, uint8_t qty, uint8_t data[]);

#endif
