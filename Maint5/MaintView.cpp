// MaintView.cpp : implementation of the CMaintView class
//

#include "stdafx.h"
#include "Maint.h"
#include <math.h>

#include "MaintDoc.h"
#include "MaintView.h"
#include "MainFrm.h"

#include "Set64EnergeLimit.h"

#include "ManulModifyAnolPara.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern unsigned char g_chFileType;
extern int g_DataSample_Mod_Sel;

extern int	g_SamplingTime;
extern int  g_SampleMaxTime;
extern CString g_strFileName;
extern CString g_strBuckFileName;
extern int	g_Port_Sel;
extern int  g_HostBaud;
extern int  g_BucketBaud;

extern TYPE type;

//位置边界保存标志
extern BOOL g_bPLimitChangeValid;

//效率统计 评估
BOOL g_bOperating_32bkEffectEvaluate = false;
CString	g_strOperating_32bkEffectFilePath;
int g_iEffect32bk_goto_one[64];
float g_fEffect32bk_Evalu_SquE;
int g_iOperatingEffect_block = 0;

//32峰位统计
BOOL g_bOperating_32bkPeakStat = false;
CString g_strOperating_32bkPeakFilePath;

HANDLE		g_hEvtSend;

int g_Blockindex = 0;
int g_Bucketindex = 0;
BOOL g_bChannelChangeSel = true;
int g_Backup_iMidfilter_sdot[64][64];  //中值滤波备份

HANDLE g_hEvtRec;
BOOL	g_bEvtSend = false;
CRITICAL_SECTION  g_ClkRecSafe;

int g_draw_size=8;
int g_draw_frame = 5;

int g_draw_startpoint_x = 50;
int g_draw_startpoint_y = 90;

bool g_bshow_status_3D = true;
int g_2D_Loffset;

int B_X[9] = {1,5,12,20,30,40,48,58,64};
int B_Y[9] = {1,5,12,20,30,40,48,58,64};

/////////////////////////////////////////////////////////////////////////////
// CMaintView

IMPLEMENT_DYNCREATE(CMaintView, CView)

BEGIN_MESSAGE_MAP(CMaintView, CView)
	//{{AFX_MSG_MAP(CMaintView)
	ON_BN_CLICKED(IDC_SHOWBAR_3D_RADIO, OnShowbar3dRadio)
	ON_BN_CLICKED(IDC_SHOWBAR_2D_RADIO, OnShowbar2dRadio)
	ON_BN_CLICKED(IDC_2D_SHOW_X, On2dShowX)
	ON_BN_CLICKED(IDC_2D_SHOW_Y, On2dShowY)
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_BN_CLICKED(IDC_CALCULATE_LIMIT, OnCalculateLimit)
	ON_BN_CLICKED(IDC_SAVE_CUR_LIMIT, OnSaveCurLimit)
	ON_BN_CLICKED(IDC_REDRAW_LIMIT, OnRedrawLimit)
	ON_BN_CLICKED(IDC_LIMIT_STAT, OnLimitStat)
	ON_BN_CLICKED(IDC_OPEN_LIMITTAB, OnOpenLimittab)
	ON_COMMAND(ID_MENU_FASTOPEN, OnMenuFastopen)
	ON_CBN_SELCHANGE(IDC_SHOW_BLOCK_SEL, OnSelchangeShowBlockSel)
	ON_BN_CLICKED(IDC_OFFSET1_ADD, OnOffset1_Add)
	ON_BN_CLICKED(IDC_OFFSET1_DEC, OnOffset1_Dec)
	ON_WM_RBUTTONDOWN()
	ON_WM_TIMER()
	ON_COMMAND(IDC_MENU_DATASAMPLE, OnMenuDatasample)
	ON_COMMAND(IDC_MENU_ANOL, OnMenuAnol)
	ON_BN_CLICKED(IDC_EN_MOVEWHOLELINE, OnEnMovewholeline)
	ON_BN_CLICKED(IDC_PEAK_VALE_STAT, OnPeakValeStat)
	ON_BN_CLICKED(IDC_CHECK_VER_HOR, OnCheckVerHor)
	ON_WM_KEYUP()
	ON_WM_KEYDOWN()
	ON_BN_CLICKED(IDC_CHECK_MIDFILTER, OnCheckMidfilter)
	ON_COMMAND(ID_MENU_LOAD32BK, OnMenuLoad32bk)
	ON_COMMAND(AFX_ID_VIEW_LP_EVALUATE, OnLP_Evaluate)
	ON_BN_CLICKED(IDC_EFFECENT_EVALUAT, OnEffecentEvaluat)
	ON_COMMAND(AFX_ID_VIEW_32BKLP_EVALUATE, OnView32bklpEvaluate)
	ON_COMMAND(ID_MENU_32BUCKET_PEAK_STAT, OnMenu32bucketPeakStat)
	ON_BN_CLICKED(IDC_REDRAW, OnRedrawButton)
	ON_WM_ERASEBKGND()
	ON_WM_SIZE()
	ON_COMMAND(ID_MENU_32BK_PARA_MODIFY, OnMenu32bkParaModify)
	ON_COMMAND(ID_MENU_32BK_PARA_MANUL_MODIFY, OnMenu32bkParaManulModify)
	ON_COMMAND(ID_MENU_FASTOPEN32FILE, OnMenuFastopen32file)
	ON_COMMAND(ID_FASTOPEN32FILE_NEXT, OnMenuFastopen32fileNext)
	ON_COMMAND(ID_FASTOPEN32FILE_LAST, OnMenuFastopen32fileLast)
	ON_WM_RBUTTONDBLCLK()
	ON_WM_LBUTTONDBLCLK()
	ON_COMMAND(IDC_FILE_SAVE_ENERGE_LIMIT, OnFileSaveEnergeLimit)
	ON_COMMAND(ID_OUTPUT_32BKLP_EVALUATE, OnOutput32bklpEvaluate)
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_MOD_GOODBYE,OnModGoodBye)

//	ON_MESSAGE(WM_AUTOSAMPLE_RECFINISH, OnAutoSample_RecData)  //


	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)

END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMaintView construction/destruction

CMaintView::CMaintView()
{

	m_pCacheBitmap = new CBitmap;
	m_pview = new CNModeless(this);
	m_bEnLimitLineRedraw = false;
	m_bEnLimitStat_MemDCRedraw = false;
	m_bEnPeakValeStat_MemDCRedraw = false;


	m_ShowModelSel = 10;   //显示默认背景

	m_pLMouseDown = false;

	m_bEnSave_Edit_B_Lab = false;
	bDirectShift_X_Y = true;
	bSetValue_OverLimit = false;
	m_bDraw_Small_X = false;
	m_bDraw_Small_Y = false;
	m_bEnShowPeakValeRatio = false;

	g_bPLimitChangeValid = false;

	//-----能谱操作----//
	m_bEnergeSpectOperate_En = false;
	m_iEnergeSpectShowNum = 0;
	m_bEnergeSpectZoomInMode = false;
	m_bEnergeSpectSelEn = false;
	
	m_iEnergeSpectCurBlockNo = 0;
	m_iEnergeSpectCurSynNo = 0; //当前晶体号
	
	m_bEnergeSpectMove_Eth_Low = false;
	m_bEnergeSpectMove_Eth_Hight = false;
	
	memset(m_iEnergeSpectrum64,0,8*64*64*4);
	
	for(int i=0;i<8;i++){	//能谱阈值 初始化
		for(int j=0;j<64;j++) {
			m_iEnergeSpectEth_Low[i][j] = 0;
			m_iEnergeSpectEth_Hight[i][j] = 63;
		}
	}

	//-------【散点边界处理】----------//
	m_bButtonDown = false;
	m_bMoveLongLine = false;
	m_bSameDriction_X = true;
	m_bShort_Line_Valid = false;
	m_bthisMoveInValid = false;

	for(i=0;i<64;i++) { //横坐标
			for(int j=0;j<64;j++) {
				sdot[i][j] = 0;					
			}	
	}

	for(i=0;i<8;i++) {
			for(int j=0;j<8;j++) {						
				m_Position_X0[i][j] = j*8;
				m_Position_Y0[i][j] = i*8;
				m_Position_X1[i][j] = (j+1)*8 -1;
				m_Position_Y1[i][j] = (i+1)*8 -1;
			}
	}
	for(int block=0;block<8;block++) {
		for(i=0;i<8;i++) {
				for(int j=0;j<8;j++) {						
					m_8Block_Position_X0[block][i][j] = j*8;
					m_8Block_Position_Y0[block][i][j] = i*8;
					m_8Block_Position_X1[block][i][j] = (j+1)*8 -1;
					m_8Block_Position_Y1[block][i][j] = (i+1)*8 -1;
				}
		}
	}

	memset(m_Efficent_Evalu_FileA_Max_X,0,8*4);
	memset(m_Efficent_Evalu_FileA_Max_Y,0,8*4);
	memset(m_Efficent_Evalu_Max_X,0,8*4);
	memset(m_Efficent_Evalu_Max_Y,0,8*4);

	memset(m_Efficent_Evalu_FileA_data,0,8*8*8*4);
	m_bEnEfficent_Evaluate = false;
	InitializeCriticalSection(&g_ClkRecSafe);

	g_bOperating_32bkEffectEvaluate = false;

}

CMaintView::~CMaintView()
{
	delete m_pview;

	m_pCacheBitmap->DeleteObject();

	CloseHandle(g_hEvtRec);
	CloseHandle(g_hEvtSend);

	DeleteCriticalSection(&g_ClkRecSafe);
}

BOOL CMaintView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

//	delete m_pload32bk;

	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CMaintView drawing

void CMaintView::OnDraw(CDC* pDC)
{
	CMaintDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	CBitmap bitmap;
	CDC dcMemory;

	CRect clientrect;
	GetClientRect(clientrect);

	switch(m_ShowModelSel) {
	case 0:  //3 D 重画
//		GetClientRect(&rcClient);
//		pDC->FillSolidRect(&rcClient,rf);		
		OnRedraw();		
		break;
	case 1:  //位置边界刷新；
		RedrawLimitFile();
		break;
	case 2:  //64能谱显示；
		OnRedraw();
		m_bEnergeSpectSelEn = false;
		m_bEnergeSpectZoomInMode =false; //先重画一下3d 试图

		//开始画64能谱图象
		m_bEnergeSpectOperate_En = true;  
		memcpy(m_iEnergeSpectrum64,pDoc->BuffDot3w,8*64*64*4);
		OnDraw64EngerSpectrum();
		m_bEnergeSpectSelEn = true;
		m_bEnergeSpectZoomInMode =false;
		break;
	case 3: //峰谷比统计
		OnPeakValeStat();
		break;
	case 4: //效率统计
		OnLimitStat();
		break;
	case 5:  //偏移刷新
		OnEffecentEvaluat();
		break;

	case 10: //初始化背景
		bitmap.LoadBitmap(IDB_BACKGROUND);
		dcMemory.CreateCompatibleDC(pDC);
		dcMemory.SelectObject(&bitmap);
		pDC->BitBlt(0,0,clientrect.Width(),clientrect.Height(),&dcMemory,0,0,SRCCOPY);
		
		break;
	default:

		OnRedraw();
		break;
	}


}

/////////////////////////////////////////////////////////////////////////////
// CMaintView printing

BOOL CMaintView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CMaintView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CMaintView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CMaintView diagnostics

#ifdef _DEBUG
void CMaintView::AssertValid() const
{
	CView::AssertValid();
}

