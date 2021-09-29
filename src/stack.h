/** @file
  Interfejs klasy stosu

  @author Jakub Jagiełła
  @date 2021
*/

#ifndef __STACK_H__
#define __STACK_H__

#include "poly.h"

/**
 * Struktura reprezentująca stos wielomianów.
 */ 
typedef struct Stack {
    size_t size; ///< liczba elementów stosu
    size_t capacity; ///< pojemność stosu
    Poly* arr; ///< tablica zawierająca elementy stosu
} Stack;

/**
 * Tworzy pusty stos wielomianów.
 * @param[in] capacity : pojemność stosu
 * @return wskaźnik na utworzony stos
 */
Stack* StackCreate(size_t capacity);

/**
 * Usuwa stos z pamięci.
 * @param[in, out] stack : stos
 */
void StackDestroy(Stack* stack);

/**
 * Podwaja pojemność stosu.
 * @param[in, out] stack : stos
 */
void StackResize(Stack* stack);

/**
 * Sprawdza, czy stos jest pusty.
 * @param[in] stack : stos
 * @return czy stos jest pusty?
 */
bool StackIsEmpty(const Stack *stack);

/**
 * Sprawdza, czy stos jest pełny.
 * @param[in] stack : stos
 * @return czy stos jest pełny?
 */
bool StackIsFull(const Stack *stack);

/**
 * Dodaje wielomian na wierzch stosu.
 * @param[in, out] stack : stos
 * @param[in] p : wielomian
 */ 
void StackPush(Stack *stack, Poly p);

/**
 * Zwraca wskaźnik na wielomian z wierzchu stosu.
 * Zakłada, że stos jest niepusty.
 * @param[in] stack : stos
 * @return wskaźnik na wielomian z wierzchu stosu
 */
Poly* StackPeek(const Stack *stack);

/**
 * Zwraca wskaźnik na wielomian pod wielomianem z wierzchu stosu.
 * Zakłada, że stos zawiera co najmniej 2 wielomiany.
 * @param[in] stack : stos
 * @return wskaźnik na wielomian pod wielomianem z wierzchu stosu
 */
Poly* StackPeekSecond(const Stack *stack);

/**
 * Usuwa wielomian z wierzchu stosu.
 * Zakłada, że stos jest niepusty.
 * @param[in, out] stack : stos
 */
void StackPop(Stack *stack);

#endif
