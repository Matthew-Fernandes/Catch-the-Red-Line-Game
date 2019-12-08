/*
 * Custom_Test.c
 *
 * Created: 11/23/2019 1:40:39 PM
 * Author : Matthew
 */ 

#include <avr/io.h>
#include <stdlib.h>
#include <time.h>
#include <avr/interrupt.h>
#include "io.h"

unsigned char D0 = 0x00;
unsigned char D1 = 0x00;
unsigned char Red_Pos = -1;
unsigned char change = 4;
unsigned long Random_elapsedTime = 0;
unsigned long Random_period = 4500;
unsigned long Comb_elapsedTime = 50;
unsigned char point_val[12] = {0,0,1,1,2,2,3,3,4,4,5,5};
unsigned char stage_val = 0;
const unsigned long timerPeriod = 1;
unsigned long tens_score_elapsed = 0;
unsigned long score_elapsed = 0;

volatile unsigned char TimerFlag = 0; 

unsigned long _avr_timer_M = 1;
unsigned long _avr_timer_cntcurr = 0;

void ADC_init(){
	ADCSRA |= (1 << ADEN) | (1 << ADSC) | (1 << ADATE);
}

void TimerOn() {
	
	TCCR1B = 0x0B;
	OCR1A = 125;
	TIMSK1 = 0x02;
	TCNT1 = 0;
	_avr_timer_cntcurr = _avr_timer_M;
	SREG = 0x80;
}

void TimerOff() {
	TCCR1B = 0x00;
}

void TimerISR() {
	TimerFlag = 1;
}

ISR(TIMER1_COMPA_vect) {
	_avr_timer_cntcurr--; 
	if (_avr_timer_cntcurr == 0) {
		TimerISR();
		_avr_timer_cntcurr = _avr_timer_M;
	}
}

void TimerSet (unsigned long M) {
	_avr_timer_M = M;
	_avr_timer_cntcurr = _avr_timer_M;
}

enum States {Init, stat0, stat1, stat2, stat3, stat4, stat5, stat6, stat7, Wait, Go} state;
signed char count;
void Tick(){
	D0 = ~PIND & 0x01;
	D1 = ~PIND & 0x02;
	switch (state) { //Actions
		case Init:
			state = stat4;
			break;
		case stat0:
			if(D0 || ADC >= 1000)
			{
				state = Wait;
			}
			else if(D1 || ADC <= 50)
			{
				state = Wait;
			}
			else
			{
				state = stat0;
			}
			break;
		case stat1:
			if(D0 || ADC >= 1000)
			{
				state = Wait;
			}
			else if (D1 || ADC <= 50)
			{
				state = Wait;
			}
			else
			{
				state = stat1;
			}
			break;
		case stat2:
			if(D0 || ADC >= 1000)
			{
				state = Wait;
			}
			else if (D1 || ADC <= 50)
			{
				state = Wait;
			}
			else
			{
				state = stat2;
			}
			break;
		case stat3:
			if(D0 || ADC >= 1000)
			{
				state = Wait;
			}
			else if (D1 || ADC <= 50)
			{
				state = Wait;
			}
			else
			{
				state = stat3;
			}
			break;
		case stat4:
			if(D0 || ADC >= 1000)
			{
				state = Wait;
			}
			else if (D1 || ADC <= 50)
			{
				state = Wait;
			}
			else
			{
				state = stat4;
			}
			break;
		case stat5:
			if(D0 || ADC >= 1000)
			{
				state = Wait;
			}
			else if (D1 || ADC <= 50)
			{
				state = Wait;
			}
			else
			{
				state = stat5;
			}
			break;
		case stat6:
			if(D0 || ADC >= 1000)
			{
				state = Wait;
			}
			else if (D1 || ADC <= 50)
			{
				state = Wait;
			}
			else
			{
				state = stat6;
			}
			break;
		case stat7:
			if (D0 || ADC >= 1000)
			{
				state = Wait;
			}
			else if (D1 || ADC <= 50)
			{
				state = Wait;
			}
			else
			{
				state = stat7;
			}
		case Wait:
			if (D0 && !D1 || ADC >= 1000) {
				count = count + 1;
				if (count == 8)
				{
					count = 0;
				}
				state = Go;
			}
			else if (D1 && !D0 || ADC <= 50) {
				count = count - 1;
				if (count == (-1))
				{
					count = 7;
				}
				state = Go;
			}
			else {
				state = Wait;
			}
			break;
		case Go:
			if (count == 0)
			{
				state = stat0;
			}
			else if (count == 1)
			{
				state = stat1;
			}
			else if (count == 2)
			{
				state = stat2;
			}
			else if (count == 3)
			{
				state = stat3;
			}
			else if (count == 4)
			{
				state = stat4;
			}
			else if (count == 5)
			{
				state = stat5;
			}
			else if (count == 6)
			{
				state = stat6;
			}
			else if (count == 7)
			{
				state = stat7;
			}
			break;
		default:
			state = Init;
			break;
	}
	switch (state){ //Transitions
		case stat0:
			PORTD = 0xEF;
			PORTA = 0xFF;
			break;
		case stat1:
			PORTD = 0xDF;
			PORTA = 0xFF;
			break;
		case stat2:
			PORTA = 0xFB;
			PORTD = 0xFF;
			break;
		case stat3:
			PORTA = 0xF7;
			break;
		case stat4:
			PORTA = 0xEF;
			break;
		case stat5:
			PORTA = 0xDF;
			break;
		case stat6:
			PORTA = 0xBF;
			break;
		case stat7:
			PORTA = 0x7F;
			PORTD = 0xFF;
			break;
		case Wait:
			break;
		case Go:
			break;	
		case Init:
			count = 4;
			break;
	}
}

