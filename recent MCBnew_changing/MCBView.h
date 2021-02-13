// MCBView.h : interface of the CMCBView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MCBVIEW_H__634704AA_C885_4C4C_A793_78CECE1DB874__INCLUDED_)
#define AFX_MCBVIEW_H__634704AA_C885_4C4C_A793_78CECE1DB874__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define WM_SENDSHOW		WM_USER+102
#define WM_RECSHOW		WM_USER+103

#define WM_PRODUCE_USB_DATA   WM_USER+104

#include "MainFrm.h"
// #include "DlgPrint.h"	// Added by ClassView

//#include "SetHardWarePara.h"
//#include "Test.h"

//class CTest;
class CSetHardWarePara;   //akang add
class CEnergeKedu;
class CWidthStat;

class CMCBView : public CView
{
private:
	CSetHardWarePara *m_pHardWarePara;	//akang add
    HWND m_hHardWareModelDlg;

	CEnergeKedu *m_pEnergeKedu;
	HWND m_hEnergeKedu;

	CWidthStat *m_pWidthStat;
	HWND m_hWidthStat;


protected: // create from serialization only
	
	
	CMCBView();
	DECLARE_DYNCREATE(CMCBView)

// Attributes
public:
//	CMCBDoc* GetDocument();

// Operations
public:
	CStatusBar *pStatus;
	
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMCBView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void OnInitialUpdate();
	virtual BOOL DestroyWindow();
	virtual void OnPrepareDC(CDC* pDC, CPrintInfo* pInfo = NULL);
	protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual void OnPrint(CDC* pDC, CPrintInfo* pInfo);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMCBView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;

#endif


public:
	void Redraw();
	void OnDrawSmallWin(CDC* pDC,int centerpoint,int m_iSmallWinCHL, int m_iSmallWinCHR);
	void OnDrawBackgroud(CDC* pDC);
    void OnPrintSmallWin(CDC* pDC,int centerpoint,int m_iSmallWinCHL, int m_iSmallWinCHR);
	BOOL OnPrintExpRamLine(CDC* pDC);
    void OnPrintBackgroud(CDC * pDC);
	CMCBIO m_mcbio;

public: 
	BOOL OnDrawExpRamLine(CDC* pDC);
	void OnReDrawExpRamLine(CDC* pDC);
	void OnDrawFullRamLine(CDC* pDC);
	BOOL OnUnpack(unsigned char* recbuff,int recpackagelenth);
	

public:
	void OnMenuWidthStat(int ivalue);
	void OnMouseReport();
	BOOL OnMousePeak_FIT(float* peak_position,float* peak_cnt);
	BOOL OnMouseCalcutPeakWidth(int indiv,float* outwidth);
	BOOL OnMouseCalcutNet_ROI_Array();
	void OnMouseWholeDataFilter();
	void OnMouseCalulateWholeSum();
	BOOL OnGetCurInterest_Limits(void);
	void OnMouseCalulateSum();
	void OnMouseClearInterestRange();
	void OnMouseSetInterestRange();
	void OnDrawTextBgroud(CDC* pDC);
	void OnShowStateText(CDC* pDC,int line,  CString strtext);
	void OnExpRamModify();
	void OnExternal2ExpRam();


public:	
	bool m_icontinueflag;
	void NewIniFile();
	void CloseSave();
	bool m_continueflag;
	void OnPrintCoo(CDC *pDC);
	void PrintMyDraw(CDC* pDC);
	void PrintView();
	void OnButtonPrint(void);
	void OnUsbWrFlashPara();
	void OnUsbParaSet(BOOL Enreset);
	void OnFullRam2ExpRamInsertVal();
	float OnCaculateFullRamEthValue();
	float OnCaculateExpRamEthValue();
	BOOL OnPrintMapCalculateData(float* outData);
	void OnPrintMapTable(CDC *pdc, int lineNo, int startTable_X, int startTable_Y,float* pdata);
	BOOL m_bPrintMapEn;
	void OnHandleSelectMCBmode();
	BOOL OnHandChangeSmallWin(int pointx,int pointy);
	void OnHandleSelectBuffmode();
	
	void OnHardWareParaUnpack(unsigned char * pdata, int size);
	void OnMenuLoadTimerCount();
	BOOL SecondToDate(MYDATE *date,UINT *second);
	BOOL DateToSecond(UINT *second,MYDATE *date);
	void OnMenuProduceEnergeScale(float fvalue);
	void OnMenuSelectEnergeScale();
	void OnMenuSelectWidthStat();

    void ResumeW();
	void TextScroll(CString s);
	//ͨ���õĽ��ջ��� �� ָ��
	unsigned char* m_recbuff;
	unsigned char* m_secrecbuf;
	unsigned char* m_precbuf; 
	int m_iworkcounter;

    BOOL m_bLoad1PackageUsbData;   //�ϴ�USB�ӿڵ�MCB���ݣ�ֻ���ϴ�һ������

	UINT m_iUsbSampleTimeCnt;  //Usb����ģʽ�£�����ʱ�������
	int m_iSetSampleTime;			//���ò���ʱ��
	BOOL m_bSetSampleTimeRealMode;  //����Ϊʵʱ����ģʽ

