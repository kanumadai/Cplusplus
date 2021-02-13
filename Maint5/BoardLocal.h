//{{AFX_INCLUDES()
#include "msflexgrid.h"
#include "mscomm.h"
//}}AFX_INCLUDES
#if !defined(AFX_BOARDLOCAL_H__0B7DCF01_C1A4_11D6_9644_00E04C50B69B__INCLUDED_)
#define AFX_BOARDLOCAL_H__0B7DCF01_C1A4_11D6_9644_00E04C50B69B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// BoardLocal.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CBoardLocal dialog

class CBoardLocal : public CDialog
{
// Construction
public:
	void OnSend8BlockPara();
//	CWinThread *m_pThread_BoardLocal;
	unsigned char m_uchPLimit[8][8][8][4];   //【块号】【Y方向】【X方向】【x0，y0，x1,y1】

	int m_iSuccessACKNum;

	unsigned char TxdData[72*4];
	int	m_InOut_Sel;
	int m_Chip_Sel;
	unsigned char rectempl[64][4];
	unsigned char strRecData[80*4];
	int	iRecNum;
	bool bAckFrame;
//	int m_workcounter;

	unsigned char m_sendnum;

	void SendCommand(unsigned char *CommandStr, int SendNum);

	CBoardLocal(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CBoardLocal)
	enum { IDD = IDD_FPGA_BOARDLOCAL };
	CButton	m_bChannelChangeSel;
	CEdit	m_BoardLocal_Edit;
	CComboBox	m_Block_Sel;
	CComboBox	m_Bucket_Sel;
	CMSFlexGrid	m_Grid;
	CMSComm	m_MSComm;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBoardLocal)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CBoardLocal)
	afx_msg void OnDblClickMsflexgrid1();
	afx_msg void OnKillfocusBoardlocalEdit();
	afx_msg void OnEepromRadio();
	afx_msg void OnFpgaRadio();
	afx_msg void OnInputRadio();
	afx_msg void OnOutputRadio();
	virtual BOOL OnInitDialog();
	afx_msg void OnCommMscomm();
	afx_msg void OnOKe();
	virtual void OnCancel();
	afx_msg void OnClose();
	afx_msg void OnLoaddata();
	afx_msg void OnDestroy();
	afx_msg void OnSelchangeBlockSel();
	afx_msg void OnChannelChangeSel();
	afx_msg void OnSelchangeBucketSel();
	DECLARE_EVENTSINK_MAP()
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BOARDLOCAL_H__0B7DCF01_C1A4_11D6_9644_00E04C50B69B__INCLUDED_)
