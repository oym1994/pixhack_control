#include"control.h"
#include "stdafx.h"
using namespace std;
bool IsDataReady = 0;
uint8_t system_id = 0x01;
uint8_t component_id = 0x01;
extern float refer_roll,refer_pitch,refer_yaw, refer_thrust ;
float rollspeed = 0, pitchspeed = 0, yawspeed = 0;
float X = 3.1415, Y = 3.1415, Z = 0, x = 0, y = 0, z = 0, w = 0;
uint8_t system_type = 0x01;
uint8_t autopilot_type = 0x01;
uint8_t target_system = 0x01;
uint8_t target_component = 0x01;
extern float attitude[7];
extern float relative_altitude, local_altitude, altitude_monotonic, altitude_amsl, altitude_terrain;
uint8_t BufReceive[MAVLINK_MAX_PACKET_LEN];
uint8_t BufSend[MAVLINK_MAX_PACKET_LEN];
uint16_t BufSendLen = 0;
uint16_t BufReceiveLen = 0;
#define give_thrust 0.5;
extern float yaw;
extern BOOL Right_rotation,Left_rotation;
//extern float pitch_balance, roll_balance;
void fourElement()
{
	X = refer_roll;

	Y = refer_pitch;
	if(Right_rotation==0 && Left_rotation==0)	refer_yaw =yaw;
	Z = refer_yaw;

	x = sin(X / 2)*cos(Z / 2)*cos(Y / 2) - sin(Y / 2)*sin(Z / 2)*cos(X / 2);
	y = sin(Y / 2)*cos(Z / 2)*cos(X / 2) + cos(Y / 2)*sin(Z / 2)*sin(X / 2);
	z = cos(Y / 2)*sin(Z / 2)*cos(X / 2) - sin(Y / 2)*cos(Z / 2)*sin(X / 2);
	w = cos(Y / 2)*cos(Z / 2)*cos(X / 2) + sin(Y / 2)*sin(Z / 2)*sin(X / 2);

	return;
}

//extern float roll,pitch,yaw;
void move_forward()
{
	refer_roll = 0.03;//attitude[0]; 
	refer_pitch = -0.15;
	refer_yaw = attitude[2];
	X = refer_roll;
	Y = refer_pitch;
	Z = yaw;
	yawspeed = 0;
	refer_thrust = give_thrust;
	fourElement();
}

void move_back()
{
	refer_roll = 0.03;//attitude[0]; 
	refer_pitch = 0.15;
	refer_yaw = attitude[2];
	X = refer_roll;
	Y = refer_pitch;
	Z = refer_yaw;
	yawspeed = 0;
	refer_thrust = give_thrust;
	fourElement();
}

void move_left()
{
	refer_roll = -0.08;
	refer_pitch = 0;
	refer_yaw = attitude[2];
	X = refer_roll;
	Y = refer_pitch;
	Z = refer_yaw;
	yawspeed = 0;
	refer_thrust = give_thrust;
	fourElement();
}

void move_right()
{
	refer_roll = 0.2;
	refer_pitch = 0;
	refer_yaw = attitude[2];
	X = refer_roll;
	Y = refer_pitch;
	Z = refer_yaw;
	yawspeed = 0;
	refer_thrust = give_thrust;
	fourElement();
}

void stay()
{
	refer_roll = 0.03;
	refer_pitch = 0;
	refer_yaw = attitude[2];
	X = refer_roll;
	Y = refer_pitch;
	Z = refer_yaw;
	yawspeed = 0;
	refer_thrust = give_thrust;
	fourElement();
}

void spin_clockwise()
{
	refer_roll = 0.03;
	refer_pitch = 0;
	refer_yaw = attitude[2] * 1.05;
	yawspeed = 0.2;
	if (refer_yaw>3.1415)  refer_yaw = refer_yaw - 3.1415;
	X = refer_roll;
	Y = refer_pitch;
	Z = refer_yaw;
	refer_thrust = give_thrust;
	fourElement();

}

void spin_anticlockwise()
{
	refer_roll = 0.03;
	refer_pitch = 0;
	refer_yaw = attitude[2] * 0.95;
	yawspeed = -0.2;
	if (refer_yaw<0)  refer_yaw = refer_yaw + 3.1415;
	X = refer_roll;
	Y = refer_pitch;
	Z = refer_yaw;
	refer_thrust = give_thrust;
	fourElement();

}

void drop()
{
	refer_roll = 0.03;
	refer_pitch = 0;
	refer_yaw = attitude[2];
	yawspeed = 0;
	refer_thrust = 0.45;
	X = refer_roll;
	Y = refer_pitch;
	Z = refer_yaw;

	fourElement();

}

