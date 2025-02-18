/*******************************
	Name:	Jeffrey J. Richardson
	Date:	October 5, 2023
	
	Special UART routines to communicate with the New Haven Serial LCD

	IO: This version of the code is setup to utilize UART3 (Pin TX3)
		This can be changed by changing the UART # in the UART initialization routine

	Usage:	Make certain to call the init_LCD_UART() first to setup the communication channel
			Then, use the put_command_LCD() to turn on, clear, and home the display
			
			Once the display is initialized, you can move the cursor to a desired location
				using the put_cursor_LCD().  Alternatively, the put_cursor_xy() could be used.
			
			The putstring_LCD() can be utilized to send a string of characters like "Hello World"
			
			If you have a single value to send to the LCD, the putchar_LCD() can be used.


********************************/

// these are the function prototypes

void init_LCD_UART(void);
void putchar_LCD(uint8_t);
void put_command_LCD(uint8_t);
void put_cursor_LCD(uint8_t);
void putstring_LCD(char *str);
void SetCursor(uint8_t, uint8_t);

/***************************************************************************************************
	
	This function must be called first to setup the communication channel
	
	UART3 is setup for 9600, N, 8, 1 with a 16 MHz crystal
	
***************************************************************************************************/
void init_LCD_UART(void)
{

	UCSR3A = 0x00;			
	UCSR3B = 0x18;			// enable UART TX and RX
	UCSR3C = 0x06;			// set the UART for N, 8, 1
	UBRR3L = 103;			// set BAUD Rate for 9600 with 16MHz clock
	UBRR3H = 0;

}

/***************************************************************************************************

	This is the lowest level function that is actually responsible for sending data to the display through the UART
	
	This function ensures that the UART buffer is open to receive a character.  If the buffer is not
	open, then the function will wait until the buffer is open before loading the next character.
	
	This function is called by the other functions, but it can also be called direct.

***************************************************************************************************/
void putchar_LCD(uint8_t ch)
{

    while ( (UCSR3A & (1 << UDRE3)) == 0 )		// wait until there is room in the transmit buffer
	{
	}

    UDR3 = ch;									// load the character into the UART data register

}

/***************************************************************************************************

	This function is designed to send commands to the serial LCD.
	
	The function sends the command indicator value of 0xFE first, then the value, and then delays to allow the LCD
	time to execute the command.  This function allows the main code to be simplified 

***************************************************************************************************/
void put_command_LCD(uint8_t cmd)
{
	putchar_LCD(0xFE);
	putchar_LCD(cmd);
	
	_delay_ms(5);
}

/***************************************************************************************************

	This function is designed to send the cursor to a specific location.
	
	The function adds the two-sequence command prior to sending the cursor location.  This allows the 
	main code to be simplified by not requiring the main function to send all three values

***************************************************************************************************/
void put_cursor_LCD(uint8_t cmd)
{
	putchar_LCD(0xFE);
	putchar_LCD(0x45);
	putchar_LCD(cmd);
	
	_delay_ms(5);
}

/***************************************************************************************************

	This function will send a string of characters to the LCD.  
	
	This is a simple function and does allow the formatting of messages.  It only sends the final string
	of characters like "Hello".
	
	If you need to format a string of characters, then utilize the sprintf function to format the data
	into an array, and then utilize this function to send the formatted message that is stored in the array

***************************************************************************************************/
void putstring_LCD(char *str)
{
	while( *str != 0)
	{
		putchar_LCD(*str++);
	}
}


/***************************************************************************************************

	This function will send the cursor to a specific location specified by an X, Y value, where X is
	the vertical line and Y is the horizontal or column position on the line.  
	
	This function is an alternative to the send put_cursor function.  Use whichever you prefer
	
***************************************************************************************************/
void SetCursor(uint8_t ypos, uint8_t xpos) 
{
	uint8_t position;
	
	// Bounding the X-Position for a 20 character display
	if (xpos > 19)		// cannot be higher than 19.  The first position is 0 
	{
		xpos = 19;		// if over, then set to the maximum allowed value
	}

	// Bounding the Y-Position
	if (ypos > 3)		// can't be higher than 3 for a 4-line display
	{
		ypos = 3;		// set to the third line if over the maximum value
	}
	
	// set the starting value depending on which line it is
	if ( ypos == 0)
	{
		position = 0;		// value for the first position on Line 0
	}
	
	if ( ypos == 1)
	{
		position = 64;		// value for the first position on Line 0
	}
	
	if ( ypos == 2)
	{
		position = 20;		// value for the first position on Line 0
	}
	
	if ( ypos == 3)
	{
		position = 84;		// value for the first position on Line 0
	}

	position = position + xpos;		// this combines the starting value for the line with the horizontal position
									// to form the final value that must be sent to the LCD to set the cursor

	put_cursor_LCD(position);   // send address to the LCD using the previous function
}