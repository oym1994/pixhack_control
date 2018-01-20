#pragma once


// MOUSECTR �Ի���

class MOUSECTR : public CDialogEx
{
	DECLARE_DYNAMIC(MOUSECTR)
//private :
//	CRgn m_rgn;

public:
	MOUSECTR(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~MOUSECTR();
	BOOL OnInitDialog();
private:
	CRgn m_rgn;//window region


// �Ի�������
	enum { IDD = MOUSE_CONTROL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnNcMouseMove(UINT nHitTest, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnMouseHWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
};
