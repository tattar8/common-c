/**
 * @file
 * This file contains an implementation of a statically allocated, fixed-max-length array
 * holding elements of a single datatype (and size). The array itself does not hold
 * any data, rather, it stores the data in an external buffer supplied during
 * initialization.
 *
 * @note This object should work across all architecture-specific implementations
 *
 *
 * #### Example Usage: ####
 *
 * @code
 * cxa_array_t myArray;
 * uint16_t myArray_buffer[16];			// where the data is actually stored
 *
 * // initialize the array with an element type of uint16 (2 bytes), storing a maximum of 16 elements
 * // note the subtle differences:
 * // sizeof(*myArray_buffer)      2 bytes, the size of each element in the array
 * // sizeof(myArray_buffer)       32 bytes, 16 elements of type uint16 (2 bytes)
 * cxa_array_init(&myArray, sizeof(*myArray_buffer), (void*)myArray_buffer, sizeof(myArrayBuffer));
 * // OR more simply:
 * cxa_array_initStd(&myArray, myArray_buffer);
 *
 * ...
 *
 * // add a new value to the array
 * uint16_t newVal = 1234;
 * cxa_array_append(&myArray, (void*)&newVal);
 *
 * ...
 *
 * // see how many elements are in the array (should be 1 at this point)
 * size_t numElems = cxa_array_getSize_elems(&myArray);
 *
 * // now access that element (which currently exists in the array)
 * uint16_t *addedElem = (uint16_t*)cxa_array_get(&myArray, 0);
 * @endcode
 *
 *
 * @copyright 2015 opencxa.org
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * @author Christopher Armenio
 */
#ifndef CXA_ARRAY_H_
#define CXA_ARRAY_H_


// ******** includes ********
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#include <cxa_config.h>


// ******** global macro definitions ********
/**
 * @public
 * @brief Shortcut to initialize an array with a buffer of an explicit data type
 *
 * @code
 * cxa_array_t myArray;
 * double myBuffer[100];
 *
 * cxa_array_initStd(&myArray, myBuffer);
 * // equivalent to
 * cxa_array_init(&myArray, sizeof(*myBuffer), (void)myBuffer, sizeof(myBuffer));
 * @endcode
 *
 * @param[in] arrIn pointer to the array to initialize
 * @param[in] bufferIn pointer to the declared c-style array which
 * 		will contain the data for the array
 */
#define cxa_array_initStd(arrIn, bufferIn)						cxa_array_init((arrIn), ((bufferIn) == NULL) ? 0 : sizeof(*(bufferIn)), ((void*)(bufferIn)), ((bufferIn) == NULL) ? 0 : sizeof(bufferIn))


/**
 * @public
 * @brief Shortcut to iterate over all items in an array
 *
 * @code
 * cxa_array_t myArray;
 * double myBuffer[100];
 * cxa_array_initStd(&myArray, myBuffer);
 *
 * // add items to the array
 *
 * cxa_array_iterate(&myArray, currItem, double)
 * {
 * 		printf("currItem: %lf\n", *currItem);
 * }
 * @endcode
 *
 * @param[in] arrIn pointer to the pre-initialized array
 * @param[in] varNameIn name of the variable which will be initialized
 * 		by the loop. For each iteration, this variable will be a pointer
 * 		to the current array element of the datatype 'elemTypeIn'
 * @param[in] elemTypeIn the base datatype of each element (not their pointer)
 */
#define cxa_array_iterate(arrIn, varNameIn, elemTypeIn)			for(elemTypeIn* (varNameIn) = ((elemTypeIn*)((arrIn)->bufferLoc));		\
																    (varNameIn) < (((arrIn)->bufferLoc == NULL) ? (elemTypeIn*)NULL : (elemTypeIn*)&(((uint8_t*)((arrIn)->bufferLoc))[((arrIn)->insertIndex) * ((arrIn)->datatypeSize_bytes)]));	\
																    (varNameIn)++)


// ******** global type definitions *********
/**
 * @public
 * @brief "Forward" declaration of the cxa_array_t object
 */
typedef struct cxa_array cxa_array_t;


/**
 * @private
 */
struct cxa_array
{
	void *bufferLoc;
	size_t insertIndex;

	size_t datatypeSize_bytes;
	size_t maxNumElements;
};


