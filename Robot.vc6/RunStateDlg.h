#if !defined(AFX_RUNSTATEDLG_H__2A5B26A4_49D8_429C_A9B1_36F35E03DBB5__INCLUDED_)
#define AFX_RUNSTATEDLG_H__2A5B26A4_49D8_429C_A9B1_36F35E03DBB5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000



// RunStateDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CRunStateDlg dialog

class CRunStateDlg : public CDialog
{
// Construction
public:
	void OnShowWindow(BOOL bShow, UINT nStatus);
	CRunStateDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CRunStateDlg)
	enum { IDD = IDD_RunState };
	//}}AFX_DATA
    UINT    m_ReturnShow[13];
	UINT	m_UintSensor1;
	UINT	m_UintSensor2;
	UINT	m_UintSensor3;
	UINT	m_UintSensor4;
	UINT	m_UintSensor21;
	UINT	m_UintSensor22;
	UINT	m_UintSensor23;
	UINT	m_UintSensor24;

	long	m_velocity1;
	long	m_velocity2;
	long	m_velocity3;
	long	m_velocity4;
	long	m_velocity5;
	long	m_velocity6;
	long	m_velocity7;
	long	m_velocity8;	
	
	int  	m_AD1;
	int 	m_AD2;
	UINT	m_AD3;
	int  	m_AD4;
	UINT	m_IO1;
	UINT	m_IO2;
	UINT	m_IO3;
	UINT	m_IO4;
	UINT	m_IO5;
	UINT	m_IO6;
	UINT	m_IO7;
	UINT	m_IO8;
	long	m_RevTurnPos1;
	long	m_RevTurnPos3;
	long	m_RevTurnPos5;
	long	m_RevTurnPos7;
	long	m_RevTurnPos2;
	long	m_RevTurnPos4;
	long	m_RevTurnPos6;
	long	m_RevTurnPos8;
	int  IR[11];
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRunStateDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CRunStateDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RUNSTATEDLG_H__2A5B26A4_49D8_429C_A9B1_36F35E03DBB5__INCLUDED_)
