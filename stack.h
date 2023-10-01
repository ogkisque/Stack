#ifndef STACK_HEADER
#define STACK_HEADER

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include "colors.h"

#define CANARY
#define HASH

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

const int CAPACITY_START = 8;
const int COEFF_ALLOC = 2;
#ifdef CANARY
const Canaryt REF_VAL_CAN = 0xFFFFFFFFFFFFFFFF;
#endif

#ifdef CANARY
#define IFDEF_INIT_LCANARY() \
    Canaryt l_canary;
#define IFDEF_INIT_RCANARY() \
    Canaryt r_canary;
#define IFDEF_MEM_ALLOC()                                       \
    stk->l_canary = REF_VAL_CAN;                                \
    stk->r_canary = REF_VAL_CAN;                                \
    stk->data = (TYPE*) calloc (sizeof (Canaryt) * 2 + CAPACITY_START * sizeof (TYPE), 1);  \
    ((Canaryt*) (stk->data))[0] = REF_VAL_CAN;                                              \
    ((Canaryt*) (GET_ELEM(stk, CAPACITY_START)))[0] = REF_VAL_CAN;
#define IFDEF_DTOR_CANARY()                                  \
    stk->l_canary = 0;                                       \
    stk->r_canary = 0;                                       \
    ((Canaryt*)stk->data)[0] = 0;                            \
    ((Canaryt*) get_elem_point (stk, stk->capacity))[0] = 0;
#define IFDEF_GET_ELEM() \
    return (TYPE*) ((char*) (stk->data) + sizeof (Canaryt) + num * sizeof (TYPE));
#define IFDEF_CANARY_HASH()                                                                                          \
    for (char* i = (char*) ((char*) stk->data + sizeof (Canaryt)); i < ((char*) GET_ELEM(stk, stk->size_st)); i++) \
        hash_st = hash_st * 33 + *i;
#define IFDEF_CHECK_CANARY()                                                                                           \
    if ((stk->l_canary) != REF_VAL_CAN)                                       error |= L_CANARY_ERR;           \
    if ((stk->r_canary) != REF_VAL_CAN)                                       error |= R_CANARY_ERR;           \
    if (((Canaryt*) (stk->data))[0] != REF_VAL_CAN)                           error |= L_CANARY_DATA_ERR;      \
    if (((Canaryt*) (GET_ELEM(stk, stk->capacity)))[0] != REF_VAL_CAN)        error |= R_CANARY_DATA_ERR;
#define IFDEF_PRINT_CANARY_DATA()                                                                                \
    printf ("Left canary in data is 0x%llX\n", ((Canaryt*) stk->data)[0]);                                     \
    printf ("Right canary in data is 0x%llX\n", ((Canaryt*) (GET_ELEM(stk, stk->capacity)))[0]);
#define IFDEF_PRINT_CANARY()                                      \
    printf ("Left canary if 0x%llX\n", stk->l_canary);            \
    printf ("Right canary if 0x%llX\n", stk->r_canary);
#define IFDEF_MEM_REALLOC() \
    new_size = 2 * sizeof (Canaryt) + stk->capacity * sizeof (TYPE);
#define IFDEF_SET_CANARY \
    ((Canaryt*) (GET_ELEM(stk, stk->capacity)))[0] = REF_VAL_CAN;
#else
#define IFDEF_MEM_ALLOC()                                       \
    stk->data = (TYPE*) calloc (CAPACITY_START, sizeof (TYPE));
#define IFDEF_DTOR_CANARY()
#define IFDEF_GET_ELEM() \
    return &(stk->data[num]);
#define IFDEF_CANARY_HASH()                                                                                  \
    for (int i = 0; i < stk->size_st; i++)                                                                 \
        for (char* j = (char*) &(stk->data[i]); j < (char*) (&(stk->data[i]) + sizeof (TYPE)); j++)        \
            hash_st = hash_st * 33 + *j;
#define IFDEF_CHECK_CANARY()
#define IFDEF_PRINT_CANARY_DATA()
#define IFDEF_PRINT_CANARY()
#define IFDEF_MEM_REALLOC() \
    new_size = stk->capacity * sizeof (TYPE);
#define IFDEF_SET_CANARY
#endif

#ifdef HASH
#define IFDEF_INIT_HASH() \
    Hasht hash_st;
#define IFDEF_GET_HASH()              \
    stk->hash_st = GET_HASH(stk);
