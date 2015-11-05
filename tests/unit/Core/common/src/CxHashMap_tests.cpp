#include "core/common/CxHashMap.h"
#include "core/CxTestCore.h"

#define ass_cap_size(in_map,in_cap,in_size)		\
	ass_eq(in_map.capacity(), in_cap);				\
	ass_eq(in_map.size(), in_size);					\
	ass_eq(in_map.count(), in_size)
	
#define ass_hm_eq(in_hm0,in_hm1)								\
	ass_true(in_hm0 == in_hm0 && in_hm1 == in_hm1);		\
	ass_false(in_hm0 != in_hm0 || in_hm1 != in_hm1);	\
	ass_true(in_hm0 == in_hm1 && in_hm1 == in_hm0);		\
	ass_false(in_hm0 != in_hm1 || in_hm1 != in_hm0)

#define ass_hm_neq(in_hm0,in_hm1)								\
	ass_true(in_hm0 == in_hm0 && in_hm1 == in_hm1);		\
	ass_false(in_hm0 != in_hm0 || in_hm1 != in_hm1);	\
	ass_false(in_hm0 == in_hm1 || in_hm1 == in_hm0);	\
	ass_true(in_hm0 != in_hm1 && in_hm1 != in_hm0)

namespace cat {

	class Cx_Obj {
	  public:
		CxF32 x,y,z;
		Cx_Obj(CxF32 in_x=0, CxF32 in_y=0, CxF32 in_z=0) : x(in_x), y(in_y), z(in_z) {}
		CxBool operator==(const Cx_Obj &in_o) const {
			return (x == in_o.x && y == in_o.y && z == in_o.z);
		}
		CxBool operator!=(const Cx_Obj &in_o) const {
			return (x != in_o.x || y != in_o.y || z != in_o.z);
		}
	};

	std::ostream & operator<<(std::ostream &inout_out, const Cx_Obj &in_obj) {
		return std::cout << "(" << in_obj.x << ", " << in_obj.y << ", " << in_obj.z << ")";
	}

   CxU32 CxHash(const Cx_Obj &in_obj) {
		return (CxU32)(((int)in_obj.x) | (((int)in_obj.y*3) << 8) | (((int)in_obj.z*5) << 16));
	}
	
	/**
	 * @tests CxHashMap()
	 * @tests CxHashMap(CxI32, CxF32)
	 * @tests CxHashMap(const CxHashMap<K,V> &)
	 * @tests CxHashMap(CxHashMap<K,V> &&)
	 * @tests ~CxHashMap()
	 * @tests size() const
	 * @tests count() const
	 * @tests capacity() const
	 */
	void testCxHashMapCreateAndDestroy() {
		BEGIN_TEST;

		/* Test simple types as keys */
		CxHashMap<CxI32, CxF32> h0;
		ass_cap_size(h0, 0, 0);
		CxHashMap<CxI32, CxF32> h1(10);
		ass_cap_size(h1, 13, 0);
		
		CxHashMap<CxI32, CxF32> h2(h1);
		ass_cap_size(h2, 13, 0);
		ass_cap_size(h1, 13, 0);

		CxHashMap<CxI32, CxF32> h3(CxHashMap<CxI32, CxF32>(10));
		ass_cap_size(h3, 13, 0);

		/* Test Object types as keys */
		CxHashMap<Cx_Obj, CxF32> h4;
		ass_cap_size(h4, 0, 0);
		CxHashMap<Cx_Obj, CxF32> h5(10);
		ass_cap_size(h5, 13, 0);
		
		CxHashMap<Cx_Obj, CxF32> h6(h5);
		ass_cap_size(h6, 13, 0);
		ass_cap_size(h5, 13, 0);

		CxHashMap<Cx_Obj, CxF32> h7(CxHashMap<Cx_Obj, CxF32>(10));
		ass_cap_size(h7, 13, 0);

		/* Test strings as keys */
		CxHashMap<CxChar *, Cx_Obj> h8;
		ass_cap_size(h8, 0, 0);
		CxHashMap<CxChar *, CxF32> h9(10);
		ass_cap_size(h9, 13, 0);
		
		CxHashMap<CxChar *, CxF32> h10(h9);
		ass_cap_size(h10, 13, 0);
		ass_cap_size(h9, 13, 0);

		CxHashMap<CxChar *, Cx_Obj> h11(CxHashMap<CxChar *, Cx_Obj>(10));
		ass_cap_size(h11, 13, 0);

		/* Test bad maps */
		CxHashMap<CxI32, Cx_Obj> h12(0);

		/* Test destructor */
		CxHashMap<CxI32, Cx_Obj> *h13 = new CxHashMap<CxI32, Cx_Obj>();
		ass_true(h13->capacity() == 0 && h13->size() == 0);
		delete h13;

		h13 = new CxHashMap<CxI32, Cx_Obj>(10);
		ass_true(h13->capacity() == 13 && h13->size() == 0);
		delete h13;

		CxHashMap<Cx_Obj, Cx_Obj> *h14 = new CxHashMap<Cx_Obj, Cx_Obj>(10);
		ass_true(h14->capacity() == 13 && h14->size() == 0);
		delete h14;

		CxHashMap<CxChar *, Cx_Obj> *h15 = new CxHashMap<CxChar *, Cx_Obj>(10);
		ass_true(h15->capacity() == 13 && h15->size() == 0);
		delete h15;
	  
		FINISH_TEST;
	}

