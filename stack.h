#ifndef STACK_HEADER
#define STACK_HEADER

#define CANARY

#include <stdlib.h>
#include <stdio.h>
#include <limits.h>

#include "colors.h"

typedef int Elemt;
#ifdef CANARY
typedef unsigned long long Canaryt;
#endif

enum Errors
{
    CORRECT =           -1,
    NULL_POINTER =       1,
    MEM_ALLOC_ERR =      2,
    NEGATIVE_SIZE =      3,
    NEGATIVE_CAPACITY =  4,
    SIZE_MORE_CAPACITY = 5,
    RUBBISH =            6,
    EMPTY_STACK =        7,
#ifdef CANARY
    L_CANARY_ERR =       8,
    R_CANARY_ERR =       9,
    L_CANARY_DATA_ERR =  10,
    R_CANARY_DATA_ERR =  11,
#endif
};

enum Actions
{
    EXPAND = 1,
    REDUCE = 2
};

struct Stack
{
#ifdef CANARY
    Canaryt l_canary;
#endif

    int size_st;
    int capacity;
    Elemt* data;

    const char* name;
    const char* file;
    const char* func;
    int line;

#ifdef CANARY
    Canaryt r_canary;
#endif
};

const int CAPACITY_START = 1;
const int COEFF_ALLOC = 2;
#ifdef CANARY
const Canaryt REF_VAL_CAN = 0xFFFFFFFFFFFFFFFF;
#endif

Errors stack_ctor (Stack* stk, const char* name, const char* file, const char* func, int line);
Errors stack_dtor (Stack* stk);
Errors stack_push (Stack* stk, Elemt value);
Errors stack_pop (Stack* stk, Elemt* value);
Errors stack_verify (const Stack* stk);
void print_error (Errors error);
void stack_dump (const Stack* stk, Errors error, const char* file, const char* func, int line);
Errors stack_realloc (Stack* stk, Actions action);
Errors print_stack (Stack* stk);
#ifdef CANARY
Elemt* get_elem_point (const Stack* stk, int num);
#endif

#endif //STACK_HEADER
