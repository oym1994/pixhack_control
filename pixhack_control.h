
// pixhack_control.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// Cpixhack_controlApp: 
// �йش����ʵ�֣������ pixhack_control.cpp
//

class Cpixhack_controlApp : public CWinApp
{
public:
	Cpixhack_controlApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern Cpixhack_controlApp theApp;