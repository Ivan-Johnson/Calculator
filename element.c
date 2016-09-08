#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <math.h>
#include "element.h"

char* concatenate_strings (char* arg1, char* arg2){
  char *c = malloc(sizeof(char) * (strlen(arg1) + strlen(arg2) + 1));
  if (c == 0){
    printf("out of memory\n");
    exit(EXIT_FAILURE);
  }
  sprintf(c,"%s%s",arg1,arg2);
  return c;
}


char* element_to_string(Element* e) {//TODO CLEAN UP THIS MESS
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
    totalLength = snprintf(text, defaultLength, "%f(.)", e->valueDouble);
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
  assert(e1->valueOperator != ')' && e2->valueOperator != ')');//because ')' is handled manually in calculon.c, this should never happen.
  //given 2 operators e1 and e2, returns negative when e1 is a lower priority than e1, 0 when equal, and positive when e1 is a higher priority
  static char *priorities = "=+-*/%^("; //reversed order of operations so that low priority operators have low indexes
  return strchr(priorities, e1->valueOperator) - strchr(priorities, e2->valueOperator);
}

bool element_is_parenthesis(Element* e){
  assert(e->type == ELEMENT_TYPE_OPERATOR);
  return e->valueOperator == '(' || e->valueOperator == ')';
}

ElementType element_get_effective_type(Element *e){
  assert(e != NULL);
  if (e->type == ELEMENT_TYPE_VARIABLE){
    e = e->valueVariableValue;
    assert(e != NULL);
  }
  return e->type;
}

Element *element_deref_variable_type(Element *e){
  return e == NULL ? NULL : e->type == ELEMENT_TYPE_VARIABLE ? e->valueVariableValue : e;
}

bool element_is_literal(Element *e){
  return e->type == ELEMENT_TYPE_DOUBLE || e->type == ELEMENT_TYPE_INTEGER || e->type == ELEMENT_TYPE_STRING;
}

