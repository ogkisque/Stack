#ifndef STACK_HEADER
#define STACK_HEADER

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <stdbool.h>
#include "colors.h"

#define HASH
#define CANARY

#ifdef CANARY
#define ON_CANARY(...) __VA_ARGS__
#else
#define ON_CANARY(...)
#endif

#ifdef HASH
#define ON_HASH(...) __VA_ARGS__
#else
#define HASH(...)
#endif

ON_CANARY
(
    typedef unsigned long long Canaryt;
)
ON_HASH
(
    typedef unsigned long long Hasht;
)

enum Errors
{
    NULL_POINTER =       1 << 1,
    MEM_ALLOC_ERR =      1 << 2,
    NEGATIVE_SIZE =      1 << 3,
    NEGATIVE_CAPACITY =  1 << 4,
    SIZE_MORE_CAPACITY = 1 << 5,
    RUBBISH =            1 << 6,
    EMPTY_STACK =        1 << 7,
    ON_CANARY
    (
        L_CANARY_ERR =       1 << 8,
        R_CANARY_ERR =       1 << 9,
        L_CANARY_DATA_ERR =  1 << 10,
        R_CANARY_DATA_ERR =  1 << 11,
    )
    ON_HASH
    (
        HASH_ERR =           1 << 12
    )
};

enum Actions
{
    EXPAND = 1,
    REDUCE = 2
};

typedef struct Stack
{
    ON_CANARY
    (
        Canaryt l_canary;
    )
    size_t capacity;
    size_t size_st;
    void* data;
    const char* name;
    const char* file;
    const char* func;
    int line;
    ON_HASH
    (
        Hasht hash_st;
    )
    ON_CANARY
    (
        Canaryt r_canary;
    )
} Stack;

int stack_ctor_kernel (Stack* stk, const char* name, const char* file, const char* func, int line, size_t size_el);
int stack_dtor_kernel (Stack* stk, size_t size_el);
int stack_push_kernel (Stack* stk, void* value, size_t size_el);
int stack_pop_kernel (Stack* stk, void* value, size_t size_el);
int stack_verify_kernel (Stack* stk, size_t size_el);
void print_error (int error);
void stack_dump_kernel (const Stack* stk, int error, const char* file, const char* func, int line, size_t size_el);
int stack_realloc_kernel (Stack* stk, enum Actions action, size_t size_el);
bool need_realloc (const Stack* stk);
void* get_elem_point_kernel (const Stack* stk, int num, size_t elem_el);
int stack_print_kernel (Stack* stk, size_t size_el);
ON_HASH
(
    Hasht get_hash (const Stack* stk, size_t size_el);
)

#define STACK_DUMP(stk, error, size_el) \
        stack_dump_kernel (stk, error, __FILE__, __func__, __LINE__, size_el)

#define STACK_PRINT_ELEM(stk, i, size_el)                                                               \
        _Generic ((*stk).data,                                                                          \
        int*: printf ("%d ", (*(int*) get_elem_point_kernel ((Stack*) stk, i, size_el))),               \
        double*: printf ("%lf ", (*(double*) get_elem_point_kernel ((Stack*) stk, i, size_el)))         \
        )

#define STACK_DUMP_ELEM(stk, i, size_el)                                                                    \
        _Generic ((*stk).data,                                                                              \
        int*: printf ("[%d] = %d\n", i, (*(int*) get_elem_point_kernel ((Stack*) stk, i, size_el))),        \
        double*: printf ("[%d] = %lf\n", i, (*(double*) get_elem_point_kernel ((Stack*) stk, i, size_el)))  \
        )

