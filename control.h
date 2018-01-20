#ifndef _CONTROL_H
#define _CONTROL_H
#include <iostream>
#include <stdio.h>
#include<conio.h>
#include "afxwin.h"
#include<stdint.h>
#include "c_library_v1_master_mavlink/common/mavlink.h"
void fourElement(void);
void move_forward(void);
void move_back(void);
void move_left(void);
void move_right(void);
void stay(void);
void spin_clockwise(void);
void spin_anticlockwise(void);
void drop(void);
void rise(void);
void Control(void);


#endif