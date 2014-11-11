INCLUDES=-IFreeRTOS/Source -IFreeRTOS/Source/include -IFreeRTOS/Source/portable/GCC/ATMega323

KILLWARN=-w -O0

main: main.c
	avr-gcc -mmcu=atmega1284p ${INCLUDES} ${KILLWARN} croutine.c heap_1.c list.c port.c queue.c tasks.c timers.c main.c -o main.elf
	
P1M: p1_master.c
	avr-gcc -mmcu=atmega1284p ${INCLUDES} ${KILLWARN} croutine.c heap_1.c list.c port.c queue.c tasks.c timers.c p1_master.c -o main.elf

P1S: p1_slave.c
	avr-gcc -mmcu=atmega1284p ${INCLUDES} ${KILLWARN} croutine.c heap_1.c list.c port.c queue.c tasks.c timers.c p1_slave.c -o main.elf

P2M: p2_master.c
	avr-gcc -mmcu=atmega1284p ${INCLUDES} ${KILLWARN} croutine.c heap_1.c list.c port.c queue.c tasks.c timers.c p2_master.c -o main.elf

P2S: p2_slave.c
	avr-gcc -mmcu=atmega1284p ${INCLUDES} ${KILLWARN} croutine.c heap_1.c list.c port.c queue.c tasks.c timers.c p2_slave.c -o main.elf

	
P2: p2.c
	avr-gcc -mmcu=atmega1284p ${INCLUDES} ${KILLWARN} croutine.c heap_1.c list.c port.c queue.c tasks.c timers.c p2.c -o main.elf
	
P3: p3.c
	avr-gcc -mmcu=atmega1284p ${INCLUDES} ${KILLWARN} croutine.c heap_1.c list.c port.c queue.c tasks.c timers.c p3.c -o main.elf
	
P4: p4.c
	avr-gcc -mmcu=atmega1284p ${INCLUDES} ${KILLWARN} croutine.c heap_1.c list.c port.c queue.c tasks.c timers.c p4.c -o main.elf

P5: p5.c
	avr-gcc -mmcu=atmega1284p ${INCLUDES} ${KILLWARN} croutine.c heap_1.c list.c port.c queue.c tasks.c timers.c p5.c -o main.elf

	
flash: main.elf
	avrdude -p atmega1284p -F -U flash:w:main.elf:e -c avrispmkII

killjtag:
	avrdude -U lfuse:w:0xc2:m -U hfuse:w:0xd9:m -U efuse:w:0xff:m -c avrispmkII -p atmega1284p -F
	
div8:
	avrdude -U lfuse:w:0x42:m -U hfuse:w:0xd9:m -U efuse:w:0xff:m -c avrispmkII -p atmega1284p -F