	BOOL m_bEnReadFPGATimeValue; // �Ƿ��ȡ����ʱ��
	int m_iLiveTime;
	int m_iRealTime;
	int m_ExpRamDataLong_old;
	int m_ExpRamDataLong_new;
	BOOL m_bExpRamLongNew_valid;

	int	m_iProgCtrlData[10];		//�̿�����

	//��Ҫ���ݻ�����
	float* m_farrFullRam;
	float* m_farrExpRam;
    float* m_farrExpRamprint;

	UINT* m_arrFullRam;
	UINT* m_arrExpRam;
	int m_SampleRealTime;
	int m_SampleLiveTime;
	int m_InterestChannalCounter;

	UINT* m_arrExternalRam;
	UINT* m_arrBUFFExternalRam;
	UINT* m_arrMCBExternalRam;

	UINT* m_pUsbProduceBuff;

	//���ݴ���������
	//UINT m_iNet_ROI_Array[1024*8];
	int* m_iNet_ROI_Array;
	UINT m_iInterest_LCh;	//�������Ȥ�� ��߽�
	UINT m_iInterest_RCh;	//�������Ȥ�� �ұ߽�
	float m_fPeak_Position;
	float m_fPeak_Cnt;

//	BOOL m_bMenuWidthStat;
	CTime  time;

	//�����̶Ȳ�������
	BOOL m_bMenuEnergeScaleFinish;
	float m_fMenuEnergeScaleFirstVal;
	float m_fMenuEnergeScalePeakP1;
	float m_fMenuEnergeScaleSecondVal;
	float m_fMenuEnergeScalePeakP2;
	float m_fMenuEnergeScale_K;
	float m_fMenuEnergeScale_b;
	bool  m_bMenuEnergeScale_CalcuEn;  // �����̶ȼ���ʹ��

	
	//���ƴ��� ����
	int m_iViewWindowSize_X;	//��ͼ�Ĵ��ڴ�С����X����
	int m_iViewWindowSize_Y;	//��ͼ�Ĵ��ڴ�С����Y����

	int	m_iSmallWinCenPoint;
	int m_iSmallWinCHL;
	int m_iSmallWinCHR;
	int m_iBigWinCenPoint;
	int m_iSamllWinWidth;

	BOOL m_bDrawLine_Sel;   //��չ�� ���ߡ������ѡ��

	unsigned char m_iBuffComboxSel;

	int m_iSetInterestLimitNo1;
	int m_iSetInterestLimitNo2;
	BOOL m_bSetInterestLimitFinish;

	int m_iClearInterestLimitNo1;
	int m_iClearInterestLimitNo2;
	BOOL m_bClearInterestLimitFinish;

	CString m_strStateLine1Text;		
	CString m_strStateLine2Text;
	CString m_strStateLine3Text;
	CString m_strStateLine4Text;
	CString m_strStateLine5Text;

	int m_iMouseDoubleHitNo;    //˫�����ʱ������Ȥ����
	BOOL m_bMouseDoubleHit;     //���˫����־

	unsigned int m_iFullChannelWidth;	//ȫ�׵��� (1024,2048,4096,8192����)
  
	BOOL m_bAutoCalcuExpRamEth; //�����Զ�������չ�յ���ֵ
	BOOL m_bAutoCalcuFullRamEth; //�����Զ�����ȫ�׵���ֵ

	BOOL m_bDrawLogData_Sel;  //ѡ�� �Զ�����ʽ ��ʾ����

	BOOL m_bUsbOnTimeShow; //����Usb��ʱ��ʾ/��ȡ�豸������
	UINT m_iUsbRecPackageCnt; //Usb����MCB���ݵİ���

	UINT m_iReadFlashSel;

	COLORREF m_Full_BackGround_Color;   //ȫ�ױ�����ɫ
	COLORREF m_Exp_BackGround_Color;    //��չ�ױ�����ɫ
	COLORREF m_Interest_Color;  //����Ȥ����ɫ
	COLORREF m_SmallWin_BackGround_Color;  //FULLС���ڱ���ɫ

	
	BOOL m_bMCBModeSel;
	bool m_startflag;
	bool m_1;  /////////////////changing for print
	bool m_2;
	bool m_3;
	bool m_4;
	bool m_5;
	bool m_smallwin;
	bool m_centerline;
	int  m_SmallWinCHL;
	int  m_SmallWinCHR;
	int  m_SmallWinWidth;
	int  m_selfwidthmin;
	int  m_selfwidthmax;
	int  m_selfvalue;
	int  m_saveselfvalue;
    int  m_timelen;
	int  m_len2;
	int  m_len3;
    ////////////////////////////////////////////////////////////changing BUF SCALE
    unsigned int  m_iBuffFullChannelWidth;
    unsigned int  m_iBuffSmallWindowWidth;
	unsigned char m_iBuffFullChannelWidthSel;
	unsigned char m_iBuffSmallWindowWidthSel;
	unsigned int  m_iBuffSmallWinCenPoint;
	 int  m_iBuffSmallWinCHL;
	unsigned int  m_iBuffSmallWinCHR;	
    unsigned int  m_iBuffFullRamEthValue;
    unsigned int  m_iBuffExpRamEthValue;
    unsigned char m_iBuffFullRamCheckSel;
    unsigned char  m_iBuffExpRamCheckSel;
	unsigned char  m_bBuffExpRamViewSel;
	UINT m_iBuffLiveTimeValuex;
	CScrollBar m_ScrollBar;
	int m_pos;
	int m_textlength;
	CString m_scrollstring[2000];
	int m_starttime_hour;
	int m_starttime_min;
	int m_starttime_sec;
	bool m_scrollflag;
	CFont font;
	/////////////////////////////////////////////////////////////////
    bool m_iTimeMcbSourceflag;
	unsigned char m_iTimeFileSourceflag;
    
