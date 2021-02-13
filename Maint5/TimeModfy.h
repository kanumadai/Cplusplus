//{{AFX_INCLUDES()
#include "mscomm.h"
#include "msflexgrid.h"
//}}AFX_INCLUDES
#if !defined(AFX_TIMEMODFY_H__A8DE0481_C3F3_11D6_9644_00E04C50B69B__INCLUDED_)
#define AFX_TIMEMODFY_H__A8DE0481_C3F3_11D6_9644_00E04C50B69B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TimeModfy.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CTimeModfy dialog

class CTimeModfy : public CDialog
{
// Construction
public:
	void OnSend8BlockPara();
//	CWinThread *g_pThread_TimeModify;

	int m_iSuccessACKNum;


	unsigned char TxdData[72*4];
	int	m_InOut_Sel;
	int m_Chip_Sel;
	unsigned char rectempl[64][4];
	unsigned char strRecData[80*4];
	int	iRecNum;
	bool bAckFrame;

	void SendCommand(unsigned char *CommandStr, int SendNum);

	CTimeModfy(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CTimeModfy)
	enum { IDD = IDD_FPGA_TIMEMODFY };
	CEdit	m_TimeModfy_Edit;
	CComboBox	m_Bucket_Sel;
	CComboBox	m_Block_Sel;
	CMSComm	m_MSComm;
	CMSFlexGrid	m_Grid;
	CString	m_strTMod_Init_Value;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTimeModfy)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CTimeModfy)
	afx_msg void OnOutputRadio();
	afx_msg void OnDblClickMsflexgrid();
	afx_msg void OnInputRadio();
	afx_msg void OnFpgaRadio();
	afx_msg void OnEepromRadio();
	afx_msg void OnOKe();
	virtual BOOL OnInitDialog();
	afx_msg void OnKillfocusTimemodfyEdit();
	afx_msg void OnCommMscomm();
	afx_msg void OnClose();
	afx_msg void OnOut();
	afx_msg void OnDestroy();
	afx_msg void OnSetTmodInit();
	afx_msg void OnSelchangeBlockSel();
	DECLARE_EVENTSINK_MAP()
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TIMEMODFY_H__A8DE0481_C3F3_11D6_9644_00E04C50B69B__INCLUDED_)
