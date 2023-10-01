#include "stack.h"

STACK_TEMPLATE(int)
STACK_TEMPLATE(double)

int main ()
{
    Stack_int a = {};
    Stack_double b = {};

    STACK_CTOR(&a);
    STACK_CTOR(&b);

    STACK_PUSH(&a, 1);
    STACK_PUSH(&a, 2);
    STACK_PUSH(&a, 3);
    STACK_PUSH(&a, 4);
    STACK_PUSH(&a, 5);
    STACK_PUSH(&a, 6);
    STACK_PUSH(&a, 7);
    STACK_PUSH(&a, 8);
    STACK_PUSH(&a, 9);
    STACK_PUSH(&a, 10);

    STACK_PUSH(&b, 5.555);
    STACK_PUSH(&b, 10.11);

    STACK_PRINT(&a);
    STACK_PRINT(&b);

    printf ("Size - %d, capacity - %d\n", a.size_st, a.capacity);
    for (int i = 0; i < a.size_st; i++)
        printf ("%d ", (a.data)[i]);
    printf ("\n");

    printf ("Size - %d, capacity - %d\n", b.size_st, b.capacity);
    for (int i = 0; i < b.size_st; i++)
        printf ("%lf ", (b.data)[i]);
    printf ("\n");

    int num = 0;
    STACK_POP(&a, &num);
    STACK_POP(&a, &num);
    STACK_POP(&a, &num);
    STACK_POP(&a, &num);
    printf ("Size - %d, capacity - %d\n", a.size_st, a.capacity);
    for (int i = 0; i < a.size_st; i++)
        printf ("%d ", (a.data)[i]);
    printf ("\n");
    STACK_POP(&a, &num);
    STACK_POP(&a, &num);
    STACK_POP(&a, &num);
    printf ("Size - %d, capacity - %d\n", a.size_st, a.capacity);
    for (int i = 0; i < a.size_st; i++)
        printf ("%d ", (a.data)[i]);
    printf ("\n");

    STACK_DTOR(&a);
}