void CMaintView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CMaintDoc* CMaintView::GetDocument() // non-debug version is inline
{
	//ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMaintDoc)));
	m_pDocument->IsKindOf(RUNTIME_CLASS(CMaintDoc));
	return (CMaintDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMaintView message handlers



void CMaintView::OnRedrawButton() 
{	
	m_ShowModelSel = 0;
	OnRedraw();
}

void CMaintView::OnRedraw()
{
	CMaintDoc*  pDoc = GetDocument();
	CString m_strEditThresh;
    CString m_strEditOffset;
	CString m_strEditLOffset;
	CString m_strEditHOffset;
	int tplace;
	long Offset;
	long LOffset,HOffset;
	long longtdata[65];
	CFile fShowFile;

	m_bMoving64EnergLimitEn = false;

	CMainFrame* pMainFrame=
	      (CMainFrame*)GetParentFrame();
	CComboBox *pBlockindex = (CComboBox *)
		(pMainFrame)->m_wndShowBar.GetDlgItem(IDC_SHOW_BLOCK_SEL);
//	CEdit *pSelFile= (CEdit *)
//		(pMainFrame)->m_wndShowBar.GetDlgItem(IDC_SHOW_STATIC_SELFILE);
	CEdit* pThreshvalue=(CEdit*)
		(pMainFrame->m_wndShowBar.GetDlgItem(IDC_3D_SHOW_THREVALUE));	
	CEdit* pLOffsetvalue=(CEdit*)
		(pMainFrame->m_wndShowBar.GetDlgItem(IDC_2D_SHOW_OFFSET1));
	CEdit* pHOffsetvalue=(CEdit*)
		(pMainFrame->m_wndShowBar.GetDlgItem(IDC_2D_SHOW_OFFSET2));

	CButton *pBlockButton = (CButton *)
		(pMainFrame)->m_wndShowBar.GetDlgItem(IDC_CHECK_MIDFILTER);


	pStatus=(CStatusBar*)
			AfxGetApp()->m_pMainWnd->GetDescendantWindow(ID_VIEW_STATUS_BAR);

	
	int lenth = g_strFileName.GetLength();
	if (lenth > 72) {
		char chtemp[500];
		char chfilename[400];
		int i;
		for(i=0;i<lenth;i++) {
			chtemp[i] = g_strFileName.GetAt(i);
		}
		sprintf(chfilename,"%c%c%c ...",chtemp[0],chtemp[1],chtemp[2]);

		for(i=lenth-69;i<lenth;i++) {
			sprintf(chfilename,"%s%c",chfilename,chtemp[i]);
		}
		
		if(g_bPLimitChangeValid) {
			sprintf(chfilename,"%s%c",chfilename,'*');
		}

		pStatus->SetPaneText(3,chfilename,1);
	}
	else {
		if(g_bPLimitChangeValid) {
			pStatus->SetPaneText(3,g_strFileName+"*",1);
		}
		else {
			pStatus->SetPaneText(3,g_strFileName,1);
		}
	}
	
	

	int nBlindex= pBlockindex->GetCurSel();

	if (g_bOperating_32bkEffectEvaluate) {
		nBlindex = g_iOperatingEffect_block;		
	}


	g_Blockindex = nBlindex;
	m_iEnergeSpectCurBlockNo = nBlindex;

	if(pDoc->bFirstFileOpened ) {
		pDoc->bFirstFileOpened = false;

		memcpy(g_Backup_iMidfilter_sdot,pDoc->BuffDot3w[nBlindex],64*64*4);
		if(m_bEnMidFilter) { //进行中值滤波
			pBlockButton->SetCheck(0);
			m_bEnMidFilter = false;
			memcpy(pDoc->BuffDot3w[m_Midfilter_Old_BlockNo],g_Backup_iMidfilter_sdot,64*64*4);
		}
	}


	m_bEnergeSpectOperate_En = false;
	m_bEnergeSpectZoomInMode =false;

	//取消峰谷比显示
	m_bEnShowPeakValeRatio = false;

	if(g_chFileType == 0xC5) { //能谱文件
		m_bEnergeSpectOperate_En = true;
		m_bEnergeSpectSelEn = false;	//akang edit 	

		memcpy(m_iEnergeSpectrum64,pDoc->BuffDot3w,8*64*64*4);
	}

	if (m_ShowModelSel == 2) {   //64能谱显示时用的， 显示64能谱时不刷新3D视图；		
		m_Show64EngerSpectrum();  //akang915
		return;
	}

	if (m_ShowModelSel == 4) { // 效率统计刷新
		m_bEnLimitStat_MemDCRedraw = true;
	}

	if((nBlindex<0)||(nBlindex>7)) {
			return ;
	}

	for(int j=0;j<64;j++) {
			for (int k=0;k<64;k++) {
				sdot[j][k] = pDoc->BuffDot3w[nBlindex][j][k];
			}
	}

	if (g_bOperating_32bkEffectEvaluate) {
		return;
	}

	CDC* pDC=GetDC();

	CRect clientrect;
	GetClientRect(clientrect);

	CDC MemDC; //首先定义一个显示设备对象
	CBitmap MemBitmap;//定义一个位图对象	
	MemDC.CreateCompatibleDC(NULL);
	MemBitmap.CreateCompatibleBitmap(pDC,clientrect.Width(),clientrect.Height());
	CBitmap *pOldBit=MemDC.SelectObject(&MemBitmap);
	
	MemDC.FillSolidRect(0,0,clientrect.Width(),clientrect.Height(),RGB(26,131,162));

	
	if(m_iValue3d_2d == 3) { //show 3d 	
		g_bshow_status_3D = true;

		//统计总计数
		UINT allCounter = 0;
		for(int j=0;j<64;j++) {
			for(int k=0;k<64;k++) {			
				allCounter += pDoc->BuffDot3w[nBlindex][j][k];
			}
		}

		pThreshvalue->GetWindowText(m_strEditThresh);
	   if(!m_strEditThresh.IsEmpty())
	   {		
            pDoc->temp=atol(m_strEditThresh);        
	        pThreshvalue->Invalidate();
	   }
	   else
	   {	//找最大值
		   pDoc->temp = pDoc->BuffDot3w[nBlindex][0][0];  //找第 0 块数据
		   for(int j=0;j<64;j++) {
				for(int k=0;k<64;k++) {
						if(pDoc->BuffDot3w[nBlindex][j][k] >= pDoc->temp)
							pDoc->temp = pDoc->BuffDot3w[nBlindex][j][k];
				}
		   }
	   }

		COLORREF rf=RGB(26,131,162);//RGB(15,155,85); 
		CRect rcClient;
		GetClientRect(&rcClient);
		MemDC.FillSolidRect(&rcClient,rf);
		MemDC.SetTextColor(RGB(255,255,0));

		CPen pShow(PS_SOLID,1,RGB(255,255,0));//RGB(0,0,255)
	    MemDC.SelectObject(&pShow);

		switch (g_chFileType) {
			case 0x22: {
						MemDC.TextOut(250,5," 未定义格式   3D 视图 ");
						MemDC.TextOut(200,23," --------------------   水平 方向   -------------------> ");
						MemDC.MoveTo(55,105);
						MemDC.LineTo(55,190);
						MemDC.TextOut(50,200,"垂");
						MemDC.TextOut(50,220,"直");
						MemDC.TextOut(50,240,"方");
						MemDC.TextOut(50,260,"向");
						MemDC.TextOut(52,360,"V");
						MemDC.MoveTo(55,280);
						MemDC.LineTo(55,365);				
						break;
					   }
			case 0xc7: { //散点
						//pSelFile->SetWindowText("文件类型为： \n\n 散点模式文件");
						MemDC.TextOut(250,5," 散点模式    3D 视图 ");
						MemDC.TextOut(200,23," --------------------   X 方向   -------------------> ");
						MemDC.MoveTo(55,105);
						MemDC.LineTo(55,190);
						MemDC.TextOut(50,200,"Y");
						MemDC.TextOut(50,220,"方");
						MemDC.TextOut(50,240,"向");
						MemDC.TextOut(52,360,"V");
						MemDC.MoveTo(55,280);
						MemDC.LineTo(55,365);
						break;
					   }
			case 0xc5: { //能量
						//pSelFile->SetWindowText("文件类型为： \n\n  能量模式文件");
						MemDC.TextOut(250,5," 能方格式   3D 视图 ");
						MemDC.TextOut(200,23," --------------------   E 方向   -------------------> ");
						MemDC.MoveTo(55,105);
						MemDC.LineTo(55,190);
						MemDC.TextOut(50,200,"条");
						MemDC.TextOut(50,220,"号");
						MemDC.TextOut(50,240,"方");
						MemDC.TextOut(50,260,"向");
						MemDC.TextOut(52,360,"V");
						MemDC.MoveTo(55,280);
						MemDC.LineTo(55,365);
						break;
					   }
			case 0xc6: { //时方
						//pSelFile->SetWindowText("文件类型为： \n\n  时方模式文件");
						MemDC.TextOut(250,5," 时方格式   3D 视图 ");
						MemDC.TextOut(200,23," --------------------   时间 方向   -------------------> ");
						MemDC.MoveTo(55,105);
						MemDC.LineTo(55,190);
						MemDC.TextOut(50,200,"条");
						MemDC.TextOut(50,220,"号");
						MemDC.TextOut(50,240,"方");
						MemDC.TextOut(50,260,"向");
						MemDC.TextOut(52,360,"V");
						MemDC.MoveTo(55,280);
						MemDC.LineTo(55,365);
						break;
					   }
			case 0xc8: { //符合
						//pSelFile->SetWindowText("文件类型为： \n\n  符合模式文件");
						MemDC.TextOut(250,5," 符合0 格式    3D 视图 ");
						MemDC.TextOut(200,23," --------------------   时间 方向   -------------------> ");
						MemDC.MoveTo(55,105);
						MemDC.LineTo(55,190);
						MemDC.TextOut(50,200,"条");
						MemDC.TextOut(50,220,"号");
						MemDC.TextOut(50,240,"方");
						MemDC.TextOut(50,260,"向");
						MemDC.TextOut(52,360,"V");
						MemDC.MoveTo(55,280);
						MemDC.LineTo(55,365);
						break;
					   }
			case 0xc9: { //符合
						//pSelFile->SetWindowText("文件类型为： \n\n  符合模式文件");
						MemDC.TextOut(250,5," 符合1 格式    3D 视图 ");
						MemDC.TextOut(200,23," --------------------   时间 方向   -------------------> ");
						MemDC.MoveTo(55,105);
						MemDC.LineTo(55,190);
						MemDC.TextOut(50,200,"条");
						MemDC.TextOut(50,220,"号");
						MemDC.TextOut(50,240,"方");
						MemDC.TextOut(50,260,"向");
						MemDC.TextOut(52,360,"V");
						MemDC.MoveTo(55,280);
						MemDC.LineTo(55,365);
						break;
					   }
			default :  { //unkown
						//pSelFile->SetWindowText("文件类型为： \n\n  不认识的文件");
						MemDC.TextOut(250,5," 未定义格式 3D 视图 ");
						MemDC.TextOut(200,23," --------------------   水平 方向   -------------------> ");
						MemDC.MoveTo(55,105);
						MemDC.LineTo(55,190);
						MemDC.TextOut(50,200,"垂");
						MemDC.TextOut(50,220,"直");
						MemDC.TextOut(50,240,"方");
						MemDC.TextOut(50,260,"向");
						MemDC.TextOut(52,360,"V");
						MemDC.MoveTo(55,280);
						MemDC.LineTo(55,365);	
						break;
					   }
		}

		
		CString strshowMax;
		strshowMax.Format(" 阈值 为 %d ",pDoc->temp);
		MemDC.SetTextColor(RGB(255,255,0));
		MemDC.TextOut(250,g_draw_startpoint_x + g_draw_size*64 + 10 ,strshowMax);
		
		strshowMax.Format("总事例数： %d",allCounter);
		MemDC.TextOut(480,6,strshowMax);

		if(pDoc->m_chrAnolParam[nBlindex][1] != 0) {//[nBlindex][0] 
			MemDC.TextOut(90,g_draw_startpoint_x + g_draw_size*64 + 35 ,"模拟板参数：");
			strshowMax.Format("(放大倍数) A路:  %d     B路: %d   C 路: %d      D 路: %d ",pDoc->m_chrAnolParam[nBlindex][1],
				pDoc->m_chrAnolParam[nBlindex][2],pDoc->m_chrAnolParam[nBlindex][3],pDoc->m_chrAnolParam[nBlindex][4]);
			MemDC.TextOut(190,g_draw_startpoint_x + g_draw_size*64 + 35 ,strshowMax);
			strshowMax.Format("X偏置:  %d   Y偏置: %d   E偏置: %d  Eth能阈: %d ",pDoc->m_chrAnolParam[nBlindex][5],
				pDoc->m_chrAnolParam[nBlindex][6],pDoc->m_chrAnolParam[nBlindex][7],pDoc->m_chrAnolParam[nBlindex][8]);
			MemDC.TextOut(190,g_draw_startpoint_x + g_draw_size*64 + 55 ,strshowMax);
		
		}
		else {
			MemDC.TextOut(90,g_draw_startpoint_x + g_draw_size*64 + 35 ,"模拟板参数：");
			MemDC.TextOut(190,g_draw_startpoint_x + g_draw_size*64 + 35 ,"(放大倍数) A路: **     B路: **    C 路: **     D 路: ** ");
			MemDC.TextOut(190,g_draw_startpoint_x + g_draw_size*64 + 55 ,"X偏置: **   Y偏置: **   E偏置: **  Eth能阈:   ** ");
		}

	    CPen pShowBlack(PS_SOLID,1,RGB(0,0,0));//RGB(0,0,255)
	    MemDC.SelectObject(&pShowBlack);
	   int y = g_draw_startpoint_x;	   	 
	   for(j=0;j<64;j++) {
		    int x = g_draw_startpoint_y;
		   	for(int k=0;k<64;k++) {	
					CBrush bShow;							
					bShow.CreateSolidBrush(RGB(GetBValue(ColorSet(pDoc->BuffDot3w[nBlindex][j][k],pDoc->temp)), 
				    GetGValue(ColorSet(pDoc->BuffDot3w[nBlindex][j][k],pDoc->temp)),
				    GetRValue(ColorSet(pDoc->BuffDot3w[nBlindex][j][k],pDoc->temp))));// 30003000				 					  
					CBrush* pOldBrush=MemDC.SelectObject(&bShow);// 3000 
					//pDshow->Rectangle(x-g_draw_frame,y-g_draw_frame,x+g_draw_frame,y+g_draw_frame);// 
					MemDC.Rectangle(x-g_draw_frame,y-g_draw_frame,x+g_draw_frame,y+g_draw_frame);// 

					//pDshow->SelectObject(pOldBrush);//++++
					MemDC.SelectObject(pOldBrush);//++++					
					bShow.DeleteObject();
					x += g_draw_size;
			}
			y += g_draw_size;
	   }
	   

	} //end of show 3d
	else if(m_iValue3d_2d == 2){   // ---『show 2d 』---//	

		g_bshow_status_3D = false;

	   pThreshvalue->GetWindowText(m_strEditThresh);
	   if(!m_strEditThresh.IsEmpty())
	   {		
            pDoc->temp=atol(m_strEditThresh);        
	     //   pThreshvalue->Invalidate();
	   }
	   else
	   {    //找最大值
		    pDoc->temp = pDoc->BuffDot3w[nBlindex][0][0];  //找第 0 块数据
			for(int j=0;j<64;j++)
				for(int k=0;k<63;k++) {
						if(pDoc->BuffDot3w[nBlindex][j][k] >= pDoc->temp)
							pDoc->temp = pDoc->BuffDot3w[nBlindex][j][k];
				}
	   }

		pLOffsetvalue->GetWindowText(m_strEditLOffset);
		pHOffsetvalue->GetWindowText(m_strEditHOffset);

		//COLORREF rf=RGB(15,155,85); //rf=RGB( 15, 125,185); 
		COLORREF rf=RGB(26,131,162);//RGB(15,155,85); 
		CRect rcClient;
		GetClientRect(&rcClient);
		MemDC.FillSolidRect(&rcClient,rf);

        CPen pShow(PS_SOLID,2,RGB(15, 105,185));//RGB(0,210,0)
	    CPen* pOldPen=NULL;
	    pOldPen=MemDC.SelectObject(&pShow);

		MemDC.MoveTo(g_draw_startpoint_y,g_draw_startpoint_x); 
		MemDC.LineTo(g_draw_startpoint_y,g_draw_size*64+g_draw_startpoint_x);
		MemDC.MoveTo(g_draw_startpoint_y,g_draw_startpoint_x);
		MemDC.LineTo(g_draw_startpoint_y-3,g_draw_startpoint_x+5);

	    MemDC.MoveTo(g_draw_startpoint_y,g_draw_size*64+g_draw_startpoint_x);
		MemDC.LineTo(g_draw_startpoint_y +g_draw_size*64 ,g_draw_size*64+g_draw_startpoint_x);
	    MemDC.MoveTo(g_draw_startpoint_y +g_draw_size*64,g_draw_size*64+g_draw_startpoint_x);
        MemDC.LineTo(g_draw_startpoint_y +g_draw_size*64 - 5,g_draw_size*64+g_draw_startpoint_x +3);

		CString strshowMax;
	    MemDC.SetTextColor(RGB(255,255,0));
	    MemDC.TextOut(40,40,"阈值");
		strshowMax.Format("%d ",pDoc->temp);
	    MemDC.TextOut(40,55,strshowMax);

	    MemDC.TextOut(40,g_draw_size*64+g_draw_startpoint_x +5,"(0,0)");

	    MemDC.TextOut(g_draw_startpoint_y +g_draw_size*64 - 10,g_draw_size*64+g_draw_startpoint_x +5,"No. 63");

		if(m_iValueX_Y == 1) {
			MemDC.TextOut(155,g_draw_size*64+g_draw_startpoint_x +5,"X 方向Offset: ");
		}else MemDC.TextOut(155,g_draw_size*64+g_draw_startpoint_x +5,"Y 方向Offset: ");
		MemDC.TextOut(250,g_draw_size*64+g_draw_startpoint_x +5,m_strEditLOffset);
		MemDC.TextOut(270,g_draw_size*64+g_draw_startpoint_x +5," -- ");
		MemDC.TextOut(290,g_draw_size*64+g_draw_startpoint_x +5,m_strEditHOffset);


		if((!m_strEditLOffset.IsEmpty())&&(!m_strEditHOffset.IsEmpty())) {
			LOffset=atol(m_strEditLOffset);
			HOffset=atol(m_strEditHOffset);

			g_2D_Loffset = LOffset;
			if((LOffset>=64)||(LOffset<0)) {
					AfxMessageBox(" Offset 越限啦 !\n（在0－－63之间） ");
						   
					goto ExitRedraw;
					
			}
			if((HOffset>=64)||(HOffset<0)) {
					AfxMessageBox(" Offset 越限啦 !\n（在0－－63之间） ");

					goto ExitRedraw;
					//将内存中的图拷贝到屏幕上进行显示
					pDC->BitBlt(0,0,clientrect.Width(),clientrect.Height(),&MemDC,0,0,SRCCOPY);
			}
			if(LOffset > HOffset) { 
					AfxMessageBox(" Offset1 应该 小于 Offset2 ");
					goto ExitRedraw;
			}
//---------------------------------------------
			if(m_iValueX_Y == 1) { // X
	
				for(int m=0;m<64;m++)  longtdata[m]=0;
				for(int k=0;k<64;k++) {
					for(int i=LOffset;i<=HOffset;i++) {					
						longtdata[k] +=  pDoc->BuffDot3w[nBlindex][k][i];
					}
					//longtdata[k] = (long)(longtdata[k]/(HOffset+1-LOffset));
				}	
				if(m_strEditThresh.IsEmpty()) { //再次选最大值
					pDoc->temp = longtdata[0];
					for(int n=0;n<63;n++) {
						if(longtdata[n] >= pDoc->temp) 
							pDoc->temp = longtdata[n];
					}
				} else pDoc->temp=atol(m_strEditThresh); 

				int x = g_draw_startpoint_y; 
				for(int i=0;i<64;i++) {			
					if(pDoc->temp<1) pDoc->temp = 1;

					tplace = g_draw_size*64 + g_draw_startpoint_x- (int)(g_draw_size*64*(float)longtdata[i]/(pDoc->temp));
					if(tplace < 30) tplace = 30;
					if(tplace > g_draw_size*64 + g_draw_startpoint_x) tplace = g_draw_size*64;

					CBrush bShow;				
					bShow.CreateSolidBrush(RGB(
								   GetBValue(ColorSet(longtdata[i],pDoc->temp)), 
								   GetGValue(ColorSet(longtdata[i],pDoc->temp)),
								   GetRValue(ColorSet(longtdata[i],pDoc->temp))));			 					  
					CBrush* pOldBrush=MemDC.SelectObject(&bShow);// 
					MemDC.Rectangle(x-g_draw_frame,tplace-g_draw_frame,x+g_draw_frame,tplace+g_draw_frame);// 
						
					MemDC.SelectObject(pOldBrush);//++++
					bShow.DeleteObject();
					x += g_draw_size;
				}
						   //画二维 直线
				CPen pShow(PS_SOLID,2,RGB(85, 15,185));//RGB(0,210,0)
				CPen* pOldPen=NULL;
				pOldPen=MemDC.SelectObject(&pShow);
				x = g_draw_startpoint_y+8 ; 
				tplace = g_draw_size*64 + g_draw_startpoint_x - (int)(g_draw_size*64*longtdata[0]/(pDoc->temp));
				if(tplace < 30) tplace = 30;
				if(tplace > g_draw_size*64 + g_draw_startpoint_x) tplace = g_draw_size*64;
				for(int kk=1;kk<64;kk++) {																				
					MemDC.MoveTo(x-8,tplace);
					x += g_draw_size;
					tplace = g_draw_size*64 + g_draw_startpoint_x - (int)(g_draw_size*64*longtdata[kk]/(pDoc->temp));
					if(pDoc->temp < 1) pDoc->temp = 1;
					if(tplace < 30) tplace = 30;
					if(tplace > g_draw_size*64 + g_draw_startpoint_x) tplace = g_draw_size*64;
					MemDC.LineTo(x-8,tplace);
				}
			} //m_iValueX_Y = X
			else if(m_iValueX_Y == 2) { //m_iValueX_Y = Y  Offset1 ,Offset2 都有效时

				//CDC* pDshow=GetDC(); 				
				for(int m=0;m<64;m++)  longtdata[m]=0;
				for(int k=0;k<64;k++) {
					for(int i=LOffset;i<=HOffset;i++) {					
						longtdata[k] +=  pDoc->BuffDot3w[nBlindex][i][k];
					}
					//longtdata[k] = (long)(longtdata[k]/(HOffset+1-LOffset)); //akang 去掉了
				}	
				if(m_strEditThresh.IsEmpty()) { //再次选最大值
					pDoc->temp = longtdata[0];
					for(int n=0;n<63;n++) {
						if(longtdata[n] >= pDoc->temp) 
							pDoc->temp = longtdata[n];
					}
				} else pDoc->temp=atol(m_strEditThresh); 

				int x = g_draw_startpoint_y; 
				for(int i=0;i<64;i++) {
					if(pDoc->temp < 1) pDoc->temp = 1;
					tplace = g_draw_size*64 + g_draw_startpoint_x - (int)(g_draw_size*64*longtdata[i]/(pDoc->temp));
					if(tplace < 30) tplace = 30;
					if(tplace > g_draw_size*64 + g_draw_startpoint_x) tplace = g_draw_size*64;
					CBrush bShow;				
					bShow.CreateSolidBrush(RGB(
								   GetBValue(ColorSet(longtdata[i],pDoc->temp)), 
								   GetGValue(ColorSet(longtdata[i],pDoc->temp)),
								   GetRValue(ColorSet(longtdata[i],pDoc->temp))));			 					  
					CBrush* pOldBrush=MemDC.SelectObject(&bShow);// 
					MemDC.Rectangle(x-g_draw_frame,tplace-g_draw_frame,x+g_draw_frame,tplace+g_draw_frame);// 
						
					MemDC.SelectObject(pOldBrush);//++++
					bShow.DeleteObject();
					x += g_draw_size;
				}

				CPen pShow(PS_SOLID,2,RGB(85, 15,185));//   RGB(0,210,0)
				CPen* pOldPen=NULL;
				pOldPen=MemDC.SelectObject(&pShow);
				x = g_draw_startpoint_y+8 ; 
				tplace = g_draw_startpoint_x + 64*g_draw_size- (int)(g_draw_size*64*longtdata[0]/(pDoc->temp));
				
				if(tplace < 30) tplace = 30;
				if(tplace > g_draw_size*64 + g_draw_startpoint_x) tplace = g_draw_size*64;
				
				for(int kk=1;kk<64;kk++) {																				
					MemDC.MoveTo(x-8,tplace);
					x += g_draw_size;
					tplace = g_draw_size*64 + g_draw_startpoint_x - (int)(g_draw_size*64*longtdata[kk]/(pDoc->temp));
					if(pDoc->temp < 1) pDoc->temp = 1;
					if(tplace < 30) tplace = 30;
					if(tplace > g_draw_size*64 + g_draw_startpoint_x) tplace = g_draw_size*64;

					MemDC.LineTo(x-8,tplace);
				}

				CPen pShow111(PS_SOLID,1,RGB(255, 255,128));//   RGB(0,210,0)
				MemDC.SelectObject(&pShow111);			
				x = 39;
				MemDC.MoveTo(x * g_draw_size + g_draw_startpoint_y,50);
				MemDC.LineTo(x * g_draw_size + g_draw_startpoint_y,560);
				x = 42;
				MemDC.MoveTo(x * g_draw_size + g_draw_startpoint_y,50);
				MemDC.LineTo(x * g_draw_size + g_draw_startpoint_y,560);
				
			}; //m_iValueX_Y = Y			
			
			goto ExitRedraw;

		}; //end  Offset1 ,Offset2 都有效时
////-------------------------------------------
		
		pLOffsetvalue->GetWindowText(m_strEditOffset);
		if(!m_strEditOffset.IsEmpty())
		{
			Offset=atol(m_strEditOffset);
			g_2D_Loffset = Offset;
			
			if(m_iValueX_Y == 1) { // X				 
				if((Offset>=64)||(Offset<0)) {
					AfxMessageBox(" Offset 越限啦 !\n（在0－－63之间） ");
					delete pDoc;
					goto ExitRedraw;
				}
				int x = g_draw_startpoint_y; 
				for(int i=0;i<64;i++) {
					if(pDoc->temp == 0) pDoc->temp = 1;
					tplace = g_draw_size*64 + g_draw_startpoint_x - (int)(g_draw_size*64*pDoc->BuffDot3w[nBlindex][i][Offset]/pDoc->temp);
					if(tplace < 30) tplace = 25;
					if(tplace > g_draw_size*64 + g_draw_startpoint_x) tplace = g_draw_size*64;
					CBrush bShow;				
					bShow.CreateSolidBrush(RGB(
								   GetBValue(ColorSet(pDoc->BuffDot3w[nBlindex][i][Offset],pDoc->temp)), 
								   GetGValue(ColorSet(pDoc->BuffDot3w[nBlindex][i][Offset],pDoc->temp)),
								   GetRValue(ColorSet(pDoc->BuffDot3w[nBlindex][i][Offset],pDoc->temp))));			 					  
					CBrush* pOldBrush=MemDC.SelectObject(&bShow);// 
					MemDC.Rectangle(x-g_draw_frame,tplace-g_draw_frame,x+g_draw_frame,tplace+g_draw_frame);// 
						
					MemDC.SelectObject(pOldBrush);//++++
					bShow.DeleteObject();
					x += g_draw_size;
				}
				
//				 //画二维 直线
				CPen pShow(PS_SOLID,2,RGB(85, 15,185));//RGB(0,210,0)
				CPen* pOldPen=NULL;
				pOldPen=MemDC.SelectObject(&pShow);
				x = g_draw_startpoint_y+8 ; 
				tplace = g_draw_startpoint_x + 64*g_draw_size- (int)(g_draw_size*64*pDoc->BuffDot3w[nBlindex][0][Offset]/(pDoc->temp));
				if(tplace < 25) tplace = 30;
				if(tplace > g_draw_size*64 + g_draw_startpoint_x) tplace = g_draw_size*64;

				for(int kk=1;kk<64;kk++) {																				
					MemDC.MoveTo(x-8,tplace);
					x += g_draw_size;
					tplace = g_draw_size*64 + g_draw_startpoint_x - (int)(g_draw_size*64*pDoc->BuffDot3w[nBlindex][kk][Offset]/(pDoc->temp));
					if(pDoc->temp < 1) pDoc->temp = 1;
					if(tplace < 30) tplace = 25;
					if(tplace > g_draw_size*64 + g_draw_startpoint_x) tplace = g_draw_size*64;

					MemDC.LineTo(x-8,tplace);
				}

			} //m_iValueX_Y = X
			else if(m_iValueX_Y == 2) { //m_iValueX_Y = Y				
				if((Offset>=64)||(Offset<0)) {
					AfxMessageBox(" Offset 越限啦 ! \n（在0－－63之间）   ");
					delete pDoc;
					goto ExitRedraw;
				}
				int x = g_draw_startpoint_y; 
				for(int i=0;i<64;i++) {
					if(pDoc->temp == 0) pDoc->temp = 1;
					tplace = g_draw_size*64 + g_draw_startpoint_x - (int)(g_draw_size*64*(pDoc->BuffDot3w[nBlindex][Offset][i])/pDoc->temp);
					if(tplace < 30) tplace = 30;
					if(tplace > g_draw_size*64 + g_draw_startpoint_x) tplace = g_draw_size*64;
					CBrush bShow;				
					bShow.CreateSolidBrush(RGB(
								   GetBValue(ColorSet(pDoc->BuffDot3w[nBlindex][Offset][i],pDoc->temp)), 
								   GetGValue(ColorSet(pDoc->BuffDot3w[nBlindex][Offset][i],pDoc->temp)),
								   GetRValue(ColorSet(pDoc->BuffDot3w[nBlindex][Offset][i],pDoc->temp))));			 					  
					CBrush* pOldBrush=MemDC.SelectObject(&bShow);// 
					MemDC.Rectangle(x-g_draw_frame,tplace-g_draw_frame,x+g_draw_frame,tplace+g_draw_frame);// 
						
					MemDC.SelectObject(pOldBrush);//++++
					bShow.DeleteObject();
					x += g_draw_size;
				}
				
//				 //画二维 直线
				CPen pShow(PS_SOLID,2,RGB(85, 15,185));//RGB(0,210,0)
				CPen* pOldPen=NULL;
				pOldPen=MemDC.SelectObject(&pShow);
				x = g_draw_startpoint_y+8 ; 
				tplace = g_draw_startpoint_x + 64*g_draw_size- (int)(g_draw_size*64*pDoc->BuffDot3w[nBlindex][Offset][0]/(pDoc->temp));
				if(tplace < 30) tplace = 30;
				if(tplace > g_draw_size*64 + g_draw_startpoint_x) tplace = g_draw_size*64;
				for(int kk=1;kk<64;kk++) {																				
					MemDC.MoveTo(x-8,tplace);
					x += g_draw_size;
					tplace = g_draw_size*64 + g_draw_startpoint_x - (int)(g_draw_size*64*pDoc->BuffDot3w[nBlindex][Offset][kk]/(pDoc->temp));
					if(pDoc->temp < 1) pDoc->temp = 1;
					if(tplace < 25) tplace = 25;
					if(tplace > g_draw_size*64 + g_draw_startpoint_x) tplace = g_draw_size*64;

					MemDC.LineTo(x-8,tplace);
				}
			}; //m_iValueX_Y = Y
		}

	} // ---『show 2d 』---//	
	else{ //show bank

		COLORREF rf=RGB(26,131,162);//RGB(15,155,85); 
		CRect rcClient;
		GetClientRect(&rcClient);
		pDC->FillSolidRect(&rcClient,rf);
	}

ExitRedraw:
	if (m_bEnLimitLineRedraw) {
		m_bEnLimitLineRedraw = false;		
		RedrawLimitFile_To_MemDC(&MemDC);			
	}

	if (m_bEnLimitStat_MemDCRedraw) {
		m_bEnLimitStat_MemDCRedraw = false;
		OnLimitStat_MemDCRedraw(&MemDC);
	}

	if (m_bEnPeakValeStat_MemDCRedraw) {
		m_bEnPeakValeStat_MemDCRedraw = false;
		OnPeakValeStat_MemDCRedraw(&MemDC);
	}

	pDC->BitBlt(0,0,clientrect.Width(),clientrect.Height(),&MemDC,0,0,SRCCOPY);	

	MemBitmap.DeleteObject();
	MemDC.DeleteDC();
	ReleaseDC(pDC);

}


long CMaintView::ColorSet(long nCol, long nu)
{
		long int colorNumber[64]={
		 0x00000000,0x000c0000,0x00180000,0x00240000,
	     0x00300000,0x003c0000,0x00480000,0x00540000,
		 0x00600000,0x006c0000,0x00780000,0x00840000,
		 0x00900000,0x009c0000,0x00a80000,0x00b40000,
		 0x00c00000,0x00cc0000,0x00d80000,0x00e40000,
		 0x00f00000,0x00fc0000,0x00ff0000,0x00ff0c00,
		 0x00ff1800,0x00ff2400,0x00ff3000,0x00ff3c00,
		 0x00ff4800,0x00ff5400,0x00ff6000,0x00ff6c00,
		 0x00ff7800,0x00ff8400,0x00ff9000,0x00ff9c00,
		 0x00ffa800,0x00ffb400,0x00ffc000,0x00ffcc00,
		 0x00ffd800,0x00ffe400,0x00fff000,0x00fffc00,
		 0x00ffff00,0x00ffff0c,0x00ffff18,0x00ffff24,
		 0x00ffff30,0x00ffff3c,0x00ffff48,0x00ffff54,
		 0x00ffff60,0x00ffff6c,0x00ffff78,0x00ffff84,
		 0x00ffffc0,0x00ffffcc,0x00ffffd8,0x00ffffe4,
		 0x00fffff0,0x00fffff6,0x00fffffc,0x00ffffff}; 
/*		 0x00000000,0x000000a0,0x000000b0,0x000000c0,
		 0x000000d0,0x000000e0,0x000000f0,0x000000ff,
		 0x000010ff,0x000020ff,0x000030ff,0x000040ff,
		 0x000050ff,0x000060ff,0x000070ff,0x000080ff,
		 0x000090ff,0x0000a0ff,0x0000b0ff,0x0000c0ff,
		 0x0000d0ff,0x0000e0ff,0x0000f0ff,0x0000ffff,
		 0x0010ffff,0x0020fff0,0x0030ffe0,0x0040ffd0,
		 0x0050ffc0,0x0060ffb0,0x0070ffa0,0x0080ff90,
		 0x0090ff80,0x00a0ff70,0x00b0ff60,0x00c0ff50,
		 0x00d0ff40,0x00e0ff30,0x00f0ff20,0x00ffff10,
		 0x00ffff00,0x00fff000,0x00ffe000,0x00ffd000,
		 0x00ffc000,0x00ffb000,0x00ffa000,0x00ff9000,
		 0x00ff8000,0x00ff7000,0x00ff6000,0x00ff5000,
		 0x00ff4000,0x00ff3000,0x00ff2000,0x00ff1000,
		 0x00ff0000,0x00ff2424,0x00ff4949,0x00ff6d6d,
		 0x00ff9292,0x00ffb6b6,0x00ffdbdb,0x00ffffff */

/*	CInterfaceDoc* pDoc = GetDocument();
//	ASSERT_VALID(pDoc);*/

//	long span=(nu+1)/64;
//	if (nCol >= nu) {
//		return colorNumber[63];
//	}
//
//    if( span == 0 )  return colorNumber[0]; 
//	else return colorNumber[nCol/span];

	if (nu<64) { nu = 64; };
	if (nCol >= nu) { return colorNumber[63]; }
	else return colorNumber[64*nCol/nu];

	/* (nCol>=0&&nCol<=span)
		    return colorNumber[0];
		else if( nCol>span&&nCol<=2*span)
            return colorNumber[1];
		else if(nCol>2*span&&nCol<=3*span)
			return colorNumber[2];
		else if(nCol>3*span&&nCol<=4*span)
			return colorNumber[3];
		else if(nCol>4*span&&nCol<=5*span)
			return colorNumber[4];
		else if(nCol>5*span&&nCol<=6*span)
			return colorNumber[5];
		else if(nCol>6*span&&nCol<=7*span)
			return colorNumber[6];
		else if(nCol>7*span&&nCol<=8*span)
			return colorNumber[7];
		else if(nCol>8*span&&nCol<=9*span)
			return colorNumber[8];
		else if(nCol>9*span&&nCol<=10*span)
			return colorNumber[9];
		else if(nCol>10*span&&nCol<=11*span)
			return colorNumber[10];
		else if(nCol>11*span&&nCol<=12*span)
			return colorNumber[11];
		else if(nCol>12*span&&nCol<=13*span)
			return colorNumber[12];
		else if(nCol>13*span&&nCol<=14*span)
			return colorNumber[13];
		else if(nCol>14*span&&nCol<=15*span)
			return colorNumber[14];
		else if(nCol>15*span&&nCol<=16*span)
			return colorNumber[15];
		else if(nCol>16*span&&nCol<=17*span)
			return colorNumber[16];
		else if(nCol>17*span&&nCol<=18*span)
			return colorNumber[17];
		else if(nCol>18*span&&nCol<=19*span)
			return colorNumber[18];
		else if(nCol>19*span&&nCol<=20*span)
			return colorNumber[19];
		else if(nCol>20*span&&nCol<=21*span)
			return colorNumber[20];
		else if(nCol>21*span&&nCol<=22*span)
			return colorNumber[21];
		else if(nCol>22*span&&nCol<=23*span)
			return colorNumber[22];
		else if(nCol>23*span&&nCol<=24*span)
			return colorNumber[23];
		else if(nCol>24*span&&nCol<=25*span)
			return colorNumber[24];
		else if(nCol>25*span&&nCol<=26*span)
			return colorNumber[25];
		else if(nCol>26*span&&nCol<=27*span)
			return colorNumber[26];
		else if(nCol>27*span&&nCol<=28*span)
			return colorNumber[27];
		else if(nCol>28*span&&nCol<=29*span)
			return colorNumber[28];
		else if(nCol>29*span&&nCol<=30*span)
			return colorNumber[29];
		else if(nCol>30*span&&nCol<=31*span)
			return colorNumber[30];
		else if(nCol>31*span&&nCol<=32*span)
			return colorNumber[31];
		else if(nCol>32*span&&nCol<=33*span)
			return colorNumber[32];
		else if(nCol>33*span&&nCol<=34*span)
			return colorNumber[33];
		else if(nCol>34*span&&nCol<=35*span)
			return colorNumber[34];
		else if(nCol>35*span&&nCol<=36*span)
			return colorNumber[35];
		else if(nCol>36*span&&nCol<=37*span)
			return colorNumber[36];
		else if(nCol>37*span&&nCol<=38*span)
			return colorNumber[37];
		else if(nCol>38*span&&nCol<=39*span)
			return colorNumber[38];
		else if(nCol>39*span&&nCol<=40*span)
			return colorNumber[39];
		else if(nCol>40*span&&nCol<=41*span)
			return colorNumber[40];
		else if(nCol>41*span&&nCol<=42*span)
			return colorNumber[41];
		else if(nCol>42*span&&nCol<=43*span)
			return colorNumber[42];
		else if(nCol>43*span&&nCol<=44*span)
			return colorNumber[43];
		else if(nCol>44*span&&nCol<=45*span)
			return colorNumber[44];
		else if(nCol>45*span&&nCol<=46*span)
			return colorNumber[45];
		else if(nCol>46*span&&nCol<=47*span)
			return colorNumber[46];
		else if(nCol>47*span&&nCol<=48*span)
			return colorNumber[47];
		else if(nCol>48*span&&nCol<=49*span)
			return colorNumber[48];
		else if(nCol>49*span&&nCol<=50*span)
			return colorNumber[49];
		else if(nCol>50*span&&nCol<=51*span)
			return colorNumber[50];
		else if(nCol>51*span&&nCol<=52*span)
			return colorNumber[51];
		else if(nCol>52*span&&nCol<=53*span)
			return colorNumber[52];
		else if(nCol>53*span&&nCol<=54*span)
			return colorNumber[53];
		else if(nCol>54*span&&nCol<=55*span)
			return colorNumber[54];
		else if(nCol>55*span&&nCol<=56*span)
			return colorNumber[55];
		else if(nCol>56*span&&nCol<=57*span)
			return colorNumber[56];
		else if(nCol>57*span&&nCol<=58*span)
			return colorNumber[57];
		else if(nCol>58*span&&nCol<=59*span)
			return colorNumber[58];
		else if(nCol>59*span&&nCol<=60*span)
			return colorNumber[59];
		else if(nCol>60*span&&nCol<=61*span)
			return colorNumber[60];
		else if(nCol>61*span&&nCol<=62*span)
			return colorNumber[61];
		else if(nCol>62*span&&nCol<=63*span)
			return colorNumber[62];
		else //if(nCol>63*span&&nCol<=64*span)
			return colorNumber[63];
//		else //if(nCol[]>64*span&&nCol[]<=span)
//			return colorNumber[64];
*/
}

void CMaintView::OnShowbar3dRadio() 
{
	m_iValue3d_2d = 3;

	m_ShowModelSel = 0;
	OnRedraw();
	
}

void CMaintView::OnShowbar2dRadio() 
{
	m_ShowModelSel = 0;
	m_iValue3d_2d = 2;
	OnRedraw();
}

void CMaintView::On2dShowX() 
{
	m_ShowModelSel = 0;

	m_iValueX_Y = 1;
	OnRedraw();
}

void CMaintView::On2dShowY() 
{
	m_ShowModelSel = 0;

	m_iValueX_Y = 2;
	OnRedraw();
	
}

void CMaintView::OnMouseMove(UINT nFlags, CPoint point) 
{
	CView::OnMouseMove(nFlags, point);
	CMaintDoc* pDoc = GetDocument();
	CStatusBar* pStatus=(CStatusBar*)
		AfxGetApp()->m_pMainWnd->GetDescendantWindow(ID_VIEW_STATUS_BAR);

	CMainFrame* pMainFrame = (CMainFrame*)GetParentFrame();
	CEdit* pCurvalue_Limt=(CEdit*)(pMainFrame->m_wndShowBar.GetDlgItem(IDC_CURVALUE_LIMIT));

	if(m_bMoving64EnergLimitEn) { // 能谱操作  进行64显示模式下的 能谱边界调整 
		int barsize_x = 100;
		int barsize_y = 80;
		int barstart_x = 50;
		int barstart_y = 75;
		char chrtext[200];

		CDC* pDC=GetDC();
		CPen pShow(PS_SOLID,1,RGB(255,10,,0));//
		CPen* pOldPen=NULL;
		pOldPen=pDC->SelectObject(&pShow); 
		pDC->SetROP2(R2_NOT);		
		
		if ((point.x >= ( barsize_x * (m_iEnergeSpectCurSynNo % 8) + barstart_x)) &&
			(point.x <= (barsize_x * (m_iEnergeSpectCurSynNo % 8) + barstart_x + 63)) ){
				
			if ((point.y < (barsize_y * (m_iEnergeSpectCurSynNo / 8) + barstart_y)) && (point.y > (barsize_y * (m_iEnergeSpectCurSynNo / 8) + barstart_y - 40)) ) {
				pDC->MoveTo(m_Oldpoint.x, barsize_y* (m_iEnergeSpectCurSynNo / 8) + barstart_y);
				pDC->LineTo(m_Oldpoint.x, barsize_y* (m_iEnergeSpectCurSynNo / 8) - 40 + barstart_y);
		
				pDC->MoveTo(point.x, barsize_y* (m_iEnergeSpectCurSynNo / 8) + barstart_y);
				pDC->LineTo(point.x, barsize_y* (m_iEnergeSpectCurSynNo / 8) - 40 + barstart_y);
					
				m_bMoving64EnergLimitEn = true;
				m_Oldpoint = point;

				if(m_bEnergeSpectMove_Eth_Low) {
					sprintf(chrtext,"正在操作：%d号 Eth 低：%d",m_iEnergeSpectCurSynNo,(point.x- barsize_x * (m_iEnergeSpectCurSynNo % 8) - barstart_x));					
				//	m_iEnergeSpectEth_Low[m_iEnergeSpectCurBlockNo][m_iEnergeSpectCurSynNo] = point.x- barsize_x * (m_iEnergeSpectCurSynNo % 8) - barstart_x;
					pStatus->SetPaneText(2,chrtext,1);
				}
				else if(m_bEnergeSpectMove_Eth_Hight) {
					sprintf(chrtext,"正在操作：%d号 Eth 高 ：%d",m_iEnergeSpectCurSynNo,(point.x- barsize_x * (m_iEnergeSpectCurSynNo % 8) - barstart_x));
				//	m_iEnergeSpectEth_Hight[m_iEnergeSpectCurBlockNo][m_iEnergeSpectCurSynNo] = point.x- barsize_x * (m_iEnergeSpectCurSynNo % 8) - barstart_x;	
					pStatus->SetPaneText(2,chrtext,1);
				}
			}		
		}				

		ReleaseDC(pDC);
		return;
	}

	if(m_bEnergeSpectZoomInMode) {
			CDC* pDC=GetDC(); //画二维 直线
			CPen pShow2(PS_SOLID,2,RGB(255,10,,0));//RGB(0,210,0)
			CPen* pOldPen=NULL;
			pOldPen=pDC->SelectObject(&pShow2);

			pDC->SetROP2(R2_NOT);
			CString strtext;

			//----------【平移能谱上下边界】--------//
			if(((point.x <g_draw_startpoint_y-1)||(point.x> g_draw_startpoint_y +g_draw_size*63+1))&&((m_bEnergeSpectMove_Eth_Low)||(m_bEnergeSpectMove_Eth_Hight)))  {
					ReleaseDC(pDC);
					return;
			}
			if((m_bEnergeSpectMove_Eth_Low)||(m_bEnergeSpectMove_Eth_Hight)) {
				pDC->MoveTo(m_Oldpoint.x, 60);
				pDC->LineTo(m_Oldpoint.x, 50+8*64);
				pDC->MoveTo(point.x, 60);
				pDC->LineTo(point.x, 50+8*64);
				m_Oldpoint = point;
				if(m_bEnergeSpectMove_Eth_Low) {
					strtext.Format("正在操作：Eth 低：%d",(point.x-90)/8);

					m_iEnergeSpectEth_Low[m_iEnergeSpectCurBlockNo][m_iEnergeSpectCurSynNo] = (point.x-90)/8;
					pStatus->SetPaneText(2,strtext,1);
				}
				else if(m_bEnergeSpectMove_Eth_Hight) {
					strtext.Format("正在操作：Eth 高 ：%d",(point.x-90)/8);
					m_iEnergeSpectEth_Hight[m_iEnergeSpectCurBlockNo][m_iEnergeSpectCurSynNo] = (point.x-90)/8;	
					pStatus->SetPaneText(2,strtext,1);
				}
			}
			ReleaseDC(pDC);
		return;
	}


	CString strtext;
	int showvalue = 0;
	int point_x = 0,point_y = 0;

	point_x = point.x;
	point_y = point.y;

	point_x -= g_draw_startpoint_y - g_draw_frame;
	point_y -= g_draw_startpoint_x - g_draw_frame;

	char tbuferr[100];
	if((point_x<0)||(point_y<0)) {		
		sprintf(tbuferr,"鼠标值：(%3d,%3d) 图象值 无效区",point.x,point.y);
		pStatus->SetPaneText(0,tbuferr,1);
		return;
	}
	if((point_x >= g_draw_size*64)||(point_y >= g_draw_size*64)) {
		sprintf(tbuferr,"鼠标值：(%3d,%3d) 图象值 无效区",point.x,point.y);
		pStatus->SetPaneText(0,tbuferr,1);
		return;
	}


	point_x = (int)(point_x / g_draw_size);//g_draw_size
	point_y = (int)(point_y / g_draw_size);//g_draw_size

	if(!g_bshow_status_3D) {
		
		int temp = g_Blockindex;
		int temp2 = g_2D_Loffset;

		if(m_iValueX_Y == 1) { // X 
			showvalue = pDoc->BuffDot3w[g_Blockindex][point_x][g_2D_Loffset];  
		}
		else { //y
			showvalue = pDoc->BuffDot3w[g_Blockindex][g_2D_Loffset][point_x];  
		}		
		
		char tbuferr5[100];
		sprintf(tbuferr5," 指示图象值: %5d   图象坐标:( %3d,  %3d)",showvalue,point_x,point_y);		
		pStatus->SetPaneText(0,tbuferr5,1);

		return;
	}	

	if(g_Blockindex != 0xffff) { 
		showvalue = pDoc->BuffDot3w[g_Blockindex][point_y][point_x];  
		char tbuf[100];
		if(pStatus)	{		
			sprintf(tbuf,"      图象值: %5d    (图象座标 %3d, %3d )",showvalue,point_x,point_y);
			pStatus->SetPaneText(0,tbuf,1);//在状态条的第一个窗格中输出当前鼠标的位置   
		}	 
	}	
		

	//---------【散点的位置边界平移】------//	

	CDC* pDC=GetDC(); //画二维 直线
	CPen pShow2(PS_SOLID,2,RGB(255,10,,0));//RGB(0,210,0)
	CPen* pOldPen=NULL;
	pOldPen=pDC->SelectObject(&pShow2);

	pDC->SetROP2(R2_NOT);

	int idraw_startpoint_y = g_draw_startpoint_y - 4;
	int idraw_startpoint_x = g_draw_startpoint_x - 4;


	

	if(m_bSameDriction_X) { //水平方向一致
		if(m_bButtonDown) {
			if(m_bEnMoveBreakLine) {  //画打断的水平长线
				if((m_sdotOldpoint.y < idraw_startpoint_x + m_Position_Y0[m_iMoveCurLine_Y-1][m_iMoveCurLine_X]*8 +5)   //akang 2003.04.25 -1 
					||(m_sdotOldpoint.y > idraw_startpoint_x + m_Position_Y1[m_iMoveCurLine_Y][m_iMoveCurLine_X]*8 -5))  { //akang 2003.04.25 -1 
					
					m_bthisMoveInValid = true;
					m_sdotOldpoint = point;
					ReleaseDC(pDC);

					RedrawLimitFile();
					return;
				}
				::SetCursor(::LoadCursor(NULL,IDC_SIZENS));// IDC_SIZEWE  IDC_SIZENS
				if(m_iMoveCurLine_X <7 ) {
					pDC->MoveTo(idraw_startpoint_y + m_Position_X0[m_iMoveCurLine_Y][m_iMoveCurLine_X]*8,m_sdotOldpoint.y);
					pDC->LineTo(idraw_startpoint_y + (m_Position_X1[m_iMoveCurLine_Y][m_iMoveCurLine_X]+1)*8,m_sdotOldpoint.y);
					pDC->MoveTo(idraw_startpoint_y + m_Position_X0[m_iMoveCurLine_Y][m_iMoveCurLine_X]*8,point.y);
					pDC->LineTo(idraw_startpoint_y + (m_Position_X1[m_iMoveCurLine_Y][m_iMoveCurLine_X]+1)*8,point.y);					
				}
				else { //akang915
					pDC->MoveTo(idraw_startpoint_y + m_Position_X0[m_iMoveCurLine_Y][m_iMoveCurLine_X]*8 ,m_sdotOldpoint.y); 
					pDC->LineTo(idraw_startpoint_y + (m_Position_X1[m_iMoveCurLine_Y][m_iMoveCurLine_X])*8 + 8,m_sdotOldpoint.y);
					pDC->MoveTo(idraw_startpoint_y + m_Position_X0[m_iMoveCurLine_Y][m_iMoveCurLine_X]*8 ,point.y);
					pDC->LineTo(idraw_startpoint_y + (m_Position_X1[m_iMoveCurLine_Y][m_iMoveCurLine_X])*8 + 8,point.y);

				}

				m_sdotOldpoint = point;
				ReleaseDC(pDC);
				return;
			}

			if(m_bMoveLongLine) {//画长线
				if((m_sdotOldpoint.y < idraw_startpoint_x + m_Position_Y0[m_Long_Moving_Tab -1][1]*8 +5)   //akang 2003.04.25 -1 
					||(m_sdotOldpoint.y > idraw_startpoint_x + m_Position_Y1[m_Long_Moving_Tab ][1]*8 -5))  { //akang 2003.04.25 -1 
					m_bthisMoveInValid = true;
					m_sdotOldpoint = point;
					ReleaseDC(pDC);
					RedrawLimitFile();
					return;
				}
				::SetCursor(::LoadCursor(NULL,IDC_SIZENS));// IDC_SIZEWE  IDC_SIZENS
				pDC->MoveTo(idraw_startpoint_y + m_Position_X0[m_Long_Moving_Tab][1]*8,m_sdotOldpoint.y);
				pDC->LineTo(idraw_startpoint_y + m_Position_X0[m_Long_Moving_Tab][7]*8,m_sdotOldpoint.y);
				pDC->MoveTo(idraw_startpoint_y + m_Position_X0[m_Long_Moving_Tab][1]*8,point.y);
				pDC->LineTo(idraw_startpoint_y + m_Position_X0[m_Long_Moving_Tab][7]*8,point.y);					
									
				m_sdotOldpoint = point;
				ReleaseDC(pDC);
				return;
			}
			else { //画水平短线
				if(m_bShort_Line_Valid) { //水平方向的垂直短线有效
					if((m_sdotOldpoint.x < idraw_startpoint_y + m_Position_X0[m_Moving_Tab_Ver][m_Moving_Tab_Hor -1]*8 +5)  //akang 2003.04.25 -1 
						||(m_sdotOldpoint.x > idraw_startpoint_y + m_Position_X1[m_Moving_Tab_Ver][m_Moving_Tab_Hor]*8 -5))  { //判断越限 //akang 2003.04.25 -1 
						m_sdotOldpoint = point;
						m_bthisMoveInValid = true;
						ReleaseDC(pDC);
						RedrawLimitFile();
						return;
					}
					::SetCursor(::LoadCursor(NULL,IDC_SIZEWE));// IDC_SIZEWE  IDC_SIZENS
					
					if(m_Moving_Tab_Ver == 7) {
						pDC->MoveTo(m_sdotOldpoint.x,idraw_startpoint_x + m_Position_Y0[m_Moving_Tab_Ver][m_Moving_Tab_Hor]*8);//akang915
						pDC->LineTo(m_sdotOldpoint.x,idraw_startpoint_x + m_Position_Y1[m_Moving_Tab_Ver][m_Moving_Tab_Hor]*8 + 8); //akang915
					}					
					else {
						pDC->MoveTo(m_sdotOldpoint.x,idraw_startpoint_x + m_Position_Y0[m_Moving_Tab_Ver][m_Moving_Tab_Hor]*8 );
						pDC->LineTo(m_sdotOldpoint.x,idraw_startpoint_x + (m_Position_Y1[m_Moving_Tab_Ver][m_Moving_Tab_Hor]+1)*8);
					}
										
					if(m_Moving_Tab_Ver == 7) {
						pDC->MoveTo(point.x,idraw_startpoint_x + m_Position_Y0[m_Moving_Tab_Ver][m_Moving_Tab_Hor]*8 );
						pDC->LineTo(point.x,idraw_startpoint_x + m_Position_Y1[m_Moving_Tab_Ver][m_Moving_Tab_Hor]*8 + 8);
					}
					else {
						pDC->MoveTo(point.x,idraw_startpoint_x + m_Position_Y0[m_Moving_Tab_Ver][m_Moving_Tab_Hor]*8);
						pDC->LineTo(point.x,idraw_startpoint_x + (m_Position_Y1[m_Moving_Tab_Ver][m_Moving_Tab_Hor]+1)*8);
					}
					m_sdotOldpoint = point;
					ReleaseDC(pDC);
					return;
				}
				if(m_Moving_Tab_Hor == 0) {// 水平前短线
					if((m_sdotOldpoint.y < idraw_startpoint_x + m_Position_Y0[m_Moving_Tab_Ver -1][0]*8 +5) //akang 2003.04.25 +4 
						||(m_sdotOldpoint.y > idraw_startpoint_x + m_Position_Y1[m_Moving_Tab_Ver][0]*8 -5))  {//判断越限 //akang 2003.04.25 -1 
						m_sdotOldpoint = point;
						m_bthisMoveInValid = true;
						ReleaseDC(pDC);
						RedrawLimitFile();
						return;
					}
					::SetCursor(::LoadCursor(NULL,IDC_SIZENS));// IDC_SIZEWE  IDC_SIZENS
					pDC->MoveTo(idraw_startpoint_y + m_Position_X0[m_Moving_Tab_Ver][0]*8,m_sdotOldpoint.y);
					pDC->LineTo(idraw_startpoint_y + (m_Position_X1[m_Moving_Tab_Ver][0]+1)*8,m_sdotOldpoint.y);
					pDC->MoveTo(idraw_startpoint_y + m_Position_X0[m_Moving_Tab_Ver][0]*8,point.y);
					pDC->LineTo(idraw_startpoint_y + (m_Position_X1[m_Moving_Tab_Ver][0]+1)*8,point.y);					
										
					m_sdotOldpoint = point;
					ReleaseDC(pDC);
					return;
				}
				else {//水平后短线
					if((m_sdotOldpoint.y < idraw_startpoint_x + m_Position_Y0[m_Moving_Tab_Ver -1][7]*8 +5)  //akang 2003.04.25 +4 
						||(m_sdotOldpoint.y > idraw_startpoint_x + m_Position_Y1[m_Moving_Tab_Ver][7]*8 -5))  { //判断越限 //akang 2003.04.25 -1 
						m_sdotOldpoint = point;
						m_bthisMoveInValid = true;
						ReleaseDC(pDC);
						RedrawLimitFile();
						return;
					}
					::SetCursor(::LoadCursor(NULL,IDC_SIZENS));// IDC_SIZEWE  IDC_SIZENS
					pDC->MoveTo(idraw_startpoint_y + m_Position_X0[m_Moving_Tab_Ver][7]*8,m_sdotOldpoint.y);
					pDC->LineTo(idraw_startpoint_y + m_Position_X1[m_Moving_Tab_Ver][7]*8 + 8,m_sdotOldpoint.y);//akang915
					pDC->MoveTo(idraw_startpoint_y + m_Position_X0[m_Moving_Tab_Ver][7]*8,point.y);
					pDC->LineTo(idraw_startpoint_y + m_Position_X1[m_Moving_Tab_Ver][7]*8 + 8,point.y);	//akang915				
										
					m_sdotOldpoint = point;
					ReleaseDC(pDC);
					return;
				}
			}

		}

	}
	else { //垂直方向
		if(m_bButtonDown) {
			if(m_bEnMoveBreakLine) {  //画打断的水平长线
				if((m_sdotOldpoint.x < idraw_startpoint_y + m_Position_X0[m_iMoveCurLine_Y][m_iMoveCurLine_X-1]*8 +5)
					||(m_sdotOldpoint.x > idraw_startpoint_y + m_Position_X1[m_iMoveCurLine_Y][m_iMoveCurLine_X]*8 -5))  {  //akang 2003.04.25 -1 
										
					m_bthisMoveInValid = true;
					m_sdotOldpoint = point;
					ReleaseDC(pDC);
					RedrawLimitFile();
					return;
				}
			    ::SetCursor(::LoadCursor(NULL,IDC_SIZEWE));// IDC_SIZEWE  IDC_SIZENS
				if(m_iMoveCurLine_Y <7) {
					pDC->MoveTo(m_sdotOldpoint.x,idraw_startpoint_x + m_Position_Y0[m_iMoveCurLine_Y][m_iMoveCurLine_X]*8);
					pDC->LineTo(m_sdotOldpoint.x,idraw_startpoint_x + (m_Position_Y1[m_iMoveCurLine_Y][m_iMoveCurLine_X]+1)*8);
					pDC->MoveTo(point.x,idraw_startpoint_x + m_Position_Y0[m_iMoveCurLine_Y][m_iMoveCurLine_X]*8);
					pDC->LineTo(point.x,idraw_startpoint_x + (m_Position_Y1[m_iMoveCurLine_Y][m_iMoveCurLine_X]+1)*8);					
				}
				else {
					pDC->MoveTo(m_sdotOldpoint.x,idraw_startpoint_x + m_Position_Y0[m_iMoveCurLine_Y][m_iMoveCurLine_X]*8);
					pDC->LineTo(m_sdotOldpoint.x,idraw_startpoint_x + (m_Position_Y1[m_iMoveCurLine_Y][m_iMoveCurLine_X])*8 + 8);
					pDC->MoveTo(point.x,idraw_startpoint_x + m_Position_Y0[m_iMoveCurLine_Y][m_iMoveCurLine_X]*8 );
					pDC->LineTo(point.x,idraw_startpoint_x + (m_Position_Y1[m_iMoveCurLine_Y][m_iMoveCurLine_X])*8 + 8);	
				}

				m_sdotOldpoint = point;
				ReleaseDC(pDC);
				return;
			}

			if(m_bMoveLongLine) {//画长线 
				if((m_sdotOldpoint.x < idraw_startpoint_y + m_Position_X0[1][m_Long_Moving_Tab -1]*8 +5)  //akang 2003.04.25 +4 
					||(m_sdotOldpoint.x > idraw_startpoint_y + m_Position_X1[1][m_Long_Moving_Tab]*8 -5))  { //判断越限   //akang 2003.04.25 -1 
					m_bthisMoveInValid = true;
					m_sdotOldpoint = point;
					ReleaseDC(pDC);
					RedrawLimitFile();
					return;
				}
				::SetCursor(::LoadCursor(NULL,IDC_SIZEWE));// IDC_SIZEWE  IDC_SIZENS
				pDC->MoveTo(m_sdotOldpoint.x,idraw_startpoint_x + m_Position_Y0[1][m_Long_Moving_Tab]*8);
				pDC->LineTo(m_sdotOldpoint.x,idraw_startpoint_x + m_Position_Y0[7][m_Long_Moving_Tab]*8);
				pDC->MoveTo(point.x,idraw_startpoint_x + m_Position_Y0[1][m_Long_Moving_Tab]*8);
				pDC->LineTo(point.x,idraw_startpoint_x + m_Position_Y0[7][m_Long_Moving_Tab]*8);					
									
				m_sdotOldpoint = point;
				ReleaseDC(pDC);
				return;
			}
			else{// 画垂直短线
				if(m_bShort_Line_Valid) { //垂直方向的水平短线有效
					if((m_sdotOldpoint.y < idraw_startpoint_x + m_Position_Y0[m_Moving_Tab_Ver-1][m_Moving_Tab_Hor]*8 +5) //akang 2003.04.25 +4 
						||(m_sdotOldpoint.y > idraw_startpoint_x + m_Position_Y1[m_Moving_Tab_Ver][m_Moving_Tab_Hor]*8 -5))  { //判断越限 //akang 2003.04.25 -1 
						m_sdotOldpoint = point;
						m_bthisMoveInValid = true;
						ReleaseDC(pDC);
						RedrawLimitFile();
						return;
					}
					::SetCursor(::LoadCursor(NULL,IDC_SIZENS));// IDC_SIZEWE  IDC_SIZENS	
					
					if(m_Moving_Tab_Hor == 7) {
						pDC->MoveTo(idraw_startpoint_y + m_Position_X0[m_Moving_Tab_Ver][m_Moving_Tab_Hor]*8,m_sdotOldpoint.y);
						pDC->LineTo(idraw_startpoint_y + m_Position_X1[m_Moving_Tab_Ver][m_Moving_Tab_Hor]*8 +8,m_sdotOldpoint.y);
					}
					else {
						pDC->MoveTo(idraw_startpoint_y + m_Position_X0[m_Moving_Tab_Ver][m_Moving_Tab_Hor]*8,m_sdotOldpoint.y);
						pDC->LineTo(idraw_startpoint_y + (m_Position_X1[m_Moving_Tab_Ver][m_Moving_Tab_Hor]+1)*8,m_sdotOldpoint.y);
					}
					
					if(m_Moving_Tab_Hor == 7) {
						pDC->MoveTo(idraw_startpoint_y + m_Position_X0[m_Moving_Tab_Ver][m_Moving_Tab_Hor]*8,point.y);
						pDC->LineTo(idraw_startpoint_y + m_Position_X1[m_Moving_Tab_Ver][m_Moving_Tab_Hor]*8 +8,point.y);
					}
					else {
						pDC->MoveTo(idraw_startpoint_y + m_Position_X0[m_Moving_Tab_Ver][m_Moving_Tab_Hor]*8,point.y);
						pDC->LineTo(idraw_startpoint_y + (m_Position_X1[m_Moving_Tab_Ver][m_Moving_Tab_Hor]+1)*8,point.y);
					}
					m_sdotOldpoint = point;
					ReleaseDC(pDC);
					return;
				}


				if(m_Moving_Tab_Ver == 0) {// 垂直前短线
					if((m_sdotOldpoint.x < idraw_startpoint_y + m_Position_X0[0][m_Moving_Tab_Hor -1]*8 +5)   //akang 2003.04.25 +4 
						||(m_sdotOldpoint.x > idraw_startpoint_y + m_Position_X1[0][m_Moving_Tab_Hor]*8 -5))  { //判断越限  //akang 2003.04.25 -1 
						m_sdotOldpoint = point;
						m_bthisMoveInValid = true;						
						ReleaseDC(pDC);
						RedrawLimitFile();
						return;
					}
					::SetCursor(::LoadCursor(NULL,IDC_SIZEWE));// IDC_SIZEWE  IDC_SIZENS
					pDC->MoveTo(m_sdotOldpoint.x,idraw_startpoint_x + m_Position_Y0[0][m_Moving_Tab_Hor]*8);
					pDC->LineTo(m_sdotOldpoint.x,idraw_startpoint_x + (m_Position_Y1[0][m_Moving_Tab_Hor]+1)*8);
					pDC->MoveTo(point.x,idraw_startpoint_x + m_Position_Y0[0][m_Moving_Tab_Hor]*8);
					pDC->LineTo(point.x,idraw_startpoint_x + (m_Position_Y1[0][m_Moving_Tab_Hor]+1)*8);					
										
					m_sdotOldpoint = point;
					ReleaseDC(pDC);
					return;
				}
				else { //垂直后短线
					if((m_sdotOldpoint.x < idraw_startpoint_y + m_Position_X0[7][m_Moving_Tab_Hor -1]*8 +5)  ////akang 2003.04.25 +4 
						||(m_sdotOldpoint.x > idraw_startpoint_y + m_Position_X1[7][m_Moving_Tab_Hor]*8 -5))  { //判断越限  //akang 2003.04.25 -1 
						m_sdotOldpoint = point;
						m_bthisMoveInValid = true;
						ReleaseDC(pDC);
						RedrawLimitFile();
						return;
					}
					::SetCursor(::LoadCursor(NULL,IDC_SIZEWE));// IDC_SIZEWE  IDC_SIZENS
					pDC->MoveTo(m_sdotOldpoint.x,idraw_startpoint_x + m_Position_Y0[7][m_Moving_Tab_Hor]*8);
					pDC->LineTo(m_sdotOldpoint.x,idraw_startpoint_x + m_Position_Y1[7][m_Moving_Tab_Hor]*8 + 8);
					pDC->MoveTo(point.x,idraw_startpoint_x + m_Position_Y0[7][m_Moving_Tab_Hor]*8);
					pDC->LineTo(point.x,idraw_startpoint_x + m_Position_Y1[7][m_Moving_Tab_Hor]*8 + 8);					
										
					m_sdotOldpoint = point;
					ReleaseDC(pDC);
					return;
				}
			}
		}

	}

	ReleaseDC(pDC);


}

void CMaintView::OnLButtonDown(UINT nFlags, CPoint point) 
{
	CString strtext;
	CView::OnLButtonDown(nFlags, point);
	CMainFrame* pMainFrame=
	      (CMainFrame*)GetParentFrame();
	CEdit* pOldvalue_Limt=(CEdit*)
		(pMainFrame->m_wndShowBar.GetDlgItem(IDC_OLDVALUE_LIMIT));
	CEdit* pCurvalue_Limt=(CEdit*)
		(pMainFrame->m_wndShowBar.GetDlgItem(IDC_CURVALUE_LIMIT));
	CComboBox *pBlockindex = (CComboBox *)
		(pMainFrame)->m_wndShowBar.GetDlgItem(IDC_SHOW_BLOCK_SEL);

	m_Oldpoint = point;

	m_bMoving64EnergLimitEn = false;

	if((m_bEnergeSpectZoomInMode )&&(g_DataSample_Mod_Sel != 0xC7)) { //能谱操作方式 
			if((point.y < g_draw_size*64+g_draw_startpoint_x)&&(point.y>400)) {
				if((point.x >= (g_draw_startpoint_y +g_draw_size*m_iEnergeSpectEth_Low[m_iEnergeSpectCurBlockNo][m_iEnergeSpectCurSynNo] -10))
					&&(point.x <= (g_draw_startpoint_y +g_draw_size*m_iEnergeSpectEth_Low[m_iEnergeSpectCurBlockNo][m_iEnergeSpectCurSynNo] +10))) {
					m_bEnergeSpectMove_Eth_Low = true;
					m_bEnergeSpectMove_Eth_Hight = false;					
				}
				else if((point.x >= (g_draw_startpoint_y +g_draw_size*m_iEnergeSpectEth_Hight[m_iEnergeSpectCurBlockNo][m_iEnergeSpectCurSynNo] -10))
					&&(point.x <= (g_draw_startpoint_y +g_draw_size*m_iEnergeSpectEth_Hight[m_iEnergeSpectCurBlockNo][m_iEnergeSpectCurSynNo] +10))) {
					m_bEnergeSpectMove_Eth_Low = false;
					m_bEnergeSpectMove_Eth_Hight = true;
				}
			}
			char textout[50];
			sprintf(textout,"能谱放大显示: 操作块号 %d    晶体条号 %d",m_iEnergeSpectCurBlockNo,m_iEnergeSpectCurSynNo);
			pStatus->SetPaneText(0,textout,1);
			sprintf(textout,"状态显示： 能谱下阈值：%d    上阈值：%d",m_iEnergeSpectEth_Low[m_iEnergeSpectCurBlockNo][m_iEnergeSpectCurSynNo],
					m_iEnergeSpectEth_Hight[m_iEnergeSpectCurBlockNo][m_iEnergeSpectCurSynNo]);
			pStatus->SetPaneText(2,textout,1);

		return;
	}

	int nBlindex= pBlockindex->GetCurSel();
	if(nBlindex == -1) nBlindex = 0;

	if((m_bEnergeSpectSelEn)&&(g_DataSample_Mod_Sel != 0xC7) ) { // 能谱操作  进行64显示模式下的 能谱边界调整 
		int barsize_x = 100;
		int barsize_y = 80;
		int barstart_x = 50;
		int barstart_y = 75;

		CDC* pDC=GetDC();
		CPen pShow(PS_SOLID,1,RGB(255,10,,0));//
		CPen* pOldPen=NULL;
		pOldPen=pDC->SelectObject(&pShow); 
		pDC->SetROP2(R2_NOT);
		
		for(int numy=0;numy<8;numy++) {
			for(int numx=0;numx<8;numx++){ // 能谱 低阈判断
				if ((point.x >= (m_iEnergeSpectEth_Low[nBlindex][numy*8 + numx] + barsize_x * numx + barstart_x -4)) &&
					(point.x <= (m_iEnergeSpectEth_Low[nBlindex][numy*8 + numx] + barsize_x * numx + barstart_x +4)) ){
					
					if ((point.y < barsize_y * numy + barstart_y) && (point.y > barsize_y * numy + barstart_y - 40)) {
						pDC->MoveTo(m_iEnergeSpectEth_Low[nBlindex][numy*8 + numx] + barsize_x * numx + barstart_x, barsize_y* numy + barstart_y);
						pDC->LineTo(m_iEnergeSpectEth_Low[nBlindex][numy*8 + numx] + barsize_x * numx + barstart_x, barsize_y* numy - 40 + barstart_y);
					
						m_bMoving64EnergLimitEn = true;
						m_iEnergeSpectCurBlockNo = nBlindex;
						m_iEnergeSpectCurSynNo = numy * 8 + numx;
						m_bEnergeSpectMove_Eth_Low = true;
						m_bEnergeSpectMove_Eth_Hight = false;
					}		
				}
				else { // 能谱 高阈判断
					if ((point.x >= (m_iEnergeSpectEth_Hight[nBlindex][numy*8 + numx] + barsize_x * numx + barstart_x -4)) &&
						(point.x <= (m_iEnergeSpectEth_Hight[nBlindex][numy*8 + numx] + barsize_x * numx + barstart_x +4)) ){
						
						if ((point.y < barsize_y * numy + barstart_y) && (point.y > barsize_y * numy + barstart_y - 40)) {
							pDC->MoveTo(m_iEnergeSpectEth_Hight[nBlindex][numy*8 + numx] + barsize_x * numx + barstart_x, barsize_y* numy + barstart_y);
							pDC->LineTo(m_iEnergeSpectEth_Hight[nBlindex][numy*8 + numx] + barsize_x * numx + barstart_x, barsize_y* numy - 40 + barstart_y);
							
							m_bMoving64EnergLimitEn = true;
							m_iEnergeSpectCurBlockNo = nBlindex;
							m_iEnergeSpectCurSynNo = numy * 8 + numx;
							m_bEnergeSpectMove_Eth_Hight = true;
							m_bEnergeSpectMove_Eth_Low = false;
						}		
					}
				}
			}
		}		

		ReleaseDC(pDC);
		return;
	}

	//-------峰谷比操作-----//
	if((m_bEnShowPeakValeRatio) &&(g_DataSample_Mod_Sel == 0xC7)) { // 能谱操作 选择
		int selnum;
		bool flag = false;
		for(int i=0;i<8;i++) { //水平
			if((point.x > g_draw_startpoint_y +i*g_draw_size*8)&&(point.x < g_draw_startpoint_y +i*g_draw_size*8 + g_draw_size*8)) {
				for(int j=0;j<8;j++) {
					if((point.y > g_draw_startpoint_x +j*g_draw_size*8)&&(point.y < g_draw_startpoint_x +j*g_draw_size*8 + g_draw_size*8)) {
						selnum = i + j*8;
						flag = true;						
					}
				}
			}			
		}
		if(flag) {			
			OnShowPeakValeRatio(selnum);
			return;
		}	
		return;
		
	}


//---------【散点的位置边界平移】------//
//	SetCapture();	
	CRect rect;
	GetClientRect(&rect);
	ClientToScreen(&rect);

	CDC* pDC=GetDC();
	CPen pShow(PS_SOLID,2,RGB(15, 105,185));//RGB(0,210,0)
	CPen* pOldPen=NULL;
	pOldPen=pDC->SelectObject(&pShow); 
	pDC->SetROP2(R2_NOT);

	int idraw_startpoint_y = g_draw_startpoint_y - 4;
	int idraw_startpoint_x = g_draw_startpoint_x - 4;

	if(m_bSameDriction_X) { //水平方向一致
		if(!m_bMoveWholeLine) { //画水平打断的短线；
			for(int i=1;i<8;i++) {			
				for(int no=1;no<=7;no++) {  //
					if((point.x <= idraw_startpoint_y + m_Position_X1[i][no]*8)&&(point.x >= idraw_startpoint_y + m_Position_X0[i][no]*8)) {
						if((point.y <= idraw_startpoint_x + m_Position_Y0[i][no]*8 +3)&&(point.y >= idraw_startpoint_x + m_Position_Y0[i][no]*8 -3)) {
							if((m_Position_X1[i-1][no] == m_Position_X1[i][no])&&(m_Position_X0[i-1][no] == m_Position_X0[i][no])){  //判断前后是否平齐
								
									m_iMoveCurLine_X = no;
									m_iMoveCurLine_Y = i;	
									m_bMovingVerDirection = false;
									m_bEnMoveBreakLine = true;
									m_bButtonDown = true;

									::SetCursor(::LoadCursor(NULL,IDC_SIZENS));// IDC_SIZEWE  IDC_SIZENS
									m_sdotOldpoint = point;
									if(m_iMoveCurLine_X < 7) {
										pDC->MoveTo(idraw_startpoint_y + m_Position_X0[m_iMoveCurLine_Y][m_iMoveCurLine_X]*8,idraw_startpoint_x + m_Position_Y0[m_iMoveCurLine_Y][m_iMoveCurLine_X]*8);
										pDC->LineTo(idraw_startpoint_y + (m_Position_X1[m_iMoveCurLine_Y][m_iMoveCurLine_X]+1)*8,idraw_startpoint_x + m_Position_Y0[m_iMoveCurLine_Y][m_iMoveCurLine_X]*8);
										pDC->MoveTo(idraw_startpoint_y + m_Position_X0[m_iMoveCurLine_Y][m_iMoveCurLine_X]*8,m_sdotOldpoint.y);
										pDC->LineTo(idraw_startpoint_y + (m_Position_X1[m_iMoveCurLine_Y][m_iMoveCurLine_X]+1)*8,m_sdotOldpoint.y);	
									}
									else {
										pDC->MoveTo(idraw_startpoint_y + m_Position_X0[m_iMoveCurLine_Y][m_iMoveCurLine_X]*8 ,idraw_startpoint_x + m_Position_Y0[m_iMoveCurLine_Y][m_iMoveCurLine_X]*8);
										pDC->LineTo(idraw_startpoint_y + (m_Position_X1[m_iMoveCurLine_Y][m_iMoveCurLine_X])*8 + 8,idraw_startpoint_x + m_Position_Y0[m_iMoveCurLine_Y][m_iMoveCurLine_X]*8);
										pDC->MoveTo(idraw_startpoint_y + m_Position_X0[m_iMoveCurLine_Y][m_iMoveCurLine_X]*8 ,m_sdotOldpoint.y);
										pDC->LineTo(idraw_startpoint_y + (m_Position_X1[m_iMoveCurLine_Y][m_iMoveCurLine_X])*8 + 8,m_sdotOldpoint.y);	
									
									}									

									ReleaseDC(pDC);
									return;								
							}
						}
					}
				}				
			}  // end of for
		}

		for(int i=1;i<8;i++) { //画水平线
			//画水平长线			
			for(int no=1;no<7;no++) {
					if((point.x <= idraw_startpoint_y + m_Position_X1[i][no]*8)&&(point.x >= idraw_startpoint_y + m_Position_X0[i][no]*8)) {
						if((point.y <= idraw_startpoint_x + m_Position_Y0[i][no]*8 +3)&&(point.y >= idraw_startpoint_x + m_Position_Y0[i][no]*8 -3)) {
							if((m_Position_X1[i-1][0] == m_Position_X1[i][0])&&(m_Position_X0[i-1][7] == m_Position_X0[i][7])){  //判断前后是否平齐	
								if(!m_bMoveWholeLine) { //先判断 非 水平打断的短线
									ReleaseDC(pDC);
									return;
								}
									m_bMoveLongLine = true;						
									m_Long_Moving_Tab = i;	//有效范围0－7									
									m_bButtonDown = true;
									m_Moving_Tab_Hor = 0;
									m_Moving_Tab_Ver = 0;

									::SetCursor(::LoadCursor(NULL,IDC_SIZENS));// IDC_SIZEWE  IDC_SIZENS

									m_sdotOldpoint = point;
									pDC->MoveTo(idraw_startpoint_y + m_Position_X0[i][1]*8,idraw_startpoint_x + m_Position_Y0[i][no]*8);
									pDC->LineTo(idraw_startpoint_y + m_Position_X0[i][7]*8,idraw_startpoint_x + m_Position_Y0[i][no]*8);
									pDC->MoveTo(idraw_startpoint_y + m_Position_X0[i][1]*8,m_sdotOldpoint.y);
									pDC->LineTo(idraw_startpoint_y + m_Position_X0[i][7]*8,m_sdotOldpoint.y);	
									ReleaseDC(pDC);
									return;								
							}
					}
				}
			}
			//画水平前短线		
			if((point.y <= idraw_startpoint_x + m_Position_Y0[i][0]*8 +3)&&(point.y >= idraw_startpoint_x + m_Position_Y0[i][0]*8 -3)) {						
				if((point.x < idraw_startpoint_y + m_Position_X0[i][1]*8)&&(point.x > idraw_startpoint_y + m_Position_X0[i][0]*8)) { //水平前一短线
					if(m_Position_X1[i-1][0] == m_Position_X1[i][0]){  //判断前后/上下是否平齐
						m_bMoveLongLine = false;
						m_Long_Moving_Tab = 0;	//有效范围0－7
						m_bButtonDown = true;
						m_Moving_Tab_Hor = 0;
						m_Moving_Tab_Ver = i;

						::SetCursor(::LoadCursor(NULL,IDC_SIZENS));// IDC_SIZEWE  IDC_SIZENS

						m_sdotOldpoint = point;
						pDC->MoveTo(idraw_startpoint_y + m_Position_X0[i][0]*8,idraw_startpoint_x + m_Position_Y0[i][0]*8);
						pDC->LineTo(idraw_startpoint_y + (m_Position_X1[i][0]+1)*8,idraw_startpoint_x + m_Position_Y0[i][0]*8);
						pDC->MoveTo(idraw_startpoint_y + m_Position_X0[i][0]*8,m_sdotOldpoint.y);
						pDC->LineTo(idraw_startpoint_y + (m_Position_X1[i][0]+1)*8,m_sdotOldpoint.y);	
						ReleaseDC(pDC);
						return;
					}
				}				
			}
			//画水平后短线
			if((point.y <= idraw_startpoint_x + m_Position_Y0[i][7]*8 +3)&&(point.y >= idraw_startpoint_x + m_Position_Y0[i][7]*8 -3)) {
				if((point.x < idraw_startpoint_y + m_Position_X1[i][7]*8)&&(point.x > idraw_startpoint_y + m_Position_X0[i][7]*8)) { //水平后一短线
					if(m_Position_X0[i-1][7] == m_Position_X0[i][7]){  //判断前后/上下是否平齐
						m_bMoveLongLine = false;
						m_Long_Moving_Tab = 0;	//有效范围0－7
						m_bButtonDown = true;
						m_Moving_Tab_Hor = 7;
						m_Moving_Tab_Ver = i;

						::SetCursor(::LoadCursor(NULL,IDC_SIZENS));// IDC_SIZEWE  IDC_SIZENS

						m_sdotOldpoint = point;
						pDC->MoveTo(idraw_startpoint_y + m_Position_X0[i][7]*8,idraw_startpoint_x + m_Position_Y0[i][7]*8);
						pDC->LineTo(idraw_startpoint_y + m_Position_X1[i][7]*8 + 8,idraw_startpoint_x + m_Position_Y0[i][7]*8);
						pDC->MoveTo(idraw_startpoint_y + m_Position_X0[i][7]*8,m_sdotOldpoint.y);
						pDC->LineTo(idraw_startpoint_y + m_Position_X1[i][7]*8 + 8,m_sdotOldpoint.y);	
						ReleaseDC(pDC);
						return;
					}
				}
			}
		}
		//-----【水平方向的垂直短线】------//		
			for(i=0;i<8;i++) {				
				for(int j=1;j<8;j++) {
					if((point.x < idraw_startpoint_y + m_Position_X0[i][j]*8+5)&&(point.x > idraw_startpoint_y + m_Position_X0[i][j]*8-5)) {
						if((point.y < idraw_startpoint_x + m_Position_Y1[i][j]*8)&&(point.y > idraw_startpoint_x + m_Position_Y0[i][j]*8)) {
							if((m_Position_Y0[i][j-1] == m_Position_Y0[i][j])&&(m_Position_Y1[i][j-1] == m_Position_Y1[i][j])){  //判断前后/上下是否平齐
								m_bMoveLongLine = false;
								m_Long_Moving_Tab = 0;	//有效范围0－7
								m_bShort_Line_Valid = true;
								m_bButtonDown = true;
								m_Moving_Tab_Hor = j;
								m_Moving_Tab_Ver = i;

								::SetCursor(::LoadCursor(NULL,IDC_SIZEWE));// IDC_SIZEWE  IDC_SIZENS

								m_sdotOldpoint = point;
								
								if(i==7) {
									pDC->MoveTo(idraw_startpoint_y + m_Position_X0[i][j]*8,idraw_startpoint_x + m_Position_Y0[i][j]*8 );
									pDC->LineTo(idraw_startpoint_y + m_Position_X0[i][j]*8,idraw_startpoint_x + m_Position_Y1[i][j]*8 + 8); //akang915
								}
								else {
									pDC->MoveTo(idraw_startpoint_y + m_Position_X0[i][j]*8,idraw_startpoint_x + m_Position_Y0[i][j]*8);
									pDC->LineTo(idraw_startpoint_y + m_Position_X0[i][j]*8,idraw_startpoint_x + (m_Position_Y1[i][j]+1)*8);
								}

								
								if(i==7) {
									pDC->MoveTo(m_sdotOldpoint.x,idraw_startpoint_x + m_Position_Y0[i][j]*8);
									pDC->LineTo(m_sdotOldpoint.x,idraw_startpoint_x + m_Position_Y1[i][j]*8 + 8); //akang915	
								}
								else {
									pDC->MoveTo(m_sdotOldpoint.x,idraw_startpoint_x + m_Position_Y0[i][j]*8);
									pDC->LineTo(m_sdotOldpoint.x,idraw_startpoint_x + (m_Position_Y1[i][j]+1)*8);	
								}
								ReleaseDC(pDC);
								return;
							}
						}
					}
				}
			}

		

	}
	else {   //垂直方向一致
		if(!m_bMoveWholeLine) { //画垂直打断的短线；
			for(int i=1;i<8;i++) {			
				for(int no=1;no<=7;no++) {
					if((point.y <= idraw_startpoint_x + m_Position_Y1[i][no]*8)&&(point.y >= idraw_startpoint_x + m_Position_Y0[i][no]*8)) {
						if((point.x <= idraw_startpoint_y + m_Position_X0[i][no]*8 +3)&&(point.x >= idraw_startpoint_y + m_Position_X0[i][no]*8 -3)) {
							if((m_Position_Y1[i][no-1] == m_Position_Y1[i][no])&&(m_Position_Y0[i][no-1] == m_Position_Y0[i][no])){  //判断前后是否平齐
								
									m_iMoveCurLine_X = no;
									m_iMoveCurLine_Y = i;	

									m_bMovingVerDirection = true;
									m_bEnMoveBreakLine = true;
									m_bButtonDown = true;	
									::SetCursor(::LoadCursor(NULL,IDC_SIZEWE));// IDC_SIZEWE  IDC_SIZENS									

									m_sdotOldpoint = point;

									if(m_iMoveCurLine_Y <7) {
										pDC->MoveTo(idraw_startpoint_y + m_Position_X0[m_iMoveCurLine_Y][m_iMoveCurLine_X]*8,idraw_startpoint_x + m_Position_Y0[m_iMoveCurLine_Y][m_iMoveCurLine_X]*8);
										pDC->LineTo(idraw_startpoint_y + m_Position_X0[m_iMoveCurLine_Y][m_iMoveCurLine_X]*8,idraw_startpoint_x + (m_Position_Y1[m_iMoveCurLine_Y][m_iMoveCurLine_X]+1)*8);
										pDC->MoveTo(m_sdotOldpoint.x,idraw_startpoint_x + m_Position_Y0[m_iMoveCurLine_Y][m_iMoveCurLine_X]*8);
										pDC->LineTo(m_sdotOldpoint.x,idraw_startpoint_x + (m_Position_Y1[m_iMoveCurLine_Y][m_iMoveCurLine_X]+1)*8);	
									}
									else {
										pDC->MoveTo(idraw_startpoint_y + m_Position_X0[m_iMoveCurLine_Y][m_iMoveCurLine_X]*8,idraw_startpoint_x + m_Position_Y0[m_iMoveCurLine_Y][m_iMoveCurLine_X]*8);
										pDC->LineTo(idraw_startpoint_y + m_Position_X0[m_iMoveCurLine_Y][m_iMoveCurLine_X]*8,idraw_startpoint_x + (m_Position_Y1[m_iMoveCurLine_Y][m_iMoveCurLine_X])*8 +8);//akang915
										pDC->MoveTo(m_sdotOldpoint.x,idraw_startpoint_x + m_Position_Y0[m_iMoveCurLine_Y][m_iMoveCurLine_X]*8);
										pDC->LineTo(m_sdotOldpoint.x,idraw_startpoint_x + (m_Position_Y1[m_iMoveCurLine_Y][m_iMoveCurLine_X])*8 + 8);//akang915

									}

									ReleaseDC(pDC);
									return;

							}


						}
					}
				}				
			}  // end of for
		}

		for(int i=1;i<8;i++) { //画垂直长线
			for(int no=1;no<=7;no++) {
				if((point.y <= idraw_startpoint_x + m_Position_Y1[i][no]*8)&&(point.y >= idraw_startpoint_x + m_Position_Y0[i][no]*8)) {
					if((point.x <= idraw_startpoint_y + m_Position_X0[i][no]*8 +3)&&(point.x >= idraw_startpoint_y + m_Position_X0[i][no]*8 -3)) {
						if((m_Position_Y1[0][no-1] == m_Position_Y1[0][no])&&(m_Position_Y0[7][no-1] == m_Position_Y0[7][no])){  //判断前后是否平齐	
								
							if(!m_bMoveWholeLine) { //先判断 非 垂直打断的短线
								ReleaseDC(pDC);
								return;
							}	
							if(i < 7) {
								m_bMoveLongLine = true;
								m_Long_Moving_Tab = no;	//有效范围0－7  
								m_bButtonDown = true;

								::SetCursor(::LoadCursor(NULL,IDC_SIZEWE));// IDC_SIZEWE  IDC_SIZENS

								m_sdotOldpoint = point;
								pDC->MoveTo(idraw_startpoint_y + m_Position_X0[i][no]*8,idraw_startpoint_x + m_Position_Y0[1][no]*8);
								pDC->LineTo(idraw_startpoint_y + m_Position_X0[i][no]*8,idraw_startpoint_x + m_Position_Y0[7][no]*8);
								pDC->MoveTo(m_sdotOldpoint.x,idraw_startpoint_x + m_Position_Y0[1][no]*8);
								pDC->LineTo(m_sdotOldpoint.x,idraw_startpoint_x + m_Position_Y0[7][no]*8);	
								ReleaseDC(pDC);
								return;
							}
						}
					}
				}
			}
			//画垂直前短线		
			if((point.x <= idraw_startpoint_y + m_Position_X0[0][i]*8 +3)&&(point.x >= idraw_startpoint_y + m_Position_X0[0][i]*8 -3)) {						
				if((point.y < idraw_startpoint_x + m_Position_Y1[0][i]*8)&&(point.y > idraw_startpoint_x + m_Position_Y0[0][i]*8)) { //垂直前一短线
					if(m_Position_Y1[0][i-1] == m_Position_Y1[0][i]){  //判断前后/上下是否平齐
						m_bMoveLongLine = false;
						m_Long_Moving_Tab = 0;	//有效范围0－7
						m_bButtonDown = true;
						m_Moving_Tab_Hor = i;
						m_Moving_Tab_Ver = 0;
						::SetCursor(::LoadCursor(NULL,IDC_SIZEWE));// IDC_SIZEWE  IDC_SIZENS

						m_sdotOldpoint = point;
						pDC->MoveTo(idraw_startpoint_y + m_Position_X0[0][i]*8,idraw_startpoint_x + m_Position_Y0[0][i]*8);
						pDC->LineTo(idraw_startpoint_y + m_Position_X0[0][i]*8,idraw_startpoint_x + (m_Position_Y1[0][i]+1)*8);
						pDC->MoveTo(m_sdotOldpoint.x,idraw_startpoint_x + m_Position_Y0[0][i]*8);
						pDC->LineTo(m_sdotOldpoint.x,idraw_startpoint_x + (m_Position_Y1[0][i]+1)*8);	
						ReleaseDC(pDC);
						return;
					}
				}				
			}
			//画垂直后一短线
			if((point.x <= idraw_startpoint_y + m_Position_X0[7][i]*8 +3)&&(point.x >= idraw_startpoint_y + m_Position_X0[7][i]*8 -3)) {						
				if((point.y < idraw_startpoint_x + m_Position_Y1[7][i]*8)&&(point.y > idraw_startpoint_x + m_Position_Y0[7][i]*8)) { //垂直后一短线
					if(m_Position_Y0[7][i-1] == m_Position_Y0[7][i]){  //判断前后/上下是否平齐
						m_bMoveLongLine = false;
						m_Long_Moving_Tab = 0;	//有效范围0－7
						m_bButtonDown = true;
						m_Moving_Tab_Hor = i;
						m_Moving_Tab_Ver = 7;

						::SetCursor(::LoadCursor(NULL,IDC_SIZEWE));// IDC_SIZEWE  IDC_SIZENS

						m_sdotOldpoint = point;
						pDC->MoveTo(idraw_startpoint_y + m_Position_X0[7][i]*8,idraw_startpoint_x + m_Position_Y0[7][i]*8);
						pDC->LineTo(idraw_startpoint_y + m_Position_X0[7][i]*8,idraw_startpoint_x + m_Position_Y1[7][i]*8 + 8);
						pDC->MoveTo(m_sdotOldpoint.x,idraw_startpoint_x + m_Position_Y0[7][i]*8);
						pDC->LineTo(m_sdotOldpoint.x,idraw_startpoint_x + m_Position_Y1[7][i]*8 + 8);	
						ReleaseDC(pDC);
						return;
					}
				}				
			}
		}
		//-----【垂直方向的水平短线】------//
		{
			for(int i=0;i<8;i++) { //水平方向				
				for(int j=1;j<8;j++) { //垂直方向
					if((point.y < idraw_startpoint_x + m_Position_Y0[j][i]*8+5)&&(point.y > idraw_startpoint_x + m_Position_Y0[j][i]*8-5)) {
						if((point.x < idraw_startpoint_y + m_Position_X1[j][i]*8)&&(point.x > idraw_startpoint_y + m_Position_X0[j][i]*8)) {
							if((m_Position_X0[j-1][i] == m_Position_X0[j][i])&&(m_Position_X1[j-1][i] == m_Position_X1[j][i])){  //判断前后/上下是否平齐
								m_bMoveLongLine = false;
								m_Long_Moving_Tab = 0;	//有效范围0－7
								m_bShort_Line_Valid = true;
								m_bButtonDown = true;
								m_Moving_Tab_Hor = i;
								m_Moving_Tab_Ver = j;

								::SetCursor(::LoadCursor(NULL,IDC_SIZENS));// IDC_SIZEWE  IDC_SIZENS

								m_sdotOldpoint = point;
								
								if( i==7) {
									pDC->MoveTo(idraw_startpoint_y + m_Position_X0[j][i]*8,idraw_startpoint_x + m_Position_Y0[j][i]*8);
									pDC->LineTo(idraw_startpoint_y + m_Position_X1[j][i]*8 +8,idraw_startpoint_x + m_Position_Y0[j][i]*8); //akang915
								}
								else {
									pDC->MoveTo(idraw_startpoint_y + m_Position_X0[j][i]*8,idraw_startpoint_x + m_Position_Y0[j][i]*8);
									pDC->LineTo(idraw_startpoint_y + (m_Position_X1[j][i]+1)*8,idraw_startpoint_x + m_Position_Y0[j][i]*8);
								}

								
								if(i==7) {
									pDC->MoveTo(idraw_startpoint_y + m_Position_X0[j][i]*8,m_sdotOldpoint.y);
									pDC->LineTo(idraw_startpoint_y + m_Position_X1[j][i]*8 + 8,m_sdotOldpoint.y);	 //akang915
								}
								else {
									pDC->MoveTo(idraw_startpoint_y + m_Position_X0[j][i]*8,m_sdotOldpoint.y);
									pDC->LineTo(idraw_startpoint_y + (m_Position_X1[j][i]+1)*8,m_sdotOldpoint.y);	
								}
								ReleaseDC(pDC);
								return;
							}
						 
						}
					}
				}
			}
		} //end of 【垂直方向的水平短线】/////////////////////////////////////////////////

	}

	ReleaseDC(pDC);

	CView::OnLButtonDown(nFlags, point);
}

void CMaintView::OnLButtonUp(UINT nFlags, CPoint point) 
{
	ReleaseCapture();
	ClipCursor(NULL);
	CView::OnLButtonUp(nFlags, point);

	CStatusBar* pStatus=(CStatusBar*)
		AfxGetApp()->m_pMainWnd->GetDescendantWindow(ID_VIEW_STATUS_BAR);	

	if(m_bMoving64EnergLimitEn) { // 能谱操作  进行64显示模式下的 能谱边界调整 
		int barsize_x = 100;
		int barsize_y = 80;
		int barstart_x = 50;
		int barstart_y = 75;
		char chrtext[200];

		m_bMoving64EnergLimitEn = false;

		if ((point.x >= ( barsize_x * (m_iEnergeSpectCurSynNo % 8) + barstart_x)) &&
			(point.x <= (barsize_x * (m_iEnergeSpectCurSynNo % 8) + barstart_x + 63)) ){
			
			if ((point.y < (barsize_y * (m_iEnergeSpectCurSynNo / 8) + barstart_y)) && (point.y > (barsize_y * (m_iEnergeSpectCurSynNo / 8) + barstart_y - 40)) ) {
				
				m_Oldpoint = point;
				
				if(m_bEnergeSpectMove_Eth_Low) {
					m_iEnergeSpectEth_Low[m_iEnergeSpectCurBlockNo][m_iEnergeSpectCurSynNo] = point.x- barsize_x * (m_iEnergeSpectCurSynNo % 8) - barstart_x;
				}
				else if(m_bEnergeSpectMove_Eth_Hight) {
					m_iEnergeSpectEth_Hight[m_iEnergeSpectCurBlockNo][m_iEnergeSpectCurSynNo] = point.x- barsize_x * (m_iEnergeSpectCurSynNo % 8) - barstart_x;	
				}
			}		
		}		
		m_Show64EngerSpectrum();
		sprintf(chrtext,"晶体条%d : Low Eth %d, High Eth %d",m_iEnergeSpectCurSynNo,m_iEnergeSpectEth_Low[m_iEnergeSpectCurBlockNo][m_iEnergeSpectCurSynNo],
			m_iEnergeSpectEth_Hight[m_iEnergeSpectCurBlockNo][m_iEnergeSpectCurSynNo]);
		pStatus->SetPaneText(2,chrtext,1);
		return;
	}


	if(m_bEnergeSpectZoomInMode) { //能谱操作方式
		if((m_bEnergeSpectMove_Eth_Low)||(m_bEnergeSpectMove_Eth_Hight)) {
			m_bEnergeSpectMove_Eth_Low = false;
			m_bEnergeSpectMove_Eth_Hight = false;
			//OnRedraw_2d();[m_iEnergeSpectCurBlockNo][m_iEnergeSpectCurSynNo]
			OnDrawZoomInEnergeSpect(m_iEnergeSpectCurBlockNo,m_iEnergeSpectCurSynNo);
		}
		CString strtext;
		strtext.Format("状态显示：当前能谱 Eth 低：%d   Eth高: %d",m_iEnergeSpectEth_Low[m_iEnergeSpectCurBlockNo][m_iEnergeSpectCurSynNo],m_iEnergeSpectEth_Hight[m_iEnergeSpectCurBlockNo][m_iEnergeSpectCurSynNo]);
		pStatus->SetPaneText(2,strtext,1);
		return;
	}	

	//---------【散点的位置边界平移】------//	
	int idraw_startpoint_y = g_draw_startpoint_y - 4;
	int idraw_startpoint_x = g_draw_startpoint_x - 4;

	if(!m_bButtonDown) return;

	g_bPLimitChangeValid = true;

	if((m_bEnMoveBreakLine)) {  //断线平移
		m_bEnMoveBreakLine = false;
		m_bButtonDown = false;	
		if(m_bthisMoveInValid) {  //移动是否有效
			m_bthisMoveInValid = false;
			RedrawLimitFile(); ///akang
			return;
		}
		if(!m_bMovingVerDirection) {  //水平方向
			int pointvalue = (point.y-idraw_startpoint_x +4)/8;
			if((pointvalue > 63)||(pointvalue <0)) return;
			m_Position_Y0[m_iMoveCurLine_Y][m_iMoveCurLine_X] = pointvalue;
			m_Position_Y1[m_iMoveCurLine_Y-1][m_iMoveCurLine_X] = pointvalue-1;
		}
		if(m_bMovingVerDirection) {  //垂直方向
			int pointvalue = (point.x-idraw_startpoint_y +4)/8;
			if((pointvalue > 63)||(pointvalue <0)) return;
			m_Position_X0[m_iMoveCurLine_Y][m_iMoveCurLine_X] = pointvalue;
			m_Position_X1[m_iMoveCurLine_Y][m_iMoveCurLine_X-1] = pointvalue-1;
		}		
		RedrawLimitFile(); ///akang
		return;
	}

	if(m_bMoveLongLine) { //长线
		if(m_bSameDriction_X) {//水平方向
			int pointvalue = (point.y-idraw_startpoint_x +4)/8;
			if((pointvalue > 63)||(pointvalue <0)) return;
			for(int i=1;i<7;i++) {
				if(!m_bthisMoveInValid) {
					m_Position_Y0[m_Long_Moving_Tab][i] = pointvalue;
					m_Position_Y1[m_Long_Moving_Tab-1][i] = pointvalue-1;
				}
			}			
		}
		else { //垂直方向
			int pointvalue = (point.x-idraw_startpoint_y +4)/8;
			if((pointvalue > 63)||(pointvalue <0)) return;
			for(int i=1;i<7;i++) {
				if(!m_bthisMoveInValid) {
					m_Position_X0[i][m_Long_Moving_Tab] = pointvalue;
					m_Position_X1[i][m_Long_Moving_Tab-1] = pointvalue-1;					
				}
			}		
		}
		m_bthisMoveInValid = false;
		m_bMoveLongLine = false;
		m_bButtonDown = false;	
		
		RedrawLimitFile(); ///akang
	}
	else { //短线
		if(m_bSameDriction_X) { //水平短线
			int pointvalue = (point.y-idraw_startpoint_x +4)/8;
			if((pointvalue > 63)||(pointvalue <0)) return;

			if(m_bShort_Line_Valid) {  //水平方向一致的 垂直短线有效
				pointvalue = (point.x-idraw_startpoint_y +4)/8;
				if((pointvalue > 63)||(pointvalue <0)) return;	

				if(!m_bthisMoveInValid) {
					m_Position_X0[m_Moving_Tab_Ver][m_Moving_Tab_Hor] = pointvalue;
					m_Position_X1[m_Moving_Tab_Ver][m_Moving_Tab_Hor-1] = pointvalue-1;	
				}
				m_bthisMoveInValid = false;
				m_bShort_Line_Valid = false;
				m_bMoveLongLine = false;
				m_bButtonDown = false;
				RedrawLimitFile(); ///akang
				return;
			}
			if(m_Moving_Tab_Hor == 0) { //前一段水平短线
				if(!m_bthisMoveInValid) {
					m_Position_Y0[m_Moving_Tab_Ver][0] = pointvalue;
					m_Position_Y1[m_Moving_Tab_Ver-1][0] = pointvalue-1;					
				}
			}
			else{//后一段水平短线
				if(!m_bthisMoveInValid) {
					m_Position_Y0[m_Moving_Tab_Ver][7] = pointvalue;
					m_Position_Y1[m_Moving_Tab_Ver-1][7] = pointvalue-1;					
				}
			}		
			RedrawLimitFile(); ///akang
		}
		else { // 垂直短线
			int pointvalue = (point.x-idraw_startpoint_y +4)/8;
			if((pointvalue > 63)||(pointvalue <0)) return;

			if(m_bShort_Line_Valid) {  //垂直方向一致的 水平短线有效
				pointvalue = (point.y-idraw_startpoint_x +4)/8;
				if((pointvalue > 63)||(pointvalue <0)) return;	

				if(!m_bthisMoveInValid) {
					m_Position_Y0[m_Moving_Tab_Ver][m_Moving_Tab_Hor] = pointvalue;
					m_Position_Y1[m_Moving_Tab_Ver-1][m_Moving_Tab_Hor] = pointvalue-1;						
				}
				m_bthisMoveInValid = false;
				m_bShort_Line_Valid = false;
				m_bMoveLongLine = false;
				m_bButtonDown = false;
				RedrawLimitFile(); ///akang
				return;
			}

			if(m_Moving_Tab_Ver == 0) { //前一段垂直短线
				if(!m_bthisMoveInValid) {
					m_Position_X0[0][m_Moving_Tab_Hor] = pointvalue;
					m_Position_X1[0][m_Moving_Tab_Hor-1] = pointvalue-1;					
				}
			}
			else{//后一段垂直短线
				if(!m_bthisMoveInValid) {
					m_Position_X0[7][m_Moving_Tab_Hor] = pointvalue;
					m_Position_X1[7][m_Moving_Tab_Hor-1] = pointvalue-1;					
				}
			}
		}
		m_bthisMoveInValid = false;
		m_bMoveLongLine = false;
		m_bButtonDown = false;			
	}
	
	RedrawLimitFile(); ///akang

}

// --------------------------------------------------

void CMaintView::Calculate_X_Limit()
{
	float map_x[8][64];
	float max_x[8][8];
	float min_x[8][9];

	float temp;
	int dddd;
   
	int B[9];
	for (int k=0;k<9;k++) B[k] = B_Y[k];
	for(int i=0;i<8;i++) {
		for(int j=0;j<64;j++) {
			map_x[i][j] = 0;
		}
	}

	for (i=0;i<64;i++) {
		for (int j=B[0]-1;j<B[1];j++) {
			map_x[0][i] += sdot[i][j];
		}
	}
	for ( i=1;i<8;i++) {
		for( int j=0;j<64;j++) {
			for( k=B[i];k<B[i+1];k++) {
				map_x[i][j] += sdot[j][k];
			}
		}
	}

    for( i =0;i<8;i++)
		for (int j=0;j<9;j++)
			m_Limit_Min_X[i][j] = 1;



// filter 3 dot      
float a = 0.05;    //a = 0.09;    
float b = 1+ 2*a;

//	for( i=0;i<8;i++) {
//		map_x[i][1] = (a*map_x[i][63] + map_x[i][0] + a*map_x[i][1] )/b;
//		map_x[i][64] = (a*map_x[i][0] + map_x[i][63] + a*map_x[i][62] )/b;
//		for( int num = 1;num<63;num++){
//			map_x[i][num] = (a*map_x[i][num-1] + map_x[i][num] + a*map_x[i][num+1])/b;
//		}
//	}

// find  X ( Max, Min Limit)

	for( i=0;i<8;i++) {
		max_x[i][0] = map_x[i][0];
		min_x[i][0] = map_x[i][0];
		min_x[i][8] = map_x[i][63];
	}

	for( i=0;i<8;i++) {
		int min_num_x = 0;
		int max_num_x = 0;
		if( map_x[i][0]<map_x[i][1]) {
			min_x[i][0] = map_x[i][1];
			m_Limit_Min_X[i][min_num_x] = 1; 
		}
		if( map_x[i][63]<map_x[i][62]) {
			min_x[i][8] = map_x[i][63];
			m_Limit_Min_X[i][8] = 64; 
		}

		for(int j=1;j<63;j++) {
			if((map_x[i][j]< map_x[i][j+1]) && (map_x[i][j] < map_x[i][j-1])) {//11
				if(j-m_Limit_Min_X[i][min_num_x] >=3) {//add
					min_num_x ++;
					min_x[i][min_num_x] = map_x[i][j];
					if (min_num_x > 8) {                 
               			min_num_x = 8;
					}
					m_Limit_Min_X[i][min_num_x] = j+1;  
				}

			} //end 11
			
			if((map_x[i][j]> map_x[i][j+1]) && (map_x[i][j] > map_x[i][j-1])) {
				max_x[i][max_num_x] = map_x[i][j];
				m_Limit_Max_X[i][max_num_x] = j+1;         
				max_num_x ++ ;        
			}
			int tttt= j;
			if( (tttt-(int)(m_Limit_Min_X[i][min_num_x])) >= 12) { //--new add
				 min_num_x ++;
				 min_x[i][min_num_x] = map_x[i][j]; 				 
				 m_Limit_Min_X[i][min_num_x] = m_Limit_Min_X[i][min_num_x-1]+ 12; 			   
			}//---new add
		}

		if (min_num_x < 8)  { 
			  dddd = m_Limit_Min_X[i][min_num_x];
			  temp = 64 - dddd; //m_Limit_Min_X[i][min_num_x];
			  int lenth = (int)( temp / (8-min_num_x));
			  for (int kkk= min_num_x+1;kkk<8; kkk++) {
				 if (m_Limit_Min_X[i][kkk-1] < 64-lenth)
					m_Limit_Min_X[i][kkk] = m_Limit_Min_X[i][kkk-1] + lenth;   
				 else 
					m_Limit_Min_X[i][kkk] = 64;  				 
			  } 
		}//end of if min_num_x < 8

		m_Limit_Min_X[i][8] = 64;  //akang 2000/10.28
		m_Limit_Min_X[i][0] = 1;  //akang 2000/10.28

	} //end for i=1:8

//  calculate X avarage 
//  avarage_limit_min_x(1:9) = 0;  
	for(i=0;i<9;i++) m_Avarage_Limit_Min_X[i] = 0;

	for ( i=1;i<8;i++) { //1-7
		for ( k=1;i<8;i++) {//k=1:7
			m_Avarage_Limit_Min_X[i] += m_Limit_Min_X[k][i]; 
		}
		m_Avarage_Limit_Min_X[i] = m_Avarage_Limit_Min_X[i]/7;        
	}
	int int_avarage_limit_min_x[9];
	for(i=0;i<9;i++) {
		int_avarage_limit_min_x[i]= (int)(m_Avarage_Limit_Min_X[i] + 0.5); 
	}
		
	m_Sum_Square_X = 0.0;
	float ftemp;
	for(i=1;i<8;i++) {
		for(int j=1;j<8;j++) {
			ftemp = (float)m_Limit_Min_X[j][i] - (float)m_Avarage_Limit_Min_X[i];
			m_Sum_Square_X += ftemp *  ftemp;
		}
	}
  
	//平均值转移 到 B_X[]
	for(i=1;i<8;i++)
		B_X[i]= int_avarage_limit_min_x[i];

}

void CMaintView::Calculate_Y_Limit()
{
	float map_y[8][64];
	float max_y[8][8];
	float min_y[8][9];

	float temp;
	int dddd;
   
	int B[9];
	for (int k=0;k<9;k++) B[k] = B_X[k];
	//for (int k=0;k<9;k++) B[k] = B_X_Test[k];

	for(int i=0;i<8;i++) {
		for(int j=0;j<64;j++) {
			map_y[i][j] = 0;
		}
	}

	float tt1,tt2;
	tt1= map_y[0][0];
	tt2= map_y[0][1];
	tt1= map_y[0][2];
	tt2= map_y[1][0];

	for (i=0;i<64;i++) {
		for (int j=B[0]-1;j<B[1];j++) {
			map_y[0][i] += sdot[j][i];
		}
	}
	for ( i=1;i<8;i++) {
		for( int j=0;j<64;j++) {
			for( k=B[i];k<B[i+1];k++) {
				map_y[i][j] += sdot[k][j];
			}
		}
	}

	tt1= map_y[0][0];
	tt2= map_y[0][1];
	tt1= map_y[0][2];
	tt2= map_y[1][0];


    for( i =0;i<8;i++) {
		for (int j=0;j<9;j++)
			m_Limit_Min_Y[i][j] = 1;
	}
// filter 3 dot      
float a = 0.05;    //a = 0.09;    
float b = 1+ 2*a;

//	for( i=0;i<8;i++) {
//		map_y[i][1] = (a*map_y[i][63] + map_y[i][0] + a*map_y[i][1] )/b;
//		map_y[i][64] = (a*map_y[i][0] + map_y[i][63] + a*map_y[i][62] )/b;
//		for( int num = 1;num<63;num++){
//			map_y[i][num] = (a*map_y[i][num-1] + map_y[i][num] + a*map_y[i][num+1])/b;
//		}
//	}

// find  Y ( Max, Min Limit)
	for( i=0;i<8;i++) {
		max_y[i][0] = map_y[i][0];
		min_y[i][0] = map_y[i][0];
		min_y[i][8] = map_y[i][63];
	}

//m_limit_min_x(1:8,1:9) = 1;
//m_limit_max_x(1:8,1:9) = 1;
	for( i=0;i<8;i++) {
		int min_num_y = 0;
		int max_num_y = 0;
		if( map_y[i][0]<map_y[i][1]) {
			min_y[i][0] = map_y[i][1];
			m_Limit_Min_Y[i][min_num_y] = 1; 
		}
		if( map_y[i][63]<map_y[i][62]) {
			min_y[i][8] = map_y[i][63];
			m_Limit_Min_Y[i][8] = 64; 
		}

		for(int j=1;j<63;j++) {
			if((map_y[i][j]< map_y[i][j+1]) && (map_y[i][j] < map_y[i][j-1])) {//11
				if(j-m_Limit_Min_Y[i][min_num_y] >=3) {//add
					min_num_y ++;
					min_y[i][min_num_y] = map_y[i][j];
					if (min_num_y > 8) {                 
               			min_num_y = 8;
					}
					m_Limit_Min_Y[i][min_num_y] = j+1;  
				}

			} //end 11
			
			if((map_y[i][j]> map_y[i][j+1]) && (map_y[i][j] > map_y[i][j-1])) {
				max_y[i][max_num_y] = map_y[i][j];
				m_Limit_Max_Y[i][max_num_y] = j+1;         
				max_num_y ++ ;        
			}
			int tttt= j;
			if( (tttt-m_Limit_Min_Y[i][min_num_y]) >= 12 ) { //--new add
				 min_num_y ++;
				 min_y[i][min_num_y] = map_y[i][j];  
				 m_Limit_Min_Y[i][min_num_y] = m_Limit_Min_Y[i][min_num_y-1]+ 8; 			   
			}//---new add
		}

		if (min_num_y < 8)  { 
			  dddd = m_Limit_Min_Y[i][min_num_y];
			  temp = 64 - dddd; //m_Limit_Min_X[i][min_num_x];
			  int lenth = (int)( temp / (8-min_num_y));
			  for (int kkk= min_num_y+1;kkk<8; kkk++) {
				 if (m_Limit_Min_Y[i][kkk-1] < 64-lenth)
					m_Limit_Min_Y[i][kkk] = m_Limit_Min_Y[i][kkk-1] + lenth;   
				 else 
					m_Limit_Min_Y[i][kkk] = 64;  				 
			  } 
		}//end of if min_num_y < 8
		m_Limit_Min_Y[i][8] = 64;  //akang 2000/10.28
		m_Limit_Min_Y[i][0] = 1;  //akang 2000/10.28



	} //end for i=1:8

//  calculate Y avarage 
//  avarage_limit_min_y(1:9) = 0;  
	for(i=0;i<9;i++) m_Avarage_Limit_Min_Y[i] = 0;

	for ( i=1;i<8;i++) { //1-7
		for ( k=1;i<8;i++) {//k=1:7
			m_Avarage_Limit_Min_Y[i] += m_Limit_Min_Y[k][i]; 
		}
		m_Avarage_Limit_Min_Y[i] = m_Avarage_Limit_Min_Y[i]/7;        
	}
	int int_avarage_limit_min_y[9];
	for(i=0;i<9;i++) {
		int_avarage_limit_min_y[i]= (int)(m_Avarage_Limit_Min_Y[i] + 0.5); 
	}

	m_Sum_Square_Y = 0.0;
	float ftemp2;
	for(i=1;i<8;i++) {
		for(int j=1;j<8;j++) {
			ftemp2 = ((float)m_Limit_Min_Y[j][i] - m_Avarage_Limit_Min_Y[i]);
			m_Sum_Square_Y += (ftemp2 *  ftemp2);
		}
	}

	//平均值转移 到 B_X[]
	for(i=1;i<8;i++)
		B_Y[i]= int_avarage_limit_min_y[i];

}

void CMaintView::OnPaintLine(int *B, int direct)
{

}

void CMaintView::OnCalculateLimit() 
{
	m_Sum_Square_X = 0;
	m_Sum_Square_Y = 0;
	for(int loop=0;loop<8;loop++) {
		Calculate_X_Limit();
		Calculate_Y_Limit();
	}

	CDC* pDC=GetDC();
    CPen pShow(PS_SOLID,2,RGB(15, 105,185));//RGB(0,210,0)
    CPen* pOldPen=NULL;
    pOldPen=pDC->SelectObject(&pShow); 
	pDC->SetROP2(R2_NOT);

	if(m_Sum_Square_X < m_Sum_Square_Y) {  

		bDirectShift_X_Y = true;

		for(int i=0;i<9;i++) {  //画 水平直线
			pDC->MoveTo(90,50+ (B_X[i]-1)*8);
			pDC->LineTo(90+63*8,50+(B_X[i]-1)*8);
		}		
		for(i=0;i<8;i++) {
			for(int j=0;j<9;j++) {
				pDC->MoveTo(90+(m_Limit_Min_Y[i][j]-1)*8,50+(B_X[i]-1)*8);
				pDC->LineTo(90+(m_Limit_Min_Y[i][j]-1)*8,50+(B_X[i]-1)*8 + (B_X[i+1]-B_X[i])*8);
			}
		}

		for(i=0;i<8;i++) {
			for(int j=0;j<9;j++) {
				m_Save_Limit[i][j] = m_Limit_Min_Y[i][j];
				m_Save_UpDownLimit_B[i][j] = m_Limit_Min_Y[i][j];
			}
		}
		for(i=0;i<9;i++) {
			m_Save_UpDownLimit_B[0][i] = B_X[i];
			m_Save_UpDownLimit_B[1][i] = B_X[i];
			m_Save_UpDownLimt_Lenth[0][i] = m_Limit_Min_Y[i][1];
			m_Save_UpDownLimt_Lenth[1][i] = m_Limit_Min_Y[i][7];
		}

	}
	else {  
		bDirectShift_X_Y = false;

		for(int i=0;i<9;i++) {  //画 垂直直线
			pDC->MoveTo(90+(B_Y[i]-1)*8,50);
			pDC->LineTo(90+(B_Y[i]-1)*8,50 + 63*8);
		}		
		for(i=0;i<8;i++) {
			for(int j=0;j<9;j++) {
				pDC->MoveTo(90+(B_Y[i]-1)*8,50+(m_Limit_Min_X[i][j]-1)*8);
				pDC->LineTo(90+(B_Y[i]-1)*8+(B_Y[i+1]-B_Y[i])*8,50+(m_Limit_Min_X[i][j]-1)*8);
			}
		}

		for( i=0;i<8;i++) {
			for(int j=0;j<9;j++) {
				m_Save_Limit[i][j] = m_Limit_Min_X[i][j];
				m_Save_UpDownLimit_B[i][j] = m_Limit_Min_X[i][j];
			}
		}
		for(i=0;i<9;i++) {
			m_Save_UpDownLimit_B[0][i] = B_Y[i];
			m_Save_UpDownLimit_B[1][i] = B_Y[i];
			m_Save_UpDownLimt_Lenth[0][i] = m_Limit_Min_X[i][1];
			m_Save_UpDownLimt_Lenth[1][i] = m_Limit_Min_X[i][7];
		}		
	}

	pOldPen=NULL;
	ReleaseDC(pDC);
	OnPreSaveLimitFile();      // 保存边界

	AfxMessageBox(" 要想手动编辑位置边界表\n 请重新导入边界文件LimitFile.dat");
}

void CMaintView::OnSaveCurLimit() 
{
	RedrawLimitFile();

	CString FilePathName; 
	CFileDialog dlg(TRUE,"dat","*.dat");///TRUE为OPEN对话框，FALSE为SAVE AS对话框 

	dlg.m_ofn.lpstrFilter = ("保存位置边界文件""*.*");//() 
	if(dlg.DoModal()==IDOK) FilePathName=dlg.GetPathName(); 
	else return;


	CFile tempfile;
	if(!tempfile.Open(FilePathName,CFile::modeRead)) {
			AfxMessageBox("不能进行编辑操作 (没有找到此文件) ");
			return;
	}
	tempfile.Close();

	CFile savefile;
	if( !savefile.Open( FilePathName, CFile::modeReadWrite) ) {			
	}

//	int filelenth = savefile.GetLength();
//	if(filelenth != (128*4 +4)*1024 ) {
//		AfxMessageBox("文件长度不对！保存不成功！");
//		return;
//	}


	unsigned char filehead[128];
	savefile.SeekToBegin();
	filehead[0] = 0x01;
	savefile.Seek(FILEHEAD_PLIMIT_ATTR_ADDR,CFile::begin );	
	savefile.Write(filehead,1);

	savefile.Seek(PARA_PLIMIT_DATA_ADDR,CFile::begin);
	unsigned char temp[4]={0,0,0,0};
	
	for(int block=0;block<8;block++) {
		for(int i=0;i<8;i++) {
			for(int j=0;j<8;j++) {	
				temp[0] = m_8Block_Position_X0[block][i][j];
				temp[1] = m_8Block_Position_Y0[block][i][j];
				temp[2] = m_8Block_Position_X1[block][i][j];
				temp[3] = m_8Block_Position_Y1[block][i][j];
				savefile.Write(temp,4);
			}
		}	
	}
	savefile.Close();

	g_bPLimitChangeValid = false;

}

void CMaintView::OnRedrawLimit() 
{
//	Beep(2000,500);
	if(g_DataSample_Mod_Sel != 0xC7) {
		return;
	}

	m_ShowModelSel = 1;  //
	RedrawLimitFile();
}

void CMaintView::OnLimitStat() 
{
	if(g_DataSample_Mod_Sel != 0xC7) {
		return;
	}
	m_bShowLimitStat = true;  //控制显示 效率统计的
	RedrawLimitFile();
	m_bShowLimitStat = false;  //控制显示 效率统计的

	m_ShowModelSel = 4;	

	m_bEnLimitStat_MemDCRedraw = true;
	OnRedraw();	

}
void CMaintView::OnLimitStat_MemDCRedraw(CDC *pDC)
{
	int X0[8][8],X1[8][8],Y0[8][8],Y1[8][8];
	for(int i=0;i<8;i++) {
			for(int j=0;j<8;j++) {						
				X0[i][j] = m_Position_X0[i][j];
				Y0[i][j] = m_Position_Y0[i][j];
				X1[i][j] = m_Position_X1[i][j];
				Y1[i][j] = m_Position_Y1[i][j];
			}
	}

	long limit_stat[8][8];
	int igoto_one[8][8];

	memset(limit_stat,0,8*8*4);
	for(i=0;i<8;i++) { //纵坐标
		for(int j=0;j<8;j++) { //横坐标
			for(int k=X0[i][j];k<=X1[i][j];k++) {
				for(int m=Y0[i][j];m<=Y1[i][j];m++) {
					limit_stat[i][j] +=  sdot[m][k];					
				}
			}
		}
	}

	int max_stat= limit_stat[0][0];

	for(i=0;i<8;i++) {
		for(int j=0;j<8;j++){
			if(limit_stat[i][j] > max_stat) {
				max_stat = limit_stat[i][j];							
			}
		}
	}
	
	if(max_stat < 1) max_stat = 1;
	for(i=0;i<8;i++) {
		for(int j=0;j<8;j++){
			igoto_one[i][j] = 100* limit_stat[i][j]/max_stat ;
		}
	}
	
//	CDC* pDC=GetDC();
	CString strtext;
	pDC->SetBkColor(RGB(250,250,250));
	pDC->SetTextColor(RGB(105,15,205));

//	   CDC* pDshow=GetDC();	    
	   int y = g_draw_startpoint_x + 27;	 
	   for(int j=0;j<8;j++) {
		    int x = g_draw_startpoint_y+27;
		   	for(int k=0;k<8;k++) {
					CBrush bShow;			
					bShow.CreateSolidBrush(
						RGB(
						GetBValue(ColorSet(limit_stat[j][k],max_stat)), 
						GetGValue(ColorSet(limit_stat[j][k],max_stat)),
						GetRValue(ColorSet(limit_stat[j][k],max_stat))
						)
						);// 30003000					
					CBrush* pOldBrush=pDC->SelectObject(&bShow);// 3000 
					pDC->Rectangle(x-32,y-32,x+32,y+32);// 
					
					pDC->SelectObject(pOldBrush);//++++
					bShow.DeleteObject();

					strtext.Format("%d",igoto_one[j][k]);				
					pDC->TextOut(x-5,y-5,strtext);			
					
					x += g_draw_size*8;
			}
			y += g_draw_size*8;
	   }

//	   ReleaseDC(pDshow);
//	   ReleaseDC(pDC);

}


void CMaintView::OnOpenLimittab() 
{
	CString FilePathName; 
	if (g_bOperating_32bkEffectEvaluate) {
		FilePathName = g_strOperating_32bkEffectFilePath;
	}
	else {
		CFileDialog dlg3(TRUE,"dat","*.dat");///TRUE为OPEN对话框，FALSE为SAVE AS对话框 
		dlg3.m_ofn.lpstrFilter = ("读位置边界文件 ""*.dat");//
		if(dlg3.DoModal()==IDOK) FilePathName=dlg3.GetPathName(); 
		else return;
	}

	CFile file;
	if(!file.Open(FilePathName,CFile::modeRead)) {
		AfxMessageBox("文件读错误");
		return;
	}

//	int filelenth = file.GetLength();
//	if(filelenth != (128*4 +4)*1024 ) {
//		AfxMessageBox("文件长度不对！");
//		return;
//	}

	unsigned char chartemp[128];
//	file.Seek(FILEHEAD_PLIMIT_ATTR_ADDR,CFile::begin );	
//	file.Read(chartemp,1);
//	if(chartemp[0] != 0x01) {
//		AfxMessageBox("文件的位置边界数据无效");
//		return;
//	}

	file.Seek(PARA_PLIMIT_DATA_ADDR,CFile::begin );	
	for(int block=0;block<8;block++) {
			for(int i=0;i<8;i++) {
					for(int j=0;j<8;j++) {
						file.Read(chartemp,4);			
						m_8Block_Position_X0[block][i][j] = (int)chartemp[0];
						m_8Block_Position_Y0[block][i][j] = (int)chartemp[1];
						m_8Block_Position_X1[block][i][j] = (int)chartemp[2];
						m_8Block_Position_Y1[block][i][j] = (int)chartemp[3];
					}
			}
	}
	file.Close();

	m_bSameDriction_X = true;
	for(int i=1;i<7;i++) {
		for(int j=1;j<6;j++) {
			if(m_Position_Y0[i][j] != m_Position_Y0[i][j+1]) {
				m_bSameDriction_X = false; 
			}
		}
	}
//	if(uchfun[0] == 0x10) m_bSameDriction_X = true;  //水平垂直功能判断
//	if(uchfun[0] == 0x20) m_bSameDriction_X = false; // 

	CMainFrame* pMainFrame = (CMainFrame*)GetParentFrame();
	CComboBox *pBlockindex = (CComboBox *)(pMainFrame)->m_wndShowBar.GetDlgItem(IDC_SHOW_BLOCK_SEL);

	int Bindex = pBlockindex->GetCurSel();	
	if((Bindex<0)||(Bindex>7)) Bindex = 0;
	m_8Block_Cur_Sel = Bindex;

	for(i=0;i<8;i++) {
			for(int j=0;j<8;j++) {							
				m_Position_X0[i][j] = m_8Block_Position_X0[m_8Block_Cur_Sel][i][j];
				m_Position_Y0[i][j] = m_8Block_Position_Y0[m_8Block_Cur_Sel][i][j];
				m_Position_X1[i][j] = m_8Block_Position_X1[m_8Block_Cur_Sel][i][j];
				m_Position_Y1[i][j] = m_8Block_Position_Y1[m_8Block_Cur_Sel][i][j];
			}
	}

	RedrawLimitFile();

}

void CMaintView::RedrawLimitFile()
{
	m_bEnLimitLineRedraw = true;
	OnRedraw();

}

void CMaintView::RedrawLimitFile_To_MemDC(CDC *pDC)
{
	CMainFrame* pMainFrame=
		(CMainFrame*)GetParentFrame();
	CComboBox *pBlockindex = (CComboBox *)
		(pMainFrame)->m_wndShowBar.GetDlgItem(IDC_SHOW_BLOCK_SEL);
	
	int Bindex = pBlockindex->GetCurSel();
	
	if (g_bOperating_32bkEffectEvaluate) {
		Bindex = g_iOperatingEffect_block;		
	}
	
	if((Bindex<0)||(Bindex>7)) Bindex = 0;
	
	for(int i=0;i<8;i++) {
		for(int j=0;j<8;j++) {							
			m_8Block_Position_X0[m_8Block_Cur_Sel][i][j] = m_Position_X0[i][j];
			m_8Block_Position_Y0[m_8Block_Cur_Sel][i][j] = m_Position_Y0[i][j];
			m_8Block_Position_X1[m_8Block_Cur_Sel][i][j] = m_Position_X1[i][j];
			m_8Block_Position_Y1[m_8Block_Cur_Sel][i][j] = m_Position_Y1[i][j];
		}
	}
	
	m_8Block_Cur_Sel = Bindex;
	for(i=0;i<8;i++) {
		for(int j=0;j<8;j++) {							
			m_Position_X0[i][j] = m_8Block_Position_X0[m_8Block_Cur_Sel][i][j];
			m_Position_Y0[i][j] = m_8Block_Position_Y0[m_8Block_Cur_Sel][i][j];
			m_Position_X1[i][j] = m_8Block_Position_X1[m_8Block_Cur_Sel][i][j];
			m_Position_Y1[i][j] = m_8Block_Position_Y1[m_8Block_Cur_Sel][i][j];
		}
	}
	
	
	if (g_bOperating_32bkEffectEvaluate) {
		return;
	}
	
	if(m_bShowLimitStat) { //显示效率时 用的变量标志
		return;
	}



	CPen pShow(PS_SOLID,2,RGB(15, 105,185));//RGB(0,210,0)
	CPen* pOldPen=NULL;
	pOldPen=pDC->SelectObject(&pShow); 
	pDC->SetROP2(R2_NOT);
	
/*
	for( i=0;i<8;i++) {
		for(int j=0;j<8;j++) {
			pDC->MoveTo(g_draw_startpoint_y  + m_Position_X0[i][j]*8,g_draw_startpoint_x  + (m_Position_Y0[i][j])*8); 
			if(j<7) pDC->LineTo(g_draw_startpoint_y  + (m_Position_X1[i][j]+1)*8,g_draw_startpoint_x  + m_Position_Y0[i][j]*8);
			else pDC->LineTo(g_draw_startpoint_y  + m_Position_X1[i][j]*8,g_draw_startpoint_x  + m_Position_Y0[i][j]*8);
			
			pDC->MoveTo(g_draw_startpoint_y  + m_Position_X0[i][j]*8,g_draw_startpoint_x  + (m_Position_Y0[i][j])*8);
			if(i<7) pDC->LineTo(g_draw_startpoint_y  + m_Position_X0[i][j]*8,g_draw_startpoint_x  + (m_Position_Y1[i][j]+1)*8);
			else  pDC->LineTo(g_draw_startpoint_y  + m_Position_X0[i][j]*8,g_draw_startpoint_x  + m_Position_Y1[i][j]*8); 
		}
	}	
	pDC->MoveTo(g_draw_startpoint_y  + m_Position_X1[0][7]*8,g_draw_startpoint_x  + (m_Position_Y0[0][7])*8); 
	pDC->LineTo(g_draw_startpoint_y  + m_Position_X1[0][7]*8,g_draw_startpoint_x  + (m_Position_Y1[7][7])*8);
	
	pDC->MoveTo(g_draw_startpoint_y  + m_Position_X0[7][0]*8,g_draw_startpoint_x  + (m_Position_Y1[7][0])*8); 
	pDC->LineTo(g_draw_startpoint_y  + m_Position_X1[7][7]*8,g_draw_startpoint_x  + (m_Position_Y1[7][0])*8);
*/

	int idraw_startpoint_y = g_draw_startpoint_y - 4;
	int idraw_startpoint_x = g_draw_startpoint_x - 4;

	for( i=0;i<8;i++) {
		for(int j=0;j<8;j++) {
			pDC->MoveTo(idraw_startpoint_y  + m_Position_X0[i][j]*8,idraw_startpoint_x  + (m_Position_Y0[i][j])*8); 
			if(j<7) pDC->LineTo(idraw_startpoint_y  + (m_Position_X1[i][j]+1)*8,idraw_startpoint_x  + m_Position_Y0[i][j]*8);
			else pDC->LineTo(idraw_startpoint_y  + m_Position_X1[i][j]*8 + 5,idraw_startpoint_x  + m_Position_Y0[i][j]*8);
			
			pDC->MoveTo(idraw_startpoint_y  + m_Position_X0[i][j]*8,idraw_startpoint_x  + (m_Position_Y0[i][j])*8);
			if(i<7) pDC->LineTo(idraw_startpoint_y  + m_Position_X0[i][j]*8,idraw_startpoint_x  + (m_Position_Y1[i][j]+1)*8);
			else  pDC->LineTo(idraw_startpoint_y  + m_Position_X0[i][j]*8,idraw_startpoint_x  + m_Position_Y1[i][j]*8 + 5); 
		}
	}	
	pDC->MoveTo(idraw_startpoint_y  + m_Position_X1[0][7]*8 + 8,idraw_startpoint_x  + (m_Position_Y0[0][7])*8); 
	pDC->LineTo(idraw_startpoint_y  + m_Position_X1[0][7]*8 + 8,idraw_startpoint_x  + (m_Position_Y1[7][7])*8 + 8);
	
	pDC->MoveTo(idraw_startpoint_y  + m_Position_X0[7][0]*8 ,idraw_startpoint_x  + (m_Position_Y1[7][0])*8 + 8); 
	pDC->LineTo(idraw_startpoint_y  + m_Position_X1[7][7]*8 + 8,idraw_startpoint_x  + (m_Position_Y1[7][0])*8 + 8);
	
	
}



void CMaintView::OnPreSaveLimitFile()
{
	unsigned char filehead[128];

	if(bSetValue_OverLimit) {
		AfxMessageBox("设置的边界值越限 啦");
		return;
	}
	if(bDirectShift_X_Y) { //水平方向
		if((!m_bDraw_Small_X)&&(m_bEnSave_Edit_B_Lab)&&(!m_bMoveUp)&&(!m_bMoveDown)) B_X[m_iEdit_B_Lab] = m_iEdit_B_CurValue;	
	}
	else {
		if((!m_bDraw_Small_Y)&&(m_bEnSave_Edit_B_Lab)&&(m_bMoveUp)&&(!m_bMoveDown)) B_Y[m_iEdit_B_Lab] = m_iEdit_B_CurValue;	
	}	


	if( !fLimitfile.Open( "LimitFile.dat", CFile::modeCreate | CFile::modeWrite) ) {
		AfxMessageBox("保存边界值失败");
		return;
	}

	for(int i=0;i<128;i++) filehead[i] = 0xbb;					
	fLimitfile.Write(filehead,128);				//定制的文件头	

	for(i=0;i<8;i++) {
		for(int j=0;j<8;j++) {
			for(int k=0;k<4;k++) {
				savedata[i][j][k]=0;
			}
		}
	}
	if(bDirectShift_X_Y) { //水平方向   //大部分边界向下偏， 最后的向上偏
		for(i=0;i<7;i++) { //1－7组  前面部分
			savedata[i][0][0] = 0;
			savedata[i][0][1] = (unsigned char)m_Save_UpDownLimit_B[0][i]-1;
			savedata[i][0][2] = m_Limit_Min_Y[i][1]-1-1;
			savedata[i][0][3] = (unsigned char)m_Save_UpDownLimit_B[0][i+1]-1-1;
		}  
		{		//第8组 前面部分
			savedata[7][0][0] = 0;
			savedata[7][0][1] = (unsigned char)m_Save_UpDownLimit_B[0][7]-1;
			savedata[7][0][2] = m_Limit_Min_Y[7][1]-1-1;
			savedata[7][0][3] = (unsigned char)m_Save_UpDownLimit_B[0][8]-1;
		}


		for(i=0;i<7;i++) {   //1-7组的后面部分
			savedata[i][7][0] = m_Limit_Min_Y[i][7]-1;
			savedata[i][7][1] = (unsigned char)m_Save_UpDownLimit_B[1][i]-1;
			savedata[i][7][2] = m_Limit_Min_Y[i][8]-1;
			savedata[i][7][3] = (unsigned char)m_Save_UpDownLimit_B[1][i+1]-1-1;
		}
		{		//8组 的后面部分
			savedata[7][7][0] = m_Limit_Min_Y[7][7]-1;
			savedata[7][7][1] = (unsigned char)m_Save_UpDownLimit_B[1][7]-1;
			savedata[7][7][2] = m_Limit_Min_Y[7][8]-1;
			savedata[7][7][3] = (unsigned char)m_Save_UpDownLimit_B[1][8]-1;
		}

		for(i=0;i<7;i++) {  //中间部分的 1－7组数据
			for(int j=1;j<7;j++) {
				savedata[i][j][0] = m_Limit_Min_Y[i][j]-1;
				savedata[i][j][1] = B_X[i]-1;
				savedata[i][j][2] = m_Limit_Min_Y[i][j+1]-1-1;
				savedata[i][j][3] = B_X[i+1]-1-1;
			}
		}
		{	//中间部分的 第8组数据
			for(int j=1;j<7;j++) {
				savedata[7][j][0] = m_Limit_Min_Y[i][j]-1;
				savedata[7][j][1] = B_X[7]-1;
				savedata[7][j][2] = m_Limit_Min_Y[i][j+1]-1-1;
				savedata[7][j][3] = B_X[8]-1;
			}
		}
		//shui ping over
	}
	else {  //垂直方向             ???????????????????????
		for(i=0;i<7;i++) { //垂直方向1－7组  前面部分
			savedata[0][i][0] = (unsigned char)m_Save_UpDownLimit_B[0][i]-1;//m_Limit_Min_X[i][0]-1
			savedata[0][i][1] = 0;
			savedata[0][i][2] = (unsigned char)m_Save_UpDownLimit_B[0][i+1]-1-1;
			savedata[0][i][3] = m_Limit_Min_X[i][1]-1-1;
		}  
		{		//垂直方向第8组 前面部分
			savedata[0][7][0] = (unsigned char)m_Save_UpDownLimit_B[0][7]-1;
			savedata[0][7][1] = 0;
			savedata[0][7][2] = (unsigned char)m_Save_UpDownLimit_B[0][8]-1;
			savedata[0][7][3] = m_Limit_Min_X[7][1]-1-1;
		}


		for(i=0;i<7;i++) {   //垂直方向1-7组的后面部分
			savedata[7][i][0] = (unsigned char)m_Save_UpDownLimit_B[1][i]-1;
			savedata[7][i][1] = m_Limit_Min_X[i][7]-1;
			savedata[7][i][2] = (unsigned char)m_Save_UpDownLimit_B[1][i+1]-1-1;
			savedata[7][i][3] = m_Limit_Min_X[i][8]-1;
		}
		{		//垂直方向8组 的后面部分
			savedata[7][7][0] = (unsigned char)m_Save_UpDownLimit_B[1][i]-1;
			savedata[7][7][1] = m_Limit_Min_X[i][7]-1;
			savedata[7][7][2] = (unsigned char)m_Save_UpDownLimit_B[1][i+1]-1;
			savedata[7][7][3] = m_Limit_Min_X[i][8]-1;
		}

		for(i=1;i<7;i++) {  //垂直方向中间部分的 1－7组数据
			for(int j=0;j<7;j++) {
				savedata[i][j][0] = B_Y[j]-1;
				savedata[i][j][1] = m_Limit_Min_X[j][i]-1;
				savedata[i][j][2] = B_Y[j+1]-1-1;
				savedata[i][j][3] = m_Limit_Min_X[j][i+1]-1-1;
			}
		}
		{	//垂直方向中间部分的 第8组数据
			for(int j=1;j<7;j++) {
				savedata[j][7][0] = B_Y[7]-1;
				savedata[j][7][1] = m_Limit_Min_X[7][j]-1;
				savedata[j][7][2] = B_Y[8]-1;
				savedata[j][7][3] = m_Limit_Min_X[7][j+1]-1-1;
			}
		}
		//垂直方向 over
	}

	unsigned char arrdata[4];
	for(i=0;i<8;i++) {
		for(int j=0;j<8;j++) {
			for(int k=0;k<4;k++) { 
				arrdata[k] = savedata[i][j][k]; 				
			};

			m_Position_X0[i][j] = (int)arrdata[0];
			m_Position_X1[i][j] = (int)arrdata[1];
			m_Position_Y0[i][j] = (int)arrdata[2];
			m_Position_Y1[i][j] = (int)arrdata[3];

			fLimitfile.Write(arrdata,4);
		}
	}

	fLimitfile.Close();		
	Sleep(10);

	m_bSameDriction_X = true;
	for(i=1;i<7;i++) {
		for(int j=1;j<6;j++) {
			if(m_Position_Y0[i][j] != m_Position_Y0[i][j+1]) {
				m_bSameDriction_X = false; 
			}
		}
	}
}

void CMaintView::OnOpenShiftLimitFile()
{
	CString FilePathName; 
	long datacounter;
	CFile file;
	DWORD filelenth;

	file.Open("LimitFile.dat",CFile::modeRead);
	filelenth = file.GetLength();
	datacounter = filelenth/4;

	if(datacounter < 32+ 8*8) {
		AfxMessageBox("打开文件大小不对，请核实");
		return;
	}

	unsigned char filehead[4];
	file.Read(filehead,4);

	if((filehead[0]==0xBB)&&(filehead[1]==0xBB)) {  //文件头  判断
		unsigned char chartemp[4];
		for(int i=0;i<124;i++) {   // 128 
			file.Read(chartemp,1);
		}
		for(i=0;i<8;i++) {
			for(int j=0;j<8;j++) {
				file.Read(chartemp,4);			
				m_Position_X0[i][j] = (int)chartemp[0];
				m_Position_Y0[i][j] = (int)chartemp[1];
				m_Position_X1[i][j] = (int)chartemp[2];
				m_Position_Y1[i][j] = (int)chartemp[3];
			}
		}
		file.Close();
	}

	m_bSameDriction_X = true;
	for(int i=1;i<7;i++) {
		for(int j=1;j<6;j++) {
			if(m_Position_Y0[i][j] != m_Position_Y0[i][j+1]) {
				m_bSameDriction_X = false; 
			}
		}
	}

	RedrawLimitFile();
}

void CMaintView::OnInitialUpdate() 
{
	CView::OnInitialUpdate();
	
	CMainFrame* pMainFrame =  (CMainFrame*)GetParentFrame();

	CButton* pShowbar_3DRadio = (CButton *)
		  (pMainFrame->m_wndShowBar.GetDlgItem(IDC_SHOWBAR_3D_RADIO));
	CButton* pShowbar_X_Dir = (CButton *)
		  (pMainFrame->m_wndShowBar.GetDlgItem(IDC_2D_SHOW_X));
	CEdit* pShowbar_Offset1 = (CEdit *)
		  (pMainFrame->m_wndShowBar.GetDlgItem(IDC_2D_SHOW_OFFSET1));
	CEdit* pShowbar_Offset2 = (CEdit *)
		  (pMainFrame->m_wndShowBar.GetDlgItem(IDC_2D_SHOW_OFFSET2));
	CButton* pEnMoveWholeLine = (CButton *)
		  (pMainFrame->m_wndShowBar.GetDlgItem(IDC_EN_MOVEWHOLELINE));

	CButton* pShowEnable = (CButton *)
		  (pMainFrame->m_wndShowBar.GetDlgItem(IDC_REDRAW));	

	CButton* pCalculate = (CButton *)
		  (pMainFrame->m_wndShowBar.GetDlgItem(IDC_CALCULATE_LIMIT));

	pCalculate->ShowWindow(false);

	pEnMoveWholeLine->SetCheck(1);
	m_bMoveWholeLine = true;
	m_bMovingVerDirection = true;
	m_bEnMoveBreakLine = false;

	m_bShowLimitStat = false;  // 效率统计 显示用的控制变量

	pShowbar_X_Dir->SetCheck(1);
	m_iValueX_Y = 1;
	pShowbar_3DRadio->SetCheck(1);
	m_iValue3d_2d = 3;  //默认为3D显示方式

	pShowbar_Offset1->SetWindowText("0");
	pShowbar_Offset2->SetWindowText("63");

	m_bEnMidFilter = false; //中值滤波 取消
	m_Midfilter_Old_BlockNo = 0;

	pStatus=(CStatusBar*)
			AfxGetApp()->m_pMainWnd->GetDescendantWindow(ID_VIEW_STATUS_BAR);
	pStatus->SetPaneInfo(0,5,SBPS_NORMAL,230);
	pStatus->SetPaneText(0,"系统启动成功",1);
	pStatus->SetPaneInfo(1,5,SBPS_NORMAL,20);
	pStatus->SetPaneText(1,"＃＃",1);
	pStatus->SetPaneInfo(2,5,SBPS_NORMAL,240);
	pStatus->SetPaneText(2,"相关显示",1);
	pStatus->SetPaneInfo(3,5,SBPS_NORMAL,600);
	pStatus->SetPaneText(3,g_strFileName,1);


	g_hEvtRec = CreateEvent(NULL, TRUE, FALSE, NULL);
	g_hEvtSend = CreateEvent(NULL, TRUE, FALSE, NULL);
	ResetEvent(g_hEvtSend);

	SetTimer(1,2000,NULL);
	m_8Block_Cur_Sel = 0;

	m_bEnLoad32BKComm = true;

	m_b32FileOpenEn = false;

	m_bCalc32BuckEnergeLimit = false;
	m_bCalc32BuckEnergeLimit_smallWidth = false;
	m_bMoving64EnergLimitEn = false;


	m_i64EthPara[0][0] = 25;
	m_i64EthPara[0][1] = 61;
	m_i64EthPara[0][2] = 1;

	m_i64EthPara[1][0] = 14;
	m_i64EthPara[1][1] = 13;
	m_i64EthPara[1][2] = 0;

	m_i64EthPara[2][0] = 14;
	m_i64EthPara[2][1] = 13;
	m_i64EthPara[2][2] = 0;

	m_i64EthPara[3][0] = 12;
	m_i64EthPara[3][1] = 15;
	m_i64EthPara[3][2] = 0;

	m_i64EthPara[4][0] = 5;
	m_i64EthPara[4][1] = 50;
	m_i64EthPara[4][2] = 1;

}

void CMaintView::OnMenuFastopen() 
{
	CMaintDoc*  pDoc = GetDocument();

	CFile file;
	if(!file.Open("recfile.dat",CFile::modeRead)) {
		AfxMessageBox("数据采集的原文件(recfile.dat)没有找到，\n   请重新上载或检查文件路径");
		return;
	}
	unsigned char chartemp[128];

	file.SeekToBegin();

	file.Seek(FILEHEAD_TYPE_ADDR,CFile::begin);  //读取 文件类型
	file.Read(chartemp,2);
	BOOL bSampledfile = false;
	if((chartemp[0] == 0x01)&&(chartemp[1] == 0x01)) {
		bSampledfile = true;  //
	}

	pDoc->bFirstFileOpened = true;
	memset(pDoc->BuffDot3w,0,8*64*64*4);
	
	switch(g_DataSample_Mod_Sel) {
		case 0xc7: 
					file.Seek(FILEHEAD_SDOT_ATTR_ADDR,CFile::begin);
					file.Read(chartemp,1);
					if(chartemp[0] != 0x01) AfxMessageBox("散点数据无效");

					file.Seek(DATA_SDOT_ADDR,CFile::begin);
					g_chFileType = 0xc7;
					break;
		case 0xc5:	
					file.Seek(FILEHEAD_ENERGESPECTRUM_ATTR_ADDR,CFile::begin);
					file.Read(chartemp,1);
					if(chartemp[0] != 0x01) AfxMessageBox("能方数据无效");

					file.Seek(DATA_ESPECT_ADDR,CFile::begin);
					g_chFileType = 0xc5;
					break;
		case 0xc6:  
					file.Seek(FILEHEAD_TIMEMODE_ATTR_ADDR,CFile::begin);
					file.Read(chartemp,1);
					if(chartemp[0] != 0x01) AfxMessageBox("时方数据无效");			

					file.Seek(DATA_TIMEMODE_ADDR,CFile::begin);
					g_chFileType = 0xc6;
					break;
		case 0xc8:	
					file.Seek(FILEHEAD_FUHEMODE_ATTR_ADDR,CFile::begin);
					file.Read(chartemp,1);
					if(chartemp[0] != 0x01) AfxMessageBox("符合数据无效");

					file.Seek(DATA_FUHEMODE_ADDR,CFile::begin);
					g_chFileType = 0xc8;
					break;
		default: g_chFileType = 0x00;
					break;
	}

	if(bSampledfile) {
			for(int i=0;i<8;i++)	{	
				for(int j=0;j<64;j++) {
					for(int k=0;k<64;k++) {
						file.Read(chartemp,2);						
						pDoc->BuffDot3w[i][j][k] = chartemp[1];
						pDoc->BuffDot3w[i][j][k] <<= 8;
						pDoc->BuffDot3w[i][j][k] += chartemp[0];
					}
				}
			}

	}

	file.Close();

	memcpy(g_Backup_iMidfilter_sdot,pDoc->BuffDot3w[g_Blockindex],64*64*4);
	
	if(bSampledfile) {
			pDoc->bFileIsSample = true;
			m_bEnergeSpectOperate_En = false;
			//计算能谱数据

			if(g_DataSample_Mod_Sel == 0xC5) { //能谱文件
				m_bEnergeSpectOperate_En =true;
				memcpy(m_iEnergeSpectrum64,pDoc->BuffDot3w,8*64*64*4);
			}			
			//memset(pDoc->BuffDot3w,0,8*64*64*4);
			OnRedraw();
	}

	g_strBuckFileName = "recfile";
}

void CMaintView::OnSelchangeShowBlockSel() 
{
	CMaintDoc*  pDoc = GetDocument();
	CMainFrame* pMainFrame=(CMainFrame*)GetParentFrame();
	CButton *pBlockindex = (CButton *)
		(pMainFrame)->m_wndShowBar.GetDlgItem(IDC_CHECK_MIDFILTER);

	if(m_bEnMidFilter) { //进行中值滤波
		pBlockindex->SetCheck(0);
		m_bEnMidFilter = false;
		memcpy(pDoc->BuffDot3w[m_Midfilter_Old_BlockNo],g_Backup_iMidfilter_sdot,64*64*4);
	}

	//OnRedraw();	

	if (m_ShowModelSel == 10) {  //将初始化模式转为 3D 刷新模式
		m_ShowModelSel = 0;
	}

	CDC *pdc= GetDC();
	OnDraw(pdc);
	ReleaseDC(pdc);


}


void CMaintView::OnOffset1_Add() 
{
	CString m_strOffset1;
	CMainFrame* pMainFrame=
	      (CMainFrame*)GetParentFrame();
	CComboBox *pBlockindex = (CComboBox *)
		(pMainFrame)->m_wndShowBar.GetDlgItem(IDC_SHOW_BLOCK_SEL);

	CEdit* pLOffsetvalue=(CEdit*)
		(pMainFrame->m_wndShowBar.GetDlgItem(IDC_2D_SHOW_OFFSET1));

	pLOffsetvalue->GetWindowText(m_strOffset1);
	if(!m_strOffset1.IsEmpty())
	   {		
            int ioffset=atol(m_strOffset1);  
			ioffset ++;
			if(ioffset >=64) ioffset = 0;
			CString strtemp;
			strtemp.Format("%d", ioffset);
			pLOffsetvalue->SetWindowText(strtemp);	        
	   }

	m_ShowModelSel = 0;
	OnRedraw();	
}

void CMaintView::OnOffset1_Dec() 
{
	CString m_strOffset1;
	CMainFrame* pMainFrame=
	      (CMainFrame*)GetParentFrame();
	CComboBox *pBlockindex = (CComboBox *)
		(pMainFrame)->m_wndShowBar.GetDlgItem(IDC_SHOW_BLOCK_SEL);

	CEdit* pLOffsetvalue=(CEdit*)
		(pMainFrame->m_wndShowBar.GetDlgItem(IDC_2D_SHOW_OFFSET1));

	pLOffsetvalue->GetWindowText(m_strOffset1);
	if(!m_strOffset1.IsEmpty())
	   {		
            int ioffset=atol(m_strOffset1);  
			ioffset --;
			if((ioffset >=64)||(ioffset<0)) ioffset = 63;
			
			CString strtemp;
			strtemp.Format("%d", ioffset);
			pLOffsetvalue->SetWindowText(strtemp);	        
	   }

	m_ShowModelSel = 0;
	OnRedraw();
	
}

void CMaintView::DocToClient(CPoint &point)
{
	CClientDC dc(this);
	OnPrepareDC(&dc, NULL);
	dc.LPtoDP(&point);

}

void CMaintView::DocToClient(CRect &rect)
{
	CClientDC dc(this);
	OnPrepareDC(&dc, NULL);
	dc.LPtoDP(rect);
	rect.NormalizeRect();
}

void CMaintView::OnDraw64EngerSpectrum()
{
		CMainFrame* pMainFrame= (CMainFrame*)GetParentFrame();
		CComboBox *pBlockindex = (CComboBox *)
			(pMainFrame)->m_wndShowBar.GetDlgItem(IDC_SHOW_BLOCK_SEL);
		int nBlindex= pBlockindex->GetCurSel();

		CDC* pDC=GetDC();

		CRect rcClient;
		GetClientRect(rcClient);

		CDC MemDC; //首先定义一个显示设备对象
		CBitmap MemBitmap;//定义一个位图对象	
		MemDC.CreateCompatibleDC(NULL);
		MemBitmap.CreateCompatibleBitmap(pDC,rcClient.Width(),rcClient.Height());
		CBitmap *pOldBit=MemDC.SelectObject(&MemBitmap);
		
		MemDC.FillSolidRect(0,0,rcClient.Width(),rcClient.Height(),RGB(26,131,162));

		CPen pShow5(PS_SOLID,2,RGB(234,234,234));//RGB(197,207,190)
	    CPen* pOldPen=NULL;
	    pOldPen=MemDC.SelectObject(&pShow5);

		CBrush bShow;			
		bShow.CreateSolidBrush(RGB(234,234,234));// 

		CBrush* pOldBrush=MemDC.SelectObject(&bShow);// 

		int barsize_x = 70;
		int barsize_y = 55;
		int barstart_x = 50;
		int barstart_y = 20;
		for(int i=0;i<8;i++) {
			barstart_x = 50;
			for (int j=0;j<8;j++) {
				MemDC.TextOut(barstart_x-5,barstart_y+barsize_y,"0");
				MemDC.TextOut(barstart_x+barsize_x-15,barstart_y+barsize_y,"63");

				MemDC.Rectangle(barstart_x,barstart_y,barstart_x+barsize_x,barstart_y+barsize_y);//
				barstart_x += 100;	
				
			}
			barstart_y += 80;	
		}
		barsize_x = 70;
		barsize_y = 55;
		barstart_x = 50;
		barstart_y = 20;


		CPen pShow(PS_SOLID,1,RGB(85, 15,185));//RGB(85, 15,185)

		CPen pShowLine(PS_SOLID,1,RGB(251, 36,36));
		pOldPen=NULL;
		pOldPen=MemDC.SelectObject(&pShow);

		int gotooneEnergeSpectrum[64];
		int maxEnergeSpectrum=0;
		int Num=0;
		char textout[10];



		barstart_y = 20;
		for(int Num_y=0;Num_y<8;Num_y++) {
			barstart_x = 50;
			for(int Num_x=0;Num_x<8;Num_x++ ) {

/*				// test add
				int filter3dot[64];
				filter3dot[0] = (m_iEnergeSpectrum64[nBlindex][Num][0] + m_iEnergeSpectrum64[nBlindex][Num][1])/2;
				filter3dot[63] = (m_iEnergeSpectrum64[nBlindex][Num][62] + m_iEnergeSpectrum64[nBlindex][Num][63])/2;
				for(int kkkk=1; kkkk<63; kkkk++) {
					filter3dot[kkkk] = (m_iEnergeSpectrum64[nBlindex][Num][kkkk-1] + m_iEnergeSpectrum64[nBlindex][Num][kkkk] 
						+ m_iEnergeSpectrum64[nBlindex][Num][kkkk+1])/3;
				}
				memcpy(&m_iEnergeSpectrum64[nBlindex][Num],filter3dot,64*4);
				// test add
				
				// test add
				//int filter3dot[64];
				filter3dot[0] = (m_iEnergeSpectrum64[nBlindex][Num][0] + m_iEnergeSpectrum64[nBlindex][Num][1])/2;
				filter3dot[63] = (m_iEnergeSpectrum64[nBlindex][Num][62] + m_iEnergeSpectrum64[nBlindex][Num][63])/2;
				for( kkkk=1; kkkk<63; kkkk++) {
					filter3dot[kkkk] = (m_iEnergeSpectrum64[nBlindex][Num][kkkk-1] + m_iEnergeSpectrum64[nBlindex][Num][kkkk] 
						+ m_iEnergeSpectrum64[nBlindex][Num][kkkk+1])/3;
				}
				memcpy(&m_iEnergeSpectrum64[nBlindex][Num],filter3dot,64*4);
				// test add

				//int filter3dot[64];
				filter3dot[0] = (m_iEnergeSpectrum64[nBlindex][Num][0] + m_iEnergeSpectrum64[nBlindex][Num][1])/2;
				filter3dot[63] = (m_iEnergeSpectrum64[nBlindex][Num][62] + m_iEnergeSpectrum64[nBlindex][Num][63])/2;
				for( kkkk=1; kkkk<63; kkkk++) {
					filter3dot[kkkk] = (m_iEnergeSpectrum64[nBlindex][Num][kkkk-1] + m_iEnergeSpectrum64[nBlindex][Num][kkkk] 
						+ m_iEnergeSpectrum64[nBlindex][Num][kkkk+1])/3;
				}
				memcpy(&m_iEnergeSpectrum64[nBlindex][Num],filter3dot,64*4);
				// test add
				//MessageBeep(100);     
*/				


				maxEnergeSpectrum=0;
				for(i=0;i<63;i++) { //64
					if(m_iEnergeSpectrum64[nBlindex][Num][i] > maxEnergeSpectrum)  maxEnergeSpectrum = m_iEnergeSpectrum64[nBlindex][Num][i];
				}
				if (maxEnergeSpectrum<1) maxEnergeSpectrum = 1;		
				for(i=0;i<64;i++) {
					gotooneEnergeSpectrum[i] =  (50 * (float)m_iEnergeSpectrum64[nBlindex][Num][i]) / maxEnergeSpectrum;
					if(gotooneEnergeSpectrum[i] > 54) gotooneEnergeSpectrum[i] = 54;
				}
				sprintf(textout,"%.3d",maxEnergeSpectrum);
				MemDC.TextOut(barstart_x-25,barstart_y,textout);
				sprintf(textout,"(%.2d)",Num);
				MemDC.TextOut(barstart_x+25,barstart_y-17,textout);
				for(i=0;i<63;i++) {
					MemDC.MoveTo(barstart_x+i,  barstart_y+55 - gotooneEnergeSpectrum[i]);
					MemDC.LineTo(barstart_x+1+i,barstart_y+55 - gotooneEnergeSpectrum[i+1]);					
				}
				
				//画 能量边界
				pOldPen=MemDC.SelectObject(&pShowLine);
				MemDC.MoveTo(barstart_x+m_iEnergeSpectEth_Low[m_iEnergeSpectCurBlockNo][Num], barstart_y+55 );
				MemDC.LineTo(barstart_x+m_iEnergeSpectEth_Low[m_iEnergeSpectCurBlockNo][Num],barstart_y+55 - 40);
				
				MemDC.MoveTo(barstart_x+m_iEnergeSpectEth_Hight[m_iEnergeSpectCurBlockNo][Num], barstart_y+55 );
				MemDC.LineTo(barstart_x+m_iEnergeSpectEth_Hight[m_iEnergeSpectCurBlockNo][Num],barstart_y+55 - 40);

				pOldPen=MemDC.SelectObject(&pShow);
				//

				barstart_x += 100;
				Num++;
			}
			barstart_y += 80;	
		}

		//将内存中的图拷贝到屏幕上进行显示
		pDC->BitBlt(0,0,rcClient.Width(),rcClient.Height(),&MemDC,0,0,SRCCOPY);	   
		//绘图完成后的清理
		MemBitmap.DeleteObject();
		MemDC.DeleteDC();
		ReleaseDC(pDC);

//		ReleaseDC(pDC);		
}

void CMaintView::OnDrawZoomInEnergeSpect(int nblock,int num)
{
		m_bEnergeSpectSelEn = false;
		m_bEnergeSpectZoomInMode = true;

		int	longtdata[64];
		for(int nn=0;nn<64;nn++) {
			longtdata[nn] = m_iEnergeSpectrum64[nblock][num][nn];
		}	

		int maxtemp = longtdata[0];
		for(int n=0;n<63;n++) {
			if(longtdata[n] >= maxtemp) maxtemp = longtdata[n];					
		} 

		CDC* pDC=GetDC();
		COLORREF rf=RGB(26,131,162);//RGB(26,131,162); 
		CRect rcClient;
		GetClientRect(&rcClient);
		pDC->FillSolidRect(&rcClient,rf);
		pDC->SetTextColor(RGB(255,255,0));// RGB(255,255,0)

		CPen pShow(PS_SOLID,4,RGB(202,95,204));//RGB(200,200,0)
		CPen* pOldPen=NULL;
		pOldPen=pDC->SelectObject(&pShow);

		char chtext[50];
		sprintf(chtext,"第 %d 块第 %d 条 能谱曲线图", nblock,num);
		pDC->TextOut(220,10,chtext);

		pDC->MoveTo(g_draw_startpoint_y,g_draw_startpoint_x); 
		pDC->LineTo(g_draw_startpoint_y,g_draw_size*64+g_draw_startpoint_x);
		pDC->MoveTo(g_draw_startpoint_y,g_draw_startpoint_x);
		pDC->LineTo(g_draw_startpoint_y-3,g_draw_startpoint_x+5);

	    pDC->MoveTo(g_draw_startpoint_y,g_draw_size*64+g_draw_startpoint_x);
		pDC->LineTo(g_draw_startpoint_y +g_draw_size*64 ,g_draw_size*64+g_draw_startpoint_x);
	    pDC->MoveTo(g_draw_startpoint_y +g_draw_size*64,g_draw_size*64+g_draw_startpoint_x);
        pDC->LineTo(g_draw_startpoint_y +g_draw_size*64 - 5,g_draw_size*64+g_draw_startpoint_x +3);

		CString strshowMax;
	    pDC->SetTextColor(RGB(255,255,0));
	    pDC->TextOut(40,40,"阈值");
		strshowMax.Format("%d ",maxtemp);
//		pDC->SetTextColor(RGB(255,128,128));
	    pDC->TextOut(40,55,strshowMax);

	    pDC->TextOut(40+30,g_draw_size*64+g_draw_startpoint_x+5,"(0,0)");
	    pDC->TextOut(g_draw_startpoint_y +g_draw_size*64 - 10,g_draw_size*64+g_draw_startpoint_x +5,"63");

//		ReleaseDC(pDC);

		int x = g_draw_startpoint_y;
	//	CDC* pDC=GetDC(); //画二维 直线
		CPen pShow2(PS_SOLID,2,RGB(254,226,245));//   RGB(85, 15,185)
		pOldPen=pDC->SelectObject(&pShow2);
		x = g_draw_startpoint_y+8 ; 
		if(maxtemp <1) maxtemp = 1;
		int tplace = g_draw_startpoint_x + 64*g_draw_size- (int)(g_draw_size*64*longtdata[0]/(maxtemp));
		if(tplace < 30) tplace = 30;	
		for(int kk=1;kk<64;kk++) {																				
			pDC->MoveTo(x-8,tplace);
			x += g_draw_size;
			tplace = g_draw_size*64 + g_draw_startpoint_x - (int)(g_draw_size*64*longtdata[kk]/(maxtemp));
			if(tplace > g_draw_size*64 + g_draw_startpoint_x) tplace = g_draw_size*64;
			if(maxtemp < 1) maxtemp = 1;
			if(tplace < 30) tplace = 30;			
			pDC->LineTo(x-8,tplace);
		}


		CPen pShow3(PS_SOLID,2,RGB(255,10,,0));//RGB(0,210,0)
	    pOldPen=pDC->SelectObject(&pShow3);

		if(m_iEnergeSpectCurSynNo > 63) m_iEnergeSpectCurSynNo = 63;
		if(m_iEnergeSpectCurSynNo < 0) m_iEnergeSpectCurSynNo = 0;

		pDC->MoveTo(g_draw_startpoint_y + g_draw_size*m_iEnergeSpectEth_Low[m_iEnergeSpectCurBlockNo][m_iEnergeSpectCurSynNo], g_draw_size*64+g_draw_startpoint_x);
		pDC->LineTo(g_draw_startpoint_y + g_draw_size*m_iEnergeSpectEth_Low[m_iEnergeSpectCurBlockNo][m_iEnergeSpectCurSynNo], 400);
		pDC->MoveTo(g_draw_startpoint_y +g_draw_size*m_iEnergeSpectEth_Hight[m_iEnergeSpectCurBlockNo][m_iEnergeSpectCurSynNo], g_draw_size*64+g_draw_startpoint_x);
		pDC->LineTo(g_draw_startpoint_y +g_draw_size*m_iEnergeSpectEth_Hight[m_iEnergeSpectCurBlockNo][m_iEnergeSpectCurSynNo], 400);

		ReleaseDC(pDC);				

}

void CMaintView::OnRButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if(!m_bEnergeSpectOperate_En) return;

	CPoint ScreenPoint;   //计算在整个屏幕上的位置,否则点的位置是指用户区域的位置
    ScreenPoint = point;
    ClientToScreen(&ScreenPoint);

	UINT CommandId = 0;
    CMenu MyContextMenu;
    MyContextMenu.CreatePopupMenu();
    MyContextMenu.AppendMenu(MF_STRING, ++CommandId, "显示64个能谱");
    MyContextMenu.AppendMenu(MF_STRING, ++CommandId, "正常3D显示");
	MyContextMenu.AppendMenu(MF_STRING, ++CommandId, "导入能谱边界");
    MyContextMenu.AppendMenu(MF_STRING, ++CommandId, "保存能谱边界");
	MyContextMenu.AppendMenu(MF_STRING, ++CommandId, "计算1个bucket能谱边界");

    MyContextMenu.AppendMenu(MF_STRING, ID_SEPARATOR); 
	MyContextMenu.AppendMenu(MF_STRING, ++CommandId, "峰位统计");
//	MyContextMenu.AppendMenu(MF_STRING, ++CommandId, "32Bucket峰位统计");
	MyContextMenu.AppendMenu(MF_STRING, ID_SEPARATOR); 
    MyContextMenu.AppendMenu(MF_STRING, ++CommandId, "计算/保存32个Bucket能谱边界");
	MyContextMenu.AppendMenu(MF_STRING, ID_SEPARATOR);
    MyContextMenu.AppendMenu(MF_STRING, ++CommandId, "32个Bucket能谱边界 参数设置");
	//... (Append as many commands as you like)
    //指定菜单的类型.
    UINT Command = (UINT)MyContextMenu.TrackPopupMenu(TPM_LEFTALIGN|TPM_NONOTIFY | TPM_RETURNCMD,
		ScreenPoint.x, ScreenPoint.y, this);
    //使用TPM_NONOTIFY | TPM_RETURNCMD返回被选择的菜单命令,0代表没有(比如
    //点击菜单外面)
    //如果你不加入这两个类型,你的父窗口将会从你选的命令中收到WM_COMMAND消息

	MyContextMenu.DestroyMenu();
    //switch is only necessary if you use TPM_NONOTIFY | TPM_RETURNCMD !
    //switch只在你使用两种类型时才有必要.
//	CMaintDoc*  pDoc = GetDocument();  
	switch(Command)
    {
		case 0:		
			//没有命令,所以不必做任何事情.			
			break;
		case 1:	//显示64个能谱
			m_Show64EngerSpectrum();
			break;
		case 2:	//正常显示
			m_ShowModelSel = 0;	
			OnRedraw();
			m_bEnergeSpectSelEn = false;
			m_bEnergeSpectZoomInMode =false;			
			break;
		case 3:		
			OnLoadEthFile();
			OnDraw64EngerSpectrum();
			m_bEnergeSpectOperate_En = true;
			m_bEnergeSpectSelEn = true;
			m_bEnergeSpectZoomInMode =false;
			break;
		case 4:			
			OnSaveEthFile();	//保存能谱文件
			break;
		case 5:
			OnFastCalcEthLowHighLimit();	//快速计算1个Bucket能谱上下阈		
			break;
		case 6:
			OnPeakPositionStat();	//风味统计！	
			break;
		case 7:			
			//AfxMessageBox(" XX编写，以后再说吧！");
			OnFastCalcEthLimit32Buck();
			break;
/*		case 8:
			m_bCalc32BuckEnergeLimit_smallWidth = true;
			OnFastCalcEthLimit32Buck();
			m_bCalc32BuckEnergeLimit_smallWidth = false;
			break;
*/
		case 8: //32bucket 能谱边界参数设置
			OnSet32EnergeLimitPara();
			break;
		default:					
			break;
	}
	
	CView::OnRButtonDown(nFlags, point);
}

void CMaintView::OnSaveEthFile()
{

	CString FilePathName; 
	CFileDialog dlg(TRUE);///TRUE为OPEN对话框，FALSE为SAVE AS对话框 

	dlg.m_ofn.lpstrFilter = ("保存能阈边界文件""*.*");//() 
	if(dlg.DoModal()==IDOK) FilePathName=dlg.GetPathName(); 
	else return;

	CFile tempfile;
	if(!tempfile.Open(FilePathName,CFile::modeRead)) {
			AfxMessageBox("不能进行编辑操作 (没有找到此文件) ");
			return;
	}
	tempfile.Close();

	CFile savefile;
	if( !savefile.Open( FilePathName, CFile::modeReadWrite) ) {			
	}

//	int filelenth = savefile.GetLength();
//	if(filelenth != (128*4 +4)*1024 ) {
//		AfxMessageBox("文件长度不对！保存不成功！");
//		return;
//	}


	unsigned char filehead[128];
	savefile.SeekToBegin();
	filehead[0] = 0x01;
	savefile.Seek(FILEHEAD_ELIMIT_ATTR_ADDR,CFile::begin );	
	savefile.Write(filehead,1);

	savefile.Seek(PARA_ETHLIMIT_DATA_ADDR,CFile::begin);
	unsigned char temp[4]={0,0,0,0};
	
	for(int block=0;block<8;block++) {
		for(int i=0;i<64;i++) {	
			if(m_iEnergeSpectEth_Low[block][i] > m_iEnergeSpectEth_Hight[block][i])	{
				temp[1] = (unsigned char)m_iEnergeSpectEth_Low[block][i];
				temp[0] = (unsigned char)m_iEnergeSpectEth_Hight[block][i];
			} 
			else {
				temp[0] = (unsigned char)m_iEnergeSpectEth_Low[block][i];
				temp[1] = (unsigned char)m_iEnergeSpectEth_Hight[block][i];
			}		
			
			savefile.Write(temp,2);				
		}	
	}
	savefile.Close();	

}

void CMaintView::OnLoadEthFile()
{
	CString FilePathName; 
	CFileDialog dlg(TRUE,"dat","*.dat");///TRUE为OPEN对话框，FALSE为SAVE AS对话框 

	dlg.m_ofn.lpstrFilter = ("能量边界导入.dat");//
	if(dlg.DoModal()==IDOK) FilePathName=dlg.GetPathName(); 
	else return;
	CFile file;
	if(!file.Open(FilePathName,CFile::modeRead)) {
		AfxMessageBox("文件读错误");
		return;
	}
//	int filelenth = file.GetLength();
//	if(filelenth != (128*4 +4)*1024 ) {
//		AfxMessageBox("文件长度不对！");
//		return;
//	}

	unsigned char chartemp[128];
	file.Seek(FILEHEAD_ELIMIT_ATTR_ADDR,CFile::begin );	
	file.Read(chartemp,1);
	if(chartemp[0] != 0x01) {
		AfxMessageBox("文件的能量边界数据无效");
		return;
	}

	file.Seek(PARA_ETHLIMIT_DATA_ADDR,CFile::begin );	

	for(int block=0;block<8;block++) {
		for(int i=0;i<64;i++) {				
			file.Read(chartemp,2);			
			m_iEnergeSpectEth_Low[block][i] = chartemp[0];
			m_iEnergeSpectEth_Hight[block][i] = chartemp[1];				
		}
	}

	file.Close();


}

void CMaintView::OnFastCalcEthLowHighLimit()
{
	int maxEth64 = 10;
	int maxnum = 5;
	int maxpeakvalue = 10;
	int maxpeaknum = 5;

	int persent50;
	BOOL lowflag = true;
	int filter3dot[64];

	for(int block=0;block<8;block++) {  
		for(int num=0;num<64;num++) {    // 计算 64 晶体条
			maxnum = 5;
			maxpeaknum = 5;
			maxEth64 = 10;
			lowflag = true;
			//三点平滑
			filter3dot[0] = (m_iEnergeSpectrum64[block][num][0] + m_iEnergeSpectrum64[block][num][1])/2;
			filter3dot[63] = (m_iEnergeSpectrum64[block][num][62] + m_iEnergeSpectrum64[block][num][63])/2;
			for(int kkkk=1; kkkk<63; kkkk++) {
				filter3dot[kkkk] = (m_iEnergeSpectrum64[block][num][kkkk-1] + m_iEnergeSpectrum64[block][num][kkkk] 
					+ m_iEnergeSpectrum64[block][num][kkkk+1])/3;
			}

/*			// test add  能谱滤波测试  三点滤波
			int filter3buff[64];
			filter3buff[0] = (filter3dot[0] + filter3dot[1])/2;
			filter3buff[63] = (filter3dot[62] + filter3dot[63])/2;
			for( kkkk=1; kkkk<63; kkkk++) {
				filter3buff[kkkk] = (filter3dot[kkkk-1] + filter3dot[kkkk] 
					+ filter3dot[kkkk+1])/3;
			}
			memcpy(&filter3dot,filter3buff,64*4);
			
			// test add			 三点滤波
			filter3buff[0] = (filter3dot[0] + filter3dot[1])/2;
			filter3buff[63] = (filter3dot[62] + filter3dot[63])/2;
			for( kkkk=1; kkkk<63; kkkk++) {
				filter3buff[kkkk] = (filter3dot[kkkk-1] + filter3dot[kkkk] 
					+ filter3dot[kkkk+1])/3;
			}
			memcpy(&filter3dot,filter3buff,64*4);
			// test add
*/
			// 找峰位的 最大值 
			maxEth64 = 10;
			for(int i=0;i<64;i++) { //64
				if(filter3dot[i] > maxEth64)  {
					maxEth64 = filter3dot[i];
					persent50 = (int)(maxEth64*5/10);
					maxnum = i;
				}
			}

			if((block == 2)&&(num == 6)) {
				i = 61;
			}
   
			//找峰值 峰位  极大值
			i = 61;
			maxpeakvalue = 10;
			while (i > 5) {
				if((filter3dot[i] >= filter3dot[i-1])&&(filter3dot[i] > filter3dot[i+1])&& (filter3dot[i] > persent50)) {
					if((filter3dot[i] >= filter3dot[i-2])&&(filter3dot[i] > filter3dot[i+2])) {
						if((filter3dot[i] >= filter3dot[i-3])&&(filter3dot[i] > filter3dot[i+3])) {
							if((filter3dot[i] >= filter3dot[i-4])&&(filter3dot[i] > filter3dot[i+4])) {
								if((filter3dot[i] >= filter3dot[i-5])&&(filter3dot[i] > filter3dot[i+5])) {
									maxpeakvalue = filter3dot[i];
									maxpeaknum = i;

									maxnum = i;    // akang 
									break;
								}
							}
						}
					}
				}
				i --;  
			}


			//对 晶体条编号进行 “编环”
			int xx = num % 8,yy = num/8;
			int ring = 0;
			//一环
			if( ((xx >= 3)&&(xx <= 4)) && ((yy<=4)&&(yy >= 3)) ) {
				ring = 0;
			}

			//2环
			if((xx == 2)&& ((yy>=2)&&(yy<= 5))) {
				ring = 1;
			}
			if((xx == 5)&& ((yy>=2)&&(yy<= 5))) {
				ring = 1;
			}
			if((yy == 2)&& ((xx>=2)&&(xx<= 5))) {
				ring = 1;
			}
			if((yy == 5)&& ((xx>=2)&&(xx<= 5))) {
				ring = 1;
			}

			//三环
			if((xx == 1)&& ((yy>=1)&&(yy<= 6))) {
				ring = 2;
			}
			if((xx == 6)&& ((yy>=1)&&(yy<= 6))) {
				ring = 2;
			}
			if((yy == 1)&& ((xx>=1)&&(xx<= 6))) {
				ring = 2;
			}
			if((yy == 6)&& ((xx>=1)&&(xx<= 6))) {
				ring = 2;
			}	
			
			//四环
			if((xx == 0) ||(xx == 7)){
				ring = 3;
			}
			if((yy == 0) || (yy == 7)) {
				ring = 3;
			}

			//四边角
			if ((xx == 0) &&(yy == 0)) {
				ring = 4;
			}
			if ((xx == 0) &&(yy == 7)) {
				ring = 4;
			}
			if ((xx == 7) &&(yy == 0)) {
				ring = 4;
			}
			if ((xx == 7) &&(yy == 7)) {
				ring = 4;
			}


			// 按照峰位 的百分比来 确定能谱 上下边界 （下：32％，  上：27％）
			if(m_i64EthPara[5][2] == 1) {	// 允许 百分比计算
				if(maxnum >= 60) {
					m_iEnergeSpectEth_Low[block][num] = ( (float)maxnum * (1 - 0.32) );     //
					m_iEnergeSpectEth_Hight[block][num] = 62; 
				}
				if(maxnum <= 5) {
					m_iEnergeSpectEth_Low[block][num] =  5;    //
					m_iEnergeSpectEth_Hight[block][num] = ( (float)maxnum * (1 + 0.27) + 0.5); 
				}

				if((maxnum > 5)&&( maxnum <60)) {
					m_iEnergeSpectEth_Low[block][num] =  ( (float)maxnum * (1 - 0.32) ); 
					m_iEnergeSpectEth_Hight[block][num] = ( (float)maxnum * (1 + 0.27) + 0.5); 
				}
				if(m_iEnergeSpectEth_Hight[block][num] > 62) {
					m_iEnergeSpectEth_Hight[block][num] = 62;
				}
			}
			else {	//  允许 按照 固定宽度 进行能谱上下限设置 				 	
				if((maxnum >= m_i64EthPara[ring][0] + 5)&&(maxnum <= 61 - m_i64EthPara[ring][1])) {
					m_iEnergeSpectEth_Low[block][num] = maxnum - m_i64EthPara[ring][0];     //
					m_iEnergeSpectEth_Hight[block][num] = maxnum + m_i64EthPara[ring][1];    //
				}
				else {
					if(maxnum < m_i64EthPara[ring][0] + 5) {
						m_iEnergeSpectEth_Low[block][num] = 5;
						m_iEnergeSpectEth_Hight[block][num] = maxnum + m_i64EthPara[ring][1];
						if (m_iEnergeSpectEth_Hight[block][num] >= 62) {
							m_iEnergeSpectEth_Hight[block][num] = 62;
						}
					};
					if(maxnum > 61 - m_i64EthPara[ring][1]) { 
						m_iEnergeSpectEth_Low[block][num] = maxnum - m_i64EthPara[ring][0];
						m_iEnergeSpectEth_Hight[block][num] = 62; 

						if (m_iEnergeSpectEth_Low[block][num] <= 5) {
							m_iEnergeSpectEth_Low[block][num] = 5;
						}
					};
				}
			} // end if 


		} //64 晶体条计算 end  

		// 如果计算能谱的 环线 被设置成 定值， 进行定值处理！！！
		if(m_i64EthPara[0][2] == 1) {//环一	 定值设置		
			m_iEnergeSpectEth_Low[block][27] = m_i64EthPara[0][0];
			m_iEnergeSpectEth_Hight[block][27] = m_i64EthPara[0][1]; 
			m_iEnergeSpectEth_Low[block][28] = m_i64EthPara[0][0];
			m_iEnergeSpectEth_Hight[block][28] = m_i64EthPara[0][1]; 
			m_iEnergeSpectEth_Low[block][35] = m_i64EthPara[0][0];
			m_iEnergeSpectEth_Hight[block][35] = m_i64EthPara[0][1]; 
			m_iEnergeSpectEth_Low[block][36] = m_i64EthPara[0][0];
			m_iEnergeSpectEth_Hight[block][36] = m_i64EthPara[0][1]; 
		}

		if(m_i64EthPara[1][2] == 1) {//环二  定值设置
			for(int ii=2;ii<=5;ii++) {
				m_iEnergeSpectEth_Low[block][2+ ii * 8] = m_i64EthPara[1][0];
				m_iEnergeSpectEth_Hight[block][2+ ii * 8] = m_i64EthPara[1][1]; 
			}

			for( ii=2;ii<=5;ii++) {
				m_iEnergeSpectEth_Low[block][5+ ii * 8] = m_i64EthPara[1][0];
				m_iEnergeSpectEth_Hight[block][5+ ii * 8] = m_i64EthPara[1][1]; 
			}

			for( ii=2;ii<=5;ii++) {
				m_iEnergeSpectEth_Low[block][ii+ 2 * 8] = m_i64EthPara[1][0];
				m_iEnergeSpectEth_Hight[block][ii+ 2 * 8] = m_i64EthPara[1][1]; 
			}

			for( ii=2;ii<=5;ii++) {
				m_iEnergeSpectEth_Low[block][ii+ 5 * 8] = m_i64EthPara[1][0];
				m_iEnergeSpectEth_Hight[block][ii+ 5 * 8] = m_i64EthPara[1][1]; 
			}
		}

		if(m_i64EthPara[2][2] == 1) {//环3  定值设置
			for(int ii=1;ii<=6;ii++) {
				m_iEnergeSpectEth_Low[block][1+ ii * 8] = m_i64EthPara[2][0];
				m_iEnergeSpectEth_Hight[block][1+ ii * 8] = m_i64EthPara[2][1]; 
			}
			for( ii=1;ii<=6;ii++) {
				m_iEnergeSpectEth_Low[block][6+ ii * 8] = m_i64EthPara[2][0];
				m_iEnergeSpectEth_Hight[block][6+ ii * 8] = m_i64EthPara[2][1]; 
			}

			for( ii=1;ii<=6;ii++) {
				m_iEnergeSpectEth_Low[block][ii+ 1 * 8] = m_i64EthPara[2][0];
				m_iEnergeSpectEth_Hight[block][ii+ 1 * 8] = m_i64EthPara[2][1]; 
			}
			for( ii=1;ii<=6;ii++) {
				m_iEnergeSpectEth_Low[block][ii + 6 * 8] = m_i64EthPara[2][0];
				m_iEnergeSpectEth_Hight[block][ii+ 6 * 8] = m_i64EthPara[2][1]; 
			}
		}

		if(m_i64EthPara[3][2] == 1) {//环4  定值设置
			for(int ii=1;ii<=6;ii++) {
				m_iEnergeSpectEth_Low[block][0+ ii * 8] = m_i64EthPara[3][0];
				m_iEnergeSpectEth_Hight[block][0+ ii * 8] = m_i64EthPara[3][1]; 
			}
			for( ii=1;ii<=6;ii++) {
				m_iEnergeSpectEth_Low[block][7+ ii * 8] = m_i64EthPara[3][0];
				m_iEnergeSpectEth_Hight[block][7+ ii * 8] = m_i64EthPara[3][1]; 
			}

			for( ii=1;ii<=6;ii++) {
				m_iEnergeSpectEth_Low[block][ii+ 0 * 8] = m_i64EthPara[3][0];
				m_iEnergeSpectEth_Hight[block][ii+ 0 * 8] = m_i64EthPara[3][1]; 
			}
			for( ii=1;ii<=6;ii++) {
				m_iEnergeSpectEth_Low[block][ii + 7 * 8] = m_i64EthPara[3][0];
				m_iEnergeSpectEth_Hight[block][ii+ 7 * 8] = m_i64EthPara[3][1]; 
			}
		}

		if(m_i64EthPara[4][2] == 1) {//四边角  定值设置
			m_iEnergeSpectEth_Low[block][0] = m_i64EthPara[4][0];
			m_iEnergeSpectEth_Hight[block][0] = m_i64EthPara[4][1]; 

			m_iEnergeSpectEth_Low[block][7] = m_i64EthPara[4][0];
			m_iEnergeSpectEth_Hight[block][7] = m_i64EthPara[4][1]; 

			m_iEnergeSpectEth_Low[block][56] = m_i64EthPara[4][0];
			m_iEnergeSpectEth_Hight[block][56] = m_i64EthPara[4][1]; 

			m_iEnergeSpectEth_Low[block][63] = m_i64EthPara[4][0];
			m_iEnergeSpectEth_Hight[block][63] = m_i64EthPara[4][1]; 
		}
	
	
	} // 完成一个 Bucket 的能谱边界计算
	

	if (m_bCalc32BuckEnergeLimit) {
		return;
	}

	//显示64个能谱
	CMaintDoc*  pDoc = GetDocument();  
	m_bEnergeSpectOperate_En = true;
	memcpy(m_iEnergeSpectrum64,pDoc->BuffDot3w,8*64*64*4);
	OnDraw64EngerSpectrum();
	m_bEnergeSpectSelEn = true;
	m_bEnergeSpectZoomInMode =false;

	AfxMessageBox("完成 8 Block 的能谱边界计算!");

}

void CMaintView::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default

	if(g_SampleMaxTime <= 0) return; 

	CString strtext;
	if((++g_SamplingTime) > g_SampleMaxTime) {
		strtext.Format("(未发停止命令)PC机已经完成设定的采样时间：\n   %d分 %d 秒", (int)(2*g_SampleMaxTime/60),((g_SampleMaxTime*2)%60));
		g_SampleMaxTime = 0;
	}
	else {
		strtext.Format("已经采样时间：\n   %d分 %d 秒", (int)(2*g_SamplingTime/60),((g_SamplingTime*2)%60));
	}

	switch(nIDEvent) {
		case 0: 
			break;
		case 1:
			pStatus->SetPaneText(2,strtext,1);
			break;
		case 2:
			break;
		default:
			break;
	}
	CView::OnTimer(nIDEvent);
}

