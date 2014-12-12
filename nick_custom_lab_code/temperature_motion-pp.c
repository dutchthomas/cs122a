# 1 "temperature_motion.c"
# 1 "<command-line>"
# 1 "temperature_motion.c"
# 1 "/usr/lib/avr/include/avr/io.h" 1 3
# 99 "/usr/lib/avr/include/avr/io.h" 3
# 1 "/usr/lib/avr/include/avr/sfr_defs.h" 1 3
# 126 "/usr/lib/avr/include/avr/sfr_defs.h" 3
# 1 "/usr/lib/avr/include/inttypes.h" 1 3
# 37 "/usr/lib/avr/include/inttypes.h" 3
# 1 "/usr/lib/gcc/avr/4.8.2/include/stdint.h" 1 3 4
# 9 "/usr/lib/gcc/avr/4.8.2/include/stdint.h" 3 4
# 1 "/usr/lib/avr/include/stdint.h" 1 3 4
# 121 "/usr/lib/avr/include/stdint.h" 3 4
typedef signed int int8_t __attribute__((__mode__(__QI__)));
typedef unsigned int uint8_t __attribute__((__mode__(__QI__)));
typedef signed int int16_t __attribute__ ((__mode__ (__HI__)));
typedef unsigned int uint16_t __attribute__ ((__mode__ (__HI__)));
typedef signed int int32_t __attribute__ ((__mode__ (__SI__)));
typedef unsigned int uint32_t __attribute__ ((__mode__ (__SI__)));

typedef signed int int64_t __attribute__((__mode__(__DI__)));
typedef unsigned int uint64_t __attribute__((__mode__(__DI__)));
# 142 "/usr/lib/avr/include/stdint.h" 3 4
typedef int16_t intptr_t;




typedef uint16_t uintptr_t;
# 159 "/usr/lib/avr/include/stdint.h" 3 4
typedef int8_t int_least8_t;




typedef uint8_t uint_least8_t;




typedef int16_t int_least16_t;




typedef uint16_t uint_least16_t;




typedef int32_t int_least32_t;




typedef uint32_t uint_least32_t;







typedef int64_t int_least64_t;






typedef uint64_t uint_least64_t;
# 213 "/usr/lib/avr/include/stdint.h" 3 4
typedef int8_t int_fast8_t;




typedef uint8_t uint_fast8_t;




typedef int16_t int_fast16_t;




typedef uint16_t uint_fast16_t;




typedef int32_t int_fast32_t;




typedef uint32_t uint_fast32_t;







typedef int64_t int_fast64_t;






typedef uint64_t uint_fast64_t;
# 273 "/usr/lib/avr/include/stdint.h" 3 4
typedef int64_t intmax_t;




typedef uint64_t uintmax_t;
# 10 "/usr/lib/gcc/avr/4.8.2/include/stdint.h" 2 3 4
# 38 "/usr/lib/avr/include/inttypes.h" 2 3
# 77 "/usr/lib/avr/include/inttypes.h" 3
typedef int32_t int_farptr_t;



typedef uint32_t uint_farptr_t;
# 127 "/usr/lib/avr/include/avr/sfr_defs.h" 2 3
# 100 "/usr/lib/avr/include/avr/io.h" 2 3
# 156 "/usr/lib/avr/include/avr/io.h" 3
# 1 "/usr/lib/avr/include/avr/iom1284p.h" 1 3
# 157 "/usr/lib/avr/include/avr/io.h" 2 3
# 432 "/usr/lib/avr/include/avr/io.h" 3
# 1 "/usr/lib/avr/include/avr/portpins.h" 1 3
# 433 "/usr/lib/avr/include/avr/io.h" 2 3

# 1 "/usr/lib/avr/include/avr/common.h" 1 3
# 435 "/usr/lib/avr/include/avr/io.h" 2 3

# 1 "/usr/lib/avr/include/avr/version.h" 1 3
# 437 "/usr/lib/avr/include/avr/io.h" 2 3


# 1 "/usr/lib/avr/include/avr/fuse.h" 1 3
# 239 "/usr/lib/avr/include/avr/fuse.h" 3
typedef struct
{
    unsigned char low;
    unsigned char high;
    unsigned char extended;
} __fuse_t;
# 440 "/usr/lib/avr/include/avr/io.h" 2 3


