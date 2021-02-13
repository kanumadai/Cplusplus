//{{AFX_INCLUDES()
#include "msflexgrid.h"
#include "mscomm.h"
//}}AFX_INCLUDES
#if !defined(AFX_CYSNUM_H__756163A1_C3FC_11D6_9644_00E04C50B69B__INCLUDED_)
#define AFX_CYSNUM_H__756163A1_C3FC_11D6_9644_00E04C50B69B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CysNum.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCysNum dialog

class CCysNum : public CDialog
{
// Construction
public:
//	CWinThread *g_pThread_CysNum;


	unsigned char TxdData[72*4];
	int	m_InOut_Sel;
	int m_Chip_Sel;
	unsigned char rectempl[64][4];
	unsigned char strRecData[80*4];
	int	iRecNum;
	bool bAckFrame;

	void SendCommand(unsigned char *CommandStr, int SendNum);


	CCysNum(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CCysNum)
	enum { IDD = IDD_FPGA_CYSNUM };
	CComboBox	m_Bucket_Sel;
	CComboBox	m_Block_Sel;
	CEdit	m_CysNum_Edit;
	CMSFlexGrid	m_Grid;
	CMSComm	m_MSComm;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCysNum)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CCysNum)
	afx_msg void OnOutputRadio();
	afx_msg void OnDblClickMsflexgrid1();
	afx_msg void OnCommMscomm();
	afx_msg void OnInputRadio();
	afx_msg void OnFpgaRadio();
	afx_msg void OnEepromRadio();
	afx_msg void OnKillfocusCysnumEdit();
	afx_msg void OnOKe();
	virtual BOOL OnInitDialog();
	virtual void OnCancel();
	afx_msg void OnClose();
	afx_msg void OnDestroy();
	DECLARE_EVENTSINK_MAP()
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CYSNUM_H__756163A1_C3FC_11D6_9644_00E04C50B69B__INCLUDED_)