	/**
	 * @tests CxHashMap(const CxHashMap<K,V> &)
	 * @tests CxHashMap(CxHashMap<K,V> &&)
	 * @tests operator=(const CxHashMap<K,V> &)
	 * @tests operator=(CxHashMap<K,V> &&)
	 * @tests swap(CxHashMap<K,V> &)
	 */
	void testCxHashMapCopyAndAssign() {
		BEGIN_TEST;
		
		/* ################ Test with simply key,value types ############ */
		CxHashMap<CxI32, CxF32> h0(10);
		CxHashMap<CxI32, CxF32> h1(14);
		ass_true(h0.capacity() < h1.capacity());

		h0.swap(h1);
		ass_true(h0.capacity() > h1.capacity());

		h0.insert(3, 99);
		h0.insert(82, -98);
		h0.insert(-293, 9);
		h0.insert(21, 1);
		h0.insert(0, 2);

		ass_eq(h0.size(), 5);
		ass_eq(h1.size(), 0);
		h0.swap(h1);
		ass_true(h0.capacity() < h1.capacity());
		ass_eq(h0.size(), 0);
		ass_eq(h1.size(), 5);
		ass_eq(h1.value(82, 0), -98);
		ass_eq(h0.value(82, 0), 0);

		/* Test copy assignment */
		h0 = h1;
		ass_eq(h0.size(), 5);  ass_eq(h1.size(), 5);
		ass_eq(h1.value(82, 0), -98);  ass_eq(h0.value(82, 0), -98);
		h0[-1] = -100;
		ass_eq(h0.size(), 6);  ass_eq(h1.size(), 5);
		ass_eq(h1.value(-1, 0), 0);  ass_eq(h0.value(-1, 0), -100);
		h1[-1] = 100;
		ass_eq(h0.size(), 6);  ass_eq(h1.size(), 6);
		ass_eq(h1.value(-1, 0), 100);  ass_eq(h0.value(-1, 0), -100);

		/* Test move assignment */
		h1.insert(-2, 22);
		h0 = static_cast<CxHashMap<CxI32, CxF32> &&>(h1);
		ass_cap_size(h1, 0, 0);
		ass_eq(h0.size(), 7);
		ass_eq(h1.value(-2, 0), 0);
		ass_eq(h0.value(-2, 0), 22);

		/* Test copy constructor */
		CxHashMap<CxI32, CxF32> h2(h0);
		ass_true(h0.size() == 7 && h2.size() == 7);
		ass_eq(h2.value(-2, 0), 22);
		ass_eq(h0.value(-2, 0), 22);
		ass_true(h2 == h0 && h0 == h2);
		h2.insert(69, 42);
		ass_true(h2.size() == 8 && h0.size() == 7);
		ass_eq(h2.value(69, 0), 42);
		ass_eq(h0.value(69, 0), 0);
		
		/* Test Move constructor */
		CxHashMap<CxI32, CxF32> h3(static_cast<CxHashMap<CxI32, CxF32> &&>(h2));
		ass_cap_size(h2, 0, 0);
		ass_eq(h3.value(69, 0), 42);
		ass_eq(h2.value(69, 0), 0);




		/* ################ Test with Object key,value types ############ */
		CxHashMap<Cx_Obj, Cx_Obj> h4(10);
		CxHashMap<Cx_Obj, Cx_Obj> h5(14);
		ass_true(h4.capacity() < h5.capacity());

		h4.swap(h5);
		ass_true(h4.capacity() > h5.capacity());

		h4.insert(Cx_Obj(3,2,1), Cx_Obj(9,9,1));
		h4.insert(Cx_Obj(8,2,1), Cx_Obj(2,3,1));
		h4.insert(Cx_Obj(2,3,9), Cx_Obj(9,9,0));

		ass_eq(h4.size(), 3);
		ass_eq(h5.size(), 0);
		h4.swap(h5);
		ass_true(h4.capacity() < h5.capacity());
		ass_eq(h4.size(), 0);
		ass_eq(h5.size(), 3);
		ass_eq(h5.value(Cx_Obj(8,2,1), Cx_Obj(0,0,0)), Cx_Obj(2,3,1));
		ass_eq(h4.value(Cx_Obj(8,2,1), Cx_Obj(0,0,0)), Cx_Obj(0,0,0));

		/* Test copy assignment */
		h4 = h5;
		ass_eq(h4.size(), 3);  ass_eq(h5.size(), 3);
		ass_eq(h5.value(Cx_Obj(8,2,1), Cx_Obj()), Cx_Obj(2,3,1));
		ass_eq(h4.value(Cx_Obj(8,2,1), Cx_Obj()), Cx_Obj(2,3,1));
		h4[Cx_Obj(1,2,3)] = Cx_Obj(-1,0,0);
		ass_eq(h4.size(), 4);  ass_eq(h5.size(), 3);
		ass_eq(h5.value(Cx_Obj(1,2,3), Cx_Obj()), Cx_Obj());
		ass_eq(h4.value(Cx_Obj(1,2,3), Cx_Obj()), Cx_Obj(-1,0,0));
		h5[Cx_Obj(1,2,3)] = Cx_Obj(1,0,0);
		ass_eq(h4.size(), 4);  ass_eq(h5.size(), 4);
		ass_eq(h5.value(Cx_Obj(1,2,3), Cx_Obj()), Cx_Obj(1,0,0));
		ass_eq(h4.value(Cx_Obj(1,2,3), Cx_Obj()), Cx_Obj(-1,0,0));

		/* Test move assignment */
		h5.insert(Cx_Obj(-2,1,3), Cx_Obj(2,2,0));
		h4 = static_cast<CxHashMap<Cx_Obj, Cx_Obj> &&>(h5);
		ass_cap_size(h5, 0, 0);
		ass_eq(h4.size(), 5);
		ass_eq(h5.value(Cx_Obj(-2,1,3), Cx_Obj()), Cx_Obj(0));
		ass_eq(h4.value(Cx_Obj(-2,1,3), Cx_Obj()), Cx_Obj(2,2,0));

		/* Test copy constructor */
		CxHashMap<Cx_Obj, Cx_Obj> h6(h4);
		ass_true(h4.size() == 5 && h6.size() == 5);
		ass_eq(h6.value(Cx_Obj(-2,1,3), Cx_Obj()), Cx_Obj(2,2,0));
		ass_eq(h4.value(Cx_Obj(-2,1,3), Cx_Obj()), Cx_Obj(2,2,0));
		ass_true(h6 == h4 && h4 == h6);
		h6.insert(Cx_Obj(4,6,9), Cx_Obj(4,2,0));
		ass_true(h6.size() == 6 && h4.size() == 5);
		ass_eq(h6.value(Cx_Obj(4,6,9), Cx_Obj()), Cx_Obj(4,2,0));
		ass_eq(h4.value(Cx_Obj(4,6,9), Cx_Obj()), Cx_Obj());
		
		/* Test Move constructor */
		CxHashMap<Cx_Obj, Cx_Obj> h7(static_cast<CxHashMap<Cx_Obj, Cx_Obj> &&>(h6));
		ass_cap_size(h6, 0, 0);
		ass_eq(h7.value(Cx_Obj(4,6,9), Cx_Obj()), Cx_Obj(4,2,0));
		ass_eq(h6.value(Cx_Obj(4,6,9), Cx_Obj()), Cx_Obj());


		/* ################ Test with string key types ############ */
		CxHashMap<CxChar *, Cx_Obj> h8(10);
		CxHashMap<CxChar *, Cx_Obj> h9(14);
		ass_true(h8.capacity() < h9.capacity());

		h8.swap(h9);
		ass_true(h8.capacity() > h9.capacity());

		h8.insert("Moo", Cx_Obj(9,9,1));
		h8.insert("Meow", Cx_Obj(2,3,1));
		h8.insert("Woof", Cx_Obj(9,9,0));

		ass_eq(h8.size(), 3);
		ass_eq(h9.size(), 0);
		h8.swap(h9);
		ass_true(h8.capacity() < h9.capacity());
		ass_eq(h8.size(), 0);
		ass_eq(h9.size(), 3);
		ass_eq(h9.value("Meow", Cx_Obj(0,0,0)), Cx_Obj(2,3,1));
		ass_eq(h8.value("Meow", Cx_Obj(0,0,0)), Cx_Obj(0,0,0));

		/* Test copy assignment */
		h8 = h9;
		ass_eq(h8.size(), 3);  ass_eq(h9.size(), 3);
		ass_eq(h9.value("Meow", Cx_Obj()), Cx_Obj(2,3,1));
		ass_eq(h8.value("Meow", Cx_Obj()), Cx_Obj(2,3,1));
		h8["Squeak"] = Cx_Obj(-1,0,0);
		ass_eq(h8.size(), 4);  ass_eq(h9.size(), 3);
		ass_eq(h9.value("Squeak", Cx_Obj()), Cx_Obj());
		ass_eq(h8.value("Squeak", Cx_Obj()), Cx_Obj(-1,0,0));
		h9["Squeak"] = Cx_Obj(1,0,0);
		ass_eq(h8.size(), 4);  ass_eq(h9.size(), 4);
		ass_eq(h9.value("Squeak", Cx_Obj()), Cx_Obj(1,0,0));
		ass_eq(h8.value("Squeak", Cx_Obj()), Cx_Obj(-1,0,0));

		/* Test move assignment */
		h9.insert("Bark", Cx_Obj(2,2,0));
		h8 = static_cast<CxHashMap<CxChar *, Cx_Obj> &&>(h9);
		ass_cap_size(h9, 0, 0);
		ass_eq(h8.size(), 5);
		ass_eq(h9.value("Bark", Cx_Obj()), Cx_Obj(0));
		ass_eq(h8.value("Bark", Cx_Obj()), Cx_Obj(2,2,0));

		/* Test copy constructor */
		CxHashMap<CxChar *, Cx_Obj> h10(h8);
		ass_true(h8.size() == 5 && h10.size() == 5);
		ass_eq(h10.value("Bark", Cx_Obj()), Cx_Obj(2,2,0));
		ass_eq(h8.value("Bark", Cx_Obj()), Cx_Obj(2,2,0));
		ass_true(h10 == h8 && h8 == h10);
		h10.insert("Nay", Cx_Obj(4,2,0));
		ass_true(h10.size() == 6 && h8.size() == 5);
		ass_eq(h10.value("Nay", Cx_Obj()), Cx_Obj(4,2,0));
		ass_eq(h8.value("Nay", Cx_Obj()), Cx_Obj());
		
		/* Test Move constructor */
		CxHashMap<CxChar *, Cx_Obj> h11(static_cast<CxHashMap<CxChar *, Cx_Obj> &&>(h10));
		ass_cap_size(h10, 0, 0);
		ass_eq(h11.value("Nay", Cx_Obj()), Cx_Obj(4,2,0));
		ass_eq(h10.value("Nay", Cx_Obj()), Cx_Obj());
		
		
		FINISH_TEST;
	}

