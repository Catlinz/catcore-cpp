#include "core/testcore.h"
#include "core/string/hungrystring.h"

namespace cc {
	
	void testHungryStringCreateAndDestroy() {
		BEGIN_TEST;

		HungryString hs;
		ass_eq(hs.baseString(), NIL);
		ass_eq(hs.isFull(), false);
		ass_eq(hs.maxLength(), 0);
		ass_eq(hs.remainingFeeds(), 0);
		ass_eq(hs.cStr(), NIL);

		hs = HungryString("/user/%/dir/%/config.conf", 256);
		ass_eq(strcmp(hs.baseString(), "/user/%/dir/%/config.conf"), 0);
		ass_eq(hs.isFull(), false);
		ass_eq(hs.maxLength(), 256);
		ass_eq(hs.remainingFeeds(), 2);
		ass_eq(hs.cStr(), NIL);
	
		HungryString hs2 = hs;
		ass_eq(strcmp(hs2.baseString(), "/user/%/dir/%/config.conf"), 0);
		ass_eq(hs2.isFull(), false);
		ass_eq(hs2.maxLength(), 256);
		ass_eq(hs2.remainingFeeds(), 2);
		ass_eq(hs2.cStr(), NIL);

		HungryString* nhs = new HungryString();
		ass_eq(nhs->baseString(), NIL);
		ass_eq(nhs->isFull(), false);
		ass_eq(nhs->maxLength(), 0);
		ass_eq(nhs->remainingFeeds(), 0);
		ass_eq(nhs->cStr(), NIL);
		delete nhs;

		nhs = new HungryString("/user2/%/dir/%/%%%.conf", 512);
		ass_eq(strcmp(nhs->baseString(), "/user2/%/dir/%/%%%.conf"), 0);
		ass_eq(nhs->isFull(), false);
		ass_eq(nhs->maxLength(), 512);
		ass_eq(nhs->remainingFeeds(), 3);
		ass_eq(nhs->cStr(), NIL);

		delete nhs;		
		
		FINISH_TEST;		
	}

