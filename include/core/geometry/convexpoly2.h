#ifndef CAT_CORE_GEOMETRY_CONVEXPOLY2_H
#define CAT_CORE_GEOMETRY_CONVEXPOLY2_H
/**
 * @copyright Copyright Catlin Zilinksi, 2014.  All rights reserved.
 *
 * @file convexpoly2.h
 * @brief Contains a polygon class template.
 *
 * @author Catlin Zilinski
 * @date Sept 3, 2014
 */

#include "core/geometry/rect.h"
#include "core/util/vector.h"

namespace Cat {

	/**
	 * @class ConvexPoly2 convexpoly2.h "core/geometry/convexpoly2.h"
	 * @brief Contains a 2D convex polygon class template.
	 *
	 * A 2D Convex Polygon class template with methods for 
	 * creating and working with the convex polygon.
	 *
	 * @author Catlin Zilinski
	 * @version 2
	 * @since July 26, 2014
	 */
	template<typename T>
	class ConvexPoly2 {
	  public:
		/**
		 * @brief Create an empty polygon.
		 */
		inline ConvexPoly2() {}

		/**
		 * @brief Create an empty polygon and initialise the vector.
		 * @param size The size to initialise the vector of points to.
		 */
		explicit inline ConvexPoly2(Size size) {
			m_points.initVectorWithCapacity(size);
		}

		/**
		 * @brief Create a polygon from a vector of points.
		 * We assume that the points are in clockwise order.
		 * The contents of the vector are copied.
		 * @param points The Vector of points.
		 */
		explicit ConvexPoly2(const Vector< Point2<T> >& vector);

		/**
		 * @brief Create a polygon from a rectangle.
		 * @param rect The rectangle to create the polygon from.
		 */
		explicit ConvexPoly2(const Rect<T>& rect);

		/**
		 * @brief constructor to copy a translated polygon.
		 * @param src The polygon to copy.
		 * @param dx The amount to translate in the x direction.
		 * @param dy The amount to translate in the y direction.
		 */
		ConvexPoly2(const ConvexPoly2<T>& src, T dx, T dy);		

		/**
		 * @brief Copy constructor for polygons.
		 * @param src The source polygon to copy.
		 */
		ConvexPoly2(const ConvexPoly2<T>& src);

		/**
		 * @brief overloaded assignment operator.  
		 * @param rhs The polygon to copy from.
		 */
		ConvexPoly2<T>& operator=(const ConvexPoly2<T>& rhs);
		
		/**
		 * @brief Overloaded equality operator
		 * @param rhs The ConvexPolygon to compare to.
		 * @return True if the polygons are equal in size and position.
		 */
		Boolean operator==(const ConvexPoly2<T>& rhs) const;

		/**
		 * @brief Overloaded inequality operator
		 * @param rhs The ConvexPolygon to compare to.
		 * @return True if the polygons are NOT equal in size and position.
		 */
		Boolean operator!=(const ConvexPoly2<T>& rhs) const;

		/**
		 * @brief Check to see if two polygons are approximately equal.
		 * @param rhs The polygon to check for approximate equality.
		 * @return true if the two polygons are approximately equal.
		 */
		Boolean approx(const ConvexPoly2<T>& rhs) const;		

		/**
		 * @brief Get the y-coordinate of the bottom-most point of the polygon.
		 * @return The y-coordinate of the bottom-most point of the polygon.
		 */
		inline T bottom() const {
			return m_bounds.bottom();
		}
		
		/**
		 * @brief Test to see if a point is in the ConvexPolygon.
		 * @param point The point to test to see if it is in the polygon.
		 * @return True if the point is in the polygon.
		 */
		Boolean contains(const Point2<T>& point) const;

		/**
		 * @brief Test to see if a point is in the ConvexPolygon.
		 * @param x The x-coordinate of the point.
		 * @param y The y-coordinate of the point.
		 * @return True if the point is in the polygon.
		 */
		inline Boolean contains(Real x, Real y) const {
			return contains(Point2<T>((T)x, (T)y));
		}

