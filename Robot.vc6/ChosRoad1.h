#if !defined(AFX_CHOSROAD1_H__A1FF9A70_E900_44B9_B127_2991CCFA4FE1__INCLUDED_)
#define AFX_CHOSROAD1_H__A1FF9A70_E900_44B9_B127_2991CCFA4FE1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ChosRoad1.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CChosRoad1 dialog

class CChosRoad1 : public CDialog
{
// Construction
public:
	CChosRoad1(CWnd* pParent = NULL);   // standard constructor
	int  Chooseflag;//路线选择标志
// Dialog Data
	//{{AFX_DATA(CChosRoad1)
	enum { IDD = IDD_ChoRoad1 };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CChosRoad1)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CChosRoad1)
	afx_msg void OnDireMovetoCT();
	afx_msg void OnDireLeavetoCT();
	afx_msg void OnDireHoldDownCT();
	afx_msg void OnCtPet();
	afx_msg void OnPetCt();
	afx_msg void OnDireMovetoPET();
	afx_msg void OnDireLeavetoPET();
	afx_msg void OnDireHoldDownPET();
	afx_msg void OnNextLimited();
	afx_msg void OnProLimited();
	afx_msg void OnNextLimited90();
	afx_msg void OnProLimited90();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CHOSROAD1_H__A1FF9A70_E900_44B9_B127_2991CCFA4FE1__INCLUDED_)
