#ifndef CAT_CORE_MATH_ANGLE_H
#define CAT_CORE_MATH_ANGLE_H
/**
 * @copyright Catlin Zilinksi, 2013.  All rights reserved.
 *
 * @file angle.h: 
 * @brief Contains a simple Angle class to encapsulate an angle.
 *
 * The Angle class encapsulates an angle.  The angle is setable in radians 
 * or degrees, and the value in radians or degrees can be retreived.
 *
 * @author Catlin Zilinski
 * @date Dec 2, 2013
 */

#include "core/math/mathcore.h"

namespace Cat {
	/**
	 * @class Angle angle.h "core/math/angle.h"
	 * @brief A simple Angle class to encapsulate angle measures.
	 *
	 * The Angle class stores an angle in radians.  The angle can be 
	 * specified in radians or degrees, though the constructor only
	 * takes an angle in radians.  To construct an angle in degrees, 
	 * you must use one of the static helper methods.
	 *
	 * @author Catlin Zilinski
	 * @version 1
	 * @since Dec 2, 2013
	 */
	class Angle {
	  public:

		const static Real EPSILON;
		const static Angle kAZero;
		

		/**
		 * @brief Creates a zero angle
		 */
		Angle() : m_radians(0.0) {}

		/**
		 * @brief Creates a new angle from a radian angle value.
		 * @param radians The angle in radians.
		 */
		explicit Angle(Real angleRad) : m_radians(angleRad) {}

		/**
		 * @brief Overloaded equality operator for two angles.
		 * @param rhs The angle to test for equality.
		 * @return True if the angles are equal.
		 */
		inline Boolean operator==(const Angle& rhs) const { return m_radians == rhs.radians(); }

		/**
		 * @brief Overloaded inequality operator for two angles.
		 * @param rhs The angle to test for inequality.
		 * @return True if the angles are not equal.
		 */
		inline Boolean operator!=(const Angle& rhs) const { return m_radians != rhs.radians(); }

		/**
		 * @brief Overloaded greater than operator for angles.
		 * @param rhs The angle to test..
		 * @return True if this angle is greater than the other angle.
		 */
		inline Boolean operator>(const Angle& rhs) const { return m_radians > rhs.radians(); }

		/**
		 * @brief Overloaded less than operator for angles.
		 * @param rhs The angle to test..
		 * @return True if this angle is less than the other angle.
		 */
		inline Boolean operator<(const Angle& rhs) const { return m_radians < rhs.radians(); }

			/**
		 * @brief Overloaded greater than or equals operator for angles.
		 * @param rhs The angle to test..
		 * @return True if this angle is greater than or equal to the other angle.
		 */
		inline Boolean operator>=(const Angle& rhs) const { return m_radians >= rhs.radians(); }

		/**
		 * @brief Overloaded less than or equals operator for angles.
		 * @param rhs The angle to test..
		 * @return True if this angle is less than or equal to the other angle.
		 */
		inline Boolean operator<=(const Angle& rhs) const { return m_radians <= rhs.radians(); }

		/**
		 * @brief Test two angles for approximate equality.
		 * @param rhs The angle to test agains this one.
		 * @return true if the angles are approximately equal.
		 */
		inline Boolean approx(const Angle& rhs) const {
			return (fabs( m_radians - rhs.m_radians) < Angle::EPSILON);			
		}
		
		
		/**
		 * @brief Method to get the value of the angle in radians.
		 * @return The value of the angle in radians.
		 */
		inline Real radians() const { return m_radians; }
		
		/**
		 * @brief Method to get the value of the angle in degrees.
		 * @return The value of the angle in degrees.
		 */
		inline Real degrees() const { return (m_radians * CAT_RADTODEG); }
		
		/**
		 * @brief Set the value of the angle in radians.
		 * @param angleRad The angle in radians.
		 */
		inline void setRadians(const Real angleRad) { m_radians = angleRad; }
		
		/**
		 * @brief Set the value of the angle in degrees.
		 * @param angleDeg The angle in degrees.
		 */
		inline void setDegrees( const Real angleDeg) { m_radians = (angleDeg * CAT_DEGTORAD); }
		
		/**
		 * @brief Overloaded addition operator to add to angles.
		 * @param rhs The angle to add.
		 * @return A new angle that is the sum of this + rhs.
		 */
		inline Angle operator+(const Angle& rhs) const { return Angle(m_radians + rhs.radians()); }
		
		/**
		 * @brief Overloaded addition operator to add another angle to this one.
		 * @param rhs The angle to add to this one.
		 * @return A reference to this angle
		 */
		inline Angle& operator+=(const Angle& rhs) { m_radians += rhs.radians(); return *this; }
		
		/**
		 * @brief Overloaded subtraction operator to subtract two angles.
		 * @param rhs The angle to subtract.
		 * @return A new angle that is the difference of this - rhs.
		 */
		inline Angle operator-(const Angle& rhs) const { return Angle(m_radians - rhs.radians()); }
		
		/**
		 * @brief Overloaded subtraction operator to subtract an angle from this one.
		 * @param rhs The angle to subtract from this one.
		 * @return A reference to this angle.
		 */
		inline Angle& operator-=(const Angle& rhs) { m_radians -= rhs.radians(); return *this; }
		
		/**
		 * @brief Overloaded multiplication operator to multiply an angle by a scalar.
		 * @param scalar The scalar to multiply by.
		 * @return A new angle that is the product of this*scalar.
		 */
		inline Angle operator*(const Real scalar) const { return Angle(m_radians*scalar); }
		inline friend Angle operator*(const Real scalar, const Angle& rhs) { return Angle(scalar*rhs.radians()); }
		
		/**
		 * @brief Overloaded multiplication operator to multiply this angle by a scalar.
		 * @param scalar The scalar to multiply by.
		 * @return A reference to this angle.
		 */
		inline Angle& operator*=(const Real scalar) { m_radians *= scalar; return *this; }

			/**
		 * @brief Overloaded division operator to divide an angle by a scalar.
		 * @param scalar The scalar to divide by.
		 * @return A new angle that is the quotient of this/scalar.
		 */
		inline Angle operator/(const Real scalar) const { return Angle(m_radians/scalar); }
		inline friend Angle operator/(const Real scalar, const Angle& rhs) { return Angle(scalar/rhs.radians()); }
		
		/**
		 * @brief Overloaded division operator to divide this angle by a scalar.
		 * @param scalar The scalar to divide by.
		 * @return A reference to this angle.
		 */
		inline Angle& operator/=(const Real scalar) { m_radians /= scalar; return *this; }
		
		/**
		 * @brief Overloaded negation operator to negate an angle.
		 * @return A copy of the angle, negated.
		 */
		inline Angle operator-() const {
			return Angle(-m_radians);
		}
		

		/**
		 * @brief Function to create an angle from a radian value.
		 * @param angleRad The angle in radians.
		 * @return A new Angle.
		 */
		static inline Angle fromRadians(const Real angleRad) { return Angle(angleRad); }
		
		/**
		 * @brief Function to create an angle from a degree value.
		 * @param angleDeg The angle in degrees.
		 * @return A new Angle.
		 */
		static inline Angle fromDegrees(const Real angleDeg) { return Angle(angleDeg * CAT_DEGTORAD); }		

	  private: 
		Real m_radians;
	};

#ifdef DEBUG
	inline std::ostream& operator<<(std::ostream& out, const Angle& a) {
		return out << a.radians() << "rad";
	}	
#endif // DEBUG
	

} // namespace Cat

#endif // CAT_CORE_MATH_ANGLE_H
