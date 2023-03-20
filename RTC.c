/*
 * RTC.c
 *
 * Created: 2/5/2023 1:41:42 AM
 *  Author: pinguu
 */ 

#include "RTC.h"

/*void SendStart() {
	TWCR = (1<<TWINT) | (1<<TWSTA) | (1<<TWEN);
}*/

void SendStop() {
	TWCR = (1<<TWINT) | (1<<TWSTO) | (1<<TWEN);
}

void RTC_init(void)
{
	TWBR = 0x08;
}

unsigned char BeginRead(unsigned char addr) {
	int error = BeginWrite(addr);
	
	if(error != 0) return error;
	
	TWCR = (1<<TWINT) | (1<<TWSTA) | (1<<TWEN); 
	
	while(!(TWCR & (1<<TWINT))); 
	if ((TWSR & 0xF8) != TW_REP_START) return 4;
	
	TWDR = RTC_ADDR | TW_READ;
	TWCR = (1 << TWINT) | (1 << TWEN);
	
	while ( !(TWCR & (1 << TWINT)) );
	if( (TWSR & 0xF8) != TW_MR_SLA_ACK) return 5;
	
	return 0;
}

unsigned char BeginWrite(unsigned char addr) {
	PORTB = 0x00;
	
	TWCR = (1<<TWINT) | (1<<TWSTA) | (1<<TWEN);
	
	while(!(TWCR & (1<<TWINT))); //czekamy az twint bedzie 0 (co oznacza ze wyslalo dane)
	if ((TWSR & 0xF8) != TW_START) return 1; //sprawdzamy czy b3 - b7 w rejestrze statusu odpowiadaj? warto?ci? wys?ania startu (TW_START = 0x08)
	PORTB = 0x01;
	TWDR = RTC_ADDR | TW_WRITE; //do rejestru danych (b?dziemy to wysy?a?) wpisujemy adres slave + 0 (TW_WRITE odpowiada bitowi mówi?cemu ?e master przsy?a dane)
	TWCR = (1 << TWINT) | (1 << TWEN); // PO WPISANIU slaveAddr wpisujemy 1 do TWINT (czyszczenie flagi) i 1 do TWEN ('w??cza' TWI)
	
	while (!(TWCR & (1 << TWINT))); //znów czakamy a? flaga b?dzie 0 (czyli ?e przesy? sko?czony)
	PORTB = 0x00;
	if ((TWSR & 0xF8) != TW_MT_SLA_ACK) return 2; // je?li bity b3 - b7 w rejestrze statusu nie odpowiadaj? statusowi odebrania od slave ACK wysy?amy b??d (TW_MT_SLA_ACK = 0x18)
	
	TWDR = addr; //do danych wpisujemy adres od którego zaczniemy wpisywa? dane
	TWCR = ( 1 << TWINT) | (1 << TWEN); // PO WPISANIU slaveAddr wpisujemy 1 do TWINT (czyszczenie flagi) i 1 do TWEN ('w??cza' TWI)
	PORTB = 0x01;
	while (!(TWCR & (1 << TWINT)));
	if( (TWSR & 0xF8) != TW_MT_DATA_ACK) return 3; //je?li bity b3 - b7 w rejestrze statusu nie odpowiadaj? wys?aniu danych wysy?amy b??d (TW_MT_DATA_ACK = 0x28)
	PORTB = 0x00;
	return 0;
}

void SendAck() {
	
}

unsigned char ReadByte(unsigned char *data, bool ack){
	
	/*const unsigned char status = ack ? TW_MR_DATA_ACK : TW_MR_DATA_NACK; 

	TWCR = (1 << TWINT) | (1 << TWEN) | (ack ? (1 << TWEA) : 0);

	while (!(TWCR & (1 << TWINT)));
	if ((TWSR & 0xF8) != status) return -1; */
	
	if(ack) {
		TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWEA);
	} else {
		TWCR = (1 << TWINT) | (1 << TWEN);
	}
	
	while( !(TWCR &(1 << TWINT)) );
	if(ack) {
		if( (TWSR & 0xF8) != TW_MR_DATA_ACK) return 6;
	} else if ( (TWSR & 0xF8) != TW_MR_DATA_NACK) return 7;
	
	*data = TWDR;
	return 0;
}

unsigned char SetByte(unsigned char data) {
	TWDR = data;
	TWCR = (1 << TWEN) | (1 << TWINT);
	
	while( !(TWCR & (1 << TWINT)) );
	if( (TWSR & 0xF8) != TW_MT_DATA_ACK) return 7;
	
	return 0;
}

unsigned char ReadTime( struct TIME *time) {
	unsigned char data = 0;
	unsigned char error = BeginRead(0x00);
	
	if(error != 0) return error;
	
	for(int i = 0; i < 3; i++){
		
		error = ReadByte(&data, i != 2);
		if(error != 0) return error;
		switch(i) {
			case 0:
				time->sec = data;
				break;
			case 1:
				time->min = data;
				break;
			case 2:
				time->h = data;
				break;
		}
	}
		SendStop();
		return 0;
	
}

unsigned char SetTime(struct TIME time) {
	unsigned char error = BeginWrite(0x00);	
	
	if(error != 0) return error;
	
	error = SetByte(time.sec);
	if(error != 0) return error;
	error = SetByte(time.min);
	if(error != 0) return error;
	error = SetByte(time.h);
	if(error != 0) return error;
	SendStop();
	return 0;
}