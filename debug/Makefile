main: main.c
	avr-gcc -mmcu=atmega1284p -O2 main.c -o main.elf

P1: p1.c
	avr-gcc -mmcu=atmega1284p -O2 p1.c -o main.elf
	
P2: p2.c
	avr-gcc -mmcu=atmega1284p -O2 p2.c -o main.elf

	
flash: main.elf
	avrdude -p atmega1284p -F -U flash:w:main.elf:e -c avrispmkII

killjtag:
	avrdude -U lfuse:w:0xc2:m -U hfuse:w:0xd9:m -U efuse:w:0xff:m -c avrispmkII -p atmega1284p -F
	
div8:
	avrdude -U lfuse:w:0x42:m -U hfuse:w:0xd9:m -U efuse:w:0xff:m -c avrispmkII -p atmega1284p -F