// ACQCenterDlg.h : header file
//

#if !defined(AFX_ACQCENTERDLG_H__35618B7B_F867_4853_AD41_5A2F2C65DE54__INCLUDED_)
#define AFX_ACQCENTERDLG_H__35618B7B_F867_4853_AD41_5A2F2C65DE54__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CACQCenterDlg dialog

class CACQCenterDlg : public CDialog
{
// Construction
public:
	CACQCenterDlg(CWnd* pParent = NULL);	// standard constructor

	//COWS m_OWS;
	//CSIO m_SIO;

// Dialog Data
	//{{AFX_DATA(CACQCenterDlg)
	enum { IDD = IDD_ACQCENTER_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CACQCenterDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

public:
	void ShowTips();
//	BOOL SelfTest_Bucket();
	static void SystemReboot();
	BOOL SelfTest_FH();
	BOOL SelfTest_OWS(void);
	BOOL SelfTest_SIO(void);

	// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CACQCenterDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnDestroy();
	afx_msg void OnPauseDispCoininfo();
	afx_msg void OnPauseDispSioinfo();
	virtual void OnOK();
	//}}AFX_MSG
	afx_msg LRESULT OnSystemSelfTest(WPARAM wParam, LPARAM lPaarm);
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ACQCENTERDLG_H__35618B7B_F867_4853_AD41_5A2F2C65DE54__INCLUDED_)
