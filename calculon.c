#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <assert.h>
#include "scanner.h"
#include "linked_list.h"
#include "queue.h"
#include "element.h"
#include "stack.h"

const char* NAME = "Ivan T. Johnson";
const char* FLAG_SHOWNAME = "v";
const char* FLAG_PRINTPOSTFIX = "d";

//TODO FREE THINGS WHEN DONE WITH THEM. WHEN FREEING ELEMENTS BE SURE TO FREE THEIR STRINGS
//TODO SUBMISSION PASSWORD IS "seventeen?"
//TODO BUG: ERRORS WITH x + 5 BECAUSE IT ASSUMES STARTING WITH VARIABLE MEANS IT IS AN ASSIGNMENT

/*
  char* strCat (char* arg1, char* arg2){
  char *c = malloc(sizeof(char) * (strlen(arg1) + strlen(arg2) + 1));
  if (c == 0){
  printf("out of memory\n");
  exit(EXIT_FAILURE);
  }
  sprintf(c,"%s%s",arg1,arg2);
  return c;
  }
//*/

typedef struct {
  bool printName;
  bool printPostfix;
  FILE* file;
} parsedArgs;

void showUsage(char *progName) {
  printf("Usage: %s [option] [filename]\n", progName);
}

void validateMemory(void* pointer) {//TODO DELETE THIS FUNCTION. OR AT LEAST INLINE IT.
  if (pointer == NULL) {
    printf("out of memory\n");
    exit(EXIT_FAILURE);
  }
}

void printElementQueue(void *queue) {//TODO DELETE THIS FUNCTION BEFORE TURNING IN
  Element *e;
  printf("PRINTING QUEUE\n");
  for (int i = queue_size(queue) - 1; i >= 0; i--) {
    e = linked_list_get(queue, i);
    /*
    printf("%i: %s\n", i, element_to_string(e));
    /*/
    printf("%s ",element_to_string(e));
    //*/
  }
}

parsedArgs *processArguments(int argc, char **argv) {
  parsedArgs *args = malloc(sizeof(parsedArgs));
  validateMemory(args);
  args->printName = false;
  args->printPostfix = false;
  args->file = NULL;
  for (int i = 1; i < argc; i++) {
    if (argv[i][0] == '-') {//is a flag
      int j = 1;
      while (argv[i][j] != '\0') {
        if (argv[i][j] == *FLAG_SHOWNAME)
          args->printName = true;
        else if (argv[i][j] == *FLAG_PRINTPOSTFIX)
          args->printPostfix = true;
        else {
          printf("Error: \'%c\' is not a valid option for %s\n", argv[i][j], argv[0]);
          exit(EXIT_FAILURE);
        }
        j++;
      }
    } else {
      if (args->file != NULL) {
        printf("Error: received two or more filename parameters");
        exit(EXIT_FAILURE);
      }
      args->file = fopen(argv[i], "r");
    }
  }
  if (args->file == NULL)
    args->file = stdin;
  return args;
}

void* readExpression(FILE *f) {
  Element *v = NULL;
  void* queue = new_queue();
  bool reached_expression_end = false;
  while (!reached_expression_end) {
    if (stringPending(f)) {
      v = new_Element_string(readString(f));
    } else {
      char *str = readToken(f);
      if (str != NULL) {
        if (strchr(str, '.') != 0)
          v = new_Element_double(atof(str));
        else if (isdigit(*str) || (strlen(str) > 1 && *str == '-'))
          v = new_Element_integer(atoi(str));
        else if (!isalpha(*str) || strcmp(str, ELEMENT_OPERATOR_VARIABLE_DECLARATION) == 0)
          if (*str == ';')
            reached_expression_end = true;
          else
            v = new_Element_operator(str);
        else
          v = new_Element_variable(str);
      }
    }
    if (v != NULL)
      queue_enqueue(queue, v);
    v = NULL;
    if (feof(f))
      reached_expression_end = true;
  }
  if (feof(f)) {
    if (queue_size(queue) == 0)
      return queue;
    printf("missing \';\' for expression:\n");
    printElementQueue(queue);
    exit(EXIT_FAILURE);
  }
  return queue;
}

void* convert(void* infix) {
  //TODO
  void* operator_stack = new_stack();
  void* postfix_queue = new_queue();
  Element *next;
  printf("INFIX: ");
  printElementQueue(infix);
  printf("\n");
  printf("\n");
  while (queue_size(infix) > 0) {
    next = queue_dequeue(infix);
    if (next->type == ELEMENT_TYPE_OPERATOR){
      if (stack_size(operator_stack)==0 || element_compare_operators(next,stack_peek(operator_stack)) > 0){
	stack_push(operator_stack, next);
      }else{
	do{
	  if (!element_is_parenthesis(stack_peek(operator_stack))){
	    queue_enqueue(postfix_queue, stack_pop(operator_stack));
	  }else{
	    stack_pop(operator_stack);
	  }
	}while (stack_size(operator_stack) > 0 && element_compare_operators(next,stack_peek(operator_stack)) <= 0);
	stack_push(operator_stack, next);
      }
    }else{
      queue_enqueue(postfix_queue, next);
    }
  }
  while(stack_size(operator_stack)>0){
    if (!element_is_parenthesis(stack_peek(operator_stack))){
      queue_enqueue(postfix_queue, stack_pop(operator_stack));
    }else{
      stack_pop(operator_stack);//TODO, THIS SHOULD ERROR?
    }
  }
  return postfix_queue;
}

Element* evaluate(void* postfix) {
  //TODO
  printf("POSTFIX: ");
  printElementQueue(postfix);
  printf("\n");
  printf("\n");
  printf("\n");
  return NULL;
}

int main(int argc, char **argv) {
  parsedArgs *args = processArguments(argc, argv);
  if (args->printName) {
    printf("%s", NAME);
    return 0;
  }
  void* infix = readExpression(args->file);
  while (queue_size(infix) != 0) {
    Element *front = (Element *) queue_peek(infix);
    if (front->type == ELEMENT_TYPE_OPERATOR && front->valueOperator == *ELEMENT_OPERATOR_VARIABLE_DECLARATION) {//it could start with operator '('
      queue_dequeue(infix); //removes the "var" operator
      assert(queue_size(infix) > 0);
      Element* var = queue_peek(infix);
      assert(var->type == ELEMENT_TYPE_VARIABLE);
      //TODO create the variable var
      if (queue_size(infix) == 1) {
        //ie. if variable is declared but not initiated
        //TODO SHOULD NEVER HAPPEN?
        infix = readExpression(args->file);
        continue;
      }
      front = (Element *) queue_peek(infix);
    }
    //TODO EXCEPT MAKE SURE THAT WE'LL BE ABLE TO PRINT OUT THE POSTFIX IF IT'S THE LAST IN THE LIST.
    //TODO REMOVE THIS CODE. ONLY HERE FOR TESTING.
    evaluate(convert(infix));
    
    //TODO do we need to do anything if there's an expression that's not being assigned to a variable...? (assuming it's not the last one in the file)

    printf("\n"); //separates the expressions to make the output easier to read
    infix = readExpression(args->file);
  }
  return 0;
}