Element* element_sum (Element *eLeft, Element *eRight){
  eLeft = element_deref_variable_type(eLeft);
  eRight = element_deref_variable_type(eRight);
  assert(element_is_literal(eLeft) && element_is_literal(eRight));
  if (eLeft->type == ELEMENT_TYPE_DOUBLE || eRight->type == ELEMENT_TYPE_DOUBLE){
    //then the result is a double.
    if (eLeft->type == ELEMENT_TYPE_STRING){
      return new_Element_double(atof(eLeft->valueString) + eRight->valueDouble);
    }
    if (eRight->type == ELEMENT_TYPE_STRING){
      return new_Element_double(eLeft->valueDouble + atof(eRight->valueString));
    }
    if (eLeft->type == ELEMENT_TYPE_INTEGER){
      return new_Element_double(eLeft->valueInteger + eRight->valueDouble);
    }
    if (eRight->type == ELEMENT_TYPE_INTEGER){
      return new_Element_double(eLeft->valueDouble + eRight->valueInteger);
    }
    assert(eLeft->type == ELEMENT_TYPE_DOUBLE && eRight->type == ELEMENT_TYPE_DOUBLE);
    return new_Element_double(eLeft->valueDouble + eRight->valueDouble);
  }
  if (eLeft->type == ELEMENT_TYPE_INTEGER || eRight->type == ELEMENT_TYPE_INTEGER){
    //then the result is an integer
    if (eLeft->type == ELEMENT_TYPE_STRING){
      return new_Element_integer(atoi(eLeft->valueString) + eRight->valueInteger);
    }
    if (eRight->type == ELEMENT_TYPE_STRING){
      return new_Element_integer(eLeft->valueInteger + atoi(eRight->valueString));
    }
    assert(eLeft->type == ELEMENT_TYPE_INTEGER && eRight->type == ELEMENT_TYPE_INTEGER);    
    return new_Element_integer(eLeft->valueInteger+eRight->valueInteger);
  }
  assert(eLeft->type == ELEMENT_TYPE_STRING && eRight->type == ELEMENT_TYPE_STRING);
  return new_Element_string(concatenate_strings(eLeft->valueString, eRight->valueString));
}
Element* element_difference (Element *eLeft, Element *eRight){
  eLeft = element_deref_variable_type(eLeft);
  eRight = element_deref_variable_type(eRight);
  assert(element_is_literal(eLeft) && element_is_literal(eRight));
  if (eLeft->type == ELEMENT_TYPE_DOUBLE || eRight->type == ELEMENT_TYPE_DOUBLE){
    //then the result is a double.
    if (eLeft->type == ELEMENT_TYPE_STRING){
      return new_Element_double(atof(eLeft->valueString) - eRight->valueDouble);
    }
    if (eRight->type == ELEMENT_TYPE_STRING){
      return new_Element_double(eLeft->valueDouble - atof(eRight->valueString));
    }
    if (eLeft->type == ELEMENT_TYPE_INTEGER){
      return new_Element_double(eLeft->valueInteger - eRight->valueDouble);
    }
    if (eRight->type == ELEMENT_TYPE_INTEGER){
      return new_Element_double(eLeft->valueDouble - eRight->valueInteger);
    }
    assert(eLeft->type == ELEMENT_TYPE_DOUBLE && eRight->type == ELEMENT_TYPE_DOUBLE);
    return new_Element_double(eLeft->valueDouble - eRight->valueDouble);
  }
  if (eLeft->type == ELEMENT_TYPE_INTEGER || eRight->type == ELEMENT_TYPE_INTEGER){
    //then the result is an integer
    if (eLeft->type == ELEMENT_TYPE_STRING){
      return new_Element_integer(atoi(eLeft->valueString) - eRight->valueInteger);
    }
    if (eRight->type == ELEMENT_TYPE_STRING){
      return new_Element_integer(eLeft->valueInteger - atoi(eRight->valueString));
    }
    assert(eLeft->type == ELEMENT_TYPE_INTEGER && eRight->type == ELEMENT_TYPE_INTEGER);    
    return new_Element_integer(eLeft->valueInteger - eRight->valueInteger);
  }
  printf("minus is trying to operate on 2 strings. This should never happen.");
  exit(EXIT_FAILURE);
}
Element* element_product (Element *eLeft, Element *eRight){
  eLeft = element_deref_variable_type(eLeft);
  eRight = element_deref_variable_type(eRight);
  assert(element_is_literal(eLeft) && element_is_literal(eRight));
  if (eLeft->type == ELEMENT_TYPE_DOUBLE || eRight->type == ELEMENT_TYPE_DOUBLE){
    //then the result is a double.
    if (eLeft->type == ELEMENT_TYPE_STRING){
      return new_Element_double(atof(eLeft->valueString) * eRight->valueDouble);
    }
    if (eRight->type == ELEMENT_TYPE_STRING){
      return new_Element_double(eLeft->valueDouble * atof(eRight->valueString));
    }
    if (eLeft->type == ELEMENT_TYPE_INTEGER){
      return new_Element_double(eLeft->valueInteger * eRight->valueDouble);
    }
    if (eRight->type == ELEMENT_TYPE_INTEGER){
      return new_Element_double(eLeft->valueDouble * eRight->valueInteger);
    }
    assert(eLeft->type == ELEMENT_TYPE_DOUBLE && eRight->type == ELEMENT_TYPE_DOUBLE);
    return new_Element_double(eLeft->valueDouble * eRight->valueDouble);
  }
  if (eLeft->type == ELEMENT_TYPE_INTEGER || eRight->type == ELEMENT_TYPE_INTEGER){
    //then the result is an integer
    if (eLeft->type == ELEMENT_TYPE_STRING){
      return new_Element_integer(atoi(eLeft->valueString) * eRight->valueInteger);
    }
    if (eRight->type == ELEMENT_TYPE_STRING){
      return new_Element_integer(eLeft->valueInteger * atoi(eRight->valueString));
    }
    assert(eLeft->type == ELEMENT_TYPE_INTEGER && eRight->type == ELEMENT_TYPE_INTEGER);    
    return new_Element_integer(eLeft->valueInteger * eRight->valueInteger);
  }
  printf("multiplication is trying to operate on 2 strings. This should never happen.");
  exit(EXIT_FAILURE);
}

