#include "usart_ATmega1284.h"
#include <string.h>

int uoutLen = 0;
unsigned char uoutLine;

unsigned char uoutBuffer[2000];

void uoutInit(unsigned char n);
void uoutTick();
void uoutSend(char *string);

void uoutInit(unsigned char n)
{
    uoutLine = n;
    initUSART(n);
}

void uoutTick()
{
    int count = 0;

    while(uoutLen)
    {
        while(!USART_IsSendReady(uoutLine)); // Yeah... busy wait for output ready
        
        USART_Send(uoutBuffer[count], uoutLine);
        
        count++;
        uoutLen--;
    }
    
    memmove(uoutBuffer, uoutBuffer+count, count+uoutLen); 
}

void uoutSendInt(int input)
{
    char output[100];
    
    itoa(input, output, 10);
    uoutSend(output);
}

void uoutSend(char *string)
{
    int n = strlen(string);
 
    if((n + uoutLen) > 2000)
    {
        return; // Too long yo.
    }
    
    memcpy(uoutBuffer+uoutLen, string, n);
    
    uoutLen += n;
}