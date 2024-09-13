/*
 * TLE9879_Board.cpp
 *
 *  Created on: 30.05.2017
 *      Author: OttJulia
 */

#include "TLE9879_Group.h"
#include "SPI.h"

// BEMF PARAMS
// uint16: BEMF_SPIKE_FILT, BEMF_BLANK_FILT, BEMF_POLE_PAIRS, BEMF_SPEED_KP, BEMF_SPEED_KI, BEMF_SPEED_TEST_ENABLE, BEMF_START_FREQ_ZERO
const uint8_t indices_16bit_BEMF[] = {28, 29, 6, 2, 3, 17, 16};
const uint8_t indices_16bit_BEMF_size = (uint8_t) (sizeof (indices_16bit_BEMF) / sizeof(indices_16bit_BEMF[0]));
// HALL PARAMS
// uint16: HALL_POLE_PAIRS, HALL_INIT_DUTY, HALL_INPUT_A, HALL_INPUT_B, HALL_INPUT_C, HALL_OFFSET_60DEGREE_EN, HALL_ANGLE_DELAY_EN, HALL_DELAY_ANGLE, HALL_DELAY_MINSPEED, HALL_SPEED_KP, HALL_SPEED_KI
const uint8_t indices_16bit_HALL[] = {2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};
const uint8_t indices_16bit_HALL_size = (uint8_t) (sizeof (indices_16bit_HALL) / sizeof(indices_16bit_HALL[0]));
// FOC PARAMS
// uint16: FOC_SPEED_KP, FOC_SPEED_KI, FOC_CSA_OFFSET, FOC_POLE_PAIRS, FOC_START_FREQ_ZERO
const uint8_t indices_16bit_FOC[] = {10, 11, 28, 29, 38};
const uint8_t indices_16bit_FOC_size = (uint8_t) (sizeof (indices_16bit_FOC) / sizeof(indices_16bit_FOC[0]));

const char* const TLE9879_Group::Board::modenames[4] = { "Bootloader", "BEMF", "Hall", "FOC" };

// Constructor
TLE9879_Group::Board::Board(uint8_t number, StatusInfo *info)
{
    boardnr = number;
    slaveselectpin = number + 3;
    status = info;

    // is requested board available?
    boardControl();
}

TLE9879_Group::Board::~Board()
{
    if (data_FOC)  delete data_FOC;
    if (data_BEMF) delete data_BEMF;
    if (data_HALL) delete data_HALL;
}

// functions for communication
void TLE9879_Group::Board::sendMessage(uint16_t data)
{
    digitalWrite(slaveselectpin, LOW);  // take the SS pin low to select board
    delayMicroseconds(100);
       SPI.transfer16(data);
       delayMicroseconds(100);
       digitalWrite(slaveselectpin, HIGH);  // take the SS pin high to de-select board
       delayMicroseconds(100);
}

uint16_t TLE9879_Group::Board::readAnswer()
{
    uint16_t receivedMessage = 0;
    delayMicroseconds(100);
    digitalWrite(slaveselectpin, LOW);
    delayMicroseconds(100);
    receivedMessage = SPI.transfer16(0x0000);
    delayMicroseconds(100);
    digitalWrite(slaveselectpin, HIGH);

    return receivedMessage;
}

boolean TLE9879_Group::Board::sendMessageAndCheckAnswer(uint16_t command)
{
    sendMessage(command);
    //delayMicroseconds(100);
    uint16_t answer = readAnswer();

    if(answer == (command + CONFIRM_OFFSET)) return true;
    else
    {
        Serial.print(F("WARNING: Board["));
        Serial.print(boardnr);
        Serial.print(F("]; Command["));
        Serial.print(command, HEX);
        Serial.print(F("]; Answer["));
        Serial.print(answer, HEX);
        Serial.println(F("]"));
        return false;
    }
}

boolean TLE9879_Group::Board::isAvailable()
{
    return board_available;
}

