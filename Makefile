all: Main.exe Replay_Manager.exe
	.\Main.exe

Btlshp.o: Btlshp.h Btlshp.c
	gcc -c Btlshp.c -o Btlshp.o

ai.o : ai.c ai.h
	gcc -c ai.c -o ai.o

replay.o : replay.c replay.h
	gcc -c replay.c -o replay.o

SDES.o : SDES.c SDES.h
	gcc -c SDES.c -o SDES.o

Main.exe: Main.c Btlshp.o ai.o replay.o SDES.o
	gcc Main.c Btlshp.o ai.o replay.o SDES.o -o Main.exe

Replay_Manager.exe: Replay_Manager.c Btlshp.o replay.o SDES.o
	gcc Replay_Manager.c Btlshp.o replay.o SDES.o -o Replay_Manager.exe

clean:
	del /Q *.o *.exe 2>nul