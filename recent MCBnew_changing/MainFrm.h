// MainFrm.h : interface of the CMainFrame class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAINFRM_H__2B1ABA63_7985_4310_B432_02BD0FA2A0F3__INCLUDED_)
#define AFX_MAINFRM_H__2B1ABA63_7985_4310_B432_02BD0FA2A0F3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CMainFrame : public CFrameWnd
{
	
protected: // create from serialization only
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainFrame)
	public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
	//}}AFX_VIRTUAL

// Implementation
public:
	CDialogBar m_wndDlgBar;
	CDialogBar m_wndDlgBottom;
    CToolBar    m_wndToolBar;
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // control bar embedded members
	CStatusBar  m_wndStatusBar;
//	CToolBar    m_wndToolBar;

// Generated message map functions
protected:
	//{{AFX_MSG(CMainFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnUpdateIdClearBuff(CCmdUI* pCmdUI);
	afx_msg void OnUpdatePrintPre(CCmdUI* pCmdUI);
	afx_msg void OnClose();
	afx_msg void OnUpdateIdStartsample(CCmdUI* pCmdUI);
	afx_msg void OnUpdateIdOutReport(CCmdUI* pCmdUI);
	afx_msg void OnUpdateIdTestsample(CCmdUI* pCmdUI);
	afx_msg void OnUpdateIdSendsample(CCmdUI* pCmdUI);
	afx_msg void OnUpdateIdResetsample(CCmdUI* pCmdUI);
	afx_msg void OnUpdateIdReceivesample(CCmdUI* pCmdUI);
	afx_msg void OnUpdateIdContinuesample(CCmdUI* pCmdUI);
	afx_msg void OnUpdateIdWholedataSum(CCmdUI* pCmdUI);
	afx_msg void OnUpdateIdWholedataFilter(CCmdUI* pCmdUI);
	afx_msg void OnUpdateIdInterestSum(CCmdUI* pCmdUI);
	afx_msg void OnUpdateIdEnergeKedu(CCmdUI* pCmdUI);
	afx_msg void OnUpdateIdWidthStat(CCmdUI* pCmdUI);
	afx_msg void OnUpdateMenuFileSaveAs(CCmdUI* pCmdUI);
	afx_msg void OnUpdateFilePrint(CCmdUI* pCmdUI);
	afx_msg void OnUpdateEXTERNALDATA_MCB2BUFF(CCmdUI* pCmdUI);
	afx_msg void OnMoving(UINT fwSide, LPRECT pRect);
	afx_msg void OnSizing(UINT fwSide, LPRECT pRect);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINFRM_H__2B1ABA63_7985_4310_B432_02BD0FA2A0F3__INCLUDED_)