# 1 "/usr/lib/avr/include/avr/lock.h" 1 3
# 443 "/usr/lib/avr/include/avr/io.h" 2 3
# 2 "temperature_motion.c" 2
# 1 "/usr/lib/avr/include/avr/interrupt.h" 1 3
# 3 "temperature_motion.c" 2
# 1 "includes/lcd.h" 1






# 1 "/usr/lib/avr/include/stdio.h" 1 3
# 45 "/usr/lib/avr/include/stdio.h" 3
# 1 "/usr/lib/gcc/avr/4.8.2/include/stdarg.h" 1 3 4
# 40 "/usr/lib/gcc/avr/4.8.2/include/stdarg.h" 3 4
typedef __builtin_va_list __gnuc_va_list;
# 98 "/usr/lib/gcc/avr/4.8.2/include/stdarg.h" 3 4
typedef __gnuc_va_list va_list;
# 46 "/usr/lib/avr/include/stdio.h" 2 3



# 1 "/usr/lib/gcc/avr/4.8.2/include/stddef.h" 1 3 4
# 212 "/usr/lib/gcc/avr/4.8.2/include/stddef.h" 3 4
typedef unsigned int size_t;
# 50 "/usr/lib/avr/include/stdio.h" 2 3
# 242 "/usr/lib/avr/include/stdio.h" 3
struct __file {
 char *buf;
 unsigned char unget;
 uint8_t flags;
# 261 "/usr/lib/avr/include/stdio.h" 3
 int size;
 int len;
 int (*put)(char, struct __file *);
 int (*get)(struct __file *);
 void *udata;
};
# 405 "/usr/lib/avr/include/stdio.h" 3
extern struct __file *__iob[];
# 417 "/usr/lib/avr/include/stdio.h" 3
extern struct __file *fdevopen(int (*__put)(char, struct __file*), int (*__get)(struct __file*));
# 434 "/usr/lib/avr/include/stdio.h" 3
extern int fclose(struct __file *__stream);
# 608 "/usr/lib/avr/include/stdio.h" 3
extern int vfprintf(struct __file *__stream, const char *__fmt, va_list __ap);





extern int vfprintf_P(struct __file *__stream, const char *__fmt, va_list __ap);






extern int fputc(int __c, struct __file *__stream);




extern int putc(int __c, struct __file *__stream);


extern int putchar(int __c);
# 649 "/usr/lib/avr/include/stdio.h" 3
extern int printf(const char *__fmt, ...);





extern int printf_P(const char *__fmt, ...);







extern int vprintf(const char *__fmt, va_list __ap);





extern int sprintf(char *__s, const char *__fmt, ...);





extern int sprintf_P(char *__s, const char *__fmt, ...);
# 685 "/usr/lib/avr/include/stdio.h" 3
extern int snprintf(char *__s, size_t __n, const char *__fmt, ...);





extern int snprintf_P(char *__s, size_t __n, const char *__fmt, ...);





extern int vsprintf(char *__s, const char *__fmt, va_list ap);





extern int vsprintf_P(char *__s, const char *__fmt, va_list ap);
# 713 "/usr/lib/avr/include/stdio.h" 3
extern int vsnprintf(char *__s, size_t __n, const char *__fmt, va_list ap);





extern int vsnprintf_P(char *__s, size_t __n, const char *__fmt, va_list ap);




extern int fprintf(struct __file *__stream, const char *__fmt, ...);





extern int fprintf_P(struct __file *__stream, const char *__fmt, ...);






extern int fputs(const char *__str, struct __file *__stream);




extern int fputs_P(const char *__str, struct __file *__stream);





extern int puts(const char *__str);




extern int puts_P(const char *__str);
# 762 "/usr/lib/avr/include/stdio.h" 3
extern size_t fwrite(const void *__ptr, size_t __size, size_t __nmemb,
         struct __file *__stream);







extern int fgetc(struct __file *__stream);




extern int getc(struct __file *__stream);


extern int getchar(void);
# 810 "/usr/lib/avr/include/stdio.h" 3
extern int ungetc(int __c, struct __file *__stream);
# 822 "/usr/lib/avr/include/stdio.h" 3
extern char *fgets(char *__str, int __size, struct __file *__stream);






