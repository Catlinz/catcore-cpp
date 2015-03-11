#include "core/geometry/boundingrect.h"

namespace Cat {

	Boolean BoundingRect::contains(const Point2f& point,
											 const Mat4& matrix,
											 I32 transforms) const {
		const Mat4::MatrixData& m = matrix.getMatrixDataConst();
		
		if (transforms == Mat4::kMTranslation) {
			/* Matrix only contains a translation */
			return Rectf::contains(Point2f(point.x - m.m41, point.y - m.m42));			
		}
		else if ((transforms & Mat4::kMRotation) != 0) {
			/* Matrix contains rotation and translation */
			return Rectf::contains(Point2f(m.m11*(point.x - m.m41) + m.m12*(point.y - m.m42),
											m.m21*(point.x - m.m41) + m.m22*(point.y - m.m42)));
		}
		else {
			/* Assume all */

		}
		return false;		
	}

	void BoundingRect::expandToContain(const Rectf& rect) {
		if (rect.left() < m_topLeft.x) {
			m_size.width += (m_topLeft.x - rect.left());
			m_topLeft.x = rect.left();
		}
		if (rect.top() > m_topLeft.y) {
			m_size.height += (rect.top() - m_topLeft.y);
			m_topLeft.y = rect.top();
		}
		if (rect.right() > m_topLeft.x + m_size.width) {
			m_size.height += (rect.right() -
									(m_topLeft.x + m_size.width));
		}
		if (rect.bottom() < m_topLeft.y - m_size.height) {
			m_size.height += ((m_topLeft.y - m_size.height) -
									rect.bottom());
		}
	}
	
	
	
} // namespace Cat
