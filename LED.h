/*
 * LED.h
 *
 * Created: 2/16/2023 10:19:02 PM
 *  Author: pinguu
 */ 

#include "RTC.h"

//#define F_CPU   8000000

#include <avr/io.h>
#include <util/delay.h>
#include <stdint.h>


#ifndef LED_H_
#define LED_H_

extern void output_grb(uint8_t *ptr, uint16_t cout);

void ShowChangingTime(struct TIME *time); 
void ShowTime(struct TIME time);
//void simpleRGB ();
//void set_color(uint8_t * p_buf, uint8_t led, uint8_t r, uint8_t g, uint8_t b);
void SetColor(uint8_t * buf, uint8_t ledNum);
void SetZero(uint8_t * buf, uint8_t ledNum);
#endif /* LED_H_ */