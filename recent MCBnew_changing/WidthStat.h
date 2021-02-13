#if !defined(AFX_WIDTHSTAT_H__33EAFA22_3F89_4536_B433_F77EA6CD2E76__INCLUDED_)
#define AFX_WIDTHSTAT_H__33EAFA22_3F89_4536_B433_F77EA6CD2E76__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// WidthStat.h : header file
//

#define WM_WIDTHSTAT_GOODBYE	 WM_USER + 110
#define WM_WIDTHSTAT_IDOK	 WM_USER + 107

/////////////////////////////////////////////////////////////////////////////
// CWidthStat dialog

class CWidthStat : public CDialog
{
// Construction
private: 
	CView *m_pView;

public:
	CWidthStat(CView *pView);
	BOOL Create();


	CWidthStat(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CWidthStat)
	enum { IDD = IDD_WIDTH_STAT };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWidthStat)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CWidthStat)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WIDTHSTAT_H__33EAFA22_3F89_4536_B433_F77EA6CD2E76__INCLUDED_)
