/** @file
  Interfejs klasy kalkulatora

  @author Jakub Jagiełła
  @date 2021
*/

#include "check_ptr.h"
#include "calc_functions.h"
#include "parser.h"

#include <stdlib.h>
#include <errno.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>

/**
 * Domyślny rozmiar tablicy.
 */ 
#define DEFAULT_SIZE 32

/**
 * Maksymalna liczba znaków potrzebna do zapisania liczby z dozwolonego zakresu.
 */ 
#define MAX_NUMBER_LENGTH 24

/**
 * Łączy dwa napisy w jeden. 
 * W razie potrzeby realokuje @p s1, żeby nie zabrakło pamięci.
 * @param[in] s1 : pierwszy string
 * @param[in] s2 : drugi string
 * @param[in] capacity : wskaźnik na zmienną zawierającą ilość zaalokowanej pamięci pod @p s1
 * @param[in] size1 : długość pierwszego string
 * @param[in] size2 : długość drugiego stringa
 * @return konkatenacja @p s1 z @p s2
 */ 
static char* SafeStrcat(char *s1, char *s2, size_t *capacity, size_t size1, size_t size2) {
    if (size1 + size2 >= *capacity) {
        while (size1 + size2 >= *capacity) {
            *capacity *= 2;
        }

        s1 = realloc(s1, *capacity * sizeof(char));
        CHECK_PTR(s1);
    }

    return strcat(s1, s2);
}

/**
 * Konwertuje wielomian na stringa, zapisuje jego długość.
 * @param[in] p : wielomian
 * @param[out] size : wskaźnik na zmienną, w której zostanie zapisana długość stringa
 * @return string reprezentujący wielomian
 */ 
static char* CalcToString(const Poly *p, size_t *size) {
    if (PolyIsCoeff(p)) {
        char *s = malloc(MAX_NUMBER_LENGTH * sizeof(char));
        CHECK_PTR(s);

        *size = sprintf(s, "%ld", p->coeff);

        return s;
    }
    else {
        size_t capacity = DEFAULT_SIZE;
        size_t size1 = 1;
        size_t size2, size3;

        char *s = malloc(capacity * sizeof(char));
        CHECK_PTR(s);
        s[0] = '\0';

        char *exp = malloc(MAX_NUMBER_LENGTH * sizeof(char));
        CHECK_PTR(exp);

        char *q = CalcToString(&(p->arr[0].p), &size2);

        size3 = sprintf(exp, "%d", p->arr[0].exp);

        s = SafeStrcat(s, "(", &capacity, size1, 1);
        size1++;
        s = SafeStrcat(s, q, &capacity, size1, size2);
        size1 += size2;
        s = SafeStrcat(s, ",", &capacity, size1, 1);
        size1++;
        s = SafeStrcat(s, exp, &capacity, size1, size3);
        size1 += size3;
        s = SafeStrcat(s, ")", &capacity, size1, 1);
        size1++;

        free(exp);
        free(q);

        for (size_t i = 1; i < p->size; i++) {
            char *exp = malloc(MAX_NUMBER_LENGTH * sizeof(char));
            CHECK_PTR(exp);
            
            char *q = CalcToString(&(p->arr[i].p), &size2);

            size3 = sprintf(exp, "%d", p->arr[i].exp);

            s = SafeStrcat(s, "+(", &capacity, size1, 2);
            size1 += 2;
            s = SafeStrcat(s, q, &capacity, size1, size2);
            size1 += size2;
            s = SafeStrcat(s, ",", &capacity, size1, 1);
            size1++;
            s = SafeStrcat(s, exp, &capacity, size1, size3);
            size1 += size3;
            s = SafeStrcat(s, ")", &capacity, size1, 1);
            size1++;

            free(exp);
            free(q);
        }

        *size = size1;

        return s;
    }
}

void CalcZero(Stack *stack) {
    StackPush(stack, PolyZero());
}

void CalcIsCoeff(const Stack *stack, size_t line_number) {
    if (StackIsEmpty(stack)) {
        fprintf(stderr, "ERROR %ld STACK UNDERFLOW\n", line_number);

        return;
    }
    
    printf("%d\n", PolyIsCoeff(StackPeek(stack)) ? 1 : 0);
}

void CalcIsZero(const Stack *stack, size_t line_number) {
    if (StackIsEmpty(stack)) {
        fprintf(stderr, "ERROR %ld STACK UNDERFLOW\n", line_number);

        return;
    }

    printf("%d\n", PolyIsZero(StackPeek(stack)) ? 1 : 0);
}

void CalcClone(Stack *stack, size_t line_number) {
    if (StackIsEmpty(stack)) {
        fprintf(stderr, "ERROR %ld STACK UNDERFLOW\n", line_number);

        return;
    }
    
    StackPush(stack, PolyClone(StackPeek(stack)));
}

void CalcAdd(Stack *stack, size_t line_number) {
    if (stack->size < 2) {
        fprintf(stderr, "ERROR %ld STACK UNDERFLOW\n", line_number);

        return;
    }

    Poly *p1 = StackPeek(stack);
    Poly *p2 = StackPeekSecond(stack);

    Poly r = PolyAdd(p1, p2);

    StackPop(stack);
    StackPop(stack);

    StackPush(stack, r);
}

void CalcMul(Stack *stack, size_t line_number) {
    if (stack->size < 2) {
        fprintf(stderr, "ERROR %ld STACK UNDERFLOW\n", line_number);

        return;
    }

    Poly *p1 = StackPeek(stack);
    Poly *p2 = StackPeekSecond(stack);

    Poly r = PolyMul(p1, p2);

    StackPop(stack);
    StackPop(stack);

    StackPush(stack, r);
}

