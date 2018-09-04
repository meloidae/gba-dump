CC=gcc
CFLAGS=



gba_dump: main.c multiboot.c spi_util.c
	$(CC) -o $@ $^ -lwiringPi

clean:
	-rm *.o gba_dump