// commands
// 0x01: modeControl
boolean TLE9879_Group::Board::modeControl(uint8_t requestedmode)
{	
    // is the board available
    if(!isAvailable()) return false;

    // get current mode
    if(requestedmode == GETCURRENTMODE)
    {
        sendMessage(MODECONTROL + GETCURRENTMODE);
        uint16_t answer = readAnswer();

        if((answer == 0x1110) || (answer == 0x1111) || (answer == 0x1112) || (answer == 0x1113))
        {
            uint8_t answeredmode = (uint8_t)(answer & 0x000F);
            currentmode = answeredmode;
            return true;
        }
        else
        {
            status->code = ERR_MODE_READING_FAILED;
            return false;
        }
    }

    // already in the requested mode
    //if(requestedmode == currentmode) return true;
    
    // requested mode is not valid
    if(requestedmode > 3)
    {
        status->code = ERR_INVALID_PARAMETER;
        status->additionalInfo[0] = (uint16_t) requestedmode;
        return false;
    }

    // send mode change command
    sendMessage(MODECONTROL + requestedmode);
    // wait while slave is processing modeControl
    
    delay(1000);
    uint16_t answer = readAnswer();
    if(answer != (MODECONTROL + requestedmode + CONFIRM_OFFSET))
    {		
        // try to read current mode from TLE9879_Board
        delay(1000);
        sendMessage(MODECONTROL + GETCURRENTMODE);
        delay(100);
        uint16_t answer = readAnswer();
        uint16_t successanswer = MODECONTROL + GETCURRENTMODE + requestedmode + CONFIRM_OFFSET;

        // mode change failed
        if(answer != successanswer)
        {
            status->code = ERR_MODE_CHANGE_FAILED;
            status->additionalInfo[0] = (uint16_t) currentmode;
            status->additionalInfo[1] = (uint16_t) requestedmode;
            status->additionalInfo[2] = (uint16_t) answer;
            return false;
        }
    }
    
    // mode change successful
    switch(requestedmode)
    {
        case FOC: data_FOC = new data_FOC_union(); break;
        case BEMF: data_BEMF = new data_BEMF_union(); break;
        case HALL: data_HALL = new data_HALL_union(); break;
        default: break;
    }
    currentmode = requestedmode;
    return true;
}

// 0x02: loadDataset
boolean TLE9879_Group::Board::loadDataset(uint8_t datasetnr)
{
    if(!isAvailable()) return false;
    if(currentmode == BOOTLOADER)
    {
        status->code = ERR_STILL_IN_BOOTLOADER;
        return false;
    }
    if(((datasetnr > 3) && (datasetnr < 0x10)) || (datasetnr > 0x13))
    {
        status->code = ERR_INVALID_DATASET_NUMBER;
        status->additionalInfo[0] = datasetnr;
        return false;
    }
    sendMessage(LOADDATASET + datasetnr);
    delay(50);
    sendMessage(CHECKSUCCESS);
    uint16_t answer = readAnswer();

    if(answer != LOADDATASET + datasetnr + CONFIRM_OFFSET)
    {
        status->code = ERR_FAILED;
        status->additionalInfo[0] = datasetnr;
        return false;
    }
    return true;
}

// 0x03: readDataset
uint8_t TLE9879_Group::Board::readDataset()
{
    if(!isAvailable()) return 0;

    // find out mode to decide which dataset needs to be read
    boolean success = modeControl(GETCURRENTMODE);
    if(!success)
    {
        status->code = ERR_MODE_READING_FAILED;
        return 0;
    }

    uint8_t calcCRC, recvCRC;
    uint16_t nrofmessages;
    uint8_t *uint8ptr;

    switch(currentmode)
    {
        case BOOTLOADER:
            status->code = ERR_STILL_IN_BOOTLOADER;
            return 0;

        case BEMF:			
            sendMessage(READDATASET);
            nrofmessages = readAnswer();

            for(uint8_t i = 0; i < nrofmessages; i++)
            {
                data_BEMF->dataarray_BEMF[i] = readAnswer();
            }

            uint8ptr = (uint8_t*) data_BEMF;
            calcCRC = CRC8(uint8ptr, NUMBEROF_BYTES_BEMF);
            recvCRC = (uint8_t)readAnswer();
            break;

        case HALL:
            sendMessage(READDATASET);
            nrofmessages = readAnswer();

            for(uint8_t i = 0; i < nrofmessages; i++)
            {
                data_HALL->dataarray_HALL[i] = readAnswer();
            }

            uint8ptr = (uint8_t*) data_HALL;
            calcCRC = CRC8(uint8ptr, NUMBEROF_BYTES_HALL);
            recvCRC = (uint8_t)readAnswer();
            break;

        case FOC:
            sendMessage(READDATASET);
            nrofmessages = readAnswer();
            readAnswer();

            for(uint8_t i = 0; i < nrofmessages; i++)
            {
                data_FOC->dataarray_FOC[i] = readAnswer();
            }

            uint8ptr = (uint8_t*) data_FOC;
            calcCRC = CRC8(uint8ptr, NUMBEROF_BYTES_FOC);
            recvCRC = (uint8_t) readAnswer();
            break;
            
        default:
            return 0;
    }
    
    if(calcCRC != recvCRC)
    {
        status->code = ERR_CHECKSUM_IS_WRONG;
        status->additionalInfo[0] = calcCRC;
        status->additionalInfo[1] = recvCRC;
        return 0;
    }
    return 3;
}

