#ifndef CAT_CORE_UTIL_ARGS_H
#define CAT_CORE_UTIL_ARGS_H

/**
 * @copyright Copyright Catlin Zilinski, 2015.  All rights reserved.
 *
 * @file args.h
 * @brief Contains the Args class to storing and retrieving arguments.
 *
 *
 * @author Catlin Zilinski
 * @date Mar 11, 2015
 */

#include "core/corelib.h"

namespace Cat {

	class ArgsN {
	  public:
		ArgsN() : m_it(0) {}

	  protected:
		IntegralType* m_it;
	};

	template <int N>
	class Args : public ArgsN {
	  public:
		Args() { m_it = m_data; }
		
		
	  private:
		IntegralType m_data[N];
	};
	
	
} // namespace Cat

#endif // CAT_CORE_UTIL_ARGS
