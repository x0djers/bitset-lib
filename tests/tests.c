#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <time.h>

#include "../src/bitset/bitset.h"
#include "../src/errors/errors.h"
#include "../src/output/output.h"


void testAdd()
{
	BitSet set = createBitSet(10);

	bool isAllAdded = true;

	addBitSetElement(&set, 1);
	addBitSetElement(&set, 5);
	addBitSetElement(&set, 8);

	isAllAdded &= isBitSetContains(&set, 1);
	isAllAdded &= isBitSetContains(&set, 5);
	isAllAdded &= isBitSetContains(&set, 8);

	assertWithMessage(isAllAdded, getTestErrorMessage(ADD_TEST_ERROR));

	destroyBitSet(&set);
}

void testAddMany()
{
	BitSet set = createBitSet(10);

	bool isAllAdded = true;

	uint64_t elements[3] = {1, 5, 8};
	addManyBitSetElements(&set, 3, elements);

	isAllAdded &= isBitSetContains(&set, 1);
	isAllAdded &= isBitSetContains(&set, 5);
	isAllAdded &= isBitSetContains(&set, 8);

	assertWithMessage(isAllAdded, getTestErrorMessage(ADD_MANY_TEST_ERROR));

	destroyBitSet(&set);
}

void testBoundary()
{
	BitSet set = createBitSet(64);

	bool isAllAdded = true;

	addBitSetElement(&set, 0);
	addBitSetElement(&set, 64);

	assertWithMessage(isAllAdded, getTestErrorMessage(BOUNDARY_TEST_ERROR));

	destroyBitSet(&set);
}

void testRemove()
{
	BitSet set = createBitSet(10);

	bool isAllRemoved = true;

	addBitSetElement(&set, 1);
	addBitSetElement(&set, 5);
	addBitSetElement(&set, 8);

	removeBitSetElement(&set, 1);
	removeBitSetElement(&set, 5);
	removeBitSetElement(&set, 8);

	isAllRemoved &= !isBitSetContains(&set, 1);
	isAllRemoved &= !isBitSetContains(&set, 5);
	isAllRemoved &= !isBitSetContains(&set, 8);

	assertWithMessage(isAllRemoved, getTestErrorMessage(REMOVE_TEST_ERROR));

	destroyBitSet(&set);
}

void testPerformance()
{
	const size_t N = 1000000;
	BitSet set = createBitSet(N);

	clock_t start = clock();

	for (size_t iter = 0; iter < N; iter++) addBitSetElement(&set, iter);

	clock_t end = clock();
	printf("Adding %zu elements took %.3f seconds.\n", N,
		   (double)(end - start) / CLOCKS_PER_SEC);

	destroyBitSet(&set);
}

// Test with valgrind
void testMemoryLeak()
{
	const size_t N = 1000;
	BitSet set = createBitSet(N);

	for (size_t iter = 0; iter < N; iter++) addBitSetElement(&set, iter);

	destroyBitSet(&set);
}

void testSubset()
{
	size_t smallerSize = 500;
	size_t biggerSize  = 1000;

	{
		BitSet smallerSet = createBitSet(smallerSize);
		BitSet biggerSet  = createBitSet(biggerSize);

		uint64_t values[10] = {0, 1, 5, 15, 50, 60, 252, 250};

		addManyBitSetElements(&smallerSet, 10, values);
		addManyBitSetElements(&biggerSet, 10, values);

		assertWithMessage(isSubset(&smallerSet, &biggerSet),
						  getTestErrorMessage(SUBSET_TEST_ERROR));

		destroyBitSet(&smallerSet);
		destroyBitSet(&biggerSet);
	}

	{
		BitSet smallerSet = createBitSet(smallerSize);
		BitSet biggerSet  = createBitSet(biggerSize);

		uint64_t smallerValues[5] = {0, 1, 2, 4, 8};
		uint64_t biggerValues[10] = {0, 1, 2, 4, 8, 16, 32, 64, 128, 256};

		addManyBitSetElements(&smallerSet, 5, smallerValues);
		addManyBitSetElements(&biggerSet, 10, biggerValues);

		assertWithMessage(isSubset(&smallerSet, &biggerSet),
						  getTestErrorMessage(SUBSET_TEST_ERROR));

		destroyBitSet(&smallerSet);
		destroyBitSet(&biggerSet);
	}
}

