#include "core/testcore.h"
#include "core/string/string.h"

namespace cc {
	
	void testStringCreateAndDestroy() {
		BEGIN_TEST;

		String s;
		ass_true(s.isEmpty());
		ass_eq(s.length(), 0);
		ass_eq(s.cStr(), NIL);

		String s2('c');
		ass_false(s2.isEmpty());
		ass_eq(s2.length(), 1);
		ass_neq(s2.cStr(), NIL);
		ass_eq(s2[0], 'c');
		ass_eq(s2[1], '\0');

		String s3("Meow");
		ass_false(s3.isEmpty());
		ass_eq(s3.length(), 4);
		ass_neq(s3.cStr(), NIL);
		ass_eq(s3[0], 'M');
		ass_eq(s3[1], 'e');
		ass_eq(s3[2], 'o');
		ass_eq(s3[3], 'w');
		ass_eq(s3[4], '\0');

		String* sp = new String();
		ass_true(sp->isEmpty());
		ass_eq(sp->length(), 0);
		ass_eq(sp->cStr(), NIL);
		delete sp;

		sp = new String('z');
		ass_false(sp->isEmpty());
		ass_eq(sp->length(), 1);
		ass_neq(sp->cStr(), NIL);
		ass_eq(sp->at(0), 'z');
		ass_eq(sp->at(1), '\0');
		delete sp;

		sp = new String("Moo");
		ass_false(sp->isEmpty());
		ass_eq(sp->length(), 3);
		ass_neq(sp->cStr(), NIL);
		ass_eq(sp->at(0), 'M');
		ass_eq(sp->at(1), 'o');
		ass_eq(sp->at(2), 'o');
		ass_eq(sp->at(3), '\0');
		delete sp;

		StringPtr spp = String::create();
		ass_true(spp->isEmpty());
		ass_eq(spp->length(), 0);
		ass_eq(spp->cStr(), NIL);

		spp = String::create('q');
		ass_false(spp->isEmpty());
		ass_eq(spp->length(), 1);
		ass_neq(spp->cStr(), NIL);
		ass_eq(spp->at(0), 'q');
		ass_eq(spp->at(1), '\0');

		spp = String::create("Woof");
		ass_false(spp->isEmpty());
		ass_eq(spp->length(), 4);
		ass_neq(spp->cStr(), NIL);
		ass_eq(spp->at(0), 'W');
		ass_eq(spp->at(1), 'o');
		ass_eq(spp->at(2), 'o');
		ass_eq(spp->at(3), 'f');
		ass_eq(spp->at(4), '\0');
		
		FINISH_TEST;		
	}

