#pragma once


// CTL �Ի���

class CTL : public CDialog
{
	DECLARE_DYNAMIC(CTL)

public:
	CTL(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CTL();

// �Ի�������
	enum { IDD = MOUSE_CONTROL };

private:
	CRgn m_rgn;//window region


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
};
