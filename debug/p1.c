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

#define NRF_CHANNEL 7
#define MAX_TEMP 90
#define MIN_TEMP 60

char digOn = 0;
char temp = 0;
char d0 = -1;
char d1 = -1;
char t = 0;
char remoteTimeout = -1;

#include "nrf.h"
#include "radio.c"

// Global

/* Task definitions */

char channel = 7;

char masterMac[3] = { 0x56, 0x34, 0x12 }; // MAC address of clicker, packed backwards


/* Radio control interface */
int SMTick1(task* t)
{
    static unsigned char uInput;

    // Actions
    switch(t->state)
    {        
        case 0:
            PORTA = 0x00;

            if(USART_HasReceived(0))
            {
                uInput = USART_Receive(0);
                
                switch(uInput)
                {
                    case 'r':
                        nrfPrintReg();
                    break;
                    
                    case 'm':
                        nrfMaster();
                    break;

                    default:
                    break;
                }
            }            
            
        break;
        
        case 1:
            PORTA = 0x01;
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

int SMTick3(task* t)
{
    if(remoteTimeout >= 0)
    {
        remoteTimeout++;
    }
    
    if(remoteTimeout > 3)
    {
        remoteTimeout = -1;
        uoutSend("Remote Timeout \n\r");
        tempCancel();
    }
}

/* End task definitions */

int main(void)
{
    uoutInit(0);
    
    uoutSend("Master start\n\r");

    // PORT config
    DDRA = 0xFF; PORTA = 0x00;
    DDRB = 0x01; PORTB = 0x02;
    DDRD = 0x08;
    
    // Spi as master
    spiInitMaster();
    
    // Radio init
    nrfMaster();
    
    enableINT0();
    
    static task task1, task2, task3;
    task *tasks[] = { &task1, &task2, &task3};
    
    unsigned short numTasks = sizeof(tasks)/sizeof(task*);
    
    task1.period = 100;
    task1.TickFn = &SMTick1;

    task2.period = 50;
    task2.TickFn = &uoutTick;
    
    task3.period = 1000;
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