	/**
	 * @tests insert(const K &, const V &)
	 * @tests insert(const K &, V &&)
	 * @tests value(const K &) const
	 * @tests value(const K &, const V &) const
	 * @tests operator[](const K &)
	 * @tests operator[](const K &) const
	 * @tests defaultValue() const
	 * @tests size() const
	 * @tests count() const
	 * @tests capacity() const
	 */
	void testCxHashMapInsertAndAccess() {
		BEGIN_TEST;

		/* ########### Test simple key type ############# */
		CxHashMap<CxI32, CxF32> h0(10);
		ass_cap_size(h0, 13, 0);
		ass_eq(h0.value(3, 0), 0);
		ass_eq(h0.value(0, 0), 0);

		h0.insert(3, 99);  ass_cap_size(h0, 13, 1);
		ass_eq(h0.value(3), 99);
		h0.insert(82, -98);  ass_cap_size(h0, 13, 2);
		h0.insert(-293, 9);  ass_cap_size(h0, 13, 3);
		h0.insert(21, 1);  ass_cap_size(h0, 13, 4);
		h0.insert(0, 2);  ass_cap_size(h0, 13, 5);

		ass_eq(h0.value(3), 99);
		ass_eq(h0.value(82), -98);
		ass_eq(h0.value(-293), 9);
		ass_eq(h0.value(21), 1);
		ass_eq(h0.value(0), 2);

		ass_eq(h0.value(0, 0), 2);
		ass_eq(h0.value(-21), h0.defaultValue());
		ass_eq(h0.value(-21, 22), 22);

		ass_eq(h0[-21], h0.defaultValue());
		ass_neq(h0.value(-21, 22), 22);
		ass_eq(h0.size(), 6);
		h0[-21] = 22;
		ass_eq(h0.value(-21), 22);
		ass_eq(h0[-21], 22);
		ass_eq(h0.size(), 6);

		h0[99] = 22;
		ass_eq(h0.value(99, 0), 22);
		ass_eq(h0.size(), 7);

		const CxHashMap<CxI32, CxF32> hc0 = h0;
		ass_eq(h0.size(), 7);
		ass_eq(hc0.size(), 7);
		ass_eq(hc0[99], 22);
		ass_eq(hc0[3], 99);
		ass_eq(hc0[4], hc0.defaultValue());
		ass_eq(hc0[-293], 9);
		ass_eq(hc0.size(), 7);

		CxHashMap<CxI32, CxF32> hz0;
		ass_cap_size(hz0, 0, 0);
		hz0.insert(32, 1.5);
		ass_eq(hz0.size(), 1);
		ass_true(hz0.capacity() >= 2);
		ass_eq(hz0.value(32, 0), 1.5);
	   hz0.free();
		ass_cap_size(hz0, 0, 0);
		ass_eq(hz0.value(32, 0), 0);
		hz0[22] = 93;
		ass_eq(hz0.size(), 1);
		ass_true(hz0.capacity() >= 2);
		ass_eq(hz0.value(22, 0), 93);
		
		/* ########### Test object key type ############# */
		CxHashMap<Cx_Obj, Cx_Obj> h1(10);
		ass_cap_size(h1, 13, 0);
		ass_eq(h1.value(Cx_Obj(1, 2,3 ), Cx_Obj()), Cx_Obj());
		ass_eq(h1.value(Cx_Obj(-1,-1,-1), Cx_Obj(-1,-1,-1)), Cx_Obj(-1,-1,-1));

		h1.insert(Cx_Obj(1,2,3), Cx_Obj(99, 99, 0));  ass_cap_size(h1, 13, 1);
		ass_eq(h1.value(Cx_Obj(1,2,3)), Cx_Obj(99, 99, 0));
		h1.insert(Cx_Obj(3,4,5), Cx_Obj(-9,-8,1));  ass_cap_size(h1, 13, 2);
		h1.insert(Cx_Obj(-123,4,-5), Cx_Obj(9, 10, 100));  ass_cap_size(h1, 13, 3);
		h1.insert(Cx_Obj(2,2,2), Cx_Obj(1,1,1));  ass_cap_size(h1, 13, 4);
		h1.insert(Cx_Obj(), Cx_Obj(2,2,2));  ass_cap_size(h1, 13, 5);

		ass_eq(h1.value(Cx_Obj(1,2,3)), Cx_Obj(99,99,0));
		ass_eq(h1.value(Cx_Obj(3,4,5)), Cx_Obj(-9,-8,1));
		ass_eq(h1.value(Cx_Obj(-123,4,-5)), Cx_Obj(9,10,100));
		ass_eq(h1.value(Cx_Obj(2,2,2)), Cx_Obj(1,1,1));
		ass_eq(h1.value(Cx_Obj()), Cx_Obj(2,2,2));

		ass_eq(h1.value(Cx_Obj(), Cx_Obj()), Cx_Obj(2,2,2));
		ass_eq(h1.value(Cx_Obj(-2,-1,1)), h1.defaultValue());
		ass_eq(h1.value(Cx_Obj(-2,-1,1), Cx_Obj(2,2,2)), Cx_Obj(2,2,2));

		ass_eq(h1[Cx_Obj(-2,-1,1)], h1.defaultValue());
		ass_eq(h1.value(Cx_Obj(-2,-1,1), Cx_Obj(2,2,2)), Cx_Obj());
		ass_eq(h1.size(), 6);
		h1[Cx_Obj(-2,-1,1)] = Cx_Obj(2,2,2);
		ass_eq(h1.value(Cx_Obj(-2,-1,1)), Cx_Obj(2,2,2));
		ass_eq(h1[Cx_Obj(-2,-1,1)], Cx_Obj(2,2,2));
		ass_eq(h1.size(), 6);

		h1[Cx_Obj(9,9,9)] = Cx_Obj(2,2,2);
		ass_eq(h1.value(Cx_Obj(9,9,9), Cx_Obj()), Cx_Obj(2,2,2));
		ass_eq(h1.size(), 7);

		const CxHashMap<Cx_Obj, Cx_Obj> hc1 = h1;
		ass_eq(h1.size(), 7);
		ass_eq(hc1.size(), 7);
		ass_eq(hc1[Cx_Obj(9,9,9)], Cx_Obj(2,2,2));
		ass_eq(hc1[Cx_Obj(1,2,3)], Cx_Obj(99,99,0));
		ass_eq(hc1[Cx_Obj(4,4,-4)], hc1.defaultValue());
		ass_eq(hc1[Cx_Obj(3,4,5)], Cx_Obj(-9,-8,1));
		ass_eq(hc1.size(), 7);
		
		CxHashMap<Cx_Obj, Cx_Obj> hz1;
		ass_cap_size(hz1, 0, 0);
		hz1.insert(Cx_Obj(1,2,3), Cx_Obj(-1,0,2));
		ass_eq(hz1.size(), 1);
		ass_true(hz1.capacity() >= 2);
		ass_eq(hz1.value(Cx_Obj(1,2,3), Cx_Obj(0,0,0)), Cx_Obj(-1,0,2));
	   hz1.free();
		ass_cap_size(hz1, 0, 0);
		ass_eq(hz1.value(Cx_Obj(1,2,3), Cx_Obj(0,0,0)), Cx_Obj(0,0,0));
		hz1[Cx_Obj(-1,2,2)] = Cx_Obj(9,3,44);
		ass_eq(hz1.size(), 1);
		ass_true(hz1.capacity() >= 2);
		ass_eq(hz1.value(Cx_Obj(-1,2,2), Cx_Obj(0,0,0)), Cx_Obj(9,3,44));

		/* ########### Test string key type ############# */
		CxHashMap<CxChar *, Cx_Obj> h2(10);
		ass_cap_size(h2, 13, 0);
		ass_eq(h2.value("Meow", Cx_Obj()), Cx_Obj());
		ass_eq(h2.value("Moo", Cx_Obj(-1,-1,-1)), Cx_Obj(-1,-1,-1));

		h2.insert("Meow99", Cx_Obj(99, 99, 0));  ass_cap_size(h2, 13, 1);
		ass_eq(h2.value("Meow99"), Cx_Obj(99, 99, 0));
		h2.insert("Meow-9", Cx_Obj(-9,-8,1));  ass_cap_size(h2, 13, 2);
		h2.insert("Meow100", Cx_Obj(9, 10, 100));  ass_cap_size(h2, 13, 3);
		h2.insert("Meow1", Cx_Obj(1,1,1));  ass_cap_size(h2, 13, 4);
		h2.insert("Meow2", Cx_Obj(2,2,2));  ass_cap_size(h2, 13, 5);

		ass_eq(h2.value("Meow99"), Cx_Obj(99,99,0));
		ass_eq(h2.value("Meow-9"), Cx_Obj(-9,-8,1));
		ass_eq(h2.value("Meow100"), Cx_Obj(9,10,100));
		ass_eq(h2.value("Meow1"), Cx_Obj(1,1,1));
		ass_eq(h2.value("Meow2"), Cx_Obj(2,2,2));
		ass_eq(h2.value(CxHash("Meow2")), Cx_Obj(2,2,2));
		ass_eq(h2.value(CxHash("Meow99"), Cx_Obj()), Cx_Obj(99,99,0));

		ass_eq(h2.value("Meow2", Cx_Obj()), Cx_Obj(2,2,2));
		ass_eq(h2.value("Moo"), h2.defaultValue());
		ass_eq(h2.value("Moo", Cx_Obj(2,2,2)), Cx_Obj(2,2,2));

		ass_eq(h2["Moo-1"], h2.defaultValue());
		ass_eq(h2.value("Moo-1", Cx_Obj(2,2,2)), Cx_Obj());
		ass_eq(h2.size(), 6);
		h2["Moo-1"] = Cx_Obj(2,2,2);
		ass_eq(h2.value("Moo-1"), Cx_Obj(2,2,2));
		ass_eq(h2["Moo-1"], Cx_Obj(2,2,2));
		ass_eq(h2.size(), 6);

		h2["Moo2"] = Cx_Obj(2,2,2);
		ass_eq(h2.value("Moo2", Cx_Obj()), Cx_Obj(2,2,2));
		ass_eq(h2.size(), 7);

		const CxHashMap<CxChar *, Cx_Obj> hc2 = h2;
		ass_eq(h2.size(), 7);
		ass_eq(hc2.size(), 7);
		ass_eq(hc2["Moo2"], Cx_Obj(2,2,2));
		ass_eq(hc2["Meow99"], Cx_Obj(99,99,0));
		ass_eq(hc2["MooNone"], hc2.defaultValue());
		ass_eq(hc2["Meow-9"], Cx_Obj(-9,-8,1));
		ass_eq(hc2[CxHash("Meow-9")], Cx_Obj(-9,-8,1));
		ass_eq(hc2.size(), 7);

		CxHashMap<CxChar *, Cx_Obj> hz2;
		ass_cap_size(hz2, 0, 0);
		hz2.insert("Test1", Cx_Obj(-1,0,2));
		ass_eq(hz2.size(), 1);
		ass_true(hz2.capacity() >= 2);
		ass_eq(hz2.value("Test1", Cx_Obj(0,0,0)), Cx_Obj(-1,0,2));
	   hz2.free();
		ass_cap_size(hz2, 0, 0);
		ass_eq(hz2.value("Test1", Cx_Obj(0,0,0)), Cx_Obj(0,0,0));
		hz2["Test2"] = Cx_Obj(9,3,44);
		ass_eq(hz2.size(), 1);
		ass_true(hz2.capacity() >= 2);
		ass_eq(hz2.value("Test2", Cx_Obj(0,0,0)), Cx_Obj(9,3,44));

		FINISH_TEST;
	}

