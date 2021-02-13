#if !defined(AFX_FPGACOUNTER_H__538728A3_CB13_4E80_A36D_14C8E32C422C__INCLUDED_)
#define AFX_FPGACOUNTER_H__538728A3_CB13_4E80_A36D_14C8E32C422C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FpgaCounter.h : header file
//
	
/////////////////////////////////////////////////////////////////////////////
// CFpgaCounter dialog

class CFpgaCounter : public CDialog
{
// Construction
public:
	int m_iRecBucketNum;
	void OnSend32BuckCMD();
	int m_BuckIndex;
	int m_arr32BuckData[32][8][2];   //32bucket, 8 block, 2: 总计数；有效计数
	BOOL m_bSave32BuckData;
	void OnSave32BuckData(unsigned char * pdata);
	void OnShowData(unsigned char *pdata);
	CFpgaCounter(CWnd* pParent = NULL);   // standard constructor

	//CWinThread *g_pThread_EnFpgaCount;

	int m_RecCounter;
	unsigned char recbuf[100];

// Dialog Data
	//{{AFX_DATA(CFpgaCounter)
	enum { IDD = IDD_FPGA_COUNTER };
	CComboBox	m_Bucket_Sel;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFpgaCounter)
	public:
	virtual BOOL DestroyWindow();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	

	// Generated message map functions
	//{{AFX_MSG(CFpgaCounter)
	virtual void OnOK();
	virtual void OnCancel();
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnClose();
	afx_msg void OnDestroy();
	afx_msg void OnShowmap();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FPGACOUNTER_H__538728A3_CB13_4E80_A36D_14C8E32C422C__INCLUDED_)
