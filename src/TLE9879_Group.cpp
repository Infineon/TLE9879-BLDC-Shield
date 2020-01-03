
/*
 * TLE9879_Group.cpp
 *
 *  Created on: 19.02.2018
 *      Author: Haidari, Ahmad
 */

#include "TLE9879_Group.h"
#include "Arduino.h"
#include "SPI.h"

TLE9879_Group::TLE9879_Group(uint8_t boardCount)
{
	this->boardCount = boardCount;
	
	// start serial connection with a baud rate of 9600
	Serial.begin(9600);
	
	Serial.println(F("INFO: Starting initialization"));
	
	// set the auto-addressing pin to high
	pinMode(8, OUTPUT);
	digitalWrite(8, HIGH);
	
	// set all the slave-select-pins to high (not selected)
	pinMode(SLAVESELECT_BOARD1, OUTPUT);
	pinMode(SLAVESELECT_BOARD2, OUTPUT);
	pinMode(SLAVESELECT_BOARD3, OUTPUT);
	pinMode(SLAVESELECT_BOARD4, OUTPUT);
	digitalWrite(SLAVESELECT_BOARD1, HIGH);
	digitalWrite(SLAVESELECT_BOARD2, HIGH);
	digitalWrite(SLAVESELECT_BOARD3, HIGH);
	digitalWrite(SLAVESELECT_BOARD4, HIGH);

	delay(500);
	
	// start the SPI connection
	SPI.begin();
	SPI.beginTransaction(SPISettings(1000000, MSBFIRST, SPI_MODE3));
	
	// all boards are reset and set to bootloader mode
	resetAllBoards();
	delay(500);
	
	// Here, the given number of boards is initialized.
	// The initialization command is send repeatedly, until exactly the given number of boards are ready to be used,
	// i.e. if any board does not respond positively, the command is send again and again, until it responds.
	// This means the program will be stuck in a loop in case one board is not properly attached or does not work properly.
	for(int i = 0; i < boardCount; i++)
	{
		// create board object
		boards[i] = new Board(i + 1, status);
		
		// if the board is not available wait 2 seconds and try again
		while(!boards[i]->isAvailable())
		{
			delay(2000);
			if(status->code != ERR_NONE) processStatusCodes(BOARDCONTROL, i + 1);
			boards[i]->boardControl();
		}
		
		Serial.print(F("INFO: Board "));
		Serial.print(i + 1);
		Serial.println(F(" was successfully initialized"));
	}
	
	Serial.println(F("INFO: Done initializing"));
	delay(100);
}

TLE9879_Group::~TLE9879_Group()
{
	for(int i = 0; i < 4; i++) if(boards[i]) delete boards[i];
	delete status;
}

void TLE9879_Group::sendMessageToAll(uint16_t data)
{
	// select all four boards simultaneously
	digitalWrite(SLAVESELECT_BOARD1, LOW);
	digitalWrite(SLAVESELECT_BOARD2, LOW);
	digitalWrite(SLAVESELECT_BOARD3, LOW);
	digitalWrite(SLAVESELECT_BOARD4, LOW);
	
	// send the message to all boards
	delayMicroseconds(100);
	SPI.transfer16(data);
	delayMicroseconds(100);
	
	// deselect all four boards
	digitalWrite(SLAVESELECT_BOARD1, HIGH);
	digitalWrite(SLAVESELECT_BOARD2, HIGH);
	digitalWrite(SLAVESELECT_BOARD3, HIGH);
	digitalWrite(SLAVESELECT_BOARD4, HIGH);
	delayMicroseconds(100);
}

void TLE9879_Group::loadDataset(uint8_t pos, uint8_t boardnr)
{
	uint8_t index, end;
	if(!checkBoardNumber(boardnr, LOADDATASET, index, end)) return;
	
	for( ; index < end; index++)
	{
		boards[index]->loadDataset(pos);
		processStatusCodes(LOADDATASET, index + 1);
	}
	
	if(infoMessagesEnabled)
	{
		Serial.print(F("INFO: load dataset complete -> "));
		printBoardNr(boardnr);
		Serial.println(pos);
	}
}

void TLE9879_Group::saveDataset(uint8_t pos, uint8_t boardnr)
{	
	pos -= CUSTOMDATASET0; // custom datasets start at 0x10 instead of 0x00

	uint8_t index, end;
	if(!checkBoardNumber(boardnr, SAVEDATASET, index, end)) return;

	for( ; index < end; index++)
	{
		boards[index]->saveDataset(pos);
		processStatusCodes(SAVEDATASET, index + 1);
	}
	
	if(infoMessagesEnabled)
	{
		Serial.print(F("INFO: save dataset complete -> "));
		printBoardNr(boardnr);
		Serial.println(pos);
	}
}

