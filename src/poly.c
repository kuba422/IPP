/** @file
  Implementacja klasy wielomianów wielu zmiennych

  @author Jakub Jagiełła
  @date 2021
*/

#include "check_ptr.h"
#include "poly.h"

#include <stdlib.h>

/**
 * Zwraca maksimum z dwóch liczb.
 * @param[in] x : liczba
 * @param[in] y : liczba
 * @return @f$ \max(x, y) @f$
 */
#define MAX(x, y) (((x) >= (y)) ? (x) : (y)) 

/**
 * Podnosi liczbę do potęgi.
 * @param[in] base : podstawa
 * @param[in] exponent : wykładnik
 * @return @f$ \text{base} ^ {\text{exponent}} @f$
 */
static poly_coeff_t Exp(const poly_coeff_t base, const poly_exp_t exponent) {
    if (exponent == 0) {
        return 1;
    }
    else if (exponent % 2 == 0) {
        return Exp(base * base, exponent / 2);
    }
    else {
        return base * Exp(base, exponent - 1);
    }
}

/**
 * Porównuje jednomiany względem wykładników.
 * @param[in] ptr_m1 : jednomian @f$m_1@f$
 * @param[in] ptr_m2 : jednomian @f$m_2@f$
 * @return 1, jeżeli @f$m_1@f$ ma większy stopień
 * @return 0, jeżeli stopnie są równe
 * @return -1, jeżeli @f$m_1@f$ ma mniejszy stopień
 */
static int MonoCompare(const void *ptr_m1, const void *ptr_m2) {
    Mono *m1 = (Mono *)ptr_m1;
    Mono *m2 = (Mono *)ptr_m2;

    return (MonoGetExp(m1) > MonoGetExp(m2)) - (MonoGetExp(m1) < MonoGetExp(m2));
}

void PolyDestroy(Poly *p) {
    if (!PolyIsCoeff(p)) {
        for (size_t i = 0; i < p->size; i++) {
            MonoDestroy(&p->arr[i]);
        }

        free(p->arr);
    }
}

Poly PolyClone(const Poly *p) {
    if (PolyIsCoeff(p)) {
        return PolyFromCoeff(p->coeff);
    }
    else {
        Mono *arr = malloc(p->size * sizeof(Mono));
        CHECK_PTR(arr);
        
        for (size_t i = 0; i < p->size; i++) {
            arr[i] = MonoClone(&p->arr[i]);
        }

        return (Poly) {.size = p->size, .arr = arr};
    }
}

/**
 * Sumuje listę jednomianów i tworzy z nich wielomian.
 * Zakłada, że lista jest posortowana niemalejąco ze względu na stopień.
 * Przejmuje na własność zawartość tablicy @p monos.
 * @param[in] count : liczba jednomianów
 * @param[in] monos : tablica jednomianów
 * @return wielomian będący sumą jednomianów
 */
static Poly PolyAddSortedMonos(size_t count, Mono monos[]) {
    if (count > 0) {
        for (size_t i = 0; i < count - 1; i++) {
            assert(MonoGetExp(&monos[i]) <= MonoGetExp(&monos[i + 1]));
        }
    }

    if (count == 0 || monos == NULL) {
        return PolyZero();
    }

    size_t size = 1;
    for (size_t i = 0; i < count - 1; i++) {
        if (MonoGetExp(&monos[i]) < MonoGetExp(&monos[i + 1])) {
            size++;
        }
    }

    Mono *arr = malloc(size * sizeof(Mono));
    CHECK_PTR(arr);

    size_t j = 0, k = 0;
    for (size_t i = 0; i < size - k; i++) {
        arr[i] = MonoClone(&monos[j]);

        while (j < count - 1 && MonoGetExp(&monos[j]) == MonoGetExp(&monos[j + 1])) {
            j++; 

            Mono m = (Mono) {.p = PolyAdd(&arr[i].p, &monos[j].p), .exp = MonoGetExp(&arr[i])};

            MonoDestroy(&arr[i]); 

            arr[i] = m;
        }

        if (PolyIsZero(&arr[i].p)) {
            i--;
            k++;
        }

        j++;
    }

    for (size_t i = 0; i < count; i++) {
        MonoDestroy(&monos[i]);
    }

    if (size - k == 0) {
        free(arr);

        return PolyZero();
    }
    else if (size - k == 1 && MonoGetExp(&arr[0]) == 0 && PolyIsCoeff(&arr[0].p)) {
        Poly r = PolyFromCoeff(arr[0].p.coeff);

        free(arr);

        return r;
    }
    else {
        return (Poly) {.size = size - k, .arr = arr};
    }
}