	void testStringEquality() {
		BEGIN_TEST;
		
		String s;
		ass_true(s.isEmpty());
		ass_eq(s.length(), 0);
		const Char* test = NIL;		
		ass_eq(s.cStr(), (Char*)0);
		ass_true(s == test && s.equals(test));
		ass_true(s == s && s.equals(s));
		ass_false(s != test || !s.equals(test));
		ass_false(s != s || !s.equals(s));
		ass_true(s == "" && s.equals(""));
		ass_false( s != "" || !s.equals(""));
		ass_false( s == "a" || s.equals("a"));
		ass_true( s != "a" && !s.equals("a"));
		ass_false( s == 'a' || s.equals('a'));
		ass_true( s != 'a' && !s.equals('a'));
		ass_true( s == '\0' && s.equals('\0'));
		ass_false( s != '\0');		

		String s2("Meow");
		ass_false(s2.isEmpty());
		ass_eq(s2.length(), 4);
		ass_neq(s2.cStr(), (Char*)NIL);
		ass_false(s2 == (Char*)NIL && s2.equals((Char*)NIL));
		ass_true(s2 == s2 && s2.equals(s2));
		ass_true(s2 != (Char*)NIL && !s2.equals((Char*)NIL));
		ass_false(s2 != s2 || !s2.equals(s2));
		ass_false(s2 == s || s2.equals(s));
		ass_false(s == s2 || s.equals(s2));
		ass_true(s2 != s && !s2.equals(s));
		ass_true(s != s2 && !s.equals(s2));
		ass_true(s2 == "Meow" || s2.equals("Meow"));
		ass_false(s2 != "Meow" || !s2.equals("Meow"));
		ass_false(s2 == "meow" || s2.equals("meow"));
		ass_false(s2 == "" || s2.equals(""));
		ass_false(s2 == 'M' || s2.equals('M'));
		ass_true(s2 != 'M');
		ass_false(s2 == '\0' || s2.equals('\0'));
		ass_true(s2 != '\0');	

		String s3('M');
		ass_false(s3.isEmpty());
		ass_eq(s3.length(), 1);
		ass_neq(s3.cStr(), (Char*)NIL);
		ass_false(s3 == (Char*)NIL || s3.equals((Char*)NIL));
		ass_true(s3 == s3 && s3.equals(s3));
		ass_true(s3 != (Char*)NIL && !s3.equals((Char*)NIL));
		ass_false(s3 != s3 || !s3.equals(s3));
		ass_false(s3 == s2 || s3.equals(s2));
		ass_false(s2 == s3 || s2.equals(s3));
		ass_true(s3 != s && !s3.equals(s2));
		ass_true(s2 != s3 && !s2.equals(s3));
		ass_true(s3 == "M" && s3.equals("M"));
		ass_false(s3 != "M" || !s3.equals("M"));
		ass_false(s3 == "m" || s3.equals("m"));
		ass_true(s3 != "m" && !s3.equals("m"));		
		ass_false(s3 == "" || s3.equals(""));
		ass_true(s3 != "" && !s3.equals(""));
		ass_true(s3 == 'M' && s3.equals('M'));
		ass_false(s3 != 'M');
		ass_false(s3 == 'm' || s3.equals('m'));
		ass_true(s3 != 'm');
		ass_false(s3 == '\0' || s3.equals('\0'));
		ass_true(s3 != '\0');		
		
		
		FINISH_TEST;
	}

	void testStringEqualityIgnoreCase() {
		BEGIN_TEST;

		String s;
		ass_true(s.isEmpty());
		ass_eq(s.length(), 0);
		ass_eq(s.cStr(), (Char*)NIL);
		ass_true(s.equalsIgnoreCase((Char*)NIL));
		ass_true(s.equalsIgnoreCase(s));
		ass_true(s.equalsIgnoreCase(""));
		ass_false( s.equalsIgnoreCase("a"));
		ass_false( s.equalsIgnoreCase('a'));
		ass_true( s.equalsIgnoreCase('\0'));		
		

		String s2("Meow");
		ass_false(s2.isEmpty());
		ass_eq(s2.length(), 4);
		ass_neq(s2.cStr(), (Char*)NIL);
		ass_false(s2.equalsIgnoreCase((Char*)NIL));
		ass_true(s2.equalsIgnoreCase(s2));
		ass_false(s2.equalsIgnoreCase(s));
		ass_false(s.equalsIgnoreCase(s2));
		ass_true(s2.equalsIgnoreCase("Meow"));
		ass_true(s2.equalsIgnoreCase("meow"));
		ass_false(s2.equalsIgnoreCase(""));
		ass_false(s2.equalsIgnoreCase("meoe"));
		ass_false(s2.equalsIgnoreCase("Meoe"));
		ass_false( s2.equalsIgnoreCase('a') );
		ass_false( s2.equalsIgnoreCase('\0') );	

		String s3('M');
		ass_false(s3.isEmpty());
		ass_eq(s3.length(), 1);
		ass_neq(s3.cStr(), (Char*)NIL);
		ass_false(s3.equalsIgnoreCase((Char*)NIL));
		ass_true(s3.equalsIgnoreCase(s3));
		ass_false(s3.equalsIgnoreCase(s2));
		ass_false(s2.equalsIgnoreCase(s3));
		ass_true(s3.equalsIgnoreCase("M"));
		ass_true(s3.equalsIgnoreCase("m"));
		ass_false(s3.equalsIgnoreCase("n"));
		ass_false(s3.equalsIgnoreCase("N"));		
		ass_false(s3.equalsIgnoreCase(""));
		ass_false( s3.equalsIgnoreCase('n') );
		ass_false( s3.equalsIgnoreCase('\0') );
		ass_true( s3.equalsIgnoreCase('m') );
		ass_true( s3.equalsIgnoreCase('M') );

		String s4("Word!");
		ass_false(s4.isEmpty());
		ass_eq(s4.length(), 5);
		ass_neq(s4.cStr(), (Char*)NIL);
		ass_true(s4 == "Word!");		
		ass_true(s4.equalsIgnoreCase("Word!"));
		ass_true(s4.equalsIgnoreCase("word!"));
		ass_true(s4.equalsIgnoreCase("wOrD!"));
		ass_true(s4.equalsIgnoreCase("WORD!"));
		ass_true(s4.equalsIgnoreCase("WOrd!"));
		ass_true(s4.equalsIgnoreCase("woRD!"));
		ass_true(s4.equalsIgnoreCase("WorD!"));
		ass_true(s4.equalsIgnoreCase("worD!"));
		ass_false(s4.equalsIgnoreCase("word~"));
		ass_false(s4.equalsIgnoreCase("word"));
		ass_false( s4.equalsIgnoreCase('W') );
		ass_false( s4.equalsIgnoreCase('\0') );
		

		FINISH_TEST;
	}	

