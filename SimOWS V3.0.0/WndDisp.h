#if !defined(AFX_WNDDISP_H__16B24FC2_B851_4D0A_8952_2C96098661B1__INCLUDED_)
#define AFX_WNDDISP_H__16B24FC2_B851_4D0A_8952_2C96098661B1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// WndDisp.h : header file
//

#pragma warning(disable:4275)
#pragma warning(disable:4251)

#ifdef _BLKDRAW_DLL
	#ifdef _USRDLL
		#define DLLIOPORT __declspec( dllexport )
	#else
		#define DLLIOPORT __declspec( dllimport )
	#endif
#else
	#define DLLIOPORT
#endif

#include "DatFmt.h"

/////////////////////////////////////////////////////////////////////////////
// CWndDisp window

//only for disp time \ energy \ coin time
typedef enum __disp_mode{
	mode_8blk_2d,
	mode_8blk_1d,
	mode_1blk_64strip,
	mode_1blk_1strip,
	mode_1bucket,
	mode_allbucket
}DISP_MODE;

class DLLIOPORT CWndDisp : public CWnd
{
// Operations
public:
	virtual void ShowDat(void* pBuf, int len) {};
	virtual void ResetDat() = NULL;

protected:
    virtual long ColorTrans(long nCnt, long nMaxCnt);

	virtual void OnPalDefault();
	virtual void OnPalFlow();
	virtual void OnPalHot();
	virtual void OnPalMap();
	virtual void OnPalNih();
	virtual void OnPal256();

protected:
	CPen  m_MarkPen;
	virtual void MarkPoint(CDC* pDC, int x, int y, int width);

protected:
	BOOL m_bDispAll;
	DISP_MODE m_DispMode;
	int   m_currBlock;
	int   m_currStrip;
	HBRUSH m_hBkBrush;

	CMenu m_menu;

	BOOL  m_bDefaultPAL;
	static const long int colorNumber[64];

	CDC   m_DCBlk;
	CBitmap m_bmpBlk;
	BITMAPINFO* m_pBmpInfo;

// Construction
public:
	CWndDisp();

// Attributes
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWndDisp)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CWndDisp();

	// Generated message map functions
protected:
	//{{AFX_MSG(CWndDisp)
	afx_msg void OnShowMainwnd();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WNDDISP_H__16B24FC2_B851_4D0A_8952_2C96098661B1__INCLUDED_)
