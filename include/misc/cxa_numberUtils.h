/**
 * Copyright 2013 opencxa.org
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#ifndef CXA_NUMBERUTILS_H_
#define CXA_NUMBERUTILS_H_


/**
 * @file
 * This is a file which contains utility functions for manipulating numbers
 *
 * @author Christopher Armenio
 */


// ******** includes ********
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>


// ******** global macro definitions ********
#define CXA_MAX(x, y) 								(((x) > (y)) ? (x) : (y))
#define CXA_MIN(x, y) 								(((x) < (y)) ? (x) : (y))

#define CXA_CLAMP_HIGH(val, max)					((val) = CXA_MIN((val), (max)))
#define CXA_CLAMP_LOW(val, min)						((val) = CXA_MAX((val), (min)))
#define CXA_CLAMP_LOW_HIGH(val, min, max)			((val) = CXA_CLAMP_HIGH(CXA_CLAMP_LOW((val), (min)), (max)))


// ******** global type definitions *********


// ******** global function prototypes ********
uint16_t cxa_numberUtils_crc16_oneShot(void* dataIn, size_t dataLen_bytesIn);

uint16_t cxa_numberUtils_crc16_step(uint16_t crcIn, uint8_t byteIn);


#endif // CXA_NUMBERUTILS_H_
