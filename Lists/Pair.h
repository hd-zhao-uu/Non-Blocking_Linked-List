#ifndef PAIR_H  // include guard
#define PAIR_H
enum methodname {add, rmv, ctn};

typedef struct _Pair{
  methodname method;
  int input;
} Pair;

#endif