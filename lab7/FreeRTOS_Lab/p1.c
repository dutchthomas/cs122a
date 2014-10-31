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
        // Hardcode master
        s0 = 1;//!(PINB & 0x01);
        
        // Actions
        switch(task1_state)
        {
            case INIT:
                initUSART(0);
            break;
            
            case MASTER:
            
                if(USART_IsSendReady(0))
                {
                    USART_Send(led0, 0);
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