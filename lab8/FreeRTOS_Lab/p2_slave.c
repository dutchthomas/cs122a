/*  Thomas Zwaagstra <tzwaa001@ucr.edu>
*   Nick Gingerella <nging001@ucr.edu>
*   Lab section: 22
*   Lab 8 Part 1
*
*   I acknowledge all content contained herein, excluding template or example code,
*   is my own original work.
*/

#include <stdint.h> 
#include <stdlib.h> 
#include <stdio.h> 
#include <stdbool.h> 
#include <string.h> 
#include <math.h> 
#include <avr/io.h> 
#include <avr/interrupt.h> 
#include <avr/eeprom.h> 
#include <avr/portpins.h> 
#include <avr/pgmspace.h> 
 
//FreeRTOS include files 
#include "FreeRTOS.h" 
#include "task.h" 
#include "croutine.h" 

#include "usart_ATmega1284.h"

////////////////////////////////////////////////////////////////////////////////

#ifndef BIT_H
#define BIT_H

////////////////////////////////////////////////////////////////////////////////
//Functionality - Sets bit on a PORTx
//Parameter: Takes in a uChar for a PORTx, the pin number and the binary value 
//Returns: The new value of the PORTx
unsigned char SetBit(unsigned char pin, unsigned char number, unsigned char bin_value) 
{
	return (bin_value ? pin | (0x01 << number) : pin & ~(0x01 << number));
}

////////////////////////////////////////////////////////////////////////////////
//Functionality - Gets bit from a PINx
//Parameter: Takes in a uChar for a PINx and the pin number
//Returns: The value of the PINx
unsigned char GetBit(unsigned char port, unsigned char number) 
{
	return ( port & (0x01 << number) );
}

#endif //BIT_H


// SPI stuff
char hasData = 0;
unsigned char data; 

//========================== SPI specific functons =============================
// Master code

void SPI_MasterInit(void) {
	// Set DDRB to have MOSI, SCK, and SS as output and MISO as input
	 DDRB |= (1 << DDB4) | (1 << DDB5) | (1 << DDB7);
	 DDRB &= ~(1 << DDB6);
	// Set SPCR register to enable SPI, enable master, and use SCK frequency
	// of fosc/16  (pg. 168)
	SPCR = (1 << SPE) | (1 << MSTR) | (1 << SPR0);
	
	// Make sure global interrupts are enabled on SREG register (pg. 9)
	SREG |= 0x80;
}


// Transmit data from the master to the  slave using the SPi Data Register
void SPI_MasterTransmit(unsigned char cData) {

	// set SS low
	PORTB = SetBit(PORTB,4,0);

	// data in SPDR will be transmitted, e.g. SPDR = cData;
	SPDR = cData;

	// wait for transmission to complete
	while(!(SPSR & (1<<SPIF))) { 
		;
	}

	// set SS high
	PORTB = SetBit(PORTB,4,1);
}
//=============================================================================

// Servant code 4F B0 C0
void SPI_ServantInit(void) {
DDRB = 0x4F; PORTB = 0xB0;
SPCR = 0xC0;
SREG |= 0x80;
}

ISR(SPI_STC_vect) { // this is enabled in with the SPCR register’s “SPI
  // Interrupt Enable”
	// SPDR contains the received data, e.g. unsigned char receivedData =
// SPDR;

    if(hasData == 0)
    {
        data = SPDR;
        hasData = 1;
    }

}

enum state_labels
{
INIT,
WAIT,
SET,
PLAY
}
task1_state, task2_state;

const char pattern[4][16] = 
{
{2, 0xF0, 0x0F},
{2, 0xAA, 0x55},
{14, 0x80, 0x40, 0x20, 0x10, 0x08, 0x04, 0x02, 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40},
{4, 0x81, 0x42, 0x24, 0x18}
};

const unsigned char rates[6] = {40, 20, 10, 5, 2, 1};

// Number of patterns
const char num = 4;

char leds, i, p, count, rate, run, old;

// Pattern flasher
void Task1()
{
    while(1)
    {
        
        // Actions
        switch(task1_state)
        {
            case INIT:

            break;
            
            case WAIT:
                // Nothing to see here...
            break;
            
            case PLAY:
                    leds = pattern[p][i+1];
                    i = (i + 1) % (pattern[p][0]);
                    PORTA = leds;
            break;
            
            default:
            break;
        }
        
        // Transitions
        switch(task1_state)
        {
            case INIT:
                task1_state = WAIT;
            break;
            
            case WAIT:
            
                if(count >= rate)
                {
                    task1_state = PLAY;
                    count = 1;
            	}
            	else
            	{
            	    count++;
            	}
            break;
            
            case PLAY:
            
                task1_state = WAIT;
            break;
            
            default:
            break;
        }
    
        vTaskDelay(50);
    }

}

// Selector task

void Task2()
{
    while(1)
    {
        // Actions
        switch(task2_state)
        {
            case INIT:
                rate = rates[0];
                p = 0;
            break;
            
            case WAIT:
                
            break;
        
            default:
            break;
        }
        
        // Transitions
        switch(task2_state)
        {
            case INIT:
                task2_state = WAIT;
            break;
            
            case WAIT:
                
                
                    if(hasData)
                    {
                        if(data != old)
                        {
                            //i = 0;
                            p = (data >> 4)-1;
                            
                            
                            rate = rates[(data & 0x0F)-1];
                            count = 1;
                            old = data;
                        }
                        
                        hasData = 0;
                    }
                
                
            break;
        
            default:
            break;
        }
    
    }
    
    vTaskDelay(100);

}

int main(void) 
{ 
    DDRA = 0xFF;
    
    SPI_ServantInit();
   
    task1_state = INIT;
    task2_state = INIT;
      
    xTaskCreate(Task1, (signed portCHAR *)"Task1", configMINIMAL_STACK_SIZE, NULL, 1, NULL );
    xTaskCreate(Task2, (signed portCHAR *)"Task2", configMINIMAL_STACK_SIZE, NULL, 1, NULL );
 
    vTaskStartScheduler();
    
    return 0; 
}