	CString  m_iRealTimeValue;
	CString  m_iLiveTimeValue;
	CString  m_iTimeRatio;
	CString  m_iBuffRealTimeValue;
	UINT m_iBuffRealTimeValuex;
	CString  m_iBuffLiveTimeValue;
	CString  m_iBuffTimeRatio;
    unsigned char m_fMenuEnergeScale_U;
	bool  m_bMcbMenuEnergeScale_CalcuEn;

protected:

	BOOL m_bSmallWinPointMoveEN;
	CPoint m_Oldpoint;

	unsigned char m_iFullRamCheckSel;		
	int m_iFullRamEthValue;
	UINT m_iExpRamEthValue;
	unsigned char m_iSmallWindowWidthCheckSel;

	//

	BOOL m_bExpRamViewSel;	//ѡ��ExpRam ��ʾ��
	

	//�ļ������ͷ�� �� ʱ�����
	UINT m_iSaveFileTime;
	UINT m_iSaveFileRealTime;
	UINT m_iSaveFileLiveTime;


// Generated message map functions
protected:

	//{{AFX_MSG(CMCBView)
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnExpramViewsel();
	afx_msg void OnFullramViewsel();
	afx_msg void OnMcbModsel();
	afx_msg void OnBuffModsel();
	afx_msg void OnSelchangeComboHor();
	afx_msg void OnMenuFileOpen();
	afx_msg void OnMenuFileSaveAs();
	afx_msg void OnSelchangeComboVer();
	afx_msg void OnRealtimeSel();
	afx_msg void OnLivetimeSel();
	afx_msg void OnDestroy();
	afx_msg void OnCancelMode();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnExternaldataMcb2Buff();
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMenuSetCommport();
	afx_msg void OnMenuReceivesample();
	afx_msg void OnMenuResetsample();
	afx_msg void OnMenuStartsample();
	afx_msg void OnMenuStopsample();
	afx_msg void OnMenuTestsample();
	afx_msg void OnMenuContinuesample();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnMenuWholedataFilter();
	afx_msg void OnMenuWholedataSum();
	afx_msg void OnMenuOutReport();
	afx_msg void OnMenuInterestSum();
	afx_msg void OnMenuSendsample();
	afx_msg void OnMenuLinePointSel();
	afx_msg void OnMenuClearBuff();
	afx_msg void OnHardwareParaSet();
	afx_msg void OnMenuEnergeKedu();
	afx_msg void OnIdMenuWidthStat();
	afx_msg void OnIdMcbHelp();
	afx_msg void OnMenuLogShow();
	afx_msg void OnMenuLineShow();
	afx_msg void OnMenuColorSet();
	afx_msg void OnSelchangeComboExpHor();
	afx_msg void OnSelchangeComboExpVer();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnMove(int x, int y);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	//}}AFX_MSG

	afx_msg LRESULT OnHardWareGoodBye(WPARAM wParam,LPARAM lParam);
    afx_msg LRESULT OnWidthStatGoodBye(WPARAM wParam,LPARAM lParam);
	afx_msg LRESULT OnWidthStatIDok(WPARAM wParam,LPARAM lParam);

	afx_msg LRESULT OnEnergeKeduGoodBye(WPARAM wParam,LPARAM lParam);
	afx_msg LRESULT OnEnergeKeduIDok(WPARAM wParam,LPARAM lParam);

	afx_msg LRESULT OnSendShow(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnRecShow(WPARAM wParam, LPARAM lParam);

	afx_msg LRESULT OnProduceUsbData(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()
private:
	void OnPrintDrawTextBgroud(CDC*pDC);
	void ResumeR();
//	void ResumeW();
	void OnprintCaculateLog();
	float Fullmaxvalue;
	void ReDrawExpLine();
	int m_rangemode;
	int m_selfrange;
	int m_printmode;

	int m_range;
	void PrintPageFooter(CDC * pDC);
	void PrintPageHeader(CDC *pDC);
};


UINT PrintMyThread(LPVOID  lpData);

#ifndef _DEBUG  // debug version in MCBView.cpp
inline CMCBDoc* CMCBView::GetDocument()
   { return (CMCBDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MCBVIEW_H__634704AA_C885_4C4C_A793_78CECE1DB874__INCLUDED_)





















