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
 
//CS122A Resource Headers
#include "bit.h"
#include "keypad.h"

//FreeRTOS include files 
#include "FreeRTOS.h" 
#include "task.h" 
#include "croutine.h" 

//========= shared variables ==========
unsigned char keypadInput = 0;	//holds ascii value from keypad
unsigned char currentState = 0;	//DEBUG: holds current state

int num_steps = 0;
//========= shared variables ==========

enum exactPosition_states
{
btn1Wait,
btn1Release,
btn2Wait,
btn2Release,
btn3Wait,
btn3Release,
btn4Wait,
btn4Release,
setRotation,
noPoundRelease
} exactPosition_state;

void exactPositionTask()
{
	static unsigned char rotationValue[4] = {'\0','\0','\0','\0'};	//array holds user input that determines angle

    while(1)
    {
        
        // Actions
        switch(exactPosition_state)
        {
            case btn1Wait:
				currentState = 1;	//DEBUG

				//start here
				
            break;

			case btn1Release:
				currentState = 2;	//DEBUG

				//start here
				
            break;

			case btn2Wait:
				currentState = 3;	//DEBUG

				//start here

            break;

			case btn2Release:
				currentState = 4;	//DEBUG

				//start here

            break;

			case btn3Wait:
				currentState = 5;	//DEBUG

				//start here

            break;

			case btn3Release:
				currentState = 6;	//DEBUG

				//start here

            break;

			case btn4Wait:
				currentState = 7;	//DEBUG

				//start here

            break;

			case btn4Release:
				currentState = 8;	//DEBUG

				//start here

            break;

			case setRotation:
				currentState = 9;	//DEBUG

				//start here
				num_steps = atoi(rotationValue);	//convert entered values into a shared integer variable
            break;

			case noPoundRelease:
				currentState = 10;	//DEBUG

				//start here

            break;

            default:
            break;
        }


        
        // Transitions
        switch(exactPosition_state)
        {
            case btn1Wait:
				if( keypadInput != '#' && keypadInput != '\0' && (keypadInput >= 0x30) && (keypadInput <= 0x39) ){
					rotationValue[0] = keypadInput;
					exactPosition_state = btn1Release;
				} else {
					exactPosition_state = btn1Wait;
				}
            break;

			case btn1Release:
				if(keypadInput != '\0'){
					exactPosition_state = btn1Release;	
				} else {
					exactPosition_state = btn2Wait;
				}
            break;

			case btn2Wait:
				if( keypadInput != '#' && keypadInput != '\0' && (keypadInput >= 0x30) && (keypadInput <= 0x39) ){
					rotationValue[1] = keypadInput;
					exactPosition_state = btn2Release;
				} 
				else if(keypadInput == '#'){
					rotationValue[1] = '#';
					exactPosition_state = btn4Release;
				}
				else {
					exactPosition_state = btn2Wait;
				}
            break;

			case btn2Release:
				if(keypadInput != '\0'){
					exactPosition_state = btn2Release;	
				} else {
					exactPosition_state = btn3Wait;
				}
            break;

			case btn3Wait:
				if( keypadInput != '#' && keypadInput != '\0' && (keypadInput >= 0x30) && (keypadInput <= 0x39) ){
					rotationValue[2] = keypadInput;
					exactPosition_state = btn3Release;
				} 
				else if(keypadInput == '#'){
					rotationValue[2] = '#';
					exactPosition_state = btn4Release;
				}
				else {
					exactPosition_state = btn3Wait;
				}
            break;

			case btn3Release:
				if(keypadInput != '\0'){
					exactPosition_state = btn3Release;	
				} else {
					exactPosition_state = btn4Wait;
				}
            break;

			case btn4Wait:
				if( keypadInput != '#' && keypadInput != '\0' && (keypadInput >= 0x30) && (keypadInput <= 0x39) ){
					//the user hasn't entered # to submit the degree after entering 3 digits, throw user back
					//into the initial button 1 state					
										
					//reset value array to all nulls
					rotationValue[0] = '\0';
					rotationValue[1] = '\0';
					rotationValue[2] = '\0';
					rotationValue[3] = '\0';

					exactPosition_state = noPoundRelease;
				} 
				else if(keypadInput == '#'){
					rotationValue[3] = '#';
					exactPosition_state = btn4Release;
				}
				else {
					exactPosition_state = btn4Wait;
				}
            break;

			case btn4Release:
				if(keypadInput != '\0'){
					exactPosition_state = btn4Release;	
				} else {
					exactPosition_state = setRotation;
				}
            break;

			case setRotation:
				//reset value array to all nulls
				rotationValue[0] = '\0';
				rotationValue[1] = '\0';
				rotationValue[2] = '\0';
				rotationValue[3] = '\0';

				exactPosition_state = btn1Wait;
            break;

			case noPoundRelease:
				if(keypadInput != '\0'){
					exactPosition_state = noPoundRelease;	
				} else {
					exactPosition_state = btn1Wait;
				}				
            break;

            default:
            break;
        }
        
    
        vTaskDelay(100);
    }

}


//====================== output machine ==========================
enum outputTask_states {output} outputTask_state;
void outputTask(){
	while(1){
		//actions	
		switch(outputTask_state){
			case output:
				//assign output variables to ports here
				PORTA = num_steps;
				break;
			default:
				break;
		}

		//transitions
		switch(outputTask_state){
			case output:
				outputTask_state = output;
			default:
				break;
		}

		vTaskDelay(10);
	}
}
//====================== output machine ==========================





//====================== input machine==========================
//gathers input that can be processed by other machines

enum inputTask_states {input} inputTask_state;
void inputTask(){
	while(1){
		//actions	
		switch(inputTask_state){
			case input:
				//assign inputs to pins here
				keypadInput = GetKeypadKey();
				break;
			default:
				break;
		}

		//transitions
		switch(inputTask_state){
			case input:
				inputTask_state = input;
			default:
				break;
		}

		vTaskDelay(50);
	}
}
//====================== input machine ==========================



int main(void) 
{ 
	//initialize input and output ports
    DDRA = 0xFF; PORTA=0x00; //DDRA=1111 1111   PORTA=0000 0000
	DDRD = 0x0F; PORTD=0xF0; //DDRD=0000 1111   PORTD=1111 0000

	//set state machines to initial states
    outputTask_state = output;
    inputTask_state = input;
	exactPosition_state = btn1Wait;
      
	//create tasks
    xTaskCreate(inputTask, (signed portCHAR *)"inputTask", configMINIMAL_STACK_SIZE, NULL, 1, NULL );
	xTaskCreate(exactPositionTask, (signed portCHAR *)"exactPositionTask", configMINIMAL_STACK_SIZE, NULL, 1, NULL );
    xTaskCreate(outputTask, (signed portCHAR *)"outputTask", configMINIMAL_STACK_SIZE, NULL, 1, NULL );
 
	//run machines
    vTaskStartScheduler();
    
    return 0; 
}
