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
#include "binaryTree.h"
#include "binarySearchTree.h"

const char* NAME = "Ivan T. Johnson";
const char* FLAG_SHOWNAME = "v";
const char* FLAG_PRINTPOSTFIX = "d";

//TODO FREE THINGS WHEN DONE WITH THEM. WHEN FREEING ELEMENTS BE SURE TO FREE THEIR STRINGS
//TODO SUBMISSION PASSWORD IS "seventeen?"
//TODO DO ALL TODOS

typedef struct {
  bool printName;
  bool printPostfix;
  FILE* file;
} parsedArgs;

int wrapper_element_compare_variable_names(void *v1, void *v2) {
  //the binary search tree needs a void* void* function in order to insert things in order, and to find things.
  //this function wraps the element's Element* Element* comparison into the void* void*
  return element_compare_variable_names((Element *) v1, (Element *) v2);
}

Element *get_actual_variable(binary_search_tree *variables, Element *e) {
  //given an element of type variable that has a variable name,
  //returns an element of type variable (found in given BST) that also has a pointer to the element which stores that variable name's value
  if (e == NULL)
    return NULL;
  if (e->type == ELEMENT_TYPE_VARIABLE) {
    Element *actualVariable = BST_get(variables, e, &wrapper_element_compare_variable_names);
    return actualVariable;
  } else
    return e;//should never be executed, but just in case.
}

void printElementQueue(void *queue) {
  Element *e;
  for (int i = queue_size(queue) - 1; i >= 0; i--) {
    e = linked_list_get(queue, i);
    element_print(e, stdout);
    if (i != 0)//if not last time through loop
      printf(" ");
    else
      printf("\n");
  }
}

void showUsage(char *progName) {
  printf("Usage: %s [option] [filename]\n", progName);
}

parsedArgs *processArguments(int argc, char **argv) {
  parsedArgs *args = malloc(sizeof(parsedArgs));
  assert(args != NULL);
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
  void* operator_stack = new_stack();
  void* postfix_queue = new_queue();
  Element *next;
 dengo:
  while (queue_size(infix) > 0) {
    next = queue_dequeue(infix);
    if (next->type == ELEMENT_TYPE_OPERATOR) {
      if (next->valueOperator == ')') {
        while ((next = /*=*/ stack_pop(operator_stack))->valueOperator != '(')//everything before the first '(' in the stack is popped & enqueed into the postfix queue, then the '(' is popped and discarded.
          queue_enqueue(postfix_queue, next);
        goto dengo; // *nervously checks surroundings for velociraptors*
      }
      if (stack_size(operator_stack) == 0 || element_compare_operators(next, stack_peek(operator_stack)) > 0 || ((Element *) operator_stack)->valueOperator == '(') {
        stack_push(operator_stack, next);
      } else {
        do {
          if (next->valueOperator == ')' && ((Element *) stack_peek(operator_stack))->valueOperator == '(') {
            stack_pop(operator_stack);
            goto dengo;
          }
          if (!element_is_parenthesis(stack_peek(operator_stack))) {
            queue_enqueue(postfix_queue, stack_pop(operator_stack));
          } else {
            break;
          }
        } while (stack_size(operator_stack) > 0 && element_compare_operators(next, stack_peek(operator_stack)) <= 0 && ((Element *) stack_peek(operator_stack))->valueOperator != '(');
        stack_push(operator_stack, next);
      }
    } else {
      queue_enqueue(postfix_queue, next);
    }
  }
  while (stack_size(operator_stack) > 0) {
    if (element_is_parenthesis(stack_peek(operator_stack))) {
      printf("either there is a missing close parenthesis, or my program logic is wrong. %s, %i\n", __FILE__, __LINE__);
      printf("broke because we found a %c.\n", ((Element *) stack_peek(operator_stack))->valueOperator);
      printf("\n\n\n\n\n");
      printf("here's the postfix so far: ");
      printElementQueue(postfix_queue);
      printf("\n and here's the remaining operator_stack: ");
      printElementQueue(operator_stack);
      printf("\n");
      exit(EXIT_FAILURE);
    }
    queue_enqueue(postfix_queue, stack_pop(operator_stack));
  }
  return postfix_queue;
}