	/**
	 * @tests operator==(const CxHashMap<K,V> &)
	 * @tests operator!=(const CxHashMap<K,V> &)
	 */
	void testCxHashMapEquality() {
		BEGIN_TEST;

		/* ############ Simple key,value pairs ################ */
		CxHashMap<CxI32, CxF32> h0(15);
		CxHashMap<CxI32, CxF32> h1;
	   ass_hm_eq(h0,h1);

		h0[1] = -1;  ass_hm_neq(h0,h1);
		h1[1] = 1;  ass_hm_neq(h0,h1);

		h1[1] = -1;  ass_hm_eq(h0,h1);

		h0[1] = 1;  ass_hm_neq(h0,h1);
		h1[1] = 1;  ass_hm_eq(h0,h1);

		h0[2] = 2;  ass_hm_neq(h0,h1);
		h1[2] = -2;  ass_hm_neq(h0,h1);
		h1[2] = 2;  ass_hm_eq(h0,h1);

		h0[9] = 3;
		h0[22] = -12;
		ass_hm_neq(h0,h1);
		h1[8] = 3;
		h1[22] = -12;
		ass_hm_neq(h0,h1);

		h0[8] = 3;  ass_hm_neq(h0,h1);
		h1[9] = -3;  ass_hm_neq(h0,h1);
		h1[9] = 3;  ass_hm_eq(h0,h1);
		


		/* ############ Object key,value pairs ################ */
		CxHashMap<Cx_Obj, Cx_Obj> h2(15);
		CxHashMap<Cx_Obj, Cx_Obj> h3;
	   ass_hm_eq(h2,h3);

		h2[Cx_Obj(1)] = Cx_Obj(-1);  ass_hm_neq(h2,h3);
		h3[Cx_Obj(1)] = Cx_Obj(1);  ass_hm_neq(h2,h3);

		h3[Cx_Obj(1)] = Cx_Obj(-1);  ass_hm_eq(h2,h3);

		h2[Cx_Obj(1)] = Cx_Obj(1);  ass_hm_neq(h2,h3);
		h3[Cx_Obj(1)] = Cx_Obj(1);  ass_hm_eq(h2,h3);

		h2[Cx_Obj(2)] = Cx_Obj(2);  ass_hm_neq(h2,h3);
		h3[Cx_Obj(2)] = Cx_Obj(-2);  ass_hm_neq(h2,h3);
		h3[Cx_Obj(2)] = Cx_Obj(2);  ass_hm_eq(h2,h3);

		h2[Cx_Obj(9)] = Cx_Obj(3);
		h2[Cx_Obj(22)] = Cx_Obj(-12);
		ass_hm_neq(h2,h3);
		h3[Cx_Obj(8)] = Cx_Obj(3);
		h3[Cx_Obj(22)] = Cx_Obj(-12);
		ass_hm_neq(h2,h3);

		h2[Cx_Obj(8)] = Cx_Obj(3);  ass_hm_neq(h2,h3);
		h3[Cx_Obj(9)] = Cx_Obj(-3);  ass_hm_neq(h2,h3);
		h3[Cx_Obj(9)] = Cx_Obj(3);  ass_hm_eq(h2,h3);



		/* ############ String keys ################ */
		CxHashMap<CxChar *, Cx_Obj> h4(15);
		CxHashMap<CxChar *, Cx_Obj> h5;
	   ass_hm_eq(h4,h5);

		h4["Meow"] = Cx_Obj(-1);  ass_hm_neq(h4,h5);
		h5["Meow"] = Cx_Obj(1);  ass_hm_neq(h4,h5);

		h5["Meow"] = Cx_Obj(-1);  ass_hm_eq(h4,h5);

		h4["Meow"] = Cx_Obj(1);  ass_hm_neq(h4,h5);
		h5["Meow"] = Cx_Obj(1);  ass_hm_eq(h4,h5);

		h4["Moo"] = Cx_Obj(2);  ass_hm_neq(h4,h5);
		h5["Moo"] = Cx_Obj(-2);  ass_hm_neq(h4,h5);
		h5["Moo"] = Cx_Obj(2);  ass_hm_eq(h4,h5);

		h4["Woof"] = Cx_Obj(3);
		h4["Bark"] = Cx_Obj(-12);
		ass_hm_neq(h4,h5);
		h5["Nay"] = Cx_Obj(3);
		h5["Bark"] = Cx_Obj(-12);
		ass_hm_neq(h4,h5);

		h4["Nay"] = Cx_Obj(3);  ass_hm_neq(h4,h5);
		h5["Woof"] = Cx_Obj(-3);  ass_hm_neq(h4,h5);
		h5["Woof"] = Cx_Obj(3);  ass_hm_eq(h4,h5);
		FINISH_TEST;
	}

