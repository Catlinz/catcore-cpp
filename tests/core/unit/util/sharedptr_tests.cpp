#include "core/testcore.h"
#include "core/util/sharedptr.h"

cc::I32 testSharedPtrCount(cc::SharedPtr<int> ptr) {
	cc::I32 references = ptr.count();
	return references;
}

cc::I32 to_destroyed = 0;



namespace cc {

	class TestObject {
	public:
		TestObject() 
			: m_x(0.0f), m_y(0.0f), m_z(0.0f) {}
	  
		TestObject(F32 dx, F32 dy, F32 dz)
			: m_x(dx), m_y(dy), m_z(dz) { }

		~TestObject() {
			to_destroyed++;
			std::cout << "Destroyed count: " << to_destroyed << std::endl;			
		}
				
		Boolean test(F32 dx, F32 dy, F32 dz) {
			return (dx == m_x && dy == m_y && dz == m_z);
		}

		inline F32 x() const { return m_x; }
		inline F32 y() const { return m_y; }
		inline F32 z() const { return m_z; }
		
  	
	private:
		F32 m_x, m_y, m_z;
		
	};

	void testSharedPtrCreateAndDestroySimple() {
		BEGIN_TEST;

		int *y = (int*)malloc(sizeof(int));
		*y = 2;
		int *x = (int*)malloc(sizeof(int));
		*x = 3;

		TestObject* obj = new TestObject(1.0f, 9.0f, 100.0f);		
	
		SharedPtr<int> ptr;
		assert( ptr.countPtr() == NIL );
		assert( ptr.isNull() );
		
		
		ptr = SharedPtr<int>(y, PFM_FREE);
		assert( ptr.count() == 1 && ptr.ptrFreeMethod() == PFM_FREE );
		assert( *ptr == 2 );
		assert( !ptr.isNull() );

		SharedPtr<int> ptr2 = SharedPtr<int>(ptr);
		assert( ptr.count() == 2 && ptr2.count() == 2 );
		assert( *ptr == 2 && *ptr2 == 2 );

		ptr2 = SharedPtr<int>(x, PFM_FREE);		

		assert( ptr.count() == 1 && *ptr == 2 );
		assert( ptr2.count() == 1 && *ptr2 == 3 );

		ptr = ptr2;
		assert( ptr.count() == 2 && ptr2.count() == 2 );
		assert( *ptr == 3 && *ptr2 == 3 );		
		
		SharedPtr<TestObject> optr(obj);
		assert( optr.count() == 1 && (*optr).test(1.0f, 9.0f, 100.0f) );	

		assert( to_destroyed == 0 );	
		assert( !optr.isNull() );
		optr.setNull();
		assert( optr.isNull() );
		assert( to_destroyed == 1 );	

		obj = new TestObject(1.0f, 2.0f, 3.0f);

		optr = SharedPtr<TestObject>(obj);
		assert( optr.count() == 1 && (*optr).test(1.0f, 2.0f, 3.0f) );

		FINISH_TEST;
	}	

	void testSharedPtrMethods() {
		BEGIN_TEST;

		to_destroyed = 0;
		assert( to_destroyed == 0 );		
		
		TestObject* obj = new TestObject(1.0f, 2.0f, -3.0f);
		
		SharedPtr<TestObject> p(obj);
		assert( p->x() == 1.0f && p->y() == 2.0f && p->z() == -3.0f );
		assert( p->test(1.0f, 2.0f, -3.0f) );
		
		assert( (*p).x() == 1.0f && (*p).y() == 2.0f && (*p).z() == -3.0f );
		assert( (*p).test(1.0f, 2.0f, -3.0f) );		
		
		assert( p.get()->x() == 1.0f && p.get()->y() == 2.0f && p.get()->z() == -3.0f );
		assert( p.get()->test(1.0f, 2.0f, -3.0f) );
		
		assert( p.ptrFreeMethod() == PFM_DELETE );

		TestObject* objArray = new TestObject[3];
		objArray[0] = TestObject(1.0f, 2.0f, 3.0f);
		objArray[1] = TestObject(2.0f, -3.0f, 9.0f);
		objArray[2] = TestObject(0.0f, 0.0f, 1.0f);		
		
		p = SharedPtr<TestObject>(objArray, PFM_DELETE_ARRAY);
		assert( p[0].x() == 1.0f && p[0].y() == 2.0f && p[0].z() == 3.0f );
		assert( p[0].test(1.0f, 2.0f, 3.0f) );
		assert( p[1].x() == 2.0f && p[1].y() == -3.0f && p[1].z() == 9.0f );
		assert( p[1].test(2.0f, -3.0f, 9.0f) );
		assert( p[2].x() == 0.0f && p[2].y() == 0.0f && p[2].z() == 1.0f );
		assert( p[2].test(0.0f, 0.0f, 1.0f) );

		FINISH_TEST;
		
	}

	void testSharedPtrCreateAndDestroyParameterPassing() {
		BEGIN_TEST;

		int *y = (int*)malloc(sizeof(int));
		*y = 2;
		int *x = (int*)malloc(sizeof(int));
		*x = 3;
	
		SharedPtr<int> ptr;
		assert( ptr.countPtr() == NIL );
		
		ptr = SharedPtr<int>(y, PFM_FREE);
		assert( ptr.count() == 1 );
		assert( *ptr == 2 );

		SharedPtr<int> ptr2 = SharedPtr<int>(ptr);
		assert( ptr.count() == 2 && ptr2.count() == 2 );
		assert( *ptr == 2 && *ptr2 == 2 );

		assert( testSharedPtrCount(ptr) == 3 ); 
		assert( testSharedPtrCount(ptr2) == 3 );


		ptr2 = SharedPtr<int>(x, PFM_FREE);

		assert( testSharedPtrCount(ptr) == 2 ); 
		assert( testSharedPtrCount(ptr2) == 2 );

		assert( ptr.count() == 1 && *ptr == 2 );
		assert( ptr2.count() == 1 && *ptr2 == 3 );

		ptr = ptr2;
		assert( ptr.count() == 2 && ptr2.count() == 2 );
		assert( *ptr == 3 && *ptr2 == 3 );	
		assert( testSharedPtrCount(ptr) == 3 ); 
		assert( testSharedPtrCount(ptr2) == 3 );

		FINISH_TEST;
	}	



} // namespace cc

int main(int argc, char** argv) {
	cc::testSharedPtrCreateAndDestroySimple();
	cc::testSharedPtrMethods();	
	cc::testSharedPtrCreateAndDestroyParameterPassing();
	return 0;
}

