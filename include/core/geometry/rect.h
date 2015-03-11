#ifndef CAT_CORE_GEOMETRY_RECT_H
#define CAT_CORE_GEOMETRY_RECT_H
/**
 * @copyright Copyright Catlin Zilinksi, 2014.  All rights reserved.
 *
 * @file rect.h
 * @brief Contains a rectangle class template.
 *
 * @author Catlin Zilinski
 * @date June 26, 2014
 */

#include "core/geometry/size2.h"
#include "core/geometry/point2.h" 

namespace Cat {

	/**
	 * @class Rect rect.h "core/geometry/rect.h"
	 * @brief Contains a rectangle class template.
	 *
	 * A Rectangle class template with methods for 
	 * creating and working with the rectangle.
	 *
	 * @author Catlin Zilinski
	 * @version 1
	 * @since June 26, 2014
	 */
	template<typename T>
	class Rect {
	  public:
		/**
		 * @brief Create an empty rectangle.
		 */
		inline Rect()
			: m_topLeft(0, 0),  m_size(0, 0) {}

		/**
		 * @brief Create a rectangle at (0,0) with the specified size.
		 * @param size The size of the rectangle.
		 */
		inline explicit Rect(const Size2<T>& size)
			: m_topLeft(0, 0), m_size(size) {}

		/**
		 * @brief Create a rectangle at (0,0) with the specified size.
		 * @param width The width of the rectangle.
		 * @param height The height of the rectangle.
		 */
		inline explicit Rect(T width, T height)
			: m_topLeft(0, 0), m_size(width, height) {}

		/**
		 * @brief Create a rectangle from the top-left and bottom-right points.
		 * @param topLeft The top-left point of the rectangle.
		 * @param bottomRight The bottom-right point of the rectangle.
		 */
		inline Rect(const Point2<T>& topLeft,
							const Point2<T>& bottomRight)
		: m_topLeft(topLeft) {
			m_size.width = bottomRight.x - topLeft.x;
			m_size.height = topLeft.y - bottomRight.y;			
		}

		/**
		 * @brief Create a new Rectangle from the top-left point and the size.
		 * @param topLeft The top-left point of the rectangle.
		 * @param size The width and height of the rectangle.
		 */
		inline Rect(const Point2<T>& topLeft,
							const Size2<T>& size)
		: m_topLeft(topLeft), m_size(size) {}
		
		/**
		 * @brief Create a rectangle with the size and position.
		 * @param x The x coordinate of the top left corner.
		 * @param y The y coordinate of the top left corner.
		 * @param width The width of the rectangle.
		 * @param height The height of the rectangle.
		 */
		inline Rect(T x, T y,
							T width, T height)
		: m_topLeft(x, y), m_size(width, height) {}

		/**
		 * @brief Overloaded equality operator
		 * @param rhs The Rectangle to compare to.
		 * @return True if the rectangles are equal in size and position.
		 */
		inline Boolean operator==(const Rect<T>& rhs) const {
			return (m_topLeft == rhs.m_topLeft &&
					  m_size == rhs.m_size);
		}

		/**
		 * @brief Overloaded inequality operator
		 * @param rhs The Rectangle to compare to.
		 * @return True if the rectangles are NOT equal in size and position.
		 */
		inline Boolean operator!=(const Rect<T>& rhs) const {
			return (m_topLeft != rhs.m_topLeft ||
					  m_size != rhs.m_size);
		}

		/**
		 * @brief Get the y-coordinate of the bottom edge.
		 * @return The y-coordinate of the bottom edge.
		 */
		inline T bottom() const {
			return (m_topLeft.y - m_size.height);
		}

		/**
		 * @brief Get the bottom left corner of the rectangle.
		 * @return The bottom-left corner of the rectangle.
		 */
		inline Point2<T> bottomLeft() const {
			return Point2<T>(left(), bottom());
		}		

