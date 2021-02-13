//{{AFX_INCLUDES()
#include "mscomm.h"
//}}AFX_INCLUDES
#if !defined(AFX_COMMSET_H__556B9F41_C0AD_11D6_9644_00E04C50B69B__INCLUDED_)
#define AFX_COMMSET_H__556B9F41_C0AD_11D6_9644_00E04C50B69B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CommSet.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCommSet dialog

class CCommSet : public CDialog
{
// Construction
public:
	void OnOutputFormat();
//	CWinThread *g_pThread_CommSet;

	CString m_strOutMessage;

	CStatusBar *pStatus;
	BOOL	m_bCommOpened;
	unsigned char m_uchStatusShow[100];
	int		m_RecStatus;
	int		m_PollingNo;
	bool	m_bManulCheck;
	long	lrecCounter;
	int		m_iRecBuckNo;
	int		m_OutputFormat[32];

	bool	m_bEnUcrcError;

	CProgressCtrl m_CommSet_progress;

	unsigned char crecData[200];
	void SendCommand(unsigned char *CommandStr, int SendNum);

	CCommSet(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CCommSet)
	enum { IDD = IDD_COMMSET };
	CComboBox	m_Object_Sel;
	CComboBox	m_Bucket_Sel;
	CComboBox	m_StopBit_Sel;
	CComboBox	m_PortSet_Sel;
	CComboBox	m_DataBit_Sel;
	CComboBox	m_Baud_Set;
	CMSComm	m_MSComm;
	CString	m_PowerOn_Check;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCommSet)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CCommSet)
	virtual BOOL OnInitDialog();
	afx_msg void OnOKe();
	afx_msg void OnCancel();
	afx_msg void OnPoweronCheck();
	afx_msg void OnCommMscomm();
	afx_msg void OnClose();
	afx_msg void OnPaint();
	afx_msg void OnComsetManulCheck();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnDestroy();
	afx_msg void OnSelchangeBucketSel();
	afx_msg void OnSelchangeComboSelobject();
	DECLARE_EVENTSINK_MAP()
	//}}AFX_MSG
	afx_msg LRESULT OnStatusShow(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_COMMSET_H__556B9F41_C0AD_11D6_9644_00E04C50B69B__INCLUDED_)
