#include "core/testcore.h"
#include "core/util/staticmap.h"

namespace cc {

	Size destroyed_count = 0;

	class TestObject {
	  public:
		TestObject() : m_x(0.0f), m_y(0.0f), m_z(0.0f) {}
		TestObject(F32 x, F32 y, F32 z) : m_x(x), m_y(y), m_z(z) {}

		inline Boolean operator==(const TestObject& rval) const {
			return (m_x == rval.m_x && m_y == rval.m_y && m_z == rval.m_z);
		}

		inline Boolean operator!=(const TestObject& rval) const {
			return !(*this == rval);
		}		

		~TestObject() {
			destroyed_count++;
		}		

		Boolean test(F32 x, F32 y, F32 z) const {
			return m_x == x && m_y == y && m_z == z;
		}

		F32 m_x, m_y, m_z;

		friend std::ostream& operator<<(std::ostream& out, const TestObject& o) {
			return out << "(" << o.m_x << ", " << o.m_y << ", " << o.m_z << ")";
		}	

	};

	void testStaticMapBasicCreateAndDestroy() {
		BEGIN_TEST;
		
		StaticMap<I32> map;
		ass_eq(map.capacity(), 0);
		ass_eq(map.loadFactor(), 0.0f);
		ass_eq(map.size(), 0);
		ass_true(map.isEmpty());
		ass_eq(map.arrayLength(), 0);

		map.initWithCapacityAndLoadFactor(2, 0.2f, 0);
		ass_eq(map.capacity(), 2);
		ass_eq(map.loadFactor(), 0.2f);
		ass_eq(map.size(), 0);
		ass_true(map.isEmpty());
		ass_eq(map.arrayLength(), 10);
		ass_eq(map.nullValue(), 0);

		map = StaticMap<I32>(2, -1);
		ass_eq(map.capacity(), 2);
		ass_eq(map.loadFactor(), 0.5f);
		ass_eq(map.size(), 0);
		ass_true(map.isEmpty());
		ass_eq(map.arrayLength(), 4);
		ass_eq(map.nullValue(), -1);
		
		StaticMap<TestObject> map2(2, TestObject(-1.0f, -1.0f, -1.0f));
		ass_eq(map2.capacity(), 2);
		ass_eq(map2.loadFactor(), 0.5f);
		ass_eq(map2.size(), 0);
		ass_true(map2.isEmpty());
		ass_eq(map2.arrayLength(), 4);
		ass_eq(map2.nullValue(), TestObject(-1.0f, -1.0f, -1.0f));

		StaticMap<TestObject> map3(2, 0.2f, TestObject());
		ass_eq(map3.capacity(), 2);
		ass_eq(map3.loadFactor(), 0.2f);
		ass_eq(map3.size(), 0);
		ass_true(map3.isEmpty());
		ass_eq(map3.arrayLength(), 10);
		ass_eq(map3.nullValue(), TestObject(0.0f, 0.0f, 0.0f));

		StaticMap<TestObject>* map4 = new StaticMap<TestObject>(4, TestObject(-1.0f, -1.0f, -1.0f));
		ass_eq(map4->capacity(), 4);
		ass_eq(map4->loadFactor(), 0.5f);
		ass_eq(map4->size(), 0);
		ass_true(map4->isEmpty());
		ass_eq(map4->arrayLength(), 8);
		ass_eq(map4->nullValue(), TestObject(-1.0f, -1.0f, -1.0f));

		destroyed_count = 0;
		ass_eq(destroyed_count, 0);		
		delete map4;
		ass_eq(destroyed_count, 9);  /* arrayLength + 1 (nullvalue) */
			
		FINISH_TEST;
	}

