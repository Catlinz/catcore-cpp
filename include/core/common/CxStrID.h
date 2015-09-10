#ifndef CX_CORE_COMMON_CXSTRID_H
#define CX_CORE_COMMON_CXSTRID_H
/**
 * @copyright Catlin Zilinksi, 2015.  All rights reserved.
 *
 * @file CxStrID.h: 
 * @brief Contains a class for holding a string identifier.
 *
 * @author Catlin Zilinski
 * @date Aug 16, 2015
 */

#include "core/Cx.h"
#include "core/common/CxStr.h"
#include "core/common/CxHash.h"

namespace cat {

	/**
	 * @class CxStrID CxStrID.h "core/common/CxStrID.h"
	 * @brief A class that holds a string to use as an identifier.
	 * The CxStrID class holds a string identifier, as well as a 
	 * hash of that string, for fast comparison of ID's.
	 * 
	 * @since Aug 16, 2015
	 * @version 1
	 * @author Catlin Zilinski
	 */
	class CxStrID {
	  public:
		/** @brief A null id */
		CX_FORCE_INLINE CxStrID() : mp_str(0), m_hash(0) {}

		/** @brief Create a new ID from a string */
		CX_FORCE_INLINE CxStrID(const CxChar *in_str);

		/** @brief Create a new ID from a string (optionally not copying the string)*/
		CX_FORCE_INLINE CxStrID(CxChar *in_str, CxCopy in_copy);

		/** @brief Makes a copy of the string */
		CX_FORCE_INLINE CxStrID(const CxStrID &in_src);

		/** @brief Move constructor, steals the string. */
		CX_FORCE_INLINE CxStrID(CxStrID &&in_src);

		/** @brief Destructor... that I soooo totally didn't forget. */
		CX_FORCE_INLINE ~CxStrID();

		/** @brief Overloaded assignment operator, makes copy of the string */
		CX_FORCE_INLINE CxStrID & operator=(const CxStrID &in_src);

		/** @brief Overloaded move-assignment operator, steals the string. */
		CX_FORCE_INLINE CxStrID & operator=(CxStrID &&in_src);

		/** @return True if the two id's are equal */
		CX_FORCE_INLINE CxBool operator==(const CxStrID &in_other) const {
			return m_hash == in_other.m_hash;
		}

		/** @return True if the two id's are equal */
		CX_FORCE_INLINE CxBool operator==(CxU32 in_other) const {
			return m_hash == in_other;
		}

		/** @return True if the two id's are not equal */
		CX_FORCE_INLINE CxBool operator!=(const CxStrID &in_other) const {
			return m_hash != in_other.m_hash;
		}

		/** @return True if the two id's are not equal */
		CX_FORCE_INLINE CxBool operator!=(CxU32 in_other) const {
			return m_hash != in_other;
		}

		/**
		 * @brief Test to see if this ID is less than another ID.
		 * The test compares the string hashes (not the strings).
		 * @return True if this ID is less than the other ID.
		 */
		CX_FORCE_INLINE CxBool operator<(const CxStrID &in_other) const {
			return m_hash < in_other.m_hash;
		}
		
		/**
		 * @brief Test to see if this ID is less than or equal to another ID.
		 * The test compares the string hashes (not the strings).
		 * @return True if this ID is less than or equal to the other ID.
		 */
		CX_FORCE_INLINE CxBool operator<=(const CxStrID &in_other) const {
			return (m_hash <= in_other.m_hash);
		}

		/**
		 * @brief Test to see if this ID is greater than another ID.
		 * The test compares the string hashes (not the strings).
		 * @return True if this ID is greater than the other ID.
		 */
		CX_FORCE_INLINE CxBool operator>(const CxStrID &in_other) const {
			return m_hash > in_other.m_hash;
		}

		/**
		 * @brief Test to see if this ID is greater than or equal to another ID.
		 * The test compares the string hashes (not the strings).
		 * @return True if this ID is greater than or equal to the other ID.
		 */
		CX_FORCE_INLINE CxBool operator>=(const CxStrID &in_other) const {
			return (m_hash >= in_other.m_hash);
		}

		/** @return The hash of the string */
		CX_FORCE_INLINE CxU32 hash() const { return m_hash; }
		
		/** @return A pointer to the string. */
		CX_FORCE_INLINE const CxChar * str() const { return mp_str; }

		/**
		 * @brief Static method to generate the hash for a specified string.
		 * This method can be used when one only wants to compare a string to 
		 * a CxStrID, without creating an actual CxStrID object for the string.
		 * @param in_str The string to get the hash of.
		 * @return The hash value for the string.
		 */
		static CX_FORCE_INLINE CxU32 hash(const CxChar *in_str) {
			return hash::crc32(in_str);
		}

	  private:
		CxChar *mp_str;
		CxU32 m_hash;
	};

	CX_FORCE_INLINE CxStrID::CxStrID(const CxChar *in_str)
		: mp_str(0), m_hash(0) {
		if (in_str != 0) {
			mp_str = str::copy(in_str);
			m_hash = hash::crc32(in_str);
		}
	}

	CX_FORCE_INLINE CxStrID::CxStrID(CxChar *in_str, CxCopy in_copy)
		: mp_str(0), m_hash(0) {
		if (in_str != 0) {
			if (in_copy == kCxNoCopy) { mp_str = in_str; }
			else { mp_str = str::copy(in_str); }
			m_hash = hash::crc32(in_str);
		}
	}
	
	CX_FORCE_INLINE CxStrID::CxStrID(const CxStrID &in_src)
		: mp_str(0), m_hash(0) {
		if (in_src.mp_str != 0) {
			mp_str = str::copy(in_src.mp_str);
			m_hash = in_src.m_hash;
		}
	}

	CX_FORCE_INLINE CxStrID::CxStrID(CxStrID &&in_src)
		: mp_str(in_src.mp_str), m_hash(in_src.m_hash) { in_src.mp_str = 0; }

	CX_FORCE_INLINE CxStrID::~CxStrID() {
		str::free(mp_str);
		m_hash = 0;
	}

	CX_FORCE_INLINE CxStrID & CxStrID::operator=(const CxStrID &in_src) {
		str::free(mp_str);
		if (in_src.mp_str != 0) {
			mp_str = str::copy(in_src.mp_str);
			m_hash = in_src.m_hash;
		}
		else { m_hash = 0; }
		return *this;
	}

	CX_FORCE_INLINE CxStrID & CxStrID::operator=(CxStrID &&in_src) {
		str::free(mp_str);
		mp_str = in_src.mp_str;
		m_hash = in_src.m_hash;
		in_src.mp_str = 0;
		return *this;
	}

} // namespace cat

#endif // CX_CORE_COMMON_CXSTRID_H
