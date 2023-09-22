#include "stack.h"

#define STACK_DUMP(stk, error) \
        stack_dump (stk, error, __FILE__, __func__, __LINE__);

Errors stack_ctor (Stack* stk)
{
    if (!stk)
        return NULL_POINTER;

    stk->capacity = CAPACITY_START;
    stk->size_st = 0;
    stk->data = (Elemt*) calloc (CAPACITY_START, sizeof (Elemt));

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

    stk->size_st = -1;
    stk->capacity = -1;
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

    (stk->data)[stk->size_st] = value;
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

    *value = (stk->data)[stk->size_st - 1];
    (stk->data)[stk->size_st - 1] = INT_MAX;
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
    if (!stk)                               return NULL_POINTER;
    if (!(stk->data))                       return NULL_POINTER;
    if ((stk->size_st) < 0)                 return NEGATIVE_SIZE;
    if ((stk->capacity) < 0)                return NEGATIVE_CAPACITY;
    if ((stk->capacity) < (stk->size_st))   return SIZE_MORE_CAPACITY;
    for (int i = 0; i < (stk->size_st); i++)
        if ((stk->data)[i] == INT_MAX)       return RUBBISH;
    return CORRECT;
}

void print_error (Errors error)
{
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
        default:
            break;
    }
}

void stack_dump (const Stack* stk, Errors error, const char* file, const char* func, int line)
{
    printf (RED_COL "Error is in %s in function %s in %d line\n", file, func, line);
    if (!stk)
    {
        printf ("Null pointer of stack\n");
        return;
    }

    print_error (error);
    printf ("Size     of stack is %d\n"
            "Capacity of stack is %d\n",
            stk->size_st,
            stk->capacity);

    if (!(stk->data))
    {
        printf ("Null pointer of data in stack\n" OFF_COL);
        return;
    }
    for (int i = 0; i < stk->capacity; i++)
    {
        if (i < stk->size_st)
            printf ("*");
        else
            printf (" ");
        printf ("[%d] = %d\n", i, (stk->data)[i]);
    }
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
    if (stk->size_st > 0)
    {
        for (int i = 0; i < stk->size_st; i++)
            printf ("%d ", (stk->data)[i]);
        printf ("\n");
    }

    return CORRECT;
}
