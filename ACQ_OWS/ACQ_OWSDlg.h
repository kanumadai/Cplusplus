// ACQ_OWSDlg.h : header file
//

#if !defined(AFX_ACQ_OWSDLG_H__3A7A103C_2012_4AE3_85F1_F025AC13CFDB__INCLUDED_)
#define AFX_ACQ_OWSDLG_H__3A7A103C_2012_4AE3_85F1_F025AC13CFDB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CACQ_OWSDlg dialog

class CACQ_OWSDlg : public CDialog
{
// Construction
public:
	CACQ_OWSDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CACQ_OWSDlg)
	enum { IDD = IDD_ACQ_OWS_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CACQ_OWSDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CACQ_OWSDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ACQ_OWSDLG_H__3A7A103C_2012_4AE3_85F1_F025AC13CFDB__INCLUDED_)
