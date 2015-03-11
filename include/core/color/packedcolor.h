#ifndef CAT_CORE_COLOR_PACKEDCOLOR_H
#define CAT_CORE_COLOR_PACKEDCOLOR_H
/**
 * @copyright Copyright Catlin Zilinksi, 2014.  All rights reserved.
 *
 * @file packedcolor.h
 * @brief A set of functions to manipulate rgba colors packed into an int.
 *
 * @author Catlin Zilinski
 * @date June 30, 2014
 */

#include "core/corelib.h"

namespace Cat {

	typedef I32 PackedColorVal;	

	namespace PackedColor {

		/**
		 * @brief Create a PackedColorVal from an RGB value.
		 * @param r The red value (0 - 255).
		 * @param g The green value (0 - 255).
		 * @param b The blue value (0 - 255).
		 * @return A packed color value from the RGB value.
		 */
		inline PackedColorVal fromRGB(UByte r, UByte g, UByte b) {
			return (0xFF000000 | (b << 16) | (g << 8) | r);
		}

		/**
		 * @brief Create a PackedColorVal from an RGB value.
		 * @param r The red value (0 - 1).
		 * @param g The green value (0 - 1).
		 * @param b The blue value (0 - 1).
		 * @return A packed color value from the RGB value.
		 */
		PackedColorVal fromRGB(Real r, Real g, Real b);		
		
		/**
		 * @brief Create a PackedColorVal from an RGB value.
		 * @param r The red value (0 - 255).
		 * @param g The green value (0 - 255).
		 * @param b The blue value (0 - 255).
		 * @param a The alpha value (0 - 255).
		 * @return A packed color value from the RGB value.
		 */
		inline PackedColorVal fromRGBA(UByte r, UByte g, UByte b, UByte a) {
			return ((a << 24) | (b << 16) | (g << 8) | r);
		}

		/**
		 * @brief Create a PackedColorVal from an RGB value.
		 * @param r The red value (0 - 1).
		 * @param g The green value (0 - 1).
		 * @param b The blue value (0 - 1).
		 * @param a The alpha value (0 - 1).
		 * @return A packed color value from the RGB value.
		 */
		PackedColorVal fromRGBA(Real r, Real b, Real g, Real a);

		/**
		 * @brief Method to get the red component from the packed color.
		 * @return The red component (0 - 255).
		 */
		inline UByte red(PackedColorVal color) {
			return (UByte)(color & 0x000000ff);
		}

		/**
		 * @brief Method to get the green component from the packed color.
		 * @return The green component (0 - 255).
		 */
		inline UByte green(PackedColorVal color) {
			return (UByte)((color & 0x0000ff00) >> 8);
		}

		/**
		 * @brief Method to get the blue component from the packed color.
		 * @return The blue component (0 - 255).
		 */
		inline UByte blue(PackedColorVal color) {
			return (UByte)((color & 0x00ff0000) >> 16);
		}

		/**
		 * @brief Method to get the alpha component from the packed color.
		 * @return The alpha component (0 - 255).
		 */
		inline UByte alpha(PackedColorVal color) {
			return (UByte)((color & 0xff000000) >> 24);
		}
		
		
		
	}

} // namespace Cat


#endif // CAT_CORE_COLOR_PACKEDCOLOR_H
