// MaintView.h : interface of the CMaintView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAINTVIEW_H__D7D6420D_BF45_11D6_9644_00E04C50B69B__INCLUDED_)
#define AFX_MAINTVIEW_H__D7D6420D_BF45_11D6_9644_00E04C50B69B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "DataSample.h"
#include "Anol.h"
#include "CysNum.h"
#include "PeakVale.h"
#include "EffectStat.h"

#include "NModeless.h"

//class CLoad32BK;

class CMaintView : public CView
{
protected:
	bool m_pLMouseDown;
	HCURSOR m_HCursor;
	CPoint m_Oldpoint;
	CPoint m_Newpoint;

	HWND hmodel;
	CNModeless *m_pview;

protected: // create from serialization only
	CMaintView();
	DECLARE_DYNCREATE(CMaintView)

// Attributes
public:
	CMaintDoc* GetDocument();

// Operations
public:
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMaintView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void OnInitialUpdate();
	protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	//}}AFX_VIRTUAL

// Implementation
public:
	CDataSample  datasample;
	CAnol	anol;
	CCysNum cysnum;
	CPEAKVALE  peakvaleratio;

	int m_ShowModelSel;

protected:
	CStatusBar *pStatus;
	CBitmap* m_pCacheBitmap;
	CBitmap* pOldBitmap;
	BOOL m_bEnLimitLineRedraw;
	BOOL m_bEnLimitStat_MemDCRedraw;
	BOOL m_bEnPeakValeStat_MemDCRedraw;


	BOOL m_bEnLoad32BKComm;

	int m_iMidfilter_sdot[64][64];	//中值滤波 散点数据
	BOOL m_bEnMidFilter;			//中值滤波使能
	int m_Midfilter_Old_BlockNo;

	//效率统计
	int m_Efficent_Evalu_FileA_Max_X[8];   //效率统计预值
	int m_Efficent_Evalu_FileA_Max_Y[8];	//效率统计预值
	int m_Efficent_Evalu_Max_X[8];			//效率统计最大值位置
	int m_Efficent_Evalu_Max_Y[8];			//效率统计最大值位置
	BOOL m_bEnEfficent_Evaluate;
	float m_Efficent_Evalu_SquE;
	int m_Efficent_Evalu_FileA_data[8][8][8];

	BOOL m_bShowLimitStat;  // 效率统计 显示用的控制变量

	//32峰位统计	
	int m_iPeakstat_fileA[32][8],m_iPeakstat_fileB[32][8];  //A ，B 文件
	int m_iPeakstat_file_buff[32][8]; //中转用的文件
	int m_iValue_B_A[32][8];   // B-A 差值
	float m_fSquareStatValue;		//A,B问题峰位差值  均方差

	long sdot[64][64];
	int m_Limit_Min_X[8][9];
	int m_Limit_Max_X[8][9];

	int m_Limit_Min_Y[8][9];
	int m_Limit_Max_Y[8][9];

	int m_Save_UpDownLimit_B[2][9];
	int m_Save_UpDownLimt_Lenth[2][9];
	int m_Save_Limit[8][9];
	int m_Save_B[9];
	
	bool m_bMoveUp;
	bool m_bMoveDown;

	bool m_bEnShowPeakValeRatio;  //使能显示峰谷比


	unsigned char savedata[8][8][4];  //akang 11.18
	float m_Avarage_Limit_Min_X[9];
	float m_Avarage_Limit_Min_Y[9];
	float m_Sum_Square_X;
	float m_Sum_Square_Y;
	bool bDirectShift_X_Y;

	int	m_iEdit_B_Lab;
	int m_iEdit_B_CurValue;
	bool m_bEnSave_Edit_B_Lab;
	bool bSetValue_OverLimit;

	int m_itemp_lab1;
	int m_itemp_lab2;
	bool m_bDraw_Small_X;
	bool m_bDraw_Small_Y;

	CFile	fLimitfile;

	//-------【64能谱操作】--------//
	BOOL  m_bEnergeSpectOperate_En;

	int	m_iEnergeSpectrum64[8][64][64];  //8:块号，64：条号；64：数值

	BOOL m_bEnergeSpectSelEn; //能谱64小图选择使能
	BOOL m_bEnergeSpectZoomInMode; //能谱的放大模式
	int m_iEnergeSpectShowNum;	//能谱放大显示号码

	int m_iEnergeSpectEth_Low[8][64];	//能谱下阈 block，条号；
	int m_iEnergeSpectEth_Hight[8][64];
	int m_iEnergeSpectCurBlockNo;
	int m_iEnergeSpectCurSynNo; //当前晶体号
	BOOL m_bEnergeSpectMove_Eth_Low;
	BOOL m_bEnergeSpectMove_Eth_Hight;

