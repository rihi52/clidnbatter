CC = gcc
CFLAGS = -lpthread -ldl

combatV2: main.c combat.c sqlite3.c lookup.c
	$(CC) main.c combat.c sqlite3.c lookup.c $(CFLAGS) -o combatV2

clean:
	rm -f *.o combatV2