void TLE9879_Group::readDataset(uint8_t boardnr)
{
	if(boardnr > ALL_BOARDS || boardnr == 0) 
	{
		status->code = ERR_INVALID_BOARD_NR;
		status->additionalInfo[0] = boardnr;
	}
	
	uint8_t index, end;
	if(!checkBoardNumber(boardnr, READDATASET, index, end)) return;
	
	for( ; index < end; index++)
	{
		boards[index]->readDataset();
		processStatusCodes(READDATASET, index + 1);
	}
	
	if(infoMessagesEnabled)
	{
		Serial.print(F("INFO: read dataset complete -> "));
		printBoardNr(boardnr);
		Serial.println("");
	}
}

void TLE9879_Group::writeDataset(uint8_t boardnr)
{
	uint8_t index, end;
	if(!checkBoardNumber(boardnr, WRITEDATASET, index, end)) return;
	
	for( ; index < end; index++)
	{
		boards[index]->writeDataset();
		processStatusCodes(WRITEDATASET, index + 1);
	}
	
	if(infoMessagesEnabled)
	{
		Serial.print(F("INFO: write dataset complete -> "));
		printBoardNr(boardnr);
		Serial.println();
	}
}

// TODO: TEST THIS
void TLE9879_Group::copyDataset(uint8_t mode, uint8_t from, uint8_t to)
{
	from--;
	
	if(mode == FOC || mode == BEMF || mode == HALL)
	{
		uint8_t index, end;
		if(!checkBoardNumber(to, COPYDATASET, index, end)) return;
		if(from > BOARD4 - 1)
		{
			status->code = ERR_INVALID_BOARD_NR;
			status->additionalInfo[0] = from;
		} else
		{
			for( ; index < end; index++)
			{
				if(index == from) continue;
				
				switch(mode)
				{
					case FOC: boards[index]->data_FOC->datastruct_FOC = boards[from]->data_FOC->datastruct_FOC; break;
					case BEMF: (*boards[index]->data_BEMF) = (*boards[from]->data_BEMF); break;
					case HALL: (*boards[index]->data_HALL) = (*boards[from]->data_HALL); break;
				}
			}
		}
	} else 
	{
		status->code = ERR_INVALID_PARAMETER;
		status->additionalInfo[0] = mode;
	}
	
	processStatusCodes(COPYDATASET, 0);
	
	if(infoMessagesEnabled)
	{
		Serial.print(F("INFO: copy dataset complete -> "));
		printBoardNr(to);
		Serial.println();
	}
}

void TLE9879_Group::setParameter(uint8_t parameter, float value, uint8_t boardnr)
{
	uint8_t index, end;
	if(!checkBoardNumber(boardnr, CHANGEPARAMETER, index, end)) return;
	
	for( ; index < end; index++)
	{
		boards[index]->changeParameter(parameter, value);
		processStatusCodes(CHANGEPARAMETER, index + 1);
	}
	
	if(infoMessagesEnabled)
	{
		Serial.print(F("INFO: parameter change complete -> "));
		printBoardNr(boardnr);
		Serial.print(F("parameter: "));
		Serial.print(parameter);
		Serial.print(F("; value: "));
		Serial.println(value);
	}
}

void TLE9879_Group::setMotorMode(uint8_t mode, uint8_t boardnr)
{
	if (mode > STOP_MOTOR)
	{
		status->code = ERR_INVALID_PARAMETER;
		status->additionalInfo[0] = mode;
		return;
	}
	
	uint8_t index, end;
	if (!checkBoardNumber(boardnr, MOTORCONTROL, index, end)) return;
	
	for ( ; index < end; index++)
	{
		boards[index]->motorControl(mode);
		processStatusCodes(MOTORCONTROL, index + 1);
	}
	
	if(infoMessagesEnabled)
	{
		Serial.print(F("INFO: motor mode change complete -> "));
		printBoardNr(boardnr);
		if (mode == START_MOTOR) Serial.println(F("start"));
		else Serial.println(F("stop"));
	}
}

void TLE9879_Group::setMotorSpeed(float motorspeed, uint8_t boardnr)
{
	uint8_t index, end;
	if(!checkBoardNumber(boardnr, SETMOTORSPEED, index, end)) return;
	
	for( ; index < end; index++)
	{
		boards[index]->setMotorspeed(motorspeed);
		processStatusCodes(SETMOTORSPEED, index + 1);
	}
	
	if(infoMessagesEnabled)
	{
		Serial.print(F("INFO: motor speed change complete -> "));
		printBoardNr(boardnr);
		Serial.println(motorspeed);
	}
}