void testStrictSubset()
{
	size_t size = 1000;

	{
		BitSet smallerSet = createBitSet(size);
		BitSet biggerSet  = createBitSet(size);

		uint64_t values[13] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};

		addManyBitSetElements(&smallerSet, 13, values);
		addManyBitSetElements(&biggerSet, 13, values);

		assertWithMessage(!isStrictSubset(&smallerSet, &biggerSet),
						  getTestErrorMessage(STRICT_SUBSET_TEST_ERROR));

		destroyBitSet(&smallerSet);
		destroyBitSet(&biggerSet);
	}

	{
		BitSet smallerSet = createBitSet(size);
		BitSet biggerSet  = createBitSet(size);

		uint64_t smallerValues[5] = {2, 4, 6, 8, 10};
		uint64_t biggerValues[10] = {1, 3, 5, 7, 9, 11, 13, 15, 17, 19};

		addManyBitSetElements(&smallerSet, 5, smallerValues);
		addManyBitSetElements(&biggerSet, 10, biggerValues);

		assertWithMessage(!isStrictSubset(&smallerSet, &biggerSet),
						  getTestErrorMessage(STRICT_SUBSET_TEST_ERROR));

		destroyBitSet(&smallerSet);
		destroyBitSet(&biggerSet);
	}
}

void testUnion()
{
	const size_t N = 250;

	{
		BitSet set1 = createBitSet(N);
		BitSet set2	= createBitSet(N);
		BitSet expectedSet = createBitSet(N);

		for (size_t iter = 0; iter < N; iter += 2) {
			addBitSetElement(&set1, iter);
		}
		for (size_t iter = 1; iter < N; iter += 2) {
			addBitSetElement(&set2, iter);
		}
		for (size_t iter = 0; iter < N; iter++) {
			addBitSetElement(&expectedSet, iter);
		}

		BitSet result = getBitSetsUnion(&set1, &set2);

		assertWithMessage(isBitSetsEqual(&result, &expectedSet),
						  getTestErrorMessage(UNION_TEST_ERROR));

		destroyBitSet(&set1);
		destroyBitSet(&set2);
		destroyBitSet(&result);
		destroyBitSet(&expectedSet);
	}

	{
		BitSet set1	= createBitSet(N);
		BitSet set2	= createBitSet(N);
		BitSet expectedSet = createBitSet(N);

		uint64_t values[10] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};

		addManyBitSetElements(&set1, 10, values);
		addManyBitSetElements(&set2, 10, values);
		addManyBitSetElements(&expectedSet, 10, values);

		BitSet result = getBitSetsUnion(&set1, &set2);

		assertWithMessage(isBitSetsEqual(&result, &expectedSet),
						  getTestErrorMessage(UNION_TEST_ERROR));

		destroyBitSet(&set1);
		destroyBitSet(&set2);
		destroyBitSet(&result);
		destroyBitSet(&expectedSet);
	}

	{
		BitSet set1	= createBitSet(N);
		BitSet set2	= createBitSet(N);
		BitSet expectedSet = createBitSet(N);

		uint64_t values1[7]	= {0, 37, 50, 70, 124, 250, 300};
		uint64_t values2[5]	= {0, 1, 5, 58, 300};
		uint64_t expectedValues[10] = {0, 1, 5, 37, 50, 58, 70, 124, 250, 300};

		addManyBitSetElements(&set1, 7, values1);
		addManyBitSetElements(&set2, 5, values2);
		addManyBitSetElements(&expectedSet, 10, expectedValues);

		BitSet result = getBitSetsUnion(&set1, &set2);

		assertWithMessage(isBitSetsEqual(&result, &expectedSet),
						  getTestErrorMessage(UNION_TEST_ERROR));

		destroyBitSet(&set1);
		destroyBitSet(&set2);
		destroyBitSet(&result);
		destroyBitSet(&expectedSet);
	}
}

