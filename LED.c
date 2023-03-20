/*
 * LED.c
 *
 * Created: 2/5/2023 1:42:26 AM
 *  Author: pinguu
 */ 

#include "LED.h"

void SetColor(uint8_t * buf, uint8_t ledNum) {
	uint16_t index = 3*ledNum;
	buf[index++] = 0;
	buf[index++] = 10;
	buf[index] = 10;
}

void SetZero(uint8_t * buf, uint8_t ledNum) {
	uint16_t index = 3*ledNum;
	buf[index++] = 0;
	buf[index++] = 0;
	buf[index] = 0;
}

void ShowChangingTime(struct TIME *time) {
	
	uint8_t buf[45*3], tmp;
	//unsigned char offset;
	memset(buf, 0, sizeof(buf));
	   // B0 is our output	

	int timeNum = 6;
	if(time->min >= 61){
		 timeNum -= 4;
		 for(int i = 0; i < 32; i++) {
			 SetColor(buf, i);
		 }
	}
	else if(time->sec >= 61) {
		timeNum -= 2;
		for(int i = 0; i < 16; i++) {
			SetColor(buf, i);
		}
	}
	
	for(int i = 0; i < timeNum; i++) {
		//if( (i % 2) != 0) offset = 0x0F;
		//else offset = 0xF0;
		
		switch(i){
			case 0:
			tmp = (time->h & 0x0F) + 33;
			break;
			case 1:
			tmp = (time->h & 0xF0) / 0x0F + 42;
			break;
			case 2:
			tmp = (time->min & 0x0F) + 16;
			break;
			case 3:
			tmp = (time->min & 0xF0) / 0x0F + 26;
			break;
			case 4:
			tmp = (time->sec & 0x0F);
			break;
			case 5:
			tmp = (time->sec & 0xF0) / 0x0F + 10;
			break;
		}
		SetColor(buf, tmp);
	}
	output_grb(buf, 45*3);
}

void ShowTime(struct TIME time) {
	
	uint8_t buf[45*3], tmp;
	//unsigned char offset;
	memset(buf, 0, sizeof(buf));
			//B0 is our output
	
	for(int i = 0; i < 45; i ++) {
		SetZero(buf, i);
	}
	
	for(int i = 0; i < 6; i++) {
		//if( (i % 2) != 0) offset = 0x0F;
		//else offset = 0xF0;
		
		switch(i){
			case 0: 
				tmp = (time.h & 0x0F) + 32;
				break;
			case 1:
				tmp = (time.h & 0xF0) / 0x0F + 42;
				break;
			case 2:
				tmp = (time.min & 0x0F) + 16;
				break;
			case 3:
				tmp = (time.min & 0xF0) / 0x0F + 26;
				break;
			case 4:
				tmp = (time.sec & 0x0F);
				break;
			case 5:
				tmp = (time.sec & 0xF0) / 0x0F + 10;
				break;
		}
		SetColor(buf, tmp);
	}
	output_grb(buf, 45*3);
}