void CalcNeg(Stack *stack, size_t line_number) {
    if (StackIsEmpty(stack)) {
        fprintf(stderr, "ERROR %ld STACK UNDERFLOW\n", line_number);

        return;
    }

    Poly p = PolyNeg(StackPeek(stack));

    StackPop(stack);

    StackPush(stack, p);
}

void CalcSub(Stack *stack, size_t line_number) {
    if (stack->size < 2) {
        fprintf(stderr, "ERROR %ld STACK UNDERFLOW\n", line_number);

        return;
    }

    Poly *p1 = StackPeek(stack);
    Poly *p2 = StackPeekSecond(stack);

    Poly r = PolySub(p1, p2);

    StackPop(stack);
    StackPop(stack);

    StackPush(stack, r);
}

void CalcIsEq(const Stack *stack, size_t line_number) {
    if (stack->size < 2) {
        fprintf(stderr, "ERROR %ld STACK UNDERFLOW\n", line_number);

        return;
    }
    
    printf("%d\n", PolyIsEq(StackPeek(stack), StackPeekSecond(stack)) ? 1 : 0);
}

void CalcDeg(const Stack *stack, size_t line_number) {
    if (StackIsEmpty(stack)) {
        fprintf(stderr, "ERROR %ld STACK UNDERFLOW\n", line_number);

        return;
    }

    printf("%d\n", PolyDeg(StackPeek(stack)));
}

void CalcDegBy(const Stack *stack, unsigned long long idx, size_t line_number) {
    if (StackIsEmpty(stack)) {
        fprintf(stderr, "ERROR %ld STACK UNDERFLOW\n", line_number);

        return;
    }

    printf("%d\n", PolyDegBy(StackPeek(stack), idx));
}

void CalcAt(Stack *stack, long long x, size_t line_number) {
    if (StackIsEmpty(stack)) {
        fprintf(stderr, "ERROR %ld STACK UNDERFLOW\n", line_number);

        return;
    }

    Poly p = PolyAt(StackPeek(stack), x);

    StackPop(stack);

    StackPush(stack, p);
}

void CalcCompose(Stack *stack, unsigned long long k, size_t line_number) {
    if (stack->size <= k) {
        fprintf(stderr, "ERROR %ld STACK UNDERFLOW\n", line_number);

        return;
    }

    Poly p = PolyClone(StackPeek(stack));
    StackPop(stack);

    Poly *q = malloc(k * sizeof(Poly));
    CHECK_PTR(q);
    for (size_t i = 0; i < k; i++) {
        q[k - i - 1] = PolyClone(StackPeek(stack));

        StackPop(stack);
    }

    StackPush(stack, PolyCompose(&p, k, q));

    PolyDestroy(&p);
    for (size_t i = 0; i < k; i++) {
        PolyDestroy(&(q[i]));
    }
    free(q);
}

void CalcPrint(const Stack *stack, size_t line_number) {
    if (StackIsEmpty(stack)) {
        fprintf(stderr, "ERROR %ld STACK UNDERFLOW\n", line_number);

        return;
    }

    size_t size = 0;

    char *s = CalcToString(StackPeek(stack), &size);

    printf("%s\n", s);

    free(s);
}

void CalcPop(Stack *stack, size_t line_number) {
    if (StackIsEmpty(stack)) {
        fprintf(stderr, "ERROR %ld STACK UNDERFLOW\n", line_number);

        return;
    }

    StackPop(stack);
}

void CalcExecute(Stack *stack, size_t line_number, int command) {
    if (command == ZERO) {
        CalcZero(stack);
    }
    else if (command == IS_COEFF) {
        CalcIsCoeff(stack, line_number);
    }
    else if (command == IS_ZERO) {
        CalcIsZero(stack, line_number);
    }
    else if (command == CLONE) {
        CalcClone(stack, line_number);
    }
    else if (command == ADD) {
        CalcAdd(stack, line_number);
    }
    else if (command == MUL) {
        CalcMul(stack, line_number);
    }
    else if (command == NEG) {
        CalcNeg(stack, line_number);
    }
    else if (command == SUB) {
        CalcSub(stack, line_number);
    }
    else if (command == IS_EQ) {
        CalcIsEq(stack, line_number);
    }
    else if (command == DEG) {
        CalcDeg(stack, line_number);
    }
    else if (command == PRINT) {
        CalcPrint(stack, line_number);
    }
    else if (command == POP) {
        CalcPop(stack, line_number);
    }
}

void CalcRun() {
    Stack* stack = StackCreate(DEFAULT_SIZE);
    ssize_t line_length;
    size_t line_number = 0;

    char *line = NULL;
    size_t n = 0;
    errno = 0;

    while ((line_length = getline(&line, &n, stdin)) != -1) {
        line_number++;

        if (isalpha(line[0])) {
            int command = ParserCommand(line, line_length, line_number);

            if (command == AT) {
                char *endptr = NULL;

                CalcAt(stack, strtoll(line + 3, &endptr, 10), line_number);
            }
            else if (command == DEG_BY) {
                char *endptr = NULL;

                CalcDegBy(stack, strtoull(line + 7, &endptr, 10), line_number);
            }
            else if (command == COMPOSE) {
                char *endptr = NULL;

                CalcCompose(stack, strtoull(line + 8, &endptr, 10), line_number);
            }
            else {
                CalcExecute(stack, line_number, command);
            }
        }
        else if (!(line[0] == '#') && !(line[0] == '\n')) {
            bool valid = true;

            Poly p = ParserPoly(line, line_length, line_number, &valid);

            if (valid) {
                StackPush(stack, p);
            }
        }

        errno = 0;
    }

    if (errno) {
        exit(1);
    }

    free(line);
    StackDestroy(stack);
}