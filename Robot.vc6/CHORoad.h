#include "robotDlg.h"
#if !defined(AFX_CHOROAD_H__555FB820_BBC0_4B9D_B3B3_7489332EBC8A__INCLUDED_)
#define AFX_CHOROAD_H__555FB820_BBC0_4B9D_B3B3_7489332EBC8A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CHORoad.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCHORoad dialog

class CCHORoad : public CDialog
{
// Construction
public:
	void ChoStation();
	CRobotDlg robot;
	CCHORoad(CWnd* pParent = NULL);// standard constructor
	int  Chooseflag;//路线选择标志
// Dialog Data
	//{{AFX_DATA(CCHORoad)
	enum { IDD = IDD_ChoRoad };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA
    
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCHORoad)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CCHORoad)
	afx_msg void On0_CT();
	afx_msg void On0_MRI();
	afx_msg void On0_PET();
	afx_msg void OnCT_0();
	afx_msg void OnCT_MRI();
	afx_msg void OnCT_PET();
	afx_msg void OnMRI_0();
	afx_msg void OnMRI_CT();
	afx_msg void OnMRI_PET();
	afx_msg void OnPET_0();
	afx_msg void OnPET_CT();
	afx_msg void OnPET_MRI();
	afx_msg void OnCancellim();
	afx_msg void OnPreview();
	afx_msg void OnNext();
	afx_msg void OnReSetState();
	afx_msg void OnDirectHold();
	afx_msg void OnDirectMove();
	afx_msg void OnDirectMove2();
	afx_msg void OnDirectHold2();
	afx_msg void OnDirectMove3();
	afx_msg void OnDirectMove4();
	afx_msg void OnMRITurning90();
	afx_msg void OnDirectHoldCT();
	afx_msg void OnDirectMoveCT();
	afx_msg void OnDirectMove6CT();
	afx_msg void OnDirectMovePETMRI();
	afx_msg void OnPETTurning91();
	afx_msg void OnDirectHoldPETMRI1();
	afx_msg void OnDirectMovePETMRI4();
	afx_msg void OnDirectHoldMRIPET();
	afx_msg void OnDirectMoveMRIPET2();
	afx_msg void OnDirectMoveMRIPET4();
	afx_msg void OnReadHold();
	afx_msg void OnNextLimited();
	afx_msg void OnProLimited();
	afx_msg void OnMRITurning92();
	afx_msg void OnMRITurning93();
	afx_msg void OnMRITurning94();
	afx_msg void OnMRITurning95();
	afx_msg void OnNextLimited2();
	afx_msg void OnProLimited2();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CHOROAD_H__555FB820_BBC0_4B9D_B3B3_7489332EBC8A__INCLUDED_)