// 0x04: writeDataset
boolean TLE9879_Group::Board::writeDataset(void)
{
    uint8_t nrofmessages = 0, nrofbytes = 0, calcCRC = 0;
    uint8_t *uint8ptr;

    if(!isAvailable()) return false;

    boolean success = modeControl(GETCURRENTMODE);
    if(!success)
    {
        status->code = ERR_MODE_READING_FAILED;
        return false;
    }

    switch(currentmode)
    {
        case BOOTLOADER:
            status->code = ERR_STILL_IN_BOOTLOADER;
            return false;
            break;
            
        case BEMF:
            nrofmessages = NUMBEROF_MESSAGES_BEMF;
            uint8ptr = (uint8_t*)data_BEMF->dataarray_BEMF;
            calcCRC = CRC8(uint8ptr, NUMBEROF_BYTES_BEMF);
            sendMessage(WRITEDATASET + nrofmessages);
            for(int i = 0; i < nrofmessages; i++) 
            {
                sendMessage(data_BEMF->dataarray_BEMF[i]);
            }
            break;
            
        case HALL:
            nrofmessages = NUMBEROF_MESSAGES_HALL;
            uint8ptr = (uint8_t*)data_HALL->dataarray_HALL;
            calcCRC = CRC8(uint8ptr, NUMBEROF_BYTES_HALL);
            sendMessage(WRITEDATASET + nrofmessages);
            for(int i = 0; i < nrofmessages; i++)
            {
                sendMessage(data_HALL->dataarray_HALL[i]);
            }
            break;

        case FOC:
            nrofmessages = NUMBEROF_MESSAGES_FOC;
            uint8ptr = (uint8_t*)data_FOC->dataarray_FOC;
            calcCRC = CRC8(uint8ptr, NUMBEROF_BYTES_FOC);
            sendMessage(WRITEDATASET + nrofmessages);
            for(int i = 0; i < nrofmessages; i++)
            {
                sendMessage(data_FOC->dataarray_FOC[i]);
            }
            break;
    }

    sendMessage(calcCRC);
    delay(50);
    sendMessage(CHECKSUCCESS);
    uint16_t answer = readAnswer();


    if(answer != (WRITEDATASET + nrofmessages + CONFIRM_OFFSET))
    {
        status->code = ERR_FAILED_ANSWER;
        status->additionalInfo[0] = answer;
        status->additionalInfo[1] = WRITEDATASET + nrofmessages + CONFIRM_OFFSET;
        return false;
    }
    else return true;
}