	void testStaticMapCopy() {
		BEGIN_TEST;

		StaticMap<TestObject> map(4, TestObject(-1.0f, -1.0f, -1.0f));
		ass_eq(map.capacity(), 4);
		ass_eq(map.loadFactor(), 0.5f);
		ass_eq(map.size(), 0);
		ass_true(map.isEmpty());
		ass_eq(map.arrayLength(), 8);
		ass_eq(map.nullValue(), TestObject(-1.0f, -1.0f, -1.0f));

		StaticMap<TestObject> map2(map);
		ass_eq(map2.capacity(), 4);
		ass_eq(map2.loadFactor(), 0.5f);
		ass_eq(map2.size(), 0);
		ass_true(map2.isEmpty());
		ass_eq(map2.arrayLength(), 8);
		ass_eq(map2.nullValue(), TestObject(-1.0f, -1.0f, -1.0f));

		destroyed_count = 0;
		map2 = map;		
		ass_eq(destroyed_count, 8);
		ass_eq(map2.capacity(), 4);
		ass_eq(map2.loadFactor(), 0.5f);
		ass_eq(map2.size(), 0);
		ass_true(map2.isEmpty());
		ass_eq(map2.arrayLength(), 8);
		ass_eq(map2.nullValue(), TestObject(-1.0f, -1.0f, -1.0f));		
		ass_eq(map.capacity(), 4);
		ass_eq(map.loadFactor(), 0.5f);
		ass_eq(map.size(), 0);
		ass_true(map.isEmpty());
		ass_eq(map.arrayLength(), 8);
		ass_eq(map.nullValue(), TestObject(-1.0f, -1.0f, -1.0f));

		map2 = StaticMap<TestObject>(2, TestObject(0.0f, 0.0f, 0.0f));
		ass_eq(map2.capacity(), 2);
		ass_eq(map2.loadFactor(), 0.5f);
		ass_eq(map2.size(), 0);
		ass_true(map2.isEmpty());
		ass_eq(map2.arrayLength(), 4);
		ass_eq(map2.nullValue(), TestObject(0.0f, 0.0f, 0.0f));

		destroyed_count = 0;
		ass_eq(destroyed_count, 0);
		map2 = map;
		ass_eq(destroyed_count, 4);
		ass_eq(map2.capacity(), 4);
		ass_eq(map2.loadFactor(), 0.5f);
		ass_eq(map2.size(), 0);
		ass_true(map2.isEmpty());
		ass_eq(map2.arrayLength(), 8);
		ass_eq(map2.nullValue(), TestObject(-1.0f, -1.0f, -1.0f));		
		ass_eq(map.capacity(), 4);
		ass_eq(map.loadFactor(), 0.5f);
		ass_eq(map.size(), 0);
		ass_true(map.isEmpty());
		ass_eq(map.arrayLength(), 8);
		ass_eq(map.nullValue(), TestObject(-1.0f, -1.0f, -1.0f));

		map.insert("test1", TestObject(1.0f, 2.0f, 3.0f));
		map.insert("test2", TestObject(-1.0f, 2.0f, -3.0f));
		map.insert("test3", TestObject(34.4f, 232.1f, 232.4f));

		ass_true(map.contains("test1") && map.contains("test2") && map.contains("test3"));
		ass_eq(map.at("test1"), TestObject(1.0f, 2.0f, 3.0f));
		ass_eq(map.at("test2"), TestObject(-1.0f, 2.0f, -3.0f));
		ass_eq(map.at("test3"), TestObject(34.4f, 232.1f, 232.4f));

		ass_false(map2.contains("test1") && map2.contains("test2") && map2.contains("test3"));

		map2 = map;
		ass_true(map2.contains("test1") && map2.contains("test2") && map2.contains("test3"));
		ass_true(map.contains("test1") && map.contains("test2") && map.contains("test3"));

		map.clear();
		ass_false(map.contains("test1") && map.contains("test2") && map.contains("test3"));
		ass_true(map2.contains("test1") && map2.contains("test2") && map2.contains("test3"));
		
		FINISH_TEST;
	}
	

