
/*
 * TLE9879_Group.h
 *
 *  Created on: 19.02.2018
 *      Author: Haidari, Ahmad
 */

#ifndef TLE9879_FOURPACK_H_
#define TLE9879_FOURPACK_H_

#include "Arduino.h"

typedef bool boolean;

// --------------------DEFINES--------------------
// Constructor
#define BOARD1 		1
#define BOARD2 		2
#define BOARD3 		3
#define BOARD4 		4
#define ALL_BOARDS	5

// common
#define CONFIRM_OFFSET 		0x1000
#define SLAVESELECT_BOARD1 	4
#define SLAVESELECT_BOARD2 	5
#define SLAVESELECT_BOARD3 	6
#define SLAVESELECT_BOARD4 	7

// 0x01: modeControl
#define MODECONTROL 		0x0100
#define BOOTLOADER 			0x00
#define BEMF 				0x01
#define HALL 				0x02
#define FOC 				0x03
#define GETCURRENTMODE 		0x10

// 0x02: loadDataset
#define LOADDATASET 		0x0200
#define DATASET0 			0x00
#define DATASET1 			0x01
#define DATASET2 			0x02
#define DATASET3 			0x03
#define CUSTOMDATASET0 		0x10
#define CUSTOMDATASET1		0x11
#define CUSTOMDATASET2 		0x12
#define CUSTOMDATASET3 		0x13

// 0x03: readDataset
#define READDATASET 0x0300

// 0x04: writeDataset
#define WRITEDATASET 			0x0400
#define NUMBEROF_MESSAGES_FOC	(46u)
#define NUMBEROF_MESSAGES_BEMF	(30u)
#define NUMBEROF_MESSAGES_HALL	(22u)
#define NUMBEROF_BYTES_FOC		(NUMBEROF_MESSAGES_FOC * 2u)
#define NUMBEROF_BYTES_BEMF		(NUMBEROF_MESSAGES_BEMF * 2u)
#define NUMBEROF_BYTES_HALL		(NUMBEROF_MESSAGES_HALL * 2u)

// 0x05: changeParameter
#define CHANGEPARAMETER 				0x0500
// BEMF parameter: 0...29
#define	BEMF_PWM_FREQ 					0
#define	BEMF_SPEED_KP					2
#define	BEMF_SPEED_KI					3
#define	BEMF_TIME_CONST_SPEED_FILT_TIME	4
#define	BEMF_POLE_PAIRS					6
#define	BEMF_RESERVED_PADDING			7
#define	BEMF_ZERO_VEC_TIME				8
#define	BEMF_END_START_SPEED			10
#define	BEMF_START_ACCEL				12
#define	BEMF_RUN_ACCEL					14
#define	BEMF_START_FREQ_ZERO			16
#define	BEMF_SPEED_TEST_ENABLE			17
#define	BEMF_SWITCH_ON_SPEED			18
#define	BEMF_START_SPEED_PWM_MIN		20
#define	BEMF_START_SPEED_PWM_MIN_OFFSET	22
#define	BEMF_SPEED_BEGIN_PWM_MIN		24
#define BEMF_SPEED_PWM_MIN				26
#define	BEMF_SPIKE_FILT					28
#define	BEMF_BLANK_FILT					29

// Hall parameter: 30...52
#define HALLOFFSET 					30
#define HALL_PWM_FREQ				(0+HALLOFFSET)
#define HALL_POLE_PAIRS				(2+HALLOFFSET)
#define HALL_INIT_DUTY				(3+HALLOFFSET)
#define HALL_INPUT_A				(4+HALLOFFSET)
#define HALL_INPUT_B				(5+HALLOFFSET)
#define HALL_INPUT_C				(6+HALLOFFSET)
#define HALL_OFFSET_60DEGREE_EN		(7+HALLOFFSET)
#define HALL_ANGLE_DELAY_EN			(8+HALLOFFSET)
#define HALL_DELAY_ANGLE			(9+HALLOFFSET)
#define HALL_DELAY_MINSPEED			(10+HALLOFFSET)
#define HALL_SPEED_KP				(11+HALLOFFSET)
#define HALL_SPEED_KI				(12+HALLOFFSET)
#define HALL_RESERVED_PADDING		(13+HALLOFFSET)
#define HALL_SPEED_IMIN				(14+HALLOFFSET)
#define HALL_SPEED_IMAX				(16+HALLOFFSET)
#define HALL_SPEED_PIMIN			(18+HALLOFFSET)
#define HALL_SPEED_PIMAX			(20+HALLOFFSET)