enum Stat {Initz, Rand0, Rand1, Rand2, Rand3, Rand4, Rand5, Rand6, Rand7} stat;
void Random()
{
	switch(stat)
	{
		case Initz:
			Red_Pos = rand() % 8;
			if(Red_Pos == 0)
			{
				stat = Rand0;
			}
			else if(Red_Pos == 1)
			{
				stat = Rand1;
			}
			else if(Red_Pos == 2)
			{
				stat = Rand2;
			}
			else if(Red_Pos == 3)
			{
				stat = Rand3;
			}
			else if(Red_Pos == 4)
			{
				stat = Rand4;
			}
			else if(Red_Pos == 5)
			{
				stat = Rand5;
			}
			else if(Red_Pos == 6)
			{
				stat = Rand6;
			}
			else if(Red_Pos >= 7)
			{
				stat = Rand7;
			}
			else
			{
				stat = Initz;
			}
			break;
		case Rand0:
			stat = Initz;
			break;
		case Rand1:
			stat = Initz;
			break;
		case Rand2:
			stat = Initz;
			break;
		case Rand3:
			stat = Initz;
			break;
		case Rand4:
			stat = Initz;
			break;
		case Rand5:
			stat = Initz;
			break;
		case Rand6:
			stat = Initz;
			break;
		case Rand7:
			stat = Initz;
			break;
		default:
			stat = Initz;
			break;
	}
	switch(stat)
	{
		case Init:
			break;
		case Rand0:
			PORTB = 0xFE;
			break;
		case Rand1:
			PORTB = 0xFD;
			break;
		case Rand2:
			PORTB = 0xFB;
			break;
		case Rand3:
			PORTB = 0xF7;
			break;
		case Rand4:
			PORTB = 0xEF;
			break;
		case Rand5:
			PORTB = 0xDF;
			break;
		case Rand6:
			PORTB = 0xBF;
			break;
		case Rand7:
			PORTB = 0x7F;
			break;
	}
}

enum Scores {Begin, Intro, Score, D_check} Sc_state;
unsigned char points = 0;
unsigned char score_time_sec = 0;
unsigned char score_time_tens = 0;
void LCD_Score()
{
	switch(Sc_state)
	{
		case Begin:
			Sc_state = Intro;
			break;
		case Intro:
			Sc_state = Score;
			break;
		case Score:
			Sc_state = Score;
			break;
		default:
			Sc_state = Begin;
			break;
	}
	switch(Sc_state)
	{
		case Begin:
			LCD_ClearScreen();
			break;
		case Intro:
			LCD_DisplayString(1, "Catch the Red   Line in 30 sec.");
			break;
		case Score:
			LCD_DisplayString(1, "Score:   Stage: Time:");
			LCD_Cursor(8);
			LCD_WriteData(point_val[points] + '0');
			LCD_Cursor(24);
			LCD_WriteData(score_time_sec + '0');
			LCD_Cursor(23);
			LCD_WriteData(score_time_tens + '0');
			LCD_Cursor(16);
			LCD_WriteData(stage_val + '0');
			break;	
	}
}

