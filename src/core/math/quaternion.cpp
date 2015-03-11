#include "core/math/quaternion.h"

namespace Cat {

	const Real Quaternion::kEpsilon = 0.00001f;
	const Quaternion Quaternion::kZero =
		Quaternion(REAL(0.0), REAL(0.0), REAL(0.0), REAL(0.0));
	const Quaternion Quaternion::kIdentity =
		Quaternion(REAL(0.0), REAL(0.0), REAL(0.0), REAL(1.0));
	
	Quaternion Quaternion::operator*(const Quaternion& rhs) const {
		return Quaternion(
			(w * rhs.x) + (x * rhs.w) + (y * rhs.z) - (z * rhs.y),
			(w * rhs.y) + (y * rhs.w) + (z * rhs.x) - (x * rhs.z),
			(w * rhs.z) + (z * rhs.w) + (x * rhs.y) - (y * rhs.x),
			(w * rhs.w) - (x * rhs.x) - (y * rhs.y) - (z * rhs.z));
	}

	Quaternion& Quaternion::operator*=(const Quaternion& rhs) {
	   Real qx = (w * rhs.x) + (x * rhs.w) + (y * rhs.z) - (z * rhs.y);
		Real qy = (w * rhs.y) + (y * rhs.w) + (z * rhs.x) - (x * rhs.z);
		Real qz = (w * rhs.z) + (z * rhs.w) + (x * rhs.y) - (y * rhs.x);
		w = (w * rhs.w) - (x * rhs.x) - (y * rhs.y) - (z * rhs.z);
		x = qx;
		y = qy;
		z = qz;
		return *this;
	}

	/**
	 * @todo: Optimize
	 */
	Vec3 Quaternion::operator*(const Vec3& rhs) const {
		// Normalize the vector
		Vec3 result = rhs;				
		Real factor = Math::lengthSquared(result);

		if (Math::abs(factor) > kEpsilon &&
			 Math::abs(factor - REAL(1.0)) > kEpsilon && factor != 0) {
			factor = Math::inversesqrt(factor);
			result.x *= factor;
			result.y *= factor;
			result.z *= factor;
		}

		// Assumes quaternion is normalized
#if defined(DEBUG)
		if (!isUnitLength()) {
			DWARN("Computing vector rotation by NON-unit length quaternion!");
		}
#endif

		Quaternion vectorAsQuaternion(result.x, result.y, result.z, REAL(0.0));

		// We rotate the vector by computing this*rhs*-this, hence why we need
		// the Quaternion to be unit length (to get the conjugate).	
	   Quaternion tmpResult = (*this) * (vectorAsQuaternion * unitInverse());
		
		return Vec3(tmpResult.x * factor, tmpResult.y * factor, tmpResult.z * factor);
	}

	/**
	 * @todo: Optimize
	 */
	void Quaternion::rotate(Vec3& v) const {
		// Normalize the vector				
		Real factor = Math::lengthSquared(v);

		if (Math::abs(factor) > kEpsilon &&
			 Math::abs(factor - REAL(1.0)) > kEpsilon && factor != 0) {
			factor = Math::inversesqrt(factor);
			v.x *= factor;
			v.y *= factor;
			v.z *= factor;
		}

		// Assumes quaternion is normalized
#if defined(DEBUG)
		if (!isUnitLength()) {
			DWARN("Computing vector rotation by NON-unit length quaternion!");
		}
#endif

		Quaternion vectorAsQuaternion(v.x, v.y, v.z, REAL(0.0));

		// We rotate the vector by computing this*rhs*-this, hence why we need
		// the Quaternion to be unit length (to get the conjugate).	
	   Quaternion tmpResult = (*this) * (vectorAsQuaternion * unitInverse());
		v.x = tmpResult.x * factor;
		v.y = tmpResult.y * factor;
		v.z = tmpResult.z * factor;
	}

	void Quaternion::fromAxisAngle(const Vec3& axis, const Angle& angle) {
		Real sinOfAngle = Math::sin(angle.radians()*REAL(0.5));
		Vec3 norm = Math::normalised(axis);

		x = norm.x * sinOfAngle;
		y = norm.y * sinOfAngle;
		z = norm.z * sinOfAngle;		
		w = Math::cos(angle.radians()*REAL(0.5));	
	}

