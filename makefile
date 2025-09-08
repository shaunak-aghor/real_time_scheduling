default: ./src/utils.o ./src/scheduler.o ./src/main.o
	gcc -o ./bin/sched ./src/utils.o ./src/scheduler.o ./src/main.o -lm
	rm -rf *.o

utils.o: ./src/utils.c
	gcc -c ./src/utils.c

main.o: ./src/main.c
	gcc -c ./src/main.c

scheduler.o: ./src/scheduler.c
	gcc -c ./src/scheduler.c

clean: 
	rm -rf *.o
	