		/**
		 * @brief Test to see if a polygon is contained in this polygon.
		 * This method returns true only if the entire polygon is contained 
		 * within this polygon.
		 * @param poly The polygon to test to see if it is in this polygon.
		 * @return True if the polygon is entirely contained within this polygon.
		 */
		Boolean contains(const ConvexPoly2<T>& poly) const;

		/**
		 * @brief Get the number of points in the polygon.
		 * @return The number of points in the polygon.
		 */
		inline Size count() const {
			return m_points.size();
		}

		/**
		 * @brief Get the height of the polygon.
		 * @return The height of the polygon.
		 */
		inline T height() const {
			return m_bounds.height();
		}

		/**
		 * @brief Check to see if another polygon intersects this one.
		 * @param The polygon to check for intersection.
		 * @return True if the other polygon intersects this one.
		 */
		//	Boolean intersects(const ConvexPoly2<T>& poly) const;
		
		/**
		 * @brief Make this polygon the intersection between it and another.
		 * @param poly The polygon to intersect this polygon with.
		 */
		//	void intersect(const ConvexPoly2<T>& poly);

		/**
		 * @brief Get the intersection of this polygon and another.
		 * @param poly The polygon to intersect this polygon with.
		 * @return The intersection of the two polygons.
		 */
		//	ConvexPoly2<T> intersection(const ConvexPoly2<T>& poly) const;		

		/**
		 * @brief Check to see if the polygon is empty.
		 * An empty polygon has its width and height both set to zero.
		 * @return True if the polygon is empty.
		 */
		inline Boolean isEmpty() const {
			return m_points.size() == 0 || m_bounds.isEmpty();			
		}

		/**
		 * @brief Check to see if the polygon is a valid polygon.
		 * A valid polygon is a polygon with positive, non-zero width and height.
		 * @return True if the polygon is valid.
		 */
		inline Boolean isValid() const {
			return !isEmpty();			
		}

		/**
		 * @brief Get the x-coordinate of the left-most point of the polygon.
		 * @return The x-coordinate of the left-most point of the polygon.
		 */
		inline T left() const {
			return m_bounds.left();
		}

		/**
		 * @brief Move the polygons Bottom edge to the specified y-coordinate, size does not change.
		 * @param y The y-coordinate to move the bottom edge to.
		 */
		void moveBottomTo(T y);		

		/**
		 * @brief Move the polygons BottomLeft point to the specified point, size does not change.
		 * @param point The point to move the Bottom-left corner of the polygon to.
		 */
		void moveBottomLeftTo(const Point2<T>& point);		

		/**
		 * @brief Move the polygons BottomRight point to the specified point, size does not change.
		 * @param point The point to move the Bottom-right corner of the polygon to.
		 */
		void moveBottomRightTo(const Point2<T>& point);
		
		/**
		 * @brief Move the center of the polygon to the specified point, size does not change.
		 * @param point The point to move the center of the polygon to.
		 */
		void moveCenterTo(const Point2<T>& point);		

		/**
		 * @brief Move the polygons Left edge to the specified x-coordinate, size does not change.
		 * @param x The x-coordinate to move the left edge to.
		 */
		void moveLeftTo(T x);		

		/**
		 * @brief Move the polygons Right edge to the specified x-coordinate, size does not change.
		 * @param x The x-coordinate to move the right edge to.
		 */
		void moveRightTo(T x);		

		/**
		 * @brief Move the polygons Top-Left corner to the specified point, (x, y), size does not change.
		 * @param x The x-coordinate to move the top left corner to.
		 * @param y The y-coordinate to move the top left corner to.
		 */
		void moveTo(T x, T y);		

		/**
		 * @brief Move the polygons Top-Left corner to the specified point, size does not change.
		 * @param point The point to move the top left corner to.
		 */
		inline void moveTo(const Point2<T>& point) {
			moveTo(point.x, point.y);
		}

