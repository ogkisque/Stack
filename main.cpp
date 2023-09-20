#include "stack.h"

int main ()
{
    Stack stk = {};
    Elemt num = 0;
    stack_ctor (&stk);
    print_stack (&stk);
    stack_pop (&stk, &num);
    printf ("%d\n", num);
    print_stack (&stk);

    stack_push (&stk, 10);
    print_stack (&stk);
    stack_push (&stk, 20);
    print_stack (&stk);
    stack_push (&stk, 30);
    print_stack (&stk);
    stack_push (&stk, 40);
    print_stack (&stk);
    stack_push (&stk, 50);
    print_stack (&stk);
    stack_push (&stk, 60);
    print_stack (&stk);

    stack_pop (&stk, &num);
    printf ("%d\n", num);
    print_stack (&stk);
    stack_pop (&stk, &num);
    printf ("%d\n", num);
    print_stack (&stk);
    stack_pop (&stk, &num);
    printf ("%d\n", num);
    print_stack (&stk);
    stack_pop (&stk, &num);
    printf ("%d\n", num);
    print_stack (&stk);

    stack_dtor (&stk);
    print_stack (&stk);

    printf ("------------\n");

    Stack stk1 = {};
    stk1.data = NULL;
    print_stack (&stk1);
    stack_ctor (&stk1);
    print_stack (&stk1);
    stack_push (&stk1, 1);
    stack_push (&stk1, 2);
    stack_push (&stk1, 3);
    stack_push (&stk1, 4);
    stack_push (&stk1, 5);
    print_stack (&stk1);
    stk1.capacity = 3;
    stack_push (&stk1, 6);
    stk1.capacity = 8;
    stk1.size_st = -5;
    stack_pop (&stk1, &num);
    return 0;
}