	void Quaternion::toAxisAngle(Vec3& axis, Angle& angle) const {
		Real scale = Math::sqrt(x*x + y*y + z*z);
		if (scale != 0) {
			scale = REAL(1.0) / scale;
			axis.x = x * scale;
			axis.y = y * scale;
			axis.z = z * scale;
			angle.setRadians(Math::acos(w) * REAL(2.0));
		}
		else {
			axis.x = axis.y = axis.z = 0;
			angle.setRadians(0);
		}
	}

	void Quaternion::toAxisAngle(Vec3& axis, Angle& angle,
										  const Vec3& p_checkAngle) const {
		Real scale = Math::sqrt(x*x + y*y + z*z);
		if (scale != 0) {
			scale = REAL(1.0) / scale;
			axis.x = x * scale;
			axis.y = y * scale;
			axis.z = z * scale;
			angle.setRadians(Math::acos(w) * REAL(2.0));
			if (Math::approx(axis, -p_checkAngle)) {
				axis *= -1;
				angle *= -1;
			}
		}
		else {
			axis.x = axis.y = axis.z = 0;
			angle.setRadians(0);
		}
	}
	

	void Quaternion::fromEulerAngles(const Angle& pitch, const Angle& yaw, const Angle& roll) {
		// Create 3 quaternions for each angle / axis, and multiply together.
		fromAxisAngle(Vec3(REAL(0.0), REAL(1.0), REAL(0.0)), yaw);
		operator*=(Quaternion(Vec3(REAL(0.0), REAL(1.0), REAL(1.0)), roll));
		operator*=(Quaternion(Vec3(REAL(1.0), REAL(0.0), REAL(0.0)), pitch));
 
	}

	void Quaternion::toEulerAngles(Angle& po_pitch, Angle& po_yaw, Angle& po_roll) {
		
	   F64 test = x*y + z*w;
		if (test > 0.499) { // singularity at north pole
			po_yaw.setRadians(2 * atan2(x,w));
			po_roll.setRadians(M_PI / 2);
		   po_pitch.setRadians(0);
			return;
		}
		if (test < -0.499) { // singularity at south pole
			po_yaw.setRadians(-2 * atan2(x,w));
			po_roll.setRadians(M_PI / 2);
			po_pitch.setRadians(0);
			return;
		}
		po_yaw.setRadians(atan2(2*y*w-2*x*z , 1 - 2*y*y - 2*z*z));
		po_roll.setRadians(asin(2*test));
		po_pitch.setRadians(atan2(2*x*w-2*y*z , 1 - 2*x*x - 2*z*z));
	}	
	
	void Quaternion::fromLocalAxes(const Vec3& xaxis, const Vec3& yaxis, const Vec3& zaxis) {
		Mat3 m(xaxis, yaxis, zaxis);
		fromMatrix(m);				
	}

	Vec3 Quaternion::getTranslation(Real dx, Real dy, Real dz) const {
		return Vec3(
			dx*( (x*x)-(y*y)-(z*z)+(w*w) ) + 2*(dy*( x*y - z*w ) + dz*( x*z + y*w )), 
			dy*( -(x*x)+(y*y)-(z*z)+(w*w) ) + 2*(dx*( x*y + z*w ) + dz*( -x*w + y*z )),
			dz*( -(x*x)-(y*y)+(z*z)+(w*w) ) + 2*(dx*( x*z - y*w ) + dy*( x*w + y*z ))
			);		
	}
	

	void Quaternion::toLocalAxes(Vec3& xaxis, Vec3& yaxis, Vec3& zaxis) const {	
		xaxis = xAxis();
		yaxis = yAxis();
		zaxis = zAxis();		
	}
	
