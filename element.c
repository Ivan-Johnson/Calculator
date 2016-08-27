#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "element.h"


char* elementToString(Element* e){
  int defaultLength = 15;
  int totalLength;
  char *text = malloc(sizeof(char) * defaultLength);
  
  switch(e->type){
  case ELEMENT_TYPE_INTEGER:
    totalLength = snprintf(text, defaultLength, "(int) %i", e->valueInteger);
    if (totalLength >= 0 && totalLength < defaultLength)
      return text;
    text = malloc(sizeof(char)*totalLength + 1);//+1 is for the NULL char
    snprintf(text, totalLength, "(int) %i", e->valueInteger);
    return text;
  case ELEMENT_TYPE_DOUBLE:
    totalLength = snprintf(text, defaultLength, "(double) %f", e->valueDouble);
    if (totalLength >= 0 && totalLength < defaultLength)
      return text;
    text = malloc(sizeof(char)*totalLength + 1);//+1 is for the NULL char
    snprintf(text, totalLength, "(double) %f", e->valueDouble);
    return text;
  case ELEMENT_TYPE_STRING:
    return e->valueString;
  case ELEMENT_TYPE_VARIABLE:
    return "some variable";
    //printf("(variable named) %s\n",e->valueVariable);
  case ELEMENT_TYPE_OPERATOR:
    totalLength = snprintf(text, defaultLength, "(operator) %c", e->valueOperator);
    if (totalLength >= 0 && totalLength < defaultLength)
      return text;
    text = malloc(sizeof(char)*totalLength + 1);//+1 is for the NULL char
    snprintf(text, totalLength, "(operator) %c", e->valueOperator);
    return text;
  default:
    printf("THIS SHOULD NEVER HAPPEN!!!\n");
    exit(EXIT_FAILURE);
  };
}


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

