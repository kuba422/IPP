/** @file
  Implementacja klasy parsera

  @author Jakub Jagiełła
  @date 2021
*/

#include "check_ptr.h"
#include "parser.h"
#include "poly.h"
#include "stack.h"
#include "limits.h"

#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>

/**
 * Symbol ASCII znaku "tab".
 */ 
#define TAB 9

/**
 * Sprawdza, czy znak jest cyfrą.
 */ 
#define DIGIT(x) ((x >= '0') && (x <= '9'))

/**
 * Rozpoznaje, czy wiersza zawiera poprawną komendę AT.
 * Jeżeli wiersz nie zawiera poprawnej komendy, wypisuje odpowiedni komunikat na stderr.
 * Zakłada, że pierwsze 2 znaki wiersza to "AT".
 * @param[in] line : wiersz
 * @param[in] line_length : długość wiersza
 * @param[in] line_number : number wiersza
 * @return -1, jeżeli wiersz nie zawierał poprawnej komendy
 * @return symbol AT, jeżeli wiersza zawierał poprawną komendę
 */ 
static int ParserCommandAt(const char *line, size_t line_length, size_t line_number) {
    if ((line_length == 2) || (line_length == 3  && (line[2] == ' ' || line[2] == '\n')) 
        || (line_length >= 3 && line[2] == TAB)) {
        fprintf(stderr, "ERROR %ld AT WRONG VALUE\n", line_number);

        return -1;
    }
    else if (line_length >= 4 && line[2] == ' ') {
        bool valid_line = true;

        valid_line &= DIGIT(line[3]) || (line[3] == '-');
        for (size_t i = 4; i < line_length - 1; i++) {
            valid_line &= DIGIT(line[i]);
        }
        valid_line &= DIGIT(line[line_length - 1]) || (line[line_length - 1] == '\n');
        valid_line &= !((line_length == 4) && (line[3] == '-'));
        valid_line &= !((line_length == 5) && (line[3] == '-') && (line[4] == '\n'));

        if (!valid_line) {
            fprintf(stderr, "ERROR %ld AT WRONG VALUE\n", line_number);
            
            return -1;
        }
        else {
            char *end = NULL;
            errno = 0;

            strtoll(line + 3, &end, 10);

            if (!errno) {
                return AT;
            }
            else {
                fprintf(stderr, "ERROR %ld AT WRONG VALUE\n", line_number);
                
                return -1;
            }
        }
    }
    else {
        fprintf(stderr, "ERROR %ld WRONG COMMAND\n", line_number);    
        
        return -1;
    }
}

/**
 * Rozpoznaje, czy wiersza zawiera poprawną komendę DEG_BY.
 * Jeżeli wiersz nie zawiera poprawnej komendy, wypisuje odpowiedni komunikat na stderr.
 * Zakłada, że pierwsze 6 znaków wiersza to "DEG_BY".
 * @param[in] line : wiersz
 * @param[in] line_length : długość wiersza
 * @param[in] line_number : number wiersza
 * @return -1, jeżeli wiersz nie zawierał poprawnej komendy
 * @return symbol DEG_BY, jeżeli wiersza zawierał poprawną komendę
 */ 
static int ParserCommandDegBy(const char *line, size_t line_length, size_t line_number) {
    if ((line_length == 6) || (line_length == 7 && (line[6] == ' ' || line[6] == '\n')) 
        || (line_length >= 7 && line[6] == TAB)) {
        fprintf(stderr, "ERROR %ld DEG BY WRONG VARIABLE\n", line_number);

        return -1;    
    }
    else if (line_length >= 8 && line[6] == ' ') {
        bool valid_line = true;

        valid_line &= !(line[7] == '\n');
        for (size_t i = 7; i < line_length - 1; i++) {
            valid_line &= DIGIT(line[i]);
        }
        valid_line &= DIGIT(line[line_length - 1]) || (line[line_length - 1] == '\n');

        if (!valid_line) {
            
            fprintf(stderr, "ERROR %ld DEG BY WRONG VARIABLE\n", line_number);
            return -1;
        }
        else {
            char *end = NULL;
            errno = 0;

            strtoull(line + 7, &end, 10);

            if (!errno) {
                return DEG_BY;
            }
            else {
                fprintf(stderr, "ERROR %ld DEG BY WRONG VARIABLE\n", line_number);
                
                return -1;
            }
        }
    }
    else {
        fprintf(stderr, "ERROR %ld WRONG COMMAND\n", line_number);
        
        return -1;
    }
}

