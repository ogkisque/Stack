#include "stack.h"

STACK_TEMPLATE(int)
STACK_TEMPLATE(double)

int main ()
{
    Stack_int stk_int = {};
    Stack_double stk_double = {};

    int num1 = 0;
    double num2 = 0.0;

    STACK_CTOR(&stk_int);
    STACK_CTOR(&stk_double);

    STACK_PUSH(&stk_int, 1);
    STACK_PUSH(&stk_int, 2);
    STACK_PUSH(&stk_int, 3);
    STACK_PUSH(&stk_int, 4);
    STACK_PUSH(&stk_int, 5);
    STACK_PRINT(&stk_int);
    STACK_POP(&stk_int, &num1);
    STACK_PRINT(&stk_int);

    STACK_PUSH(&stk_double, 1.5);
    STACK_PUSH(&stk_double, 2.6);
    STACK_PUSH(&stk_double, 69.69);
    STACK_PRINT(&stk_double);
    STACK_POP(&stk_double, &num2);
    STACK_PRINT(&stk_double);

    STACK_DTOR(&stk_int);
    STACK_DTOR(&stk_double);

    Stack_int stk_int1 = {};
    Stack_double stk_double1 = {};

    STACK_CTOR(&stk_int1);
    STACK_CTOR(&stk_double1);

    STACK_POP(&stk_int1, &num1);

}