#define IFDEF_HASH_FUNCTION()                                                              \
        Hasht get_hash_##TYPE (const Stack_##TYPE* stk)                                    \
        {                                                                                  \
            Hasht hash_st = 5381;                                                          \
            for (char* i = (char*) &(stk->size_st); i < ((char*) &(stk->size_st) + sizeof (stk->size_st)); i++)    \
                hash_st = hash_st * 33 + *i;                                                                       \
            for (char* i = (char*) &(stk->capacity); i < ((char*) &(stk->capacity) + sizeof (stk->capacity)); i++) \
                hash_st = hash_st * 33 + *i;                                                                       \
            IFDEF_CANARY_HASH();                                                                                   \
            return hash_st;                                                                                                \
        }
#define IFDEF_CHECK_HASH()                                                                                     \
    Hasht prev_hash = stk->hash_st;                                                                            \
    Hasht new_hash = GET_HASH(stk);                                                                            \
    stk->hash_st = new_hash;                                                                                   \
    if (prev_hash != new_hash)                                                error |= HASH_ERR;
#define IFDEF_PRINT_HASH()                                        \
    printf ("Hash is %llu\n", stk->hash_st);
#else
#define IFDEF_GET_HASH()
#define IFDEF_HASH_FUNCTION()
#define IFDEF_CHECK_HASH()
#define IFDEF_PRINT_HASH()
#endif

