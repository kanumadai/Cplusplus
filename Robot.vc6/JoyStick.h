#if !defined(AFX_JOYSTICK_H__E8923CEF_8F34_4958_A3C1_A7C3B3F3E3C2__INCLUDED_)
#define AFX_JOYSTICK_H__E8923CEF_8F34_4958_A3C1_A7C3B3F3E3C2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// JoyStick.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CJoyStick dialog
  
class CJoyStick : public CDialog
{
// Construction
public:
	void UpdataJoyStick1();
	void UpdataJoyStick();
	HRESULT InitDInput1(HWND hDlg);
	HRESULT UpdateInputState();
	CJoyStick(CWnd* pParent = NULL);   // standard constructor
	        // DInput joystick state 
	afx_msg void OnConnectJoy();
// Dialog Data
	//{{AFX_DATA(CJoyStick)
	enum { IDD = IDD_JoyStick };
	int		m_X1;
	int		m_X2;
	int		m_Y1;
	int		m_Y2;
	BOOL    m_IO[10];
 
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CJoyStick)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CJoyStick)

	afx_msg void OnRevData();
	afx_msg void OnRevData1();
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_JOYSTICK_H__E8923CEF_8F34_4958_A3C1_A7C3B3F3E3C2__INCLUDED_)
