/*
 * PgmStorage.h
 *
 *  Created on: Apr 20, 2017
 *      Author: danny
 */

#ifndef INT_PGMSTORAGE_PGMSTORAGE_H_
#define INT_PGMSTORAGE_PGMSTORAGE_H_
#include <avr/pgmspace.h>

#define PGM_SIZE	128

/*
 * mem cpy from program space to sram space
 * const length of PGM_SIZE
 */
void PgmStorageGet(char * dst, const char * src);

#endif /* INT_PGMSTORAGE_PGMSTORAGE_H_ */