void testIntersection()
{
	const size_t N = 250;

	{
		BitSet set1	= createBitSet(N);
		BitSet set2	= createBitSet(N);
		BitSet expectedSet = createBitSet(N);

		for (size_t iter = 0; iter < N; iter += 2) {
			addBitSetElement(&set1, iter);
		}
		for (size_t iter = 1; iter < N; iter += 2) {
			addBitSetElement(&set2, iter);
		}

		BitSet result = getBitSetsIntersection(&set1, &set2);

		assertWithMessage(isBitSetsEqual(&result, &expectedSet),
						  getTestErrorMessage(INTERSECTION_TEST_ERROR));

		destroyBitSet(&set1);
		destroyBitSet(&set2);
		destroyBitSet(&result);
		destroyBitSet(&expectedSet);
	}

	{
		BitSet set1	= createBitSet(N);
		BitSet set2	= createBitSet(N);
		BitSet expectedSet = createBitSet(N);

		uint64_t values[10] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};

		addManyBitSetElements(&set1, 10, values);
		addManyBitSetElements(&set2, 10, values);
		addManyBitSetElements(&expectedSet, 10, values);

		BitSet result = getBitSetsIntersection(&set1, &set2);

		assertWithMessage(isBitSetsEqual(&result, &expectedSet),
						  getTestErrorMessage(INTERSECTION_TEST_ERROR));

		destroyBitSet(&set1);
		destroyBitSet(&set2);
		destroyBitSet(&result);
		destroyBitSet(&expectedSet);
	}

	{
		BitSet set1	= createBitSet(N);
		BitSet set2	= createBitSet(N);
		BitSet expectedSet = createBitSet(N);

		uint64_t values1[7]	= {0, 37, 50, 70, 124, 250, 300};
		uint64_t values2[5]	= {0, 1, 5, 58, 300};
		uint64_t expectedValues[1] = {0};

		addManyBitSetElements(&set1, 7, values1);
		addManyBitSetElements(&set2, 5, values2);
		addManyBitSetElements(&expectedSet, 1, expectedValues);

		BitSet result = getBitSetsIntersection(&set1, &set2);

		assertWithMessage(isBitSetsEqual(&result, &expectedSet),
						  getTestErrorMessage(INTERSECTION_TEST_ERROR));

		destroyBitSet(&set1);
		destroyBitSet(&set2);
		destroyBitSet(&result);
		destroyBitSet(&expectedSet);
	}
}

void testDiff()
{
	size_t setSize = 1000;

	{
		BitSet set1	= createBitSet(setSize);
		BitSet set2	= createBitSet(setSize);
		BitSet expectedSet = createBitSet(setSize);

		uint64_t values1[6]	= {1, 2, 3, 4, 5, 6};
		uint64_t values2[6]	= {5, 6, 8, 10, 12, 14};
		uint64_t expectedValues[4] = {1, 2, 3, 4};

		addManyBitSetElements(&set1, 6, values1);
		addManyBitSetElements(&set2, 6, values2);
		addManyBitSetElements(&expectedSet, 4, expectedValues);

		BitSet result = getBitSetsDiff(&set1, &set2);

		assertWithMessage(isBitSetsEqual(&expectedSet, &result),
						  getTestErrorMessage(DIFFERENCE_TEST_ERROR));

		destroyBitSet(&set1);
		destroyBitSet(&set2);
		destroyBitSet(&result);
		destroyBitSet(&expectedSet);
	}

	{
		BitSet set1	= createBitSet(setSize);
		BitSet set2	= createBitSet(setSize);
		BitSet expectedSet = createBitSet(setSize);

		uint64_t values1[6] = {0, 1, 2, 3, 4, 5};
		uint64_t values2[6] = {0, 1, 2, 3, 4, 5};

		addManyBitSetElements(&set1, 6, values1);
		addManyBitSetElements(&set2, 6, values2);

		BitSet result = getBitSetsDiff(&set1, &set2);

		assertWithMessage(isBitSetsEqual(&result, &expectedSet),
						  getTestErrorMessage(DIFFERENCE_TEST_ERROR));

		destroyBitSet(&set1);
		destroyBitSet(&set2);
		destroyBitSet(&result);
		destroyBitSet(&expectedSet);
	}

	{
		BitSet set1	= createBitSet(setSize);
		BitSet set2	= createBitSet(setSize);
		BitSet expectedSet = createBitSet(setSize);

		uint64_t values1[6]	= {0, 1, 2, 3, 4, 5};
		uint64_t expectedValues[6] = {0, 1, 2, 3, 4, 5};

		addManyBitSetElements(&set1, 6, values1);
		addManyBitSetElements(&expectedSet, 6, expectedValues);

		BitSet result = getBitSetsDiff(&set1, &set2);

		assertWithMessage(isBitSetsEqual(&result, &expectedSet),
						  getTestErrorMessage(DIFFERENCE_TEST_ERROR));

		destroyBitSet(&set1);
		destroyBitSet(&set2);
		destroyBitSet(&result);
		destroyBitSet(&expectedSet);
	}
}

