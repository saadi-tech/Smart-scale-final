#include<avr/io.h>

#define  F_CPU 8000000UL

#define ADMUX_ADCMASK  ((1 << MUX3)|(1 << MUX2)|(1 << MUX1)|(1 << MUX0))
#define ADMUX_REFMASK  ((1 << REFS1)|(1 << REFS0))

#define ADMUX_REF_AREF ((0 << REFS1)|(0 << REFS0))
#define ADMUX_REF_AVCC ((0 << REFS1)|(1 << REFS0))
#define ADMUX_REF_RESV ((1 << REFS1)|(0 << REFS0))
#define ADMUX_REF_VBG  ((1 << REFS1)|(1 << REFS0))

#define ADMUX_ADC_VBG  ((1 << MUX3)|(1 << MUX2)|(1 << MUX1)|(0 << MUX0))


#include<util/delay.h>

#include "spi.h"
#include "st7735.h"
#include "st7735_gfx.h"
#include "st7735_font.h"
#include <string.h>
#include "logo_bw.h"
#include "free_sans.h"
#include <stdio.h>

void clear_screen();
void show_main_screen(float,char[],float,uint16_t,char[]);
char read_button_press();
void show_button_pressed(char);
uint16_t measure_supply();


char button_pressed;   //1-> Enter... 2-> Back  3-> Next   4-> Previous

char string[10];   //string for writing formatting on the LCD.
char menu[10][16];

char enter = 2;
char back = 1;
char next = 4;
char previous = 3;

char exited = 0 ;
uint8_t menu_index = 0;

int main(void) {
	uart_init(9600);
	DDRC = DDRC & (0b1000011);
	PORTC |= (0b111100);
    spi_init();
    st7735_init();
	st7735_set_orientation(ST7735_LANDSCAPE);
	clear_screen();
	
	ADMUX = (1 << REFS1) | (1 << REFS0)| (0 << ADLAR)| (0 << MUX3) | (0 << MUX2) | (0 << MUX1) | (0 << MUX0);
	ADCSRA = (1 << ADEN)| (0 << ADSC)| (0 << ADATE)| (1 << ADPS2)|(0 << ADPS1)|(1 << ADPS0);
	ADCSRA |= (1 << ADSC);         // start dummy conversion
	while (ADCSRA & (1 << ADSC));  // wait for dummy to finish
	
	
	show_main_screen(69.7,"g/cm3",2.65,300,"g/cm3");
	
	
	
	while(1){
    
    button_pressed = read_button_press();
	
		if (button_pressed == enter){
					//if enter is pressed...
			set_menu(0);
			menu_index = 0;
			show_option(menu_index);
			
			
			while(exited == 0 ){
				button_pressed = read_button_press();
				
				
					
					switch (button_pressed){
						case (4):
							if(menu_index<5){
							menu_index++;
							show_option(menu_index);
							}
							break;
						
						case(3):
							if(menu_index>0){
							menu_index --;
							show_option(menu_index);
							}
							break;
						
						case(1):
							exited = 1;
							break;
							
						case(2):
							
							switch (menu_index){
								case 5:
								show_battery_screen();
								break;
							
							}
							
							break;
							
						
					}		
				
	}
	exited = 0;
	menu_index = 0;
	show_main_screen(69.7,"g/cm3",2.65,300,"g/cm3");
	
		

	}
    
}

}

void clear_screen(){
	st7735_fill_rect(0, 0, 160, 128, ST7735_COLOR_BLACK);
}

void show_main_screen(float value, char unit[],float solid_sg,uint16_t sample_number,char sg_unit[]){
	clear_screen();
	
	
	dtostrf(value,4, 1, string);
	
	st7735_draw_text(0, 32, string, &FreeSans, 2, ST7735_COLOR_WHITE);
	st7735_draw_text(80, 32, unit, &FreeSans, 1, ST7735_COLOR_WHITE);
	
	
	dtostrf(solid_sg,4, 1, string);
	strcat(string,sg_unit);
	st7735_draw_text(0, 70, "Solid SG :", &FreeSans, 1, ST7735_COLOR_WHITE);
	st7735_draw_text(40, 85, string, &FreeSans, 1, ST7735_COLOR_WHITE);
	
	itoa(sample_number,string,10);
	
	st7735_draw_text(0, 118, "Sample pt :", &FreeSans, 1, ST7735_COLOR_WHITE);
	st7735_draw_text(110, 118, string, &FreeSans, 1, ST7735_COLOR_WHITE);
	
	
	
}


char read_button_press(){
	
	if (  !(PINC & (1 << PINC5))  ){
		//1 pressed...
		return 1;
		
	}
	if (  !(PINC & (1 << PINC4))  ){
		//2 pressed...
		return 2;
		
	}
	if (  !(PINC & (1 << PINC3))  ){
		//3 pressed...
		return 3;
		
	}
	if (  !(PINC & (1 << PINC2))  ){
		//4 pressed...
		return 4;
		
	}
	
	return 0;
	
}

