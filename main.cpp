#include "stack.h"

#define STACK_CTOR(stk) \
        stack_ctor ((stk), #stk, __FILE__, __func__, __LINE__)

int main ()
{
    Stack* stk;
    make_stack (&stk);
    Elemt num = 0;
    STACK_CTOR(stk);
    print_stack (stk);
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
    stack_pop (stk, &num);
    stack_pop (stk, &num);
    stack_pop (stk, &num);
    stack_pop (stk, &num);
    stack_pop (stk, &num);
    stack_pop (stk, &num);
    print_stack (stk);
    //*(int*)((char*)stk + sizeof (Canaryt)) = 10;
    stack_push (stk, 6);
    return 0;
}

