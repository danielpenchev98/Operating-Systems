#ifndef CONSTANTS_H
#define CONSTANTS_H

// bit position
// type of the segment
// mapping bit position -> position of value 

//First segment - text
#define DEVICE_NAME 0
#define DEVICE_NAME_SEG 0
#define DEVICE_NAME_VALUE_POS 0

#define ROM_REVISION 1
#define ROM_REVISION_SEG 0
#define ROM_REVISION_VALUE_POS 1

#define SERIAL_NUMBER 2
#define SERIAL_NUMBER_SEG 0
#define SERIAL_NUMBER_VALUE_POS 2

//Second segment - text
#define BD_ADDR_PART0 0
#define BD_ADDR_PART0_SEG 1
#define BD_ADDR_PART0_VALUE_POS 0

#define BD_ADDR_PART1 1
#define BD_ADDR_PART1_SEG 1
#define BD_ADDR_PART1_VALUE_POS 1

#define BD_PASS_PART0 2
#define BD_PASS_PART0_SEG 1
#define BD_PASS_PART0_VALUE_POS 2

//Third segment - numerical
#define SERIAL_BAUDRATE 0
#define SERIAL_BAUDRATE_SEG 2
#define SERIAL_BAUDRATE_VALUE_POS 0

#define AUDIO_BITRATE 1
#define AUDIO_BITRATE_SEG 2
#define AUDIO_BITRATE_VALUE_POS 1

#define SLEEP_PERIOD 2 
#define SLEEP_PERIOD_SEG 2
#define SLEEP_PERIOD_VALUE_POS 2

//Fourth segment - bits
#define SERIAL_PARITY 0
#define SERIAL_PARITY_SEG 3
#define SERIAL_PARITY_VALUE_POS 0

#define SERIAL_DATA_BIT 1
#define SERIAL_DATA_BIT_SEG 3
#define SERIAL_DATA_BIT_VALUE_POS 1

#define SERIAL_STOP_BIT 2
#define SERIAL_STOP_BIT_SEG 3
#define SERIAL_STOP_BIT_VALUE_POS 2

//Fifth segment - text
#define BD_PASS_PART1 0
#define BD_PASS_PART1_SEG 4
#define BD_PASS_PART1_VALUE_POS 0

#define ROM_CHECKSUM_PART0 1
#define ROM_CHECKSUM_PART0_SEG 4
#define ROM_CHECKSUM_PART0_VALUE_POS 1

#define ROM_CHECKSUM_PART1 2
#define ROM_CHECKSUM_PART1_SEG 4
#define ROM_CHECKSUM_PART1_VALUE_POS 2

#define NUMBER_OF_SEGMENTS 5

extern const int typeSegments[];

//array with the names of all parameters
extern const char * const paramName[];

//size of that array
extern const int paramSize;

//regex,representing the value limits of the parameters
extern const char* const paramLimits[];

//segment info
#define META_INFO_LENGTH 8
#define BITS_META_INFO_LENGTH 7
#define VALUE_INFO_LENGTH 56
#define SEGMENT_LENGTH 64


#define TEXT_DATA_TYPE 16
#define NUMERIC_DATA_TYPE 4
#define BYTE_DATA_TYPE 1

#endif