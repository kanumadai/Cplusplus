// DynShowDlg.h : header file
//

#if !defined(AFX_DYNSHOWDLG_H__16AFE716_FC3B_4F33_A8D3_4E71CE74B545__INCLUDED_)
#define AFX_DYNSHOWDLG_H__16AFE716_FC3B_4F33_A8D3_4E71CE74B545__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "PeakVale.h"
#include "EnergeSpectEth.h"
#include "FileSave.h"


#define WM_RECVEIVE_FULL WM_USER+102


enum SEL_2D_SHOW_MODE {
	SEL_2D_SHOW_NULL = 0,
	SEL_2D_TOTAL_ENERGESPEC = 1,
	SEL_2D_SHOW_X = 2,
	SEL_2D_SHOW_Y = 3,
    SEL_2D_TIME_SPEC = 4,
	SEL_2D_CRYSTAL_ENERGESPEC = 5,
	SEL_2D_TIME_FUHE = 6
};



/////////////////////////////////////////////////////////////////////////////
// CDynShowDlg dialog

class CDynShowDlg : public CDialog
{
// Construction
public:
	void OnRedraw_3d();
	void OnRedraw_2d();
	long ColorSet(long nCol, long nu);

	void OnTimeSpect_redraw_2D();
	void OnSaveLimitFile();
	void OnLoadLimitFile();
	void RedrawLimitFile(void);
	void OnCompareValueShow();
	void OnAmplifierCorrect();
	void OnLoadEnergeSpectrumLimitFile();
public:
	CDynShowDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CDynShowDlg)
	enum { IDD = IDD_DYNSHOW_DIALOG };
	CButton	m_Check_Midfilter;
	CButton	m_EnLPWholeLineMove;
//	CComboBox	m_ProgCtrl_Sel;
	CButton m_EnergeSpectrum_Sel;
	CString	m_strSampleTime;
	CString	m_strSampleShowMode;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDynShowDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
public:
	CString m_CurFileName;
	BOOL OnAnalyzeTimeFuheData(char* pData,int datasize);
	void OnShowTimeFuheMap_2D();
	void OnMidfilter();
	void OnShowPeakValeRatio(int no);

	CFileSave  filesave;

	CEnergeSpectEth  energeSpectEth;
	CPEAKVALE  peakvaleratio;
	bool m_bEnShowPeakValeRatio;  //使能显示峰谷比

	unsigned char* m_RecFastData;
	long	m_RecFastCounter;
	double long    m_reccounter;

protected:	
	//文件操作
	BOOL fileoper_Newfile;
	int	 fileoper_iblocksel;

	int	m_iProgCtrlData[10];		//程控数据
	long m_isdot_datafile[64][64]; // 散点数据文件， 【垂直】【水平】

	
	int m_iMidfilter_sdot[64][64];	//中值滤波 散点数据
	int m_Backup_iMidfilter_sdot[64][64];  //中值滤波备份
	BOOL m_bEnMidFilter;			//中值滤波使能


	long m_sdot[64][64];

	long m_sdot_file1[64][64];
	long m_sdot_file2[64][64];
	CString m_FileName1;
	CString m_FileName2;

	int	m_iSdotFileSel;
	bool m_bCompareFinish;

	bool m_bSampleTimeMode;
	int m_iSampleTime;
	int m_iSampleTimeCounter;
	long m_iSampleCount;

	long m_Limit_Stat[8][8];
	long m_Limit_Stat_Second[8][8];

	long m_igoto_one_first[8][8];
	long m_igoto_one_second[8][8];


	CPoint m_Oldpoint;
	bool m_bMove_LeftLimit;
	bool m_bMove_RightLimit;

	int m_iSmallMap_Show_Sel;
	CFile m_RealTimeFile;

	bool bAutoRedraw_down;
	int ileft_limit , iright_limit;
	bool bredraw2D_Energe;
	
	bool m_bSelEthLimit_HandleInput; //默认选择 能谱边界的输入方式为 手动方式
	//---------------能谱数据---------//
	bool m_bSelShowEnergeSpectrum;  //3D 的显示方式选择
	int m_Energer_Spectrum[64];
	int m_Time_Spectrum[128];
	int m_iEth_datafile[64][64]; // 能量数据文件， 【条号】【能量计数】
	int m_iEnergeSpectEth_Low[64];
	int m_iEnergeSpectEth_Hight[64];


	//------【操作位置边界】-------//
	bool m_bSameDriction_X;
	bool m_bMoveLongLine;
	int m_Long_Moving_Tab;
	bool m_bButtonDown;
	bool m_bShort_Line_Valid;
	CPoint m_sdotOldpoint;
	int m_Moving_Tab_Ver;
	int m_Moving_Tab_Hor;
	bool m_bthisMoveInValid;

	int m_Position_X0[8][8];
	int m_Position_X1[8][8];
	int m_Position_Y0[8][8];
	int m_Position_Y1[8][8];
	//new add veriable ;
	bool m_bEnMoveBreakLine;
	bool m_bMoveWholeLine;		//整条线平移；
	bool m_bMovingVerDirection; //正在移动的线为垂直方向；
	int  m_iMoveCurLine_X;		//水平标号
	int  m_iMoveCurLine_Y;		//垂直标号

	//时间符合数据区
	int m_arrTimeFuheData[256];   //-127 --- + 128 共256个数据
	INT m_iWorkMode;  // 工作模式选择
protected:
	HICON m_hIcon;


	// Generated message map functions
	//{{AFX_MSG(CDynShowDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnRedraw();
	afx_msg void OnOpenFile();
	afx_msg void OnEnergChartRedraw();
	virtual void OnOK();
	afx_msg void OnClose();
	afx_msg void OnSaveFile();
	afx_msg void OnStartSample();
	afx_msg void OnStopSample();
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnEnergerSpectrum_Sel();
	afx_msg void OnXDirection_Sel();
	afx_msg void OnYDirection_Sel();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnCompareStat();
	afx_msg void OnProgControl();
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnSampleModeSel();
	afx_msg void OnTimeSpectrumSel();
	afx_msg void On2d_SynEnergeSpectrumSel();
	afx_msg void OnAddSynEnergSpectrumNo();
	afx_msg void OnEthReset();
	afx_msg void OnEnLimitPositionWholeMove();
	afx_msg void OnPeakValeStat();
	afx_msg void OnLimitStat();
	afx_msg void OnEnerspect64eth();
	afx_msg void OnCheck1();
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnCheckMidfilter();
	afx_msg void OnClearData();
	afx_msg void OnTimeFuheSel();
	//}}AFX_MSG

	afx_msg LRESULT OnRecveive(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DYNSHOWDLG_H__16AFE716_FC3B_4F33_A8D3_4E71CE74B545__INCLUDED_)
