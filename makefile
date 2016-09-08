OBJS = scanner.o element.o linked_list.o queue.o stack.o calculon.o binaryTree.o binarySearchTree.o
OPTS = -Wall -Werror -O2 -g -std=c99

calculon : $(OBJS)
		gcc $(OPTS) $(OBJS) -o calculon -lm

.PHONY : clean test

calculon.o : calculon.c scanner.h linked_list.h queue.h element.h stack.h binaryTree.o
		gcc $(OPTS) -c calculon.c

binaryTree.o : binaryTree.c binaryTree.h
		gcc $(OPTS) -c binaryTree.c

binarySearchTree.o : binarySearchTree.c binarySearchTree.h
		gcc $(OPTS) -c binarySearchTree.c

element.o : element.c element.h
		gcc $(OPTS) -c element.c

scanner.o : scanner.c scanner.h
		gcc $(OPTS) -c scanner.c

linked_list.o : linked_list.c linked_list.h
		gcc $(OPTS) -c linked_list.c

queue.o : queue.h queue.c linked_list.h
		gcc $(OPTS) -c queue.c

stack.o : stack.h stack.c linked_list.h
		gcc $(OPTS) -c stack.c

test : calculon
		cat in.txt | ./calculon

clean :
		rm -f $(OBJS) calculon

