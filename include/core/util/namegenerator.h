#ifndef CAT_CORE_UTIL_NAMEGENERATOR_H
#define CAT_CORE_UTIL_NAMEGENERATOR_H
/**
 * @copyright Copyright Catlin Zilinksi, 2014.  All rights reserved.
 *
 * @file namegenerator.h
 * @brief A class to generate a set of names based on a prefix.
 *
 * @author Catlin Zilinski
 * @date Mar 25, 2014
 */

#include "core/threading/spinlock.h"

#define CAT_MAX_GENERATED_NAME_NUMBER_LENGTH 11

namespace Cat {
	
	/**
	 * @class NameGenerator namegenerator.h "core/util/namegenerator.h"
	 * @brief A class that is able to generate a set of names base on a prefix.
	 *
	 * The NameGenerator class takes a prefix string and then is able to 
	 * generate names of the form "prefix_<num>".
	 *
	 * @author Catlin Zilinski
	 * @version 1
	 * @since Mar 25, 2014
	 */
	class NameGenerator {		
	  public:

		static const Char encodingTable[64];		

		/**
		 * @brief Create an empty, uninitialized NameGenerator.
		 */
		NameGenerator() : m_pPrefix(NIL), m_prefixLength(0), m_maxLength(0), m_count(0) {}

		/**
		 * @brief Create a NameGenerator with the specified prefix.
		 * @param prefix The prefix string for name generation.
		 */
		NameGenerator(const Char* prefix) : m_pPrefix(NIL) {
			initWithPrefix(prefix);
		}

		/**
		 * @brief Copy constructor.
		 * @param src NameGenerator to copy from.
		 */
		NameGenerator(const NameGenerator& src);

		/**
		 * @brief Overloaded assignment operator.
		 * @param src The NameGenerator to copy from.
		 */
	   NameGenerator& operator=(const NameGenerator& src);		

		/**
		 * @brief The destructor, just frees the memory from the prefix.
		 */
		~NameGenerator();

		/**
		 * @brief Get the current count that we are at.
		 * @return The current value of the count.
		 */
		inline U64 count() const { return m_count; }		

		/**
		 * @brief Generate and return a new name.
		 * @return The generated name.
		 */
		Char* generate();		
					
		/**
		 * @brief Initialize an uninitialized name generator.
		 * @param prefix The prefix to use for the generator.
		 */
		void initWithPrefix(const Char* prefix);

		/**
		 * @brief Get the max length of the generated string.
		 * @return The max length of the generated string.
		 */
		inline U32 maxLength() const { return m_maxLength; }

		/**
		 * @brief Get prefix string.
		 * @return The Prefix string.
		 */
		inline const Char* prefix() const { return m_pPrefix; }

		/**
		 * @brief Get the length of the prefix string.
		 * @return The length of the prefix string.
		 */
		inline U32 prefixLength() const { return m_prefixLength; }		
		
	  private:
	   Char* m_pPrefix;
		U32   m_prefixLength;
		U32   m_maxLength;				
		U64   m_count;
		Spinlock m_lock;		
		
		

	};

} // namespace Cat

#endif // CAT_CORE_UTIL_NAMEGENERATOR_H
