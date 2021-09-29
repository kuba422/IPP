/** @file
  Interfejs klasy parsera

  @author Jakub Jagiełła
  @date 2021
*/

#ifndef __PARSER_H__
#define __PARSER_H__

#include "poly.h"

/**
 * Symbole komend kalkulatora.
 */ 
enum commands {
    ZERO = 1,
    IS_COEFF = 2,
    IS_ZERO = 3,
    CLONE = 4,
    ADD = 5,
    MUL = 6,
    NEG = 7,
    SUB = 8,
    IS_EQ = 9,
    DEG = 10,
    DEG_BY = 11,
    AT = 12,
    PRINT = 13,
    POP = 14,
    COMPOSE = 15
};

/**
 * Rozpoznaje, jaką komendę zawiera wiersz.
 * Jeżeli wiersz nie zawiera poprawnej komendy, wypisuje odpowiedni komunikat na stderr.
 * Zakłada, że wiersz pierwszy znak wiersza jest literą alfabetu angielskiego.
 * @param[in] line : wiersz
 * @param[in] line_length : długość wiersza
 * @param[in] line_number : number wiersza
 * @return -1, jeżeli wiersz nie zawierał poprawnej komendy
 * @return symbol odpowiedniej komendy, jeżeli wiersza zawierał poprawną komendę
 */ 
int ParserCommand(const char *line, size_t line_length, size_t line_number);

/**
 * Rozpoznaje, czy wiersz zawiera poprawny wielomian.
 * Jeżeli nie jest, ustawia wartość @p *valid na false.
 * Zakłada, że pierwszy znak wiersza nie jest literą alfabetu angielskiego.
 * @param[in] line : wiersz
 * @param[in] line_length : długość wiersza
 * @param[in] line_number : number wiersza
 * @param[in, out] valid : wskaźnik na zmienną, która przechwuje informację, czy parsowany wielomian jest poprawny
 * @return PolyZero(), jeżeli wiersz nie zawierał poprawnej komendy
 * @return odpowiedni wielomian, jeżeli wiersza zawierał poprawny wielomian
 */ 
Poly ParserPoly(const char *line, size_t line_length, size_t line_number, bool *valid);

#endif