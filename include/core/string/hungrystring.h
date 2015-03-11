#ifndef CAT_CORE_STRING_HUNGRYSTRING_H
#define CAT_CORE_STRING_HUNGRYSTRING_H
/**
 * @copyright Copyright Catlin Zilinksi, 2014.  All rights reserved.
 *
 * @file hungrystring.h
 * @brief A string with variable parameters that can be fed to the string.
 *
 * @author Catlin Zilinski
 * @date Mar 27, 2014
 */

#include "core/corelib.h"

namespace Cat {
	
	/**
	 * @class HungryString hungrystring.h "core/string/hungrystring.h"
	 * @brief A string with variable parameters that can be fed to the string.
	 * 
	 * The HungryString class represents a string that contains variable 
	 * parameters that can be 'fed' to the string, in order, to build a 
	 * string to use.  For example, we could have the string 
	 * "/users/%/dir/something/%" where the "%" characters are the variable
	 * locations, and if we called str.feed("Meow"), the string would then 
	 * look like "/users/Meow/dir/something/%".
	 *
	 * @author Catlin Zilinski
	 * @version 1
	 * @since Mar 27, 2014
	 */
	class HungryString {		
	  public:
		/**
		 * @brief Create an empty HungryString.
		 */
		HungryString()
			: m_pString(NIL), m_pBuiltString(NIL), m_maxLength(0),
			  m_remainingFeeds(0), m_currentIdx(0),
			  m_currentBuildIdx(0), m_finished(false) {}		
		/**
		 * @brief Initialize the HungryString.
		 * @param string The base String with the variables to replace.
		 * @param maxLength The max length of the completed HungryString. 
		 */
		HungryString(const Char* string, Size maxLength);

		/**
		 * @brief Copy constructor to protect against deletion.
		 * @param src The HungryString to copy from.
		 */
		HungryString(const HungryString& src);		
		
		/**
		 * @brief Destroys the HungryString and the memory associated with it.
		 */
		~HungryString();

		/**
		 * @brief Overloaded assignment operator to protect from deletions.
		 * @param src The HungryString to copy from.
		 */
		HungryString& operator=(const HungryString& str);

		/**
		 * @brief Get a pointer to the base string we are feeding.
		 * @return The base string.
		 */
		const Char* baseString() const { return m_pString; }		

		/**
		 * @brief Feed the HungryString a parameter.
		 * @param str The string to feed into the next available parameter.
		 * @return A reference to the HungryString so we can chain feedings.
		 */
		HungryString& feed(const Char* str);

		/**
		 * @brief Check to see if the hungry string is full.
		 * @return False if the hungry string can accept any more parameters.
		 */
		inline Boolean isFull() const { return m_finished; }

		/**
		 * @brief Get the maximum length the finished string can be.
		 * @return The max length of the finished string.
		 */
		inline Size maxLength() const { return m_maxLength; }		

		/**
		 * @brief Get the number of feedings remaining.
		 * @return The number of remaining Feedings.
		 */
		inline Size remainingFeeds() const { return m_remainingFeeds; }

		/**
		 * @brief Get the built string, or NIL if not finished yet.
		 * @return The finished string or NIL if not finished yet.
		 */
		inline const Char* cStr() const {
			if (isFull()) {				
				return m_pBuiltString;
			}
			else {
				return NIL;
			}			
		}
	
	  private:
		/**
		 * @brief Determine the number of paramaters in the string on initialisation.
		 */
		void findAndSetNumberOfParamsInString();

		/**
		 * @brief Find the location of the next parameter and copy rest into built
		 */
		void findNextParamAndCopy();
		
		
		Char* m_pString;
		Char* m_pBuiltString;
		Size m_maxLength;
		Size m_remainingFeeds;
		Size m_currentIdx;
		Size m_currentBuildIdx;
		Boolean m_finished;		
	};

} // namespace Cat


#endif // CAT_CORE_STRING_HUNGRYSTRING_H