void CMaintView::OnMenuDatasample() 
{
	datasample.DoModal();
	return;	
}

void CMaintView::OnMenuAnol() 
{	
	anol.DoModal();
	return;	
}

void CMaintView::OnEnMovewholeline() 
{
	m_bMoveWholeLine = (!m_bMoveWholeLine);	
}

void CMaintView::OnPeakValeStat() 
{
	if(g_DataSample_Mod_Sel != 0xC7) {
		return;
	}
	m_bShowLimitStat = true;  //控制显示 效率统计的
	RedrawLimitFile();
	m_bShowLimitStat = false;  //控制显示 效率统计的


	m_ShowModelSel = 3;	
	m_bEnPeakValeStat_MemDCRedraw = true;  //再刷分古比
	OnRedraw();
	
}

void CMaintView::OnPeakValeStat_MemDCRedraw(CDC *pDC)
{
	
	CMaintDoc*  pDoc = GetDocument();
	CMainFrame* pMainFrame=
	      (CMainFrame*)GetParentFrame();
	CComboBox *pBlockindex = (CComboBox *)
		(pMainFrame)->m_wndShowBar.GetDlgItem(IDC_SHOW_BLOCK_SEL);
	CEdit* pThPeakVale=(CEdit*)
		(pMainFrame->m_wndShowBar.GetDlgItem(IDC_PEAKVALE_ETH));

	int nindex = pBlockindex->GetCurSel();
	float iPeakValue;  //akang
	CString strPeakvalue;
	pThPeakVale->GetWindowText(strPeakvalue);
	if(!strPeakvalue.IsEmpty()) {		
            iPeakValue=atof(strPeakvalue);        
	        pThPeakVale->Invalidate();
	}
	else iPeakValue = 0;


	//计算；
	if((nindex>7)||(nindex<0)) nindex = 0;
	memcpy(peakvaleratio.iSdot,pDoc->BuffDot3w[nindex],64*64*4);

	memcpy(peakvaleratio.Position_X0,m_Position_X0,8*8*4);
	memcpy(peakvaleratio.Position_Y0,m_Position_Y0,8*8*4);
	memcpy(peakvaleratio.Position_X1,m_Position_X1,8*8*4);
	memcpy(peakvaleratio.Position_Y1,m_Position_Y1,8*8*4);

	peakvaleratio.CalPeakValeRatio();


	//-----------------按阈值统计
	int showstatnum[8][8];
	memset(showstatnum,0,8*8*4);
	for(int i=0;i<8;i++) {	
		for(int j=0;j<8;j++) {
			for(int k=0;k<8;k++) {
				if(peakvaleratio.fPeakVale_Ratio[i][j][k] < iPeakValue) {
					if(peakvaleratio.fPeakVale_Ratio[i][j][k] != -1) {
						showstatnum[i][j] ++;
					}
				}
			}
		}
	}

//	CDC* pDC=GetDC();

	CString strtext;
	pDC->SetBkColor(RGB(250,250,250));
	pDC->SetTextColor(RGB(105,15,205));
	  // CDC* pDshow=GetDC();	    
	   int y = g_draw_startpoint_x + 27;
	   for(int j=0;j<8;j++) {
		    int x = g_draw_startpoint_y+27;
		   	for(int k=0;k<8;k++) {
					CBrush bShow;			
					bShow.CreateSolidBrush(
						RGB(
						GetBValue(ColorSet(showstatnum[j][k]*10,100)), //颜色显示
						GetGValue(ColorSet(showstatnum[j][k]*10,100)),
						GetRValue(ColorSet(showstatnum[j][k]*10,100))
						)
						);					
					CBrush* pOldBrush=pDC->SelectObject(&bShow);
					pDC->Rectangle(x-32,y-32,x+32,y+32);
					
					pDC->SelectObject(pOldBrush);
					bShow.DeleteObject();

					strtext.Format("%d",showstatnum[j][k]);	//数值显示		
					pDC->TextOut(x-5,y-5,strtext);			
					
					x += g_draw_size*8;
			}
			y += g_draw_size*8;
	   }

	CBrush bShow;			
	bShow.CreateSolidBrush(RGB(72,183,152));//  大窗口				 					  
	CBrush* pOldBrush=pDC->SelectObject(&bShow);
	pDC->Rectangle(630,60,850,520);// 
	pDC->SelectObject(pOldBrush);//++++
	bShow.DeleteObject();

//  ReleaseDC(pDshow);
//	ReleaseDC(pDC);

	m_bEnShowPeakValeRatio = true;

}

