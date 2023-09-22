#ifndef STACK_HEADER
#define STACK_HEADER

#include <stdlib.h>
#include <stdio.h>
#include <limits.h>

#include "colors.h"

typedef int Elemt;

enum Errors
{
    CORRECT =           -1,
    NULL_POINTER =       1,
    MEM_ALLOC_ERR =      2,
    NEGATIVE_SIZE =      3,
    NEGATIVE_CAPACITY =  4,
    SIZE_MORE_CAPACITY = 5,
    RUBBISH =            6,
    EMPTY_STACK =        7
};

enum Actions
{
    EXPAND = 1,
    REDUCE = 2
};

struct Stack
{
    Elemt* data;
    int size_st;
    int capacity;
};

const int CAPACITY_START = 1;
const int COEFF_ALLOC = 2;

Errors stack_ctor (Stack* stk);
Errors stack_dtor (Stack* stk);
Errors stack_push (Stack* stk, Elemt value);
Errors stack_pop (Stack* stk, Elemt* value);
Errors stack_verify (const Stack* stk);
void print_error (Errors error);
void stack_dump (const Stack* stk, Errors error, const char* file, const char* func, int line);
Errors stack_realloc (Stack* stk, Actions action);
Errors print_stack (Stack* stk);

#endif //STACK_HEADER