		/**
		 * @brief Get the bottom right corner of the rectangle.
		 * @return The bottom right corner of the rectangle.
		 */
		inline Point2<T> bottomRight() const { 
			return Point2<T>(right(), bottom()); 
		}

		/**
		 * @brief Get the coordinates of the centre of the rectangle.
		 * @return The Centre of the rectangle.
		 */
		inline Point2<Real> center() const {
			return Point2<Real>(m_topLeft.x + (REAL(0.5)*m_size.width),
									  m_topLeft.y - (REAL(0.5)*m_size.height));
		}

		/**
		 * @brief Test to see if a point is in the Rectangle.
		 * @param x The x-coordinate of the point.
		 * @param y The y-coordinate of the point.
		 * @return True if the point is in the rectangle.
		 */
		inline Boolean contains(Real x, Real y) const {
			return (x > m_topLeft.x &&
					  x < (m_topLeft.x + m_size.width) &&
					  y < m_topLeft.y &&
					  y > (m_topLeft.y - m_size.height));
		}

		/**
		 * @brief Test to see if a point is in the Rectangle.
		 * @param point The point to test to see if it is in the rectangle.
		 * @return True if the point is in the rectangle.
		 */
		inline Boolean contains(const Point2<T>& point) const {
			return contains((Real)point.x, (Real)point.y);			
		}

		/**
		 * @brief Test to see if a rectangle is contained in this rectangle.
		 * This method returns true only if the entire rectangle is contained 
		 * within this rectangle.
		 * @param rect The rectangle to test to see if it is in this rectangle.
		 * @return True if the rectangle is entirely contained within this rectangle.
		 */
		inline Boolean contains(const Rect<T>& rect) const {
			return (rect.m_topLeft.x > m_topLeft.x &&
					  rect.m_topLeft.y < m_topLeft.y &&
					  rect.right() < right() &&
					  rect.bottom() > bottom());			
		}

		/**
		 * @brief Get the height of the rectangle.
		 * @return The height of the rectangle.
		 */
		inline T height() const {
			return m_size.height;
		}

		/**
		 * @brief Check to see if another rectangle intersects this one.
		 * @param The rectangle to check for intersection.
		 * @return True if the other rectangle intersects this one.
		 */
		inline Boolean intersects(const Rect<T>& rect) const {
			return (rect.m_topLeft.x < right() &&
					  rect.m_topLeft.y > bottom() &&
					  rect.right() > m_topLeft.x &&
					  rect.bottom() < m_topLeft.y);
		}

		/**
		 * @brief Make this rectangle the intersection between it and another.
		 * @param rect The rectangle to intersect this rectangle with.
		 */
		void intersect(const Rect<T>& rect);

		/**
		 * @brief Get the intersection of this rectangle and another.
		 * @param rect The rectangle to intersect this rectangle with.
		 * @return The intersection of the two rectangles.
		 */
		Rect<T> intersection(const Rect<T>& rect) const;		

		/**
		 * @brief Check to see if the rectangle is empty.
		 * An empty rectangle has its width and height both set to zero.
		 * @return True if the rectangle is empty.
		 */
		inline Boolean isEmpty() const {
			return (m_size.width == 0 &&
					  m_size.height == 0);
		}

		/**
		 * @brief Check to see if the rectangle is a valid rectangle.
		 * A valid rectangle is a rectangle with positive, non-zero width and height.
		 * @return True if the rectangle is valid.
		 */
		inline Boolean isValid() const {
			return (m_size.width > 0 &&
					  m_size.height > 0);
		}

		/**
		 * @brief Get the x-coordinate of the top-left corner.
		 * @return The x-coordinate of the top left corner.
		 */
		inline T left() const {
			return m_topLeft.x;
		}

		/**
		 * @brief Move the rectangles Bottom edge to the specified y-coordinate, size does not change.
		 * @param y The y-coordinate to move the bottom edge to.
		 */
		inline void moveBottomTo(T y) {
			m_topLeft.y = y + m_size.height;
		}