	void testStaticMapBasicObjectInsert() {
		BEGIN_TEST;

		TestObject nullObj = TestObject(-1.0f, -1.0f, -1.0f);
		TestObject meow = TestObject(1.0f, 2.0f, 3.0f);
		TestObject moo = TestObject(2.0f, 4.0f, -6.0f);
		TestObject moo2 = TestObject(0.0f, 0.0f, 0.0f);
		TestObject meow0 = TestObject(-42.0f, 34.23f, 0.0f);
		TestObject mooo = TestObject(-1.0f, -1.0f, -1.01f);
		
		
		StaticMap<TestObject> map(5, nullObj);
		ass_eq(map.capacity(), 5);
		ass_eq(map.loadFactor(), 0.5f);
		ass_eq(map.size(), 0);
		ass_true(map.isEmpty());
		ass_eq(map.arrayLength(), 10);
		ass_eq(map.nullValue(), nullObj);

		ass_false(map.contains("meow"));
		ass_false(map.contains("moo"));
		ass_false(map.contains("moo2"));
		ass_false(map.contains("meow0"));
		ass_false(map.contains("mooo"));

		ass_eq(map.at("meow"), nullObj);
		ass_eq(map.at("moo"), nullObj);
		ass_eq(map.at("moo2"), nullObj);
		ass_eq(map.at("meow0"), nullObj);
		ass_eq(map.at("mooo"), nullObj);

		map.insert("meow", TestObject(1.0f, 2.0f, 3.0f));
		ass_eq(map.size(), 1);
		ass_eq(map.capacity(), 5);
		ass_eq(map.arrayLength(), 10);		
		map.insert("moo", moo);
		ass_eq(map.size(), 2);
		ass_eq(map.capacity(), 5);
		ass_eq(map.arrayLength(), 10);
		map.insert("moo2", moo2);
		ass_eq(map.size(), 3);
		ass_eq(map.capacity(), 5);
		ass_eq(map.arrayLength(), 10);
		map.insert("meow0", meow0);
		ass_eq(map.size(), 4);
		ass_eq(map.capacity(), 5);
		ass_eq(map.arrayLength(), 10);
		map.insert("mooo", mooo);
		ass_eq(map.size(), 5);
		ass_eq(map.capacity(), 5);
		ass_eq(map.arrayLength(), 10);

		ass_true(map.contains("meow"));
		ass_true(map.contains("moo"));
		ass_true(map.contains("moo2"));
		ass_true(map.contains("meow0"));
		ass_true(map.contains("mooo"));

		ass_eq(map.at("meow"), meow);
		ass_eq(map.at("moo"), TestObject(2.0f, 4.0f, -6.0f));
		ass_eq(map.at("moo2"), moo2);
		ass_eq(map.at("meow0"), meow0);
		ass_eq(map.at("mooo"), mooo);

		ass_eq(map.capacity(), 5);
		ass_eq(map.loadFactor(), 0.5f);
		ass_eq(map.size(), 5);
		ass_false(map.isEmpty());
		ass_eq(map.arrayLength(), 10);
		ass_eq(map.nullValue(), nullObj);		
			
		FINISH_TEST;
	}

	
	void testStaticMapBasicIntegerInsert() {
		BEGIN_TEST;

		I32 nullObj = -1;
		I32 meow = 1;
		I32 moo = 2;
		I32 moo2 = 0;
		I32 meow0 = -2;
		I32 mooo = -99;
		
		
		StaticMap<I32> map(5, nullObj);
		ass_eq(map.capacity(), 5);
		ass_eq(map.loadFactor(), 0.5f);
		ass_eq(map.size(), 0);
		ass_true(map.isEmpty());
		ass_eq(map.arrayLength(), 10);
		ass_eq(map.nullValue(), nullObj);

		ass_false(map.contains("meow"));
		ass_false(map.contains("moo"));
		ass_false(map.contains("moo2"));
		ass_false(map.contains("meow0"));
		ass_false(map.contains("mooo"));

		ass_eq(map.at("meow"), nullObj);
		ass_eq(map.at("moo"), nullObj);
		ass_eq(map.at("moo2"), nullObj);
		ass_eq(map.at("meow0"), nullObj);
		ass_eq(map.at("mooo"), nullObj);

		map.insert("meow", 1);
		ass_eq(map.size(), 1);
		ass_eq(map.capacity(), 5);
		ass_eq(map.arrayLength(), 10);		
		map.insert("moo", moo);
		ass_eq(map.size(), 2);
		ass_eq(map.capacity(), 5);
		ass_eq(map.arrayLength(), 10);
		map.insert("moo2", moo2);
		ass_eq(map.size(), 3);
		ass_eq(map.capacity(), 5);
		ass_eq(map.arrayLength(), 10);
		map.insert("meow0", meow0);
		ass_eq(map.size(), 4);
		ass_eq(map.capacity(), 5);
		ass_eq(map.arrayLength(), 10);
		map.insert("mooo", mooo);
		ass_eq(map.size(), 5);
		ass_eq(map.capacity(), 5);
		ass_eq(map.arrayLength(), 10);

		ass_true(map.contains("meow"));
		ass_true(map.contains("moo"));
		ass_true(map.contains("moo2"));
		ass_true(map.contains("meow0"));
		ass_true(map.contains("mooo"));

		ass_eq(map.at("meow"), meow);
		ass_eq(map.at("moo"), 2);
		ass_eq(map.at("moo2"), moo2);
		ass_eq(map.at("meow0"), meow0);
		ass_eq(map.at("mooo"), mooo);

		ass_eq(map.capacity(), 5);
		ass_eq(map.loadFactor(), 0.5f);
		ass_eq(map.size(), 5);
		ass_false(map.isEmpty());
		ass_eq(map.arrayLength(), 10);
		ass_eq(map.nullValue(), nullObj);		
			
		FINISH_TEST;
	}


