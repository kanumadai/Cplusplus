#if !defined(AFX_WNDDISPCOINTIME_H__AD765FC2_1ACF_47D6_B2CA_0A32532BC7B1__INCLUDED_)
#define AFX_WNDDISPCOINTIME_H__AD765FC2_1ACF_47D6_B2CA_0A32532BC7B1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// WndDispCointime.h : header file
//

#include "WndDisp.h"

/////////////////////////////////////////////////////////////////////////////
// CWndDispCointime window

class CWndDispCointime : public CWndDisp
{
// Construction
public:
	virtual void ShowDat(char* szFile);
	virtual void ResetDat();

protected :
	int m_4BtoFOV_Table[32][15];
	int Bucket_4BtoFOV(int sel_buck, int buck4b);
	int m_Bucket5B[15];

	int m_nSelectedBucket;
	int m_nDispData[15][8][256];  //[fov][block][time channel]
	int m_bDataValid[15];
	int m_nMaxCnt;
	
	int m_nCurrDispBucket;

	CDC m_DCClient;
	CBitmap m_bmpClient;

	CPen m_GridPen;
	CPen m_LinePen;

public:
	CWndDispCointime();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWndDispCointime)
	//}}AFX_VIRTUAL

// Implementation
public:
	void CreateWnd(void);
	virtual ~CWndDispCointime();

	// Generated message map functions
protected:
	void DrawGrid(CDC *pDC, const RECT *pRect, int GridSize);
	void Draw( CDC* pDC);
	void GetBlockRect(int nBlockorBucket, RECT *pRect);
	void AdjWndSize();
	//{{AFX_MSG(CWndDispCointime)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnPaint();
	afx_msg void OnBucketAll();
	afx_msg void OnBucket0();
	afx_msg void OnBucket1();
	afx_msg void OnBucket2();
	afx_msg void OnBucket3();
	afx_msg void OnBucket4();
	afx_msg void OnBucket5();
	afx_msg void OnBucket6();
	afx_msg void OnBucket7();
	afx_msg void OnBucket8();
	afx_msg void OnBucket9();
	afx_msg void OnBucket10();
	afx_msg void OnBucket11();
	afx_msg void OnBucket12();
	afx_msg void OnBucket13();
	afx_msg void OnBucket14();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WNDDISPCOINTIME_H__AD765FC2_1ACF_47D6_B2CA_0A32532BC7B1__INCLUDED_)
