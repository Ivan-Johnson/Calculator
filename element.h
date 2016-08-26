#ifndef ELEMENT_H
#define ELEMENT_H

enum ElementType {ELEMENT_TYPE_INTEGER, ELEMENT_TYPE_DOUBLE, ELEMENT_TYPE_STRING, ELEMENT_TYPE_VARIABLE};
typedef enum ElementType ElementType;

struct Element{
  ElementType type;
  int valueInteger;
  double valueDouble;
  char *valueString;//yes, it is unneccessary to have both a valueString and valueVariable, but it make code more readable
  char *valueVariable;
};
typedef struct Element Element;

extern Element *newElementInteger(int);
extern Element *newElementDouble(double);
extern Element *newElementString(char*);
extern Element *newElementVariable(char*);

#endif