void CMaintView::OnShowPeakValeRatio(int no)
{
	CDC* pDC=GetDC();
	int iPeakValue = no;
	CString strtextout;

	CBrush bShow;			
	bShow.CreateSolidBrush(RGB(72,183,152));//  				 					  
	CBrush* pOldBrush=pDC->SelectObject(&bShow);
	pDC->Rectangle(630,60,850,520);// 
	pDC->SelectObject(pOldBrush);//++++
	bShow.DeleteObject();

	pDC->SetBkColor(RGB(72,183,152));
	pDC->SetTextColor(RGB(30,43,31));  //RGB(236,28,33)

	if(peakvaleratio.fPeakVale_Ratio[iPeakValue/8][iPeakValue%8][0] != -1) {
		strtextout.Format("X=%d，Y=%d； 左上角：%.3f  ",iPeakValue%8, iPeakValue/8,peakvaleratio.fPeakVale_Ratio[iPeakValue/8][iPeakValue%8][0]);
	} else strtextout.Format("X=%d，Y=%d； 左上角：**** ",iPeakValue%8, iPeakValue/8);

	pDC->TextOut(635,65,strtextout);
	if(peakvaleratio.fPeakVale_Ratio[iPeakValue/8][iPeakValue%8][1] != -1) {
		strtextout.Format("X=%d，Y=%d； 上方：%.3f ",iPeakValue%8, iPeakValue/8,peakvaleratio.fPeakVale_Ratio[iPeakValue/8][iPeakValue%8][1]);
	} else strtextout.Format("X=%d，Y=%d； 上方：**** ",iPeakValue%8, iPeakValue/8); 
	pDC->TextOut(635,85,strtextout);
	if(peakvaleratio.fPeakVale_Ratio[iPeakValue/8][iPeakValue%8][2] != -1) {
		strtextout.Format("X=%d，Y=%d； 右上角：%.3f ",iPeakValue%8, iPeakValue/8,peakvaleratio.fPeakVale_Ratio[iPeakValue/8][iPeakValue%8][2]);
	} else strtextout.Format("X=%d，Y=%d； 右上角：**** ",iPeakValue%8, iPeakValue/8); 
	pDC->TextOut(635,105,strtextout);
	if(peakvaleratio.fPeakVale_Ratio[iPeakValue/8][iPeakValue%8][3] != -1) {
		strtextout.Format("X=%d，Y=%d； 右方：%.3f ",iPeakValue%8, iPeakValue/8,peakvaleratio.fPeakVale_Ratio[iPeakValue/8][iPeakValue%8][3]);
	} else strtextout.Format("X=%d，Y=%d； 右方：**** ",iPeakValue%8, iPeakValue/8); 

	pDC->TextOut(635,125,strtextout);
	if(peakvaleratio.fPeakVale_Ratio[iPeakValue/8][iPeakValue%8][4] != -1) {
		strtextout.Format("X=%d，Y=%d； 右下角：%.3f ",iPeakValue%8, iPeakValue/8,peakvaleratio.fPeakVale_Ratio[iPeakValue/8][iPeakValue%8][4]);
	} else strtextout.Format("X=%d，Y=%d； 右下角：**** ",iPeakValue%8, iPeakValue/8); 
	pDC->TextOut(635,145,strtextout);

	if(peakvaleratio.fPeakVale_Ratio[iPeakValue/8][iPeakValue%8][5] != -1) {
		strtextout.Format("X=%d，Y=%d； 下方：%.3f ",iPeakValue%8, iPeakValue/8,peakvaleratio.fPeakVale_Ratio[iPeakValue/8][iPeakValue%8][5]);
	} else strtextout.Format("X=%d，Y=%d； 下方：****  ",iPeakValue%8, iPeakValue/8); 
	pDC->TextOut(635,165,strtextout);
	if(peakvaleratio.fPeakVale_Ratio[iPeakValue/8][iPeakValue%8][6] != -1) {
		strtextout.Format("X=%d，Y=%d； 左下角：%.3f ",iPeakValue%8, iPeakValue/8,peakvaleratio.fPeakVale_Ratio[iPeakValue/8][iPeakValue%8][6]);
	} else strtextout.Format("X=%d，Y=%d； 左下角：**** ",iPeakValue%8, iPeakValue/8); 
	pDC->TextOut(635,185,strtextout);
	if(peakvaleratio.fPeakVale_Ratio[iPeakValue/8][iPeakValue%8][7] != -1) {
		strtextout.Format("X=%d，Y=%d； 左方：%.3f ",iPeakValue%8, iPeakValue/8,peakvaleratio.fPeakVale_Ratio[iPeakValue/8][iPeakValue%8][7]);
	} else strtextout.Format("X=%d，Y=%d； 左方：**** ",iPeakValue%8, iPeakValue/8); 
	pDC->TextOut(635,205,strtextout);

	//show text;

	strtextout.Format("晶体条号： %d  ,X=%d，Y=%d；",iPeakValue,iPeakValue%8, iPeakValue/8); 
	pDC->TextOut(635,255,strtextout);
	

	ReleaseDC(pDC);

}

