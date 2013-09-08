/*
 * hash.h
 *
 *  Description: Hasher functional declaration.
 *
 *  Created on: 08.09.2013
 *      Author: Maxim Alov <m.alov@samsung.com>
 */

#ifndef HASH_H_
#define HASH_H_

#include <functional>
#include "types.h"


namespace mw {

/// @class Hasher
/// @brief Represents hashing function for specific types.
template <typename Type>
class Hasher;

template <>
class Hasher<ID_t> : public std::hash<ID_t> {
};

} /* namespace mw */

#endif /* HASH_H_ */
