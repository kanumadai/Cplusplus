#ifndef __WNDTIME_H__
#define __WNDTIME_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// WndDispTime.h : header file
//

#include "WndDisp.h"

/////////////////////////////////////////////////////////////////////////////
// CWndDispTime window

class DLLIOPORT CWndDispTime : public CWndDisp
{
public:
	virtual void ShowDat(void* pBuf, int len);
	virtual void ResetDat();

protected:
	long* m_pTimeBuf[8];
	BYTE* m_pDispBuf[8];
	int m_nMaxCnt;

	int m_nCntTime[8][128];
	int m_nMaxCntTime;

	//int m_nCntStrip[8][64];
	//int m_nMaxCntStrip;

	CDC   m_DCClient;
	CBitmap m_bmpClient;

	CPen m_GridPen;
	CPen m_LinePen;

// Construction
public:
	CWndDispTime();

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
	virtual ~CWndDispTime();

	// Generated message map functions
protected:
	virtual void DatFmt();
	virtual void AdjWndSize();
	virtual void DrawGrid(CDC* pDC, const RECT* pRect, int GridSize);
	void GetBlockRect(int nBlock, int nStrip, RECT* pRect);
	//{{AFX_MSG(CWndDispTime)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnPaint();
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnDisp2d();
	afx_msg void OnDisp1d();
	afx_msg void OnPalDefault();
	afx_msg void OnPal256();
	afx_msg void OnPalFlow();
	afx_msg void OnPalHot();
	afx_msg void OnPalMap();
	afx_msg void OnPalNih();
	afx_msg void OnSave();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WNDDISPSD_H__FC523C39_40AE_44D1_8A78_F9172DB03273__INCLUDED_)
