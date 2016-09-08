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
  struct Element *valueVariableValue;
  char valueOperator;
};
typedef struct Element Element;

extern const char ELEMENT_OPERATOR_VARIABLE_DECLARATION[];
extern const char ELEMENT_VALID_OPERATORS[];

extern Element *new_Element_integer(int);
extern Element *new_Element_double(double);
extern Element *new_Element_string(char*);
extern Element *new_Element_variable(char*);
extern Element *new_Element_operator(char*);

extern char* element_to_string(Element* e);
extern int element_compare_operators(Element* e1, Element* e2);
extern bool element_is_parenthesis(Element* e);
extern ElementType element_get_effective_type(Element *e);
extern Element *element_deref_variable_type(Element *e);
extern bool element_is_literal(Element *e);

extern Element* element_sum (Element *eLeft, Element *eRight);
extern Element* element_difference (Element *eLeft, Element *eRight);
extern Element* element_product (Element *eLeft, Element *eRight);
extern Element* element_quotient (Element *eLeft, Element *eRight);
extern Element* element_modulo (Element *eLeft, Element *eRight);
extern Element* element_exponentiate (Element *eLeft, Element *eRight);

#endif