	void testHungryStringFeedSimple() {
		BEGIN_TEST;

		HungryString hs = HungryString("/user/%/dir/%/config.conf", 256);
		ass_eq(strcmp(hs.baseString(), "/user/%/dir/%/config.conf"), 0);
		ass_eq(hs.isFull(), false);
		ass_eq(hs.maxLength(), 256);
		ass_eq(hs.remainingFeeds(), 2);
		ass_eq(hs.cStr(), NIL);

		hs.feed("Meow1");
		ass_eq(strcmp(hs.baseString(), "/user/%/dir/%/config.conf"), 0);
		ass_eq(hs.isFull(), false);
		ass_eq(hs.maxLength(), 256);
		ass_eq(hs.remainingFeeds(), 1);
		ass_eq(hs.cStr(), NIL);

		hs.feed("moooo");
		std::cout << "String: " << hs.cStr() << std::endl;		
		ass_eq(strcmp(hs.baseString(), "/user/%/dir/%/config.conf"), 0);
		ass_eq(hs.remainingFeeds(), 0);
		ass_eq(hs.isFull(), true);
		ass_eq(hs.maxLength(), 256);		
		ass_eq(strcmp(hs.cStr(), "/user/Meow1/dir/moooo/config.conf"), 0);

		hs.feed("Moasasdasd");
		ass_eq(strcmp(hs.baseString(), "/user/%/dir/%/config.conf"), 0);
		ass_eq(hs.remainingFeeds(), 0);
		ass_eq(hs.isFull(), true);
		ass_eq(hs.maxLength(), 256);		
		ass_eq(strcmp(hs.cStr(), "/user/Meow1/dir/moooo/config.conf"), 0);

		hs = HungryString("/usr/%/meow", 64);
		ass_eq(strcmp(hs.baseString(), "/usr/%/meow"), 0);
		ass_eq(hs.isFull(), false);
		ass_eq(hs.maxLength(), 64);
		ass_eq(hs.remainingFeeds(), 1);
		ass_eq(hs.cStr(), NIL);

		hs.feed("Mooooo%");
		ass_eq(strcmp(hs.baseString(), "/usr/%/meow"), 0);
		ass_eq(hs.remainingFeeds(), 0);
		ass_eq(hs.isFull(), true);
		ass_eq(hs.maxLength(), 64);		
		ass_eq(strcmp(hs.cStr(), "/usr/Mooooo%/meow"), 0);

		hs = HungryString("%/dir/file.file", 64);
		ass_eq(strcmp(hs.baseString(), "%/dir/file.file"), 0);
		ass_eq(hs.isFull(), false);
		ass_eq(hs.maxLength(), 64);
		ass_eq(hs.remainingFeeds(), 1);
		ass_eq(hs.cStr(), NIL);

		hs.feed("%Mooooo%");
		ass_eq(strcmp(hs.baseString(), "%/dir/file.file"), 0);
		ass_eq(hs.remainingFeeds(), 0);
		ass_eq(hs.isFull(), true);
		ass_eq(hs.maxLength(), 64);		
		ass_eq(strcmp(hs.cStr(), "%Mooooo%/dir/file.file"), 0);

		hs = HungryString("/dir/file.file%", 64);
		ass_eq(strcmp(hs.baseString(), "/dir/file.file%"), 0);
		ass_eq(hs.isFull(), false);
		ass_eq(hs.maxLength(), 64);
		ass_eq(hs.remainingFeeds(), 1);
		ass_eq(hs.cStr(), NIL);

		hs.feed("MoooooMoo");
		ass_eq(strcmp(hs.baseString(), "/dir/file.file%"), 0);
		ass_eq(hs.remainingFeeds(), 0);
		ass_eq(hs.isFull(), true);
		ass_eq(hs.maxLength(), 64);		
		ass_eq(strcmp(hs.cStr(), "/dir/file.fileMoooooMoo"), 0);

		hs = HungryString("%", 64);
		ass_eq(strcmp(hs.baseString(), "%"), 0);
		ass_eq(hs.isFull(), false);
		ass_eq(hs.maxLength(), 64);
		ass_eq(hs.remainingFeeds(), 1);
		ass_eq(hs.cStr(), NIL);

		hs.feed("MoooooMoo");
		ass_eq(strcmp(hs.baseString(), "%"), 0);
		ass_eq(hs.remainingFeeds(), 0);
		ass_eq(hs.isFull(), true);
		ass_eq(hs.maxLength(), 64);		
		ass_eq(strcmp(hs.cStr(), "MoooooMoo"), 0);
		

		FINISH_TEST;
	}

	void testHungryStringFeedNoFeeds() {
		BEGIN_TEST;

		HungryString hs = HungryString("/user/dir/config.conf", 256);
		ass_eq(strcmp(hs.baseString(), "/user/dir/config.conf"), 0);
		ass_eq(hs.isFull(), true);
		ass_eq(hs.maxLength(), 256);
		ass_eq(hs.remainingFeeds(), 0);
		ass_eq(strcmp(hs.cStr(), "/user/dir/config.conf"), 0);

		hs.feed("Moasdasd");
		ass_eq(strcmp(hs.baseString(), "/user/dir/config.conf"), 0);
		ass_eq(hs.isFull(), true);
		ass_eq(hs.maxLength(), 256);
		ass_eq(hs.remainingFeeds(), 0);
		ass_eq(strcmp(hs.cStr(), "/user/dir/config.conf"), 0);
	
		FINISH_TEST;
	}

