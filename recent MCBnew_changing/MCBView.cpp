// MCBView.cpp : implementation of the CMCBView class
//
#include "stdafx.h"
#include "MCB.h"

#include "MCBIO.h"
#include "CommApi.h"
#include "EzusbApi.h"
#include "UsbParaSet.h"
#include "ReadFlashSel.h"

#include "MCBDoc.h"
#include "MCBView.h"
#include "MainFrm.h"
#include "math.h"
#include "ProgCtrl.h"
#include "ComSel.h"
#include "Math.h"
#include "SetHardWarePara.h"
#include "EnergeKedu.h"
#include "WidthStat.h"
#include "ini.h"
#include "ColorSet.h"
#include "DLGPRINT1.h"



#include <io.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>



extern HARDWARE_PARE g_HardWare_Pare;


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "debug.h"
//#include "Fast57600.h"
HWND g_hEdit;

DWORD g_ComPortSel = COM1_PORT;
DWORD g_CommBaud = 57600;
HANDLE g_hComm;
HANDLE g_hUsb;
HANDLE g_hUsbReadThread;
BOOL g_bRunUsbReadDataThread;
BOOL g_bEnReadFlashData = FALSE;

int g_iSetInterestLimitNo1;	//����Ȥ��E
int g_iSetInterestLimitNo2;

extern CWinThread* g_pThread_Monitor; //����߳�ָՁE
extern CWinThread* g_pThread_Send;		//�����߳�ָՁE
extern bool g_bRunMonitorThread;
extern bool g_bRunSendThread;

extern int	g_RecPackageLenth; //ȷ��ÿ���������ݰ�E� ����
extern UINT		g_ThreadCMD;
extern BOOL		g_bExec;		//���ڽ���ͨ�ű�E�
extern BOOL		g_bComOpen;
extern HANDLE		g_hEvtSend;
extern HANDLE		g_hEvtFinishRec;    //́Eɽ��վ���������շ�ͬ���á� 
extern HANDLE		g_hExitSendThread;
extern HANDLE		g_hExitMonitThread;


extern MCB_OPER	g_MCB_Oper;			// ������Ƶ� ��ز�������
extern TYPE	type;	//����ת��
extern HARDWARE_PARE g_HardWare_Pare;  // Ӳ�������ṹ
extern FILEHEADER FileHeader; //�ļ�ͷ
extern MYDATE date;


DWORD WINAPI UsbReadDataThread(LPVOID  lpData);


/////////////////////////////////////////////////////////////////////////////
// CMCBView

IMPLEMENT_DYNCREATE(CMCBView, CView)

BEGIN_MESSAGE_MAP(CMCBView, CView)
	//{{AFX_MSG_MAP(CMCBView)
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_BN_CLICKED(IDC_EXPRAM_VIEWSEL, OnExpramViewsel)
	ON_BN_CLICKED(IDC_FULLRAM_VIEWSEL, OnFullramViewsel)
	ON_BN_CLICKED(IDC_MCB_MODSEL, OnMcbModsel)
	ON_BN_CLICKED(IDC_BUFF_MODSEL, OnBuffModsel)
	ON_CBN_SELCHANGE(IDC_COMBO_HOR, OnSelchangeComboHor)
	ON_COMMAND(ID_MENU_FILE_OPEN, OnMenuFileOpen)
	ON_COMMAND(ID_MENU_FILE_SAVE_AS, OnMenuFileSaveAs)
	ON_CBN_SELCHANGE(IDC_COMBO_VER, OnSelchangeComboVer)
	ON_BN_CLICKED(IDC_REALTIME_SEL, OnRealtimeSel)
	ON_BN_CLICKED(IDC_LIVETIME_SEL, OnLivetimeSel)
	ON_WM_DESTROY()
	ON_WM_CANCELMODE()
	ON_WM_TIMER()
	ON_WM_KEYUP()
	ON_WM_KILLFOCUS()
	ON_BN_CLICKED(IDC_EXTERNALDATA_MCB2BUFF, OnExternaldataMcb2Buff)
	ON_WM_RBUTTONDOWN()
	ON_COMMAND(AFX_ID_SET_COMMPORT, OnMenuSetCommport)
	ON_COMMAND(AFX_ID_RECEIVESAMPLE, OnMenuReceivesample)
	ON_COMMAND(AFX_ID_RESETSAMPLE, OnMenuResetsample)
	ON_COMMAND(AFX_ID_STARTSAMPLE, OnMenuStartsample)
	ON_COMMAND(AFX_ID_STOPSAMPLE, OnMenuStopsample)
	ON_COMMAND(AFX_ID_TESTSAMPLE, OnMenuTestsample)
	ON_COMMAND(AFX_ID_CONTINUESAMPLE, OnMenuContinuesample)
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDBLCLK()
	ON_COMMAND(AFX_ID_WHOLEDATA_FILTER, OnMenuWholedataFilter)
	ON_COMMAND(AFX_ID_WHOLEDATA_SUM, OnMenuWholedataSum)
	ON_COMMAND(AFX_ID_OUT_REPORT, OnMenuOutReport)
	ON_COMMAND(AFX_ID_INTEREST_SUM, OnMenuInterestSum)
	ON_COMMAND(AFX_ID_SENDSAMPLE, OnMenuSendsample)
	ON_COMMAND(AFX_ID_LINE_POINT_SEL, OnMenuLinePointSel)
	ON_COMMAND(AFX_ID_CLEAR_BUFF, OnMenuClearBuff)
	ON_COMMAND(AFX_ID_HARDWARE_PARA_SET, OnHardwareParaSet)
	ON_COMMAND(AFX_ID_ENERGE_KEDU, OnMenuEnergeKedu)
	ON_COMMAND(AFX_ID_WIDTH_STAT, OnIdMenuWidthStat)
	ON_COMMAND(AFX_ID_MCB_HELP, OnIdMcbHelp)
	ON_COMMAND(AFX_ID_LOG_SHOW, OnMenuLogShow)
	ON_COMMAND(AFX_ID_LINE_SHOW, OnMenuLineShow)
	ON_COMMAND(AFX_ID_COLOR_SET, OnMenuColorSet)
	ON_CBN_SELCHANGE(IDC_COMBO_EXP_HOR, OnSelchangeComboExpHor)
	ON_CBN_SELCHANGE(IDC_COMBO_EXP_VER, OnSelchangeComboExpVer)
	ON_WM_CREATE()
	ON_WM_VSCROLL()
	ON_WM_MOVE()
	ON_WM_KEYDOWN()
	ON_WM_HSCROLL()
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
	ON_COMMAND(IDD_PRINT_PRE,OnButtonPrint)	//���Ӱ�ť

	ON_MESSAGE(WM_SENDSHOW, OnSendShow)
	ON_MESSAGE(WM_RECSHOW, OnRecShow)
	ON_MESSAGE(WM_HARDEWAREPARA_GOODBYE,OnHardWareGoodBye)
	ON_MESSAGE(WM_WIDTHSTAT_GOODBYE,OnWidthStatGoodBye)
	ON_MESSAGE(WM_WIDTHSTAT_IDOK,OnWidthStatIDok)

	ON_MESSAGE(WM_ENERGE_KEDU_GOODBYE,OnEnergeKeduGoodBye)
	ON_MESSAGE(WM_ENERGE_KEDU_IDOK,OnEnergeKeduIDok)

	ON_MESSAGE(WM_PRODUCE_USB_DATA,OnProduceUsbData)
    

END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMCBView construction/destruction

CMCBView::CMCBView()
{
	m_pHardWarePara = new CSetHardWarePara(this);
	m_pEnergeKedu = new CEnergeKedu(this);
	m_pWidthStat = new CWidthStat(this);
	
	
	m_arrFullRam = new UINT[8192];
	m_arrExpRam = new UINT[8192];
	m_farrFullRam = new float[8192];
	m_farrExpRam = new float[8192];
    m_farrExpRamprint=new float[8192];

	m_pUsbProduceBuff = new UINT[8192*8];
	m_arrExternalRam = new UINT[8192*8];
	m_arrBUFFExternalRam = new UINT[8192*8];
	m_arrMCBExternalRam = new UINT[8192*8];

	m_iNet_ROI_Array = new int[8192*8];

	m_recbuff = new unsigned char[8192*4*8];
	m_secrecbuf = new unsigned char[8192*4*8];
	
	memset(m_iNet_ROI_Array,0,8192*8*4);

	memset(m_recbuff,0,8192*4*8);
	memset(m_recbuff,0,8192*4*8);

	memset(m_farrFullRam,0,8192*4);
	memset(m_farrExpRam,0,8192*4);
    memset(m_farrExpRamprint,0,8192*4);

	memset(m_arrFullRam,0,8192*4);
	memset(m_arrExpRam,0,8192*4);
	memset(m_arrExternalRam,0,8192*4*8);

	memset(m_arrBUFFExternalRam,0,8192*4*8);
	memset(m_arrMCBExternalRam,0,8192*4*8);	
	memset(m_pUsbProduceBuff,0,8192*4*8);

	m_bMCBModeSel= TRUE;
	m_bLoad1PackageUsbData = FALSE;
	m_strStateLine1Text = " ";
	m_strStateLine2Text = " ";
	m_strStateLine3Text = " ";
	m_bMouseDoubleHit = FALSE;
	m_bMenuEnergeScale_CalcuEn = FALSE;
	m_fMenuEnergeScale_K = 0;
	m_fMenuEnergeScale_b = 0;
	m_iLiveTime = 0;
	m_iRealTime = 0;

	m_iUsbRecPackageCnt = 0;

	m_iBuffComboxSel = 17;
	m_iFullRamCheckSel= 17;		
	m_iFullRamEthValue = 64;
	m_iExpRamEthValue = 64;
	m_iSmallWindowWidthCheckSel =2;
	m_iSmallWindowWidthCheckSel=2;
	m_iSamllWinWidth=256;
	//����һ�𶯣��͸�Eµ�ǰ��ʱ��ֵ 
	time = CTime::GetCurrentTime();
	date.year = time.GetYear();
	date.month = time.GetMonth();
	date.day = time.GetDay();
	date.hour = time.GetHour();
	date.minite = time.GetMinute();
	date.second = time.GetSecond();	
	DateToSecond(&m_iSaveFileTime,&date);
//	m_iSaveFileTime = 0;
	m_iSaveFileRealTime = 0;
	m_iSaveFileLiveTime = 0;
	m_bDrawLine_Sel = false;

	m_iReadFlashSel = 0;

	m_bEnReadFPGATimeValue = false;
	m_strStateLine4Text = "�ļ����� ***    ʱ�䣺***   ";
	m_strStateLine5Text = "ʵʱ�䣺 ***    ��汼䣺***   ";

	m_bMenuEnergeScaleFinish = false;

	m_bPrintMapEn = FALSE;

	m_bAutoCalcuExpRamEth = FALSE;
	m_bAutoCalcuFullRamEth = FALSE;
	//////////////////////////////////////////////////////////////////////////
	g_iSetInterestLimitNo1 = 0;
	g_iSetInterestLimitNo2 = 0;

    m_continueflag=0;
	m_startflag=0; /////////////////changing
     ///////////////////////////////changing print
	 m_1=0;
	 m_2=0;
	 m_3=0;
	 m_4=0;
	 m_5=0;
	 m_printmode=0;

     m_SmallWinWidth=0;
	 m_SmallWinCHL=0;
	 m_SmallWinCHR=0;
	 m_selfwidthmin=0;
	 m_selfwidthmax=0;
	 m_selfvalue=0;
	 m_saveselfvalue=0;
	 m_rangemode=0;
	 m_range=0;

     m_iBuffFullChannelWidth=0;
     m_iBuffSmallWindowWidth=0;
	 m_iBuffFullChannelWidthSel=0;
	 m_iBuffSmallWindowWidthSel=0;
     m_iBuffSmallWinCenPoint=0;
	 m_iBuffSmallWinCHL=0;
	 m_iBuffSmallWinCHR=0;	
//     m_iBuffFullRamEthValue;
//     m_iBuffExpRamEthValue;
     m_iBuffFullRamCheckSel=17;
     m_iBuffExpRamCheckSel=17;
	 m_bBuffExpRamViewSel=FALSE;
	 m_iBuffSmallWindowWidth=256;


     m_iTimeMcbSourceflag=0;
	 m_iTimeFileSourceflag=2;

     m_fMenuEnergeScale_U=0;
	 m_bMcbMenuEnergeScale_CalcuEn=FALSE;
	 m_iBuffRealTimeValuex=0;
	 m_iBuffLiveTimeValuex = 0;
	 m_pos=0;
	 m_textlength=1;
	 m_scrollflag=0;
	 m_icontinueflag=0;
}

CMCBView::~CMCBView()
{
	delete m_arrExternalRam;
	delete m_arrFullRam;
	delete m_arrExpRam;
	delete m_farrFullRam;
	delete m_farrExpRam;
    delete  m_farrExpRamprint;

	delete m_pUsbProduceBuff;
	delete m_arrBUFFExternalRam;
	delete m_arrMCBExternalRam;
	delete m_recbuff;
	delete m_secrecbuf;

	delete m_iNet_ROI_Array;

	delete m_pHardWarePara;
	delete m_pEnergeKedu;
	delete m_pWidthStat;

}

UINT PrintMyThread(LPVOID  lpData)
{
	if (((CDC *)lpData)->IsPrinting())
	{	 
		/*m_iSamllWinWidth=m_SmallWinWidth;
		m_iSmallWinCHL=m_SmallWinCHL;
		m_iSmallWinCHR=m_SmallWinCHR;
		m_iExpRamEthValue=m_saveselfvalue;
		Redraw();
		AfxMessageBox("shit");
       AfxEndThread(0,TRUE);*/
	}
//	AfxMessageBox("shit");

	return 1;
}

void CMCBView::DoDataExchange(CDataExchange* pDX) 
{
	// TODO: Add your specialized code here and/or call the base class
	CView::DoDataExchange(pDX);
}

BOOL CMCBView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CMCBView drawing

void CMCBView::OnDraw(CDC* pDC)
{
	CRect ViewRec;
	GetWindowRect(ViewRec);
	m_iViewWindowSize_X = ViewRec.Width();
	m_iViewWindowSize_Y = ViewRec.Height();
	m_iBigWinCenPoint = m_iViewWindowSize_X/2;

    m_ScrollBar.SetWindowPos(NULL,m_iViewWindowSize_X-20,m_iViewWindowSize_Y*42/50+2,20,m_iViewWindowSize_Y*8/50,SWP_SHOWWINDOW);
    m_ScrollBar.SetScrollPos(m_pos);
    m_ScrollBar.ShowWindow(TRUE);
	
	CMainFrame* pMainFrame = (CMainFrame*)GetParentFrame();
	CStatic *pTime=(CStatic *)(pMainFrame->m_wndDlgBar.GetDlgItem(IDC_STATIC_TIME));
    CStatic *pRealTime=(CStatic *)(pMainFrame->m_wndDlgBar.GetDlgItem(IDC_SHOW_REAL_TIME));
    CStatic *pLiveTime=(CStatic *)(pMainFrame->m_wndDlgBar.GetDlgItem(IDC_SHOW_LIVE_TIME));
    CStatic *pRatioTime=(CStatic *)(pMainFrame->m_wndDlgBar.GetDlgItem(IDC_SHOW_RATIO_TIME));
    CStatic *pChannel=(CStatic *)(pMainFrame->m_wndDlgBar.GetDlgItem(IDC_STATIC_CHANNEL));
    CStatic *pCount=(CStatic *)(pMainFrame->m_wndDlgBar.GetDlgItem(IDC_STATIC_COUNT));
	CStatic *pTimeout=(CStatic *)(pMainFrame->m_wndDlgBar.GetDlgItem(IDC_QUERY_TIMEOUT));
    CStatic *pSamplestate=(CStatic *)(pMainFrame->m_wndDlgBar.GetDlgItem(IDC_QUERY_SAMPLE_STATE));
    CButton *pRealTimeSel=(CButton *)(pMainFrame->m_wndDlgBar.GetDlgItem(IDC_REALTIME_SEL));
    CButton *pLiveTimeSel=(CButton *)(pMainFrame->m_wndDlgBar.GetDlgItem(IDC_LIVETIME_SEL));
    CEdit *pTimeEdit=(CEdit *)(pMainFrame->m_wndDlgBar.GetDlgItem(IDC_TIMER_EDIT));
	CButton *pMCBModeSel=(CButton *)(pMainFrame->m_wndDlgBar.GetDlgItem(IDC_MCB_MODSEL));
	CButton *pBUFFModeSel=(CButton *)(pMainFrame->m_wndDlgBar.GetDlgItem(IDC_BUFF_MODSEL));
    CStatic *pTimeUnit=(CStatic *)(pMainFrame->m_wndDlgBar.GetDlgItem(IDC_STATIC_TIMEUNIT));  
	CButton *pMCB2BUF=(CButton *)(pMainFrame->m_wndDlgBar.GetDlgItem(IDC_EXTERNALDATA_MCB2BUFF));
    CStatic *pGain=(CStatic *)(pMainFrame->m_wndDlgBar.GetDlgItem(IDC_STATIC_GAIN));
    CStatic *pGainVer=(CStatic *)(pMainFrame->m_wndDlgBar.GetDlgItem(IDC_STATIC_GAIN_VER));
    CStatic *pHor=(CStatic *)(pMainFrame->m_wndDlgBar.GetDlgItem(IDC_STATIC_HORI));
    CStatic *pHorVer=(CStatic *)(pMainFrame->m_wndDlgBar.GetDlgItem(IDC_STATIC_HOR_VER));
    CStatic *pSystime=(CStatic *)(pMainFrame->m_wndDlgBar.GetDlgItem(IDC_STATE_SHOW));
//    CComboBox *pComboHor=(CComboBox *)(pMainFrame->m_wndDlgBar.GetDlgItem(IDC_COMBO_HOR));
//    CComboBox *pComboVer=(CComboBox *)(pMainFrame->m_wndDlgBar.GetDlgItem(IDC_COMBO_VER));
//    CComboBox *pComboExpHor=(CComboBox *)(pMainFrame->m_wndDlgBar.GetDlgItem(IDC_COMBO_EXP_HOR));
//    CComboBox *pComboExpVer=(CComboBox *)(pMainFrame->m_wndDlgBar.GetDlgItem(IDC_COMBO_EXP_VER));

//   pComboHor->SetFont(&font);
//   pComboVer->SetFont(&font);
//  pComboExpHor->SetFont(&font);
//   pComboExpVer->SetFont(&font);
   pTime->SetFont(&font);
   pRealTime->SetFont(&font);
   pLiveTime->SetFont(&font);
   pRatioTime->SetFont(&font);
   pChannel->SetFont(&font);
   pCount->SetFont(&font);
   pTimeout->SetFont(&font);
   pSamplestate->SetFont(&font);
   pRealTimeSel->SetFont(&font);
   pLiveTimeSel->SetFont(&font);
   pTimeEdit->SetFont(&font);
   pMCBModeSel->SetFont(&font);
   pBUFFModeSel->SetFont(&font);
   pTimeUnit->SetFont(&font);
   pMCB2BUF->SetFont(&font);
   pGain->SetFont(&font);
   pGainVer->SetFont(&font);
   pHor->SetFont(&font);
   pHorVer->SetFont(&font);
   pSystime->SetFont(&font);

	if ((pDC->IsPrinting()==FALSE))
	{	
		Redraw();
//		m_ScrollBar.ShowWindow(FALSE);
		m_ScrollBar.ShowWindow(TRUE);
		return;
	}
    if ((pDC->IsPrinting())==TRUE)
    {
		PrintMyDraw(NULL);
    }
	
}

/////////////////////////////////////////////////////////////////////////////
// CMCBView printing

BOOL CMCBView::OnPreparePrinting(CPrintInfo* pInfo)
{
	pInfo->SetMaxPage(1);
	//////////////////////////////////////////////////////////////////////////
	//���Դ�Eʹ��ӡ���򡪡�maxwell
	PRINTDLG pd;
	LPDEVMODE  lpDevMode;
	pd=pInfo->m_pPD->m_pd;
	if(AfxGetApp()->GetPrinterDeviceDefaults(&pd))
	{
		lpDevMode=(LPDEVMODE)GlobalLock(pd.hDevMode);
		if(lpDevMode)
		{
		lpDevMode->dmPaperSize=DMPAPER_A4;	//a4
		lpDevMode->dmOrientation=DMORIENT_LANDSCAPE;//����ӡ������Ϊ�����ӡ��
		}
    GlobalUnlock(pd.hDevMode);
	}//end of maxwell
//////////////////////////////////////////////////////////////////////////
	return DoPreparePrinting(pInfo);
}

void CMCBView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing

}


void CMCBView::OnEndPrinting(CDC* pDC, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
	
   m_1=0;
   m_3=0;


   //AfxBeginThread(PrintMyThread,(LPVOID) pDC,NULL,0,0,NULL);
}

/////////////////////////////////////////////////////////////////////////////
// CMCBView diagnostics

#ifdef _DEBUG
void CMCBView::AssertValid() const
{
	CView::AssertValid();
}

void CMCBView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

/*CMCBDoc* CMCBView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMCBDoc)));
	return (CMCBDoc*)m_pDocument;
}
*/
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMCBView message handlers

void CMCBView::OnInitialUpdate() 
{
	CView::OnInitialUpdate();
	
	CMainFrame* pMainFrame = (CMainFrame*)GetParentFrame();
	CButton* pMCBSel = (CButton *)(pMainFrame->m_wndDlgBar.GetDlgItem(IDC_MCB_MODSEL));
	CButton* pRealTimeSel = (CButton *)(pMainFrame->m_wndDlgBar.GetDlgItem(IDC_REALTIME_SEL));
//	CButton* pExpViewSel = (CButton *)(pMainFrame->m_wndDlgBar.GetDlgItem(IDC_EXPRAM_VIEWSEL));
	CComboBox* pComboxHor = (CComboBox *)(pMainFrame->m_wndDlgBar.GetDlgItem(IDC_COMBO_HOR));
	CComboBox* pComboxVer = (CComboBox *)(pMainFrame->m_wndDlgBar.GetDlgItem(IDC_COMBO_VER));
	CComboBox* pComboxExpVer = (CComboBox *)(pMainFrame->m_wndDlgBar.GetDlgItem(IDC_COMBO_EXP_VER));
	CButton* pSampleTimeValue = (CButton *)(pMainFrame->m_wndDlgBar.GetDlgItem(IDC_TIMER_EDIT));
	CButton* pMCB_TO_BUF = (CButton *)(pMainFrame->m_wndDlgBar.GetDlgItem(IDC_EXTERNALDATA_MCB2BUFF));
	CEdit* pQueryTimeout=(CEdit*)(pMainFrame->m_wndDlgBar.GetDlgItem(IDC_QUERY_TIMEOUT));
	CEdit* pQuerySampleState=(CEdit*)(pMainFrame->m_wndDlgBar.GetDlgItem(IDC_QUERY_SAMPLE_STATE));
//	CEdit *pSource=(CEdit*)(pMainFrame->m_wndDlgBar.GetDlgItem(IDC_SOURCE));
//	pSource->SetWindowText("����:��");
	
//    CEdit* pSetTime = (CEdit*)(pMainFrame->m_wndDlgBar.GetDlgItem(IDC_STATE_START_TIME));
//    pSetTime->SetWindowText("��ʼ:��            ֹͣ:��");
	
    CStatic *pTime=(CStatic *)(pMainFrame->m_wndDlgBar.GetDlgItem(IDC_STATIC_TEST));

	font.CreateFont(13,0,0,0,FW_NORMAL,FALSE,FALSE,0,
						ANSI_CHARSET,OUT_DEFAULT_PRECIS,
						CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,
						DEFAULT_PITCH | FF_SWISS ,"��́E);

	g_bComOpen = false;
	pStatus=(CStatusBar*)AfxGetApp()->m_pMainWnd->GetDescendantWindow(ID_VIEW_STATUS_BAR);
	pStatus->SetPaneInfo(0,5,SBPS_NORMAL,100);
	pStatus->SetPaneText(0,"ϵͳ�����ɹ�",1);
	pStatus->SetPaneInfo(1,5,SBPS_NORMAL,20);
	pStatus->SetPaneText(1,"����",1);
	pStatus->SetPaneInfo(2,5,SBPS_NORMAL,400);
	pStatus->SetPaneText(2,"����״̬������δ��",1);
	pStatus->SetPaneInfo(3,5,SBPS_NORMAL,800);
	pStatus->SetPaneText(3,"��ʾ",1);

	g_hUsbReadThread = INVALID_HANDLE_VALUE;
	g_hUsb = INVALID_HANDLE_VALUE;
	g_hComm = INVALID_HANDLE_VALUE;
	
	m_iSamllWinWidth = 256;
	m_bExpRamLongNew_valid = TRUE;		//ExpRam ���ȵ���ֵ��Ч

	m_bSetInterestLimitFinish = false;		//���ø���ȤǁEʹ��
	m_bClearInterestLimitFinish = false;		//���ø���ȤǁEʹ��

	pMCBSel->SetCheck(1);
	
	pRealTimeSel->SetCheck(1);
//	pExpViewSel->SetCheck(1);

	m_iSmallWinCenPoint = 200;
	m_iSmallWinCHL = m_iSmallWinCenPoint - m_iSamllWinWidth/2;
	m_iSmallWinCHR = m_iSmallWinCenPoint + m_iSamllWinWidth/2;

	m_iBigWinCenPoint = 256;
	m_bSmallWinPointMoveEN = false;

	m_ExpRamDataLong_new = (m_iSmallWinCHR - m_iSmallWinCHL +1)*3;
    
     m_iBuffFullChannelWidth=8192;
     m_iBuffSmallWindowWidth=256;
	 m_iBuffFullChannelWidthSel=3;
	 m_iBuffSmallWindowWidthSel=2;
     m_iBuffSmallWinCenPoint=200;
	 m_iBuffSmallWinCHL=m_iSmallWinCHL;
	 m_iBuffSmallWinCHR=m_iSmallWinCHR;	
     m_iBuffFullRamCheckSel=17;
     m_iBuffExpRamCheckSel=17;
     m_iRealTimeValue="ʵʱ:0.0";
     m_iLiveTimeValue="���:0.0";
     m_iTimeRatio="����:100%";
     m_iBuffRealTimeValue="ʵʱ:0.0";
     m_iBuffLiveTimeValue="���:0.0";
     m_iBuffTimeRatio="����:100%";

	m_bExpRamViewSel = true;
	m_iSetSampleTime = 0;
	m_bSetSampleTimeRealMode = true;

	m_iUsbSampleTimeCnt = 0;
    
    m_scrollstring[0].Format("%s","------------------ MCB �û���־ --------------------------");
    m_scrollstring[1].Format("%s","--- ��������");    

	int progdata[10] = {255,105,105,105,105,127,127,127,88,00};	
	memcpy(m_iProgCtrlData,progdata,10*4);
	
	g_ThreadCMD = 0x00;	

	SetTimer(3,500,NULL);
	time = CTime::GetCurrentTime();
	int intbuff;
		g_HardWare_Pare.bANSER_Pulse = TRUE;
		g_HardWare_Pare.bDT_Pulse = TRUE;
		g_HardWare_Pare.bENADC_Pulse = TRUE;
		g_HardWare_Pare.bREADY_Pulse = TRUE;
		g_HardWare_Pare.bReadorWriteOperate = TRUE;
		g_HardWare_Pare.bSampleModeRealTime = TRUE;
		g_HardWare_Pare.bDataPolarity_Pulse = TRUE;
		g_HardWare_Pare.fEngerScaleOffset = 0;
		g_HardWare_Pare.fEngerScaleSlope = 0;
		m_fMenuEnergeScale_K = g_HardWare_Pare.fEngerScaleSlope;
		m_fMenuEnergeScale_b = g_HardWare_Pare.fEngerScaleOffset;
		
		g_HardWare_Pare.iChannelWidth = 8192;
		g_HardWare_Pare.iEngerScaleUnit = 0;
		g_HardWare_Pare.iSampleTimeValue = 80000;
		g_HardWare_Pare.bInputAnolSignalSel = TRUE;

		g_HardWare_Pare.iEth_Up_UsbADC = 16383;
		g_HardWare_Pare.iEth_Down_UsbADC = 8200;

		g_HardWare_Pare.iTime_Up_ETH = 200;
		g_HardWare_Pare.iTime_Down_ETH = 8;

		m_bMenuEnergeScale_CalcuEn = FALSE;
		m_bDrawLogData_Sel = TRUE;

		g_ComPortSel = COM1_PORT;
		m_Full_BackGround_Color = RGB(128,128,128);
		m_Exp_BackGround_Color = RGB(115,138,155);
		m_Interest_Color = RGB(166,166,255);
		m_SmallWin_BackGround_Color = RGB(115,138,155);
  FILE *fp=fopen("MCBini_05_1028.ini","rb");
  if(fp==NULL)
  {
	  fp=fopen("MCBini_05_1028.ini","wb");	
	  g_HardWare_Pare.bANSER_Pulse = TRUE;
	  g_HardWare_Pare.bDT_Pulse = TRUE;
		g_HardWare_Pare.bENADC_Pulse = TRUE;
		g_HardWare_Pare.bREADY_Pulse = TRUE;
		g_HardWare_Pare.bReadorWriteOperate = TRUE;
		g_HardWare_Pare.bSampleModeRealTime = TRUE;
		g_HardWare_Pare.bDataPolarity_Pulse = TRUE;
		g_HardWare_Pare.fEngerScaleOffset = 0;
		g_HardWare_Pare.fEngerScaleSlope = 0;
		m_fMenuEnergeScale_K = g_HardWare_Pare.fEngerScaleSlope;
		m_fMenuEnergeScale_b = g_HardWare_Pare.fEngerScaleOffset;
		
		g_HardWare_Pare.iChannelWidth = 8192;
		g_HardWare_Pare.iEngerScaleUnit = 0;
		g_HardWare_Pare.iSampleTimeValue = 80000;
		g_HardWare_Pare.bInputAnolSignalSel = TRUE;

		g_HardWare_Pare.iEth_Up_UsbADC = 16383;
		g_HardWare_Pare.iEth_Down_UsbADC = 8200;

		g_HardWare_Pare.iTime_Up_ETH = 200;
		g_HardWare_Pare.iTime_Down_ETH = 8;

		m_bMenuEnergeScale_CalcuEn = FALSE;
		m_bDrawLogData_Sel = TRUE;

		g_ComPortSel = COM1_PORT;

		m_Full_BackGround_Color = RGB(128,128,128);
		m_Exp_BackGround_Color = RGB(115,138,155);
		m_Interest_Color = RGB(166,166,255);
		m_SmallWin_BackGround_Color = RGB(115,138,155);

    fseek(fp,0,SEEK_SET);
	fclose(fp);
  }

/////////////////////////////////////////////////////////////////////////////////
  else
  {
	fread(&g_ComPortSel,sizeof(DWORD),1,fp);
	fread(&g_HardWare_Pare.bANSER_Pulse,1,1,fp);
	fread(&g_HardWare_Pare.bDT_Pulse, 1,1,fp);
	fread(&g_HardWare_Pare.bENADC_Pulse,1,1,fp);
	fread(&g_HardWare_Pare.bREADY_Pulse,1,1,fp);
	fread(&g_HardWare_Pare.bReadorWriteOperate,1,1,fp);
	fread(&g_HardWare_Pare.bSampleModeRealTime,1,1,fp);
	fread(& g_HardWare_Pare.bDataPolarity_Pulse,1,1,fp);
	fread(&g_HardWare_Pare.fEngerScaleOffset,4,1,fp);
	fread(&g_HardWare_Pare.fEngerScaleSlope,4,1,fp);	
	fread(&g_HardWare_Pare.iChannelWidth,4,1,fp);
	fread(&g_HardWare_Pare.iEngerScaleUnit,4,1,fp);
	fread(&g_HardWare_Pare.iSampleTimeValue,4,1,fp);
//////////////////////////////////////////////////////////////////////////////color
	fread(&m_Full_BackGround_Color,sizeof(COLORREF),1,fp);			
	fread(&m_Exp_BackGround_Color,sizeof(COLORREF),1,fp);
	fread(&m_Interest_Color,sizeof(COLORREF),1,fp);
	fread(&m_SmallWin_BackGround_Color,sizeof(COLORREF),1,fp);
////////////////////////////////////////////////////////////////////////////////usb
	fread(&g_HardWare_Pare.iEth_Up_UsbADC,4,1,fp);
	fread(&g_HardWare_Pare.iEth_Down_UsbADC,4,1,fp);
	fread(&g_HardWare_Pare.iTime_Up_ETH,4,1,fp);
	fread(&g_HardWare_Pare.iTime_Down_ETH,4,1,fp);
	fread(&g_HardWare_Pare.bInputAnolSignalSel,1,1,fp);
	fclose(fp);
  }
////////////////////////////////////////////////////////////////////////////////
	//��ʼ�������� ����ͳ�ƶԻ���E
	pMCB_TO_BUF->ShowWindow(TRUE);
	char chrbuff[100];
	sprintf(chrbuff,"%d",g_HardWare_Pare.iSampleTimeValue);
	pSampleTimeValue->SetWindowText(chrbuff);

	m_iFullChannelWidth = g_HardWare_Pare.iChannelWidth; 
	////////////////////////////////////////////////changing
	pComboxHor->ResetContent();
	pComboxHor->AddString("64");
	pComboxHor->AddString("128");
	pComboxHor->AddString("256");
	pComboxHor->AddString("512");
    pComboxHor->AddString("1024");
	pComboxHor->AddString("2048");
	pComboxHor->AddString("4096");
	pComboxHor->AddString("8192");
    pComboxHor->SetCurSel(m_iSmallWindowWidthCheckSel);

	pComboxExpVer->ResetContent();
	pComboxExpVer->AddString("8192k");
	pComboxExpVer->AddString("4096k");
	pComboxExpVer->AddString("2048k");
	pComboxExpVer->AddString("1024k");
    pComboxExpVer->AddString("512k");
	pComboxExpVer->AddString("256k");
	pComboxExpVer->AddString("128k");
	pComboxExpVer->AddString("64k");
    pComboxExpVer->AddString("32k");
	pComboxExpVer->AddString("16k");
	pComboxExpVer->AddString("8k");
	pComboxExpVer->AddString("4k");
	pComboxExpVer->AddString("2k");
	pComboxExpVer->AddString("1024");
    pComboxExpVer->AddString("512");
	pComboxExpVer->AddString("256");
	pComboxExpVer->AddString("128");
	pComboxExpVer->AddString("64");

    pComboxExpVer->SetCurSel(m_iBuffExpRamCheckSel);
	pComboxVer->SetCurSel(m_iBuffComboxSel);
OnFullramViewsel();
OnExpramViewsel();
/*
	if(m_bDrawLogData_Sel == TRUE){
		pComboxVer->ResetContent();
		pComboxVer->AddString("Log");
		pComboxVer->SetCurSel(0);
	}
	else{
		pComboxVer->SetCurSel(13);
	}
*/
	//USB�˿ڵĲ�������
	if(g_ComPortSel == USB_PORT) { 
		g_hUsb = OpenUsbDevice();
		if (g_hUsb == INVALID_HANDLE_VALUE) {
			//AfxMessageBox("Usb�˿ڴ򿪴���\r\nǁE���Ӳ���豸�Ƿ�ӵ磬USB��·�Ƿ����á�");
			pStatus->SetPaneText(2,"ͨ�Ŷ˿�:  USB�˿ڴ򿪴��E",1);
		}
		else {
			pStatus->SetPaneText(2,"ͨ�Ŷ˿�:  USB�˿ڴ򿪳ɹ� ",1);
			CloseUsbDevice(g_hUsb);
			g_hUsb = INVALID_HANDLE_VALUE;
		}		
		pQueryTimeout->SetWindowText("USB�˿�");
		pQuerySampleState->SetWindowText("ֹͣ�ɼ�����");

		return;
	}

	pQueryTimeout->SetWindowText("��ѯ��ʱ: STOP");
	pQuerySampleState->SetWindowText("���ݻ�ȡ: STOP");

	    CComboBox *pComboHor=(CComboBox *)(pMainFrame->m_wndDlgBar.GetDlgItem(IDC_COMBO_HOR));
    CComboBox *pComboVer=(CComboBox *)(pMainFrame->m_wndDlgBar.GetDlgItem(IDC_COMBO_VER));
    CComboBox *pComboExpHor=(CComboBox *)(pMainFrame->m_wndDlgBar.GetDlgItem(IDC_COMBO_EXP_HOR));
    CComboBox *pComboExpVer=(CComboBox *)(pMainFrame->m_wndDlgBar.GetDlgItem(IDC_COMBO_EXP_VER));

   pComboHor->SetFont(&font);
   pComboVer->SetFont(&font);
   pComboExpHor->SetFont(&font);
   pComboExpVer->SetFont(&font);

	//����һ�������ʹ򿪴���
		g_hComm = OpenShmMaint232(g_ComPortSel,g_CommBaud);
		if(g_hComm == INVALID_HANDLE_VALUE) {
			sprintf(chrbuff,"����Ĭ�ϵĴ���%d �򿪴��E\r\n1.ǁEر��Ѿ��򿪸ô��ڵ�����\r\n2.�������ñ�������Ĭ�ϴ��ڡ�",g_ComPortSel);
            CString s;
			s.Format("%s","���ڴ򿪴��E 1.ǁEر��Ѿ��򿪸ô��ڵ����� 2.�������ñ�������Ĭ�ϴ���.");
			TextScroll(s);
			g_bComOpen = FALSE;

			OnExpramViewsel();
			OnFullramViewsel();
            ResumeR();
			return ;
		}
		
		g_hEvtFinishRec = CreateEvent(NULL, TRUE, FALSE, NULL);
		
		g_bExec = FALSE;
		
		g_hEvtSend = CreateEvent(NULL, TRUE, FALSE, NULL);
		g_hExitSendThread = CreateEvent(NULL,TRUE,FALSE,NULL);
		g_hExitMonitThread = CreateEvent(NULL,TRUE,FALSE,NULL);
		
		g_pThread_Monitor = AfxBeginThread(MonitorThread,
			(LPVOID)this,
			THREAD_PRIORITY_BELOW_NORMAL,
			0,
			CREATE_SUSPENDED);						
		
		g_pThread_Monitor->m_bAutoDelete = FALSE;
		g_pThread_Monitor->ResumeThread();
		
		
		g_pThread_Send = AfxBeginThread(SendThread,
			(LPVOID)this,
			THREAD_PRIORITY_BELOW_NORMAL,
			0,
			CREATE_SUSPENDED);						
		
		g_pThread_Send->m_bAutoDelete = FALSE;
		g_pThread_Send->ResumeThread();
		
		if (g_ComPortSel == COM1_PORT) {
			pStatus->SetPaneText(2,"ͨ�Ŷ˿�:  ����1 �򿪳ɹ�",1);
		}
		if (g_ComPortSel == COM2_PORT) {
			pStatus->SetPaneText(2,"ͨ�Ŷ˿�:  ����2 �򿪳ɹ�",1);
		}

		g_bComOpen = TRUE;

        ResumeR();
        OnFullramViewsel();
		OnExpramViewsel();

//	CMainFrame* pMainFrame = (CMainFrame*)GetParentFrame();
//    m_ScrollBar=(CScrollBar *)(pMainFrame->m_wndDlgBar.GetDlgItem(IDC_SCROLLBAR1));
//	m_ScrollBar->SetScrollRange(0,100);

}

LRESULT CMCBView:: OnSendShow(WPARAM wParam, LPARAM lParam)
{
	unsigned char* ptext = (unsigned char*)wParam;

	char outtext[100];
	sprintf(outtext, "Send==> ");
	int kkk = (int)lParam;
	if(kkk > 20) kkk = 20;
	for(int j=0; j<kkk; j++) {
		 sprintf(outtext, "%s%2.2x ",outtext,ptext[j]);
	}

	if((int)lParam >= 20) sprintf(outtext, "%s%s ",outtext,"......");

	pStatus->SetPaneText(2,outtext,1);
	return 0L;
}

LRESULT CMCBView:: OnRecShow(WPARAM wParam, LPARAM lParam)
{	
	unsigned char* ptext = (unsigned char*)wParam;
	//�����ְ�E
	memcpy(m_precbuf,ptext,lParam);
	m_iworkcounter += (int)lParam;

	if(m_iworkcounter < g_RecPackageLenth)
	{	
		m_precbuf += (int)lParam;     //workcounter
		return 0L;									
	} 

	if(m_bExpRamLongNew_valid)
	{ //����ո�E���ExpRam�ı߽磬����ʾ��ǰ����
		PulseEvent(g_hEvtFinishRec);  //akangquan 
		return 0;
	}


	memcpy(m_secrecbuf,m_recbuff,m_iworkcounter);	

	char outtext[100];
	sprintf(outtext, "REC<== (%d)",m_iworkcounter);
	int lenth = m_iworkcounter;
	if(lenth > 18) lenth = 18;
	for(int j=0; j<lenth; j++) 
	{
		 sprintf(outtext, "%s%2.2x ",outtext,m_secrecbuf[j]);
	}
	if(m_iworkcounter > 18) sprintf(outtext, "%s%s ",outtext,"......");
	pStatus->SetPaneText(2,outtext,1);


	m_iworkcounter = 0;
	m_precbuf = m_recbuff;

//	PulseEvent(g_hEvtFinishRec);  //�����E��ݺ��ٷ��� ������́E�������
	OnUnpack(m_secrecbuf,g_RecPackageLenth);

	PulseEvent(g_hEvtFinishRec);  //akangquan 
    
  /*CMainFrame *p=(CMainFrame *)GetParentFrame();
    if (m_continueflag)
    {
		CButton *pButton=(CButton *)(p->m_wndDlgBar.GetDlgItem(IDC_FULLRAM_VIEWSEL));
		pButton->SetCheck(TRUE);
        pButton=(CButton *)(p->m_wndDlgBar.GetDlgItem(IDC_EXPRAM_VIEWSEL));
		pButton->SetCheck(FALSE);
		
      CIni inifile;
		int kk;
    	if(inifile.Open("MCBini_05_1028.ini"))
		{ 	
			kk=inifile.ReadInt("MCB HardWare", "iChannelWidth");

			inifile.Close();
		}
	    m_iFullChannelWidth=kk;
 	    CComboBox *pComboxHor=(CComboBox *)(p->m_wndDlgBar.GetDlgItem(IDC_COMBO_HOR));
		pComboxHor->ResetContent();
		pComboxHor->AddString("1024");
		pComboxHor->AddString("2048");
		pComboxHor->AddString("4096");
		pComboxHor->AddString("8192");

		switch(m_iFullChannelWidth) /////////////////////////////////////////changing
		{
			case 1024: pComboxHor->SetCurSel(0); break;
			case 2048: pComboxHor->SetCurSel(1); break;
			case 4096: pComboxHor->SetCurSel(2); break;
			case 8192: pComboxHor->SetCurSel(3); break;
			default: break;
		}
		m_continueflag=FALSE;
    }*/

	if (m_bEnReadFPGATimeValue) {
		Sleep(100);
		m_bEnReadFPGATimeValue = false;
		OnMenuLoadTimerCount(); //�ϴ�ʱ��ֵ
	}

	return 0L;
}

//����󴰿ڵı���ɫ
void CMCBView::OnDrawBackgroud(CDC * pDC)
{
	CBrush bShow;			
	if (m_bPrintMapEn) {//pDC->IsPrinting()
		bShow.CreateSolidBrush(RGB(255,255,255));//  (RGB(27,146,180));�󴰿�
	}
	else{
		bShow.CreateSolidBrush(m_Exp_BackGround_Color);//RGB(45,45,45) �󴰿� ����	
	}
	
	CBrush* pOldBrush=pDC->SelectObject(&bShow);
	pDC->Rectangle(0,m_iViewWindowSize_Y*95/500+1,m_iViewWindowSize_X,m_iViewWindowSize_Y*420/500);// 
	pDC->SelectObject(pOldBrush);//++++
	bShow.DeleteObject();
		
	CPen pShow1(PS_SOLID,3,RGB(200,150,150));//�ֽ���
	CPen* pOldPen1=NULL;
	pOldPen1=pDC->SelectObject(&pShow1);
	pDC->MoveTo(0,m_iViewWindowSize_Y*420/500);
	pDC->LineTo(m_iViewWindowSize_X,m_iViewWindowSize_Y*420/500);

}

void CMCBView::OnDrawSmallWin(CDC* pDC,int centerpoint,int m_iSmallWinCHL, int m_iSmallWinCHR)
{
	CPen pShow0(PS_SOLID,1,RGB(80,80,80));//RGB(72,183,152)
	pDC->SelectObject(&pShow0);	
	CBrush bShow;	
	
	//����ȫ�׵ı���
	if (m_bPrintMapEn) 
	{//pDC->IsPrinting()
		bShow.CreateSolidBrush(RGB(255,255,255));//  ����ȫ�׵ı���
	}
	else 
	{
		bShow.CreateSolidBrush(m_Full_BackGround_Color);//RGB(100,100,100) ����ȫ�׵ı���
	}				 					  
	CBrush* pOldBrush=pDC->SelectObject(&bShow);

	pDC->Rectangle(0,0,m_iViewWindowSize_X,m_iViewWindowSize_Y*95/500);// m_iViewWindowSize_X
	pDC->SelectObject(pOldBrush);//++++
	bShow.DeleteObject();

	CPen pShowsmall(PS_SOLID,1,RGB(5,5,5));//
	CPen pShowsmallPrint(PS_SOLID,1,RGB(212,212,212));//
	pDC->SelectObject(&pShowsmall);
	
	if (m_bPrintMapEn) 
	{//pDC->IsPrinting()
		bShow.CreateSolidBrush(RGB(255,255,255));//  print 
	}
	else 
	{
		bShow.CreateSolidBrush(m_SmallWin_BackGround_Color);//С���ڱ��� RGB(19,142,124)
	}	

	//��С���ڱ���

	pOldBrush=pDC->SelectObject(&bShow);
	if (m_bMCBModeSel)
	{
			pDC->Rectangle(m_iSmallWinCHL * m_iViewWindowSize_X/((int)m_iFullChannelWidth),0+1,m_iSmallWinCHR * m_iViewWindowSize_X/m_iFullChannelWidth,m_iViewWindowSize_Y*95/500);// centerpoint-100,548,centerpoint+100,900
	}
	else
	{
		pDC->Rectangle((int)(m_iBuffSmallWinCHL)* m_iViewWindowSize_X/((int)m_iBuffFullChannelWidth),0+1,(int)m_iBuffSmallWinCHR * m_iViewWindowSize_X/(int)m_iBuffFullChannelWidth,m_iViewWindowSize_Y*95/500);// centerpoint-100,548,centerpoint+100,900
	}
	pDC->SelectObject(pOldBrush);//++++
	bShow.DeleteObject();
		
	CPen pShow11(PS_SOLID,3,RGB(200,150,150));//�ֽ���
	pDC->SelectObject(&pShow11);
	pDC->MoveTo(0,m_iViewWindowSize_Y*95/500);
	pDC->LineTo(m_iViewWindowSize_X,m_iViewWindowSize_Y*95/500);

}

void CMCBView::OnPrintSmallWin(CDC* pDC,int centerpoint,int m_iSmallWinCHL, int m_iSmallWinCHR)////////////////changing
{
    
    if(m_smallwin)
	{
	
	CPen pShow0(PS_SOLID,1,RGB(80,80,80));
	pDC->SelectObject(&pShow0);	
	CBrush bShow;	
	bShow.CreateSolidBrush(RGB(255,255,255));//  ����ȫ�׵ı���				  
	CBrush* pOldBrush=pDC->SelectObject(&bShow);

	pDC->Rectangle(0,0,m_iViewWindowSize_X,m_iViewWindowSize_Y*95/500);// m_iViewWindowSize_X
	pDC->SelectObject(pOldBrush);//++++
	bShow.DeleteObject();

	CPen pShowsmall(PS_SOLID,1,RGB(5,5,5));//
	CPen pShowsmallPrint(PS_SOLID,1,RGB(212,212,212));//
	pDC->SelectObject(&pShowsmall);

	//��С���ڱ���
/*    if(m_smallwin)
	{
		pOldBrush=pDC->SelectObject(&bShow);
		pDC->Rectangle(m_iSmallWinCHL * m_iViewWindowSize_X/m_iFullChannelWidth,0+1,m_iSmallWinCHR * m_iViewWindowSize_X/m_iFullChannelWidth,m_iViewWindowSize_Y*95/500 - 2);// centerpoint-100,548,centerpoint+100,900
		pDC->SelectObject(pOldBrush);//++++
		bShow.DeleteObject();
	
//		CPen pShow11(PS_SOLID,2,RGB(200,150,150));//�ֽ���
//		pDC->SelectObject(&pShow11);
//		pDC->MoveTo(0,m_iViewWindowSize_Y*95/500);
//		pDC->LineTo(m_iViewWindowSize_X,m_iViewWindowSize_Y*95/500);
	}*/
    /////////////////////////////////////////////////////////////fulldraw
    float maxvalue;
	maxvalue = OnCaculateFullRamEthValue();
	if((maxvalue > -0.001) && (maxvalue < 0.001)) 
	{
		maxvalue = m_iViewWindowSize_Y*95/500;
	}
	
	int go2one[1024];
	if (m_bDrawLogData_Sel) {
		for(int i=0;i<512;i++) {
			go2one[i] = m_iViewWindowSize_Y*95/500 - ((m_iViewWindowSize_Y*95/500) )*((float)(m_farrFullRam[i])/(maxvalue));
			if(go2one[i] >= m_iViewWindowSize_Y*94/500) go2one[i] = m_iViewWindowSize_Y*94/500;
			if(go2one[i] < m_iViewWindowSize_Y*5/500) go2one[i] = m_iViewWindowSize_Y*5/500; 
		}
	}
	else{
		for(int i=0;i<512;i++) {
			go2one[i] = m_iViewWindowSize_Y*95/500 - ((m_iViewWindowSize_Y*95/500) )*((float)(m_arrFullRam[i] & 0x7fffffff)/maxvalue);
			if(go2one[i] >= m_iViewWindowSize_Y*94/500) go2one[i] = m_iViewWindowSize_Y*94/500;
			if(go2one[i] < m_iViewWindowSize_Y*5/500) go2one[i] = m_iViewWindowSize_Y*5/500; 
		}
	}

	pDC->MoveTo(0,m_iViewWindowSize_Y*95/500-1);
	pDC->LineTo(0,m_iViewWindowSize_Y*95/500-1);

	CPen pShow2(PS_SOLID,1,RGB(255,255,255));//RGB(128,0,0)FullRam������(����ɫ)
	CPen pShow3(PS_SOLID,1,m_Interest_Color);//FullRam������(����Ȥɫ)
	CPen pShowPrint(PS_SOLID,1,RGB(0,0,0));//FullRam������(��ӡ��ɫ)
	
	CPen* pOldPen=NULL;
	CPen* pOldPenOther=NULL;

	float step_x,high_y;

	step_x = (float)m_iViewWindowSize_X / 512.0;
	high_y = (float)m_iViewWindowSize_Y*95/500.0;
	pDC->MoveTo(0,go2one[0]-2); //m_iViewWindowSize_Y*95/500
	for(int i=0;i<511;i++) {
		if(m_arrFullRam[i] >= 0x80000000 ) {
			if (m_bPrintMapEn) {//pDC->IsPrinting()
				pOldPen=pDC->SelectObject(&pShowPrint);
			}
			else{
				pOldPen=pDC->SelectObject(&pShow3);
			}			

			//Ϊ�������򴰿ڴ�С���������ֵÿ����ƣ��໭һ�����ߣ�
			pDC->MoveTo(i*step_x,high_y - 2);
			pDC->LineTo(i*step_x,go2one[i] - 2);
			pDC->MoveTo(i*step_x + 1,high_y - 2);
			pDC->LineTo(i*step_x + 1,go2one[i] - 2);
		}
		else {			
			if (m_bPrintMapEn) {
				pOldPen=pDC->SelectObject(&pShowPrint);
			}
			else{
				pOldPen=pDC->SelectObject(&pShow2);
			}
			if(m_arrFullRam[i+1] < 0x80000000 ) {
				pDC->MoveTo(i*step_x,go2one[i]-2);
				pDC->LineTo((i+1)*step_x,go2one[i+1]-2);
			}
			else {
				pDC->MoveTo(i*step_x,go2one[i]-2);
				pDC->LineTo((i)*step_x,go2one[i]-2);
			}
			
		}
	}
    
	}
}

BOOL CMCBView::OnPrintExpRamLine(CDC *pDC)///////////////////////changing
{
	CMainFrame* pMainFrame= (CMainFrame*)GetParentFrame();
	CComboBox* pComboxVer = (CComboBox *) (pMainFrame->m_wndDlgBar.GetDlgItem(IDC_COMBO_VER));
	
	float maxvalue = 1;

	int go2one[8193];

    

	COLORREF color1 =RGB(0,128,128);
	if(m_rangemode==1)
	{
		maxvalue =m_range;
	}

 

	if(m_rangemode==2)
	{
		maxvalue=m_selfrange;
	}

   if (m_bDrawLogData_Sel)
    {
        OnprintCaculateLog();
		maxvalue=8;
    }
    
////////////////////////////////////////////////////////////////////changing
   CRect rect;
   GetClientRect(&rect);
   if (m_bDrawLogData_Sel) 
   {
		for(int i=0;i<m_SmallWinWidth;i++) 
		{   
			int j=0;
            j = (float)m_iViewWindowSize_Y*418.0/500-(float)m_iViewWindowSize_Y*418.0/500*(m_farrExpRamprint[i+m_SmallWinCHL]/(float)(maxvalue));
		 //j = (float)(rect.Height())-(float)(rect.Height())*(m_farrExpRam[i]/(float)(maxvalue));	
			go2one[i]=j;
			if(j >= (float)m_iViewWindowSize_Y*418.0/500)
			{
				go2one[i] = m_iViewWindowSize_Y*418.0/500;
			}

		}
	}
	else
	{
		for(int i=0;i<m_SmallWinWidth;i++)
		{
			int k=0;
            k =(float)m_iViewWindowSize_Y*418.0/500-m_iViewWindowSize_Y*418/500*((float)(m_arrExternalRam[i+m_SmallWinCHL]& 0x7fffffff)/maxvalue);
		//	k =(float)(rect.Height())-(rect.Height())*((float)(m_arrExpRam[i]& 0x7fffffff)/maxvalue);
			go2one[i]=k;
			if(k >=(m_iViewWindowSize_Y*418.0/500))
			{
				go2one[i] = m_iViewWindowSize_Y*418.0/500;
			}

		}
	}
    
	CPen pShow2(PS_SOLID,1,RGB(255,0,128));
	CPen pShow3(PS_SOLID,2,m_Interest_Color);

	CBrush bShow;
	bShow.CreateSolidBrush(m_Interest_Color);
	BOOL bcounter = false;
	int intcounter=0;
	float intertdotbuff;
    float  high_y;
	high_y = (float)m_iViewWindowSize_Y*418/500.0;

	CPen* pOldPen=NULL;
	pOldPen=pDC->SelectObject(&pShow2);

    pDC->MoveTo(0,m_iViewWindowSize_Y*418/500);
	for(int i=0;i<m_SmallWinWidth;i++)
	{
		/*if((m_arrExternalRam[i] >= 0x80000000))
		{
			pDC->SelectObject(&pShow3);
			if (m_bDrawLine_Sel) 
			{  
				pDC->LineTo(i*m_iViewWindowSize_X/m_SmallWinWidth,go2one[i]);
			}
			else 
			{ 
				if(m_bPrintMapEn == TRUE)
				{
					pDC->SetPixel(i*m_iViewWindowSize_X/m_SmallWinWidth,go2one[i] + 1,RGB(255,0,0));
					pDC->SetPixel(i*m_iViewWindowSize_X/m_SmallWinWidth,go2one[i],RGB(255,0,0));
				}
			}			
			pDC->SelectObject(&bShow); 
		}
		else*/
		if(go2one[i]!=0)
		{
			pOldPen=pDC->SelectObject(&pShow2);			
			if (m_bDrawLine_Sel) 
			{  				
				pDC->LineTo((i*m_iViewWindowSize_X/m_SmallWinWidth),go2one[i]);				
			}
			else 
			{ 				
				if(m_bPrintMapEn == TRUE)
				{

					if (go2one[i]< m_iViewWindowSize_Y*418/500)
					{				
					   pDC->SetPixel((i*m_iViewWindowSize_X/m_SmallWinWidth),go2one[i] + 1,color1);
					   pDC->SetPixel((i*m_iViewWindowSize_X/m_SmallWinWidth),go2one[i],color1);
					}					
				}			
			}						
		}
	}
	return TRUE;
  
}

void CMCBView::OnPrintBackgroud(CDC * pDC)
{
    CBrush bShow;			
    CPen pen(PS_SOLID,1,RGB(255,255,255));
	bShow.CreateSolidBrush(RGB(255,255,255));//  (RGB(27,146,180));�󴰿�
	CBrush* pOldBrush=pDC->SelectObject(&bShow);
	pDC->SelectObject(&pen);
	pDC->Rectangle(0,0,m_iViewWindowSize_X,m_iViewWindowSize_Y*420/500);// 
	pDC->SelectObject(pOldBrush);//++++
	bShow.DeleteObject();
	pen.DeleteObject();
		
/*	CPen pShow1(PS_SOLID,3,RGB(200,150,150));//�ֽ���//////////////////////changing
	CPen* pOldPen1=NULL;
	pOldPen1=pDC->SelectObject(&pShow1);
	pDC->MoveTo(0,m_iViewWindowSize_Y*420/500);
	pDC->LineTo(m_iViewWindowSize_X,m_iViewWindowSize_Y*420/500);
*/
}

void CMCBView::OnMouseMove(UINT nFlags, CPoint point) 
{		

////////////////////////////////////////////////////////////////changing
/*	CDC* pDC=GetDC();
	if(m_bSmallWinPointMoveEN) {			
		    CPen pShow2(PS_SOLID,1,RGB(255,10,0));//RGB(0,210,0)
			CPen* pOldPen=NULL;
			pOldPen=pDC->SelectObject(&pShow2);

			pDC->SetROP2(R2_NOT);

			pDC->MoveTo(m_Oldpoint.x, 0);
			pDC->LineTo(m_Oldpoint.x, m_iViewWindowSize_Y*95/500);//93 
			pDC->MoveTo(point.x, 0);
			pDC->LineTo(point.x, m_iViewWindowSize_Y*95/500); 
			m_Oldpoint = point;
//			strbuf.Format("��ַ: %d (����) ����:  **** ",m_Oldpoint.x * g_HardWare_Pare.iChannelWidth/m_iViewWindowSize_X);

			m_strStateLine1Text = strbuf;
			OnShowStateText(pDC,1,strbuf);			
	}

	ReleaseDC(pDC);	*/
//////////////////////////////////////////////////////////////////////////
	//���Ӳ��Դ�Eaxwell
	CMainFrame* pMainFrame = (CMainFrame*)GetParentFrame();

//	CString strbuf;

   if(m_bSmallWinPointMoveEN) 
   {
//		m_bSmallWinPointMoveEN = false;
     if (m_bMCBModeSel)
     {
		OnFullramViewsel();
		
		m_iSmallWinCenPoint = point.x*m_iFullChannelWidth/m_iViewWindowSize_X;
		m_iSmallWinCHL = m_iSmallWinCenPoint - m_iSamllWinWidth/2 ;
		m_iSmallWinCHR = m_iSmallWinCenPoint + m_iSamllWinWidth/2 ;			
		if(point.x * m_iFullChannelWidth/m_iViewWindowSize_X <= (m_iSamllWinWidth/2)) {
			m_iSmallWinCHL = 0;	
		};
		if(point.x * m_iFullChannelWidth/m_iViewWindowSize_X >= m_iFullChannelWidth-1 - m_iSamllWinWidth/2) {
			m_iSmallWinCHR = m_iFullChannelWidth-1; 
		};
//		strbuf.Format("��ַ:  %d (����)     ����: %d        ������߽�E%d  �����ұ߽�E%d     ����߶�:%d",
//			m_iSmallWinCenPoint,(m_arrExpRam[(m_iSamllWinWidth/2)] & 0x7fffffff), 
//			m_iSmallWinCHL,m_iSmallWinCHR,m_iExpRamEthValue);
//		m_strStateLine1Text = strbuf;		
		m_ExpRamDataLong_new = (m_iSmallWinCHR - m_iSmallWinCHL +1)*3;
      ReDrawExpLine();
	}
   else
   {
		OnFullramViewsel();
		m_iBuffSmallWinCenPoint= point.x*m_iBuffFullChannelWidth/m_iViewWindowSize_X;
		m_iBuffSmallWinCHL = m_iBuffSmallWinCenPoint - m_iBuffSmallWindowWidth/2 ;
		m_iBuffSmallWinCHR = m_iBuffSmallWinCenPoint + m_iBuffSmallWindowWidth/2 ;			
		if(point.x * m_iBuffFullChannelWidth/m_iViewWindowSize_X <= (m_iBuffSmallWindowWidth/2)) 
		{
			m_iBuffSmallWinCHL = 0;	
		};
		if(point.x * m_iBuffFullChannelWidth/m_iViewWindowSize_X >= m_iBuffFullChannelWidth-1 - m_iBuffSmallWindowWidth/2) 
		{
			m_iBuffSmallWinCHR = m_iBuffFullChannelWidth-1; 
		};	
        ReDrawExpLine();      
   }
  }
  
//	m_ScrollBar.SetScrollPos(m_pos);
//////////////////////////////////////////////////////////////////////////
	//end of maxwell
	CView::OnMouseMove(nFlags, point);
}//

void CMCBView::OnLButtonDown(UINT nFlags, CPoint point) 
{

		
	//	m_ScrollBar.ShowScrollBar(TRUE);		
	//----------��Full �׸���Ȥ ƽ�ơ�--------//		
//	if((point.x*1024/m_iViewWindowSize_X < m_iSmallWinCHR)&&(point.x *1024/m_iViewWindowSize_X > m_iSmallWinCHL)) {	
		if((point.y >0)&&(point.y<m_iViewWindowSize_Y*90/500)) {	
			m_bSmallWinPointMoveEN = true;
		}		
//	}

		// Expend ��չ������ƽ��
	if ((point.y > m_iViewWindowSize_Y*90/500 ) && (point.y < m_iViewWindowSize_Y*420/500)) 
	{
		if(FALSE == OnHandChangeSmallWin(point.x,point.y) ) 
		{
//			Redraw();
//			ReDrawExpLine();
			return;
		}
		OnExpramViewsel();
	}
//    m_ScrollBar.ShowScrollBar(FALSE);
//	m_ScrollBar.ShowScrollBar(TRUE);
	
	CView::OnLButtonDown(nFlags, point);
}

void CMCBView::OnLButtonUp(UINT nFlags, CPoint point) 
{	
	
	m_ScrollBar.ShowScrollBar(FALSE);
	m_ScrollBar.ShowScrollBar(TRUE);
	
	CMainFrame* pMainFrame = (CMainFrame*)GetParentFrame();
	CString strbuf;
	if(m_bSmallWinPointMoveEN) 
	{
	 if (m_bMCBModeSel)
	 {
		m_bSmallWinPointMoveEN = false;

		OnFullramViewsel();
		
		m_iSmallWinCenPoint = point.x*m_iFullChannelWidth/m_iViewWindowSize_X;
		m_iSmallWinCHL = m_iSmallWinCenPoint - m_iSamllWinWidth/2 ;
		m_iSmallWinCHR = m_iSmallWinCenPoint + m_iSamllWinWidth/2 ;			
		if(point.x * m_iFullChannelWidth/m_iViewWindowSize_X <= (m_iSamllWinWidth/2)) {
			m_iSmallWinCHL = 0;	
		};
		if(point.x * m_iFullChannelWidth/m_iViewWindowSize_X >= m_iFullChannelWidth-1 - m_iSamllWinWidth/2) {
			m_iSmallWinCHR = m_iFullChannelWidth-1; 
		};
//		strbuf.Format("��ַ:  %d (����)     ����: %d        ������߽�E%d  �����ұ߽�E%d     ����߶�:%d",
//			m_iSmallWinCenPoint,(m_arrExpRam[(m_iSamllWinWidth/2)] & 0x7fffffff), 
//			m_iSmallWinCHL,m_iSmallWinCHR,m_iExpRamEthValue);

		m_strStateLine1Text = strbuf;		
		m_bExpRamLongNew_valid = true;
		m_ExpRamDataLong_new = (m_iSmallWinCHR - m_iSmallWinCHL +1)*3;

		//��ǰϵͳ���� MCB��ʽ���������ڲɼ�����ʱ����ʱ�轫FullRam������ת����ֵ�� ExpRam
		if ((g_ComPortSel == COM1_PORT)||(g_ComPortSel == COM2_PORT)) 
		{
			OnFullRam2ExpRamInsertVal();
		}				
	    Redraw();

	 }
	 else
	 {
		m_bSmallWinPointMoveEN = false;
		OnFullramViewsel();

		m_iBuffSmallWinCenPoint = point.x*m_iBuffFullChannelWidth/m_iViewWindowSize_X;
		m_iBuffSmallWinCHL = m_iBuffSmallWinCenPoint - m_iBuffSmallWindowWidth/2 ;
		m_iBuffSmallWinCHR = m_iBuffSmallWinCenPoint + m_iBuffSmallWindowWidth/2 ;			
		if(point.x * m_iBuffFullChannelWidth/m_iViewWindowSize_X <= (m_iBuffSmallWindowWidth/2)) 
		{
			m_iBuffSmallWinCHL = 0;	
		};
		if(point.x * m_iBuffFullChannelWidth/m_iViewWindowSize_X >= m_iBuffFullChannelWidth-1 - m_iBuffSmallWindowWidth/2) 
		{
			m_iBuffSmallWinCHR = m_iBuffFullChannelWidth-1; 
		};		
		m_bExpRamLongNew_valid = true;
		m_ExpRamDataLong_new = (m_iBuffSmallWinCHR - m_iBuffSmallWinCHL +1)*3;
	    Redraw();

	 }
	}


	CView::OnLButtonUp(nFlags, point);
}

void CMCBView::Redraw()
{	
	if((!m_bMCBModeSel)||(!g_MCB_Oper.bQueryEn)) 
	{
		int kk =0;
		
		if (m_bMCBModeSel) kk= 2 * m_iFullChannelWidth/1024;
        else kk= 2*m_iBuffFullChannelWidth/1024;
		
		UINT max = 0;
		for(int i=0;i<512;i++) 
		{
			max = 0;
			for(int j=i*kk;j<(i+1)*kk;j++) 
			{ //ѡ��kk ���ȷ�Χ�������һ������ֵ
				if (m_arrExternalRam[j] > max)
				{
					max = m_arrExternalRam[j];
				}
			}
			m_arrFullRam[i] = max;
		}
		OnExternal2ExpRam();
	}

	CDC* pDC=GetDC();
	
	CDC MemDC; //���ȶ���һ����ʾ�豸��ρE
	CBitmap MemBitmap;//����һ��λͼ��ρE
	MemDC.CreateCompatibleDC(NULL);
	MemBitmap.CreateCompatibleBitmap(pDC,1600,1200);
	CBitmap *pOldBit=MemDC.SelectObject(&MemBitmap);	
	MemDC.FillSolidRect(0,0,1600,1200,RGB(219,219,219));

	OnDrawBackgroud(&MemDC);
	OnDrawSmallWin(&MemDC,m_iSmallWinCenPoint,m_iSmallWinCHL,m_iSmallWinCHR);
	OnDrawExpRamLine(&MemDC);	//����ͼ
	OnDrawFullRamLine(&MemDC);	//ȫͼ
	
	OnDrawTextBgroud(&MemDC);
//	OnShowStateText(&MemDC,1,m_strStateLine1Text);
//	OnShowStateText(&MemDC,2,m_strStateLine2Text);
//	OnShowStateText(&MemDC,3,m_strStateLine3Text);

	pDC->BitBlt(0,0,1600,1200,&MemDC,0,0,SRCCOPY);	
	
	MemBitmap.DeleteObject();
	MemDC.DeleteDC();
	ReleaseDC(pDC);	
//	m_ScrollBar.ShowScrollBar(FALSE);
//	m_ScrollBar.ShowScrollBar(TRUE);

}

///////////////////////////////////////////////////////////////////////////
void CMCBView::ReDrawExpLine()
{
	if((!m_bMCBModeSel)||(!g_MCB_Oper.bQueryEn)) 
	{
		int kk =0;
		if (m_bMCBModeSel) kk= 2* m_iFullChannelWidth/1024;
		else  kk= 2*m_iBuffFullChannelWidth/1024;
		UINT max = 0;
		for(int i=0;i<512;i++) 
		{
			max = 0;
			for(int j=i*kk;j<(i+1)*kk;j++) 
			{ //ѡ��kk ���ȷ�Χ�������һ������ֵ
				if (m_arrExternalRam[j] > max)
				{
					max = m_arrExternalRam[j];
				}
			}
			m_arrFullRam[i] = max;
		}
		OnExternal2ExpRam();
	}

	CDC* pDC=GetDC();
	
	CDC MemDC; //���ȶ���һ����ʾ�豸��ρE
	CBitmap MemBitmap;//����һ��λͼ��ρE
	MemDC.CreateCompatibleDC(NULL);
	MemBitmap.CreateCompatibleBitmap(pDC,1024,960);
	CBitmap *pOldBit=MemDC.SelectObject(&MemBitmap);	
	MemDC.FillSolidRect(0,0,1024,960,RGB(219,219,219));//26,131,162

	OnDrawBackgroud(&MemDC);
	OnDrawSmallWin(&MemDC,m_iSmallWinCenPoint,	m_iSmallWinCHL,m_iSmallWinCHR);
	
	OnReDrawExpRamLine(&MemDC);

	OnDrawFullRamLine(&MemDC);	//ȫͼ
	
	OnDrawTextBgroud(&MemDC);
//	OnShowStateText(&MemDC,1,m_strStateLine1Text);
//	OnShowStateText(&MemDC,2,m_strStateLine2Text);
//	OnShowStateText(&MemDC,3,m_strStateLine3Text);

	pDC->BitBlt(0,0,1024,960,&MemDC,0,0,SRCCOPY);	
	
	MemBitmap.DeleteObject();
	MemDC.DeleteDC();
	ReleaseDC(pDC);	

//  m_ScrollBar.ShowScrollBar(FALSE);
//	m_ScrollBar.ShowScrollBar(TRUE);
//	m_ScrollBar.SetFocus();

}

void CMCBView::OnReDrawExpRamLine(CDC* pDC)
{
   CMainFrame* pMainFrame= (CMainFrame*)GetParentFrame();
   CComboBox* pComboxVer = (CComboBox *) (pMainFrame->m_wndDlgBar.GetDlgItem(IDC_COMBO_VER));
   CComboBox* pComboxExpVer = (CComboBox *) (pMainFrame->m_wndDlgBar.GetDlgItem(IDC_COMBO_EXP_VER));

   CString strbuf,strUnit;
   CString strFull,strExp;
   if (m_bMCBModeSel)
   {
	 m_iExpRamEthValue=OnCaculateExpRamEthValue();

	if (m_bDrawLogData_Sel)
	{
		strFull.Format("%s","Log");
		strExp.Format("%s","Log");
	}
	else
	{
		if (m_iFullRamEthValue>1024)
		{
	       int i=m_iFullRamEthValue/1024;
			strFull.Format("%i%s",i,"k");

		}
		else
		{
			int i=m_iFullRamEthValue;
			strFull.Format("%i",i);
		}
    
	  if (m_iExpRamEthValue>1024)
		{
		 int i=m_iExpRamEthValue/1024;
			strExp.Format("%i%s",i,"k");
		}
		else
		{
			int i=m_iExpRamEthValue;
			strExp.Format("%i",i);
		}
	}

	float kev;
	kev = g_HardWare_Pare.fEngerScaleSlope * m_iSmallWinCenPoint  + g_HardWare_Pare.fEngerScaleOffset;
	if (m_bMcbMenuEnergeScale_CalcuEn) 
	{
		switch(g_HardWare_Pare.iEngerScaleUnit) 
		{
		case 0: strUnit.Format("��ַ:  %d     ",m_iSmallWinCenPoint);
			break;
		case 1: 			
			strUnit.Format("��ַ:  %d (%.3f eV)    ",m_iSmallWinCenPoint,kev);
			break;
		case 2:
			strUnit.Format("��ַ:  %d (%.3f KeV)    ",m_iSmallWinCenPoint,kev);
			break;
		case 3:
			strUnit.Format("��ַ:  %d (%.3f MeV)    ",m_iSmallWinCenPoint,kev);
			break;
		default:  
			strUnit.Format("��ַ:  %d (*****)    ",m_iSmallWinCenPoint);
			break;
		}
		if(!m_bPrintMapEn)
		{
			strbuf.Format("����:%d         Full: VScale:%s     HScale(Gain):%d            Exp: VScale:%s   HScale:%d",
			(m_arrExpRam[(m_iSamllWinWidth/2)] & 0x7fffffff), strFull,m_iFullChannelWidth,strExp,m_iSamllWinWidth);
			m_strStateLine1Text = strUnit + strbuf;
		}
	}
	else {
		if(!m_bPrintMapEn)
		{
			strbuf.Format("��ַ:  %d    ����:%d         Full: VScale:%s     HScale(Gain):%d            Exp: VScale:%s   HScale:%d",
			m_iSmallWinCenPoint,(m_arrExpRam[(m_iSamllWinWidth/2)] & 0x7fffffff), strFull,m_iFullChannelWidth,strExp,m_iSamllWinWidth);
			m_strStateLine1Text = strbuf;
		}

	}

	float maxvalue = 1;
	float fmaxvalue = 0.1;
	float fminvalue = 10;
	
	if (m_bDrawLogData_Sel) 
	{

	}
    else
		{
		for(int j=0;j<m_iSamllWinWidth;j++)
		{
			maxvalue = max(maxvalue,(m_arrExpRam[j] & 0x7fffffff));
		}
	
			j=0;
			m_iExpRamEthValue = 64;
		while(j<17)
		{
			if(maxvalue < m_iExpRamEthValue) break;
			j++;
			m_iExpRamEthValue *= 2;
		}
			m_iBuffComboxSel=17-j;
			maxvalue =m_iExpRamEthValue;
		}
	pComboxExpVer->SetCurSel(m_iBuffComboxSel);

	
	if (m_bDrawLogData_Sel)
	{
			maxvalue=8;
	}	
	int go2one[8193];
	if (m_bDrawLogData_Sel) {
		for(int i=0;i<m_iSamllWinWidth;i++) {
			go2one[i] = (float)m_iViewWindowSize_Y*418.0/500 - (float)m_iViewWindowSize_Y*(325.0/500)*(m_farrExpRam[i]/(float)(maxvalue));
			if(go2one[i] > (float)m_iViewWindowSize_Y*418/500) go2one[i] = m_iViewWindowSize_Y*418/500;
			if(go2one[i] < (float)m_iViewWindowSize_Y*100/500) go2one[i] = m_iViewWindowSize_Y*100/500;
		}
	}
	else{
		for(int i=0;i<m_iSamllWinWidth;i++) {
			go2one[i] = m_iViewWindowSize_Y*418/500 - m_iViewWindowSize_Y*325/500*((float)(m_arrExpRam[i] & 0x7fffffff)/maxvalue);
			if(go2one[i] > m_iViewWindowSize_Y*418/500) go2one[i] = m_iViewWindowSize_Y*418/500;
			if(go2one[i] < m_iViewWindowSize_Y*100/500) go2one[i] = m_iViewWindowSize_Y*100/500;
		}
	}
	
	CPen pShow2(PS_SOLID,1,RGB(255,0,128));//������
	CPen pShow3(PS_SOLID,2,m_Interest_Color);//����Ȥ����
	CBrush bShow;
	bShow.CreateSolidBrush(m_Interest_Color);

	CPen pShowCenter(PS_SOLID,1,RGB(240,20,30));//������	
	pDC->SelectObject(&pShowCenter);
	pDC->MoveTo(m_iBigWinCenPoint,m_iViewWindowSize_Y*95/500);
	pDC->LineTo(m_iBigWinCenPoint,m_iViewWindowSize_Y*420/500);//	

	CPen* pOldPen=NULL;
	pOldPen=pDC->SelectObject(&pShow2);
	pDC->MoveTo(0,m_iViewWindowSize_Y*418/500);

	BOOL bcounter = false;
	int intcounter=0;
	float intertdotbuff;
	float step_x,high_y;

	step_x = (float)m_iViewWindowSize_X/(float)m_iSamllWinWidth;
	high_y = (float)m_iViewWindowSize_Y*418/500.0;
	for(int i=0;i<m_iSamllWinWidth;i++) 
	{
		if(m_arrExpRam[i] >= 0x80000000 ) 
		{
			pDC->SelectObject(&pShow3);
			pDC->SelectObject(&bShow);
			pDC->Rectangle(i*step_x,go2one[i],(i+1)*step_x,high_y+1);
			
		}
		else {
			pOldPen=pDC->SelectObject(&pShow2);			
			if (m_bDrawLine_Sel) {  //����				
				pDC->LineTo((i*m_iViewWindowSize_X/m_iSamllWinWidth),go2one[i]);				
			}
			else { //��E Ϊ���������ȣ�������㣨���£�				
				if(m_bPrintMapEn == TRUE){
					pDC->SetPixel((i*m_iViewWindowSize_X/m_iSamllWinWidth),go2one[i] + 1,RGB(10,0,0));
					pDC->SetPixel((i*m_iViewWindowSize_X/m_iSamllWinWidth),go2one[i],RGB(10,0,0));
				}
				else{
					pDC->SetPixel((i*m_iViewWindowSize_X/m_iSamllWinWidth),go2one[i] + 1,RGB(255,255,255));
					pDC->SetPixel((i*m_iViewWindowSize_X/m_iSamllWinWidth),go2one[i],RGB(255,255,255));
				}

			}
		}		
	}

	if (m_arrExpRam[(m_iSamllWinWidth/2)] >= 0x80000000 ) 
	{
		CPen pShowCenter22(PS_SOLID,1,RGB(226,233,107));//������
		pDC->SelectObject(&pShowCenter22);
		pDC->MoveTo(m_iBigWinCenPoint,m_iViewWindowSize_Y*95/500);
		pDC->LineTo(m_iBigWinCenPoint,m_iViewWindowSize_Y*420/500);//
	}
 }
 else
 {
	 m_iBuffExpRamEthValue=OnCaculateExpRamEthValue();

	if (m_bDrawLogData_Sel)
	{
		strFull.Format("%s","Log");
		strExp.Format("%s","Log");
	}
	else
	{
		if (m_iBuffFullRamEthValue>1024)
		{
	       int i=m_iBuffFullRamEthValue/1024;
			strFull.Format("%i%s",i,"k");

		}
		else
		{
			int i=m_iBuffFullRamEthValue;
			strFull.Format("%i",i);
		}
    
	  if (m_iBuffExpRamEthValue>1024)
		{
		 int i=m_iBuffExpRamEthValue/1024;
			strExp.Format("%i%s",i,"k");
		}
		else
		{
			int i=m_iBuffExpRamEthValue;
			strExp.Format("%i",i);
		}
	}

	float kev;
	kev = m_fMenuEnergeScale_K * m_iBuffSmallWinCenPoint  + m_fMenuEnergeScale_b;
	if (m_bMenuEnergeScale_CalcuEn) 
	{

		switch(m_fMenuEnergeScale_U) 
		{
		case 0: strUnit.Format("��ַ:  %d     ",m_iBuffSmallWinCenPoint);
			break;
		case 1: 			
			strUnit.Format("��ַ:  %d (%.3f eV)    ",m_iBuffSmallWinCenPoint,kev);
			break;
		case 2:
			strUnit.Format("��ַ:  %d (%.3f KeV)    ",m_iBuffSmallWinCenPoint,kev);
			break;
		case 3:
			strUnit.Format("��ַ:  %d (%.3f MeV)    ",m_iBuffSmallWinCenPoint,kev);
			break;
		default:  
			strUnit.Format("��ַ:  %d (*****)    ",m_iBuffSmallWinCenPoint);
			break;
		}
		if(!m_bPrintMapEn)
		{
			strbuf.Format("����:%d         Full: VScale:%s     HScale(Gain):%d            Exp: VScale:%s   HScale:%d",
			(m_arrExpRam[(m_iBuffSmallWindowWidth/2)] & 0x7fffffff), strFull,m_iBuffFullChannelWidth,strExp,m_iBuffSmallWindowWidth);
			m_strStateLine1Text = strUnit + strbuf;
		}
	}
	else {
		if(!m_bPrintMapEn)
		{
			strbuf.Format("��ַ:  %d    ����:%d         Full: VScale:%s     HScale(Gain):%d            Exp: VScale:%s   HScale:%d",
			m_iBuffSmallWinCenPoint,(m_arrExpRam[(m_iBuffSmallWindowWidth/2)] & 0x7fffffff), strFull,m_iBuffFullChannelWidth,strExp,m_iBuffSmallWindowWidth);
			m_strStateLine1Text = strbuf;
		}

	}

	float maxvalue = 1;
	float fmaxvalue = 0.1;
	float fminvalue = 10;
	
	if (m_bDrawLogData_Sel) 
	{

	}
    else
		{
		for(int j=0;j<m_iBuffSmallWindowWidth;j++)
		{
			maxvalue = max(maxvalue,(m_arrExpRam[j] & 0x7fffffff));
		}
	
			j=0;
			m_iBuffExpRamEthValue = 64;
		while(j<17)
		{
			if(maxvalue < m_iBuffExpRamEthValue) break;
			j++;
			m_iBuffExpRamEthValue *= 2;
		}
			m_iBuffExpRamCheckSel=17-j;
			maxvalue =m_iBuffExpRamEthValue;
		}
	pComboxExpVer->SetCurSel(m_iBuffExpRamCheckSel);
	if (m_bDrawLogData_Sel)
	{
			maxvalue=8;
	}	
	int go2one[8193];
	if (m_bDrawLogData_Sel) {
		for(int i=0;i<m_iBuffSmallWindowWidth;i++) 
		{
			go2one[i] = (float)m_iViewWindowSize_Y*418.0/500 - (float)m_iViewWindowSize_Y*(325.0/500)*(m_farrExpRam[i]/(float)(maxvalue));
			if(go2one[i] > (float)m_iViewWindowSize_Y*418/500) go2one[i] = m_iViewWindowSize_Y*418/500;
			if(go2one[i] < (float)m_iViewWindowSize_Y*100/500) go2one[i] = m_iViewWindowSize_Y*100/500;
		}
	}
	else{
		for(int i=0;i<m_iBuffSmallWindowWidth;i++)
		{
			go2one[i] = m_iViewWindowSize_Y*418/500 - m_iViewWindowSize_Y*325/500*((float)(m_arrExpRam[i] & 0x7fffffff)/maxvalue);
			if(go2one[i] > m_iViewWindowSize_Y*418/500) go2one[i] = m_iViewWindowSize_Y*418/500;
			if(go2one[i] < m_iViewWindowSize_Y*100/500) go2one[i] = m_iViewWindowSize_Y*100/500;
		}
	}
	
	CPen pShow2(PS_SOLID,1,RGB(255,0,128));//������
	CPen pShow3(PS_SOLID,2,m_Interest_Color);//����Ȥ����
	CBrush bShow;
	bShow.CreateSolidBrush(m_Interest_Color);

	CPen pShowCenter(PS_SOLID,1,RGB(240,20,30));//������	
	pDC->SelectObject(&pShowCenter);
	pDC->MoveTo(m_iBigWinCenPoint,m_iViewWindowSize_Y*95/500);
	pDC->LineTo(m_iBigWinCenPoint,m_iViewWindowSize_Y*420/500);//	

	CPen* pOldPen=NULL;
	pOldPen=pDC->SelectObject(&pShow2);
	pDC->MoveTo(0,m_iViewWindowSize_Y*418/500);

	BOOL bcounter = false;
	int intcounter=0;
	float intertdotbuff;
	float step_x,high_y;

	step_x = (float)m_iViewWindowSize_X/(float)m_iBuffSmallWindowWidth;
	high_y = (float)m_iViewWindowSize_Y*418/500.0;
	for(int i=0;i<m_iBuffSmallWindowWidth;i++) 
	{
		if(m_arrExpRam[i] >= 0x80000000 ) 
		{
			pDC->SelectObject(&pShow3);
			pDC->SelectObject(&bShow);
			pDC->Rectangle(i*step_x,go2one[i],(i+1)*step_x,high_y+1);
			
		}
		else 
		{
			pOldPen=pDC->SelectObject(&pShow2);			
			if (m_bDrawLine_Sel) 
			{  //����				
				pDC->LineTo((i*m_iViewWindowSize_X/m_iBuffSmallWindowWidth),go2one[i]);				
			}
			else { //��E Ϊ���������ȣ�������㣨���£�				
				if(m_bPrintMapEn == TRUE){
					pDC->SetPixel((i*m_iViewWindowSize_X/m_iBuffSmallWindowWidth),go2one[i] + 1,RGB(10,0,0));
					pDC->SetPixel((i*m_iViewWindowSize_X/m_iBuffSmallWindowWidth),go2one[i],RGB(10,0,0));
				}
				else{
					pDC->SetPixel((i*m_iViewWindowSize_X/m_iBuffSmallWindowWidth),go2one[i] + 1,RGB(255,255,255));
					pDC->SetPixel((i*m_iViewWindowSize_X/m_iBuffSmallWindowWidth),go2one[i],RGB(255,255,255));
				}

			}
		}		
	}

	if (m_arrExpRam[(m_iBuffSmallWindowWidth/2)] >= 0x80000000 ) 
	{
		CPen pShowCenter22(PS_SOLID,1,RGB(226,233,107));//������
		pDC->SelectObject(&pShowCenter22);
		pDC->MoveTo(m_iBigWinCenPoint,m_iViewWindowSize_Y*95/500);
		pDC->LineTo(m_iBigWinCenPoint,m_iViewWindowSize_Y*420/500);//
	}
 }
	
}




//-----------------------------------------------------------------//
//Input: recpackagelenth �������ݰ�Eĳ���
//
//
//-----------------------------------------------------------------//
BOOL CMCBView::OnUnpack(unsigned char *recbuff, int recpackagelenth)
{
	CMainFrame* pMainFrame = (CMainFrame*)GetParentFrame();

//	CEdit* pShowTime = (CEdit*)(pMainFrame->m_wndDlgBar.GetDlgItem(IDC_SHOW_REAL_LIVE_TIME));
	CEdit* pShowRealTime = (CEdit*)(pMainFrame->m_wndDlgBar.GetDlgItem(IDC_SHOW_REAL_TIME));
	CEdit* pShowLiveTime = (CEdit*)(pMainFrame->m_wndDlgBar.GetDlgItem(IDC_SHOW_LIVE_TIME));
	CEdit* pShowRatioTime = (CEdit*)(pMainFrame->m_wndDlgBar.GetDlgItem(IDC_SHOW_RATIO_TIME));

	CEdit* pQueryTimeout=(CEdit*)(pMainFrame->m_wndDlgBar.GetDlgItem(IDC_QUERY_TIMEOUT));
	CEdit* pQuerySampleState=(CEdit*)(pMainFrame->m_wndDlgBar.GetDlgItem(IDC_QUERY_SAMPLE_STATE));

	char chrtext[200];
	CString text;
	UINT itemp;
	float frotio;
	switch( g_ThreadCMD ){
			case IDC_CTL_START_SAMPLE:	
				if(recpackagelenth == 1) 
				{
					sprintf(chrtext,"��ʼӦ��\n��������Ϊ %.2x   ",recbuff[0]);
//					AfxMessageBox(chrtext);
					text.Format("%s %.2x   ","��ʼӦ��E��������Ϊ:",recbuff[0]);
					TextScroll(text);
				}
				else 
				{
//					AfxMessageBox("Ӧ�����ݵĳ��Ȳ���,����ʼ������");
					text.Format("%s ","Ӧ�����ݵĳ��Ȳ���,����ʼ������");
					TextScroll(text);
				}
				break;
			case IDC_CTL_STOP_SAMPLE:
				if(recpackagelenth == m_iFullChannelWidth * 3) { // ��ʱ����External Ram�е����ݣ�
					for(int i=0;i<m_iFullChannelWidth;i++) 
					{
						type.uchdata[0] = recbuff[i*3];
						type.uchdata[1] = recbuff[i*3 +1];
						type.uchdata[2] = recbuff[i*3 +2];						
						m_arrExternalRam[i] = (m_arrExternalRam[i] & 0x80000000) | (type.intdata & 0x00FFFFFF);
						m_arrMCBExternalRam[i] = (m_arrMCBExternalRam[i] & 0x80000000) | (type.intdata & 0x00FFFFFF);
					}
					//Redraw();						
					OnHandleSelectMCBmode();
					
					pQueryTimeout->SetWindowText("��ѯ��ʱ: Stop!");
					pQuerySampleState->SetWindowText("���ݻ�ȡ: Stop!");
                    m_continueflag=0;
					KillTimer(1);
					m_bEnReadFPGATimeValue = true;
					
				}
				else 
				{
//					AfxMessageBox("�ϴ�ExternalRam���ݵĳ��Ȳ���");
					text.Format("�ռ�����: %s ","�ϴ�ExternalRam���ݵĳ��Ȳ���");
					TextScroll(text);
				}
				break;
			case IDC_COMTEST: //����
				if(recpackagelenth == 256) {
					int i = 0;
					while(i < 256) {
						if( recbuff[i] != i)  i = 300;
						i++;
					}
					if(i == 256) {
						PulseEvent(g_hEvtFinishRec);  //akangquan 
						sprintf(chrtext,"���� Ӧ��ɹ���");
						text.Format("ͨ�Ų���: %s","����Ӧ��ɹ�!");
					}
					else 
					{ 
						sprintf(chrtext,"���� �������ݲ��ԣ�");
						text.Format("ͨ�Ų���: %s","���Խ������ݲ���!");
					}
//					AfxMessageBox(chrtext);
					TextScroll(text);
				}
				else {
//					AfxMessageBox("����Ӧ�����ݵĳ��Ȳ���");
					text.Format("ͨ�Ų���: %s","����Ӧ�����ݵĳ��Ȳ���!");
					TextScroll(text);
				}
				break;	
			case CTL_LOAD_FULL_RAM:
				if(recpackagelenth == 512*3) { // ��ʱ����Full Ram�е����ݣ�
					for(int i=0;i<512;i++) 
					{
						type.uchdata[0] = recbuff[i*3];
						type.uchdata[1] = recbuff[i*3 +1];
						type.uchdata[2] = recbuff[i*3 +2];
						m_arrFullRam[i] = (m_arrFullRam[i] & 0x80000000) | (type.intdata & 0x00FFFFFF);
					}
					if(!m_bMCBModeSel) break;
					Redraw();					
				}
				else {
//					AfxMessageBox("�ϴ�FullRam���ݵĳ��Ȳ���");	
                    text.Format("%s","�ϴ�FullRam���ݵĳ��Ȳ���!");
					TextScroll(text);
				}
				break;
			case CTL_LOAD_EXP_RAM:			
				if(recpackagelenth == m_ExpRamDataLong_old) { // ��ʱ����Expend Ram�е����ݣ�					
					for(int i=0;i<1024;i++) 
					{
						m_arrExpRam[i] &= 0x80000000;
					}
					for(i=0;i< m_ExpRamDataLong_old/3 ;i++) 
					{
						type.uchdata[0] = recbuff[i*3];
						type.uchdata[1] = recbuff[i*3 +1];
						type.uchdata[2] = recbuff[i*3 +2];
						m_arrExpRam[i] = (type.intdata & 0x00FFFFFF);
					}
					if(!m_bMCBModeSel) break;
					OnExpRamModify();
					Redraw();
				}
				else {
//					AfxMessageBox("�ϴ�ExpRam���ݵĳ��Ȳ���");
					text.Format("%s","�ϴ�ExpRam���ݵĳ��Ȳ���!");
					TextScroll(text);
				}
				break;
			case CTL_LOAD_TIME_VALUE:
				if(recpackagelenth == 8) { // ��ʱ����ʱ��ֵ					
					type.uchdata[0] = recbuff[0];
					type.uchdata[1] = recbuff[1];
					type.uchdata[2] = recbuff[2];
					type.uchdata[3] = recbuff[3];			
					m_iRealTime = type.intdata;
					type.uchdata[0] = recbuff[4];
					type.uchdata[1] = recbuff[5];
					type.uchdata[2] = recbuff[6];
					type.uchdata[3] = recbuff[7];
					m_iLiveTime = type.intdata;

					if(m_iRealTime == 0) {
						frotio = 100.;
					}
					else{
						frotio = (100.0*(float)(m_iRealTime - m_iLiveTime)/(float)m_iRealTime);
					}

					sprintf(chrtext,"ʵʱ: %ds",m_iRealTime/100); //ʱ�䵥λ��1/100ÁE
					pShowRealTime->SetWindowText(chrtext);
					
					sprintf(chrtext,"���: %ds",m_iLiveTime/100);
					pShowLiveTime->SetWindowText(chrtext);
					sprintf(chrtext,"����: %.2f %%",frotio);
					pShowRatioTime->SetWindowText(chrtext);

					//��E� �ɼ��� ʵʱ��E�� ��汼�E
					m_iSaveFileRealTime = m_iRealTime;
					m_iSaveFileLiveTime = m_iLiveTime;

				}
				else {
//					AfxMessageBox("ʱ�����ݵĳ��Ȳ���");
					text.Format("%s","ʱ�����ݵĳ��Ȳ���!");
					TextScroll(text);
				}
				break;
			case CTL_LOAD_INTERST_CHANNEL_COUNTER:
				if(recpackagelenth == 3) { // ��ʱ����Ȥ������					
					type.uchdata[0] = recbuff[0];
					type.uchdata[1] = recbuff[1];
					type.uchdata[2] = recbuff[2];					
					itemp = type.intdata & 0x40FFFFFF;
//					sprintf(chrtext,"��ַ:  %d    ����:%d       ������߽�E%d  �����ұ߽�E%d   ����߶�: %d",m_iSmallWinCenPoint,itemp, m_iSmallWinCHL,m_iSmallWinCHR,m_iExpRamEthValue);
					m_strStateLine1Text = chrtext;
					Redraw();											
				}
				else {
//					AfxMessageBox("����Ȥ���������ݵĳ��Ȳ���   ");	
					text.Format("%s","����Ȥ���������ݵĳ��Ȳ���!");
					TextScroll(text);
				}
				break;
			case IDC_RECEIVE_EXTERNAL_RAM:
				if(recpackagelenth == m_iFullChannelWidth * 3) { // ��ʱ����External Ram�е����ݣ�
					for(int i=0;i<m_iFullChannelWidth;i++) {
						type.uchdata[0] = recbuff[i*3];
						type.uchdata[1] = recbuff[i*3 +1];
						type.uchdata[2] = recbuff[i*3 +2];
									
						m_arrExternalRam[i] = (m_arrExternalRam[i] & 0x80000000) | (type.intdata & 0x00FFFFFF);
						m_arrMCBExternalRam[i] = (m_arrMCBExternalRam[i] & 0x80000000) | (type.intdata & 0x00FFFFFF);
						
					}					
					m_bEnReadFPGATimeValue = true;

					pQueryTimeout->SetWindowText("��ѯ��ʱ: Stop!");
					pQuerySampleState->SetWindowText("���ݻ�ȡ: Stop!");
					m_continueflag=0;
					m_ScrollBar.ShowScrollBar(FALSE);
					m_ScrollBar.ShowScrollBar(TRUE);
					//Redraw();
					OnHandleSelectMCBmode();
				}
				else {
//					AfxMessageBox("�ϴ�ExternalRam���ݵĳ��Ȳ���");
					text.Format("%s","�ϴ�ExternalRam���ݵĳ��Ȳ���!");
					TextScroll(text);
				}
				break;
			case IDC_SEND_EXTERNAL_RAM:
				if(recpackagelenth == 1) {
					if(recbuff[0] == 1) {
//						AfxMessageBox("�´�ExternalRam����́E�");
						text.Format("%s","�´�ExternalRam����́E�!");
						TextScroll(text);
					}
				}  
				break;
			case IDC_QUERY_TIMEOUT:		//��ѯ��ʱ��
				if(recpackagelenth == 1) { // 					
					if(recbuff[0] == TRUE) {						
						pQueryTimeout->SetWindowText("��ѯ��ʱ: Stop!");
						pQuerySampleState->SetWindowText("���ݻ�ȡ: Stop!");
						KillTimer(1);						
						OnMenuStopsample();
						m_continueflag=0;
                        m_ScrollBar.ShowScrollBar(FALSE);
					    m_ScrollBar.ShowScrollBar(TRUE);
					}else 
					{
						pQueryTimeout->SetWindowText("��ѯ��ʱ: Run");
					}

				}
				else {
//					AfxMessageBox("��ѯ��ʱ��Ӧ��ĳ��Ȳ���   ");
					text.Format("%s","��ѯ��ʱ��Ӧ��ĳ��Ȳ���!");
					TextScroll(text);
				}
				break;
			case IDC_QUERY_SAMPLE_STATE: //��ѯ���ݻ�ȡ״̬
				if(recpackagelenth == 1) { // 					
					if(recbuff[0] == TRUE) 
					{						
						pQuerySampleState->SetWindowText("���ݻ�ȡ: Run");
					}
					else { 
						pQuerySampleState->SetWindowText("���ݻ�ȡ: Stop!");
						KillTimer(1);												
						OnMenuStopsample();
					}
				}
				else {
//					AfxMessageBox("���ݻ�ȡ״̬Ӧ��ĳ��Ȳ���   ");
					text.Format("%s","���ݻ�ȡ״̬Ӧ��ĳ��Ȳ���!");
					TextScroll(text);
				}
				break;
			case CTL_HARDWARE_READ: // Ӳ��������ȡ
				if (recpackagelenth == 23) { //
						OnHardWareParaUnpack(recbuff,recpackagelenth);
						m_pHardWarePara->OnUpdataShow(); // ��E�Ӳ�������Ի���E
				}
				else {
//					AfxMessageBox("Ӳ��������ȡ��Ӧ�𳤶Ȳ���");
					text.Format("%s","Ӳ��������ȡ��Ӧ�𳤶Ȳ���!");
					TextScroll(text);
				}
				break;
			case CTL_HARDWARE_WRITE:
				break;
			default:
				break;
	}
	g_ThreadCMD = NULL;		//����ÁE����Ϊ��
	return true;
}

//Ŀǰ ȫ���û���512��E
void CMCBView::OnDrawFullRamLine(CDC* pDC)  
{		
  float maxvalue;
  if (m_bMCBModeSel)
  {
	maxvalue = OnCaculateFullRamEthValue();
	if (m_bDrawLogData_Sel)
	{
			maxvalue=8;
	}	
	if((maxvalue > -0.001) && (maxvalue < 0.001))
	{
		maxvalue = m_iViewWindowSize_Y*95/500;
	}
	
	int go2one[1024];
	if (m_bDrawLogData_Sel) 
	{
		for(int i=0;i<512;i++) 
		{
			go2one[i] = m_iViewWindowSize_Y*95/500 - ((m_iViewWindowSize_Y*95/500) )*((float)(m_farrFullRam[i])/(maxvalue));
			if(go2one[i] >= m_iViewWindowSize_Y*94/500) go2one[i] = m_iViewWindowSize_Y*94/500;
			if(go2one[i] < m_iViewWindowSize_Y*5/500) go2one[i] = m_iViewWindowSize_Y*5/500; 
		}
	}
	else
	{
		for(int i=0;i<512;i++) 
		{
			go2one[i] = m_iViewWindowSize_Y*95/500 - ((m_iViewWindowSize_Y*95/500) )*((float)(m_arrFullRam[i] & 0x7fffffff)/maxvalue);
			if(go2one[i] >= m_iViewWindowSize_Y*94/500) go2one[i] = m_iViewWindowSize_Y*94/500;
			if(go2one[i] < m_iViewWindowSize_Y*5/500) go2one[i] = m_iViewWindowSize_Y*5/500; 
		}
	}

	pDC->MoveTo(0,m_iViewWindowSize_Y*95/500-1);
	pDC->LineTo(0,m_iViewWindowSize_Y*95/500-1);

	CPen pShow2(PS_SOLID,1,RGB(255,255,255));//RGB(128,0,0)FullRam������(����ɫ)
	CPen pShow3(PS_SOLID,1,m_Interest_Color);//FullRam������(����Ȥɫ)
	CPen pShowPrint(PS_SOLID,1,RGB(0,0,0));//FullRam������(��ӡ��ɫ)
	
	CPen* pOldPen=NULL;
	CPen* pOldPenOther=NULL;

	float step_x,high_y;

	step_x = (float)m_iViewWindowSize_X / 512.0;
	high_y = (float)m_iViewWindowSize_Y*95/500.0;

	pDC->MoveTo(0,go2one[0]-2); //m_iViewWindowSize_Y*95/500
	for(int i=0;i<512;i++) 
	{
		if(m_arrFullRam[i] >= 0x80000000 ) 
		{
			if (m_bPrintMapEn) 
			{
				pOldPen=pDC->SelectObject(&pShowPrint);
			}
			else{
				pOldPen=pDC->SelectObject(&pShow3);
			}			

			//Ϊ�������򴰿ڴ�С���������ֵÿ����ƣ��໭һ�����ߣ�
			pDC->MoveTo(i*step_x,high_y-2);
			pDC->LineTo(i*step_x,go2one[i]-2);
			if (m_arrFullRam[i+1]>=0x80000000)
			{
				pDC->MoveTo(i*step_x + 1,high_y-2);
				pDC->LineTo(i*step_x + 1,go2one[i]-2);
				pDC->MoveTo(i*step_x + 2,high_y-2);
				pDC->LineTo(i*step_x + 2,go2one[i]-2);
			}

		}
		else {			
			if (m_bPrintMapEn) {
				pOldPen=pDC->SelectObject(&pShowPrint);
			}
			else{
				pOldPen=pDC->SelectObject(&pShow2);
			}
			if(m_arrFullRam[i+1] < 0x80000000 ) {
				pDC->MoveTo(i*step_x,go2one[i]);
				pDC->LineTo((i+1)*step_x,go2one[i+1]);
			}
			else {
				pDC->MoveTo(i*step_x,go2one[i]);
				pDC->LineTo((i)*step_x,go2one[i]);
			}
			
		}
	}

	CPen pShowCen(PS_SOLID,1,RGB(255,10,0));//RGB(0,210,0)
	pDC->SelectObject(&pShowCen);
	pDC->MoveTo(m_iSmallWinCenPoint* m_iViewWindowSize_X/m_iFullChannelWidth -1,0);
	pDC->LineTo(m_iSmallWinCenPoint* m_iViewWindowSize_X/m_iFullChannelWidth -1,m_iViewWindowSize_Y*95/500-1);

	CPen pShowlege(PS_SOLID,1,RGB(0,0,0));//RGB(0,210,0)
	pDC->SelectObject(&pShowlege);
	pDC->MoveTo(m_iSmallWinCHL* m_iViewWindowSize_X/m_iFullChannelWidth,0);
	pDC->LineTo(m_iSmallWinCHL* m_iViewWindowSize_X/m_iFullChannelWidth,m_iViewWindowSize_Y*95/500-1);
	pDC->MoveTo(m_iSmallWinCHR* m_iViewWindowSize_X/m_iFullChannelWidth -1,0);
	pDC->LineTo(m_iSmallWinCHR* m_iViewWindowSize_X/m_iFullChannelWidth -1,m_iViewWindowSize_Y*95/500-1);
  }
  else
  {
	maxvalue = OnCaculateFullRamEthValue();
	if (m_bDrawLogData_Sel)
	{
			maxvalue=8;
	}	
	if((maxvalue > -0.001) && (maxvalue < 0.001))
	{
		maxvalue = m_iViewWindowSize_Y*95/500;
	}
	
	int go2one[1024];
	if (m_bDrawLogData_Sel) 
	{
		for(int i=0;i<512;i++) 
		{
			go2one[i] = m_iViewWindowSize_Y*95/500 - ((m_iViewWindowSize_Y*95/500) )*((float)(m_farrFullRam[i])/(maxvalue));
			if(go2one[i] >= m_iViewWindowSize_Y*94/500) go2one[i] = m_iViewWindowSize_Y*94/500;
			if(go2one[i] < m_iViewWindowSize_Y*5/500) go2one[i] = m_iViewWindowSize_Y*5/500; 
		}
	}
	else
	{
		for(int i=0;i<512;i++) 
		{
			go2one[i] = m_iViewWindowSize_Y*95/500 - ((m_iViewWindowSize_Y*95/500) )*((float)(m_arrFullRam[i] & 0x7fffffff)/maxvalue);
			if(go2one[i] >= m_iViewWindowSize_Y*94/500) go2one[i] = m_iViewWindowSize_Y*94/500;
			if(go2one[i] < m_iViewWindowSize_Y*5/500) go2one[i] = m_iViewWindowSize_Y*5/500; 
		}
	}

	pDC->MoveTo(0,m_iViewWindowSize_Y*95/500-1);
	pDC->LineTo(0,m_iViewWindowSize_Y*95/500-1);

	CPen pShow2(PS_SOLID,1,RGB(255,255,255));//RGB(128,0,0)FullRam������(����ɫ)
	CPen pShow3(PS_SOLID,1,m_Interest_Color);//FullRam������(����Ȥɫ)
	CPen pShowPrint(PS_SOLID,1,RGB(0,0,0));//FullRam������(��ӡ��ɫ)
	
	CPen* pOldPen=NULL;
	CPen* pOldPenOther=NULL;

	float step_x,high_y;

	step_x = (float)m_iViewWindowSize_X / 512.0;
	high_y = (float)m_iViewWindowSize_Y*95/500.0;

	pDC->MoveTo(0,go2one[0]-2); //m_iViewWindowSize_Y*95/500
	for(int i=0;i<512;i++) {
		if(m_arrFullRam[i] >= 0x80000000 ) {
			if (m_bPrintMapEn) {//pDC->IsPrinting()
				pOldPen=pDC->SelectObject(&pShowPrint);
			}
			else{
				pOldPen=pDC->SelectObject(&pShow3);
			}			

			//Ϊ�������򴰿ڴ�С���������ֵÿ����ƣ��໭һ�����ߣ�
			pDC->MoveTo(i*step_x,high_y-2);
			pDC->LineTo(i*step_x,go2one[i]-2);
			if (m_arrFullRam[i+1]>=0x80000000)
			{
				pDC->MoveTo(i*step_x + 1,high_y-2);
				pDC->LineTo(i*step_x + 1,go2one[i]-2);
				pDC->MoveTo(i*step_x + 2,high_y-2);
				pDC->LineTo(i*step_x + 2,go2one[i]-2);
			}

		}
		else {			
			if (m_bPrintMapEn) {
				pOldPen=pDC->SelectObject(&pShowPrint);
			}
			else{
				pOldPen=pDC->SelectObject(&pShow2);
			}
			if(m_arrFullRam[i+1] < 0x80000000 ) {
				pDC->MoveTo(i*step_x,go2one[i]);
				pDC->LineTo((i+1)*step_x,go2one[i+1]);
			}
			else {
				pDC->MoveTo(i*step_x,go2one[i]);
				pDC->LineTo((i)*step_x,go2one[i]);
			}
			
		}
	}

	CPen pShowCen(PS_SOLID,1,RGB(255,10,0));//RGB(0,210,0)
	pDC->SelectObject(&pShowCen);
	pDC->MoveTo(m_iBuffSmallWinCenPoint* m_iViewWindowSize_X/m_iBuffFullChannelWidth -1,0);
	pDC->LineTo(m_iBuffSmallWinCenPoint* m_iViewWindowSize_X/m_iBuffFullChannelWidth -1,m_iViewWindowSize_Y*95/500-1);

	CPen pShowlege(PS_SOLID,1,RGB(0,0,0));//RGB(0,210,0)
	pDC->SelectObject(&pShowlege);
	pDC->MoveTo(m_iBuffSmallWinCHL* m_iViewWindowSize_X/m_iBuffFullChannelWidth,0);
	pDC->LineTo(m_iBuffSmallWinCHL* m_iViewWindowSize_X/m_iBuffFullChannelWidth,m_iViewWindowSize_Y*95/500-1);
	pDC->MoveTo(m_iBuffSmallWinCHR* m_iViewWindowSize_X/m_iBuffFullChannelWidth -1,0);
	pDC->LineTo(m_iBuffSmallWinCHR* m_iViewWindowSize_X/m_iBuffFullChannelWidth -1,m_iViewWindowSize_Y*95/500-1);
  }

}

BOOL CMCBView::OnDrawExpRamLine(CDC* pDC)
{

	CMainFrame* pMainFrame= (CMainFrame*)GetParentFrame();
	CComboBox* pComboxVer = (CComboBox *) (pMainFrame->m_wndDlgBar.GetDlgItem(IDC_COMBO_VER));
	CButton * pStaticChannel = (CButton *)(pMainFrame->m_wndDlgBar.GetDlgItem(IDC_STATIC_CHANNEL));
	CButton * pStaticCount = (CButton *)(pMainFrame->m_wndDlgBar.GetDlgItem(IDC_STATIC_COUNT));

    CString channel,count;
/*	if (m_bMCBModeSel)
    {
		channel.Format("��ַ: %d",m_iSmallWinCenPoint);
		count.Format("����: %d",m_arrExternalRam[m_iSmallWinCenPoint]);
        pStaticChannel->SetWindowText(channel);
		pStaticCount->SetWindowText(count);
    }
*/
	CString strbuf,strUnit;
	CString strFull,strExp;
    m_iExpRamEthValue=OnCaculateExpRamEthValue();
    m_iBuffExpRamEthValue=OnCaculateExpRamEthValue();
 if (m_bMCBModeSel)
 {
	if (m_bDrawLogData_Sel)
	{
		strFull.Format("%s","Log");
		strExp.Format("%s","Log");
	}
	else
	{
		if (m_iFullRamEthValue>1024)
		{
	       int i=m_iFullRamEthValue/1024;
			strFull.Format("%i%s",i,"k");

		}
		else
		{
			int i=m_iFullRamEthValue;
			strFull.Format("%i",i);
		}
    
	  if (m_iExpRamEthValue>1024)
		{
		 int i=m_iExpRamEthValue/1024;
			strExp.Format("%i%s",i,"k");
		}
		else
		{
			int i=m_iExpRamEthValue;
			strExp.Format("%i",i);
		}
	}
	
	float kev;
	kev = g_HardWare_Pare.fEngerScaleSlope * m_iSmallWinCenPoint  + g_HardWare_Pare.fEngerScaleOffset;
	if (m_bMcbMenuEnergeScale_CalcuEn) 
	{
		switch(g_HardWare_Pare.iEngerScaleUnit) 
		{
		case 0: strUnit.Format("��ַ:  %d     ",m_iSmallWinCenPoint);
			    channel.Format(" %d CHN\n NONE Energy",m_iSmallWinCenPoint);
			break;
		case 1: 			
			strUnit.Format("��ַ:  %d (%.3f eV)    ",m_iSmallWinCenPoint,kev);
			channel.Format(" %d CHN\n %.3f eV    ",m_iSmallWinCenPoint,kev);
			break;
		case 2:
			strUnit.Format("��ַ:  %d (%.3f KeV)    ",m_iSmallWinCenPoint,kev);
			channel.Format(" %d CHN\n %.3f KeV",m_iSmallWinCenPoint,kev);
			break;
		case 3:
			strUnit.Format("��ַ:  %d %.3f MeV    ",m_iSmallWinCenPoint,kev);
			channel.Format(" %d CHN\n %.3f MeV",m_iSmallWinCenPoint,kev);
			break;
		default:  
			strUnit.Format("��ַ:  %d (*****)    ",m_iSmallWinCenPoint);
			break;
		}

		if(!m_bPrintMapEn)
		{
			strbuf.Format("����:%d         Full: VScale:%s     HScale(Gain):%d            Exp: VScale:%s   HScale:%d",
			(m_arrExpRam[(m_iSamllWinWidth/2)] & 0x7fffffff), strFull,m_iFullChannelWidth,strExp,m_iSamllWinWidth);
            count.Format(" %d CNTS",m_arrExpRam[(m_iSamllWinWidth/2)]&0x7fffffff);
			m_strStateLine1Text = strUnit + strbuf;
		}
		
	}
	else {
		if(!m_bPrintMapEn)
		{
			strbuf.Format("��ַ:  %d    ����:%d         Full: VScale:%s     HScale(Gain):%d            Exp: VScale:%s   HScale:%d",
			m_iSmallWinCenPoint,(m_arrExpRam[(m_iSamllWinWidth/2)] & 0x7fffffff), strFull,m_iFullChannelWidth,strExp,m_iSamllWinWidth);
			channel.Format(" %d CHN\n NONE Energy",m_iSmallWinCenPoint);
		    count.Format(" %d CNTS",m_arrExpRam[(m_iSamllWinWidth/2)]&0x7fffffff);
			m_strStateLine1Text = strbuf;
		}

	}
	
    pStaticChannel->SetWindowText(channel);
    pStaticCount->SetWindowText(count);

	float maxvalue = 1;
	maxvalue = OnCaculateExpRamEthValue();
	if (m_bDrawLogData_Sel)
	{
			maxvalue=8;		
	}
	int go2one[8193];
	if (m_bDrawLogData_Sel) {
		for(int i=0;i<m_iSamllWinWidth;i++) {
			go2one[i] = (float)m_iViewWindowSize_Y*418.0/500 - (float)m_iViewWindowSize_Y*(325.0/500)*(m_farrExpRam[i]/(float)(maxvalue));
			if(go2one[i] > (float)m_iViewWindowSize_Y*418/500) go2one[i] = m_iViewWindowSize_Y*418/500;
			if(go2one[i] < (float)m_iViewWindowSize_Y*100/500) go2one[i] = m_iViewWindowSize_Y*100/500;
		}
	}
	else{
		for(int i=0;i<m_iSamllWinWidth;i++) {
			go2one[i] = m_iViewWindowSize_Y*418/500 - m_iViewWindowSize_Y*325/500*((float)(m_arrExpRam[i] & 0x7fffffff)/maxvalue);
			if(go2one[i] > m_iViewWindowSize_Y*418/500) go2one[i] = m_iViewWindowSize_Y*418/500;
			if(go2one[i] < m_iViewWindowSize_Y*100/500) go2one[i] = m_iViewWindowSize_Y*100/500;
		}
	}
	
	CPen pShow2(PS_SOLID,1,RGB(255,0,128));//������
	CPen pShow3(PS_SOLID,2,m_Interest_Color);//����Ȥ����
	CBrush bShow;
	bShow.CreateSolidBrush(m_Interest_Color);

	CPen pShowCenter(PS_SOLID,1,RGB(240,20,30));//������	
	pDC->SelectObject(&pShowCenter);
	pDC->MoveTo(m_iBigWinCenPoint,m_iViewWindowSize_Y*95/500);
	pDC->LineTo(m_iBigWinCenPoint,m_iViewWindowSize_Y*420/500);//	

	CPen* pOldPen=NULL;
	pOldPen=pDC->SelectObject(&pShow2);
	pDC->MoveTo(0,m_iViewWindowSize_Y*418/500);

	BOOL bcounter = false;
	int intcounter=0;
	float intertdotbuff;
	float step_x,high_y;

	step_x = (float)m_iViewWindowSize_X/(float)m_iSamllWinWidth;
	high_y = (float)m_iViewWindowSize_Y*418/500.0;
	for(int i=0;i<m_iSamllWinWidth;i++) 
	{
		if(m_arrExpRam[i] >= 0x80000000 ) 
		{
			pDC->SelectObject(&pShow3);
			pDC->SelectObject(&bShow);
			pDC->Rectangle(i*step_x,go2one[i],(i+1)*step_x,high_y+1);
			
		}
		
		else 
		{
			pOldPen=pDC->SelectObject(&pShow2);			
			if (m_bDrawLine_Sel) 
			{  //����				
				pDC->LineTo((i*m_iViewWindowSize_X/m_iSamllWinWidth),go2one[i]);				
			}
			else 
			{ //��E Ϊ���������ȣ�������㣨���£�				
				if(m_bPrintMapEn == TRUE)
				{
					pDC->SetPixel((i*m_iViewWindowSize_X/m_iSamllWinWidth),go2one[i] + 1,RGB(10,0,0));
					pDC->SetPixel((i*m_iViewWindowSize_X/m_iSamllWinWidth),go2one[i],RGB(10,0,0));
				}
				else
				{
					pDC->SetPixel((i*m_iViewWindowSize_X/m_iSamllWinWidth),go2one[i] + 1,RGB(255,255,255));
					pDC->SetPixel((i*m_iViewWindowSize_X/m_iSamllWinWidth),go2one[i],RGB(255,255,255));
				}

			}
		}		
	}

	if (m_arrExpRam[(m_iSamllWinWidth/2)] >= 0x80000000 ) 
	{
		CPen pShowCenter22(PS_SOLID,1,RGB(226,233,107));//������
		pDC->SelectObject(&pShowCenter22);
		pDC->MoveTo(m_iBigWinCenPoint,m_iViewWindowSize_Y*95/500);
		pDC->LineTo(m_iBigWinCenPoint,m_iViewWindowSize_Y*420/500);//
	}
	return TRUE;
 }
 else
 {
	if (m_bDrawLogData_Sel)
	{
		strFull.Format("%s","Log");
		strExp.Format("%s","Log");
	}
	else
	{
		if (m_iBuffFullRamEthValue>1024)
		{
	       int i=m_iBuffFullRamEthValue/1024;
			strFull.Format("%i%s",i,"k");
		}
		else
		{
			int i=m_iBuffFullRamEthValue;
			strFull.Format("%i",i);
		}
    
	  if (m_iBuffExpRamEthValue>1024)
		{
		 int i=m_iBuffExpRamEthValue/1024;
			strExp.Format("%i%s",i,"k");
		}
		else
		{
			int i=m_iBuffExpRamEthValue;
			strExp.Format("%i",i);
		}
	}
	
	float kev;
	kev = m_fMenuEnergeScale_K * m_iBuffSmallWinCenPoint  + m_fMenuEnergeScale_b;
	if (m_bMenuEnergeScale_CalcuEn) 
	{
		switch(m_fMenuEnergeScale_U) 
		{
		case 0: strUnit.Format("��ַ:  %d     ",m_iBuffSmallWinCenPoint);
			channel.Format(" %d CHN \n NONE Energy",m_iBuffSmallWinCenPoint);
			break;
		case 1: 			
			strUnit.Format("��ַ:  %d (%.3feV)    ",m_iBuffSmallWinCenPoint,kev);
			channel.Format(" %d CHN\n %.3f eV",m_iBuffSmallWinCenPoint,kev);
			break;
		case 2:
			strUnit.Format("��ַ:  %d (%.3f KeV)    ",m_iBuffSmallWinCenPoint,kev);
			channel.Format(" %d CHN\n %.3f KeV",m_iBuffSmallWinCenPoint,kev);
			break;
		case 3:
			strUnit.Format("��ַ:  %d (%.3f MeV)    ",m_iBuffSmallWinCenPoint,kev);
			channel.Format(" %d CHN\n %.3f MeV",m_iBuffSmallWinCenPoint,kev);
			break;
		default:  
			strUnit.Format("��ַ:  %d (*****)    ",m_iBuffSmallWinCenPoint);
			break;
		}
		if(!m_bPrintMapEn)
		{
			strbuf.Format("����:%d         Full: VScale:%s     HScale(Gain):%d            Exp: VScale:%s   HScale:%d",
			(m_arrExpRam[(m_iSamllWinWidth/2)] & 0x7fffffff), strFull,m_iBuffFullChannelWidth,strExp,m_iBuffSmallWindowWidth);
            count.Format(" %d CNTS",m_arrExpRam[(m_iBuffSmallWindowWidth/2)]&0x7fffffff);
			m_strStateLine1Text = strUnit + strbuf;
		}
	}
	else {
		if(!m_bPrintMapEn)
		{
			strbuf.Format("��ַ:  %d    ����:%d         Full: VScale:%s     HScale(Gain):%d            Exp: VScale:%s   HScale:%d",
			m_iBuffSmallWinCenPoint,(m_arrExpRam[(m_iBuffSmallWindowWidth/2)] & 0x7fffffff), strFull,m_iBuffFullChannelWidth,strExp,m_iBuffSmallWindowWidth);
			channel.Format(" %d CHN \n NONE Energy",m_iBuffSmallWinCenPoint);
            count.Format(" %d CNTS",m_arrExpRam[(m_iBuffSmallWindowWidth/2)]&0x7fffffff);
			m_strStateLine1Text = strbuf;
		}

	}
	
    pStaticChannel->SetWindowText(channel);
	pStaticCount->SetWindowText(count);


	float maxvalue = 1;
	maxvalue = OnCaculateExpRamEthValue();
	if (m_bDrawLogData_Sel)
	{
			maxvalue=8;		
	}
	int go2one[8193];
	if (m_bDrawLogData_Sel)
	{
		for(int i=0;i<m_iBuffSmallWindowWidth;i++) 
		{
			go2one[i] = (float)m_iViewWindowSize_Y*418.0/500 - (float)m_iViewWindowSize_Y*(325.0/500)*(m_farrExpRam[i]/(float)(maxvalue));
			if(go2one[i] > (float)m_iViewWindowSize_Y*418/500) go2one[i] = m_iViewWindowSize_Y*418/500;
			if(go2one[i] < (float)m_iViewWindowSize_Y*100/500) go2one[i] = m_iViewWindowSize_Y*100/500;
		}
	}
	else
	{
		for(int i=0;i<m_iBuffSmallWindowWidth;i++) 
		{
			go2one[i] = m_iViewWindowSize_Y*418/500 - m_iViewWindowSize_Y*325/500*((float)(m_arrExpRam[i] & 0x7fffffff)/maxvalue);
			if(go2one[i] > m_iViewWindowSize_Y*418/500) go2one[i] = m_iViewWindowSize_Y*418/500;
			if(go2one[i] < m_iViewWindowSize_Y*100/500) go2one[i] = m_iViewWindowSize_Y*100/500;
		}
	}
	
	CPen pShow2(PS_SOLID,1,RGB(255,0,128));//������
	CPen pShow3(PS_SOLID,2,m_Interest_Color);//����Ȥ����
	CBrush bShow;
	bShow.CreateSolidBrush(m_Interest_Color);

	CPen pShowCenter(PS_SOLID,1,RGB(240,20,30));//������	
	pDC->SelectObject(&pShowCenter);
	pDC->MoveTo(m_iBigWinCenPoint,m_iViewWindowSize_Y*95/500);
	pDC->LineTo(m_iBigWinCenPoint,m_iViewWindowSize_Y*420/500);//	

	CPen* pOldPen=NULL;
	pOldPen=pDC->SelectObject(&pShow2);
	pDC->MoveTo(0,m_iViewWindowSize_Y*418/500);

	BOOL bcounter = false;
	int intcounter=0;
	float intertdotbuff;
	float step_x,high_y;

	step_x = (float)m_iViewWindowSize_X/(float)m_iBuffSmallWindowWidth;
	high_y = (float)m_iViewWindowSize_Y*418/500.0;
	for(int i=0;i<m_iBuffSmallWindowWidth;i++) 
	{
		if(m_arrExpRam[i] >= 0x80000000 ) 
		{
			pDC->SelectObject(&pShow3);
			pDC->SelectObject(&bShow);
			pDC->Rectangle(i*step_x,go2one[i],(i+1)*step_x,high_y+1);
			
		}
		
		else 
		{
			pOldPen=pDC->SelectObject(&pShow2);			
			if (m_bDrawLine_Sel) 
			{  				
				pDC->LineTo((i*m_iViewWindowSize_X/m_iBuffSmallWindowWidth),go2one[i]);				
			}
			else 
			{ 				
				if(m_bPrintMapEn == TRUE)
				{
					pDC->SetPixel((i*m_iViewWindowSize_X/m_iBuffSmallWindowWidth),go2one[i] + 1,RGB(10,0,0));
					pDC->SetPixel((i*m_iViewWindowSize_X/m_iBuffSmallWindowWidth),go2one[i],RGB(10,0,0));
				}
				else
				{
					pDC->SetPixel((i*m_iViewWindowSize_X/m_iBuffSmallWindowWidth),go2one[i] + 1,RGB(255,255,255));
					pDC->SetPixel((i*m_iViewWindowSize_X/m_iBuffSmallWindowWidth),go2one[i],RGB(255,255,255));
				}

			}
		}		
	}

	if (m_arrExpRam[(m_iBuffSmallWindowWidth/2)] >= 0x80000000 ) 
	{
		CPen pShowCenter22(PS_SOLID,1,RGB(226,233,107));//������
		pDC->SelectObject(&pShowCenter22);
		pDC->MoveTo(m_iBigWinCenPoint,m_iViewWindowSize_Y*95/500);
		pDC->LineTo(m_iBigWinCenPoint,m_iViewWindowSize_Y*420/500);//
	}
	return TRUE;
 }
}

void CMCBView::OnExpramViewsel() 
{
	CMainFrame* pMainFrame = (CMainFrame*)GetParentFrame();
//	CButton* pExpViewSel = (CButton *)(pMainFrame->m_wndDlgBar.GetDlgItem(IDC_EXPRAM_VIEWSEL));
//	CButton* pFullViewSel = (CButton *)(pMainFrame->m_wndDlgBar.GetDlgItem(IDC_FULLRAM_VIEWSEL));
	CComboBox* pComboxHor = (CComboBox *)(pMainFrame->m_wndDlgBar.GetDlgItem(IDC_COMBO_HOR));
//	CComboBox* pComboxVer = (CComboBox *)(pMainFrame->m_wndDlgBar.GetDlgItem(IDC_COMBO_VER));
	CButton * pStaticHor = (CButton *)(pMainFrame->m_wndDlgBar.GetDlgItem(IDC_STATIC_HOR));
	CButton* pButton = (CButton *)(pMainFrame->m_wndDlgBar.GetDlgItem(IDC_MCB_MODSEL));
	CComboBox* pComboxExpHor = (CComboBox *)(pMainFrame->m_wndDlgBar.GetDlgItem(IDC_COMBO_EXP_HOR));
	CComboBox* pComboxExpVer = (CComboBox *)(pMainFrame->m_wndDlgBar.GetDlgItem(IDC_COMBO_EXP_VER));	
	
//	pExpViewSel->SetCheck(1);
//	pFullViewSel->SetCheck(0);
 //   m_bExpRamViewSel = true;

	
//	pStaticHor->SetWindowText("Hor");
 if (m_bMCBModeSel)
 {	
	 pComboxHor->EnableWindow(TRUE);
	if(pButton->GetCheck()&&(m_startflag==1))
	{
		pComboxExpHor->ResetContent();
		pComboxExpHor->AddString("64");
		pComboxExpHor->AddString("128");
		pComboxExpHor->AddString("256");
		pComboxExpHor->AddString("512");
	}
	else
	{
			pComboxExpHor->ResetContent();
            switch(m_iFullChannelWidth)////////////////////////////////changing
			{
				case 1024: 
				pComboxExpHor->AddString("64");
				pComboxExpHor->AddString("128");
				pComboxExpHor->AddString("256");
				pComboxExpHor->AddString("512");
                pComboxExpHor->AddString("1024");break;
				case 2048: 				
				pComboxExpHor->AddString("64");
				pComboxExpHor->AddString("128");
				pComboxExpHor->AddString("256");
				pComboxExpHor->AddString("512");
                pComboxExpHor->AddString("1024");
				pComboxExpHor->AddString("2048");break;
				case 4096:
				pComboxExpHor->AddString("64");
				pComboxExpHor->AddString("128");
				pComboxExpHor->AddString("256");
				pComboxExpHor->AddString("512");
                pComboxExpHor->AddString("1024");
				pComboxExpHor->AddString("2048");
				pComboxExpHor->AddString("4096");break;	
				case 8192: 				
				pComboxExpHor->AddString("64");
				pComboxExpHor->AddString("128");
				pComboxExpHor->AddString("256");
				pComboxExpHor->AddString("512");
                pComboxExpHor->AddString("1024");
				pComboxExpHor->AddString("2048");
				pComboxExpHor->AddString("4096");
				pComboxExpHor->AddString("8192");break;
				default: break;
			}

	}
	if(pButton->GetCheck()&&(m_startflag==1))
	{
		if(m_iSmallWindowWidthCheckSel>3)
		m_iSmallWindowWidthCheckSel=3;
		pComboxExpHor->SetCurSel(m_iSmallWindowWidthCheckSel);
	}
	else
	{
		
		int index=0;
        switch(m_iFullChannelWidth)
		{
				case 1024: index=4;break;
				case 2048: index=5;break;
				case 4096: index=6;break;
				case 8192: index=7;break;
		}
		if(m_iSmallWindowWidthCheckSel>index)
		{
           m_iSmallWindowWidthCheckSel=index;
		}
		pComboxExpHor->SetCurSel(m_iSmallWindowWidthCheckSel);
	}/////////////////////////////////////////////////////////////changing
    

	int nindexHor = m_iSmallWindowWidthCheckSel;

	switch(nindexHor) {
	case 0:
		m_iSamllWinWidth = 64;
		break;
	case 1:
		m_iSamllWinWidth = 128;
		break;
	case 2:
		m_iSamllWinWidth = 256;
		break;
	case 3:
		m_iSamllWinWidth = 512;
		break;
	case 4:
		m_iSamllWinWidth = 1024;
		break;
	case 5:
		m_iSamllWinWidth = 2048;
		break;
	case 6:
		m_iSamllWinWidth = 4096;
		break;
	case 7:
		m_iSamllWinWidth = 8192;
		break;
	default:
		break;
	}	
	m_iSmallWinCHL = m_iSmallWinCenPoint - m_iSamllWinWidth/2;
	m_iSmallWinCHR = m_iSmallWinCenPoint + m_iSamllWinWidth/2;

	if (m_iSmallWinCHL< 0) {m_iSmallWinCHL = 0;	}
	if (m_iSmallWinCHR > m_iFullChannelWidth-1) {m_iSmallWinCHR = m_iFullChannelWidth;	}

	pComboxExpVer->SetCurSel(m_iBuffComboxSel);
	int iinput= 17 - m_iBuffComboxSel;
	int j = 64;
	for(int i=0;i<iinput;i++) 
	{
		j *= 2;
	}
    m_iExpRamEthValue=j;
	m_bExpRamViewSel = true;
	
	Redraw();
}
 else
 {
	 //pComboxHor->EnableWindow(TRUE);
	 pComboxExpHor->ResetContent();
    switch(m_iBuffFullChannelWidth)////////////////////////////////changing
	{
	    case 1024: 
			    pComboxExpHor->AddString("64");
				pComboxExpHor->AddString("128");
				pComboxExpHor->AddString("256");
				pComboxExpHor->AddString("512");
                pComboxExpHor->AddString("1024");break;
	    case 2048: 				
				pComboxExpHor->AddString("64");
				pComboxExpHor->AddString("128");
				pComboxExpHor->AddString("256");
				pComboxExpHor->AddString("512");
                pComboxExpHor->AddString("1024");
				pComboxExpHor->AddString("2048");break;
		case 4096:
				pComboxExpHor->AddString("64");
				pComboxExpHor->AddString("128");
				pComboxExpHor->AddString("256");
				pComboxExpHor->AddString("512");
                pComboxExpHor->AddString("1024");
				pComboxExpHor->AddString("2048");
				pComboxExpHor->AddString("4096");break;	
		case 8192: 				
				pComboxExpHor->AddString("64");
				pComboxExpHor->AddString("128");
				pComboxExpHor->AddString("256");
				pComboxExpHor->AddString("512");
                pComboxExpHor->AddString("1024");
				pComboxExpHor->AddString("2048");
				pComboxExpHor->AddString("4096");
				pComboxExpHor->AddString("8192");break;
		default: break;
			
	}
	int index=0;
    switch(m_iBuffFullChannelWidth)
	{
		case 1024: index=4;break;
		case 2048: index=5;break;
		case 4096: index=6;break;
		case 8192: index=7;break;
	}
	if(m_iBuffSmallWindowWidthSel>index)
	{
       m_iBuffSmallWindowWidthSel=index;
	}
	pComboxExpHor->SetCurSel(m_iBuffSmallWindowWidthSel);
	int nindexHor = m_iBuffSmallWindowWidthSel;

	switch(nindexHor) 
	{
	case 0:
		m_iBuffSmallWindowWidth = 64;
		break;
	case 1:
		m_iBuffSmallWindowWidth = 128;
		break;
	case 2:
		m_iBuffSmallWindowWidth = 256;
		break;
	case 3:
		m_iBuffSmallWindowWidth = 512;
		break;
	case 4:
		m_iBuffSmallWindowWidth = 1024;
		break;
	case 5:
		m_iBuffSmallWindowWidth = 2048;
		break;
	case 6:
		m_iBuffSmallWindowWidth = 4096;
		break;
	case 7:
		m_iBuffSmallWindowWidth = 8192;
		break;
	default:
		break;
	}	
	m_iBuffSmallWinCHL = m_iBuffSmallWinCenPoint - m_iBuffSmallWindowWidth/2;
	m_iBuffSmallWinCHR = m_iBuffSmallWinCenPoint + m_iBuffSmallWindowWidth/2;

	if (m_iBuffSmallWinCHL< 0) {m_iBuffSmallWinCHL = 0;	}
	if (m_iBuffSmallWinCHR > m_iBuffFullChannelWidth-1) {m_iBuffSmallWinCHR = m_iBuffFullChannelWidth;}

//	pComboxExpVer->SetCurSel(m_iBuffExpRamCheckSel);
	int iinput= 17 - m_iBuffExpRamCheckSel;
	int j = 64;
	for(int i=0;i<iinput;i++) 
	{
		j *= 2;
	}
    m_iBuffExpRamEthValue=j;
	m_bExpRamViewSel = true;
//	Redraw();
 }

}

void CMCBView::OnFullramViewsel() 
{
	CMainFrame* pMainFrame = (CMainFrame*)GetParentFrame();
//	CButton* pExpViewSel = (CButton *)(pMainFrame->m_wndDlgBar.GetDlgItem(IDC_EXPRAM_VIEWSEL));
//	CButton* pFullViewSel = (CButton *)(pMainFrame->m_wndDlgBar.GetDlgItem(IDC_FULLRAM_VIEWSEL));
	
	CComboBox* pComboxHor = (CComboBox *)(pMainFrame->m_wndDlgBar.GetDlgItem(IDC_COMBO_HOR));
	CComboBox* pComboxVer = (CComboBox *)(pMainFrame->m_wndDlgBar.GetDlgItem(IDC_COMBO_VER));
	CButton * pStaticHor = (CButton *)(pMainFrame->m_wndDlgBar.GetDlgItem(IDC_STATIC_HOR));
	CComboBox* pComboxExpVer = (CComboBox *)(pMainFrame->m_wndDlgBar.GetDlgItem(IDC_COMBO_EXP_VER));	

	pStaticHor->SetWindowText("Gain");

//	pExpViewSel->SetCheck(0);
//	pFullViewSel->SetCheck(1);

	pComboxHor->ResetContent();
	pComboxHor->AddString("1024");
	pComboxHor->AddString("2048");
	pComboxHor->AddString("4096");
	pComboxHor->AddString("8192");
  	/*if(m_iSmallWindowWidthCheckSel>3)///////////////////////////////changing
	{
		m_iSmallWindowWidthCheckSel=3;
	}
	pComboxHor->SetCurSel(m_iSmallWindowWidthCheckSel);*/
 if (m_bMCBModeSel)
  {
 	
	 switch(m_iFullChannelWidth) 
	{
 		case 1024: pComboxHor->SetCurSel(0); break;
 		case 2048: pComboxHor->SetCurSel(1); break;
 		case 4096: pComboxHor->SetCurSel(2); break;
 		case 8192: pComboxHor->SetCurSel(3); break;	
 		default:   pComboxHor->SetCurSel(0); break;		
 	}	
    pComboxHor->EnableWindow(TRUE);

    pComboxVer->SetCurSel(m_iFullRamCheckSel);
	int kkk1=64;
	for(int j=0;j<17-m_iFullRamCheckSel;j++)
	{
		kkk1 *= 2;
	}
    	m_iFullRamEthValue = kkk1;	
	
	///////////////////////////////////////////////////////////////changing
	m_iSmallWinCHL = m_iSmallWinCenPoint - m_iSamllWinWidth/2;
	m_iSmallWinCHR = m_iSmallWinCenPoint + m_iSamllWinWidth/2;


//	m_bExpRamViewSel = false;
	Redraw();
  }
  else
  {
 	switch(m_iBuffFullChannelWidth) 
	{
 		case 1024: pComboxHor->SetCurSel(0); break;
 		case 2048: pComboxHor->SetCurSel(1); break;
 		case 4096: pComboxHor->SetCurSel(2); break;
 		case 8192: pComboxHor->SetCurSel(3); break;	
 		default:   pComboxHor->SetCurSel(0); break;		
 	}	
    pComboxHor->EnableWindow(FALSE);
    pComboxVer->SetCurSel(m_iBuffFullRamCheckSel);
	
	int kkk1=64;
	for(int j=0;j<17-m_iBuffFullRamCheckSel;j++)
	{
		kkk1 *= 2;
	}
    	m_iBuffFullRamEthValue = kkk1;	
	
	///////////////////////////////////////////////////////////////changing
	m_iBuffSmallWinCHL = m_iBuffSmallWinCenPoint - m_iBuffSmallWindowWidth/2;
	m_iBuffSmallWinCHR = m_iBuffSmallWinCenPoint + m_iBuffSmallWindowWidth/2;
//	m_bExpRamViewSel = false;
	Redraw();
  }
 

}

void CMCBView::OnMcbModsel() 
{
	//���ؿ���ͳ�ƶԻ���E
	CMainFrame* pMainFrame = (CMainFrame*)GetParentFrame();
	CButton* pMCB_TO_BUF = (CButton *)(pMainFrame->m_wndDlgBar.GetDlgItem(IDC_EXTERNALDATA_MCB2BUFF));
	CComboBox* pComboxHor = (CComboBox*)(pMainFrame->m_wndDlgBar.GetDlgItem(IDC_COMBO_HOR));
	CComboBox* pComboxExpHor = (CComboBox*)(pMainFrame->m_wndDlgBar.GetDlgItem(IDC_COMBO_EXP_HOR));
//	CButton* pFULLHor = (CButton*)(pMainFrame->m_wndDlgBar.GetDlgItem(IDC_FULLRAM_VIEWSEL));
// 	CButton* pExpHor = (CButton*)(pMainFrame->m_wndDlgBar.GetDlgItem(IDC_EXPRAM_VIEWSEL));
	CEdit* pShowRealTime = (CEdit*)(pMainFrame->m_wndDlgBar.GetDlgItem(IDC_SHOW_REAL_TIME));
	CEdit* pShowLiveTime = (CEdit*)(pMainFrame->m_wndDlgBar.GetDlgItem(IDC_SHOW_LIVE_TIME));
	CEdit* pShowRatioTime = (CEdit*)(pMainFrame->m_wndDlgBar.GetDlgItem(IDC_SHOW_RATIO_TIME));
// 	pExpHor->SetCheck(TRUE);
//	pFULLHor->SetCheck(FALSE);

    pComboxHor->EnableWindow(TRUE);
		switch(m_iFullChannelWidth) /////////////////////////////////////////changing
		{
			case 1024: pComboxHor->SetCurSel(0); break;
			case 2048: pComboxHor->SetCurSel(1); break;
			case 4096: pComboxHor->SetCurSel(2); break;
			case 8192: pComboxHor->SetCurSel(3); break;
			default: break;
		}
   //////////////////////////////////////////////////////////changing
		if(m_startflag)
		{
			pComboxExpHor->ResetContent();
		    pComboxExpHor->AddString("64");
		    pComboxExpHor->AddString("128");
			pComboxExpHor->AddString("256");
			pComboxExpHor->AddString("512");
			if (m_iSamllWinWidth>=512)
			{
				m_iSmallWindowWidthCheckSel=3;
			}
			pComboxExpHor->SetCurSel(m_iSmallWindowWidthCheckSel);
		}
		if(!m_startflag)
		{
			pComboxExpHor->ResetContent();
			pComboxExpHor->AddString("64");
			pComboxExpHor->AddString("128");
			pComboxExpHor->AddString("256");
			pComboxExpHor->AddString("512");
			pComboxExpHor->AddString("1024");
			pComboxExpHor->AddString("2048");
			pComboxExpHor->AddString("4096");
			pComboxExpHor->AddString("8192");
			pComboxExpHor->SetCurSel(m_iSmallWindowWidthCheckSel);
		}

//	pComboxHor->SetCurSel(m_iSmallWindowWidthCheckSel);///////////////////changing
//	CComboBox* pComboxVer = (CComboBox *)(pMainFrame->m_wndDlgBar.GetDlgItem(IDC_COMBO_VER));
//	m_iBuffComboxSel = pComboxVer->GetCurSel();
//	pMCB_TO_BUF->ShowWindow(TRUE);
	
	g_MCB_Oper.iStartQueryNo = MCB_START_QUERYNO;
	g_MCB_Oper.iCurQueryNo = MCB_START_QUERYNO;

	m_bMCBModeSel= TRUE;
	
	for(int i=0;i<1024*8;i++) 
	{
//		m_arrExternalRam[i] = ((m_arrMCBExternalRam[i] & 0x7fffffff) | (m_arrBUFFExternalRam[i] & 0x80000000));
		m_arrExternalRam[i]=m_arrMCBExternalRam[i];
	}
    for(i=0;i<1024*8;i++)
	{
		m_arrFullRam[i] &= 0x00000000;
		m_arrExpRam[i] &= 0x00000000;
	}
///////////////////////////////////////////////////////////////changing
/*	for(i=0;i<1024*8;i++)
	{
		m_arrMCBExternalRam[i] &= 0x80000000;
	}*/
///////////////////////////////////////////////////////////////
	m_bAutoCalcuExpRamEth = TRUE;
//	m_bAutoCalcuFullRamEth = TRUE;

OnFullramViewsel();
OnExpramViewsel();


	Redraw();

    m_ScrollBar.ShowScrollBar(FALSE);
	m_ScrollBar.ShowScrollBar(TRUE);


	m_bAutoCalcuExpRamEth = FALSE;
	m_bAutoCalcuFullRamEth = FALSE;
////////////////////////////////////////////////////////////////changing
/*
	if (!pFULLHor->GetCheck())
	{
		OnExpramViewsel();
	}
	else
	{
		OnExpramViewsel();
		OnFullramViewsel();
	}
*/   
    pMCB_TO_BUF->SetWindowText("MCB--->BUF");
}

void CMCBView::OnBuffModsel() 
{
	CMainFrame* pMainFrame = (CMainFrame*)GetParentFrame();
	CButton* pMCB_TO_BUF = (CButton *)(pMainFrame->m_wndDlgBar.GetDlgItem(IDC_EXTERNALDATA_MCB2BUFF));
	CEdit* pShowRealTime = (CEdit*)(pMainFrame->m_wndDlgBar.GetDlgItem(IDC_SHOW_REAL_TIME));
	CEdit* pShowLiveTime = (CEdit*)(pMainFrame->m_wndDlgBar.GetDlgItem(IDC_SHOW_LIVE_TIME));
	CEdit* pShowRatioTime = (CEdit*)(pMainFrame->m_wndDlgBar.GetDlgItem(IDC_SHOW_RATIO_TIME));

//	pMCB_TO_BUF->ShowWindow(FALSE);
	
	CComboBox* pComboxExpHor = (CComboBox*)(pMainFrame->m_wndDlgBar.GetDlgItem(IDC_COMBO_EXP_HOR));
	CComboBox* pComboxHor = (CComboBox*)(pMainFrame->m_wndDlgBar.GetDlgItem(IDC_COMBO_HOR));
//	CButton* pFULL_BUF = (CButton *)(pMainFrame->m_wndDlgBar.GetDlgItem(IDC_FULLRAM_VIEWSEL));
	if (m_iTimeFileSourceflag==1)
	{
//        CEdit *pSource=(CEdit*)(pMainFrame->m_wndDlgBar.GetDlgItem(IDC_SOURCE));
//		pSource->SetWindowText("����:�ļ�");
	}
		/*pComboxHor->ResetContent();
		pComboxHor->AddString("1024");
		pComboxHor->AddString("2048");
		pComboxHor->AddString("4096");
		pComboxHor->AddString("8192");
		*/
		switch(m_iBuffFullChannelWidth)////////////////////////////////changing
		{
			case 1024: pComboxHor->SetCurSel(0); break;
			case 2048: pComboxHor->SetCurSel(1); break;
			case 4096: pComboxHor->SetCurSel(2); break;
			case 8192: pComboxHor->SetCurSel(3); break;
			default: break;
		}
        
		pComboxHor->EnableWindow(FALSE);
	
		pComboxExpHor->ResetContent();
		pComboxExpHor->AddString("64");
		pComboxExpHor->AddString("128");
		pComboxExpHor->AddString("256");
		pComboxExpHor->AddString("512");
		pComboxExpHor->AddString("1024");
		pComboxExpHor->AddString("2048");
		pComboxExpHor->AddString("4096");
		pComboxExpHor->AddString("8192");

		pComboxExpHor->SetCurSel(m_iBuffSmallWindowWidthSel);//////////changing


	g_MCB_Oper.iStartQueryNo = BUFF_START_QUERYNO;
	g_MCB_Oper.iCurQueryNo = BUFF_START_QUERYNO;

	m_bMCBModeSel= false;

	memcpy(m_arrExternalRam,m_arrBUFFExternalRam,1024*4*8);

//	CButton* pButton = (CButton*)GetDlgItem(IDC_COMBO_HOR);///////////changing
	m_bAutoCalcuExpRamEth = TRUE;
//	m_bAutoCalcuFullRamEth = TRUE;

OnFullramViewsel();
OnExpramViewsel();


	Redraw();

	m_ScrollBar.ShowScrollBar(FALSE);
	m_ScrollBar.ShowScrollBar(TRUE);

	
	m_bAutoCalcuExpRamEth = FALSE;
	m_bAutoCalcuFullRamEth = FALSE;
	pMCB_TO_BUF->SetWindowText("MCB<---BUF");

}

void CMCBView::OnSelchangeComboHor() 
{
	CMainFrame* pMainFrame = (CMainFrame*)GetParentFrame();
	CComboBox* pComboxHor = (CComboBox *)(pMainFrame->m_wndDlgBar.GetDlgItem(IDC_COMBO_HOR));
	CComboBox* pComboxExpHor = (CComboBox *)(pMainFrame->m_wndDlgBar.GetDlgItem(IDC_COMBO_EXP_HOR));
  int nindexHor = pComboxHor->GetCurSel();
  
   if (m_bMCBModeSel)
   {
        // ȫ��ѡ��ģʽ
		//AfxMessageBox("Ŀǰȫ��ֻ��ѡԁE1024 ��ʽ  ");
		//pComboxHor->SetCurSel(0);
		//return;
		if (g_MCB_Oper.bQueryEn) 
		{ // ϵͳ�ڲɼ��Ĺ����в��ܸı����E

			switch(m_iFullChannelWidth) 
			{
			case 1024: pComboxHor->SetCurSel(0); break;
			case 2048: pComboxHor->SetCurSel(1); break;
			case 4096: pComboxHor->SetCurSel(2); break;
			case 8192: pComboxHor->SetCurSel(3); break;
			default: break;
			}
			Redraw();
			return;
		}
		switch(nindexHor) 
		{
			case 0:		m_iFullChannelWidth = 1024; break;
			case 1:		m_iFullChannelWidth = 2048; break;
			case 2:		m_iFullChannelWidth = 4096; break;
			case 3:		m_iFullChannelWidth = 8192; break;
			default: m_iFullChannelWidth = 1024;	break;
		}
		g_HardWare_Pare.iChannelWidth = m_iFullChannelWidth;

		m_iSmallWinCenPoint = m_iFullChannelWidth/2;
	    m_iSmallWinCHL = m_iSmallWinCenPoint - m_iSamllWinWidth/2 ;
		m_iSmallWinCHR = m_iSmallWinCenPoint + m_iSamllWinWidth/2 ;

		OnExpramViewsel();
		OnFullramViewsel();
		Redraw();
		return;	
  }
  else
  {
		switch(nindexHor) 
		{
			case 0:		m_iBuffFullChannelWidth = 1024; break;
			case 1:		m_iBuffFullChannelWidth = 2048; break;
			case 2:		m_iBuffFullChannelWidth= 4096; break;
			case 3:		m_iBuffFullChannelWidth = 8192; break;
			default: m_iBuffFullChannelWidth = 1024;	break;
		}

		m_iBuffSmallWinCenPoint = m_iBuffFullChannelWidth/2;
		m_iBuffSmallWinCHL = m_iBuffSmallWinCenPoint - m_iBuffSmallWindowWidth/2 ;
		m_iBuffSmallWinCHR = m_iBuffSmallWinCenPoint + m_iBuffSmallWindowWidth/2 ;	
		OnExpramViewsel();
        OnFullramViewsel();
		Redraw();
		return;	
  }
}
void CMCBView::OnSelchangeComboVer() 
{
	CMainFrame* pMainFrame=	(CMainFrame*)GetParentFrame();
	CComboBox* pComboxVer = (CComboBox *)(pMainFrame->m_wndDlgBar.GetDlgItem(IDC_COMBO_VER));
	int index=pComboxVer->GetCurSel();
 if (m_bMCBModeSel)
  {				
//		m_iFullRamCheckSel= pComboxVer->GetCurSel();
//		pComboxVer->SetCurSel(m_iFullRamCheckSel);
//		m_iFullRamEthValue = 64;
//		for(int i=0;i< (17 - m_iFullRamCheckSel);i++) {
//			m_iFullRamEthValue *= 2;}
        /////////////////////changing
		int kkk1=64;
		for(int j=0;j<17-index;j++)
		{
			kkk1 *= 2;
		}
		m_iFullRamEthValue = kkk1;
		m_iFullRamCheckSel =index;
//		}		
	
/*	else 
	{
//		int nindex = pComboxVer->GetCurSel();///////////////////changing
		int kkk=64;
		for(int i=0;i<17-index;i++)
		{
			kkk *= 2;
		}
		m_iExpRamEthValue = kkk;
		m_iBuffComboxSel =index;
	}	
	Redraw();
	*/
 }
 else
 {				
		int kkk1=64;
		for(int j=0;j<17-index;j++)
		{
			kkk1 *= 2;
		}
		m_iBuffFullRamEthValue = kkk1;
		m_iBuffFullRamCheckSel =index;	
	
/*	else 
	{
		int kkk=64;
		for(int i=0;i<17-index;i++)
		{
			kkk *= 2;
		}
		m_iBuffExpRamEthValue = kkk;
		m_iBuffExpRamCheckSel =index;
	}*/	
	Redraw();
	
 }

}


void CMCBView::OnMenuFileOpen() 
{
	CMainFrame* pMainFrame = (CMainFrame*)GetParentFrame();

	CEdit* pShowRealTime = (CEdit*)(pMainFrame->m_wndDlgBar.GetDlgItem(IDC_SHOW_REAL_TIME));
	CEdit* pShowLiveTime = (CEdit*)(pMainFrame->m_wndDlgBar.GetDlgItem(IDC_SHOW_LIVE_TIME));
	CEdit* pShowRatioTime = (CEdit*)(pMainFrame->m_wndDlgBar.GetDlgItem(IDC_SHOW_RATIO_TIME));
	CComboBox* pComboxHor = (CComboBox *)(pMainFrame->m_wndDlgBar.GetDlgItem(IDC_COMBO_HOR));
	CButton* pMCBSel = (CButton *)(pMainFrame->m_wndDlgBar.GetDlgItem(IDC_MCB_MODSEL));
	CButton* pBufSel = (CButton *)(pMainFrame->m_wndDlgBar.GetDlgItem(IDC_BUFF_MODSEL));
	CButton* pRealTimeSel = (CButton *)(pMainFrame->m_wndDlgBar.GetDlgItem(IDC_REALTIME_SEL));
	CButton* pLiveTimeSel = (CButton *)(pMainFrame->m_wndDlgBar.GetDlgItem(IDC_LIVETIME_SEL));
/*	if(m_bMCBModeSel) 
	{
		OnBuffModsel();                    //////////////////////////
		pMCBSel->SetCheck(0);
		pBufSel->SetCheck(1);
	}
*/	
	m_iTimeMcbSourceflag=0;
	m_iTimeFileSourceflag=1;
	unsigned char chartemp[128];
	UINT livetime;
	UINT realtime;
	float flivetime;
	float frealtime;
	float irotio;

	CString FilePathName; 
	CString FileTitle;
	m_fMenuEnergeScale_U = 0;
	m_fMenuEnergeScale_K = 0;
    m_fMenuEnergeScale_b = 0;
	
	CFileDialog dlg(TRUE,"mca","*.mca",OFN_NOCHANGEDIR);///TRUEΪOPEN�Ի���FALSEΪSAVE AS�Ի���E

	dlg.m_ofn.lpstrFilter = ("�������(*.mca)\0*.mca");//
	if(dlg.DoModal()==IDOK) 
	{
		FilePathName=dlg.GetPathName(); 
		FileTitle = dlg.GetFileName();
        if(m_bMCBModeSel) 
		{
			OnBuffModsel();	
		    pMCBSel->SetCheck(0);
		    pBufSel->SetCheck(1);
		}
	}
	else return;

	CFile file;
	file.Open(FilePathName,CFile::modeRead);
	memset(m_arrBUFFExternalRam,0,1024*4*8);
	file.Read(&FileHeader,sizeof(FileHeader));

	CString strout = "�ļ�����" + FileTitle;
	CString strbuff;	
	UINT time;
	time = FileHeader.timeH;		
	time = (time << 16) + FileHeader.timeL;
	m_iSaveFileTime = time;
	SecondToDate(&date,&time);


	int kk = FileHeader.unittype;
	strbuff.Format("    ʱ�䣺%dāE.2d��%.2d�� %.2d��E.2d��%.2dÁE,
		date.year,date.month,date.day,date.hour,date.minite,date.second);	
	

	livetime = FileHeader.livetimeH;
	livetime = (livetime << 16) + FileHeader.livetimeL;
	m_iSaveFileLiveTime = livetime;
	flivetime = livetime /100;
	realtime = FileHeader.realtimeH;
	realtime = (realtime << 16) + FileHeader.realtimeL;
	m_iSaveFileRealTime = realtime;
	frealtime = realtime /100;

	if (realtime <= 0) { realtime = 1; }
	irotio = 100*(float)(realtime - livetime)/realtime;

	m_strStateLine2Text = strout + strbuff;
	CString s;	
	s.Format("%s","���ļ�: ");
	s=s+m_strStateLine2Text;
	TextScroll(s);
	m_strStateLine4Text = m_strStateLine2Text;
    
    m_fMenuEnergeScale_U = FileHeader.unittype;

    if(FileHeader.unittype != 0) m_bMenuEnergeScale_CalcuEn=TRUE;
    else m_bMenuEnergeScale_CalcuEn=FALSE;


	if(FileHeader.unittype != 0) 
	{ //ֻ���ļ��п̶�ʱ�Ŷ�ȡϵ����
		m_fMenuEnergeScale_K = FileHeader.c1;
		m_fMenuEnergeScale_b = FileHeader.c0;	
//		g_HardWare_Pare.fEngerScaleSlope = m_fMenuEnergeScale_K;  ////changing
//		g_HardWare_Pare.fEngerScaleOffset = m_fMenuEnergeScale_b;		
	}
  
	//�����̶ȵĵ�λ�ж�
	if ((FileHeader.unittype <= 3)&&(FileHeader.unittype >= 0)) 
	{
//		g_HardWare_Pare.iEngerScaleUnit = FileHeader.unittype;    ////changing
	}
//	else g_HardWare_Pare.iEngerScaleUnit = 0;                     ///changing

	switch(m_fMenuEnergeScale_U)
	{
	case 0:	
		strout.Format("�����޿̶� ϵ��k:%.3f ƫ��b:%.3f ʵʱ:%.2fs ���:%.2fs ����:%.2f%%",m_fMenuEnergeScale_K,m_fMenuEnergeScale_b,
			frealtime,flivetime,irotio);
		break;
	case 1: 			
		strout.Format("������λ:eV ϵ��k:%.3f ƫ��b:%.3f ʵʱ:%.2fs ���:%.2fs ����:%.2f%%",m_fMenuEnergeScale_K,m_fMenuEnergeScale_b,
			frealtime,flivetime,irotio);
		break;
	case 2:
		strout.Format("������λ:KeV ϵ��k:%.3f ƫ��b:%.3f ʵʱ:%.2fs ���:%.2fs ����:%.2f%%",m_fMenuEnergeScale_K,m_fMenuEnergeScale_b,
			frealtime,flivetime,irotio);
		break;
	case 3:
		strout.Format("������λ:MeV ϵ��k:%.3f ƫ��b:%.3f ʵʱ:%.2fs ���:%.2fs ����:%.2f%%",m_fMenuEnergeScale_K,m_fMenuEnergeScale_b,
			frealtime,flivetime,irotio);
		break;
	default:  
		strout.Format("�����޿̶�, ϵ��k:%.3f ƫ��b:%.3f ʵʱ:%.2fs ���:%.2fs ����:%.2f%%",m_fMenuEnergeScale_K,m_fMenuEnergeScale_b,
			frealtime,flivetime,irotio,FileHeader.chn_number);
		break;
	}

	if (m_fMenuEnergeScale_U>0)m_bMenuEnergeScale_CalcuEn = TRUE;
	m_strStateLine3Text.Format("%s","         ");
	m_strStateLine3Text	= m_strStateLine3Text+strout;
    TextScroll(m_strStateLine3Text);

//	file.Read(chartemp,128);// 128 �ļ�ͷ
//	if((chartemp[0] != 0x10)&&(chartemp[1] != 0x10)) {
//		AfxMessageBox("�򿪵��ļ����Ͳ��ԣ�ǁE�ʵ");
//		return;
//	}

	int RdLenth = 1024;
	switch(FileHeader.chn_number) 
	{
	case 1024: RdLenth = 1024; break;
	case 2048: RdLenth = 2048; break;
	case 4096: RdLenth = 4096; break;
	case 8192: RdLenth = 8192; break;
	default: 
		file.Close();
		AfxMessageBox("�ļ��ĸ�ʽ���ԣ�ǁE�ʵԭ�ļ���\r\n������֧����ЁE192������",MB_ICONINFORMATION);		
		return;
	}	

	file.Seek(128,CFile::begin);
	for(int i=0;i<RdLenth;i++) {		//m_iFullChannelWidth�� ����
		file.Read(chartemp,4);
		m_arrBUFFExternalRam[i] = chartemp[3];
		m_arrBUFFExternalRam[i] <<= 8;
		m_arrBUFFExternalRam[i] += chartemp[2];
		m_arrBUFFExternalRam[i] <<= 8;
		m_arrBUFFExternalRam[i] += chartemp[1];  
		m_arrBUFFExternalRam[i] <<= 8;
		m_arrBUFFExternalRam[i] += chartemp[0];
		
	}
	file.Close();

    m_iBuffFullChannelWidth=FileHeader.chn_number;
	memcpy(m_arrExternalRam,m_arrBUFFExternalRam,1024*4*8);

//	m_iFullChannelWidth = FileHeader.chn_number;
    
//	g_HardWare_Pare.iChannelWidth = FileHeader.chn_number;	
	
//	m_iSmallWinCenPoint = m_iFullChannelWidth/2;
//	m_iSmallWinCHL = m_iSmallWinCenPoint - m_iSamllWinWidth/2 ;
//	m_iSmallWinCHR = m_iSmallWinCenPoint + m_iSamllWinWidth/2 ;	
    m_iBuffSmallWinCenPoint=m_iBuffFullChannelWidth/2;
	m_iBuffSmallWinCHL = m_iBuffSmallWinCenPoint - m_iBuffSmallWindowWidth/2 ;
	m_iBuffSmallWinCHR = m_iBuffSmallWinCenPoint + m_iBuffSmallWindowWidth/2 ;	

	
		switch(RdLenth)
		{
		case 1024: pComboxHor->SetCurSel(0); break;
		case 2048: pComboxHor->SetCurSel(1); break;
		case 4096: pComboxHor->SetCurSel(2); break;
		case 8192: pComboxHor->SetCurSel(3); break;
		default: pComboxHor->SetCurSel(0); break;
		}

	//����ת��
	int kkkk = 2*m_iBuffFullChannelWidth/1024;
	UINT max = 0;
	for(i=0;i<512;i++) {
		max = 0;
		for(int j=i*kkkk;j<(i+1)*kkkk;j++) { //ѡ��kk ���ȷ�Χ�������һ������ֵ
			if (m_arrExternalRam[j] > max) {
				max = m_arrExternalRam[j];
			}
		}
		m_arrFullRam[i] = max;
	}
	
	UINT buffvalue = 64;	
	for(i=0;i<512;i++)
	{
		buffvalue = max((m_arrFullRam[i] & 0x7fffffff),buffvalue);
	}

	i=0;
	m_iBuffFullRamEthValue = 64;
	while(i<17){
		if(buffvalue < (UINT)m_iBuffFullRamEthValue) break;
		i++;
		m_iBuffFullRamEthValue *= 2;
	}
	m_iBuffFullRamCheckSel = 17-i;

	OnExternal2ExpRam();

	buffvalue = 64;
	for(i=0;i<=m_iBuffSmallWindowWidth;i++) 
	{
		buffvalue = max((m_arrExpRam[i]& 0x7fffffff),buffvalue);
	}
	i=0;
	m_iBuffExpRamEthValue = 64;
	while(i<17){
		if(buffvalue < (UINT)m_iBuffExpRamEthValue) break;
		i++;
		m_iBuffExpRamEthValue *= 2;
	}
	m_iBuffExpRamCheckSel = 17-i;	

	//������ֵ��ʾ����Ϊ ������ʽlog
//	CMainFrame* pMainFrame = (CMainFrame*)GetParentFrame();
	CComboBox* pComboxVer = (CComboBox *)(pMainFrame->m_wndDlgBar.GetDlgItem(IDC_COMBO_VER));
	pComboxVer->ResetContent();
	pComboxVer->AddString("Log");
	pComboxVer->SetCurSel(0);
	m_bDrawLogData_Sel=TRUE;
	CComboBox* pComboxExpVer = (CComboBox *)(pMainFrame->m_wndDlgBar.GetDlgItem(IDC_COMBO_EXP_VER));
	pComboxExpVer->ResetContent();
	pComboxExpVer->AddString("Log");
	pComboxExpVer->SetCurSel(0);
	OnExpramViewsel();
	Redraw();

	char chrtext[20];
	m_iBuffRealTimeValuex=frealtime;
	m_iBuffLiveTimeValuex=flivetime;
	sprintf(chrtext,"ʵʱ: %.2fs",frealtime);
	m_iBuffRealTimeValue.Format("%s",chrtext);

	sprintf(chrtext,"���: %.2fs",flivetime);
	m_iBuffLiveTimeValue.Format("%s",chrtext);

	sprintf(chrtext,"����: %.2f %%",irotio);
	m_iBuffTimeRatio.Format("%s",chrtext);

//	CEdit *pSource=(CEdit*)(pMainFrame->m_wndDlgBar.GetDlgItem(IDC_SOURCE));
//	FileTitle = "����:�ļ�";
//	pSource->SetWindowText(FileTitle);
 
   if (!m_bMCBModeSel)
   {
	if (FileHeader.acq_mode==0x100)
	{
           pRealTimeSel->SetCheck(1);
		   pLiveTimeSel->SetCheck(0);
	}
	if (FileHeader.acq_mode==0x101)
	{
           pRealTimeSel->SetCheck(0);
		   pLiveTimeSel->SetCheck(1);        
	}
   }
}

void CMCBView::OnMenuFileSaveAs() 
{
	CString FilePathName; 
	CString FileName;
	CString strout = "�ļ�ÁE ";
	CFileDialog dlg(FALSE,"mca","*.mca",OFN_NOCHANGEDIR);///TRUEΪOPEN�Ի���FALSEΪSAVE AS�Ի���E

	dlg.m_ofn.lpstrFilter = ("�������.mca");//
	if(dlg.DoModal()==IDOK) {
		FilePathName=dlg.GetPathName(); 
		FileName = dlg.GetFileName();
	}
	else return;

	strout += FileName;

	CFile savefile;
	if( !savefile.Open( FilePathName, CFile::modeCreate | CFile::modeWrite) )
	{
		AfxMessageBox("�ļ�дȁE��E);
		return;
	}

	savefile.SetLength(128+1024*4*8); //Ŀǰ�ļ���СΪ 128+1024*4

	FileHeader.readout_type = 2;
	FileHeader.mac_number = 1;
	FileHeader.region = 0;
	FileHeader.tag_nL = 0;
	FileHeader.tag_nH = 0;
	FileHeader.acq_mode = 0x0100;
	FileHeader.timeL = 0x3838;
	FileHeader.timeH = 0x3838;
	FileHeader.livetimeL = 0x4848;
	FileHeader.livetimeH = 0x4848;	
	FileHeader.realtimeL = 0x5252;	
	FileHeader.realtimeH = 0x5252;
	FileHeader.c2 = 0;
	FileHeader.c1 = 1.00;
	FileHeader.c0 = 2.00;
	FileHeader.unittype = 0;
	FileHeader.format = 0;
	FileHeader.order = 1;
	FileHeader.spm_num = 1;
	FileHeader.naa_flag = 27575;	
	FileHeader.chn_number = m_iBuffFullChannelWidth;
    
	if (g_HardWare_Pare.bSampleModeRealTime) FileHeader.acq_mode = 0x0100;
	else FileHeader.acq_mode = 0x0101;
    
//�����̶����ݱ���ͬ��
	FileHeader.unittype =(char) m_fMenuEnergeScale_U;
//	FileHeader.c1 = g_HardWare_Pare.fEngerScaleSlope;
//	FileHeader.c0 = g_HardWare_Pare.fEngerScaleOffset;
	FileHeader.c1 = m_fMenuEnergeScale_K;
	FileHeader.c0 = m_fMenuEnergeScale_b;    
    
	//ʵʱ������ݱ���E
	FileHeader.livetimeL = m_iSaveFileLiveTime;
	FileHeader.livetimeH = (m_iSaveFileLiveTime >> 16);

	FileHeader.realtimeL = m_iSaveFileRealTime;
	FileHeader.realtimeH = (m_iSaveFileRealTime >> 16);

	//������ϵͳʱ�䱣��E
	FileHeader.timeL = m_iSaveFileTime;
	FileHeader.timeH = (m_iSaveFileTime>>16);

	savefile.Seek(0,CFile::begin);
	savefile.Write(&FileHeader,sizeof(FileHeader));

	CString strbuff,strUnit;
	SecondToDate(&date,&m_iSaveFileTime);	
	int kk = FileHeader.unittype;
	strbuff.Format("     ʱ�䣺%dāE.2d��%.2d�� %.2d��E%.2d�� %.2dÁE,date.year,date.month,date.day,date.hour,date.minite,date.second);	
	
	strout += strbuff;
    m_strStateLine2Text.Format("%s","�����ļ�: ");
	m_strStateLine2Text = m_strStateLine2Text+strout;	
    TextScroll(m_strStateLine2Text);

	switch(FileHeader.unittype) 
	{

	case 0:  strUnit.Format("����δ�̶� ");
		break;
	case 1:  strUnit.Format("�����̶ȵ�λ: eV ");
		break;
	case 2:	 strUnit.Format("�����̶ȵ�λ: KeV ");
		break;
	case 3:	 strUnit.Format("�����̶ȵ�λ: MeV ");
		break;
	default: strUnit.Format("����δ�̶� "); 
		break;

	}
	
	if (m_iSaveFileRealTime == 0) {		
		strout.Format("ϵ��k:%.2f ƫ��b:%.2f ʵʱ:0.00s ���:%.2fs ����:100%%",m_fMenuEnergeScale_K,m_fMenuEnergeScale_b,
				m_iSaveFileLiveTime/100.);
	}
	else {
		float irotio = (float)(m_iSaveFileRealTime - m_iSaveFileLiveTime)/(float)m_iSaveFileRealTime;
		strout.Format("ϵ��k:%.2f ƫ��b:%.2f ʵʱ:%.2fs ���:%.2fs ����:%.2f%%",m_fMenuEnergeScale_K,m_fMenuEnergeScale_b,
			m_iSaveFileRealTime/100.,m_iSaveFileLiveTime/100.,100*irotio);
	}
    m_strStateLine3Text.Format("%s","                 ");
	m_strStateLine3Text = m_strStateLine3Text+strUnit + strout;
	TextScroll(m_strStateLine3Text);
    
	savefile.Seek(128,CFile::begin);
	for(int i=0;i<m_iBuffFullChannelWidth;i++) 
	{
		type.intdata = m_arrExternalRam[i];
		savefile.Write(type.uchdata,4);		
	}

/*	savefile.Seek(1024+128,CFile::begin);
	for(i=1;i<=9;i++) {
		type.intdata = 100*i;
		savefile.Write(type.uchdata,4);		
	}
	for(i=0;i<17;i++) {
		type.intdata = 50*(17-i);
		savefile.Write(type.uchdata,4);		
	}
*/	
	savefile.Close();

	Redraw();	
}

void CMCBView::OnExternal2ExpRam()
{
  int i;
  memset(m_arrExpRam,0,1024*4);
  if (m_bMCBModeSel)
  {
	if((m_iSmallWinCHL>0)&&(m_iSmallWinCHR < m_iFullChannelWidth-1))
	{
		for(i=m_iSmallWinCHL;i<=m_iSmallWinCHR;i++) 
		{
			m_arrExpRam[i-m_iSmallWinCHL] = m_arrExternalRam[i];
		}
	}
	if(m_iSmallWinCHL == 0)
	{
		memset(m_arrExpRam,0,(m_iSamllWinWidth-m_iSmallWinCHR)*4);
		for(i=0;i<m_iSmallWinCHR;i++) 
		{
			m_arrExpRam[i+(m_iSamllWinWidth-m_iSmallWinCHR)] = m_arrExternalRam[i];
		}

	}
	if(m_iSmallWinCHR >= m_iFullChannelWidth-1) 
	{
		for(i=m_iSmallWinCHL;i<m_iFullChannelWidth;i++) 
		{
			m_arrExpRam[i-m_iSmallWinCHL] = m_arrExternalRam[i];
		}
		for(i=m_iFullChannelWidth-m_iSmallWinCHL;i<m_iSamllWinWidth;i++)
		{
			m_arrExpRam[i] = 0;
		}
	}
  }
  else
  {
	if((m_iBuffSmallWinCHL>0)&&(m_iBuffSmallWinCHR < m_iBuffFullChannelWidth-1))
	{
		for(i=m_iBuffSmallWinCHL;i<=m_iBuffSmallWinCHR;i++) 
		{
			m_arrExpRam[i-m_iBuffSmallWinCHL] = m_arrExternalRam[i];
		}
	}
	if(m_iBuffSmallWinCHL == 0)
	{
		memset(m_arrExpRam,0,(m_iBuffSmallWindowWidth-m_iBuffSmallWinCHR)*4);
		for(i=0;i<m_iBuffSmallWinCHR;i++) 
		{
			m_arrExpRam[i+(m_iBuffSmallWindowWidth-m_iBuffSmallWinCHR)] = m_arrExternalRam[i];
		}

	}
	if(m_iBuffSmallWinCHR >= m_iBuffFullChannelWidth-1) 
	{
		for(i=m_iBuffSmallWinCHL;i<m_iBuffFullChannelWidth;i++) 
		{
			m_arrExpRam[i-m_iBuffSmallWinCHL] = m_arrExternalRam[i];
		}
		for(i=m_iBuffFullChannelWidth-m_iBuffSmallWinCHL;i<m_iBuffSmallWindowWidth;i++)
		{
			m_arrExpRam[i] = 0;
		}
	}
  }

}

void CMCBView::OnExpRamModify()
{
	int arrExpRam[1024];  

	for(int i=m_iSmallWinCHL;i<=m_iSmallWinCHR;i++) 
	{
	//	m_arrExpRam[i-m_iSmallWinCHL] = ((m_arrBUFFExternalRam[i] & 0x80000000) | m_arrExpRam[i-m_iSmallWinCHL] );//////////////////may problem
        m_arrExpRam[i-m_iSmallWinCHL] = (m_arrExpRam[i-m_iSmallWinCHL]);
	}

	memcpy(arrExpRam,m_arrExpRam,1024*4);

	if(m_iSmallWinCHL == 0) {
		memset(m_arrExpRam,0,(m_iSamllWinWidth-m_iSmallWinCHR)*4);
		for(i=0;i<m_iSmallWinCHR;i++) {
			m_arrExpRam[i+(m_iSamllWinWidth-m_iSmallWinCHR)] = arrExpRam[i];
		}

	}
	if(m_iSmallWinCHR >= m_iFullChannelWidth-1) {
		for(i=m_iSmallWinCHL;i<m_iFullChannelWidth;i++) {
			m_arrExpRam[i-m_iSmallWinCHL] = arrExpRam[i-m_iSmallWinCHL];
		}
		for(i=m_iFullChannelWidth-m_iSmallWinCHL;i<m_iSamllWinWidth;i++) {
			m_arrExpRam[i] = 0;
		}
	}

}



void CMCBView::OnRealtimeSel() 
{
	m_bSetSampleTimeRealMode = TRUE;
	g_HardWare_Pare.bSampleModeRealTime = TRUE;
	
}

void CMCBView::OnLivetimeSel() 
{
	m_bSetSampleTimeRealMode = FALSE;
	g_HardWare_Pare.bSampleModeRealTime = FALSE;
}


BOOL CMCBView::DestroyWindow() 
{
	SetEvent(g_hEvtSend);
	
	SetEvent(g_hExitMonitThread);
	SetEvent(g_hExitSendThread);	

	g_MCB_Oper.bQueryEn = false;
	g_MCB_Oper.bStopCMDEn = false;

	g_bRunMonitorThread = false;
	g_bRunSendThread =  false;

	CloseShmMaint232(g_hComm);
	g_hComm = INVALID_HANDLE_VALUE;

	Sleep(1500);
	delete g_pThread_Monitor; //����߳�ָՁE
	delete g_pThread_Send;
	
	return CView::DestroyWindow();
}

void CMCBView::OnDestroy() 
{
	SetEvent(g_hEvtSend);

	SetEvent(g_hExitMonitThread);
	SetEvent(g_hExitSendThread);

	g_MCB_Oper.bQueryEn = false;
	g_MCB_Oper.bStopCMDEn = false;

	g_bRunMonitorThread = false;
	g_bRunSendThread =  false;

//	Sleep(1500);

	if (!g_bComOpen) {
		CView::OnDestroy();
        ResumeW();
		return;
	}

	if( WaitForSingleObject( g_pThread_Monitor->m_hThread,  3000) == WAIT_TIMEOUT ) {
		TerminateThread( g_pThread_Monitor->m_hThread, -1);		
	}
	else delete g_pThread_Monitor;

	if( WaitForSingleObject( g_pThread_Send->m_hThread,  3000) == WAIT_TIMEOUT ) {
		TerminateThread( g_pThread_Send->m_hThread, -1);		
	}
	else delete g_pThread_Send;

	CloseShmMaint232(g_hComm);
	g_hComm = INVALID_HANDLE_VALUE;

	if (g_hUsb != INVALID_HANDLE_VALUE) {
		CloseUsbDevice(g_hUsb);
		g_hUsb = INVALID_HANDLE_VALUE;
	}
	CView::OnDestroy();
}

void CMCBView::OnCancelMode() 
{
	CView::OnCancelMode();
	
}

void CMCBView::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	CMainFrame* pMainFrame = (CMainFrame*)GetParentFrame();
	CEdit* pQueryTimeout=(CEdit*)(pMainFrame->m_wndDlgBar.GetDlgItem(IDC_QUERY_TIMEOUT));
	CEdit* pQuerySampleState=(CEdit*)(pMainFrame->m_wndDlgBar.GetDlgItem(IDC_QUERY_SAMPLE_STATE));
	CEdit* pShowstate = (CEdit*)(pMainFrame->m_wndDlgBar.GetDlgItem(IDC_STATE_SHOW));
	CEdit* pRealTime = (CEdit*)(pMainFrame->m_wndDlgBar.GetDlgItem(IDC_SHOW_REAL_TIME));
	CEdit* pLiveTime = (CEdit*)(pMainFrame->m_wndDlgBar.GetDlgItem(IDC_SHOW_LIVE_TIME));
	CEdit* pRatio = (CEdit*)(pMainFrame->m_wndDlgBar.GetDlgItem(IDC_SHOW_RATIO_TIME));
	
	CTime  time = CTime::GetCurrentTime();
	CString strtime;
	char   chrBuff[20];

	//time.m_time

	switch(nIDEvent) {
		case 0: 
			m_bUsbOnTimeShow = TRUE;
			m_iUsbSampleTimeCnt ++;
			if(m_iUsbSampleTimeCnt >= g_HardWare_Pare.iSampleTimeValue*2){ //timer0��ʱΪ0.5ÁE
				OnMenuStopsample();	
			}
			if (g_HardWare_Pare.bInputAnolSignalSel == TRUE) { //
				sprintf(chrBuff,"ʵʱ: %ds",m_iUsbSampleTimeCnt/2);
				pRealTime->SetWindowText(chrBuff);
				sprintf(chrBuff,"���: %ds",m_iUsbSampleTimeCnt/2);
				pLiveTime->SetWindowText(chrBuff);
				sprintf(chrBuff,"����: 100%%");
				pRatio->SetWindowText(chrBuff);
				m_iSaveFileRealTime = m_iUsbSampleTimeCnt/2;
				m_iSaveFileLiveTime = m_iUsbSampleTimeCnt/2;
			}

			break;
		case 1:
			if (m_bMCBModeSel)
			{
				pQuerySampleState->SetWindowText("���ݻ�ȡ:   ");
				pQueryTimeout->SetWindowText("��ѯ��ʱ:    ");
			}
			else
			{
				m_ScrollBar.ShowWindow(FALSE);
				m_ScrollBar.ShowWindow(TRUE);
			}
			break;
		case 3:
			strtime.Format("%dāE.2d��%.2d��\r\n   %.2d:%.2d:%.2d",
			time.GetYear(),time.GetMonth(),time.GetDay(),time.GetHour(),time.GetMinute(),time.GetSecond());
			pShowstate->SetWindowText(strtime);
			break;
		default:
			break;
	}
	

	CView::OnTimer(nIDEvent);
}


void CMCBView::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	CMainFrame* pMainFrame=
	      (CMainFrame*)GetParentFrame();

	CString strbuf;

		if(nChar == VK_LEFT) 
		{  		
			if (m_bMCBModeSel)
			{
				if((--m_iSmallWinCenPoint) <= 0) m_iSmallWinCenPoint = 0;
			}
			else
			{
				if ((m_iBuffSmallWinCenPoint--) < 1) m_iBuffSmallWinCenPoint = 0;
			}

		};
		if(nChar == VK_RIGHT)
		{  
		    if (m_bMCBModeSel)
		    {
				if((++m_iSmallWinCenPoint) >= m_iFullChannelWidth) m_iSmallWinCenPoint = m_iFullChannelWidth - 1;
		    }
			else
			{
				if((++m_iBuffSmallWinCenPoint) >= m_iBuffFullChannelWidth) m_iBuffSmallWinCenPoint = m_iBuffFullChannelWidth - 1;
			}
		};	
		if(nChar == VK_PRIOR) 
		{  // Page Up
			if (m_bMCBModeSel)
			{
			   m_iSmallWinCenPoint += 10;
			   if((m_iSmallWinCenPoint) >= m_iFullChannelWidth) m_iSmallWinCenPoint = m_iFullChannelWidth-1;
			   if (--m_iSmallWinCenPoint<0) m_iSmallWinCenPoint=0;
			}
			else
			{
			   if((m_iBuffSmallWinCenPoint>=0)&&(m_iBuffSmallWinCenPoint<m_iBuffFullChannelWidth))m_iBuffSmallWinCenPoint += 10;
			   if((m_iBuffSmallWinCenPoint) >= m_iBuffFullChannelWidth) m_iBuffSmallWinCenPoint = m_iBuffFullChannelWidth-1;
			   if (--m_iBuffSmallWinCenPoint<0) m_iBuffSmallWinCenPoint=0;
			}
		
		};	
		if(nChar == VK_NEXT) 
		{  // Page down
			if (m_bMCBModeSel)
			{	
				m_iSmallWinCenPoint -= 10;
			   if((m_iSmallWinCenPoint) <= 0) m_iSmallWinCenPoint = 0;
			}
			else
			{
			   if (m_iBuffSmallWinCenPoint>=10)m_iBuffSmallWinCenPoint -= 10;
			   if (m_iBuffSmallWinCenPoint<10) m_iBuffSmallWinCenPoint=0;
			}

		};	

		if(nChar == VK_HOME) 
		{   //home

			if(m_bMCBModeSel)m_iSmallWinCenPoint = 0;
			else m_iBuffSmallWinCenPoint = 0;
		};	
		if(nChar == VK_END)
		{   //end
		    if(m_bMCBModeSel)m_iSmallWinCenPoint = m_iFullChannelWidth-1;
			else m_iBuffSmallWinCenPoint = m_iBuffFullChannelWidth-1;
		};


		m_iSmallWinCHL = m_iSmallWinCenPoint - m_iSamllWinWidth/2;
		m_iSmallWinCHR = m_iSmallWinCenPoint + m_iSamllWinWidth/2;			
		if(m_iSmallWinCenPoint <= m_iSamllWinWidth/2) {
			m_iSmallWinCHL = 0;	
		};
		if(m_iSmallWinCenPoint >= m_iFullChannelWidth-1 - m_iSamllWinWidth/2) { m_iSmallWinCHR = m_iFullChannelWidth-1;	};

        m_iBuffSmallWinCHL = m_iBuffSmallWinCenPoint - m_iBuffSmallWindowWidth/2;
		m_iBuffSmallWinCHR = m_iBuffSmallWinCenPoint + m_iBuffSmallWindowWidth/2;			
		if(m_iBuffSmallWinCenPoint <= m_iBuffSmallWindowWidth/2) {
			m_iBuffSmallWinCHL = 0;	
		};
		if(m_iBuffSmallWinCenPoint >= m_iBuffFullChannelWidth-1 - m_iBuffSmallWindowWidth) { m_iBuffSmallWinCHR = m_iBuffFullChannelWidth-1;};

		//��ǰϵͳ���� MCB��ʽ���������ڲɼ�����ʱ����ʱ�轫FullRam������ת����ֵ�� ExpRam
		if (m_bMCBModeSel) OnFullRam2ExpRamInsertVal();
		Redraw();

	m_ScrollBar.ShowScrollBar(FALSE);
	m_ScrollBar.ShowScrollBar(TRUE);

		m_bExpRamLongNew_valid = true;
		m_ExpRamDataLong_new = (m_iSmallWinCHR - m_iSmallWinCHL +1)*3;
	
	CView::OnKeyUp(nChar, nRepCnt, nFlags);
}

void CMCBView::OnKillFocus(CWnd* pNewWnd) 
{


}

void CMCBView::OnExternaldataMcb2Buff() 
{
	CMainFrame* pMainFrame = (CMainFrame*)GetParentFrame();
	CButton* pMCBSel = (CButton *)(pMainFrame->m_wndDlgBar.GetDlgItem(IDC_MCB_MODSEL));
	CButton* pBUFSel = (CButton *)(pMainFrame->m_wndDlgBar.GetDlgItem(IDC_BUFF_MODSEL));
	CButton* pMCB_TO_BUF = (CButton *)(pMainFrame->m_wndDlgBar.GetDlgItem(IDC_EXTERNALDATA_MCB2BUFF));
    CEdit* pShowRealTime = (CEdit*)(pMainFrame->m_wndDlgBar.GetDlgItem(IDC_SHOW_REAL_TIME));
	CEdit* pShowLiveTime = (CEdit*)(pMainFrame->m_wndDlgBar.GetDlgItem(IDC_SHOW_LIVE_TIME));
	CEdit* pShowRatioTime = (CEdit*)(pMainFrame->m_wndDlgBar.GetDlgItem(IDC_SHOW_RATIO_TIME));
    CEdit* pTimeShow = (CEdit*)(pMainFrame->m_wndDlgBar.GetDlgItem(IDC_TIMER_EDIT));
  if (m_bMCBModeSel)
  {
	
	m_iTimeMcbSourceflag=1;
	m_iTimeFileSourceflag=0;
	pMCBSel->SetCheck(0);
	pBUFSel->SetCheck(1);
	for(int i=0;i<1024*8;i++)///////////////////////////////������
	{
        m_arrBUFFExternalRam[i] = m_arrMCBExternalRam[i];
	}
	memcpy(m_arrExternalRam,m_arrBUFFExternalRam,1024*4*8);

   /////////////////////////////////////////////////////////Full����������
    m_iBuffFullChannelWidth=m_iFullChannelWidth;
	m_iBuffFullRamCheckSel=m_iFullRamCheckSel;
    m_iBuffFullRamEthValue=m_iBuffFullRamEthValue;
	
   /////////////////////////////////////////////////////////Exp����������
    m_iBuffSmallWindowWidth=m_iSamllWinWidth;
    m_iBuffSmallWindowWidthSel=m_iSmallWindowWidthCheckSel;
    m_iBuffExpRamEthValue=m_iExpRamEthValue;
	m_iBuffExpRamCheckSel=m_iBuffComboxSel;	
   
   /////////////////////////////////////////////////////////mark��С��������
	m_iBuffSmallWinCenPoint=m_iSmallWinCenPoint;
    m_iBuffSmallWinCHL=m_iSmallWinCHL;
    m_iBuffSmallWinCHR=m_iSmallWinCHR;	
    
   /////////////////////////////////////////////////////////Time
/*	pShowRealTime->GetWindowText(m_iRealTimeValue);
	pShowLiveTime->GetWindowText(m_iLiveTimeValue);
	pShowRatioTime->GetWindowText(m_iTimeRatio);

	m_iBuffRealTimeValue=m_iRealTimeValue;
	m_iBuffLiveTimeValue=m_iLiveTimeValue;
	m_iBuffTimeRatio=m_iTimeRatio;
*/
   /////////////////////////////////////////////////////////Energe
	m_fMenuEnergeScale_K = g_HardWare_Pare.fEngerScaleSlope;
    m_fMenuEnergeScale_b = g_HardWare_Pare.fEngerScaleOffset;
	m_fMenuEnergeScale_U = g_HardWare_Pare.iEngerScaleUnit;
    m_bMenuEnergeScale_CalcuEn = m_bMcbMenuEnergeScale_CalcuEn;

	pMCB_TO_BUF->SetWindowText("MCB--->BUF");
//  CEdit *pSource=(CEdit*)(pMainFrame->m_wndDlgBar.GetDlgItem(IDC_SOURCE));
//	pSource->SetWindowText("����:MCB");

	g_MCB_Oper.iStartQueryNo = BUFF_START_QUERYNO;
	g_MCB_Oper.iCurQueryNo = BUFF_START_QUERYNO;	
	OnBuffModsel();
//    if (m_bExpRamViewSel) OnExpramViewsel();
//	else OnFullramViewsel();

	Redraw();
	
  }
  else
  {	
	m_iTimeMcbSourceflag=0;
	pMCBSel->SetCheck(1);
	pBUFSel->SetCheck(0);

//    pMCB_TO_BUF->SetWindowText("MCB<---BUF");

   /////////////////////////////////////////////////////////Full����������	
    m_iFullChannelWidth=m_iBuffFullChannelWidth;
	m_iFullRamCheckSel=m_iBuffFullRamCheckSel;
    m_iFullRamEthValue=(int)m_iBuffFullRamEthValue;

  /////////////////////////////////////////////////////////Exp����������
	m_iSamllWinWidth=(int)m_iBuffSmallWindowWidth;
	m_iBuffComboxSel=m_iBuffExpRamCheckSel;
    m_iExpRamEthValue=m_iBuffExpRamEthValue;
    m_iSmallWindowWidthCheckSel=m_iBuffSmallWindowWidthSel;

  /////////////////////////////////////////////////////////mark��С��������
	m_iSmallWinCenPoint=(int)m_iBuffSmallWinCenPoint;
    m_iSmallWinCHL=m_iBuffSmallWinCHL;
    m_iSmallWinCHR=m_iBuffSmallWinCHR;	
  ////////////////////////////////////////////////////////Time
	m_iRealTimeValue=m_iBuffRealTimeValue;
	m_iLiveTimeValue=m_iBuffLiveTimeValue;
	m_iTimeRatio=m_iBuffTimeRatio;
  /////////////////////////////////////////////////////////Energe
	g_HardWare_Pare.fEngerScaleSlope = m_fMenuEnergeScale_K;
    g_HardWare_Pare.fEngerScaleOffset = m_fMenuEnergeScale_b;
    g_HardWare_Pare.iEngerScaleUnit =m_fMenuEnergeScale_U;   
	m_bMcbMenuEnergeScale_CalcuEn = m_bMenuEnergeScale_CalcuEn;

//  CEdit *pSource=(CEdit*)(pMainFrame->m_wndDlgBar.GetDlgItem(IDC_SOURCE));
//	pSource->SetWindowText("�����ļ�");

	g_MCB_Oper.iStartQueryNo = BUFF_START_QUERYNO;
	g_MCB_Oper.iCurQueryNo = BUFF_START_QUERYNO;
	
	OnMcbModsel();
//	if (m_bExpRamViewSel) OnExpramViewsel();
//	else OnFullramViewsel();
    
    g_HardWare_Pare.iChannelWidth=m_iBuffFullChannelWidth;
    
	for(int i=0;i<1024*8;i++) 
	{
		m_arrMCBExternalRam[i] = m_arrBUFFExternalRam[i];
		
	}
	memcpy(m_arrExternalRam,m_arrMCBExternalRam,1024*4*8);

	
	m_strStateLine2Text=" ";
    m_strStateLine3Text=" ";
	
	Redraw();

	pShowRealTime->SetWindowText(m_iRealTimeValue);
	pShowLiveTime->SetWindowText(m_iLiveTimeValue);
	pShowRatioTime->SetWindowText(m_iTimeRatio);
	
    CString ss;
	if(FileHeader.acq_mode==0x100)ss.Format("%d",m_iBuffRealTimeValuex);
	if(FileHeader.acq_mode==0x101)ss.Format("%d",m_iBuffLiveTimeValuex);
    pTimeShow->SetWindowText(ss);
	
  }
}

/*void CMCBView::OnProgctrl() 
{
	//����Ϊģ��������� ���֣������Ѿ����Σ���ΪMCB��������
	//	CProgCtrl  progcontrl;
	//	memcpy(progcontrl.iProgCtrlData,m_iProgCtrlData,10*4);
	//	if(IDOK != progcontrl.DoModal()) {
	//		return;
	//	}
	//	memcpy(m_iProgCtrlData,progcontrl.iProgCtrlData,10*4);
	//	if(!g_bComOpen) {
	//		AfxMessageBox("����û�д� \nǁEȴ򿪴��ڣ�",MB_ICONINFORMATION);
	//		return;
	//	}
	//	if(g_bExec){
	//		AfxMessageBox("���ڵȴ���һ��ÁE���Ӧ��ǁE�һ�������",MB_ICONINFORMATION);
	//		return;
	//	}	
	//	//���ջ�������E
	//	memset(m_recbuff,0,1024*4*8);
	//	memset(m_secrecbuf,0,1024*4*8);
	//	m_precbuf = m_recbuff;
	//	m_iworkcounter = 0;
	//
	//	g_ThreadCMD = CTL_PROG_CONFIG_ANOLPARA;
	//	SetEvent(g_hEvtSend);
		

	//MCB �������öԻ���E
	OnHardwareParaSet();

}
*/



void CMCBView::OnRButtonDown(UINT nFlags, CPoint point) 
{

	CPoint ScreenPoint;   //������������Ļ�ϵ�λ��,������λ����ָ�û������λ��
    ScreenPoint = point;
    ClientToScreen(&ScreenPoint);

	UINT CommandId = 0;
    CMenu MyContextMenu;

	MyContextMenu.CreatePopupMenu();

	if (m_bMCBModeSel) { //��MCBģʽ�£����ܽ���������ݷ�΁E
		MyContextMenu.AppendMenu(MF_STRING, ++CommandId, "ѡ��BUFFģʽ, �������ݷ�΁E);  //1		
	}
	else
	{
		MyContextMenu.AppendMenu(MF_STRING, ++CommandId, "���ø���ȤǁE);  //1
		MyContextMenu.AppendMenu(MF_STRING, ++CommandId, "�������ȤǁE);  //2
		MyContextMenu.AppendMenu(MF_STRING, ID_SEPARATOR); //to get separator line
		MyContextMenu.AppendMenu(MF_STRING, ++CommandId, "����Ȥ�����");   //3
		MyContextMenu.AppendMenu(MF_STRING, ++CommandId, "ȫ��ƽ��");       //4
		MyContextMenu.AppendMenu(MF_STRING, ++CommandId, "���������");     //5
		MyContextMenu.AppendMenu(MF_STRING, ID_SEPARATOR); //to get separator line
		MyContextMenu.AppendMenu(MF_STRING, ++CommandId, "�������ͳ��");     //6
		MyContextMenu.AppendMenu(MF_STRING, ++CommandId, "�����̶�");     //7

		MyContextMenu.AppendMenu(MF_STRING, ID_SEPARATOR); //to get separator line
		MyContextMenu.AppendMenu(MF_STRING, ++CommandId, "�����䳁E);		//8
	}

    //ָ���˵�������.
    UINT Command = (UINT)MyContextMenu.TrackPopupMenu(TPM_LEFTALIGN|TPM_NONOTIFY | TPM_RETURNCMD,
		ScreenPoint.x, ScreenPoint.y, this);
    //ʹ��TPM_NONOTIFY | TPM_RETURNCMD���ر�ѡ��Ĳ˵�ÁE�E0����߻��(��ȁE
    //����˵���ÁE
    //����㲻��ȁE���������,��ĸ����ڽ������ѡ��ÁE����յ�WM_COMMAND��Ϣ

	MyContextMenu.DestroyMenu();
    //switch is only necessary if you use TPM_NONOTIFY | TPM_RETURNCMD !
    //switchֻ����ʹ����������ʱ���б�Ҫ.

	switch(Command)
    {
		case 0:		
			//û��ÁE�E���Բ������κ���ǁE			
			break;
		case 1:	//��ʾ64������

			//��MCBģʽ�£����ܽ���������ݷ�΁E
			if (m_bMCBModeSel)
			{
				OnHandleSelectBuffmode();
			}
			else
			{
				OnMouseSetInterestRange();
			}			
			break;
		case 2:	//������ʾ	
			OnMouseClearInterestRange();
			break;
		case 3:	
			OnMouseCalulateSum();
			break;
		case 4:			
			OnMouseWholeDataFilter();	
			break;
		case 5:
			OnMouseCalulateWholeSum();
			break;
		case 6:
			//OnMenuSelectWidthStat();
			OnIdMenuWidthStat();
			break;
		case 7:
			OnMenuEnergeKedu();
			//OnMenuSelectEnergeScale();
			break;
		case 8:
			OnMouseReport();
			break;
		default:		
			//do default	AfxMessageBox("Invalide Selected");			
			break;
	}	
	CView::OnRButtonDown(nFlags, point);
}

void CMCBView::OnMenuSetCommport() 
{
	CMainFrame* pMainFrame = (CMainFrame*)GetParentFrame();
	CEdit* pQueryTimeout=(CEdit*)(pMainFrame->m_wndDlgBar.GetDlgItem(IDC_QUERY_TIMEOUT));
	CEdit* pQuerySampleState=(CEdit*)(pMainFrame->m_wndDlgBar.GetDlgItem(IDC_QUERY_SAMPLE_STATE));
	
	CComSel comsel;
    CString strtime;

	if (g_hUsb != INVALID_HANDLE_VALUE) {
//		AfxMessageBox("USB��ȡ�߳����ڹ��������ܽ���ͨ�Ŷ˿�ѡԁE\r\nǁEȵ���� ֹͣ��,Ȼ�����ԡ�");
		strtime.Format("%s","USB��ȡ�߳����ڹ�ׁE���ܽ���ͨ�Ŷ˿�ѡԁEǁEȵ㻁Eֹͣ',Ȼ������.");
		TextScroll(strtime);
		return;
	}


	comsel.m_bComOpen = g_bComOpen;
	if(IDOK != comsel.DoModal() ) {
		return;
	}

	if(g_ComPortSel == USB_PORT) { //USB�˿ڵĲ�������
		if (g_hComm != INVALID_HANDLE_VALUE) { //�رմ���
			SetEvent(g_hEvtSend);
			SetEvent(g_hExitMonitThread);
			SetEvent(g_hExitSendThread);			
			g_MCB_Oper.bQueryEn = false;
			g_MCB_Oper.bStopCMDEn = false;			
			g_bRunMonitorThread = false;
			g_bRunSendThread = false;					
			if( WaitForSingleObject( g_pThread_Monitor->m_hThread,  3000) == WAIT_TIMEOUT ) {
				TerminateThread( g_pThread_Monitor->m_hThread, -1);		
			}
			else delete g_pThread_Monitor;			
			if( WaitForSingleObject( g_pThread_Send->m_hThread,  3000) == WAIT_TIMEOUT ) {
				TerminateThread( g_pThread_Send->m_hThread, -1);		
			}
			else delete g_pThread_Send;			
			CloseShmMaint232(g_hComm);
			g_hComm = INVALID_HANDLE_VALUE;			
			pStatus->SetPaneText(2,"ͨ�Ŷ˿�: �����ѹر�",1);			
			KillTimer(1);
			Sleep(100);
			g_bComOpen = FALSE;
		}

		g_hUsb = OpenUsbDevice();
		if (g_hUsb == INVALID_HANDLE_VALUE) {
			pStatus->SetPaneText(2,"ͨ�Ŷ˿�:  USB�˿ڴ򿪴��E,1);
			return;
		}
		CloseUsbDevice(g_hUsb);
		g_hUsb = INVALID_HANDLE_VALUE;
		pStatus->SetPaneText(2,"ͨ�Ŷ˿�:  USB�˿ڴ򿪳ɹ�",1);
		pQueryTimeout->SetWindowText("USB�˿�");
		pQuerySampleState->SetWindowText("ֹͣ�ɼ�����");
		return;
	}

	pQueryTimeout->SetWindowText("��ѯ��ʱ: STOP");
	pQuerySampleState->SetWindowText("���ݻ�ȡ: STOP");

    //���ڵĲ�������
	if (g_bComOpen) { //��������Ѵ򿪣���رյ�ǰ����			
		SetEvent(g_hEvtSend);
		SetEvent(g_hExitMonitThread);
		SetEvent(g_hExitSendThread);
		
		g_MCB_Oper.bQueryEn = false;
		g_MCB_Oper.bStopCMDEn = false;
		
		g_bRunMonitorThread = false;
		g_bRunSendThread = false;
		
		
		if( WaitForSingleObject( g_pThread_Monitor->m_hThread,  3000) == WAIT_TIMEOUT ) {
			TerminateThread( g_pThread_Monitor->m_hThread, -1);		
		}
		else delete g_pThread_Monitor;
		
		if( WaitForSingleObject( g_pThread_Send->m_hThread,  3000) == WAIT_TIMEOUT ) {
			TerminateThread( g_pThread_Send->m_hThread, -1);		
		}
		else delete g_pThread_Send;
		
		CloseShmMaint232(g_hComm);
		g_hComm = INVALID_HANDLE_VALUE;
		
		pStatus->SetPaneText(2,"ͨ�Ŷ˿�: �����ѹر�",1);
		
		KillTimer(1);
	}

	Sleep(100);

	//�򿪵�ǰѡ��Ĵ���
	g_hComm = OpenShmMaint232(g_ComPortSel,g_CommBaud);
	if(g_hComm == INVALID_HANDLE_VALUE) {
		//	MessageBox("Communication Port Open Error!");
		AfxMessageBox("��ǰѡ��Ĵ��ڴ򿪴��E\r\n1.ǁEر��Ѿ��򿪸ô��ڵ�����\r\n2.�������ñ��������������ڡ�");
		strtime.Format("%s","��ǰѡ��Ĵ��ڴ򿪴��E1.ǁEر��Ѿ��򿪸ô��ڵ�����2.�������ñ���������������.");
		TextScroll(strtime);
		g_bComOpen = FALSE;
		return ;
	}
	
	g_hEvtFinishRec = CreateEvent(NULL, TRUE, FALSE, NULL);
	
	g_bExec = FALSE;
	
	g_hEvtSend = CreateEvent(NULL, TRUE, FALSE, NULL);
	g_hExitSendThread = CreateEvent(NULL,TRUE,FALSE,NULL);
	g_hExitMonitThread = CreateEvent(NULL,TRUE,FALSE,NULL);
	
	g_pThread_Monitor = AfxBeginThread(MonitorThread,
								(LPVOID)this,
								THREAD_PRIORITY_BELOW_NORMAL,
								0,
								CREATE_SUSPENDED);						
	
	g_pThread_Monitor->m_bAutoDelete = FALSE;
	g_pThread_Monitor->ResumeThread();
	
	
	g_pThread_Send = AfxBeginThread(SendThread,
								(LPVOID)this,
								THREAD_PRIORITY_BELOW_NORMAL,
								0,
								CREATE_SUSPENDED);						
	
	g_pThread_Send->m_bAutoDelete = FALSE;
	g_pThread_Send->ResumeThread();
	
	if (g_ComPortSel == COM1_PORT) {
		pStatus->SetPaneText(2,"ͨ�Ŷ˿�: ����1 �򿪳ɹ�",1);
	}
	if (g_ComPortSel == COM2_PORT) {
		pStatus->SetPaneText(2,"ͨ�Ŷ˿�: ����2 �򿪳ɹ�",1);
	}


	g_bComOpen = TRUE;
	g_MCB_Oper.bQueryEn = false;
}

void CMCBView::OnMenuReceivesample() 
{
//	time = CTime::GetCurrentTime();
	CString strtime;
//	strtime.Format("�ռ�����: %.2d:%.2d:%.2d",time.GetHour(),time.GetMinute(),time.GetSecond());
//    TextScroll(strtime);
	
	//USB���Ʋ��� �ж�
	if (g_ComPortSel == USB_PORT) {
	
/*		if (g_hUsbReadThread != INVALID_HANDLE_VALUE) {
			AfxMessageBox("USB��ȡ�߳���������!\r\nǁEȵ����ֹͣ�����Ժ�����.",MB_ICONINFORMATION);
			return;
		}
		if(IDNO == AfxMessageBox("ȷ��MCBӲ���豸�����ڡ��ֶ�ģʽ�� ? \r\n(���ֶ�ģʽ���£�Ӳ����Modeָʾ����) ",MB_YESNO | MB_ICONINFORMATION)){
			pStatus->SetPaneText(2,"ȡ�����ζ�ȡFlash���ݵĲ�ׁE,1);
			return;
		}		

		CReadFlashSel FlashSel;
		FlashSel.iFlashSel = m_iReadFlashSel;
		if (FlashSel.DoModal() == IDCANCEL) {			
			return;
		}
		m_iReadFlashSel = FlashSel.iFlashSel;
*/
	    m_iReadFlashSel = 0;
		KillTimer(0);
 
		m_bLoad1PackageUsbData = TRUE;
		m_bMCBModeSel = TRUE;
		m_bUsbOnTimeShow = TRUE;
		g_MCB_Oper.bQueryEn = FALSE;
		g_hUsb = OpenUsbDevice();
		if (g_hUsb == INVALID_HANDLE_VALUE) {
//			AfxMessageBox("USB�˿ڴ򿪴���\r\nǁE�E���·����.");
			strtime.Format("%s","USB�˿ڴ򿪴��EǁE�E���·����.");
			TextScroll(strtime);
			return;
		}	

		Beep(2500,80);
		g_bEnReadFlashData = TRUE;
		DWORD dwID;
		g_hUsbReadThread = CreateThread(NULL,0,UsbReadDataThread,(LPVOID)this,0,&dwID);
		pStatus->SetPaneText(2,"׼����ȡFlash�е�����...",1);
		return;
	}


	if(!g_bComOpen) 
	{
//		AfxMessageBox("����û�д� \nǁEȴ򿪴��ڣ�",MB_ICONINFORMATION);
        strtime.Format("%s","����û�д�,ǁEȴ򿪴���");
        TextScroll(strtime);
		return;
	}
	if((g_bExec)||(g_MCB_Oper.bQueryEn))
	{
//		AfxMessageBox("���ڵȴ���һ��ÁE���Ӧ��ǁE�һ�������",MB_ICONINFORMATION);
        strtime.Format("%s","���ڵȴ���һ��ÁE���Ӧ��EǁE�һ�������");
        TextScroll(strtime);		
		return;
	}
	time = CTime::GetCurrentTime();
//	CString strtime;
	strtime.Format("�ռ�����: %.2d:%.2d:%.2d",time.GetHour(),time.GetMinute(),time.GetSecond());
    TextScroll(strtime);
	//���ջ�������E
	memset(m_recbuff,0,1024*4*8);
	memset(m_secrecbuf,0,1024*4*8);
	m_precbuf = m_recbuff;
	m_iworkcounter = 0;

	m_bExpRamLongNew_valid = FALSE;//�����ⰁE��ݱ����գ����ⰁE

	g_ThreadCMD = IDC_RECEIVE_EXTERNAL_RAM;
	SetEvent(g_hEvtSend);
	    
	
}

void CMCBView::OnMenuResetsample() 
{
	
//	time=CTime::GetCurrentTime();
	CString s;
//	s.Format("Ӳ����λ: %.2d:%.2d:%.2d",time.GetHour(),time.GetMinute(),time.GetSecond());
//	TextScroll(s);	
	
	if (g_ComPortSel == USB_PORT) {
		g_hUsb = OpenUsbDevice();
		if (g_hUsb == INVALID_HANDLE_VALUE) {
//			AfxMessageBox("USB�˿ڴ򿪴���\r\nǁE�E���·����.");
			s.Format("%s","USB�˿ڴ򿪴��EǁE�E���·����!");
			TextScroll(s);
			return;
		}	
		OnUsbParaSet(TRUE);
		CloseUsbDevice(g_hUsb);
		g_hUsb = INVALID_HANDLE_VALUE;
		pStatus->SetPaneText(2,"Ӳ����λ����́E�",1);
		s.Format("%s","Ӳ����λ: Ӳ����λ����́E�");
		TextScroll(s);
		return;
	}



	if(!g_bComOpen) 
	{
//		AfxMessageBox("����û�д� \nǁEȴ򿪴��ڣ�",MB_ICONINFORMATION);
		s.Format("%s","����û�д� ,ǁEȴ򿪴���!");
	    TextScroll(s);
		return;
	}
	if((g_bExec)&&(g_MCB_Oper.bQueryEn))
	{
//		AfxMessageBox("���ڵȴ���һ��ÁE���Ӧ��ǁE�һ�������",MB_ICONINFORMATION);
		s.Format("%s","���ڵȴ���һ��ÁE���Ӧ��ǁE�һ�������!");
		TextScroll(s);
		return;
	}
	
	time=CTime::GetCurrentTime();
//	CString s;
	s.Format("Ӳ����λ: %.2d:%.2d:%.2d",time.GetHour(),time.GetMinute(),time.GetSecond());
	TextScroll(s);
	//���ջ�������E
	memset(m_recbuff,0,1024*4*8);
	memset(m_secrecbuf,0,1024*4*8);
	m_precbuf = m_recbuff;
	m_iworkcounter = 0;

	g_ThreadCMD = IDC_RESET_SYSTEM;

	g_MCB_Oper.bQueryEn = false;
	g_MCB_Oper.bStopCMDEn = false;

	SetEvent(g_hEvtSend);
}

void CMCBView::OnMenuSendsample() //�������ݵ�Sram
{
//	   time = CTime::GetCurrentTime();
	   CString strtime;
//	   strtime.Format("��������: %.2d:%.2d:%.2d",time.GetHour(),time.GetMinute(),time.GetSecond());	
//	   TextScroll(strtime);

	if (g_ComPortSel == USB_PORT) {
		g_hUsb = OpenUsbDevice();
		if (g_hUsb == INVALID_HANDLE_VALUE) {
//			AfxMessageBox("USB�˿ڴ򿪴���\r\nǁE�E���·����.");
			strtime.Format("%s","USB�˿ڴ򿪴��EǁE�E���·����.");
			TextScroll(strtime);
			return;
		}	

		unsigned char *sendbuff;
		sendbuff = new unsigned char[64*1024];

		//дSRAM�����в��ԣ�һ��д8K����E
		if (m_iFullChannelWidth > 8*1024) 
		{
//			AfxMessageBox("Ŀǰϵͳֻ֧��8K��������,\r\n����ЁE��ӵ����E�E����Ա��ϵ��");
		    strtime.Format("%s","��������: Ŀǰϵͳֻ֧��8K��������,\n����ЁE��ӵ����E�E����Ա��ϵ��");	
			TextScroll(strtime);
			m_iFullChannelWidth = 8*1024;
		}
		for(int i=0;i<m_iFullChannelWidth;i++) {  //{(8*1024)
			sendbuff[i*8+0] = (unsigned char)i;	
			sendbuff[i*8+1] = (unsigned char)(i>>8);
			sendbuff[i*8+2] = 0x00;
			sendbuff[i*8+3] = 0x50;	//���õ�ַÁE�E
			sendbuff[i*8+4] = (unsigned char)m_arrBUFFExternalRam[i];	
			sendbuff[i*8+5] = (unsigned char)(m_arrBUFFExternalRam[i]>>8);
			sendbuff[i*8+6] = (unsigned char)(m_arrBUFFExternalRam[i]>>16);
			sendbuff[i*8+7] = 0x51; //��������		
		}
		WriteUsbData(g_hUsb,sendbuff,m_iFullChannelWidth*8);//64*1024

        delete sendbuff;

		CloseUsbDevice(g_hUsb);
		g_hUsb = INVALID_HANDLE_VALUE;
		pStatus->SetPaneText(2,"���������ļ���Sram ́E�",1);
        Beep(1200,80);
		return;
	}


	if(!g_bComOpen) {
//		AfxMessageBox("����û�д� \nǁEȴ򿪴��ڣ�",MB_ICONINFORMATION);
		strtime.Format("%s","����û�д�,ǁEȴ򿪴��ڣ�");
		TextScroll(strtime);
		return;
	}
	if((g_bExec)&&(g_MCB_Oper.bQueryEn)){
//		AfxMessageBox("���ڵȴ���һ��ÁE���Ӧ��ǁE�һ�������",MB_ICONINFORMATION);
		strtime.Format("%s","���ڵȴ���һ��ÁE���Ӧ��ǁE�һ�������");
		TextScroll(strtime);
		return;
	}

	time = CTime::GetCurrentTime();
//	CString strtime;
	strtime.Format("��������: %.2d:%.2d:%.2d",time.GetHour(),time.GetMinute(),time.GetSecond());	
	TextScroll(strtime);
	//���ջ�������E
	memset(m_recbuff,0,1024*4*8);
	memset(m_secrecbuf,0,1024*4*8);
	m_precbuf = m_recbuff;
	m_iworkcounter = 0;

//	memcpy(m_arrExternalRam,m_arrBUFFExternalRam,1024*4);
	
	g_ThreadCMD = IDC_SEND_EXTERNAL_RAM;	
	SetEvent(g_hEvtSend);	
}

void CMCBView::OnMenuStartsample() 
{
    ///////////////////////changing
	m_startflag=1;
	CMainFrame* pMainFrame = (CMainFrame*)GetParentFrame();
	CEdit* pSetTime = (CEdit*)(pMainFrame->m_wndDlgBar.GetDlgItem(IDC_TIMER_EDIT));
	CButton* pStartSample = (CButton*)(pMainFrame->m_wndDlgBar.GetDlgItem(IDC_CTL_START_SAMPLE));
	CEdit* pShowstate = (CEdit*)(pMainFrame->m_wndDlgBar.GetDlgItem(IDC_STATE_SHOW));
//	CEdit* pShowstarttime = (CEdit*)(pMainFrame->m_wndDlgBar.GetDlgItem(IDC_STATE_START_TIME));
	CButton* pRealTimeSel = (CButton*)(pMainFrame->m_wndDlgBar.GetDlgItem(IDC_REALTIME_SEL));
	CButton* pLiveTimeSel = (CButton*)(pMainFrame->m_wndDlgBar.GetDlgItem(IDC_LIVETIME_SEL));
	CEdit* pQueryTimeout=(CEdit*)(pMainFrame->m_wndDlgBar.GetDlgItem(IDC_QUERY_TIMEOUT));
	CEdit* pQuerySampleState=(CEdit*)(pMainFrame->m_wndDlgBar.GetDlgItem(IDC_QUERY_SAMPLE_STATE));
	CComboBox* pComboxExpHor = (CComboBox *)(pMainFrame->m_wndDlgBar.GetDlgItem(IDC_COMBO_EXP_HOR));
//	CButton* pFULLHor = (CButton*)(pMainFrame->m_wndDlgBar.GetDlgItem(IDC_FULLRAM_VIEWSEL));
//	CButton* pExpHor = (CButton*)(pMainFrame->m_wndDlgBar.GetDlgItem(IDC_EXPRAM_VIEWSEL));
	CButton* pBuf = (CButton*)(pMainFrame->m_wndDlgBar.GetDlgItem(IDC_BUFF_MODSEL));
	CButton* pMcb2Buf = (CButton*)(pMainFrame->m_wndDlgBar.GetDlgItem(IDC_EXTERNALDATA_MCB2BUFF));

    pSetTime->EnableWindow(FALSE);

   
	time = CTime::GetCurrentTime();
	CString strtime;
	strtime.Format("�ɼ���ʼ: %.2d:%.2d:%.2d...",time.GetHour(),time.GetMinute(),time.GetSecond());
    m_starttime_hour=time.GetHour();
	m_starttime_min=time.GetMinute();
	m_starttime_sec=time.GetSecond();


	if ((g_ComPortSel == COM1_PORT)||(g_ComPortSel == COM2_PORT)) {
		if(!g_bComOpen) {
//			AfxMessageBox("ͨ�Ŷ˿�û�д� \nǁEȴ򿪶˿ڣ�",MB_ICONINFORMATION);
			strtime.Format("%s","ͨ�Ŷ˿�û�д�,ǁEȴ򿪶˿�!");
			TextScroll(strtime);
			pMcb2Buf->ShowWindow(TRUE);
            pSetTime->EnableWindow(TRUE);
			m_startflag=0;
			return;
		}
		if(g_bExec){
//			AfxMessageBox("���ڵȴ���һ��ÁE���Ӧ��ǁE�һ�������",MB_ICONINFORMATION);
			strtime.Format("%s","���ڵȴ���һ��ÁE���Ӧ��EǁE�һ�������!");
			TextScroll(strtime);
			pMcb2Buf->ShowWindow(TRUE);
            pSetTime->EnableWindow(TRUE);
			m_startflag=0;
			return;
		}
	}
	//expֻ��ѡԁE12
	if (!pBuf->GetCheck()&&m_startflag)
	{
		pComboxExpHor->ResetContent();
		pComboxExpHor->AddString("64");
		pComboxExpHor->AddString("128");
		pComboxExpHor->AddString("256");
		pComboxExpHor->AddString("512");
	}
   
	if(m_iSamllWinWidth>=512)
    {
		
        m_iSamllWinWidth=512;
		m_iSmallWinCHL = m_iSmallWinCenPoint - m_iSamllWinWidth/2;
		m_iSmallWinCHR = m_iSmallWinCenPoint + m_iSamllWinWidth/2;
		if (m_bMCBModeSel)pComboxExpHor->SetCurSel(3);
	}
    else
    {
		switch(m_iSamllWinWidth)
		{
			   case 64: pComboxExpHor->SetCurSel(0); break;
			   case 128:pComboxExpHor->SetCurSel(1); break;
			   case 256:pComboxExpHor->SetCurSel(2); break;
			   case 512:pComboxExpHor->SetCurSel(3); break;
		}
    }
	if (m_iSmallWinCHL< 0) {m_iSmallWinCHL = 0;	}
	if (m_iSmallWinCHR > m_iFullChannelWidth-1) {m_iSmallWinCHR = m_iFullChannelWidth;	}
	Redraw();
	
    //////////////////////////////////////////////////////////////changing
	/*	pShowstarttime->SetWindowText(strtime);
	date.year = time.GetYear();
	date.month = time.GetMonth();
	date.day = time.GetDay();
	date.hour = time.GetHour();
	date.minite = time.GetMinute();
	date.second = time.GetSecond();	
	DateToSecond(&m_iSaveFileTime,&date);*/
	m_iSaveFileRealTime = 0;
	m_iSaveFileLiveTime = 0;
     
	//�����ʾ����ǁE
	for(int i=0;i<1024;i++)
	{
		m_arrFullRam[i] &= 0x00000000;/////////changing
		m_arrExpRam[i] &= 0x00000000;/////////changing

	}

	for(i=0;i<1024*8;i++)
	{
		m_arrMCBExternalRam[i] &= 0x80000000;/////////changing
	}

	if(m_bSetSampleTimeRealMode == TRUE) {
		pRealTimeSel->EnableWindow(TRUE);
		pLiveTimeSel->EnableWindow(FALSE);
	}
	else{
		pRealTimeSel->EnableWindow(FALSE);
		pLiveTimeSel->EnableWindow(TRUE);
	}

	//���ջ�������E
	memset(m_recbuff,0,1024*4*8);
	memset(m_secrecbuf,0,1024*4*8);
   


	m_precbuf = m_recbuff;
	m_iworkcounter = 0;	
	CString m_strSettime;
	pSetTime->GetWindowText(m_strSettime);
	if(!m_strSettime.IsEmpty()) {		
           m_iSetSampleTime =atol(m_strSettime); 
	}
	else m_iSetSampleTime = 0x7fffff00;
	g_HardWare_Pare.iSampleTimeValue = m_iSetSampleTime;
	m_iUsbSampleTimeCnt = 0;
	
	//USB���Ʋ��� �ж�
	if (g_ComPortSel == USB_PORT) {
		if (g_hUsbReadThread != INVALID_HANDLE_VALUE) {
//			AfxMessageBox("USB��ȡ�߳���������!\r\nǁEȵ����ֹͣ�����Ժ�����.",MB_ICONINFORMATION);
			strtime.Format("%s","USB��ȡ�߳���������!\r\nǁEȵ����ֹͣ�����Ժ�����.");
			TextScroll(strtime);
			pMcb2Buf->ShowWindow(TRUE);
            pSetTime->EnableWindow(TRUE);
			m_startflag=0;
			return;
		}

		m_bLoad1PackageUsbData = FALSE;
		m_bUsbOnTimeShow = TRUE;
		m_iUsbRecPackageCnt = 0;
		g_MCB_Oper.bQueryEn = FALSE;
		g_bEnReadFlashData = FALSE;
		g_hUsb = OpenUsbDevice();
		if (g_hUsb == INVALID_HANDLE_VALUE) {
//			AfxMessageBox("USB�˿ڴ򿪴���\r\nǁE�E���·����.");
			strtime.Format("%s","USB�˿ڴ򿪴���\r\nǁE�E���·����.");
			TextScroll(strtime);
			pMcb2Buf->ShowWindow(TRUE);
            pSetTime->EnableWindow(TRUE);
			m_startflag=0;			
			return;
		}

		OnUsbParaSet(FALSE);

		pQueryTimeout->SetWindowText("USB�˿�");
		pQuerySampleState->SetWindowText("��ʼ�ɼ�����");

		pStatus->SetPaneText(2,"USB�˿�׼���ɼ�����...",1);

		//��ʼ ÁE�E
		char chrPara[4];
		chrPara[0] = 0x00;	
		chrPara[1] = 0x00;
		chrPara[2] = 0x00;
		chrPara[3] = 0x10;
		WriteUsbData(g_hUsb,chrPara,4);		
		SetTimer(0,500,NULL);
		DWORD dwID;
		g_hUsbReadThread = CreateThread(NULL,0,UsbReadDataThread,(LPVOID)this,0,&dwID);
		return;
	}//end of usb


//    pMcb2Buf->ShowWindow(FALSE);
    TextScroll(strtime);
	//����Ϊ���ڿ��Ʋ���
	g_ThreadCMD = IDC_CTL_START_SAMPLE;
	m_bExpRamLongNew_valid = TRUE;//�������ݱ�ˢ�¹�
	g_MCB_Oper.bQueryEn = true;
	g_MCB_Oper.bStopCMDEn = false;	
	SetEvent(g_hEvtSend);

	if(m_bMCBModeSel) {
		g_MCB_Oper.iStartQueryNo = MCB_START_QUERYNO;
		g_MCB_Oper.iCurQueryNo = MCB_START_QUERYNO;
	}
	else{
		g_MCB_Oper.iStartQueryNo = BUFF_START_QUERYNO;
		g_MCB_Oper.iCurQueryNo = BUFF_START_QUERYNO;
	}
	
}

DWORD WINAPI UsbReadDataThread(LPVOID  lpData)
{
	CMCBView *ph=(CMCBView *)lpData ;		
	CString curText;
//	char chrRecStat[100];	
	
	char SendFpgaCmd[10];
	SendFpgaCmd[0] = 0x00;
	SendFpgaCmd[1] = 0x00;
	SendFpgaCmd[2] = 0x00;
	SendFpgaCmd[3] = 0x61;

	char ReadTimeParaCMD[10];
	ReadTimeParaCMD[0] = 0x00;
	ReadTimeParaCMD[1] = 0x00;
	ReadTimeParaCMD[2] = 0x00;
	ReadTimeParaCMD[3] = 0x70;

	
	UINT speedbuff =0;
	int size = 0;	
	UINT Mcbwidth;
	if (g_HardWare_Pare.bInputAnolSignalSel) { //
		Mcbwidth = 8192;   // * 2��������2����׼��ȥ����ֵ����
	}
	else{
		Mcbwidth = g_HardWare_Pare.iChannelWidth; 
	}
	
	int recDataCnt = 0;

	unsigned char *pdata;
	pdata= new unsigned char[65536];
	g_bRunUsbReadDataThread = TRUE;
	while(g_bRunUsbReadDataThread) {//64			
		if (ph->m_bUsbOnTimeShow == TRUE) {
			ph->m_bUsbOnTimeShow = FALSE;
			WriteUsbData(g_hUsb,SendFpgaCmd,4);	
			size = ReadUsbData(g_hUsb,pdata,Mcbwidth*4);//�̶���ȡ16K ����E

			if (size >= (int)Mcbwidth*4) {
				memcpy(ph->m_pUsbProduceBuff,pdata,size);
				ph->SendMessage(WM_PRODUCE_USB_DATA,size,0);
				if (ph->m_bLoad1PackageUsbData == TRUE) {
                      g_bRunUsbReadDataThread = FALSE;
				}
			}
			//��ȡʱ�����  IndataMCBģʽʱ��ȡʱ�����
			if (g_HardWare_Pare.bInputAnolSignalSel == FALSE) { //
				WriteUsbData(g_hUsb,ReadTimeParaCMD,4);	
				size = ReadUsbData(g_hUsb,pdata,1024);
				memcpy(ph->m_pUsbProduceBuff,pdata,size);
				ph->SendMessage(WM_PRODUCE_USB_DATA,1024,0);
			}
		}
		Sleep(100);					
	}
	
	delete pdata;		

	return 0;
}

LRESULT CMCBView::OnProduceUsbData(WPARAM wParam, LPARAM lParam)
{	
	int channelWidth = g_HardWare_Pare.iChannelWidth;
    
    if (wParam == 1024) { //ʱ������ĳ���
		CMainFrame* pMainFrame = (CMainFrame*)GetParentFrame();
		CEdit* pShowRealTime = (CEdit*)(pMainFrame->m_wndDlgBar.GetDlgItem(IDC_SHOW_REAL_TIME));
		CEdit* pShowLiveTime = (CEdit*)(pMainFrame->m_wndDlgBar.GetDlgItem(IDC_SHOW_LIVE_TIME));
		CEdit* pShowRatioTime = (CEdit*)(pMainFrame->m_wndDlgBar.GetDlgItem(IDC_SHOW_RATIO_TIME));
		
		char chrtext[200];
//		UINT itemp;
		float frotio; 
	
	
		m_iRealTime = m_pUsbProduceBuff[0];
		m_iLiveTime = m_pUsbProduceBuff[1];
		
		if(m_iRealTime == 0) {
			frotio = 100.;
		}
		else{
			frotio = (float)(100.0*(float)(m_iRealTime - m_iLiveTime)/(float)m_iRealTime);
		}
		
		sprintf(chrtext,"ʵʱ: %.2fs",((float)m_iRealTime/100.)); //ʱ�䵥λ��1/100ÁE
		pShowRealTime->SetWindowText(chrtext);
		
		sprintf(chrtext,"���: %.2fs",((float)m_iLiveTime/100.));
		pShowLiveTime->SetWindowText(chrtext);
		sprintf(chrtext,"����: %.2f %%",frotio);
		pShowRatioTime->SetWindowText(chrtext);
		
		//��E� �ɼ��� ʵʱ��E�� ��汼�E
		m_iSaveFileRealTime = m_iRealTime;
		m_iSaveFileLiveTime = m_iLiveTime;
		return 0;
    }

	//����ģʽ���� ��00,01,02,03,04,05��
    if (g_HardWare_Pare.bInputAnolSignalSel == TRUE) {//05=>ģ����ȁE��ADC_MCBģʽ
		if (4096 == channelWidth) {
			for(int i=0;i<channelWidth;i++){
				m_pUsbProduceBuff[i] = (((m_pUsbProduceBuff[i*2] + m_pUsbProduceBuff[i*2 + 1]) & 0x00FFFFFF));
			}		
		}
		if (2048 == channelWidth) {
			for(int i=0;i<channelWidth;i++){
				int buffkk = 0;
				for(int k=0;k<4;k++){
					buffkk += m_pUsbProduceBuff[i*4 + k];			
				}
				m_pUsbProduceBuff[i] = (buffkk & 0x00FFFFFF); 		
			}		
		}
		if (1024 == channelWidth) {
			for(int i=0;i<channelWidth;i++){
				int buffkk = 0;
				for(int k=0;k<8;k++){
					buffkk += m_pUsbProduceBuff[i*8 + k];			
				}
				m_pUsbProduceBuff[i] = (buffkk & 0x00FFFFFF); 		
			}		
		}
    }


	for(int i=0;i<channelWidth;i++) {
		m_arrMCBExternalRam[i] = (m_arrBUFFExternalRam[i] & 0x80000000) | (m_pUsbProduceBuff[i] & 0x00FFFFFF);
	}

	if (m_bMCBModeSel == TRUE) 
	{ //ΪMCB��ʽ������ʾ���ݲ���
		memcpy(m_arrExternalRam,m_arrMCBExternalRam,1024*4*8);
		Redraw();
	}
	
	m_iUsbRecPackageCnt++;
	char chrRecStat[80];
	sprintf(chrRecStat,"���ݰ�E�:%d  ����E%d",m_iUsbRecPackageCnt,g_HardWare_Pare.iChannelWidth);
	pStatus->SetPaneText(2,chrRecStat,1);
	
	if ((g_bEnReadFlashData == TRUE) && (g_ComPortSel == USB_PORT)){
    	CloseHandle(g_hUsbReadThread);
		g_hUsbReadThread = INVALID_HANDLE_VALUE;
		CloseUsbDevice(g_hUsb);
		g_hUsb = INVALID_HANDLE_VALUE;
	}

   return TRUE;
}

void CMCBView::OnMenuStopsample() 
{	
	CMainFrame* pMainFrame = (CMainFrame*)GetParentFrame();
	CEdit* pShowstate = (CEdit*)(pMainFrame->m_wndDlgBar.GetDlgItem(IDC_STATE_SHOW));
//	CEdit* pShowstarttime = (CEdit*)(pMainFrame->m_wndDlgBar.GetDlgItem(IDC_STATE_START_TIME));
	CButton* pRealTimeSel = (CButton*)(pMainFrame->m_wndDlgBar.GetDlgItem(IDC_REALTIME_SEL));
	CButton* pLiveTimeSel = (CButton*)(pMainFrame->m_wndDlgBar.GetDlgItem(IDC_LIVETIME_SEL));
	CEdit* pQueryTimeout=(CEdit*)(pMainFrame->m_wndDlgBar.GetDlgItem(IDC_QUERY_TIMEOUT));
	CEdit* pQuerySampleState=(CEdit*)(pMainFrame->m_wndDlgBar.GetDlgItem(IDC_QUERY_SAMPLE_STATE));
	CEdit* pSetTime = (CEdit*)(pMainFrame->m_wndDlgBar.GetDlgItem(IDC_TIMER_EDIT));
	CButton* pMcb2Buf = (CButton*)(pMainFrame->m_wndDlgBar.GetDlgItem(IDC_EXTERNALDATA_MCB2BUFF));	
	CComboBox* pComboxExpHor = (CComboBox *)(pMainFrame->m_wndDlgBar.GetDlgItem(IDC_COMBO_EXP_HOR));
	
	pSetTime->EnableWindow(TRUE);
    pMcb2Buf->ShowWindow(TRUE);

	CString strStopTime;
	
	if ((g_ComPortSel == COM1_PORT)||(g_ComPortSel == COM2_PORT)) {
		if(!g_bComOpen) {
			AfxMessageBox("����û�д� \nǁEȴ򿪴��ڣ�",MB_ICONINFORMATION);
			strStopTime.Format("%s","����û�д�,ǁEȴ򿪴���!");
			TextScroll(strStopTime);
			return;
		}
	}

	pComboxExpHor->ResetContent();
	pComboxExpHor->AddString("64");
	pComboxExpHor->AddString("128");
	pComboxExpHor->AddString("256");
	pComboxExpHor->AddString("512");
	pComboxExpHor->AddString("1024");
	pComboxExpHor->AddString("2048");
    pComboxExpHor->AddString("4096");
	pComboxExpHor->AddString("8192");
	pComboxExpHor->SetCurSel(m_iSmallWindowWidthCheckSel);


//	CString strStartTime;
//	pShowstarttime->GetWindowText(strStartTime);

	pRealTimeSel->EnableWindow(TRUE);
	pLiveTimeSel->EnableWindow(TRUE);

	CTime ttime;
	CString strtime;
	
	//USB���Ʋ��� �ж�
	if (g_ComPortSel == USB_PORT) {
		//��ʼ
		char chrPara[4];
		chrPara[0] = 0x00;	
		chrPara[1] = 0x00;
		chrPara[2] = 0x00;
		chrPara[3] = 0x11;
		WriteUsbData(g_hUsb,chrPara,4);	

		g_MCB_Oper.bQueryEn = FALSE;
		m_bUsbOnTimeShow = FALSE;
		g_bRunUsbReadDataThread = FALSE;
		g_bEnReadFlashData = FALSE;
		KillTimer(0);
		if (g_hUsbReadThread == INVALID_HANDLE_VALUE) {
			return;
		}
		if( WaitForSingleObject(g_hUsbReadThread,1300) == WAIT_TIMEOUT ) {
			CancelIo(g_hUsb);
			TerminateThread(g_hUsbReadThread, -1);		
		}
		
		CloseHandle(g_hUsbReadThread);
		g_hUsbReadThread = INVALID_HANDLE_VALUE;
		CloseUsbDevice(g_hUsb);
		g_hUsb = INVALID_HANDLE_VALUE;	
		
		ttime = CTime::GetCurrentTime();
//		strStopTime.Format("Stop  at: %.2d:%.2d:%.2d",ttime.GetHour(),ttime.GetMinute(),ttime.GetSecond());
//		pShowstarttime->SetWindowText(strStartTime + "\n" + strStopTime);
		Sleep(80);		
		pQueryTimeout->SetWindowText("USB�˿�");
		pQuerySampleState->SetWindowText("ֹͣ�ɼ�����");
		
		char chrRecStat[80];
		sprintf(chrRecStat,"ֹͣ�ɼ�! �ѽ���MCB��E�:%d     ����E%d",m_iUsbRecPackageCnt,g_HardWare_Pare.iChannelWidth);
		pStatus->SetPaneText(2,chrRecStat,1);
		Beep(800,50);

		return;
	}

    
	ttime = CTime::GetCurrentTime();
	if (g_MCB_Oper.bQueryEn) {	
		strStopTime.Format("ֹͣ�ɼ�: %.2d:%.2d:%.2d",ttime.GetHour(),ttime.GetMinute(),ttime.GetSecond());
//      strStopTime=m_starttime+strStopTime;
		TextScroll(strStopTime);
//		m_starttime.Empty();
		//pShowstarttime->SetWindowText(strStartTime + "\n" + strStopTime);
	}

	m_precbuf = m_recbuff;
	m_iworkcounter = 0;

	g_MCB_Oper.bQueryEn = false;
	g_MCB_Oper.bStopCMDEn = true;

	KillTimer(1);
	SetTimer(3,500,NULL);

   ////////////////////////////////////////////////////////////////changing
    if(m_startflag==1)
	{
//		CButton* pFullViewSel = (CButton *)(pMainFrame->m_wndDlgBar.GetDlgItem(IDC_FULLRAM_VIEWSEL));
		CComboBox* pComboxHor = (CComboBox *)(pMainFrame->m_wndDlgBar.GetDlgItem(IDC_COMBO_HOR));
//        if(m_bExpRamViewSel)
		{
              OnExpramViewsel(); 
		}
//		else
		{
              OnFullramViewsel();
		}
	}
	m_startflag=0;
	m_icontinueflag=0;
}

void CMCBView::OnMenuTestsample() 
{
	
//	time = CTime::GetCurrentTime();
	CString strtime;
//	strtime.Format("ͨ�Ų���: %.2d:%.2d:%.2d",time.GetHour(),time.GetMinute(),time.GetSecond());	
//	TextScroll(strtime);
	
	if(!g_bComOpen) 
	{
		AfxMessageBox("����û�д� \nǁEȴ򿪴��ڣ�",MB_ICONINFORMATION);
		strtime.Format("%s","����û�д�,ǁEȴ򿪴���!");	
		TextScroll(strtime);
		return;
	}
	memset(m_recbuff,0,1024*4*8);
	memset(m_secrecbuf,0,1024*4*8);
	m_precbuf = m_recbuff;
	m_iworkcounter = 0;

	if((g_bExec)&&(g_MCB_Oper.bQueryEn)){
		AfxMessageBox("���ڵȴ���һ��ÁE���Ӧ��ǁE�һ�������",MB_ICONINFORMATION);
		strtime.Format("%s","���ڵȴ���һ��ÁE���Ӧ��ǁE�һ�������!");	
		TextScroll(strtime);		
		return;
	}
    time = CTime::GetCurrentTime();
//	CString strtime;
	strtime.Format("ͨ�Ų���: %.2d:%.2d:%.2d",time.GetHour(),time.GetMinute(),time.GetSecond());	
	TextScroll(strtime);

	g_MCB_Oper.bQueryEn = false;		//��ֹ��ѯ
	g_MCB_Oper.bStopCMDEn = false;		//��ֹֹͣ

	m_bExpRamLongNew_valid = FALSE; //��E�������Ч����֤�ⰁE��ݱ����յ�

	g_ThreadCMD = IDC_COMTEST;
	SetEvent(g_hEvtSend);	
}

void CMCBView::OnMenuLoadTimerCount()
{
	CString s;
	if(!g_bComOpen) {
		AfxMessageBox("����û�д� \nǁEȴ򿪴��ڣ�",MB_ICONINFORMATION);
        s.Format("%s","����û�д�,ǁEȴ򿪴���!");
		TextScroll(s);
		return;
	}
	memset(m_recbuff,0,1024*4*8);
	memset(m_secrecbuf,0,1024*4*8);
	m_precbuf = m_recbuff;
	m_iworkcounter = 0;
	
	if((g_bExec)||(g_MCB_Oper.bQueryEn)){
		AfxMessageBox("���ڵȴ���һ��ÁE���Ӧ��ǁE�һ�������",MB_ICONINFORMATION);
        s.Format("%s","���ڵȴ���һ��ÁE���Ӧ��ǁE�һ�������!");
		TextScroll(s);		
		return;
	}
	
	g_MCB_Oper.bQueryEn = false;		//��ֹ��ѯ
	g_MCB_Oper.bStopCMDEn = false;		//��ֹֹͣ
	
	g_ThreadCMD = CTL_LOAD_TIME_VALUE;
	SetEvent(g_hEvtSend);
}



void CMCBView::OnMenuContinuesample() 
{
	//	OnCtlContinueSample();
	CMainFrame* pMainFrame = (CMainFrame*)GetParentFrame();
	CEdit* pSetTime = (CEdit*)(pMainFrame->m_wndDlgBar.GetDlgItem(IDC_TIMER_EDIT));
	CButton* pStartSample = (CButton*)(pMainFrame->m_wndDlgBar.GetDlgItem(IDC_CTL_START_SAMPLE));
	CEdit* pShowstate = (CEdit*)(pMainFrame->m_wndDlgBar.GetDlgItem(IDC_STATE_SHOW));
//	CEdit* pShowstarttime = (CEdit*)(pMainFrame->m_wndDlgBar.GetDlgItem(IDC_STATE_START_TIME));


//   time = CTime::GetCurrentTime();
	CString strtime;
//	strtime.Format("�����ɼ�: %.2d:%.2d:%.2d",time.GetHour(),time.GetMinute(),time.GetSecond());	
//    TextScroll(strtime);
	//	pShowstarttime->SetWindowText(strtime);

	//USB���Ʋ��� �ж�
	if (g_ComPortSel == USB_PORT) {
		m_bUsbOnTimeShow = TRUE;
		m_iUsbRecPackageCnt = 0;
		g_MCB_Oper.bQueryEn = FALSE;
		g_hUsb = OpenUsbDevice();
		if (g_hUsb == INVALID_HANDLE_VALUE) {
			AfxMessageBox("USB�˿ڴ򿪴���\r\nǁE�E���·����.");
			strtime.Format("%s","USB�˿ڴ򿪴��EǁE�E���·����.");	
			TextScroll(strtime);
			return;
		}
		
		ResetUsbDevice(g_hUsb);
		Sleep(350);
		m_bLoad1PackageUsbData = false;

		//��ʼ ÁE�E
		char chrPara[4];
		chrPara[0] = 0x00;	
		chrPara[1] = 0x00;
		chrPara[2] = 0x00;
		chrPara[3] = 0x10;
		WriteUsbData(g_hUsb,chrPara,4);		
		SetTimer(0,500,NULL);
		Beep(1200,80);
		DWORD dwID;
		g_hUsbReadThread = CreateThread(NULL,0,UsbReadDataThread,(LPVOID)this,0,&dwID);
		return;
	}
	
	if(!g_bComOpen) {
		AfxMessageBox("����û�д� \nǁEȴ򿪴��ڣ�",MB_ICONINFORMATION);
		strtime.Format("%s","����û�д�,ǁEȴ򿪴���!");	
		TextScroll(strtime);
		return;
	}
	if(g_bExec){
		AfxMessageBox("���ڵȴ���һ��ÁE���Ӧ��ǁE�һ�������",MB_ICONINFORMATION);
		strtime.Format("%s","���ڵȴ���һ��ÁE���Ӧ��ǁE�һ�������");	
		TextScroll(strtime);
		return;
	}	
    m_icontinueflag=1;
    time = CTime::GetCurrentTime();
//	CString strtime;
	strtime.Format("�����ɼ�: %.2d:%.2d:%.2d",time.GetHour(),time.GetMinute(),time.GetSecond());	
    TextScroll(strtime);

	//���ջ�������E
	memset(m_recbuff,0,1024*4*8);
	memset(m_secrecbuf,0,1024*4*8);
	m_precbuf = m_recbuff;
	m_iworkcounter = 0;

	m_bExpRamLongNew_valid = TRUE;//�������ݱ�ˢ�¹�

	if(m_bMCBModeSel) {
		g_MCB_Oper.iStartQueryNo = MCB_START_QUERYNO;
		g_MCB_Oper.iCurQueryNo = MCB_START_QUERYNO;
	}
	else{
		g_MCB_Oper.iStartQueryNo = BUFF_START_QUERYNO;
		g_MCB_Oper.iCurQueryNo = BUFF_START_QUERYNO;
	}


	
	m_continueflag=1;
	g_ThreadCMD = CTL_HARDWARE_READ;

	g_ThreadCMD = IDC_CONTINUE_SAMPLE;
	SetEvent(g_hEvtSend);

	g_MCB_Oper.bQueryEn = true;
	g_MCB_Oper.bStopCMDEn = false;
	
}

void CMCBView::OnMenuEnergeKedu() 
{
	m_hEnergeKedu = m_pEnergeKedu->GetSafeHwnd();
	if(m_hEnergeKedu == NULL)
	{  //��ֻ֤����һ������	
//	   m_pEnergeKedu->m_iEnergeUnit = g_HardWare_Pare.iEngerScaleUnit;
	   m_pEnergeKedu->m_iEnergeUnit = m_fMenuEnergeScale_U;
	   //m_bMenuEnergeScaleFinish = false;
	   m_pEnergeKedu->Create();	
	   int xxx = m_iViewWindowSize_X/2 - 70;
	   int yyy = m_iViewWindowSize_Y/2 - 120;
	   if (xxx <= 20) {   xxx = 20;	  }
	   if (yyy <= 20) {   yyy = 20;	  }
	   m_pEnergeKedu->SetWindowPos(NULL,xxx,yyy,200,130,SWP_SHOWWINDOW);
	}	
}


void CMCBView::OnIdMenuWidthStat() 
{
	m_hWidthStat = m_pWidthStat->GetSafeHwnd();
	if(m_hWidthStat == NULL) {  //��ֻ֤����һ������		
	   m_pWidthStat->Create();	
	   int xxx = m_iViewWindowSize_X/2 - 70;
	   int yyy = m_iViewWindowSize_Y/2 - 120;
	   if (xxx <= 20) {   xxx = 20;	  }
	   if (yyy <= 20) {   yyy = 20;	  }
	   m_pWidthStat->SetWindowPos(NULL,xxx,yyy,150,130,SWP_SHOWWINDOW);
	}
	
}

void CMCBView::OnShowStateText(CDC* pDC,int line, CString strtext)
{
	if (m_bPrintMapEn) {//pDC->IsPrinting()
		pDC->SetBkColor(RGB(255,255,255));
	}
	else{
		pDC->SetBkColor(RGB(212,212,212));
	}	

	CString strbank = "                        ";
	strbank += strbank;
	strbank += strbank;
	strbank += strbank;

	pDC->SetTextColor(RGB(27,21,225));
	if(line == 1) {
		pDC->SetTextColor(RGB(171,32,191));
		//pDC->TextOut(10,425,strbank);
		//pDC->TextOut(10,425,strtext);
		pDC->TextOut(10,m_iViewWindowSize_Y*420/500+5,strbank);
		pDC->TextOut(10,m_iViewWindowSize_Y*420/500+5,strtext);

	}
	else if(line == 2) {
		m_strStateLine2Text = strtext;
		//pDC->TextOut(10,443,strbank);
		//pDC->TextOut(10,443,strtext);
		pDC->TextOut(10,m_iViewWindowSize_Y*420/500+32,strbank);
		pDC->TextOut(10,m_iViewWindowSize_Y*420/500+32,strtext);
	}
	else if(line == 3) {
		m_strStateLine3Text = strtext;
		//pDC->TextOut(10,461,strbank);
		//pDC->TextOut(10,461,strtext);
		pDC->TextOut(10,m_iViewWindowSize_Y*420/500+51,strbank);
		pDC->TextOut(10,m_iViewWindowSize_Y*420/500+51,strtext);
	}
	else if (line == 4) {
		m_strStateLine4Text = strtext;
		//pDC->TextOut(10,501,strbank);
		//pDC->TextOut(10,501,strtext);
		pDC->TextOut(10,m_iViewWindowSize_Y*420/500+91,strbank);
		pDC->TextOut(10,m_iViewWindowSize_Y*420/500+91,strtext);
	}
	if (line == 5) {
		m_strStateLine5Text = strtext;
		pDC->TextOut(10,m_iViewWindowSize_Y*420/500+111,strbank);
		pDC->TextOut(10,m_iViewWindowSize_Y*420/500+111,strtext);
	}
}

void CMCBView::OnDrawTextBgroud(CDC* pDC)
{
	CBrush bShow;
    CPen pen(PS_SOLID,1,RGB(0,0,0));
/*	if (m_bPrintMapEn) 
	{//pDC->IsPrinting()
		bShow.CreateSolidBrush(RGB(255,255,255));// �����ֿ�E
		CPen pen(PS_SOLID,1,RGB(255,255,255));
	}
	else 
	{
		bShow.CreateSolidBrush(RGB(212,212,212));// �����ֿ�E
		CPen pen(PS_SOLID,1,RGB(0,0,0));
	}
	*/
    bShow.CreateSolidBrush(RGB(255,255,255));
	CBrush* pOldBrush=pDC->SelectObject(&bShow);
	pDC->SelectObject(&pen);
    pDC->Rectangle(0,m_iViewWindowSize_Y*42/50+2,m_iViewWindowSize_X-20,m_iViewWindowSize_Y);//
  
  if (!pDC->IsPrinting())
  {
    CFont font;
	font.CreateFont(13.5,0,0,0,FW_NORMAL,FALSE,FALSE,0,
						ANSI_CHARSET,OUT_DEFAULT_PRECIS,
						CLIP_DEFAULT_PRECIS,PROOF_QUALITY,
						DEFAULT_PITCH | FF_SWISS ,"��́E);

    pDC->SetBkColor(RGB(255,255,255));
	pDC->SelectObject(&font);
	if (m_pos>1995)
	{
		m_pos=1995;
	}
	if (m_pos<=1995)
    {
		pDC->TextOut(10,m_iViewWindowSize_Y*420/500+5,m_scrollstring[m_pos]);
		pDC->TextOut(10,m_iViewWindowSize_Y*420/500+23,m_scrollstring[m_pos+1]);
		pDC->TextOut(10,m_iViewWindowSize_Y*420/500+40,m_scrollstring[m_pos+2]);
		pDC->TextOut(10,m_iViewWindowSize_Y*420/500+57,m_scrollstring[m_pos+3]);
		pDC->TextOut(10,m_iViewWindowSize_Y*420/500+74,m_scrollstring[m_pos+4]);
	}
	font.DeleteObject();
  }	
    bShow.DeleteObject();
	pen.DeleteObject();
}

void CMCBView::OnMouseSetInterestRange()
{

     if (m_bMCBModeSel) 
	{
		return;
	}

	CString strtext;
	if(! m_bSetInterestLimitFinish) 
	{
		
		m_iSetInterestLimitNo1 = m_iBuffSmallWinCenPoint;
		m_bSetInterestLimitFinish = true;
	
		strtext.Format("��һ��E%d,ǁE�ȁEڶ���E ...", m_iSetInterestLimitNo1);
		m_strStateLine2Text.Format("%s","���ø���ȤǁE ");
		m_strStateLine2Text = m_strStateLine2Text+strtext;
        TextScroll(m_strStateLine2Text);
		Redraw();

	}
	else 
	{
		m_bSetInterestLimitFinish = false;
		m_iSetInterestLimitNo2 = m_iBuffSmallWinCenPoint;

		int max, min ;
		if(m_iSetInterestLimitNo1 >= m_iSetInterestLimitNo2)
		{
			max = m_iSetInterestLimitNo1;
			min = m_iSetInterestLimitNo2;
		}
		else 
		{
			max = m_iSetInterestLimitNo2;
			min = m_iSetInterestLimitNo1;
		}

		for(int i=min;i<=max; i++) 
		{
			m_arrExternalRam[i] = (m_arrExternalRam[i] | 0x80000000 );
		}

		int kk = 2*m_iBuffFullChannelWidth/1024;
		UINT max_value = 0;
		for( i=0;i<512;i++) 
		{
			max_value = 0;
			for(int j=i*kk;j<(i+1)*kk;j++) 
			{ //ѡ��kk ���ȷ�Χ�������һ������ֵ
				if (m_arrExternalRam[j] > max_value)
				{
					max_value = m_arrExternalRam[j];
				}
			}
			m_arrFullRam[i] = max_value;
		}

		OnExternal2ExpRam();

		strtext.Format("���ø���Ȥ������߽�Ϊ:%d, �ұ߽�Ϊ:%d", min, max);
		m_strStateLine2Text.Format("%s","             ");
		m_strStateLine2Text = m_strStateLine2Text+strtext;
		TextScroll(m_strStateLine2Text);
		g_iSetInterestLimitNo2 = min;
		g_iSetInterestLimitNo1 = max;
		Redraw();
		memcpy(m_arrBUFFExternalRam,m_arrExternalRam,1024*4*8); ///akang 
	}
     
}

void CMCBView::OnMouseClearInterestRange()
{
	if (m_bMCBModeSel)
	{
		return;
	}

	CString strtext;
	if(! m_bClearInterestLimitFinish) 
	{
		m_iClearInterestLimitNo1 = m_iBuffSmallWinCenPoint;
		m_bClearInterestLimitFinish = true;
	
		strtext.Format("��һ��E%d,ǁE�ȁEڶ���E ...", m_iClearInterestLimitNo1);
		m_strStateLine2Text.Format("%s","�������ȤǁE ");
		m_strStateLine2Text = m_strStateLine2Text+strtext;
		TextScroll(m_strStateLine2Text);		
		Redraw();
	}
	else 
	{
		m_bClearInterestLimitFinish = false;
		m_iClearInterestLimitNo2 = m_iBuffSmallWinCenPoint;

		int max, min ;
		if(m_iClearInterestLimitNo1 >= m_iClearInterestLimitNo2) 
		{
			max = m_iClearInterestLimitNo1;
			min = m_iClearInterestLimitNo2;
		}
		else {
			max = m_iClearInterestLimitNo2;
			min = m_iClearInterestLimitNo1;
		}

		for(int i=min;i<=max; i++) {
			m_arrExternalRam[i] = (m_arrExternalRam[i] & 0x7fffffff );
			m_arrBUFFExternalRam[i] = (m_arrBUFFExternalRam[i] & 0x7fffffff );
	//		m_arrMCBExternalRam[i] = (m_arrMCBExternalRam[i] & 0x7fffffff );////////changing
		}

		int kk = 2*m_iBuffFullChannelWidth/1024;
		UINT max_value = 0;
		for(i=0;i<512;i++) 
		{
			max_value = 0;
			for(int j=i*kk;j<(i+1)*kk;j++) { //ѡ��kk ���ȷ�Χ�������һ������ֵ
				if (m_arrExternalRam[j] > max_value) {
					max_value = m_arrExternalRam[j];
				}
			}
			m_arrFullRam[i] = max_value;
		}

		OnExternal2ExpRam();

		Redraw();

		strtext.Format("��߽�Ϊ:%d, �ұ߽�Ϊ:%d ", min, max);
		m_strStateLine2Text.Format("%s","             ");
		m_strStateLine2Text = m_strStateLine2Text+strtext;
		TextScroll(m_strStateLine2Text);
		Redraw();
		memcpy(m_arrBUFFExternalRam,m_arrExternalRam,1024*4*8); ///akang 
	}

}

void CMCBView::OnMouseCalulateSum()
{

	if(FALSE == OnGetCurInterest_Limits()) {		
		m_strStateLine2Text = " ";
		m_strStateLine3Text = "����Ȥ��ѡ����Ч";
		TextScroll(m_strStateLine3Text);
		Redraw();
		return;
	}

	UINT intcounter = 0;
	CString strtext;	

	for(UINT i= m_iInterest_LCh;i<=m_iInterest_RCh;i++) {
		intcounter += m_arrExternalRam[i] - 0x80000000;
	}	
	strtext.Format("����Ȥ������: %u", intcounter);
    TextScroll(strtext);
	m_strStateLine3Text = strtext;
	m_strStateLine2Text = " ";
	Redraw();
}

void CMCBView::OnMenuWidthStat(int invalue) 
{
	UINT iInputWidth = invalue;
	CString strtext;
	float fOutWidth = 0;

	if (false == OnMouseCalcutNet_ROI_Array()) {
		m_strStateLine2Text = " ";
		m_strStateLine3Text = "���������Ч";
		TextScroll(m_strStateLine3Text);
		Redraw();
		return;
	};

	if(false == OnMouseCalcutPeakWidth(iInputWidth,&fOutWidth)) {
		m_strStateLine2Text = " ";
		m_strStateLine3Text = "����ͳ�Ʋ���������";
		TextScroll(m_strStateLine3Text);
		Redraw();
		return;
	}
	strtext.Format("����Ȥ��λ:%.3f   ����:%.3f",	m_fPeak_Position+ m_iInterest_LCh,m_fPeak_Cnt);
	m_strStateLine2Text.Format("%s","����ͳ��: ");
	m_strStateLine2Text = m_strStateLine2Text+strtext;
	TextScroll(m_strStateLine2Text);
	strtext.Format(" Fw%dm ͳ�ƽṁE%.3f",iInputWidth,fOutWidth);
    m_strStateLine3Text.Format("%s","        ");
	m_strStateLine3Text = m_strStateLine3Text+strtext;
	TextScroll(m_strStateLine3Text);
	Redraw();
			
}

//Input:  m_iSmallWinCenPoint
//OutPut: m_iInterest_LCh,m_iInterest_RCh,,
//		  States  ��ȡ����Ȥ�����ұ߽�E
BOOL CMCBView::OnGetCurInterest_Limits(void)
{
		int iInterestNo = m_iBuffSmallWinCenPoint;  //

		if (m_bMouseDoubleHit)
		{
			m_bMouseDoubleHit = false;
			iInterestNo = m_iMouseDoubleHitNo;
		}

		if ((iInterestNo > m_iBuffFullChannelWidth)||(iInterestNo<0))
		{
			return false;
		}
		if(m_arrExternalRam[iInterestNo] < 0x80000000) 
		{
			return false;
		}

		int i = iInterestNo;
		while ((m_arrExternalRam[i] >= 0x80000000)&&(i>=0))
		{	
			i--;			
		}
		if (i < 0) { m_iInterest_LCh = 0; }
		else m_iInterest_LCh = i+1;


		i = iInterestNo;
		while ((m_arrExternalRam[i] >= 0x80000000)&&(i < m_iBuffFullChannelWidth)) {	
			i++;			
		}
		if (i >= m_iBuffFullChannelWidth) { m_iInterest_RCh = m_iBuffFullChannelWidth-1; }
		else m_iInterest_RCh = i-1;

		return true;
}

void CMCBView::OnMouseCalulateWholeSum()
{
	__int64 intcounter = 0;
	CString strtext;		
	for(int i=0;i<m_iBuffFullChannelWidth;i++) {
		intcounter += (m_arrExternalRam[i] & 0x7fffffff);
	}	
	if (intcounter <= 1000000) {
		strtext.Format("�����: %d", (UINT)intcounter);
		TextScroll(strtext);
	}
	else {
		strtext.Format("�����: %d%6.6d", (UINT)(intcounter/1000000),(UINT)(intcounter%1000000));
		TextScroll(strtext);
	}
	
	m_strStateLine2Text = " ";
	m_strStateLine3Text = strtext;
	Redraw();


}

void CMCBView::OnMouseWholeDataFilter()
{	
	CString strtext;	
	UINT buff[1024*8];
	UINT temp0;
	UINT temp1;
	UINT temp2;

	CString strtime;
	time = CTime::GetCurrentTime();
	strtime.Format("ȫ��ƽ�� %.2d:%.2d:%.2d",time.GetHour(),time.GetMinute(),time.GetSecond());	
	TextScroll(strtime);

	for(int i=1;i<m_iBuffFullChannelWidth;i++) {
		temp0 = (m_arrExternalRam[i-1] & 0x7fffffff);
		temp1 = (m_arrExternalRam[i] & 0x7fffffff);
		temp2 = (m_arrExternalRam[i+1] & 0x7fffffff);
		buff[i] = (temp0 + temp1 + temp2) / 3;
		if (m_arrExternalRam[i] >= 0x80000000) {
			buff[i] = (buff[i] | 0x80000000);
		}
	}	
	
	temp0 = (m_arrExternalRam[0] & 0x7fffffff);
	temp1 = (m_arrExternalRam[1] & 0x7fffffff);
	buff[0] = (temp0 + temp1)/2;
	if (m_arrExternalRam[0] >= 0x80000000) {
		buff[0] = (buff[0] | 0x80000000);
	}

	temp0 = (m_arrExternalRam[m_iBuffFullChannelWidth-2] & 0x7fffffff);
	temp1 = (m_arrExternalRam[m_iBuffFullChannelWidth-1] & 0x7fffffff);
	buff[m_iBuffFullChannelWidth-1] = (temp0 + temp1)/2;

	if (m_arrExternalRam[m_iFullChannelWidth-1] >= 0x80000000) {
		buff[m_iBuffFullChannelWidth-1] = (buff[m_iBuffFullChannelWidth-1] | 0x80000000);
	}

	memcpy(m_arrExternalRam,buff,1024*4*8);

	m_strStateLine2Text = " ";
	time = CTime::GetCurrentTime();
	strtime.Format("ȫ��ƽ��́E� %.2d:%.2d:%.2d",time.GetHour(),time.GetMinute(),time.GetSecond());	
	TextScroll(strtime);	
	Redraw();

}

//����������
BOOL CMCBView::OnMouseCalcutNet_ROI_Array()
{
	if (false == OnGetCurInterest_Limits()) 
	{  //�߽���Ч���˳�E
		return false;
	}

	int lenth = m_iInterest_RCh - m_iInterest_LCh;
	if ( lenth <= 6) 
	{
		return false;
	}
	
	int* buff;
	float k = 0;
	float low_avg = 0, high_avg = 0;
	buff = new int[1024*8];

	memset(m_iNet_ROI_Array,0,1024*4*8);   
	memset(buff,0,1024*4*8);

	for(int index=0;index<=lenth;index++) {
		buff[index] = (m_arrExternalRam[index + m_iInterest_LCh] & 0x7fffffff);
	}

	low_avg = (float)(buff[0] + buff[1] + buff[2])/3; 
	high_avg = (float)(buff[lenth-2] + buff[lenth-1] + buff[lenth])/3; 

	k = (high_avg - low_avg) / (float)(lenth -1);

	for(int i=0;i<=lenth;i++) {		
		m_iNet_ROI_Array[i] = (buff[i] - (int)(low_avg + k*i + 0.5));
	}
	
	delete buff;
	return true;
}

//�����������
BOOL CMCBView::OnMouseCalcutPeakWidth(int indiv, float* outwidth)
{
	float peak_position,peak_cnt;

	if (indiv == 0) {
		return false;
	}

	if(false == OnMousePeak_FIT(&peak_position,&peak_cnt)) {
		return false;	
	}

	m_fPeak_Position = peak_position; //�����λ����
	m_fPeak_Cnt = peak_cnt;

	int L_Chan,R_Chan,Max_Chan;
    float Target,Max,L_Step,R_step;
	float L_Part,L_Fraction,R_Step,R_Part,R_Fraction;
    UINT Index; 

	R_step = 1;
	Max = 0;
	Max_Chan = 0;	
	for(Index=0;Index< (m_iInterest_RCh - m_iInterest_LCh +1);Index++) {
		if (m_iNet_ROI_Array[Index] > Max) {
			Max = (float)m_iNet_ROI_Array[Index];
			Max_Chan = Index;
		}
	}

	L_Chan = Max_Chan; 
	R_Chan = Max_Chan;  //
	Target = ( peak_cnt/ (float)indiv) ;  //ҪǁEָ����������� ����֮������
//	Target = ((float)m_iNet_ROI_Array[Max_Chan]/ indiv);  //ҪǁE����ֵ ����֮������
	bool flag = true;
	while ( (m_iNet_ROI_Array[L_Chan] >= Target)&&(flag) ) {
		if((--L_Chan)<0) {
			 flag = false;
			 L_Chan = 0;
		}
	}
	flag = true;
	while ( (m_iNet_ROI_Array[R_Chan] >= Target)&&(flag)) {
		if((++R_Chan) > 1023){
			flag = false;
			R_Chan = 1023;
		}
	}

	L_Step = (float)(m_iNet_ROI_Array[L_Chan+1] - m_iNet_ROI_Array[L_Chan]);
	L_Part = Target - m_iNet_ROI_Array[L_Chan];
	
	if (L_Step <= 0.00001) {	L_Step = 1;	}
	L_Fraction = L_Part/L_Step;
	R_Step = (float)(m_iNet_ROI_Array[R_Chan-1] - m_iNet_ROI_Array[R_Chan]);
	R_Part = (Target) - m_iNet_ROI_Array[R_Chan];

	if (R_Step <= 0.00001) {	R_Step = 1;	}
	R_Fraction = R_Part/R_Step;
	*outwidth = (R_Chan - R_Fraction)-(L_Chan + L_Fraction);  //output

	return true;
}


//��λ���Ӽ�ˁE
//input: m_iNet_ROI_Array[]   //�۳����׾�ՁE
//input: ����Ȥ���ұ߽�E
//output�� ��ֵ����λ
BOOL CMCBView::OnMousePeak_FIT(float* peak_position, float* peak_cnt)
{
	int L_Chan,High_Chan,R_Chan,Index,Max,Count;
	int Half,Dummy;	
	float Sum_Chan,Sum_Chan2,Sum_Chan3,Sum_Chan4,Trans_Type;
	float Sum_Dat,Sum_Chan_Dat,Sum_Chan2_Dat;
	float Min_11,Min_12,Min_13,Det_Coef,Det_A,Det_B,Det_C;
	float A,B,C,Ln_Dat;

	Max = m_iInterest_RCh - m_iInterest_LCh;
//	Peak_Fit = true;
	High_Chan = 0;

	for(Index=0;Index<=Max;Index++) {
		if (m_iNet_ROI_Array[Index] > m_iNet_ROI_Array[High_Chan]) {
			High_Chan = Index;
		}
	}
	if (m_iNet_ROI_Array[High_Chan] < 2) {
		// ??????
		return false;
	}

	L_Chan = High_Chan; 
	R_Chan = High_Chan;	//2�ַ�����ȡֵ
	Half = m_iNet_ROI_Array[High_Chan] / 2;

	bool flag = true;
	while (((float)m_iNet_ROI_Array[L_Chan] > Half)&&(flag)){		
		if( (--L_Chan) < 0 ) {
			flag = false;
			L_Chan = 0;
		}
	}
	flag = true;
	while (((float)m_iNet_ROI_Array[R_Chan] > Half)&&(flag)) { 		
		if( (++R_Chan) > 1023  ) {
			flag = false;
			R_Chan = 1023;
		}
	}

	Count = R_Chan - L_Chan +1;         //ͳ��2�ַ��Ŀ���
	if (Count < 2) {
		//???
		return false;
	}
	
	Sum_Chan = 0;
	Sum_Chan2 = 0;
	Sum_Chan3 = 0;
	Sum_Chan4 = 0;
	Sum_Dat = 0;
	Sum_Chan_Dat = 0;
	Sum_Chan2_Dat = 0;
			   
	for(Index= L_Chan; Index<= R_Chan;Index++) {

		Dummy = Index;
		Trans_Type = (float)m_iNet_ROI_Array[Index];
		Sum_Chan = Sum_Chan + Dummy;					//ͨ����
		Sum_Dat = (float)(Sum_Dat + log(Trans_Type));   			//ָ����
		Sum_Chan_Dat = (float)(Sum_Chan_Dat + Dummy * log(Trans_Type));// ͨ���š�ָ�� �� ��
		Dummy = Dummy * Index;                    
		Sum_Chan2 = Sum_Chan2 + Dummy;             //ͨ����ƽ�� ��
		Sum_Chan2_Dat = (float)(Sum_Chan2_Dat + Dummy * log(Trans_Type));//ͨ��ƽ���͡�ָ��
		Dummy = Dummy * Index;                    
		Sum_Chan3 = Sum_Chan3 + Dummy;				// ͨ�������� ��
		Dummy = Dummy * Index;
		Sum_Chan4 = Sum_Chan4 + Dummy;              // ͨ����4�η���
	}

	
	Min_11 = Sum_Chan2 * Sum_Chan4 - Sum_Chan3 * Sum_Chan3;   //
	Min_12 = Sum_Chan * Sum_Chan4 - Sum_Chan2 * Sum_Chan3;
	Min_13 = Sum_Chan * Sum_Chan3 - Sum_Chan2 * Sum_Chan2;
									//Count: 2�ַ��Ŀ���
	Det_Coef = Count * Min_11 - Sum_Chan * Min_12 + Sum_Chan2 * Min_13;
	if (Det_Coef <= 0) {
		//????
		return false;
	}

	Min_11 = Sum_Chan2 * Sum_Chan4 - Sum_Chan3 * Sum_Chan3;
	Min_12 = Sum_Chan_Dat * Sum_Chan4 - Sum_Chan2_Dat * Sum_Chan3;
	Min_13 = Sum_Chan_Dat * Sum_Chan3 - Sum_Chan2_Dat * Sum_Chan2;
	Det_A = Sum_Dat * Min_11 - Sum_Chan * Min_12 + Sum_Chan2 * Min_13;
		   
	Min_11 = Sum_Chan_Dat * Sum_Chan4 - Sum_Chan2_Dat * Sum_Chan3;
	Min_12 = Sum_Chan * Sum_Chan4 - Sum_Chan2 * Sum_Chan3;
	Min_13 = Sum_Chan * Sum_Chan2_Dat - Sum_Chan2 * Sum_Chan_Dat;
	Det_B = Count * Min_11 - Sum_Dat * Min_12 + Sum_Chan2 * Min_13;
		   
	Min_11 = Sum_Chan2 * Sum_Chan2_Dat - Sum_Chan3 * Sum_Chan_Dat;
	Min_12 = Sum_Chan * Sum_Chan2_Dat - Sum_Chan2 * Sum_Chan_Dat;
	Min_13 = Sum_Chan * Sum_Chan3 - Sum_Chan2 * Sum_Chan2;
	Det_C = Count * Min_11 - Sum_Chan * Min_12 + Sum_Dat * Min_13;

	
	A = Det_A/Det_Coef;
	B = Det_B/Det_Coef;
	C = Det_C/Det_Coef;
   
	*peak_position = (-B)/(C+C);//��λ
	Ln_Dat = C * (*peak_position) * (*peak_position) + B * (*peak_position) + A;
	*peak_cnt = (float)exp(Ln_Dat);		//��λ�ļ���
		
	return true;
}

void CMCBView::OnMouseReport()
{
	float peak_position,peak_cnt;
	UINT Weight_Sum=0;	
	CString strtext;
	int Net_Sum=0;
	float outwidth;

	if (false == OnMouseCalcutNet_ROI_Array()) {
		m_strStateLine2Text = " ";
		m_strStateLine3Text = "���������Ч";
		TextScroll(m_strStateLine3Text);
		Redraw();
		return;
	};

	//�����
	for(UINT i=0;i<(m_iInterest_RCh-m_iInterest_LCh + 1);i++) {
		Net_Sum += m_iNet_ROI_Array[i];
	}
	//������
	for(i=m_iInterest_LCh;i<m_iInterest_RCh+1;i++) {
		Weight_Sum +=  ( m_arrExternalRam[i] & 0x7fffffff );
	}
	strtext.Format("�����:%u     �����:%u      ��߽�E%d     �ұ߽�E%d",
				Net_Sum,Weight_Sum,m_iInterest_LCh,m_iInterest_RCh);
	m_strStateLine3Text.Format("%s","�����䳁E ");
	m_strStateLine3Text	= m_strStateLine3Text+strtext;
	TextScroll(m_strStateLine3Text);
	
	if(false == OnMousePeak_FIT(&peak_position , &peak_cnt) ) {
		m_strStateLine2Text = "��λͳ����Ч";
		m_strStateLine3Text = " ";
		TextScroll(m_strStateLine2Text);
		Redraw();
		return;
	}; 

	OnMouseCalcutPeakWidth(2, &outwidth);

	//�����̶����� ͬ��	
//	m_fMenuEnergeScale_K = g_HardWare_Pare.fEngerScaleSlope;
//	m_fMenuEnergeScale_b = g_HardWare_Pare.fEngerScaleOffset;

	float kev;
	float fkww;
	if (m_bMenuEnergeScale_CalcuEn) 
	{
		kev = (peak_position + m_iInterest_LCh) * m_fMenuEnergeScale_K + m_fMenuEnergeScale_b ;
		fkww = outwidth * m_fMenuEnergeScale_K;

		switch(m_fMenuEnergeScale_U) //////////////////////////////////changing
		{
			case 0: 
				strtext.Format("��λ:%.3f(%.3f)      ����:%.3f      Fw2m:%.3f(%.3f)",
					peak_position + m_iInterest_LCh,kev,peak_cnt,outwidth,fkww);
				break;
			case 1: 			
				strtext.Format("��λ:%.3f(%.3f ev)     ����:%.3f      Fw2m:%.3f(%.3f ev)",
					peak_position + m_iInterest_LCh,kev,peak_cnt,outwidth,fkww);
				break;
			case 2:
				strtext.Format("��λ:%.3f(%.3f Kev)     ����:%.3f      Fw2m:%.3f(%.3f Kev)",
					peak_position + m_iInterest_LCh,kev,peak_cnt,outwidth,fkww);
				break;
			case 3:
				strtext.Format("��λ:%.3f(%.3f Mev)     ����:%.3f      Fw2m:%.3f(%.3f Mev)",
					peak_position + m_iInterest_LCh,kev,peak_cnt,outwidth,fkww);
				break;
			default:  
				strtext.Format("��λ:%.3f(%.3f)     ����:%.3f      Fw2m:%.3f(%.3f)",
					peak_position + m_iInterest_LCh,kev,peak_cnt,outwidth,fkww);
				break;
		}

	}
	else {
		strtext.Format("��λ:%.3f   ����:%.3f      Fw2m: %.3f(***Kev)",
			peak_position + m_iInterest_LCh,peak_cnt,outwidth);
	}
	
	m_strStateLine2Text.Format("%s","          ");
	m_strStateLine2Text = m_strStateLine2Text+strtext;
	TextScroll(m_strStateLine2Text);
	Redraw();	

}

BOOL CMCBView::OnEraseBkgnd(CDC* pDC) 
{
	return true;
//	return CView::OnEraseBkgnd(pDC);
}

void CMCBView::OnLButtonDblClk(UINT nFlags, CPoint point) ////////////////changing
{
/*	int ioffset = 0;
	if((point.y > m_iViewWindowSize_Y*100/500)&&(point.y < m_iViewWindowSize_Y*420/500)) {	   //�󴰿ڸ߶� 100������420 
		m_strStateLine2Text = " ";
		m_strStateLine3Text = " ";		

		ioffset = (float)(m_iSamllWinWidth * point.x) / m_iViewWindowSize_X;

		if( m_arrExternalRam[m_iSmallWinCHL + ioffset] >= 0x80000000 ) {
			m_iMouseDoubleHitNo	= m_iSmallWinCHL + ioffset;
			m_bMouseDoubleHit = true;
			OnMouseReport();
		}
		else Redraw();
		
	}
*/
	CView::OnLButtonDblClk(nFlags, point);
}

/*
void CMCBView::OnDlgbarButton_2Function() 
{
	if (m_bMenuWidthStat) { //�������ͳ��
		OnMenuWidthStat();
	}
	else {  //�����̶�
		OnMenuProduceEnergeScale();		
	}
}
*/

void CMCBView::OnMenuSelectWidthStat()
{
//	CMainFrame* pMainFrame = (CMainFrame*)GetParentFrame();
//	CButton* pButton = (CButton *)(pMainFrame->m_wndDlgBar.GetDlgItem(IDC_DLGBAR_BUTTON));
	
//	pButton->SetWindowText("����");
//    m_bMenuWidthStat = true;	

}

void CMCBView::OnMenuSelectEnergeScale()
{
//	CMainFrame* pMainFrame = (CMainFrame*)GetParentFrame();
//	CButton* pButton = (CButton *)(pMainFrame->m_wndDlgBar.GetDlgItem(IDC_DLGBAR_BUTTON));
	
//	pButton->SetWindowText(" Kev ");
//    m_bMenuWidthStat = false;	
//	m_bMenuEnergeScaleFinish = false;
	
}

void CMCBView::OnMenuProduceEnergeScale(float fvalue)
{
	float	fInputData = fvalue;
	float   fPeakPosition,fpeak_cnt;


	if (false == OnMouseCalcutNet_ROI_Array()) {
		m_strStateLine2Text = " ";
		m_strStateLine3Text = "�����������Ч";
		TextScroll(m_strStateLine3Text);
		Redraw();
		return;
	};	
	if(false == OnMousePeak_FIT(&fPeakPosition , &fpeak_cnt) ) {
		m_strStateLine2Text = "��λͳ����Ч";
		m_strStateLine3Text = " ";
        TextScroll(m_strStateLine3Text);
		Redraw();
		return;
	}; 	
//	OnMouseCalcutPeakWidth(2, &fPeakPosition);

	CString strtext;
	if (!m_bMenuEnergeScaleFinish)
	{  //������һ��E�̶�
		m_bMenuEnergeScaleFinish = TRUE;
		m_bMenuEnergeScale_CalcuEn = FALSE;
		m_fMenuEnergeScaleFirstVal = fInputData ;
		m_fMenuEnergeScalePeakP1 = fPeakPosition+ m_iInterest_LCh;
		
		switch(m_fMenuEnergeScale_U)
		{
		case 0: 
			strtext.Format("No1��λ:%.3f �̶�(%.3f)     ǁE�ȁEo2�̶�ֵ...",m_fMenuEnergeScalePeakP1,m_fMenuEnergeScaleFirstVal);
			break;
		case 1: 			
			strtext.Format("No1��λ:%.3f �̶�(%.3fev)   ǁE�ȁEo2�̶�ֵ...",m_fMenuEnergeScalePeakP1,m_fMenuEnergeScaleFirstVal);
			
			break;
		case 2:
			strtext.Format("No1��λ:%.3f �̶�(%.3fKev)   ǁE�ȁEo2�̶�ֵ...",m_fMenuEnergeScalePeakP1,m_fMenuEnergeScaleFirstVal);
			break;
		case 3:
			strtext.Format("No1��λ:%.3f �̶�(%.3fMev)   ǁE�ȁEo2�̶�ֵ...",m_fMenuEnergeScalePeakP1,m_fMenuEnergeScaleFirstVal);
			break;
		default:  
			strtext.Format("No1��λ:%.3f �̶�(%.3fKev)   ǁE�ȁEo2�̶�ֵ...",m_fMenuEnergeScalePeakP1,m_fMenuEnergeScaleFirstVal);
			break;
		}
		
		
		m_strStateLine2Text.Format("%s","�����̶�: ");
		m_strStateLine2Text = m_strStateLine2Text+strtext;
		TextScroll(m_strStateLine2Text);
		Redraw();
	}
	else { //�����ڶ���̶�
		m_bMenuEnergeScaleFinish = FALSE;
		m_fMenuEnergeScaleSecondVal = fInputData;
		m_fMenuEnergeScalePeakP2 = fPeakPosition + m_iInterest_LCh;


		switch(m_fMenuEnergeScale_U)
		{
		case 0: 
			strtext.Format("No1��λ:%.3f �̶�(%.3f) No2��λ:%.3f �̶�(%.3f)",m_fMenuEnergeScalePeakP1,m_fMenuEnergeScaleFirstVal,
				m_fMenuEnergeScalePeakP2,m_fMenuEnergeScaleSecondVal);
			break;
		case 1: 			
			strtext.Format("No1��λ:%.3f �̶�(%.3fev) No2��λ:%.3f �̶�(%.3fev)",m_fMenuEnergeScalePeakP1,m_fMenuEnergeScaleFirstVal,
				m_fMenuEnergeScalePeakP2,m_fMenuEnergeScaleSecondVal);
			break;
		case 2:
			strtext.Format("No1��λ:%.3f �̶�(%.3fKev) No2��λ:%.3f �̶�(%.3fKev)",m_fMenuEnergeScalePeakP1,m_fMenuEnergeScaleFirstVal,
				m_fMenuEnergeScalePeakP2,m_fMenuEnergeScaleSecondVal);
			break;
		case 3:
			strtext.Format("No1��λ:%.3f �̶�(%.3fMev) No2��λ:%.3f �̶�(%.3fMev)",m_fMenuEnergeScalePeakP1,m_fMenuEnergeScaleFirstVal,
				m_fMenuEnergeScalePeakP2,m_fMenuEnergeScaleSecondVal);
			break;
		default:  
			strtext.Format("No1��λ:%.3f �̶�(%.3fKev) No2��λ:%.3f �̶�(%.3fKev)",m_fMenuEnergeScalePeakP1,m_fMenuEnergeScaleFirstVal,
				m_fMenuEnergeScalePeakP2,m_fMenuEnergeScaleSecondVal);
			break;
		}

		m_strStateLine2Text.Format("%s","         ");
		m_strStateLine2Text = m_strStateLine2Text+strtext;
        TextScroll(m_strStateLine2Text);
		if (m_fMenuEnergeScalePeakP2 == m_fMenuEnergeScalePeakP1) 
		{	
			m_strStateLine3Text = "�����̶�ϵ����Ч";
			m_strStateLine2Text.Format("%s","         ");
            m_strStateLine2Text = m_strStateLine2Text+m_strStateLine3Text;
			TextScroll(m_strStateLine2Text);
			Redraw();
			return;
		}
		m_fMenuEnergeScale_K = (m_fMenuEnergeScaleSecondVal - m_fMenuEnergeScaleFirstVal) / (m_fMenuEnergeScalePeakP2 - m_fMenuEnergeScalePeakP1);
		m_fMenuEnergeScale_b = 	m_fMenuEnergeScaleSecondVal - m_fMenuEnergeScale_K * m_fMenuEnergeScalePeakP2;
		
		//�����̶����� ͬ��	
///		g_HardWare_Pare.fEngerScaleSlope = m_fMenuEnergeScale_K;
///		g_HardWare_Pare.fEngerScaleOffset = m_fMenuEnergeScale_b;


		strtext.Format("����ϵ��K:%.3f ƫ��b:%.3f ",m_fMenuEnergeScale_K,m_fMenuEnergeScale_b);
		m_strStateLine3Text.Format("%s","         ");
        m_strStateLine3Text = m_strStateLine3Text+strtext;
        TextScroll(m_strStateLine3Text);
		m_bMenuEnergeScale_CalcuEn = TRUE;
		Redraw();
/*
		int intbuff;
		CIni inifile;
		if(inifile.Open("MCBini_05_1028.ini"))
		{ //���������ʾ����
			inifile.Write("MCB HardWare", "fEngerScaleOffset_Int", g_HardWare_Pare.fEngerScaleOffset);
			intbuff = (int)(g_HardWare_Pare.fEngerScaleOffset*10000.) % 10000;
			inifile.Write("MCB HardWare", "fEngerScaleOffset_Dot",intbuff);
			
			inifile.Write("MCB HardWare", "fEngerScaleSlope_Int", g_HardWare_Pare.fEngerScaleSlope);
			intbuff = (int)(g_HardWare_Pare.fEngerScaleSlope*10000.) % 10000;
			inifile.Write("MCB HardWare", "fEngerScaleSlope_Dot", intbuff);
			inifile.Write("MCB HardWare", "iEngerScaleUnit", g_HardWare_Pare.iEngerScaleUnit);

			inifile.Save();
			inifile.Close();
		}
*/

	}


}

BOOL CMCBView::DateToSecond(UINT *second,MYDATE *date)
{
	int table_month[12] = {31,28,31,30,31,30,31,31,30,31,30,31}; 
	if (date->year < 1970) {
		return false;
	}
	*second = 0;
	int i;
	for(i=1970;i<date->year;i++) {
		if (i%4 == 0) {	*second += 366*24*3600;	}
		else *second += 365*24*3600;
	}
	for(i=1;i<date->month;i++) {
		if((i==2)&&(date->year%4 == 0)) { *second += 29*24*3600;	}
		else *second += table_month[i-1]*24*3600;
	}
	*second += (date->day-1)*24*3600;
	*second += date->hour* 3600;
	*second += date->minite * 60;
	*second += date->second;
	return true;
}

BOOL CMCBView::SecondToDate(MYDATE *date, UINT *second)
{
	int table_month[12] = {31,28,31,30,31,30,31,31,30,31,30,31}; 	
	if(*second < 0) return false;
	int iday = (*second)/(24*3600);
	date->year = 1970;
	date->month = 1;
	date->day = 1;
	date->hour = 0;
	date->minite = 0;
	date->second = 0;
	while (1) {
		if (date->year % 4 == 0) {
			if(iday<366) break;		
			iday -= 366;			
		}else {
			if(iday < 365) break;
			iday -= 365;
		}		
		date->year ++;
	}	
	int i=1;
	while (1) {		
		if (iday < table_month[i-1]) {
			break;
		}
		if ((i==2)&&(date->year % 4 == 0)){
			iday -= 29;
		}else iday -= table_month[i-1];
		i++;
		date->month ++;	
	}
	date->day += iday;
	*second = (*second)%(24*3600);
	date->hour = (*second) /3600 ;
	*second = (*second) %3600;
	date->minite = (*second)/60;
	date->second = (*second)%60;
	return true;	
}

void CMCBView::OnMenuWholedataFilter() 
{
	OnMouseWholeDataFilter();	
}

void CMCBView::OnMenuWholedataSum() 
{
	OnMouseCalulateWholeSum();
}

void CMCBView::OnMenuOutReport() 
{
	OnMouseReport();	
}

void CMCBView::OnMenuInterestSum() 
{
	OnMouseCalulateSum();
}

void CMCBView::OnMenuLinePointSel() 
{
	m_bDrawLine_Sel = !m_bDrawLine_Sel;
	Redraw();
	m_ScrollBar.ShowScrollBar(FALSE);
	m_ScrollBar.ShowScrollBar(TRUE);	
} 

//����/���� ��ʾѡԁE
void CMCBView::OnMenuLogShow() 
{
	m_bDrawLogData_Sel = TRUE;
	Redraw();

	//������ֵ��ʾ����Ϊ ������ʽlog
	CMainFrame* pMainFrame = (CMainFrame*)GetParentFrame();
	CComboBox* pComboxVer = (CComboBox *)(pMainFrame->m_wndDlgBar.GetDlgItem(IDC_COMBO_VER));
	pComboxVer->ResetContent();
	pComboxVer->AddString("Log");
	pComboxVer->SetCurSel(0);

	CComboBox* pComboxExpVer = (CComboBox *)(pMainFrame->m_wndDlgBar.GetDlgItem(IDC_COMBO_EXP_VER));
	pComboxExpVer->ResetContent();
	pComboxExpVer->AddString("Log");
	pComboxExpVer->SetCurSel(0);

	m_ScrollBar.ShowScrollBar(FALSE);
	m_ScrollBar.ShowScrollBar(TRUE);

}
void CMCBView::OnMenuLineShow() 
{
	//����������ֵ��ʾ����Ϊ ���Է�ʽ
	CMainFrame* pMainFrame = (CMainFrame*)GetParentFrame();
	CComboBox* pComboxVer = (CComboBox *)(pMainFrame->m_wndDlgBar.GetDlgItem(IDC_COMBO_VER));
	pComboxVer->ResetContent();	
	pComboxVer->AddString("8192k");
	pComboxVer->AddString("4096k");
	pComboxVer->AddString("2048k");
	pComboxVer->AddString("1024k");
	pComboxVer->AddString("512k");
	pComboxVer->AddString("256k");
	pComboxVer->AddString("128k");
	pComboxVer->AddString("64k");
	pComboxVer->AddString("32k");
	pComboxVer->AddString("16k");
	pComboxVer->AddString("8k");
	pComboxVer->AddString("4k");
	pComboxVer->AddString("2k");
	pComboxVer->AddString("1024");
	pComboxVer->AddString("512");
	pComboxVer->AddString("256");
	pComboxVer->AddString("128");
	pComboxVer->AddString("64");

	pComboxVer->SetCurSel(m_iBuffComboxSel);
    

    CComboBox* pComboxExpVer = (CComboBox *)(pMainFrame->m_wndDlgBar.GetDlgItem(IDC_COMBO_EXP_VER));
	pComboxExpVer->ResetContent();	
	pComboxExpVer->AddString("8192k");
	pComboxExpVer->AddString("4096k");
	pComboxExpVer->AddString("2048k");
	pComboxExpVer->AddString("1024k");
	pComboxExpVer->AddString("512k");
	pComboxExpVer->AddString("256k");
	pComboxExpVer->AddString("128k");
	pComboxExpVer->AddString("64k");
	pComboxExpVer->AddString("32k");
	pComboxExpVer->AddString("16k");
	pComboxExpVer->AddString("8k");
	pComboxExpVer->AddString("4k");
	pComboxExpVer->AddString("2k");
	pComboxExpVer->AddString("1024");
	pComboxExpVer->AddString("512");
	pComboxExpVer->AddString("256");
	pComboxExpVer->AddString("128");
	pComboxExpVer->AddString("64");

    pComboxExpVer->SetCurSel(m_iBuffExpRamCheckSel);

	m_bDrawLogData_Sel = FALSE;
	
	m_bAutoCalcuExpRamEth = TRUE;
	m_bAutoCalcuFullRamEth = TRUE;
/*
   int index=0;
	CButton* pFULL_BUF = (CButton *)(pMainFrame->m_wndDlgBar.GetDlgItem(IDC_FULLRAM_VIEWSEL));
	m_iFullRamEthValue=OnCaculateFullRamEthValue();
	if(pFULL_BUF->GetCheck())			
	{	int jjj=m_iFullRamEthValue/64;
		for(int j=0;j<=17;j++)
		{
			
			jjj=jjj/2;
			if (jjj==1) break;
		}
        index=17-j-1;
		m_iFullRamCheckSel =index;

		pComboxVer->SetCurSel(m_iFullRamCheckSel);
	}
	else 
	{
		m_iExpRamEthValue=OnCaculateExpRamEthValue();
		int kkk=m_iExpRamEthValue/64;
		for(int j=0;j<=17;j++)
		{
			
			kkk=kkk/2;
			if (kkk==1) break;
		}
        index=17-j-1;
		m_iBuffComboxSel =index;
		
        pComboxVer->SetCurSel(m_iBuffComboxSel);
	}	
*/

	Redraw();

	m_bAutoCalcuExpRamEth = FALSE;
	m_bAutoCalcuFullRamEth = FALSE;

	m_ScrollBar.ShowScrollBar(FALSE);
	m_ScrollBar.ShowScrollBar(TRUE);
}

void CMCBView::OnMenuClearBuff() 
{
	m_iTimeFileSourceflag=2;
    time = CTime::GetCurrentTime();
	if (m_bMCBModeSel) 
	{
		if (!g_MCB_Oper.bQueryEn) 
		{ //����ѯMCB��ʱ���������ǁE		
			for(int i=0;i<1024*8;i++) 
			{
			//	m_arrMCBExternalRam[i] &= 0x80000000;
			//	m_arrExternalRam[i] &= 0x80000000;
				m_arrMCBExternalRam[i] &= 0x00000000;
				m_arrExternalRam[i] &= 0x00000000;
				g_HardWare_Pare.fEngerScaleOffset = 0;
				g_HardWare_Pare.fEngerScaleSlope = 0;
				g_HardWare_Pare.iEngerScaleUnit = 0;
                
			}
		CString strtime;
		strtime.Format("���MCB������ %.2d:%.2d:%.2d",time.GetHour(),time.GetMinute(),time.GetSecond());
		TextScroll(strtime);
		}
	}
	else 
	{
		for(int i=0;i<1024*8;i++) 
		{//m_iFullChannelWidth

			m_arrBUFFExternalRam[i] &= 0x00000000;
			m_arrExternalRam[i] &= 0x00000000;
			m_fMenuEnergeScale_U = 0;
            m_fMenuEnergeScale_K = 0;
			m_fMenuEnergeScale_b = 0;
		}
		CString strtime;
		strtime.Format("���BUF������ %.2d:%.2d:%.2d",time.GetHour(),time.GetMinute(),time.GetSecond());
		TextScroll(strtime);
	}
	Redraw();	
}



void CMCBView::OnPrint(CDC* pDC, CPrintInfo* pInfo) 
{
	// TODO: Add your specialized code here and/or call the base class

//		CView::OnPrint(pDC, pInfo);	//ԭ���Ŀ�Ҫ��
		
	    CRect rect;
		GetClientRect(&rect);
		CBrush brush(RGB(255,255,255));
		CPen pen(PS_SOLID,1,RGB(255,255,255));
		pDC->SelectObject(&pen);
		pDC->SelectObject(&brush);
		pDC->Rectangle(&rect);
		brush.DeleteObject();

		
/*	    PrintMyDraw(pDC);		
		
		OnPrintCoo(pDC);
		
		*/
		//����ר�Ŵ�ӡ�ĺ���
		
    	PrintPageHeader(pDC);
        PrintMyDraw(pDC);
		OnPrintCoo(pDC);
		PrintPageFooter(pDC);

}

void CMCBView::OnPrepareDC(CDC* pDC, CPrintInfo* pInfo) 
{
//	pDC->SetViewportOrg(75,300);  // ͼ����E
	//pDC->SetViewportOrg(75,400);  // ͼ����E

	pDC->SetMapMode(MM_ANISOTROPIC);
	//ת������E��䷽ʽ
	CRect rect;
	GetWindowRect(&rect);
	int x=rect.Width();
	int y=rect.Height();
    pDC->SetWindowExt(x,y); // ȷ�����ڴ�С
    //�õ�ʵ���豸ÿ�߼�Ӣ�����������
    int xLogPixPerInch = pDC->GetDeviceCaps(LOGPIXELSX); 
    int yLogPixPerInch = pDC->GetDeviceCaps(LOGPIXELSY);
    //�õ��豸����E��߼�����Eı���
    long xExt = (long)x * xLogPixPerInch/68; //68
    long yExt = (long)y * yLogPixPerInch/96 ;//
	pDC->SetViewportExt((int)xExt, (int)yExt);
//    pDC->SetViewportOrg(75,yExt/7);
   //	CView::OnPrepareDC(pDC, pInfo);
}

void CMCBView::PrintPageHeader(CDC *pDC)
{
	CString	 str;	
	CFont	fonttext;
	fonttext.CreateFont(30,10,0,0,400,FALSE,FALSE,0,
						ANSI_CHARSET,OUT_DEFAULT_PRECIS,
						CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,
						DEFAULT_PITCH | FF_SWISS ,"Arial");

	pDC->SelectObject(&fonttext);

	pDC->SetBkColor(RGB(255,255,255));
	pDC->SetTextColor(RGB(0,0,0));

//	pDC->SetViewportOrg(0,15);  // ҳü��E
    pDC->SetViewportOrg(0,0);
	CPoint point(0,0);
	pDC->TextOut(point.x+300,point.y,"MCB������ݷ�΁E);
    fonttext.DeleteObject();
	CPen pShow(PS_SOLID,2,RGB(0,0,0));
	pDC->SelectObject(&pShow);
//	pDC->SetViewportOrg(75,400);  // ͼ����E
    pDC->SetViewportOrg(75,300);

}

void CMCBView::PrintPageFooter(CDC *pDC)
{

    pDC->SetViewportOrg(0,0);
    float ver;
	ver=560;
	CPoint point(0,0);
	if(m_1==1)
	{
		pDC->TextOut(point.x+30,ver,m_strStateLine4Text);
	}
	CString strText;
	
	if(m_3==1)
	{
		strText.Format("ʵʱ�䣺%d��%dÁE   ��汼䣺%d��%dÁE,m_iSaveFileRealTime/60,m_iSaveFileRealTime%60,m_iSaveFileLiveTime/60,m_iSaveFileLiveTime%60);
		pDC->TextOut(point.x+30,580,strText);
	}
    
}

void CMCBView::OnHardwareParaSet() 
{
	CMainFrame* pMainFrame = (CMainFrame*)GetParentFrame();
	CComboBox* pComboxHor = (CComboBox *)(pMainFrame->m_wndDlgBar.GetDlgItem(IDC_COMBO_HOR));
	CEdit* pSetTime = (CEdit*)(pMainFrame->m_wndDlgBar.GetDlgItem(IDC_TIMER_EDIT));
	
	if (g_ComPortSel == USB_PORT) {
		CUsbParaSet usbparaset;
		if(usbparaset.DoModal() == IDOK){
			g_hUsb = OpenUsbDevice();
			if (g_hUsb == INVALID_HANDLE_VALUE) {
				AfxMessageBox("USB�˿ڴ򿪴���\r\nǁE�E�ͨ����.");
				return;
			}

			OnUsbParaSet(TRUE);	
			
			CloseUsbDevice(g_hUsb);

			m_iFullChannelWidth = g_HardWare_Pare.iChannelWidth;
			m_iSmallWinCenPoint = m_iFullChannelWidth/2;
			m_iSmallWinCHL = m_iSmallWinCenPoint - m_iSamllWinWidth/2 ;
			m_iSmallWinCHR = m_iSmallWinCenPoint + m_iSamllWinWidth/2 ;	
//			if (!m_bExpRamViewSel) {
				switch(g_HardWare_Pare.iChannelWidth) {
				case 1024: pComboxHor->SetCurSel(0); break;
				case 2048: pComboxHor->SetCurSel(1); break;
				case 4096: pComboxHor->SetCurSel(2); break;
				case 8192: pComboxHor->SetCurSel(3); break;
				default: break;
//				} 			
			}

			g_hUsb = INVALID_HANDLE_VALUE;
			pStatus->SetPaneText(2,"Ӳ����������́E�",1);

			Redraw();
		}		
		return;
	}

	
	CString m_strSettime;
	pSetTime->GetWindowText(m_strSettime);
	if(!m_strSettime.IsEmpty()) {		
		m_iSetSampleTime =atol(m_strSettime); 
	}else m_iSetSampleTime = 0x7fffff00;
	
	//�����Ʋ����ŵ���һ���ṹ�У������滻ԭ���ı���
	g_HardWare_Pare.iSampleTimeValue = m_iSetSampleTime; 

    m_hHardWareModelDlg = m_pHardWarePara->GetSafeHwnd();
	if(m_hHardWareModelDlg == NULL) {  //��ֻ֤����һ������		
		m_pHardWarePara->Create();		
	}

}

void CMCBView::OnHardWareParaUnpack(unsigned char *pdata, int size)
{
	unsigned char recdata[30];
	memcpy(recdata,pdata,size);
	//����E
	type.intdata = 0;
	type.uchdata[0] = recdata[0];
	type.uchdata[1] = recdata[1];
	g_HardWare_Pare.iChannelWidth = type.intdata;    
	//�ɼ�ʱ��ֵ  ʱ��ĵ�λΪ 1/100 s 
	type.uchdata[0] = recdata[2];
	type.uchdata[1] = recdata[3];
	type.uchdata[2] = recdata[4];
	type.uchdata[3] = recdata[5];
	g_HardWare_Pare.iSampleTimeValue = type.intdata / 100; 
	
	//�����̶�б��   	//pdata[6]
					    //pdata[7]
	type.uchdata[0] = recdata[8];
	type.uchdata[1] = recdata[9];
	type.uchdata[2] = recdata[10];
	type.uchdata[3] = recdata[11];
	g_HardWare_Pare.fEngerScaleSlope = type.intdata;
	float fdata;
	type.intdata = 0;
	type.uchdata[0] = recdata[12];
	type.uchdata[1] = recdata[13];
	fdata = (float)type.intdata/1000.;
	g_HardWare_Pare.fEngerScaleSlope += fdata;
					
	//�����̶�ƫ��	//recdata[14]
					//recdata[15]
	type.uchdata[0] = recdata[16];
	type.uchdata[1] = recdata[17];
	type.uchdata[2] = recdata[18];
	type.uchdata[3] = recdata[19];
	g_HardWare_Pare.fEngerScaleOffset = type.intdata;	
	type.intdata = 0;
	type.uchdata[0] = recdata[20];
	type.uchdata[1] = recdata[21];
	fdata = (float)type.intdata/1000.;
	g_HardWare_Pare.fEngerScaleOffset += fdata;

	//�ⰁE�����������E
    if ((recdata[22] & 0x80) == 0x80) {
		g_HardWare_Pare.bSampleModeRealTime = true;
    }
	else g_HardWare_Pare.bSampleModeRealTime = false;

	if ( (recdata[22] & 0x40) == 0x40) {
		g_HardWare_Pare.bDT_Pulse = true;
    }
	else g_HardWare_Pare.bDT_Pulse = false;
	if ((recdata[22] & 0x20) == 0x20) {
		g_HardWare_Pare.bREADY_Pulse = true;
    }
	else g_HardWare_Pare.bREADY_Pulse = false;
	if ((recdata[22] & 0x10) == 0x10) {
		g_HardWare_Pare.bANSER_Pulse = true;
    }
	else g_HardWare_Pare.bANSER_Pulse = false;
	if ((recdata[22] & 0x08) == 0x08) {
		g_HardWare_Pare.bENADC_Pulse = true;
    }
	else g_HardWare_Pare.bENADC_Pulse = false;

	if ((recdata[22] & 0x04) == 0x04) {
		g_HardWare_Pare.bDataPolarity_Pulse = true;
    }
	else g_HardWare_Pare.bDataPolarity_Pulse = false;

	int midbuff = ( recdata[22] & 0x03);
	g_HardWare_Pare.iEngerScaleUnit = midbuff;	

	Beep(2000,200);
	
}

LRESULT CMCBView::OnEnergeKeduGoodBye(WPARAM wParam, LPARAM lParam)
{
	if(wParam == IDCANCEL) {	
		m_pEnergeKedu->DestroyWindow();
	}	
	m_ScrollBar.ShowScrollBar(FALSE);
	m_ScrollBar.ShowScrollBar(TRUE);
	return 0;
}

LRESULT CMCBView::OnEnergeKeduIDok(WPARAM wParam, LPARAM lParam)
{
	int ivalue = (int)wParam;
	int idot = (int)lParam;
//	g_HardWare_Pare.iEngerScaleUnit = m_pEnergeKedu->m_iEnergeUnit;
    m_fMenuEnergeScale_U=m_pEnergeKedu->m_iEnergeUnit;
	
	float fvalue = ivalue + (float)idot/10000;
	
   
	m_pEnergeKedu->DestroyWindow();	
    OnMenuProduceEnergeScale(fvalue);

	m_ScrollBar.ShowWindow(FALSE);
	m_ScrollBar.ShowWindow(TRUE);
	return 0;
}

LRESULT CMCBView::OnWidthStatIDok(WPARAM wParam, LPARAM lParam)
{
	int ivalue = (int)wParam;

	OnMenuWidthStat(ivalue); // ����ͳ��

	m_pWidthStat->DestroyWindow();	

	return 0;
}
LRESULT CMCBView::OnWidthStatGoodBye(WPARAM wParam, LPARAM lParam)
{
	if(wParam == IDCANCEL) {	
		m_pWidthStat->DestroyWindow();
	}

	m_ScrollBar.ShowScrollBar(FALSE);
	m_ScrollBar.ShowScrollBar(TRUE);

	return 0;
}

LRESULT CMCBView::OnHardWareGoodBye(WPARAM wParam, LPARAM lParam)
{
	//������ DlgBarӁE��������Ի�������ݽ���ͬ��������//
	CMainFrame* pMainFrame = (CMainFrame*)GetParentFrame();
	CButton* pRealTimeSel = (CButton *)
		(pMainFrame->m_wndDlgBar.GetDlgItem(IDC_REALTIME_SEL));
	CButton* pLiveTimeSel = (CButton *)
		(pMainFrame->m_wndDlgBar.GetDlgItem(IDC_LIVETIME_SEL));
	CButton* pSampleTimeValue = (CButton *)
		(pMainFrame->m_wndDlgBar.GetDlgItem(IDC_TIMER_EDIT));
	CComboBox* pComboxHor = (CComboBox *)
		(pMainFrame->m_wndDlgBar.GetDlgItem(IDC_COMBO_HOR));

	CString strtext;
	strtext.Format("%d",g_HardWare_Pare.iSampleTimeValue);
	pSampleTimeValue->SetWindowText(strtext);

    m_iFullChannelWidth = g_HardWare_Pare.iChannelWidth;
//	if (!m_bExpRamViewSel) {
		switch(g_HardWare_Pare.iChannelWidth) {
		case 1024: pComboxHor->SetCurSel(0); break;
		case 2048: pComboxHor->SetCurSel(1); break;
		case 4096: pComboxHor->SetCurSel(2); break;
		case 8192: pComboxHor->SetCurSel(3); break;
		default: break;
		} 			
//	}	

	if (g_HardWare_Pare.bSampleModeRealTime) {
		pRealTimeSel->SetCheck(1);
		pLiveTimeSel->SetCheck(0);
	}
	else{
		pRealTimeSel->SetCheck(0);
		pLiveTimeSel->SetCheck(1);
	}
	//����������Ϊ  DlgBarӁE��������Ի�������ݽ���ͬ��������//

	if(wParam == IDCANCEL) 
	{	
		m_pHardWarePara->DestroyWindow();
	}

	//�����̶����� ͬ��	
//	m_fMenuEnergeScale_K = g_HardWare_Pare.fEngerScaleSlope;   /////changing
//	m_fMenuEnergeScale_b = g_HardWare_Pare.fEngerScaleOffset;	

	m_strStateLine2Text = " ";
	m_strStateLine3Text = " ";
	m_strStateLine4Text = " ";
	Redraw();
	
	m_ScrollBar.ShowScrollBar(FALSE);
	m_ScrollBar.ShowScrollBar(TRUE);

	return 0L;
}

void CMCBView::OnHandleSelectBuffmode()
{
	//��ʾ����ͳ�ƶԻ���E
	CMainFrame* pMainFrame = (CMainFrame*)GetParentFrame();
	CButton* pmcbSel = (CButton *)(pMainFrame->m_wndDlgBar.GetDlgItem(IDC_MCB_MODSEL));
	CButton* pbuffSel = (CButton *)(pMainFrame->m_wndDlgBar.GetDlgItem(IDC_BUFF_MODSEL));
	CButton* pMCB_TO_BUF = (CButton *)(pMainFrame->m_wndDlgBar.GetDlgItem(IDC_EXTERNALDATA_MCB2BUFF));
	
	pmcbSel->SetCheck(0);
	pbuffSel->SetCheck(1);

//	pMCB_TO_BUF->ShowWindow(FALSE);
	
	g_MCB_Oper.iStartQueryNo = BUFF_START_QUERYNO;
	g_MCB_Oper.iCurQueryNo = BUFF_START_QUERYNO;
	
	m_bMCBModeSel= FALSE;
	
	memcpy(m_arrExternalRam,m_arrBUFFExternalRam,1024*4*8);
	OnBuffModsel();
	Redraw();
	m_ScrollBar.ShowWindow(FALSE);
	m_ScrollBar.ShowWindow(TRUE);

}

BOOL CMCBView::OnHandChangeSmallWin(int pointx, int pointy)
{
   
  if (m_bMCBModeSel)
  {
	int Xoffset = m_iSamllWinWidth * (pointx - m_iViewWindowSize_X/2) /m_iViewWindowSize_X; 

	int cenpoint = m_iSmallWinCenPoint + Xoffset;

	float maxvalue = 1;
	maxvalue = OnCaculateExpRamEthValue();
	int *go2one;
	go2one= new int [8193];
	if (m_bDrawLogData_Sel) 
	{
		for(int i=0;i<m_iSamllWinWidth;i++) 
		{
			go2one[i] = (float)m_iViewWindowSize_Y*418.0/500 - (float)m_iViewWindowSize_Y*(325.0/500)*(m_farrExpRam[i]/(float)(maxvalue));
			if(go2one[i] > (float)m_iViewWindowSize_Y*418/500) go2one[i] = m_iViewWindowSize_Y*418/500;
			if(go2one[i] < (float)m_iViewWindowSize_Y*100/500) go2one[i] = m_iViewWindowSize_Y*100/500;
		}
	}
	else{
		for(int i=0;i<m_iSamllWinWidth;i++) {
			go2one[i] = m_iViewWindowSize_Y*418/500 - m_iViewWindowSize_Y*325/500*((float)(m_arrExpRam[i] & 0x7fffffff)/maxvalue);
			if(go2one[i] > m_iViewWindowSize_Y*418/500) go2one[i] = m_iViewWindowSize_Y*418/500;
			if(go2one[i] < m_iViewWindowSize_Y*100/500) go2one[i] = m_iViewWindowSize_Y*100/500;
		}
	}
	
	//�����ж���E���� λ���Ƿ�Ϊ����Ȥ�ڣ����Ǿͷ��ش��E
/*	int kkk = (float)pointx / ( (float)m_iViewWindowSize_X/(float)m_iSamllWinWidth ); 

	if((m_arrExpRam[kkk] >= 0x80000000) && (m_arrExpRam[m_iSamllWinWidth/2] >= 0x80000000)) {			
		int maxpos = m_iSamllWinWidth/2;
		int minpos = kkk;
		BOOL flag = FALSE;
		if (kkk > m_iSamllWinWidth/2) { 
			maxpos = kkk;
			minpos = m_iSamllWinWidth/2;
		}
		for(int k=minpos;((k<maxpos)&&(!flag));k++){
			if(m_arrExpRam[k] < 0x80000000) {
				flag = TRUE;	
			}
		}
		
		if( (pointy > go2one[kkk]) && (flag == FALSE) ) {
			delete go2one;
			return FALSE;
		}		
	}	



	delete go2one;
	
*/	
	m_iSmallWinCenPoint = m_iSmallWinCenPoint + Xoffset;
	m_iSmallWinCHL = m_iSmallWinCenPoint - m_iSamllWinWidth/2;
	m_iSmallWinCHR = m_iSmallWinCenPoint + m_iSamllWinWidth/2;

	//����E��ж�
	if ((cenpoint < m_iSamllWinWidth/2)&&(cenpoint > 0)) 
	{
		m_iSmallWinCHL = 0;
	}
	if (cenpoint >= m_iFullChannelWidth-1 - m_iSamllWinWidth/2) 
	{
		m_iSmallWinCHR = m_iFullChannelWidth-1;
	}

	//����жϣ�����ֵ�ж�
	if (cenpoint <= 0) {	
		m_iSmallWinCenPoint = 0;
		m_iSmallWinCHL = 0;
		m_iSmallWinCHR = m_iSamllWinWidth/2;		
	}
	if (m_iSmallWinCHL<0)
	{
		m_iSmallWinCHL=0;
	}
	if (cenpoint >=(int)(m_iFullChannelWidth-1))
	{
		m_iSmallWinCenPoint = m_iFullChannelWidth-1;
		m_iSmallWinCHL = m_iSmallWinCenPoint - m_iSamllWinWidth/2;
		m_iSmallWinCHR = m_iFullChannelWidth-1;		
	}
		
    CString strbuf;
	strbuf.Format("��ַ:  %d (����)     ����: %d        ������߽�E%d  �����ұ߽�E%d",m_iSmallWinCenPoint,(m_arrExpRam[(m_iSamllWinWidth/2)] & 0x7fffffff), m_iSmallWinCHL,m_iSmallWinCHR);
	m_strStateLine1Text = strbuf;		
	m_bExpRamLongNew_valid = true;
	m_ExpRamDataLong_new = (m_iSmallWinCHR - m_iSmallWinCHL +1)*3;

	//��ǰϵͳ���� MCB��ʽ���������ڲɼ�����ʱ����ʱ�轫FullRam������ת����ֵ�� ExpRam
	OnFullRam2ExpRamInsertVal();
//	ReDrawExpLine();
	return TRUE;
  }
  else
  {
	int Xoffset  = ((int)m_iBuffSmallWindowWidth) * (pointx - m_iViewWindowSize_X/2) /m_iViewWindowSize_X;
	  
	  int cenpoint = m_iBuffSmallWinCenPoint + Xoffset;

	float maxvalue = 1;
	maxvalue = OnCaculateExpRamEthValue();
	int *go2one;
	go2one= new int [8193];
	if (m_bDrawLogData_Sel) 
	{
		for(int i=0;i<m_iBuffSmallWindowWidth;i++) 
		{
			go2one[i] = (float)m_iViewWindowSize_Y*418.0/500 - (float)m_iViewWindowSize_Y*(325.0/500)*(m_farrExpRam[i]/(float)(maxvalue));
			if(go2one[i] > (float)m_iViewWindowSize_Y*418/500) go2one[i] = m_iViewWindowSize_Y*418/500;
			if(go2one[i] < (float)m_iViewWindowSize_Y*100/500) go2one[i] = m_iViewWindowSize_Y*100/500;
		}
	}
	else{
		for(int i=0;i<m_iBuffSmallWindowWidth;i++) {
			go2one[i] = m_iViewWindowSize_Y*418/500 - m_iViewWindowSize_Y*325/500*((float)(m_arrExpRam[i] & 0x7fffffff)/maxvalue);
			if(go2one[i] > m_iViewWindowSize_Y*418/500) go2one[i] = m_iViewWindowSize_Y*418/500;
			if(go2one[i] < m_iViewWindowSize_Y*100/500) go2one[i] = m_iViewWindowSize_Y*100/500;
		}
	}
	
	//�����ж���E���� λ���Ƿ�Ϊ����Ȥ�ڣ����Ǿͷ��ش��E
/*	int kkk = (float)pointx / ( (float)m_iViewWindowSize_X/(float)m_iBuffSmallWindowWidth ); 

	if((m_arrExpRam[kkk] >= 0x80000000) && (m_arrExpRam[m_iBuffSmallWindowWidth/2] >= 0x80000000)) {			
		int maxpos = m_iBuffSmallWindowWidth/2;
		int minpos = kkk;
		BOOL flag = FALSE;
		if (kkk > m_iBuffSmallWindowWidth/2) { 
			maxpos = kkk;
			minpos = m_iBuffSmallWindowWidth/2;
		}
		for(int k=minpos;((k<maxpos)&&(!flag));k++){
			if(m_arrExpRam[k] < 0x80000000) {
				flag = TRUE;	
			}
		}
		
		if( (pointy > go2one[kkk]) && (flag == FALSE) ) {
			delete go2one;
			return FALSE;
		}		
	}	



	delete go2one;
*/		
	m_iBuffSmallWinCenPoint = m_iBuffSmallWinCenPoint + Xoffset;
	m_iBuffSmallWinCHL = m_iBuffSmallWinCenPoint - m_iBuffSmallWindowWidth/2;
	m_iBuffSmallWinCHR = m_iBuffSmallWinCenPoint + m_iBuffSmallWindowWidth/2;

	//����E��ж�
	if ((cenpoint < m_iBuffSmallWindowWidth/2)&&(cenpoint > 0)) 
	{
		m_iBuffSmallWinCHL = 0;
	}
	if (m_iBuffSmallWinCHL<0)
	{
       m_iBuffSmallWinCHL=0;
	}
	if (cenpoint >= m_iBuffFullChannelWidth-1 - m_iBuffSmallWindowWidth/2) 
	{
		m_iBuffSmallWinCHR = m_iBuffFullChannelWidth-1;
	}

	//����жϣ�����ֵ�ж�
	if (cenpoint <= 0) 
	{	
		m_iBuffSmallWinCenPoint = 0;
		m_iBuffSmallWinCHL = 0;
		m_iBuffSmallWinCHR = m_iBuffSmallWindowWidth/2;		
	}
	if (cenpoint >=(int)(m_iBuffFullChannelWidth-1))
	{
		m_iBuffSmallWinCenPoint = m_iBuffFullChannelWidth-1;
		m_iBuffSmallWinCHL = m_iBuffSmallWinCenPoint - m_iBuffSmallWindowWidth/2;
		m_iBuffSmallWinCHR = m_iBuffFullChannelWidth-1;		
	}	
//	  ReDrawExpLine();
	  return TRUE;
  }
}

void CMCBView::OnHandleSelectMCBmode()
{
	//��ʾ����ͳ�ƶԻ���E
	CMainFrame* pMainFrame = (CMainFrame*)GetParentFrame();
	CButton* pmcbSel = (CButton *)(pMainFrame->m_wndDlgBar.GetDlgItem(IDC_MCB_MODSEL));
	CButton* pbuffSel = (CButton *)(pMainFrame->m_wndDlgBar.GetDlgItem(IDC_BUFF_MODSEL));
	CButton* pMCB_TO_BUF = (CButton *)(pMainFrame->m_wndDlgBar.GetDlgItem(IDC_EXTERNALDATA_MCB2BUFF));
	
	pmcbSel->SetCheck(1);
	pbuffSel->SetCheck(0);
	
//	pMCB_TO_BUF->ShowWindow(TRUE);

	
	g_MCB_Oper.iStartQueryNo = MCB_START_QUERYNO;
	g_MCB_Oper.iCurQueryNo = MCB_START_QUERYNO;
	
	m_bMCBModeSel= TRUE;	

	OnMcbModsel();
	Redraw();
}

//����ӡҳ��� ��ԁE 
//lineNo �� 0 ��澱��
void CMCBView::OnPrintMapTable(CDC *pdc, int lineNo, int startTable_X, int startTable_Y,float* pdata)
{
	int tablesize_X = 512;
	int tablesize_Y = 28;
	int table_X,table_Y;
	char chtest[50];

	//�����
	if (lineNo == 0) { 
		table_X = startTable_X;
		table_Y = startTable_Y;
		pdc->MoveTo(table_X,table_Y);
		pdc->LineTo(table_X + tablesize_X,table_Y);
		pdc->LineTo(table_X + tablesize_X,table_Y+tablesize_Y);
		pdc->LineTo(table_X,table_Y+tablesize_Y);
		pdc->LineTo(table_X,table_Y);
		
		sprintf(chtest,"���");
		pdc->TextOut(table_X+6,table_Y+5,chtest);
		pdc->MoveTo(table_X+30,table_Y);
		pdc->LineTo(table_X+30,table_Y + tablesize_Y);

		sprintf(chtest,"��λ");
		pdc->TextOut(table_X+42,table_Y+5,chtest);
		pdc->MoveTo(table_X+90,table_Y);
		pdc->LineTo(table_X+90,table_Y + tablesize_Y);

		sprintf(chtest,"����");
		pdc->TextOut(table_X+110,table_Y+5,chtest);
		pdc->MoveTo(table_X+170,table_Y);
		pdc->LineTo(table_X+170,table_Y + tablesize_Y);

		sprintf(chtest,"�����");
		pdc->TextOut(table_X+180,table_Y+5,chtest);
		pdc->MoveTo(table_X+250,table_Y);
		pdc->LineTo(table_X+250,table_Y + tablesize_Y);

		sprintf(chtest,"�����");
		pdc->TextOut(table_X+270,table_Y+5,chtest);
		pdc->MoveTo(table_X+340,table_Y);
		pdc->LineTo(table_X+340,table_Y + tablesize_Y);

		switch(g_HardWare_Pare.iEngerScaleUnit) {
			case 0:
				sprintf(chtest,"��E��hn");//��E�E
				break;
			case 1:
				sprintf(chtest,"��E��v");//��E�E
				break;
			case 2:
				sprintf(chtest,"��E��ev");//��E�E
				break;
			case 3:
				sprintf(chtest,"��E��ev");//��E�E
				break;
			default:
				sprintf(chtest,"��E��hn");//��E�E
				break;
		}
		pdc->TextOut(table_X+350,table_Y+5,chtest);
		pdc->MoveTo(table_X+390,table_Y);
		pdc->LineTo(table_X+390,table_Y + tablesize_Y);

		sprintf(chtest,"�߽�E);
		pdc->TextOut(table_X+420,table_Y+5,chtest);
		pdc->MoveTo(table_X+512,table_Y);
		pdc->LineTo(table_X+512,table_Y + tablesize_Y);
	}

	//����ǰ�кŵ����ݱ��񣬲���������
	if (lineNo > 0) { 
		table_X = startTable_X;
		table_Y = startTable_Y + tablesize_Y * lineNo;
	
		pdc->MoveTo(table_X,table_Y);
		pdc->LineTo(table_X + tablesize_X,table_Y);
		pdc->LineTo(table_X + tablesize_X,table_Y+tablesize_Y);
		pdc->LineTo(table_X,table_Y+tablesize_Y);
		pdc->LineTo(table_X,table_Y);
		
		sprintf(chtest,"%d",lineNo); //���
		pdc->TextOut(table_X+6,table_Y+5,chtest);
		pdc->MoveTo(table_X+30,table_Y);
		pdc->LineTo(table_X+30,table_Y + tablesize_Y);
		
		if (pdata[0] >= 0.001) {
			sprintf(chtest,"%.2f",pdata[0]);//��λ
		}
		else { sprintf(chtest,"--"); }
		
		pdc->TextOut(table_X+42,table_Y+5,chtest);
		pdc->MoveTo(table_X+90,table_Y);
		pdc->LineTo(table_X+90,table_Y + tablesize_Y);
		
		if (pdata[1] >= 0.001) {
			sprintf(chtest,"%.2f",pdata[1]);//����
		}
		else sprintf(chtest,"--");
		
		pdc->TextOut(table_X+110,table_Y+5,chtest);
		pdc->MoveTo(table_X+170,table_Y);
		pdc->LineTo(table_X+170,table_Y + tablesize_Y);
		
		if (pdata[2] >= 0.001) {
			sprintf(chtest,"%.0f",pdata[2]);//�����
		}
		else sprintf(chtest,"--");
		
		pdc->TextOut(table_X+180,table_Y+5,chtest);
		pdc->MoveTo(table_X+250,table_Y);
		pdc->LineTo(table_X+250,table_Y + tablesize_Y);
		
		if (pdata[3] >= 0.001) {
			sprintf(chtest,"%.0f",pdata[3]);//�����
		}
		else sprintf(chtest,"--");
		
		pdc->TextOut(table_X+265,table_Y+5,chtest);
		pdc->MoveTo(table_X+340,table_Y);
		pdc->LineTo(table_X+340,table_Y + tablesize_Y);
		
		if (pdata[4] >= 0.001) {
			sprintf(chtest,"%.2f",pdata[4]);//��E�E				
		}
		else sprintf(chtest,"--");
		
		pdc->TextOut(table_X+355,table_Y+5,chtest);
		pdc->MoveTo(table_X+390,table_Y);
		pdc->LineTo(table_X+390,table_Y + tablesize_Y);
		
		sprintf(chtest,"%d---%d",(int)pdata[5],(int)pdata[6]);//�߽�E
		pdc->TextOut(table_X+400,table_Y+5,chtest);
		pdc->MoveTo(table_X+512,table_Y);
		pdc->LineTo(table_X+512,table_Y + tablesize_Y);		
	}

}

BOOL CMCBView::OnPrintMapCalculateData(float* outData)
{	
	float peak_position,peak_cnt;
	UINT Weight_Sum=0;	
	CString strtext;
	int Net_Sum=0;
	float outwidth;

	if (false == OnMouseCalcutNet_ROI_Array()) 
	{
		m_strStateLine2Text = " ";
		m_strStateLine3Text = "���������Ч";
		return FALSE;
	};

	//�����
	for(UINT i=0;i<(m_iInterest_RCh-m_iInterest_LCh + 1);i++) 
	{
		Net_Sum += m_iNet_ROI_Array[i];
	}
	outData[2] = Net_Sum;
	//������
	for(i=m_iInterest_LCh;i<m_iInterest_RCh+1;i++) 
	{
		Weight_Sum +=  ( m_arrExternalRam[i] & 0x7fffffff );
	}
	outData[3] = Weight_Sum;

	if(false == OnMousePeak_FIT(&peak_position , &peak_cnt) )
	{
		m_strStateLine2Text = "��λͳ����Ч";
		m_strStateLine3Text = " ";
		return FALSE;
	}; 

	OnMouseCalcutPeakWidth(2, &outwidth);

	//�����̶����� ͬ��	
//	m_fMenuEnergeScale_K = g_HardWare_Pare.fEngerScaleSlope;
//	m_fMenuEnergeScale_b = g_HardWare_Pare.fEngerScaleOffset;

	float kev;
	float fkww;
	if (m_bMenuEnergeScale_CalcuEn) {
		kev = (peak_position + m_iInterest_LCh) * m_fMenuEnergeScale_K + m_fMenuEnergeScale_b ;
		fkww = outwidth * m_fMenuEnergeScale_K;
		outData[0] = peak_position + m_iInterest_LCh;
		outData[1] = peak_cnt;
		outData[4] = fkww;
	}
	else {
		outData[0] = 0;
		outData[1] = 0;
		outData[4] = 0;
	}

	outData[5] = m_iInterest_LCh;
	outData[6] = m_iInterest_RCh;

	return TRUE;
}

void CMCBView::OnIdMcbHelp() 
{

//	WinExec("c:\\Program Files\\McaProgram\\hh.exe c:\\Program Files\\McaProgram\\McbHelpme.chm",SW_SHOW);	
	WinExec("hh McbHelpme.chm",SW_SHOW);	
}

float CMCBView::OnCaculateExpRamEthValue()
{
	CMainFrame* pMainFrame=	(CMainFrame*)GetParentFrame();
	CComboBox* pComboxVer = (CComboBox*)(pMainFrame->m_wndDlgBar.GetDlgItem(IDC_COMBO_VER));
    CComboBox* pComboxExpVer = (CComboBox*)(pMainFrame->m_wndDlgBar.GetDlgItem(IDC_COMBO_EXP_VER));

	float fmaxvalue = 0.1;
	float fminvalue = 10;
	unsigned int Width=0;
  if (m_bMCBModeSel)
  {
	 Width=m_iSamllWinWidth;
	if (m_bDrawLogData_Sel) 
	{
		for(int i=0; i< Width; i++)
		{
			if ((m_arrExpRam[i] & 0x7fffffff) <= 1) 
			{
				m_farrExpRam[i] = 0;
			}
			else
			{
				m_farrExpRam[i] = log10(m_arrExpRam[i] & 0x7fffffff);
			}		
			if (m_farrExpRam[i] < fminvalue) 
			{
				fminvalue = m_farrExpRam[i];
			}				
		}
///////////////////////////////////////////////////change
/*		for(i=0; i<m_iSamllWinWidth; i++)
		{
			m_farrExpRam[i] = m_farrExpRam[i] - fminvalue * 0.9; 			
		}
*/		
		fmaxvalue = 0.001;
		for(i=0;i<Width; i++ )
		{
			if (m_farrExpRam[i] > fmaxvalue)
			{
				fmaxvalue = m_farrExpRam[i];
			}
		}
		return (float)(fmaxvalue); //* 1.2
	}
	

	BOOL bautoEnable = FALSE;

	if((m_bMCBModeSel)&&(g_MCB_Oper.bQueryEn)) 
	{
		bautoEnable = TRUE;
	}
	if (m_bAutoCalcuExpRamEth) 
	{
		bautoEnable = TRUE;
	}
	if ((m_bMCBModeSel)&&(g_ComPortSel == USB_PORT)) 
	{
		bautoEnable = TRUE;
    }


	UINT maxvalue = (m_arrExpRam[0] & 0x7fffffff);
	if (bautoEnable)
	{
		for(int i=0;i<m_iSamllWinWidth;i++)
		{
			maxvalue = max(maxvalue,(m_arrExpRam[i] & 0x7fffffff));
		}		
		i=0;
		m_iExpRamEthValue = 64;
		while(i<17)
		{
			if(maxvalue < m_iExpRamEthValue) break;
			i++;
			m_iExpRamEthValue *= 2;
		}
		m_iBuffComboxSel = 17-i;		//////////////////////////////changing
		maxvalue = m_iExpRamEthValue; // ������ֵ��Ϊ ��������ֵ	
		pComboxExpVer->SetCurSel(m_iBuffComboxSel);
	}
	else 
	{
		maxvalue = m_iExpRamEthValue;
	}
//	if (m_bExpRamViewSel)
	{
		pComboxExpVer->SetCurSel(m_iBuffComboxSel);
	}
	if(maxvalue == 0) maxvalue = 64;
	return (float)maxvalue;
  }
  else
  {
	Width=m_iBuffSmallWindowWidth;
	if (m_bDrawLogData_Sel) 
	{
		for(int i=0; i< Width; i++)
		{
			if ((m_arrExpRam[i] & 0x7fffffff) <= 1) 
			{
				m_farrExpRam[i] = 0;
			}
			else
			{
				m_farrExpRam[i] = log10(m_arrExpRam[i] & 0x7fffffff);
			}		
			if (m_farrExpRam[i] < fminvalue) 
			{
				fminvalue = m_farrExpRam[i];
			}				
		}
		fmaxvalue = 0.001;
		for(i=0;i<Width; i++ )
		{
			if (m_farrExpRam[i] > fmaxvalue)
			{
				fmaxvalue = m_farrExpRam[i];
			}
		}
		return (float)(fmaxvalue); 
	}
	BOOL bautoEnable = FALSE;
	if((m_bMCBModeSel)&&(g_MCB_Oper.bQueryEn)) 
	{
		bautoEnable = TRUE;
	}
	if (m_bAutoCalcuExpRamEth) 
	{
		bautoEnable = TRUE;
	}
	if ((m_bMCBModeSel)&&(g_ComPortSel == USB_PORT)) 
	{
		bautoEnable = TRUE;
    }
	UINT maxvalue = (m_arrExpRam[0] & 0x7fffffff);
	if (bautoEnable)
	{
		for(int i=0;i<m_iSamllWinWidth;i++)
		{
			maxvalue = max(maxvalue,(m_arrExpRam[i] & 0x7fffffff));
		}		
		i=0;
		m_iBuffExpRamEthValue = 64;
		while(i<17)
		{
			if(maxvalue < m_iBuffExpRamEthValue ) break;
			i++;
			m_iBuffExpRamEthValue  *= 2;
		}
		m_iBuffExpRamCheckSel = 17-i;		//////////////////////////////changing
		maxvalue = m_iBuffExpRamEthValue; // ������ֵ��Ϊ ��������ֵ	
		pComboxExpVer->SetCurSel(m_iBuffExpRamCheckSel);
	}
	else 
	{
		maxvalue = m_iBuffExpRamEthValue;
	}
//	if (m_bExpRamViewSel)
	{
		pComboxExpVer->SetCurSel(m_iBuffExpRamCheckSel);
	}
	if(maxvalue == 0) maxvalue = 64;
	return (float)maxvalue;
  }
}

float CMCBView::OnCaculateFullRamEthValue()
{
	CMainFrame* pMainFrame=	(CMainFrame*)GetParentFrame();
	CComboBox* pComboxVer = (CComboBox *)(pMainFrame->m_wndDlgBar.GetDlgItem(IDC_COMBO_VER));

	float fmaxvalue = 0.1;
	float fminvalue = 10.0;
  if (m_bMCBModeSel)
  {
	if (m_bDrawLogData_Sel)
	{
		for(int i=0; i< 512; i++){
			if ((m_arrFullRam[i] & 0x7fffffff) <= 1) 
			{
				m_farrFullRam[i] = 0;
			}
			else 
			{
				m_farrFullRam[i] = log10(m_arrFullRam[i] & 0x7fffffff) ;
			}		
			if (m_farrFullRam[i] < fminvalue) 
			{
				fminvalue = m_farrFullRam[i];
			}		
		}   
		
/*		for(i=0; i<512; i++){
			m_farrFullRam[i] = m_farrFullRam[i] - fminvalue * 0.9; 			
		}
*/		
		fmaxvalue = 0.001;
//		fminvalue = 5;
		for(i=0;i<512; i++ ){
			if (m_farrFullRam[i] > fmaxvalue) {
				fmaxvalue = m_farrFullRam[i];
			}
		}		
		return (float)(fmaxvalue );//* 1.2
	}
	

	BOOL bautoEnable = FALSE;
	
	if((m_bMCBModeSel)&&(g_MCB_Oper.bQueryEn)) {
		bautoEnable = TRUE;
	}
	if (m_bAutoCalcuFullRamEth) {
		bautoEnable = TRUE;
	}
	if ((m_bMCBModeSel)&&(g_ComPortSel == USB_PORT)) {
		bautoEnable = TRUE;
    }

	
	UINT maxvalue = (m_arrFullRam[0] & 0x7fffffff);
	if (bautoEnable) 
	{	
		for(int i=0;i<512;i++) 
		{  //
			maxvalue = max(maxvalue,(m_arrFullRam[i]&0x7fffffff));
		}
		
		i=0;
		m_iFullRamEthValue=64;
//		m_iFullRamEthValue = 64;////////////////////////////////changing
		while((i<=17))
		{
			
			i++;
			m_iFullRamEthValue *= 2;
			if(maxvalue < m_iFullRamEthValue) break;		
		}
		m_iFullRamCheckSel = 17-i;///////////////////////////changing
		
		maxvalue = m_iFullRamEthValue; // ������ֵ��Ϊ ��������ֵ
		pComboxVer->SetCurSel(m_iFullRamCheckSel);
	}
	else {  //BUFF ��ʽ
		maxvalue = m_iFullRamEthValue;
	}
	
	if(maxvalue == 0) maxvalue = 64;
	return (float)maxvalue;
  }
  else
  {
	if (m_bDrawLogData_Sel)
	{
		for(int i=0; i< 512; i++){
			if ((m_arrFullRam[i] & 0x7fffffff) <= 1) 
			{
				m_farrFullRam[i] = 0;
			}
			else 
			{
				m_farrFullRam[i] = log10(m_arrFullRam[i] & 0x7fffffff) ;
			}		
			if (m_farrFullRam[i] < fminvalue) 
			{
				fminvalue = m_farrFullRam[i];
			}		
		}   
		fmaxvalue = 0.001;
		for(i=0;i<512; i++ )
		{
			if (m_farrFullRam[i] > fmaxvalue) 
			{
				fmaxvalue = m_farrFullRam[i];
			}
		}		
		return (float)(fmaxvalue );//* 1.2
	}
	BOOL bautoEnable = FALSE;
	
	if((m_bMCBModeSel)&&(g_MCB_Oper.bQueryEn)) {
		bautoEnable = TRUE;
	}
	if (m_bAutoCalcuFullRamEth) {
		bautoEnable = TRUE;
	}
	if ((m_bMCBModeSel)&&(g_ComPortSel == USB_PORT)) {
		bautoEnable = TRUE;
    }	
	UINT maxvalue = (m_arrFullRam[0] & 0x7fffffff);
	if (bautoEnable) 
	{	
		for(int i=0;i<512;i++) 
		{  //
			maxvalue = max(maxvalue,(m_arrFullRam[i]&0x7fffffff));
		}
		
		i=0;
		m_iBuffFullRamEthValue = 64;
		while((i<=17))
		{
			i++;
			m_iBuffFullRamEthValue *= 2;
			if(maxvalue < (UINT)m_iBuffFullRamEthValue) break;
		}
		maxvalue = m_iBuffFullRamEthValue; // ������ֵ��Ϊ ��������ֵ
		m_iBuffFullRamCheckSel=17-i;
		pComboxVer->SetCurSel(m_iBuffFullRamCheckSel);
	}
	else 
	{  
		maxvalue = m_iBuffFullRamEthValue;
	}
	if(maxvalue == 0) maxvalue = 64;
	return (float)maxvalue;
  }
}

//FullRamρEExpRam ��ֵת�ƣ�Ŀ������MCB�ɼ���ʽʱ��E�ˢ��ExpRam��Ļ
void CMCBView::OnFullRam2ExpRamInsertVal()
{
	return; //����ֱ�ӷ��أ�������ֵ��

	//MCB��ʽ�������ڲɼ�״̬��
	if(m_bMCBModeSel && g_MCB_Oper.bQueryEn) {	
		int insertnum = m_iFullChannelWidth/512;
		int startaddr = m_iSmallWinCHL/insertnum;
		int endaddr = m_iSmallWinCHR/insertnum;
		int bufAA,bufBB;
		int i;
		if((m_iSmallWinCHL>0)&&(m_iSmallWinCHR < m_iFullChannelWidth-1)) {
			for(i=startaddr;i<endaddr;i++) { 
				for(int kk=0;kk<insertnum;kk++){
					bufAA = (m_arrFullRam[i] & 0x7fffffff);
					bufBB = (m_arrFullRam[i+1] & 0x7fffffff);
					if (m_arrFullRam[i] >= 0x80000000) {						
						m_arrExpRam[(i-startaddr)*insertnum + kk] = bufAA + (float)kk*(bufBB - bufAA)/insertnum;
						m_arrExpRam[(i-startaddr)*insertnum + kk] |= 0x80000000;
					}
					else{
						m_arrExpRam[(i-startaddr)*insertnum + kk] = bufAA + (float)kk*(bufBB - bufAA)/(float)insertnum;
						m_arrExpRam[(i-startaddr)*insertnum + kk] &= 0x7FFFFFFF;
					}					
				}			
			}
		}
		

		if(m_iSmallWinCHL == 0) {
			memset(m_arrExpRam,0,(m_iSamllWinWidth-m_iSmallWinCHR)*4);			
			for(i=startaddr;i<endaddr;i++) { //m_iSamllWinWidth
				for(int kk=0;kk<insertnum;kk++){
					bufAA = (m_arrFullRam[i] & 0x7fffffff);
					bufBB = (m_arrFullRam[i+1] & 0x7fffffff);
					if (m_arrFullRam[i] >= 0x80000000) {						
						m_arrExpRam[(i+((m_iSamllWinWidth-m_iSmallWinCHR)/insertnum))*insertnum + kk] = bufAA + (float)kk*(bufBB - bufAA)/(float)insertnum;
						m_arrExpRam[(i+((m_iSamllWinWidth-m_iSmallWinCHR)/insertnum))*insertnum + kk] |= 0x80000000;
					}
					else{
						m_arrExpRam[(i+((m_iSamllWinWidth-m_iSmallWinCHR)/insertnum))*insertnum + kk] = bufAA + (float)kk*(bufBB - bufAA)/(float)insertnum;
						m_arrExpRam[(i+((m_iSamllWinWidth-m_iSmallWinCHR)/insertnum))*insertnum + kk] &= 0x7FFFFFFF;
					}
				}	
			}			
		}
		if(m_iSmallWinCHR >= m_iFullChannelWidth-1) {		
			for(i=startaddr;i<(m_iFullChannelWidth/insertnum);i++) { //m_iSamllWinWidth
				for(int kk=0;kk<insertnum;kk++){
					bufAA = (m_arrFullRam[i-1] & 0x7fffffff);
					bufBB = (m_arrFullRam[i] & 0x7fffffff);
					if (m_arrFullRam[i] >= 0x80000000) {						
						m_arrExpRam[(i-startaddr)*insertnum + kk] = bufAA + (float)kk*(bufBB - bufAA)/(float)insertnum;
						m_arrExpRam[(i-startaddr)*insertnum + kk] |= 0x80000000;
					}
					else{
						m_arrExpRam[(i-startaddr)*insertnum + kk] = bufAA + (float)kk*(bufBB - bufAA)/(float)insertnum;
						m_arrExpRam[(i-startaddr)*insertnum + kk] &= 0x7FFFFFFF;
					}
					
				}	
			}			
			for(i=m_iFullChannelWidth-m_iSmallWinCHL;i<m_iSamllWinWidth;i++) {
				m_arrExpRam[i] = 0;
			}
		}		
		
	};//end of ��m_bMCBModeSel && g_MCB_Oper.bQueryEn��
 
}


void CMCBView::OnMenuColorSet() 
{
	CColorSet colorset;
	colorset.m_Full_Color = m_Full_BackGround_Color;
	colorset.m_Exp_Color = m_Exp_BackGround_Color;
	colorset.m_Interest_Color = m_Interest_Color;	
	colorset.m_SmallWin_Color = m_SmallWin_BackGround_Color;

	if( IDOK == colorset.DoModal()) 
	{
		m_Full_BackGround_Color = colorset.m_Full_Color;
		m_Exp_BackGround_Color = colorset.m_Exp_Color;
		m_Interest_Color = colorset.m_Interest_Color;
		m_SmallWin_BackGround_Color = colorset.m_SmallWin_Color;

	/*	CIni inifile;
		if(inifile.Open("MCBini_05_1028.ini"))
		{ //���������ʾ����
			
			int Para_1_Full_BackGround_Color = (m_Full_BackGround_Color & 0x000000FF);
			int Para_2_Full_BackGround_Color = ((m_Full_BackGround_Color >> 8) & 0x000000FF);
			int Para_3_Full_BackGround_Color = ((m_Full_BackGround_Color >> 16) & 0x000000FF);
			int Para_1_Exp_BackGround_Color = (m_Exp_BackGround_Color & 0x000000FF);
			int Para_2_Exp_BackGround_Color = ((m_Exp_BackGround_Color >> 8) & 0x000000FF);
			int Para_3_Exp_BackGround_Color = ((m_Exp_BackGround_Color >> 16) & 0x000000FF);
			int Para_1_Interest_Color = (m_Interest_Color & 0x000000FF);
			int Para_2_Interest_Color = ((m_Interest_Color >> 8) & 0x000000FF);
			int Para_3_Interest_Color = ((m_Interest_Color >> 16) & 0x000000FF);
			int Para_1_SmallWin_BackGround_Color = (m_SmallWin_BackGround_Color & 0x000000FF);
			int Para_2_SmallWin_BackGround_Color = ((m_SmallWin_BackGround_Color >> 8) & 0x000000FF);
			int Para_3_SmallWin_BackGround_Color = ((m_SmallWin_BackGround_Color >> 16) & 0x000000FF);
			
			inifile.Open("MCBini_05_1028.ini");
			inifile.Write("MCB Color Set","Para_1_Full_BackGround_Color",Para_1_Full_BackGround_Color);
			inifile.Write("MCB Color Set","Para_2_Full_BackGround_Color",Para_2_Full_BackGround_Color);
			inifile.Write("MCB Color Set","Para_3_Full_BackGround_Color",Para_3_Full_BackGround_Color);
			
			inifile.Write("MCB Color Set","Para_1_Exp_BackGround_Color",Para_1_Exp_BackGround_Color);
			inifile.Write("MCB Color Set","Para_2_Exp_BackGround_Color",Para_2_Exp_BackGround_Color);
			inifile.Write("MCB Color Set","Para_3_Exp_BackGround_Color",Para_3_Exp_BackGround_Color);
			
			inifile.Write("MCB Color Set","Para_1_Interest_Color",Para_1_Interest_Color);
			inifile.Write("MCB Color Set","Para_2_Interest_Color",Para_2_Interest_Color);
			inifile.Write("MCB Color Set","Para_3_Interest_Color",Para_3_Interest_Color);
			
			inifile.Write("MCB Color Set","Para_1_SmallWin_BackGround_Color",Para_1_SmallWin_BackGround_Color);
			inifile.Write("MCB Color Set","Para_2_SmallWin_BackGround_Color",Para_2_SmallWin_BackGround_Color);
			inifile.Write("MCB Color Set","Para_3_SmallWin_BackGround_Color",Para_3_SmallWin_BackGround_Color);

			inifile.Save();
			inifile.Close();
		}*/
	}

	Redraw();
	m_ScrollBar.ShowScrollBar(FALSE);
	m_ScrollBar.ShowScrollBar(TRUE);
}

void CMCBView::OnUsbParaSet(BOOL Enreset)
{
	char chrPara[10];
	if (Enreset == TRUE) {
		ResetUsbDevice(g_hUsb);
		Sleep(300);
		
		//������λÁE�E��0x101010��
		chrPara[0] = 0x10; 
		chrPara[1] = 0x10;
		chrPara[2] = 0x10;
		chrPara[3] = 0x10;
		WriteUsbData(g_hUsb,chrPara,4);
		Sleep(250);

	}

/*	//ֹͣ�ɼ�
	chrPara[0] = 0x00; 
	chrPara[1] = 0x00;
	chrPara[2] = 0x00;
	chrPara[3] = 0x11;
	WriteUsbData(g_hUsb,chrPara,4);
	Sleep(30);
*/	

	//���Sram
	chrPara[0] = 0x00;	
	chrPara[1] = 0x00;
	chrPara[2] = 0x00;
	chrPara[3] = 0x62;
	WriteUsbData(g_hUsb,chrPara,4);		
	Sleep(30);

	//����ģʽ���� ��00,01,02,03,04,05��
    if (g_HardWare_Pare.bInputAnolSignalSel == TRUE) {//05=>ģ����ȁE��ADC_MCBģʽ
		chrPara[0] = 0x05;     
		chrPara[1] = 0x00;
		chrPara[2] = 0x00;
		chrPara[3] = 0x80;
    }
	else{                  
		chrPara[0] = 0x06;   // 06=>������ȁEInData_MCBģʽ 
		chrPara[1] = 0x00;
		chrPara[2] = 0x00;
		chrPara[3] = 0x80;
	}
	WriteUsbData(g_hUsb,chrPara,4);
	Sleep(30);

	//����InData����
	if (g_HardWare_Pare.bREADY_Pulse == TRUE) { //Ready �ź�
		chrPara[0] |= 0x01;
	}else{ chrPara[0] &= 0xFE;	}

	if (g_HardWare_Pare.bDT_Pulse == TRUE) { //DeadTime �ź�
		chrPara[0] |= 0x02;
	}else{ chrPara[0] &= 0xFD;	}

	if (g_HardWare_Pare.bANSER_Pulse == TRUE) { //Answer �ź�
		chrPara[0] |= 0x04;
	}else{ chrPara[0] &= 0xFB;	}

	if (g_HardWare_Pare.bDataPolarity_Pulse == TRUE) { //Data���� �ź�
		chrPara[0] |= 0x08;
	}else{ chrPara[0] &= 0xF7;	}

	if (g_HardWare_Pare.bENADC_Pulse == TRUE) { //EnADC �ź�
		chrPara[0] |= 0x10;
	}else{ chrPara[0] &= 0xEF;	}

//	chrPara[0] = 0xFF; 
	chrPara[1] = 0x00;
	chrPara[2] = 0x00;
	chrPara[3] = 0x66;
	WriteUsbData(g_hUsb,chrPara,4);
	Sleep(30);

	//A����
	int ibuff = g_HardWare_Pare.iEth_Up_UsbADC;	
	chrPara[0] = (char)ibuff; 
	chrPara[1] = (char)(ibuff>>8);
	chrPara[2] = 0x00;
	chrPara[3] = 0xA0;
	WriteUsbData(g_hUsb,chrPara,4);
	Sleep(30);
	
	//A����
	ibuff = g_HardWare_Pare.iEth_Down_UsbADC;
	chrPara[0] = (char)ibuff; 
	chrPara[1] = (char)(ibuff>>8);
	chrPara[2] = 0x00;
	chrPara[3] = 0xA1;
	WriteUsbData(g_hUsb,chrPara,4);
	Sleep(30);

	//��������
	ibuff = 256;
	chrPara[0] = (char)ibuff;
	chrPara[1] = (char)(ibuff>>8);
	chrPara[2] = 0x00;
	chrPara[3] = 0xC0; 
	WriteUsbData(g_hUsb,chrPara,4);
	Sleep(30);
	
	//���������س���
	ibuff = 4; //5
	chrPara[0] = (char)ibuff;
	chrPara[1] = 0;
	chrPara[2] = 0x00;
	chrPara[3] = 0xD0; 
	WriteUsbData(g_hUsb,chrPara,4);
	Sleep(30);

	//�����½��س���
	ibuff = 5;//6
	chrPara[0] = (char)ibuff;
	chrPara[1] = 0;
	chrPara[2] = 0x00;
	chrPara[3] = 0xD1; 
	WriteUsbData(g_hUsb,chrPara,4);	
	Sleep(30);
	
	//ʱ������
	ibuff = g_HardWare_Pare.iTime_Up_ETH; 
	chrPara[0] = (char)ibuff;
	chrPara[1] = 0;
	chrPara[2] = 0x00;
	chrPara[3] = 0xD3; 
	WriteUsbData(g_hUsb,chrPara,4);	
	Sleep(30);

	//ʱ������
	ibuff = g_HardWare_Pare.iTime_Down_ETH; 
	chrPara[0] = (char)ibuff;
	chrPara[1] = 0;
	chrPara[2] = 0x00;
	chrPara[3] = 0xD4; 
	WriteUsbData(g_hUsb,chrPara,4);	
	Sleep(30);
	
	//�ڴ�������ƫ��
	chrPara[0] = 0x20; 
	chrPara[1] = 0x00;
	chrPara[2] = 0x00;
	chrPara[3] = 0xC2;
	WriteUsbData(g_hUsb,chrPara,4);
	Sleep(30);
	
	//���õ���E 
	if (g_HardWare_Pare.bInputAnolSignalSel) { //ģ����ȁE���ʱ��Ĭ��Ϊ8K����E
        ibuff = 8192;
	}
	else{
	    ibuff = g_HardWare_Pare.iChannelWidth;//
	}

	chrPara[0] = (char)ibuff;		//���Sram
	chrPara[1] = (char)(ibuff>>8);
	chrPara[2] = (char)(ibuff>>16);
	chrPara[3] = 0x65;
	WriteUsbData(g_hUsb,chrPara,4);	
	Sleep(30);
	

	
	if (Enreset == TRUE) 
	{
		Beep(2000,80);
	}
	else
	{
		Beep(1200,80);
	}
	
}

void CMCBView::OnUsbWrFlashPara()
{
	//A����
	char chrPara[4];
	//A����
	chrPara[0] = 0x50;
	chrPara[1] = 0xE0;
	chrPara[2] = 0;
	chrPara[3] = 0;
	WriteUsbData(g_hUsb,chrPara,4);
	Sleep(500);
	
	int ibuff = g_HardWare_Pare.iEth_Up_UsbADC;	
	chrPara[0] = 0x50;
	chrPara[1] = 0x50;
	chrPara[2] = (char)ibuff;
	chrPara[3] = (char)(ibuff>>8);
	WriteUsbData(g_hUsb,chrPara,4);
	Sleep(200);
	
	//A����
	ibuff = g_HardWare_Pare.iEth_Down_UsbADC;
	chrPara[0] = 0x50;
	chrPara[1] = 0x51;
	chrPara[2] = (char)ibuff;
	chrPara[3] = (char)(ibuff>>8);
	WriteUsbData(g_hUsb,chrPara,4);
	Sleep(200);
	
	//���õ���E
	chrPara[0] = 0x50;
	chrPara[1] = 0x00;
	chrPara[2] = (char)g_HardWare_Pare.iChannelWidth;
	chrPara[3] = (char)(g_HardWare_Pare.iChannelWidth>>8);
	WriteUsbData(g_hUsb,chrPara,4);	

	Beep(1200,80);
}

void CMCBView::OnButtonPrint()
{
	DLGPRINT dlg;
    if(dlg.DoModal()==IDOK)
	{
		m_printmode=dlg.m_printmode;
       if(m_bMCBModeSel)
	   {
		    m_SmallWinWidth=m_iSamllWinWidth;
			m_SmallWinCHL=m_iSmallWinCHL;
			m_SmallWinCHR=m_iSmallWinCHR;
			m_saveselfvalue=m_iExpRamEthValue;
	   }
	   else
	   {
		    m_SmallWinWidth=m_iBuffSmallWindowWidth;
			m_SmallWinCHL=m_iBuffSmallWinCHL;
			m_SmallWinCHR=m_iBuffSmallWinCHR;
			m_saveselfvalue=m_iBuffExpRamEthValue;
	   }
        if (m_printmode==1)
        {
			if(m_bMCBModeSel)
			{
				m_SmallWinWidth=m_iFullChannelWidth;
				m_SmallWinCHL=0;
				m_SmallWinCHR=m_iFullChannelWidth;
			}
			else
			{
				m_SmallWinWidth=m_iBuffFullChannelWidth;
				m_SmallWinCHL=0;
				m_SmallWinCHR=m_iBuffFullChannelWidth;
			}
        }
		if (m_printmode==2)
		{
            m_SmallWinWidth=dlg.m_max-dlg.m_min+1;
			m_SmallWinCHL=dlg.m_min;
            m_SmallWinCHR=dlg.m_max;
			
		}
        if (m_printmode==3)
		{
            m_SmallWinWidth=dlg.m_max-dlg.m_min+1;
			m_SmallWinCHL=dlg.m_min;
            m_SmallWinCHR=dlg.m_max;
		}


		m_selfvalue=dlg.m_selfvalue;
		
		int kkk1=64;
		for(int j=0;j<17-m_selfvalue;j++)
		{
			kkk1 *= 2;
		}
    	m_selfrange= kkk1;
		m_range=dlg.m_range;

		m_rangemode=dlg.m_rangemode;
        m_1=dlg.m_1;
		m_2=dlg.m_2;
		m_3=dlg.m_3;
		m_4=dlg.m_4;
		m_5=dlg.m_5;
     
		PrintView();
	}
	
}

void CMCBView::PrintView()
{	

   CView::OnFilePrintPreview();
   

}


void CMCBView::PrintMyDraw(CDC *pDC)
{
	
	CRect ViewRec;
	CRect Rect;
	GetWindowRect(ViewRec);
	GetClientRect(&Rect);
    int x=Rect.Width();
	int y=Rect.Height();
	
	m_iViewWindowSize_X = x;
	m_iViewWindowSize_Y = y;
	m_iBigWinCenPoint = m_iViewWindowSize_X/2;
  
	 	if((!m_bMCBModeSel)||(!g_MCB_Oper.bQueryEn)) 
		{
			int kk =0;
			if (m_bMCBModeSel)kk= 2*m_iFullChannelWidth/1024;
			else 	kk=2*m_iBuffFullChannelWidth/1024;
			UINT max = 0;
		for(int i=0;i<512;i++) 
		{
			max = 0;
			for(int j=i*kk;j<(i+1)*kk;j++) 
			{ 
				//ѡ��kk ���ȷ�Χ�������һ������ֵ
				if (m_arrExternalRam[j] > max) 
				{
					max = m_arrExternalRam[j];
				}
			}
			m_arrFullRam[i] = max;
		}
		OnExternal2ExpRam();
	}

	m_bPrintMapEn = TRUE;

	CRect clientrect;
	GetClientRect(clientrect);
	x=Rect.Width();
	y=Rect.Height();
	CDC MemDC; //���ȶ���һ����ʾ�豸��ρE
	CBitmap MemBitmap;//����һ��λͼ��ρE
	MemDC.CreateCompatibleDC(NULL);
	MemBitmap.CreateCompatibleBitmap(&MemDC,clientrect.Width(),clientrect.Height()); //�Զ���һ��ͼ��Ĵ�С
	CBitmap *pOldBit=MemDC.SelectObject(&MemBitmap);
// 	OnDrawBackgroud(&MemDC);//////////////////////////////////////////////////changing
	OnPrintBackgroud(&MemDC);
	OnPrintDrawTextBgroud(&MemDC);
// 	OnPrintSmallWin(&MemDC,m_iSmallWinCenPoint,m_iSmallWinCHL,m_iSmallWinCHR);/////////////changing
    OnPrintExpRamLine(&MemDC);
//	OnDrawExpRamLine(&MemDC);////////////////////////changing
//	OnDrawFullRamLine(&MemDC);////////////////////////changing


/*	
 	OnShowStateText(&MemDC,1,m_strStateLine1Text);
 	m_strStateLine2Text = " ";
 	m_strStateLine3Text = " ";
 	OnShowStateText(&MemDC,2,m_strStateLine2Text);
	OnShowStateText(&MemDC,3,m_strStateLine3Text);	
*/
//	PrintPageHeader(&MemDC);
//	OnPrintCoo(&MemDC);
//	PrintPageFooter(&MemDC);

 	//���»�һ��ͼ��ı߿�E
	CPen pen(PS_SOLID,1,RGB(255,0,0));
	MemDC.SelectObject(&pen);
	MemDC.MoveTo(0,0);
	MemDC.LineTo(clientrect.Width()-2,0);
	MemDC.LineTo(clientrect.Width()-2,m_iViewWindowSize_Y*418.0/500);
	MemDC.LineTo(0,m_iViewWindowSize_Y*418.0/500);
	MemDC.LineTo(0,0);
                  //30,10     710,510
 	pDC->StretchBlt(30,30,710,510,&MemDC,0,0,clientrect.Width(),clientrect.Height(),SRCCOPY);	
	MemBitmap.DeleteObject();
	pen.DeleteObject();
 	MemDC.DeleteDC();
 	m_bPrintMapEn = FALSE;

}

void CMCBView::OnPrintCoo(CDC *pDC)//��ӡ����E
{
	CString	 str;	
	CFont	fonttext;
	fonttext.CreateFont(-15,0,0,0,400,FALSE,FALSE,0,
						ANSI_CHARSET,OUT_DEFAULT_PRECIS,
						CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,
						DEFAULT_PITCH | FF_SWISS ,"Arial");

	pDC->SelectObject(&fonttext);

	pDC->SetBkColor(RGB(255,255,255));
	pDC->SetTextColor(RGB(0,0,0));
    
	CRect rect;
	GetClientRect(&rect);
    int x=rect.Width();
	int y=rect.Height();
	int z=m_iViewWindowSize_Y*418.0/500;
	float hor=m_iViewWindowSize_X/6;
//	pDC->SetViewportOrg(0,15);// ҳü��E
	CPoint point(0,0);
    float ver=((m_iViewWindowSize_Y*418.0/500)/7);
//  int ver=(rect.Height())/8;
	ver=ver*510/m_iViewWindowSize_Y;
	hor=hor*710/x;
    CPen pen(PS_SOLID,1,RGB(0,0,0));
    pDC->SelectObject(&pen);
	int max=OnCaculateExpRamEthValue();
	if (m_bDrawLogData_Sel)
	{
	
        for (int i=0;i<8;i++)
        {
			/*pDC->MoveTo(point.x+35,point.y+147+61*i-61-25);
			pDC->LineTo(point.x+39,point.y+147+61*i-61-25);
			str.Format("10^%d",i);
            pDC->TextOut(point.x+25-str.GetLength()*5,point.y+147+61*(7-i)-61-32,str);
			*/
			pDC->MoveTo(27,ver*i+30);
			pDC->LineTo(30,ver*i+30);
			str.Format("10^%d",i);
            pDC->TextOut(point.x+20-str.GetLength()*5,ver*(7-i)-5+30,str);

        }
	}
	else
	{
		UINT iExpEth =0;
		if (m_rangemode==2)
		{
			iExpEth = m_selfrange/5;           
		}
		if(m_rangemode==1)
		{
			iExpEth = m_range/5;
		}

		if (m_rangemode==1)
		{
			if (m_range>1024)str.Format("%dk",m_range/1024);
			if(m_range<=1024)str.Format("%d",m_range);
		}
		if (m_rangemode==2)
		{
			if (m_selfrange>1024)str.Format("%dk",m_selfrange/1024);
			if (m_selfrange<=1024)str.Format("%d",m_selfrange);
				
		}
		     pDC->MoveTo(27,30);
			 pDC->LineTo(30,30);
			 pDC->TextOut(point.x+20-str.GetLength()*5,30,str);	//���ߵ�E
	
		float lver=((m_iViewWindowSize_Y*418.0/500)/5);
		lver=lver*510/m_iViewWindowSize_Y;
	    for(int i=1;i<6;i++)//��ӡ������E
		{
			pDC->MoveTo(27,i*lver+30);
			pDC->LineTo(30,i*lver+30);
			//�̶�	
			if ((iExpEth*(5-i)>1024))
			{
		         str.Format("%dk",iExpEth*(5-i)/1024);
			}
            
			else
				str.Format("%d",iExpEth*(5-i));
			    pDC->TextOut(point.x+20-str.GetLength()*5,i*lver-8+30,str);	//���ߵ�E

		}
    }
/*	for(int i=0;i<25;i++)
	{	
		pDC->TextOut(point.x+28*(i+1),point.y+553,"����������������");//ȫ�ǿո���ͬ
		pDC->TextOut(point.x+28*(i+1),point.y+537,"����������������");
		pDC->TextOut(point.x+28*(i+1),point.y+521,"����������������");
		pDC->TextOut(point.x+28*(i+1),point.y+505,"����������������");
		pDC->TextOut(point.x+28*(i+1),point.y+489,"��������������  ");
	}
*/
//	������EͿ̶�
	int iMiddleCH;

		iMiddleCH=(m_SmallWinCHR-m_SmallWinCHL)/6.00;
    
	for(int i=0;i<3;i++)//��ӡ������E
	{
		/*pDC->MoveTo(point.x+39+i*118,7*ver);
		pDC->LineTo(point.x+39+i*118,7*ver+3);
		str.Format("%d",m_iSmallWinCHL+iMiddleCH*i);
		pDC->TextOut(point.x+32+i*118,7*ver+3,str);*/
		pDC->MoveTo(i*hor+30,7*ver+30);
		pDC->LineTo(i*hor+30,7*ver+5+30);
        str.Format("%d",m_SmallWinCHL+iMiddleCH*i);
        if (i!=0)
        {
			pDC->TextOut(i*hor+20,7*ver+5+30,str);
        }
		else
            pDC->TextOut(i*hor+27,7*ver+5+30,str);
		
	}
	//��ӡ���ߵ�����E
		pDC->MoveTo(3*hor+30,7*ver+30);
		pDC->LineTo(3*hor+30,7*ver+5+30);
		str.Format("%d",m_SmallWinCHL+3*iMiddleCH);

		pDC->TextOut(3*hor+20,7*ver+5+30,str);
	for(i=4;i<6;i++)//��ӡ������E
	{
		pDC->MoveTo(i*hor+30,7*ver+30);
		pDC->LineTo(i*hor+30,7*ver+5+30);
		str.Format("%d",m_SmallWinCHL+iMiddleCH*i);
		pDC->TextOut(i*hor+20,7*ver+5+30,str);
	}
		pDC->MoveTo(6*hor+30,7*ver+30);
		pDC->LineTo(6*hor+30,7*ver+5+30);
	if (m_printmode==1)
	{
       str.Format("%d",m_SmallWinCHR-1);
	}
	if (m_printmode!=1)
	{
		str.Format("%d",m_SmallWinCHR);
	}

	pDC->TextOut(6*hor+15,7*ver+5+30,str);////////////////////////changing
	//////////////////////////////////////////////////////////////////////////

    pen.DeleteObject();
	CPen pShow(PS_SOLID,1,RGB(0,0,0));
	pDC->SelectObject(&pShow);
//	pDC->SetViewportOrg(75,400);  // ͼ����E
    pDC->SetViewportOrg(75,300);
	
}

void CMCBView::OnprintCaculateLog()
{
	if (m_bDrawLogData_Sel) 
	{
	   for(int i=0;i<8192;i++)
		m_farrExpRamprint[i] = log10(m_arrExternalRam[i] & 0x7fffffff);	
	}
}

void CMCBView::ResumeW()     //////changing
{
	FILE *fp=fopen("MCBini_05_1028.ini","wb");
	fseek(fp,0,SEEK_SET);
///////////////////////////////////////////////////////// hardware
	fwrite(&g_ComPortSel,sizeof(DWORD),1,fp);
	fwrite(&g_HardWare_Pare.bANSER_Pulse,1,1,fp);
	fwrite(&g_HardWare_Pare.bDT_Pulse, 1,1,fp);
	fwrite(&g_HardWare_Pare.bENADC_Pulse,1,1,fp);
	fwrite(&g_HardWare_Pare.bREADY_Pulse,1,1,fp);
	fwrite(&g_HardWare_Pare.bReadorWriteOperate,1,1,fp);
	fwrite(&g_HardWare_Pare.bSampleModeRealTime,1,1,fp);
	fwrite(& g_HardWare_Pare.bDataPolarity_Pulse,1,1,fp);
	fwrite(&g_HardWare_Pare.fEngerScaleOffset,4,1,fp);
	fwrite(&g_HardWare_Pare.fEngerScaleSlope,4,1,fp);	
	fwrite(&g_HardWare_Pare.iChannelWidth,4,1,fp);
	fwrite(&g_HardWare_Pare.iEngerScaleUnit,4,1,fp);
	fwrite(&g_HardWare_Pare.iSampleTimeValue,4,1,fp);
//////////////////////////////////////////////////////////////////////////////color
	fwrite(&m_Full_BackGround_Color,sizeof(COLORREF),1,fp);			
	fwrite(&m_Exp_BackGround_Color,sizeof(COLORREF),1,fp);
	fwrite(&m_Interest_Color,sizeof(COLORREF),1,fp);
	fwrite(&m_SmallWin_BackGround_Color,sizeof(COLORREF),1,fp);
////////////////////////////////////////////////////////////////////////////////usb
	fwrite(&g_HardWare_Pare.iEth_Up_UsbADC,4,1,fp);
	fwrite(&g_HardWare_Pare.iEth_Down_UsbADC,4,1,fp);
	fwrite(&g_HardWare_Pare.iTime_Up_ETH,4,1,fp);
	fwrite(&g_HardWare_Pare.iTime_Down_ETH,4,1,fp);
	fwrite(&g_HardWare_Pare.bInputAnolSignalSel,1,1,fp);
	int j=ftell(fp);
////////////////////////////////////////////////////////////////////////////////////////////////////////////// 
/////////////////////////////////////////////////////////BUFF  data
    fwrite(m_arrBUFFExternalRam,4,8192,fp);                                /////////////////////////////33790
/////////////////////////////////////////////////////////MCA  data
	fwrite(m_arrMCBExternalRam,4,8192,fp);                                 /////////////////////////////66558
/////////////////////////////////////////////////////////hardware parameter
//    fwrite(&g_HardWare_Pare,sizeof(g_HardWare_Pare),1,fp);                 /////////////////////////////66626
/////////////////////////////////////////////////////////sys parameter
    fwrite(&m_bMCBModeSel,sizeof(bool),1,fp); 
    fwrite(&m_bExpRamViewSel,sizeof(bool),1,fp);
	fwrite(&m_bDrawLogData_Sel,sizeof(bool),1,fp);
	fwrite(&m_iSmallWindowWidthCheckSel,1,1,fp);
	fwrite(&m_iBuffSmallWindowWidthSel,1,1,fp);
	fwrite(&m_iFullRamCheckSel,1,1,fp);
	fwrite(&m_iBuffFullRamCheckSel,1,1,fp);
	fwrite(&m_iBuffComboxSel,1,1,fp);
	fwrite(&m_iBuffExpRamCheckSel,1,1,fp);
	fwrite(&m_startflag,sizeof(bool),1,fp);
	fwrite(&m_bSetSampleTimeRealMode,sizeof(bool),1,fp);
	fwrite(&m_iSamllWinWidth,4,1,fp);
	fwrite(&m_iBuffSmallWindowWidth,sizeof(unsigned int),1,fp);
	fwrite(&m_iFullRamEthValue,4,1,fp);
	fwrite(&m_iBuffFullRamEthValue,4,1,fp);
//////////////////////////////////////////////////////////view parameter
	fwrite(&m_iSmallWinCenPoint,sizeof(int),1,fp);
	fwrite(&m_iBuffSmallWinCenPoint,4,1,fp);
	fwrite(&m_iFullChannelWidth,sizeof(unsigned int),1,fp);
	fwrite(&m_iBuffFullChannelWidth,4,1,fp);
	fwrite(&m_fMenuEnergeScale_K,sizeof(float),1,fp);
	fwrite(&m_fMenuEnergeScale_b,sizeof(float),1,fp);
	fwrite(&m_fMenuEnergeScale_U,1,1,fp);
	fwrite(&m_bMenuEnergeScale_CalcuEn,1,1,fp);
	fwrite(&m_bMcbMenuEnergeScale_CalcuEn,1,1,fp);
	fwrite(&g_HardWare_Pare.fEngerScaleSlope,sizeof(float),1,fp);
	fwrite(&g_HardWare_Pare.fEngerScaleOffset,sizeof(float),1,fp);
	fwrite(&g_HardWare_Pare.iEngerScaleUnit,sizeof(int),1,fp);
	fwrite(&m_iTimeFileSourceflag,1,1,fp);
	fwrite(&g_HardWare_Pare.bInputAnolSignalSel,1,1,fp);

    fwrite(&m_starttime_hour,sizeof(float),1,fp);
	fwrite(&m_starttime_min,sizeof(float),1,fp);
	fwrite(&m_starttime_sec,sizeof(float),1,fp);
///////////////////////////////////////////////////////////
//	CMainFrame* pMainFrame = (CMainFrame*)GetParentFrame();
//	CEdit* pStartTime = (CEdit*)(pMainFrame->m_wndDlgBar.GetDlgItem(IDC_STATE_START_TIME));
//  CString s;
//	pStartTime->GetWindowText(s);
//	int yy=s.GetLength();
//	if(!s.IsEmpty()) fprintf(fp,"%s",s);
///////////////////////////////////////////////////////////Finish Write
	fclose(fp);


}

void CMCBView::ResumeR()    ///changing
{
    UINT position;
	FILE *fp=fopen("MCBini_05_1028.ini","rb");
	CString stime;
	stime.Format("%s","��ʼ:��            ֹͣ:��");
	bool kkk=1;
  if(fp!=NULL)
  {
	  fseek(fp,0,SEEK_END);
	  position=ftell(fp);
	  unsigned char temp[4];
   if (position==0)
   {
	 return;
   }
   else
   {
	   fseek(fp,64,SEEK_SET);
	   for (int i=0;i<8192;i++)
	  {
		  
		  fread(temp,1,4,fp);
		  m_arrBUFFExternalRam[i] = temp[3];
		  m_arrBUFFExternalRam[i] <<= 8;
		  m_arrBUFFExternalRam[i] +=temp[2];
		  m_arrBUFFExternalRam[i] <<= 8;
		  m_arrBUFFExternalRam[i] +=temp[1];
		  m_arrBUFFExternalRam[i] <<= 8;
		  m_arrBUFFExternalRam[i] += temp[0];

		  if (m_arrBUFFExternalRam[i]==3435973836) m_arrBUFFExternalRam[i]=0;

	  }
///////////////////////////////////////////////////////////MCB data
      for (i=0;i<8192;i++)
	  {
		  position=ftell(fp);
			  fread(temp,1,4,fp);
			  m_arrMCBExternalRam[i] = temp[3];
			  m_arrMCBExternalRam[i] <<= 8;
			  m_arrMCBExternalRam[i] +=temp[2];
			  m_arrMCBExternalRam[i] <<= 8;
			  m_arrMCBExternalRam[i] +=temp[1];
			  m_arrMCBExternalRam[i] <<= 8;
			  m_arrMCBExternalRam[i] += temp[0];

			  if (m_arrMCBExternalRam[i]==3435973836) m_arrMCBExternalRam[i]=0;
	  }
///////////////////////////////////////////////////////////hardware paramter
//	fread(&g_HardWare_Pare,sizeof(g_HardWare_Pare),1,fp);
///////////////////////////////////////////////////////////sys parameter
	fread(&m_bMCBModeSel,1,1,fp);
	if (m_bMCBModeSel>1)
	{
		fclose(fp);
        NewIniFile();
	}
    fread(&m_bExpRamViewSel,sizeof(bool),1,fp);
	fread(&m_bDrawLogData_Sel,sizeof(bool),1,fp);
	fread(&m_iSmallWindowWidthCheckSel,1,1,fp);
	fread(&m_iBuffSmallWindowWidthSel,1,1,fp);
	fread(&m_iFullRamCheckSel,1,1,fp);
    fread(&m_iBuffFullRamCheckSel,1,1,fp);
	fread(&m_iBuffComboxSel,1,1,fp);
    fread(&m_iBuffExpRamCheckSel,1,1,fp);
	fread(&m_startflag,sizeof(bool),1,fp);
	fread(&m_bSetSampleTimeRealMode,sizeof(bool),1,fp);

	fread(&temp,1,4,fp);
    m_iSamllWinWidth = temp[3];
	m_iSamllWinWidth <<= 8;
	m_iSamllWinWidth +=temp[2];
	m_iSamllWinWidth <<= 8;
	m_iSamllWinWidth +=temp[1];
	m_iSamllWinWidth <<= 8;
	m_iSamllWinWidth += temp[0];
	if (m_iSamllWinWidth>8192)
	{
		fclose(fp);
		NewIniFile();
	}
	fread(&temp,1,4,fp);
	m_iBuffSmallWindowWidth = temp[3];
	m_iBuffSmallWindowWidth <<= 8;
	m_iBuffSmallWindowWidth +=temp[2];
	m_iBuffSmallWindowWidth <<= 8;
	m_iBuffSmallWindowWidth +=temp[1];
	m_iBuffSmallWindowWidth <<= 8;
	m_iBuffSmallWindowWidth += temp[0];
/////////////////////////////////////////////////////////////
    fread(&m_iFullRamEthValue,4,1,fp);
	fread(&m_iBuffFullRamEthValue,4,1,fp);
///////////////////////////////////////////////////////////smallwindowcenter
	fread(&temp,1,4,fp);
    m_iSmallWinCenPoint = temp[3];
	m_iSmallWinCenPoint <<= 8;
	m_iSmallWinCenPoint +=temp[2];
	m_iSmallWinCenPoint <<= 8;
	m_iSmallWinCenPoint +=temp[1];
	m_iSmallWinCenPoint <<= 8;
	m_iSmallWinCenPoint += temp[0];
    

	m_iSmallWinCHL=m_iSmallWinCenPoint-m_iSamllWinWidth/2;
	m_iSmallWinCHR=m_iSmallWinCenPoint+m_iSamllWinWidth/2;
	
///////////////////////////////////////////////////////////BUf
	fread(&temp,1,4,fp);
	m_iBuffSmallWinCenPoint = temp[3];
	m_iBuffSmallWinCenPoint <<= 8;
	m_iBuffSmallWinCenPoint +=temp[2];
	m_iBuffSmallWinCenPoint <<= 8;
	m_iBuffSmallWinCenPoint +=temp[1];
	m_iBuffSmallWinCenPoint <<= 8;
	m_iBuffSmallWinCenPoint += temp[0];
	
	m_iBuffSmallWinCHL=m_iBuffSmallWinCenPoint-m_iBuffSmallWindowWidth/2;
	m_iBuffSmallWinCHR=m_iBuffSmallWinCenPoint+m_iBuffSmallWindowWidth/2;
///////////////////////////////////////////////////////////FullChannelWidth     
	fread(&temp,1,4,fp);
    m_iFullChannelWidth = temp[3];
	m_iFullChannelWidth <<= 8;
	m_iFullChannelWidth +=temp[2];
	m_iFullChannelWidth <<= 8;
	m_iFullChannelWidth +=temp[1];
	m_iFullChannelWidth <<= 8;
	m_iFullChannelWidth += temp[0];
/////////////////////////////////////////////////////////////BUFFullChannelWidth
	fread(&temp,1,4,fp);
	m_iBuffFullChannelWidth = temp[3];
	m_iBuffFullChannelWidth <<= 8;
	m_iBuffFullChannelWidth +=temp[2];
	m_iBuffFullChannelWidth <<= 8;
	m_iBuffFullChannelWidth +=temp[1];
	m_iBuffFullChannelWidth <<= 8;
	m_iBuffFullChannelWidth += temp[0];
///////////////////////////////////////////////////////////////Energy
  fread(&m_fMenuEnergeScale_K,sizeof(float),1,fp);

/*	int MenuEnergeScale_K=0;
	MenuEnergeScale_K= temp[3];
	MenuEnergeScale_K <<= 8;
	MenuEnergeScale_K +=temp[2];
	MenuEnergeScale_K <<= 8;
	MenuEnergeScale_K +=temp[1];
	MenuEnergeScale_K <<= 8;
	MenuEnergeScale_K += temp[0];

    m_fMenuEnergeScale_K = (float)MenuEnergeScale_K;*/
	fread(&m_fMenuEnergeScale_b,sizeof(float),1,fp);
/*  m_fMenuEnergeScale_b = temp[3];
	m_fMenuEnergeScale_b <<= 8;
	m_fMenuEnergeScale_b +=temp[2];
	m_fMenuEnergeScale_b <<= 8;
	m_fMenuEnergeScale_b +=temp[1];
	m_fMenuEnergeScale_b <<= 8;
	m_fMenuEnergeScale_b += temp[0];
*/
	fread(&m_fMenuEnergeScale_U,1,1,fp);
	fread(&m_bMenuEnergeScale_CalcuEn,1,1,fp);
	fread(&m_bMcbMenuEnergeScale_CalcuEn,1,1,fp);
	fread(&g_HardWare_Pare.fEngerScaleSlope,sizeof(float),1,fp);
	fread(&g_HardWare_Pare.fEngerScaleOffset,sizeof(float),1,fp);
	fread(&g_HardWare_Pare.iEngerScaleUnit,sizeof(int),1,fp);
	fread(&m_iTimeFileSourceflag,1,1,fp);
	fread(&g_HardWare_Pare.bInputAnolSignalSel,1,1,fp);
///////////////////////////////////////////////////////////////string
    fread(&m_starttime_hour,sizeof(float),1,fp);
	fread(&m_starttime_min,sizeof(float),1,fp);
	fread(&m_starttime_sec,sizeof(float),1,fp);
 }
///////////////////////////////////////////////////////////Finish Read
	fclose(fp);
}
	int file=_open("MCBini_05_1028.ini",_O_RDWR | _O_CREAT, _S_IREAD | _S_IWRITE ,NULL);
	_chsize(file,0);
	_close(file);
////////////////////////////////////////////////////////////////View parameter response	

	CMainFrame* pMainFrame = (CMainFrame*)GetParentFrame();
	CButton* pMCB_TO_BUF = (CButton *)(pMainFrame->m_wndDlgBar.GetDlgItem(IDC_EXTERNALDATA_MCB2BUFF));
	CEdit* pShowRealTime = (CEdit*)(pMainFrame->m_wndDlgBar.GetDlgItem(IDC_SHOW_REAL_TIME));
	CEdit* pShowLiveTime = (CEdit*)(pMainFrame->m_wndDlgBar.GetDlgItem(IDC_SHOW_LIVE_TIME));
	CEdit* pShowRatioTime = (CEdit*)(pMainFrame->m_wndDlgBar.GetDlgItem(IDC_SHOW_RATIO_TIME));
    CEdit* pSetTime = (CEdit*)(pMainFrame->m_wndDlgBar.GetDlgItem(IDC_TIMER_EDIT));
	CButton* pMCBSel = (CButton *)(pMainFrame->m_wndDlgBar.GetDlgItem(IDC_MCB_MODSEL));
	CButton* pBufSel = (CButton *)(pMainFrame->m_wndDlgBar.GetDlgItem(IDC_BUFF_MODSEL));
//	CButton* pFullSel = (CButton *)(pMainFrame->m_wndDlgBar.GetDlgItem(IDC_FULLRAM_VIEWSEL));
//	CButton* pExpSel = (CButton *)(pMainFrame->m_wndDlgBar.GetDlgItem(IDC_EXPRAM_VIEWSEL));
    CButton* pRealTimeSel = (CButton *)(pMainFrame->m_wndDlgBar.GetDlgItem(IDC_REALTIME_SEL));
	CButton* pLiveTimeSel = (CButton *)(pMainFrame->m_wndDlgBar.GetDlgItem(IDC_LIVETIME_SEL));
	CComboBox* pVer=(CComboBox *)(pMainFrame->m_wndDlgBar.GetDlgItem(IDC_COMBO_VER));
//	CEdit* pStartTime = (CEdit*)(pMainFrame->m_wndDlgBar.GetDlgItem(IDC_STATE_START_TIME));
	CComboBox* pComboHor=(CComboBox *)(pMainFrame->m_wndDlgBar.GetDlgItem(IDC_COMBO_HOR));
	CComboBox* pComboExpHor=(CComboBox *)(pMainFrame->m_wndDlgBar.GetDlgItem(IDC_COMBO_EXP_HOR));
	CComboBox* pExpVer=(CComboBox *)(pMainFrame->m_wndDlgBar.GetDlgItem(IDC_COMBO_EXP_VER));
 
	if (m_startflag)
	{
//        pMCB_TO_BUF->ShowWindow(FALSE);
	    m_scrollstring[2].Format("�ɼ���ʼ��: %.2d:%.2d:%.2d...",m_starttime_hour,m_starttime_min,m_starttime_sec);
		m_textlength = 2;
	}
//////////////////////////////////////////////////////
    if (m_bMCBModeSel)
    {
		pMCBSel->SetCheck(1);
		pBufSel->SetCheck(0);
        if (!m_startflag)
        {
			OnMcbModsel();
        }
		
    }                                    ////Mcb Buf
    else
	{
		pMCBSel->SetCheck(0);
		pBufSel->SetCheck(1);
		OnBuffModsel();
	}
/////////////////////////////////////////////////////////    
  if (m_bMCBModeSel)
  {
	
	
//		pFullSel->SetCheck(0);
//		pExpSel->SetCheck(1);
		OnExpramViewsel();
		int kkk1=64;
	    for(int j=0;j<17-m_iFullRamCheckSel;j++)
		{
			kkk1 *= 2;
		}
			m_iFullRamEthValue = kkk1;	
		                                  ////Exp Full
	
	
//		pFullSel->SetCheck(1);
//		pExpSel->SetCheck(0);
		OnFullramViewsel();
		 kkk1=64;
		for(j=0;j<17-m_iBuffComboxSel;j++)
		{
			kkk1 *= 2;
		}
		m_iExpRamEthValue = kkk1;	
	
  }
  else
  {
	
//		pFullSel->SetCheck(0);
//		pExpSel->SetCheck(1);
		OnExpramViewsel();
		int kkk1=64;
	    for(int j=0;j<17-m_iBuffFullRamCheckSel;j++)
		{
			kkk1 *= 2;
		}
			m_iBuffFullRamEthValue = kkk1;	
	                                 ////Exp Full
	
//		pFullSel->SetCheck(1);
//		pExpSel->SetCheck(0);
		OnFullramViewsel();
		 kkk1=64;
		for( j=0;j<17-m_iBuffExpRamCheckSel;j++)
		{
			kkk1 *= 2;
		}
		m_iBuffExpRamEthValue = kkk1;
	
  }
/////////////////////////////////////////////////////////
   if (m_bDrawLogData_Sel)
   {
		   pVer->ResetContent();
	       pVer->AddString("Log");               //////Line Log
		   pVer->SetCurSel(0);

		   pExpVer->ResetContent();
		   pExpVer->AddString("Log");               //////Line Log
		   pExpVer->SetCurSel(0);

   }
//////////////////////////////////////////////////////////  
		if (g_HardWare_Pare.bSampleModeRealTime)
		{
           pRealTimeSel->SetCheck(1);
		   pLiveTimeSel->SetCheck(0);
		}
		else
		{
           pRealTimeSel->SetCheck(0);
		   pLiveTimeSel->SetCheck(1);        /////////////Realtime  Livetime
		}
    
///////////////////////////////////////////////////////////

   char c[20];

   sprintf(c,"%d",g_HardWare_Pare.iSampleTimeValue);      ////time value
   pSetTime->SetWindowText(c);

////////////////////////////////////////////////////////////
	UINT livetime;
	UINT realtime;
	float flivetime;
	float frealtime;
	float irotio;

	livetime = FileHeader.livetimeH;
	livetime = (livetime << 16) + FileHeader.livetimeL;
	m_iSaveFileLiveTime = livetime;
	flivetime = livetime /100;
	realtime = FileHeader.realtimeH;
	realtime = (realtime << 16) + FileHeader.realtimeL;
	m_iSaveFileRealTime = realtime;
	frealtime = realtime /100;
                                                      ///////////realtime livetime
	if (realtime <= 0) { realtime = 1; }
	irotio = 100*(float)(realtime - livetime)/realtime;   
    if (!m_bMCBModeSel)
    {
   /*  	char chrtext[20];
		sprintf(chrtext,"ʵʱ: %.2fs",frealtime);
		pShowRealTime->SetWindowText(chrtext);
		sprintf(chrtext,"���: %.2fs",flivetime);
		pShowLiveTime->SetWindowText(chrtext);
		sprintf(chrtext,"����: %.2f %%",irotio);
		pShowRatioTime->SetWindowText(chrtext);*/
	}
/////////////////////////////////////////////////////////////////
//    if(kkk)pStartTime->SetWindowText(stime);
/////////////////////////////////////////////////////////////////
	if (m_startflag)
	{
		g_ThreadCMD = CTL_HARDWARE_READ;
		g_ThreadCMD = IDC_CONTINUE_SAMPLE;
//      g_ThreadCMD = IDC_RECEIVE_EXTERNAL_RAM;
		SetEvent(g_hEvtSend);
		g_MCB_Oper.bQueryEn = true;
		g_MCB_Oper.bStopCMDEn = false;
    	CMainFrame* pMainFrame = (CMainFrame*)GetParentFrame();
		CButton* pRealTimeSel = (CButton*)(pMainFrame->m_wndDlgBar.GetDlgItem(IDC_REALTIME_SEL));
		CButton* pLiveTimeSel = (CButton*)(pMainFrame->m_wndDlgBar.GetDlgItem(IDC_LIVETIME_SEL));
		if (g_HardWare_Pare.bSampleModeRealTime)
		{
			pRealTimeSel->EnableWindow(TRUE);
			pLiveTimeSel->EnableWindow(FALSE);
		 }
		 else
		{
			pRealTimeSel->EnableWindow(FALSE);
			pLiveTimeSel->EnableWindow(TRUE);
		}
	}
/*
	if (m_iTimeFileSourceflag==0)
	{
		CEdit *pSource=(CEdit*)(pMainFrame->m_wndDlgBar.GetDlgItem(IDC_SOURCE));
		pSource->SetWindowText("����:MCB");
	}
	if(m_iTimeFileSourceflag==1)
	{
        CEdit *pSource=(CEdit*)(pMainFrame->m_wndDlgBar.GetDlgItem(IDC_SOURCE));
		pSource->SetWindowText("����:�ļ�");
	}
	if(m_iTimeFileSourceflag==2)
	{
        CEdit *pSource=(CEdit*)(pMainFrame->m_wndDlgBar.GetDlgItem(IDC_SOURCE));
		pSource->SetWindowText("����:��");
	}
*/

}

void CMCBView::CloseSave()////////////////////changing ljt
{

}

void CMCBView::NewIniFile()
{

}

void CMCBView::OnSelchangeComboExpHor() 
{
	// TODO: Add your control notification handler code here
  CMainFrame* pMainFrame = (CMainFrame*)GetParentFrame();
  CComboBox* pComboxHor = (CComboBox *)(pMainFrame->m_wndDlgBar.GetDlgItem(IDC_COMBO_EXP_HOR));
  int nindexHor = pComboxHor->GetCurSel();

  if (m_bMCBModeSel)
  {
	m_iSmallWindowWidthCheckSel = nindexHor;
	switch(nindexHor) 
	{
		case 0:
			m_iSamllWinWidth = 64;
			break;
		case 1:
			m_iSamllWinWidth = 128;
			break;
		case 2:
			m_iSamllWinWidth = 256;
			break;
		case 3:
			m_iSamllWinWidth = 512;
			break;
		case 4:
			m_iSamllWinWidth = 1024;
			break;
		case 5:
			m_iSamllWinWidth = 2048;
			break;
		case 6:
			m_iSamllWinWidth = 4096;
			break;
		case 7:
			m_iSamllWinWidth = 8192;
			break;
			
		default:
			break;
	}	

	m_iSmallWinCHL = m_iSmallWinCenPoint - m_iSamllWinWidth/2;
	m_iSmallWinCHR = m_iSmallWinCenPoint + m_iSamllWinWidth/2;
	if (m_iSmallWinCHL < 0) { m_iSmallWinCHL = 0; }
	if (m_iSmallWinCHR > m_iFullChannelWidth-1) { m_iSmallWinCHR = m_iFullChannelWidth;}
	m_bExpRamLongNew_valid = true;
	
	m_ExpRamDataLong_new = (m_iSmallWinCHR - m_iSmallWinCHL +1)*3;
	Redraw();
  }
  else
  {
     m_iBuffSmallWindowWidthSel=nindexHor;
	  switch(nindexHor)
	  {
		case 0:
			m_iBuffSmallWindowWidth = 64;
			break;
		case 1:
			m_iBuffSmallWindowWidth = 128;
			break;
		case 2:
			m_iBuffSmallWindowWidth = 256;
			break;
		case 3:
			m_iBuffSmallWindowWidth = 512;
			break;
		case 4:
			m_iBuffSmallWindowWidth = 1024;
			break;
		case 5:
			m_iBuffSmallWindowWidth = 2048;
			break;
		case 6:
			m_iBuffSmallWindowWidth = 4096;
			break;
		case 7:
			m_iBuffSmallWindowWidth = 8192;
			break;
			
		default:
			break;

	  }
    m_iBuffSmallWinCHL = m_iBuffSmallWinCenPoint - m_iBuffSmallWindowWidth/2;
	m_iBuffSmallWinCHR = m_iBuffSmallWinCenPoint + m_iBuffSmallWindowWidth/2;
	if (m_iBuffSmallWinCenPoint < (m_iBuffSmallWindowWidth/2))m_iBuffSmallWinCHL = 0; 
	if (m_iBuffSmallWinCHR > m_iBuffFullChannelWidth-1)m_iBuffSmallWinCHR = m_iBuffFullChannelWidth;
	Redraw();
  }
}

void CMCBView::OnSelchangeComboExpVer() 
{
	// TODO: Add your control notification handler code here
	CMainFrame* pMainFrame=	(CMainFrame*)GetParentFrame();
	CComboBox* pComboxVer = (CComboBox *)(pMainFrame->m_wndDlgBar.GetDlgItem(IDC_COMBO_EXP_VER));
	int index=pComboxVer->GetCurSel();

	if (m_bMCBModeSel)
	{
		int kkk=64;
		for(int i=0;i<17-index;i++)
		{
			kkk *= 2;
		}
		m_iExpRamEthValue = kkk;
		m_iBuffComboxSel =index;
	    Redraw();
	}
	else
	{
		int kkk=64;
		for(int i=0;i<17-index;i++)
		{
			kkk *= 2;
		}
		m_iBuffExpRamEthValue = kkk;
		m_iBuffExpRamCheckSel =index;	
	    Redraw();
	}
}

int CMCBView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
    m_scrollflag=1;
//	m_ScrollBar.Create(SBS_VERT | SBS_TOPALIGN | WS_CHILD, CRect(880-20,653*42/50+2,880,653), this, 10000);
//	m_ScrollBar.SetScrollRange(0,2000);
//  m_ScrollBar.ShowScrollBar();
	CRect ViewRec;
	GetWindowRect(ViewRec);
	m_iViewWindowSize_X = ViewRec.Width();
	m_iViewWindowSize_Y = ViewRec.Height();
	
	m_ScrollBar.Create(SBS_VERT | SBS_TOPALIGN | WS_CHILD, CRect(m_iViewWindowSize_X-20,m_iViewWindowSize_Y*42/50+2,m_iViewWindowSize_X,m_iViewWindowSize_Y), this, 10000);
	m_ScrollBar.SetScrollRange(0,2000);
//    m_ScrollBar.ShowScrollBar();
	return 0;
}

void CMCBView::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	// TODO: Add your message handler code here and/or call default

   m_pos = pScrollBar->GetScrollPos();
   
   switch(nSBCode)
   {
		case SB_LINEUP:
			if(m_pos<=m_textlength) m_pos--;
			if(m_pos<1) m_pos=0;
			pScrollBar->SetScrollPos(m_pos);break;
		case SB_LINEDOWN:
			 m_pos++;
			if(m_pos>m_textlength-1) m_pos=m_textlength;
			pScrollBar->SetScrollPos(m_pos);break;
		case SB_PAGEDOWN:
			m_pos+=2;
			if(m_pos>m_textlength-2) m_pos=m_textlength;
			pScrollBar->SetScrollPos(m_pos);break;
		case SB_PAGEUP:
			m_pos-=2;
			if(m_pos<2) m_pos=0;
			pScrollBar->SetScrollPos(m_pos);break;
		case SB_THUMBTRACK:
			m_pos=nPos;
			if (nPos>m_textlength-1) m_pos=m_textlength;
			pScrollBar->SetScrollPos(m_pos);break;
   }
    OnDrawTextBgroud(GetDC());
	CView::OnVScroll(nSBCode, nPos, pScrollBar);
}

void CMCBView::TextScroll(CString s)
{
    m_textlength++;
	if(m_textlength>=5)
	{
		m_pos=m_textlength-1;

	}
//	m_pos=m_textlength-1;
	if (m_textlength>1999)
	{
		m_textlength=2;
        m_pos=1;
		for (int i=2;i<2000;i++)
		{
			m_scrollstring[i].Empty();
		}
		MessageBox("     ��־������E�����\nǰ����־�Ѿ���ϵͳ�Զ�ɾ��!");
	}
	m_scrollstring[m_textlength]=s;
	m_ScrollBar.SetScrollRange(0,m_textlength);
	m_ScrollBar.SetScrollPos(m_pos);
	Redraw();
	m_ScrollBar.ShowWindow(FALSE);
	m_ScrollBar.ShowWindow(TRUE);
}

void CMCBView::OnPrintDrawTextBgroud(CDC *pDC)
{
	CBrush bShow;
    CPen pen(PS_SOLID,1,RGB(255,255,255));
    bShow.CreateSolidBrush(RGB(255,255,255));
	CBrush* pOldBrush=pDC->SelectObject(&bShow);
	pDC->SelectObject(&pen);
    pDC->Rectangle(0,m_iViewWindowSize_Y*42/50,m_iViewWindowSize_X,m_iViewWindowSize_Y);//
    pen.DeleteObject();
	bShow.DeleteObject();
}

void CMCBView::OnMove(int x, int y) 
{
	CView::OnMove(x, y);
	// TODO: Add your message handler code here
 
}

void CMCBView::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	// TODO: Add your message handler code here and/or call default
//	MessageBox("sdfsdf");
	CView::OnHScroll(nSBCode, nPos, pScrollBar);
}
