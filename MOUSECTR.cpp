// MOUSECTR.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "pixhack_control.h"
#include "MOUSECTR.h"
#include "afxdialogex.h"
float refer_roll=0.036, refer_pitch, refer_yaw, refer_thrust=0.38;

// MOUSECTR �Ի���

IMPLEMENT_DYNAMIC(MOUSECTR, CDialogEx)

MOUSECTR::MOUSECTR(CWnd* pParent /*=NULL*/)
	: CDialogEx(MOUSECTR::IDD, pParent)
{

}

MOUSECTR::~MOUSECTR()
{
}

void MOUSECTR::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(MOUSECTR, CDialogEx)
	ON_WM_NCMOUSEMOVE()
	ON_WM_MOUSEMOVE()
	ON_WM_MOUSEHWHEEL()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEWHEEL()
END_MESSAGE_MAP()


// MOUSECTR ��Ϣ�������

BOOL MOUSECTR::OnInitDialog()
{
	

		// ����Բ�Ǿ�������
	MOUSECTR::SetFocus();
	CRect rc; // rounded rectangle 
	GetWindowRect(&rc); // window rect in screen coords

//	m_rgn.CreateRoundRectRgn(rc.left, rc.top, rc.right, rc.bottom,
	//	50, 50); // rounded rect w/50 pixel corners



	m_rgn.CreateEllipticRgn(50, 50, 500, 500);
//	m_rgn.

	//�����������
	SetWindowRgn(m_rgn, TRUE); // set window region to make rounded window

	return TRUE; // ���ǽ��������õ��ؼ������򷵻�TRUE
}

void MOUSECTR::OnNcMouseMove(UINT nHitTest, CPoint point)
{
	// TODO:  �ڴ������Ϣ�����������/�����Ĭ��ֵ

	CDialogEx::OnNcMouseMove(nHitTest, point);
}

int i;

void MOUSECTR::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO:  �ڴ������Ϣ�����������/�����Ĭ��ֵ
	i = point.x;
	refer_roll = 0.3*(point.x-275)/225;
	refer_pitch = 0.3*(point.y - 275) / 225;
	CDialogEx::OnMouseMove(nFlags, point);
}


void MOUSECTR::OnMouseHWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// �˹���Ҫ�� Windows Vista ����߰汾��
	// _WIN32_WINNT ���ű��� >= 0x0600��
	// TODO:  �ڴ������Ϣ�����������/�����Ĭ��ֵ
	//SetFoucs();
	
/*	refer_thrust = (float)zDelta;
	CDialogEx::OnMouseHWheel(nFlags, zDelta, pt);
*/}


void MOUSECTR::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO:  �ڴ������Ϣ�����������/�����Ĭ��ֵ
	MOUSECTR::SetFocus();
	CDialogEx::OnLButtonDown(nFlags, point);
}


BOOL MOUSECTR::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// TODO:  �ڴ������Ϣ�����������/�����Ĭ��ֵ
	if (zDelta > 0)
	{
		refer_thrust = refer_thrust + 0.01;
	}

	else
	{
		refer_thrust = refer_thrust - 0.01;
	}

	if (refer_thrust > 0.4) refer_thrust = 0.4;
	if (refer_thrust<0.35)  refer_thrust =0.35;

	CDialogEx::OnMouseHWheel(nFlags, zDelta, pt);
	return CDialogEx::OnMouseWheel(nFlags, zDelta, pt);
}