	void testStringCopyAssignmentAndClone() {
		BEGIN_TEST;
		
		String s;
		ass_true(s.isEmpty());
		ass_eq(s.length(), 0);
		ass_eq(s.cStr(), (Char*)NIL);
		
		String s2("Meow");
		ass_false(s2.isEmpty());
		ass_eq(s2.length(), 4);
		ass_neq(s2.cStr(), (Char*)NIL);
		ass_true(s2 == "Meow");		

		s = s2;
	   ass_false(s.isEmpty());
		ass_eq(s.length(), 4);
		ass_neq(s.cStr(), (Char*)NIL);
		ass_true(s == "Meow");	  

		s = String();
		ass_true(s.isEmpty());
		ass_eq(s.length(), 0);
		ass_eq(s.cStr(), (Char*)NIL);
		ass_false(s2.isEmpty());
		ass_eq(s2.length(), 4);
		ass_neq(s2.cStr(), (Char*)NIL);
		ass_true(s2 == "Meow");		

		s2 = s;
		ass_true(s2.isEmpty());
		ass_eq(s2.length(), 0);
		ass_eq(s2.cStr(), (Char*)NIL);
		ass_true(s2 == "");		

		s2 = String("Moooo");
		ass_false(s2.isEmpty());
		ass_eq(s2.length(), 5);
		ass_neq(s2.cStr(), (Char*)NIL);
		ass_true(s.isEmpty());
		ass_eq(s.length(), 0);
		ass_eq(s.cStr(), (Char*)NIL);
		ass_true(s2 == "Moooo");
		ass_true(s == "");		

		String s3(s2);
		ass_false(s2.isEmpty());
		ass_eq(s2.length(), 5);
		ass_neq(s2.cStr(), (Char*)NIL);
		ass_false(s3.isEmpty());
		ass_eq(s3.length(), 5);
		ass_neq(s3.cStr(), (Char*)NIL);
		ass_true(s3 == "Moooo" && s2 == "Moooo" && s == "");		

		s2 = s;
		ass_true(s2.isEmpty());
		ass_eq(s2.length(), 0);
		ass_eq(s2.cStr(), (Char*)NIL);
		ass_false(s3.isEmpty());
		ass_eq(s3.length(), 5);
		ass_neq(s3.cStr(), (Char*)NIL);
		ass_true(s3 == "Moooo" && s2 == "" && s == "");

		s3 = String(s);
		ass_true(s.isEmpty());
		ass_eq(s.length(), 0);
		ass_eq(s.cStr(), (Char*)NIL);
		ass_true(s2.isEmpty());
		ass_eq(s2.length(), 0);
		ass_eq(s2.cStr(), (Char*)NIL);
		ass_true(s3.isEmpty());
		ass_eq(s3.length(), 0);
		ass_eq(s3.cStr(), (Char*)NIL);
		ass_true(s3 == "" && s2 == "" && s == "");

		s3 = String("Merow");
		ass_true(s.isEmpty());
		ass_eq(s.length(), 0);
		ass_eq(s.cStr(), (Char*)NIL);
		ass_true(s2.isEmpty());
		ass_eq(s2.length(), 0);
		ass_eq(s2.cStr(), (Char*)NIL);
		ass_false(s3.isEmpty());
		ass_eq(s3.length(), 5);
		ass_neq(s3.cStr(), (Char*)NIL);
		ass_true(s3 == "Merow" && s2 == "" && s == "");

		s2 = "Mo!";
		ass_true(s.isEmpty());
		ass_eq(s.length(), 0);
		ass_eq(s.cStr(), (Char*)NIL);
		ass_false(s2.isEmpty());
		ass_eq(s2.length(), 3);
		ass_neq(s2.cStr(), (Char*)NIL);
		ass_false(s3.isEmpty());
		ass_eq(s3.length(), 5);
		ass_neq(s3.cStr(), (Char*)NIL);
		ass_true(s3 == "Merow" && s2 == "Mo!" && s == "");

		s.copy(s2);
		ass_false(s.isEmpty());
		ass_eq(s.length(), 3);
		ass_neq(s.cStr(), (Char*)NIL);
		ass_false(s2.isEmpty());
		ass_eq(s2.length(), 3);
		ass_neq(s2.cStr(), (Char*)NIL);
		ass_false(s3.isEmpty());
		ass_eq(s3.length(), 5);
		ass_neq(s3.cStr(), (Char*)NIL);
		ass_true(s3 == "Merow" && s2 == "Mo!" && s == "Mo!");

		s2.copy(s3);
		ass_false(s.isEmpty());
		ass_eq(s.length(), 3);
		ass_neq(s.cStr(), (Char*)NIL);
		ass_false(s2.isEmpty());
		ass_eq(s2.length(), 5);
		ass_neq(s2.cStr(), (Char*)NIL);
		ass_false(s3.isEmpty());
		ass_eq(s3.length(), 5);
		ass_neq(s3.cStr(), (Char*)NIL);
		ass_true(s3 == "Merow" && s2 == "Merow" && s == "Mo!");

		StringPtr sp = s3.clone();
		ass_false(sp->isEmpty());
		ass_eq(sp->length(), 5);
		ass_neq(sp->cStr(), (Char*)NIL);
		ass_true(sp->equals("Merow"));

		StringPtr sp2 = sp;
		ass_false(sp2->isEmpty());
		ass_eq(sp2->length(), 5);
		ass_neq(sp2->cStr(), (Char*)NIL);
		ass_true(sp2->equals("Merow"));

		sp = s.clone();
		ass_false(sp->isEmpty());
		ass_eq(sp->length(), 3);
		ass_neq(sp->cStr(), (Char*)NIL);
		ass_true(sp->equals("Mo!"));
		ass_false(sp2->isEmpty());
		ass_eq(sp2->length(), 5);
		ass_neq(sp2->cStr(), (Char*)NIL);
		ass_true(sp2->equals("Merow"));

		s3 = "Nothing.";
		ass_false(sp->isEmpty());
		ass_eq(sp->length(), 3);
		ass_neq(sp->cStr(), (Char*)NIL);
		ass_true(sp->equals("Mo!"));
		ass_false(sp2->isEmpty());
		ass_eq(sp2->length(), 5);
		ass_neq(sp2->cStr(), (Char*)NIL);
		ass_true(sp2->equals("Merow"));
		ass_false(s3.isEmpty());
		ass_eq(s3.length(), 8);
		ass_neq(s3.cStr(), (Char*)NIL);
		ass_true(s3 == "Nothing." && s2 == "Merow" && s == "Mo!");				
		
		FINISH_TEST;
	}
	

} // namespace cc

int main(int argc, char** argv) {
	cc::testStringCreateAndDestroy();
	cc::testStringEquality();
	cc::testStringEqualityIgnoreCase();
	cc::testStringCopyAssignmentAndClone();		
	
	return 0;
}

