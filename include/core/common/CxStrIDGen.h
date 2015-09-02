#ifndef CX_CORE_COMMON_CXSTRIDGEN_H
#define CX_CORE_COMMON_CXSTRIDGEN_H
/**
 * @copyright Catlin Zilinksi, 2015.  All rights reserved.
 *
 * @file CxStrIDGen.h: 
 * @brief Contains a class for generating string ID's.
 *
 * @author Catlin Zilinski
 * @date Sept 2, 2015
 */

#include "core/Cx.h"
#include "core/CxStrID.h"

namespace cat {

	/**
	 * @class CxStrIDGen CxStrIDGen.h "core/common/CxStrIDGen.h"
	 * A class that is used to generate unique string ID's with a common prefix.
	 * The ID's are generated as <prefix>_# where # is a base-64 number composed of
	 * the characters 0-9, a-z, A-Z, +, /.
	 * 
	 * @since Sept 2, 2015
	 * @version 1
	 * @author Catlin Zilinski
	 */
	class CxStrIDGen {
	  public:
		static const CxChar s_encodingTable[64];

		/** @brief Create an empty, uninitialized generator. */
		CX_FORCE_INLINE CxStrIDGen() : mp_prefix(0), m_prefixLn(0), m_maxLn(0), m_count(0) {}

		/**
		 * @brief Create a generator with the specified prefix.
		 * @param in_prefix The prefix string for ID generation.
		 */
		CX_FORCE_INLINE CxStrIDGen(const CxChar *in_prefix)
			 : mp_prefix(0), m_prefixLn(0), m_maxLn(0), m_count(0) {
			initWithPrefix(in_prefix);
		}

		/** @brief Copy constructor, makes a copy of the prefix string */
		CxStrIDGen(const CxStrIDGen &in_src);

		/** @brief Free's the memory for the prefix string */
		~CxStrIDGen();

		/** @brief Overloaded assignment operator, copies prefix string */
		CxStrIDGen & operator=(const CxStrIDGen &in_src);

		/** @return The current value of the count. */
		CX_FORCE_INLINE CxI64 count() const { return m_count; }

		/**
		 * @brief Generate and return a new name.
		 * @return The generated name.
		 */
		CxStrID generate();

		/**
		 * @brief Initialize an uninitialized ID generator.
		 * @param in_prefix The prefix to use for the generator.
		 */
		void initWithPrefix(const CxChar *in_prefix);

		/** @return The max length of the generated string. */
		CX_FORCE_INLINE CxI32 maxLength() const { return m_maxLn; }

		/** @return The prefix string for generated ID's. */
		CX_FORCE_INLINE const CxChar * prefix() const { return mp_prefix; }

		/** @return The length of the prefix string. */
		CX_FORCE_INLINE CxI32 prefixLength() const { return m_prefixLn; }
		
	  private:
		CxU64 m_count;
		CxChar* mp_prefix;
		CxI64   m_count;
		
		CxI32   m_prefixLength;
		CxI32   m_maxLength;				
	};

} // namespace cat

#endif // CX_CORE_COMMON_CXSTRIDGEN_H