/**
 * Rozpoznaje, czy wiersza zawiera poprawną komendę COMPOSE.
 * Jeżeli wiersz nie zawiera poprawnej komendy, wypisuje odpowiedni komunikat na stderr.
 * Zakłada, że pierwsze 7 znaków wiersza to "COMPOSE".
 * @param[in] line : wiersz
 * @param[in] line_length : długość wiersza
 * @param[in] line_number : number wiersza
 * @return -1, jeżeli wiersz nie zawierał poprawnej komendy
 * @return symbol COMPOSE, jeżeli wiersza zawierał poprawną komendę
 */ 
static int ParserCommandCompose(const char *line, size_t line_length, size_t line_number) {
    if ((line_length == 7) || (line_length == 8 && (line[7] == ' ' || line[7] == '\n')) 
        || (line_length >= 8 && line[7] == TAB)) {
        fprintf(stderr, "ERROR %ld COMPOSE WRONG PARAMETER\n", line_number);

        return -1;    
    }
    else if (line_length >= 9 && line[7] == ' ') {
        bool valid_line = true;

        valid_line &= !(line[8] == '\n');
        for (size_t i = 8; i < line_length - 1; i++) {
            valid_line &= DIGIT(line[i]);
        }
        valid_line &= DIGIT(line[line_length - 1]) || (line[line_length - 1] == '\n');

        if (!valid_line) {
            
            fprintf(stderr, "ERROR %ld COMPOSE WRONG PARAMETER\n", line_number);
            return -1;
        }
        else {
            char *end = NULL;
            errno = 0;

            strtoull(line + 8, &end, 10);

            if (!errno) {
                return COMPOSE;
            }
            else {
                fprintf(stderr, "ERROR %ld COMPOSE WRONG PARAMETER\n", line_number);
                
                return -1;
            }
        }
    }
    else {
        fprintf(stderr, "ERROR %ld WRONG COMMAND\n", line_number);
        
        return -1;
    }
}

/**
 * Sprawdza, czy wiersz zawiera poprawne wyrażenie nawiasowe.
 * Sprawdza, czy wiersz nie zawiera jawnie błędnych podciągów.
 * @param[in] line : wiersz
 * @param[in] line_length : długość wiersza
 * @return czy wykryto błędną sekwencję?
 */ 
static bool ParserValidBrackets(const char *line, size_t line_length) {
    size_t left = 0;
    size_t right = 0;

    bool valid = true;
    char prev, next;
    for (size_t i = 1; i < line_length; i++) {
        prev = line[i - 1];
        next = line[i];

        if (prev == '(') {
            left++;

            valid &= (DIGIT(next) || next == '(' || next == '-');
        }
        else if (prev == ')') {
            right++;

            valid &= (left >= right);
            valid &= (next == '+' || next == ',');
        }
        else if (DIGIT(prev)) {
            valid &= (DIGIT(next) || next == ',' || next == ')');
        }
        else if (prev == '-' || prev == ',') {
            valid &= DIGIT(next);
        }
        else if (prev == '+') {
            valid &= (next == '(');
        }
        else {
            valid = false;
        }

        if (!valid) {
            break;
        }
    }

    valid &= (left == right + 1 && line[line_length - 1] == ')');

    return valid;
}

/**
 * Sprawdza, czy wiersz zawiera poprawną liczbę z odpowiedniego zakresu.
 * Zakładamy, że wiersz zaczyna się od cyfry bądź znaku "minus".
 * @param[in] line : wiersz
 * @param[in] line_length : długość wiersza
 * @return czy wiersz zawiera poprawną liczbę?
 */ 
static bool ParserValidConst(const char *line, size_t line_length) {
    bool valid = true;

    for (size_t i = 1; i < line_length; i++) {
        valid &= DIGIT(line[i]);

        if (!valid) {
            break;
        }
    }

    if (valid) {
        char *end = NULL;
        errno = 0;

        strtoll(line, &end, 10);

        if (errno) {
            valid = false;
        }
    }

    return valid;
}

static Poly ParserPolyAux(const char *line, size_t line_length, size_t line_number, bool *valid, bool write);

/**
 * Sprawdza, czy wiersz zawiera poprawny wielomian.
 * Zakładamy, że wiersz jest postaci "(...)".
 * @param[in] line : wiersz
 * @param[in] line_length : długość wiersza
 * @param[in] line_number : numer wiersza
 * @param[in, out] valid : wskaźnik na zmienną przechowującą informacje, czy wiersz był poprawny
 * @param[in] write : czy wypisywać informacje o błędach?
 * @return wielomian z wiersza, jeżeli wiersz zawierał poprawny wielomian
 * @return wielomian zerowy i ustawienie @p valid na false, jeżeli wiersz nie zawierał wielomianu
 */ 
