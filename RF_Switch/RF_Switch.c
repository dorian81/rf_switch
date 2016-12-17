/*
 * RF_Switch.c
 *
 * Created: 13.12.2016 22:16:38
 *  Author: Robert
 */ 

# define F_CPU 1000000UL

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

void toggle(int sw);

int sw_state = 0x00;

ISR(TIMER0_OVF_vect){
	int sw = PINC;
	
	if (sw != sw_state){
		toggle(sw_state ^ sw);
		sw_state = sw;
	}
}

ISR(INT0_vect){
	int remote = PIND >> 4;
	toggle(remote);
}

void toggle(int sw){
	switch(sw){
		case 1:{
			PORTB ^= (1 << PINB0);
			break;
		}
		case 2:{
			PORTB ^= (1 << PINB1);
			break;
		}
		case 4:{
			PORTB ^= (1 << PINB2);
			break;
		}
		case 8:{
			PORTB ^= (1 << PINB3);
			break;
		}
		case 9:{
			PORTB = 0x0F;
			break;
		}
		case 10:{
			PORTB = 0x00;
			break;
		}
	}
}

void init(){
	DDRB = 0x0F;
	DDRC = 0x00;
	DDRD = 0x00;
	
	TCCR0 |= (1<<CS02);
	TIMSK |= (1<<TOIE0);
	
	MCUCR=0x03;
	GICR |= 1<<INT0;
	sei();
	
	sw_state = PINC;
}

int main(void)
{
	init();
	
    while(1);
}