enum Comb_States {Comb_Init, Combine, Up_point, Reset, Dec_Score} Comb_state;	

enum Stage {Stage_Begin, Won, Lost, Stage1, Stage2, Stage3, Stage4, Stage5} stage;
void Stages()
{
	switch(stage)
	{
		case Stage_Begin:
			if(stage_val == 1)
			{
				stage = Stage1;	
			}
			else if(stage_val == 2)
			{
				stage = Stage2;
			}
			else if(stage_val == 3)
			{
				stage = Stage3;
			}
			else if(stage_val == 4)
			{
				stage = Stage4;
			}
			else if(stage_val == 5)
			{
				stage = Stage5;
			}
			break;
		case Stage1:
			if(points == 12)
			{
				stage = Stage_Begin;
			}
			else if(points != 12 && score_time_tens == 3 && score_time_sec == 1)
			{
				stage = Lost;
			}
			else
			{
				stage = Stage1;
			}
			break;
		case Stage2:
			if(points == 12)
			{
				stage = Stage_Begin;
			}
			else if(points != 12 && score_time_tens == 3 && score_time_sec == 1)
			{
				stage = Lost;
			}
			else
			{
				stage = Stage2;
			}
			break;
		case Stage3:
			if(points == 12)
			{
				stage = Stage_Begin;
			}
			else if(points != 12 && score_time_tens == 3 && score_time_sec == 1)
			{
				stage = Lost;
			}
			else
			{
				stage = Stage3;
			}
			break;
		case Stage4:
			if(points == 12)
			{
				stage = Stage_Begin;
			}
			else if(points != 12 && score_time_tens == 3 && score_time_sec == 1)
			{
				stage = Lost;
			}
			else
			{
				stage = Stage4;
			}
			break;
		case Stage5:
			if(points == 12)
			{
				stage = Won;
			}
			else if(points != 12 && score_time_tens == 2 && score_time_sec == 9)
			{
				stage = Lost;
			}
			else
			{
				stage = Stage5;
			}
			break;
		case Won:
			stage = Stage_Begin;
			break;
		case Lost:
			stage = Stage_Begin;
			break;
		default:
			stage = Stage_Begin;
			break;
	}
	switch(stage)
	{
		case Stage_Begin:
			points = 0;
			score_time_sec = 0;
			score_time_tens = 0;
			tens_score_elapsed = 0;
			score_elapsed = 0;
			stage_val += 1;
			Random_period -= 750;
			break;
		case Stage1:
			stage_val = 1;
			break;
		case Stage2:
			stage_val = 2;
			break;
		case Stage3:
			stage_val = 3;
			break;
		case Stage4:
			stage_val = 4;
			break;
		case Stage5:
			stage_val = 5;
			break;
		case Won:
			Winner();
			TimerOff();
			TimerSet(timerPeriod);
			TimerOn();
			score_time_sec = 0;
			score_time_tens = 0;
			stage_val = 0;
			Random_period = 4500;
			state = -1;
			Sc_state = -1;
			Comb_state = -1;
			stage = -1;
			stat = -1;
			break;
		case Lost:
			Loser();
			TimerOff();
			TimerSet(timerPeriod);
			TimerOn();
			score_time_sec = 0;
			score_time_tens = 0;
			stage_val = 0;
			Random_period = 4500;
			state = -1;
			Sc_state = -1;
			Comb_state = -1;
			stage = -1;
			stat = -1;
			break;
	}
}