/**
 * Usuwa jednomiany zerowe z @p monos i zwraca odpowiedni wielomian.
 * Zakładamy, że jednomiany są posortowane ściśle rosnąco ze względu na wykładniki.
 * Przejmuje na wartość tablicę @p monos.
 * @param[in] size : liczba jednomianów
 * @param[in] monos : tablica jednomianów
 * @return urposzczony wielomian składający się z jednomianów z @p monos
 */ 
static Poly Simplify(size_t size, Mono monos[]) {
    if (size == 1) {
        if ((monos[0].exp == 0 && PolyIsCoeff(&(monos[0].p))) || 
            PolyIsZero(&(monos[0].p))) {

            Poly p = monos[0].p;

            free(monos);
            
            return p;
        }
        else {
            return (Poly) {.arr = monos, .size = 1};
        }
    }

    size_t new_size = 0;

    for (size_t i = 0; i < size; i++) {
        if (!PolyIsZero(&(monos[i].p))) {
            new_size++;
        }
    }

    if (new_size == size) {
        return (Poly) {.arr = monos, .size = size};
    }
    else if (new_size == 1) {
        size_t i = 0;
        while (PolyIsZero(&(monos[i].p))) {
            i++;
        }

        Mono *new_monos = malloc(sizeof(Mono));
        CHECK_PTR(new_monos);

        new_monos[0] = monos[i];

        free(monos);

        return Simplify(1, new_monos);
    }
    else if (new_size == 0) {
        free(monos);

        return PolyZero();
    }
    
    Mono* new_monos = malloc(new_size * sizeof(Mono));
    CHECK_PTR(new_monos);

    size_t j = 0;
    for (size_t i = 0; i < size; i++) {
        if (!PolyIsZero(&(monos[i].p))) {
            new_monos[j] = monos[i];
            
            j++;
        }
    }

    free(monos);

    return (Poly) {.arr = new_monos, .size = new_size};
}

Poly PolyAdd(const Poly *p, const Poly *q) {
    if (PolyIsCoeff(p) && PolyIsCoeff(q)) {
        return PolyFromCoeff(p->coeff + q->coeff);
    }
    else if (!PolyIsCoeff(p) && !PolyIsCoeff(q)) {
        Mono *arr = malloc((p->size + q->size) * sizeof(Mono));
        CHECK_PTR(arr);

        size_t i_p = 0;
        size_t i_q = 0;
        size_t i = 1;

        if (p->arr[0].exp <= q->arr[0].exp) {
            arr[0] = MonoClone(&(p->arr[0]));
            
            i_p++;
        }
        else {
            arr[0] = MonoClone(&(q->arr[0]));

            i_q++;
        }

        while (i_p + i_q < p->size + q->size) {
            if ((i_p == p->size && i_q < q->size) || 
                (!(i_p < p->size && i_q == q->size) && p->arr[i_p].exp > q->arr[i_q].exp)) {
                if (arr[i - 1].exp == q->arr[i_q].exp) {
                    Poly r = PolyAdd(&(arr[i - 1].p), &(q->arr[i_q].p));

                    PolyDestroy(&(arr[i - 1].p));

                    arr[i - 1].p = r;
                }
                else {
                    arr[i] = MonoClone(&(q->arr[i_q]));

                    i++;
                }

                i_q++;
            }
            else {
                if (arr[i - 1].exp == p->arr[i_p].exp) {
                    Poly r = PolyAdd(&(arr[i - 1].p), &(p->arr[i_p].p));

                    PolyDestroy(&(arr[i - 1].p));

                    arr[i - 1].p = r;
                }
                else {
                    arr[i] = MonoClone(&(p->arr[i_p]));

                    i++;
                }

                i_p++;
            }
        }

        return Simplify(i, arr);
    }
    else {
        if (PolyIsCoeff(p)) {
            return PolyAdd(q, p);
        }

        Mono arr[] = {(Mono) {.p = PolyClone(q), .exp = 0}};

        Poly r = (Poly) {.arr = arr, .size = 1};

        return PolyAdd(p, &r);
    }
}

