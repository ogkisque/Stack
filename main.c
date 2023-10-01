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

    int num = 0;
    STACK_POP(&a, &num);
    STACK_POP(&a, &num);
    STACK_POP(&a, &num);
    STACK_POP(&a, &num);
    STACK_PRINT(&a);

    STACK_POP(&a, &num);
    STACK_POP(&a, &num);
    STACK_POP(&a, &num);
    STACK_PRINT(&a);

    STACK_DTOR(&a);
    STACK_DTOR(&b);
}