		/**
		 * @brief Move the rectangles BottomLeft point to the specified point, size does not change.
		 * @param point The point to move the Bottom-left corner of the rectangle to.
		 */
		inline void moveBottomLeftTo(const Point2<T>& point) {
			m_topLeft.x = point.x;			
			m_topLeft.y = point.y + m_size.height;
		}

		/**
		 * @brief Move the rectangles BottomRight point to the specified point, size does not change.
		 * @param point The point to move the Bottom-right corner of the rectangle to.
		 */
		inline void moveBottomRightTo(const Point2<T>& point) {
			m_topLeft.x = point.x - m_size.width;			
			m_topLeft.y = point.y + m_size.height;
		}

		/**
		 * @brief Move the center of the rectangle to the specified point, size does not change.
		 * @param point The point to move the center of the rectangle to.
		 */
		inline void moveCenterTo(const Point2<T>& point) {
			m_topLeft.x = point.x - (m_size.width * 0.5);
			m_topLeft.y = point.y + (m_size.height * 0.5);
		}

		/**
		 * @brief Move the rectangles Left edge to the specified x-coordinate, size does not change.
		 * @param x The x-coordinate to move the left edge to.
		 */
		inline void moveLeftTo(T x) {
			m_topLeft.x = x;			
		}

		/**
		 * @brief Move the rectangles Right edge to the specified x-coordinate, size does not change.
		 * @param x The x-coordinate to move the right edge to.
		 */
		inline void moveRightTo(T x) {
			m_topLeft.x = x - m_size.width;			
		}

		/**
		 * @brief Move the rectangles Top-Left corner to the specified point, (x, y), size does not change.
		 * @param x The x-coordinate to move the top left corner to.
		 * @param y The y-coordinate to move the top left corner to.
		 */
		inline void moveTo(T x, T y) {
			m_topLeft.x = x;
			m_topLeft.y = y;			
		}

		/**
		 * @brief Move the rectangles Top-Left corner to the specified point, size does not change.
		 * @param point The point to move the top left corner to.
		 */
		inline void moveTo(const Point2<T>& point) {
			m_topLeft = point;			
		}

		/**
		 * @brief Move the rectangles Top edge to the specified y-coordinate, size does not change.
		 * @param y The y-coordinate to move the top edge to.
		 */
		inline void moveTopTo(T y) {
			m_topLeft.y = y;			
		}

		/**
		 * @brief Move the rectangles top-right point to the specified point, size does not change.
		 * @param point The point to move the top-right corner of the rectangle to.
		 */
		inline void moveTopRightTo(const Point2<T>& point) {
			m_topLeft.x = point.x - m_size.width;			
			m_topLeft.y = point.y;
		}

		/**
		 * @brief Normalize the rectangle, that is, make sure width and height are not negative.
		 * This method takes a rectangle and swaps the left/right and/or bottom/top sides to 
		 * make the rectangle have positive width/height.
		 */
		void normalise();		


		/**
		 * @brief Get the x-coordinate of the right edge.
		 * @return The x-coordinate of the right edge.
		 */
		inline T right() const {
			return (m_topLeft.x + m_size.width);
		}

		/**
		 * @brief Set the y-coordinate of the bottom edge, MAY change the height of the rectangle.
		 * @param y The y-coordinate to set the bottom edge to.
		 */
		inline void setBottom(T y) {
			m_size.height = m_topLeft.y - y;
		}

		/**
		 * @brief Set the position of the bottom-left corner, MAY change the width and/or height.
		 * @param pos The position to set the bottom-left corner to.
		 */
		inline void setBottomLeft(const Point2<T>& pos) {
			m_size.height = m_topLeft.y - pos.y;
			setLeft(pos.x);			
		}

		/**
		 * @brief Set the position of the bottom-right corner, MAY change the width and/or height.
		 * @param pos The position to set the bottom-right corner to.
		 */
		inline void setBottomRight(const Point2<T>& pos) {
			m_size.height = m_topLeft.y - pos.y;
			m_size.width =  pos.x - m_topLeft.x;			
		}

