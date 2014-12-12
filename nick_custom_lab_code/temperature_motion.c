#include <avr/io.h>
#include <avr/interrupt.h>

//includes for lcd and Get/Set bit
#include "includes/bit.h"
#include "includes/lcd.h"

//includes for scheduler
#include "includes/tomsched.h" 
#include "includes/timer.h"

//includes for temperature sensor
#include "includes/onewire_ds18b20.h"
#include "includes/onewire.h"


// Globals
unsigned char sensor;	//input from motion sensor
char* motionSensorMsg;	//lcd message


// Task definitions 
//====================== lcd machine ==========================
//Prints information about the system to the LCD. The cstrings constructed in
//the lightControl machine are output to the LCD through here.
enum lcdControl_states {lcd_init, s1,} lcdControl_state;
int lcdDisplayTick(task* t){

	//actions	
	switch(t->state){
		case lcd_init:
			//LCD_init();							//initialize LCD
			LCD_ClearScreen();					//clear screen of any artifacts
			break;
		case s1:
			//LCD_ClearScreen();					//clear screen of any artifacts
			//LCD_DisplayString(1,motionSensorMsg);	//display motion sensor message
			break;
		default:
			break;
	}

	//transitions
	switch(t->state){
		case lcd_init:
			t->state = s1;
			break;
		case s1:
			t->state = s1;
			break;
		default:
			break;
	}
}


//Movement Detector
//updates globals "sensor" variable
enum motionSensor_states {sensorInit, sensorOn} motionSensor_state;
int motionSensorTick(task* t)
{
static unsigned char initCounter = 0;
static char int_buffer[10];
const unsigned char personImg = 0x00;
static int temperature;
static unsigned char cursorPos;
static struct OnewireDevice therm;
	// Actions
    switch(t->state)
    {
        case sensorInit:
			motionSensorMsg = "Initializing";

			//wait 5 seconds before starting detection. This gives the 
			//motion sensor time to initialize
            if(initCounter <= 50){
				initCounter++;
			} else {
				initCounter = 0;
			}
			ds18b20_setup_device(&therm, &PORTA, 0);
			ds18b20_set_resolution(&therm, LOW);

        break;
        
        case sensorOn:
			if( sensor ){

				PORTA = SetBit(PORTA,2,1);
				LCD_Cursor(16);
				LCD_WriteData(personImg);

			} else {

				PORTA = SetBit(PORTA,2,0);
				LCD_DisplayString(16," ");
				

			}


			//**************Build Temperature Message**************
			LCD_DisplayString(17,"Temp:");
			
			//get temp sensor reading
			//temperature = (((TEMP_SENSOR + 2))*1.8) + 32;
			temperature = (int)ds18b20_get_temperature(&therm);
			
			itoa(temperature, int_buffer,10);
			cursorPos = 22;
			LCD_Cursor(cursorPos);

			//print out contents of the temp sensor reading
			//LCD_ClearScreen();			
			unsigned char i;
			for(i=0; int_buffer[i]; i++){
				LCD_WriteData(int_buffer[i]);
				cursorPos++;
			}

			//print out Fahrenhiet degree symbols
			LCD_DisplayString(24,"F");
			LCD_Cursor(25);
			LCD_WriteData((char)223);
			//**************Build Temperature Message**************

        break;
        default:
        break;
    }
    
    // Transitions
    switch(t->state)
    {   
        case sensorInit:
			if(initCounter <= 50){
				t->state = sensorInit;
			} else {
            	t->state = sensorOn;
			}
        break;
        
        case sensorOn:
            t->state = sensorOn;
        break;
        
        default:
        break;
    }

}

/* End task definitions */



int main(void)
{

    // PORT config
	DDRC = 0xFF; PORTC = 0x00;	//1111 1111 : 0000 0000
	DDRB = 0xFF; PORTB = 0x00; 	//1111 1111 : 0000 0000
	DDRA = 0xFD; PORTA = 0x02;	//1111 1101 : 0000 0010
   

	//scheduler setup
    static task motionSensorPoll, lcdDisplay;
    task *tasks[] = { &lcdDisplay, &motionSensorPoll };
    
    unsigned short numTasks = sizeof(tasks)/sizeof(task*);
    
	//LCD display task
    lcdDisplay.period = 500;
    lcdDisplay.TickFn = &lcdDisplayTick;

	//Motion sensor polling task
	motionSensorPoll.period = 100;
	motionSensorPoll.TickFn = &motionSensorTick;
    	
    unsigned short gcd = tasksInit(tasks, numTasks);
    
    // Timer
    TimerSet(gcd);
    TimerOn();



	//custom character for lcd
	const char personPattern[8] = {0x04, 0x0A, 0x04, 0x1f, 0x04, 0x04, 0x0A, 0x11};
	LCD_build(0,personPattern);
	
	//initialize LCD
	LCD_init();    

    unsigned short i; // Iterator
    while(1)
    {
		sensor = GetBit(PINA,1);

        tasksTick(tasks, numTasks);

        while(!TimerFlag); // Wait for a GCD period  
        TimerFlag = 0;
     
    }
    
    return 0;
}

