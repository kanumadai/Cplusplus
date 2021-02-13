//{{AFX_INCLUDES()
#include "mscomm.h"
//}}AFX_INCLUDES
#if !defined(AFX_ENVIRMENTPARAM_H__7E3F04A1_C172_11D6_9644_00E04C50B69B__INCLUDED_)
#define AFX_ENVIRMENTPARAM_H__7E3F04A1_C172_11D6_9644_00E04C50B69B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EnvirmentParam.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CEnvirmentParam dialog

class CEnvirmentParam : public CDialog
{
// Construction
public:
	int m_iRecTempNo;
	float m_fdrawtempdata[590];
	void OnDrawTempLine(float tempvalue);
	int m_iSumTemp;
	int m_iArrayTemp[32];
	void PrtString(unsigned char *pdata, BOOL enshow);

	int iENPara_ShowLineNum;
	int m_loopnum;
	BOOL m_bEnQuery32Bucket;

	CStatusBar *pStatus;
	unsigned char m_uchStatusShow[100];
	unsigned char txdData[20];
	bool bKillTimerFinished;
	long irecCounter;
	unsigned char recData[100];

	int m_iTempHighEth;
	int m_iTempLowEth;

	float m_fSumVoltage;
	float m_fArrayVoltage[32];

	float m_fSumNegVoltage;
	float m_fArrayNegVoltage[32];


	bool m_bEnUcrcError;

	void SendCommand(unsigned char *CommandStr, int SendNum);
	CEnvirmentParam(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CEnvirmentParam)
	enum { IDD = IDD_ENVIRMENTPARAM };
	CEdit	m_EditENPara_Show;
	CComboBox	m_Bucket_Sel;
	CMSComm	m_MSComm;
	CString	m_strVali_Static;
	CString	m_strThem_Static;
	CString	m_strNvali_Static;
	CString	m_strENPara_Show;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEnvirmentParam)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CEnvirmentParam)
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnCommMscomm();
	afx_msg void OnButtStartQuery();
	afx_msg void OnOKe();
	afx_msg void OnSelchangeBucketSel();
	afx_msg void OnDropdownBucketSel();
	afx_msg void OnClose();
	afx_msg void OnDestroy();
	afx_msg void OnEnQuery32Bucket();
	afx_msg void OnClearshow();
	afx_msg void OnStopQuery();
	DECLARE_EVENTSINK_MAP()
	//}}AFX_MSG
	afx_msg LRESULT OnStatusShow(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ENVIRMENTPARAM_H__7E3F04A1_C172_11D6_9644_00E04C50B69B__INCLUDED_)