// 0x05: changeParameter
boolean TLE9879_Group::Board::changeParameter(uint8_t index, float data)
{
    if(!isAvailable()) return false;

    boolean datacheckOK = false, dataIsUint16;
    boolean success = modeControl(GETCURRENTMODE);
    if(!success)
    {
        status->code = ERR_MODE_READING_FAILED;
        return false;
    }

    switch(currentmode)
    {
        case BOOTLOADER:
            status->code = ERR_STILL_IN_BOOTLOADER;
            return false;
            
        case BEMF:
            // no offset at index
            if(index >= HALLOFFSET)
            {
                status->code = ERR_INVALID_PARAMETER_INDEX;
                status->additionalInfo[0] = (uint16_t) BEMF;
                status->additionalInfo[1] = (uint16_t) index;
                return false;
            }
            dataIsUint16 = isvalueinarray(index, indices_16bit_BEMF, indices_16bit_BEMF_size);
            break;
            
        case HALL:
            // offset between HALLOFFSER and FOCOFFSET at index
            if((index < HALLOFFSET) || index > FOCOFFSET)
            {
                status->code = ERR_INVALID_PARAMETER_INDEX;
                status->additionalInfo[0] = (uint16_t) HALL;
                status->additionalInfo[1] = (uint16_t) index;
                return false;
            }
            index -= HALLOFFSET;
            dataIsUint16 = isvalueinarray(index, indices_16bit_HALL, indices_16bit_HALL_size);
            break;
            
        case FOC:
            // offset FOCOFFSET at index
            if(index < FOCOFFSET)
            {
                status->code = ERR_INVALID_PARAMETER_INDEX;
                status->additionalInfo[0] = (uint16_t) FOC;
                status->additionalInfo[1] = (uint16_t) index;
                return false;
            }
            index -= FOCOFFSET;
            dataIsUint16 = isvalueinarray(index, indices_16bit_FOC, indices_16bit_FOC_size);
            break;
    }

    if (dataIsUint16) // 16bit variable
    {
        uint16_t uint16val = (uint16_t)(data);
        datacheckOK = checkuint16val(currentmode, index, &uint16val);
        if(datacheckOK)
        {
            sendMessage(CHANGEPARAMETER + index);
            sendMessage((uint16_t)uint16val);
        }
        else return false;
    }
    else
    {
        uint32_t *uint32ptr = (uint32_t*) &data;
        datacheckOK = checkfloatval(currentmode, index, &data);
        if(datacheckOK)
        {
            sendMessage(CHANGEPARAMETER + index);
            sendMessage((uint16_t)((*uint32ptr)>>16));
            sendMessage((uint16_t)(*uint32ptr));
        }
        else return false;
    }

    delay(50);
    sendMessage(CHECKSUCCESS);
    uint16_t answer = readAnswer();
    if(answer != CHANGEPARAMETER + index + CONFIRM_OFFSET)
    {
        status->code = ERR_FAILED;
        return false;
    }
    return true;
}
// 0x06: saveDataset
boolean TLE9879_Group::Board::saveDataset(uint8_t position)
{
    if(!isAvailable()) return false;

    if(currentmode == BOOTLOADER)
    {
        status->code = ERR_STILL_IN_BOOTLOADER;
        return false;
    }

    if(position > 3)
    {
        status->code = ERR_INVALID_DATASET_NUMBER;
        status->additionalInfo[0] = position;
        return false;
    }

    sendMessage(SAVEDATASET + position);
    delay(50);
    sendMessage(CHECKSUCCESS);
    uint16_t answer = readAnswer();

    if(answer != SAVEDATASET + position + CONFIRM_OFFSET)
    {
        status->code = ERR_FAILED_ANSWER;
        status->additionalInfo[0] = answer;
        status->additionalInfo[1] = SAVEDATASET + position + CONFIRM_OFFSET;
        return false;
    }
    return true;
}

// 0x07: setMotorspeed
boolean TLE9879_Group::Board::setMotorspeed(float motorspeed)
{
    if(!isAvailable()) return false;
    if(currentmode == BOOTLOADER)
    {
        status->code = ERR_STILL_IN_BOOTLOADER;
        return false;
    }

    sendMessage(SETMOTORSPEED);
    sendMessage((int16_t)motorspeed);

    delay(50);
    sendMessage(CHECKSUCCESS);
    uint16_t answer = readAnswer();

     if(answer != SETMOTORSPEED +CONFIRM_OFFSET)
    {
        status->code = ERR_FAILED;
        return false;
    }
    this->motorspeed = motorspeed;
    return true;
}

// 0x08: motorControl-
boolean TLE9879_Group::Board::motorControl(uint8_t command)
{
    if(!isAvailable()) return false;
    if(currentmode == BOOTLOADER)
    {
        status->code = ERR_STILL_IN_BOOTLOADER;
        return false;
    }
    if((command != START_MOTOR) && (command != STOP_MOTOR))
    {
        status->code = ERR_INVALID_PARAMETER;
        status->additionalInfo[0] = command;
        return false;
    }
    sendMessage(MOTORCONTROL+command);
    delay(50);
    sendMessage(CHECKSUCCESS);
    uint16_t answer = readAnswer();

    if(answer != MOTORCONTROL + command + CONFIRM_OFFSET)
     {
        status->code = ERR_FAILED;
        Serial.print(F("WARNING: Board["));
        Serial.print(boardnr);
        Serial.print(F("]; Command["));
        Serial.print(command, HEX);
        Serial.print(F("]; Answer["));
        Serial.print(answer, HEX);
        Serial.println(F("]"));
        return false;
    }
    return true;
}

