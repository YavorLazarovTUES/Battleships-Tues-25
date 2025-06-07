all: Main.exe
	.\Main.exe

Btlshp.o: Btlshp.h Btlshp.c
	gcc -c Btlshp.c -o Btlshp.o

replay.o : replay.c replay.h
	gcc -c replay.c -o replay.o

Main.exe: Main.c Btlshp.o replay.o
	gcc Main.c Btlshp.o replay.o -o Main.exe

clean:
	del /Q *.o *.exe 2>nul