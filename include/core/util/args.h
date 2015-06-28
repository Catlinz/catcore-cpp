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

	/**
	 * @class ArgsN args.h "core/util/args.h"
	 * @brief The base Args class for storing / retreiving arguments.
	 * 
	 * @author Catlin Zilinski
	 * @version 1
	 * @since Mar 11, 2015
	 */
	class ArgsN {
	  public:
		ArgsN() : m_it(0) {}

		/**
		 * @param in_idx The argument index.
		 * @return A reference to the specified argument value.
		 */
		IntegralType& arg(int in_idx) {
			return m_it[in_idx];
		}
		
		/**
		 * @param in_idx The argument index.
		 * @return A constant reference to the specified argument value.
		 */
		const IntegralType& arg(const int in_idx) {
			return m_it[in_idx];
		}
		
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
