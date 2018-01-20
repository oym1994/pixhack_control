#include "stdafx.h"
#include "pixhack_control.h"
#include "pixhack_controlDlg.h"
#include "afxdialogex.h"
#include"mavlink_msg_decode.h"
#include "radio2degree.h"
#include"control.h"
#include"resource.h"
#include"MOUSECTR.h"
#include "tchart1.h"
#include "CSeries.h"  
#include <Windows.h>
#include <WinBase.h>


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define title1_send 0xAA
#define title2_send 0xAF
#define title1_received 0xAA
#define title2_received 0xAF

#define BYTE0(dwTemp)       ( *( (char *)(&dwTemp) + 0) )
#define BYTE1(dwTemp)       ( *( (char *)(&dwTemp) + 1) )
#define BYTE2(dwTemp)       ( *( (char *)(&dwTemp) + 2) )
#define BYTE3(dwTemp)       ( *( (char *)(&dwTemp) + 3) )

enum FLYING_ORDER{ Hovering_Fly, Forward_Fly, Backward_Fly, Left_Fly, Right_Fly, LeftRotation_Fly, RightRotation_Fly, Ascend_Fly, Descend_Fly };
/*#define Forward_Fly  1
#define Backward_Fly  2 
#define Left_Fly  3 
#define Right_Fly  4 
#define LeftRotation_Fly  5 
#define RightRotation_Fly  6 
#define Ascend_Fly  7 
#define Descend_Fly  8 
#define Hovering_Fly 0
int Flying_Order = 0;                     //飞行指令，专用于脑控模式
*/

FLYING_ORDER Flying_Order = Hovering_Fly;
char* mmfm_base_address;
bool stop = 0;
int flag_disarm = 1, flag_arm = 1, a = 0, com_num = 2, num = 0;
CString temp;
extern float pitch, yaw, roll;
BOOL Rise = 0, fall = 0, left = 0, right = 0, backward = 0, forward = 0, Right_rotation = 0, Left_rotation = 0, Ready = 0, RC_Initianize = 0;
float vx, vy, vz;
uint8_t data_to_send[50], lose_control_flag = 0;;
extern bool rise_allowed;
uint16_t Channel1 = 1500, Channel2 = 1500, Channel3 = 1500, Channel4 = 1500, Channel5 = 1100, Channel6 = 1100, Channel7 = 1100, Channel8 = 1100;
uint16_t Keep_Control=0;




void Set_RCChannel(uint16_t m_1, uint16_t m_2, uint16_t m_3, uint16_t m_4, uint16_t m_5, uint16_t m_6, uint16_t m_7, uint16_t m_8)
{
	uint8_t _cnt = 0;
	data_to_send[_cnt++] = title1_send;
	data_to_send[_cnt++] = title2_send;
	data_to_send[_cnt++] = 0x06;
	data_to_send[_cnt++] = 0;
	data_to_send[_cnt++] = BYTE1(m_1);
	data_to_send[_cnt++] = BYTE0(m_1);
	data_to_send[_cnt++] = BYTE1(m_2);
	data_to_send[_cnt++] = BYTE0(m_2);
	data_to_send[_cnt++] = BYTE1(m_3);
	data_to_send[_cnt++] = BYTE0(m_3);
	data_to_send[_cnt++] = BYTE1(m_4);
	data_to_send[_cnt++] = BYTE0(m_4);
	data_to_send[_cnt++] = BYTE1(m_5);
	data_to_send[_cnt++] = BYTE0(m_5);
	data_to_send[_cnt++] = BYTE1(m_6);
	data_to_send[_cnt++] = BYTE0(m_6);
	data_to_send[_cnt++] = BYTE1(m_7);
	data_to_send[_cnt++] = BYTE0(m_7);
	data_to_send[_cnt++] = BYTE1(m_8);
	data_to_send[_cnt++] = BYTE0(m_8);
	data_to_send[3] = _cnt - 4;
	uint8_t sum = 0;
	for (uint8_t i = 0; i<_cnt; i++)
	sum += data_to_send[i];
	data_to_send[_cnt++] = sum;
	num = _cnt;
	Ready = 1;
	 
}

