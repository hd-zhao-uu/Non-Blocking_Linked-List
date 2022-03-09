#ifndef OPERATION_H  // include guard
#define OPERATION_H

#include "Pair.h"

typedef struct _Operation{
  methodname method;
  int input;
  int output; // if output is bool, then true = 1, false = 0

  _Operation(Pair p) {
      method = p.method;
      input = p.input;
      output = 0;
  }

  _Operation( methodname method, int input, int output): 
    method(method), input(input), output(output) {}

} Operation;

#endif