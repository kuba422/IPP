/** @file
  Definicja makra używanego we wszystkich plikach.

  @author Jakub Jagiełła
  @date 2021
*/

#ifndef __CHECK_PTR_H__
#define __CHECK_PTR_H__

/**
 * Sprawdza, czy alokacja pamięci się powiodła.
 * @param[in] p : adres
 * @return kończy program z kodem 1, jeżeli alokacja się nie powiodła
 */
#define CHECK_PTR(p)  \
  do {                \
    if (p == NULL) {  \
      exit(1);        \
    }                 \
  } while (0)


#endif
