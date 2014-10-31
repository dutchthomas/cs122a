/*  Thomas Zwaagstra <tzwaa001@ucr.edu>
*   Nick Gingerella <nging001@ucr.edu>
*   Lab section: 22
*   Lab 5 Part 4
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

void transmit_data(unsigned char data, unsigned char line);

void transmit_data(unsigned char data, unsigned char line)
{
	int i;
	
	for(i = 7; i >= 0; --i)
	{
	    // Sets SRCLR to 1 to allow data to be set
	    // clears SRCLK in preparation of sending data
	    if(line == 0)
	    {
		    PORTC = 0x08;
	    }
	    else if(line == 1)
	    {
		    PORTC = 0x20;
	    }
		
		// set SER = next bit of data to be sent
		PORTC |= ((data >> i) & 0x01);
		
		// set SRCLK = 1. Rising edge shifts next bit of data into the shift register.
		PORTC |= 0x04;
	}
	
	// set RCLK = 1. Rising edge copies data from "shift" to "storage"
	if(line == 0)
	{
	    PORTC |= 0x02;
    }
    else if(line == 1)
    {
	    PORTC |= 0x10;
    }
	
	// clears all lines for new transmission
	PORTC = 0x00;
}

void A2D_init() {
      ADCSRA |= (1 << ADEN) | (1 << ADSC) | (1 << ADATE);
    // ADEN: Enables analog-to-digital conversion
	// ADSC: Starts analog-to-digital conversion
	// ADATE: Enables auto-triggering, allowing for constant
	//	    analog to digital conversions.

}

// Pins on PORTA are used as input for A2D conversion
//    The default channel is 0 (PA0)
// The value of pinNum determines the pin on PORTA
//    used for A2D conversion
// Valid values range between 0 and 7, where the value
//    represents the desired pin for A2D conversion
		
void Set_A2D_Pin(unsigned char pinNum)
{
    ADMUX = (pinNum <= 0x07) ? pinNum : ADMUX;
    // Allow channel to stabilize
    static unsigned char i = 0;
    for ( i=0; i<15; i++ ) { asm("nop"); } 
}

enum state_labels
{
INIT,
WAIT,
PRESS,
RELEASE,
REACT
}
task1_state, task2_stateA, task2_stateB, task3_state;

char field[5][8] =
{
{0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0}
};

unsigned char b0, b1, row, col;

unsigned char c0, c1;

unsigned short divider, min, max, input0, input1;

void Task1()
{
    int i, j;
    unsigned char t1, t2;

    while(1)
    {
        for(i = 0; i < 8; i++)
        {
            t1 =  (0x01 << i);
            
            // Disable column
            transmit_data(0, 1);
        
            // which rows
            
            t2 = 0;
            
            for(j = 0; j < 5; j++)
            {
                t2 |= field[4-j][7-i] << j;
            }
            
            transmit_data(~(t2), 0);
            
            // which column
            transmit_data(t1, 1);
            

            vTaskDelay(1);

        }
        
    }

}

void controlTick(unsigned short *r, unsigned short *t0, signed char *dir, unsigned char *c, int *task_state, unsigned char *pos, unsigned short *input, int range)
{
        // Actions
        switch(*task_state)
        {
            case INIT:
                *pos = 0;
                *c = 0;
                min = 0;
                max = 1024;
                *dir = 0;
                
                divider = (max - min)/9;
            break;
            
            case WAIT:
                *r = *input / divider;
               
                if(*r < 4)
                {
                    *r = 4 - *r;
                    *dir = -1;
                }
                else if(*r == 4)
                {
                    *r = 0;
                    *dir = 0;
                }
                else if (*r > 4)
                {
                    *r = *r - 4;
                    *dir = 1;
                }
                
                switch(*r)
                {
                    case 0:
                        *t0 = 0;
                    break;
                    
                    case 1:
                        *t0 = 20;
                    break;
                    
                    case 2:
                        *t0 = 10;
                    break;
                    
                    case 3:
                        *t0 = 5;
                    break;
                    
                    case 4:
                        *t0 = 2;
                    break;
                }
                
                (*c)++;
                
            break;
            
            case REACT:
                if(*dir > 0)
                {
                    if(*pos > 0)
                    {
                        (*pos)--;
                    }
                    else
                    {
                        *pos = range;
                    }
                }
                else if(*dir < 0)
                {
                    if(*pos < range)
                    {
                        (*pos)++;
                    }
                    else
                    {
                        *pos = 0;
                    }
                }
            
                *c = 1;
            break;
            
            default:
            break;
        }
        
        // Transitions
        switch(*task_state)
        {
            case INIT:
                *task_state = WAIT;
            break;
            
            case WAIT:
                if(*c > *t0)
                {
                    *task_state = REACT;
                }
            break;
            
            case REACT:
                *task_state = WAIT;
            break;
            
            default:
            break;
        }

}

void Task2()
{
    int i, j;
    
    unsigned short r0, r1, t0, t1;
    
    signed char dir0, dir1;

    while(1)
    {
        controlTick(&r0, &t0, &dir0, &c0, &task2_stateA, &row, &input0, 8);
        
        controlTick(&r1, &t1, &dir1, &c1, &task2_stateB, &col, &input1, 5);
        
        // Write field
        
        for(i = 0; i < 8; i++)
        {
            for(j = 0; j < 5; j++)
            {
                if(i == row && j == col)
                {
                    field[j][i] = 1;
                }
                else
                {
                    field[j][i] = 0;
                }
                
            }
        }
        
        vTaskDelay(50);
    }
}

void Task3()
{
    while(1)
    {
        Set_A2D_Pin(0);
        vTaskDelay(5);
        input0 = ADC;
        Set_A2D_Pin(1);
        vTaskDelay(5);
        input1 = ADC;
    }
}

int main(void) 
{ 
    //DDRA = 0x00; PORTA=0xFF;
    DDRC = 0xFF;
    DDRD = 0xFF;
    DDRB = 0xFF;
    
    A2D_init();
    
    transmit_data(0x00, 0);
    transmit_data(0xFF, 1);
   
    task1_state = INIT;
    task2_stateA = INIT;
    task2_stateB = INIT;
    task3_state = INIT;
  
    xTaskCreate(Task1, (signed portCHAR *)"Task1", configMINIMAL_STACK_SIZE, NULL, 1, NULL );
    xTaskCreate(Task2, (signed portCHAR *)"Task2", configMINIMAL_STACK_SIZE, NULL, 1, NULL );
    xTaskCreate(Task3, (signed portCHAR *)"Task3", configMINIMAL_STACK_SIZE, NULL, 1, NULL );
 
    vTaskStartScheduler();
    
    return 0; 
}