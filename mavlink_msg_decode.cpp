#include "mavlink_msg_decode.h"
#include "stdafx.h"

uint64_t time_usec; /*< Timestamp (microseconds, synced to UNIX time or since system boot)*/
uint32_t integration_time_us; /*< Integration time in microseconds. Divide integrated_x and integrated_y by the integration time to obtain average flow. The integration time also indicates the.*/
float integrated_x; /*< Flow in radians around X axis (Sensor RH rotation about the X axis induces a positive flow. Sensor linear motion along the positive Y axis induces a negative flow.)*/
float integrated_y; /*< Flow in radians around Y axis (Sensor RH rotation about the Y axis induces a positive flow. Sensor linear motion along the positive X axis induces a positive flow.)*/
float integrated_xgyro; /*< RH rotation around X axis (rad)*/
float integrated_ygyro; /*< RH rotation around Y axis (rad)*/
float integrated_zgyro; /*< RH rotation around Z axis (rad)*/
uint32_t time_delta_distance_us; /*< Time in microseconds since the distance was sampled.*/
float distance; /*< Distance to the center of the flow field in meters. Positive value (including zero): distance known. Negative value: Unknown distance.*/
int16_t temperature; /*< Temperature * 100 in centi-degrees Celsius*/
uint8_t sensor_id; /*< Sensor ID*/
uint8_t quality; /*< Optical flow quality / confidence. 0: no valid flow, 255: maximum quality*/
uint32_t Alttime_boot;

int16_t Pwork_data[1][10];
mavlink_heartbeat_t  heartbeat;
float attitude[7];
float pitch = 0,yaw=0,roll=0;

bool rise_allowed = 1, initial_flag = 1;
float initial_height = 0;

//GPS information 
float lat; /*< Latitude (WGS84), in degrees * 1E7*/
float lon; /*< Longitude (WGS84), in degrees * 1E7*/
float alt; /*< Altitude (AMSL, NOT WGS84), in meters * 1000 (positive for up). Note that virtually all GPS modules provide the AMSL altitude in addition to the WGS84 altitude.*/
//uint16_t eph; /*< GPS HDOP horizontal dilution of position (unitless). If unknown, set to: UINT16_MAX*/
//uint16_t epv; /*< GPS VDOP vertical dilution of position (unitless). If unknown, set to: UINT16_MAX*/
//uint16_t vel; /*< GPS ground speed (m/s * 100). If unknown, set to: UINT16_MAX*/
//uint16_t cog; /*< Course over ground (NOT heading, but direction of movement) in degrees * 100, 0.0..359.99 degrees. If unknown, set to: UINT16_MAX*/
//uint8_t fix_type; /*< See the GPS_FIX_TYPE enum.*/
uint8_t satellites_visible; /*< Number of satellites visible. If unknown, set to 255*/