	void testStaticMapBasicAutoIncreaseCapacity() {
		BEGIN_TEST;

		TestObject nullObj = TestObject(-1.0f, -1.0f, -1.0f);
		TestObject meow = TestObject(1.0f, 2.0f, 3.0f);
		TestObject moo = TestObject(2.0f, 4.0f, -6.0f);
		TestObject moo2 = TestObject(0.0f, 0.0f, 0.0f);
		TestObject meow0 = TestObject(-42.0f, 34.23f, 0.0f);
		TestObject mooo = TestObject(-1.0f, -1.0f, -1.01f);
		
		
		StaticMap<TestObject> map(5, nullObj);
		ass_eq(map.capacity(), 5);
		ass_eq(map.loadFactor(), 0.5f);
		ass_eq(map.size(), 0);
		ass_true(map.isEmpty());
		ass_eq(map.arrayLength(), 10);
		ass_eq(map.nullValue(), nullObj);

		map.insert("meow", TestObject(1.0f, 2.0f, 3.0f));
		ass_eq(map.size(), 1);
		ass_eq(map.capacity(), 5);
		ass_eq(map.arrayLength(), 10);		
		map.insert("moo", moo);
		map.insert("moo2", moo2);
		map.insert("meow0", meow0);
		map.insert("mooo", mooo);
		ass_eq(map.size(), 5);
		ass_eq(map.capacity(), 5);
		ass_eq(map.arrayLength(), 10);

		ass_true(map.contains("meow"));
		ass_true(map.contains("moo"));
		ass_true(map.contains("moo2"));
		ass_true(map.contains("meow0"));
		ass_true(map.contains("mooo"));

		ass_eq(map.at("meow"), meow);
		ass_eq(map.at("moo"), TestObject(2.0f, 4.0f, -6.0f));
		ass_eq(map.at("moo2"), moo2);
		ass_eq(map.at("meow0"), meow0);
		ass_eq(map.at("mooo"), mooo);

		ass_eq(map.capacity(), 5);
		ass_eq(map.loadFactor(), 0.5f);
		ass_eq(map.size(), 5);
		ass_false(map.isEmpty());
		ass_eq(map.arrayLength(), 10);
		ass_eq(map.nullValue(), nullObj);

		map.insert("Uhoh", TestObject(1.0f, 343.0f, -2323.0f));
		ass_eq(map.capacity(), 10);
		ass_eq(map.loadFactor(), 0.5f);
		ass_eq(map.size(), 6);
		ass_false(map.isEmpty());
		ass_eq(map.arrayLength(), 20);
		ass_eq(map.nullValue(), nullObj);


		FINISH_TEST;
	}

