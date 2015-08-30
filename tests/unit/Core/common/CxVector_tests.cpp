#include "core/common/CxVector.h"

#define CONT_TYPE(in_type) CxVector<in_type>
#include "CxContainerTestCode.h"

namespace cat {

	/** 
	 * @tests CxVector()
	 * @tests CxVector(CxI32)
	 * @tests CxVector(CxI32,const T &)
	 * @tests CxVector(T *,CxI32,CxCopy)
	 * @tests CxVector(const CxVector<T> &)
	 * @tests ~CxVector()
	 * @tests operator=(const CxVector<T> &)
	 */
	void testCxVectorCreateAndDestroy() {
		BEGIN_TEST;
		testCxContainerClassCreateAndDestroy();
		FINISH_TEST;
	}

	/** 
	 * @tests operator==(const CxVector<T> &) const
	 * @tests operator!=(const CxVector<T> &) const
	 */
	void testCxVectorEquality() {
		BEGIN_TEST;
		testCxContainerClassEquality();
		FINISH_TEST;
	}

	/** 
	 * @tests operator[](CxI32)
	 * @tests operator[](const CxI32) const
	 * @tests at(CxI32) const
	 * @tests value(CxI32) const
	 * @tests constData() const
	 * @tests data()
	 * @tests data() const
	 */
	void testCxVectorAccess() {
		BEGIN_TEST;
		testCxContainerClassAccess();
		FINISH_TEST;
	}

	/**
	 * @tests operator+(const CxVector<T> &) const
	 * @tests operator+=(const CxVector<T> &)
	 * @tests operator<<(const CxVector<T> &)
	 * @tests append(const CxVector<T> &)
	 */
	void testCxVectorConcatenate() {
		BEGIN_TEST;
		testCxContainerClassConcatenate();
		FINISH_TEST;
	}

	/**
	 * @tests append(const T &)
	 * @tests operator<<(const T &)
	 * @tests operator+=(const T &)
	 */
	void testCxVectorAppend() {
		BEGIN_TEST;
		testCxContainerClassAppend();
		FINISH_TEST;
	}

	/**
	 * @tests capacity() const
	 * @tests size() const
	 * @tests length() const
	 * @tests count() const
	 * @tests reserve(CxI32)
	 * @tests isEmpty() const
	 */
	void testCxVectorCapacity() {
		BEGIN_TEST;
		testCxContainerClassCapacity();
		FINISH_TEST;
	}
	
	/**
	 * @tests set(CxI32, const T &)
	 * @tests setAll(const T &)
	 */
	void testCxVectorSet() {
		BEGIN_TEST;
		testCxContainerClassSet();
		FINISH_TEST;
	}

	/**
	 * @tests compact()
	 */
	void testCxVectorCompact() {
		BEGIN_TEST;
		testCxContainerClassCompact();
		FINISH_TEST;
	}

	/**
	 * @tests contains(const T &)
	 */
	void testCxVectorContains() {
		BEGIN_TEST;
		testCxContainerClassContains();
		FINISH_TEST;
	}

	/** 
	 * @tests count(const T &)
	 */
	void testCxVectorCount() {
		BEGIN_TEST;
		testCxContainerClassCount();
		FINISH_TEST;
	}

	/**
	 * @tests first()
	 * @tests first() const
	 * @tests last()
	 * @tests last() const
	 */
	void testCxVectorFirstLast() {
		BEGIN_TEST;
		testCxContainerClassFirstLast();
		FINISH_TEST;
	}

	/**
	 * @tests dealloc()
	 * @tests deallocPtr()
	 */
	void testCxVectorDealloc() {
		BEGIN_TEST;
		testCxContainerClassDealloc();
		FINISH_TEST;
	}

	/**
	 * @tests endsWith(const T &)
	 */
	void testCxVectorEndsWith() {
		BEGIN_TEST;
		testCxContainerClassEndsWith();
		FINISH_TEST;
	}

	/**
	 * @tests eraseAt(CxI32)
	 * @tests eraseFirst()
	 * @tests eraseLast()
	 * @tests eraseAll()
	 */
	void testCxVectorErase() {
		BEGIN_TEST;
		testCxContainerClassErase();
		FINISH_TEST;
	}

	/**
	 * @tests fill(const T &, CxI32)
	 */
	void testCxVectorFill() {
		BEGIN_TEST;
		testCxContainerClassFill();
		FINISH_TEST;
	}

	/**
	 * @tests indexOf(const T &, CxI32) const
	 * @tests lastIndexOf(const T &, CxI32) const
	 */
	void testCxVectorIndexOf() {
		BEGIN_TEST;
		testCxContainerClassIndexOf();
		FINISH_TEST;
	}

	void testCxVectorInsert() {
		BEGIN_TEST;
		testCxContainerClassInsert();
		FINISH_TEST;
	}

