#include "bitset.h"

#include "../errors/errors.h"

BitSet createBitSet(const size_t capacity) {
	BitSet bitSet;

	bitSet.capacity = capacity;
	bitSet.size = (capacity + 63) >> 6;

	bitSet.bits = (uint64_t *)calloc(bitSet.size, sizeof(uint64_t));

	if (bitSet.bits == NULL) {
		free(bitSet.bits);
		bitSet.bits	= NULL;
		bitSet.capacity = 0;
	}

	return bitSet;
}

void destroyBitSet(BitSet *bitSet) {
	bitSet->size = 0;
	bitSet->capacity = 0;
	free(bitSet->bits);
	bitSet->bits = NULL;
}

BaseErrorCode checkElementValidity(const BitSet	 *bitSet,
								   const uint64_t element) {
	BaseErrorCode validityStatus = NONE_ERROR;
	if (element > (uint64_t)bitSet->capacity) {
		validityStatus = CAPACITY_EXCEEDING_ERROR;
	}

	return validityStatus;
}

BaseErrorCode addBitSetElement(const BitSet *bitSet, const uint64_t element) {
	BaseErrorCode statusCode = NONE_ERROR;

	statusCode = checkElementValidity(bitSet, element);

	if (statusCode == NONE_ERROR) {
		const uint64_t blockPosition = element / BIT_PER_BLOCK;
		const uint64_t bitOffset = BIT_PER_BLOCK - element % BIT_PER_BLOCK - 1;
		bitSet->bits[blockPosition] |= 1ULL << bitOffset;
	}

	return statusCode;
}

BaseErrorCode addManyBitSetElements(const BitSet *bitSet, const size_t count,
									const uint64_t elements[]) {
	BaseErrorCode statusCode = NONE_ERROR;

	for (size_t iter = 0; iter < count; iter++) {
		statusCode = addBitSetElement(bitSet, elements[iter]);
	}

	return statusCode;
}

BaseErrorCode removeBitSetElement(const BitSet *bitSet, const uint64_t element) {
	BaseErrorCode statusCode = NONE_ERROR;

	statusCode = checkElementValidity(bitSet, element);

	if (statusCode == NONE_ERROR) {
		const uint64_t blockPosition = element / BIT_PER_BLOCK;
		const uint64_t bitOffset = BIT_PER_BLOCK - element % BIT_PER_BLOCK - 1;
		bitSet->bits[blockPosition] &= ~(1ULL << bitOffset);
	}

	return statusCode;
}

bool isBitSetContains(const BitSet *bitSet, const uint64_t element) {
	bool isContains = false;
	const BaseErrorCode statusCode = checkElementValidity(bitSet, element);

	if (statusCode == NONE_ERROR) {
		const uint64_t blockPosition = element / BIT_PER_BLOCK;
		const uint64_t bitOffset = BIT_PER_BLOCK - element % BIT_PER_BLOCK - 1;

		isContains = bitSet->bits[blockPosition] >> bitOffset & 1;
	}
	return isContains;
}

bool isBitSetsEqual(const BitSet *bitSet1, const BitSet *bitSet2) {
	bool isEquals = true;
	if (bitSet1->capacity != bitSet2->capacity ||
		bitSet1->size != bitSet2->size) {
		isEquals = false;
	} else {
		const size_t setLength = bitSet1->size;
		for (size_t blockPos = 0; blockPos < setLength; blockPos++)
			isEquals = bitSet1->bits[blockPos] == bitSet2->bits[blockPos];
	}

	return isEquals;
}

bool isSubset(const BitSet *bitSetA, const BitSet *bitSetB) {
	bool isSubSet = true;

	isSubSet = bitSetA->size <= bitSetB->size;

	for (size_t iter = 0; iter < bitSetA->capacity && isSubSet; iter++) {
		const size_t blockPos = iter / BIT_PER_BLOCK;
		const size_t bitOffset = BIT_PER_BLOCK - iter % BIT_PER_BLOCK - 1;
		const bool numberInA = bitSetA->bits[blockPos] >> bitOffset & 1;
		const bool numberInB = bitSetB->bits[blockPos] >> bitOffset & 1;

		isSubSet = !(numberInA && !numberInB);
	}
	return isSubSet;
}

bool isStrictSubset(const BitSet *bitSetA, const BitSet *bitSetB) {
	const bool isStrictSubset =
		isSubset(bitSetA, bitSetB) && !isBitSetsEqual(bitSetA, bitSetB);
	return isStrictSubset;
}

size_t getMaxBitSetCapacity(const BitSet *bitSetA, const BitSet *bitSetB) {
	return bitSetA->capacity > bitSetB->capacity ? bitSetA->capacity
												 : bitSetB->capacity;
}

