#include "core/time/unix/time.h"

namespace Cat {
	
#if defined (DEBUG)
	std::ostream& operator<<(std::ostream& out, const Time& t) {
		return out << t.nano();
	}
#endif // DEBUG
	
} // namespace Cat
