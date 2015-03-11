#ifndef CAT_CORE_SYS_OSX_SYSTEM_H
#define CAT_CORE_SYS_OSX_SYSTEM_H
/**
 * @copyright Copyright Catlin Zilinksi, 2014.  All rights reserved.
 *
 * @file system.h
 * @brief A static class to wrap certain system functionality.
 *
 * @author Catlin Zilinski
 * @date Apr 2, 2014
 */

#include <unistd.h>
#include "core/string/string.h"

namespace Cat {
	
	/**
	 * @class System system.h "core/sys/system.h"
	 * @brief A static class to wrap certain functionality.
	 *
	 * @author Catlin Zilinski
	 * @version 1
	 * @since Apr 2, 2014
	 */
	class System {		
	  public:

		/**
		 * @brief Get the name of the currently logged in user.
		 * This method will check to see if we've already fetched the name
		 * of the user, and if not, will fetch it, then return it.
		 * @return A StringPtr with the Name of the currently logged in user.
		 */
		static inline StringPtr& getLoginName() {
			if (m_pLoginName.isNull()) {
				m_pLoginName = String::create(getlogin());
			}
			return m_pLoginName;
		}

		/**
		 * @brief Change the current working directory.
		 * @param dir The directory to change to.
		 * @return kENoError if success, another error code if failed.
		 */
		static Error::ErrEnum chdir(const Char* dir);
		
		/**
		 * @brief Get the value of the current working directory.
		 * @return The value of the current working directory.
		 */
		static StringPtr& getCwd();
		
	  private:
		static StringPtr m_pLoginName;
		static StringPtr m_pCwd;		
	};

} // namespace Cat

#endif // CAT_CORE_SYS_OSX_SYSTEM_H
