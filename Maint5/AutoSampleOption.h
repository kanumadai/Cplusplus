#if !defined(AFX_AUTOSAMPLEOPTION_H__9D1DAE3C_6971_43B7_9EEA_9847BC02E4A9__INCLUDED_)
#define AFX_AUTOSAMPLEOPTION_H__9D1DAE3C_6971_43B7_9EEA_9847BC02E4A9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AutoSampleOption.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CAutoSampleOption dialog

class CAutoSampleOption : public CDialog
{
// Construction
public:
	int m_arrSampleModSel[5];		
	int m_iSampleTime;
	int m_iReSampleNum;
	int m_arrAutoSampParaSel[10];

public:
	

	CAutoSampleOption(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CAutoSampleOption)
	enum { IDD = IDD_AUTOSAMPLE_OPTION };
	CButton	m_Para_TimeSerier;
	CButton	m_Para_TimeModify;
	CButton	m_Para_Position_Limit;
	CButton	m_Para_Energe_Limit;
	CButton	m_Para_Cysnum;
	CButton	m_Para_Anol_Para;
	CButton	m_Check_Time;
	CButton	m_Check_Energe;
	CButton	m_Check_Fuhe1;
	CButton	m_Check_Fuhe2;
	CButton	m_Check_Sdot;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAutoSampleOption)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CAutoSampleOption)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_AUTOSAMPLEOPTION_H__9D1DAE3C_6971_43B7_9EEA_9847BC02E4A9__INCLUDED_)
