//{{AFX_INCLUDES()
#include "mscomm.h"
#include "msflexgrid.h"
//}}AFX_INCLUDES
#if !defined(AFX_TSERIES_H__756163A2_C3FC_11D6_9644_00E04C50B69B__INCLUDED_)
#define AFX_TSERIES_H__756163A2_C3FC_11D6_9644_00E04C50B69B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Tseries.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CTseries dialog

class CTseries : public CDialog
{
// Construction
public:
//	CWinThread *m_pThread;

	unsigned char TxdData[72*4];
	int	m_InOut_Sel;
	int m_Chip_Sel;
	unsigned char rectempl[17][5];
	unsigned char strRecData[80*4];
	int	iRecNum;
	bool bAckFrame;

	void SendCommand(unsigned char *CommandStr, int SendNum);

	CTseries(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CTseries)
	enum { IDD = IDD_FPGA_TSERIES };
	CComboBox	m_Block_Sel;
	CComboBox	m_Bucket_Sel;
	CEdit	m_Tseries_Edit;
	CMSComm	m_MSComm;
	CMSFlexGrid	m_Grid;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTseries)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CTseries)
	afx_msg void OnDblClickMsflexgrid1();
	afx_msg void OnCommMscomm();
	afx_msg void OnInputRadio();
	afx_msg void OnOutputRadio();
	afx_msg void OnOKe();
	virtual BOOL OnInitDialog();
	afx_msg void OnKillfocusTseriesEdit();
	afx_msg void OnClose();
	afx_msg void OnEepromRadio();
	afx_msg void OnFpgaRadio();
	afx_msg void OnOut();
	afx_msg void OnDestroy();
	DECLARE_EVENTSINK_MAP()
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TSERIES_H__756163A2_C3FC_11D6_9644_00E04C50B69B__INCLUDED_)
