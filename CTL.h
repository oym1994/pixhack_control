#pragma once


// CTL 对话框

class CTL : public CDialog
{
	DECLARE_DYNAMIC(CTL)

public:
	CTL(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CTL();

// 对话框数据
	enum { IDD = MOUSE_CONTROL };

private:
	CRgn m_rgn;//window region


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
};