		/**
		 * @brief Move the polygons Top edge to the specified y-coordinate, size does not change.
		 * @param y The y-coordinate to move the top edge to.
		 */
		void moveTopTo(T y);		

		/**
		 * @brief Move the polygons top-right point to the specified point, size does not change.
		 * @param point The point to move the top-right corner of the polygon to.
		 */
		void moveTopRightTo(const Point2<T>& point);

		/**
		 * @brief Get a point in the polygon.
		 * @param idx The index of the point.
		 * @return The point in the polygon.
		 */
		inline const Point2<T>& pointAt(I32 idx) const {
			return m_points.at(idx);
		}

		/**
		 * @brief Get the bounding rectangle for this polygon.
		 * @return The bounding rectangle for this polygon.
		 */
		inline const Rect<T>& rect() const {
			return m_bounds;
		}

		/**
		 * @brief Check to see if a point is inside the polygon's bounding rectangle.
		 * @param point The point to test.
		 * @return True if the point is in the bounding rectangle for the polygon.
		 */
		inline Boolean rectContains(const Point2<T>& point) const {
			return m_bounds.contains(point);
		}

		/**
		 * @brief Check to see if a point is inside the polygon's bounding rectangle.
		 * @param x The floating point x-coordinate of the point.
		 * @param y The floating point y-coordinate of the point.
		 * @return True if the point is in the bounding rectangle for the polygon.
		 */
		inline Boolean rectContains(Real x, Real y) const {
			return m_bounds.contains(x, y);
		}

		/**
		 * @brief Get the x-coordinate of the right-most point of the polygon.
		 * @return The x-coordinate of the right-most point of the polygon.
		 */
		inline T right() const {
			return m_bounds.right();
		}

		/**
		 * @brief Set the y-coordinate of the bottom edge.
		 * This method will set the y coordinate of the bottom most point of the 
		 * polygon, and then scale the rest of the points relative to the top of the 
		 * polygon.  I.E., the top most point will not move.
		 * @param y The y-coordinate to set the bottom edge to.
		 */
		void setBottom(T y);		

		/**
		 * @brief Set the position of the bottom-left corner.
		 * @see setBottom(T)
		 * @see setLeft(T)
		 * @param pos The position to set the bottom-left corner to.
		 */
		inline void setBottomLeft(const Point2<T>& pos) {
			setBottom(pos.y);
			setLeft(pos.x);
		}		
		
		/**
		 * @brief Set the position of the bottom-right corner.
		 * @see setBottom(y)
		 * @see setRight(x)
		 * @param pos The position to set the bottom-right corner to.
		 */
		inline void setBottomRight(const Point2<T>& pos) {
			setBottom(pos.y);
			setRight(pos.x);			
		}

		/**
		 * @brief Set the height of the polygon.  
		 * This method sets the height of the polygon and scales relative to the
		 * top left corner of the polygon's bounds.
		 * @param height The height to set the polygon to.
		 */
		void setHeight(T height);		

		/**
		 * @brief Set the x-coordinate of the left edge.
		 * This method will set the x coordinate of the left most point of the 
		 * polygon, and then scale the rest of the points relative to the right of the 
		 * polygon.  I.E., the right most point will not move.
		 * @param x The x-coordinate to set the left edge to.
		 */
		void setLeft(T x);

		/**
		 * @brief Set the top-left corner and the size of the polygon.
		 * @param x The x-coordinate of the top left corner.
		 * @param y The y-coordinate of the top left corner.
		 * @param width The width of the polygon.
		 * @param height The height of the polygon.
		 */
		inline void setBounds(const Rect<T> bounds) {
			setSize(bounds.size());
			moveTo(bounds.topLeft());
		}		

		/**
		 * @brief Set the x-coordinate of the right edge.
		 * This method will set the x coordinate of the right most point of the 
		 * polygon, and then scale the rest of the points relative to the left of the 
		 * polygon.  I.E., the left most point will not move.
		 * @param x The x-coordinate to set the right edge to.
		 */
		void setRight(T x);