	/** 
	 * @tests contains(const K &)
	 * @tests count(const K &)
	 * @tests find(const K &)
	 * @tests end()
	 * @tests begin()
	 */
	void testCxHashMapContainsCountFind() {
		BEGIN_TEST;

		/* ############# Simple key/value pairs ############ */
		CxHashMap<CxI32, CxF32> h0;
		ass_false(h0.contains(12));
		ass_zero(h0.count(12));
		ass_true(h0.find(12) == h0.end());

		h0[12] = 99;
		ass_true(h0.contains(12));
		ass_eq(h0.count(12), 1);
		ass_true(h0.find(12) != h0.end());
		ass_true(h0.find(12) == h0.begin());
		ass_true(h0.find(12).value() == 99);
		ass_false(h0.contains(11));
		ass_zero(h0.count(11));
		ass_true(h0.find(11) == h0.end());

		h0[11] = -22;
		ass_true(h0.contains(11) && h0.contains(12));
		ass_eq(h0.count(11), 1);
		ass_eq(h0.count(12), 1);
		ass_true(h0.find(12) != h0.end());
		ass_true(h0.find(12) == h0.begin() || h0.find(11) == h0.begin());
		ass_false(h0.find(12) == h0.begin() && h0.find(11) == h0.begin());
		ass_true(h0.find(12).value() == 99);
		ass_true(h0.find(11).value() == -22);
		ass_false(h0.contains(0));
		ass_zero(h0.count(0));
		ass_true(h0.find(0) == h0.end());



		/* ############# Object key/value pairs ############ */
		CxHashMap<Cx_Obj, Cx_Obj> h1;
		ass_false(h1.contains(Cx_Obj(1,2,0)));
		ass_zero(h1.count(Cx_Obj(1,2,0)));
		ass_true(h1.find(Cx_Obj(1,2,0)) == h1.end());

		h1[Cx_Obj(1,2,0)] = Cx_Obj(9,9);
		ass_true(h1.contains(Cx_Obj(1,2,0)));
		ass_eq(h1.count(Cx_Obj(1,2,0)), 1);
		ass_true(h1.find(Cx_Obj(1,2,0)) != h1.end());
		ass_true(h1.find(Cx_Obj(1,2,0)) == h1.begin());
		ass_true(h1.find(Cx_Obj(1,2,0)).value() == Cx_Obj(9,9));
		ass_false(h1.contains(Cx_Obj(1,1,0)));
		ass_zero(h1.count(Cx_Obj(1,1,0)));
		ass_true(h1.find(Cx_Obj(1,1,0)) == h1.end());

		h1[Cx_Obj(1,1,0)] = Cx_Obj(-2,-2,0);
		ass_true(h1.contains(Cx_Obj(1,1,0)) && h1.contains(Cx_Obj(1,2,0)));
		ass_eq(h1.count(Cx_Obj(1,1,0)), 1);
		ass_eq(h1.count(Cx_Obj(1,2,0)), 1);
		ass_true(h1.find(Cx_Obj(1,2,0)) != h1.end());
		ass_true(h1.find(Cx_Obj(1,2,0)) == h1.begin() || h1.find(Cx_Obj(1,1,0)) == h1.begin());
		ass_false(h1.find(Cx_Obj(1,2,0)) == h1.begin() && h1.find(Cx_Obj(1,1,0)) == h1.begin());
		ass_true(h1.find(Cx_Obj(1,2,0)).value() == Cx_Obj(9,9));
		ass_true(h1.find(Cx_Obj(1,1,0)).value() == Cx_Obj(-2,-2,0));
		ass_false(h1.contains(Cx_Obj()));
		ass_zero(h1.count(Cx_Obj()));
		ass_true(h1.find(Cx_Obj()) == h1.end());



		/* ############# string keys ############ */
		CxHashMap<CxChar *, Cx_Obj> h2;
		ass_false(h2.contains("Meow"));
		ass_zero(h2.count("Meow"));
		ass_true(h2.find("Meow") == h2.end());

		h2["Meow"] = Cx_Obj(9,9);
		ass_true(h2.contains("Meow"));
		ass_eq(h2.count("Meow"), 1);
		ass_true(h2.find("Meow") != h2.end());
		ass_true(h2.find("Meow") == h2.begin());
		ass_true(h2.find("Meow").value() == Cx_Obj(9,9));
		ass_false(h2.contains("Moo"));
		ass_zero(h2.count("Moo"));
		ass_true(h2.find("Moo") == h2.end());

		h2["Moo"] = Cx_Obj(-2,-2,0);
		ass_true(h2.contains("Moo") && h2.contains("Meow"));
		ass_eq(h2.count("Moo"), 1);
		ass_eq(h2.count("Meow"), 1);
		ass_true(h2.find("Meow") != h2.end());
		ass_true(h2.find("Meow") == h2.begin() || h2.find("Moo") == h2.begin());
		ass_false(h2.find("Meow") == h2.begin() && h2.find("Moo") == h2.begin());
		ass_true(h2.find("Meow").value() == Cx_Obj(9,9));
		ass_true(h2.find("Moo").value() == Cx_Obj(-2,-2,0));
		ass_false(h2.contains("Woof"));
		ass_zero(h2.count("Woof"));
		ass_true(h2.find("Woof") == h2.end());
		
		FINISH_TEST;
	}

