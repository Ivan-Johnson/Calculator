#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "element.h"

char* element_to_string(Element* e) {//TODO CLEAN UP THIS MESS
  int defaultLength = 15;
  int totalLength;
  char *text = malloc(sizeof(char) * defaultLength);

  switch (e->type) {
  case ELEMENT_TYPE_INTEGER:
    totalLength = snprintf(text, defaultLength, "(int) %i", e->valueInteger);
    if (totalLength >= 0 && totalLength < defaultLength)
      return text;
    free(text);
    totalLength++; //to make space for the NULL char
    text = malloc(sizeof(char)*totalLength);
    snprintf(text, totalLength, "(int) %i", e->valueInteger);
    return text;
  case ELEMENT_TYPE_DOUBLE:
    totalLength = snprintf(text, defaultLength, "(double) %f", e->valueDouble);
    if (totalLength >= 0 && totalLength < defaultLength)
      return text;
    free(text);
    totalLength++; //to make space for the NULL char
    text = malloc(sizeof(char)*totalLength);
    snprintf(text, totalLength, "(double) %f", e->valueDouble);
    return text;
  case ELEMENT_TYPE_STRING:
    totalLength = snprintf(text, defaultLength, "\"%s\"", e->valueString);
    if (totalLength >= 0 && totalLength < defaultLength)
      return text;
    free(text);
    totalLength++; //to make space for the NULL char
    text = malloc(sizeof(char)*totalLength);
    snprintf(text, totalLength, "\"%s\"", e->valueString);
    return text;
  case ELEMENT_TYPE_VARIABLE:
    totalLength = snprintf(text, defaultLength, "(var) %s", e->valueVariable);
    if (totalLength >= 0 && totalLength < defaultLength)
      return text;
    free(text);
    totalLength++; //to make space for the NULL char
    text = malloc(sizeof(char)*totalLength);
    snprintf(text, totalLength, "(var) %s", e->valueVariable);
    return text;
    //printf("(variable named) %s\n",e->valueVariable);
  case ELEMENT_TYPE_OPERATOR:
    totalLength = snprintf(text, defaultLength, "(operator) %c", e->valueOperator);
    if (totalLength >= 0 && totalLength < defaultLength)
      return text;
    free(text);
    totalLength++; //to make space for the NULL char
    text = malloc(sizeof(char)*totalLength);
    snprintf(text, totalLength, "(operator) %c", e->valueOperator);
    return text;
  default:
    printf("ERROR: trying to print an element that has no type. file: %s line: %i\n", __FILE__, __LINE__);
    printf("this elements data: valDoub %f, valStr %s, valVar %s, valOp \'%c\'\n", e->valueDouble, e->valueString, e->valueVariable, e->valueOperator);
    exit(EXIT_FAILURE);
  };
}

Element *newElement() {//private function
  Element *e = malloc(sizeof(Element));
  if (e == NULL) {
    printf("out of memory");
    exit(EXIT_FAILURE);
  }
  return e;
}

const char ELEMENT_OPERATOR_VARIABLE_DECLARATION[] = "var"; //"var" is stored as a v
const char ELEMENT_VALID_OPERATORS[] = "()=+-*/%^v;";

Element *new_Element_integer(int i) {
  Element *e = newElement();
  e->type = ELEMENT_TYPE_INTEGER;
  e->valueInteger = i;
  return e;
}

Element *new_Element_double(double d) {
  Element *e = newElement();
  e->type = ELEMENT_TYPE_DOUBLE;
  e->valueDouble = d;
  return e;
}

Element *new_Element_string(char* string) {
  Element *e = newElement();
  e->type = ELEMENT_TYPE_STRING;
  e->valueString = string;
  return e;
}

Element *new_Element_variable(char* variableName) {
  Element *e = newElement();
  e->type = ELEMENT_TYPE_VARIABLE;
  e->valueVariable = variableName;
  return e;
}

Element *new_Element_operator(char* str) {
  Element *e = newElement();
  e->type = ELEMENT_TYPE_OPERATOR;
  assert(strchr(ELEMENT_VALID_OPERATORS, *str));
  e->valueOperator = *str;
  if (e->valueOperator == *ELEMENT_OPERATOR_VARIABLE_DECLARATION) // if first letter is 'v'
    assert(strcmp(str, ELEMENT_OPERATOR_VARIABLE_DECLARATION) == 0); //check to make sure str is 'var'
  else
    assert(strlen(str) == 1);
  return e;
}

int element_compare_operators(Element *e1, Element *e2) {
  assert(e1->type == ELEMENT_TYPE_OPERATOR && e2->type == ELEMENT_TYPE_OPERATOR);
  //given 2 operators e1 and e2, returns negative when e1 is a lower priority than e1, 0 when equal, and positive when e1 is a higher priority
  static char *priorities = ")+-*/%^("; //reversed order of operations so that low priority operators have low indexes
  return strchr(priorities, e1->valueOperator) - strchr(priorities, e2->valueOperator);
}