	void testStaticMapBasicClear() {
		BEGIN_TEST;

		TestObject nullObj = TestObject(-1.0f, -1.0f, -1.0f);
		TestObject meow = TestObject(1.0f, 2.0f, 3.0f);
		TestObject moo = TestObject(2.0f, 4.0f, -6.0f);
		TestObject moo2 = TestObject(0.0f, 0.0f, 0.0f);
		TestObject meow0 = TestObject(-42.0f, 34.23f, 0.0f);
		TestObject mooo = TestObject(-1.0f, -1.0f, -1.01f);
		
		
		StaticMap<TestObject> map(5, nullObj);
		ass_eq(map.capacity(), 5);
		ass_eq(map.loadFactor(), 0.5f);
		ass_eq(map.size(), 0);
		ass_true(map.isEmpty());
		ass_eq(map.arrayLength(), 10);
		ass_eq(map.nullValue(), nullObj);

		map.insert("meow", TestObject(1.0f, 2.0f, 3.0f));
		ass_eq(map.size(), 1);
		ass_eq(map.capacity(), 5);
		ass_eq(map.arrayLength(), 10);		
		map.insert("moo", moo);
		map.insert("moo2", moo2);
		map.insert("meow0", meow0);
		map.insert("mooo", mooo);
		ass_eq(map.size(), 5);
		ass_eq(map.capacity(), 5);
		ass_eq(map.arrayLength(), 10);

		ass_true(map.contains("meow"));
		ass_true(map.contains("moo"));
		ass_true(map.contains("moo2"));
		ass_true(map.contains("meow0"));
		ass_true(map.contains("mooo"));

		ass_eq(map.at("meow"), meow);
		ass_eq(map.at("moo"), TestObject(2.0f, 4.0f, -6.0f));
		ass_eq(map.at("moo2"), moo2);
		ass_eq(map.at("meow0"), meow0);
		ass_eq(map.at("mooo"), mooo);

		ass_eq(map.capacity(), 5);
		ass_eq(map.loadFactor(), 0.5f);
		ass_eq(map.size(), 5);
		ass_false(map.isEmpty());
		ass_eq(map.arrayLength(), 10);
		ass_eq(map.nullValue(), nullObj);

		destroyed_count = 0;		
		map.clear();
		ass_eq(destroyed_count, 10);
		ass_false(map.contains("meow"));
		ass_false(map.contains("moo"));
		ass_false(map.contains("moo2"));
		ass_false(map.contains("meow0"));
		ass_false(map.contains("mooo"));

		ass_eq(map.at("meow"), nullObj);
		ass_eq(map.at("moo"), TestObject(-1.0f, -1.0f, -1.0f));
		ass_eq(map.at("moo2"), nullObj);
		ass_eq(map.at("meow0"), nullObj);
		ass_eq(map.at("mooo"), nullObj);
		
		ass_eq(map.capacity(), 5);
		ass_eq(map.loadFactor(), 0.5f);
		ass_eq(map.size(), 0);
		ass_true(map.isEmpty());
		ass_eq(map.arrayLength(), 10);
		ass_eq(map.nullValue(), nullObj);

		map.insert("testing", TestObject(-1.0f, 2.0f, -3.0f));
		ass_eq(map.capacity(), 5);
		ass_eq(map.loadFactor(), 0.5f);
		ass_eq(map.size(), 1);
		ass_false(map.isEmpty());
		ass_eq(map.arrayLength(), 10);
		ass_eq(map.nullValue(), nullObj);

		ass_true(map.contains("testing"));
		ass_eq(map.at("testing"), TestObject(-1.0f, 2.0f, -3.0f));
			
		FINISH_TEST;
	}

