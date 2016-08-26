OBJS = calculon.o element.o scanner.o linked_list.o queue.o
OPTS = -Wall -Werror -O2 -g -std=c99

all : $(OBJS)
		gcc $(OPTS) $(OBJS) -o calculon -lm

calculon.o : calculon.c
		gcc $(OPTS) -c calculon.c

element.o : element.c element.h
		gcc $(OPTS) -c element.c

scanner.o : scanner.c scanner.h
		gcc $(OPTS) -c scanner.c

linked_list.o : linked_list.c linked_list.h
		gcc $(OPTS) -c linked_list.c

queue.o : queue.h queue.c
		gcc $(OPTS) -c queue.c

test : all
		./calculon in.txt

clean :
		rm $(OBJS) calculon