#define STACK_TEMPLATE(TYPE)                                      \
        typedef struct Stack_##TYPE                               \
        {                                                         \
        IFDEF_INIT_LCANARY();                                     \
        size_t capacity;                                          \
        size_t size_st;                                           \
        TYPE* data;                                               \
        const char* name;                                         \
        const char* file;                                         \
        const char* func;                                         \
        int line;                                                 \
        IFDEF_INIT_HASH();                                        \
        IFDEF_INIT_RCANARY();                                     \
        } Stack_##TYPE;                                           \
                                                                  \
        int stack_ctor_##TYPE (Stack_##TYPE* stk, const char* name, const char* file, const char* func, int line) \
        {                                                         \
            int error = 0;                                        \
            if (!(stk && name && file && func))                   \
            {                                                     \
                error |= NULL_POINTER;                            \
                return error;                                     \
            }                                                     \
            stk->capacity = CAPACITY_START;                       \
            stk->size_st = 0;                                     \
            IFDEF_MEM_ALLOC();                                    \
            stk->name = name;                                                                       \
            stk->file = file;                                                                       \
            stk->func = func;                                                                       \
            stk->line = line;                                                                       \
            for (int i = 0; i < stk->capacity; i++)                                                 \
                (GET_ELEM(stk, i))[0] = INT_MAX;                                                    \
            IFDEF_GET_HASH();                                                                       \                                                                                             \
            error = STACK_VERIFY(stk);                                                              \
            if (error != 0)                                                                         \
            {                                                                                       \
                STACK_DUMP(stk, error);                                                             \
                return error;                                                                       \
            }                                                                                       \
            return error;                                                                           \
        }                                                         \
                                                                  \
        int stack_push_##TYPE (Stack_##TYPE* stk, TYPE value)     \
        {                                                         \
            int error = STACK_VERIFY(stk);                        \
            if (error != 0)                                       \
            {                                                     \
                STACK_DUMP(stk, error);                           \
                return error;                                     \
            }                                                     \
            if (need_realloc (stk))                               \
            {                                                     \
                error = STACK_REALLOC(stk, EXPAND);               \
                if (error != 0)                                   \
                {                                                 \
                    STACK_DUMP(stk, error);                       \
                    return error;                                 \
                }                                                 \
            }                                                     \
            (GET_ELEM(stk, stk->size_st))[0] = value;             \
            (stk->size_st)++;                                     \
            IFDEF_GET_HASH();                                     \
            return error;                                         \
        }                                                         \
                                                                  \
        int stack_pop_##TYPE (Stack_##TYPE* stk, TYPE* value)     \
        {                                                         \
            int error = STACK_VERIFY(stk);                        \
            if (error != 0)                                       \
            {                                                     \
                STACK_DUMP(stk, error);                           \
                return error;                                     \
            }                                                     \
            if (!value)                                           \
            {                                                     \
                error |= MEM_ALLOC_ERR;                           \
                return error;                                     \
            }                                                     \
            if ((stk->size_st) == 0)                              \
            {                                                     \
                printf ("Stack is empty. Unable to perform pop\n");\
                *value = INT_MAX;                                 \
                error |= EMPTY_STACK;                             \
                return error;                                     \
            }                                                     \
            *value = (GET_ELEM(stk, stk->size_st - 1))[0];        \
            (GET_ELEM(stk, stk->size_st - 1))[0] = INT_MAX;       \
            (stk->size_st)--;                                     \
            if (need_realloc (stk))                               \
            {                                                     \
                error = STACK_REALLOC(stk, REDUCE);               \
                if (error != 0)                                   \
                {                                                 \
                    STACK_DUMP(stk, error);                       \
                    return error;                                 \
                }                                                 \
            }                                                     \
            IFDEF_GET_HASH();                                     \
            return error;                                         \
        }                                                         \
                                                                  \
        int stack_print_##TYPE (Stack_##TYPE* stk)                \
        {                                                         \
            int error = STACK_VERIFY(stk);                        \
            if (error != 0)                                       \
            {                                                     \
                STACK_DUMP(stk, error);                           \
                return error;                                     \
            }                                                     \
            printf ("Size - %d, capacity - %d\n",                 \
                    stk->size_st, stk->capacity);                 \
            for (int i = 0; i < stk->size_st; i++)                \
            {                                                     \
                _Generic (stk->data,                              \
                int*: printf ("%d ", (stk->data)[i]),             \
                double*: printf ("%lf ", (stk->data)[i]));        \
            }                                                     \
            printf ("\n");                                        \
        }                                                         \
                                                                  \
        int stack_dtor_##TYPE (Stack_##TYPE* stk)                 \
        {                                                         \
            int error = 0;                                        \
            if (!stk)                                             \
            {                                                     \
                error |= NULL_POINTER;                            \
                return error;                                     \
            }                                                     \
            IFDEF_DTOR_CANARY();                                  \
            stk->size_st = -1;                                       \
            stk->capacity = -1;                                      \
            stk->name = NULL;                                        \
            stk->func = NULL;                                        \
            stk->file = NULL;                                        \
            stk->line = -1;                                          \
            for (int i = 0; i < (stk->size_st); i++)                 \
                *get_elem_point (stk, i) = INT_MAX;                  \
            free (stk->data);                                        \
            stk->data = NULL;                                        \
            return error;                                            \
        }                                                         \
                                                                  \
        TYPE* get_elem_point_##TYPE (const Stack_##TYPE* stk, int num)   \
        {                                                                \
            IFDEF_GET_ELEM();                                            \
        }                                                                                  \
                                                                                           \
        IFDEF_HASH_FUNCTION();                                                             \
                                                                                                                       \
        int stack_verify_##TYPE (Stack_##TYPE* stk)                                                                    \
        {                                                                                                              \
            int error = 0;                                                                                             \
            if (!(stk && (stk->data) && (stk->name) && (stk->file) && (stk->func)))                                    \
            {                                                                                                          \
                error |= NULL_POINTER;                                                                                 \
                return error;                                                                                          \
            }                                                                                                          \
            if ((stk->size_st) < 0)                                                   error |= NEGATIVE_SIZE;          \
            if ((stk->capacity) < 0)                                                  error |= NEGATIVE_CAPACITY;      \
            if ((stk->capacity) < (stk->size_st))                                     error |= SIZE_MORE_CAPACITY;     \
            for (int i = 0; i < (stk->size_st); i++)                                                                   \
                    if ((*GET_ELEM(stk, i)) == INT_MAX)                               error |= RUBBISH;                \
            IFDEF_CHECH_CANARY();                                                                                      \
            IFDEF_CHECH_HASH();                                                                                        \
            return error;                                                                                              \
        }                                                                                                              \
                                                                                                                       \
        void stack_dump_##TYPE (const Stack_##TYPE* stk, int error)                                                               \
        {                                                                                                              \
            if (!stk)                                                                                                  \
            {                                                                                                          \
                printf ("Null pointer of stack\n");                                                                    \
                return;                                                                                                \
            }                                                                                                          \
            printf (RED_COL "Error is in file %s, function %s, line %d in stack %s.\n", stk->file, stk->func, stk->line, stk->name); \
            print_error (error);                                                                                       \
            printf (RED_COL);                                                                                          \
            IFDEF_PRINT_CANARY();                                                                                      \
            IFDEF_PRINT_HASH();                                                                                        \
            printf ("Size     of stack is %d\n"                                                                        \
                    "Capacity of stack is %d\n",                                                                       \
                    stk->size_st,                                                                                      \
                    stk->capacity);                                                                                    \
            if (!(stk->data))                                                                                          \
            {                                                                                                          \
                printf ("Null pointer of data in stack\n" OFF_COL);                                                    \
                return;                                                                                                \
            }                                                                                                          \
            IFDEF_PRINT_CANARY_DATA();                                                                                 \
            for (int i = 0; i < stk->capacity; i++)                                                                    \
            {                                                                                                          \
                if (i < stk->size_st)                                                                                  \
                    printf ("*");                                                                                      \
                else                                                                                                   \
                    printf (" ");                                                                                      \
                _Generic (stk->data,                                                                                   \
                int*: printf ("[%d] = %d\n", i, (GET_ELEM(stk, i))[0]),                                                \
                double*: printf ("[%d] = %lf\n", i, (GET_ELEM(stk, i))[0]));                                           \
            }                                                                                                          \
            printf (OFF_COL);                                                                                          \
        }                                                                                                              \
                                                                                \
        int stack_realloc_##TYPE (Stack_##TYPE* stk, Actions action)            \
        {                                                                       \
            int error = 0;                                                      \
            int capacity_start = stk->capacity;                                 \
            TYPE* data_start = stk->data;                                       \
            if (action == EXPAND)                                               \
                stk->capacity = (stk->capacity) * COEFF_ALLOC;                  \
            else                                                                \
                stk->capacity = (stk->capacity) / COEFF_ALLOC;                  \
            int new_size = 0;                                                   \
            IFDEF_MEM_REALLOC();                                                \
            stk->data = (TYPE*) realloc (stk->data, new_size);                  \
            if (!(stk->data))                                                   \
            {                                                                   \
                stk->data = data_start;                                         \
                stk->capacity = capacity_start;                                 \
                error |= MEM_ALLOC_ERR;                                         \
                    return error;                                               \
            }                                                                   \
            IFDEF_SET_CANARY();                                                 \
            for (int i = stk->size_st; i < stk->capacity; i++)                  \
                (GET_ELEM(stk, i))[0] = INT_MAX;                                \
            IFDEF_GET_HASH();                                                   \
            return error;                                                       \
        }