	BOOL m_bCalc32BuckEnergeLimit;  //计算32Bucket能谱边界数据标志
	BOOL m_bCalc32BuckEnergeLimit_smallWidth; //计算32Bucket能谱边界数据 窄宽度标志

	BOOL m_bMoving64EnergLimitEn; //移动64 能谱边界 的使能 标志

	int m_i64EthPara[6][3];  //[5]:表示环线类型，[3]: 阈值 数据 等

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

	//new add  文件整合
	int m_8Block_Position_X0[8][8][8];
	int m_8Block_Position_X1[8][8][8];
	int m_8Block_Position_Y0[8][8][8];
	int m_8Block_Position_Y1[8][8][8];
	int m_8Block_Cur_Sel; 


	//32 files Open
	CString m_strOpen32FileName; 
	int m_iOpen32FileNo;  
	BOOL m_b32FileOpenEn;


public:
	void OnOutput32BKEvelute(CString outfilename,int buck,int block,int *effectdata, float effectSquareSalue,BOOL boutRelativevalue);
	void OnSet32EnergeLimitPara();
	BOOL OnDeal2Save32EnergerLimit(CString filename);
	void OnFastCalcEthLimit32Buck();
	void m_Show64EngerSpectrum();
	void OnFastOpen32FileKeyDec();
	void OnFastOpen32FileKeyAdd();
	
	void OnMenu32BuckAnolParaModity(int bucket,int inPeakPosition,CString filename);
	void OnPeakValeStat_MemDCRedraw(CDC * pDC);
	void OnLimitStat_MemDCRedraw(CDC* pDC);
	void RedrawLimitFile_To_MemDC(CDC* pDC);
	void OnRedrawReal();
	void OnRedraw();
	void OnMenu32buckPeakOutText(int buckno,CString outfilename);
	BOOL OnEffectSaveData2File(CString outfilename, int buck,int block,int * effectdata, float effectSquareSalue,BOOL boutRelativevalue);
	void OnEffectCalFirstEvalute();
	void OnMouseFastCal32EthPeakStat();
	void OnSaveEthPeakStat(CString filename,int bucketno,int *peakvalue);
	void OnPeakPositionStat();
	void OnMidfilter();
	void OnShowPeakValeRatio(int no);
	void OnFastCalcEthLowHighLimit();
	void OnLoadEthFile();
	void OnSaveEthFile();
	void OnDrawZoomInEnergeSpect(int nblock,int num);
	void OnDraw64EngerSpectrum();
	void DocToClient(CRect& rect);
	void DocToClient(CPoint& point);
	void OnOpenShiftLimitFile();
	void OnPreSaveLimitFile();
	void RedrawLimitFile(void);
	int m_iValueX_Y;
	int m_iValue3d_2d;
	long ColorSet(long nCol,long nu);
	virtual ~CMaintView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	void Calculate_X_Limit();
	void Calculate_Y_Limit();

	//{{AFX_MSG(CMaintView)
	afx_msg void OnShowbar3dRadio();
	afx_msg void OnShowbar2dRadio();
	afx_msg void On2dShowX();
	afx_msg void On2dShowY();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnCalculateLimit();
	afx_msg void OnSaveCurLimit();
	afx_msg void OnRedrawLimit();
	afx_msg void OnLimitStat();
	afx_msg void OnOpenLimittab();
	afx_msg void OnMenuFastopen();
	afx_msg void OnSelchangeShowBlockSel();
	afx_msg void OnOffset1_Add();
	afx_msg void OnOffset1_Dec();
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnMenuDatasample();
	afx_msg void OnMenuAnol();
	afx_msg void OnEnMovewholeline();
	afx_msg void OnPeakValeStat();
	afx_msg void OnCheckVerHor();
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnCheckMidfilter();
	afx_msg void OnMenuLoad32bk();
	afx_msg void OnLP_Evaluate();	
	afx_msg void OnEffecentEvaluat();	//int block
	afx_msg void OnView32bklpEvaluate();
	afx_msg void OnMenu32bucketPeakStat();
	afx_msg void OnRedrawButton();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnMenu32bkParaModify();
	afx_msg void OnMenu32bkParaManulModify();
	afx_msg void OnMenuFastopen32file();
	afx_msg void OnMenuFastopen32fileNext();
	afx_msg void OnMenuFastopen32fileLast();
	afx_msg void OnRButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnFileSaveEnergeLimit();
	afx_msg void OnOutput32bklpEvaluate();
	//}}AFX_MSG
	afx_msg LRESULT OnModGoodBye(WPARAM wParam,LPARAM lParam);

	DECLARE_MESSAGE_MAP()
private:
	void OnPaintLine(int *B, int direct);
};

#ifndef _DEBUG  // debug version in MaintView.cpp
inline CMaintDoc* CMaintView::GetDocument()
   { return (CMaintDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINTVIEW_H__D7D6420D_BF45_11D6_9644_00E04C50B69B__INCLUDED_)
