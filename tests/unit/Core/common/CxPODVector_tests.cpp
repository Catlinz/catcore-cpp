#include "core/common/CxPODVector.h"

#define CONT_TYPE(in_type) CxPODVector<in_type>
#include "CxContainerTestCode.h"

namespace cat {

	/** 
	 * @tests CxPODVector()
	 * @tests CxPODVector(CxI32)
	 * @tests CxPODVector(CxI32,const T &)
	 * @tests CxPODVector(T *,CxI32,CxCopy)
	 * @tests CxPODVector(const CxPODVector<T> &)
	 * @tests ~CxPODVector()
	 * @tests operator=(const CxPODVector<T> &)
	 */
	void testCxPODVectorCreateAndDestroy() {
		BEGIN_TEST;
		testCxContainerClassCreateAndDestroy();
		FINISH_TEST;
	}

	/** 
	 * @tests operator==(const CxPODVector<T> &) const
	 * @tests operator!=(const CxPODVector<T> &) const
	 */
	void testCxPODVectorEquality() {
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
	void testCxPODVectorAccess() {
		BEGIN_TEST;
		testCxContainerClassAccess();
		FINISH_TEST;
	}

	/**
	 * @tests operator+(const CxPODVector<T> &) const
	 * @tests operator+=(const CxPODVector<T> &)
	 * @tests operator<<(const CxPODVector<T> &)
	 * @tests append(const CxPODVector<T> &)
	 */
	void testCxPODVectorConcatenate() {
		BEGIN_TEST;
		testCxContainerClassConcatenate();
		FINISH_TEST;
	}

	/**
	 * @tests append(const T &)
	 * @tests operator<<(const T &)
	 * @tests operator+=(const T &)
	 */
	void testCxPODVectorAppend() {
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
	void testCxPODVectorCapacity() {
		BEGIN_TEST;
		testCxContainerClassCapacity();
		FINISH_TEST;
	}
	
	/**
	 * @tests set(CxI32, const T &)
	 * @tests setAll(const T &)
	 */
	void testCxPODVectorSet() {
		BEGIN_TEST;
		testCxContainerClassSet();
		FINISH_TEST;
	}

	/**
	 * @tests compact()
	 */
	void testCxPODVectorCompact() {
		BEGIN_TEST;
		testCxContainerClassCompact();
		FINISH_TEST;
	}

	/**
	 * @tests contains(const T &)
	 */
	void testCxPODVectorContains() {
		BEGIN_TEST;
		testCxContainerClassContains();
		FINISH_TEST;
	}

	/** 
	 * @tests count(const T &)
	 */
	void testCxPODVectorCount() {
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
	void testCxPODVectorFirstLast() {
		BEGIN_TEST;
		testCxContainerClassFirstLast();
		FINISH_TEST;
	}

	/**
	 * @tests dealloc()
	 * @tests deallocPtr()
	 */
	void testCxPODVectorDealloc() {
		BEGIN_TEST;
		testCxContainerClassDealloc();
		FINISH_TEST;
	}

	/**
	 * @tests endsWith(const T &)
	 */
	void testCxPODVectorEndsWith() {
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
	void testCxPODVectorErase() {
		BEGIN_TEST;
		testCxContainerClassErase();
		FINISH_TEST;
	}

	/**
	 * @tests fill(const T &, CxI32)
	 */
	void testCxPODVectorFill() {
		BEGIN_TEST;
		testCxContainerClassFill();
		FINISH_TEST;
	}

	/**
	 * @tests indexOf(const T &, CxI32) const
	 * @tests lastIndexOf(const T &, CxI32) const
	 */
	void testCxPODVectorIndexOf() {
		BEGIN_TEST;
		testCxContainerClassIndexOf();
		FINISH_TEST;
	}

	void testCxPODVectorInsert() {
		BEGIN_TEST;
		testCxContainerClassInsert();
		FINISH_TEST;
	}

	/**
	 * @tests remove(const T &)
	 */
	void testCxPODVectorRemove() {
		BEGIN_TEST;
		testCxContainerClassRemove();
		FINISH_TEST;
	}

	/**
	 * @tests removeAll(const T &)
	 */
	void testCxPODVectorRemoveAll() {
		BEGIN_TEST;
		testCxContainerClassRemoveAll();
		FINISH_TEST;
	}

	/** 
	 * @tests removeAt(CxI32)
	 */
	void testCxPODVectorRemoveAt() {
		BEGIN_TEST;
		testCxContainerClassRemoveAt();
		FINISH_TEST;
	}

	/**
	 * @tests removeFirst()
	 * @tests removeLast()
	 */
	void testCxPODVectorRemoveFirstLast() {
		BEGIN_TEST;
		testCxContainerClassRemoveFirstLast();
		FINISH_TEST;
	}

	/**
	 * @tests replace(const T &, const T &)
	 */
	void testCxPODVectorReplace() {
		BEGIN_TEST;
		testCxContainerClassReplace();
		FINISH_TEST;
	}

	/**
	 * @tests replaceAll(const T & const T &)
	 */
	void testCxPODVectorReplaceAll() {
		BEGIN_TEST;
		testCxContainerClassReplaceAll();
		FINISH_TEST;
	}

	/**
	 * @tests replaceAt(CxI32, const T &)
	 */
	void testCxPODVectorReplaceAt() {
		BEGIN_TEST;
		testCxContainerClassReplaceAt();
		FINISH_TEST;
	}

	/**
	 * @tests reserve(CxI32)
	 * @tests resizeToCapacity(CxI32)
	 */
	void testCxPODVectorReserve() {
		BEGIN_TEST;
		testCxContainerClassReserve();
		FINISH_TEST;
	}

	/**
	 * @tests resize(CxI32)
	 * @tests resizeToCapacity(CxI32)
	 */
	void testCxPODVectorResize() {
		BEGIN_TEST;
		testCxContainerClassResize();
		FINISH_TEST;
	}

	/**
	 * @tests sort(CxI32 (*)(const void *, const void *)
	 * @tests sortAsc()
	 * @tests sortDesc()
	 */
	void testCxPODVectorSort() {
		BEGIN_TEST;
		testCxContainerClassSort();
		FINISH_TEST;
	}

	/**
	 * @tests startsWith(const T &)
	 */
	void testCxPODVectorStartsWith() {
		BEGIN_TEST;
		testCxContainerClassStartsWith();
		FINISH_TEST;
	}

	/**
	 * @tests swap(const CxPODVector<T> &)
	 */
	void testCxPODVectorSwap() {
		BEGIN_TEST;
		testCxContainerClassSwap();
		FINISH_TEST;
	}

	/**
	 * @tests takeAt(CxI32)
	 */
	void testCxPODVectorTakeAt() {
		BEGIN_TEST;
		testCxContainerClassTakeAt();
		FINISH_TEST;
	}

	/**
	 * @tests takeFirst()
	 * @tests takeLast()
	 */
	void testCxPODVectorTakeFirstLast() {
		BEGIN_TEST;
		testCxContainerClassTakeFirstLast();
		FINISH_TEST;
	}

	/**
	 * @tests retain()
	 * @tests release()
	 * @tests retainCount() const
	 */
	void testCxPODVectorISPTR() {
		BEGIN_TEST;
		testCxContainerClassISPTR();
		FINISH_TEST;
	}
	
} // namespace cat

int main(int argc, char **argv) {
	cat::testCxPODVectorCreateAndDestroy();
	cat::testCxPODVectorEquality();
	cat::testCxPODVectorAccess();
	cat::testCxPODVectorConcatenate();
	cat::testCxPODVectorAppend();
	cat::testCxPODVectorCapacity();
	cat::testCxPODVectorSet();
	cat::testCxPODVectorCompact();
	cat::testCxPODVectorContains();
	cat::testCxPODVectorCount();
	cat::testCxPODVectorFirstLast();
	cat::testCxPODVectorDealloc();
	cat::testCxPODVectorEndsWith();
	cat::testCxPODVectorErase();
	cat::testCxPODVectorFill();
	cat::testCxPODVectorIndexOf();
	cat::testCxPODVectorInsert();
	cat::testCxPODVectorRemove();
	cat::testCxPODVectorRemoveAll();
	cat::testCxPODVectorRemoveAt();
	cat::testCxPODVectorRemoveFirstLast();
	cat::testCxPODVectorReplace();
	cat::testCxPODVectorReplaceAll();
	cat::testCxPODVectorReplaceAt();
	cat::testCxPODVectorReserve();
	cat::testCxPODVectorResize();
	cat::testCxPODVectorSort();
	cat::testCxPODVectorStartsWith();
	cat::testCxPODVectorSwap();
	cat::testCxPODVectorTakeAt();
	cat::testCxPODVectorTakeFirstLast();
	cat::testCxPODVectorISPTR();
	return 0;
}
