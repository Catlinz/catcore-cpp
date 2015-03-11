#ifndef CAT_CORE_GEOMETRY_POINT2_H
#define CAT_CORE_GEOMETRY_POINT2_H
/**
 * @copyright Copyright Catlin Zilinksi, 2014.  All rights reserved.
 *
 * @file Point2.h
 * @brief Defines a 2D point class template.
 *
 * @author Catlin Zilinski
 * @date June 25, 2014
 */

#include "core/math/vec2.h"

namespace Cat {

	/**
	 * @class Point2 point2.h "core/geometry/point2.h"
	 * @brief Defines a 2D point class template.
	 *
	 * The template for 2D point values.
	 *
	 * @author Catlin Zilinski
	 * @version 1
	 * @since June 25, 2014
	 */
	template <typename T>
	class Point2 {
	  public:
		T x, y;		

		/**
		 * @brief Create a point at (0,0)
		 */
		Point2() : x(0), y(0) {}

		/**
		 * @brief Create a Point width the given x and y coordinates.
		 * @param x The x-coordinate.
		 * @param y The y-coordinate.
		 */
		Point2(T px, T py) : x(px), y(py) {}		
		
		/**
		 * @brief Overloaded equality operator
		 * @param rhs The point to compare to.
		 * @return True if the points are equal.
		 */
		inline Boolean operator==(const Point2<T>& rhs) const {
			return (x == rhs.x && y == rhs.y);
		}

		/**
		 * @brief Overloaded inequality operator.
		 * @param rhs The point to compare to.
		 * @return True if the points are NOT equal.
		 */
		inline Boolean operator!=(const Point2<T>& rhs) const { 
			return (x != rhs.x || y != rhs.y);
		}

		/**
		 * @brief Overloaded addition operator.
		 * @param rhs The Vec2 to add with this point.
		 * @return A point that is translated by Vec2.
		 */
		inline Point2<T> operator+(const Vec2& rhs) const {
			return Point2<T>((T)(x + rhs.x), (T)(y + rhs.y));
		}

		/**
		 * @brief Overloaded subtraction operator.
		 * @param rhs The Vec2 to subtract with this point.
		 * @return A point that is translated by -Vec2.
		 */
		inline Point2<T> operator-(const Vec2& rhs) const {
			return Point2<T>((T)(x - rhs.x), (T)(y - rhs.y));
		}

		/**
		 * @brief Overloaded self addition operator.
		 * @param rhs The Vec2 to add to this point.
		 * @return A reference to this point translated by rhs.
		 */
		inline Point2<T>& operator+=(const Vec2& rhs) {
			x += (T)rhs.x;
			y += (T)rhs.y;
			return *this;			
		}

		/**
		 * @brief Overloaded self subtraction operator.
		 * @param rhs The Vec2 to subtract from this point.
		 * @return A reference to this point translated by -rhs.
		 */
		inline Point2<T>& operator-=(const Vec2& rhs) {
			x -= (T)rhs.x;
			y -= (T)rhs.y;
			return *this;			
		}

		/**
		 * @brief Static method to use in sorting to compare two points.
		 * We sort the points by first the lower x-coordinate, and then 
		 * by the lower y-coordinate.
		 * @param p1 The first point.
		 * @param p2 The second point.
		 * @return 1 if p1 > p2, 0 if equal, -1 if p1 < p2.
		 */
		static I32 compare(const void* p1, const void* p2);

		/**
		 * @brief Static method to see if 3 points make a right turn.
		 * @param p1 The first point.
		 * @param p2 The second point.
		 * @param p3 The last point.
		 * @return True if the angle between p1p2 and p2p3 is a right turn.
		 */
		static inline Boolean isRightTurn(const Point2<T>& p1, const Point2<T>& p2,
													 const Point2<T>& p3) {
			return ((p2.x - p1.x) * (p3.y - p1.y) -
					  (p2.y - p1.y) * (p3.x - p1.x)) < 0;
		}

		/**
		 * @brief Static method to see if 3 points make a left turn.
		 * @param p1 The first point.
		 * @param p2 The second point.
		 * @param p3 The last point.
		 * @return True if the angle between p1p2 and p2p3 is a left turn.
		 */
		static inline Boolean isLeftTurn(const Point2<T>& p1, const Point2<T>& p2,
													 const Point2<T>& p3) {
			return ((p2.x - p1.x) * (p3.y - p1.y) -
					  (p2.y - p1.y) * (p3.x - p1.x)) > 0;
		}
			

		
#ifdef DEBUG
		inline friend std::ostream& operator<<(std::ostream& out, const Point2<T>& p) {
			return out << "(" << p.x << ", " << p.y << ")";
		}	
#endif // DEBUG
	
				
	};

	template<typename T>
	I32 Point2<T>::compare(const void* p1, const void* p2) {
		const Point2<T>* pp1 = reinterpret_cast< const Point2<T>* >(p1);
		const Point2<T>* pp2 = reinterpret_cast< const Point2<T>* >(p2);
		if ((*pp1) == (*pp2)) {
			return 0;
		}
		else if (pp1->x < pp2->x || (pp1->x == pp2->x && pp1->y < pp2->y)) {
			return -1;
		}
		else {
			return 1;
		}
	}	
} // namespace Cat
#endif // CAT_CORE_GEOMETRY_POINT2_H
