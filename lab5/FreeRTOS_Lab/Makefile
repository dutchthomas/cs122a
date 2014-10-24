INCLUDES=-IFreeRTOS/Source -IFreeRTOS/Source/include -IFreeRTOS/Source/portable/GCC/ATMega323

KILLWARN=-w -O0

main: main.c
	avr-gcc -mmcu=atmega1284p ${INCLUDES} ${KILLWARN} croutine.c heap_1.c list.c port.c queue.c tasks.c timers.c main.c -o main.elf
	
P1: lab03_part2_1.c
	avr-gcc -mmcu=atmega1284p ${INCLUDES} ${KILLWARN} croutine.c heap_1.c list.c port.c queue.c tasks.c timers.c p1.c -o main.elf
	
P2: lab03_part2_2.c
	avr-gcc -mmcu=atmega1284p ${INCLUDES} ${KILLWARN} croutine.c heap_1.c list.c port.c queue.c tasks.c timers.c p2.c -o main.elf
	
P3: lab03_part2_3.c
	avr-gcc -mmcu=atmega1284p ${INCLUDES} ${KILLWARN} croutine.c heap_1.c list.c port.c queue.c tasks.c timers.c p3.c -o main.elf
	
P4: lab03_part2_4.c
	avr-gcc -mmcu=atmega1284p ${INCLUDES} ${KILLWARN} croutine.c heap_1.c list.c port.c queue.c tasks.c timers.c p4.c -o main.elf
	
flash: main.elf
	avrdude -p atmega1284p -F -U flash:w:main.elf:e -c avrispmkII

killjtag:
	avrdude -U lfuse:w:0xc2:m -U hfuse:w:0xd9:m -U efuse:w:0xff:m -c avrispmkII -p atmega1284p -F
	
div8:
	avrdude -U lfuse:w:0x42:m -U hfuse:w:0xd9:m -U efuse:w:0xff:m -c avrispmkII -p atmega1284p -F