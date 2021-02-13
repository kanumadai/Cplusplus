#if !defined(AFX_FPGACOUNTMAP_H__9E361A70_677C_4707_9BA7_1CDCBF8FC368__INCLUDED_)
#define AFX_FPGACOUNTMAP_H__9E361A70_677C_4707_9BA7_1CDCBF8FC368__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FpgaCountMap.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CFpgaCountMap dialog

class CFpgaCountMap : public CDialog
{
// Construction
public:
	void OnShowEfficent();
	void OnSaveFile();
	void OnOpenFile();
	int m_arr32BuckData[32][8][2];
	void OnDrawLine();
	CFpgaCountMap(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CFpgaCountMap)
	enum { IDD = IDD_FPGA_COUNT_MAP };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFpgaCountMap)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CFpgaCountMap)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FPGACOUNTMAP_H__9E361A70_677C_4707_9BA7_1CDCBF8FC368__INCLUDED_)