void CMaintView::OnCheckVerHor() 
{
	m_bSameDriction_X = (!m_bSameDriction_X);
	
}

void CMaintView::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	// TODO: Add your message handler code here and/or call default
	
	CView::OnKeyUp(nChar, nRepCnt, nFlags);
}

void CMaintView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	// TODO: Add your message handler code here and/or call default

	if(nChar == VK_F5) {  OnRedraw();  };
	if(nChar == VK_F8) { 
		if(g_DataSample_Mod_Sel != 0xC7) {
			return;
		}
		RedrawLimitFile();  
	};
	

//	if(nChar == VK_RIGHT) {
//		OnFastOpen32FileKeyAdd();
//	}
//
//	if(nChar == VK_LEFT) {
//		OnFastOpen32FileKeyDec();
//	}

	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CMaintView::OnCheckMidfilter() 
{
	CMaintDoc*  pDoc = GetDocument();

	CMainFrame* pMainFrame=
	      (CMainFrame*)GetParentFrame();
	CComboBox *pBlockindex = (CComboBox *)
		(pMainFrame)->m_wndShowBar.GetDlgItem(IDC_SHOW_BLOCK_SEL);

	
	int nBlindex= pBlockindex->GetCurSel();

	m_bEnMidFilter = (!m_bEnMidFilter);

	if(m_bEnMidFilter) { //进行中值滤波
		m_Midfilter_Old_BlockNo = nBlindex;
		memcpy(g_Backup_iMidfilter_sdot,pDoc->BuffDot3w[nBlindex],64*64*4);
		OnMidfilter();
		memcpy(pDoc->BuffDot3w[nBlindex],m_iMidfilter_sdot,64*64*4);
	}
	else {
		memcpy(pDoc->BuffDot3w[nBlindex],g_Backup_iMidfilter_sdot,64*64*4);
	}
	
	OnRedraw();
}

