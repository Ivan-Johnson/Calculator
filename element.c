#include <stdlib.h>
#include <stdio.h>
#include "element.h"

Element *newElement(){//private function
  Element *e = malloc(sizeof (Element));
  if (e == NULL){
    printf("out of memory");
    exit (EXIT_FAILURE);
  }
  return e;
}


/*
typedef enum elementType{INTEGER, DOUBLE, STRING, VARIABLE};

typedef struct element {
  elementType type;
  int valueInteger;
  double valueDouble;
  char *valueString;//yes, it is unneccessary to have both a valueString and valueVariable, but it make code more readable
  char *valueVariable;
}
//*/
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