// FOC parameter: 52...98
#define FOCOFFSET 					52
#define FOC_R_SHUNT					(0+FOCOFFSET)
#define FOC_NOM_CUR					(2+FOCOFFSET)
#define FOC_PWM_FREQ				(4+FOCOFFSET)
#define FOC_R_PHASE					(6+FOCOFFSET)
#define FOC_L_PHASE					(8+FOCOFFSET)
#define FOC_SPEED_KP				(10+FOCOFFSET)
#define FOC_SPEED_KI				(11+FOCOFFSET)
#define FOC_MAX_POS_REF_CUR			(12+FOCOFFSET)
#define FOC_MAX_NEG_REF_CUR			(14+FOCOFFSET)
#define FOC_MIN_POS_REF_CUR			(16+FOCOFFSET)
#define FOC_MIN_NEG_REF_CUR			(18+FOCOFFSET)
#define FOC_MAX_CUR_SPEED			(20+FOCOFFSET)
#define FOC_MIN_CUR_SPEED			(22+FOCOFFSET)
#define FOC_SPEED_FILT_TIME			(24+FOCOFFSET)
#define FOC_FLUX_ADJUST				(26+FOCOFFSET)
#define FOC_POLE_PAIRS				(29+FOCOFFSET)
#define FOC_START_CUR_IF			(30+FOCOFFSET)
#define FOC_ZERO_VEC_TIME			(32+FOCOFFSET)
#define FOC_END_START_SPEED			(34+FOCOFFSET)
#define FOC_START_ACCEL				(36+FOCOFFSET)
#define FOC_START_FREQ_ZERO			(38+FOCOFFSET)
#define FOC_RESERVED				(39+FOCOFFSET)
#define FOC_SWITCH_ON_SPEED			(40+FOCOFFSET)
#define FOC_CUR_ADJUST				(42+FOCOFFSET)
#define FOC_MAX_SPEED				(44+FOCOFFSET)

// TODO: kann man im ConfigWizard auf der ersten Seite nicht einstellen --> über SPI??
#define FOC_CSA_OFFSET				(28+FOCOFFSET)

// 0x06: saveDataset
#define SAVEDATASET 				0x0600
#define POSITION0 					0
#define POSITION1 					1
#define POSITION2 					2
#define POSITION3 					3

// 0x07: setMotorspeed
#define SETMOTORSPEED				0x0700

// 0x08: motorControl
#define MOTORCONTROL 				0x0800
#define START_MOTOR 				0x00
#define STOP_MOTOR 					0x01

// 0x09: boardControl
#define BOARDCONTROL 				0x0900
#define AUTOADDRESSING 				0x00
#define BOARD_AVAILABLE 			0x01
#define RESET 						0x02

// 0x0A: LEDOn
#define LED							0x0E00
#define LED_ON 						0x0A00
#define LED_RED 					0x02
#define LED_GREEN 					0x03
#define LED_BLUE 					0x13
#define COLOR_BLACK					0x00
#define COLOR_BLUE					0x01
#define COLOR_GREEN					0x02
#define COLOR_TURQUOISE				0x03
#define COLOR_RED					0x04
#define COLOR_PURPLE				0x05
#define COLOR_YELLOW				0x06
#define COLOR_WHITE					0x07
#define COLOR_MAX_NUMBER			0x07

// 0x0B: LEDOff
#define LED_OFF 					0x0B00

// 0x0C: Check for error
#define CHECK_ERROR					0x0C00
#define ERR_NONE					0x00
#define ERR_STILL_IN_BOOTLOADER		0x01
#define ERR_CHECKSUM_IS_WRONG		0x02
#define ERR_NOT_AVAILABLE			0x03
#define ERR_MODE_READING_FAILED		0x04
#define ERR_MODE_CHANGE_FAILED		0x05
#define ERR_INVALID_DATASET_NUMBER	0x06
#define ERR_INVALID_PARAMETER_INDEX	0x07
#define ERR_FAILED					0x08
#define ERR_INVALID_PARAMETER		0x09
#define ERR_INVALID_BOARD_NR		0x0A
#define ERR_FAILED_ANSWER			0x0B

#define COPYDATASET 0x0D00

// Error Handling Structure
typedef struct
{
	uint8_t code = ERR_NONE;						// possible error code of a function
	uint16_t additionalInfo[5] = { 0, 0, 0, 0, 0 };	// additional info can be stored here
} StatusInfo; 