static Poly ParserMono(const char *line, size_t line_length, size_t line_number, bool *valid, bool write) {
    for (size_t i = line_length - 1; i > 0; i--) {
        if (line[i] == ',') {
            for (size_t j = i + 1; j < line_length - 1; j++) {
                *valid &= DIGIT(line[j]);
            }

            char *end = NULL;
            errno = 0;

            long exp = strtol(line + i + 1, &end, 10);

            if (!(*valid) || errno || (exp < 0) || (exp > LONG_MAX)) {
                if (write) {
                    fprintf(stderr, "ERROR %ld WRONG POLY\n", line_number);
                }

                *valid = false;

                return PolyZero();
            }
            else {
                Poly p = ParserPolyAux(line + 1, i - 1, line_number, valid, false);

                if (*valid) {
                    if (PolyIsZero(&p)) {
                        return PolyZero();
                    }
                    else if (PolyIsCoeff(&p) && exp == 0) {
                        return p;
                    }
                    else {
                        Mono *m = malloc(sizeof(Mono));
                        CHECK_PTR(m);
                
                        m[0] = (Mono) {.p = p, .exp = exp};

                        Poly r = (Poly) {.arr = m, .size = 1};

                        return r;
                    }
                }
                else {
                    if (write) {
                        fprintf(stderr, "ERROR %ld WRONG POLY\n", line_number);
                    }

                    *valid = false;

                    return PolyZero();
                }
            }

        }
    }

    if (write) {
        fprintf(stderr, "ERROR %ld WRONG POLY\n", line_number);
    }

    *valid = false;

    return PolyZero();
}

/**
 * Sprawdza, czy wiersz zawiera poprawny wielomian.
 * Zakładamy, że wiersz jest postaci "(...)+(...)+...+(...)".
 * @param[in] line : wiersz
 * @param[in] line_length : długość wiersza
 * @param[in] line_number : numer wiersza
 * @param[in, out] valid : wskaźnik na zmienną przechowującą informacje, czy wiersz był poprawny
 * @param[in] write : czy wypisywać informacje o błędach?
 * @return wielomian z wiersza, jeżeli wiersz zawierał poprawny wielomian
 * @return wielomian zerowy i ustawienie @p valid na false, jeżeli wiersz nie zawierał wielomianu
 */ 
static Poly ParserMonos(const char *line, size_t line_length, size_t line_number, bool *valid, bool write) {
    size_t left = 0;
    size_t right = 0;
    size_t count = 1;

    for (size_t i = 0; i < line_length; i++) {
        if (line[i] == '(') {
            left++;
        }
        else if (line[i] == ')') {
            right++;
        }

        if ((left == right) && (i != line_length - 1)) {
            if ((line[i + 1] == '+') && (line[i + 2] == '(')) {
                count++;
            }
            else if (line[i] != '+') {
                *valid = false;

                if (write) {
                    fprintf(stderr, "%ld ERROR %ld WRONG POLY\n", i, line_number);
                }

                return PolyZero();
            }
        }
    }

    Mono *monos = malloc(count * sizeof(Mono));
    CHECK_PTR(monos);

    ssize_t *arr = malloc((count + 1) * sizeof(ssize_t));
    CHECK_PTR(arr);
    arr[0] = -1;
    arr[count] = line_length;

    size_t k = 1;
    for (size_t i = 0; i < line_length; i++) {
        if (line[i] == '(') {
            left++;
        }
        else if (line[i] == ')') {
            right++;
        }

        if ((left == right) && (i != line_length - 1) 
            && (line[i + 1] == '+') && (line[i + 2] == '(')) {
            arr[k] = i + 1;

            k++;
        }
    }

    for (size_t i = 1; i <= count; i++) {
        Poly p = ParserMono(line + arr[i - 1] + 1, arr[i] - arr[i - 1] - 1, line_number, valid, false);

        if (*valid) {
            if (PolyIsCoeff(&p)) {
                monos[i - 1] = (Mono) {.p = p, .exp = 0};
            }
            else {
                monos[i - 1] = p.arr[0];

                free(p.arr);
            }
        }
        else {
            for (size_t j = 0; j < i - 1; j++) {
                MonoDestroy(&monos[j]);
            }

            free(arr);
            free(monos);

            return PolyZero();
        }
    }

    Poly r = PolyAddMonos(count, monos);

    free(arr);
    free(monos);

    return r;
}