void TLE9879_Group::setMode(uint8_t mode, uint8_t boardnr, bool fastMode)
{
	uint8_t index, end;
	if(!checkBoardNumber(boardnr, MODECONTROL, index, end)) return;
	
	if(fastMode)
	{
		sendMessageToAll(MODECONTROL + mode);
		delay(1000);
		for( ; index < end; index++)
		{
			boards[index]->modeControl(GETCURRENTMODE);
			if(boards[index]->getCurrentMode() != mode)
			{
				status->code = ERR_MODE_CHANGE_FAILED;
				status->additionalInfo[0] = (uint16_t) boards[index]->getCurrentMode();
				status->additionalInfo[1] = (uint16_t) mode;
				status->additionalInfo[2] = (uint16_t) 0;
				processStatusCodes(MODECONTROL, index + 1);
			}
		}
	} else
	{	
		for( ; index < end; index++)
		{
			boards[index]->modeControl(mode);
			processStatusCodes(MODECONTROL, index + 1);
		}
	}
	
	if(infoMessagesEnabled)
	{
		Serial.print(F("INFO: mode change complete -> "));
		printBoardNr(boardnr);
		if(mode == GETCURRENTMODE) Serial.println(F("Get Current Mode"));
		else Serial.println(Board::modenames[mode]);
	}
}

void TLE9879_Group::setLed(uint16_t led, uint16_t mode, uint8_t boardnr)
{
	uint8_t index, end;
	if(!checkBoardNumber(boardnr, LED, index, end)) return;
	
	for( ; index < end; index++)
	{
		if(mode == LED_ON) boards[index]->LEDOn(led);
		else if(mode == LED_OFF) boards[index]->LEDOff(led);
		processStatusCodes(LED, index + 1);
	}
	
	if(infoMessagesEnabled)
	{
		Serial.println(F("INFO: LED control complete")); // TODO maybe add more information (e.g. on/off)
	}
}

void TLE9879_Group::setLedColor(uint8_t color, uint8_t boardnr)
{
	// blue LED
	if(color & 1) setLed(LED_BLUE, LED_ON, boardnr);
	else setLed(LED_BLUE, LED_OFF, boardnr);
	
	// green LED
	if(color & 2) setLed(LED_GREEN, LED_ON, boardnr);
	else setLed(LED_GREEN, LED_OFF, boardnr);
	
	// red LED
	if(color & 4) setLed(LED_RED, LED_ON, boardnr);
	else setLed(LED_RED, LED_OFF, boardnr);
}

void TLE9879_Group::startAutoAddressing()
{
	Serial.println(F("INFO: Doing auto-addressing"));
	sendMessageToAll(BOARDCONTROL + AUTOADDRESSING);
	delay(100); // let slaves do auto-addressing
}

void TLE9879_Group::resetAllBoards()
{
	Serial.println(F("INFO: Sending reset request to all boards"));
	// stop all motors (they may be running)
	sendMessageToAll(MOTORCONTROL + STOP_MOTOR);
	delay(10);
	// reset all TLE9879_Boards
	sendMessageToAll(BOARDCONTROL + RESET);
	delay(1000); // TODO reduce time, let slaves do reset
}

void TLE9879_Group::checkErrors()
{
	static unsigned long time = 0;
	
	if(millis() - time < 1000) return;
	
	for(int i = 0; i < boardCount; i++)
	{
		if(!boards[i]) continue;
		
		boards[i]->sendMessage(CHECK_ERROR);
		uint16_t answer = boards[i]->readAnswer();
		if(answer != CHECK_ERROR + CONFIRM_OFFSET)
		{
			Serial.print(F("WARNING: Error Checks; Board: "));
			Serial.print(i + 1);
			Serial.print(F("; Mode: "));
			Serial.print(Board::modenames[boards[i]->getCurrentMode()]);
			Serial.print(F("; Error code: "));
			
			for(uint16_t i = 1; i <= 0x1000; i *= 2)
			{
				if((i & answer) != 0)
				{
					Serial.print(i, HEX);
					Serial.print(F(", "));
				}
			}
			Serial.print(F("answer: "));
			Serial.println(answer, HEX);
		}
	}
	
	time = millis();
}

/*
TLE9879_Group::Board* TLE9879_Group::getBoard(uint8_t index)
{
	if(index > BOARD4) return nullptr;
	return boards[index - 1];
}
*/