// --------------------DATATYPES--------------------
// ----------BEMF----------
typedef struct
{
	float PWM_Frequency;			/* PWM Frequency */
	uint16_t SpeedPi_Kp;        	/* Speedcontroller Kp *64 */
	uint16_t SpeedPi_Ki;        	/* Speedcontroller Ki */
	float TimeConstantSpeedFilter;	/* Time constant for Speedfilter*/
	uint16_t PolePair;              /* Pol Pair counter */
	uint16_t ReservedForPadding;	/* For memory alignment purposes */
	float TimeSpeedzero;            /* Time for Speed zero */
	float StartSpeedEnd;       		/* Max Speed for Start */
	float StartSpeedSlewRate;  		/* Start Speed SlewRate */
	float RunSpeedSlewRate;  		/* Run Speed SlewRate */
	uint16_t EnableFrZero;          /* Enable Start width Frequenz=0*/
	uint16_t SpeedTestEnable;       /* Enable Speedtest*/
	float SpeedLevelSwitchOn;    	/* Speedlevel for switch on */
	float StartSpeedPwmMin;     	/* Speed pwm start min */
	float StartSpeedPwmMinOffset;   /* Speed pwm start min offset*/
	float SpeedBeginPwmMin;         /* Speed pwm run min */
	float SpeedPwmMin;          	/* Speed pwm run min */
} TEmo_Bemfpar_Cfg;

typedef struct
{
	TEmo_Bemfpar_Cfg emocfg_BEMF;
	uint16_t spike_filt;
	uint16_t blank_filt;
} Tdatastruct_BEMF;

typedef union
{
	uint16_t dataarray_BEMF[NUMBEROF_MESSAGES_BEMF];
	Tdatastruct_BEMF datastruct_BEMF;
} data_BEMF_union;

// ----------HALL----------
typedef struct
{
	float PWM_Frequency;
	uint16_t PolePair;
	uint16_t initDutyCycle;
	uint16_t inputA;
	uint16_t inputB;
	uint16_t inputC;
	uint16_t offset60degEn;
	uint16_t angleDelayEn;
	uint16_t delayAngle;
	uint16_t delayMinspeed;
	uint16_t SpeedPi_Kp;
	uint16_t SpeedPi_Ki;
	uint16_t ReservedForPadding;
	float speedIMin;
	float speedIMax;
	float speedPiMin;
	float speedPiMax;
} TEmo_Hallpar_Cfg;

typedef union
{
	uint16_t dataarray_HALL[NUMBEROF_MESSAGES_HALL];
	TEmo_Hallpar_Cfg datastruct_HALL;
} data_HALL_union;

// ----------FOC----------
typedef struct
{
    float Rshunt;
    float NominalCurrent;				/* Nominal Current */
    float PWM_Frequency;				/* PWM Frequency */
    float PhaseRes;						/* brief Phase resistance */
    float PhaseInd;						/* Phase inductance */
    uint16_t SpeedPi_Kp;				/* Speedcontroller Kp *64 */
    uint16_t SpeedPi_Ki;				/* Speedcontroller Ki */
    float MaxRefCurr;					/* MaxRefCurrent for SpeedController */
    float MinRefCurr;					/* MinRefCurrent for SpeedController */
    float MaxRefStartCurr;				/* MaxRefCurrent for SpeedController by Start*/
    float MinRefStartCurr;				/* MinRefCurrent for SpeedController by Start*/
    float SpeedLevelPos;				/* SpeedLevel for MaxRefCurrent for SpeedController by Start*/
    float SpeedLevelNeg;				/* SpeedLevel for MinRefCurrent for SpeedController by Start*/
    float TimeConstantSpeedFilter;		/* Time constant for Speedfilter*/
    float TimeConstantEstFluxFilter;	/* Time constant for estimator flux */
    uint16_t CsaOffset;					/* Offset of current sense amplifier */
    uint16_t PolePair;					/* Pol Pair counter */
    float StartCurrent;					/* Start Current */
    float TimeSpeedzero;				/* Time for Speed zero */
    float StartSpeedEnd;				/* Max Speed for Start */
    float StartSpeedSlewRate;			/* Start Speed SlewRate */
    uint16_t EnableFrZero;				/* Enable Start width Frequenz=0*/
    uint16_t reserved;
	float SpeedLevelSwitchOn;			/* Speedlevel for switch on */
    float AdjustmCurrentControl;		/* Adjustment for CurrentControl */
    float MaxSpeed;						/* Maximum Speed */
} TEmo_Focpar_Cfg;