void CMaintView::OnMidfilter()
{
	CMaintDoc*  pDoc = GetDocument();
	CMainFrame* pMainFrame=
	      (CMainFrame*)GetParentFrame();
	CComboBox *pBlockindex = (CComboBox *)
		(pMainFrame)->m_wndShowBar.GetDlgItem(IDC_SHOW_BLOCK_SEL);

	int nBlindex= pBlockindex->GetCurSel();

	int avrage;
	memcpy(m_iMidfilter_sdot,pDoc->BuffDot3w[nBlindex],64*64*4);
	for(int i=1;i<63;i++) {
		for(int j=1;j<63;j++) {
			avrage = 0;
			avrage += pDoc->BuffDot3w[nBlindex][i-1][j-1];
			avrage += pDoc->BuffDot3w[nBlindex][i-1][j];
			avrage += pDoc->BuffDot3w[nBlindex][i-1][j+1];
			avrage += pDoc->BuffDot3w[nBlindex][i][j+1];
			avrage += pDoc->BuffDot3w[nBlindex][i+1][j+1];
			avrage += pDoc->BuffDot3w[nBlindex][i+1][j];
			avrage += pDoc->BuffDot3w[nBlindex][i+1][j-1];
			avrage += pDoc->BuffDot3w[nBlindex][i][j-1];
			avrage += pDoc->BuffDot3w[nBlindex][i][j];

			avrage = avrage/9;
			m_iMidfilter_sdot[i][j] = avrage;
		}
	}

	for(i=1;i<63;i++) {
		avrage = 0;
		avrage += pDoc->BuffDot3w[nBlindex][0][i-1];
		avrage += pDoc->BuffDot3w[nBlindex][0][i];
		avrage += pDoc->BuffDot3w[nBlindex][0][i+1];
		avrage = avrage/3;
		m_iMidfilter_sdot[0][i] = avrage;
	}
	for(i=1;i<63;i++) {
		avrage = 0;
		avrage += pDoc->BuffDot3w[nBlindex][63][i-1];
		avrage += pDoc->BuffDot3w[nBlindex][63][i];
		avrage += pDoc->BuffDot3w[nBlindex][63][i+1];
		avrage = avrage/3;
		m_iMidfilter_sdot[63][i] = avrage;
	}


	for(i=1;i<63;i++) {
		avrage = 0;
		avrage += pDoc->BuffDot3w[nBlindex][i-1][0];
		avrage += pDoc->BuffDot3w[nBlindex][i][0];
		avrage += pDoc->BuffDot3w[nBlindex][i+1][0];
		avrage = avrage/3;
		m_iMidfilter_sdot[i][0] = avrage;
	}
	for(i=1;i<63;i++) {
		avrage = 0;
		avrage += pDoc->BuffDot3w[nBlindex][i-1][63];
		avrage += pDoc->BuffDot3w[nBlindex][i][63];
		avrage += pDoc->BuffDot3w[nBlindex][i+1][63];
		avrage = avrage/3;
		m_iMidfilter_sdot[i][63] = avrage;
	}

	avrage = 0;
	avrage += pDoc->BuffDot3w[nBlindex][0][0];
	avrage += pDoc->BuffDot3w[nBlindex][0][1];
	avrage += pDoc->BuffDot3w[nBlindex][1][0];
	avrage = avrage/3;
	m_iMidfilter_sdot[0][0] = avrage;

	avrage = 0;
	avrage += pDoc->BuffDot3w[nBlindex][0][62];
	avrage += pDoc->BuffDot3w[nBlindex][0][63];
	avrage += pDoc->BuffDot3w[nBlindex][1][63];
	avrage = avrage/3;
	m_iMidfilter_sdot[0][63] = avrage;

	avrage = 0;
	avrage += pDoc->BuffDot3w[nBlindex][62][0];
	avrage += pDoc->BuffDot3w[nBlindex][63][0];
	avrage += pDoc->BuffDot3w[nBlindex][63][1];
	avrage = avrage/3;
	m_iMidfilter_sdot[63][0] = avrage;

	avrage = 0;
	avrage += pDoc->BuffDot3w[nBlindex][62][63];
	avrage += pDoc->BuffDot3w[nBlindex][63][63];
	avrage += pDoc->BuffDot3w[nBlindex][63][62];
	avrage = avrage/3;
	m_iMidfilter_sdot[63][63] = avrage;

}

void CMaintView::OnMenuLoad32bk() 
{	
	hmodel = m_pview->GetSafeHwnd();
	if(hmodel == NULL) {  //保证只创建一个窗口

		m_pview->m_bEnComm = m_bEnLoad32BKComm;
		m_pview->m_PortNo = g_Port_Sel;

		m_pview->Create();		
	}
	
}

LRESULT CMaintView::OnModGoodBye(WPARAM wParam,LPARAM lParam) 
{

	if(wParam == IDCANCEL) {	
		m_bEnLoad32BKComm = m_pview->m_bEnComm;
		g_Port_Sel = m_pview->m_PortNo;
		m_pview->DestroyWindow();
		//m_pDlg = NULL;
	}
	else {
		
	}
	return 0L;
}

