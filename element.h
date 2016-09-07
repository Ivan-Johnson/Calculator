#ifndef ELEMENT_H
#define ELEMENT_H
#include <stdbool.h>

enum ElementType {
    ELEMENT_TYPE_INTEGER, ELEMENT_TYPE_DOUBLE, ELEMENT_TYPE_STRING, ELEMENT_TYPE_VARIABLE, ELEMENT_TYPE_OPERATOR
};
typedef enum ElementType ElementType;

struct Element {
  ElementType type;
  int valueInteger;
  double valueDouble;
  char *valueString; //yes, it is unneccessary to have both String and Variable, but it is more consistent this way.
  char *valueVariableName;
  Element *valueVariableValue;
  char valueOperator;
};
typedef struct Element Element;

extern const char ELEMENT_OPERATOR_VARIABLE_DECLARATION[];
extern const char ELEMENT_VALID_OPERATORS[];

extern char* element_to_string(Element* e);

extern Element *new_Element_integer(int);
extern Element *new_Element_double(double);
extern Element *new_Element_string(char*);
extern Element *new_Element_variable(char*);
extern Element *new_Element_operator(char*);
extern int element_compare_operators(Element* e1, Element* e2);
extern bool element_is_parenthesis(Element* e);

#endif
