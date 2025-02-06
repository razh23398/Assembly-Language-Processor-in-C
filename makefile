main.exe: HELP.o LISTS.o MACRO.o MAIN.o STAGE_ONE.o STAGE_TWO.o TO_FILE.o
	gcc HELP.o LISTS.o MACRO.o MAIN.o STAGE_ONE.o STAGE_TWO.o TO_FILE.o -ansi -Wall -pedantic -o main.exe

MAIN.o: MAIN.c main.h
	gcc -c MAIN.c -ansi -Wall -pedantic -o MAIN.o

MACRO.o: MACRO.c main.h
	gcc -c MACRO.c -ansi -Wall -pedantic -o MACRO.o

HELP.o: HELP.c main.h
	gcc -c HELP.c -ansi -Wall -pedantic -o HELP.o

LISTS.o: LISTS.c main.h
	gcc -c LISTS.c -ansi -Wall -pedantic -o LISTS.o

STAGE_ONE.o: STAGE_ONE.c main.h
	gcc -c STAGE_ONE.c -ansi -Wall -pedantic -o STAGE_ONE.o

STAGE_TWO.o: STAGE_TWO.c main.h
	gcc -c STAGE_TWO.c -ansi -Wall -pedantic -o STAGE_TWO.o

TO_FILE.o: TO_FILE.c main.h
	gcc -c TO_FILE.c -ansi -Wall -pedantic -o TO_FILE.o

clean:
	del *o
	del *ext
	del *ext
	del *ent
	del *ob
	del *log