		/**
		 * @brief Set the width and height of the polygon.
		 * @param width The width of the polygon to set to.
		 * @param height The value to set the polygon height to.
		 */
		inline void setSize(T pWidth, T pHeight) {
			setWidth(pWidth);
			setHeight(pHeight);
		}		

		/**
		 * @brief Set the width and height of the polygon.
		 * @param width The width of the polygon to set to.
		 * @param height The value to set the polygon height to.
		 */
		inline void setSize(const Size2<T>& pSize) {
			setSize(pSize.width, pSize.height);
		}

		/**
		 * @brief Set the polygon to be the convex hull of a vector of points.
		 * @param points The vector of points get get the convex hull of.
		 */
		void setToConvexHull(const Vector< Point2<T> >& points);		

		/**
		 * @brief Set the y-coordinate of the top edge.
		 * This method will set the y coordinate of the top most point of the 
		 * polygon, and then scale the rest of the points relative to the bottom of the 
		 * polygon.  I.E., the bottom most point will not move.
		 * @param y The y-coordinate to set the top edge to.
		 */
		void setTop(T y);		

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
		 * @brief Set the width of the polygon.
		 * This method sets the width of the polygon, while keeping the left edge the same.
		 * @param width The width to set the polygon to.
		 */
		void setWidth(T width);		
		
		/**
		 * @brief Get the width and height of the polygon.
		 * @return The Width and Height of the polygon.
		 */
		inline const Size2<T>& size() const {
			return m_bounds.size();
		}

	   /**
		 * @brief Get the y-coordinate of the top-left corner.
		 * @return The y-coordinate of the top left corner.
		 */
		inline T top() const {
			return m_bounds.top();
		}

		/**
		 * @brief Get the top left corner of the polygon.
		 * @return The top left corner of the polygon.
		 */
		inline const Point2<T>& topLeft() const {
			return m_bounds.topLeft();			
		}

		/**
		 * @brief Get the top right corner of the polygon.
		 * @return The top right corner of the polygon.
		 */
		inline Point2<T> topRight() const {
			return m_bounds.topRight();			
		}

		/**
		 * @brief Translate the polygon by the specified amount.
		 * @param dx The amount to translate in the x dipolyion.
		 * @param dy The amount to translate in the y-dipolyion.
		 */
		void translate(T dx, T dy);		
		
		/**
		 * @brief Translate the polygon by the specified amount.
		 * @param offset The amount to translate the polygon by.
		 */
		inline void translate(const Vec2& offset) {
		   translate((T)offset.x, (T)offset.y);			
		}

		/**
		 * @brief Get the polygon translated the polygon by the specified amount.
		 * @param dx The amount to translate in the x dipolyion.
		 * @param dy The amount to translate in the y-dipolyion.
		 * @return A copy of the translated polygon.
		 */
		inline ConvexPoly2<T> translated(T dx, T dy) const {
			return ConvexPoly2<T>(*this, dx, dy);
		}		
		
		/**
		 * @brief Get the polygon translated the polygon by the specified amount.
		 * @param offset The amount to translate the polygon by.
		 * @return A copy of the translated polygon.
		 */
		inline ConvexPoly2<T> translated(const Vec2& offset) const {
			return translated((T)offset.x, (T)offset.y);			
		}

		/**
		 * @brief Set to the union of this and another polygon.
		 * @param poly The polygon to union with.
		 */
		void unionWith(const ConvexPoly2<T>& poly);	

		/**
		 * @brief Get a polygon that is the bounding polygon of the union of the two polygons.
		 * @param poly The polygon to union with.
		 * @return The bounding polygon containing both polygons.
		 */
		ConvexPoly2<T> unionedWith(const ConvexPoly2<T>& poly) const;

		/**
		 * @brief Get the width of the polygon.
		 * @return The width of the polygon.
		 */
		inline T width() const {
			return m_bounds.width();
		}