Element* evaluate(void* postfix, binary_search_tree *variables) {
  void *stack = new_stack();
  Element *tmpRight, *tmpLeft;
  Element *this;
  Element *result;
  while (queue_size(postfix) > 0) {
    this = queue_dequeue(postfix);
    assert(this != NULL);
    switch (this->type) {
    case ELEMENT_TYPE_INTEGER:// all values 
    case ELEMENT_TYPE_DOUBLE: // pushed to
    case ELEMENT_TYPE_STRING: // the stack
      stack_push(stack, this);
      break;
    case ELEMENT_TYPE_VARIABLE:
      result = get_actual_variable(variables, this);
      if (result == NULL){
	printf("variable %s was not declared\n", this->valueVariableName);
	exit(EXIT_FAILURE);
      }
      stack_push(stack, result);
      break;
    case ELEMENT_TYPE_OPERATOR:
      assert(stack_size(stack) >= 2);

      tmpRight = stack_pop(stack);
      tmpLeft = stack_pop(stack);
      
      
      assert(tmpRight != NULL && tmpLeft != NULL);
      
      
      switch (this->valueOperator) {
      case '=':
        assert(tmpLeft->type == ELEMENT_TYPE_VARIABLE);
	if (tmpRight->type == ELEMENT_TYPE_VARIABLE)
	  tmpRight = element_get_effective_value(tmpRight);//gets the value of the variable
        assert(element_is_literal(tmpRight)); //because '=' is lowest priority operator, right should have already been evaluated to a literal/variable
        Element *actualVariable = BST_get(variables, tmpLeft, &wrapper_element_compare_variable_names);
        if (actualVariable == NULL) {
          printf("undeclared variable \"%s\" was found.", tmpLeft->valueVariableName);
          exit(EXIT_FAILURE);
        }
        actualVariable->valueVariableValue = tmpRight;
        result = actualVariable->valueVariableValue;
        break;
      case '+':
        tmpLeft = element_get_effective_value(tmpLeft);
        tmpRight = element_get_effective_value(tmpRight);
        result = element_sum(tmpLeft, tmpRight);
        break;
      case '-':
        tmpLeft = element_get_effective_value(tmpLeft);
        tmpRight = element_get_effective_value(tmpRight);
        result = element_difference(tmpLeft, tmpRight);
        break;
      case '*':
        tmpLeft = element_get_effective_value(tmpLeft);
        tmpRight = element_get_effective_value(tmpRight);
        result = element_product(tmpLeft, tmpRight);
        break;
      case '/':
        tmpLeft = element_get_effective_value(tmpLeft);
        tmpRight = element_get_effective_value(tmpRight);
        result = element_quotient(tmpLeft, tmpRight);
        break;
      case '%':
        tmpLeft = element_get_effective_value(tmpLeft);
        tmpRight = element_get_effective_value(tmpRight);
        result = element_modulo(tmpLeft, tmpRight);
        break;
      case '^':
        tmpLeft = element_get_effective_value(tmpLeft);
        tmpRight = element_get_effective_value(tmpRight);
        result = element_exponentiate(tmpLeft, tmpRight);
        break;
      default:
        printf("Trying to compute unknown operator \'%c\'", this->valueOperator);
        exit(EXIT_FAILURE);
      }
      assert(result != NULL);
      assert(element_is_literal(result));
      stack_push(stack, result);
      break;
    }
  }
  assert(stack_size(stack) == 1);
  return element_get_effective_value(stack_pop(stack));
}

int main(int argc, char **argv) {
  parsedArgs *args = processArguments(argc, argv);
  if (args->printName) {
    printf("%s\n", NAME);
    return EXIT_SUCCESS;
  }
  binary_search_tree *treeVar = new_binary_search_tree();
  void* infix = readExpression(args->file);

  while (queue_size(infix) != 0) {
    Element *front = (Element *) queue_peek(infix);
    if (front->type == ELEMENT_TYPE_OPERATOR && front->valueOperator == 'v') {//if declaring variable
      queue_dequeue(infix); //removes the "var" operator
      assert(queue_size(infix) > 0);
      Element* var = queue_peek(infix);
      assert(var->type == ELEMENT_TYPE_VARIABLE);

      BST_insert(treeVar, var, &wrapper_element_compare_variable_names);

      if (queue_size(infix) == 1) {
	printf("ERROR: variable is being declared but not initiated. This should never happen.");
	exit(EXIT_FAILURE);
      }
      front = (Element *) queue_peek(infix);
    }
    void *postfix = convert(infix);

    infix = readExpression(args->file);
    
    if (feof(args->file)){//if this is the last expression
      if (args->printPostfix) {
	printElementQueue(postfix);
	return 0;
      }else{
	element_print(evaluate(postfix, treeVar), stdout);
	printf("\n");
	return 0;
      }
    }

    
    evaluate(postfix, treeVar);
  }

  return EXIT_SUCCESS;
}