	void testHungryStringFeedAndCopy() {
		BEGIN_TEST;

		HungryString hs = HungryString("/user/%/dir/%/config.conf", 256);
		ass_eq(strcmp(hs.baseString(), "/user/%/dir/%/config.conf"), 0);
		ass_eq(hs.isFull(), false);
		ass_eq(hs.maxLength(), 256);
		ass_eq(hs.remainingFeeds(), 2);
		ass_eq(hs.cStr(), NIL);

		hs.feed("Meow1");		
		HungryString hs2 = hs;
		HungryString hs3(hs2);
		ass_eq(strcmp(hs3.baseString(), "/user/%/dir/%/config.conf"), 0);
		ass_eq(hs3.isFull(), false);
		ass_eq(hs3.maxLength(), 256);
		ass_eq(hs3.remainingFeeds(), 1);
		ass_eq(hs3.cStr(), NIL);
		ass_eq(strcmp(hs2.baseString(), "/user/%/dir/%/config.conf"), 0);
		ass_eq(hs2.isFull(), false);
		ass_eq(hs2.maxLength(), 256);
		ass_eq(hs2.remainingFeeds(), 1);
		ass_eq(hs2.cStr(), NIL);
		ass_eq(strcmp(hs.baseString(), "/user/%/dir/%/config.conf"), 0);
		ass_eq(hs.isFull(), false);
		ass_eq(hs.maxLength(), 256);
		ass_eq(hs.remainingFeeds(), 1);
		ass_eq(hs.cStr(), NIL);
		
		hs.feed("moooo");
	
		ass_eq(strcmp(hs.baseString(), "/user/%/dir/%/config.conf"), 0);
		ass_eq(hs.remainingFeeds(), 0);
		ass_eq(hs.isFull(), true);
		ass_eq(hs.maxLength(), 256);		
		ass_eq(strcmp(hs.cStr(), "/user/Meow1/dir/moooo/config.conf"), 0);
		ass_eq(strcmp(hs2.baseString(), "/user/%/dir/%/config.conf"), 0);
		ass_eq(hs2.isFull(), false);
		ass_eq(hs2.maxLength(), 256);
		ass_eq(hs2.remainingFeeds(), 1);
		ass_eq(hs2.cStr(), NIL);
		ass_eq(strcmp(hs3.baseString(), "/user/%/dir/%/config.conf"), 0);
		ass_eq(hs3.isFull(), false);
		ass_eq(hs3.maxLength(), 256);
		ass_eq(hs3.remainingFeeds(), 1);
		ass_eq(hs3.cStr(), NIL);
		
		hs2.feed("Merroowww");
		ass_eq(strcmp(hs.baseString(), "/user/%/dir/%/config.conf"), 0);
		ass_eq(strcmp(hs.cStr(), "/user/Meow1/dir/moooo/config.conf"), 0);
		ass_eq(hs.remainingFeeds(), 0);
		ass_eq(hs.isFull(), true);
		ass_eq(hs.maxLength(), 256);		
		ass_eq(strcmp(hs2.cStr(), "/user/Meow1/dir/Merroowww/config.conf"), 0);
		ass_eq(strcmp(hs2.baseString(), "/user/%/dir/%/config.conf"), 0);
		ass_eq(hs2.remainingFeeds(), 0);
		ass_eq(hs2.isFull(), true);
		ass_eq(hs2.maxLength(), 256);
		ass_eq(strcmp(hs3.baseString(), "/user/%/dir/%/config.conf"), 0);
		ass_eq(hs3.isFull(), false);
		ass_eq(hs3.maxLength(), 256);
		ass_eq(hs3.remainingFeeds(), 1);
		ass_eq(hs3.cStr(), NIL);

		hs3.feed("Woof");
		ass_eq(strcmp(hs.baseString(), "/user/%/dir/%/config.conf"), 0);
		ass_eq(strcmp(hs.cStr(), "/user/Meow1/dir/moooo/config.conf"), 0);
		ass_eq(hs.remainingFeeds(), 0);
		ass_eq(hs.isFull(), true);
		ass_eq(hs.maxLength(), 256);		
		ass_eq(strcmp(hs2.cStr(), "/user/Meow1/dir/Merroowww/config.conf"), 0);
		ass_eq(strcmp(hs2.baseString(), "/user/%/dir/%/config.conf"), 0);
		ass_eq(hs2.remainingFeeds(), 0);
		ass_eq(hs2.isFull(), true);
		ass_eq(hs2.maxLength(), 256);		
		ass_eq(strcmp(hs3.cStr(), "/user/Meow1/dir/Woof/config.conf"), 0);
		ass_eq(strcmp(hs3.baseString(), "/user/%/dir/%/config.conf"), 0);
		ass_eq(hs3.remainingFeeds(), 0);
		ass_eq(hs3.isFull(), true);
		ass_eq(hs3.maxLength(), 256);		

		hs2 = HungryString();
		ass_eq(hs2.baseString(), NIL);
		ass_eq(hs2.isFull(), false);
		ass_eq(hs2.maxLength(), 0);
		ass_eq(hs2.remainingFeeds(), 0);
		ass_eq(hs2.cStr(), NIL);
		ass_eq(strcmp(hs.baseString(), "/user/%/dir/%/config.conf"), 0);
		ass_eq(strcmp(hs.cStr(), "/user/Meow1/dir/moooo/config.conf"), 0);
		ass_eq(hs.remainingFeeds(), 0);
		ass_eq(hs.isFull(), true);
		ass_eq(hs.maxLength(), 256);				
		ass_eq(strcmp(hs3.cStr(), "/user/Meow1/dir/Woof/config.conf"), 0);
		ass_eq(strcmp(hs3.baseString(), "/user/%/dir/%/config.conf"), 0);
		ass_eq(hs3.remainingFeeds(), 0);
		ass_eq(hs3.isFull(), true);
		ass_eq(hs3.maxLength(), 256);

		hs = hs2;
		ass_eq(hs2.baseString(), NIL);
		ass_eq(hs2.isFull(), false);
		ass_eq(hs2.maxLength(), 0);
		ass_eq(hs2.remainingFeeds(), 0);
		ass_eq(hs2.cStr(), NIL);
		ass_eq(hs.baseString(), NIL);
		ass_eq(hs.isFull(), false);
		ass_eq(hs.maxLength(), 0);
		ass_eq(hs.remainingFeeds(), 0);
		ass_eq(hs.cStr(), NIL);				
		ass_eq(strcmp(hs3.cStr(), "/user/Meow1/dir/Woof/config.conf"), 0);
		ass_eq(strcmp(hs3.baseString(), "/user/%/dir/%/config.conf"), 0);
		ass_eq(hs3.remainingFeeds(), 0);
		ass_eq(hs3.isFull(), true);
		ass_eq(hs3.maxLength(), 256);

		FINISH_TEST;
	}

