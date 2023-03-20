/*
 * RCT1.c
 *
 * Created: 1/17/2023 7:21:27 PM
 * Author : pinguu
 */ 
#include <avr/io.h>
#include "LED.h"


void GetTime(struct TIME *time) {
		
	int tmp = 1;
	
	time->h = 0;
	time->min = 61;
	time->sec = 61;
	
	ShowChangingTime(time);
	
	while (tmp) {
		if( !(PIND & (1 << PD7))) {
			_delay_ms(100);
			if(!(PIND & (1 << PD7))) {
				
				if(time->h >= 0x23){
					time->h = 0;
				} else if((time->h & 0x0F) >= 0x09) {
					if( ((time->h & 0xF0) == 0x00) || ((time->h & 0xF0) == 0x10)) {
						time->h += 0x07;
					} 
				} else{
					time->h++;
				}
				
				ShowChangingTime(time);
			}
		}
		
		if( !(PIND & (1 << PD6))) {
			_delay_ms(100);
			if(!(PIND & (1 << PD6))) {
				tmp = 0;
				_delay_ms(2000);
			}
		}
	}
	
	time->min = 0;
	ShowChangingTime(time);
	tmp = 1;
	
	while (tmp) {
		if( !(PIND & (1 << PD7))) {
			_delay_ms(100);
			if(!(PIND & (1 << PD7))) {
				if(time->min >= 0x59){
					time->min = 0;
				}
				else if((time->min & 0x0F) >= 9) {
					if( (time->min) < 0x50) {
						time->min += 0x07;
					}
				} else {
					time->min++;
				}
				
				ShowChangingTime(time);
			}
		}
		
		if( !(PIND & (1 << PD6))) {
			_delay_ms(100);
			if(!(PIND & (1 << PD6))) {
				tmp = 0;
				_delay_ms(2000);
			}
		}
	}
		
		time->sec = 0;
		ShowChangingTime(time);
		tmp = 1;
		while (tmp) {
			if( !(PIND & (1 << PD7))) {
				_delay_ms(100);
				if(time->sec >= 0x59){
					time->sec = 0;
				}
				else if((time->sec & 0x0F) >= 9) {
					if( (time->sec) < 0x50) {
						time->sec += 0x07;
					}
				} else {
					time->sec++;
				}
					
					ShowChangingTime(time);
				}
			}
			
			if( !(PIND & (1 << PD6))) {
				_delay_ms(100);
				if(!(PIND & (1 << PD6))) {
					tmp = 0;
					_delay_ms(2000);
				}
			}
	}
	
}

int main(void)
{
	
	RTC_init();
	
	struct TIME time;
	
	DDRB |= (1<<PB0); //B0 -> output for led
	
	DDRD &= ~(1<<PD7); //D7 jako wejscie
	PORTD |= (1<<PD7); //D7 podciagamy do vcc
	DDRD &= ~(1<<PD6);
	PORTD |= (1<<PD6);
	
	DDRC |= (1<<PC3);
	PORTC &= ~(1<<PC3);
	DDRC |= (1<<PC2);
	PORTC &= ~(1<<PC2);
	
	while(1) {
		if( !(PIND & (1 << PD7))) {
			_delay_ms(100);
			PORTC |= (1<<PC3);
			_delay_ms(100);
			PORTC &= ~(1<<PC3);
			if(!(PIND & (1 << PD7))) {
				PORTC |= (1<<PC2);
				_delay_ms(100);
				PORTC &= ~(1<<PC2);
				
				GetTime(&time);
				SetTime(time);
			}
		}
		
		ReadTime(&time);
		ShowTime(time);
		_delay_ms(1000);
	}
}