// 0x09: boardControl
boolean TLE9879_Group::Board::boardControl()
{	
    // board available?
    boolean success = sendMessageAndCheckAnswer(BOARDCONTROL + BOARD_AVAILABLE);
    board_available = success;
    return success;
}

// 0x0A: LEDOn
boolean TLE9879_Group::Board::LEDOn(uint8_t led)
{
    if(!isAvailable())
        return false;

    if((led != LED_RED) && (led != LED_GREEN) && (led != LED_BLUE))
    {
        Serial.println(F("Requested LED was not valid. Valid LEDs are: LED_RED, LED_GREEN, LED_BLUE"));
        return false;
    }
    boolean success = sendMessageAndCheckAnswer(LED_ON + led);	
    return success;
}

// 0x0B: LEDOff
boolean TLE9879_Group::Board::LEDOff(uint8_t led)
{
    if(!isAvailable())
        return false;

    if((led != LED_RED) && (led != LED_GREEN) && (led != LED_BLUE))
    {
        Serial.println(F("Requested LED was not valid. Valid LEDs are: LED_RED, LED_GREEN, LED_BLUE"));
        return false;
    }
    boolean success = sendMessageAndCheckAnswer(LED_OFF + led);
    return success;
}

//0x0F: getMotorspeed
int16_t TLE9879_Group::Board::getMotorspeed()
{
    if(!isAvailable()) return false;
    if(currentmode == BOOTLOADER)
    {
        status->code = ERR_STILL_IN_BOOTLOADER;
        return false;
    }
    
    sendMessage(GETMOTORSPEED);
    act_speed = readAnswer();
    Serial.print(F("INFO: Speed Of Motor"));
    Serial.print(boardnr);
    Serial.print(F("->"));
    Serial.println(act_speed);
    return true;
}

uint8_t TLE9879_Group::Board::isvalueinarray(uint8_t val, const uint8_t *indices_16bit_BEMF, const uint8_t size)
{
    int i;
    for (i=0; i < size; i++)
    {
        if (indices_16bit_BEMF[i] == val)
        {
            return 1;
        }
    }
    return 0;
}

//CRC-8 - based on the CRC8 formulas by Dallas/Maxim
//code released under the therms of the GNU GPL 3.0 license
//at http://www.leonardomiliani.com/en/2013/un-semplice-crc8-per-arduino/
uint8_t TLE9879_Group::Board::CRC8(uint8_t *data, uint8_t len)
{
    uint8_t crc = 0x55;
    while (len--)
    {
        uint8_t extract = *data++;
        for (uint8_t tempI = 8; tempI; tempI--)
        {
            uint8_t sum = (crc ^ extract) & 0x01;
            crc >>= 1;
            if (sum)
            {
                crc ^= 0x8C;
            }
            extract >>= 1;
        }
    }
    return crc;
}