void CMaintView::OnPeakPositionStat()
{
	CMaintDoc*  pDoc = GetDocument();
	CMainFrame* pMainFrame=(CMainFrame*)GetParentFrame();

	CComboBox *pBlockindex = (CComboBox *)
		(pMainFrame)->m_wndShowBar.GetDlgItem(IDC_SHOW_BLOCK_SEL);
	CEdit* pThreshvalue=(CEdit*)
		(pMainFrame->m_wndShowBar.GetDlgItem(IDC_3D_SHOW_THREVALUE));

	int nindex = pBlockindex->GetCurSel();
	if((nindex >8)||(nindex <0)) nindex = 0;


	int  peakdata[8][64];
	memset(peakdata,0,8*64*4);
	for(int nblock=0;nblock<8;nblock++) {  //算能谱
		for(int k=0;k<64;k++) {
				for(int i=0;i<64;i++) {					
					peakdata[nblock][k] +=  pDoc->BuffDot3w[nblock][i][k];
				}
		}
	}

	int peak3dotfilter[8][64];
	memset(peak3dotfilter,0,8*64*4);
	for(nblock=0;nblock<8;nblock++) {  //3点滤波
		for(int i=1;i<62;i++) {
			peak3dotfilter[nblock][i] = peakdata[nblock][i-1] + peakdata[nblock][i] + peakdata[nblock][i+1] ;
			peak3dotfilter[nblock][i] = (float)peak3dotfilter[nblock][i] / 3 ;
		}
		peak3dotfilter[nblock][62] = peakdata[nblock][62];
		peak3dotfilter[nblock][63] = peakdata[nblock][63];
	}

	int maxpeakval[8] = {1};
	int peakval_limitL[8] = {0}; 
	int	peakval_limitR[8] = {63};
	int peakval_table[8] = {1};

	int i;
	for(nblock=0;nblock<8;nblock++) {   //找峰值 峰位
		int i = 61;
		while (i > 2) {
			if((peak3dotfilter[nblock][i] >= peak3dotfilter[nblock][i-1])&&(peak3dotfilter[nblock][i] > peak3dotfilter[nblock][i+1])) {
				if((i>8)||(i<55)) {
					if((peak3dotfilter[nblock][i] > peak3dotfilter[nblock][i-5])&&(peak3dotfilter[nblock][i] > peak3dotfilter[nblock][i+5])) {
						maxpeakval[nblock] = peak3dotfilter[nblock][i];
						peakval_table[nblock] = i;
						int kk = i-1;
						while (kk > i-5) {
							if (peak3dotfilter[nblock][kk] > peak3dotfilter[nblock][i]) {
								if((peak3dotfilter[nblock][kk] > peak3dotfilter[nblock][kk-3])&&(peak3dotfilter[nblock][kk] > peak3dotfilter[nblock][kk+3])) {
									maxpeakval[nblock] = peak3dotfilter[nblock][kk];
									peakval_table[nblock] = kk;
									break;
								}							
							}
							kk --;
						}
						break;
					}
				}
			}
			i --;  
		}
	}
	
	for(nblock=0;nblock<8;nblock++) {
		i = peakval_table[nblock];
		while (i >0) {
			if((peak3dotfilter[nblock][i] < peak3dotfilter[nblock][i+1])&&(peak3dotfilter[nblock][i] < peak3dotfilter[nblock][i-1])) {
				if(peakval_table[nblock] - i > 5) {
					peakval_limitL[nblock] = i;
					peakval_limitR[nblock] = peakval_table[nblock] + peakval_table[nblock] - peakval_limitL[nblock];
					if(peakval_limitR[nblock]>63) peakval_limitR[nblock] = 63;
					break;
				}
			}
			i --;
		}		
	}
	
	CDC* pDC=GetDC();
	COLORREF rf=RGB(26,131,162);//RGB(15,155,85); 
	CRect rcClient;
	GetClientRect(&rcClient);
	pDC->FillSolidRect(&rcClient,rf);

	pDC->TextOut(250,5," 3 点滤波 视图 ");
	pDC->TextOut(200,23," --------------------   水平 方向   -------------------> ");
	pDC->MoveTo(55,105);
	pDC->LineTo(55,190);
	pDC->TextOut(50,200,"垂");
	pDC->TextOut(50,220,"直");
	pDC->TextOut(50,240,"方");
	pDC->TextOut(50,260,"向");
	pDC->TextOut(52,360,"V");
	pDC->MoveTo(55,280);
	pDC->LineTo(55,365);
	
	pDC->MoveTo(g_draw_startpoint_y,g_draw_startpoint_x); 
	pDC->LineTo(g_draw_startpoint_y,g_draw_size*64+g_draw_startpoint_x);
	pDC->MoveTo(g_draw_startpoint_y,g_draw_startpoint_x);
	pDC->LineTo(g_draw_startpoint_y-3,g_draw_startpoint_x+5);

    pDC->MoveTo(g_draw_startpoint_y,g_draw_size*64+g_draw_startpoint_x);
	pDC->LineTo(g_draw_startpoint_y +g_draw_size*64 ,g_draw_size*64+g_draw_startpoint_x);
	pDC->MoveTo(g_draw_startpoint_y +g_draw_size*64,g_draw_size*64+g_draw_startpoint_x);
    pDC->LineTo(g_draw_startpoint_y +g_draw_size*64 - 5,g_draw_size*64+g_draw_startpoint_x +3);



	CBrush bShow;			
	bShow.CreateSolidBrush(RGB(72,183,152));//  大窗口				 					  
	CBrush* pOldBrush=pDC->SelectObject(&bShow);
	pDC->Rectangle(630,60,850,520);// 
	pDC->SelectObject(pOldBrush);//++++

	int longtdata[64];

	memcpy(longtdata,peak3dotfilter[nindex],64*4);
	pDoc->temp = maxpeakval[nindex];

	CString strEditThresh;
	pThreshvalue->GetWindowText(strEditThresh);
	if(!strEditThresh.IsEmpty())  {		
            pDoc->temp=atol(strEditThresh);        
	        pThreshvalue->Invalidate();
	}


	int tplace;

	int x = g_draw_startpoint_y; 
	for(i=0;i<64;i++) {
		if(pDoc->temp < 1) pDoc->temp = 1;
		tplace = g_draw_size*64 + g_draw_startpoint_x - (int)(g_draw_size*64*longtdata[i]/(pDoc->temp));
		if(tplace < 30) tplace = 30;
		if(tplace > g_draw_size*64 + g_draw_startpoint_x) tplace = g_draw_size*64;
	
		CBrush bShow;				
		bShow.CreateSolidBrush(RGB(
					   GetBValue(ColorSet(longtdata[i],pDoc->temp)), 
					   GetGValue(ColorSet(longtdata[i],pDoc->temp)),
					   GetRValue(ColorSet(longtdata[i],pDoc->temp))));			 					  
		CBrush* pOldBrush=pDC->SelectObject(&bShow);// 
		pDC->Rectangle(x-g_draw_frame,tplace-g_draw_frame,x+g_draw_frame,tplace+g_draw_frame);// 
				
		pDC->SelectObject(pOldBrush);//++++
		bShow.DeleteObject();
		x += g_draw_size;
	}

	CPen pShow(PS_SOLID,2,RGB(85, 15,185));//   RGB(0,210,0)
	CPen* pOldPen=NULL;
	pOldPen=pDC->SelectObject(&pShow);
	x = g_draw_startpoint_y+8 ; 
	tplace = g_draw_startpoint_x + 64*g_draw_size- (int)(g_draw_size*64*longtdata[0]/(pDoc->temp));
		
	if(tplace < 30) tplace = 30;
	if(tplace > g_draw_size*64 + g_draw_startpoint_x) tplace = g_draw_size*64;
				
	for(int kk=1;kk<64;kk++) {																				
		pDC->MoveTo(x-8,tplace);
		x += g_draw_size;
		tplace = g_draw_size*64 + g_draw_startpoint_x - (int)(g_draw_size*64*longtdata[kk]/(pDoc->temp));
		if(pDoc->temp < 1) pDoc->temp = 1;
		if(tplace < 30) tplace = 30;
		if(tplace > g_draw_size*64 + g_draw_startpoint_x) tplace = g_draw_size*64;
		pDC->LineTo(x-8,tplace);
	}

	CPen pShow2(PS_SOLID,1,RGB(205, 18,135));// 红
	pDC->SelectObject(&pShow2);
	pDC->MoveTo(g_draw_startpoint_y + g_draw_size*peakval_table[nindex],40);
	pDC->LineTo(g_draw_startpoint_y + g_draw_size*peakval_table[nindex],560);


	CPen pShow111(PS_SOLID,1,RGB(255, 255,128));//   RGB(0,210,0)
	pDC->SelectObject(&pShow111);			
	x = 39;
	pDC->MoveTo(x * g_draw_size + g_draw_startpoint_y,50);
	pDC->LineTo(x * g_draw_size + g_draw_startpoint_y,560);
	x = 42;
	pDC->MoveTo(x * g_draw_size + g_draw_startpoint_y,50);
	pDC->LineTo(x * g_draw_size + g_draw_startpoint_y,560);


	pDC->SetBkColor(RGB(72,183,152));
	pDC->SetTextColor(RGB(30,43,31));  //RGB(236,28,33)

	char chbuf[50];
	sprintf(chbuf,"BK0  峰位:%.2d,      左: %.2d  右: %.2d", peakval_table[0],	peakval_limitL[0],peakval_limitR[0]);
	pDC->TextOut(635,65,chbuf);
	sprintf(chbuf,"BK1  峰位:%.2d,      左: %.2d  右: %.2d", peakval_table[1],	peakval_limitL[1],peakval_limitR[1]);
	pDC->TextOut(635,85,chbuf);
	sprintf(chbuf,"BK2  峰位:%.2d,      左: %.2d  右: %.2d", peakval_table[2],	peakval_limitL[2],peakval_limitR[2]);
	pDC->TextOut(635,105,chbuf);
	sprintf(chbuf,"BK3  峰位:%.2d,      左: %.2d  右: %.2d", peakval_table[3],	peakval_limitL[3],peakval_limitR[3]);
	pDC->TextOut(635,125,chbuf);
	sprintf(chbuf,"BK4  峰位:%.2d,      左: %.2d  右: %.2d", peakval_table[4],	peakval_limitL[4],peakval_limitR[4]);
	pDC->TextOut(635,145,chbuf);
	sprintf(chbuf,"BK5  峰位:%.2d,      左: %.2d  右: %.2d", peakval_table[5],	peakval_limitL[5],peakval_limitR[5]);
	pDC->TextOut(635,165,chbuf);
	sprintf(chbuf,"BK6  峰位:%.2d,      左: %.2d  右: %.2d", peakval_table[6],	peakval_limitL[6],peakval_limitR[6]);
	pDC->TextOut(635,185,chbuf); 
	sprintf(chbuf,"BK7  峰位:%.2d,      左: %.2d  右: %.2d", peakval_table[7],	peakval_limitL[7],peakval_limitR[7]);
	pDC->TextOut(635,205,chbuf);

	sprintf(chbuf," ( BK 表示 Block )");
	pDC->TextOut(635,255,chbuf);

	sprintf(chbuf,"当前Block%d :峰位:%d 峰值:%d",nindex,peakval_table[nindex],peak3dotfilter[nblock][peakval_table[nindex]]);
	pDC->TextOut(635,285,chbuf);
	ReleaseDC(pDC);

//	OnSaveEthPeakStat(nindex,peakval_table,peakval_limitL,peakval_limitR);
}

void CMaintView::OnSaveEthPeakStat(CString filename,int bucketno,int *peakvalue)
{
//	CString FilePathName; 
	//	CFileDialog dlg(TRUE,"txt","*.txt");///TRUE为OPEN对话框，FALSE为SAVE AS对话框 
	//	
	//	dlg.m_ofn.lpstrFilter = ("能谱峰值统计.txt");//
	//	if(dlg.DoModal()==IDOK) FilePathName=dlg.GetPathName(); 
	//	else return;
	//
	char strtemp[100];
	CFile myfile;
	BOOL bfileopensucc = true;
	
	if(!myfile.Open(filename + ".txt",CFile::modeRead)) {
		bfileopensucc = false;		
	}
	if (bfileopensucc) {		
		myfile.Close();
	}
	
	if (!bfileopensucc) {
		myfile.Open(filename+ ".txt",CFile::modeCreate | CFile::modeWrite);
		strcpy(strtemp,"                  能谱峰值统计      \r\n"); 
		myfile.Write(strtemp,strlen(strtemp));
		strcpy(strtemp,"Bucket No      b0   b1   b2   b3   b4   b5   b6   b7     时间 \r\n"); 
		myfile.Write(strtemp,strlen(strtemp));	
	}
	else {
		myfile.Open(filename+ ".txt",CFile::modeReadWrite);
		myfile.Seek(0,CFile::end);
	}

	CTime  time = CTime::GetCurrentTime();
	sprintf(strtemp,"Bucket %.2d:     %.2d   %.2d   %.2d   %.2d   %.2d   %.2d   %.2d   %.2d   (%d月%.2d日%.2d:%.2d )\r\n",bucketno,
		peakvalue[0],peakvalue[1],peakvalue[2],peakvalue[3],peakvalue[4],peakvalue[5],peakvalue[6],
		peakvalue[7],time.GetMonth(),time.GetDay(),time.GetHour(),time.GetMinute());

	myfile.Write(strtemp,strlen(strtemp));	

	myfile.Close();	

}

void CMaintView::OnMouseFastCal32EthPeakStat()
{
	CMaintDoc*  pDoc = GetDocument();
	CMainFrame* pMainFrame=(CMainFrame*)GetParentFrame();

	CComboBox *pBlockindex = (CComboBox *)
		(pMainFrame)->m_wndShowBar.GetDlgItem(IDC_SHOW_BLOCK_SEL);
	CEdit* pThreshvalue=(CEdit*)
		(pMainFrame->m_wndShowBar.GetDlgItem(IDC_3D_SHOW_THREVALUE));


	CString FilePathName; 
	CString buckfilename;

	if (g_bOperating_32bkPeakStat) {
		buckfilename = g_strOperating_32bkPeakFilePath;	
	}
	else {	
		CFileDialog dlg(TRUE,"","");///TRUE为OPEN对话框，FALSE为SAVE AS对话框 	
		dlg.m_ofn.lpstrFilter = ("能谱峰值统计");//
		if(dlg.DoModal()==IDOK) {
			FilePathName=dlg.GetPathName(); 
			buckfilename=dlg.GetFileTitle();
		}
		else return;
	}

	int maxpeakval[8] = {1};
	int peakval_limitL[8] = {0}; 
	int	peakval_limitR[8] = {63};
	int peakval_table[8] = {1};
	int peakdata[8][64];

	CFile dotsfile;
	CString filename;
	CString strtemp;
	unsigned char chartemp[10];
	int buffdots3w[8][64][64] = {0 };
	for(int bucket=0;bucket<32;bucket++) {

		if (g_bOperating_32bkPeakStat) {			
			filename = g_strOperating_32bkPeakFilePath;
		}
		else {
			strtemp.Format("%.2d",bucket);
			filename = buckfilename + strtemp + ".dat";	
		}

		if(dotsfile.Open(filename,CFile::modeRead)) {

			dotsfile.Seek(FILEHEAD_TYPE_ADDR,CFile::begin);  //读取 文件类型
			dotsfile.Read(chartemp,2);
			BOOL bSampledfile = true;
			if((chartemp[0] == 0x01)&&(chartemp[1] == 0x01)) { //4字节保存过得数据
				bSampledfile = false;  //
			}

			dotsfile.Seek(DATA_ESPECT_ADDR,CFile::begin);
			if (bSampledfile) {
				for(int blk=0;blk<8;blk++) {
					for(int y=0;y<64;y++) {
						for(int x=0;x<64;x++) {
							dotsfile.Read(&type.uchdata[0],4);
							buffdots3w[blk][y][x] = type.intdata;
						}
					}
				}
			}
			else {
				type.intdata = 0;
				for(int blk=0;blk<8;blk++) {
					for(int y=0;y<64;y++) {
						for(int x=0;x<64;x++) {
							dotsfile.Read(&type.uchdata[0],2);
							buffdots3w[blk][y][x] = type.intdata;
						}
					}
				}
			}	
			
			dotsfile.Close();			

			memset(peakdata,0,8*64*4);
			for(int nblock=0;nblock<8;nblock++) {  //算能谱
				for(int k=0;k<64;k++) {
					for(int i=0;i<64;i++) {					
						peakdata[nblock][k] +=  buffdots3w[nblock][i][k];
					}
				}
			}
			
			int peak3dotfilter[8][64];
			memset(peak3dotfilter,0,8*64*4);
			for(nblock=0;nblock<8;nblock++) {  //3点滤波
				for(int i=1;i<62;i++) {
					peak3dotfilter[nblock][i] = peakdata[nblock][i-1] + peakdata[nblock][i] + peakdata[nblock][i+1] ;
					peak3dotfilter[nblock][i] = (float)peak3dotfilter[nblock][i] / 3 ;
				}
				peak3dotfilter[nblock][62] = peakdata[nblock][62];
				peak3dotfilter[nblock][63] = peakdata[nblock][63];
			}
			
			
			int i;
			for(nblock=0;nblock<8;nblock++) {   //找峰值 峰位
				int i = 61;
				while (i > 2) {
					if((peak3dotfilter[nblock][i] >= peak3dotfilter[nblock][i-1])&&(peak3dotfilter[nblock][i] > peak3dotfilter[nblock][i+1])) {
						if((i>8)||(i<55)) {
							if((peak3dotfilter[nblock][i] > peak3dotfilter[nblock][i-5])&&(peak3dotfilter[nblock][i] > peak3dotfilter[nblock][i+5])) {
								maxpeakval[nblock] = peak3dotfilter[nblock][i];
								peakval_table[nblock] = i;
								int kk = i-1;
								while (kk > i-5) {
									if (peak3dotfilter[nblock][kk] > peak3dotfilter[nblock][i]) {
										if((peak3dotfilter[nblock][kk] > peak3dotfilter[nblock][kk-3])&&(peak3dotfilter[nblock][kk] > peak3dotfilter[nblock][kk+3])) {
											maxpeakval[nblock] = peak3dotfilter[nblock][kk];
											peakval_table[nblock] = kk;
											break;
										}							
									}
									kk --;
								}
								break;
							}
						}
					}
					i --;  
				}
			}
			
			if (g_bOperating_32bkPeakStat) {
				memcpy(&(m_iPeakstat_file_buff[0][0]),&(peakval_table[0]),8*4);
				bucket = 32;
			}
			else {
				OnSaveEthPeakStat(buckfilename,bucket,peakval_table);
			}	
			
			
		}
	}


}

void CMaintView::OnLP_Evaluate() 
{

	CMaintDoc*  pDoc = GetDocument();

	pDoc->OnFileOpen();  //打开第一文件A

	OnRedraw();

	OnOpenLimittab();  //导入位置边界

	RedrawLimitFile();

	OnRedraw();

	OnEffectCalFirstEvalute();

	pDoc->OnFileOpen();  //打开第二文件B （目标文件）

	OnRedraw();

	RedrawLimitFile();

	OnEffecentEvaluat();  //效率统计 ，，输出

}

void CMaintView::OnEffecentEvaluat() //int block
{
	if(g_DataSample_Mod_Sel != 0xC7) {
		return;
	}

	m_ShowModelSel = 5;
	RedrawLimitFile();

	CMaintDoc*  pDoc = GetDocument();
	CMainFrame* pMainFrame = (CMainFrame*)GetParentFrame();
	CComboBox *pBlockindex = (CComboBox *)
		(pMainFrame)->m_wndShowBar.GetDlgItem(IDC_SHOW_BLOCK_SEL);

	int nBlindex;
	nBlindex= pBlockindex->GetCurSel();		
	if (g_bOperating_32bkEffectEvaluate) {
		nBlindex = g_iOperatingEffect_block;		
	}

	if ((nBlindex >8)||(nBlindex<0)) { return;	}


	int X0[8][8],X1[8][8],Y0[8][8],Y1[8][8];
	for(int i=0;i<8;i++) {
		for(int j=0;j<8;j++) {						
			X0[i][j] = m_8Block_Position_X0[nBlindex][i][j];
			Y0[i][j] = m_8Block_Position_Y0[nBlindex][i][j];
			X1[i][j] = m_8Block_Position_X1[nBlindex][i][j];
			Y1[i][j] = m_8Block_Position_Y1[nBlindex][i][j];
		}
	}
	
	long limit_stat[8][8] = { 0 };
	int igoto_one[8][8] = { 0 };

	memset(limit_stat,0,8*8*4);
	for(i=0;i<8;i++) { //纵坐标
		for(int j=0;j<8;j++) { //横坐标
			for(int k=X0[i][j];k<=X1[i][j];k++) {
				for(int m=Y0[i][j];m<=Y1[i][j];m++) {
					//limit_stat[i][j] +=  sdot[m][k];						
					limit_stat[i][j] += pDoc->BuffDot3w[nBlindex][m][k];					
				}
			}
		}
	}
	
	int max_stat;		
	max_stat = limit_stat[m_Efficent_Evalu_Max_Y[nBlindex]][m_Efficent_Evalu_Max_X[nBlindex]];	
	
	if(max_stat < 1) max_stat = 1;
	for(i=0;i<8;i++) {
		for(int j=0;j<8;j++){
			igoto_one[i][j] = 100* (float)limit_stat[i][j]/max_stat ;
		}
	}	

		m_Efficent_Evalu_SquE = 0;
		for(i=0;i<8;i++) {
			for(int j=0;j<8;j++){
				m_Efficent_Evalu_SquE +=  (m_Efficent_Evalu_FileA_data[nBlindex][i][j] - igoto_one[i][j]) * (m_Efficent_Evalu_FileA_data[nBlindex][i][j] - igoto_one[i][j]);
				igoto_one[i][j] =  m_Efficent_Evalu_FileA_data[nBlindex][i][j] - igoto_one[i][j];
			}
		}
		m_Efficent_Evalu_SquE = (float)m_Efficent_Evalu_SquE / 64;
		m_Efficent_Evalu_SquE = sqrt((float)m_Efficent_Evalu_SquE);	
		
		memcpy(g_iEffect32bk_goto_one,&igoto_one[0][0],8*8*4);
		g_fEffect32bk_Evalu_SquE = m_Efficent_Evalu_SquE;
		

		if (g_bOperating_32bkEffectEvaluate) {
			return;
		}

    	CDC* pDC=GetDC();
		CString strtext;
		pDC->SetTextColor(RGB(105,15,205));
	
	   CDC* pDshow=GetDC();	    
	   int y = g_draw_startpoint_x + 27;	 
	   for(int j=0;j<8;j++) {
		   int x = g_draw_startpoint_y+27;
		   for(int k=0;k<8;k++) {
			   CBrush bShow;			
			   bShow.CreateSolidBrush(
				   RGB(GetBValue(ColorSet(limit_stat[j][k],max_stat)), 
						GetGValue(ColorSet(limit_stat[j][k],max_stat)),
						GetRValue(ColorSet(limit_stat[j][k],max_stat)) )
				   );//					
			   CBrush* pOldBrush=pDshow->SelectObject(&bShow);// 
			   pDshow->Rectangle(x-32,y-32,x+32,y+32);// 
			   
			   pDshow->SelectObject(pOldBrush);//++++
			   bShow.DeleteObject();
			   
			   strtext.Format("%d",igoto_one[j][k]);				
			   pDC->TextOut(x-5,y-5,strtext);			
			   
			   x += g_draw_size*8;
		   }
		   y += g_draw_size*8;
	   }
	   char chrout[10];
	   sprintf(chrout,"效率均方差: %.3f",m_Efficent_Evalu_SquE);
	   pDC->TextOut(635,65,chrout);
	   ReleaseDC(pDshow);	
	
}

void CMaintView::OnEffectCalFirstEvalute()
{
	CMaintDoc*  pDoc = GetDocument();

	int X0[8][8][8],X1[8][8][8],Y0[8][8][8],Y1[8][8][8];

	for(int block=0;block<8;block ++) {
		for(int i=0;i<8;i++) { 
			for(int j=0;j<8;j++) {						
				X0[block][i][j] = m_8Block_Position_X0[block][i][j];
				Y0[block][i][j] = m_8Block_Position_Y0[block][i][j];
				X1[block][i][j] = m_8Block_Position_X1[block][i][j];
				Y1[block][i][j] = m_8Block_Position_Y1[block][i][j];
			}
		}
	}

	long limit_stat[8][8][8];
	int igoto_one[8][8][8];

	memset(limit_stat,0,8*8*8*4);
	for(block=0;block<8;block++) {
		for(int i=0;i<8;i++) { //纵坐标
			for(int j=0;j<8;j++) { //横坐标
				for(int k=X0[block][i][j];k<=X1[block][i][j];k++) {
					for(int m=Y0[block][i][j];m<=Y1[block][i][j];m++) {
						//limit_stat[i][j] +=  sdot[m][k];	
						limit_stat[block][i][j] += pDoc->BuffDot3w[block][m][k];
					}
				}
			}
		}
	}


	int max_stat[8] = {1};	
	memset(m_Efficent_Evalu_Max_X,0,8*4);
	memset(m_Efficent_Evalu_Max_Y,0,8*4);

	for(block=0;block<8;block++) {
		for(int i=0;i<8;i++) {
			for(int j=0;j<8;j++){
				if(limit_stat[block][i][j] > max_stat[block]) {
					max_stat[block] = limit_stat[block][i][j];				
					m_Efficent_Evalu_Max_X[block] = j;
					m_Efficent_Evalu_Max_Y[block] = i;								
				}
			}
		}	
		if(max_stat[block] < 1) max_stat[block] = 1;
		for(i=0;i<8;i++) {
			for(int j=0;j<8;j++){
				igoto_one[block][i][j] = 100* (float)limit_stat[block][i][j]/max_stat[block] ;
			}
		}
	}
	memcpy(m_Efficent_Evalu_FileA_data,igoto_one,8*8*8*4);

}

void CMaintView::OnView32bklpEvaluate() 
{
	CMaintDoc*  pDoc = GetDocument();

	CEffectStat effectstat;
	if (IDOK != effectstat.DoModal()) { return;	}

	CFile srcfile;
	CFile destfile;
	CString strtemp;
	BOOL bFileOpenSuccess = false;
	for(int bk=effectstat.m_iStartFileNo;bk<=effectstat.m_iEndFileNo;bk++) {
		bFileOpenSuccess = false;
		strtemp.Format("%.2d.dat",bk);
		if (srcfile.Open(effectstat.m_Effect_SourceFileName+ strtemp,CFile::modeRead)) { 
			srcfile.Close();
			if (destfile.Open(effectstat.m_Effect_DestFileName+ strtemp,CFile::modeRead)) {
				bFileOpenSuccess = true;  //二个文件都存在								
				destfile.Close();
			}
		}
		if (bFileOpenSuccess) {// 读取 源文件，源文件边界，目标文件；
			g_bOperating_32bkEffectEvaluate = true;	
			g_strOperating_32bkEffectFilePath = effectstat.m_Effect_SourceFilePath + strtemp;

			pDoc->OnFileOpen();  //打开第一文件A
			
			OnRedraw();
			
			OnOpenLimittab();  //导入位置边界
			
			RedrawLimitFile();
			
			OnRedraw();
			
			OnEffectCalFirstEvalute();

			g_strOperating_32bkEffectFilePath = effectstat.m_Effect_DestFilePath + strtemp;
			
			pDoc->OnFileOpen();  //打开第二文件B （目标文件）
			
			OnRedraw();
			
			RedrawLimitFile();

			g_bOperating_32bkEffectEvaluate = true;	
			for(int block=0;block<8; block++) {
				g_iOperatingEffect_block = block;
				OnRedraw();				
				RedrawLimitFile();
				OnEffecentEvaluat();  // g_iOperatingEffect_block  率统计
				OnEffectSaveData2File(effectstat.m_Effect_OutTextFilePath,bk,block,g_iEffect32bk_goto_one,g_fEffect32bk_Evalu_SquE,effectstat.m_bOutRelativeValue);
			}
			
			g_bOperating_32bkEffectEvaluate = false;			
		}
	}
	AfxMessageBox(" 效率统计完成 ");
	g_bOperating_32bkEffectEvaluate = false;
	
}


BOOL CMaintView::OnEffectSaveData2File(CString outfilename,int buck,int block,int *effectdata, float effectSquareSalue,BOOL boutRelativevalue)
{
	char strtemp[100];
	CFile myfile;
	BOOL bfileopensucc = true;	
	if(!myfile.Open(outfilename + ".txt",CFile::modeRead)) {
		bfileopensucc = false;		
	}
	if (bfileopensucc) {		
		myfile.Close();
	}
	
	if (!bfileopensucc) {
		myfile.Open(outfilename+ ".txt",CFile::modeCreate | CFile::modeWrite);
		strcpy(strtemp,"                  效率统计 评估数据      \r\n\r\n"); 
		myfile.Write(strtemp,strlen(strtemp));
		
	}
	else {
		myfile.Open(outfilename+ ".txt",CFile::modeReadWrite);
		myfile.Seek(0,CFile::end);
	}
	CTime  time = CTime::GetCurrentTime(); 
	sprintf(strtemp, "Bucket%.2d  block %.2d  (时间：%.2d月%.2d日 %.2d:%.2d)   效率计算均方差：%.3f  \r\n", buck, block,time.GetMonth(),time.GetDay(),time.GetHour(),time.GetMinute(),effectSquareSalue);
	myfile.Write(strtemp,strlen(strtemp));
	
	if(boutRelativevalue) {
		sprintf(strtemp, "\r\n");
		myfile.Write(strtemp,strlen(strtemp));
		strcpy(strtemp,"            X0    X1    X2    X3    X4    X5    X6    X7 \r\n"); 
		myfile.Write(strtemp,strlen(strtemp));		
		for(int i=0;i<8;i++) {	
			sprintf(strtemp,"      Y%d   %+.2d   %+.2d   %+.2d   %+.2d   %+.2d   %+.2d   %+.2d   %+.2d   \r\n",i,
				effectdata[0+8*i],effectdata[1+8*i],effectdata[2+8*i],effectdata[3+8*i],effectdata[4+8*i],effectdata[5+8*i],effectdata[6+8*i],
				effectdata[7+8*i]);
			myfile.Write(strtemp,strlen(strtemp));	
		}
		sprintf(strtemp, "------------------------------------------------------- \r\n");
		myfile.Write(strtemp,strlen(strtemp));		
	}

	if (block == 7) { 
		sprintf(strtemp, "********************************************************\r\n");
		myfile.Write(strtemp,strlen(strtemp));	
	}
	
	myfile.Close();	
	return true;
}

void CMaintView::OnMenu32bucketPeakStat() 
{
	CEffectStat effectstat;
	
	effectstat.m_bEncheck = false;
	effectstat.m_bSel_Peak_Stat = true;
	if (IDOK != effectstat.DoModal()) { return;	}

	CFile srcfile;
	CFile destfile;
	CString strtemp;
	BOOL bFileOpenSuccess = false;
	int StatCounter = 0;

	m_fSquareStatValue = 0;

	g_strOperating_32bkPeakFilePath = effectstat.m_Effect_SourceFilePath ;
	OnMenu32buckPeakOutText(33, effectstat.m_Effect_OutTextFilePath);  //保存A  文件路径
	g_strOperating_32bkPeakFilePath = effectstat.m_Effect_DestFilePath;
	OnMenu32buckPeakOutText(34, effectstat.m_Effect_OutTextFilePath);  //保存B  文件路径

	OnMenu32buckPeakOutText(-1, effectstat.m_Effect_OutTextFilePath);  //输出标题行

	for(int bk=effectstat.m_iStartFileNo;bk<=effectstat.m_iEndFileNo;bk++) {
		bFileOpenSuccess = false;
		strtemp.Format("%.2d.dat",bk);
		if (srcfile.Open(effectstat.m_Effect_SourceFileName+ strtemp,CFile::modeRead)) { 
			srcfile.Close();
			if (destfile.Open(effectstat.m_Effect_DestFileName+ strtemp,CFile::modeRead)) {
				bFileOpenSuccess = true;  //二个文件都存在								
				destfile.Close();
			}
		}
		if (bFileOpenSuccess) {// 读取 源文件，源文件边界，目标文件；
			StatCounter++;
			g_bOperating_32bkPeakStat = true;
			
			g_strOperating_32bkPeakFilePath = effectstat.m_Effect_SourceFilePath + strtemp;
			
			OnMouseFastCal32EthPeakStat();
			memcpy(&(m_iPeakstat_fileA[bk][0]),&(m_iPeakstat_file_buff[0][0]),8*4);

			g_strOperating_32bkPeakFilePath = effectstat.m_Effect_DestFilePath + strtemp;
			OnMouseFastCal32EthPeakStat();
			memcpy(&(m_iPeakstat_fileB[bk][0]),&(m_iPeakstat_file_buff[0][0]),8*4);				
		
			//outfiledate;
			for(int i=0;i<8;i++) {
				m_iValue_B_A[bk][i] = m_iPeakstat_fileB[bk][i] - m_iPeakstat_fileA[bk][i];
				m_fSquareStatValue +=  (m_iValue_B_A[bk][i] * m_iValue_B_A[bk][i]) ;
			}			
			OnMenu32buckPeakOutText(bk, effectstat.m_Effect_OutTextFilePath);

			g_bOperating_32bkPeakStat = false;
		}
	}

	////////////////////////
	m_fSquareStatValue = sqrt( (float)m_fSquareStatValue/(8*StatCounter));
	OnMenu32buckPeakOutText(32,effectstat.m_Effect_OutTextFilePath);

	char chtemp[50];
	sprintf(chtemp,"  %d Bucket 峰位统计完成",StatCounter);
	AfxMessageBox(chtemp);
	
}

//调用 m_iPeakstat_fileA
//调用 m_iPeakstat_fileB
//调用 
void CMaintView::OnMenu32buckPeakOutText(int buckno ,CString outfilename)
{	
	char strtemp[100];
	CTime timer = CTime::GetCurrentTime();
	CFile myfile;
	BOOL bfileopensucc = true;	
	if(!myfile.Open(outfilename + ".txt",CFile::modeRead)) {
		bfileopensucc = false;		
	}
	if (bfileopensucc) {		
		myfile.Close();
	}
	
	if (!bfileopensucc) {  
		myfile.Open(outfilename+ ".txt",CFile::modeCreate | CFile::modeWrite);
		sprintf(strtemp,"*******************************************************************\r\n"); 
		myfile.Write(strtemp,strlen(strtemp));
		strcpy(strtemp,"\r\n                           Bucket 峰位统计    \r\n\r\n"); 
		myfile.Write(strtemp,strlen(strtemp));
		sprintf(strtemp,"*******************************************************************\r\n"); 
		myfile.Write(strtemp,strlen(strtemp));
		
	}
	else {
		myfile.Open(outfilename+ ".txt",CFile::modeReadWrite);
		myfile.Seek(0,CFile::end);
	}

	if (buckno == 32) {		//输出计算结果
		sprintf(strtemp,"------------------------------------------------------------------\r\n"); 
		myfile.Write(strtemp,strlen(strtemp));
		sprintf(strtemp,"     均方差:                    ××           ××          %.3f \r\n\r\n",m_fSquareStatValue); 
		myfile.Write(strtemp,strlen(strtemp));
		sprintf(strtemp,"------------------------------------------------------------------\r\n"); 
		myfile.Write(strtemp,strlen(strtemp));
		sprintf(strtemp,"------------------------------------------------------------------\r\n\r\n"); 
		myfile.Write(strtemp,strlen(strtemp));
		myfile.Close();
		return;
	}
	if (buckno == 33) {  //输出 A 文件路径；
		CString strout = " ( A文件路径： ";
		strout += g_strOperating_32bkPeakFilePath + "** )\r\n";
		myfile.Write(strout,strlen(strout));
		myfile.Close();
		return;
	}
	if (buckno == 34) {  //输出 B 文件路径；
		CString strout = " ( B文件路径： ";
		strout += g_strOperating_32bkPeakFilePath + "** )\r\n";
		myfile.Write(strout,strlen(strout));	

		sprintf(strtemp," ( 数据统计时间： %d年%.2d月%.2d日%.2d:%.2d )\r\n\r\n",timer.GetYear(),timer.GetMonth(),timer.GetDay(),timer.GetHour(),timer.GetMinute());
		myfile.Write(strtemp,strlen(strtemp));
		myfile.Close();
		return;
	}

	if (buckno == -1) {  //输出标题行
		sprintf(strtemp,"   Bucket号   Block号       A文件(峰位)     B文件(峰位)     差值(B-A) \r\n"); 
		myfile.Write(strtemp,strlen(strtemp));
		myfile.Close();
		return;
	}
	
	sprintf(strtemp,"   Bucket%.2d:  block%.2d            %.2d            %.2d            %+.2d \r\n",buckno,0,m_iPeakstat_fileA[buckno][0],m_iPeakstat_fileB[buckno][0],m_iValue_B_A[buckno][0]); 
	myfile.Write(strtemp,strlen(strtemp));
	for(int i=1;i<8;i++) {
		sprintf(strtemp,"              block%.2d            %.2d            %.2d            %+.2d \r\n",i,m_iPeakstat_fileA[buckno][i],m_iPeakstat_fileB[buckno][i],m_iValue_B_A[buckno][i]); 
		myfile.Write(strtemp,strlen(strtemp));
	}

	sprintf(strtemp,"\r\n"); 
	myfile.Write(strtemp,strlen(strtemp));

	myfile.Close();	

}

BOOL CMaintView::OnEraseBkgnd(CDC* pDC) 
{
	
//	return CView::OnEraseBkgnd(pDC);

	return true;
}

void CMaintView::OnSize(UINT nType, int cx, int cy) 
{
	CView::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	
}