	void testStaticMapBasicEraseAll() {
		BEGIN_TEST;

		destroyed_count = 0;
		ass_eq(destroyed_count, 0);		
		
		TestObject* nullObj = NIL;		
		TestObject* moo = new TestObject(2.0f, 4.0f, -6.0f);
		TestObject* moo2 = new TestObject(0.0f, 0.0f, 0.0f);
		TestObject* meow0 = new TestObject(-42.0f, 34.23f, 0.0f);
		TestObject* mooo = new TestObject(-1.0f, -1.0f, -1.01f);
		
		
		StaticMap<TestObject*> map(5, nullObj);
		ass_eq(map.capacity(), 5);
		ass_eq(map.loadFactor(), 0.5f);
		ass_eq(map.size(), 0);
		ass_true(map.isEmpty());
		ass_eq(map.arrayLength(), 10);
		ass_eq(map.nullValue(), nullObj);

		map.insert("meow", new TestObject(1.0f, 2.0f, 3.0f));
		ass_eq(map.size(), 1);
		ass_eq(map.capacity(), 5);
		ass_eq(map.arrayLength(), 10);		
		map.insert("moo", moo);
		map.insert("moo2", moo2);
		map.insert("meow0", meow0);
		map.insert("mooo", mooo);
		ass_eq(map.size(), 5);
		ass_eq(map.capacity(), 5);
		ass_eq(map.arrayLength(), 10);

		ass_true(map.contains("meow"));
		ass_true(map.contains("moo"));
		ass_true(map.contains("moo2"));
		ass_true(map.contains("meow0"));
		ass_true(map.contains("mooo"));

		ass_true(map.at("meow")->test(1.0f, 2.0f, 3.0f));
		ass_eq(map.at("moo"), moo);
		ass_eq(map.at("moo2"), moo2);
		ass_eq(map.at("meow0"), meow0);
		ass_eq(map.at("mooo"), mooo);

		ass_eq(map.capacity(), 5);
		ass_eq(map.loadFactor(), 0.5f);
		ass_eq(map.size(), 5);
		ass_false(map.isEmpty());
		ass_eq(map.arrayLength(), 10);
		ass_eq(map.nullValue(), nullObj);

		ass_eq(destroyed_count, 0);		
		map.eraseAll();
		ass_eq(destroyed_count, 5);
		ass_false(map.contains("meow"));
		ass_false(map.contains("moo"));
		ass_false(map.contains("moo2"));
		ass_false(map.contains("meow0"));
		ass_false(map.contains("mooo"));

		ass_eq(map.at("meow"), nullObj);
		ass_eq(map.at("moo"), NIL);
		ass_eq(map.at("moo2"), nullObj);
		ass_eq(map.at("meow0"), nullObj);
		ass_eq(map.at("mooo"), nullObj);
		
		ass_eq(map.capacity(), 5);
		ass_eq(map.loadFactor(), 0.5f);
		ass_eq(map.size(), 0);
		ass_true(map.isEmpty());
		ass_eq(map.arrayLength(), 10);
		ass_eq(map.nullValue(), nullObj);

		map.insert("testing", new TestObject(-1.0f, 2.0f, -3.0f));
		ass_eq(map.capacity(), 5);
		ass_eq(map.loadFactor(), 0.5f);
		ass_eq(map.size(), 1);
		ass_false(map.isEmpty());
		ass_eq(map.arrayLength(), 10);
		ass_eq(map.nullValue(), nullObj);

		ass_true(map.contains("testing"));
		ass_true(map.at("testing")->test(-1.0f, 2.0f, -3.0f));

		ass_eq(destroyed_count, 5);		
		map.eraseAll();
		ass_eq(destroyed_count, 6);
		ass_eq(map.capacity(), 5);
		ass_eq(map.loadFactor(), 0.5f);
		ass_eq(map.size(), 0);
		ass_true(map.isEmpty());
		ass_eq(map.arrayLength(), 10);
		ass_eq(map.nullValue(), nullObj);
		
		FINISH_TEST;
	}





}

int main(int argc, char** argv) {
	cc::testStaticMapBasicCreateAndDestroy();
	cc::testStaticMapCopy();	
	cc::testStaticMapBasicObjectInsert();
	cc::testStaticMapBasicIntegerInsert();	
	cc::testStaticMapBasicAutoIncreaseCapacity();
	cc::testStaticMapBasicClear();
	cc::testStaticMapBasicEraseAll();
	return 0;
}




