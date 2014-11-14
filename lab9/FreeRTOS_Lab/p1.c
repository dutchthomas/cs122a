/*  Thomas Zwaagstra <tzwaa001@ucr.edu>
*   Nick Gingerella <nging001@ucr.edu>
*   Lab section: 22
*   Lab 9 Part 1
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


enum state_labels
{
INIT,
MASTER,
SLAVE
}
task1_state, task2_state;

char signals[8] =  { 0x01, 0x03, 0x02, 0x06, 0x04, 0x0C, 0x08, 0x09 };

char sigs;

int num = 8;
int i = 0;

char dir = 0;

char b0, b1, b2, b3, b4, b5;

void Task1()
{
    while(1)
    {
        sigs = signals[i];
        
        // Input
        b0 = !(PIND & 0x01);
        b1 = !(PIND & 0x02);
        b2 = !(PIND & 0x04);
        b3 = !(PIND & 0x08);
        b4 = !(PIND & 0x10);
        b5 = !(PIND & 0x20);

        PORTA = b0 | (b1 << 1) | (b2 << 2) | (b3 << 3) | (b4 << 4) | (b5 << 5);
        
        if(b0)
        {
            dir = 1;
        }
        else if(b1)
        {
            dir = 2;
        }
        else
        {
            dir = 0;
        }
        

        
        if(dir)
        {
            PORTC = 0xF0 | (sigs & 0x0F);
        }
        
        if(dir == 1) // clockwise
        {
            i = (i + 1) % num;
        }
        else if(dir == 2) // counter clockwise
        {
            i = i ? i-1 : num;
        }
        else // do not move;
        {
            i = 0;
        }
        
        vTaskDelay(2);    

    }

}


int main(void) 
{ 
    DDRA = 0xFF;// PORTA=0xFF;
    DDRC = 0x0F; PORTC = 0xF0;
    DDRD = 0x00; PORTD = 0xFF;
   
    task1_state = INIT;
      
    xTaskCreate(Task1, (signed portCHAR *)"Task1", configMINIMAL_STACK_SIZE, NULL, 1, NULL );
 
    vTaskStartScheduler();
    
    return 0; 
}