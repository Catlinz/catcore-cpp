#ifndef CAT_CORE_GEOMETRY_SIZE2_H
#define CAT_CORE_GEOMETRY_SIZE2_H
/**
 * @copyright Copyright Catlin Zilinksi, 2014.  All rights reserved.
 *
 * @file Size2.h
 * @brief Defines a 2D Size object template class..
 *
 * @author Catlin Zilinski
 * @date June 26, 2014
 */

#include "core/corelib.h"

namespace Cat {

	/**
	 * @class Size2f size2f.h "core/geometry/size2f.h"
	 * @brief A 2D Size object template class.
	 *
	 * A 2D Size object template class that contains a width and a height.
	 *
	 * @author Catlin Zilinski
	 * @version 2
	 * @since Feb 7, 2014
	 */
	template <typename T>
	class Size2 {
	  public:
		T width, height;		

		/**
		 * @brief Create a zero sized object.
		 */
		Size2()
			: width(0), height(0) {}

		/**
		 * @brief Create a 'square' size object.
		 * @param val The value for both the width and the height.
		 */
		Size2(T val)
			: width(val), height(val) {}		

		/**
		 * @brief Create a Size2<T> width the given width and height.
		 * @param width The width.
		 * @param height The height.
		 */
		Size2(T swidth, T sheight)
			: width(swidth), height(sheight) {}
		
  		/**
		 * @brief Overloaded equality operator
		 * @param rhs The point to compare to.
		 * @return True if the points are equal.
		 */
		inline Boolean operator==(const Size2<T>& rhs) const {
			return (width == rhs.width && height == rhs.height);
		}

		/**
		 * @brief Overloaded inequality operator.
		 * @param rhs The point to compare to.
		 * @return True if the points are NOT equal.
		 */
		inline Boolean operator!=(const Size2<T>& rhs) const { 
			return (width != rhs.width || height != rhs.height);
		}

		/**
		 * @brief Overloaded addition operator.
		 * @param rhs The size to add to this one.
		 * @return A new size that is the sum of the two sizes.
		 */
		inline Size2<T> operator+(const Size2<T>& rhs) const {
			return Size2<T>(width + rhs.width, height + rhs.height);
		}

		/**
		 * @brief Overloaded subtraction operator.
		 * @param rhs The size to subtract from this one.
		 * @return A new size that is the difference of the two sizes.
		 */
		inline Size2<T> operator-(const Size2<T>& rhs) const {
			return Size2<T>(width - rhs.width, height - rhs.height);
		}

		/**
		 * @brief Overloaded self addition operator.
		 * @param rhs The size to add to this one.
		 * @return A reference to this size after the addition.
		 */
		inline Size2<T>& operator+=(const Size2<T>& rhs) {
			width += rhs.width;
			height += rhs.height;
			return *this;			
		}

		/**
		 * @brief Overloaded self subtraction operator.
		 * @param rhs The size to subtract from this one.
		 * @return A reference to this size after the subtraction.
		 */
		inline Size2<T>& operator-=(const Size2<T>& rhs) {
			width -= rhs.width;
			height -= rhs.height;
			return *this;			
		}

		/**
		 * @brief Overloaded scalar multiplication operator.
		 * @param scalar The scalar to multiply by.
		 * @return A new size multiplied by the scalar.
		 */
		inline Size2<T> operator*(T scalar) const {
			return Size2<T>(width * scalar, height * scalar);
		}

		/**
		 * @brief Overloaded scalar self multiplication operator.
		 * @param scalar The scalar to multiply by.
		 * @return A reference to this Size after multiplication.
		 */
		inline Size2<T> operator*=(T scalar) {
		   width *= scalar;
			height *= scalar;
			return *this;			
		}	

#ifdef DEBUG
		friend std::ostream& operator<<(std::ostream& out, const Size2<T>& s) {
			return out << "(" << s.width << " x " << s.height << ")";
		}	
#endif // DEBUG 
				
	};


	

} // namespace Cat
#endif // CAT_CORE_GEOMETRY_SIZE2_H
