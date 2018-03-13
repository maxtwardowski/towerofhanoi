all: towerofhanoi

towerofhanoi: towerofhanoi.o primlib.o
	gcc -g $^ -o $@ -lm -lSDL -lpthread -lSDL_gfx

.c.o:
	gcc -g -Wall -pedantic -Werror -std=c99 -c -D_REENTRANT $<

primlib.o: primlib.c primlib.h

towerofhanoi.o: towerofhanoi.c primlib.h

clean:
	-rm primlib.o towerofhanoi.o towerofhanoi

# -Wall -pedantic -Werror
