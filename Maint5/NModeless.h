#if !defined(AFX_NMODELESS_H__E3C956B6_3DFE_491B_B006_C47BE95957CC__INCLUDED_)
#define AFX_NMODELESS_H__E3C956B6_3DFE_491B_B006_C47BE95957CC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// NModeless.h : header file
//

#include "StdAfx.h"

#define	WM_MOD_GOODBYE	WM_USER + 105
/////////////////////////////////////////////////////////////////////////////
// CNModeless dialog

#define  DATA64K_PRE		0x00
#define  DATA64K_CONTINUE   0x01

#define  CHECKLOCAL		0x02
#define  CHECKANOL		0x03
#define  CHECKENERGE	0x04
#define	 CHECKTIMEMODFY 0x05
#define  CHECKTSERISE	0x06
#define  CHECKCYSNUM	0x07
#define CMDNUM		8

typedef struct OP32BK_STR {	
	BOOL  bReadOP;					//读操作
	BOOL  bInputCMD[CMDNUM];		//用户是否有 命令码输入
	unsigned char funcode[CMDNUM];
	unsigned char funWTEEprom[CMDNUM];
	unsigned char funReadEEprom[CMDNUM];
	unsigned char funObject[CMDNUM];   //

	int packlenth[CMDNUM];
	int fileaddr[CMDNUM];
	int ackpackNo[CMDNUM];	
	
	int opratingAckNo;
	int opratingCMDNo;
	int opratingBlock;
	int opratingBucket;
};


class CNModeless : public CDialog
{
// Construction
public:
	void OnStopAutoSample();
	void OnStartAutoSample();
	BOOL m_bShowAutoCheck;
	void OnOK();
	BOOL On32FileProc(unsigned char *data);
	BOOL ON32FileOpen(int bucketNo,BOOL breadfile,int cmdnum,unsigned char *data);
	void OninitOp32BK();

	BOOL OnSendCmdPack();
	void OnOpenFile();
	BOOL OnInitComm();
	CNModeless(CWnd* pParent = NULL);   // standard constructor

	CNModeless(CView *pView);
	BOOL Create();

	BOOL m_bEnPara;

	OP32BK_STR    Op32BK;    // 操作32BUCKet 的读写	

	//自动采样变量
	BOOL m_bAutoSampleOperate;
	int m_iSampleTime;
	int m_arrSampleModSel[5];
	int m_arrAutoSampParaSel[10];
	int m_iAutoSampleModleNo;
	int m_iReSampleNum;
	CString m_strAutoSampleStatus;
	int m_iCurSampleTime;
	int m_iCurReAutoSamplNo;
//	BOOL m_bAutoRecDatingModel;

	BOOL m_bAutoSampleStart;   //控制按钮的显示
	BOOL m_bRecAnolPara;
	int  m_iCurSampleModel;



	int m_PortNo;
	CStatusBar *pStatus;
	BOOL m_EnClearBuff;
	BOOL m_bEnComm;
	int m_LoadBucketNo;
	int m_LoadBucketMax;

	int m_DataSampleModeSel;

	int m_iTimeReceivedData;

	int		m_iworkcounter;
	unsigned char m_recbuff[9*1024];
	unsigned char m_secrecbuf[9*1024];
	unsigned char *m_precbuf;
	int m_8KdataNo;
	int m_ShowCounterTh;

	unsigned char m_dataEnerge[2][64]; //1 个block的数据
	unsigned char m_dataLocal[4][64];


	CFile frecfile;
	BOOL bCloseFile;

	CString strRecStatus;
	CString FilePathName;
	CString FileName;


// Dialog Data
	//{{AFX_DATA(CNModeless)
	enum { IDD = IDD_MODELESS };
	CButton	m_Check_TSerise;
	CButton	m_Check_TimeModfy;
	CButton	m_Check_Energe;
	CButton	m_Check_CysNum;
	CButton	m_Check_Anol;
	CButton	m_Check_Local;
	
	CString	m_strStartPoint;
	CString	m_strEndPoint;
	//}}AFX_DATA
	CButton m_check_load;


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CNModeless)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	CView	*m_pView;
	HANDLE	m_hSetTimeOut;



	// Generated message map functions
	//{{AFX_MSG(CNModeless)
	virtual void OnCancel();
	afx_msg void OnTimer(UINT nIDEvent);
	virtual BOOL OnInitDialog();
	afx_msg void OnReName();
	afx_msg void OnDataPara();
	afx_msg void OnCheckAutosample();
	afx_msg void OnButtonAutoSample();
	//}}AFX_MSG
	afx_msg LRESULT On32BKRecveive(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnAutoSampRecFinish(WPARAM wParam,LPARAM lParam);

	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NMODELESS_H__E3C956B6_3DFE_491B_B006_C47BE95957CC__INCLUDED_)
