#include "stack.h"


int main ()
{
    Stack* stk;
    MAKE_STACK (&stk);
    Elemt num = 0;

    stack_pop (stk, &num);
    stack_push (stk, 1);
    stack_push (stk, 2);
    stack_push (stk, 3);
    stack_push (stk, 4);
    stack_push (stk, 5);
    stack_push (stk, 1);
    stack_push (stk, 2);
    stack_push (stk, 3);
    stack_push (stk, 4);
    stack_push (stk, 5);
    print_stack (stk);
    stack_pop (stk, &num);
    stack_pop (stk, &num);
    stack_pop (stk, &num);
    print_stack (stk);
    *((char*) stk + sizeof (Canaryt)) = 1;
    print_stack (stk);

    delete_stack (&stk);
    print_stack (stk);
    return 0;
}

