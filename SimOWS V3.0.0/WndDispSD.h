#if !defined(AFX_WNDDISPSD_H__FC523C39_40AE_44D1_8A78_F9172DB03273__INCLUDED_)
#define AFX_WNDDISPSD_H__FC523C39_40AE_44D1_8A78_F9172DB03273__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// WndDispSD.h : header file
//

#include "WndDisp.h"

/////////////////////////////////////////////////////////////////////////////
// CWndDispSD window

class DLLIOPORT CWndDispSD : public CWndDisp
{
public:
	virtual void ShowDat(void* pBuf, int len);
	virtual void ResetDat();

protected:
	long  m_nMaxCnt;
	long* m_pSDBuf[8];
	BYTE* m_pDispBuf[8];

	CDC   m_DCClient;
	CBitmap m_bmpClient;

	CPen m_GridPen;

protected:

// Construction
public:
	CWndDispSD();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWndDispSD)
	//}}AFX_VIRTUAL

// Implementation
public:
	void Draw(CDC* pDC);
	void CreateWnd(void);
	virtual ~CWndDispSD();

	// Generated message map functions
protected:
	virtual void DatFmt();
	virtual void DrawGrid(CDC* pDC, const RECT* pRect, int GridSize);
	void GetBlockRect(int nBlock, RECT* pRect);
	//{{AFX_MSG(CWndDispSD)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnPaint();
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnPalDefault();
	afx_msg void OnPalFlow();
	afx_msg void OnPalHot();
	afx_msg void OnPalMap();
	afx_msg void OnPalNih();
	afx_msg void OnPal256();
	afx_msg void OnSave();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WNDDISPSD_H__FC523C39_40AE_44D1_8A78_F9172DB03273__INCLUDED_)
