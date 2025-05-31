all: Main.exe

Btlshp.o: Btlshp.h Btlshp.c
	gcc -c Btlshp.c -o Btlshp.o

Main.exe: Main.c Btlshp.o
	gcc Main.c Btlshp.o -o Main.exe

clean:
	del /Q *.o *.exe 2>nul