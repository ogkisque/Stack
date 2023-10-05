#include "stack.h"

const int CAPACITY_START = 8;
const int COEFF_ALLOC = 2;
const unsigned long long POISON = 0xFFFFFFFFFFFFFFFF;
ON_CANARY
(
    const Canaryt REF_VAL_CAN = 0xFFFFFFFFFFFFFFFF;
    const Canaryt POISON_VAL_CAN = 0x0000000000000000;
)

int stack_ctor_kernel (Stack* stk, const char* name, const char* file, const char* func, int line, size_t size_el)
{
    int error = 0;
    if (!(stk && name && file && func))
    {
        error |= NULL_POINTER;
        return error;
    }

    stk->capacity = CAPACITY_START;
    stk->size_st = 0;

#ifndef CANARY
    stk->data = calloc (CAPACITY_START, size_el);
#else
    stk->l_canary = REF_VAL_CAN;
    stk->r_canary = REF_VAL_CAN;
    stk->data = calloc (sizeof (Canaryt) * 2 + CAPACITY_START * size_el, 1);
    *((Canaryt*) stk->data) = REF_VAL_CAN;
    *((Canaryt*) get_elem_point_kernel (stk, CAPACITY_START, size_el)) = REF_VAL_CAN;
#endif
    stk->name = name;
    stk->file = file;
    stk->func = func;
    stk->line = line;
    for (int i = 0; i < stk->capacity; i++)
        memcpy (get_elem_point_kernel (stk, i, size_el), &POISON, size_el);
    ON_HASH
    (
        stk->hash_st = get_hash (stk, size_el);
    )
    error = stack_verify_kernel (stk, size_el);
    if (error != 0)
    {
        STACK_DUMP(stk, error, size_el);
        return error;
    }

    return error;
}

int stack_dtor_kernel (Stack* stk, size_t size_el)
{
    int error = 0;
    if (!stk)
    {
        error |= NULL_POINTER;
        return error;
    }

    ON_CANARY
    (
        stk->l_canary = 0;
        stk->r_canary = 0;
        *((Canaryt*) stk->data) = POISON_VAL_CAN;
        *((Canaryt*) get_elem_point_kernel (stk, stk->capacity, size_el)) = POISON_VAL_CAN;
    )
    for (int i = 0; i < (stk->size_st); i++)
        memcpy (get_elem_point_kernel (stk, i, size_el), &POISON, size_el);
    stk->size_st = -1;
    stk->capacity = -1;
    stk->name = NULL;
    stk->func = NULL;
    stk->file = NULL;
    stk->line = -1;
    free (stk->data);
    stk->data = NULL;
    return error;
}

int stack_push_kernel (Stack* stk, void* value, size_t size_el)
{
    int error = stack_verify_kernel (stk, size_el);
    if (error != 0)
    {
        STACK_DUMP(stk, error, size_el);
        return error;
    }

    if (need_realloc (stk))
    {
        error = stack_realloc_kernel (stk, EXPAND, size_el);
        if (error != 0)
        {
            STACK_DUMP(stk, error, size_el);
            return error;
        }
    }

    memcpy (get_elem_point_kernel (stk, stk->size_st, size_el), value, size_el);
    (stk->size_st)++;
    ON_HASH
    (
    stk->hash_st = get_hash (stk, size_el);
    )
    return error;
}

int stack_pop_kernel (Stack* stk, void* value, size_t size_el)
{
    int error = stack_verify_kernel (stk, size_el);
    if (error != 0)
    {
        STACK_DUMP(stk, error, size_el);
        return error;
    }

    if (!value)
    {
        error |= NULL_POINTER;
        return error;
    }

    if ((stk->size_st) == 0)
    {
        printf ("Stack is empty. Unable to perform pop\n");
        error |= EMPTY_STACK;
        return error;
    }

    memcpy (value, get_elem_point_kernel (stk, stk->size_st - 1, size_el), size_el);
    memcpy (get_elem_point_kernel (stk, stk->size_st - 1, size_el), &POISON, size_el);
    (stk->size_st)--;

    if (need_realloc (stk))
    {
        error = stack_realloc_kernel (stk, REDUCE, size_el);
        if (error != 0)
        {
            STACK_DUMP(stk, error, size_el);
            return error;
        }
    }
    ON_HASH
    (
        stk->hash_st = get_hash (stk, size_el);
    )
    return error;
}

