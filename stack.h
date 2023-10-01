#ifndef STACK_HEADER
#define STACK_HEADER

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define STACK_TEMPLATE(TYPE)                                      \
        typedef struct Stack_##TYPE                               \
        {                                                         \
        size_t capacity;                                          \
        size_t size_st;                                           \
        TYPE* data;                                               \
        } Stack_##TYPE;                                           \
                                                                  \
        void stack_ctor_##TYPE (Stack_##TYPE* stk)                \
        {                                                         \
            stk->size_st = 0;                                     \
            stk->capacity = 8;                                    \
            stk->data = (TYPE*) calloc (8, sizeof (TYPE));        \
        }                                                         \
                                                                  \
        void stack_push_##TYPE (Stack_##TYPE* stk, TYPE value)    \
        {                                                         \
            stk->data[stk->size_st] = value;                      \
            (stk->size_st)++;                                     \
            if (need_realloc (stk->size_st, stk->capacity))       \
            {                                                     \
                stk->capacity = stk->capacity * 2;                \
                stk->data = (TYPE*) realloc (stk->data,           \
                            stk->capacity * sizeof (TYPE));       \
            }                                                     \
        }                                                         \
                                                                  \
        void stack_pop_##TYPE (Stack_##TYPE* stk, TYPE* value)    \
        {                                                         \
            *value = stk->data[stk->size_st - 1];                 \
            (stk->size_st)--;                                     \
            if (need_realloc (stk->size_st, stk->capacity))       \
            {                                                     \
                stk->capacity = stk->capacity / 2;                \
                stk->data = (TYPE*) realloc (stk->data,           \
                            stk->capacity * sizeof (TYPE));       \
            }                                                     \
        }                                                         \
                                                                  \
        void stack_print_##TYPE (Stack_##TYPE* stk)               \
        {                                                         \
            printf ("Size - %d, capacity - %d\n",                 \
                    stk->size_st, stk->capacity);                 \
            for (int i = 0; i < stk->size_st; i++)                \
            {                                                     \
                if (strcmp (#TYPE, "int"))                        \
                    printf ("%d ", (stk->data)[i]);               \
                else                                              \
                    printf ("%lf ", (stk->data)[i]);              \
            }                                                     \
            printf ("\n");                                        \
        }                                                         \
                                                                  \
        void stack_dtor_##TYPE (Stack_##TYPE* stk)                \
        {                                                         \
            (stk)->size_st = 0;                                   \
            (stk)->capacity = 8;                                  \
            free ((stk)->data);                                   \
        }

int need_realloc (size_t size_st, size_t capacity)
{
    return ((size_st < capacity / 4) && (capacity > 8))
            || (size_st == capacity);
}

#define STACK_CTOR(stk)             \
        _Generic ((*stk).data,      \
        int*: stack_ctor_int,       \
        double*: stack_ctor_double  \
        ) (stk)

#define STACK_PUSH(stk, value)     \
        _Generic ((value),         \
        int: stack_push_int,       \
        double: stack_push_double  \
        ) (stk, value)

#define STACK_POP(stk, value)      \
        _Generic ((value),         \
        int*: stack_pop_int,       \
        double*: stack_pop_double  \
        ) (stk, value)

#define STACK_DTOR(stk)             \
        _Generic ((*stk).data,      \
        int*: stack_ctor_int,       \
        double*: stack_ctor_double  \
        ) (stk)

#define STACK_PRINT(stk)             \
        _Generic ((*stk).data,       \
        int*: stack_print_int,       \
        double*: stack_print_double  \
        ) (stk)

#endif //STACK_HEADER