void CMaintView::OnMenu32bkParaModify() 
{
	return; //akang 2003.05.21


	CEffectStat effectstat;
	
	effectstat.m_bEncheck = false;
	effectstat.m_bSel_Peak_Stat = false;
	if (IDOK != effectstat.DoModal()) { return;	}

	if (effectstat.m_Effect_SourceFilePath == " ") {
		return;
	}

	CFile srcfile;
	CString strtemp;
	BOOL bFileOpenSuccess = false;
	int StatCounter = 0;
	
	m_fSquareStatValue = 0;
	
	g_strOperating_32bkPeakFilePath = effectstat.m_Effect_SourceFilePath ;
	
	for(int bk=effectstat.m_iStartFileNo;bk<=effectstat.m_iEndFileNo;bk++) {
		bFileOpenSuccess = false;
		strtemp.Format("%.2d.dat",bk);
		if (srcfile.Open(effectstat.m_Effect_SourceFileName+ strtemp,CFile::modeRead)) { 
			srcfile.Close();
			bFileOpenSuccess = true;  //文件都存在			
		}
		if (bFileOpenSuccess) {// 读取 源文件，源文件边界，目标文件；
			StatCounter++;
			g_bOperating_32bkPeakStat = true;
			
			g_strOperating_32bkPeakFilePath = effectstat.m_Effect_SourceFilePath + strtemp;
			
			OnMouseFastCal32EthPeakStat();
			memcpy(&(m_iPeakstat_fileA[bk][0]),&(m_iPeakstat_file_buff[0][0]),8*4);
			
			OnMenu32BuckAnolParaModity(bk, effectstat.m_iRefPeakPosition, g_strOperating_32bkPeakFilePath);

			g_bOperating_32bkPeakStat = false;
		}
	}
	
	char chtemp[50];
	sprintf(chtemp,"  %d Bucket 模拟参数调整完成",StatCounter);
	AfxMessageBox(chtemp);
}

void CMaintView::OnMenu32BuckAnolParaModity(int bucket,int inPeakPosition,CString filename)
{
	CFile datafile;
	unsigned char chartemp[10];
	
	if(datafile.Open(filename,CFile::modeReadWrite)) {			
		for(int block=0;block<8;block++) { 
			if (bucket%2 == 0) {			
				datafile.Seek(PARA_ANOLPARA_DATA_ADDR + block * BLOCK_ANOLPARA_DATA_SIZE,CFile::begin );
			}
			else {
				datafile.Seek(PARA_ANOLPARA_DATA_ADDR + (7 - block)* BLOCK_ANOLPARA_DATA_SIZE,CFile::begin ); 
			}
			datafile.Read(chartemp,8);

			chartemp[0] += inPeakPosition - m_iPeakstat_fileA[bucket][block];
			chartemp[1] += inPeakPosition - m_iPeakstat_fileA[bucket][block];
			chartemp[2] += inPeakPosition - m_iPeakstat_fileA[bucket][block];
			chartemp[3] += inPeakPosition - m_iPeakstat_fileA[bucket][block];
			
			if (bucket%2 == 0) {			
				datafile.Seek(PARA_ANOLPARA_DATA_ADDR + block * BLOCK_ANOLPARA_DATA_SIZE,CFile::begin );
			}else datafile.Seek(PARA_ANOLPARA_DATA_ADDR + (7 - block)* BLOCK_ANOLPARA_DATA_SIZE,CFile::begin ); 
			
			datafile.Write(chartemp,8);			
		}
		datafile.Close();
	}

}

void CMaintView::OnMenu32bkParaManulModify() 
{
	CManulModifyAnolPara modifyAnolPara;

	modifyAnolPara.DoModal();

}



void CMaintView::OnMenuFastopen32file() 
{
	CMaintDoc*  pDoc = GetDocument();	 

	CFileDialog dlg(FALSE,"","");///TRUE为OPEN对话框，FALSE为SAVE AS对话框 
	dlg.m_ofn.lpstrFilter = ("通配符文件名**.dat");//
	if(dlg.DoModal()==IDOK) {
		m_strOpen32FileName=dlg.GetPathName(); 
		m_iOpen32FileNo = 0;
	}
	else return;		

	g_bPLimitChangeValid = false;

	CString strtext;
	CString strfilename;
					
	BOOL bFileValid = false;
	int bucket=0;	
	
	m_b32FileOpenEn = false;
	CFile file;
	while((!bFileValid)&&(bucket < 32)) {
	 	strtext.Format("%2.2d.dat",bucket);
		strfilename = m_strOpen32FileName + strtext;
		if(file.Open(strfilename,CFile::modeRead)) {
			bFileValid = true;	 			
			m_iOpen32FileNo = bucket; 
			g_strFileName =  strfilename; //show file name on status
			bucket = 0;
		}	
		bucket ++;
	}
			 
	if (!bFileValid) {
		AfxMessageBox("选择的文件通配符无效，请重新选择\n注：名字后面不能加数字");
		return;
	}	   
	m_b32FileOpenEn = true;


	CMainFrame* pMainFrame = (CMainFrame*)GetParentFrame();
	CComboBox *pBlockindex = (CComboBox *)
		(pMainFrame)->m_wndShowBar.GetDlgItem(IDC_SHOW_BLOCK_SEL);
	pBlockindex->SetCurSel(0);
	g_Blockindex = 0; 
	UpdateData(false);	

	unsigned char chartemp[128];

	file.SeekToBegin();

	file.Seek(FILEHEAD_TYPE_ADDR,CFile::begin);  //读取 文件类型
	file.Read(chartemp,2);

	BOOL bSampledfile = true;  
//	BOOL bSampledfile = false;
//	if((chartemp[0] == 0x01)&&(chartemp[1] == 0x01)) {
//		bSampledfile = true;  //							//akang 11-13
//	}

	pDoc->bFirstFileOpened = true;
	memset(pDoc->BuffDot3w,0,8*64*64*4);

	int block;
	int i,j;
	
	switch(g_DataSample_Mod_Sel) {
		case 0xc7: 
					//读取模拟数据
					memset(pDoc->m_chrAnolParam,0,8*10);
					file.Seek(PARA_ANOLPARA_DATA_ADDR,CFile::begin);
					for( i=0;i<8;i++) {
						file.Read(chartemp,8);
						for(int k=0;k<8;k++) {
							pDoc->m_chrAnolParam[i][k+1] = chartemp[k];
						}
					}
					//先读取位置边界数据
					file.Seek(PARA_PLIMIT_DATA_ADDR,CFile::begin );	
					for( block=0;block<8;block++) {
						for( i=0;i<8;i++) {
							for( j=0;j<8;j++) {
								file.Read(chartemp,4);			
								m_8Block_Position_X0[block][i][j] = (int)chartemp[0];
								m_8Block_Position_Y0[block][i][j] = (int)chartemp[1];
								m_8Block_Position_X1[block][i][j] = (int)chartemp[2];
								m_8Block_Position_Y1[block][i][j] = (int)chartemp[3];
							}
						}
					}
					m_8Block_Cur_Sel = 0;
					for(i=0;i<8;i++) {
						for( j=0;j<8;j++) {							
							m_Position_X0[i][j] = m_8Block_Position_X0[m_8Block_Cur_Sel][i][j];
							m_Position_Y0[i][j] = m_8Block_Position_Y0[m_8Block_Cur_Sel][i][j];
							m_Position_X1[i][j] = m_8Block_Position_X1[m_8Block_Cur_Sel][i][j];
							m_Position_Y1[i][j] = m_8Block_Position_Y1[m_8Block_Cur_Sel][i][j];
						}
					}

					file.Seek(FILEHEAD_SDOT_ATTR_ADDR,CFile::begin);
					file.Read(chartemp,1);
				//	if(chartemp[0] != 0x01) AfxMessageBox("散点数据无效");

					file.Seek(DATA_SDOT_ADDR,CFile::begin);
					g_chFileType = 0xc7;
					break;
		case 0xc5:	
					//先读取能谱边界
					file.Seek(FILEHEAD_ELIMIT_ATTR_ADDR,CFile::begin );	
					file.Read(chartemp,1);
					if(chartemp[0] != 0x01) {
				//		AfxMessageBox("文件的能量边界数据无效");
				//		return;
					}					
					file.Seek(PARA_ETHLIMIT_DATA_ADDR,CFile::begin );					
					for(block=0;block<8;block++) {
						for(i=0;i<64;i++) {				
							file.Read(chartemp,2);			
							m_iEnergeSpectEth_Low[block][i] = chartemp[0];
							m_iEnergeSpectEth_Hight[block][i] = chartemp[1];				
						}
					}
					
					file.Seek(FILEHEAD_ENERGESPECTRUM_ATTR_ADDR,CFile::begin);
					file.Read(chartemp,1);
				//	if(chartemp[0] != 0x01) AfxMessageBox("能方数据无效");

					file.Seek(DATA_ESPECT_ADDR,CFile::begin);
					g_chFileType = 0xc5;
					break;
		case 0xc6:  
					file.Seek(FILEHEAD_TIMEMODE_ATTR_ADDR,CFile::begin);
					file.Read(chartemp,1);
				//	if(chartemp[0] != 0x01) AfxMessageBox("时方数据无效");			

					file.Seek(DATA_TIMEMODE_ADDR,CFile::begin);
					g_chFileType = 0xc6;
					break;
		case 0xc8:	
					file.Seek(FILEHEAD_FUHEMODE_ATTR_ADDR,CFile::begin);
					file.Read(chartemp,1);
				//	if(chartemp[0] != 0x01) AfxMessageBox("符合数据无效");

					file.Seek(DATA_FUHEMODE_ADDR,CFile::begin);
					g_chFileType = 0xc8;
					break;
		default: g_chFileType = 0x00;
					break;
	}

	if(bSampledfile) {
			for( i=0;i<8;i++)	{	
				for(int j=0;j<64;j++) {
					for(int k=0;k<64;k++) {
						file.Read(chartemp,2);						
						pDoc->BuffDot3w[i][j][k] = chartemp[1];
						pDoc->BuffDot3w[i][j][k] <<= 8;
						pDoc->BuffDot3w[i][j][k] += chartemp[0];
					}
				}
			}
	}

	file.Close();

	memcpy(g_Backup_iMidfilter_sdot,pDoc->BuffDot3w[g_Blockindex],64*64*4);
	
	if(bSampledfile) {
			pDoc->bFileIsSample = true;
			m_bEnergeSpectOperate_En = false;
			//计算能谱数据	
			if(g_DataSample_Mod_Sel == 0xC5) { //能谱文件
				m_bEnergeSpectOperate_En =true;
				memcpy(m_iEnergeSpectrum64,pDoc->BuffDot3w,8*64*64*4);
			}				
			OnRedraw();
	}

	if(g_DataSample_Mod_Sel != 0xC7) {
		return;
	}	
	if (m_ShowModelSel == 1) { //刷新位置边界
		RedrawLimitFile();
	};

}

void CMaintView::OnFastOpen32FileKeyAdd()
{
	CMaintDoc*  pDoc = GetDocument();

	if (!m_b32FileOpenEn) { return;	}	

	g_bPLimitChangeValid = false;

	CString strtext;
	CString strfilename;
					
	BOOL bFileValid = false; 

	int bucket = m_iOpen32FileNo + 1;	
	if (bucket >= 32) {
		bucket = 31;
	}
	
	m_b32FileOpenEn = false;
	CFile file;
	while((!bFileValid)&&(bucket < 32)) {
	 	strtext.Format("%2.2d.dat",bucket);
		strfilename = m_strOpen32FileName + strtext;
		if(file.Open(strfilename,CFile::modeRead)) {
			bFileValid = true;	 			
			m_iOpen32FileNo = bucket; 
			g_strFileName =  strfilename; //show file name on status
			bucket = 0;
		}	
		bucket ++;
	}

	if (bucket >= 32) {
		m_b32FileOpenEn = true;
	}
			 
	if (!bFileValid) {		
		//AfxMessageBox("选择的文件通配符无效，请重新选择\n注：名字后面不能加数字");
		return;
	}	   
	m_b32FileOpenEn = true;

	CMainFrame* pMainFrame = (CMainFrame*)GetParentFrame();
	CComboBox *pBlockindex = (CComboBox *)
		(pMainFrame)->m_wndShowBar.GetDlgItem(IDC_SHOW_BLOCK_SEL);
	pBlockindex->SetCurSel(0);
	g_Blockindex = 0; 
	UpdateData(false);	

	unsigned char chartemp[128];

	file.SeekToBegin();

	file.Seek(FILEHEAD_TYPE_ADDR,CFile::begin);  //读取 文件类型
	file.Read(chartemp,2);


	BOOL bSampledfile = true;  
//	BOOL bSampledfile = false;
//	if((chartemp[0] == 0x01)&&(chartemp[1] == 0x01)) {
//		bSampledfile = true;  //							//akang 11-13
//	}

	pDoc->bFirstFileOpened = true;
	memset(pDoc->BuffDot3w,0,8*64*64*4);

	int block;
	int i,j;
	
	switch(g_DataSample_Mod_Sel) {
		case 0xc7:  
					//读取模拟数据
					memset(pDoc->m_chrAnolParam,0,8*10);
					file.Seek(PARA_ANOLPARA_DATA_ADDR,CFile::begin);
					for( i=0;i<8;i++) {
						file.Read(chartemp,8);
						for(int k=0;k<8;k++) {
							pDoc->m_chrAnolParam[i][k+1] = chartemp[k];
						}
					}
					//先读取位置边界数据
					file.Seek(PARA_PLIMIT_DATA_ADDR,CFile::begin );	
					for( block=0;block<8;block++) {
						for( i=0;i<8;i++) {
							for( j=0;j<8;j++) {
								file.Read(chartemp,4);			
								m_8Block_Position_X0[block][i][j] = (int)chartemp[0];
								m_8Block_Position_Y0[block][i][j] = (int)chartemp[1];
								m_8Block_Position_X1[block][i][j] = (int)chartemp[2];
								m_8Block_Position_Y1[block][i][j] = (int)chartemp[3];
							}
						}
					}
					m_8Block_Cur_Sel = 0;
					for(i=0;i<8;i++) {
						for( j=0;j<8;j++) {							
							m_Position_X0[i][j] = m_8Block_Position_X0[m_8Block_Cur_Sel][i][j];
							m_Position_Y0[i][j] = m_8Block_Position_Y0[m_8Block_Cur_Sel][i][j];
							m_Position_X1[i][j] = m_8Block_Position_X1[m_8Block_Cur_Sel][i][j];
							m_Position_Y1[i][j] = m_8Block_Position_Y1[m_8Block_Cur_Sel][i][j];
						}
					}

					file.Seek(FILEHEAD_SDOT_ATTR_ADDR,CFile::begin);
					file.Read(chartemp,1);
				//	if(chartemp[0] != 0x01) AfxMessageBox("散点数据无效");

					file.Seek(DATA_SDOT_ADDR,CFile::begin);
					g_chFileType = 0xc7;
					break;
		case 0xc5:	
					//先读取能谱边界
					file.Seek(FILEHEAD_ELIMIT_ATTR_ADDR,CFile::begin );	
					file.Read(chartemp,1);
					if(chartemp[0] != 0x01) {
				//		AfxMessageBox("文件的能量边界数据无效");
				//		return;
					}					
					file.Seek(PARA_ETHLIMIT_DATA_ADDR,CFile::begin );					
					for(block=0;block<8;block++) {
						for(i=0;i<64;i++) {				
							file.Read(chartemp,2);			
							m_iEnergeSpectEth_Low[block][i] = chartemp[0];
							m_iEnergeSpectEth_Hight[block][i] = chartemp[1];				
						}
					}


					file.Seek(FILEHEAD_ENERGESPECTRUM_ATTR_ADDR,CFile::begin);
					file.Read(chartemp,1);
				//	if(chartemp[0] != 0x01) AfxMessageBox("能方数据无效");

					file.Seek(DATA_ESPECT_ADDR,CFile::begin);
					g_chFileType = 0xc5;
					break;
		case 0xc6:  
					file.Seek(FILEHEAD_TIMEMODE_ATTR_ADDR,CFile::begin);
					file.Read(chartemp,1);
				//	if(chartemp[0] != 0x01) AfxMessageBox("时方数据无效");			

					file.Seek(DATA_TIMEMODE_ADDR,CFile::begin);
					g_chFileType = 0xc6;
					break;
		case 0xc8:	
					file.Seek(FILEHEAD_FUHEMODE_ATTR_ADDR,CFile::begin);
					file.Read(chartemp,1);
				//	if(chartemp[0] != 0x01) AfxMessageBox("符合数据无效");

					file.Seek(DATA_FUHEMODE_ADDR,CFile::begin);
					g_chFileType = 0xc8;
					break;
		default: g_chFileType = 0x00;
					break;
	}

	if(bSampledfile) {
			for(i=0;i<8;i++)	{	
				for(int j=0;j<64;j++) {
					for(int k=0;k<64;k++) {
						file.Read(chartemp,2);						
						pDoc->BuffDot3w[i][j][k] = chartemp[1];
						pDoc->BuffDot3w[i][j][k] <<= 8;
						pDoc->BuffDot3w[i][j][k] += chartemp[0];
					}
				}
			}

	}

	file.Close();

	memcpy(g_Backup_iMidfilter_sdot,pDoc->BuffDot3w[g_Blockindex],64*64*4);
	
	if(bSampledfile) {
			pDoc->bFileIsSample = true;
			m_bEnergeSpectOperate_En = false;
			//计算能谱数据	
			if(g_DataSample_Mod_Sel == 0xC5) { //能谱文件
				m_bEnergeSpectOperate_En =true;
				memcpy(m_iEnergeSpectrum64,pDoc->BuffDot3w,8*64*64*4);
			}				
			OnRedraw();
	}

	if(g_DataSample_Mod_Sel != 0xC7) {
		return;
	}
	if (m_ShowModelSel == 1) { //刷新位置边界
		RedrawLimitFile();
	};


}

void CMaintView::OnFastOpen32FileKeyDec()
{
	CMaintDoc*  pDoc = GetDocument();

	if (!m_b32FileOpenEn) { return;	}

	g_bPLimitChangeValid = false;

	CString strtext;
	CString strfilename;
					
	BOOL bFileValid = false; 
	int bucket = m_iOpen32FileNo - 1;	
	if (bucket < 0) {  bucket = 0;	}
	
	CFile file;
	while((!bFileValid)&&(bucket >=0)) {
	 	strtext.Format("%2.2d.dat",bucket);
		strfilename = m_strOpen32FileName + strtext;
		if(file.Open(strfilename,CFile::modeRead)) {
			bFileValid = true;	 			
			m_iOpen32FileNo = bucket; 
			g_strFileName =  strfilename; //show file name on status
			bucket = 0;
		}	
		bucket --;
	}
	if (bucket < 0) {
		m_b32FileOpenEn = true;
	}
			 
	if (!bFileValid) {
		//AfxMessageBox("选择的文件通配符无效，请重新选择\n注：名字后面不能加数字");
		return;
	}	   
	
	m_b32FileOpenEn = true;

	CMainFrame* pMainFrame=	(CMainFrame*)GetParentFrame();
	CComboBox *pBlockindex = (CComboBox *)
		(pMainFrame)->m_wndShowBar.GetDlgItem(IDC_SHOW_BLOCK_SEL);
	pBlockindex->SetCurSel(0);
	g_Blockindex = 0; 
	UpdateData(false);



	unsigned char chartemp[128];

	file.SeekToBegin();

	file.Seek(FILEHEAD_TYPE_ADDR,CFile::begin);  //读取 文件类型
	file.Read(chartemp,2);

	BOOL bSampledfile = true;  
//	BOOL bSampledfile = false;
//	if((chartemp[0] == 0x01)&&(chartemp[1] == 0x01)) {
//		bSampledfile = true;  //							//akang 11-13
//	}

	pDoc->bFirstFileOpened = true;
	memset(pDoc->BuffDot3w,0,8*64*64*4);

	int block;
	int i,j;
	
	switch(g_DataSample_Mod_Sel) {
		case 0xc7: 
					//读取模拟数据
					memset(pDoc->m_chrAnolParam,0,8*10);
					file.Seek(PARA_ANOLPARA_DATA_ADDR,CFile::begin);
					for( i=0;i<8;i++) {
						file.Read(chartemp,8);
						for(int k=0;k<8;k++) {
							pDoc->m_chrAnolParam[i][k+1] = chartemp[k];
						}
					}

					//先读取位置边界数据
					file.Seek(PARA_PLIMIT_DATA_ADDR,CFile::begin );	
					for( block=0;block<8;block++) {
						for( i=0;i<8;i++) {
							for( j=0;j<8;j++) {
								file.Read(chartemp,4);			
								m_8Block_Position_X0[block][i][j] = (int)chartemp[0];
								m_8Block_Position_Y0[block][i][j] = (int)chartemp[1];
								m_8Block_Position_X1[block][i][j] = (int)chartemp[2];
								m_8Block_Position_Y1[block][i][j] = (int)chartemp[3];
							}
						}
					}
					m_8Block_Cur_Sel = 0;
					for(i=0;i<8;i++) {
						for( j=0;j<8;j++) {							
							m_Position_X0[i][j] = m_8Block_Position_X0[m_8Block_Cur_Sel][i][j];
							m_Position_Y0[i][j] = m_8Block_Position_Y0[m_8Block_Cur_Sel][i][j];
							m_Position_X1[i][j] = m_8Block_Position_X1[m_8Block_Cur_Sel][i][j];
							m_Position_Y1[i][j] = m_8Block_Position_Y1[m_8Block_Cur_Sel][i][j];
						}
					}

					file.Seek(FILEHEAD_SDOT_ATTR_ADDR,CFile::begin);
					file.Read(chartemp,1);
				//	if(chartemp[0] != 0x01) AfxMessageBox("散点数据无效");

					file.Seek(DATA_SDOT_ADDR,CFile::begin);
					g_chFileType = 0xc7;
					break;
		case 0xc5:	
					//先读取能谱边界
					file.Seek(FILEHEAD_ELIMIT_ATTR_ADDR,CFile::begin );	
					file.Read(chartemp,1);
					if(chartemp[0] != 0x01) {
				//		AfxMessageBox("文件的能量边界数据无效");
				//		return;
					}					
					file.Seek(PARA_ETHLIMIT_DATA_ADDR,CFile::begin );					
					for(block=0;block<8;block++) {
						for(i=0;i<64;i++) {				
							file.Read(chartemp,2);			
							m_iEnergeSpectEth_Low[block][i] = chartemp[0];
							m_iEnergeSpectEth_Hight[block][i] = chartemp[1];				
						}
					}					
					//设定读取能谱数据的起点
					file.Seek(FILEHEAD_ENERGESPECTRUM_ATTR_ADDR,CFile::begin);
					file.Read(chartemp,1);
				//	if(chartemp[0] != 0x01) AfxMessageBox("能方数据无效");

					file.Seek(DATA_ESPECT_ADDR,CFile::begin);
					g_chFileType = 0xc5;
					break;
		case 0xc6:  
					file.Seek(FILEHEAD_TIMEMODE_ATTR_ADDR,CFile::begin);
					file.Read(chartemp,1);
				//	if(chartemp[0] != 0x01) AfxMessageBox("时方数据无效");			

					file.Seek(DATA_TIMEMODE_ADDR,CFile::begin);
					g_chFileType = 0xc6;
					break;
		case 0xc8:	
					file.Seek(FILEHEAD_FUHEMODE_ATTR_ADDR,CFile::begin);
					file.Read(chartemp,1);
				//	if(chartemp[0] != 0x01) AfxMessageBox("符合数据无效");

					file.Seek(DATA_FUHEMODE_ADDR,CFile::begin);
					g_chFileType = 0xc8;
					break;
		default: g_chFileType = 0x00;
					break;
	}

	if(bSampledfile) {
			for( i=0;i<8;i++)	{	
				for(int j=0;j<64;j++) {
					for(int k=0;k<64;k++) {
						file.Read(chartemp,2);						
						pDoc->BuffDot3w[i][j][k] = chartemp[1];
						pDoc->BuffDot3w[i][j][k] <<= 8;
						pDoc->BuffDot3w[i][j][k] += chartemp[0];
					}
				}
			}

	}

	file.Close();

	memcpy(g_Backup_iMidfilter_sdot,pDoc->BuffDot3w[g_Blockindex],64*64*4);
	
	if(bSampledfile) {
			pDoc->bFileIsSample = true;
			m_bEnergeSpectOperate_En = false;
			//计算能谱数据	
			if(g_DataSample_Mod_Sel == 0xC5) { //能谱文件
				m_bEnergeSpectOperate_En =true;
				memcpy(m_iEnergeSpectrum64,pDoc->BuffDot3w,8*64*64*4);
			}				
			OnRedraw();
	}

	if(g_DataSample_Mod_Sel != 0xC7) {
		return;
	}
	if (m_ShowModelSel == 1) { //刷新位置边界
		RedrawLimitFile();
	};
	
}

void CMaintView::OnMenuFastopen32fileNext() 
{
	OnFastOpen32FileKeyAdd();
	
}

void CMaintView::OnMenuFastopen32fileLast() 
{
	OnFastOpen32FileKeyDec();	
}

void CMaintView::m_Show64EngerSpectrum()
{
	CMaintDoc*  pDoc = GetDocument(); 
	
	m_bEnergeSpectOperate_En = true;
	memcpy(m_iEnergeSpectrum64,pDoc->BuffDot3w,8*64*64*4);
	OnDraw64EngerSpectrum();
	m_bEnergeSpectSelEn = true;
	m_bEnergeSpectZoomInMode =false;
				
	m_ShowModelSel = 2;
}

void CMaintView::OnFastCalcEthLimit32Buck()
{
	CString strtype;

	if(IDYES != AfxMessageBox("确定要计算并保存32个Bucket 能谱边界？",MB_YESNO))
		return;	

	CFileDialog dlg(FALSE,"","");///TRUE为OPEN对话框，FALSE为SAVE AS对话框 
	dlg.m_ofn.lpstrFilter = ("通配符文件名**.dat");//
	if(dlg.DoModal()==IDOK) {
		m_strOpen32FileName=dlg.GetPathName(); 
	}
	else return;		
	
	CString strtext;
	CString strfilename;
	int iSuccessNum = 0;
	
	CFile file;
	for(int buck=0;buck < 32;buck ++) {
		strtext.Format("%2.2d.dat",buck);
		strfilename = m_strOpen32FileName + strtext;
		if(file.Open(strfilename,CFile::modeRead)) {
			file.Close();
			iSuccessNum ++;
			//读数据并处理保存
			OnDeal2Save32EnergerLimit(strfilename);			
		}	
	}
	strtext.Format("完成计算并保存 \n%d 个 Bucket 能谱边界！",iSuccessNum);

	for(int i=0;i<8;i++){	//能谱阈值 初始化
		for(int j=0;j<64;j++) {
			m_iEnergeSpectEth_Low[i][j] = 0;
			m_iEnergeSpectEth_Hight[i][j] = 63;
		}
	}

	AfxMessageBox(strtext);
}


BOOL CMaintView::OnDeal2Save32EnergerLimit(CString filename)
{

	CFile file;
	if( ! file.Open(filename,file.CFile::modeReadWrite)) return false;

	file.Seek(DATA_ESPECT_ADDR,CFile::begin);
	unsigned char chtemp[10];
	for( int i=0;i<8;i++)	{	
		for(int j=0;j<64;j++) {
			for(int k=0;k<64;k++) {
				file.Read(chtemp,2);
				type.uchdata[0] = chtemp[0];
				type.uchdata[1] = chtemp[1];
				m_iEnergeSpectrum64[i][j][k] = type.intdata;
			}
		}
	}
	m_bCalc32BuckEnergeLimit = true;
	
	OnFastCalcEthLowHighLimit();  //计算单个Bucket 的能谱边界

	//保存能谱数据
	unsigned char filehead[128];
	file.SeekToBegin();
	filehead[0] = 0x01;
	file.Seek(FILEHEAD_ELIMIT_ATTR_ADDR,CFile::begin );	
	file.Write(filehead,1);
	
	file.Seek(PARA_ETHLIMIT_DATA_ADDR,CFile::begin);
	unsigned char temp[4]={0,0,0,0};
	
	for(int block=0;block<8;block++) {
		for(int i=0;i<64;i++) {	
			if(m_iEnergeSpectEth_Low[block][i] > m_iEnergeSpectEth_Hight[block][i])	{
				temp[1] = (unsigned char)m_iEnergeSpectEth_Low[block][i];
				temp[0] = (unsigned char)m_iEnergeSpectEth_Hight[block][i];
			} 
			else {
				temp[0] = (unsigned char)m_iEnergeSpectEth_Low[block][i];
				temp[1] = (unsigned char)m_iEnergeSpectEth_Hight[block][i];
			}					
			file.Write(temp,2);				
		}	
	}

	m_bCalc32BuckEnergeLimit = false;
	file.Close();

	return true;
}

void CMaintView::OnRButtonDblClk(UINT nFlags, CPoint point) 
{
	CView::OnRButtonDblClk(nFlags, point);
}

void CMaintView::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	CString strtext;
	CView::OnLButtonDown(nFlags, point);
	CMainFrame* pMainFrame =	(CMainFrame*)GetParentFrame();

	CComboBox *pBlockindex = (CComboBox *)(pMainFrame)->m_wndShowBar.GetDlgItem(IDC_SHOW_BLOCK_SEL);
	
	m_Oldpoint = point;
	
	int nBlindex= pBlockindex->GetCurSel();
	if(nBlindex == -1) nBlindex = 0;
	
	if(m_bEnergeSpectSelEn) { // 能谱操作 选择
		int barsize_x = 70;
		int barsize_y = 55;
		int barstart_x = 50;
		int barstart_y = 20;
		int selnum;
		bool flag = false;
		for(int i=0;i<8;i++) { //水平
			if((point.x > barstart_x +i*100)&&(point.x < barstart_x +i*100 + 68)) {
				for(int j=0;j<8;j++) {
					if((point.y > barstart_y +j*80)&&(point.y < barstart_y +j*80 + 55)) {
						selnum = i + j*8;
						flag = true;						
					}
				}
			}			
		}
		if(flag) {
			m_iEnergeSpectCurBlockNo = nBlindex;
			m_iEnergeSpectCurSynNo = selnum; //当前晶体号
			OnDrawZoomInEnergeSpect(nBlindex,selnum);
			return;
		}			
		m_iEnergeSpectCurBlockNo = nBlindex;
	}
}

void CMaintView::OnSet32EnergeLimitPara()
{
	CSet64EnergeLimit set64EnergLimit;

	memcpy(set64EnergLimit.m_i64EthPara,m_i64EthPara,6*3*4);
	if(IDOK == set64EnergLimit.DoModal()) {
		memcpy(m_i64EthPara,set64EnergLimit.m_i64EthPara,6*3*4);	
	};
}


void CMaintView::OnFileSaveEnergeLimit() 
{
	g_bPLimitChangeValid = false;

	RedrawLimitFile();

	CFile savefile;
	if( !savefile.Open(g_strFileName, CFile::modeReadWrite) ) {	
		AfxMessageBox("文件保存错误");
		return;
	}

	unsigned char filehead[128];
	savefile.SeekToBegin();
	filehead[0] = 0x01;
	savefile.Seek(FILEHEAD_PLIMIT_ATTR_ADDR,CFile::begin );	
	savefile.Write(filehead,1);

	savefile.Seek(PARA_PLIMIT_DATA_ADDR,CFile::begin);
	unsigned char temp[4]={0,0,0,0};
	
	for(int block=0;block<8;block++) {
		for(int i=0;i<8;i++) {
			for(int j=0;j<8;j++) {	
				temp[0] = m_8Block_Position_X0[block][i][j];
				temp[1] = m_8Block_Position_Y0[block][i][j];
				temp[2] = m_8Block_Position_X1[block][i][j];
				temp[3] = m_8Block_Position_Y1[block][i][j];
				savefile.Write(temp,4);
			}
		}	
	}
	savefile.Close();

}

void CMaintView::OnOutput32bklpEvaluate() 
{
	CMaintDoc*  pDoc = GetDocument();

	CString filepathname;
	CFileDialog dlg(FALSE,"","");///TRUE为OPEN对话框，FALSE为SAVE AS对话框 
	dlg.m_ofn.lpstrFilter = ("通配符文件名**.dat");//
	if(dlg.DoModal()==IDOK) {
		filepathname=dlg.GetPathName(); 
	}
	else return;


	CFile srcfile;
	CFile destfile;
	CString strtemp;
	BOOL bFileOpenSuccess = false;
	for(int bk=0;bk<32;bk++) {

		bFileOpenSuccess = false;
		strtemp.Format("%.2d.dat",bk);
		if (srcfile.Open(filepathname+ strtemp,CFile::modeRead)) { 
			srcfile.Close();	
			bFileOpenSuccess = true;
		}

		if (bFileOpenSuccess) {// 读取 源文件，源文件边界，目标文件；
			g_bOperating_32bkEffectEvaluate = true;	
			g_strOperating_32bkEffectFilePath = filepathname + strtemp;

			pDoc->OnFileOpen();  //打开第一文件A
			
			OnRedraw();
			
			OnOpenLimittab();  //导入位置边界
			
			RedrawLimitFile();
			
			OnRedraw();
			
			OnEffectCalFirstEvalute();			
			
	    	g_bOperating_32bkEffectEvaluate = true;	
			for(int block=0;block<8; block++) {
				g_iOperatingEffect_block = block;
				OnRedraw();				
				RedrawLimitFile();
				int onetobuff[64];
				for(int k=0;k<8;k++){
					for(int j=0;j<8;j++) {
						onetobuff[k*8+j] = m_Efficent_Evalu_FileA_data[block][k][j];
					}
				}
				OnOutput32BKEvelute(filepathname,bk,block,onetobuff,g_fEffect32bk_Evalu_SquE,true);

			//	OnEffecentEvaluat();  // g_iOperatingEffect_block  效率统计
			//	OnOutput32BKEvelute(filepathname,bk,block,m_Efficent_Evalu_FileA_data,g_fEffect32bk_Evalu_SquE,true);
			}
			g_bOperating_32bkEffectEvaluate = false;	
		
		}
	}
	AfxMessageBox(" 效率统计完成 ");
	g_bOperating_32bkEffectEvaluate = false;
	
}

void CMaintView::OnOutput32BKEvelute(CString outfilename,int buck,int block,int *effectdata, float effectSquareSalue,BOOL boutRelativevalue)
{
	char strtemp[100];
	CFile myfile;
	BOOL bfileopensucc = true;	
	if(!myfile.Open(outfilename + ".txt",CFile::modeRead)) {
		bfileopensucc = false;		
	}
	if (bfileopensucc) {		
		myfile.Close();
	}
	
	if (!bfileopensucc) {
		myfile.Open(outfilename+ ".txt",CFile::modeCreate | CFile::modeWrite);
		strcpy(strtemp,"                  效率 低于30% 统计结果       \r\n\r\n"); 
		myfile.Write(strtemp,strlen(strtemp));

		CTime  time = CTime::GetCurrentTime(); 
		sprintf(strtemp, "统计时间：%.2d月%.2d日 %.2d:%.2d   \r\n", time.GetMonth(),time.GetDay(),time.GetHour(),time.GetMinute());
		myfile.Write(strtemp,strlen(strtemp));

		strcpy(strtemp,"//-------------------------------------------//\r\n"); 
		myfile.Write(strtemp,strlen(strtemp));
		
	}
	else {
		myfile.Open(outfilename+ ".txt",CFile::modeReadWrite);
		myfile.Seek(0,CFile::end);
	}

	if(boutRelativevalue) {

		for(int i=0;i<8;i++) {	
			for(int kk=0;kk<8;kk++) {
				if(effectdata[8*i + kk] < 30) {
					sprintf(strtemp,"Bucket%.2d  block %.2d   条号：%.2d   效率： %d   \r\n",buck,block,8*i + kk, effectdata[8*i + kk] );
					myfile.Write(strtemp,strlen(strtemp));
				}
			}				
		}				
	}

	if(block == 7) {
		strcpy(strtemp,"//-------------------------------------------//\r\n"); 
		myfile.Write(strtemp,strlen(strtemp));
	}
	
	myfile.Close();	
	return ;
}


