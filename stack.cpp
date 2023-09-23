#include "stack.h"

#define STACK_DUMP(stk, error) \
        stack_dump (stk, error, __FILE__, __func__, __LINE__);

Errors stack_ctor (Stack* stk, const char* name, const char* file, const char* func, int line)
{
    if (!(stk && name && file && func))
        return NULL_POINTER;

    stk->capacity = CAPACITY_START;
    stk->size_st = 0;

#ifndef CANARY
    stk->data = (Elemt*) calloc (CAPACITY_START, sizeof (Elemt));
#else
    stk->l_canary = REF_VAL_CAN;
    stk->r_canary = REF_VAL_CAN;
    stk->data = (Elemt*) calloc (sizeof (Canaryt) * 2 + CAPACITY_START * sizeof (Elemt), 1);
    ((Canaryt*) (stk->data))[0] = REF_VAL_CAN;
    ((Canaryt*) (get_elem_point (stk, CAPACITY_START)))[0] = REF_VAL_CAN;
#endif

    stk->name = name;
    stk->file = file;
    stk->func = func;
    stk->line = line;

    Errors error = stack_verify (stk);
    if (error != CORRECT)
    {
        STACK_DUMP(stk, error);
        return error;
    }

    return CORRECT;
}

Errors stack_dtor (Stack* stk)
{
    if (!stk)
        return NULL_POINTER;

#ifdef CANARY
    stk->l_canary = 0;
    stk->r_canary = 0;
#endif

    stk->size_st = -1;
    stk->capacity = -1;
    stk->name = NULL;
    stk->func = NULL;
    stk->file = NULL;
    stk->line = -1;
    free (stk->data);
    stk->data = NULL;

    return CORRECT;
}

Errors stack_push (Stack* stk, Elemt value)
{
    Errors error = stack_verify (stk);
    if (error != CORRECT)
    {
        STACK_DUMP(stk, error);
        return error;
    }

    if (stk->size_st == stk->capacity)
    {
        error = stack_realloc (stk, EXPAND);
        if (error != CORRECT)
        {
            STACK_DUMP(stk, error);
            return error;
        }
    }

    (get_elem_point (stk, stk->size_st))[0] = value;
    (stk->size_st)++;
    return CORRECT;
}

Errors stack_pop (Stack* stk, Elemt* value)
{
    Errors error = stack_verify (stk);
    if (error != CORRECT)
    {
        STACK_DUMP(stk, error);
        return error;
    }

    if ((stk->size_st) == 0)
    {
        printf ("Stack is empty. Unable to perform pop\n");
        *value = INT_MAX;
        return EMPTY_STACK;
    }

    *value = (get_elem_point (stk, stk->size_st - 1))[0];
    (get_elem_point (stk, stk->size_st - 1))[0] = INT_MAX;
    (stk->size_st)--;
    if ((stk->size_st) < ((stk->capacity) / COEFF_ALLOC))
    {
        error = stack_realloc (stk, REDUCE);
        if (error != CORRECT)
        {
            STACK_DUMP(stk, error);
            return error;
        }
    }

    return CORRECT;
}

Errors stack_verify (const Stack* stk)
{
    if (!(stk && (stk->data) && (stk->name) && (stk->file) && (stk->func)))   return NULL_POINTER;
    if ((stk->size_st) < 0)                                                   return NEGATIVE_SIZE;
    if ((stk->capacity) < 0)                                                  return NEGATIVE_CAPACITY;
    if ((stk->capacity) < (stk->size_st))                                     return SIZE_MORE_CAPACITY;

#ifdef CANARY
    if ((stk->l_canary) != REF_VAL_CAN)                                       return L_CANARY_ERR;
    if ((stk->r_canary) != REF_VAL_CAN)                                       return R_CANARY_ERR;
    if (((Canaryt*) (stk->data))[0] != REF_VAL_CAN)                           return L_CANARY_DATA_ERR;
    if (((Canaryt*) (get_elem_point (stk, stk->capacity)))[0] != REF_VAL_CAN) return R_CANARY_DATA_ERR;
    for (int i = 0; i < (stk->size_st); i++)
        if ((get_elem_point (stk, i))[0] == INT_MAX)                          return RUBBISH;
#else
    for (int i = 0; i < (stk->size_st); i++)
        if ((stk->data)[i] == INT_MAX)                                        return RUBBISH;
#endif

    return CORRECT;
}

void print_error (Errors error)
{
    printf (BOLD_RED_COL);
    switch (error)
    {
        case CORRECT:
            printf ("Correct\n");
            break;
        case NULL_POINTER:
            printf ("Null pointer\n");
            break;
        case MEM_ALLOC_ERR:
            printf ("Error of memory allocation\n");
            break;
        case NEGATIVE_SIZE:
            printf ("Size of stack is negative\n");
            break;
        case NEGATIVE_CAPACITY:
            printf ("Size of capacity is negative\n");
            break;
        case SIZE_MORE_CAPACITY:
            printf ("Size is more than capacity\n");
            break;
        case RUBBISH:
            printf ("This stack contains rubbish values\n");
            break;
        case EMPTY_STACK:
            printf ("This stack is empty\n");
            break;

#ifdef CANARY
        case L_CANARY_ERR:
            printf ("Left canary is incorrect\n");
            break;
        case R_CANARY_ERR:
            printf ("Right canary is incorrect\n");
            break;
        case L_CANARY_DATA_ERR:
            printf ("Left canary in data is incorrect\n");
            break;
        case R_CANARY_DATA_ERR:
            printf ("Right canary in data is incorrect\n");
            break;
#endif

        default:
            break;
    }
    printf (OFF_COL);
}