unsigned char D3,D4;
void Tick_Rand_Comb()
{
	D3 = ~PIND & 0x04;
	D4 = ~PIND & 0x08;
	switch(Comb_state)
	{
		case Comb_Init:
			if(D3 && (PORTA == PORTB))
			{
				Comb_state = Combine;
			}
			else if(D3 && PORTD == 0xEF && PORTB == 0xFE)
			{
				Comb_state = Combine;
			}
			else if(D3 && PORTD == 0xDF && PORTB == 0xFD)
			{
				Comb_state = Combine;
			}
			/*else if(D3 && (PORTA != PORTB))
			{
				Comb_state = Dec_Score;
			}
			else if(D3 && PORTD != 0xEF && PORTB != 0xFE)
			{
				Comb_state = Dec_Score;
			}
			else if(D3 && PORTD != 0xDF && PORTB != 0xFD)
			{
				Comb_state = Dec_Score;
			}*/
			else if(D4)
			{
				Comb_state = Reset;
			}
			else
			{
				Comb_state = Comb_Init;
			}
			break;
		case Combine:
			Comb_state = Comb_Init;
			break;
		case Reset:
			Comb_state = Comb_Init;
			break;
		case Up_point:
			Comb_state = Comb_Init;
			break;
		case Dec_Score:
			Comb_state = Comb_Init;
			break;
		default:
			Comb_state = Comb_Init;
			break;
	}
	switch(Comb_state)
	{
		case Comb_Init:
			break;
		case Combine:
			points += 1;
			Random_elapsedTime = Random_period;
			break;
		case Dec_Score:
			points -= 1;
			if(points <= 0)
			{
				points = 0;
			}
			Random_elapsedTime = Random_period;
			break;
		case Reset:
			TimerOff();
			TimerSet(timerPeriod);
			TimerOn();
			score_time_sec = 0;
			score_time_tens = 0;
			stage_val = 0;
			points = 0;
			Random_period = 4500;
			state = -1;
			Sc_state = -1;
			Comb_state = -1;
			stage = -1;
			stat = -1;
			break;
		
	}
}

void Winner()
{
	TimerOff();
	LCD_ClearScreen();
	TimerSet(5000);
	TimerOn();
	LCD_DisplayString(1, "Congratulations, You Win!");
	return;
}

void Loser()
{
	TimerOff();
	LCD_ClearScreen();
	TimerSet(5000);
	TimerOn();
	LCD_DisplayString(1, "Sorry, You Lose :(");
	return;
}

int main(void)
{
	unsigned long Tick_elapsedTime = 60;
	const unsigned long Tick_period = 60;
	unsigned long LCD_Score_elapsedTime = 1000;
	const unsigned long LCD_Score_period = 1000;
	const unsigned long Comb_period = 50;
	const unsigned long score_period = 1000;
	const unsigned long long tens_score_period = 10000;
	unsigned long stages_elapsedTime = 50;
	const unsigned long stages_period = 50;
	
	
	DDRB = 0xFF; PORTB = 0x00;
	DDRA = 0xFC; PORTA = 0x03;
	DDRC = 0xFF; PORTC = 0x00;
	DDRD = 0xF0; PORTD = 0x0F;
	
	PORTD = 0xFF;
	PORTA = 0xFF;
	PORTB = 0xFF;
	srand(time(NULL));
	TimerSet(timerPeriod);
	TimerOn();
	ADC_init();
	LCD_init();
	LCD_ClearScreen();
	state = -1;
	Sc_state = -1;
	Comb_state = -1;
	
	while (1) {
		if(Tick_elapsedTime >= Tick_period)
		{
			Tick();
			Tick_elapsedTime = 0;
		}
		if(Comb_elapsedTime >= Comb_period)
		{
			Tick_Rand_Comb();
			Comb_elapsedTime = 0;
		}
		if(Random_elapsedTime >= Random_period)
		{
			Random();
			Random_elapsedTime = 0;
		}
		if(LCD_Score_elapsedTime >= LCD_Score_period)
		{
			LCD_Score();
			LCD_Score_elapsedTime = 0;
		}
		if(score_elapsed >= score_period)
		{
			score_time_sec += 1;
			if(score_time_sec == 10)
			{
				score_time_sec = 0;
			}
			score_elapsed = 0;
		}
		if(tens_score_elapsed >= tens_score_period)
		{
			score_time_tens += 1;
			if(score_time_tens == 6)
			{
				score_time_tens = 0;
			}
			tens_score_elapsed = 0;
		}
		score_elapsed += timerPeriod;
		tens_score_elapsed += timerPeriod;
		if(stages_elapsedTime >= stages_period)
		{
			Stages();
			stages_elapsedTime = 0;
		}
		while (!TimerFlag){}
		TimerFlag = 0;
		Tick_elapsedTime += timerPeriod;
		Random_elapsedTime += timerPeriod;
		LCD_Score_elapsedTime += timerPeriod;
		Comb_elapsedTime += timerPeriod;
		stages_elapsedTime += timerPeriod;
		
	}
	LCD_ClearScreen();
	LCD_DisplayString(1, "Done");
	return 1;
}

