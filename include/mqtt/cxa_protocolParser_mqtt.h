/**
 * @file
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
#ifndef CXA_PROTOCOLPARSER_MQTT_H_
#define CXA_PROTOCOLPARSER_MQTT_H_


// ******** includes ********
#include <cxa_protocolParser.h>
#include <cxa_mqtt_message.h>
#include <cxa_stateMachine.h>


// ******** global macro definitions ********


// ******** global type definitions *********
typedef struct
{
	cxa_protocolParser_t super;

	cxa_stateMachine_t stateMachine;
	size_t remainingBytesToReceive;
}cxa_protocolParser_mqtt_t;


// ******** global function prototypes ********
void cxa_protocolParser_mqtt_init(cxa_protocolParser_mqtt_t *const mppIn, cxa_ioStream_t *const ioStreamIn, cxa_fixedByteBuffer_t *const buffIn);


#endif // CXA_PROTOCOLPARSER_MQTT_H_
