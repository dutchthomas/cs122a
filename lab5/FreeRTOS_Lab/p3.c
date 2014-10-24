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


enum state_labels
{
INIT,
WAIT,
PRESS,
RELEASE
}
task1_state, task2_state;

char field[5][8] =
{
{0,0,0,0,0,0,0,0},
{0,0,1,1,1,1,0,0},
{0,0,1,0,0,1,0,0},
{0,0,1,1,1,1,0,0},
{0,0,0,0,0,0,0,0}
};

unsigned char b0, b1, row;


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
            
            vTaskDelay(2);


        }
    }

}

void Task2()
{
    int i, j;

    while(1)
    {
        b0 = !(PINA & 0x01);
        b1 = !(PINA & 0x02);
    
        // Actions
        switch(task2_state)
        {
            case INIT:
                row = 0;
            break;
            
            case PRESS:
                if(b0)
                {
                    row = (row + 1)%5;
                }
                else if(b1)
                {
                    if(row == 0)
                    {
                        row = 4;
                    }
                    else
                    {
                        row--;
                    }
                }
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
                if(b0 || b1)
                {
                    task2_state = PRESS;
                }
            break;
            
            case PRESS:
                task2_state = RELEASE;
            break;
            
            case RELEASE:
                if(!b0 && !b1)
                {
                    task2_state = WAIT;
                }
            break;
            
            default:
            break;
        }
        
        for(i = 0; i < 8; i++)
        {
            for(j = 0; j < 5; j++)
            {
                if(j == row)
                {
                    field[j][i] = 1;
                }
                else
                {
                    field[j][i] = 0;
                }
                
            }
        }
        
        vTaskDelay(10);
    }
}

int main(void) 
{ 
    DDRA = 0x00; PORTA=0xFF;
    DDRC = 0xFF;
    
    transmit_data(0x00, 0);
    transmit_data(0xFF, 1);
   
    task1_state = INIT;
    task2_state = INIT;
  
    xTaskCreate(Task1, (signed portCHAR *)"Task1", configMINIMAL_STACK_SIZE, NULL, 1, NULL );
    //xTaskCreate(Task2, (signed portCHAR *)"Task2", configMINIMAL_STACK_SIZE, NULL, 1, NULL );
 
    vTaskStartScheduler();
    
    return 0; 
}