extern char *gets(char *__str);
# 840 "/usr/lib/avr/include/stdio.h" 3
extern size_t fread(void *__ptr, size_t __size, size_t __nmemb,
        struct __file *__stream);




extern void clearerr(struct __file *__stream);
# 857 "/usr/lib/avr/include/stdio.h" 3
extern int feof(struct __file *__stream);
# 868 "/usr/lib/avr/include/stdio.h" 3
extern int ferror(struct __file *__stream);






extern int vfscanf(struct __file *__stream, const char *__fmt, va_list __ap);




extern int vfscanf_P(struct __file *__stream, const char *__fmt, va_list __ap);







extern int fscanf(struct __file *__stream, const char *__fmt, ...);




extern int fscanf_P(struct __file *__stream, const char *__fmt, ...);






extern int scanf(const char *__fmt, ...);




extern int scanf_P(const char *__fmt, ...);







extern int vscanf(const char *__fmt, va_list __ap);







extern int sscanf(const char *__buf, const char *__fmt, ...);




extern int sscanf_P(const char *__buf, const char *__fmt, ...);
# 938 "/usr/lib/avr/include/stdio.h" 3
static __inline__ int fflush(struct __file *stream __attribute__((unused)))
{
 return 0;
}
# 8 "includes/lcd.h" 2
# 22 "includes/lcd.h"
void delay_ms(int miliSec) {
 int i,j;
 for(i=0;i<miliSec;i++) {
  for(j=0;j<775;j++) {
   asm("nop");
  }
 }
}



void LCD_WriteCommand (unsigned char Command) {
 (((*(volatile uint8_t *)((0x05) + 0x20))) &= ~(1 << (0)));
 (*(volatile uint8_t *)((0x08) + 0x20)) = Command;
 (((*(volatile uint8_t *)((0x05) + 0x20))) |= (1 << (1)));
 asm("nop");
 (((*(volatile uint8_t *)((0x05) + 0x20))) &= ~(1 << (1)));
 delay_ms(2);
}

void LCD_ClearScreen(void) {
 LCD_WriteCommand(0x01);
}

void LCD_init(void) {
 delay_ms(100);
 LCD_WriteCommand(0x38);
 LCD_WriteCommand(0x06);
 LCD_WriteCommand(0x0f);
 LCD_WriteCommand(0x01);
 LCD_WriteCommand(0x0C);
 delay_ms(10);
}

void LCD_WriteData(unsigned char Data) {
 (((*(volatile uint8_t *)((0x05) + 0x20))) |= (1 << (0)));
 (*(volatile uint8_t *)((0x08) + 0x20)) = Data;
 (((*(volatile uint8_t *)((0x05) + 0x20))) |= (1 << (1)));
 asm("nop");
 (((*(volatile uint8_t *)((0x05) + 0x20))) &= ~(1 << (1)));
 delay_ms(1);
}

void LCD_Cursor(unsigned char column) {
 if ( column < 17 ) {
  LCD_WriteCommand(0x80 + column - 1);
  } else {
  LCD_WriteCommand(0xB8 + column - 9);
 }
}

void LCD_DisplayString( unsigned char column, const unsigned char* string) {

 unsigned char c = column;
 while(*string) {
  LCD_Cursor(c++);
  LCD_WriteData(*string++);
 }
}
# 4 "temperature_motion.c" 2
# 1 "includes/bit.h" 1
# 14 "includes/bit.h"
unsigned char SetBit(unsigned char pin, unsigned char number, unsigned char bin_value)
{
 return (bin_value ? pin | (0x01 << number) : pin & ~(0x01 << number));
}





unsigned char GetBit(unsigned char port, unsigned char number)
{
 return ( port & (0x01 << number) );
}
# 5 "temperature_motion.c" 2

# 1 "includes/tomsched.h" 1

typedef struct task
{
signed char state;
unsigned long period;
unsigned long ticks;
unsigned long elapsedTime;
int (*TickFn)( struct task*);
} task;

