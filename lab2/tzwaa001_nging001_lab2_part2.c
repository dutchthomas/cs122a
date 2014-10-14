/*  Thomas Zwaagstra <tzwaa001@ucr.edu>
*   Nick Gingerella <nging001@ucr.edu>
*   Lab section: 22
*   Lab 2 Part 2
*
*   I acknowledge all content contained herein, excluding template or example code,
*   is my own original work.
*/

#include <avr/io.h>
#include <avr/interrupt.h>

#include "includes/scheduler.h" 
#include "includes/timer.h" 
#include "includes/bit.h" 

// Global

unsigned char leds;
unsigned char last;
unsigned char b0;
unsigned char list[30];
unsigned char i, count, num;
unsigned int hash;

unsigned char dict[30000];


/* Task definitions */

enum SM_states
{
init,
wait,
dot,
dash,
record,
display,
display_wait,
} SM_state;


int SMTick1(int state)
{
    //PORTD = state;
    
    b0 = !GetBit(PINA, 0);

    switch(state)
    {
        case -1:
            state = init;
        break;
        
        case init:
            if(b0)
            {
                state = dot;
                count = 0;
            }
            else
            {
                state = init;
            }
        break;
        
        case dot:
                
                if(b0 && count > 4)
                {
                    state = dash;
                }
                else if(!b0)
                {
                    state = record;
                }
                else
                {
                    count++;
                }


        break;
        
        case dash:
            if(!b0)
            {
                state = record;
            }
        break;
        
        case record:
            state = wait;
            count = 0;
            
            if(b0)
            {
                state = dot;
            }
        break;
        
        case wait:
            // Timeout
            if(count > 20)
            {
                state = display;
                count = 10;
            }
            
            // Next input
            if(b0)
            {
                state = dot;
            }
        break;
        
        case display:
            state = display_wait;
        break;
        
        case display_wait:
            if(count > 20)
            {
                state = init;
            }
            else
            {
                state = display_wait;
            }
        break;
        
        default:
        break;
    }
    
    switch(state)
    {
        case init:
        
            PORTD = 0x00;
            last = 0;
            i = 0;
            count = 0;
            num = 0;
            hash = 0;
            
            for(i = 0; i<=10; i++)
            {
                list[i] = 0;
            }
        break;
        
        case dot:
            PORTD = 0x01;
            last = 1;
        break;
        
        case dash:
            PORTD = 0x02;
            last = 2;
            count = 0;
        break;
        
        case record:
            PORTD = 0x00;
            list[num] = last;
            num++;
            
            last = 0;
        break;
        
        case wait:
            count++;
        break;
        
        case display:

            for(i = 0; i < num; i++)
            {
                hash = hash*10 + list[i];
            }
            
        break;

        case display_wait:
            if(num < 6 && dict[hash])
            {
                PORTD = dict[hash];
            }
            else
            {
                PORTD = 0xFF;
            }
            count++;
        break;

        default:
        break;
        
    }
    
    return state;
}

int SMTick2(int state)
{

    return state;
}

int SMTick3(int state)
{


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
    dict[12]    = 'A';
    dict[2111]  = 'B';
    dict[2121]  = 'C';
    dict[211]   = 'D';
    dict[1]     = 'E';
    dict[1121]  = 'F';
    dict[221]   = 'G';
    dict[1111]  = 'H';
    dict[11]    = 'I';
    dict[1222]  = 'J';
    dict[212]   = 'K';
    dict[1211]  = 'L';
    dict[22]    = 'M';
    dict[21]    = 'N';
    dict[222]   = 'O';
    dict[1221]  = 'P';
    dict[2212]  = 'Q';
    dict[121]   = 'R';
    dict[111]   = 'S';
    dict[2]     = 'T';
    dict[112]   = 'U';
    dict[1112]  = 'V';
    dict[122]   = 'W';
    dict[2112]  = 'X';
    dict[2122]  = 'Y';
    dict[2211]  = 'Z';
    dict[12222] = '1';
    dict[11222] = '2';
    dict[11122] = '3';
    dict[11112] = '4';
    dict[11111] = '5';
    dict[21111] = '6';
    dict[22111] = '7';
    dict[22211] = '8';
    dict[22221] = '9';
    dict[22222] = '0';
    

    // PORT config
    DDRD = 0xFF; PORTD = 0x00; // LED Output
    DDRA = 0x00; PORTA = 0xFF; // Input
    
    // Task period
    unsigned long int SMTick1_calc = 100;
    
    // GCD
    unsigned long int GCD = SMTick1_calc;
    
    // Scheduler periods
    unsigned long int SMTick1_period = SMTick1_calc/GCD;

    
    // Tasks
    static task task1;
    task *tasks[] = { &task1};
    const unsigned short numTasks = sizeof(tasks)/sizeof(task*);
    
    // Task1
    task1.state = -1;
    task1.period = SMTick1_period;
    task1.elapsedTime = SMTick1_period;
    task1.TickFct = &SMTick1;
    
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

