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
#ifndef CXA_MQTT_RPC_MESSAGE_H_
#define CXA_MQTT_RPC_MESSAGE_H_


// ******** includes ********
#include <stdbool.h>
#include <cxa_mqtt_message.h>


// ******** global macro definitions ********


// ******** global type definitions *********


// ******** global function prototypes ********
bool cxa_mqtt_rpc_message_isActionableRequest(cxa_mqtt_message_t *const msgIn, char** methodNameOut, size_t* methodNameLen_bytesOut, char** idOut, size_t* idLen_bytesOut);
bool cxa_mqtt_rpc_message_isActionableResponse(cxa_mqtt_message_t *const msgIn, char** methodNameOut, size_t* methodNameLen_bytesOut, char** idOut, size_t* idLen_bytesOut);

#endif /* CXA_MQTT_RPC_MESSAGE_H_ */
