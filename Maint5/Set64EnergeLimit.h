#if !defined(AFX_SET64ENERGELIMIT_H__BB46DD9C_B4F8_4BCD_9A63_46AF6A830EA4__INCLUDED_)
#define AFX_SET64ENERGELIMIT_H__BB46DD9C_B4F8_4BCD_9A63_46AF6A830EA4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Set64EnergeLimit.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSet64EnergeLimit dialog

class CSet64EnergeLimit : public CDialog
{
// Construction
public:

	CSet64EnergeLimit(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CSet64EnergeLimit)
	enum { IDD = IDD_SET64ENERGELIMIT_PARA };
	CButton	m_Corner_Check;
	CButton	m_Ring_4_Check;
	CButton	m_Ring_3_Check;
	CButton	m_Ring_2_Check;
	CButton	m_Ring_1_Check;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSet64EnergeLimit)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
public:
	BOOL m_bPersendSel;
		int m_i64EthPara[6][3]; //[5]:表示环线类型，[3]: 阈值 数据 等

protected:

	// Generated message map functions
	//{{AFX_MSG(CSet64EnergeLimit)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnPersendSel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SET64ENERGELIMIT_H__BB46DD9C_B4F8_4BCD_9A63_46AF6A830EA4__INCLUDED_)