	/**
	 * @tests free()
	 * @tests clear()
	 */
	void testCxHashMapClearFree() {
		BEGIN_TEST;

		/* ############### Simple key/pairs ################ */
		CxHashMap<CxI32, CxF32> h0;
		ass_cap_size(h0, 0, 0);
		h0.clear();
		h0.free();
		ass_cap_size(h0,0,0);

		h0 = CxHashMap<CxI32, CxF32>(10);
		ass_cap_size(h0, 13, 0);
		h0.clear();
		ass_cap_size(h0, 13, 0);
		h0.free();
		ass_cap_size(h0, 0, 0);

		h0[3] = 39;
		ass_eq(h0.size(), 1);
		ass_eq(h0.value(3, 0), 39);
		h0.clear();
		ass_eq(h0.size(), 0);
		ass_eq(h0.value(3, 0), 0);
		ass_true(h0.capacity() > 0);
		h0.free();
		ass_cap_size(h0, 0, 0);



		/* ############### Object key/pairs ################ */
		CxHashMap<Cx_Obj, Cx_Obj> h1;
		ass_cap_size(h1, 0, 0);
		h1.clear();
		h1.free();
		ass_cap_size(h1,0,0);

		h1 = CxHashMap<Cx_Obj, Cx_Obj>(10);
		ass_cap_size(h1, 13, 0);
		h1.clear();
		ass_cap_size(h1, 13, 0);
		h1.free();
		ass_cap_size(h1, 0, 0);

		h1[Cx_Obj(3)] = Cx_Obj(3,9);
		ass_eq(h1.size(), 1);
		ass_eq(h1.value(Cx_Obj(3), 0), Cx_Obj(3,9));
		h1.clear();
		ass_eq(h1.size(), 0);
		ass_eq(h1.value(Cx_Obj(3,9), 0), Cx_Obj());
		ass_true(h1.capacity() > 0);
		h1.free();
		ass_cap_size(h1, 0, 0);

		h1 = CxHashMap<Cx_Obj,Cx_Obj>(10);
		h1[Cx_Obj(1)] = Cx_Obj(1);
		h1[Cx_Obj(14)] = Cx_Obj(2);
		ass_eq(h1.size(), 2);
		ass_eq(h1.value(Cx_Obj(1), 0), Cx_Obj(1));
		ass_eq(h1.value(Cx_Obj(14), 0), Cx_Obj(2));
		h1.clear();
		ass_eq(h1.size(), 0);
		ass_eq(h1.value(Cx_Obj(1), 0), Cx_Obj());
		ass_eq(h1.value(Cx_Obj(14), 0), Cx_Obj());
		ass_true(h1.capacity() > 0);
		h1.free();
		ass_cap_size(h1, 0, 0);



		/* ############### string key's ################ */
		CxHashMap<CxChar *, Cx_Obj> h2;
		ass_cap_size(h2, 0, 0);
		h2.clear();
		h2.free();
		ass_cap_size(h2,0,0);

		h2 = CxHashMap<CxChar *, Cx_Obj>(10);
		ass_cap_size(h2, 13, 0);
		h2.clear();
		ass_cap_size(h2, 13, 0);
		h2.free();
		ass_cap_size(h2, 0, 0);

		h2["meow"] = Cx_Obj(3,9);
		ass_eq(h2.size(), 1);
		ass_eq(h2.value("meow", 0), Cx_Obj(3,9));
		h2.clear();
		ass_eq(h2.size(), 0);
		ass_eq(h2.value("meow", 0), Cx_Obj());
		ass_true(h2.capacity() > 0);
		h2.free();
		ass_cap_size(h2, 0, 0);

		h2 = CxHashMap<CxChar *,Cx_Obj>(10);
		h2["meow"] = Cx_Obj(1);
		h2["moo"] = Cx_Obj(2);
		ass_eq(h2.size(), 2);
		ass_eq(h2.value("meow", 0), Cx_Obj(1));
		ass_eq(h2.value("moo", 0), Cx_Obj(2));
		h2.clear();
		ass_eq(h2.size(), 0);
		ass_eq(h2.value("meow", 0), Cx_Obj());
		ass_eq(h2.value("moo", 0), Cx_Obj());
		ass_true(h2.capacity() > 0);
		h2.free();
		ass_cap_size(h2, 0, 0);
		
		FINISH_TEST;
	}

	/**
	 * @tests key(const V &) const
	 * @tests key(const V &, const K &) const
	 */
	void testCxHashMapKey() {
		BEGIN_TEST;

		/* ############# Simple key/value pair ################## */
		CxHashMap<CxI32, CxF32> h0;
		ass_eq(h0.key(2), h0.defaultKey());
		ass_eq(h0.key(2, 0), 0);

		h0 = CxHashMap<CxI32, CxF32>(10);
		ass_eq(h0.key(2), h0.defaultKey());
		ass_eq(h0.key(2, 0), 0);

		h0[1] = 2;
		ass_eq(h0.key(2), 1);
		ass_eq(h0.key(2, 0), 1);

		h0[14] = 3;
		ass_eq(h0.key(3), 14);
		ass_eq(h0.key(3, 0), 14);

		h0[2] = 2;
		ass_true(h0.key(2) == 1 || h0.key(2) == 2);


		/* ############# Object key/value pair ################## */
		CxHashMap<Cx_Obj, Cx_Obj> h1;
		ass_eq(h1.key(Cx_Obj(2)), h1.defaultKey());
		ass_eq(h1.key(Cx_Obj(2), Cx_Obj()), Cx_Obj());

		h1 = CxHashMap<Cx_Obj, Cx_Obj>(10);
		ass_eq(h1.key(Cx_Obj(2)), h1.defaultKey());
		ass_eq(h1.key(Cx_Obj(2), Cx_Obj()), Cx_Obj());

		h1[Cx_Obj(1)] = Cx_Obj(2);
		ass_eq(h1.key(Cx_Obj(2)), Cx_Obj(1));
		ass_eq(h1.key(Cx_Obj(2), Cx_Obj(0)), Cx_Obj(1));

		h1[Cx_Obj(14)] = Cx_Obj(3);
		ass_eq(h1.key(Cx_Obj(3)), Cx_Obj(14));
		ass_eq(h1.key(Cx_Obj(3), Cx_Obj(0)), Cx_Obj(14));

		h1[Cx_Obj(2)] = Cx_Obj(2);
		ass_true(h1.key(Cx_Obj(2)) == Cx_Obj(1) || h1.key(Cx_Obj(2)) == Cx_Obj(2));



		/* ############# string keys ################## */
		CxHashMap<CxChar *, Cx_Obj> h2;
		ass_eq(h2.key(Cx_Obj(2)), h2.defaultKey());
		ass_eq(h2.key(Cx_Obj(2), 0), 0);

		h2 = CxHashMap<CxChar *, Cx_Obj>(10);
		ass_eq(h2.key(Cx_Obj(2)), h2.defaultKey());
		ass_eq(h2.key(Cx_Obj(2), 0), 0);

		h2["meow"] = Cx_Obj(2);
		ass_true(str::eq(h2.key(Cx_Obj(2)), "meow"));
		ass_true(str::eq(h2.key(Cx_Obj(2), 0), "meow"));

		h2["moo"] = Cx_Obj(3);
	   ass_true(str::eq(h2.key(Cx_Obj(3)), "moo"));
		ass_true(str::eq(h2.key(Cx_Obj(3), 0), "moo"));

		h2["woof"] = Cx_Obj(2);
		ass_true(str::eq(h2.key(Cx_Obj(2)), "meow") ||
					str::eq(h2.key(Cx_Obj(2)), "woof"));

		FINISH_TEST;
	}