void Set_LoseControl_Mode(uint8_t lose_control_flag_)
{
	uint8_t _cnt = 0;
	data_to_send[_cnt++] = title1_send;
	data_to_send[_cnt++] = title2_send;
	data_to_send[_cnt++] = 0x01;
	data_to_send[_cnt++] = 0;
	data_to_send[_cnt++] = BYTE0(lose_control_flag_);
	data_to_send[3] = _cnt - 4;
	uint8_t sum = 0;
	for (uint8_t i = 0; i<_cnt; i++)
		sum += data_to_send[i];
	data_to_send[_cnt++] = sum;
	num = _cnt;
	Ready = 1;

}





class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    

protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


Cpixhack_controlDlg::Cpixhack_controlDlg(CWnd* pParent )
	: CDialogEx(Cpixhack_controlDlg::IDD, pParent)
	, pitch_string(_T(""))
	, roll_string(_T(""))
	, yaw_string(_T(""))
	, ref_pitch(_T(""))
	, ref_yaw(_T(""))
	, ref_thr(_T(""))
	, ref_roll(_T(""))
//	, Altitude_Chart(0)
, Vx_String(_T(""))
, Vy_String(_T(""))
, Vz_String(_T(""))
, Latitude_String(_T(""))
, Longitude_String(_T(""))
, Altitude_String(_T(""))
, Satalite_String(_T(""))
, RC1_String(_T(""))
, RC2_String(_T(""))
, RC3_String(_T(""))
, RC4_String(_T(""))
, RC5_String(_T(""))
, RC6_String(_T(""))
, RC7_String(_T(""))
, RC8_String(_T(""))
, Flying_State(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void Cpixhack_controlDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_MSCOMM1, m_mscomm);
	DDX_Control(pDX, OPEN, button_open);
	DDX_Control(pDX, COM, combox);
	DDX_Text(pDX, IDC_EDIT1, pitch_string);
	DDX_Text(pDX, IDC_EDIT2, roll_string);
	DDX_Text(pDX, IDC_EDIT3, yaw_string);
	DDX_Text(pDX, IDC_EDIT4, ref_pitch);
	DDX_Text(pDX, IDC_EDIT5, ref_yaw);
	DDX_Text(pDX, IDC_EDIT6, ref_thr);
	DDX_Text(pDX, IDC_EDIT7, ref_roll);
	DDX_Control(pDX, IDC_TCHART1, Altitude_Chart);
	DDX_Control(pDX, IDC_CHECK2, P_Control);
	DDX_Control(pDX, IDC_EDIT8, RollB);
	DDX_Control(pDX, IDC_EDIT9, PitchB);
	DDX_Control(pDX, IDC_CHECK3, PC_Control);
	DDX_Text(pDX, IDC_EDIT10, Vx_String);
	DDX_Text(pDX, IDC_EDIT11, Vy_String);
	DDX_Text(pDX, IDC_EDIT12, Vz_String);
	DDX_Control(pDX, IDC_CHECK4, GV_Control);
	DDX_Control(pDX, CONTROL_MODE, Control_Mode);
	DDX_Text(pDX, IDC_EDIT13, Latitude_String);
	DDX_Text(pDX, IDC_EDIT14, Longitude_String);
	DDX_Text(pDX, IDC_EDIT15, Altitude_String);
	DDX_Text(pDX, IDC_EDIT16, Satalite_String);
	DDX_Text(pDX, IDC_EDIT17, RC1_String);
	DDX_Text(pDX, IDC_EDIT18, RC2_String);
	DDX_Text(pDX, IDC_EDIT19, RC3_String);
	DDX_Text(pDX, IDC_EDIT20, RC4_String);
	DDX_Text(pDX, IDC_EDIT21, RC5_String);
	DDX_Text(pDX, IDC_EDIT23, RC6_String);
	DDX_Text(pDX, IDC_EDIT24, RC7_String);
	DDX_Text(pDX, IDC_EDIT22, RC8_String);
	DDX_Control(pDX, IDC_COMBO1, FlightMode_String);
	DDX_Control(pDX, IDC_CHECK5, Disarm_Check);
	DDX_Control(pDX, IDC_CHECK6, Kill_Mode);
	DDX_Control(pDX, IDC_CHECK7, Return_Mode);
	DDX_Control(pDX, IDC_COMBO2, Lose_Control_Protection);
	DDX_Control(pDX, IDC_CHECK8, Remote_Initianize);
	DDX_Text(pDX, IDC_EDIT26, Flying_State);
	DDX_Control(pDX, STOP, Stop_Check);
}