/**
 * Sprawdza, czy tablica jednomianów jest posortowana (rosnąco, względem mianowników).
 * @param[in] count : liczba elementów tablicy
 * @param[in] monos : tablica
 * @return czy tablica jest posortowana?
 */ 
static bool isSorted(size_t count, const Mono monos[]) {
    bool sorted = true;

    if (count > 0) {
        for (size_t i = 0; i < count - 1; i++) {
            sorted &= (MonoGetExp(&monos[i]) <= MonoGetExp(&monos[i + 1]));
        }
    }

    return sorted;
}

Poly PolyAddMonos(size_t count, const Mono monos[]) {
    Mono *arr = malloc(count * sizeof(Mono));
    CHECK_PTR(arr);
    
    for (size_t i = 0; i < count; i++) {
        arr[i] = monos[i];
    }

    if (!isSorted(count, arr)) {
        qsort(arr, count, sizeof(Mono), MonoCompare);
    }

    Poly r = PolyAddSortedMonos(count, arr);

    free(arr);
    
    return r;
}

Poly PolyOwnMonos(size_t count, Mono *monos) {
    if (!isSorted(count, monos)) {
        qsort(monos, count, sizeof(Mono), MonoCompare);
    }

    Poly r = PolyAddSortedMonos(count, monos);

    free(monos);
    
    return r;
}

Poly PolyCloneMonos(size_t count, const Mono monos[]) {
    Mono *arr = malloc(count * sizeof(Mono));
    CHECK_PTR(arr);
    
    for (size_t i = 0; i < count; i++) {
        arr[i] = MonoClone(&monos[i]);
    }

    if (!isSorted(count, arr)) {
        qsort(arr, count, sizeof(Mono), MonoCompare);
    }

    Poly r = PolyAddSortedMonos(count, arr);

    free(arr);
    
    return r;
}

Poly PolyMul(const Poly *p, const Poly *q) {
    if (PolyIsCoeff(p) && PolyIsCoeff(q)) {
        return PolyFromCoeff(p->coeff * q-> coeff);
    }
    else if (!PolyIsCoeff(p) && !PolyIsCoeff(q)) {       
        Mono *monos = malloc(q->size * sizeof(Mono));
        CHECK_PTR(monos);

        Poly r = PolyZero();

        for (size_t i_p = 0; i_p < p->size; i_p++) {
            Mono *m_p = &(p->arr[i_p]);

            for (size_t i_q = 0; i_q < q->size; i_q++) {
                Mono *m_q = &(q->arr[i_q]);

                monos[i_q] = (Mono) {.p = PolyMul(&m_p->p, &m_q->p), .exp = MonoGetExp(m_p) + MonoGetExp(m_q)};
            }

            Poly s = PolyAddSortedMonos(q->size, monos);
            Poly t = PolyAdd(&r, &s);

            PolyDestroy(&r);
            PolyDestroy(&s);

            r = t;
        }

        free(monos);

        return r;
    }
    else {
        if (PolyIsCoeff(q)) {
            if (PolyIsZero(q)) {
                return PolyZero();
            }

            Mono *monos = malloc(p->size * sizeof(Mono));
            CHECK_PTR(monos);

            size_t k = 0;
            for (size_t i = 0; i < p->size; i++) {
                Poly r = PolyMul(&p->arr[i].p, q);

                if (!PolyIsZero(&r)) {
                    monos[k] = (Mono) {.p = r, .exp = MonoGetExp(&p->arr[i])};
                    
                    k++;
                }
            }

            Poly r = PolyAddSortedMonos(k, monos);

            free(monos);

            return r;
        }
        else {
            return PolyMul(q, p);
        }
    }
}

/**
 * Neguje wielomian (bez kopiowania danych)
 * @param[in] p : wielomian
 */ 
static void PolyNegAux(Poly *p) {
    if (PolyIsCoeff(p)) {
        p->coeff *= -1;
    }
    else {
        for (size_t i = 0; i < p->size; i++) {
            PolyNegAux(&(p->arr[i].p));
        }
    }
}

Poly PolyNeg(const Poly *p) {
    Poly r = PolyClone(p);

    PolyNegAux(&r);

    return r;
}

Poly PolySub(const Poly *p, const Poly *q) {
    Poly neg_q = PolyNeg(q);
    Poly r = PolyAdd(p, &neg_q);

    PolyDestroy(&neg_q);

    return r;
}