boolean TLE9879_Group::Board::checkuint16val(uint8_t mode, uint8_t index, uint16_t *data)
{
    switch(mode)
    {
        case BEMF:
            switch(index)
            {
                case BEMF_START_FREQ_ZERO: // 1/0
                    if(*data > 0) *data = 1; break;
                case BEMF_SPEED_TEST_ENABLE: // 1/0
                    if(*data > 0) *data = 1; break;
                case BEMF_SPIKE_FILT: // code 1/2/4/8
                    switch (*data)
                    {
                        case 1: *data = 0; break;
                        case 2: *data = 1; break;
                        case 4: *data = 2; break;
                        case 8: *data = 3; break;
                        default:							
                            Serial.print(F("Invalid value '"));
                            Serial.print(*data);
                            Serial.println(F("' for BEMF_SPIKE_FILT. Valid values are: 1, 2, 4, 8"));
                            return false;
                    }
                    break;
                case BEMF_BLANK_FILT: // code 3/6/8/12/16
                    switch (*data)
                    {
                        case 3: *data = 4; break;
                        case 6: *data = 0; break;
                        case 8: *data = 1; break;
                        case 12: *data = 2; break;
                        case 16: *data = 3; break;
                        default:
                            Serial.print(F("Invalid value '"));
                            Serial.print(*data);
                            Serial.println(F("' for BEMF_BLANK_FILT. Valid values are: 3, 6, 8, 12, 16"));
                            return false;
                    }
                    break;
            }
            break;
        case HALL:
            switch(index)
            {
                case HALL_INIT_DUTY: // 0-100
                    if((*data < 0) || (*data > 100))
                    {
                        Serial.print(F("Invalid value '"));
                        Serial.print(*data);
                        Serial.println(F("' for HALL_INIT_DUTY. Valid values are: 0-100"));
                        return false;
                    }
                    break;
                case HALL_INPUT_A: // 0/1/2
                    if(*data > 2)
                    {
                        Serial.print(F("Invalid value '"));
                        Serial.print(*data);
                        Serial.println(F("' for HALL_INPUT_A. Valid values are: 0/1/2"));
                        return false;
                    }
                    break;
                case HALL_INPUT_B: // 0/1/2
                    if(*data > 2)
                    {
                        Serial.print(F("Invalid value '"));
                        Serial.print(*data);
                        Serial.println(F("' for HALL_INPUT_B. Valid values are: 0/1/2"));
                        return false;
                    }
                    break;
                case HALL_INPUT_C: // 0/1/2
                    if(*data > 2)
                    {
                        Serial.print(F("Invalid value '"));
                        Serial.print(*data);
                        Serial.println(F("' for HALL_INPUT_C. Valid values are: 0/1/2"));
                        return false;
                    }
                    break;
                case HALL_OFFSET_60DEGREE_EN: // 0/1
                    if(*data > 1) *data = 1; break;
                case HALL_ANGLE_DELAY_EN: // 0/1
                    if(*data > 1) *data = 1; break;
                case HALL_DELAY_ANGLE: // 0-59
                    if(*data > 59)
                    {
                        Serial.print(F("Invalid value '"));
                        Serial.print(*data);
                        Serial.println(F("' for HALL_DELAY_ANGLE. Valid values are: 0-59"));
                        return false;
                    }
                    break;
                case HALL_DELAY_MINSPEED: // 0-2000
                    if(*data > 2000)
                    {
                        Serial.print(F("Invalid value '"));
                        Serial.print(*data);
                        Serial.println(F("' for HALL_DELAY_MINSPEED. Valid values are: 0-2000"));
                        return false;
                    }
                    break;
            }
            break;
        case FOC:
            switch(index)
            {
                case FOC_SPEED_KP: // >0
                    if(*data == 0)
                    {
                        Serial.print(F("Invalid value '"));
                        Serial.print(*data);
                        Serial.println(F("' for FOC_SPEED_KP. Valid values are: >0"));
                        return false;
                    }
                    break;
                case FOC_SPEED_KI: // >0
                    if(*data == 0)
                    {
                        Serial.print(F("Invalid value '"));
                        Serial.print(*data);
                        Serial.println(F("' for FOC_SPEED_KI. Valid values are: >0"));
                        return false;
                    }
                    break;
                case FOC_START_FREQ_ZERO: // 0/1
                    if(*data > 1) *data = 1; break;
            }
            break;
    }
    return true;
}

