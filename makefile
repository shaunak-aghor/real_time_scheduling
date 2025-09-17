.PHONY: default clean

default: ./src/utils.o ./src/sched_new.o ./src/main.o ./src/priority_queue.o
	mkdir -p ./bin
	gcc -o ./bin/sched ./src/utils.o ./src/sched_new.o ./src/main.o ./src/priority_queue.o -lm

utils.o: ./src/utils.c
	gcc -c ./src/utils.c

main.o: ./src/main.c
	gcc -c ./src/main.c

sched_new.o: ./src/sched_new.c
	gcc -c ./src/sched_new.c

priority_queue.o: ./src/priority_queue.c
	gcc -c ./src/priority_queue.c

clean: 
	rm -rf ./src/*.o
	