BEGIN_MESSAGE_MAP(Cpixhack_controlDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(OPEN, &Cpixhack_controlDlg::OnBnClickedOpen)
	ON_BN_CLICKED(SEARCH_COM, &Cpixhack_controlDlg::OnBnClickedCom)
	ON_CBN_SELCHANGE(COM, &Cpixhack_controlDlg::OnCbnSelchangeCom)
	ON_WM_TIMER()
	ON_EN_CHANGE(IDC_EDIT8, &Cpixhack_controlDlg::OnEnChangeEdit8)
	ON_EN_CHANGE(IDC_EDIT9, &Cpixhack_controlDlg::OnEnChangeEdit9)
	ON_CBN_SELCHANGE(CONTROL_MODE, &Cpixhack_controlDlg::OnCbnSelchangeMode)
	ON_CBN_SELCHANGE(IDC_COMBO1, &Cpixhack_controlDlg::OnCbnSelchangeCombo1)
	ON_BN_CLICKED(IDC_CHECK5, &Cpixhack_controlDlg::OnBnClickedCheck5)
	ON_BN_CLICKED(IDC_CHECK7, &Cpixhack_controlDlg::OnBnClickedCheck7)
	ON_BN_CLICKED(IDC_CHECK6, &Cpixhack_controlDlg::OnBnClickedCheck6)
	ON_CBN_SELCHANGE(IDC_COMBO2, &Cpixhack_controlDlg::OnCbnSelchangeCombo2)
	ON_BN_CLICKED(IDC_CHECK8, &Cpixhack_controlDlg::OnBnClickedCheck8)
	ON_BN_CLICKED(STOP, &Cpixhack_controlDlg::OnBnClickedStop)
	END_MESSAGE_MAP()



BOOL Cpixhack_controlDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}


	SetIcon(m_hIcon, TRUE);			
	SetIcon(m_hIcon, FALSE);		
//	SetTimer(1, 100, NULL);
	Control_Mode.AddString("键盘控制");
	Control_Mode.AddString("脑电控制");
	FlightMode_String.AddString("Stablize");
	FlightMode_String.AddString("Altitude");
	FlightMode_String.AddString("Pos_Hold");
	Lose_Control_Protection.AddString("Kill");
	Lose_Control_Protection.AddString("Return");
	Lose_Control_Protection.AddString("Land");

	return TRUE;  
}

void Cpixhack_controlDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}



