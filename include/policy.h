/*
 * policy.h
 *
 *  Description: Policy between two entries class declaration.
 *
 *  Created on: 20.10.2013
 *      Author: Maxim Alov <m.alov@samsung.com>
 */

#ifndef POLICY_H_
#define POLICY_H_

#include "unistring.h"
#include "types.h"


namespace mw {

enum PolicyStatus { PS_UNKNOWN = -1,
                    PS_ENABLED = 0,
                    PS_DISABLED = 1,
                    PS_APPLIED = 2,
                    PS_PENDING = 3,
                    PS_COUNT
};

class Policy {
public:

private:
  ID_t m_id;
  WrappedString m_name;
  WrappedString m_description;
  PolicyRatio_t m_ratio;
  ID_t m_source_entry_id;
  ID_t m_destination_entry_id;
  PolicyStatus m_status;
};

}  /* namespace mw */

#endif /* POLICY_H_ */