// ******** global function prototypes ********
/**
 * @public
 * @brief Initializes the array using the specified buffer (which is empty) to store elements.
 *
 * @param[in] arrIn pointer to the pre-allocated cxa_array_t object
 * @param[in] datatypeSize_bytesIn the size of each element that will be inserted
 * 		into the array (all elements MUST be the same size)
 * @param[in] bufferLocIn pointer to the pre-allocated chunk of memory that will
 * 		be used to store elements in the array (the buffer)
 * @param[in] bufferMaxSize_bytesIn the maximum size of the chunk of memory (buffer) in bytes
 */
void cxa_array_init(cxa_array_t *const arrIn, const size_t datatypeSize_bytesIn, void *const bufferLocIn, const size_t bufferMaxSize_bytesIn);


/**
 * @public
 * @brief Initializes the array using the specified buffer (which already contains elements) to store elements.
 *
 * @param[in] arrIn pointer to the pre-allocated cxa_array_t object
 * @param[in] datatypeSize_bytesIn the size of each element that will be inserted
 * 		into the array (all elements MUST be the same size)
 * @param[in] currNumElemsIn the number of elements of the given datatypeSize in the
 *		specified buffer. The resulting array will be initialized such that
 *		::cxa_array_getSize_elems will equal currNumElemsIn.
 * @param[in] bufferLocIn pointer to the pre-allocated chunk of memory that will
 * 		be used to store elements in the array (the buffer)
 * @param[in] bufferMaxSize_bytesIn the maximum size of the chunk of memory (buffer) in bytes
 */
void cxa_array_init_inPlace(cxa_array_t *const arrIn, const size_t datatypeSize_bytesIn, const size_t currNumElemsIn, void *const bufferLocIn, const size_t bufferMaxSize_bytesIn);


/**
 * @public
 * @brief Appends an element to the end of the given array.
 * If the array is full, this function will leave the array unmodified.
 *
 * @param[in] arrIn pointer to the pre-initialized cxa_array_t object
 * @param[in] itemLocIn pointer to the element which will be copied
 * 		into the array's buffer
 *
 * @return true on successful append, false on error
 * 		(array is full, failed copy, etc)
 */
bool cxa_array_append(cxa_array_t *const arrIn, void *const itemLocIn);


/**
 * @public
 * @brief Appends an element to the end of the given array but
 * does not copy any data to the new element location.
 *
 * Instead, this function returns a pointer to the newly
 * created element in the array. This allows you to minimize
 * copy operations by initializing the element "in-place"
 * within the array.
 *
 * @param[in] arrIn pointer to the pre-initialized cxa_array_t object
 *
 * @return a pointer to the new element location within the
 *		array object OR NULL on error (array is likely full)
 */
void* cxa_array_append_empty(cxa_array_t *const arrIn);


/**
 * @public
 * @brief Removes the specified element from the array (moving
 * all following elements down)
 *
 * @param[in] arrIn pointer to the pre-initialized cxa_array_t object
 * @param[in] indexIn the index of the element which should be removed
 *
 * @return true if the element was successfully removed, false on error
 *		(invalid index, etc)
 */
bool cxa_array_remove_atIndex(cxa_array_t *const arrIn, const size_t indexIn);


/**
 * @public
 * @brief Removes the element at the specified memory location from the
 * the array (moving all following elements down)
 *
 * @param[in] arrIn pointer to the pre-initialized cxa_array_t object
 * @param[in] itemLocIn pointer to the element to remove. Must be the
 * 		starting address of the element to remove _within_ the array.
 *
 * @return true if the element was successfully removed, false on error
 *		(invalid item to remove, etc)
 */
bool cxa_array_remove(cxa_array_t *const arrIn, void *const itemLocIn);


/**
 * @public
 * @brief Returns a pointer to the element (contained within the array's
 * buffer) at the specified index.
 *
 * @param[in] arrIn pointer to the pre-initialized cxa_array_t object
 * @param[in] indexIn the index of the desired object
 *
 * @return pointer to the element, or NULL if out of bounds
 */
void* cxa_array_get(cxa_array_t *const arrIn, const size_t indexIn);