	/**
	 * @tests remove(const K &)
	 * @tests remove(Itr &)
	 * @tests take(const K &)
	 * @tests take(const K &, const V &)
	 */
	void testCxHashMapRemoveAndTake() {
		BEGIN_TEST;

		/* ########## Simple key/value pairs ########### */
		CxHashMap<CxI32, CxF32> h0;
		ass_zero(h0.remove(0));
		ass_zero(h0.remove(23));
		ass_eq(h0.take(0), h0.defaultValue());
		ass_eq(h0.take(0, 0), 0);
		ass_eq(h0.take(23, 1), 1);

		h0[1] = 23;
		ass_eq(h0.value(1, 0), 23);
		ass_eq(h0.size(), 1);
		h0.remove(2);  h0.remove(-1);
		h0.take(-1);   h0.take(2);
		ass_eq(h0.value(1, 0), 23);
		ass_eq(h0.size(), 1);
		
		h0.remove(1);
		ass_eq(h0.value(1, 0), 0);
		ass_zero(h0.size());
		ass_true(h0.take(1, 0) == 0);

		h0[1] = 99;
		h0[3] = 23;
		h0[99] = 32;
		ass_eq(h0.size(), 3);
		ass_true(h0[1] == 99 && h0[3] == 23 && h0[99] == 32);
		assert(h0.remove(99) == 1);
		assert(h0.take(3) == 23);
		ass_eq(h0.size(), 1);
		ass_true(h0.value(99, 0) == 0 && h0.value(3, 0) == 0);
		ass_eq(h0.value(1, 0), 99);
		assert(h0.take(1, 0) == 99);
		ass_eq(h0.size(), 0);
		assert(h0.take(1, 0) == 0);

		h0[1] = 23;
		h0[34] = 22;
		h0[111] = -23;
		ass_eq(h0.size(), 3);
		ass_true(h0[1] == 23 && h0[34] == 22 && h0[111] == -23);
		CxHashMap<CxI32,CxF32>::Itr itr = h0.find(34);
		ass_true(*itr == 22);
		itr = h0.remove(itr);
		ass_eq(h0.size(), 2);
		ass_eq(h0.value(34, 0), 0);
		if (itr != h0.end()) {
			ass_true(*itr == 23 || *itr == -23);
		}
		else { ass_true(itr == h0.end()); }

		itr = h0.begin();
		CxF32 val = *itr;
		ass_true(*itr == 23 || *itr == -23);
		itr = h0.remove(itr);
		ass_eq(h0.size(), 1);
		ass_true((*itr == 23 || *itr == -23) && (*itr != val));
		itr = h0.remove(itr);
		ass_eq(h0.size(), 0);
		ass_true(itr == h0.end());



		/* ########## Object key/value pairs ########### */
		CxHashMap<Cx_Obj, Cx_Obj> h1;
		ass_zero(h1.remove(Cx_Obj()));
		ass_zero(h1.remove(Cx_Obj(2,3,0)));
		ass_eq(h1.take(Cx_Obj()), h1.defaultValue());
		ass_eq(h1.take(Cx_Obj(), Cx_Obj()), Cx_Obj());
		ass_eq(h1.take(Cx_Obj(2,3,0), Cx_Obj(1)), Cx_Obj(1));

		h1[Cx_Obj(1)] = Cx_Obj(2,3,0);
		ass_eq(h1.value(Cx_Obj(1), Cx_Obj()), Cx_Obj(2,3,0));
		ass_eq(h1.size(), 1);
		h1.remove(Cx_Obj(2));  h1.remove(Cx_Obj(-1));
		h1.take(Cx_Obj(-1));   h1.take(Cx_Obj(2));
		ass_eq(h1.value(Cx_Obj(1), Cx_Obj()), Cx_Obj(2,3,0));
		ass_eq(h1.size(), 1);
		
		h1.remove(Cx_Obj(1));
		ass_eq(h1.value(Cx_Obj(1), Cx_Obj()), Cx_Obj());
		ass_zero(h1.size());
		ass_true(h1.take(Cx_Obj(1), Cx_Obj()) == Cx_Obj());

		h1[Cx_Obj(1)] = Cx_Obj(9,9);
		h1[Cx_Obj(3)] = Cx_Obj(2,3);
		h1[Cx_Obj(9,9)] = Cx_Obj(3,2);
		ass_eq(h1.size(), 3);
		ass_true(h1[Cx_Obj(1)] == Cx_Obj(9,9) && h1[Cx_Obj(3)] == Cx_Obj(2,3) && h1[Cx_Obj(9,9)] == Cx_Obj(3,2));
		assert(h1.remove(Cx_Obj(9,9)) == 1);
		assert(h1.take(Cx_Obj(3)) == Cx_Obj(2,3));
		ass_eq(h1.size(), 1);
		ass_true(h1.value(Cx_Obj(9,9), Cx_Obj()) == Cx_Obj() && h1.value(Cx_Obj(3), Cx_Obj()) == Cx_Obj());
		ass_eq(h1.value(Cx_Obj(1), Cx_Obj()), Cx_Obj(9,9));
		assert(h1.take(Cx_Obj(1), Cx_Obj()) == Cx_Obj(9,9));
		ass_eq(h1.size(), 0);
		assert(h1.take(Cx_Obj(1), Cx_Obj()) == Cx_Obj());

		h1[Cx_Obj(1)] = Cx_Obj(2,3);
		h1[Cx_Obj(3,4)] = Cx_Obj(2,2);
		h1[Cx_Obj(1,1,1)] = Cx_Obj(-2,-3);
		ass_eq(h1.size(), 3);
		ass_true(h1[Cx_Obj(1)] == Cx_Obj(2,3) && h1[Cx_Obj(3,4)] == Cx_Obj(2,2) && h1[Cx_Obj(1,1,1)] == Cx_Obj(-2,-3));
		CxHashMap<Cx_Obj,Cx_Obj>::Itr itr1 = h1.find(Cx_Obj(3,4));
		ass_true(*itr1 == Cx_Obj(2,2));
		itr1 = h1.remove(itr1);
		ass_eq(h1.size(), 2);
		ass_eq(h1.value(Cx_Obj(3,4), Cx_Obj()), Cx_Obj());
		if (itr1 != h1.end()) {
			ass_true(*itr1 == Cx_Obj(2,3) || *itr1 == Cx_Obj(-2,-3));
		}
		else { ass_true(itr1 == h1.end()); }

		itr1 = h1.begin();
		Cx_Obj val1 = *itr1;
		ass_true(*itr1 == Cx_Obj(2,3) || *itr1 == Cx_Obj(-2,-3));
		itr1 = h1.remove(itr1);
		ass_eq(h1.size(), 1);
		ass_true((*itr1 == Cx_Obj(2,3) || *itr1 == Cx_Obj(-2,-3)) && (*itr1 != val1));
		itr1 = h1.remove(itr1);
		ass_eq(h1.size(), 0);
		ass_true(itr1 == h1.end());


		/* ########## string keys ########### */
		CxHashMap<CxChar *, Cx_Obj> h2;
		ass_zero(h2.remove("Meow"));
		ass_zero(h2.remove("Moo"));
		ass_eq(h2.take("Meow"), h2.defaultValue());
		ass_eq(h2.take("Meow", Cx_Obj()), Cx_Obj());
		ass_eq(h2.take("Moo", Cx_Obj(1)), Cx_Obj(1));

		h2["Meow"] = Cx_Obj(2,3,0);
		ass_eq(h2.value("Meow", Cx_Obj()), Cx_Obj(2,3,0));
		ass_eq(h2.size(), 1);
		h2.remove("Meow2");  h2.remove("Meow-1");
		h2.take("Meow-1");   h2.take("Meow2");
		ass_eq(h2.value("Meow", Cx_Obj()), Cx_Obj(2,3,0));
		ass_eq(h2.size(), 1);
		
		h2.remove("Meow");
		ass_eq(h2.value("Meow", Cx_Obj()), Cx_Obj());
		ass_zero(h2.size());
		ass_true(h2.take("Meow", Cx_Obj()) == Cx_Obj());

		h2["Meow"] = Cx_Obj(9,9);
		h2["Moo"] = Cx_Obj(2,3);
		h2["Woof"] = Cx_Obj(3,2);
		ass_eq(h2.size(), 3);
		ass_true(h2["Meow"] == Cx_Obj(9,9) && h2["Moo"] == Cx_Obj(2,3) && h2["Woof"] == Cx_Obj(3,2));
		assert(h2.remove("Woof") == 1);
		assert(h2.take("Moo") == Cx_Obj(2,3));
		ass_eq(h2.size(), 1);
		ass_true(h2.value("Woof", Cx_Obj()) == Cx_Obj() && h2.value("Moo", Cx_Obj()) == Cx_Obj());
		ass_eq(h2.value("Meow", Cx_Obj()), Cx_Obj(9,9));
		assert(h2.take("Meow", Cx_Obj()) == Cx_Obj(9,9));
		ass_eq(h2.size(), 0);
		assert(h2.take("Meow", Cx_Obj()) == Cx_Obj());

		h2["Meow"] = Cx_Obj(2,3);
		h2["Mooo"] = Cx_Obj(2,2);
		h2["Bark"] = Cx_Obj(-2,-3);
		ass_eq(h2.size(), 3);
		ass_true(h2["Meow"] == Cx_Obj(2,3) && h2["Mooo"] == Cx_Obj(2,2) && h2["Bark"] == Cx_Obj(-2,-3));
		CxHashMap<CxChar *,Cx_Obj>::Itr itr2 = h2.find("Mooo");
		ass_true(*itr2 == Cx_Obj(2,2));
		itr2 = h2.remove(itr2);
		ass_eq(h2.size(), 2);
		ass_eq(h2.value("Mooo", Cx_Obj()), Cx_Obj());
		if (itr2 != h2.end()) {
			ass_true(*itr2 == Cx_Obj(2,3) || *itr2 == Cx_Obj(-2,-3));
		}
		else { ass_true(itr2 == h2.end()); }

		itr2 = h2.begin();
		Cx_Obj val2 = *itr2;
		ass_true(*itr2 == Cx_Obj(2,3) || *itr2 == Cx_Obj(-2,-3));
		itr2 = h2.remove(itr2);
		ass_eq(h2.size(), 1);
		ass_true((*itr2 == Cx_Obj(2,3) || *itr2 == Cx_Obj(-2,-3)) && (*itr2 != val2));
		itr2 = h2.remove(itr2);
		ass_eq(h2.size(), 0);
		ass_true(itr2 == h2.end());
		
		FINISH_TEST;
	}

