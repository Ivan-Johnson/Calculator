OBJS = scanner.o element.o linked_list.o queue.o stack.o calculon.o
OPTS = -Wall -Werror -O2 -g -std=c99

calculon : $(OBJS)
		gcc $(OPTS) $(OBJS) -o calculon -lm

.PHONY : clean test

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

stack.o : stack.h stack.c
		gcc $(OPTS) -c stack.c

test : calculon
		./calculon in.txt

clean :
		rm -f $(OBJS) calculon