uint8_t mavlink_msg_decode(int16_t(*Pwork_data)[10], mavlink_message_t msg)
{
	
	//hWnd = GetDlgItem(IDC_STATIC_D3DWND)->GetSafeHwnd();
	uint32_t custom_mode; /*< A bitfield for use for autopilot-specific flags.*/
	uint8_t type; /*< Type of the MAV (quadrotor, helicopter, etc., up to 15 types, defined in MAV_TYPE ENUM)*/
	int autopilot; /*< Autopilot type / class. defined in MAV_AUTOPILOT ENUM*/
	uint8_t base_mode; /*< System mode bitfield, see MAV_MODE_FLAG ENUM in mavlink/include/mavlink_types.h*/
	uint8_t system_status; /*< System status flag, see MAV_STATE ENUM*/
	uint8_t mavlink_version;
	float attitude[7];
	float relative_altitude, local_altitude, altitude_monotonic, altitude_amsl, altitude_terrain;
//	CRM_ComtrolDlg qqq;

	mavlink_optical_flow_rad_t optical_flow_rad;
	int16_t(*ppPwork_data)[10] = Pwork_data;
	float distance = 0.0;
	float test = 0;

	int index = 0;
	float q[4];
	printf("%d\n", msg.msgid);
	switch (msg.msgid)
	{
	case MAVLINK_MSG_ID_HEARTBEAT:
	{

		mavlink_msg_heartbeat_decode(&msg, &heartbeat);
		//system_type = heartbeat.type;
		//autopilot_type = heartbeat.autopilot;
		//target_system = system_type;
		//target_component = autopilot_type;



		/**  custom_mode = heartbeat.custom_mode;/*< A bitfield for use for autopilot-specific flags.*/
		//  type=heartbeat.type; /*< Type of the MAV (quadrotor, helicopter, etc., up to 15 types, defined in MAV_TYPE ENUM)*/
		//  autopilot = mavlink_msg_heartbeat_get_autopilot(&msg); /*< Autopilot type / class. defined in MAV_AUTOPILOT ENUM*/
		//	  base_mode=heartbeat.base_mode; /*< System mode bitfield, see MAV_MODE_FLAG ENUM in mavlink/include/mavlink_types.h*/
		//  system_status=heartbeat.system_status; /*< System status flag, see MAV_STATE ENUM*/
		// mavlink_version=heartbeat.mavlink_version;*/


		break;
	}
	case MAVLINK_MSG_ID_SET_ATTITUDE_TARGET:     //没有四元数
	{
		ppPwork_data[index][0] = (int16_t)mavlink_msg_set_attitude_target_get_time_boot_ms(&msg);
		ppPwork_data[index][1] = (int16_t)mavlink_msg_set_attitude_target_get_target_system(&msg);
		ppPwork_data[index][2] = (int16_t)mavlink_msg_set_attitude_target_get_target_component(&msg);
		ppPwork_data[index][3] = (int16_t)mavlink_msg_set_attitude_target_get_type_mask(&msg);
		ppPwork_data[index][3] = (int16_t)mavlink_msg_set_attitude_target_get_q(&msg, q);
		ppPwork_data[index][4] = (int16_t)mavlink_msg_set_attitude_target_get_body_roll_rate(&msg);
		ppPwork_data[index][5] = (int16_t)mavlink_msg_set_attitude_target_get_body_pitch_rate(&msg);
		ppPwork_data[index][6] = (int16_t)mavlink_msg_set_attitude_target_get_body_yaw_rate(&msg);
		ppPwork_data[index][7] = (int16_t)mavlink_msg_set_attitude_target_get_thrust(&msg);
		break;
	}
	case MAVLINK_MSG_ID_SET_POSITION_TARGET_LOCAL_NED:
	{
		ppPwork_data[index][0] = (int16_t)mavlink_msg_set_position_target_local_ned_get_time_boot_ms(&msg);
		ppPwork_data[index][1] = (int16_t)mavlink_msg_set_position_target_local_ned_get_target_system(&msg);
		ppPwork_data[index][2] = (int16_t)mavlink_msg_set_position_target_local_ned_get_target_component(&msg);
		ppPwork_data[index][3] = (int16_t)mavlink_msg_set_position_target_local_ned_get_coordinate_frame(&msg);
		ppPwork_data[index][4] = (int16_t)mavlink_msg_set_position_target_local_ned_get_type_mask(&msg);
		ppPwork_data[index][5] = (int16_t)mavlink_msg_set_position_target_local_ned_get_x(&msg);
		ppPwork_data[index][6] = (int16_t)mavlink_msg_set_position_target_local_ned_get_y(&msg);
		ppPwork_data[index][7] = (int16_t)mavlink_msg_set_position_target_local_ned_get_z(&msg);
		ppPwork_data[index][8] = (int16_t)mavlink_msg_set_position_target_local_ned_get_vx(&msg);
		ppPwork_data[index][9] = (int16_t)mavlink_msg_set_position_target_local_ned_get_afx(&msg);
		break;
	}
	case MAVLINK_MSG_ID_ATTITUDE:
	{
		attitude[0] = mavlink_msg_attitude_get_roll(&msg);
		attitude[1] = mavlink_msg_attitude_get_pitch(&msg);
		attitude[2] = mavlink_msg_attitude_get_yaw(&msg);
		attitude[3] = mavlink_msg_attitude_get_rollspeed(&msg);
		attitude[4] = mavlink_msg_attitude_get_pitchspeed(&msg);
		attitude[5] = mavlink_msg_attitude_get_yawspeed(&msg);
    	pitch= attitude[1];
		roll = attitude[0];
		yaw = attitude[2];
		Alttime_boot = mavlink_msg_attitude_get_time_boot_ms(&msg);
		 


		break;
	}
	case MAVLINK_MSG_ID_RAW_IMU:
	{
		ppPwork_data[index][0] = (int16_t)mavlink_msg_raw_imu_get_xacc(&msg);
		ppPwork_data[index][1] = (int16_t)mavlink_msg_raw_imu_get_yacc(&msg);
		ppPwork_data[index][2] = (int16_t)mavlink_msg_raw_imu_get_zacc(&msg);
		ppPwork_data[index][3] = (int16_t)mavlink_msg_raw_imu_get_xgyro(&msg);
		ppPwork_data[index][4] = (int16_t)mavlink_msg_raw_imu_get_ygyro(&msg);
		ppPwork_data[index][5] = (int16_t)mavlink_msg_raw_imu_get_zgyro(&msg);
		ppPwork_data[index][6] = (int16_t)mavlink_msg_raw_imu_get_xmag(&msg);
		ppPwork_data[index][7] = (int16_t)mavlink_msg_raw_imu_get_ymag(&msg);
		ppPwork_data[index][8] = (int16_t)mavlink_msg_raw_imu_get_zmag(&msg);
		break;
	}
	case MAVLINK_MSG_ID_VFR_HUD:
	{
		ppPwork_data[index][0] = (int16_t)mavlink_msg_vfr_hud_get_airspeed(&msg);
		ppPwork_data[index][1] = (int16_t)mavlink_msg_vfr_hud_get_groundspeed(&msg);
		ppPwork_data[index][2] = (int16_t)mavlink_msg_vfr_hud_get_heading(&msg);
		ppPwork_data[index][3] = (int16_t)mavlink_msg_vfr_hud_get_alt(&msg);
		break;
	}
	case MAVLINK_MSG_ID_GLOBAL_POSITION_INT:
	{
		ppPwork_data[index][0] = ((int16_t)mavlink_msg_global_position_int_get_lat(&msg)) / 10000000.0;
		ppPwork_data[index][1] = ((int16_t)mavlink_msg_global_position_int_get_lon(&msg)) / 10000000.0;
		ppPwork_data[index][2] = ((int16_t)mavlink_msg_global_position_int_get_alt(&msg)) / 1000.0;
		ppPwork_data[index][3] = ((int16_t)mavlink_msg_global_position_int_get_vx(&msg)) / 100.0;
		ppPwork_data[index][4] = ((int16_t)mavlink_msg_global_position_int_get_vy(&msg)) / 100.0;
		ppPwork_data[index][5] = ((int16_t)mavlink_msg_global_position_int_get_vz(&msg)) / 100.0;
		break;
	}
	case MAVLINK_MSG_ID_RC_CHANNELS_SCALED:
	{
		ppPwork_data[index][0] = ((int16_t)mavlink_msg_rc_channels_scaled_get_chan1_scaled(&msg)) / 1000;
		ppPwork_data[index][1] = ((int16_t)mavlink_msg_rc_channels_scaled_get_chan2_scaled(&msg)) / 1000;
		ppPwork_data[index][2] = ((int16_t)mavlink_msg_rc_channels_scaled_get_chan3_scaled(&msg)) / 1000;
		ppPwork_data[index][3] = ((int16_t)mavlink_msg_rc_channels_scaled_get_chan4_scaled(&msg)) / 1000;
		ppPwork_data[index][4] = ((int16_t)mavlink_msg_rc_channels_scaled_get_chan5_scaled(&msg)) / 1000;
		ppPwork_data[index][5] = ((int16_t)mavlink_msg_rc_channels_scaled_get_chan6_scaled(&msg)) / 1000;
		ppPwork_data[index][6] = ((int16_t)mavlink_msg_rc_channels_scaled_get_chan7_scaled(&msg)) / 1000;
		ppPwork_data[index][7] = ((int16_t)mavlink_msg_rc_channels_scaled_get_chan8_scaled(&msg)) / 1000;
		break;
	}
	case MAVLINK_MSG_ID_SERVO_OUTPUT_RAW:
	{
		ppPwork_data[index][0] = (int16_t)mavlink_msg_servo_output_raw_get_servo1_raw(&msg);
		ppPwork_data[index][1] = (int16_t)mavlink_msg_servo_output_raw_get_servo2_raw(&msg);
		ppPwork_data[index][2] = (int16_t)mavlink_msg_servo_output_raw_get_servo3_raw(&msg);
		ppPwork_data[index][3] = (int16_t)mavlink_msg_servo_output_raw_get_servo4_raw(&msg);
		break;
	}
	//             Weihua, Vicon data begin
	case MAVLINK_MSG_ID_VICON_POSITION_ESTIMATE:
	{
		ppPwork_data[index][0] = (int16_t)mavlink_msg_vicon_position_estimate_get_x(&msg);
		ppPwork_data[index][1] = (int16_t)mavlink_msg_vicon_position_estimate_get_y(&msg);
		ppPwork_data[index][2] = (int16_t)mavlink_msg_vicon_position_estimate_get_z(&msg);
		ppPwork_data[index][3] = (int16_t)mavlink_msg_vicon_position_estimate_get_roll(&msg);
		ppPwork_data[index][4] = (int16_t)mavlink_msg_vicon_position_estimate_get_pitch(&msg);
		ppPwork_data[index][5] = (int16_t)mavlink_msg_vicon_position_estimate_get_yaw(&msg);
		break;
	}

	case MAVLINK_MSG_ID_ALTITUDE:
	{

		relative_altitude = (int16_t)mavlink_msg_altitude_get_altitude_relative(&msg);
		local_altitude = (int16_t)mavlink_msg_altitude_get_altitude_local(&msg);
		altitude_monotonic = (int16_t)mavlink_msg_altitude_get_altitude_monotonic(&msg);
		altitude_amsl = (int16_t)mavlink_msg_altitude_get_altitude_amsl(&msg);
		altitude_terrain = (int16_t)mavlink_msg_altitude_get_altitude_terrain(&msg);
		//	printf("relative_altitude=%f\nlocal_altitude=%f\naltitude_monotonic=%f\naltitude_amsl=%f\naltitude_terrain;%f\n",relative_altitude,local_altitude,altitude_monotonic,altitude_amsl,altitude_terrain);
		if (initial_flag)
		{
			initial_height = local_altitude;
			initial_flag = 0;
		}
		if (local_altitude - initial_height > 3)  rise_allowed = 0;


		break;
	}


	case MAVLINK_MSG_ID_OPTICAL_FLOW_RAD:
	{
		distance = mavlink_msg_optical_flow_rad_get_distance(&msg);
		mavlink_msg_optical_flow_rad_decode(&msg, &optical_flow_rad);
		break;
	}
	case MAVLINK_MSG_ID_GPS_RAW_INT:
	{

		lat = (float)mavlink_msg_gps_raw_int_get_lat(&msg)/10000000.0;
		lon=(float)mavlink_msg_gps_raw_int_get_lon(&msg)/10000000.0;
		alt = (float)mavlink_msg_gps_raw_int_get_alt(&msg)/1000.0;
		satellites_visible=mavlink_msg_gps_raw_int_get_satellites_visible(&msg);

		break;
	}


	case 113:
	{
		distance = 0;
		break; 
	}

	case 124:
	{
		distance = 0;
		break;
	}



		default:
		break;
	}

	return (uint8_t)msg.msgid;
}