
// pixhack_controlDlg.h : 头文件
//

#pragma once
#include "mscomm1.h"
#include "afxwin.h"
#include "tchart1.h"


// Cpixhack_controlDlg 对话框
class Cpixhack_controlDlg : public CDialogEx
{
// 构造
public:
	Cpixhack_controlDlg(CWnd* pParent = NULL);	// 标准构造函数
 //   BOOL  PreTranslateMessage(MSG* pMsg);
// 对话框数据
	enum { IDD = IDD_PIXHACK_CONTROL_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CMscomm1 m_mscomm;
	afx_msg void OnBnClickedOpen();
	CButton button_open;
	afx_msg void OnBnClickedCom();
	int FindCommPort(CComboBox &cComboBox);
	CComboBox combox;
	CString pitch_string;
	CString roll_string;
	CString yaw_string;
	DECLARE_EVENTSINK_MAP()
	void OnCommMscomm1();
	afx_msg void OnCbnSelchangeCom();
	//afx_msg void OnBnClickedCheck1();
	CButton Altitude_State;
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	CString ref_pitch;
	CString ref_yaw;
//	CEdit ref_thr;
	CString ref_thr;
	CString ref_roll;
	CTchart1 Altitude_Chart;
	afx_msg void OnBnClickedCheck2();
//	CButton Control;
	CButton P_Control;
//	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
//	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
//	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	CEdit RollB;
	afx_msg void OnEnChangeEdit8();
	afx_msg void OnEnChangeEdit9();
	CEdit PitchB;
	CButton PC_State;
	CButton PC_Control;
	afx_msg void OnBnClickedCheck3();
	CString Vx_String;
	CString Vy_String;
	CString Vz_String;
	CButton GV_Control;
//	afx_msg void OnBnClickedCheck4();
//	CComboBox Control_Mode;
	CComboBox Control_Mode;
//	CString msg_string;
	CString Latitude_String;
	CString Longitude_String;
	CString Altitude_String;
	CString Satalite_String;
	afx_msg void OnCbnSelchangeMode();
	CString RC1_String;
	CString RC2_String;
	CString RC3_String;
	CString RC4_String;
	CString RC5_String;
	CString RC6_String;
	CString RC7_String;
	CString RC8_String;
	CComboBox FlightMode_String;
	afx_msg void OnCbnSelchangeCombo1();
	CButton Disarm;
	afx_msg void OnBnClickedCheck5();
	CButton Disarm_Check;
	CButton Kill_Mode;
	CButton Return_Mode;
	afx_msg void OnBnClickedCheck7();
	afx_msg void OnBnClickedCheck6();
	CComboBox Lose_Control_Protection;
	afx_msg void OnCbnSelchangeCombo2();
	CButton Remote_Initianize;
	afx_msg void OnBnClickedCheck8();
//	afx_msg void OnBnClickedButton9();
//	afx_msg void OnBnClickedButton10();
//	afx_msg void OnEnChangeEdit17();
//	CEdit Flying_State;
	CString Flying_State;
	afx_msg void OnBnClickedStop();
	CButton Stop_Check;
};
