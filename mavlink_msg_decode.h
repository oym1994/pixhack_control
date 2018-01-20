#ifndef _MAVLINK_MSG_DECODE_
#define _MAVLINK_MSG_DECODE_
#include <stdio.h>
#include "afxwin.h"
#include "c_library_v1_master_mavlink/common/mavlink.h"
#include <iostream>
#include<conio.h>
uint8_t mavlink_msg_decode(int16_t(*Pwork_data)[10], mavlink_message_t msg);


#endif