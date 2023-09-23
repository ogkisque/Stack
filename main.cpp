#include "stack.h"

#define STACK_CTOR(stk) \
        stack_ctor ((stk), #stk, __FILE__, __func__, __LINE__);

int main ()
{
    Stack stk = {};
    Elemt num = 0;
    STACK_CTOR(&stk);
    print_stack (&stk);
    stack_pop (&stk, &num);
    printf ("%d\n", num);
    print_stack (&stk);

    stack_push (&stk, 10);
    printf ("AAAA\n");
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
    STACK_CTOR(&stk1);
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

    printf ("------------\n");

    Stack stk2 = {};
    STACK_CTOR(&stk2);
    stack_push (&stk2, 1);
    stack_push (&stk2, 2);
    stack_push (&stk2, 3);
    print_stack (&stk2);
    stk2.r_canary = 200;
    stack_push (&stk2, 4);
    stack_push (&stk2, 5);

    printf ("------------\n");

    Stack stk3 = {};
    STACK_CTOR(&stk3);
    stack_push (&stk3, 1);
    stack_push (&stk3, 2);
    stack_push (&stk3, 3);
    print_stack (&stk3);
    stk3.size_st = 2;
    print_stack (&stk3);
    stk3.size_st = 3;
    ((Elemt*)((char*) stk3.data + sizeof (Canaryt)))[0] = 6;
    stack_push (&stk3, 4);
    return 0;
}

