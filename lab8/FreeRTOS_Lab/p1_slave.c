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
	
	// Set MOSI, SCK as Output
    DDRB=(1<<5)|(1<<3);
 
    // Enable SPI, Set as Master
    // Prescaler: Fosc/16, Enable Interrupts
    //The MOSI, SCK pins are as per ATMega8
    SPCR=(1<<SPE)|(1<<MSTR)|(1<<SPR0)|(1<<SPIE);
 
    // Enable Global Interrupts
    sei();
	
}

void SPI_MasterTransmit(unsigned char cData) {
// data in SPDR will be transmitted, e.g. SPDR = cData;
	// set SS low
	while(!(SPSR & (1<<SPIF))) { // wait for transmission to complete
		;
	}
// set SS high
}

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

char leds, i, p;

// Pattern flasher
void Task1()
{
    while(1)
    {
        
        // Actions
        switch(task1_state)
        {
            case INIT:
                SPI_ServantInit();
            break;
            
            case WAIT:
                if(hasData)
                {
                    PORTA = data;
                    hasData = 0;
                }
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
                // stay here
            break;
            
            default:
            break;
        }

        
    
        vTaskDelay(5);
    }

}

int main(void) 
{ 
    DDRA = 0xFF;// PORTA=0xFF;
    DDRC = 0xFF;
    DDRB = 0x00; PORTB=0xFF;
   
    task1_state = INIT;
    task2_state = INIT;
      
    xTaskCreate(Task1, (signed portCHAR *)"Task1", configMINIMAL_STACK_SIZE, NULL, 1, NULL );
 
    vTaskStartScheduler();
    
    return 0; 
}