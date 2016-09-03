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

//TODO ERROR WHEN REACH EOF WITHOUT READING ';'
//TODO MAKE SURE USING MALLOC PROPERLY; ESP. SIZEOF(CHAR)*WHATEVER
//TODO FREE THINGS WHEN DONE WITH THEM. WHEN FREEING ELEMENTS BE SURE TO FREE THEIR STRINGS
//TODO SUBMISSION PASSWORD IS "seventeen?"
//TODO TYPEDEF STACKS/QUEUES/LINKEDLISTS AS VOID*(but without *?)
//TODO RENAME EVERYTHING SO IT ALL MAKES SENSE.
//TODO BUG: ERRORS WITH x + 5 BECAUSE IT ASSUMES STARTING WITH VARIABLE MEANS IT IS AN ASSIGNMENT

/*
  char* strCat (char* arg1, char* arg2){
  char *c = malloc(sizeof(char) * (strlen(arg1) + strlen(arg2) + 1));
  if (c == 0){
  printf("out of memory\n");
  exit(1);
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

void validateMemory(void* pointer) {
  if (pointer == NULL) {
    printf("out of memory\n");
    exit(1);
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
          exit(1);
        }
        j++;
      }
    } else {
      if (args->file != NULL) {
        printf("Error: received two or more filename parameters");
        exit(1);
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

  while (!feof(f)) {
    if (stringPending(f)) {
      v = new_Element_string(readString(f));
    } else {
      char *str = readToken(f);
      if (str == NULL) {
        continue;
      }
      if (strchr(str, '.') != 0)
        v = new_Element_double(atof(str));
      else if (isdigit(*str) || (strlen(str) > 1 && *str == '-'))
        v = new_Element_integer(atoi(str));
      else if (!isalpha(*str) || strcmp(str, ELEMENT_OPERATOR_VARIABLE_DECLARATION) == 0)
        if (*str == ';')
          return queue;
        else
          v = new_Element_operator(str);
      else
        v = new_Element_variable(str);
      str = NULL; //completely unneccessary, but it makes me happy.
    }
    if (v != NULL)
      queue_enqueue(queue, v);
  }
  return queue;
}

void printElementQueue(void *queue) {//TODO DELETE THIS FUNCTION BEFORE TURNING IN
  /*
    int i = 0;
    while (size(queue)>0){//TMP
    Element *e = (Element *)dequeue(queue);
    printf("%i: %s\n",i,elementToString(e));
    i++;
    }
    /*/
  Element *e;

  for (int i = queue_size(queue) - 1; i >= 0; i--) {
    e = linked_list_get(queue, i);
    printf("%i: %s\n", i, element_to_string(e));
    //free(e);
  }//*/
}

void* convert(void* infix) {
  //TODO
  //void* operator_stack = new_stack();
  //void* postfix_queue = new_queue();
  //Element *next;
  //while (size(infix) > 0) {
  //next = dequeue(infix);
  //}
  //return postfix_queue;
  return infix;
}

Element* evaluate(void* queue) {
  //TODO
  printElementQueue(queue);
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
    if (front->type == ELEMENT_TYPE_VARIABLE) {
      Element *var = queue_dequeue(infix);
      Element *eq = queue_dequeue(infix);
      assert(var->type == ELEMENT_TYPE_VARIABLE);
      assert(eq->type == ELEMENT_TYPE_OPERATOR && eq->valueOperator == '=');
      //TODO
      /*Element *eq =*/ evaluate(convert(infix));
      /*
        if (args->printPostfix){
        printf(toString(postfix));
        return 0;
        }
       */
    } else {//it's not a variable assignment or initialization, so we don't need to do anything.
      //TODO EXCEPT MAKE SURE THAT WE'LL BE ABLE TO PRINT OUT THE POSTFIX IF IT'S THE LAST IN THE LIST.
      //TODO REMOVE THIS CODE. ONLY HERE FOR TESTING.
      evaluate(convert(infix));
    }
    //TODO do we need to do anything if there's an expression that's not being assigned to a variable...? (assuming it's not the last one in the file)

    printf("\n"); //separates the expressions to make the output easier to read
    infix = readExpression(args->file);
  }
  return 0;
}