poly_exp_t PolyDegBy(const Poly *p, size_t var_idx) {
    if (PolyIsCoeff(p)) {
        return PolyIsZero(p) ? -1 : 0;
    }
    else if (var_idx == 0) {
        poly_exp_t deg = 0;
        for (size_t i = 0; i < p->size; i++) {
            deg = MAX(deg, MonoGetExp(&p->arr[i]));
        }

        return deg;
    }
    else {
        poly_exp_t deg = 0;
        for (size_t i = 0; i < p->size; i++) {
            deg = MAX(deg, PolyDegBy(&(&p->arr[i])->p, var_idx - 1));
        }

        return deg;
    }
}

poly_exp_t PolyDeg(const Poly *p) {
    if (PolyIsCoeff(p)) {
        return PolyIsZero(p) ? -1 : 0;
    }
    else {
        poly_exp_t deg = 0;
        for (size_t i = 0; i < p->size; i++) {
            deg = MAX(deg, MonoGetExp(&p->arr[i]) + PolyDeg(&(&p->arr[i])->p));
        }

        return deg;
    }
}

bool PolyIsEq(const Poly *p, const Poly *q) {
    if (PolyIsCoeff(p) && PolyIsCoeff(q)) {
        return p->coeff == q->coeff;
    }
    else if (!PolyIsCoeff(p) && !PolyIsCoeff(q) && p->size == q->size) {
        for (size_t i = 0; i < p->size; i++) {
            if ((MonoGetExp(&p->arr[i]) != MonoGetExp(&q->arr[i]))
                || !PolyIsEq(&(&p->arr[i])->p, &(&q->arr[i])->p)) {
                return false;
            }
        }

        return true;
    }
    else {
        return false;
    }
}

Poly PolyAt(const Poly *p, poly_coeff_t x) {
    if (PolyIsCoeff(p)) {
        return PolyClone(p);
    }
    else {
        Poly q = PolyZero();

        for (size_t i = 0; i < p->size; i++) {
            Poly c = PolyFromCoeff(Exp(x, MonoGetExp(&p->arr[i])));
            c = PolyMul(&c, &(&p->arr[i])->p);

            Poly r = PolyAdd(&q, &c);
            
            PolyDestroy(&c);
            PolyDestroy(&q);

            q = r;
        }

        return q;
    }
}

/**
 * Zamienia wielomian @f$p(x_0, x_1, \dots)@f$ na wielomian 
 * @f$p(x_0, x_1, \dots, x_{k-1}, 0, 0, \dots)@f$. 
 * @param[in] p : wielomian
 * @param[in] k : liczba niezerowych zmiennych
 */
static void PolyTrim(Poly *p, size_t k) {
    if (k > 0 && !PolyIsCoeff(p)) {
        for (size_t i = 0; i < p->size; i++) {
            PolyTrim(&((p->arr[i]).p), k - 1);
        }

        *p = Simplify(p->size, p->arr);
    }
    else if (k == 0 && !PolyIsCoeff(p)) {
        if (p->arr[0].exp == 0) {
            Mono m = MonoClone(&(p->arr[0]));

            for (size_t i = 0; i < p->size; i++) {
                MonoDestroy(&(p->arr[i]));
            }

            free(p->arr);

            PolyTrim(&(m.p), 0);
            
            p->arr = NULL;
            p->coeff = m.p.coeff;
        }
        else {
            for (size_t i = 0; i < p->size; i++) {
                MonoDestroy(&(p->arr[i]));
            }

            free(p->arr);

            p->arr = NULL;
            p->coeff = 0;
        }
    }
}

/**
 * Składa wielomian @f$p@f$ z wielomianami @f$q_0, q_1, \dots, q_{k-1}@f$.
 * Zakłada, że @f$p@f$ jest postaci @f$a\cdot\prod_{i=0}^n x_i^{t_i}@f$
 * oraz @f$k \geq 1@f$.
 * @param[in] p : wielomian @f$p@f$
 * @param[in] k : liczba wielomianów składanych z @f$p@f$
 * @param[in] q : lista wielomianów składanych z @f$p@f$
 * @return @f$p(q_0, q_1, \dots)@f$
 */ 