void Cpixhack_controlDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); 

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//显示。
HCURSOR Cpixhack_controlDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void Cpixhack_controlDlg::OnBnClickedOpen()
{
	if (m_mscomm.get_PortOpen()) //如果串口是打开的，则行关闭串口
	{
		m_mscomm.put_PortOpen(FALSE);
		//AfxMessageBox(_T("串口  已关闭"));
		button_open.SetWindowTextA(_T("打开通讯"));
	}

	else
	{
		m_mscomm.put_CommPort(com_num); //选择COM1 
		m_mscomm.put_InBufferSize(1024); //接收缓冲区 
		m_mscomm.put_OutBufferSize(1024);//发送缓冲区 
		m_mscomm.put_InputLen(0);//设置当前接收区数据长度为0,表示全部读取 
		m_mscomm.put_InputMode(1);//以二进制方式读写数据
		m_mscomm.put_RThreshold(1);//接收缓冲区有1个及1个以上字符时，将引发接收数据的OnComm


		m_mscomm.put_Settings(_T("57600,n,8,1"));
		if (!m_mscomm.get_PortOpen())//如果串口没有打开则打开
		{
			m_mscomm.put_PortOpen(TRUE);//打开串口
			button_open.SetWindowTextA(_T("关闭通讯"));

		}
		else
		{
			m_mscomm.put_OutBufferCount(0);
			AfxMessageBox(_T("串口2打开失败"));
		}
	}
	
	///////////////////////////////


	const char* shared_name = "FSMState";
	//创建 
	HANDLE mmfm = ::OpenFileMappingA(FILE_MAP_ALL_ACCESS, FALSE, (LPCSTR)shared_name);
	while (mmfm == NULL)//反复创建直到成功   	
		mmfm = ::OpenFileMappingA(FILE_MAP_ALL_ACCESS, FALSE, (LPCSTR)shared_name);

	size_t view_size = 50;
	DWORD view_access = FILE_MAP_ALL_ACCESS;
	//获得映射视图
	 mmfm_base_address = (char*)MapViewOfFile(mmfm, view_access, 0, 0, view_size);
	if (mmfm_base_address == NULL)
	{
		DWORD error_code = GetLastError();
		if (error_code != 0)//0 is success
		{
			::MessageBoxA(NULL, "Please creat memory : FSMState.", "Attention", NULL);
			return ;
		}
			//cout<<"error code "<<error_code<<endl;
	}
	else
	{
		double  bb = 0;
		double* read_chars = &bb;
		//读数据
		memcpy(read_chars, mmfm_base_address, sizeof(double));

	}
	// TODO:  在此添加控件通知处理程序代码
}


void Cpixhack_controlDlg::OnBnClickedCom()
{
	FindCommPort(combox);
	// TODO:  在此添加控件通知处理程序代码
}


int Cpixhack_controlDlg::FindCommPort(CComboBox &cComboBox)
{
	HKEY hKey;
	int   iCount = 0;
	CString szFirstItemOfComPort;
	cComboBox.ResetContent();
	if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, _T("Hardware\\DeviceMap\\SerialComm"), NULL, KEY_READ, &hKey) == ERROR_SUCCESS)
	{
		const BYTE BUFFERSIZE = 255;
		char aPortName[BUFFERSIZE], aComName[BUFFERSIZE];
		DWORD dwLong = BUFFERSIZE, dwSize = BUFFERSIZE;

		while (RegEnumValue(hKey, iCount, aPortName, &dwLong, NULL, NULL, (PUCHAR)aComName, &dwSize) != ERROR_NO_MORE_ITEMS)
		{
			dwLong = dwSize = BUFFERSIZE;

			if (iCount == 0)
			{
				szFirstItemOfComPort = aComName;
			}

			cComboBox.AddString(aComName);
			++iCount;
		}
		RegCloseKey(hKey);
	}
	return (iCount);
}



BEGIN_EVENTSINK_MAP(Cpixhack_controlDlg, CDialogEx)
	ON_EVENT(Cpixhack_controlDlg, IDC_MSCOMM1, 1, Cpixhack_controlDlg::OnCommMscomm1, VTS_NONE)
END_EVENTSINK_MAP()


void LeftMoveArray(double* ptr, size_t length, double data)
{
	for (size_t i = 1; i<length; ++i)
	{
		ptr[i - 1] = ptr[i];
	}
	ptr[length - 1] = data;
}

void 	DrawLine_TeeChart(double*pitch_group, double*time_group, size_t length)
{
	for (int i = 0; i < 1000; i++)
	{



	}

}

const int length = 300;
double pitch_group[length];
double yaw_group[length];
double roll_group[length];
double time_group[length];
int ii = 0;

