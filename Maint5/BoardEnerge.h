//{{AFX_INCLUDES()
#include "mscomm.h"
#include "msflexgrid.h"
//}}AFX_INCLUDES
#if !defined(AFX_BOARDENERGE_H__00E2FD01_C3D3_11D6_9644_00E04C50B69B__INCLUDED_)
#define AFX_BOARDENERGE_H__00E2FD01_C3D3_11D6_9644_00E04C50B69B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// BoardEnerge.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CBoardEnerge dialog

class CBoardEnerge : public CDialog
{
// Construction
public:
	void OnSend8BlockPara();
//	CWinThread *g_pThread_BoardEnerge;
	unsigned char m_uchELimit[8][64][2];

	int m_iSuccessACKNum;

	unsigned char TxdData[72*4];
	int	m_InOut_Sel;
	int m_Chip_Sel;
	unsigned char rectempl[64][4];
	unsigned char strRecData[80*4];
	int	iRecNum;
	bool bAckFrame;
	unsigned char m_sendnum;

//	int m_workcounter;

	void SendCommand(unsigned char *CommandStr, int SendNum);


	CBoardEnerge(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CBoardEnerge)
	enum { IDD = IDD_FPGA_BOARDENER };
	CButton	m_bChannelChangeSel;
	CEdit	m_BoardEnerge_Edit;
	CComboBox	m_Block_Sel;
	CComboBox	m_Bucket_Sel;
	CMSComm	m_MSComm;
	CMSFlexGrid	m_Grid;
	CString	m_strEner_E1Value;
	CString	m_strEner_E2Value;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBoardEnerge)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CBoardEnerge)
	afx_msg void OnOKe();
	afx_msg void OnOutputRadio();
	afx_msg void OnCommMscomm();
	afx_msg void OnInputRadio();
	afx_msg void OnFpgaRadio();
	afx_msg void OnEepromRadio();
	virtual BOOL OnInitDialog();
	afx_msg void OnDblClickMsflexgrid();
	afx_msg void OnKillfocusBoardenergEdit();
	virtual void OnCancel();
	afx_msg void OnClose();
	afx_msg void OnDestroy();
	afx_msg void OnSetEnerInit();
	afx_msg void OnLoadfile();
	afx_msg void OnSelchangeBlockSel();
	afx_msg void OnChannelChangeSel();
	afx_msg void OnSelchangeBucketSel();
	DECLARE_EVENTSINK_MAP()
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BOARDENERGE_H__00E2FD01_C3D3_11D6_9644_00E04C50B69B__INCLUDED_)