/**
 * @protected
 * @brief Returns a pointer to the element (contained within the array's
 * buffer) at the specified index. If the index is out of bounds,
 * it will return the proper calculated point as long as the index
 * lies within the maximum size of the array.
 *
 * @param[in] arrIn pointer to the pre-initialized cxa_array_t object
 * @param[in] indexIn the index of the desired object
 *
 * @return pointer to the element, or NULL if index is outside the
 *		maximum size of the array.
 */
void *cxa_array_get_noBoundsCheck(cxa_array_t *const arrIn, const size_t indexIn);


/**
 * @public
 * @brief Overwrites an element at the specified index of the array.
 *
 * @param[in] arrIn pointer to the pre-initialized cxa_array_t object
 * @param[in] indexIn the index of the desired object. MUST be less
 * 		than ::cxa_getSize_elems.
 * @param[in] itemLocIn pointer to the element which will be copied
 * 		into the specified position in the array
 *
 * @return true on successful overwrite, false on error
 * 		(array is full, failed copy, etc)
 */
bool cxa_array_overwrite(cxa_array_t *const arrIn, const size_t indexIn, void *const itemLocIn);


/**
 * @public
 * @brief Inserts an element at the specified index of the array.
 * Subsequent elements are copied/moved to make room for the insertion.
 * The size of the array will grow by 1.
 *
 * @param[in] arrIn pointer to the pre-initialized cxa_array_t object
 * @param[in] indexIn the index of the desired object. MUST be less
 * 		than or equal to ::cxa_getSize_elems.
 * @param[in] itemLocIn pointer to the element which will be copied
 * 		into the specified position in the array
 *
 * @return true on successful insertion, false on error
 * 		(array is full)
 */
bool cxa_array_insert(cxa_array_t *const arrIn, const size_t indexIn, void *const itemLocIn);


/**
 * @public
 * @brief Determines the size of the array (in number of elements).
 *
 * @param[in] arrIn pointer to the pre-initialized cxa_array_t object
 *
 * @return the size of the array, in number of elements
 */
size_t cxa_array_getSize_elems(cxa_array_t *const arrIn);


/**
 * @public
 * @brief Determines the maximum number of elements this array can hold.
 *
 * @param[in] arrIn pointer to the pre-initialized cxa_array_t object
 *
 * @return the maximum number of elements this array can hold
 */
size_t cxa_array_getMaxSize_elems(cxa_array_t *const arrIn);


/**
 * @public
 * @brief Determines the number of free spots/elements in the array.
 *
 * @param[in] arrIn pointer to the pre-initialized cxa_array_t object
 *
 * @return the number of free elements in the array
 */
size_t cxa_array_getFreeSize_elems(cxa_array_t *const arrIn);


/**
 * @public
 * @brief Determines whether the array is full (cannot hold any more elements).
 *
 * @param[in] arrIn pointer to the pre-initialized cxa_array_t object
 *
 * @return true if the array cannot hold any more elements
 */
bool cxa_array_isFull(cxa_array_t *const arrIn);


/**
 * @public
 * @brief Determines whether the array is empty (does not hold any elements)
 *
 * @param[in] arrIn pointer to the pre-initialized cxa_array_t object
 *
 * @return true if the array does not currently contain any elements
 */
bool cxa_array_isEmpty(cxa_array_t *const arrIn);


/**
 * @public
 * @brief Clears the array, discarding any elements current contained
 * within it. This function will result in an empty array, but the
 * underlying memory may still contain the original data.
 *
 * @param[in] arrIn pointer to the pre-initialized cxa_array_t object
 */
void cxa_array_clear(cxa_array_t *const arrIn);


#ifndef CXA_FILE_DISABLE
/**
 * @public
 * @brief Writes a human-friendly representation of the bytes contained within this array (excluding unused bytes)
 * to the file descriptor.
 * <b>Example:</b>
 * @code {0x00, 0x01} @endcode
 *
 * @param[in] arrIn pointer to the pre-initialized cxa_array_t object
 * @param[in] tagIn textual tag to identify this particular print (can be NULL)
 * @param[in] fileIn pointer to an open file descriptor to which the bytes will be written
 *
 * @return true on success, false on failure
 */
bool cxa_array_writeToFile_asciiHexRep(cxa_array_t *const arrIn, const char *const tagIn, FILE *fileIn);
#endif

#endif // CXA_ARRAY_H_