void show_button_pressed(char index){
	
	clear_screen();
	
	switch (index)
	{
		
		case 1:
		st7735_draw_text(20, 75, "BACK", &FreeSans, 2, ST7735_COLOR_WHITE);
		break;
		
		case 2:
		st7735_draw_text(20, 75, "ENTER", &FreeSans, 2, ST7735_COLOR_WHITE);
		break;
		
		case 3:
		st7735_draw_text(20, 75, "PREV.", &FreeSans, 2, ST7735_COLOR_WHITE);
		break;
		
		case 4:
		st7735_draw_text(20, 75, "NEXT", &FreeSans, 2, ST7735_COLOR_WHITE);
		break;
		
		
	}
	_delay_ms(250);
	
}


void set_menu(char index){
	
	switch (index){
		
		case 0:
		strcpy(menu[0],"Sample point");
		strcpy(menu[1],"Set Solid SG");
		strcpy(menu[2],"Zero");
		strcpy(menu[3],"Measurement Type");
		strcpy(menu[4],"Set Units");
		strcpy(menu[5],"Battery Status");
		
		break;
		
		
		
	}
	
	
}

void show_option(char index){
	clear_screen();
	st7735_draw_text(70, 15, "Select", &FreeSans, 1, ST7735_COLOR_WHITE);
	st7735_draw_text(5, 75, menu[index], &FreeSans, 1, ST7735_COLOR_WHITE);
}

void show_sample_point_menu(){
	clear_screen();
	
}

void print_battery(){
	uint16_t battery = measure_supply();
	
	serial_write("Battery:");
	
	itoa(battery, string,10);
	serial_writeln(string);
}

void show_battery_screen(){
	show_battery_status();
	
	while(1){
		button_pressed = read_button_press();
		if (button_pressed == back){
		return;
		}
		
	}
}
void show_battery_status(){
	uint16_t battery = measure_supply();
	
	int percentage = ((battery-3300)/1700.0)*100;
	itoa(percentage, string,10);
	
	
	clear_screen();
	
	
	strcat(string,"%");
	st7735_draw_text(50, 42, string, &FreeSans, 2, ST7735_COLOR_WHITE);
	
	char clr = 0; //0 -> Red, 1 -> Yellow, 2 ->Green
	st7735_fill_rect(50,50,52,25,ST7735_COLOR_WHITE);
	if(percentage<33){
		st7735_fill_rect(51,51,16,23,ST7735_COLOR_RED);
	}
	if (percentage>=33 && percentage<66){
		st7735_fill_rect(51,51,16,23,ST7735_COLOR_YELLOW);
		st7735_fill_rect(68,51,16,23,ST7735_COLOR_YELLOW);
	}
	if (percentage>=66){
		st7735_fill_rect(51,51,16,23,ST7735_COLOR_GREEN);
		st7735_fill_rect(68,51,16,23,ST7735_COLOR_GREEN);
		st7735_fill_rect(85,51,16,23,ST7735_COLOR_GREEN);
	}

}

// measure supply voltage in mV
uint16_t measure_supply(void)
{
	ADMUX &= ~(ADMUX_REFMASK | ADMUX_ADCMASK);
	ADMUX |= ADMUX_REF_AREF;      // select AVCC as reference
	ADMUX |= ADMUX_ADC_VBG;       // measure bandgap reference voltage
	
	_delay_us(500);               // a delay rather than a dummy measurement is needed to give a stable reading!
	ADCSRA |= (1 << ADSC);        // start conversion
	while (ADCSRA & (1 << ADSC)); // wait to finish
	
	return (1100UL*1023/ADC);     // AVcc = Vbg/ADC*1023 = 1.1V*1023/ADC
}


void uart_init(int baud_rate){
	
	//int value =     ((freq)/(16*baud_rate)) - 1;
	int value = 51;
	UBRR0H = (value>>8);
	UBRR0L = (value);
	
	UCSR0C = 0x06;       /* Set frame format: 8data, 1stop bit  */
	UCSR0B = (1<<TXEN0); /* Enable  transmitter                 */
}

void serial_write(unsigned char data[]){
	int length = strlen(data);
	int i = 0;
	for (i = 0; i < length ; i++){
		UDR0 = data[i];
		while (!( UCSR0A & (1<<UDRE0))); /* Wait for empty transmit buffer*/
		
		
	}
	
}

void serial_writeln(unsigned char data[]){
	int length = strlen(data);
	int i = 0;
	for (i = 0; i < length ; i++){
		UDR0 = data[i];
		while (!( UCSR0A & (1<<UDRE0))); /* Wait for empty transmit buffer*/
		
		
	}
	
	serial_write("\n\r");
	
}

void serial_putch(unsigned char value){
	UDR0 = value;
	while (!( UCSR0A & (1<<UDRE0))); /* Wait for empty transmit buffer*/
	
}