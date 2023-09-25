#ifndef STACK_HEADER
#define STACK_HEADER

#define CANARY
#define HASH

#include <stdlib.h>
#include <stdio.h>
#include <limits.h>

#include "colors.h"

typedef int Elemt;
#ifdef CANARY
typedef unsigned long long Canaryt;
#endif
#ifdef HASH
typedef unsigned long long Hasht;
#endif

enum Errors
{
    NULL_POINTER =       1 << 1,
    MEM_ALLOC_ERR =      1 << 2,
    NEGATIVE_SIZE =      1 << 3,
    NEGATIVE_CAPACITY =  1 << 4,
    SIZE_MORE_CAPACITY = 1 << 5,
    RUBBISH =            1 << 6,
    EMPTY_STACK =        1 << 7,
#ifdef CANARY
    L_CANARY_ERR =       1 << 8,
    R_CANARY_ERR =       1 << 9,
    L_CANARY_DATA_ERR =  1 << 10,
    R_CANARY_DATA_ERR =  1 << 11,
#endif
#ifdef HASH
    HASH_ERR =           1 << 12
#endif
};

enum Actions
{
    EXPAND = 1,
    REDUCE = 2
};

struct Stack;

const int CAPACITY_START = 8;
const int COEFF_ALLOC = 2;
#ifdef CANARY
const Canaryt REF_VAL_CAN = 0xFFFFFFFFFFFFFFFF;
#endif

int make_stack (Stack** stk);
int stack_ctor (Stack* stk, const char* name, const char* file, const char* func, int line);
int stack_dtor (Stack* stk);
int stack_push (Stack* stk, Elemt value);
int stack_pop (Stack* stk, Elemt* value);
int print_stack (Stack* stk);

#endif //STACK_HEADER
