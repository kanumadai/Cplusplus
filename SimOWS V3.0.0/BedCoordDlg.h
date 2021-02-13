#if !defined(AFX_BEDCOORDDLG_H__7C42D9C2_B00F_454A_A80B_3DE0DF9A7117__INCLUDED_)
#define AFX_BEDCOORDDLG_H__7C42D9C2_B00F_454A_A80B_3DE0DF9A7117__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// BedCoordDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CBedCoordDlg dialog

class CBedCoordDlg : public CDialog
{
// Construction
public:
	CBedCoordDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CBedCoordDlg)
	enum { IDD = IDD_BEDCOORD };
	int		m_coord;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBedCoordDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CBedCoordDlg)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BEDCOORDDLG_H__7C42D9C2_B00F_454A_A80B_3DE0DF9A7117__INCLUDED_)