boolean TLE9879_Group::Board::checkfloatval(uint8_t mode, uint8_t index, float *data)
{
    switch(mode)
    {
        case BEMF:
            switch(index)
            {
                case BEMF_TIME_CONST_SPEED_FILT_TIME: // 0.01-1
                    if((*data < 0.01) || (*data > 1))
                    {
                        Serial.print(F("Invalid value '"));
                        Serial.print(*data);
                        Serial.println(F("' for BEMF_SPEED_FILT_TIME. Valid values are: 0.01-1."));
                        return false;
                    }
                    break;
                case BEMF_START_SPEED_PWM_MIN: // 0-0.95
                    if((*data < 0) || (*data > 0.95))
                    {
                        Serial.print(F("Invalid value '"));
                        Serial.print(*data);
                        Serial.println(F("' for BEMF_START_SPEED_PWM_MIN_OFFSET. Valid values are: 0-0.95."));
                        return false;
                    }
                    break;
                case BEMF_START_SPEED_PWM_MIN_OFFSET: // 0-0.95
                    if((*data < 0) || (*data > 0.95))
                    {
                        Serial.print(F("Invalid value '"));
                        Serial.print(*data);
                        Serial.println(F("' for BEMF_START_SPEED_PWM_MIN_OFFSET. Valid values are: 0-0.95."));
                        return false;
                    }
                    break;
                case BEMF_SPEED_BEGIN_PWM_MIN: // 0-0.95
                    if((*data < 0) || (*data > 0.95))
                    {
                        Serial.print(F("Invalid value '"));
                        Serial.print(*data);
                        Serial.println(F("' for BEMF_RUN_SPEED_PWM_MIN. Valid values are: 0-0.95."));
                        return false;
                    }
                    break;
                case BEMF_SPEED_PWM_MIN: // 0-0.95
                    if((*data < 0) || (*data > 0.95))
                    {
                        Serial.print(F("Invalid value '"));
                        Serial.print(*data);
                        Serial.println(F("' for BEMF_END_SPEED_PWM_MIN. Valid values are: 0-0.95."));
                        return false;
                    }
                    break;
            }
            break;

        case HALL:
            switch(index)
            {
                case HALL_SPEED_IMIN: // 0-100
                    if((*data < 0) || (*data > 100))
                    {
                        Serial.print(F("Invalid value '"));
                        Serial.print(*data);
                        Serial.println(F("' for HALL_SPEED_IMIN. Valid values are: 0-100."));
                        return false;
                    }
                    break;
                case HALL_SPEED_IMAX: // 0-100
                    if((*data < 0) || (*data > 100))
                    {
                        Serial.print(F("Invalid value '"));
                        Serial.print(*data);
                        Serial.println(F("' for HALL_SPEED_IMAX. Valid values are: 0-100."));
                        return false;
                    }
                    break;
                case HALL_SPEED_PIMIN: // 0-100
                    if((*data < 0) || (*data > 100))
                    {
                        Serial.print(F("Invalid value '"));
                        Serial.print(*data);
                        Serial.println(F("' for HALL_SPEED_PIMIN. Valid values are: 0-100."));
                        return false;
                    }
                    break;
                case HALL_SPEED_PIMAX: // 0-100
                    if((*data < 0) || (*data > 100))
                    {
                        Serial.print(F("Invalid value '"));
                        Serial.print(*data);
                        Serial.println(F("' for HALL_SPEED_PIMAX. Valid values are: 0-100."));
                        return false;
                    }
                    break;
            }
            break;

        case FOC:
            switch(index)
            {
                case FOC_CUR_ADJUST: // 0.01-1
                    if((*data < 0.01) || (*data > 1))
                    {
                        Serial.print(F("Invalid value '"));
                        Serial.print(*data);
                        Serial.println(F("' for FOC_CUR_ADJUST. Valid values are: 0.01-1"));
                        return false;
                    }
                    break;
                case FOC_FLUX_ADJUST: // 0.01-1
                    if((*data < 0.01) || (*data > 1))
                    {
                        Serial.print(F("Invalid value '"));
                        Serial.print(*data);
                        Serial.println(F("' for FOC_FLUX_ADJUST. Valid values are: 0.01-1"));
                        return false;
                    }
                    break;
                case FOC_SPEED_FILT_TIME: // 0.01-1
                    if((*data < 0.01) || (*data > 1))
                    {
                        Serial.print(F("Invalid value '"));
                        Serial.print(*data);
                        Serial.println(F("' for FOC_SPEED_FILT_TIME. Valid values are: 0.01-1"));
                        return false;
                    }
                    break;
                case FOC_MIN_NEG_REF_CUR: // <0
                    if(*data > 0)
                    {
                        Serial.print(F("Invalid value '"));
                        Serial.print(*data);
                        Serial.println(F("' for FOC_MIN_NEG_REF_CUR. Valid values are: <0"));
                        return false;
                    }
                    break;
                case FOC_MIN_CUR_SPEED: // <0
                    if(*data > 0)
                    {
                        Serial.print(F("Invalid value '"));
                        Serial.print(*data);
                        Serial.println(F("' for FOC_MIN_CUR_SPEED. Valid values are: <0"));
                        return false;
                    }
                    break;
                case FOC_MAX_NEG_REF_CUR: // <0
                    if(*data > 0)
                    {
                        Serial.print(F("Invalid value '"));
                        Serial.print(*data);
                        Serial.println(F("' for FOC_MAX_NEG_REF_CUR. Valid values are: <0"));
                        return false;
                    }
                    break;
            }
            break;
    }
    return true;
}

uint8_t TLE9879_Group::Board::getCurrentMode()
{
    return currentmode;
}