		/**
		 * @brief Get the x-coordinate of the top-left corner.
		 * @return The x-coordinate of the top left corner.
		 */
		inline T x() const {
			return m_bounds.x();
		}

		/**
		 * @brief Get the y-coordinate of the top-left corner.
		 * @return The y-coordinate of the top left corner.
		 */
		inline T y() const {
			return m_bounds.y();
		}

		/**
		 * @brief Static method to create a polygon from a rectangle.
		 * @param rect The rectangle to create the polygon from.
		 * @return A polygon represeting the rectangle.
		 */
		static inline ConvexPoly2<T> fromRect(const Rect<T>& rect) {
			return ConvexPoly2<T>(rect);
		}		
		

#ifdef DEBUG
		friend std::ostream& operator<<(std::ostream& out, const ConvexPoly2<T>& p) {
			if (p.count() == 0) {
				out << "Polygon[]";
			}
			else {
				out << "Polygon[";				
				for (Size i = 0; i < p.count(); ++i) {
					if (i > 0) {
						out << ", ";
					}					
					out << p.pointAt(i);
				}
				out << "]";
			}
			return out;
		}	
#endif // DEBUG 

	  protected:
		Vector< Point2<T> > m_points;
		Rect<T> m_bounds;
	};

	template<typename T>
	ConvexPoly2<T>::ConvexPoly2(const Vector< Point2<T> >& vector) {
		setToConvexHull(vector);		
	}

	template <typename T>
	ConvexPoly2<T>::ConvexPoly2(const Rect<T>& rect) {
		m_points.initVectorWithCapacity(4);		
		m_points.set(0, rect.topLeft());
		m_points.set(1, rect.topRight());
		m_points.set(2, rect.bottomRight());
		m_points.set(3, rect.bottomLeft());
		m_bounds = rect;
	}

	template <typename T>
	ConvexPoly2<T>::ConvexPoly2(const ConvexPoly2<T>& src, T dx, T dy) {
		m_points = src.m_points;
		m_bounds = src.m_bounds;
		translate(dx, dy);		
	}
	
	template<typename T>
	ConvexPoly2<T>::ConvexPoly2(const ConvexPoly2<T>& src) {
		m_points = src.m_points;
		m_bounds = src.m_bounds;		
	}
	
	template<typename T>
	ConvexPoly2<T>& ConvexPoly2<T>::operator=(const ConvexPoly2<T>& src) {
		m_points = src.m_points;
		m_bounds = src.m_bounds;
		return *this;		
	}

	template<typename T>
	Boolean ConvexPoly2<T>::operator==(const ConvexPoly2<T>& rhs) const {
		/* First check size */
		if (m_points.size() != rhs.m_points.size() || m_bounds != rhs.m_bounds) {
			return false;
		}
		else { 	/* Then check the actual vectors */
			for (Size i = 0; i < m_points.size(); ++i) {
				if (m_points.get(i) != rhs.m_points.get(i)) {
					return false;
				}
			}
		}
		return true;		
	}

	template<typename T>
	Boolean ConvexPoly2<T>::operator!=(const ConvexPoly2<T>& rhs) const {
		return !((*this) == rhs);
	}

	template <typename T>
	Boolean ConvexPoly2<T>::approx(const ConvexPoly2<T>& rhs) const {
		/* First check size */
		if (m_points.size() != rhs.m_points.size()) {
			return false;
		}
		else { 	/* Then check the actual vectors */
			for (Size i = 0; i < m_points.size(); ++i) {
				if (!cc::approx(m_points.get(i).x, rhs.m_points.get(i).x) ||
					 !cc::approx(m_points.get(i).y, rhs.m_points.get(i).y)) {
					return false;
				}
			}
		}
		return true;
	}	