typedef union
{
	uint16_t dataarray_FOC[NUMBEROF_MESSAGES_FOC];
	TEmo_Focpar_Cfg datastruct_FOC;
} data_FOC_union;

class TLE9879_Group
{
private:
	class Board;

	Board* boards[4] = { nullptr, nullptr, nullptr, nullptr };
	StatusInfo* status = new StatusInfo();
	uint8_t boardCount;
	
	void processStatusCodes(uint16_t action, uint8_t boardnr);
	void printAction(uint16_t action);
	bool checkBoardNumber(uint8_t boardnr, uint16_t action, uint8_t& startIndex, uint8_t& endIndex);
	void printBoardNr(uint8_t boardnr);

public:
	bool infoMessagesEnabled = true;

	TLE9879_Group(uint8_t boardCount);
	~TLE9879_Group();
	
	void startAutoAddressing();
	void resetAllBoards();
	void setMode(uint8_t mode, uint8_t boardnr = ALL_BOARDS, bool fastMode = false);
	void setMotorMode(uint8_t mode, uint8_t boardnr = ALL_BOARDS);
	void setMotorSpeed(float motorspeed, uint8_t boardnr = ALL_BOARDS);
	void setParameter(uint8_t parameter, float value, uint8_t boardnr = ALL_BOARDS);
	void saveDataset(uint8_t pos, uint8_t boardnr = ALL_BOARDS);
	void loadDataset(uint8_t pos, uint8_t boardnr = ALL_BOARDS);
	void readDataset(uint8_t boardnr = ALL_BOARDS);
	void writeDataset(uint8_t boardnr = ALL_BOARDS);
	void copyDataset(uint8_t mode, uint8_t from, uint8_t to = ALL_BOARDS);
	void sendMessageToAll(uint16_t data);
	void setLed(uint16_t led, uint16_t mode, uint8_t boardnr = ALL_BOARDS);
	void setLedColor(uint8_t color, uint8_t boardnr = ALL_BOARDS);
	void checkErrors();
	void testBlinky();
	//Board* getBoard(uint8_t index);
};

class TLE9879_Group::Board
{
private:
	uint8_t boardnr = 0;
	uint8_t slaveselectpin = 0;
	uint8_t currentmode = 0;
	boolean  board_available = false;
	
	int16_t motorspeed = 0;
	StatusInfo* status;
public:
	data_FOC_union* data_FOC = nullptr;
	data_BEMF_union* data_BEMF = nullptr;
	data_HALL_union* data_HALL = nullptr;
	
	static const char* const modenames[4];
	
	Board(uint8_t number, StatusInfo* info); 	// constructor
	~Board();									// destructor

	// functions for communication
	void sendMessage(uint16_t data); // select slave, send, deselect slave
	uint16_t readAnswer();
	boolean sendMessageAndCheckAnswer(uint16_t command);
	boolean isAvailable();
	//void start();

	// commands
	// 0x01: modeControl
	boolean modeControl(uint8_t mode);

	// 0x02: loadDataset
	boolean loadDataset(uint8_t datasetnr);

	//0x03: readDataset
	uint8_t readDataset(void);

	// 0x04: writeCompleteDataset
	boolean writeDataset(void);

	// 0x05: changeParameter
	boolean changeParameter(uint8_t index, float data);

	// 0x06: saveDataset
	boolean saveDataset(uint8_t position);

	// 0x07: setMotorspeed
	boolean setMotorspeed(float motorspeed);

	// 0x08: motorControl
	boolean motorControl(uint8_t command);

	// 0x09: autoAddressing
	boolean boardControl();

	// 0x0A: LEDOn
	boolean LEDOn(uint8_t led);

	// 0x0B: LEDOff
	boolean LEDOff(uint8_t led);

	// additional functions
	uint8_t isvalueinarray(uint8_t val, const uint8_t* indices_16bit, const uint8_t size);
	uint8_t CRC8(uint8_t* data, uint8_t len);
	boolean checkuint16val(uint8_t mode, uint8_t index, uint16_t* data);
	boolean checkfloatval(uint8_t mode, uint8_t index, float* data);
	int16_t getMotorspeed();
	uint8_t getCurrentMode();
	//boolean reset();
};

#endif