void testSymmetricDiff()
{
	size_t setSize = 1000;

	{
		BitSet set1	= createBitSet(setSize);
		BitSet set2	= createBitSet(setSize);
		BitSet expectedSet = createBitSet(setSize);

		uint64_t values1[6]	= {1, 6, 14, 24, 33, 75};
		uint64_t values2[6]	= {14, 24, 33, 42, 51, 60};
		uint64_t expectedValues[6] = {1, 6, 42, 51, 60, 75};

		addManyBitSetElements(&set1, 6, values1);
		addManyBitSetElements(&set2, 6, values2);
		addManyBitSetElements(&expectedSet, 6, expectedValues);

		BitSet result = getSymmetricBitSetsDiff(&set1, &set2);

		assertWithMessage(
			isBitSetsEqual(&result, &expectedSet),
			getTestErrorMessage(SYMMETRIC_DIFFERENCE_TEST_ERROR));

		destroyBitSet(&set1);
		destroyBitSet(&set2);
		destroyBitSet(&result);
		destroyBitSet(&expectedSet);
	}

	{
		BitSet set1	= createBitSet(setSize);
		BitSet set2	= createBitSet(setSize);
		BitSet expectedSet = createBitSet(setSize);

		uint64_t values1[5] = {3, 9, 15, 21, 27};
		uint64_t values2[5] = {3, 9, 15, 21, 27};

		addManyBitSetElements(&set1, 5, values1);
		addManyBitSetElements(&set2, 5, values2);

		BitSet result = getSymmetricBitSetsDiff(&set1, &set2);

		assertWithMessage(
			isBitSetsEqual(&result, &expectedSet),
			getTestErrorMessage(SYMMETRIC_DIFFERENCE_TEST_ERROR));

		destroyBitSet(&set1);
		destroyBitSet(&set2);
		destroyBitSet(&result);
		destroyBitSet(&expectedSet);
	}

	{
		BitSet set1	= createBitSet(setSize);
		BitSet set2	= createBitSet(setSize);
		BitSet expectedSet = createBitSet(setSize);

		uint64_t values1[5]	= {2, 5, 8, 11, 14};
		uint64_t values2[5] = {4, 7, 10, 13, 16};
		uint64_t expectedValues[10] = {2, 4, 5, 7, 8, 10, 11, 13, 14, 16};

		addManyBitSetElements(&set1, 5, values1);
		addManyBitSetElements(&set2, 5, values2);
		addManyBitSetElements(&expectedSet, 10, expectedValues);

		BitSet result = getSymmetricBitSetsDiff(&set1, &set2);

		assertWithMessage(
			isBitSetsEqual(&result, &expectedSet),
			getTestErrorMessage(SYMMETRIC_DIFFERENCE_TEST_ERROR));

		destroyBitSet(&set1);
		destroyBitSet(&set2);
		destroyBitSet(&result);
		destroyBitSet(&expectedSet);
	}

	{
		BitSet set1	= createBitSet(setSize);
		BitSet set2	= createBitSet(setSize);
		BitSet expectedSet = createBitSet(setSize);

		uint64_t values1[5]	= {10, 20, 30, 40, 50};
		uint64_t expectedValues[5] = {10, 20, 30, 40, 50};

		addManyBitSetElements(&set1, 5, values1);
		addManyBitSetElements(&expectedSet, 5, expectedValues);

		BitSet result = getSymmetricBitSetsDiff(&set1, &set2);

		assertWithMessage(
			isBitSetsEqual(&result, &expectedSet),
			getTestErrorMessage(SYMMETRIC_DIFFERENCE_TEST_ERROR));

		destroyBitSet(&set1);
		destroyBitSet(&set2);
		destroyBitSet(&result);
		destroyBitSet(&expectedSet);
	}
}

int main()
{
	testBoundary();
	testAdd();
	testAddMany();
	testRemove();
	testPerformance();
	testMemoryLeak();
	testSubset();
	testStrictSubset();
	testUnion();
	testIntersection();
	testDiff();
	testSymmetricDiff();

	printf("All tests passed!\n");

	return 0;
}