	template<typename T>
	Boolean ConvexPoly2<T>::contains(const Point2<T>& point) const {
		for (Size i = 1; i < m_points.size(); ++i) {
			if (Point2<T>::isLeftTurn(m_points.get(i-1), m_points.get(i), point)) {
				return false;
			}
		}
		/* Check final line */
		if (Point2<T>::isLeftTurn(m_points.last(), m_points.get(0), point)) {
			return false;
		}		
		return true;		
	}	

	template<typename T>
	Boolean ConvexPoly2<T>::contains(const ConvexPoly2<T>& poly) const {
		if (m_bounds == poly.m_bounds || m_bounds.contains(poly.m_bounds)) {
			Vector< Point2<T> > points(m_points);
			points.reserve(m_points.size() + poly.m_points.size());
			for (Size i = 0; i < poly.m_points.size(); ++i) {
				points.append(poly.m_points.get(i));
			}
			ConvexPoly2<T> test_poly(points);
			return (*this) == test_poly;
		}
		else {
			return false;
		}		
	}

	/**	template<typename T>
			Boolean ConvexPoly2<T>::intersects(const ConvexPoly2<T>& poly) const {

			return false;		
			}

			template<typename T>
			void ConvexPoly2<T>::intersect(const ConvexPoly2<T>& poly) {
			}

			template<typename T>
			ConvexPoly2<T> ConvexPoly2<T>::intersection(const ConvexPoly2<T>& poly) const {
			return ConvexPoly2<T>(*this);		
			}
	**/

	template<typename T>
	void ConvexPoly2<T>::moveBottomTo(T y) {
		translate(0, y - m_bounds.bottom());		
	}

	template<typename T>
	void ConvexPoly2<T>::moveBottomLeftTo(const Point2<T>& point) {
		translate(point.x - m_bounds.left(), point.y - m_bounds.bottom());		
	}	

	template<typename T>
	void ConvexPoly2<T>::moveBottomRightTo(const Point2<T>& point) {
	   translate(point.x - m_bounds.right(), point.y - m_bounds.bottom());
	}

	template<typename T>
	void ConvexPoly2<T>::moveCenterTo(const Point2<T>& point) {
		Point2<T> center_point = m_bounds.center();
	   translate(point.x - center_point.x, point.y - center_point.y);
	}

	template<typename T>
	void ConvexPoly2<T>::moveLeftTo(T x) {
		translate(x - m_bounds.left(), 0);
	}

	template<typename T>
	void ConvexPoly2<T>::moveRightTo(T x) {
		translate(x - m_bounds.right(), 0);
	}

	template<typename T>
	void ConvexPoly2<T>::moveTo(T x, T y) {
		translate(x - m_bounds.left(), y - m_bounds.top());
	}

	template<typename T>
	void ConvexPoly2<T>::moveTopTo(T y) {
		translate(0, y - m_bounds.top());
	}

	template <typename T>
	void ConvexPoly2<T>::moveTopRightTo(const Point2<T>& point) {
		translate(point.x - m_bounds.right(), point.y - m_bounds.top());		
	}
	

	template<typename T>
	void ConvexPoly2<T>::setBottom(T y) {
	   T dy = y - m_bounds.bottom();
		for (Size i = 0; i < m_points.size(); ++i) {
			m_points.at(i).y += (T)(((Real)(m_bounds.top() - m_points.at(i).y) / (Real)m_bounds.height())*dy);
		}
		m_bounds.setBottom(y);
	}

	template<typename T>
	void ConvexPoly2<T>::setHeight(T height) {
		setBottom(m_bounds.bottom() - (height - m_bounds.height()));		
	}

	template<typename T>
	void ConvexPoly2<T>::setLeft(T x) {
		T dx = x - m_bounds.left();
		for (Size i = 0; i < m_points.size(); ++i) {
			m_points.at(i).x += (T)(((Real)(m_bounds.right() - m_points.at(i).x) / (Real)m_bounds.width())*dx);
		}
		m_bounds.setLeft(x);
	}