BitSet getBitSetsUnion(const BitSet *bitSetA, const BitSet *bitSetB) {
	const size_t maxCapacity = getMaxBitSetCapacity(bitSetA, bitSetB);

	const BitSet resultBitSet = createBitSet(maxCapacity);

	for (size_t iter = 0; iter < resultBitSet.size; iter++) {
		uint64_t blockInA = iter < bitSetA->size ? bitSetA->bits[iter] : 0;
		uint64_t blockInB = iter < bitSetB->size ? bitSetB->bits[iter] : 0;
		resultBitSet.bits[iter] = blockInA | blockInB;
	}

	return resultBitSet;
}

BitSet getBitSetsIntersection(const BitSet *bitSetA, const BitSet *bitSetB) {
	const size_t maxCapacity = getMaxBitSetCapacity(bitSetA, bitSetB);

	const BitSet resultBitSet = createBitSet(maxCapacity);

	for (size_t iter = 0; iter < resultBitSet.size; iter++) {
		uint64_t blockInA = iter < bitSetA->size ? bitSetA->bits[iter] : 0;
		uint64_t blockInB = iter < bitSetB->size ? bitSetB->bits[iter] : 0;
		resultBitSet.bits[iter] = blockInA & blockInB;
	}

	return resultBitSet;
}

BitSet getBitSetsDiff(const BitSet *bitSetA, const BitSet *bitSetB) {
	const BitSet resultBitSet = createBitSet(bitSetA->capacity);

	for (size_t iter = 0; iter < resultBitSet.size; iter++) {
		uint64_t blockInA = iter < bitSetA->size ? bitSetA->bits[iter] : 0;
		uint64_t blockInB = iter < bitSetB->size ? bitSetB->bits[iter] : 0;
		resultBitSet.bits[iter] = blockInA & ~blockInB;
	}

	return resultBitSet;
}

BitSet getSymmetricBitSetsDiff(const BitSet *bitSetA, const BitSet *bitSetB) {
	const size_t maxCapacity = getMaxBitSetCapacity(bitSetA, bitSetB);

	const BitSet resultBitSet = createBitSet(maxCapacity);

	for (size_t iter = 0; iter < resultBitSet.size; iter++) {
		uint64_t blockInA = iter < bitSetA->size ? bitSetA->bits[iter] : 0;
		uint64_t blockInB = iter < bitSetB->size ? bitSetB->bits[iter] : 0;
		resultBitSet.bits[iter] = blockInA ^ blockInB;
	}

	return resultBitSet;
}

BitSet getBitSetComplement(const BitSet *bitSet) {
	const BitSet resultBitSet = createBitSet(bitSet->capacity);

	for (size_t iter = 0; iter < resultBitSet.size; ++iter) {
		resultBitSet.bits[iter] = ~bitSet->bits[iter];
	}

	const size_t totalBits	= resultBitSet.size * BIT_PER_BLOCK;
	const size_t excessBits = totalBits - resultBitSet.capacity;
	if (excessBits > 0) {
		const uint64_t mask = ~(1ULL << (BIT_PER_BLOCK - excessBits - 1));
		resultBitSet.bits[resultBitSet.size - 1] &= mask;
	}
	return resultBitSet;
}

BaseErrorCode printBitSet(const BitSet *bitSet, const outputFunc output) {
	BaseErrorCode statusCode = NONE_ERROR;
	size_t currentBufferSize = 0;
	size_t maxBufferSize = MIN_BUFFER_SIZE;

	char *buffer = malloc(maxBufferSize);
	if (buffer == NULL) {
		statusCode = MEMORY_ALLOCATION_ERROR;
	}

	for (uint64_t number = 0; number <= (uint64_t)bitSet->capacity; number++) {
		if (isBitSetContains(bitSet, number)) {
			const uint64_t requiredSize = snprintf(NULL, 0, "%ld", number);
			if (currentBufferSize + requiredSize >= maxBufferSize) {
				maxBufferSize *= 2;
				buffer = realloc(buffer, maxBufferSize);
				if (buffer == NULL) {
					statusCode = MEMORY_ALLOCATION_ERROR;
				}
			}

			if (!statusCode) {
				currentBufferSize += snprintf(
					buffer + currentBufferSize,
					maxBufferSize - currentBufferSize, "%ld, ", number);
			}
		}
	}

	buffer = realloc(buffer, currentBufferSize);
	if (buffer == NULL) {
		statusCode = MEMORY_ALLOCATION_ERROR;
	}

	if (!statusCode) {
		buffer[currentBufferSize - 2] = '\0';
		output(buffer);
	}

	free(buffer);
	buffer = NULL;

	return statusCode;
}
