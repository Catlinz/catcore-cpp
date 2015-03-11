#include "core/io/serialiser.h"
#include "core/io/serialisable.h"
#include "core/io/objectinputstream.h"
#include "core/io/objectoutputstream.h"

#include "core/geometry/rectf.h"
#include "core/geometry/recti.h"
#include "core/color/color4f.h"

namespace Cat {

	Size Serialiser::read(Rectf& rect, InputStream* stream) {
		DataInputStream* in = reinterpret_cast<DataInputStream*>(stream);
		Real vals[4];
		Size bytesRead = in->read(vals, sizeof(Real)*4);
		D_CONDERR((bytesRead == 0),
					 "Failed to read Rectf from stream!");		
		rect.setRect(vals[0], vals[1], vals[2], vals[3]);
		return bytesRead;
	}

	Size Serialiser::write(const Rectf& rect, OutputStream* stream) {
		DataOutputStream* out = reinterpret_cast<DataOutputStream*>(stream);
		Real vals[4];
		vals[0] = rect.x();
		vals[1] = rect.y();
		vals[2] = rect.width();
		vals[3] = rect.height();
		return out->write(vals, sizeof(Real)*4);		
	}

	Size Serialiser::read(Recti& rect, InputStream* stream) {
		DataInputStream* in = reinterpret_cast<DataInputStream*>(stream);
		I32 vals[4];
		Size bytesRead = in->read(vals, sizeof(I32)*4);
		D_CONDERR((bytesRead == 0),
					 "Failed to read Recti from stream!");		
		rect.setRect(vals[0], vals[1], vals[2], vals[3]);
		return bytesRead;
	}

	Size Serialiser::write(const Recti& rect, OutputStream* stream) {
		DataOutputStream* out = reinterpret_cast<DataOutputStream*>(stream);
		I32 vals[4];
		vals[0] = rect.x();
		vals[1] = rect.y();
		vals[2] = rect.width();
		vals[3] = rect.height();
		return out->write(vals, sizeof(I32)*4);		
	}

	Size Serialiser::read(Color4f& color, InputStream* stream) {
		Real vals[4];
		Size bytesRead = stream->read(vals, sizeof(Real)*4);
		D_CONDERR((bytesRead == 0),
					 "Failed to read Color4f from stream!");		
		color.set(vals[0], vals[1], vals[2], vals[3]);
		return bytesRead;
	}
	
	Size Serialiser::write(const Color4f& color, OutputStream* stream) {
		Real vals[4];
		vals[0] = color.r;		
		vals[1] = color.g;
		vals[2] = color.b;
		vals[3] = color.a;
		return stream->write(vals, sizeof(Real)*4);	
	}

	Size Serialiser::read(Serialisable* object, InputStream* stream) {
		ObjectInputStream* in = reinterpret_cast<ObjectInputStream*>(stream);
		Size bytesRead = in->readObject(object);
		D_CONDERR((bytesRead == 0),
					 "Failed to read Serialisable from stream!");
		return bytesRead;
	}

	Size Serialiser::write(Serialisable* object, OutputStream* stream) {
		ObjectOutputStream* out = reinterpret_cast<ObjectOutputStream*>(stream);
	   return out->writeObject(object);
	}

} // namespace Cat
