/*  Thomas Zwaagstra <tzwaa001@ucr.edu>
*   Nick Gingerella <nging001@ucr.edu>
*   Lab section: 22
*   Lab 6 Part 1
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
/*
	// set DDRB to have MISO(PB6) line as output and MOSI(PB5), SCK(PB7), and SS(PB4) as input
	DDRB |= 0x40; //01000000;
	DDRB &= 0x4F; //01001111;
	PORTB &= 0xb0; //10110000
	
	// set SPCR register to enable SPI and enable SPI interrupt (pg. 168)
	SPCR |= SPIE | SPE;
	
	// make sure global interrupts are enabled on SREG register (pg. 9)
	SREG |= 0x80;
	
	
DDRB = (1<<6);     //MISO as OUTPUT
//SPCR = (1<<SPE);   //Enable SPI
SPCR |= SPIE | SPE;

sei();

*/

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
MASTER,
SLAVE
}
task1_state, task2_state;

unsigned char led0, s0;

void Task1()
{
    while(1)
    {
        // Input
        s0 = !(PINB & 0x01);
        
        // Actions
        switch(task1_state)
        {
            case INIT:
                initUSART(0);
                initUSART(1);
            break;
            
            case MASTER:
            
                if(USART_IsSendReady(1))
                {
                    USART_Send(led0, 1);
                }
                
                PORTA = led0;
            
            break;
            
            case SLAVE:
                if(USART_HasReceived(0))
                {
                    led0 = USART_Receive(0);
                    USART_Flush(0);
                }
                
                PORTA = led0;
            
            break;
            
            default:
            break;
        }
        
        // Transitions
        switch(task1_state)
        {
            case INIT:
                task1_state = s0 ? MASTER : SLAVE;
            break;
            
            case MASTER:
                task1_state = s0 ? MASTER : SLAVE;
            break;
            
            case SLAVE:
                task1_state = s0 ? MASTER : SLAVE;
            break;
            
            default:
            break;
        }
        
    
        vTaskDelay(10);
    }

}

void Task2()
{
    while(1)
    {
        // Actions
        switch(task2_state)
        {
            case INIT:
                led0 = 0x01;
            break;
            
            case MASTER:
                led0 = !led0;
            
            break;
            
            case SLAVE:
                // Do nothing... wait!
            break;
            
            default:
            break;
        }
        
        // Transitions
        switch(task2_state)
        {
            case INIT:
                task2_state = s0 ? MASTER : SLAVE;
            break;
            
            case MASTER:
                task2_state = s0 ? MASTER : SLAVE;
            break;
            
            case SLAVE:
                task2_state = s0 ? MASTER : SLAVE;
            break;
            
            default:
            break;
        }
    
        
        
        vTaskDelay(1000);
    }
}

int main(void) 
{ 
    DDRA = 0xFF;// PORTA=0xFF;
    DDRC = 0xFF;
    DDRB = 0x00; PORTB=0xFF;
    
    led0 = 0x01;
   
    task1_state = INIT;
    task2_state = INIT;
      
    xTaskCreate(Task1, (signed portCHAR *)"Task1", configMINIMAL_STACK_SIZE, NULL, 1, NULL );
    xTaskCreate(Task2, (signed portCHAR *)"Task2", configMINIMAL_STACK_SIZE, NULL, 1, NULL );
 
    vTaskStartScheduler();
    
    return 0; 
}