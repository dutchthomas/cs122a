/*  Thomas Zwaagstra <tzwaa001@ucr.edu>
*   Nick Gingerella <nging001@ucr.edu>
*   Lab section: 22
*   Lab 8 Part 4
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
unsigned char currentState = 0;	// DEBUG: current state

unsigned char keypadInput = 0;	// ascii value from keypad
unsigned short degree = 0;		// degree entered by user
int num_steps = 0;				// number of steps for stepper motor rotation
unsigned char direction = 1;	//default to clockwise
unsigned char startMotor = 0;
//========= shared variables ==========


void motorControlTask()
{
	const unsigned char signals[8] =  { 0x01, 0x03, 0x02, 0x06, 0x04, 0x0C, 0x08, 0x09 };
	static unsigned char sigs;
	static int num = 8;
	static int i = 0;
	static unsigned char run = 0;

    while(1)
    {

            sigs = signals[i];

            if(direction == 1) // clockwise
            {
                i = (i + 1) % num;
            }
            else if(direction == 2) // counter clockwise
            {
                i = i ? i-1 : (num-1);
            }
            else // do not move;
            {
                i = 0;
            }
            
            if(direction && startMotor)
            {
                PORTC = 0xF0 | (sigs & 0x0F);
                
                if(num_steps > 0)
                {
                    num_steps--;
                } else {
					startMotor = 0; //after done with rotation, stop moving
				}
            }
        
        vTaskDelay(3);    

    }

}




//================== motorAngleControl sm =====================
// Description:
// Process 4 inputs from keypad. User enters a number in the range 0-999
// on the keypad, followed by the # symbol, and stores that number in a
// shared variable that can be used by a stepper motor. Ignores any 
// button presses that are not digits 0-9 or #.

enum motorAngleControl_states
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
} motorAngleControl_state;

void motorAngleControlTask()
{
	static unsigned char rotationValue[4] = {'\0','\0','\0','\0'};	//array holds user input that determines angle

    while(1)
    {
        
        // Actions
        switch(motorAngleControl_state)
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
				degree = atoi(rotationValue);		//convert entered values into a shared integer variable
				num_steps = (degree * 64 / 5.625);	//value for stepper motor control
				startMotor = 1;
            break;

			case noPoundRelease:
				currentState = 10;	//DEBUG

				//start here

            break;

            default:
            break;
        }


        
        // Transitions
        switch(motorAngleControl_state)
        {
            case btn1Wait:
				if( (keypadInput != '\0') && (keypadInput >= 0x30) && (keypadInput <= 0x39) ){
					rotationValue[0] = keypadInput;
					motorAngleControl_state = btn1Release;
				} else {
					motorAngleControl_state = btn1Wait;
				}
            break;

			case btn1Release:
				if(keypadInput != '\0'){
					motorAngleControl_state = btn1Release;	
				} else {
					motorAngleControl_state = btn2Wait;
				}
            break;

			case btn2Wait:
				if( keypadInput != '\0' && (keypadInput >= 0x30) && (keypadInput <= 0x39) ){
					rotationValue[1] = keypadInput;
					motorAngleControl_state = btn2Release;
				} 
				else if(keypadInput == 'A'){
					rotationValue[1] = 'A';
					direction = 1;
					motorAngleControl_state = btn4Release;
				}
				else if(keypadInput == 'B'){
					rotationValue[1] = 'B';
					direction = 2;
					motorAngleControl_state = btn4Release;
				}
				else {
					motorAngleControl_state = btn2Wait;
				}
            break;

			case btn2Release:
				if(keypadInput != '\0'){
					motorAngleControl_state = btn2Release;	
				} else {
					motorAngleControl_state = btn3Wait;
				}
            break;

			case btn3Wait:
				if( keypadInput != '\0' && (keypadInput >= 0x30) && (keypadInput <= 0x39) ){
					rotationValue[2] = keypadInput;
					motorAngleControl_state = btn3Release;
				} 
				else if(keypadInput == 'A'){
					rotationValue[2] = 'A';
					direction = 1;
					motorAngleControl_state = btn4Release;
				}
				else if(keypadInput == 'B'){
					rotationValue[2] = 'B';
					direction = 2;
					motorAngleControl_state = btn4Release;
				}
				else {
					motorAngleControl_state = btn3Wait;
				}

				/*
				if( keypadInput != '#' && keypadInput != '\0' && (keypadInput >= 0x30) && (keypadInput <= 0x39) ){
					rotationValue[2] = keypadInput;
					motorAngleControl_state = btn3Release;
				} 
				else if(keypadInput == '#'){
					rotationValue[2] = '#';
					motorAngleControl_state = btn4Release;
				}
				else {
					motorAngleControl_state = btn3Wait;
				}*/
            break;

			case btn3Release:
				if(keypadInput != '\0'){
					motorAngleControl_state = btn3Release;	
				} else {
					motorAngleControl_state = btn4Wait;
				}
            break;

			case btn4Wait:
				if( keypadInput != '\0' && (keypadInput >= 0x30) && (keypadInput <= 0x39) ){
					//the user hasn't entered # to submit the degree after entering 3 digits, throw user back
					//into the initial button 1 state					
										
					//reset value array to all nulls
					rotationValue[0] = '\0';
					rotationValue[1] = '\0';
					rotationValue[2] = '\0';
					rotationValue[3] = '\0';

					motorAngleControl_state = noPoundRelease;
				} 
				else if(keypadInput == 'A'){
					rotationValue[3] = 'A';
					direction = 1;
					motorAngleControl_state = btn4Release;
				}
				else if(keypadInput == 'B'){
					rotationValue[3] = 'B';
					direction = 2;
					motorAngleControl_state = btn4Release;
				}
				else {
					motorAngleControl_state = btn4Wait;
				}



