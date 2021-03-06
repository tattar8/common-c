/**
 * @file
 *
 * @note This object should work across all architecture-specific implementations
 *
 *
 * #### Example Usage: ####
 *
 * @code
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
#ifndef CXA_adcChannel_H_
#define CXA_adcChannel_H_


// ******** includes ********
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <cxa_array.h>


// ******** global macro definitions ********
#ifndef CXA_ADCCHAN_MAX_NUM_LISTENERS
#define CXA_ADCCHAN_MAX_NUM_LISTENERS		1
#endif


// ******** global type definitions *********
/**
 * @public
 */
typedef struct cxa_adcChannel cxa_adcChannel_t;


/**
 * @public
 */
typedef void (*cxa_adcChannel_cb_conversionComplete_t)(cxa_adcChannel_t *const adcChanIn, float readVoltageIn, void* userVarIn);


/**
 * @public
 */
typedef void (*cxa_adcChannel_cb_conversionComplete_raw_t)(cxa_adcChannel_t *const adcChanIn, const uint8_t* rawValIn, size_t rawValLenIn, void* userVarIn);


/**
 * @protected
 */
typedef bool (*cxa_adcChannel_scm_startConversion_singleShot_t)(cxa_adcChannel_t *const superIn);


/**
 * @private
 */
typedef struct
{
	cxa_adcChannel_cb_conversionComplete_t cb_convComp;
	cxa_adcChannel_cb_conversionComplete_raw_t cb_convComp_raw;

	void *userVar;
}cxa_adcChannel_listener_t;


/**
 * @private
 */
struct cxa_adcChannel
{
	cxa_adcChannel_scm_startConversion_singleShot_t scm_startConv_ss;

	cxa_array_t listeners;
	cxa_adcChannel_listener_t listeners_raw[CXA_ADCCHAN_MAX_NUM_LISTENERS];
};


// ******** global function prototypes ********
/**
 * @protected
 */
void cxa_adcChannel_init(cxa_adcChannel_t* const adcChanIn, cxa_adcChannel_scm_startConversion_singleShot_t scm_startConv_ssIn);


/**
 * @public
 */
void cxa_adcChannel_addListener(cxa_adcChannel_t *const adcChanIn,
						 cxa_adcChannel_cb_conversionComplete_t cb_convCompIn,
						 cxa_adcChannel_cb_conversionComplete_raw_t cb_convComp_rawIn,
						 void* userVarIn);


/**
 * @public
 */
bool cxa_adcChannel_startConversion_singleShot(cxa_adcChannel_t *const adcChanIn);


#endif /* CXA_ADCCHAN_H_ */
