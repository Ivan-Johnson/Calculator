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

typedef struct {
  bool printName;
  bool printPostfix;
  FILE* file;
} parsedArgs;

int wrapper_element_compare_variable_names(void *v1, void *v2){
  return element_compare_variable_names((Element *) v1, (Element *) v2);
}
Element *get_actual_variable(binary_search_tree *variables, Element *e){
  if (e == NULL)
    return NULL;
  if (e->type == ELEMENT_TYPE_VARIABLE){
    Element *actualVariable = BST_get(variables, e, &wrapper_element_compare_variable_names);
    assert(actualVariable != NULL);
    return actualVariable;
  }else
    return e;
  //  return e == NULL ? NULL : (e->type == ELEMENT_TYPE_VARIABLE ? e->valueVariableValue : e);
}

Element *get_variables_element(Element *e){
  return e == NULL ? NULL : e->type == ELEMENT_TYPE_VARIABLE ? e->valueVariableValue : e;
}

void showUsage(char *progName) {
  printf("Usage: %s [option] [filename]\n", progName);
}

void printElementQueue(void *queue) {//TODO DELETE THIS FUNCTION BEFORE TURNING IN
  Element *e;
  printf("PRINTING QUEUE\n");
  for (int i = queue_size(queue) - 1; i >= 0; i--) {
    e = linked_list_get(queue, i);
    printf("\t%s\n",element_to_string(e));
  }
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
  printElementQueue(infix);
 dengo:
  while (queue_size(infix) > 0) {
    next = queue_dequeue(infix);
    if (next->type == ELEMENT_TYPE_OPERATOR){
      if (next->valueOperator == ')'){
	while ((next=/*=*/ stack_pop(operator_stack))->valueOperator != '(')//everything before the first '(' in the stack is popped & enqueed into the postfix queue, then the '(' is popped and discarded.
	  queue_enqueue(postfix_queue,next);
	goto dengo;// *nervously checks surroundings for velociraptors*
      }
      if (stack_size(operator_stack)==0 || element_compare_operators(next,stack_peek(operator_stack)) > 0 || ((Element *) operator_stack)->valueOperator=='('){
	stack_push(operator_stack, next);
      }else{
        do{
	  if (next->valueOperator == ')' && ((Element *) stack_peek(operator_stack))->valueOperator == '('){
	    stack_pop(operator_stack);
	    goto dengo;
	  }
          if (!element_is_parenthesis(stack_peek(operator_stack))){
	    queue_enqueue(postfix_queue, stack_pop(operator_stack));
	  }else{
	    break;
	  }
	}while (stack_size(operator_stack) > 0 && element_compare_operators(next,stack_peek(operator_stack)) <= 0 && ((Element *) stack_peek(operator_stack))->valueOperator != '(');
	stack_push(operator_stack, next);
      }
    }else{
      queue_enqueue(postfix_queue, next);
    }
  }
  while(stack_size(operator_stack)>0){
    if (element_is_parenthesis(stack_peek(operator_stack))){
      printf("either there is a missing close parenthesis, or my program logic is wrong. %s, %i\n",__FILE__, __LINE__);
      printf("broke because we found a %c.\n",((Element *) stack_peek(operator_stack))->valueOperator);
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
  Element *tmp;
  while(queue_size(postfix) > 0){
    this = queue_dequeue(postfix);
    assert(this != NULL);
    switch (this->type){
    case ELEMENT_TYPE_INTEGER:// all values 
    case ELEMENT_TYPE_DOUBLE: // pushed to
    case ELEMENT_TYPE_STRING: // the stack
      stack_push(stack, this);
      break;
    case ELEMENT_TYPE_VARIABLE:
      tmp = get_actual_variable(variables, this);//TODO THIS IS A TERRIBLE VARIABLE NAME
      assert(tmp != NULL);
      stack_push(stack, tmp);
      break;
    case ELEMENT_TYPE_OPERATOR:
      assert(stack_size(stack) >= 2);

      tmpRight = stack_pop(stack);
      tmpLeft = stack_pop(stack);
      assert(tmpRight != NULL && tmpLeft != NULL);
      
      
      switch (this->valueOperator){
      case '=':
	/*
	printf("assignment has not been implemented yet.\n");
	printf("Trying to assign %s to %s.\n", element_to_string(tmpRight), element_to_string(tmpLeft));
	exit(EXIT_FAILURE);
	//*/
	assert(tmpLeft->type == ELEMENT_TYPE_VARIABLE);
	assert(element_is_literal(tmpRight));//because '=' is lowest priority operator, right should have already been evaluated to a literal
	Element *actualVariable = BST_get(variables, tmpLeft, &wrapper_element_compare_variable_names);
	if (actualVariable == NULL){
	  printf("undeclared variable \"%s\" was found.", tmpLeft->valueVariableName);
	  exit(EXIT_FAILURE);
	}
	actualVariable->valueVariableValue = tmpRight;
	result = actualVariable->valueVariableValue;//TODO by setting the result pointer like this, do I run the risk of the variable's value being changed unintentionally?
	break;
      case '+':
	tmpLeft = get_variables_element(tmpLeft);
	tmpRight = get_variables_element(tmpRight);
	result = element_sum(tmpLeft, tmpRight);
	break;
      case '-':
	tmpLeft = get_variables_element(tmpLeft);
	tmpRight = get_variables_element(tmpRight);
	result = element_difference(tmpLeft, tmpRight);
	break;
      case '*':
	tmpLeft = get_variables_element(tmpLeft);
	tmpRight = get_variables_element(tmpRight);
	result = element_product(tmpLeft, tmpRight);
	break;
      case '/':
	tmpLeft = get_variables_element(tmpLeft);
	tmpRight = get_variables_element(tmpRight);
	result = element_quotient(tmpLeft, tmpRight);
	break;
      case '%':
	tmpLeft = get_variables_element(tmpLeft);
	tmpRight = get_variables_element(tmpRight);
	result = element_modulo(tmpLeft, tmpRight);
	break;
      case '^':
	tmpLeft = get_variables_element(tmpLeft);
	tmpRight = get_variables_element(tmpRight);
	result = element_exponentiate(tmpLeft, tmpRight);
	break;
      default:
	printf("Trying to compute unknown operator \'%c\'",this->valueOperator);
	exit(EXIT_FAILURE);
      }
      assert(result != NULL);
      assert(element_is_literal(result));
      stack_push(stack, result);
      break;
    }
  }
  assert(stack_size(stack)==1);
  Element *final_result = stack_pop(stack);
  printf("answer is %s.\n",element_to_string(final_result));
  return final_result;
}

int main(int argc, char **argv) {
  parsedArgs *args = processArguments(argc, argv);
  if (args->printName) {
    printf("%s", NAME);
    return EXIT_SUCCESS;
  }
  binary_search_tree *treeVar = new_binary_search_tree();
  void* infix = readExpression(args->file);
  while (queue_size(infix) != 0) {
    Element *front = (Element *) queue_peek(infix);
    if (front->type == ELEMENT_TYPE_OPERATOR && front->valueOperator == 'v') {//if declaring variable
      printf("variable being declared\n");
      queue_dequeue(infix); //removes the "var" operator
      assert(queue_size(infix) > 0);
      Element* var = queue_peek(infix);
      assert(var->type == ELEMENT_TYPE_VARIABLE);
      
      //TODO create the variable var
      BST_insert(treeVar, var, &wrapper_element_compare_variable_names);
      
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
    evaluate(convert(infix), treeVar);
    
    //TODO do we need to do anything if there's an expression that's not being assigned to a variable...? (assuming it's not the last one in the file)

    infix = readExpression(args->file);
  }
  return EXIT_SUCCESS;
}


