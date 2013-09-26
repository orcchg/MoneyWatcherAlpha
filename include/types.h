/*
 * Types.h
 *
 *  Description:
 *
 *  Created on: 08.09.2013
 *      Author: Maxim Alov <m.alov@samsung.com>
 */

#ifndef TYPES_H_
#define TYPES_H_

#include <stdint.h>
#include "../sqlite3/sqlite3.h"


typedef sqlite3_int64 ID_t;
typedef sqlite3_int64 MoneyValue_t;
typedef sqlite3_int64 Status_t;

typedef sqlite3* DB_Handler;
typedef sqlite3_stmt* DB_Statement;

#endif /* TYPES_H_ */
