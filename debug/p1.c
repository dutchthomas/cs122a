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

#include "nrf.h"

// Global

/* Task definitions */

unsigned char uInput;

char channel = 7;

char masterMac[3] = { 0x56, 0x34, 0x12 }; // MAC address of clicker, packed backwards

void nrfPrintReg()
{
        int i;
        char data;
        
        for(i = 0; i < 10; i++)
        {
            spiRead( 0x00 | i, &data, 1);
            
            uoutSend("Reg "); uoutSendInt(i); uoutSend(": "); uoutSendInt(data); uoutSend("\n\r");
            
        }
        
        uoutSend("\n\r");
}

void nrfMaster()
{
    // Enable config line
    PORTB = SetBit(PORTB, DDB0, 0);

    //digitalWrite(CE,LOW);                          // Modify configuration Registers
    
    spiWriteByte(NRF_REG_W | NRF_CONFIG, 0x0A); // Power on
    
    // TODO delay 2 ms ???
    delay_ms(2);
    
    spiWriteByte(NRF_REG_W | NRF_CONFIG, 0x3F); // 2byte CRC, receive mode
    spiWriteByte(NRF_REG_W | NRF_EN_RXADDR, 0x01); // enable RX pipe 1
    spiWriteByte(NRF_REG_W | NRF_RX_PW_P0, 4); // packet size = mac length + data length = 3 + 1 = 4
    spiWriteByte(NRF_REG_W | NRF_EN_AA, 0x00); // disable auto ack, clicker does not support it
    spiWriteByte(NRF_REG_W | NRF_RF_CH, channel); // Set channel
    spiWriteByte(NRF_REG_W | NRF_SETUP_AW, 0x01); // 3-byte MAC
    spiWriteByte(NRF_REG_W | NRF_RF_SETUP, 0x06); // 1Mbps, high power
    
    spiWrite(NRF_FLUSH_RX, 0, 0); // Flush receive buffer
    
    spiWriteByte(NRF_REG_W | NRF_STATUS, 0x70); // Clear interupt
    spiWrite(NRF_REG_W | NRF_RX_ADDR_P0, masterMac, 3); // Send mac address to listen on
    spiWrite(NRF_REG_W | NRF_TX_ADDR, masterMac, 3); // Send mac address to listen on 
    
    // Disable config line
    PORTB = SetBit(PORTB, DDB0, 1);
}

int SMTick1(task* t)
{
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
            PORTA = 0xFF;
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
    uoutTick();
}

int SMTick3(task* t)
{

    if(!GetBit(PINB, 1)) // low = on
    {
        uoutSend("IRQ low\n\r");
        
        unsigned char packet[4]; // mac + data = 3 + 1 = 4

        spiRead(NRF_R_RX_PAYLOAD, packet, 4); // Get packet...
        spiWriteByte(NRF_REG_W | NRF_STATUS, 0x70); // Clear IRQ...
        
        uoutSend("Packet: ");
        
        int i;
        for(i = 0; i < 4; i++)
        {
            uoutSendInt(packet[i]);
            uoutSend(" ");
        }
        
        uoutSend("\n\r");
    }



}

/* End task definitions */

int main(void)
{
    uoutInit(0);
    
    uoutSend("Master start\n\r");

    // PORT config
    DDRA = 0xFF; PORTA = 0x00;
    
    DDRB = 0x01; PORTB = 0x02; // IRQ in
    
    // Spi as master
    spiInitMaster();
    
    static task task1, task2, task3;
    task *tasks[] = { &task1, &task2, &task3 };
    
    unsigned short numTasks = sizeof(tasks)/sizeof(task*);
    
    task1.period = 100;
    task1.TickFn = &SMTick1;

    task2.period = 50;
    task2.TickFn = &SMTick2;
    
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