void TLE9879_Group::processStatusCodes(uint16_t action, uint8_t boardnr)
{
	if(status->code == ERR_NONE) return;
	Serial.print(F("WARNING: Board["));
	Serial.print(boardnr);
	Serial.print(F("]; Action["));
	printAction(action);
	Serial.print(F("]; "));
	
	switch(status->code)
	{
		case ERR_STILL_IN_BOOTLOADER:
			Serial.println(F("action cannot be performed in bootloader"));
			break;
		case ERR_CHECKSUM_IS_WRONG:
			Serial.print(F("checksum does not match -> calc: "));
			Serial.print(status->additionalInfo[0]);
			Serial.print(F("; recv: "));
			Serial.println(status->additionalInfo[1]);
			break;
		case ERR_NOT_AVAILABLE:
			Serial.println(F("not available"));
			break;
		case ERR_MODE_READING_FAILED:
			Serial.println(F("mode could not be read"));
			break;
		case ERR_MODE_CHANGE_FAILED:
			Serial.print(F("failed -> from "));
			Serial.print(Board::modenames[status->additionalInfo[0]]);
			Serial.print(F(" to "));
			Serial.print(Board::modenames[status->additionalInfo[1]]);
			Serial.print(F("; Answer: "));
			Serial.println(status->additionalInfo[2], HEX);
			break;
		case ERR_INVALID_DATASET_NUMBER:
			Serial.print(F("the dataset number was invalid -> "));
			Serial.println(status->additionalInfo[0]);
			break;
		case ERR_INVALID_PARAMETER_INDEX:
			Serial.print(F("parameter index is invalid -> "));
			Serial.print(Board::modenames[status->additionalInfo[0]]);
			Serial.print(F("; "));
			Serial.println(status->additionalInfo[1]);
			break;
		case ERR_FAILED:
			Serial.println(F("failed"));
			break;
		case ERR_INVALID_PARAMETER:
			Serial.print(F("the given parameter is invalid -> "));
			Serial.println(status->additionalInfo[0]);
			break;
		case ERR_INVALID_BOARD_NR:
			Serial.print(F("the given boardnr is invalid ->"));
			Serial.println(boardnr);
			break;
		case ERR_FAILED_ANSWER:
			Serial.print(F("shield failed to confirm successful command -> answer: 0x"));
			Serial.print(status->additionalInfo[0], HEX);
			Serial.print(F(", expected: 0x"));
			Serial.println(status->additionalInfo[1], HEX);
			break;
		default:
			Serial.print(F("Error code is not available ->"));
			Serial.println(status->code);
			break;
	}
	
	status->code = ERR_NONE;
}

void TLE9879_Group::printAction(uint16_t action)
{
	switch(action)
	{
		case MODECONTROL: Serial.print(F("mode control")); break;
		case LOADDATASET: Serial.print(F("load dataset")); break;
		case READDATASET: Serial.print(F("read dataset")); break;
		case WRITEDATASET: Serial.print(F("write dataset")); break;
		case CHANGEPARAMETER: Serial.print(F("change parameter")); break;
		case SAVEDATASET: Serial.print(F("save dataset")); break;
		case SETMOTORSPEED: Serial.print(F("set motor speed")); break;
		case MOTORCONTROL: Serial.print(F("motor control")); break;
		case BOARDCONTROL: Serial.print(F("board control")); break;
		case COPYDATASET: Serial.print(F("copy dataset")); break;
		case LED: Serial.print(F("LED control")); break;
		default: break;
	}
}

bool TLE9879_Group::checkBoardNumber(uint8_t boardnr, uint16_t action, uint8_t& startIndex, uint8_t& endIndex)
{
	if(boardnr > ALL_BOARDS)
	{
		status->code = ERR_INVALID_BOARD_NR;
		processStatusCodes(action, boardnr);
		return false;
	} else if(boardnr == ALL_BOARDS)
	{
		startIndex = 0;
		endIndex = boardCount;
	} else 
	{
		startIndex = boardnr - 1;
		endIndex = boardnr;
	}
	return true;
}

void TLE9879_Group::printBoardNr(uint8_t boardnr)
{
	if(boardnr == ALL_BOARDS) Serial.print(F("Board: All; "));
	else 
	{
		Serial.print(F("Board: "));
		Serial.print(boardnr);
		Serial.print(F("; "));
	}
}

void TLE9879_Group::testBlinky()
{
	static uint8_t index = 0;
	static long timerBlinky = 0;
	
	if(millis() - timerBlinky > 1000)
	{
		index++;
		if(index > COLOR_MAX_NUMBER)
		{
			index = 0;
		}
		
		setLedColor(index);
		timerBlinky = millis();
	}
}
