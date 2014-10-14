/* Partner 1 Name & E­mail: Thomas Zwaagstra <tzwaa001@ucr.edu>
 * Partner 2 Name & E­mail:
 * Lab Section: Switzerland
 * Assignment: Lab 9 Part 4
 * 
 *
 * I acknowledge all content contained herein, excluding 
 * template or example code, is my own original work.
 */

#include <avr/io.h>
#include <avr/interrupt.h>

#include "includes/scheduler.h" 
#include "includes/timer.h" 
#include "includes/bit.h" 

// Global

unsigned char leds;

/* Task definitions */

enum SM_states
{
on,
off,
} SM_state;


int SMTick1(int state)
{

    switch(state)
    {
        case -1:
            state = off;
        break;
        
        case off:
            state = on;
        break;
        
        case on:
            state = off;
        break;
        
        default:
        break;
    }
    
    switch(state)
    {
        case -1:
        break;
        
        case off:
            leds = SetBit(leds,0,0); 
        break;
        
        case on:
            leds = SetBit(leds,0,1);
        break;
        
        default:
        break;
    }

    return state;
}

int SMTick2(int state)
{

    switch(state)
    {
        case -1:
            state = off;
        break;
        
        case off:
            state = on;
        break;
        
        case on:
            state = off;
        break;
        
        default:
        break;
    }
    
    switch(state)
    {
        case -1:
        break;
        
        case off:
            leds = SetBit(leds,1,0); 
        break;
        
        case on:
            leds = SetBit(leds,1,1);
        break;
        
        default:
        break;
    }

    return state;
}

int SMTick3(int state)
{

    switch(state)
    {
        case -1:
            state = off;
        break;
        
        case off:
            state = on;
        break;
        
        case on:
            state = off;
        break;
        
        default:
        break;
    }
    
    switch(state)
    {
        case -1:
        break;
        
        case off:
            leds = SetBit(leds,2,0); 
        break;
        
        case on:
            leds = SetBit(leds,2,1);
        break;
        
        default:
        break;
    }

    return state;
}

int SMTick4(int state)
{    
    PORTD = leds;

    return state;
}

/* End task definitions */

int main(void)
{
    // PORT config
    DDRD = 0xFF; PORTD = 0x00; // LED Output
    DDRA = 0x00; PORTA = 0xFF; // Input
    
    // Task period
    unsigned long int SMTick1_calc = 500;
    unsigned long int SMTick2_calc = 1000;
    unsigned long int SMTick3_calc = 2500;
    unsigned long int SMTick4_calc = 100;
    
    // GCD
    unsigned long int GCD = findGCD(SMTick1_calc, SMTick2_calc);
    GCD = findGCD(GCD, SMTick3_calc);
    GCD = findGCD(GCD, SMTick4_calc);
    
    // Scheduler periods
    unsigned long int SMTick1_period = SMTick1_calc/GCD;
    unsigned long int SMTick2_period = SMTick2_calc/GCD;
    unsigned long int SMTick3_period = SMTick3_calc/GCD;
    unsigned long int SMTick4_period = SMTick4_calc/GCD;
    
    // Tasks
    static task task1, task2, task3, task4;
    task *tasks[] = { &task1, &task2, &task3, &task4 };
    const unsigned short numTasks = sizeof(tasks)/sizeof(task*);
    
    // Task1
    task1.state = -1;
    task1.period = SMTick1_period;
    task1.elapsedTime = SMTick1_period;
    task1.TickFct = &SMTick1;
    
    // Task2
    task2.state = -1;
    task2.period = SMTick2_period;
    task2.elapsedTime = SMTick2_period;
    task2.TickFct = &SMTick2;
    
    // Task3
    task3.state = -1;
    task3.period = SMTick3_period;
    task3.elapsedTime = SMTick3_period;
    task3.TickFct = &SMTick3;
    
    // Task3
    task4.state = -1;
    task4.period = SMTick4_period;
    task4.elapsedTime = SMTick4_period;
    task4.TickFct = &SMTick4;
    
    // Timer
    TimerSet(GCD);
    TimerOn();
    
    unsigned short i; // Iterator
    while(1)
    {
    
        // Scheduler
        for ( i = 0; i < numTasks; i++ )
        {
            if( tasks[i]->elapsedTime == tasks[i]->period )
            {
                // Run machine, set state
                tasks[i]->state = tasks[i]->TickFct(tasks[i]->state);
                
                // Reset timer
                tasks[i]->elapsedTime = 0;
            }
            
            // Increment timer
            tasks[i]->elapsedTime++;
        }
        
        while(!TimerFlag); // Wait for a GCD period
        
        TimerFlag = 0;
        
        
        
    }
    
    return 0;
}