#define STACK_TEMPLATE(TYPE)                                                                                            \
        typedef struct Stack_##TYPE                                                                                     \
        {                                                                                                               \
            ON_CANARY                                                                                                   \
            (                                                                                                           \
                Canaryt l_canary;                                                                                       \
            )                                                                                                           \
            size_t capacity;                                                                                            \
            size_t size_st;                                                                                             \
            TYPE* data;                                                                                                 \
            const char* name;                                                                                           \
            const char* file;                                                                                           \
            const char* func;                                                                                           \
            int line;                                                                                                   \
            ON_HASH                                                                                                     \
            (                                                                                                           \
                Hasht hash;                                                                                             \
            )                                                                                                           \
            ON_CANARY                                                                                                   \
            (                                                                                                           \
                Canaryt r_canary;                                                                                       \
            )                                                                                                           \
        } Stack_##TYPE;                                                                                                 \
                                                                                                                        \
        int stack_ctor_##TYPE (Stack_##TYPE* stk, const char* name, const char* file, const char* func, int line)       \
        {                                                                                                               \
            stack_ctor_kernel ((Stack*) stk, name, file, func, line, sizeof (TYPE));                                    \
        }                                                                                                               \
                                                                                                                        \
        int stack_dtor_##TYPE (Stack_##TYPE* stk)                                                                       \
        {                                                                                                               \
            stack_dtor_kernel ((Stack*) stk, sizeof (TYPE));                                                            \
        }                                                                                                               \
                                                                                                                        \
        int stack_push_##TYPE (Stack_##TYPE* stk, TYPE value)                                                           \
        {                                                                                                               \
            stack_push_kernel ((Stack*) stk, (void*) &value, sizeof (TYPE));                                            \
        }                                                                                                               \
                                                                                                                        \
        int stack_pop_##TYPE (Stack_##TYPE* stk, TYPE* value)                                                           \
        {                                                                                                               \
            stack_pop_kernel ((Stack*) stk, (void*) value, sizeof (TYPE));                                              \
        }                                                                                                               \
                                                                                                                        \
        void stack_dump_##TYPE (Stack_##TYPE* stk, int error, const char* file, const char* func, int line)             \
        {                                                                                                               \
            stack_dump_kernel ((Stack*) stk, error, file, func, line, sizeof (TYPE));                                   \
            for (int i = 0; i < stk->capacity; i++)                                                                     \
            {                                                                                                           \
                if (i < stk->size_st)                                                                                   \
                    printf ("*");                                                                                       \
                else                                                                                                    \
                    printf (" ");                                                                                       \
                STACK_DUMP_ELEM(stk, i, sizeof (TYPE));                                                                                                     \
            }                                                                                                           \
            printf (OFF_COL);                                                                                           \
        }                                                                                                               \
                                                                                                                        \
        void stack_print_##TYPE (Stack_##TYPE* stk)                                                                     \
        {                                                                                                               \
            if (stack_print_kernel ((Stack*) stk, sizeof (TYPE)) == 0)                                                  \
            {                                                                                                           \
                if (stk->size_st > 0)                                                                                   \
                {                                                                                                       \
                    for (int i = 0; i < stk->size_st; i++)                                                              \
                    {                                                                                                   \
                        STACK_PRINT_ELEM(stk, i, sizeof (TYPE));                                                                                             \
                    }                                                                                                   \
                    printf ("\n");                                                                                      \
                }                                                                                                       \
            }                                                                                                           \
            printf (OFF_COL);                                                                                           \
        }                                                                                                               \
                                                                                                                        \
        int stack_realloc_##TYPE (Stack_##TYPE* stk, enum Actions action)                                               \
        {                                                                                                               \
            stack_realloc_kernel ((Stack*) stk, action, sizeof (TYPE));                                                 \
        }                                                                                                               \
                                                                                                                        \
        void* get_elem_point_##TYPE (const Stack* stk, int num, size_t elem_el)                                         \
        {                                                                                                               \
            get_elem_point_kernel((Stack*)stk, num, sizeof(TYPE));                                                      \
        }

#define STACK_CTOR(stk)             \
        _Generic ((*stk).data,      \
        int*: stack_ctor_int,       \
        double*: stack_ctor_double  \
        ) (stk, #stk, __FILE__, __func__, __LINE__)

#define STACK_DTOR(stk)             \
        _Generic ((*stk).data,      \
        int*: stack_dtor_int,       \
        double*: stack_dtor_double  \
        ) (stk)

#define STACK_PRINT(stk)             \
        _Generic ((*stk).data,       \
        int*: stack_print_int,       \
        double*: stack_print_double  \
        ) (stk)

#define STACK_PUSH(stk, value)     \
        _Generic ((*stk).data,     \
        int*: stack_push_int,      \
        double*: stack_push_double \
        ) (stk, value)

#define STACK_POP(stk, value)      \
        _Generic ((*stk).data,     \
        int*: stack_pop_int,       \
        double*: stack_pop_double  \
        ) (stk, value)



#endif //STACK_HEADER