static Poly PolyProductCompose(const Poly *p, size_t k, const Poly q[]) {
    if (PolyIsCoeff(p)) {
        return PolyClone(p);
    }

    Poly p_clone = PolyClone(p);
    Poly r = p_clone;

    size_t n = 0;
    while (!PolyIsCoeff(&r) && r.arr[0].exp == 0) {
        n++;

        r = r.arr[0].p;
    }

    r = p_clone;

    for (size_t i = 0; i < n; i++) {
        r = r.arr[0].p;
    }

    poly_exp_t r_exp = r.arr[0].exp;

    r.arr[0].exp = 0;
    
    Poly s = PolyFromCoeff(1);
    for (poly_exp_t i = 0; i < r_exp; i++) {
        Poly t = PolyMul(&s, &q[n]);

        PolyDestroy(&s);

        s = t;
    }

    Poly u;
    if (r.arr[0].p.arr != NULL) {
        u = PolyProductCompose(&p_clone, k, q);
    }
    else {
        u = PolyFromCoeff(r.arr[0].p.coeff);
    }

    Poly w = PolyMul(&s, &u);

    PolyDestroy(&p_clone);
    PolyDestroy(&s);
    PolyDestroy(&u);

    return w;
}

/**
 * Zamienia wielomian na listę wielomianów postaci @f$a\cdot\prod_{i=0}^n x_i^{t_i}@f$.
 * @param[in] p : wielomian
 * @param[out] poly_arr : wskaźnik na tablicę, w której zostaną zapiasane powstałe wielomiany
 * @param[out] number_of_polys : wskaźnik na zmienną, w której zostanie zapiasana liczba powstałych wielomianów
 */ 
static void PolyAsSum(const Poly *p, Poly **poly_arr, size_t *number_of_polys) {
    if (PolyIsCoeff(p)) {
        Poly *arr = malloc(sizeof(Poly));
        CHECK_PTR(arr);
        arr[0] = PolyClone(p);

        *poly_arr = arr;
        *number_of_polys = 1;
    }
    else {
        Poly **arrays = malloc(p->size * sizeof(Poly*));
        CHECK_PTR(arrays);
        size_t *numbers = malloc(p->size * sizeof(size_t));
        CHECK_PTR(numbers);
        size_t size = 0;

        for (size_t i = 0; i < p->size; i++) {
            PolyAsSum(&(p->arr[i].p), &(arrays[i]), &(numbers[i]));

            size += numbers[i];
        }

        Poly *arr = malloc(size * sizeof(Poly));
        CHECK_PTR(arr);
        size_t count = 0;

        for (size_t i = 0; i < p->size; i++) {
            for (size_t j = 0; j < numbers[i]; j++) {
                Mono m = (Mono) {.exp = p->arr[i].exp, .p = arrays[i][j]};

                Mono* m_arr = malloc(sizeof(Mono));
                CHECK_PTR(m_arr);
                m_arr[0] = m;

                arr[count + j] = (Poly) {.size = 1, .arr = m_arr};
            }

            count += numbers[i];
        }

        Poly r = arr[0];
        bool isCoeff = true;
        while (r.arr != NULL) {
            if (r.arr[0].exp != 0) {
                isCoeff = false;
            }

            r = r.arr[0].p;
        }

        if (isCoeff) {
            PolyDestroy(&arr[0]);
            arr[0] = PolyFromCoeff(r.coeff);
        }

        *poly_arr = arr;
        *number_of_polys = count;

        for (size_t i = 0; i < p->size; i++) {
            free(arrays[i]);
        }
        free(arrays);
        free(numbers);
    }
}

Poly PolyCompose(const Poly *p, size_t k, const Poly q[]) {
    Poly p_clone = PolyClone(p);

    PolyTrim(&p_clone, k);

    if (k == 0) {
        return p_clone;
    }
    else {
        Poly *poly_arr = NULL;
        size_t number_of_polys = 0;

        PolyAsSum(&p_clone, &poly_arr, &number_of_polys);

        Poly r = PolyZero();
        
        for (size_t i = 0; i < number_of_polys; i++) {
            Poly s = PolyProductCompose(&(poly_arr[i]), k, q);

            Poly t = PolyAdd(&s, &r);

            PolyDestroy(&r);
            PolyDestroy(&s);
            PolyDestroy(&(poly_arr[i]));

            r = t;
        }

        PolyDestroy(&p_clone);
        free(poly_arr);

        return r;
    }
}