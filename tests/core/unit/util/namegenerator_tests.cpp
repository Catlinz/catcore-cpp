#include "core/testcore.h"
#include "core/util/namegenerator.h"

namespace cc {

	

	void testNameGeneratorCreateAndDestroy() {
		BEGIN_TEST;

		NameGenerator g;
		ass_eq(g.prefix(), NIL);
		ass_eq(g.maxLength(), 0);
		ass_eq(g.prefixLength(), 0);
		ass_eq(g.count(), 0);

		g = NameGenerator("Test");
		ass_eq(strcmp(g.prefix(), "Test"), 0);
		ass_eq(g.maxLength(), 17);
		ass_eq(g.prefixLength(), 4);
		ass_eq(g.count(), 0);

		NameGenerator g2 = g;
		ass_eq(strcmp(g2.prefix(), "Test"), 0);
		ass_eq(g2.maxLength(), 17);
		ass_eq(g2.prefixLength(), 4);
		ass_eq(g2.count(), 0);

		NameGenerator* ng = new NameGenerator();
		ass_eq(ng->prefix(), NIL);
		ass_eq(ng->maxLength(), 0);
		ass_eq(ng->prefixLength(), 0);
		ass_eq(ng->count(), 0);
		delete ng;

		ng = new NameGenerator("Test2");
		ass_eq(strcmp(ng->prefix(), "Test2"), 0);
		ass_eq(ng->maxLength(), 18);
		ass_eq(ng->prefixLength(), 5);
		ass_eq(ng->count(), 0);

		delete ng;		
		
		FINISH_TEST;		
	}

	void testNameGeneratorInitWithPrefix() {
		BEGIN_TEST;

		NameGenerator g;
		ass_eq(g.prefix(), NIL);
		ass_eq(g.maxLength(), 0);
		ass_eq(g.prefixLength(), 0);
		ass_eq(g.count(), 0);

		g.initWithPrefix("Test");
		ass_eq(strcmp(g.prefix(), "Test"), 0);
		ass_eq(g.maxLength(), 17);
		ass_eq(g.prefixLength(), 4);
		ass_eq(g.count(), 0);

		g.initWithPrefix("Test2");
		ass_eq(strcmp(g.prefix(), "Test"), 0);
		ass_eq(g.maxLength(), 17);
		ass_eq(g.prefixLength(), 4);
		ass_eq(g.count(), 0);

		NameGenerator* ng = new NameGenerator();
		ass_eq(ng->prefix(), NIL);
		ass_eq(ng->maxLength(), 0);
		ass_eq(ng->prefixLength(), 0);
		ass_eq(ng->count(), 0);

		ng->initWithPrefix("Test2");
		ass_eq(strcmp(ng->prefix(), "Test2"), 0);
		ass_eq(ng->maxLength(), 18);
		ass_eq(ng->prefixLength(), 5);
		ass_eq(ng->count(), 0);

		delete ng;		
		FINISH_TEST;
	}

	void testNameGeneratorGenerate() {
		BEGIN_TEST;

		NameGenerator g("Test");
		Char* str = g.generate();
		std::cout << str << std::endl;
		ass_eq(strcmp(str, "Test=/++++++++++"), 0);
		delete[] str;		
		str = g.generate();
		std::cout << str << std::endl;
		ass_eq(strcmp(str, "Test=0++++++++++"), 0);

		for (I32 i = 0; i < 1000; i++) {
			delete[] str;
			str = g.generate();
		}

		std::cout << str << std::endl;
		ass_eq(strcmp(str, "Test=eD+++++++++"), 0);

		delete[] str;		

		NameGenerator g2 = g;
		str = g.generate();
		std::cout << str << std::endl;
		ass_eq(strcmp(str, "Test=fD+++++++++"), 0);
		delete[] str;

		str = g2.generate();
		std::cout << str << std::endl;
		ass_eq(strcmp(str, "Test=fD+++++++++"), 0);
		delete[] str;


		NameGenerator* ng = new NameGenerator();
		ng->initWithPrefix("MeowMix");
		str = ng->generate();
		std::cout << str << std::endl;
		ass_eq(strcmp(str, "MeowMix=/++++++++++"), 0);
		delete[] str;

		NameGenerator* ng2 = new NameGenerator(*ng);		
		str = ng->generate();
		std::cout << str << std::endl;
		ass_eq(strcmp(str, "MeowMix=0++++++++++"), 0);
		delete[] str;
		
		str = ng2->generate();
		std::cout << str << std::endl;
		ass_eq(strcmp(str, "MeowMix=0++++++++++"), 0);

		for (I32 i = 0; i < 1000; i++) {
			delete[] str;
			str = ng->generate();
		}

		std::cout << str << std::endl;
		ass_eq(strcmp(str, "MeowMix=eD+++++++++"), 0);
		delete[] str;

		str = ng2->generate();
		std::cout << str << std::endl;
		ass_eq(strcmp(str, "MeowMix=1++++++++++"), 0);
		delete[] str;
		
		delete ng;

		str = ng2->generate();
		std::cout << str << std::endl;
		ass_eq(strcmp(str, "MeowMix=2++++++++++"), 0);
		delete[] str;

		delete ng2;		

		FINISH_TEST;
	}

} // namespace cc

int main(int argc, char** argv) {
	cc::testNameGeneratorCreateAndDestroy();
	cc::testNameGeneratorInitWithPrefix();
	cc::testNameGeneratorGenerate();	
	return 0;
}