		/**
		 * @brief Set the height of the rectangle.  
		 * This method sets the height of the rectangle, without changing the position of the top edge.
		 * @param height The height to set the rectangle to.
		 */
		inline void setHeight(T height) {
			m_size.height = height;
		}

		/**
		 * @brief Set the x-coordinate of the left edge, MAY change the width of the rectangle.
		 * @param x The x-coordinate to set the left edge to.
		 */
		inline void setLeft(T x) {
			m_size.width -= x - m_topLeft.x;			
			m_topLeft.x = x;			
		}

		/**
		 * @brief Set the top-left corner and the size of the rectangle.
		 * @param x The x-coordinate of the top left corner.
		 * @param y The y-coordinate of the top left corner.
		 * @param width The width of the rectangle.
		 * @param height The height of the rectangle.
		 */
		inline void setRect(T x, T y, T width, T height) {
			m_topLeft.x = x;
			m_topLeft.y = y;
			m_size.width = width;
			m_size.height = height;
		}

		/**
		 * @brief Set the x-coordinate of the right edge, MAY change the width of the rectangle.
		 * @param x The x-coordinate to set the right edge to.
		 */
		inline void setRight(T x) {
			m_size.width = x - m_topLeft.x;			
		}

		/**
		 * @brief Set the width and height of the rectangle.
		 * @param width The width of the rectangle to set to.
		 * @param height The value to set the rectangle height to.
		 */
		inline void setSize(T width, T height) {
			m_size.width = width;
			m_size.height = height;			
		}

		/**
		 * @brief Set the width and height of the rectangle.
		 * @param width The width of the rectangle to set to.
		 * @param height The value to set the rectangle height to.
		 */
		inline void setSize(const Size2<T>& size) {
			setSize(size.width, size.height);
		}		

		/**
		 * @brief Set the y-coordinate of the top edge, MAY change the height of the rectangle.
		 * @param y The y-coordinate to set the top edge to.
		 */
		inline void setTop(T y) {
			m_size.height -= m_topLeft.y - y;			
			m_topLeft.y = y;			
		}

		/**
		 * @brief Set the position of the top-left corner, MAY change the width and/or height.
		 * @param pos The position to set the top-left corner to.
		 */
		inline void setTopLeft(const Point2<T>& pos) {
			setTop(pos.y);
			setLeft(pos.x);						
		}

		/**
		 * @brief Set the position of the top-right corner, MAY change the width and/or height.
		 * @param pos The position to set the top-right corner to.
		 */
		inline void setTopRight(const Point2<T>& pos) {
			setTop(pos.y);
			setRight(pos.x);						
		}

		/**
		 * @brief Set the width of the rectangle.
		 * This method sets the width of the rectangle, while keeping the left edge the same.
		 * @param width The width to set the rectangle to.
		 */
		inline void setWidth(T width) {
			m_size.width = width;
		}		
		
		/**
		 * @brief Get the width and height of the rectangle.
		 * @return The Width and Height of the rectangle.
		 */
		inline const Size2<T>& size() const {
			return m_size;
		}

	   /**
		 * @brief Get the y-coordinate of the top-left corner.
		 * @return The y-coordinate of the top left corner.
		 */
		inline T top() const {
			return m_topLeft.y;
		}

		/**
		 * @brief Get the top left corner of the rectangle.
		 * @return The top left corner of the rectangle.
		 */
		inline const Point2<T>& topLeft() const {
			return m_topLeft;
		}

		/**
		 * @brief Get the top right corner of the rectangle.
		 * @return The top right corner of the rectangle.
		 */
		inline Point2<T> topRight() const {
			return Point2<T>(right(), m_topLeft.y);
		}

