/*
 * RTC.h
 *
 * Created: 2/5/2023 1:41:59 AM
 *  Author: pinguu
 */ 

#include <util/twi.h>
#include <stdbool.h>

struct TIME {
	unsigned char sec;
	unsigned char min;
	unsigned char h;
	};

#ifndef RTC_H_
#define RTC_H_

void RTC_init(void);

unsigned char BeginWrite (unsigned char addr);
unsigned char BeginRead (unsigned char addr);
void SendStop (void);
//void SendStart (void);
void SendAck (void);
unsigned char ReadTime(struct TIME *time);
unsigned char SetTime (struct TIME time);
unsigned char ReadByte(unsigned char *data, bool ACK); // if ACK == 0 send ACK, else send NACK
unsigned char SetByte(unsigned char data);

#endif /* RTC_H_ */

#define RTC_ADDR 0xD0 //by?o ox68