extern uint64_t time_usec;
extern int16_t Pwork_data[1][10];
//extern float pitch, roll, yaw;
extern float distance, integrated_x, integrated_y, integrated_xgyro, integrated_ygyro, integrated_zgyro;
extern uint8_t quality, msgid;
mavlink_message_t msg;
mavlink_status_t status;
char szBuff[2];
extern bool IsDataReady;
bool flag = 0;
extern uint32_t Alttime_boot;
extern float lat, alt, lon;
extern uint8_t satellites_visible;
void Cpixhack_controlDlg::OnCommMscomm1()
{
	static unsigned int cnt = 0;
	VARIANT variant_inp; COleSafeArray safearray_inp; long len, k;
	//	unsigned int data[1024]={0};
	byte rxdata[1024]; //设置 BYTE 数组
	CString strtemp;
	if (m_mscomm.get_CommEvent() == 2)
	{
		cnt++;
		variant_inp = m_mscomm.get_Input(); //读缓冲区消息 
		safearray_inp = variant_inp; ///变量转换 
		len = safearray_inp.GetOneDimSize();
		for (k = 0; k < len; k++)
		{
			safearray_inp.GetElement(&k, rxdata + k);
		}

		for (k = 0; k < len; k++)
		{
			szBuff[1] = '\0';
			szBuff[0] = (char)(rxdata[k] & 0xFF);
			if (mavlink_parse_char(MAVLINK_COMM_0, szBuff[0], &msg, &status))
			{
				flag = 1;
				mavlink_msg_decode(Pwork_data, msg);
				flag = 0;

			}
			}
	}


	if (msg.msgid == MAVLINK_MSG_ID_GPS_RAW_INT)
	{
		Latitude_String.Format("%f",lat);
		Longitude_String.Format("%f",lon);
		Altitude_String.Format("%f",alt);
		Satalite_String.Format("%d",satellites_visible);

	}



	if (msg.msgid == MAVLINK_MSG_ID_ATTITUDE)
	{
	
			ii = length;
			LeftMoveArray(pitch_group, length, pitch);
			LeftMoveArray(roll_group, length, roll);
			LeftMoveArray(yaw_group, length, yaw);
			LeftMoveArray(time_group, length, (double)Alttime_boot / 1000);
 	    	CSeries data0 = (CSeries)Altitude_Chart.Series(0);
			CSeries data1 = (CSeries)Altitude_Chart.Series(1);
			CSeries data2 = (CSeries)Altitude_Chart.Series(2);
        	data0.Clear();
			data1.Clear();
			data2.Clear();

		for (int i = 0; i < length; i++)
		{
			data0.AddXY(time_group[i], pitch_group[i], NULL, NULL);
			data1.AddXY(time_group[i], yaw_group[i], NULL, NULL);
			data2.AddXY(time_group[i], roll_group[i], NULL, NULL);

		}
	}


	pitch_string.Format("%f",  pitch);
	roll_string.Format("%f",  roll);
	yaw_string.Format("%f", yaw);
		UpdateData(FALSE);
}

CString  com;
void Cpixhack_controlDlg::OnCbnSelchangeCom()
{

	combox.GetLBText(combox.GetCurSel(), com);
	com = com.Mid(3, 2);
	com_num = _ttoi(com);

}






