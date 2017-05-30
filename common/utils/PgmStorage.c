/*
 * PgmStorage.c
 *
 *  Created on: Apr 20, 2017
 *      Author: danny
 */
#include "PgmStorage.h"

void PgmStorageGet(char * dst, const char * src)
{
	memcpy_P(dst, src, PGM_SIZE);
}