void rise()
{
	refer_roll = 0.03;
	refer_pitch = 0;
	refer_yaw = attitude[2];
	yawspeed = 0;
	refer_thrust = 0.5;
	X = refer_roll;
	Y = refer_pitch;
	Z = refer_yaw;

	fourElement();

}

extern int a;
int flag = 0;

uint16_t  chan1_raw, chan2_raw, chan3_raw, chan4_raw, chan5_raw, chan6_raw, chan7_raw, chan8_raw;
extern float vx, vy, vz;
int k = 0;
int f;
float q[4];
void Control()
{
	 
	switch (a)
	{
	case 1:
	{
		X = 0;
		Y = 0;
		Z = -3.1415;
		fourElement();  //X:  roll ;  Y :pitch ;Z :yaw
		q[0] = w; q[1] = x; q[2] = y; q[3] = z;
		mavlink_message_t msg;
		mavlink_msg_set_attitude_target_pack(system_type, autopilot_type, &msg, 0, system_type, autopilot_type, 00000, q, 0, 0, 0, refer_thrust);// rollspeed, pitchspeed, yawspeed, thrust);
		BufSendLen = mavlink_msg_to_send_buffer(BufSend, &msg);
		IsDataReady = 1;
		break;
	}
	case  2:
	{
		mavlink_message_t msg;
		mavlink_msg_heartbeat_pack(01, 01, &msg, system_type, autopilot_type, 0, 0, 1);
		BufSendLen = mavlink_msg_to_send_buffer(BufSend, &msg);
		IsDataReady = 1;
		a = 5;
		break;
	}
	//	mavlink_message_t msg;
	case 3:
	{
		mavlink_message_t msg;
		mavlink_msg_set_position_target_local_ned_pack(01, 01, &msg, MAV_FRAME_LOCAL_NED, system_type, autopilot_type, 0, 1, 0, 0, 0, vx, vy, vz, 0, 0, 0, 0, 0);
		//mavlink_msg_set_position_target_local_ned_pack(01, 01, &msg, 0, system_type, autopilot_type, 0, 0000111111000111, 0, 0, 0, vx, vy, vz, 0, 0, 0, 0, 0);
		//(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg,
		//uint32_t time_boot_ms, uint8_t target_system, uint8_t target_component, uint8_t coordinate_frame, uint16_t type_mask, float x, float y, float z, float vx, float vy, float vz, float afx, float afy, float afz, float yaw, float yaw_rate)
		BufSendLen = mavlink_msg_to_send_buffer(BufSend, &msg);
		IsDataReady = 1;
		break;
		//	_sleep(1 * 100);
	}
	case 4:
	{
		mavlink_message_t msg;
		mavlink_msg_set_mode_pack(01, 01, &msg, system_type, 1, 1);
		//最后两个参数(base_mode,custom_mode)(1,1)
		//
		BufSendLen = mavlink_msg_to_send_buffer(BufSend, &msg);
		IsDataReady = 1;
		a = 1;
		break;
	}


	case 6:
	{
		mavlink_message_t msg;
		mavlink_msg_request_data_stream_pack(01, 01, &msg, system_type, autopilot_type, 104, 1, 0);
		BufSendLen = mavlink_msg_to_send_buffer(BufSend, &msg);
		IsDataReady = 1;
		break;
	}


	case 5:
	{


		mavlink_message_t msg;
		mavlink_msg_rc_channels_override_pack(system_type, autopilot_type, &msg, system_type, autopilot_type, chan1_raw, chan2_raw, chan3_raw, chan4_raw, chan5_raw, chan6_raw, chan7_raw, chan8_raw);
		BufSendLen = mavlink_msg_to_send_buffer(BufSend, &msg);
		IsDataReady = 1;
		cout << chan1_raw << endl << chan2_raw << endl << chan3_raw << endl;
		chan1_raw = 1500;
		chan2_raw = 1500;
		chan3_raw = 1100;
		chan4_raw = 1500;
		//			_sleep(20);
//		a = 2;
		break;
	}

	case 7:
	{
		mavlink_message_t msg;

		mavlink_msg_set_position_target_global_int_pack(01, 01, &msg, MAV_FRAME_LOCAL_NED, system_type, autopilot_type, 0, 0x0000111111000111, 0, 0, 0, vx, vy, vz, 0, 0, 0, 0, 0);
		BufSendLen = mavlink_msg_to_send_buffer(BufSend, &msg);
		IsDataReady = 1;
		break;
		
	}

	default:
	{
		IsDataReady = 0;
	}

	}
}