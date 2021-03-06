/**
 * @file
 * @copyright 2016 opencxa.org
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
#ifndef CXA_UUID128_H_
#define CXA_UUID128_H_


// ******** includes ********
#include <stdint.h>

#include <cxa_fixedByteBuffer.h>


// ******** global macro definitions ********


// ******** global type definitions *********
typedef struct
{
	char str[37];
}cxa_uuid128_string_t;


typedef struct
{
	uint8_t bytes[16];
}cxa_uuid128_t;


// ******** global function prototypes ********
void cxa_uuid128_init(cxa_uuid128_t *const uuidIn, uint8_t *const bytesIn);
bool cxa_uuid128_initFromBuffer(cxa_uuid128_t *const uuidIn, cxa_fixedByteBuffer_t *const fbbIn, size_t indexIn);
void cxa_uuid128_initRandom(cxa_uuid128_t *const uuidIn);

bool cxa_uuid128_isEqual(cxa_uuid128_t *const uuid1In, cxa_uuid128_t *const uuid2In);

void cxa_uuid128_toString(cxa_uuid128_t *const uuidIn, cxa_uuid128_string_t *const strOut);
void cxa_uuid128_toShortString(cxa_uuid128_t *const uuidIn, cxa_uuid128_string_t *const strOut);

#endif