	/**
	 * @tests reserve(CxI32)
	 */
	void testCxHashMapReserve() {
		BEGIN_TEST;

		/* ############# Simple key/value pairs ############ */
		CxHashMap<CxI32, CxF32> h0;
		ass_cap_size(h0, 0, 0);
		h0.reserve(1);
		ass_cap_size(h0, 2, 0);

		h0.reserve(0);
		ass_cap_size(h0, 2, 0);

		h0.reserve(10);
		ass_cap_size(h0, 13, 0);

		h0 = CxHashMap<CxI32, CxF32>();
		ass_cap_size(h0, 0, 0);

		h0[1] = 10;
		ass_cap_size(h0, 2, 1);
		h0[2] = 11;
		ass_cap_size(h0, 5, 2);
		h0[3] = 12;
		ass_cap_size(h0, 5, 3);

		h0.reserve(10);
		ass_cap_size(h0, 13, 3);
		ass_eq(h0.value(1, 0), 10);
		ass_eq(h0.value(2, 0), 11);
		ass_eq(h0.value(3, 0), 12);


		/* ############# Object key/value pairs ############ */
		CxHashMap<Cx_Obj, Cx_Obj> h1;
		ass_cap_size(h1, 0, 0);
		h1.reserve(1);
		ass_cap_size(h1, 2, 0);

		h1.reserve(0);
		ass_cap_size(h1, 2, 0);

		h1.reserve(10);
		ass_cap_size(h1, 13, 0);

		h1 = CxHashMap<Cx_Obj, Cx_Obj>();
		ass_cap_size(h1, 0, 0);

		h1[Cx_Obj(1)] = Cx_Obj(1,0);
		ass_cap_size(h1, 2, 1);
		h1[Cx_Obj(2)] = Cx_Obj(1,1);
		ass_cap_size(h1, 5, 2);
		h1[Cx_Obj(3)] = Cx_Obj(1,2);
		ass_cap_size(h1, 5, 3);

		h1.reserve(10);
		ass_cap_size(h1, 13, 3);
		ass_eq(h1.value(Cx_Obj(1), Cx_Obj()), Cx_Obj(1,0));
		ass_eq(h1.value(Cx_Obj(2), Cx_Obj()), Cx_Obj(1,1));
		ass_eq(h1.value(Cx_Obj(3), Cx_Obj()), Cx_Obj(1,2));



		/* ############# string keys  ############ */
		CxHashMap<CxChar *, Cx_Obj> h2;
		ass_cap_size(h2, 0, 0);
		h2.reserve(1);
		ass_cap_size(h2, 2, 0);

		h2.reserve(0);
		ass_cap_size(h2, 2, 0);

		h2.reserve(10);
		ass_cap_size(h2, 13, 0);

		h2 = CxHashMap<CxChar *, Cx_Obj>();
		ass_cap_size(h2, 0, 0);

		h2["Meow"] = Cx_Obj(1,0);
		ass_cap_size(h2, 2, 1);
		h2["Moo"] = Cx_Obj(1,1);
		ass_cap_size(h2, 5, 2);
		h2["Woof"] = Cx_Obj(1,2);
		ass_cap_size(h2, 5, 3);

		h2.reserve(10);
		ass_cap_size(h2, 13, 3);
		ass_eq(h2.value("Meow", Cx_Obj()), Cx_Obj(1,0));
		ass_eq(h2.value("Moo", Cx_Obj()), Cx_Obj(1,1));
		ass_eq(h2.value("Woof", Cx_Obj()), Cx_Obj(1,2));
		FINISH_TEST;
	}

	void testCxHashMapItr() {
		BEGIN_TEST;

		CxHashMap<CxI32, CxF32> h0(10);
		h0[1] = 11;
		h0[2] = 22;
		h0[3] = 33;
		h0[4] = 44;

		ass_eq(h0.size(), 4);
		CxHashMap<CxI32, CxF32>::Itr i0 = h0.begin();
		ass_true(i0 == h0.begin() && h0.begin() == i0);
		ass_false(i0 != h0.begin() || h0.begin() != i0);
		ass_true(i0 != h0.end());
		ass_true(*i0 = 11);
		ass_true(i0.key() == 1);
		ass_true(i0.value() == 11);

		i0 = i0 + 2;
		ass_true(i0 != h0.begin());
		ass_false(i0 == h0.begin());
		ass_true(*i0 == 33);
		ass_true(i0.key() == 3);

		i0 = i0 - 2;
		ass_true(i0 == h0.begin() && h0.begin() == i0);
		ass_true(*i0 = 11);
		ass_true(i0.key() == 1);

		CxHashMap<CxI32, CxF32>::Itr i1 = i0++;
		ass_true(i1 == h0.begin());
		ass_true(i0 != i1 && i1 != i0);
		ass_true(*i0 = 22);
		ass_true(*i1 = 11);

		i0 -= 1;
		ass_true(i0 == h0.begin());
		i1 = ++i0;
		ass_true(i1 == i0 && i0 == i1);
		ass_true(*i1 == 22 && *i0 == 22);

		i1 = i0--;
		ass_true(i0 != i1 && i1 != i0);
		ass_true(i0 == h0.begin());
		ass_true(*i0 == 11 && *i1 == 22);

		i0++;
		i1 = --i0;
		ass_true(i1 == i0 && i0 == i1);
		ass_true(*i1 == 11 && *i0 == 11);
		ass_true(i1 == h0.begin() && i0 == h0.begin());
		
		ass_true(*i0 == 11);
		++i0;
		ass_true(*i0 == 22);
		++i0;
		ass_true(*i0 == 33);
		++i0;
		ass_true(*i0 == 44);
		ass_true(i0 != h0.end());
		++i0;
		ass_true(i0 == h0.end());

		FINISH_TEST;
	}
} // namespace cat

int main(int argc, char **argv) {
	cat::testCxHashMapCreateAndDestroy();
	cat::testCxHashMapCopyAndAssign();
	cat::testCxHashMapInsertAndAccess();
	cat::testCxHashMapEquality();
	cat::testCxHashMapContainsCountFind();
	cat::testCxHashMapClearFree();
	cat::testCxHashMapKey();
	cat::testCxHashMapRemoveAndTake();
	cat::testCxHashMapReserve();
	cat::testCxHashMapItr();
	
	return 0;
}
