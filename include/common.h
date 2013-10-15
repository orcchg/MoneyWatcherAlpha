/*
 * common.h
 *
 *  Description: Common functionality.
 *
 *  Created on: 16.09.2013
 *      Author: Maxim Alov <m.alov@samsung.com>
 */

#ifndef COMMON_H_
#define COMMON_H_

#include <config.h>
/*
#if __GNUC__ <= 4 && __GNUC_MINOR__ <= 5
#define nullptr NULL
#else
#define nullptr NULL
#endif
*/
#define ENABLED_LOGGING __MW_ENABLE_LOGGING__
#define ENABLED_DB_CACHING __MW_ENABLE_CACHING__
#define ENABLED_ADVANCED_DEBUG __MW_ENABLE_ADVANCED_DEBUG__
#define __MW_DB_CACHED__

#endif /* COMMON_H_ */