		/**
		 * @brief Translate the rectangle by the specified amount.
		 * @param dx The amount to translate in the x direction.
		 * @param dy The amount to translate in the y-direction.
		 */
		inline void translate(T dx, T dy) {
			m_topLeft.x += dx;
			m_topLeft.y += dy;
		}
		
		/**
		 * @brief Translate the rectangle by the specified amount.
		 * @param offset The amount to translate the rectangle by.
		 */
		inline void translate(const Vec2& offset) {
			m_topLeft += offset;			
		}

		/**
		 * @brief Get the rectangle translated the rectangle by the specified amount.
		 * @param dx The amount to translate in the x direction.
		 * @param dy The amount to translate in the y-direction.
		 * @return A copy of the translated rectangle.
		 */
		inline Rect<T> translated(T dx, T dy) const {
			return Rect<T>(m_topLeft.x + dx, m_topLeft.y + dy, m_size.width, m_size.height);
		}
		
		/**
		 * @brief Get the rectangle translated the rectangle by the specified amount.
		 * @param offset The amount to translate the rectangle by.
		 * @return A copy of the translated rectangle.
		 */
		inline Rect<T> translated(const Vec2& offset) {
			return translated((T)offset.x, (T)offset.y);			
		}

		/**
		 * @brief Get a rectangle that is the bounding rectangle of the union of the two rectangles.
		 * @param rect The rectangle to union with.
		 * @return The bounding rectangle containing both rectangles.
		 */
		Rect<T> unionedWithRect(const Rect<T>& rect) const;

		/**
		 * @brief Set to the union of this and another rectangle.
		 * @param rect The rectangle to union with.
		 */
		void unionWithRect(const Rect<T>& rect);	

		/**
		 * @brief Get the width of the rectangle.
		 * @return The width of the rectangle.
		 */
		inline T width() const {
			return m_size.width;
		}

		/**
		 * @brief Get the x-coordinate of the top-left corner.
		 * @return The x-coordinate of the top left corner.
		 */
		inline T x() const {
			return m_topLeft.x;
		}

		/**
		 * @brief Get the y-coordinate of the top-left corner.
		 * @return The y-coordinate of the top left corner.
		 */
		inline T y() const {
			return m_topLeft.y;
		}

		/**
		 * @brief Helper method to create a Rectangle from a centre point.
		 * @param center The Centre of the rectangle.
		 * @param width The width of the Rectangle.
		 * @param height The height of the Rectangle.
		 * @return A new Rectangle cetered at center.
		 */
		inline static Rect<T> fromCenter(const Point2<T>& center,
													T width, T height) {
			return Rect<T>(center.x - width*0.5,
								center.y + height*0.5,
								width, height);
		}

		/**
		 * @brief Helper method to create a Rectangle from a centre point.
		 * @param center The Centre of the rectangle.
		 * @param size The size of the Rectangle.
		 * @return A new Rectangle cetered at center.
		 */
		inline static Rect<T> fromCenter(const Point2<T>& center,
													const Size2<T>& size) {
			return Rect<T>(center.x - size.width*0.5,
								center.y + size.height*0.5,
								size.width, size.height);
		}

		/**
		 * @brief Helper method to create a Rectangle from a centre point.
		 * @param cx The x-coordinate of the centre of the rectangle.
		 * @param cy The y-coordinate of the centre of the rectangle.
		 * @param width The width of the Rectangle.
		 * @param height The height of the Rectangle.
		 * @return A new Rectangle cetered at center.
		 */
		inline static Rect<T> fromCenter(T cx, T cy,
													T width, T height) {
			return Rect<T>(cx - width*0.5,
								cy + height*0.5,
								width, height);
		}

#ifdef DEBUG
		friend std::ostream& operator<<(std::ostream& out, const Rect<T>& r) {
			return out << r.m_size.width
						  << " x "
						  << r.m_size.height
						  << " "
						  << r.m_topLeft;
		}	
#endif // DEBUG 

	  protected:
		Point2<T> m_topLeft;
		Size2<T> m_size;		
	};

