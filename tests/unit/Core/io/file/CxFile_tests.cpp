#include "core/io/file/CxFile.h"
#include "core/CxTestCore.h"

namespace cat {

	void testCxFileBasics() {
		BEGIN_TEST;

		CxFile f0;

		FINISH_TEST;
	}
	
} // namespace cat


int main(int argc, char **argv) {
	cat::testCxFileBasics();

	return 0;
}
