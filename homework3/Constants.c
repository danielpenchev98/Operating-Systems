#include "Constants.h"

const int typeSegments[]={ 0,0,1,2,0 };

//array with the names of all parameters
const char * const paramName[] = {
"device_name",
"rom_revision",
"serial_number",
"bd_addr_part0",
"bd_addr_part1",
"bd_pass_part0",
"serial_baudrate",
"audio_bitrate",
"sleep_period",
"serial_parity",
"serial_data_bit",
"serial_stop_bit",
"bd_pass_part1",
"rom_checksum_part0",
"rom_checksum_part1"
};

//size of that array
const int paramSize=15;

//regex,representing the value limits of the parameters
const char* const paramLimits[]=
{
  "^[a-zA-Z0-9_-]*$",
  "^[.a-zA-Z0-9_-]*$",
  "^[A-Z0-9]*$",
  "^[A-Z0-9:]*$",
  "^[A-Z0-9:]*$",
  "^[A-Z0-9]*$",
  "^1200$|^2400$|^4800$|^9600$|^19200$|^115200$",
  "^32$|^96$|^128$|^160$|^192$|^256$|^320$",
  "^100$|^500$|^1000$|^5000$|^10000$",
  "^N$|^E$|^O$",
  "^5$|^6$|^7$|^8$",
  "^0$|^1$",
  "^[a-z0-9]*$",
  "^[a-z0-9]*$",
  "^[a-z0-9]*$"
};