void stack_dump (const Stack* stk, Errors error, const char* file, const char* func, int line)
{
    printf (RED_COL "Error is in file %s, function %s, line %d in stack %s.\n", stk->file, stk->func, stk->line, stk->name);
    printf ("Called from file %s, function %s, line %d.\n", file, func, line);
    if (!stk)
    {
        printf ("Null pointer of stack\n");
        return;
    }

    print_error (error);
    printf (RED_COL);

#ifdef CANARY
    printf ("Left canary if 0x%llX\n", stk->l_canary);
    printf ("Right canary if 0x%llX\n", stk->r_canary);
#endif

    printf ("Size     of stack is %d\n"
            "Capacity of stack is %d\n",
            stk->size_st,
            stk->capacity);

    if (!(stk->data))
    {
        printf ("Null pointer of data in stack\n" OFF_COL);
        return;
    }
#ifdef CANARY
    printf ("Left canary in data is 0x%llX\n", ((Canaryt*) stk->data)[0]);
    for (int i = 0; i < stk->capacity; i++)
    {
        if (i < stk->size_st)
            printf ("*");
        else
            printf (" ");
        printf ("[%d] = %d\n", i, (get_elem_point (stk, i))[0]);
    }
    printf ("Right canary in data is 0x%llX\n", ((Canaryt*) (get_elem_point (stk, stk->capacity)))[0]);
#else
    for (int i = 0; i < stk->capacity; i++)
    {
        if (i < stk->size_st)
            printf ("*");
        else
            printf (" ");
        printf ("[%d] = %d\n", i, (stk->data)[i]);
    }
#endif
    printf (OFF_COL);
}

Errors stack_realloc (Stack* stk, Actions action)
{
    Errors error = stack_verify (stk);
    if (error != CORRECT)
    {
        STACK_DUMP(stk, error);
        return error;
    }

#ifndef CANARY
    if (action == EXPAND)
    {
        stk->data = (Elemt*) realloc (stk->data,
                                     (stk->capacity) * COEFF_ALLOC *
                                     sizeof (Elemt));
        if (!(stk->data))
            return MEM_ALLOC_ERR;

        stk->capacity = (stk->capacity) * COEFF_ALLOC;
        for (int i = stk->size_st; i < stk->capacity; i++)
            (stk->data)[i] = INT_MAX;
    }
    else
    {
        stk->data = (Elemt*) realloc (stk->data,
                                     (stk->capacity) / COEFF_ALLOC *
                                     sizeof (Elemt));
        if (!(stk->data))
            return MEM_ALLOC_ERR;

        stk->capacity = (stk->capacity) / COEFF_ALLOC;
        for (int i = stk->size_st; i < stk->capacity; i++)
            (stk->data)[i] = INT_MAX;
    }
#else
    if (action == EXPAND)
    {
        ((Canaryt*) (get_elem_point (stk, stk->capacity)))[0] = INT_MAX;
        stk->data = (Elemt*) realloc (stk->data, sizeof (Canaryt) * 2 +
                                     (stk->capacity) * COEFF_ALLOC *
                                     sizeof (Elemt));
        if (!(stk->data))
            return MEM_ALLOC_ERR;

        stk->capacity = (stk->capacity) * COEFF_ALLOC;
        ((Canaryt*) (get_elem_point (stk, stk->capacity)))[0] = REF_VAL_CAN;
        for (int i = stk->size_st; i < stk->capacity; i++)
            (get_elem_point (stk, i))[0] = INT_MAX;
    }
    else
    {
        ((Canaryt*) (get_elem_point (stk, stk->capacity)))[0] = INT_MAX;
        stk->data = (Elemt*) realloc (stk->data, sizeof (Canaryt) * 2 +
                                     (stk->capacity) / COEFF_ALLOC *
                                     sizeof (Elemt));
        if (!(stk->data))
            return MEM_ALLOC_ERR;

        stk->capacity = (stk->capacity) / COEFF_ALLOC;
        ((Canaryt*) (get_elem_point (stk, stk->capacity)))[0] = REF_VAL_CAN;
        for (int i = stk->size_st; i < stk->capacity; i++)
            (get_elem_point (stk, i))[0] = INT_MAX;
    }
#endif

    return CORRECT;
}

Errors print_stack (Stack* stk)
{
    Errors error = stack_verify (stk);
    if (error != CORRECT)
    {
        STACK_DUMP(stk, error);
        return error;
    }

    printf ("Size - %d, capacity - %d\n", stk->size_st, stk->capacity);
#ifndef CANARY
    if (stk->size_st > 0)
    {
        for (int i = 0; i < stk->size_st; i++)
            printf ("%d ", (stk->data)[i]);
        printf ("\n");
    }
#else
    if (stk->size_st > 0)
    {
        for (int i = 0; i < stk->size_st; i++)
            printf ("%d ", (get_elem_point (stk, i))[0]);
        printf ("\n");
    }
#endif

    return CORRECT;
}

#ifdef CANARY
Elemt* get_elem_point (const Stack* stk, int num)
{
    return (Elemt*) ((char*) (stk->data) + sizeof (Canaryt) + num * sizeof (Elemt));
}
#endif