bool send_state = 0;
extern uint8_t BufSend[MAVLINK_MAX_PACKET_LEN];
extern uint16_t BufSendLen;
CString str[MAVLINK_MAX_PACKET_LEN];
CString test;
uint8_t aa;
int j = 0;
CString Output;
CByteArray train;
float pitch_balance = 0, roll_balance = 0;
 float refer_roll=0.035, refer_pitch, refer_yaw, refer_thrust=0.35;


 void Cpixhack_controlDlg::OnTimer(UINT_PTR nIDEvent)
 {

	 switch (nIDEvent)
	 {
	 case 4:                                                              
	 {
		 if (!RC_Initianize)
		 {
			 if (Channel5 == 1900)
			 {
				 if (Rise)
				 {
					 if (rise_allowed)
					 {
						 Channel3 = 1650;
						 Flying_State = "上升";
					 }
					 else
					 {
						 Channel3 = 1500;
						 Flying_State = "悬停";
					 }
					 if (Channel3 > 1700)
					 {
						 refer_thrust = 1700;
						 Flying_State = "上升";
					 }
				 }

				 else if (fall)

				 {
					 Channel3 = 1300;
					 if (Channel3 < 1000)  refer_thrust = 1000;
					 Flying_State = "下降";
				 }

				 else Channel3 = 1500;
			 }

			 else  Channel3 = 1000;


			 Channel1 = 1530;

			 if (right)
			 {
				 Channel1 = 1600;
				 Flying_State = "右飞";
			 }

			 else if (left)
			 {
				 Channel1 = 1380;
				 Flying_State = "左飞";

			 }

			 Channel2 = 1481;

			 if (backward)
			 {
				 Channel2 = 1600;
				 Flying_State = "后飞";

			 }

			 else if (forward)
			 {
				 Channel2 = 1400;
				 Flying_State = "前飞";
			 }


			 if (Disarm_Check.GetCheck() && flag_disarm < 50)
			 {
				 flag_disarm++;
			 }
			 else if (!Disarm_Check.GetCheck() && flag_arm < 50)
			 {
				 flag_arm++;
			 }

			 else
			 {
				 Channel4 = 1560;
				 if (Right_rotation)
				 {
					 Flying_State = "右旋";
					 Channel4 = 1660;

				 }
				 else if (Left_rotation)
				 {
					 Flying_State = "左旋";
					 Channel4 = 1460;
				 }
			 }

		 }

		 else
		 {
			 if (Rise)
			 {
				 Channel3 = Channel3 + 20;
				 if (Channel3 > 2000)  refer_thrust = 2000;
			 }

			 else if (fall)

			 {
				 Channel3 = Channel3 - 20;
				 if (Channel3 < 1000)  refer_thrust = 1000;
			 }


			 Channel1 = 1500;

			 if (right)
			 {
				 Channel1 = Channel1 + 20;
				 if (Channel1 > 2000)   Channel1 = 2000;
			 }

			 else if (left)
			 {
				 Channel1 = Channel1 - 20;
				 if (Channel1 <= 1000)   Channel1 = 1000;

			 }
			 Channel2 = 1500;

			 if (backward)
			 {
				 Channel2 = Channel2 - 20;

				 if (Channel2 <= 1000)   Channel2 = 1000;

			 }

			 else if (forward)
			 {

				 Channel2 = Channel2 + 20;
				 if (Channel2 >= 2000)   Channel2 = 2000;
			 }




			 Channel4 = 1500;
			 if (Right_rotation)
			 {
				 Channel4 = Channel4 + 20;

				 if (Channel4 >= 2000)   Channel4 = 2000;

			 }
			 else if (Left_rotation)
			 {
				 Channel4 = Channel4 - 20;
				 if (Channel2 < 1000)   Channel4 = 1000;
			 }
		 }
	 break;
	 }




	 case 5:
	 {
		 if (stop)
		 {
			 Channel1 = 1500;
			 Channel2 = 1500;
			 Channel3 = 1500;
			 Channel4 = 1500;
			 Flying_State = "悬停";
		 }

		 else
		 {
			 double  bb = 0;
			 double* read_chars = &bb;
			 //读数据
			 memcpy(read_chars, &mmfm_base_address, sizeof(double));
			 int BB = (int)bb;
			 Flying_Order = (FLYING_ORDER)BB;
			 switch (Flying_Order)
			 {
			 case Forward_Fly:
			 {
				 Channel2 = 1400;
				 Flying_State = "前飞";
				 break;
			 }

			 case Backward_Fly:
			 {
				 Channel2 = 1600;
				 Flying_State = "后飞";
				 break;
			 }
			 case Right_Fly:
			 {
				 Channel1 = 1600;
				 Flying_State = "右飞";
				 break;
			 }
			 case Left_Fly:
			 {
				 Channel1 = 1380;
				 Flying_State = "左飞";

				 break;
			 }

			 case Ascend_Fly:
			 {
				 if (rise_allowed)
				 {
					 Channel3 = 1650;
					 Flying_State = "上升";
				 }
				 else
				 {
					 Channel3 = 1500;
					 Flying_State = "悬停";
				 }
				 break;
			 }
			 
			 case Descend_Fly:
			 {
				 Channel3 = 1300;
				 if (Channel3 < 1000)  refer_thrust = 1000;
				 Flying_State = "下降";
				 break;
			 }

			 case LeftRotation_Fly:
			 {

				 Flying_State = "左旋";
				 Channel4 = 1460;
				 break;
			 }

			 case RightRotation_Fly:
			 {
				 Flying_State = "右旋";
				 Channel4 = 1660;
				 break;
			 }

			 case Hovering_Fly:
			 {
				 Channel1 = 1500;
				 Channel2 = 1500;
				 Channel3 = 1500;
				 Channel4 = 1500;
				 Flying_State = "悬停";
				 break;
			 }

			 default:
				 break;
			 }
		 }
		 UpdateData(FALSE);
		 break;
		 
	 }

	 


	default:
	{
		UpdateData(FALSE);
		CDialogEx::OnTimer(nIDEvent);
	}

	if (m_mscomm.get_PortOpen()) //
	{
		Set_RCChannel(Channel1, Channel2, Channel3, Channel4, Channel5, Channel6, Channel7, Channel8);
		RC1_String.Format("%d", Channel1);
		RC2_String.Format("%d", Channel2);
		RC3_String.Format("%d", Channel3);
		RC4_String.Format("%d", Channel4);
		RC5_String.Format("%d", Channel5);
		RC6_String.Format("%d", Channel6);
		RC7_String.Format("%d", Channel7);
		RC8_String.Format("%d", Channel8);
		for (int i = 0; i < num; i++)
		{
			train.Add(data_to_send[i]);
		}
		m_mscomm.put_Output(COleVariant(train));
	}
	train.RemoveAll();
	Ready = 0;
	UpdateData(FALSE);
	CDialogEx::OnTimer(nIDEvent);


 }


}
			
		 


	





