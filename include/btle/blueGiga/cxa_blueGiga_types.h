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
#ifndef CXA_BLUEGIGA_TYPES_H_
#define CXA_BLUEGIGA_TYPES_H_


// ******** includes ********


// ******** global macro definitions ********


// ******** global type definitions *********
typedef enum
{
	CXA_BLUEGIGA_MSGTYPE_RESPONSE = 0x00,
	CXA_BLUEGIGA_MSGTYPE_EVENT = 0x80,
	CXA_BLUEGIGA_MSGTYPE_UNKNOWN = 0x0F
}cxa_blueGiga_msgType_t;


typedef enum
{
	CXA_BLUEGIGA_CLASSID_SYSTEM = 0x00,
	CXA_BLUEGIGA_CLASSID_PERSISTENT_STORE = 0x01,
	CXA_BLUEGIGA_CLASSID_ATTR_DB = 0x02,
	CXA_BLUEGIGA_CLASSID_CONNECTION = 0x03,
	CXA_BLUEGIGA_CLASSID_ATTR_CLIENT = 0x04,
	CXA_BLUEGIGA_CLASSID_SEC_MANAGER = 0x05,
	CXA_BLUEGIGA_CLASSID_GAP = 0x06,
	CXA_BLUEGIGA_CLASSID_HARDWARE = 0x07,
	CXA_BLUEGIGA_CLASSID_UNKNOWN = 0x0FF
}cxa_blueGiga_classId_t;


typedef enum
{
	CXA_BLUEGIGA_SCANRESP_TYPE_CONN_ADVERT,
	CXA_BLUEGIGA_SCANRESP_TYPE_NONCONN_ADVERT,
	CXA_BLUEGIGA_SCANRESP_TYPE_SCAN_RESPONSE,
	CXA_BLUEGIGA_SCANRESP_TYPE_DISCOVERABLE_ADVERT
}cxa_blueGiga_scanResponsePacketType_t;


// ******** global function prototypes ********


#endif