	template <typename T>
	void Rect<T>::intersect(const Rect<T>& rect) {
		if (intersects(rect)) {
			Point2<T> topLeftPoint(m_topLeft);			
			if (rect.m_topLeft.x > topLeftPoint.x) {
				topLeftPoint.x = rect.m_topLeft.x;
			}
			if (rect.m_topLeft.y < topLeftPoint.y) {
				topLeftPoint.y = rect.m_topLeft.y;
			}
			Point2<T> bottomRightPoint(bottomRight());	
			if (rect.right() < bottomRightPoint.x) {
				bottomRightPoint.x = rect.right();
			}
			if (rect.bottom() > bottomRightPoint.y) {
				bottomRightPoint.y = rect.bottom();
			}
			m_topLeft = topLeftPoint;
			m_size.width = bottomRightPoint.x - topLeftPoint.x;
			m_size.height = topLeftPoint.y - bottomRightPoint.y;			
		}
		else {
			m_topLeft.x = 0;
			m_topLeft.y = 0;
			m_size.width = 0;
			m_size.height = 0;
		}		
			
	}

	template <typename T>
	Rect<T> Rect<T>::intersection(const Rect<T>& rect) const {
		if (intersects(rect)) {
			Point2<T> topLeftPoint(m_topLeft);			
			if (rect.m_topLeft.x > topLeftPoint.x) {
				topLeftPoint.x = rect.m_topLeft.x;
			}
			if (rect.m_topLeft.y < topLeftPoint.y) {
				topLeftPoint.y = rect.m_topLeft.y;
			}
			Point2<T> bottomRightPoint(bottomRight());	
			if (rect.right() < bottomRightPoint.x) {
				bottomRightPoint.x = rect.right();
			}
			if (rect.bottom() > bottomRightPoint.y) {
				bottomRightPoint.y = rect.bottom();
			}
			return Rect<T>(topLeftPoint, bottomRightPoint);
		}
		else {
			return Rect<T>();			
		}
	}
			
	template <typename T>
	void Rect<T>::normalise() {
		if (m_size.width < 0) {
			m_topLeft.x += m_size.width;
			m_size.width *= -1;
		}
		if (m_size.height < 0) {
			m_topLeft.y -= m_size.height;
			m_size.height *= -1;
		}		
	}

	template <typename T>
	Rect<T> Rect<T>::unionedWithRect(const Rect<T>& rect) const {
	   Point2<T> topLeftPoint(m_topLeft);
		if (rect.top() > topLeftPoint.y) {
			topLeftPoint.y = rect.top();
		}
		if (rect.left() < topLeftPoint.x) {
			topLeftPoint.x = rect.left();
		}

		Point2<T> bottomRightPoint(bottomRight());
		if (rect.right() > bottomRightPoint.x) {
			bottomRightPoint.x = rect.right();
		}
		if (rect.bottom() < bottomRightPoint.y) {
			bottomRightPoint.y = rect.bottom();
		}
		
		return Rect<T>(topLeftPoint, bottomRightPoint);
	}

	template <typename T>
	void Rect<T>::unionWithRect(const Rect<T>& rect) {
	   Point2<T> topLeftPoint(m_topLeft);
		if (rect.top() > topLeftPoint.y) {
			topLeftPoint.y = rect.top();
		}
		if (rect.left() < topLeftPoint.x) {
			topLeftPoint.x = rect.left();
		}

		Point2<T> bottomRightPoint(bottomRight());
		if (rect.right() > bottomRightPoint.x) {
			bottomRightPoint.x = rect.right();
		}
		if (rect.bottom() < bottomRightPoint.y) {
			bottomRightPoint.y = rect.bottom();
		}
		m_topLeft = topLeftPoint;
		m_size.width = bottomRightPoint.x - topLeftPoint.y;
		m_size.height = topLeftPoint.y - bottomRightPoint.y;
	}
	

} // namespace Cat
#endif // CAT_CORE_GEOMETRY_RECT_H
