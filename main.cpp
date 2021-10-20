#include <stdio.h>
#include <stdlib.h>

#include "mem_copy.h"
#include "hash.h"
#include "stack.h"

#define CODE_CHECK(FUNC, OK_CODE)     \
{                                     \
    Errors CODE = FUNC;               \
    if (CODE != OK_CODE) return CODE; \
}

void mem_copy_test() {

    puts("=====Memory=copying=test=====");

    char a = 'a';
    char b = 'b';

    printf("Before: %c %c\n", a, b);
    mem_copy(&a, &b, sizeof(char));
    printf("After:  %c %c\n\n", a, b);

    int c = 4, d = -8;

    printf("Before: %d %d\n", c, d);
    mem_copy(&c, &d, sizeof(int));
    printf("After:  %d %d\n\n", c, d);

    double aa = 1.7, bb = 22.2;
    
    printf("Before: %lg %lg\n", aa, bb);
    mem_copy(&aa, &bb, sizeof(double));
    printf("After:  %lg %lg\n\n", aa, bb);
}

Errors first_stack_test() {

    puts("======Stack=testing======\n");
    Stack stk = {};

    StackCtor_(stk, int);

    int value;

    CODE_CHECK(StackPush(&stk, &(value = 0)), STACK_OK);
    CODE_CHECK(StackPush(&stk, &(value = 1)), STACK_OK);
    CODE_CHECK(StackPush(&stk, &(value = 2)), STACK_OK);

    CODE_CHECK(StackPush(&stk, &(value = 3)), STACK_OK);
    CODE_CHECK(StackPush(&stk, &(value = 4)), STACK_OK);
    CODE_CHECK(StackPush(&stk, &(value = 5)), STACK_OK);

    /* stk.size = 10; */

    value = -1;

    CODE_CHECK(StackPop(&stk, &value), STACK_OK);
    printf("Element: %d\n", value);

    CODE_CHECK(StackPop(&stk, &value), STACK_OK);
    printf("Element: %d\n", value);

    CODE_CHECK(StackPop(&stk, &value), STACK_OK);
    printf("Element: %d\n", value);

    CODE_CHECK(StackPop(&stk, &value), STACK_OK);
    printf("Element: %d\n", value);

    CODE_CHECK(StackPop(&stk, &value), STACK_OK);
    printf("Element: %d\n", value);

    CODE_CHECK(StackPop(&stk, &value), STACK_OK);
    printf("Element: %d\n", value);

    CODE_CHECK(StackDtor(&stk), STACK_OK);
    return STACK_OK;
}

Errors second_stack_test() {

    Stack s1 = {};
    Stack s2 = {};

    StackCtor_(s1, int);
    StackCtor_(s2, double);

    int v1;
    double v2;
    CODE_CHECK(StackPush(&s1, &(v1 = 0)), STACK_OK);
    CODE_CHECK(StackPush(&s2, &(v2 = 1.7)), STACK_OK);

    CODE_CHECK(StackPush(&s1, &(v1 = 1)), STACK_OK);
    CODE_CHECK(StackPush(&s2, &(v2 = 22.7)), STACK_OK);

    puts("Firts <int> stack:");

    CODE_CHECK(StackPop(&s1, &v1), STACK_OK);
    printf("Element: %d\n", v1);

    CODE_CHECK(StackPop(&s1, &v1), STACK_OK);
    printf("Element: %d\n", v1);

    puts("Second <double> stack:");

    CODE_CHECK(StackPop(&s2, &v2), STACK_OK);
    printf("Element: %lg\n", v2);

    CODE_CHECK(StackPop(&s2, &v2), STACK_OK);
    printf("Element: %lg\n", v2);

    CODE_CHECK(StackDtor(&s1), STACK_OK);
    CODE_CHECK(StackDtor(&s2), STACK_OK);
    return STACK_OK;
}

void hash_algo_test() {

    int data[4] = {1, 2, 3, 4};

    size_t h1 = HashFunc(data, sizeof(int) * 4);
    size_t h2 = HashFunc(data, sizeof(int) * 4);

    printf("First and second hashes of data are: [%lX] [%lX]\n", h1, h2);

    int* carr = (int*)calloc(5, sizeof(int));
    carr[0] = carr[1] = carr[2] = carr[3] = carr[4] = 5;

    size_t h3 = HashFunc(carr, sizeof(int) * 5);
    size_t h4 = HashFunc(carr, sizeof(int) * 5);

    printf("First and second hashes of carr are: [%lX] [%lX]\n", h3, h4);

    free(carr);

}

int main() {

    mem_copy_test();

    puts("================================");
    printf("Result of first stack test: %d\n", first_stack_test());
    puts("================================");

    puts("================================");
    printf("Result of second stack test: %d\n", second_stack_test());
    puts("================================");
    
    puts("================================");
    puts("Result of hash algo test:");
    hash_algo_test();
    puts("================================");

    return 0;
}
