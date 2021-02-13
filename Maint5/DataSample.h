//{{AFX_INCLUDES()
#include "mscomm.h"
//}}AFX_INCLUDES
#if !defined(AFX_DATASAMPLE_H__556B9F42_C0AD_11D6_9644_00E04C50B69B__INCLUDED_)
#define AFX_DATASAMPLE_H__556B9F42_C0AD_11D6_9644_00E04C50B69B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DataSample.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDataSample dialog

class CDataSample : public CDialog
{
// Construction
public:
//	CWinThread *g_pThread_DataSample;

	CStatusBar *pStatus;

	void SendCommand(unsigned char *CommandStr,int SendNum );
	bool	bRecDataToFile;
	bool	bCloseFile;
	CFile	frecfile;
	long	m_lworkCounter;
	long	m_lrecCount;
	long	m_lrecFileCounter;
	unsigned char strSdotData[20];
	int		m_iModel_Sel;
	unsigned char m_recstrfile[1024*12];

	BOOL  m_EnClearBuff;	
	BOOL  m_bSample_UploadDataing;


	int m_PollingNo;
	int m_progress_time;
	int m_sample_time;
	CProgressCtrl m_Sample_progress;

	CDataSample(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDataSample)
	enum { IDD = IDD_DATA_SAMPLE };

	CButton	m_Mod_Sel_Sdot;
	CButton m_Mod_Sel_Edot;
	CButton m_Mod_Sel_Tdot;
	CButton m_Mod_Sel_Fuhe0;
	CButton m_Mod_Sel_Fuhe1;

	CEdit	m_edit_SetTime;
	CEdit	m_edit_SetCount;
	CComboBox	m_Bucket_Sel;
	CComboBox	m_Block_Sel;
	CMSComm	m_MSComm;
	CString	m_Edit_recCounter;
	CString	m_strSetTime;
	CString	m_strSetCount;
	CString	m_Static_RecStatus;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDataSample)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDataSample)
	afx_msg void OnCommMscomm();
	virtual BOOL OnInitDialog();
	afx_msg void OnStartSample();
	afx_msg void OnStopSample();
	afx_msg void OnCounterRedraw();
	afx_msg void OnOKe();
	afx_msg void OnRadioSdot();
	afx_msg void OnRadioFuhe1();
	afx_msg void OnRadioFuhe0();
	afx_msg void OnRadioEdot();
	afx_msg void OnRadioTdot();
	afx_msg void OnUploadData();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnClose();
	afx_msg void OnDestroy();
	afx_msg void OnAutoSample();
	DECLARE_EVENTSINK_MAP()
	//}}AFX_MSG
	afx_msg LRESULT OnRecveiveFull(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DATASAMPLE_H__556B9F42_C0AD_11D6_9644_00E04C50B69B__INCLUDED_)
