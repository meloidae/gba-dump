CC=gcc
CFLAGS=



spi_util.o: spi_util.c
	$(CC) -c -o $@ $^ -lwiringPi

multiboot.o: multiboot.c spi_util.o
	$(CC) -c -o $@ $^ -lwiringPi

gba_dump: main.c multiboot.o spi_util.o
	$(CC) -o $@ $^ -lwiringPi

clean:
	-rm *.o gba_dump