Element* element_quotient (Element *eLeft, Element *eRight){
  eLeft = element_deref_variable_type(eLeft);
  eRight = element_deref_variable_type(eRight);
  assert(element_is_literal(eLeft) && element_is_literal(eRight));
  if (eLeft->type == ELEMENT_TYPE_DOUBLE || eRight->type == ELEMENT_TYPE_DOUBLE){
    //then the result is a double.
    if (eLeft->type == ELEMENT_TYPE_STRING){
      return new_Element_double(atof(eLeft->valueString) / eRight->valueDouble);
    }
    if (eRight->type == ELEMENT_TYPE_STRING){
      return new_Element_double(eLeft->valueDouble / atof(eRight->valueString));
    }
    if (eLeft->type == ELEMENT_TYPE_INTEGER){
      return new_Element_double(eLeft->valueInteger / eRight->valueDouble);
    }
    if (eRight->type == ELEMENT_TYPE_INTEGER){
      return new_Element_double(eLeft->valueDouble / eRight->valueInteger);
    }
    assert(eLeft->type == ELEMENT_TYPE_DOUBLE && eRight->type == ELEMENT_TYPE_DOUBLE);
    return new_Element_double(eLeft->valueDouble / eRight->valueDouble);
  }
  if (eLeft->type == ELEMENT_TYPE_INTEGER || eRight->type == ELEMENT_TYPE_INTEGER){
    //then the result is an integer
    if (eLeft->type == ELEMENT_TYPE_STRING){
      return new_Element_integer(atoi(eLeft->valueString) / eRight->valueInteger);
    }
    if (eRight->type == ELEMENT_TYPE_STRING){
      return new_Element_integer(eLeft->valueInteger / atoi(eRight->valueString));
    }
    assert(eLeft->type == ELEMENT_TYPE_INTEGER && eRight->type == ELEMENT_TYPE_INTEGER);    
    return new_Element_integer(eLeft->valueInteger / eRight->valueInteger);
  }
  printf("division is trying to operate on 2 strings. This should never happen.");
  exit(EXIT_FAILURE);
}

Element* element_modulo (Element *eLeft, Element *eRight){
  eLeft = element_deref_variable_type(eLeft);
  eRight = element_deref_variable_type(eRight);
  assert(element_is_literal(eLeft) && element_is_literal(eRight));
  assert(eLeft->type != ELEMENT_TYPE_DOUBLE && eLeft->type != ELEMENT_TYPE_DOUBLE);
  if (eLeft->type == ELEMENT_TYPE_INTEGER || eRight->type == ELEMENT_TYPE_INTEGER){
    //then the result is an integer
    if (eLeft->type == ELEMENT_TYPE_STRING){
      return new_Element_integer(atoi(eLeft->valueString) % eRight->valueInteger);
    }
    if (eRight->type == ELEMENT_TYPE_STRING){
      return new_Element_integer(eLeft->valueInteger % atoi(eRight->valueString));
    }
    assert(eLeft->type == ELEMENT_TYPE_INTEGER && eRight->type == ELEMENT_TYPE_INTEGER);    
    return new_Element_integer(eLeft->valueInteger % eRight->valueInteger);
  }
  printf("mod is trying to operate on 2 strings. This should never happen.");
  exit(EXIT_FAILURE);
}

Element* element_exponentiate (Element *eLeft, Element *eRight){
  eLeft = element_deref_variable_type(eLeft);
  eRight = element_deref_variable_type(eRight);
  assert(element_is_literal(eLeft) && element_is_literal(eRight));
  if (eLeft->type == ELEMENT_TYPE_DOUBLE || eRight->type == ELEMENT_TYPE_DOUBLE){
    //then the result is a double.
    if (eLeft->type == ELEMENT_TYPE_STRING){
      return new_Element_double(pow(atof(eLeft->valueString), eRight->valueDouble));
    }
    if (eRight->type == ELEMENT_TYPE_STRING){
      return new_Element_double(pow(eLeft->valueDouble, atof(eRight->valueString)));
    }
    if (eLeft->type == ELEMENT_TYPE_INTEGER){
      return new_Element_double(pow(eLeft->valueInteger, eRight->valueDouble));
    }
    if (eRight->type == ELEMENT_TYPE_INTEGER){
      return new_Element_double(pow(eLeft->valueDouble, eRight->valueInteger));
    }
    assert(eLeft->type == ELEMENT_TYPE_DOUBLE && eRight->type == ELEMENT_TYPE_DOUBLE);
    return new_Element_double(pow(eLeft->valueDouble, eRight->valueDouble));
  }
  if (eLeft->type == ELEMENT_TYPE_INTEGER || eRight->type == ELEMENT_TYPE_INTEGER){
    //then the result is an integer
    if (eLeft->type == ELEMENT_TYPE_STRING){
      return new_Element_integer(pow(atoi(eLeft->valueString), eRight->valueInteger));
    }
    if (eRight->type == ELEMENT_TYPE_STRING){
      return new_Element_integer(pow(eLeft->valueInteger, atoi(eRight->valueString)));
    }
    assert(eLeft->type == ELEMENT_TYPE_INTEGER && eRight->type == ELEMENT_TYPE_INTEGER);    
    return new_Element_integer(pow(eLeft->valueInteger, eRight->valueInteger));
  }
  printf("minus is trying to operate on 2 strings. This should never happen.");
  exit(EXIT_FAILURE);
  return NULL;//TODO
}




