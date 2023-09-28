#include "stack.h"


int main ()
{
    Stack* stk;
    MAKE_STACK (&stk);
    stack_push (stk, 1);
    stack_push (stk, 1);
    stack_push (stk, 1);
    stack_push (stk, 1);
    stack_push (stk, 1);
    stack_push (stk, 1);
    stack_push (stk, 1);
    stack_push (stk, 1);
    stack_push (stk, 1);
    stack_push (stk, 1);
    print_stack (stk);

    int num;
    stack_pop (stk, &num);
    stack_pop (stk, &num);
    print_stack (stk);
    stack_pop (stk, &num);
    stack_pop (stk, &num);
    stack_pop (stk, &num);
    stack_pop (stk, &num);
    stack_pop (stk, &num);
    print_stack (stk);

    delete_stack (&stk);
    print_stack (stk);
    return 0;
}
