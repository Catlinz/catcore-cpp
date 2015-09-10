#include "core/common/CxStrID.h"
#include "core/CxTestCore.h"


namespace cat {

	/**
	 * @tests CxStrID()
	 * @tests CxStrID(const CxChar *)
	 * @tests CxStrID(const CxStrID &)
	 * @tests operator=(const CxStrID &)
	 * @tests hash()
	 * @tests static hash(const CxChar *)
	 * @tests str()
	 * @tests ~CxStrID()
	 */
	void testCxStrIDCreateAndDestroy() {
		BEGIN_TEST;

		CxStrID id;
		ass_zero(id.hash());
		ass_zero(id.str());

		id = CxStrID("Meow01");
		ass_true(id.hash() == CxStrID::hash("Meow01"));
		ass_true(strcmp(id.str(), "Meow01") == 0);

		CxStrID id2 = id;
		ass_true(id.hash() == CxStrID::hash("Meow01"));
		ass_true(strcmp(id.str(), "Meow01") == 0);
		ass_true(id2.hash() == CxStrID::hash("Meow01"));
		ass_true(strcmp(id2.str(), "Meow01") == 0);

		id = CxStrID("Moo01");
		ass_true(id.hash() == CxStrID::hash("Moo01"));
		ass_true(strcmp(id.str(), "Moo01") == 0);
		ass_true(id2.hash() == CxStrID::hash("Meow01"));
		ass_true(strcmp(id2.str(), "Meow01") == 0);
		ass_true(id.hash() != id2.hash());

		id2 = CxStrID();
		ass_true(id.hash() == CxStrID::hash("Moo01"));
		ass_true(strcmp(id.str(), "Moo01") == 0);
		ass_zero(id2.hash());
		ass_zero(id2.str());
		ass_true(id.hash() != id2.hash());

		id = CxStrID("Meow01");
		id2 = CxStrID("Meow02");

		const CxChar *str2 = id2.str();
		id = static_cast<CxStrID &&>(id2);
		ass_zero(id2.str());
		ass_eq(id.str(), str2);
		
		FINISH_TEST;
	}

	void testCxStrIDComparisons() {
		BEGIN_TEST;

		CxChar *str0;
		CxChar *str1;

		if (CxStrID::hash("Meow01") < CxStrID::hash("Meow02")) {
			str0 = str::copy("Meow01");
			str1 = str::copy("Meow02");
		}
		else {
			str0 = str::copy("Meow02");
			str1 = str::copy("Meow01");
		}
		
		CxStrID id0(str0);
		CxStrID id1(str1);

		CxStrID id2 = id0;

		ass_true(id0 == id0 && !(id0 != id0));
		ass_true(id1 == id1 && !(id1 != id1));
		ass_true(id0 == id2 && id2 == id0 && !(id0 != id2) && !(id2 != id0));
		ass_false(id0 == id1 || id1 == id0);
		ass_false(id1 == id2 || id2 == id1);
		ass_true(id0 != id1 && id1 != id0);
		ass_true(id1 != id2 && id2 != id1);

		ass_false(id0 < id0 || id0 < id2 || id2 < id0);
		ass_true(id0 <= id0 && id0 <= id2 && id2 <= id0);
		ass_false(id0 > id0 || id0 > id2 || id2 > id0);
		ass_true(id0 >= id0 || id0 >= id2 || id2 >= id0);

		ass_true(id0 < id1 && id1 > id0);
		ass_false(id1 < id0 && id0 > id1);
		ass_true(id0 <= id1 && id1 >= id0);
		ass_false(id1 <= id0 && id0 >= id1);

		FINISH_TEST;
	}

} // namespace cat

int main(int argc, char **argv) {

	cat::testCxStrIDCreateAndDestroy();
	cat::testCxStrIDComparisons();
	return 0;
}