BOOL Cpixhack_controlDlg::PreTranslateMessage(MSG* pMsg)
{


	if (pMsg->message == WM_KEYDOWN)
	{
		if (pMsg->wParam == VK_RIGHT && left == 0)
		{
			right = 1;
			return TRUE;
		}
		else	if (pMsg->wParam == VK_LEFT && right == 0)
		{
			left = 1;
			return TRUE;
		}

		else if (pMsg->wParam == VK_DOWN && forward == 0)
		{
			backward = 1;
			return TRUE;

		}

		else if (pMsg->wParam == VK_UP && backward == 0)
		{
			forward = 1;
			return TRUE;

		}

		else if (pMsg->wParam == 83 && Rise == 0)  //下降
		{
			fall = 1;
			return TRUE;
		}

		else if (pMsg->wParam == 87 && fall == 0)  //上升
		{
			Rise = 1;
			return TRUE;
		}

		else if (pMsg->wParam == 65 && Right_rotation == 0)  //上升
		{
			Left_rotation = 1;
			return TRUE;
		}

		else if (pMsg->wParam == 68 && Left_rotation == 0)  //上升
		{
			Right_rotation = 1;
			return TRUE;
		}
	}



	else if (pMsg->message == WM_KEYUP)
	{
		if (pMsg->wParam == VK_RIGHT)
		{

			right = 0;
			return TRUE;
		}
		else	if (pMsg->wParam == VK_LEFT)
		{
			left = 0;
			return TRUE;
		}

		else if (pMsg->wParam == VK_DOWN)
		{
			backward = 0;
			return TRUE;

		}

		else if (pMsg->wParam == VK_UP)
		{
			forward = 0;
			return TRUE;

		}

		else if (pMsg->wParam == 83)  //下降
		{
			fall = 0;
			return TRUE;
		}

		else if (pMsg->wParam == 87)  //上升
		{
			Rise = 0;
			return TRUE;
		}

		else if (pMsg->wParam == 65  )  //上升
		{
			Left_rotation = 0;
			return TRUE;
		}

		else if (pMsg->wParam == 68 )  //上升
		{
			Right_rotation = 0;
			return TRUE;
		}

	}


	return CDialogEx::PreTranslateMessage(pMsg);


}

