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

// SPI stuff
char hasData = 0;
unsigned char data; 

// Master code
void SPI_MasterInit(void) {
	// Set DDRB to have MOSI, SCK, and SS as output and MISO as input
	// Set SPCR register to enable SPI, enable master, and use SCK frequency
//   of fosc/16  (pg. 168)
	// Make sure global interrupts are enabled on SREG register (pg. 9)
}

void SPI_MasterTransmit(unsigned char cData) {
// data in SPDR will be transmitted, e.g. SPDR = cData;
	// set SS low
	while(!(SPSR & (1<<SPIF))) { // wait for transmission to complete
		;
	}
// set SS high
}

// Servant code
void SPI_ServantInit(void) {
	// set DDRB to have MISO(PB6) line as output and MOSI(PB5), SCK(PB7), and SS(PB4) as input
	DDRB |= 0x40; //01000000;
	DDRB &= 0x4F; //01001111;
	
	// set SPCR register to enable SPI and enable SPI interrupt (pg. 168)
	SPCR |= SPIE | SPE;
	
	// make sure global interrupts are enabled on SREG register (pg. 9)
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

const char pattern[4][9] = 
{
{2, 0xF0, 0x0F},
{2, 0xAA, 0x55},
{8, 0x80, 0x40, 0x20, 0x10, 0x08, 0x04, 0x02, 0x01},
{4, 0x81, 0x42, 0x24, 0x18}
};

// Number of patterns
const char num = 4;

char leds, i, p, count, rate, run;

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
            
            case SET:
                leds = 0;
                i = 0;
                p = 3;
                rate = 5;
                count = 0;
            break;
            
            case WAIT:
                // Nothing to see here...
            break;
            
            case PLAY:
                if(run)
                {
                    leds = pattern[p][i+1];
                    i = (i + 1) % (pattern[p][0]);
                    PORTA = leds;
                }
            break;
            
            default:
            break;
        }
        
        // Transitions
        switch(task1_state)
        {
            case INIT:
                if(run)
                {
                    task1_state = SET;
                }
            break;
            
            case SET:
                run = 1;
                task1_state = WAIT;
            break;
            
            case WAIT:
                if(!run)
                {
                    task1_state = SET;
                }
            
                if(count >= rate)
                {
                    task1_state = PLAY;
                    count = 0;
            	}
            	else
            	{
            	    count++;
            	}
            break;
            
            case PLAY:
                if(!run)
                {
                    task1_state = SET;
                }
            
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
                run = 0;
            
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
                
                /*
                    if(receive)
                    {
                        
                        p = data >> 4;
                        rate = (data & 0x0F);
                        run = 0;
                    }
                
                */
            break;
        
            default:
            break;
        }
    
    }
    
    vTaskDelay(10);

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