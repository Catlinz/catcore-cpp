#ifndef CAT_CORE_COLOR_COLOR4B_H
#define CAT_CORE_COLOR_COLOR4B_H
/**
 * @copyright Copyright Catlin Zilinksi, 2014.  All rights reserved.
 *
 * @file color4b.h
 * @brief A simple class to hold a Byte RGBA Colour value.
 *
 * @author Catlin Zilinski
 * @date July 7, 2014
 */

#include "core/corelib.h"

namespace Cat {
	
	/**
	 * @class Color4b color4b.h "core/color/color4b.h"
	 * @brief A simple class to hold a Byte valued RGBA Colour value.
	 *
	 * @version 1
	 * @since July 7, 2014
	 * @author Catlin Zilinski
	 */
	class Color4b {		
	  public:

		/**
		 * @brief The color constants.
		 */
		const static Color4b kCWhite;
		const static Color4b kCSilver;
		const static Color4b kCGray;
		const static Color4b kCBlack;
		const static Color4b kCRed;
		const static Color4b kCMaroon;
		const static Color4b kCYellow;
		const static Color4b kCOlive;
		const static Color4b kCLime;
		const static Color4b kCGreen;
		const static Color4b kCAqua;
		const static Color4b kCTeal;
		const static Color4b kCBlue;
		const static Color4b kCNavy;
		const static Color4b kCFuchsia;
		const static Color4b kCPurple;		
		
		UByte r, g, b, a;

		/**
		 * @brief Create a default color (transparent black).
		 */
		inline Color4b()
			: r(0), g(0), b(0), a(0) {}

		/**
		 * @brief Create a greyscale color.
		 * @param value The value of the grey (0-255)
		 */
		inline explicit Color4b(UByte value)
			: r(value), g(value), b(value), a(255) {			
		}		

		/**
		 * @brief Create a RGB color (full alpha).
		 * @param red The amount of red in the color (0 - 255)
		 * @param green The amount of green in the color (0 - 255)
		 * @param blue The amount of blue in the color (0 - 255).
		 */
		inline Color4b(UByte red, UByte green, UByte blue)
			: r(red), g(green), b(blue), a(255) {
		}

		/**
		 * @brief Create an RGBA color.
		 * @param red The amount of red in the color (0 - 255)
		 * @param green The amount of green in the color (0 - 255)
		 * @param blue The amount of blue in the color (0 - 255).
		 * @param alpha The amount of alpha in the color (0 - 255).
		 */
		inline Color4b(UByte red, UByte green, UByte blue, UByte alpha)
			: r(red), g(green), b(blue), a(alpha) {
		}

		/**
		 * @brief Set the values of an RGBA color.
		 * @param red The amount of red in the color (0 - 255)
		 * @param green The amount of green in the color (0 - 255)
		 * @param blue The amount of blue in the color (0 - 255).
		 * @param alpha The amount of alpha in the color (0 - 255).
		 */
		inline void set(UByte red, UByte green, UByte blue, UByte alpha) {
			r = red;
			g = green;
			b = blue;
			a = alpha;
		}
	};

#if defined (DEBUG)
	inline std::ostream& operator<<(std::ostream& out, const Color4b& c) {
		return out << "(" << c.r << ", " << c.g << ", "
					  << c.b << ", " << c.a << ")";
	}	
#endif /* DEBUG */
} // namespace Cat

#endif // CAT_CORE_COLOR_COLOR4B_H
