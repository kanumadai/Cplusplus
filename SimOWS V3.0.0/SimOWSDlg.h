// SimOWSDlg.h : header file
//

#if !defined(AFX_SIMOWSDLG_H__1A981072_9DCC_4A67_A853_95F2F8E47B4F__INCLUDED_)
#define AFX_SIMOWSDLG_H__1A981072_9DCC_4A67_A853_95F2F8E47B4F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ListenSocket.h"
#include "WndDispSD.h"
#include "WndDispTime.h"
#include "WndDispEnergy.h"
#include "WndDispCoinTime.h"
#include "DatFmt.h"

#include "InitACSDlg.h"
#include "FHParamDlg.h"	
#include "BucketParamDlg.h"
#include "AutoAcqCoinTimeDlg.h"	 
#include "AutoAcqBucketDlg.h"

#include "OWSSIO.h"

typedef struct _buf_stat
{
	BOOL bRecv;
	BOOL bFilled;
	BOOL bWrite;
}BUF_STAT;

/////////////////////////////////////////////////////////////////////////////
// CSimOWSDlg dialog

class CSimOWSDlg : public CDialog
{
public:
	CWndDisp* GetDispWnd();
	BOOL m_bExpertMode;

// Construction
public:
	CSimOWSDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CSimOWSDlg)
	enum { IDD = IDD_SIMOWS_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSimOWSDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

public:
	CListenSocket m_ServerSock;
	COWSSIO m_sio;

// Implementation
public:
	void ShowPetMonitor(ACQ_MONITOR* pMonitorInfo, ACQ_SYSINFO* pSysInfo);
	DWORD _ThreadAutoAcqCoinTime(void);
	DWORD _ThreadAutoAcqBucket(void);
	void SendPetAllProtocolOver();
	void SendInitACS();
	CFHParamDlg m_dlgFHParam;
	CBucketParamDlg m_dlgBucketParam;
	CAutoAcqCoinTimeDlg m_dlgAutoAcqCointTime;
	CAutoAcqBucketDlg   m_dlgAutoAcqBucket;

	BOOL m_bAcsInTestMode;

	BOOL m_bFHStart;
	DAT_FMT    m_WndType;
	CWndDispSD m_WndSD;
	CWndDispTime m_WndTime;
	CWndDispEnergy m_WndEnergy;
	CWndDispCointime m_WndCoinTime;
	
	CInitACSDlg m_dlgInitAcs;
	INIT_ITEM  m_InitAcsData[12];
	INIT_ITEM  m_InitBuckData[2];
	INIT_ITEM  m_InitEMCData[8];
	CMenu m_menu;

	int m_Timer;
	HICON m_hIcon;

	ACQ_PROTOCOL  m_protocol;

	// Generated message map functions
	//{{AFX_MSG(CSimOWSDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnDestroy();
	afx_msg void OnStart();
	afx_msg void OnStop();
	afx_msg void OnTimer(UINT nIDEvent);
	virtual void OnOK();
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnQueryBedMovestatus();
	afx_msg void OnQueryBedPos();
	afx_msg void OnQueryBedStep();
	afx_msg void OnQueryBedSwitchset();
	afx_msg void OnQueryRodRate();
	afx_msg void OnQueryRodSwitchset();
	afx_msg void OnQuerySeptaSwitchset();
	afx_msg void OnQueryTbed();
	afx_msg void OnQueryTg();
	afx_msg void OnQueryThI();
	afx_msg void OnQueryThTg();
	afx_msg void OnQueryThTw();
	afx_msg void OnQueryThV();
	afx_msg void OnQueryTrod();
	afx_msg void OnQueryTsepta();
	afx_msg void OnQueryTw();
	afx_msg void OnSetBedcoord();
	afx_msg void OnSetBedstep();
	afx_msg void OnSetRodrate();
	afx_msg void OnSetThI();
	afx_msg void OnSetThTg();
	afx_msg void OnSetThTw();
	afx_msg void OnSetThV();
	afx_msg void OnCtrlBedmovestepx();
	afx_msg void OnCtrlBedmovestepy();
	afx_msg void OnCtrlBedmovex();
	afx_msg void OnCtrlBedmovexRelate();
	afx_msg void OnCtrlBedmovey();
	afx_msg void OnCtrlBedmoveyRelate();
	afx_msg void OnCtrlBedstopall();
	afx_msg void OnCtrlBedstopx();
	afx_msg void OnCtrlBedstopy();
	afx_msg void OnCtrlRodestop();
	afx_msg void OnCtrlRodin();
	afx_msg void OnCtrlRodout();
	afx_msg void OnCtrlRodroll();
	afx_msg void OnCtrlRodstop();
	afx_msg void OnCtrlSeptaestop();
	afx_msg void OnCtrlSeptain();
	afx_msg void OnCtrlSeptaout();
	afx_msg void OnCtrlSeptastop();
	afx_msg void OnCtrlHvOff();
	afx_msg void OnCtrlHvOn();
	afx_msg void OnCtrlLaseroff();
	afx_msg void OnCtrlLaseron();
	afx_msg void OnCtrlMpowerOff();
	afx_msg void OnCtrlMpowerOn();
	afx_msg void OnDispGate();
	afx_msg void OnDispRod();
	afx_msg void OnDispBucket();
	afx_msg void OnQueryRodStatus();
	afx_msg void OnQuerySeptaStatus();
	afx_msg void OnDispLmd();
	afx_msg void OnInitACS();
	afx_msg void OnQueryHVStatus();
	afx_msg void OnTestMode();
	afx_msg void OnAutoAcqCoinTime();
	afx_msg void OnAcqBucket();
	afx_msg void OnAcqCoinpos();
	afx_msg void OnAutoAcqBucket();
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnAcqCoinTime();
	afx_msg void OnClose();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
protected:
	BOOL m_bAutoAcq;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SIMOWSDLG_H__1A981072_9DCC_4A67_A853_95F2F8E47B4F__INCLUDED_)