int stack_verify_kernel (Stack* stk, size_t size_el)
{
    int error = 0;
    if (!(stk && (stk->data) && (stk->name) && (stk->file) && (stk->func)))
    {
        error |= NULL_POINTER;
        return error;
    }
    if ((stk->size_st) < 0)                                                                     error |= NEGATIVE_SIZE;
    if ((stk->capacity) < 0)                                                                    error |= NEGATIVE_CAPACITY;
    if ((stk->capacity) < (stk->size_st))                                                       error |= SIZE_MORE_CAPACITY;
    ON_CANARY
    (
        if ((stk->l_canary) != REF_VAL_CAN)                                                     error |= L_CANARY_ERR;
        if ((stk->r_canary) != REF_VAL_CAN)                                                     error |= R_CANARY_ERR;
        if (*((Canaryt*) (stk->data)) != REF_VAL_CAN)                                           error |= L_CANARY_DATA_ERR;
        if (*((Canaryt*) (get_elem_point_kernel (stk, stk->capacity, size_el))) != REF_VAL_CAN) error |= R_CANARY_DATA_ERR;
    )

    ON_HASH
    (
    Hasht prev_hash = stk->hash_st;
    Hasht new_hash = get_hash (stk, size_el);
    stk->hash_st = new_hash;
    if (prev_hash != new_hash)                                                                  error |= HASH_ERR;
    )
    return error;
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

    ON_CANARY
    (
    if (error & L_CANARY_ERR)
        printf ("Left canary is incorrect\n");
    if (error & R_CANARY_ERR)
        printf ("Right canary is incorrect\n");
    if (error & L_CANARY_DATA_ERR)
        printf ("Left canary in data is incorrect\n");
    if (error & R_CANARY_DATA_ERR)
            printf ("Right canary in data is incorrect\n");
    )

    ON_HASH
    (
    if (error & HASH_ERR)
        printf ("Hash is incorrect\n");
    )

    printf (OFF_COL);
}

void stack_dump_kernel (const Stack* stk, int error, const char* file, const char* func, int line, size_t size_el)
{
    if (!stk)
    {
        printf ("Null pointer of stack\n");
        return;
    }

    printf (RED_COL "Error is in file %s, function %s, line %d in stack %s.\n", stk->file, stk->func, stk->line, stk->name);
    printf ("Called from file %s, function %s, line %d.\n", file, func, line);
    print_error (error);
    printf (RED_COL);

    ON_CANARY
    (
    printf ("Left canary if 0x%llX\n", stk->l_canary);
    printf ("Right canary if 0x%llX\n", stk->r_canary);
    )

    ON_HASH
    (
    printf ("Hash is %llu\n", stk->hash_st);
    )

    printf ("Size     of stack is %d\n"
            "Capacity of stack is %d\n",
            stk->size_st,
            stk->capacity);

    if (!(stk->data))
    {
        printf ("Null pointer of data in stack\n" OFF_COL);
        return;
    }

    ON_CANARY
    (
    printf ("Left canary in data is 0x%llX\n", *((Canaryt*) stk->data));
    printf ("Right canary in data is 0x%llX\n", *((Canaryt*) (get_elem_point_kernel (stk, stk->capacity, size_el))));
    )
}

int stack_realloc_kernel (Stack* stk, enum Actions action, size_t size_el)
{
    int error = 0;
    int capacity_start = stk->capacity;
    void* data_start = stk->data;

    if (action == EXPAND)
        stk->capacity = (stk->capacity) * COEFF_ALLOC;
    else
        stk->capacity = (stk->capacity) / COEFF_ALLOC;

    int new_size = 0;
#ifdef CANARY
    new_size = 2 * sizeof (Canaryt) + stk->capacity * size_el;
#else
    new_size = stk->capacity * size_el;
#endif

    stk->data = realloc (stk->data, new_size);
    if (!(stk->data))
    {
        stk->data = data_start;
        stk->capacity = capacity_start;
        error |= MEM_ALLOC_ERR;
            return error;
    }

    ON_CANARY
    (
    *((Canaryt*) (get_elem_point_kernel (stk, stk->capacity, size_el))) = REF_VAL_CAN;
    )

    for (int i = stk->size_st; i < stk->capacity; i++)
        memcpy (get_elem_point_kernel (stk, i, size_el), &POISON, size_el);

    ON_HASH
    (
    stk->hash_st = get_hash (stk, size_el);
    )

    return error;
}

bool need_realloc (const Stack* stk)
{
    return (((stk->size_st) < ((stk->capacity) / COEFF_ALLOC / COEFF_ALLOC)) && (stk->capacity > 8)) || (stk->size_st == stk->capacity);
}

void* get_elem_point_kernel (const Stack* stk, int num, size_t elem_el)
{
#ifdef CANARY
    return ((char*) (stk->data) + sizeof (Canaryt) + num * elem_el);
#else
    return ((char*) (stk->data) + num * elem_el);
#endif
}

int stack_print_kernel (Stack* stk, size_t size_el)
{
    int error = stack_verify_kernel (stk, size_el);
    if (error != 0)
    {
        STACK_DUMP(stk, error, size_el);
        return error;
    }
    printf ("Size - %d, capacity - %d\n", stk->size_st, stk->capacity);
    return error;
}

ON_HASH
(
    Hasht get_hash (const Stack* stk, size_t size_el)
    {
        Hasht hash_st = 5381;
        for (char* i = (char*) &(stk->size_st); i < ((char*) &(stk->size_st) + sizeof (stk->size_st)); i++)
            hash_st = hash_st * 33 + *i;
        for (char* i = (char*) &(stk->capacity); i < ((char*) &(stk->capacity) + sizeof (stk->capacity)); i++)
            hash_st = hash_st * 33 + *i;
#ifndef CANARY
        for (int i = 0; i < stk->size_st; i++)
            for (char* j = (char*) &(stk->data[i]); j < (char*) (&(stk->data[i]) + size_el); j++)
                hash_st = hash_st * 33 + *j;
#else
        for (char* i = (char*) ((char*) stk->data + sizeof (Canaryt)); i < ((char*) get_elem_point_kernel (stk, stk->size_st, size_el)); i++)
            hash_st = hash_st * 33 + *i;
#endif
        return hash_st;
    }
)