	void testHungryStringFeedLessSimple() {
		BEGIN_TEST;

		HungryString hs = HungryString("/user/%/dir/%/config.conf", 256);
		ass_eq(strcmp(hs.baseString(), "/user/%/dir/%/config.conf"), 0);
		ass_eq(hs.isFull(), false);
		ass_eq(hs.maxLength(), 256);
		ass_eq(hs.remainingFeeds(), 2);
		ass_eq(hs.cStr(), NIL);

		hs.feed("%Meow1%").feed("%moooo%");		
		ass_eq(strcmp(hs.baseString(), "/user/%/dir/%/config.conf"), 0);
		ass_eq(hs.remainingFeeds(), 0);
		ass_eq(hs.isFull(), true);
		ass_eq(hs.maxLength(), 256);		
		ass_eq(strcmp(hs.cStr(), "/user/%Meow1%/dir/%moooo%/config.conf"), 0);

		hs.feed("Moasasdasd");
		ass_eq(strcmp(hs.baseString(), "/user/%/dir/%/config.conf"), 0);
		ass_eq(hs.remainingFeeds(), 0);
		ass_eq(hs.isFull(), true);
		ass_eq(hs.maxLength(), 256);		
		ass_eq(strcmp(hs.cStr(), "/user/%Meow1%/dir/%moooo%/config.conf"), 0);

		hs = HungryString("/%/%/%/%/%.file", 64);
		ass_eq(strcmp(hs.baseString(), "/%/%/%/%/%.file"), 0);
		ass_eq(hs.isFull(), false);
		ass_eq(hs.maxLength(), 64);
		ass_eq(hs.remainingFeeds(), 5);
		ass_eq(hs.cStr(), NIL);

		hs.feed("cat").feed("dog").feed("woof").feed("meow").feed("configy");
		ass_eq(strcmp(hs.baseString(), "/%/%/%/%/%.file"), 0);
		ass_eq(hs.isFull(), true);
		ass_eq(hs.maxLength(), 64);
		ass_eq(hs.remainingFeeds(), 0);
		ass_eq(strcmp(hs.cStr(), "/cat/dog/woof/meow/configy.file"), 0);		
		
		hs = HungryString("/usr/%%/%/meow", 64);
		ass_eq(strcmp(hs.baseString(), "/usr/%%/%/meow"), 0);
		ass_eq(hs.isFull(), false);
		ass_eq(hs.maxLength(), 64);
		ass_eq(hs.remainingFeeds(), 1);
		ass_eq(hs.cStr(), NIL);

		hs.feed("Mooooo%");
		ass_eq(strcmp(hs.baseString(), "/usr/%%/%/meow"), 0);
		ass_eq(hs.remainingFeeds(), 0);
		ass_eq(hs.isFull(), true);
		ass_eq(hs.maxLength(), 64);		
		ass_eq(strcmp(hs.cStr(), "/usr/%/Mooooo%/meow"), 0);

		hs = HungryString("/usr/%%%/meow", 64);
		ass_eq(strcmp(hs.baseString(), "/usr/%%%/meow"), 0);
		ass_eq(hs.isFull(), false);
		ass_eq(hs.maxLength(), 64);
		ass_eq(hs.remainingFeeds(), 1);
		ass_eq(hs.cStr(), NIL);
		
		hs.feed("Mooooo%");
		ass_eq(strcmp(hs.baseString(), "/usr/%%%/meow"), 0);
		ass_eq(hs.remainingFeeds(), 0);
		ass_eq(hs.isFull(), true);
		ass_eq(hs.maxLength(), 64);		
		ass_eq(strcmp(hs.cStr(), "/usr/%Mooooo%/meow"), 0);

		hs = HungryString("%%%/dir/file.file", 64);
		ass_eq(strcmp(hs.baseString(), "%%%/dir/file.file"), 0);
		ass_eq(hs.isFull(), false);
		ass_eq(hs.maxLength(), 64);
		ass_eq(hs.remainingFeeds(), 1);
		ass_eq(hs.cStr(), NIL);

		hs.feed("%Mooooo%");
		ass_eq(strcmp(hs.baseString(), "%%%/dir/file.file"), 0);
		ass_eq(hs.remainingFeeds(), 0);
		ass_eq(hs.isFull(), true);
		ass_eq(hs.maxLength(), 64);		
		ass_eq(strcmp(hs.cStr(), "%%Mooooo%/dir/file.file"), 0);

		hs = HungryString("%%/dir/file.file%%%", 64);
		ass_eq(strcmp(hs.baseString(), "%%/dir/file.file%%%"), 0);
		ass_eq(hs.isFull(), false);
		ass_eq(hs.maxLength(), 64);
		ass_eq(hs.remainingFeeds(), 1);
		ass_eq(hs.cStr(), NIL);

		hs.feed("MoooooMoo");
		ass_eq(strcmp(hs.baseString(), "%%/dir/file.file%%%"), 0);
		ass_eq(hs.remainingFeeds(), 0);
		ass_eq(hs.isFull(), true);
		ass_eq(hs.maxLength(), 64);		
		ass_eq(strcmp(hs.cStr(), "%/dir/file.file%MoooooMoo"), 0);

		hs = HungryString("%%%", 64);
		ass_eq(strcmp(hs.baseString(), "%%%"), 0);
		ass_eq(hs.isFull(), false);
		ass_eq(hs.maxLength(), 64);
		ass_eq(hs.remainingFeeds(), 1);
		ass_eq(hs.cStr(), NIL);

		hs.feed("MoooooMoo");
		ass_eq(strcmp(hs.baseString(), "%%%"), 0);
		ass_eq(hs.remainingFeeds(), 0);
		ass_eq(hs.isFull(), true);
		ass_eq(hs.maxLength(), 64);		
		ass_eq(strcmp(hs.cStr(), "%MoooooMoo"), 0);

		FINISH_TEST;
	}

} // namespace cc

int main(int argc, char** argv) {
	cc::testHungryStringCreateAndDestroy();
	cc::testHungryStringFeedSimple();
	cc::testHungryStringFeedNoFeeds();
	cc::testHungryStringFeedAndCopy();
	cc::testHungryStringFeedLessSimple();	
	
	
	return 0;
}