unsigned short tasksGCD(task* tasks[], unsigned short numTasks);
unsigned short tasksInit(task* tasks[], unsigned short numTasks);
void tasksTick(task* tasks[], unsigned short numTasks);

unsigned short tasksInit(task* tasks[], unsigned short numTasks)
{
    int i;

    unsigned short gcd = tasksGCD(tasks, numTasks);

    for ( i = 0; i < numTasks; i++ )
    {

        tasks[i]->state = 0;


        tasks[i]->ticks = tasks[i]->period / gcd;


        tasks[i]->elapsedTime = tasks[i]->ticks;
    }

    return gcd;
}

void tasksTick(task* tasks[], unsigned short numTasks)
{
    int i;


    for ( i = 0; i < numTasks; i++ )
    {

        if( tasks[i]->elapsedTime >= tasks[i]->ticks )
        {

            tasks[i]->TickFn(tasks[i]);


            tasks[i]->elapsedTime = 0;

        }


        tasks[i]->elapsedTime++;

    }

}

unsigned short tasksGCD(task* tasks[], unsigned short numTasks)
{
 unsigned long a, b, c;

    int i;

    if(numTasks == 0)
    {
        return 0;
    }

    if(numTasks == 1)
    {
        return tasks[0]->period;
    }

    b = tasks[0]->period;

    for(i = 1; i < numTasks; i++)
    {
        a = tasks[i]->period;
        c = 1;

     while(c != 0)
     {
      c = a % b;

      if(c)
      {
          a = b;
          b = c;
      }
     }
 }

    return b;

}
# 7 "temperature_motion.c" 2
# 1 "includes/timer.h" 1
# 12 "includes/timer.h"
volatile unsigned char TimerFlag = 0;


unsigned long _avr_timer_M = 1;
unsigned long _avr_timer_cntcurr = 0;


void TimerSet(unsigned long M) {
 _avr_timer_M = M;
 _avr_timer_cntcurr = _avr_timer_M;
}

void TimerOn() {

 (*(volatile uint8_t *)(0x81)) = 0x0B;






 (*(volatile uint16_t *)(0x88)) = 125;





 (*(volatile uint8_t *)(0x6F)) = 0x02;


 (*(volatile uint16_t *)(0x84)) = 0;


 _avr_timer_cntcurr = _avr_timer_M;


 (*(volatile uint8_t *)((0x3F) + 0x20)) |= 0x80;
}

void TimerOff() {
 (*(volatile uint8_t *)(0x81)) = 0x00;
}

void TimerISR() {
 TimerFlag = 1;
}


void __vector_13 (void) __attribute__ ((signal,used, externally_visible)) ; void __vector_13 (void)
{

 _avr_timer_cntcurr--;
 if (_avr_timer_cntcurr == 0) {
  TimerISR();
  _avr_timer_cntcurr = _avr_timer_M;
 }
}
# 8 "temperature_motion.c" 2
# 1 "includes/onewire_ds18b20.h" 1



# 1 "/usr/lib/gcc/avr/4.8.2/include/stdbool.h" 1 3 4
# 5 "includes/onewire_ds18b20.h" 2
# 1 "includes/onewire.h" 1
# 17 "includes/onewire.h"
struct OnewireDevice
{
  uint64_t crc : 8;
  uint64_t serial_no : 48;
  uint64_t family_code : 8;
  volatile uint8_t* bus_port;
  uint8_t bus_pin : 4;
  uint8_t resolution : 4;
};


enum OnewireCommands{SEARCH_ROM = 0xF0, READ_ROM = 0x33, MATCH_ROM = 0x55, SKIP_ROM = 0xCC,
    ALARM_SEARCH = 0xEC};



_Bool onewire_setup_device(struct OnewireDevice *device,
                            volatile uint8_t *bus_register, uint8_t bus_pin);
void onewire_send_byte(const struct OnewireDevice *device, uint8_t byte);
uint8_t onewire_read_byte(const struct OnewireDevice *device);
_Bool onewire_send_reset_pulse(const struct OnewireDevice *device);
_Bool onewire_read_device_info(struct OnewireDevice *device);
# 6 "includes/onewire_ds18b20.h" 2