	/**
	 * @tests remove(const T &)
	 */
	void testCxVectorRemove() {
		BEGIN_TEST;
		testCxContainerClassRemove();
		FINISH_TEST;
	}

	/**
	 * @tests removeAll(const T &)
	 */
	void testCxVectorRemoveAll() {
		BEGIN_TEST;
		testCxContainerClassRemoveAll();
		FINISH_TEST;
	}

	/** 
	 * @tests removeAt(CxI32)
	 */
	void testCxVectorRemoveAt() {
		BEGIN_TEST;
		testCxContainerClassRemoveAt();
		FINISH_TEST;
	}

	/**
	 * @tests removeFirst()
	 * @tests removeLast()
	 */
	void testCxVectorRemoveFirstLast() {
		BEGIN_TEST;
		testCxContainerClassRemoveFirstLast();
		FINISH_TEST;
	}

	/**
	 * @tests replace(const T &, const T &)
	 */
	void testCxVectorReplace() {
		BEGIN_TEST;
		testCxContainerClassReplace();
		FINISH_TEST;
	}

	/**
	 * @tests replaceAll(const T & const T &)
	 */
	void testCxVectorReplaceAll() {
		BEGIN_TEST;
		testCxContainerClassReplaceAll();
		FINISH_TEST;
	}

	/**
	 * @tests replaceAt(CxI32, const T &)
	 */
	void testCxVectorReplaceAt() {
		BEGIN_TEST;
		testCxContainerClassReplaceAt();
		FINISH_TEST;
	}

	/**
	 * @tests reserve(CxI32)
	 * @tests resizeToCapacity(CxI32)
	 */
	void testCxVectorReserve() {
		BEGIN_TEST;
		testCxContainerClassReserve();
		FINISH_TEST;
	}

	/**
	 * @tests resize(CxI32)
	 * @tests resizeToCapacity(CxI32)
	 */
	void testCxVectorResize() {
		BEGIN_TEST;
		testCxContainerClassResize();
		FINISH_TEST;
	}

	/**
	 * @tests sort(CxI32 (*)(const void *, const void *)
	 * @tests sortAsc()
	 * @tests sortDesc()
	 */
	void testCxVectorSort() {
		BEGIN_TEST;
		testCxContainerClassSort();
		FINISH_TEST;
	}

	/**
	 * @tests startsWith(const T &)
	 */
	void testCxVectorStartsWith() {
		BEGIN_TEST;
		testCxContainerClassStartsWith();
		FINISH_TEST;
	}

	/**
	 * @tests swap(const CxVector<T> &)
	 */
	void testCxVectorSwap() {
		BEGIN_TEST;
		testCxContainerClassSwap();
		FINISH_TEST;
	}

	/**
	 * @tests takeAt(CxI32)
	 */
	void testCxVectorTakeAt() {
		BEGIN_TEST;
		testCxContainerClassTakeAt();
		FINISH_TEST;
	}

	/**
	 * @tests takeFirst()
	 * @tests takeLast()
	 */
	void testCxVectorTakeFirstLast() {
		BEGIN_TEST;
		testCxContainerClassTakeFirstLast();
		FINISH_TEST;
	}

	/**
	 * @tests retain()
	 * @tests release()
	 * @tests retainCount() const
	 */
	void testCxVectorISPTR() {
		BEGIN_TEST;
		testCxContainerClassISPTR();
		FINISH_TEST;
	}
	
} // namespace cat

int main(int argc, char **argv) {
	cat::testCxVectorCreateAndDestroy();
	cat::testCxVectorEquality();
	cat::testCxVectorAccess();
	cat::testCxVectorConcatenate();
	cat::testCxVectorAppend();
	cat::testCxVectorCapacity();
	cat::testCxVectorSet();
	cat::testCxVectorCompact();
	cat::testCxVectorContains();
	cat::testCxVectorCount();
	cat::testCxVectorFirstLast();
	cat::testCxVectorDealloc();
	cat::testCxVectorEndsWith();
	cat::testCxVectorErase();
	cat::testCxVectorFill();
	cat::testCxVectorIndexOf();
	cat::testCxVectorInsert();
	cat::testCxVectorRemove();
	cat::testCxVectorRemoveAll();
	cat::testCxVectorRemoveAt();
	cat::testCxVectorRemoveFirstLast();
	cat::testCxVectorReplace();
	cat::testCxVectorReplaceAll();
	cat::testCxVectorReplaceAt();
	cat::testCxVectorReserve();
	cat::testCxVectorResize();
	cat::testCxVectorSort();
	cat::testCxVectorStartsWith();
	cat::testCxVectorSwap();
	cat::testCxVectorTakeAt();
	cat::testCxVectorTakeFirstLast();
	cat::testCxVectorISPTR();
	return 0;
}
