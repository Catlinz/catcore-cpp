#ifndef CAT_CORE_IO_SERIALISER_H
#define CAT_CORE_IO_SERIALISER_H
/**
 * @copyright Copyright Catlin Zilinksi, 2014.  All rights reserved.
 *
 * @file serialiser.h
 * @brief A static namespace that can write objects to an output stream.
 *
 * @author Catlin Zilinski
 * @date June 26, 2014
 */

#include "core/corelib.h"

namespace Cat {

	class InputStream;
	class OutputStream;
	class Serialisable;
	class Color4f;	
	template <typename T> class Rect;
	

	namespace Serialiser {

		Size read(Rect<Real>& rect, InputStream* stream);		
		Size write(const Rect<Real>& rect, OutputStream* stream);

		Size read(Rect<I32>& rect, InputStream* stream);		
		Size write(const Rect<I32>& rect, OutputStream* stream);

		Size read(Color4f& color, InputStream* stream);		
		Size write(const Color4f& color, OutputStream* stream);


		Size read(Serialisable* object, InputStream* stream);
		Size write(Serialisable* object, OutputStream* stream);		
		
		
	} // namespace Serialiser

} // namespace Cat

#endif //  CAT_CORE_IO_SERIALISER_H
