#ifndef CX_CORE_STRING_CXNAMEGENERATOR_H
#define CX_CORE_STRING_CXNAMEGENERATOR_H
/**
 * @copyright Copyright Catlin Zilinksi, 2015.  All rights reserved.
 *
 * @file CxNameGenerator.h
 * @brief A class to generate a set of string names based on a prefix.
 *
 * @author Catlin Zilinski
 * @date July 17, 2015
 */

#include "core/Cx.h"

#define CX_NAMEGENERATOR_MAX_NUM_LEN 11

namespace cat {
	
	/**
	 * @class CxNameGenerator CxNameGenerator.h "core/string/CxNameGenerator.h"
	 * @brief A class that is able to generate a set of names base on a prefix.
	 *
	 * The NameGenerator class takes a prefix string and then is able to 
	 * generate names of the form "prefix_<uniqueID>".
	 *
	 * @author Catlin Zilinski
	 * @version 2
	 * @since Mar 25, 2014
	 */
	class CxNameGenerator {		
	  public:

		/* Table to convert a 64 bit value to a displayable character */
		static const CxChar kEncodingTable[64];		

		/** @brief Create an empty, uninitialized CxNameGenerator. */
		CxNameGenerator();

		/**
		 * @brief Create a CxNameGenerator with the specified prefix.
		 * @param prefix The prefix string for name generation.
		 */
		CxNameGenerator(const CxChar *in_prefix);

		/** @brief Copy constructor, does deep copy. */
		CxNameGenerator(const CxNameGenerator &in_src);

		/** @brief Overloaded assignment operator, does deep copy. */
	   CxNameGenerator& operator=(const CxNameGenerator &in_src);		

		/** @brief The destructor, just frees the memory from the prefix. */
		~CxNameGenerator();

		/** @return The current value of the counter. */
		CX_FORCE_INLINE CxU64 count() const { return m_count; }		

		/**
		 * @brief Generate and return a new name.
		 * @return The generated name.
		 */
		CxChar * generate();		
					
		/**
		 * @brief Initialize an uninitialized name generator.
		 * @param in_prefix The prefix to use for the generator.
		 */
		void initWithPrefix(const CxChar *in_prefix);

		/**
		 * @brief Get the max length of the generated string.
		 * @return The max length of the generated string.
		 */
		CX_FORCE_INLINE CxI32 maxLength() const { return m_maxLength; }

		/**
		 * @brief Get prefix string.
		 * @return The Prefix string.
		 */
	   CX_FORCE_INLINE const CxChar * prefix() const { return mp_prefix; }

		/**
		 * @brief Get the length of the prefix string.
		 * @return The length of the prefix string.
		 */
		CX_FORCE_INLINE CxI32 prefixLength() const { return m_prefixLength; }		
		
	  private:
	   CxChar* mp_prefix;
		CxI64   m_count;
		CxI32   m_prefixLength;
		CxI32   m_maxLength;
	};

} // namespace cat


#endif // CX_CORE_STRING_CXNAMEGENERATOR_H