void print_error (int error)
{
    printf (BOLD_RED_COL);
    if (error & NULL_POINTER)
        printf ("Null pointer\n");
    if (error & MEM_ALLOC_ERR)
        printf ("Error of memory allocation\n");
    if (error & NEGATIVE_SIZE)
        printf ("Size of stack is negative\n");
    if (error & NEGATIVE_CAPACITY)
        printf ("Size of capacity is negative\n");
    if (error & SIZE_MORE_CAPACITY)
        printf ("Size is more than capacity\n");
    if (error & RUBBISH)
        printf ("This stack contains rubbish values\n");
    if (error & EMPTY_STACK)
        printf ("This stack is empty\n");

#ifdef CANARY
    if (error & L_CANARY_ERR)
        printf ("Left canary is incorrect\n");
    if (error & R_CANARY_ERR)
        printf ("Right canary is incorrect\n");
    if (error & L_CANARY_DATA_ERR)
        printf ("Left canary in data is incorrect\n");
    if (error & R_CANARY_DATA_ERR)
            printf ("Right canary in data is incorrect\n");
#endif

#ifdef HASH
    if (error & HASH_ERR)
        printf ("Hash is incorrect\n");
#endif

    printf (OFF_COL);
}

int need_realloc (size_t size_st, size_t capacity)
{
    return ((size_st < capacity / COEFF_ALLOC / COEFF_ALLOC) && (capacity > CAPACITY_START))
            || (size_st == capacity);
}

#define STACK_CTOR(stk)             \
        _Generic ((*stk).data,      \
        int*: stack_ctor_int,       \
        double*: stack_ctor_double  \
        ) (stk, #stk, __FILE__, __func__, __LINE__)

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

#define GET_ELEM(stk, i)                \
        _Generic ((*stk).data,          \
        int*: get_elem_point_int,       \
        double*: get_elem_point_double  \
        ) (stk, i)

#define GET_HASH(stk)             \
        _Generic ((*stk).data,    \
        int*: get_hash_int,       \
        double*: get_hash_double  \
        ) (stk)

#define STACK_VERIFY(stk)             \
        _Generic ((*stk).data,        \
        int*: stack_verify_int,       \
        double*: stack_verify_double  \
        ) (stk)

#define STACK_DUMP(stk, error)      \
        _Generic ((*stk).data,      \
        int*: stack_dump_int,       \
        double*: stack_dump_double  \
        ) (stk, error)

#define STACK_REALLOC(stk, action)    \
        _Generic ((*stk).data,        \
        int*: stack_realloc_int,      \
        double*: stack_realloc_double \
        ) (stk, action)

#endif //STACK_HEADER
