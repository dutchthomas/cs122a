/*
 * Debug
 * 
 *
 * I acknowledge all content contained herein, excluding 
 * template or example code, is my own original work.
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include "includes/io.c"

#include "includes/tomsched.h" 
#include "includes/timer.h" 

#include "includes/uout.h"
#include "includes/spi.h"

// Global
/* Task definitions */

char data, command, len, param, output;

// SPI Transmission/reception complete ISR
ISR(SPI_STC_vect)
{
    if(!len)
    {
        uoutSend("Command: ");
        command = SPDR;
        uoutSendInt(command);
        uoutSend("\n\r");
        
        switch(command)
        {
            case 0x01:
                len = 1;
            break;
            
            case 0x02:
                len = 5;
            break;
            
            default:
            break;
        }
    }
    
    if(len)
    {
        switch(command)
        {
            case 0x01:
                data = spiSend(0x07);
                uoutSend("1: ");
                uoutSendInt(data);
                uoutSend("\n\r");
            break;
            
            case 0x02:
                data = spiSend(0x00);
                uoutSend("2: ");
                uoutSendInt(data);
                uoutSend("\n\r");
                
                uoutSendInt(PINB7);
                uoutSend("\n\r");
            break;
        }
        
        len--;   
    }
    
    
}

int SMTick1(task* t)
{
    // Actions
    switch(t->state)
    {        
        case 0:
            //PORTA = 0xF0;
        break;
        
        case 1:
            //PORTA = 0x0F;
        break;
        
        default:
        break;
    }
    
    // Transitions
    switch(t->state)
    {   
        case 0:
            t->state = 1;
        break;
        
        case 1:
            t->state = 0;
        break;
        
        default:
        break;
    }

}

int SMTick2(task* t)
{
    //PORTA = 0xFF;
    uoutTick();
}

int SMTick3(task* t)
{
    //PORTA = 0xFF;
    //uoutTick();
}

/* End task definitions */

int main(void)
{
    // Serial debug out on line 0
    uoutInit(0);
    
    uoutSend("Slave start\n\r");


    // PORT config
    DDRA = 0xFF; PORTA = 0x00;
    
    // Init SPI
    spiInitSlave();
    
    static task task1, task2, task3;
    task *tasks[] = { &task1, &task2 };
    
    unsigned short numTasks = sizeof(tasks)/sizeof(task*);
    
    task1.period = 1000;
    task1.TickFn = &SMTick1;

    task2.period = 50;
    task2.TickFn = &SMTick2;
    
    task3.period = 100;
    task3.TickFn = &SMTick3;
    
    unsigned short gcd = tasksInit(tasks, numTasks);
    
    // Timer
    TimerSet(gcd);
    TimerOn();
    
    unsigned short i; // Iterator
    while(1)
    {
        tasksTick(tasks, numTasks);
        
        while(!TimerFlag); // Wait for a GCD period  
        TimerFlag = 0;
     
    }
    
    return 0;
}

