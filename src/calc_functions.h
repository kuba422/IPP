/** @file
  Interfejs klasy kalkulatora

  @author Jakub Jagiełła
  @date 2021
*/

#ifndef __CALC_FUNCTIONS_H__
#define __CALC_FUNCTIONS_H__

#include "stack.h"

/**
 * Wstawia na wierzchołek stosu wielomian tożsamościowo równy zeru.
 * @param[in, out] stack : stos
 */ 
void CalcZero(Stack *stack);

/**
 * Sprawdza, czy wielomian na wierzchołku stosu jest współczynnikiem.
 * @param[in, out] stack : stos
 * @param[in] line_number : numer wiersza
 */ 
void CalcIsCoeff(const Stack *stack, size_t line_number);

/**
 * Sprawdza, czy wielomian na wierzchołku stosu jest tożsamościowo równy zeru.
 * @param[in, out] stack : stos
 * @param[in] line_number : numer wiersza
 */ 
void CalcIsZero(const Stack *stack, size_t line_number);

/**
 * Wstawia na stos kopię wielomianu z wierzchołka.
 * @param[in, out] stack : stos
 * @param[in] line_number : numer wiersza
 */ 
void CalcClone(Stack *stack, size_t line_number);

/**
 * Dodaje dwa wielomiany z wierzchu stosu, usuwa je i wstawia na wierzchołek stosu ich sumę.
 * @param[in, out] stack : stos
 * @param[in] line_number : numer wiersza
 */ 
void CalcAdd(Stack *stack, size_t line_number);

/**
 * Mnoży dwa wielomiany z wierzchu stosu, usuwa je i wstawia na wierzchołek stosu ich iloczyn.
 * @param[in, out] stack : stos
 * @param[in] line_number : numer wiersza
 */ 
void CalcMul(Stack *stack, size_t line_number);

/**
 * Neguje wielomian na wierzchołku stosu.
 * @param[in, out] stack : stos
 * @param[in] line_number : numer wiersza
 */ 
void CalcNeg(Stack *stack, size_t line_number);

/**
 * Odejmuje od wielomianu z wierzchołka wielomian pod wierzchołkiem, usuwa je i wstawia na wierzchołek stosu różnicę.
 * @param[in, out] stack : stos
 * @param[in] line_number : numer wiersza
 */ 
void CalcSub(Stack *stack, size_t line_number);

/**
 * Sprawdza, czy dwa wielomiany na wierzchu stosu są równe.
 * @param[in] stack : stos
 * @param[in] line_number : numer wiersza
 */ 
void CalcIsEq(const Stack *stack, size_t line_number);

/**
 * Wypisuje na standardowe wyjście stopień wielomianu.
 * @param[in] stack : stos
 * @param[in] line_number : numer wiersza
 */ 
void CalcDeg(const Stack *stack, size_t line_number);

/**
 * Wypisuje na standardowe wyjście stopień wielomianu ze względu na zmienną o numerze @p idx.
 * @param[in] stack : stos
 * @param[in] idx : number zmiennej
 * @param[in] line_number : numer wiersza
 */ 
void CalcDegBy(const Stack *stack, unsigned long long idx, size_t line_number);

/**
 * Wylicza wartość wielomianu w punkcie @p x, usuwa wielomian z wierzchołka i wstawia na stos wynik operacji.
 * @param[in, out] stack : stos
 * @param[in] x : punkt, w którym chcemy wyliczyć wartość
 * @param[in] line_number : numer wiersza
 */ 
void CalcAt(Stack *stack, long long x, size_t line_number);

/**
 * Wypisuje na standardowe wyjście złożenie wielomianu z wierzchu stosu z k kolejnymi wielomianami.
 * @param[in, out] stack : stos
 * @param[in] k : liczb składanych wielomianów
 * @param[in] line_number : number wiersza
 */ 
void CalcCompose(Stack *stack, unsigned long long k, size_t line_number);

/**
 * Wypisuje na standardowe wyjście wielomian z wierzchołka stosu.
 * @param[in] stack : stos
 * @param[in] line_number : numer wiersza
 */ 
void CalcPrint(const Stack *stack, size_t line_number);

/**
 * Usuwa wielomian z wierzchołka stosu.
 * @param[in, out] stack : stos
 * @param[in] line_number : numer wiersza
 */ 
void CalcPop(Stack *stack, size_t line_number);

/**
 * Wykonuje odpowiednią komendę.
 * @param[in, out] stack : stos
 * @param[in] line_number : numer wiersza
 * @param[in] command : symbol komendy
 */ 
void CalcExecute(Stack *stack, size_t line_number, int command);

/**
 * Uruchamia kalkulator.
 */ 
void CalcRun();

#endif