/*
				if( keypadInput != '#' && keypadInput != '\0' && (keypadInput >= 0x30) && (keypadInput <= 0x39) ){
					//the user hasn't entered # to submit the degree after entering 3 digits, throw user back
					//into the initial button 1 state					
										
					//reset value array to all nulls
					rotationValue[0] = '\0';
					rotationValue[1] = '\0';
					rotationValue[2] = '\0';
					rotationValue[3] = '\0';

					motorAngleControl_state = noPoundRelease;
				} 
				else if(keypadInput == '#'){
					rotationValue[3] = '#';
					motorAngleControl_state = btn4Release;
				}
				else {
					motorAngleControl_state = btn4Wait;
				}*/
            break;

			case btn4Release:
				if(keypadInput != '\0'){
					motorAngleControl_state = btn4Release;	
				} else {
					motorAngleControl_state = setRotation;
				}
            break;

			case setRotation:
				//reset value array to all nulls
				rotationValue[0] = '\0';
				rotationValue[1] = '\0';
				rotationValue[2] = '\0';
				rotationValue[3] = '\0';

				motorAngleControl_state = btn1Wait;
            break;

			case noPoundRelease:
				if(keypadInput != '\0'){
					motorAngleControl_state = noPoundRelease;	
				} else {
					motorAngleControl_state = btn1Wait;
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
				PORTA = currentState;
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

		vTaskDelay(20);
	}
}
//====================== input machine ==========================



int main(void) 
{ 
	//initialize input and output ports
    DDRA = 0xFF; PORTA = 0x00; //DDRA=1111 1111   PORTA=0000 0000
    DDRC = 0x0F; PORTC = 0xF0; //DDRC=0000 1111   PORTD=1111 0000
	DDRD = 0x0F; PORTD = 0xF0; //DDRD=0000 1111   PORTD=1111 0000

	//set state machines to initial states
    outputTask_state = output;
    inputTask_state = input;
	motorAngleControl_state = btn1Wait;
      
	//create tasks
    xTaskCreate(inputTask, (signed portCHAR *)"inputTask", configMINIMAL_STACK_SIZE, NULL, 1, NULL );
	xTaskCreate(motorAngleControlTask, (signed portCHAR *)"motorAngleControlTask", configMINIMAL_STACK_SIZE, NULL, 1, NULL );
	xTaskCreate(motorControlTask, (signed portCHAR *)"motorControlTask", configMINIMAL_STACK_SIZE, NULL, 1, NULL );
    xTaskCreate(outputTask, (signed portCHAR *)"outputTask", configMINIMAL_STACK_SIZE, NULL, 1, NULL );
 
	//run machines
    vTaskStartScheduler();
    
    return 0; 
}
