#ifndef CAT_CORE_COLOR_COLOR4F_H
#define CAT_CORE_COLOR_COLOR4F_H
/**
 * @copyright Copyright Catlin Zilinksi, 2014.  All rights reserved.
 *
 * @file color4f.h
 * @brief A simple class to hold a Real valued RGBA Colour value.
 *
 * @author Catlin Zilinski
 * @date July 7, 2014
 */

#include "core/corelib.h"

namespace Cat {
	
	/**
	 * @class Color4f color4f.h "core/color/color4f.h"
	 * @brief A simple class to hold a Real valued RGBA Colour value.
	 *
	 * @version 1
	 * @since July 7, 2014
	 * @author Catlin Zilinski
	 */
	class Color4f {		
	  public:

		/**
		 * @brief The color constants.
		 */
		const static Color4f kCWhite;
		const static Color4f kCSilver;
		const static Color4f kCGray;
		const static Color4f kCBlack;
		const static Color4f kCRed;
		const static Color4f kCMaroon;
		const static Color4f kCYellow;
		const static Color4f kCOlive;
		const static Color4f kCLime;
		const static Color4f kCGreen;
		const static Color4f kCAqua;
		const static Color4f kCTeal;
		const static Color4f kCBlue;
		const static Color4f kCNavy;
		const static Color4f kCFuchsia;
		const static Color4f kCPurple;		
		
		Real r, g, b, a;

		/**
		 * @brief Create a default color (transparent black).
		 */
		inline Color4f()
			: r(0), g(0), b(0), a(0) {}

		/**
		 * @brief Create a greyscale color.
		 * @param value The value of the grey (0-1.0)
		 */
		inline explicit Color4f(Real value)
			: r(value), g(value), b(value), a(REAL(1.0)) {			
		}		

		/**
		 * @brief Create a RGB color (full alpha).
		 * @param red The amount of red in the color (0 - 1.0)
		 * @param green The amount of green in the color (0 - 1.0)
		 * @param blue The amount of blue in the color (0 - 1.0).
		 */
		inline Color4f(Real red, Real green, Real blue)
			: r(red), g(green), b(blue), a(REAL(1.0)) {
		}

		/**
		 * @brief Create a RGBA color.
		 * @param red The amount of red in the color (0 - 1.0)
		 * @param green The amount of green in the color (0 - 1.0)
		 * @param blue The amount of blue in the color (0 - 1.0).
		 * @param alpha The amount of alpha in the color (0 - 1.0).
		 */
		inline Color4f(Real red, Real green, Real blue, Real alpha)
			: r(red), g(green), b(blue), a(alpha) {
		}

		/**
		 * @brief Set all the values of the RGBA Color.
		 * @param red The amount of red in the color (0 - 1.0)
		 * @param green The amount of green in the color (0 - 1.0)
		 * @param blue The amount of blue in the color (0 - 1.0).
		 * @param alpha The amount of alpha in the color (0 - 1.0).
		 */
		inline void set(Real red, Real green, Real blue, Real alpha) {
			r = red;
			g = green;
			b = blue;
			a = alpha;
		}
		
	};

#if defined (DEBUG)
	inline std::ostream& operator<<(std::ostream& out, const Color4f& c) {
		return out << "(" << c.r << ", " << c.g << ", "
					  << c.b << ", " << c.a << ")";
	}	
#endif /* DEBUG */
} // namespace Cat

#endif // CAT_CORE_COLOR_COLOR4F_H
