#ifndef BITSET_H
#define BITSET_H

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#include "../errors/errors.h"
#include "../output/output.h"

#define BIT_PER_BLOCK 64
#define MIN_BUFFER_SIZE 1024

typedef void (*outputFunc)(const char *);

typedef struct BitSet {
	uint64_t *bits;		 // Dynamic block of bits
	size_t size;		 // Number of blocks
	size_t capacity;	 // Maximum number of elements
} BitSet;

/*
  @brief Creates the structure of a biting set with a given size.

  This function initializes and returns a bit set with the specified capacity.
  If an error occurs during the creation of the bit set, it returns NULL.

  @param capacity The size (capacity) of the bit set to be created.
  @return A pointer to the created BitSet, or NULL if there was an error.
*/
BitSet createBitSet(size_t capacity);

/*
  @brief Removes the BitSet structure.

  This function frees the memory associated with the given BitSet and
  invalidates the provided pointer. After calling this function, the
  BitSet should not be used.

  @param bitSet A pointer to the BitSet to be destroyed.
                The pointer will be set to NULL after the function
                completes.
*/
void destroyBitSet(BitSet *bitSet);

/*
  @brief Adds a number to the BitSet if it is positive and permissible.

  This function attempts to add a number (element) to the given BitSet.
  The number will only be added if it is positive and permissible according
  to the rules of the BitSet. If the number is not valid (either non-positive
  or exceeding permissible limits), the function will skip adding it.

  @param bitSet A pointer to the BitSet to which the element will be added.
  @param element The number to be added to the BitSet.
                 It must be positive and within permissible range.

  @return A `BaseErrorCode` indicating the result of the operation:
           - `NONE_ERROR` if the element was successfully added.
           - An error code if the element could not be added
             (e.g., due to being negative or out of range).
*/
BaseErrorCode addBitSetElement(const BitSet *bitSet, uint64_t element);

/*
  @brief Adds several numbers in set if they are positive and permissible,
         otherwise passes them.

  This function attempts to add multiple elements to the provided BitSet.
  Each element is checked for validity (i.e., it must be positive and
  permissible based on some criteria) before it is added to the BitSet.
  If an element is not valid, it is ignored.

  @param bitSet A pointer to the BitSet structure where elements will be added.
  @param count The number of elements to be added to the BitSet.
  @param elements An array of elements to be added to the BitSet.

  @return A `BaseErrorCode` indicating the result of the operation:
           - `NONE_ERROR` if the element was successfully added.
           - An error code if the element could not be added
             (e.g., due to being negative or out of range).
*/
BaseErrorCode addManyBitSetElements(const BitSet *bitSet, size_t count,
									const uint64_t elements[]);

/*
  @brief Removes an element from the BitSet.

  This function attempts to remove the specified element from the
  given BitSet. If the element is found, it will be removed from the set.
  If the element does not exist in the set, the function will
  not modify the BitSet.

  @param bitSet A pointer to the BitSet from which the element
                will be removed.
  @param element The number to be removed from the BitSet.
                 If the element is not present, the BitSet
                 will remain unchanged.

  @return A `BaseErrorCode` indicating the result of the operation:
          - `NONE_ERROR` if the element was successfully removed.
          - An error code if the element could not be removed.
*/
BaseErrorCode removeBitSetElement(const BitSet *bitSet, uint64_t element);

/*
 @brief Checks if there is an element in the BitSet.

  This function checks whether the specified element is present
  in the given BitSet. It returns `true` if the element is found
  in the set, and `false` otherwise.

  @param bitSet A pointer to the BitSet to be checked.
  @param element The element to check for in the BitSet.

  @return `true` if the element is present in the BitSet, `false` otherwise.
*/
bool isBitSetContains(const BitSet *bitSet, uint64_t element);

/**
  @brief Checks whether two BitSets are equal.

  This function compares two BitSets and checks if they contain
  the same elements. It returns `true` if both sets are equal,
  and `false` if they are not. The sets are considered equal if they
  have the same size and contain the same elements.

  @param bitSet1 A pointer to the first BitSet to be compared.
  @param bitSet2 A pointer to the second BitSet to be compared.

  @return `true` if the BitSets are equal, `false` otherwise.
*/
bool isBitSetsEqual(const BitSet *, const BitSet *);