enum {CONVERT_T = 0x44, WRITE_SCRATCHPAD = 0x4E, READ_SCRATCHPAD = 0xBE,
    COPY_SCRATCHPAD = 0x48, RECALL_E2 = 0xB8, READ_POWER_SUPPLY = 0xB4};
enum ThermResolution {LOW = 1, MED, HIGH};

float ds18b20_get_temperature(const struct OnewireDevice *therm);
_Bool ds18b20_start_conversion(const struct OnewireDevice *therm);
float ds18b20_read_temperature(const struct OnewireDevice *therm);
_Bool ds18b20_setup_device(struct OnewireDevice *therm,
                            volatile uint8_t *bus_register, uint8_t bus_pin);
_Bool ds18b20_set_resolution(struct OnewireDevice *therm, enum ThermResolution decimal_point_resolution);
# 9 "temperature_motion.c" 2
# 1 "includes/onewire.h" 1
# 10 "temperature_motion.c" 2




unsigned char sensor;
char* motionSensorMsg;






enum lcdControl_states {lcd_init, s1,} lcdControl_state;
int lcdDisplayTick(task* t){


 switch(t->state){
  case lcd_init:

   LCD_ClearScreen();
   break;
  case s1:

   LCD_DisplayString(1,motionSensorMsg);
   break;
  default:
   break;
 }


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




enum motionSensor_states {sensorInit, sensorOn} motionSensor_state;
int motionSensorTick(task* t)
{
static unsigned char initCounter = 0;
static char int_buffer[10];
static int temperature;
static unsigned char cursorPos;
static struct OnewireDevice therm;

    switch(t->state)
    {
        case sensorInit:
   motionSensorMsg = "Initializing";



            if(initCounter <= 50){
    initCounter++;
   } else {
    initCounter = 0;
   }
   ds18b20_setup_device(&therm, &(*(volatile uint8_t *)((0x02) + 0x20)), 0);
   ds18b20_set_resolution(&therm, LOW);

        break;

        case sensorOn:
   if( sensor ){

    (*(volatile uint8_t *)((0x02) + 0x20)) = SetBit((*(volatile uint8_t *)((0x02) + 0x20)),2,1);
    motionSensorMsg = "Detected    ";

   } else {

    (*(volatile uint8_t *)((0x02) + 0x20)) = SetBit((*(volatile uint8_t *)((0x02) + 0x20)),2,0);
    motionSensorMsg = "No Detection";

   }



   LCD_DisplayString(17,"Temp:");



   temperature = (int)ds18b20_get_temperature(&therm);

   itoa(temperature, int_buffer,10);
   cursorPos = 22;
   LCD_Cursor(cursorPos);



   unsigned char i;
   for(i=0; int_buffer[i]; i++){
    LCD_WriteData(int_buffer[i]);
    cursorPos++;
   }


   LCD_DisplayString(24,"F");
   LCD_Cursor(25);
   LCD_WriteData((char)223);


        break;
        default:
        break;
    }


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





int main(void)
{


 (*(volatile uint8_t *)((0x07) + 0x20)) = 0xFF; (*(volatile uint8_t *)((0x08) + 0x20)) = 0x00;
 (*(volatile uint8_t *)((0x04) + 0x20)) = 0xFF; (*(volatile uint8_t *)((0x05) + 0x20)) = 0x00;
 (*(volatile uint8_t *)((0x01) + 0x20)) = 0xFD; (*(volatile uint8_t *)((0x02) + 0x20)) = 0x02;

    static task motionSensorPoll, lcdDisplay;
    task *tasks[] = { &lcdDisplay, &motionSensorPoll };

    unsigned short numTasks = sizeof(tasks)/sizeof(task*);


    lcdDisplay.period = 500;
    lcdDisplay.TickFn = &lcdDisplayTick;


 motionSensorPoll.period = 100;
 motionSensorPoll.TickFn = &motionSensorTick;

    unsigned short gcd = tasksInit(tasks, numTasks);


    TimerSet(gcd);
    TimerOn();



 LCD_init();



    unsigned short i;
    while(1)
    {
  sensor = GetBit((*(volatile uint8_t *)((0x00) + 0x20)),1);

        tasksTick(tasks, numTasks);

        while(!TimerFlag);
        TimerFlag = 0;

    }

    return 0;
}
