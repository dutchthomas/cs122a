main: main.c
	avr-gcc -mmcu=atmega1284p  main.c -o main.elf

P1: p1.c
	avr-gcc -mmcu=atmega1284p p1.c -o main.elf
	
P2: tzwaa001_lab9_part2.c
	avr-gcc -mmcu=atmega1284p  tzwaa001_lab9_part2.c -o main.elf

P3: tzwaa001_lab9_part3.c
	avr-gcc -mmcu=atmega1284p  tzwaa001_lab9_part3.c -o main.elf
	
P4: tzwaa001_lab9_part4.c
	avr-gcc -mmcu=atmega1284p  tzwaa001_lab9_part4.c -o main.elf

P5: tzwaa001_lab9_part5.c
	avr-gcc -mmcu=atmega1284p  tzwaa001_lab9_part5.c -o main.elf
	
flash: main.elf
	avrdude -p atmega1284p -F -U flash:w:main.elf:e -c avrispmkII

killjtag:
	avrdude -U lfuse:w:0xc2:m -U hfuse:w:0xd9:m -U efuse:w:0xff:m -c avrispmkII -p atmega1284p -F
	
div8:
	avrdude -U lfuse:w:0x42:m -U hfuse:w:0xd9:m -U efuse:w:0xff:m -c avrispmkII -p atmega1284p -F