/*
  @brief Checks whether setA is a subset of setB.

  This function checks if all elements of `bitSetA` are also present
  in `bitSetB`. The function returns `true` if `bitSetA` is a subset
  of `bitSetB`, and `false` otherwise. A set `A` is considered a subset
  of set `B` if every element of `A` is also contained in `B`.

  @param bitSetA A pointer to the first BitSet (setA) to be checked.
  @param bitSetB A pointer to the second BitSet (setB) to check against.

  @return `true` if `bitSetA` is a subset of `bitSetB`, `false` otherwise.
*/
bool isSubset(const BitSet *bitSetA, const BitSet *bitSetB);

/*
  @brief Checks whether setA is a strict subset of setB.

  This function checks if all elements of `bitSetA` are also present
  in `bitSetB`, and that `bitSetA` is not equal to `bitSetB`. In other words,
  `bitSetA` is a strict subset of `bitSetB` if every element of `bitSetA` is
  in `bitSetB`, and `bitSetA` contains fewer elements than `bitSetB`.

  @param bitSetA A pointer to the first BitSet (setA) to be checked.
  @param bitSetB A pointer to the second BitSet (setB) to check against.

  @return `true` if `bitSetA` is a strict subset of `bitSetB`, `false` otherwise.
*/
bool isStrictSubset(const BitSet *bitSetA, const BitSet *bitSetB);

/*
  @brief Returns the maximum capacity among two BitSets.

  This function compares the capacities of two BitSets and returns
  the greater of the two. The capacity of a BitSet is the maximum
  number of elements it can hold.

  @param bitSetA A pointer to the first BitSet.
  @param bitSetB A pointer to the second BitSet.

  @return The maximum capacity of the two BitSets.
*/
size_t getMaxBitSetCapacity(const BitSet *bitSetA, const BitSet *bitSetB);

/*
  @brief Creates a set with the meaning A ∪ B.

  This function calculates the union of two bit sets. The union
  of sets A and B is a set that contains all the elements that
  are in either A or B or in both.

  @param setA A pointer to the first bit set.
  @param setB A pointer to the second bit set.

  @return A new BitSet representing the union of setA and setB.
*/
BitSet getBitSetsUnion(const BitSet *, const BitSet *);

/*
  @brief Creates a set with the meaning A ∩ B.

  This function calculates the intersection of two bit sets.
  The intersection of sets A and B is a set that contains only
  the elements that are present in both A and B.

  @param setA A pointer to the first bit set.
  @param setB A pointer to the second bit set.

  @return A new BitSet representing the intersection of setA and setB.
*/
BitSet getBitSetsIntersection(const BitSet *, const BitSet *);

/*
  @brief Creates a set with the meaning A - B.

  This function calculates the difference of two bit sets.
  The difference of sets A and B is a set that contains all
  the elements that are in A but not in B.

  @param bitSetA A pointer to the first bit set.
  @param bitSetB A pointer to the second bit set.

  @return A new BitSet representing the difference of bitSetA and bitSetB.
*/
BitSet getBitSetsDiff(const BitSet *bitSetA, const BitSet *bitSetB);

/*
  @brief Creates a set with the meaning A △ B.

  This function calculates the symmetric difference of two bit sets.
  The symmetric difference of sets A and B is a set that contains
  all the elements that are in either A or B, but not in both.

  @param bitSetA A pointer to the first bit set.
  @param bitSetB A pointer to the second bit set.

  @return A new BitSet representing the symmetric difference
          of bitSetA and bitSetB.
*/
BitSet getSymmetricBitSetsDiff(const BitSet *bitSetA, const BitSet *bitSetB);

/*
  @brief Creates a set, the meaning of which is the complement of this set.

  This function calculates the complement of a bit set. The complement of
  a set A contains all the elements that are not in A. In terms of bits,
  it flips all the bits of the given bit set.

  @param bitSet A pointer to the bit set for which the complement
                will be calculated.

  @return A new BitSet representing the complement of the provided bit set.
*/
BitSet getBitSetComplement(const BitSet *);

/*
  @brief Displays a set to the function for showing.

  This function displays the contents of a bit set using the provided
  output function. The bit set is represented in a specific format,
  and the output function is responsible for displaying the data
  (e.g., printing to console, writing to a file, etc.).

  @param bitSet A pointer to the bit set that needs to be displayed.
  @param output A function pointer that will handle the actual output
                (e.g., printing to the console).

  @return A `BaseErrorCode` indicating the result of the operation:
            - `NONE_ERROR` if the bit set was successfully displayed.
            - An error code if the bit set could not be displayed.
*/
BaseErrorCode printBitSet(const BitSet *bitSet, outputFunc output);

#endif