	void Quaternion::fromMatrix(const Mat3& mat) {
		const Mat3::MatrixData& m = mat.getMatrixDataConst();
		w = (m.m11 + m.m22 + m.m33 + REAL(1.0)) / REAL(4.0);
		x = (m.m11 - m.m22 - m.m33 + REAL(1.0)) / REAL(4.0);
		y = (-m.m11 + m.m22 - m.m33 + REAL(1.0)) / REAL(4.0);
		z = (-m.m11 - m.m22 + m.m33 + REAL(1.0)) / REAL(4.0);
		
		if (x < REAL(0.0)) {
			x = REAL(0.0);
		}
		if (y < REAL(0.0)) {
			y = REAL(0.0);
		}
		if (z < REAL(0.0)) {
			z = REAL(0.0);
		}
		if (w < REAL(0.0)) {
			w = REAL(0.0);
		}
		
		x = Math::sqrt(x);
		y = Math::sqrt(y);
		z = Math::sqrt(z);
		w = Math::sqrt(w);

		/* determine the correct signs for each component. */
		if (x >= y && x >= z && x >= w) {
			//x *= 1.0;
			y *= Math::sign(m.m21 + m.m12);
			z *= Math::sign(m.m13 + m.m31);
			w *= Math::sign(m.m32 - m.m23);
		}
		else if (y >= x && y >= z && y >= w) {
			x *= Math::sign(m.m21 + m.m12);
			//y *= 1.0;
			z *= Math::sign(m.m32 + m.m23);
			w *= Math::sign(m.m13 - m.m31);
		}
		else if (z >= x && z >= y && z >= w) {
			x *= Math::sign(m.m31 + m.m13);
			y *= Math::sign(m.m32 + m.m23);
			// z*= 1.0;
			w *= Math::sign(m.m21 - m.m12);
		}
		else if (w >= x && w >= y && w >= z) {
			x *= Math::sign(m.m32 - m.m23);
			y *= Math::sign(m.m13 - m.m31);
			z *= Math::sign(m.m21 - m.m12);
			// w *= 1.0;
		}
		else {
			DERR("Error getting Quaternion from 3x3 Matrix!");
		}
	   normalise();
		
		}
	

	void Quaternion::toMatrix(Mat3& m) const {
#if defined(DEBUG)
		if (!isUnitLength()) {
			DWARN("Computing 3x3 rotation matrix from NON-unit length quaternion!");
		}
#endif
		float	x2 = x*x;
		float y2 = y*y;
		float z2 = z*z;
		float xy = x*y;
		float xz = x*z;
		float yz = y*z;
		float wx = w*x;
		float wy = w*y;
		float wz = w*z;

		Mat3::MatrixData& md = m.getMatrixData();
		md.m[0] = 1.0f - 2.0f * (y2 + z2);
		md.m[1] = 2.0f * (xy - wz);
		md.m[2] = 2.0f * (xz + wy);

		md.m[3] = 2.0f * (xy + wz);
		md.m[4] = 1.0f - 2.0f * (x2 + z2);
		md.m[5] = 2.0f * (yz - wx);

		md.m[6] = 2.0f * (xz - wy);
		md.m[7] = 2.0f * (yz + wx);
		md.m[8] = 1.0f - 2.0f * (x2 + y2);
		m.transpose();
		
	}

	/**
	 * Code from this derived from:
	 * http://lolengine.net/blog/2013/09/18/beautiful-maths-quaternion-from-vectors
	 */
	void Quaternion::fromTwoVectors(const Vec3& p_start,
											  const Vec3& p_end) {
		Vec3 axis = Math::cross(p_start, p_end);
		x = axis.x;
		y = axis.y;
		z = axis.z;
		w = Math::dot(p_start, p_end);
		w += length();
		normalise();
	}

	/**
	 * Code derived from:
	 * http://willperone.net/Code/quaternion.php.
	 */
	Quaternion Quaternion::slerp(const Quaternion& p_q1, const Quaternion& p_q2, Real p_t) {
		if (p_q1 == p_q2) {
			return p_q1;
		}
		
		Real q1_dot_q2 = p_q1.dotProduct(p_q2);

		/* If q1 dot q2 < 0, then angle between > 90, so inverting q2 gets smaller rotation. */
		Quaternion q2 = p_q2;
		if (q1_dot_q2 < 0) {
			q1_dot_q2 = -q1_dot_q2;
			q2 = -q2;
		}

		/* We do this to make sure we dont get a divide by zero
		 * when taking cos-1 of something that might be 1. 
		 * (i.e., when quaternions very close to each other).  This
		 * will also perform linear interpolation when the quaternions
		 * are about 180 from each other too, which might give a hint of 
		 * something odd, but will prevent the quaternion from going to 
		 * nan, which makes things explode, since once you go nan, you 
		 * never go back. */
		if (q1_dot_q2 > REAL(0.99)) {
			return Quaternion::lerp(p_q1, q2, p_t);
		}

		Real angle = acosf(q1_dot_q2);
		Quaternion r = p_q1 * sinf(angle * (1.0f - p_t));
		r += q2 * sinf(angle*p_t);
		r *= 1.0f/sinf(angle);
		return r;
	}
	

#ifdef DEBUG
	std::ostream& operator<<(std::ostream& out, const Quaternion& quaternion) {
		return out << quaternion.w << " + " << quaternion.x << "i + " << quaternion.y << "j + " << quaternion.z << "k";	
	}
#endif //DEBUG

} // namespace Cat



