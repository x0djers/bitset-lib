#include "errors.h"

char *getBaseErrorMessage(const BaseErrorCode code)
{
	char *message;
	switch (code) {
		case MEMORY_ALLOCATION_ERROR:
			message = "Error: dynamic memory is not allocated.";
			break;
		case CAPACITY_EXCEEDING_ERROR:
			message = "Error: capacity of the set is exceeded.";
			break;
		case NEGATIVE_NUMBER_ERROR:
			message = "Error: working with negative numbers is impossible.";
			break;
		default:
			message = "Error: unknown error.";
	}

	return message;
}

char *getTestErrorMessage(const TestErrorCode code)
{
	char *message;
	switch (code) {
		case BOUNDARY_TEST_ERROR:
			message =
				"BoundaryTest failed. "
				"Error: boundary elements not added.";
			break;
		case ADD_TEST_ERROR:
			message =
				"AddTest failed. "
				"Error: number is not added to set.";
			break;
		case ADD_MANY_TEST_ERROR:
			message =
				"AddManyTest failed. "
				"Error: numbers is not added to set.";
			break;
		case REMOVE_TEST_ERROR:
			message =
				"RemoveTest failed. "
				"Error: number is not removed from set.";
			break;
		case SUBSET_TEST_ERROR:
			message =
				"SubsetTest failed. "
				"Error: definition of subset is incorrect.";
			break;
		case STRICT_SUBSET_TEST_ERROR:
			message =
				"StrictSubsetTest failed. "
				"Error: set is not strict subset.";
			break;
		case UNION_TEST_ERROR:
			message =
				"UnionTest failed. "
				"Error: union of sets is incorrect.";
			break;
		case INTERSECTION_TEST_ERROR:
			message =
				"IntersectionTest failed. "
				"Error: intersection of sets is incorrect.";
			break;
		case DIFFERENCE_TEST_ERROR:
			message =
				"DifferenceTest failed. "
				"Error: difference of sets is incorrect.";
			break;
		case SYMMETRIC_DIFFERENCE_TEST_ERROR:
			message =
				"SymmetricDifferenceTest failed. "
				"Error: symmetric difference of sets is incorrect.";
			break;
		case COMPLEMENT_TEST_ERROR:
			message =
				"ComplementTest failed. "
				"Error: set is not a complement.";
			break;
		default:
			message = "Error: unknown error";
	}

	return message;
}