	template<typename T>
	void ConvexPoly2<T>::setRight(T x) {
		T dx = x - m_bounds.right();
		for (Size i = 0; i < m_points.size(); ++i) {
			m_points.at(i).x += (T)(((Real)(m_points.at(i).x - m_bounds.left()) / (Real)m_bounds.width())*dx);
		}
		m_bounds.setRight(x);
	}

	template<typename T>
	void ConvexPoly2<T>::setToConvexHull(const Vector< Point2<T> >& points) {
		/* First, sort the points */
		Vector< Point2<T> > sorted_points(points);
		sorted_points.sort(&(Point2<T>::compare));
		m_bounds = Rect<T>(sorted_points.get(0), sorted_points.last());

		/* If <= 2 points, then cannot have polygon. */
		if (sorted_points.size() <= 2) {
			m_points = sorted_points;
			return;			
		}

		/* Make sure have enough room for the points */
		m_points.clear();
		m_points.reserve(sorted_points.size());

		/* Create the top of the convex hull. */
		m_points.append(sorted_points.get(0));
		m_points.append(sorted_points.get(1));

		if (m_points.last().y > m_bounds.top()) {
			m_bounds.setTop(m_points.last().y);
		}		
		for (Size i = 2; i < sorted_points.size(); ++i) {			
			do {
				if (!Point2<T>::isRightTurn(m_points.get(m_points.size()-2),
													 m_points.last(),
													 sorted_points.get(i))) {
					m_points.removeLast();
				}
				else {
					break;
				}				
			} while (m_points.size() >= 2);
			m_points.append(sorted_points.get(i));
			if (m_points.last().y > m_bounds.top()) {
				m_bounds.setTop(m_points.last().y);
			}
		}

		/* Create the bottom of the convex hull. */
		if (m_points.last().y < m_bounds.bottom()) {
			m_bounds.setBottom(m_points.last().y);
		}
		Size min_size = m_points.size();		
		for (I32 i = sorted_points.size() - 2; i >= 0; --i) {
			do {
				if (!Point2<T>::isRightTurn(m_points.get(m_points.size()-2),
													 m_points.last(),
													 sorted_points.get(i)) && m_points.size() > min_size) {
					m_points.removeLast();
				}
				else {
					break;
				}				
			} while (m_points.size() >= min_size);
			if (i > 0) {				
				m_points.append(sorted_points.get(i));
				if (m_points.last().y < m_bounds.bottom()) {
					m_bounds.setBottom(m_points.last().y);
				}
			}					
		}
		if (m_points.get(0).y < m_bounds.bottom()) {
			m_bounds.setBottom(m_points.get(0).y);
		}
	}
		
	template<typename T>
	void ConvexPoly2<T>::setTop(T y) {
		T dy = y - m_bounds.top();
		for (Size i = 0; i < m_points.size(); ++i) {
			m_points.at(i).y += (T)(((Real)(m_points.at(i).y - m_bounds.bottom()) / (Real)m_bounds.height())*dy);
		}
		m_bounds.setTop(y);
	}

	template<typename T>
	void ConvexPoly2<T>::setWidth(T width) {
	   setRight(m_bounds.right() + (width - m_bounds.width()));
	}

	template<typename T>
	void ConvexPoly2<T>::translate(T dx, T dy) {
		for (Size i = 0; i < m_points.size(); ++i) {
			m_points.at(i).x += dx;
			m_points.at(i).y += dy;
		}
		m_bounds.translate(dx, dy);		
	}

	template<typename T>
	ConvexPoly2<T> ConvexPoly2<T>::unionedWith(const ConvexPoly2<T>& poly) const {
		Vector< Point2<T> > points(m_points);
		points.appendAll(poly.m_points);
		return ConvexPoly2<T>(points);		
	}

	template<typename T>
	void ConvexPoly2<T>::unionWith(const ConvexPoly2<T>& poly) {
		Vector< Point2<T> > points(m_points);
		points.appendAll(poly.m_points);
		setToConvexHull(points);
	}
} // namespace Cat
#endif // CAT_CORE_GEOMETRY_POLY_H