/**
 * Sprawdza, czy wiersz zawiera poprawny wielomian.
 * Zakładamy, że wiersz przeszedł wstępne fazy weryfikacji.
 * @param[in] line : wiersz
 * @param[in] line_length : długość wiersza
 * @param[in] line_number : numer wiersza
 * @param[in, out] valid : wskaźnik na zmienną przechowującą informacje, czy wiersz był poprawny
 * @param[in] write : czy wypisywać informacje o błędach?
 * @return wielomian z wiersza, jeżeli wiersz zawierał poprawny wielomian
 * @return wielomian zerowy i ustawienie @p valid na false, jeżeli wiersz nie zawierał wielomianu
 */ 
static Poly ParserPolyAux(const char *line, size_t line_length, size_t line_number, bool *valid, bool write) {
    if (DIGIT(line[0]) || line[0] == '-') {
        if (!ParserValidConst(line, line_length) || (line_length == 1 && line[0] == '-')) {
            if (write) {
                fprintf(stderr, "ERROR %ld WRONG POLY\n", line_number);
            }

            *valid = false;

            return PolyZero();
        }
        else {
            char *end = NULL;
            long long coeff = strtoll(line, &end, 10);

            return PolyFromCoeff(coeff);
        }
    }
    else if (line[0] == '(') {
        size_t left = 0;
        size_t right = 0;

        for (size_t i = 0; i < line_length; i++) {
            if (line[i] == '(') {
                left++;
            }
            else if (line[i] == ')') {
                right++;
            }

            if ((left == right) && (i != line_length - 1)) {
                return ParserMonos(line, line_length, line_number, valid, write);
            }
        }

        return ParserMono(line, line_length, line_number, valid, write);

    }
    else {
        if (write) {
            fprintf(stderr, "ERROR %ld WRONG POLY\n", line_number);
        }

        *valid = false;

        return PolyZero();
    }
}

int ParserCommand(const char *line, size_t line_length, size_t line_number) {
    for (size_t i = 0; i < line_length; i++) {
        if (line[i] == '\0') {
            fprintf(stderr, "ERROR %ld WRONG COMMAND\n", line_number);

            return -1;
        }
    }

    if (!strcmp(line, "ZERO\n") || !strcmp(line, "ZERO")) {
        return ZERO;
    }
    else if (!strcmp(line, "IS_COEFF\n") || !strcmp(line, "IS_COEFF")) {
        return IS_COEFF;
    }
    else if (!strcmp(line, "IS_ZERO\n") || !strcmp(line, "IS_ZERO")) {
        return IS_ZERO;
    }
    else if (!strcmp(line, "CLONE\n") || !strcmp(line, "CLONE")) {
        return CLONE;
    }
    else if (!strcmp(line, "ADD\n") || !strcmp(line, "ADD")) {
        return ADD;
    }
    else if (!strcmp(line, "MUL\n") || !strcmp(line, "MUL")) {
        return MUL;
    }
    else if (!strcmp(line, "NEG\n") || !strcmp(line, "NEG")) {
        return NEG;
    }
    else if (!strcmp(line, "SUB\n") || !strcmp(line, "SUB")) {
        return SUB;
    }
    else if (!strcmp(line, "IS_EQ\n") || !strcmp(line, "IS_EQ")) {
        return IS_EQ;
    }
    else if (!strcmp(line, "DEG\n") || !strcmp(line, "DEG")) {
        return DEG;
    }
    else if (!strcmp(line, "PRINT\n") || !strcmp(line, "PRINT")) {
        return PRINT;
    }
    else if (!strcmp(line, "POP\n") || !strcmp(line, "POP")) {
        return POP;
    }
    else if (!memcmp(line, "AT", 2)) {
        return ParserCommandAt(line, line_length, line_number);
    }
    else if (!memcmp(line, "DEG_BY", 6)) {
        return ParserCommandDegBy(line, line_length, line_number);
    }
    else if (!memcmp(line, "COMPOSE", 7)) {
        return ParserCommandCompose(line, line_length, line_number);
    }
    else {
        fprintf(stderr, "ERROR %ld WRONG COMMAND\n", line_number);

        return -1;
    }
}

Poly ParserPoly(const char *line, size_t line_length, size_t line_number, bool *valid) {
    if (line[line_length - 1] == '\n') {
        return ParserPoly(line, line_length - 1, line_number, valid);
    }
    
    for (size_t i = 0; i < line_length; i++) {
        if (line[i] == '\0') {
            fprintf(stderr, "ERROR %ld WRONG POLY\n", line_number);

            *valid = false;

            return PolyZero();
        }
    }

    if (line[0] == '(' && !ParserValidBrackets(line, line_length)) {
        fprintf(stderr, "ERROR %ld WRONG POLY\n", line_number);

        *valid = false;

        return PolyZero();
    }
    
    return ParserPolyAux(line, line_length, line_number, valid, true);
}