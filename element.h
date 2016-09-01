#ifndef ELEMENT_H
#define ELEMENT_H

enum ElementType {ELEMENT_TYPE_INTEGER, ELEMENT_TYPE_DOUBLE, ELEMENT_TYPE_STRING, ELEMENT_TYPE_VARIABLE, ELEMENT_TYPE_OPERATOR};
typedef enum ElementType ElementType;
struct Element{
  ElementType type;
  int valueInteger;
  double valueDouble;
  char *valueString;//yes, it is unneccessary to have both String and Variable, but it is more consistent this way.
  char *valueVariable;
  char valueOperator;
};
typedef struct Element Element;

extern const char ELEMENT_OPERATOR_VARIABLE_DECLARATION[];
extern const char ELEMENT_VALID_OPERATORS[];

extern char* elementToString(Element* e);

extern Element *newElementInteger(int);
extern Element *newElementDouble(double);
extern Element *newElementString(char*);
extern Element *newElementVariable(char*);
extern Element *newElementOperator(char*);
extern int compareOperators(Element* e1, Element* e2);

#endif