CString RB;
void Cpixhack_controlDlg::OnEnChangeEdit8()
{

	RollB.GetWindowText(RB);
	roll_balance = atof(RB);


}

CString PB;
void Cpixhack_controlDlg::OnEnChangeEdit9()
{
	PitchB.GetWindowText(PB);
	pitch_balance = atof(PB);
}







void Cpixhack_controlDlg::OnCbnSelchangeMode()
{
	Control_Mode.GetLBText(Control_Mode.GetCurSel(), temp);

	  if (temp == "键盘控制")
	{
	//	KillTimer(4);
		KillTimer(5);
		//	a = 3;
		SetTimer(4, 100, NULL);
	}
	else if (temp == "脑电控制")
	{
		KillTimer(4);
	//	KillTimer(5);
   //	a = 7;
		SetTimer(5, 100, NULL);

	}
	}





void Cpixhack_controlDlg::OnCbnSelchangeCombo1()
{
	FlightMode_String.GetLBText(FlightMode_String.GetCurSel(), temp);
	if (temp == "Stablize")      Channel5 = 1100;
	if (temp == "Altitude")      Channel5 = 1500;
	if (temp == "Pos_Hold")      Channel5 = 1900;
 
	// TODO:  在此添加控件通知处理程序代码
}


void Cpixhack_controlDlg::OnBnClickedCheck5()
{
	if (!Disarm_Check.GetCheck())
	{
		flag_arm = 1;
		flag_disarm = 0;
		Channel3 = 1000;
		Channel4 = 1000;
	}

	else if (Disarm_Check.GetCheck() && Channel5 == 1100)
	{
		flag_disarm = 1;
		flag_arm = 0;
		Channel3=1000;
		Channel4=2000;
	}
	else if (Disarm_Check.GetCheck() && Channel5 != 1100)
	{
		MessageBox("请先置于Stablize模式");
	}
	// TODO:  在此添加控件通知处理程序代码
}


void Cpixhack_controlDlg::OnBnClickedCheck7()
{
	if (Return_Mode.GetCheck())  Channel7 = 1900;
	else   Channel7 = 1100;
}


void Cpixhack_controlDlg::OnBnClickedCheck6()
{
	if (Kill_Mode.GetCheck())  Channel6 = 1900;
	else   Channel6 = 1000;
}


void Cpixhack_controlDlg::OnCbnSelchangeCombo2()
{
	Lose_Control_Protection.GetLBText(Lose_Control_Protection.GetCurSel(), temp);
	if (temp == "Land")            lose_control_flag = 0;
	else if (temp == "Return")      lose_control_flag = 1;
	else if (temp == "Kill")      lose_control_flag = 2;
	if (m_mscomm.get_PortOpen()) //如果串口是打开的，则行关闭串口
	{
		Set_LoseControl_Mode(lose_control_flag);		 
		for (int i = 0; i < num; i++)
		{
			train.Add(data_to_send[i]);
		}
		if (Ready==1)	m_mscomm.put_Output(COleVariant(train));
		Ready = 0;
	}
	train.RemoveAll();
	

	// TODO:  在此添加控件通知处理程序代码
}


void Cpixhack_controlDlg::OnBnClickedCheck8()
{
	if (Remote_Initianize.GetCheck())  RC_Initianize = 1;
	else    RC_Initianize = 0;

	// TODO:  在此添加控件通知处理程序代码
}

void Cpixhack_controlDlg::OnBnClickedStop()
{
	if (Stop_Check.GetCheck()) 	stop = 1;
	else stop = 0;
	// TODO:  在此添加控件通知处理程序代码
}
