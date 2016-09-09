#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <math.h>
#include "element.h"

char* concatenate_strings(char* arg1, char* arg2) {
  char *c = malloc(sizeof(char) * (strlen(arg1) + strlen(arg2) + 1));
  if (c == 0) {
    printf("out of memory\n");
    exit(EXIT_FAILURE);
  }
  sprintf(c, "%s%s", arg1, arg2);
  return c;
}

char* element_to_string(Element* e) {//TODO CLEAN UP THIS MESS //TODO have this print to an argument file/console instead
  int defaultLength = 15;
  int totalLength;
  char *text = malloc(sizeof(char) * defaultLength);

  switch (e->type) {
  case ELEMENT_TYPE_INTEGER:
    /*
      totalLength = snprintf(text, defaultLength, "(int) %i", e->valueInteger);
      /*/
    totalLength = snprintf(text, defaultLength, "%i", e->valueInteger);
    //*/
    if (totalLength >= 0 && totalLength < defaultLength)
      return text;
    free(text);
    totalLength++; //to make space for the NULL char
    text = malloc(sizeof(char)*totalLength);
    snprintf(text, totalLength, "(int) %i", e->valueInteger);
    return text;
  case ELEMENT_TYPE_DOUBLE:
    /*
      totalLength = snprintf(text, defaultLength, "(double) %f", e->valueDouble);
      /*/
    printf("ACTUAL VALUE IS %f\n", e->valueDouble);
    totalLength = snprintf(text, defaultLength, "%f", e->valueDouble);
    //*/
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
    /*
      totalLength = snprintf(text, defaultLength, "(var) %s", e->valueVariableName);
      /*/
    totalLength = snprintf(text, defaultLength, "%s", e->valueVariableName);
    //*/
    if (totalLength >= 0 && totalLength < defaultLength)
      return text;
    free(text);
    totalLength++; //to make space for the NULL char
    text = malloc(sizeof(char)*totalLength);
    snprintf(text, totalLength, "(var) %s", e->valueVariableName);
    return text;
  case ELEMENT_TYPE_OPERATOR:
    /*
      totalLength = snprintf(text, defaultLength, "(operator) %c", e->valueOperator);
      /*/
    totalLength = snprintf(text, defaultLength, "%c", e->valueOperator);
    //*/
    if (totalLength >= 0 && totalLength < defaultLength)
      return text;
    free(text);
    totalLength++; //to make space for the NULL char
    text = malloc(sizeof(char)*totalLength);
    snprintf(text, totalLength, "(operator) %c", e->valueOperator);
    return text;
  default:
    printf("ERROR: trying to print an element that has no type. file: %s line: %i\n", __FILE__, __LINE__);
    printf("this elements data: valDoub %f, valStr %s, valVarName %s, valOp \'%c\'\n", e->valueDouble, e->valueString, e->valueVariableName, e->valueOperator);
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
  e->valueVariableName = variableName;
  e->valueVariableValue = NULL;
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
  assert(e1->valueOperator != ')' && e2->valueOperator != ')'); //because ')' is handled manually in calculon.c, this should never happen.
  //given 2 operators e1 and e2, returns negative when e1 is a lower priority than e1, 0 when equal, and positive when e1 is a higher priority
  static char *priorities = "=+-*/%^("; //reversed order of operations so that low priority operators have low indexes
  return strchr(priorities, e1->valueOperator) - strchr(priorities, e2->valueOperator);
}

bool element_is_parenthesis(Element* e) {
  assert(e->type == ELEMENT_TYPE_OPERATOR);
  return e->valueOperator == '(' || e->valueOperator == ')';
}

Element *element_get_effective_value(Element *e) {
  if (e != NULL && e->type == ELEMENT_TYPE_VARIABLE) {
    e = e->valueVariableValue;
  }
  return e;
}

bool element_is_literal(Element *e) {
  assert(e != NULL);
  return e->type == ELEMENT_TYPE_DOUBLE || e->type == ELEMENT_TYPE_INTEGER || e->type == ELEMENT_TYPE_STRING;
}

void operation_failure(char *operation){
  printf("Trying to %s unknown combination of element types", operation);
  exit(EXIT_FAILURE);
}

Element* element_sum(Element *eLeft, Element *eRight) {
  assert(element_is_literal(eLeft) && element_is_literal(eRight));//assert left & right aren't NULL, and are reals, ints, or strings
  
  switch (eLeft->type){
  case ELEMENT_TYPE_DOUBLE:
    switch (eRight->type){
    case ELEMENT_TYPE_DOUBLE:
      return new_Element_double(eLeft->valueDouble + eRight->valueDouble);
    case ELEMENT_TYPE_STRING:
      return new_Element_double(eLeft->valueDouble + atof(eRight->valueString));      
    case ELEMENT_TYPE_INTEGER:
      return new_Element_double(eLeft->valueDouble + eRight->valueInteger);
    default:
      operation_failure("add");
    }
  case ELEMENT_TYPE_INTEGER:
    switch (eRight->type){
    case ELEMENT_TYPE_DOUBLE:
      return new_Element_double(eLeft->valueInteger + eRight->valueDouble);
    case ELEMENT_TYPE_STRING:
      return new_Element_integer(eLeft->valueInteger + atoi(eRight->valueString));      
    case ELEMENT_TYPE_INTEGER:
      return new_Element_integer(eLeft->valueInteger + eRight->valueInteger);
    default:
      operation_failure("add");
    }
  case ELEMENT_TYPE_STRING:
    switch (eRight->type){
    case ELEMENT_TYPE_DOUBLE:
      return new_Element_double(atof(eLeft->valueString) + eRight->valueDouble);      
    case ELEMENT_TYPE_STRING:
      return new_Element_string(concatenate_strings(eLeft->valueString, eRight->valueString));
    case ELEMENT_TYPE_INTEGER:
      return new_Element_integer(atoi(eLeft->valueString) + eRight->valueInteger);
    default:
      operation_failure("add");
    }
  default:
    operation_failure("add");
  }
  printf("The compiler is so dumb, it demands I insert this dead code.\n");
  exit(EXIT_FAILURE);
}

Element* element_difference(Element *eLeft, Element *eRight) {
  assert(element_is_literal(eLeft) && element_is_literal(eRight));

  switch (eLeft->type){
  case ELEMENT_TYPE_DOUBLE:
    switch (eRight->type){
    case ELEMENT_TYPE_DOUBLE:
      return new_Element_double(eLeft->valueDouble - eRight->valueDouble);
    case ELEMENT_TYPE_STRING:
      return new_Element_double(eLeft->valueDouble - atof(eRight->valueString));      
    case ELEMENT_TYPE_INTEGER:
      return new_Element_double(eLeft->valueDouble - eRight->valueInteger);
    default:
      operation_failure("subtract");
    }
  case ELEMENT_TYPE_INTEGER:
    switch (eRight->type){
    case ELEMENT_TYPE_DOUBLE:
      return new_Element_double(eLeft->valueInteger - eRight->valueDouble);
    case ELEMENT_TYPE_STRING:
      return new_Element_integer(eLeft->valueInteger - atoi(eRight->valueString));      
    case ELEMENT_TYPE_INTEGER:
      return new_Element_integer(eLeft->valueInteger - eRight->valueInteger);
    default:
      operation_failure("subtract");
    }
  case ELEMENT_TYPE_STRING:
    switch (eRight->type){
    case ELEMENT_TYPE_DOUBLE:
      return new_Element_double(atof(eLeft->valueString) - eRight->valueDouble);      
    case ELEMENT_TYPE_STRING:
      printf("cannot subtract two strings.\n");
      exit(EXIT_FAILURE);
    case ELEMENT_TYPE_INTEGER:
      return new_Element_double(atoi(eLeft->valueString) - eRight->valueInteger);
    default:
      operation_failure("subtract");
    }
  default:
    operation_failure("subtract");
  }
  printf("The compiler is so dumb, it demands I insert this dead code.\n");
  exit(EXIT_FAILURE);
}

Element* element_product(Element *eLeft, Element *eRight) {
  assert(element_is_literal(eLeft) && element_is_literal(eRight));

  switch (eLeft->type){
  case ELEMENT_TYPE_DOUBLE:
    switch (eRight->type){
    case ELEMENT_TYPE_DOUBLE:
      return new_Element_double(eLeft->valueDouble * eRight->valueDouble);
    case ELEMENT_TYPE_STRING:
      return new_Element_double(eLeft->valueDouble * atof(eRight->valueString));      
    case ELEMENT_TYPE_INTEGER:
      return new_Element_double(eLeft->valueDouble * eRight->valueInteger);
    default:
      operation_failure("multiply");
    }
  case ELEMENT_TYPE_INTEGER:
    switch (eRight->type){
    case ELEMENT_TYPE_DOUBLE:
      return new_Element_double(eLeft->valueInteger * eRight->valueDouble);
    case ELEMENT_TYPE_STRING:
      return new_Element_integer(eLeft->valueInteger * atoi(eRight->valueString));      
    case ELEMENT_TYPE_INTEGER:
      return new_Element_integer(eLeft->valueInteger * eRight->valueInteger);
    default:
      operation_failure("multiply");
    }
  case ELEMENT_TYPE_STRING:
    switch (eRight->type){
    case ELEMENT_TYPE_DOUBLE:
      return new_Element_double(atof(eLeft->valueString) * eRight->valueDouble);      
    case ELEMENT_TYPE_STRING:
      printf("cannot multiply two strings.\n");
      exit(EXIT_FAILURE);
    case ELEMENT_TYPE_INTEGER:
      return new_Element_double(atoi(eLeft->valueString) * eRight->valueInteger);
    default:
      operation_failure("multiply");
    }
  default:
    operation_failure("multiply");
  }
  printf("The compiler is so dumb, it demands I insert this dead code.\n");
  exit(EXIT_FAILURE);
}

Element* element_quotient(Element *eLeft, Element *eRight) {
  assert(element_is_literal(eLeft) && element_is_literal(eRight));

  switch (eLeft->type){
  case ELEMENT_TYPE_DOUBLE:
    switch (eRight->type){
    case ELEMENT_TYPE_DOUBLE:
      return new_Element_double(eLeft->valueDouble / eRight->valueDouble);
    case ELEMENT_TYPE_STRING:
      return new_Element_double(eLeft->valueDouble / atof(eRight->valueString));      
    case ELEMENT_TYPE_INTEGER:
      return new_Element_double(eLeft->valueDouble / eRight->valueInteger);
    default:
      operation_failure("divide");
    }
  case ELEMENT_TYPE_INTEGER:
    switch (eRight->type){
    case ELEMENT_TYPE_DOUBLE:
      return new_Element_double(eLeft->valueInteger / eRight->valueDouble);
    case ELEMENT_TYPE_STRING:
      return new_Element_integer(eLeft->valueInteger / atoi(eRight->valueString));      
    case ELEMENT_TYPE_INTEGER:
      return new_Element_integer(eLeft->valueInteger / eRight->valueInteger);
    default:
      operation_failure("divide");
    }
  case ELEMENT_TYPE_STRING:
    switch (eRight->type){
    case ELEMENT_TYPE_DOUBLE:
      return new_Element_double(atof(eLeft->valueString) / eRight->valueDouble);      
    case ELEMENT_TYPE_STRING:
      printf("cannot subtract two strings.\n");
      exit(EXIT_FAILURE);
    case ELEMENT_TYPE_INTEGER:
      return new_Element_double(atoi(eLeft->valueString) / eRight->valueInteger);
    default:
      operation_failure("divide");
    }
  default:
    operation_failure("divide");
  }
  printf("The compiler is so dumb, it demands I insert this dead code.\n");
  exit(EXIT_FAILURE);
}

Element* element_modulo(Element *eLeft, Element *eRight) {
  assert(element_is_literal(eLeft) && element_is_literal(eRight));

  switch (eLeft->type){
  case ELEMENT_TYPE_DOUBLE:
    printf("cannot take mod of a double");
    exit(EXIT_FAILURE);
  case ELEMENT_TYPE_INTEGER:
    switch (eRight->type){
    case ELEMENT_TYPE_DOUBLE:
      printf("cannot take mod of a double");
      exit(EXIT_FAILURE);
    case ELEMENT_TYPE_STRING:
      return new_Element_integer(eLeft->valueInteger % atoi(eRight->valueString));      
    case ELEMENT_TYPE_INTEGER:
      return new_Element_integer(eLeft->valueInteger % eRight->valueInteger);
    default:
      operation_failure("mod");
    }
  case ELEMENT_TYPE_STRING:
    switch (eRight->type){
    case ELEMENT_TYPE_DOUBLE:
      printf("cannot take mod of a double");
      exit(EXIT_FAILURE);
    case ELEMENT_TYPE_STRING:
      printf("cannot mod one string by another.\n");
      exit(EXIT_FAILURE);
    case ELEMENT_TYPE_INTEGER:
      return new_Element_double(atoi(eLeft->valueString) % eRight->valueInteger);
    default:
      operation_failure("mod");
    }
  default:
    operation_failure("mod");
  }
  printf("The compiler is so dumb, it demands I insert this dead code.\n");
  exit(EXIT_FAILURE);
}

Element* element_exponentiate(Element *eLeft, Element *eRight) {
  assert(element_is_literal(eLeft) && element_is_literal(eRight));

  switch (eLeft->type){
  case ELEMENT_TYPE_DOUBLE:
    switch (eRight->type){
    case ELEMENT_TYPE_DOUBLE:
      return new_Element_double(pow(eLeft->valueDouble, eRight->valueDouble));
    case ELEMENT_TYPE_STRING:
      return new_Element_double(pow(eLeft->valueDouble, atof(eRight->valueString)));      
    case ELEMENT_TYPE_INTEGER:
      return new_Element_double(pow(eLeft->valueDouble, eRight->valueInteger));
    default:
      operation_failure("exponent");
    }
  case ELEMENT_TYPE_INTEGER:
    switch (eRight->type){
    case ELEMENT_TYPE_DOUBLE:
      return new_Element_double(pow(eLeft->valueInteger, eRight->valueDouble));
    case ELEMENT_TYPE_STRING:
      return new_Element_integer(pow(eLeft->valueInteger, atoi(eRight->valueString)));      
    case ELEMENT_TYPE_INTEGER:
      return new_Element_integer(pow(eLeft->valueInteger, eRight->valueInteger));
    default:
      operation_failure("exponent");
    }
  case ELEMENT_TYPE_STRING:
    switch (eRight->type){
    case ELEMENT_TYPE_DOUBLE:
      return new_Element_double(pow(atof(eLeft->valueString), eRight->valueDouble));      
    case ELEMENT_TYPE_STRING:
      printf("cannot raise one string to the power of another strings.\n");
      exit(EXIT_FAILURE);
    case ELEMENT_TYPE_INTEGER:
      return new_Element_double(pow(atoi(eLeft->valueString), eRight->valueInteger));
    default:
      operation_failure("exponent");
    }
  default:
    operation_failure("exponent");
  }
  printf("The compiler is so dumb, it demands I insert this dead code.\n");
  exit(EXIT_FAILURE);
}

int element_compare_variable_names(Element *e1, Element *e2) {
  assert(e1->type == ELEMENT_TYPE_VARIABLE && e2->type == ELEMENT_TYPE_VARIABLE);
  assert(e1->valueVariableName != NULL && e2->valueVariableName != NULL);
  return strcmp(e1->valueVariableName, e2->valueVariableName);
}


