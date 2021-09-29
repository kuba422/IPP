/** @file
  Implementacja klasy stosu

  @author Jakub Jagiełła
  @date 2021
*/

#include "check_ptr.h"
#include "stack.h"

#include <stdlib.h>

Stack* StackCreate(size_t capacity) {
    assert(capacity > 0);

    Stack *stack = (Stack*) malloc(sizeof(Stack));
    CHECK_PTR(stack);

    stack->size = 0;
    stack->capacity = capacity;
    stack->arr = (Poly*) malloc(capacity * sizeof(Poly));
    CHECK_PTR(stack->arr);

    return stack;
}

void StackDestroy(Stack* stack) {
    for (size_t i = 0; i < stack->size; i++) {
        PolyDestroy(&stack->arr[i]);
    }

    free(stack->arr);
    free(stack);
}

void StackResize(Stack* stack) {
    stack->capacity *= 2;
    stack->arr = realloc(stack->arr, stack->capacity * sizeof(Poly));
    CHECK_PTR(stack->arr);
}

bool StackIsEmpty(const Stack *stack) {
    return stack->size == 0;
}

bool StackIsFull(const Stack *stack) {
    return stack->size == stack->capacity;
}

void StackPush(Stack *stack, Poly p) {
    if (StackIsFull(stack)) {
        StackResize(stack);
    }

    stack->arr[stack->size] = p;
    stack->size++;
}

Poly* StackPeek(const Stack *stack) {
    assert(!StackIsEmpty(stack));

    return &stack->arr[stack->size - 1];
}

Poly* StackPeekSecond(const Stack *stack) {
    assert(stack->size >= 2);

    return &stack->arr[stack->size - 2];
}

void StackPop(Stack *stack) {
    assert(!StackIsEmpty(stack));

    stack->size--;

    PolyDestroy(&stack->arr[stack->size]);
}
