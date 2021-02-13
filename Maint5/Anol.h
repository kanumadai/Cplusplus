#if !defined(AFX_ANOL_H__0EA01EA1_C40A_11D6_9644_00E04C50B69B__INCLUDED_)
#define AFX_ANOL_H__0EA01EA1_C40A_11D6_9644_00E04C50B69B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Anol.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CAnol dialog

class CAnol : public CDialog
{
// Construction
public:
	void OnLoad8BlockPara();
	void OnSend8BlockPara();
//	CWinThread *g_pThread_Anol;

	unsigned char m_uchrAnolPara[8][8];

	int m_iSuccessACKNum;

	int m_Chip_Sel;
	int	m_InOut_Sel;

	int RecCounter;
	unsigned char recbuf[128];
	bool bAckFrame;
	bool m_bLoaded8BlockPara;
	bool m_bEnLoad8BlockPara;
	int m_curblock;

	CAnol(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CAnol)
	enum { IDD = IDD_ANLOG };
	CButton	m_bChannelChangeSel;
	CButton	m_Anol_Chip_Eeprom;
	CButton m_Anol_Chip_Anol;
	CButton m_Anol_Chip_All;
	CButton m_Anol_IOsel_In;
	CButton m_Anol_IOsel_Out;
	CComboBox	m_Bucket_Sel;
	CComboBox	m_Block_Sel;
	CString	m_Eth_strValue;
	CString	m_Td_strValue;
	CString	m_strPlus_B;
	CString	m_strPlus_C;
	CString	m_strPlus_D;
	CString	m_strOffset_E;
	CString	m_strOffset_X;
	CString	m_strOffset_Y;
	CString	m_strPlus_A;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAnol)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CAnol)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	afx_msg void OnChipEeprom();
	afx_msg void OnChipAnol();
	afx_msg void OnChipAll();
	afx_msg void OnIOsel_Out();
	afx_msg void OnIOsel_In();
	virtual void OnCancel();
	afx_msg void OnClose();
	afx_msg void OnDestroy();
	afx_msg void OnLoadfile();
	afx_msg void OnSelchangeBlockSel();
	afx_msg void OnSavePara();
	afx_msg void OnChannelChangeSel();
	afx_msg void OnEditchangeBucketSel();
	afx_msg void OnSelchangeBucketSel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ANOL_H__0EA01EA1_C40A_11D6_9644_00E04C50B69B__INCLUDED_)
