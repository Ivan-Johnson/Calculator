#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "element.h"


Element *newElement(){//private function
  Element *e = malloc(sizeof (Element));
  if (e == NULL){
    printf("out of memory");
    exit (EXIT_FAILURE);
  }
  return e;
}

//TODO make const/extern (in .h)
const char ELEMENT_OPERATOR_VARIABLE_DECLARATION[] = "var";//"var" is stored as a v
const char ELEMENT_VALID_OPERATORS[] = "()=+-*/%^v;";

Element *newElementInteger(int i){
  Element *e = newElement();
  e->type = ELEMENT_TYPE_INTEGER;
  e->valueInteger = i;
  return e;
}
Element *newElementDouble(double d){
  Element *e = newElement();
  e->type = ELEMENT_TYPE_DOUBLE;
  e->valueDouble = d;
  return e;
}
Element *newElementString(char* string){
  Element *e = newElement();
  e->type = ELEMENT_TYPE_STRING;
  e->valueString = string;
  return e;
}
Element *newElementVariable(char* variableName){
  Element *e = newElement();
  e->type = ELEMENT_TYPE_VARIABLE;
  e->valueVariable = variableName;
  return e;
}
Element *newElementOperator(char* str){
  Element *e = newElement();
  e->type = ELEMENT_TYPE_OPERATOR;
  assert(strchr(ELEMENT_VALID_OPERATORS, *str));
  e->valueOperator = *str;
  if (e->valueOperator == *ELEMENT_OPERATOR_VARIABLE_DECLARATION)
    assert(strcmp(str, ELEMENT_OPERATOR_VARIABLE_DECLARATION)==0);
  else
    assert(strlen(str) == 1);
  return e;
}

