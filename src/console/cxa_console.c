/**
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
#include "cxa_console.h"


// ******** includes ********
#include <cxa_array.h>
#include <cxa_assert.h>
#include <cxa_delay.h>
#include <cxa_numberUtils.h>
#include <cxa_runLoop.h>
#include <cxa_stringUtils.h>
#include <string.h>


#include <cxa_logger_implementation.h>


// ******** local macro definitions ********
#define HEADER_NUM_COLS					40
#define COMMAND_BUFFER_LEN_BYTES		100
#define COMMAND_PROMPT					" > "
#define ESCAPE							"\x1b"
#define CONSOLE_RESPONSE_TIMEOUT_MS		2000
#define MAX_ARGS 4
#define MAX_ARG_LEN 80


// ******** local type definitions ********
typedef struct
{
	char command[CXA_CONSOLE_MAX_COMMAND_LEN_BYTES+1];
	cxa_console_command_cb_t cb;
	void* userVar;
}commandEntry_t;


// ******** local function prototypes ********
static void cb_onRunLoopUpdate(void* userVarIn);
static void printBootHeader(const char* deviceNameIn);
static void printCommandLine(void);
static void printError(char* errorIn);
static void printBlockLine(const char* textIn, size_t maxNumCols);
static void clearScreenReturnHome(void);

static void command_clear(cxa_ioStream_t *const ioStreamIn, void* userVarIn, int argc, char* argv[]);
static void command_help(cxa_ioStream_t *const ioStreamIn, void* userVarIn, int argc, char* argv[]);


// ********  local variable declarations *********
static cxa_ioStream_t* ioStream = NULL;

static cxa_array_t commandBuffer;
static char commandBuffer_raw[COMMAND_BUFFER_LEN_BYTES];

static cxa_array_t commandEntries;
static commandEntry_t commandEntries_raw[CXA_CONSOLE_MAXNUM_COMMANDS+2];
// add one for 'clear' and 'help' command

static bool isExecutingCommand = false;


// ******** global function implementations ********
void cxa_console_init(const char* deviceNameIn, cxa_ioStream_t *const ioStreamIn)
{
	cxa_assert(ioStreamIn);

	// save our references
	ioStream = ioStreamIn;

	// setup our arrays
	cxa_array_initStd(&commandBuffer, commandBuffer_raw);
	cxa_array_initStd(&commandEntries, commandEntries_raw);
	cxa_console_addCommand("clear", command_clear, NULL);
	cxa_console_addCommand("help", command_help, NULL);

	// register for our runLoop
	cxa_runLoop_addEntry(cb_onRunLoopUpdate, NULL);

	printBootHeader(deviceNameIn);
	printCommandLine();
}


void cxa_console_addCommand(const char* commandIn, cxa_console_command_cb_t cbIn, void* userVarIn)
{
	cxa_assert(commandIn);
	cxa_assert(strlen(commandIn) <= CXA_CONSOLE_MAX_COMMAND_LEN_BYTES);
	cxa_assert(cbIn);

	commandEntry_t newEntry = {
			.cb = cbIn,
			.userVar = userVarIn
	};
	cxa_stringUtils_copy(newEntry.command, commandIn, sizeof(newEntry.command));
	cxa_assert(cxa_array_append(&commandEntries, &newEntry));
}


void cxa_console_prelog(void)
{
	if( ioStream == NULL ) return;

	if( !isExecutingCommand )
	{
		cxa_ioStream_writeString(ioStream, ESCAPE "[2K");
		cxa_ioStream_writeByte(ioStream, '\r');
		cxa_ioStream_writeString(ioStream, ESCAPE "[1A");
	}
}


void cxa_console_postlog(void)
{
	if( ioStream == NULL ) return;

	if( !isExecutingCommand ) printCommandLine();
}


// ******** local function implementations ********
static void cb_onRunLoopUpdate(void* userVarIn)
{
	uint8_t rxByte;
	if( cxa_ioStream_readByte(ioStream, &rxByte) == CXA_IOSTREAM_READSTAT_GOTDATA )
	{
		// handle carriage returns / line feeds
		if( (rxByte == '\r') || (rxByte == '\n') )
		{
			// end of a command...make sure it's not empty
			if( cxa_array_isEmpty(&commandBuffer) ) return;

			// process our command
			bool foundCommand = false;
			cxa_array_iterate(&commandEntries, currEntry, commandEntry_t)
			{
				if( currEntry == NULL) continue;

				if( cxa_stringUtils_startsWith(cxa_array_get(&commandBuffer, 0), currEntry->command) )
				{
					// we have a matching command...give it two new lines, then call the callback


					isExecutingCommand = true;
					cxa_ioStream_writeString(ioStream, CXA_LINE_ENDING);

					char arg_buf[MAX_ARGS * MAX_ARG_LEN];
					char *argv[MAX_ARGS];
					int argc = 0;

					void* cmdBufPtr = cxa_array_get(&commandBuffer, 0);
					int cmdBufLen = cxa_array_getSize_elems(&commandBuffer);
					for (int lcv = 0; lcv < MAX_ARGS; lcv++){
							int offs = cxa_stringUtils_indexOf_withLengths(cmdBufPtr, cmdBufLen, " ", 1);
							if (offs >= 0){
									memcpy(&arg_buf[lcv*MAX_ARG_LEN], cmdBufPtr, offs);
									arg_buf[lcv*MAX_ARG_LEN + offs] = 0;
									argv[lcv] = &arg_buf[lcv*MAX_ARG_LEN];
									argc++;
									cmdBufPtr = &cmdBufPtr[offs + 1];
									cmdBufLen = cmdBufLen - offs;
							}
							else{
									//There's one argument left...
									memcpy(&arg_buf[lcv*MAX_ARG_LEN], cmdBufPtr, cmdBufLen);
									arg_buf[lcv*MAX_ARG_LEN + cmdBufLen - 1] = 0;
									argv[lcv] = &arg_buf[lcv*MAX_ARG_LEN];
									argc++;
									break;
							}
					}
					if (argc > 0) {
							if( currEntry->cb != NULL ) currEntry->cb(ioStream, currEntry->userVar, argc, argv);
						}
					else {
							if( currEntry->cb != NULL ) currEntry->cb(ioStream, currEntry->userVar, 0, NULL);
						}

					cxa_ioStream_writeString(ioStream, CXA_LINE_ENDING);
					cxa_array_clear(&commandBuffer);
					foundCommand = true;
					break;
				}
			}
			if( !foundCommand )
			{
				cxa_array_clear(&commandBuffer);
				printError("Unknown command");
			}

			printCommandLine();
			return;
		}

		// handle backspaces / deletes
		if( (rxByte == 0x08) || (rxByte == 0x7F) )
		{
			size_t numItemsInCb = cxa_array_getSize_elems(&commandBuffer);
			if( numItemsInCb > 0 )
			{
				cxa_array_remove_atIndex(&commandBuffer, numItemsInCb - 1);
				// send character to terminal which should delete last item
				cxa_ioStream_writeByte(ioStream, rxByte);
			}
			return;
		}

		// if we made it here, we're still in the middle of a command
		if( !cxa_array_append(&commandBuffer, &rxByte) )
		{
			// commandBuffer is full
			cxa_array_clear(&commandBuffer);
			printError("Command too long for buffer");
			printCommandLine();
		}
		else
		{
			cxa_ioStream_writeByte(ioStream, rxByte);
		}
	}
}


static void printBootHeader(const char* deviceNameIn)
{
	clearScreenReturnHome();

	for( int i = 0; i < HEADER_NUM_COLS; i++ ) cxa_ioStream_writeByte(ioStream, '*');
	cxa_ioStream_writeString(ioStream, CXA_LINE_ENDING);

	if( deviceNameIn != NULL ) printBlockLine(deviceNameIn, HEADER_NUM_COLS);
	printBlockLine("Type 'help' for list of commands", HEADER_NUM_COLS);

	for( int i = 0; i < HEADER_NUM_COLS; i++ ) cxa_ioStream_writeByte(ioStream, '*');
	cxa_ioStream_writeString(ioStream, CXA_LINE_ENDING);
}


static void printCommandLine(void)
{
	cxa_ioStream_writeString(ioStream, CXA_LINE_ENDING);
	cxa_ioStream_writeString(ioStream, " > ");
	cxa_array_iterate(&commandBuffer, currCharPtr, char)
	{
		if( currCharPtr == NULL ) continue;

		cxa_ioStream_writeByte(ioStream, *currCharPtr);
	}
	isExecutingCommand = false;
}


static void printError(char* errorIn)
{
	cxa_assert(errorIn);

	cxa_ioStream_writeString(ioStream, CXA_LINE_ENDING);
	cxa_ioStream_writeString(ioStream, "!! ");
	cxa_ioStream_writeLine(ioStream, errorIn);
}


static void printBlockLine(const char* textIn, size_t maxNumCols)
{
	cxa_assert(textIn);

	cxa_ioStream_writeByte(ioStream, '*');
	cxa_ioStream_writeByte(ioStream, ' ');

	// make sure to account for leading and trailing ' *'
	int totalPadding_spaces = HEADER_NUM_COLS - strlen(textIn) - 4;
	if( totalPadding_spaces < 0 )
	{
		totalPadding_spaces = 0;
	}
	int leftPadding_spaces = totalPadding_spaces / 2;
	int rightPadding_spaces = totalPadding_spaces  - leftPadding_spaces;

	for( int i = 0; i < leftPadding_spaces; i++ ) cxa_ioStream_writeByte(ioStream, ' ');
	cxa_ioStream_writeBytes(ioStream, (void*)textIn, CXA_MIN(strlen(textIn), HEADER_NUM_COLS - 4));
	for( int i = 0; i < rightPadding_spaces; i++ ) cxa_ioStream_writeByte(ioStream, ' ');

	cxa_ioStream_writeByte(ioStream, ' ');
	cxa_ioStream_writeByte(ioStream, '*');
	cxa_ioStream_writeString(ioStream, CXA_LINE_ENDING);
}


static void clearScreenReturnHome(void)
{
	// clear screen, then set cursor to home
	cxa_ioStream_writeString(ioStream, ESCAPE "[2J");
	cxa_ioStream_writeString(ioStream, ESCAPE "[H");
}


static void command_clear(cxa_ioStream_t *const ioStreamIn, void* userVarIn, int argc, char* argv[])
{
	clearScreenReturnHome();
}


static void command_help(cxa_ioStream_t *const ioStreamIn, void* userVarIn, int argc, char* argv[])
{
	cxa_ioStream_writeLine(ioStream, "Available commands:");
	cxa_array_iterate(&commandEntries, currEntry, commandEntry_t)
	{
		if( currEntry == NULL ) continue;

		for( int i = 0; i < 3; i++ ) cxa_ioStream_writeString(ioStream, " ");
		cxa_ioStream_writeLine(ioStream, (char*)currEntry->command);
	}
}
