/*
Included Files:		<avr/io.h>      AVR IO functions
					<stdio.h>
					"serial.c"
					<util/delay.h>
					"serial_LCD.c"
 
IO Connections:		This will require a couple of IO connections. The micro controller is connected to the LCD with A serial port connection, a 5 volt connection and a GND connection. 
					The TTL is utilized to read the serial connections without the LCD being connected. The connected LCD to the micro controller is also be connected to the pushbuttons
					that will allow the user to press a button.
					
Description:		This program uses and utilizes the UART (serial ports) on the ATMega2560. With the Liquid Crystal Display connected to the Arduino, the put_command_LCD command is used
					to display characters on the LCD. The LCD displays various commands inputed by user and will follow certain procedures from fortune teller to a random dice roller. 
					This program demonstrates serial communication between an ATMega2560 micro controller and an LCD display using the UART protocol
					
					1. Fortune Teller (magic 8-ball);
						- Generates random response when button is pressed
						- Displays one of four possible responses on the LCD
					
					2. Dice Simulator
						- Simulates Rolling two dice
						- Displays ASCII are representation of dice faces
						- Updates both dice simultaneously on button press
						
Key takeaways:		1. UART Communication:
						- Implementation of serial protocols for device communication
						- Understanding of baud rates and data transmission

					2. Random Number Generation:
						- Using button press duration as entropy source
						- Implementing modulo operation for range limitation

					3. LCD Control:
						- Command structures for LCD manipulation
						- Cursor positioning and text display
						- Custom character display for dice faces

					4. State Management:
						- Button debouncing
						- Multiple display modes
						- Screen clearing and updating			
 */ 

#include <avr/io.h>
#include <stdio.h>
#define F_CPU 16000000L
#include <util/delay.h>
#include "serial.c"
#include "serial_LCD.c"


int main(void)
{
	
	DDRA = 0x00;	//configured PORTA for input (pushbuttons)
	
	//PORTA = 0xFF;
	
	
	//Initialized variables for random number generation
	uint16_t randNumber = 0;
	uint16_t counter = 0;
	
	uint16_t randNum1 = 0;
	uint16_t randNum2 = 0;
	
	init_UART();		//initialize the serial ports
	printf("LCD Test Program - Lukas \n\r");
	init_LCD_UART();
	put_command_LCD(0x41); //LCD turn on
	put_command_LCD(0x51); //Clear LCD
	put_command_LCD(0x46); //Home display
	
	
	//=========TEST=============
	putstring_LCD("This is my LCD"); //display a message
	
	SetCursor(1,0);
	
	putstring_LCD("this is very lit");
	//comment the test section out to use other 
	
	
	//===========PROCEDURE 4=============
	
    while (1) 
    {
		printf("8 ball fortune teller");
		
		//Wait for button press
		while(PINA == 0)
		{
		}
		//count duration of button press for random seed
		while(PINA != 0)
		{
			counter++;
			_delay_ms(5);
		}
		
		//Generate random number and apply modulo for range 0-3
		srand(counter);
		randNumber = srand(counter) % 4;
		
		put_command_LCD(0x51); //Clear LCD
		put_command_LCD(0x46); //Home display
		
		printf("The random number mod 4 is %u\n\r", randNumber);
		
		//Display appropriate fortune based on random number
		switch (randNumber)
		{
			case 0: 
				printf("Random number is 0! \n\r");
				put_command_LCD(0x51); //Clear LCD
				SetCursor(1,0);
				putstring_LCD("     Not Today     ");
				break;
			case 1: 
				printf("Random number is 1! \n\r");
				put_command_LCD(0x51); //Clear LCD
				SetCursor(1,0);
				putstring_LCD("   Definitely Yes  ");
				break;
			case 2:
				printf("Random number is 2! \n\r");
				put_command_LCD(0x51); //Clear LCD
				SetCursor(1,0);
				putstring_LCD("  Try Again Later  ");
				break;
			case 3:
				printf("Random number is 3! \n\r");
				put_command_LCD(0x51); //Clear LCD
				SetCursor(1,0);
				putstring_LCD("       YES!!!      ");
				break;
		}	
    }
	
	
	//=========PROCEDURE 6===============
	printf("DIE randomizer");
	while(1)
	{	
		//Wait for button press
		while(PINA == 0)
		{
		}
		//count duration of button press for random seed
		while(PINA != 0)
		{
			counter++;
			_delay_ms(3);
		}
		
		//generate random numbers for both dice
		srand(counter);
		randNum1 = rand() % 5;
		randNum2 = rand() % 5;
		
		
		put_command_LCD(0x51); //Clear LCD
		put_command_LCD(0x46); //Home display
		putstring_LCD("   DIE 1     DIE 2  ");
		
		switch(randNum1){				//Switch case statements for the FIRST DIE ONLY
			case 0:
				printf("Die 1 rolled a 1\n\r");
				SetCursor(2,5);
				putstring_LCD("*");
				break;
			case 1:
				printf("Die 1 rolled a 2\n\r");
				SetCursor(2,4);
				putstring_LCD("* *");
				break;
			case 2:
				printf("Die 1 rolled a 3\n\r");
				SetCursor(1,4);
				putstring_LCD("*  ");
				SetCursor(2,4);
				putstring_LCD(" * ");
				SetCursor(3,4);
				putstring_LCD("  *");	
				break;
			case 3:
				printf("Die 1 rolled a 4\n\r");
				SetCursor(1,4);
				putstring_LCD("* *");
				SetCursor(3,4);
				putstring_LCD("* *");
				break;
			case 4:
				printf("Die 1 rolled a 5\n\r");
				SetCursor(1,3);
				putstring_LCD("*   *");
				SetCursor(2,5);
				putstring_LCD("*");
				SetCursor(3,3);
				putstring_LCD("*   *");
				break;
			case 5:
				printf("Die 1 rolled a 6\n\r");
				SetCursor(1,4);
				putstring_LCD("* *");
				SetCursor(2,4);
				putstring_LCD("* *");
				SetCursor(3,4);
				putstring_LCD("* *");
				break;
			
		}
		
	
			switch(randNum2){
				//Switch case statements for the SECOND DIE ONLY
				case 0:
					printf("Die 2 rolled a 1\n\r");
					SetCursor(2,15);
					putstring_LCD("*");
					break;
				case 1:
					printf("Die 2 rolled a 2\n\r");
					SetCursor(2,14);
					putstring_LCD("* *");
					break;
				case 2:
					printf("Die 2 rolled a 3\n\r");
					SetCursor(1,14);
					putstring_LCD("*  ");
					SetCursor(2,14);
					putstring_LCD(" * ");
					SetCursor(3,14);
					putstring_LCD("  *");
					break;
				case 3:
					printf("Die 2 rolled a 4\n\r");
					SetCursor(1,14);
					putstring_LCD("* *");
					SetCursor(3,14);
					putstring_LCD("* *");
					break;
				case 4:
					printf("Die 2 rolled a 5\n\r");
					SetCursor(1,13);
					putstring_LCD("*   *");
					SetCursor(2,15);
					putstring_LCD("*");
					SetCursor(3,13);
					putstring_LCD("*   *");
					break;
				case 5:
					printf("Die 2 rolled a 6\n\r");
					SetCursor(1,14);
					putstring_LCD("* *");
					SetCursor(2,14);
					putstring_LCD("* *");
					SetCursor(3,14);
					putstring_LCD("* *");
					break;
	
			}
		
		}
	
	
}

