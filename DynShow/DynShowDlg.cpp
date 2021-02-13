// DynShowDlg.cpp : implementation file
//

#include "stdafx.h"
#include "DynShow.h"
#include "DynShowDlg.h"
#include "Fast57600.h"
#include "ComSel.h"
#include "PeakVale.h"
#include "EnergeSpectEth.h"
#include "ProgContrl.h"
//#include "EZ_USBAPI.h"

#include "EzusbApi.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//#define TEST_USB_SPEED

// �����ٶ��õ�
DWORD dwTime1;
DWORD dwTime2;
LONG  g_TotalNum;


int g_draw_size = 8;
int g_draw_frame = 5;
int g_draw_startpoint_x = 80;
int g_draw_startpoint_y = 80;


int nBlindex = 0;

CWinThread *m_pThread_Comm;
UINT  ReadDataProc(LPVOID  lpData);
HANDLE hReadHandleBLocal;
bool g_bRunCommThread;
extern int g_ComPortNo;

HANDLE  hUsb;

union TYPE 
{
	UINT intdata;
	unsigned char uchdata[4];
};

TYPE type;


/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP

	
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDynShowDlg dialog

CDynShowDlg::CDynShowDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDynShowDlg::IDD, pParent)
{
	m_FileName1 = _T("û�е����ļ�1");
	m_FileName2 = _T("û�е����ļ�2");

	m_RecFastData = new BYTE [100*1024];



	//{{AFX_DATA_INIT(CDynShowDlg)
	m_strSampleTime = _T("");
	m_iSampleTimeCounter = 0;
	m_bSampleTimeMode = true;
	m_strSampleShowMode = _T("ʱ��(s):");
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

}

void CDynShowDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDynShowDlg)
	DDX_Control(pDX, IDC_CHECK_MIDFILTER, m_Check_Midfilter);
	DDX_Control(pDX, IDC_EN_LIMIT_POSITION_WHOLE_MOVE, m_EnLPWholeLineMove);
//	DDX_Control(pDX, IDC_COMBO_PROG_CONTRL_SEL, m_ProgCtrl_Sel);
	DDX_Control(pDX, IDC_ENERGER_SPECTRUM_SEL, m_EnergeSpectrum_Sel);
	DDX_Text(pDX, IDC_SAMPING_TIME, m_strSampleTime);
	DDX_Text(pDX, IDC_STATIC_SAMPLE_MODE, m_strSampleShowMode);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDynShowDlg, CDialog)
	//{{AFX_MSG_MAP(CDynShowDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_REDRAW, OnRedraw)
	ON_BN_CLICKED(IDC_OPEN_FILE, OnOpenFile)
	ON_BN_CLICKED(IDC_ENERGE_CHART_REDRAW, OnEnergChartRedraw)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_SAVE_FILE, OnSaveFile)
	ON_BN_CLICKED(IDC_START_SAMPLE, OnStartSample)
	ON_BN_CLICKED(IDC_STOP_SAMPLE, OnStopSample)
	ON_WM_KEYUP()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_BN_CLICKED(IDC_ENERGER_SPECTRUM_SEL, OnEnergerSpectrum_Sel)
	ON_BN_CLICKED(IDC_X_DIRECTION_SEL, OnXDirection_Sel)
	ON_BN_CLICKED(IDC_Y_DIRECTION_SEL, OnYDirection_Sel)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_PROG_CONTROL, OnProgControl)
	ON_WM_RBUTTONDOWN()
	ON_BN_CLICKED(IDC_SAMPLE_MODE_SEL, OnSampleModeSel)
	ON_BN_CLICKED(IDC_TIME_SPECTRUM_SEL, OnTimeSpectrumSel)
	ON_BN_CLICKED(IDC_RADIO1, On2d_SynEnergeSpectrumSel)
	ON_BN_CLICKED(IDC_ADDSYN_NO, OnAddSynEnergSpectrumNo)
	ON_BN_CLICKED(IDC_ETH_RESET, OnEthReset)
	ON_BN_CLICKED(IDC_EN_LIMIT_POSITION_WHOLE_MOVE, OnEnLimitPositionWholeMove)
	ON_BN_CLICKED(IDC_PEAK_VALE_STAT, OnPeakValeStat)
	ON_BN_CLICKED(IDC_LIMIT_STAT, OnLimitStat)
	ON_BN_CLICKED(IDC_ENERSPECT_64ETH, OnEnerspect64eth)
	ON_BN_CLICKED(IDC_CHECK1, OnCheck1)
	ON_WM_KEYDOWN()
	ON_BN_CLICKED(IDC_CHECK_MIDFILTER, OnCheckMidfilter)
	ON_BN_CLICKED(IDC_CLEAR_DATA, OnClearData)
	ON_WM_SETCURSOR()
	ON_BN_CLICKED(IDC_TIME_FUHE, OnTimeFuheSel)
	//}}AFX_MSG_MAP

	ON_MESSAGE(WM_RECVEIVE_FULL,OnRecveive)


END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDynShowDlg message handlers

BOOL CDynShowDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	// Add "About..." menu item to system menu.
	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here

	GetDlgItem(IDC_STOP_SAMPLE)->EnableWindow(false);
//	GetDlgItem(IDC_STAT_SHOW_MAP)->EnableWindow(false);

//	GetDlgItem(IDC_RADIO_SDOT_FILE1)->EnableWindow(false);
//	GetDlgItem(IDC_RADIO_SDOT_FILE2)->EnableWindow(false);
//	GetDlgItem(IDC_RADIO_SDOT_FILECOMPARE)->EnableWindow(false);


	SetDlgItemText(IDC_STATIC_CONTER,"���ռ�������0 ");
	SetDlgItemText(IDC_SMALL_MAP_TITILE,"������ʾͼ ");
	SetDlgItemText(IDC_3D_SHOW_THREVALUE,"100");

	
	SetDlgItemText(IDC_ENERGE_LEFT_LIMIT,"5");
	SetDlgItemText(IDC_ENERGE_RIGHT_LIMIT,"61");

//	SetDlgItemText(IDC_MIDFILTER_AUTHOR," 1 ");

	//λ�ñ߽�ƽ�Ʋ���
	m_EnLPWholeLineMove.SetCheck(1);  
	m_bMoveWholeLine = true;
	m_bMovingVerDirection = true;
	m_bEnMoveBreakLine = false;

	m_bCompareFinish = false;

	m_bEnMidFilter = false; //��ֵ�˲� ȡ��
	memset(m_Backup_iMidfilter_sdot,0,64*64*4);
	memset(m_iMidfilter_sdot,0,64*64*4);


	m_EnergeSpectrum_Sel.SetCheck(1);
	m_iSmallMap_Show_Sel = 1;
	m_RecFastCounter = 0;
	m_reccounter = 0;

	bAutoRedraw_down = false;
	bredraw2D_Energe = false;
	ileft_limit = 5;
	iright_limit = 61;
	m_bMove_LeftLimit = false;
	m_bMove_RightLimit = false;

	m_bSelEthLimit_HandleInput = true; //

	m_bSelShowEnergeSpectrum = false;  //Ĭ����ʾ ɢ����ͼ��

	//�ļ�������ʼ��
	fileoper_Newfile = true;
	fileoper_iblocksel = 0;

	m_CurFileName = ""; // �����ļ���ʼ��Ϊ��


	for(int i=0;i<64;i++){			//���64��64 ������	
		for(int j=0;j<64;j++){
			m_sdot[i][j] = 0;	//ɢ������
			m_isdot_datafile[i][j] = 0;
			m_iEth_datafile[i][j] = 0; //������������
		}
		m_Energer_Spectrum[i] = 0;
	}
	for(i=0;i<128;i++) {    //ʱ����
		m_Time_Spectrum[i] = 0;
	}

	for(i=0;i<64;i++) {
		m_iEnergeSpectEth_Low[i] = 0;
		m_iEnergeSpectEth_Hight[i] = 63;
	}
	

//	CString strprog;
	int progdata[10] = {255,105,105,105,105,127,127,127,88,00};	
	memcpy(m_iProgCtrlData,progdata,10*4);
//	strprog.Format("%d",progdata[0]);
//	SetDlgItemText(IDC_PROG_CTRL_DATA,strprog);
//	m_ProgCtrl_Sel.SetCurSel(0); //

	//-------��ɢ��߽紦��----------//
	m_bButtonDown = false;
	m_bMoveLongLine = false;
	m_bSameDriction_X = true;
	m_bShort_Line_Valid = false;
	m_bthisMoveInValid = false;

	for(i=0;i<8;i++) {
		for(int j=0;j<8;j++) {
			m_Position_X0[i][j] = j*8; 
			m_Position_Y0[i][j] = i*8;
			m_Position_X1[i][j] = (j+1)*8 -1;
			m_Position_Y1[i][j] = (i+1)*8 -1;
		}
	}
	
	m_bEnShowPeakValeRatio = false;

	filesave.bEnReadOperate = false;

	memset(m_arrTimeFuheData,0,256*4);

	m_iWorkMode = 0xAA; // ͨ������ģʽѡ��
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CDynShowDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CDynShowDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}


		OnRedraw_2d();
		OnRedraw_3d();

}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CDynShowDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CDynShowDlg::OnRedraw_3d() 
{
	m_bEnShowPeakValeRatio = false;

	long temp;
	CEdit* pThreshvalue=(CEdit*)GetDlgItem(IDC_3D_SHOW_THREVALUE);

	if(m_bSelShowEnergeSpectrum) { memcpy(m_sdot,m_iEth_datafile,64*64*4); }
	else memcpy(m_sdot,m_isdot_datafile,64*64*4);

	CDC* pDC=GetDC();			
	CRect clientrect;
	GetClientRect(clientrect);			
	CDC MemDC; //���ȶ���һ����ʾ�豸����
	CBitmap MemBitmap;//����һ��λͼ����	
	MemDC.CreateCompatibleDC(NULL);
	MemBitmap.CreateCompatibleBitmap(pDC,clientrect.Width(),clientrect.Height()); //�Զ���һ��ͼ��Ĵ�С
	CBitmap *pOldBit=MemDC.SelectObject(&MemBitmap);	

	CPen pShow5(PS_SOLID,2,RGB(219,206,198));//
	CPen* pOldPen=NULL;
	pOldPen=MemDC.SelectObject(&pShow5);
	
	CBrush bShow;			
	bShow.CreateSolidBrush(RGB(219,206,198));// 			 					  
	CBrush* pOldBrush=MemDC.SelectObject(&bShow);// 
	MemDC.Rectangle(45,45,615,615);// ˢ��С���ڵĴ�С45,45,557,557
	MemDC.SelectObject(pOldBrush);//++++
	bShow.DeleteObject();

	MemDC.SetBkColor(RGB(219,206,198));	

    MemDC.SetTextColor(RGB(0,128,0));
	MemDC.TextOut(130,56,"[ D ]");
	MemDC.TextOut(480,56,"[ B ]");
	MemDC.TextOut(130,587,"[ C ]");
	MemDC.TextOut(480,587,"[ A ]");

	MemDC.SetTextColor(RGB(0,0,0));
	CPen pShow6(PS_SOLID,1,RGB(0,0,0));//���ó� ��ɫ������
	pOldPen=MemDC.SelectObject(&pShow6);

	MemDC.TextOut(170,48,"  --------------------   ˮƽ (X) ����   -------------------> ");
	MemDC.MoveTo(55,105);
	MemDC.LineTo(55,190);
	MemDC.TextOut(50,200,"��");
	MemDC.TextOut(50,220,"ֱ");
	MemDC.TextOut(50,240,"��");
	MemDC.TextOut(50,260,"��");
	MemDC.TextOut(52,360,"V");
	MemDC.MoveTo(55,280);
	MemDC.LineTo(55,365);

	   CString m_strEditThresh;
	   pThreshvalue->GetWindowText(m_strEditThresh);
	   if(!m_strEditThresh.IsEmpty())
	   {		
            temp=atol(m_strEditThresh); 	        
	   }
	   else
	   {	//�����ֵ
		   //temp = m_BuffDot3w[nBlindex][0][0];  //�ҵ� 0 ������
		   temp = m_sdot[0][0];  //�ҵ� 0 ������
		   for(int j=0;j<64;j++) {
				for(int k=0;k<64;k++) {
						if(m_sdot[j][k] > temp)
							temp = m_sdot[j][k];
				}
		   }
	   }
	   if(temp <= 63) temp =63;

	   char chrbuff[20];
	   sprintf(chrbuff,"3D��ֵ: %d",temp);
	   SetDlgItemText(IDC_STATIC_ETH_VALUE,chrbuff);

	   int y = g_draw_startpoint_x;	 
	   for(int j=0;j<64;j++) {
		    int x = g_draw_startpoint_y;
		   	for(int k=0;k<64;k++) {
					CBrush bShow;			
					bShow.CreateSolidBrush(RGB(GetBValue(ColorSet(m_sdot[j][k],temp)), 
				    GetGValue(ColorSet(m_sdot[j][k],temp)),
				    GetRValue(ColorSet(m_sdot[j][k],temp))));// 30003000				 					  
					CBrush* pOldBrush=MemDC.SelectObject(&bShow);// 3000 
					MemDC.Rectangle(x-g_draw_frame,y-g_draw_frame,x+g_draw_frame,y+g_draw_frame);// 
					
					MemDC.SelectObject(pOldBrush);//++++
					bShow.DeleteObject();
					
					x += g_draw_size;
			}
			y += g_draw_size;
	   }

	   pDC->BitBlt(45,45,560,560,&MemDC,45,45,SRCCOPY);	
	   MemBitmap.DeleteObject();
	   MemDC.DeleteDC();
	   
	   ReleaseDC(pDC);

}

long CDynShowDlg::ColorSet(long nCol, long nu)
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

    if (nu<64) { nu = 64; };
	if (nCol >= nu) { return colorNumber[63]; }
	else return colorNumber[64*nCol/nu];
}

void CDynShowDlg::OnRedraw() 
{
	// TODO: Add your control notification handler code here

	if(m_iSdotFileSel == 1) {	//ͳ�� ɢ���ļ�1 
		SetDlgItemText(IDC_STATIC_FILE_NAME,m_FileName1);
		Sleep(10);

		for(int i=0;i<64;i++) {
			for(int j=0;j<64;j++) {
				//m_sdot[i][j] = m_sdot_file1[i][j];
				m_isdot_datafile[i][j] = m_sdot_file1[i][j];
			}
		}
	}
	else if(m_iSdotFileSel == 2) {	//ͳ�� ɢ���ļ�2 
			SetDlgItemText(IDC_STATIC_FILE_NAME,m_FileName2);
			Sleep(10);
			for(int i=0;i<64;i++) {
				for(int j=0;j<64;j++) {
					//m_sdot[i][j] = m_sdot_file2[i][j];
					m_isdot_datafile[i][j] = m_sdot_file2[i][j];
				}
			}
		} 
		else {
			//SetDlgItemText(IDC_STATIC_FILE_NAME, "����ˢ��");
		}


	OnRedraw_3d();

}

void CDynShowDlg::OnOpenFile() 
{
	unsigned char manydata[1024*1];

	CString FilePathName; 
	CFileDialog dlg(TRUE,"dat","*.dat");///TRUEΪOPEN�Ի���FALSEΪSAVE AS�Ի��� 

	dlg.m_ofn.lpstrFilter = ("(ɢ��ͼ����) ""*.dat");//
	if(dlg.DoModal()==IDOK) FilePathName=dlg.GetPathName(); 
	else return;
	m_CurFileName = FilePathName; // ���µ�ǰ�����ļ������ڱ����ļ�ʱ��

	SetDlgItemText(IDC_STATIC_FILE_NAME,FilePathName);
	CFile file;
	int filelenth;
	file.Open(FilePathName,CFile::modeRead);
	filelenth = file.GetLength();
	long datacounter;
	unsigned char byteX[1],byteY[1],byteE[1],byteT[1];
	datacounter = filelenth/4;

	if((filelenth%4) != 0) {
		AfxMessageBox("�ļ����Ȳ���.");
		return;
	}

	m_Check_Midfilter.SetCheck(0);
	m_bEnMidFilter = false;

	memset(m_sdot,0,64*64*4);
	memset(m_isdot_datafile,0,64*64*4);
	memset(m_iEth_datafile,0,64*64*4);
	memset(m_Energer_Spectrum,0,64*4);
	memset(m_Time_Spectrum,0,128*4);


	file.Read(byteX,1);
	file.Read(byteY,1);
	file.Read(byteE,1);
	file.Read(byteT,1);

	//ʱ��������ݵ� ��ȡ�ж�
	if (m_iSmallMap_Show_Sel == SEL_2D_TIME_FUHE) { //ʱ�������ʾ
		memset(m_arrTimeFuheData,0,256*4);
		file.Seek(128,CFile::begin);
		int num=0;
		int realcounter;
		while (num < filelenth-128) {
			realcounter = filelenth-128-num;
			if( realcounter >= 1024*1) {
				file.Read(manydata,1024*1);
				num += 1024*1;
				realcounter = 1024*1;
			}
			else {  
				file.Read(manydata,filelenth-128-num);
				num += filelenth-128-num;
				realcounter = filelenth-128-num;
			}
			OnAnalyzeTimeFuheData((char*)manydata,realcounter);
		}
        
		OnRedraw_2d(); //�˴�ֻˢ���� 2D ͼ��	
		return ;
	}


	if((byteX[0]==0x55)&&(byteY[0]==0x55)&&(byteE[0]==0x55)&&(byteT[0]==0x55)) {  // ���ļ�ͷ��ԭʼ����

		unsigned char chartemp[128];		
		file.Read(chartemp,124);		
		for(int i=0;i<9;i++) {
			m_iProgCtrlData[i] = chartemp[96+i];
		}

		//-----------------�� ��ȡ�� �ļ�ͷ��Դ�ļ� ��------------------------------------------
			i=0;
			int x_No = 3,y_No = 3;
			BOOL flagx = true;
			BOOL flagy = true;			

			int num =0;
			int realcounter;
			while (num < filelenth-128) {
				realcounter = filelenth-128-num;
				if( realcounter >= 1024*1) {
					file.Read(manydata,1024*1);
					num += 1024*1;
					realcounter = 1024*1;
				}
				else {  
					file.Read(manydata,filelenth-128-num);
					num += filelenth-128-num;
					realcounter = filelenth-128-num;
				}

				for(int i=0;i<realcounter/4;i++) {
					byteX[0] = (manydata[4*i] >>1)&0x3f;
					byteY[0] = (manydata[4*i+1] >>1)&0x3f;
					byteE[0] = (manydata[4*i+2] >>1)&0x3f;
					byteT[0] = (manydata[4*i+3]&0x7f);

					if((byteE[0] >= ileft_limit)&&(byteE[0] <= iright_limit)&&(m_bSelEthLimit_HandleInput)) {
						m_isdot_datafile[byteY[0]][byteX[0]] ++;
						m_Time_Spectrum[byteT[0]] ++;
					}
					m_Energer_Spectrum[byteE[0]] ++;

					//------------------------���㾧������----------//
					x_No = 3,y_No = 3;
					flagx = true;	flagy = true;
					while((flagx)||(flagy)) {
						if(byteX[0] < m_Position_X0[y_No][x_No]) { if((--x_No) < 0) flagx = false; }
						else if(byteX[0] > m_Position_X1[y_No][x_No]) {  if((++x_No) > 7) flagx = false;}
						else flagx = false;
							
						if(byteY[0] < m_Position_Y0[y_No][x_No]) { if((--y_No) < 0) flagy = false; }
						else if(byteY[0] > m_Position_Y1[y_No][x_No]) { if((++y_No) > 7) flagy = false; }
						else flagy = false;
					} 
					if((byteE[0] > m_iEnergeSpectEth_Low[y_No*8 + x_No])&&(byteE[0] < m_iEnergeSpectEth_Hight[y_No*8 + x_No])) {
						m_iEth_datafile[y_No*8 + x_No][byteE[0]] ++;
                    }

					if(!m_bSelEthLimit_HandleInput) {			
						if((byteE[0] > m_iEnergeSpectEth_Low[y_No*8 + x_No])&&(byteE[0] < m_iEnergeSpectEth_Hight[y_No*8 + x_No])) {
							m_isdot_datafile[byteY[0]][byteX[0]] ++;				
						}			
					}
				}
			}

			file.Close();

			OnRedraw_3d();
			OnRedraw_2d();
		return;
		//---------------�� end  ��ȡ�� �ļ�ͷ��Դ�ļ� ��----------------------------------------------		
	}	


		int i=1;
		int x_No = 3,y_No = 3;
		BOOL flagx = true;
		BOOL flagy = true;

		byteX[0] = (byteX[0] >>1)&0x3f;
		byteY[0] = (byteY[0] >>1)&0x3f;
		byteE[0] = (byteE[0] >>1)&0x3f;
		byteT[0] = (byteT[0]&0x7f);
		if((byteE[0] >= ileft_limit)&&(byteE[0] <= iright_limit)&&(m_bSelEthLimit_HandleInput)) {
			//m_sdot[byteX[0]][byteY[0]] ++;
			m_isdot_datafile[byteY[0]][byteX[0]] ++;
			m_Time_Spectrum[byteT[0]] ++;
		}
		m_Energer_Spectrum[byteE[0]] ++;

		//------------------------���㾧������----------//
		x_No = 3,y_No = 3;
		flagx = true;	flagy = true;
		while((flagx)||(flagy)) {
			if(byteX[0] < m_Position_X0[y_No][x_No]) { if((--x_No) < 0) flagx = false; }
			else if(byteX[0] > m_Position_X1[y_No][x_No]) {  if((++x_No) > 7) flagx = false;}
			else flagx = false;
				
			if(byteY[0] < m_Position_Y0[y_No][x_No]) { if((--y_No) < 0) flagy = false; }
			else if(byteY[0] > m_Position_Y1[y_No][x_No]) { if((++y_No) > 7) flagy = false; }
			else flagy = false;
		} 
		if((byteE[0] > m_iEnergeSpectEth_Low[y_No*8 + x_No])&&(byteE[0] < m_iEnergeSpectEth_Hight[y_No*8 + x_No]))
			m_iEth_datafile[y_No*8 + x_No][byteE[0]] ++;


		if(!m_bSelEthLimit_HandleInput) {			
			if((byteE[0] > m_iEnergeSpectEth_Low[y_No*8 + x_No])&&(byteE[0] < m_iEnergeSpectEth_Hight[y_No*8 + x_No])) {
				m_isdot_datafile[byteY[0]][byteX[0]] ++;				
			}			
		}


			int num =0;
			int realcounter;
			while (num < filelenth-4) {
				realcounter = filelenth-4-num;
				if( realcounter >= 1024*1) {
					file.Read(manydata,1024*1);
					num += 1024*1;
					realcounter = 1024*1;
				}
				else {  
					file.Read(manydata,filelenth-4-num);
					num += filelenth-4-num;
					realcounter = filelenth-4-num;
				}

				for(int i=0;i<realcounter/4;i++) {
					byteX[0] = (manydata[4*i] >>1)&0x3f;
					byteY[0] = (manydata[4*i+1] >>1)&0x3f;
					byteE[0] = (manydata[4*i+2] >>1)&0x3f;
					byteT[0] = (manydata[4*i+3]&0x7f);

					if((byteE[0] >= ileft_limit)&&(byteE[0] <= iright_limit)&&(m_bSelEthLimit_HandleInput)) {
						//m_sdot[byteX[0]][byteY[0]] ++;
						m_isdot_datafile[byteY[0]][byteX[0]] ++;
						m_Time_Spectrum[byteT[0]] ++;
					}
					m_Energer_Spectrum[byteE[0]] ++;
					//------------------------���㾧������----------//
					x_No = 3,y_No = 3;
					flagx = true;	flagy = true;
					while((flagx)||(flagy)) {
						if(byteX[0] < m_Position_X0[y_No][x_No]) { if((--x_No) < 0) flagx = false; }
						else if(byteX[0] > m_Position_X1[y_No][x_No]) {  if((++x_No) > 7) flagx = false;}
						else flagx = false;
							
						if(byteY[0] < m_Position_Y0[y_No][x_No]) { if((--y_No) < 0) flagy = false; }
						else if(byteY[0] > m_Position_Y1[y_No][x_No]) { if((++y_No) > 7) flagy = false; }
						else flagy = false;
					} 
					if((byteE[0] > m_iEnergeSpectEth_Low[y_No*8 + x_No])&&(byteE[0] < m_iEnergeSpectEth_Hight[y_No*8 + x_No]))
						m_iEth_datafile[y_No*8 + x_No][byteE[0]] ++;

					if(!m_bSelEthLimit_HandleInput) {			
						if((byteE[0] > m_iEnergeSpectEth_Low[y_No*8 + x_No])&&(byteE[0] < m_iEnergeSpectEth_Hight[y_No*8 + x_No])) {
							m_isdot_datafile[byteY[0]][byteX[0]] ++;				
						}			
					}
				}

			}

	file.Close();

	OnRedraw_3d();
	OnRedraw_2d();
	
}

void CDynShowDlg::OnEnergChartRedraw() 
{
	// TODO: Add your control notification handler code here
	OnRedraw_2d();
}


void CDynShowDlg::OnRedraw_2d()
{
	CEdit* pLeftLimit=(CEdit*)GetDlgItem(IDC_ENERGE_LEFT_LIMIT);
	CEdit* pRightLimit=(CEdit*)GetDlgItem(IDC_ENERGE_RIGHT_LIMIT);
	CEdit* pSynEnerSpectValue=(CEdit*)GetDlgItem(IDC_SYN_ENERSPECT_VALUE);

	m_bEnShowPeakValeRatio = false;

		int tplace;
		int x=650;
		long longtdata[64];
		long temp;

		if (m_iSmallMap_Show_Sel == SEL_2D_TIME_FUHE) { //ʱ�������ʾ
			OnShowTimeFuheMap_2D();
			return;
		}

		
		if(m_iSmallMap_Show_Sel == 1) { // ���ײ�����
			//for(int i=0;i<64;i++) longtdata[i] = m_Energer_Spectrum[i];   //���ļ�����
			memcpy(longtdata,m_Energer_Spectrum,64*4);
		}
		if(m_iSmallMap_Show_Sel == 2) { // X����ͶӰ
			for(int i=0;i<64;i++) longtdata[i] = 0;
			for(i=0;i<64;i++) {
				for(int j=0;j<64;j++) {
					//longtdata[i] += m_sdot[i][j];
					longtdata[i] += m_isdot_datafile[i][j];
				}
			}
		}
		if(m_iSmallMap_Show_Sel == 3) { // Y����ͶӰ
			for(int i=0;i<64;i++) longtdata[i] = 0;
			for(i=0;i<64;i++) {
				for(int j=0;j<64;j++) {
					//longtdata[i] += m_sdot[j][i];
					longtdata[i] += m_isdot_datafile[j][i];
				}
			}
		}		
		if(m_iSmallMap_Show_Sel == 4) { // ʱ��ֱ��ͼ������
			OnTimeSpect_redraw_2D();
			return;
		}	
		if(m_iSmallMap_Show_Sel == 5) { // ���������ײ�����

			CString strSynEnerSpectValue;
			pSynEnerSpectValue->GetWindowText(strSynEnerSpectValue);
			if(!strSynEnerSpectValue.IsEmpty())
			{		
			    temp=atol(strSynEnerSpectValue); 
				if((temp>63)||(temp<0)) {
					temp = 0;
					SetDlgItemText(IDC_SYN_ENERSPECT_VALUE,"0");
				}
			}
			else {  temp = 0; }

			memcpy(longtdata,m_iEth_datafile[temp],64*4);
			//return;
		}

  //�����ֵ
	   char chtext[30];
	   int  position = 0;
	   temp = longtdata[0];
	   for(int n=0;n<63;n++) {  //���һ�����������
			if(longtdata[n] > temp) {
				temp = longtdata[n];
				position = n;
			}
		} 
		sprintf(chtext,"val:%d  pos:%d",temp,position);
		temp = (long)(1.5*temp);


		CDC* pDC=GetDC();			
		CRect clientrect;
		GetClientRect(clientrect);			
		CDC MemDC; //���ȶ���һ����ʾ�豸����
		CBitmap MemBitmap;//����һ��λͼ����	
		MemDC.CreateCompatibleDC(NULL);
		MemBitmap.CreateCompatibleBitmap(pDC,clientrect.Width(),clientrect.Height()); //�Զ���һ��ͼ��Ĵ�С
		CBitmap *pOldBit=MemDC.SelectObject(&MemBitmap);
		
		
		CPen pShow5(PS_SOLID,1,RGB(212,208,200));//RGB(0,210,0)
	    CPen* pOldPen=NULL;
	    pOldPen=MemDC.SelectObject(&pShow5);

		CBrush bShow;			
		bShow.CreateSolidBrush(RGB(219,206,198));// С���ڱ���				 					  
		CBrush* pOldBrush=MemDC.SelectObject(&bShow);// 3000 
		MemDC.Rectangle(635,50,920,285);// ˢ��С���ڵĴ�С
					
		MemDC.SelectObject(pOldBrush);//++++
		bShow.DeleteObject();
		pShow5.DeleteObject();

		MemDC.SetBkColor(RGB(219,206,198));
        CPen pShow(PS_SOLID,1,RGB(15, 105,185));//RGB(0,210,0)
	    pOldPen=NULL;
	    pOldPen=MemDC.SelectObject(&pShow);

		MemDC.MoveTo(570+g_draw_startpoint_x,65); 
		MemDC.LineTo(570+g_draw_startpoint_x,265);
		MemDC.MoveTo(570+g_draw_startpoint_x,65);
		MemDC.LineTo(570+g_draw_startpoint_x-3,65+5);

	    MemDC.MoveTo(570+g_draw_startpoint_x,265);
		MemDC.LineTo(570+g_draw_startpoint_x +4*64 ,265);
	    MemDC.MoveTo(570+g_draw_startpoint_x +4*64,265);
        MemDC.LineTo(570+g_draw_startpoint_x +4*64 - 5,265+3);

		MemDC.TextOut(638,47,chtext);
	    MemDC.TextOut(635,268,"(0,0)");
		MemDC.TextOut(898,268," 63 ");     
		// ԭ������Ϊ ��650��265��  ÿ��ļ��Ϊ 4 mil    650 + 64*4 = 906; 
	
		pOldPen=MemDC.SelectObject(&pShow);
		x = 650 ; 
		if(temp<1) temp =1;
		tplace = 265- (int)(4*64*(float)longtdata[0]/temp);
		for(int kk=1;kk<64;kk++) {																				
			MemDC.MoveTo(x,tplace);
			x += 4;
			tplace = 265 - (int)(4*64*(float)longtdata[kk]/temp);
			if(temp < 1) temp = 1;
			if(tplace < 60) tplace = 60;
			if(tplace > 265) tplace = 265;

			MemDC.LineTo(x,tplace);
		}

		CString strLeftlimit;
		CString strRightlimit;
		pLeftLimit->GetWindowText(strLeftlimit);
		pRightLimit->GetWindowText(strRightlimit);


		if(!strLeftlimit.IsEmpty()) {			
				ileft_limit =atol(strLeftlimit);
				if((ileft_limit<0)||(ileft_limit>62)) { 
					ileft_limit = 0;
					SetDlgItemText(IDC_ENERGE_LEFT_LIMIT,"00");
				}
		}
		else{  ileft_limit = 0; }

		if(!strRightlimit.IsEmpty()) {		
				iright_limit =atol(strRightlimit);  
				if((iright_limit<0)||(iright_limit>63)) {
					iright_limit = 63;					
					SetDlgItemText(IDC_ENERGE_RIGHT_LIMIT,"63");
				}
		}
		else{ 	iright_limit = 63;}

		pShow.DeleteObject();

		// ���߽�
		CPen pShow2(PS_SOLID,1,RGB(255,10,,0));//RGB(0,210,0)
		pOldPen=MemDC.SelectObject(&pShow2);
		MemDC.MoveTo(650 + iright_limit*4 , 265);
		MemDC.LineTo(650 + iright_limit*4 , 265 - 190);
		MemDC.MoveTo(650 + ileft_limit*4 , 265);
		MemDC.LineTo(650 + ileft_limit*4 , 265 - 190);
		pShow2.DeleteObject();

		pDC->BitBlt(635,50,285,235,&MemDC,635,50,SRCCOPY);	
		MemBitmap.DeleteObject();
		MemDC.DeleteDC();
		ReleaseDC(pDC);


}


UINT  ReadDataProc(LPVOID  lpData)
{
	int size = 64*10;
	int read_cnt;
	CDynShowDlg *ph=(CDynShowDlg *)lpData ;	

	ph->m_RecFastCounter = 0;
	g_bRunCommThread = true;
	while (g_bRunCommThread)
	{
		if (g_ComPortNo == USB_PORT) {	//USB �˿�
			//read_cnt = ReadUsbData(hUsb, data, size);
			read_cnt = ReadUsbData(hUsb, ph->m_RecFastData, 1024);//2048
			if(!read_cnt) { 
				AfxMessageBox("USB Read Error");
				g_bRunCommThread = false;		
			}			
		}
		else { // ѡ���˴���
			read_cnt = ReadResultMaint232(ph->m_RecFastData);
			if(read_cnt == 0xffff) {
				AfxMessageBox("Read Error!");
				g_bRunCommThread = false;
			}
		}
		
		if(read_cnt > 0) {			
			if(read_cnt%4 == 0) { 
				//memcpy(ph->m_RecFastData,data,read_cnt);
				ph->SendMessage(WM_RECVEIVE_FULL,read_cnt,0);
			}
		}
		//else Sleep(10);
	}

//	delete []data;

//	CloseShmMaint232();
//	CloseUsbDevice(hUsb);
	return 0;
}



void CDynShowDlg::OnOK() 
{
	// TODO: Add extra validation here

	if(g_bRunCommThread) {
		OnStopSample();

		g_bRunCommThread = false;
		Sleep(500);
		//delete m_pThread_Comm;
	}
	else Sleep(500);

	delete m_RecFastData;

	CDialog::OnOK();
}

void CDynShowDlg::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	if(g_bRunCommThread) {
		OnStopSample();
		
		g_bRunCommThread = false;
		Sleep(500);
		//delete m_pThread_Comm;
	}
	else Sleep(500);
	
	delete m_RecFastData;

	CDialog::OnClose();
}

void CDynShowDlg::OnSaveFile() 
{
/*	CFile ftxt;
	ftxt.Open("C://DynShowTime.txt",CFile::modeCreate | CFile::modeWrite);
	char stemp[100];

	for (int ii=0;ii<128;ii++) {
		sprintf(stemp,"��ַ: %d     ������%d \r\n",ii,m_Time_Spectrum[ii]); 
		ftxt.Write(stemp,strlen(stemp)); 
	}
	ftxt.Close();
	AfxMessageBox("�ļ��Ѿ����浽C��DynshowTime.txt ",MB_OK);
	return;
*/

//	// �����ٶ��õ�
#ifdef TEST_USB_SPEED
	CString strtxt;
	float speed = (double)g_TotalNum/((double)(dwTime2 - dwTime1)/1000.);
	strtxt.Format("�������ݸ�����%d \n\r�ٶȣ�%d Byte/s",g_TotalNum,(int)speed);
	AfxMessageBox(strtxt);
	return;
#endif

	// TODO: Add your control notification handler code here
	filesave.bNewfilepackage = fileoper_Newfile;
	filesave.iBlockSel = fileoper_iblocksel;
	if( IDOK != filesave.DoModal()) {
		return;
	}
	fileoper_Newfile = filesave.bNewfilepackage;
	fileoper_iblocksel = filesave.iBlockSel;

	CString FilePathName; 
	CFile savefile;
	if(fileoper_Newfile) {
		CFileDialog dlg(FALSE,"dat","*.dat");///TRUEΪOPEN�Ի���FALSEΪSAVE AS�Ի��� 
		dlg.m_ofn.lpstrFilter = ("ɢ������ͼ���� ""*.dat");
		if(dlg.DoModal()==IDOK) FilePathName=dlg.GetPathName(); 
		else return;
		if( !savefile.Open( FilePathName, CFile::modeCreate | CFile::modeWrite) ) {			
		}
		savefile.SetLength( (128*4 + 4)*1024 );		
	} 
	else {
		CFileDialog dlg(TRUE,"dat","*.dat");///TRUEΪOPEN�Ի���FALSEΪSAVE AS�Ի���
		dlg.m_ofn.lpstrFilter = ("ɢ������ͼ���� ""*.dat");
		if(dlg.DoModal()==IDOK) FilePathName=dlg.GetPathName(); 
		else return;
		CFile tempfile;
		if(!tempfile.Open(FilePathName,CFile::modeRead)) {
			AfxMessageBox("���ܽ��б༭���� (û���ҵ����ļ�) \n������ѡ�񱣴��ļ��Ĳ�������!");
			CString strtemp;
			strtemp = "�ļ�����ʧ��!(û���ҵ�������ļ�)\n" + FilePathName;
			SetDlgItemText(IDC_STATIC_FILE_NAME,strtemp);
			return;
		}
		tempfile.Close();
		if( !savefile.Open( FilePathName, CFile::modeReadWrite) ) {			
		}
	}
	unsigned char filehead[128];
	savefile.SeekToBegin();

//	int filelenth = savefile.GetLength();
//	if(filelenth != (128*4 +4)*1024 ) {
//		AfxMessageBox("�ļ����Ȳ��ԣ����治�ɹ���");
//		return;
//	}

	unsigned char chartemp[128];
	chartemp[0] = 0x00;
	chartemp[1] = 0x00;
	savefile.Seek(FILEHEAD_TYPE_ADDR,CFile::begin);  //д �ļ�����
	savefile.Write(chartemp,2);
	
	
	// ����ģ�����
	filehead[0] = 0x01;
	savefile.Seek(FILEHEAD_ANOLPARA_ATTR_ADDR,CFile::begin );	
	savefile.Write(filehead,1);
	savefile.Seek(PARA_ANOLPARA_DATA_ADDR+ BLOCK_ANOLPARA_DATA_SIZE*fileoper_iblocksel,CFile::begin);
	for(int i=0;i<8;i++) {
		type.intdata = m_iProgCtrlData[i+1];
		filehead[i] = type.uchdata[0];
	}
	savefile.Write(filehead,8);//m_iProgCtrlData[] ����̿����ݣ�����

	//����ɢ������

	filehead[0] = 0x01;
	savefile.Seek(FILEHEAD_SDOT_ATTR_ADDR,CFile::begin );	
	savefile.Write(filehead,1);
	savefile.Seek(DATA_SDOT_ADDR+ BLOCK_SDOT_SIZE*fileoper_iblocksel,CFile::begin);
	unsigned char temp[4]={0,0,0,0};	
	for(i=0;i<64;i++) {
		for(int j=0;j<64;j++) {	
			temp[0] = (unsigned char)m_sdot[i][j];
			temp[1] = (unsigned char)(m_sdot[i][j]>>8);
			temp[2] = (unsigned char)(m_sdot[i][j]>>16);
			temp[3] = (unsigned char)(m_sdot[i][j]>>24);
			savefile.Write(temp,4);
		}
	}
	//���浱ǰλ�ñ߽�
	filehead[0] = 0x01;
	savefile.Seek(FILEHEAD_PLIMIT_ATTR_ADDR,CFile::begin );	
	savefile.Write(filehead,1);	
	savefile.Seek(PARA_PLIMIT_DATA_ADDR+ BLOCK_PLIMIT_DATA_SIZE*fileoper_iblocksel,CFile::begin);
	for(i=0;i<8;i++) {
		for(int j=0;j<8;j++) {	
			temp[0] = m_Position_X0[i][j];
			temp[1] = m_Position_Y0[i][j];
			temp[2] = m_Position_X1[i][j];
			temp[3] = m_Position_Y1[i][j];
			savefile.Write(temp,4);
		}
	}	

//�����������ݣ�������������������
	if (m_CurFileName == "") {
		savefile.Close();
		AfxMessageBox("���ȡ��ԭʼ�ļ�������");
		return;
	}
	CFile sourcFile;
	if( !sourcFile.Open( m_CurFileName, CFile::modeRead) ) {
		AfxMessageBox("ԭʼ�ļ� �򿪴���");
		return;			
	}
	int filelenth = sourcFile.GetLength();
	unsigned char byteX[1],byteY[1],byteE[1],byteT[1];
	int datacounter = filelenth/4;
	unsigned char manydata[1024];

	for(i=0;i<64;i++) {
		m_iEnergeSpectEth_Low[i] = 0;
		m_iEnergeSpectEth_Hight[i] = 63;
	}
	
	memset(m_sdot,0,64*64*4);
	memset(m_isdot_datafile,0,64*64*4);
	memset(m_iEth_datafile,0,64*64*4);
	memset(m_Energer_Spectrum,0,64*4);
	memset(m_Time_Spectrum,0,128*4);
	
	sourcFile.Seek(0,CFile::begin );	
	sourcFile.Read(byteX,1);
	sourcFile.Read(byteY,1);
	sourcFile.Read(byteE,1);
	sourcFile.Read(byteT,1);

    // ���ļ�ͷ��ԭʼ����
	sourcFile.Read(chartemp,124);
	sourcFile.Seek(128,CFile::begin );
	i=0;
	int x_No = 3,y_No = 3;
	BOOL flagx = true;
	BOOL flagy = true;			
	int num =0;
	int realcounter;
	while (num < filelenth-128) {
		realcounter = filelenth-128-num;
		if( realcounter >= 1024*1) {
			sourcFile.Read(manydata,1024*1);
			num += 1024*1;
			realcounter = 1024*1;
		}
		else {  
			sourcFile.Read(manydata,filelenth-128-num);
			num += filelenth-128-num;
			realcounter = filelenth-128-num;
		}
		
		for(int i=0;i<realcounter/4;i++) {
			byteX[0] = (manydata[4*i] >>1)&0x3f;
			byteY[0] = (manydata[4*i+1] >>1)&0x3f;
			byteE[0] = (manydata[4*i+2] >>1)&0x3f;
			byteT[0] = (manydata[4*i+3]&0x7f);
			//	byteT[0] = (manydata[4*i]&0x7f);// tests akang
		
			if((byteE[0] >= ileft_limit)&&(byteE[0] <= iright_limit)&&(m_bSelEthLimit_HandleInput)) {
				m_isdot_datafile[byteY[0]][byteX[0]] ++;
				m_Time_Spectrum[byteT[0]] ++;
			}
			m_Energer_Spectrum[byteE[0]] ++;
				
			//------------------------���㾧������----------//
			x_No = 3,y_No = 3;
			flagx = true;	flagy = true;
			while((flagx)||(flagy)) {
				if(byteX[0] < m_Position_X0[y_No][x_No]) { if((--x_No) < 0) flagx = false; }
				else if(byteX[0] > m_Position_X1[y_No][x_No]) {  if((++x_No) > 7) flagx = false;}
				else flagx = false;
			
				if(byteY[0] < m_Position_Y0[y_No][x_No]) { if((--y_No) < 0) flagy = false; }
				else if(byteY[0] > m_Position_Y1[y_No][x_No]) { if((++y_No) > 7) flagy = false; }
				else flagy = false;
			} 
			if((byteE[0] > m_iEnergeSpectEth_Low[y_No*8 + x_No])&&(byteE[0] < m_iEnergeSpectEth_Hight[y_No*8 + x_No]))
				m_iEth_datafile[y_No*8 + x_No][byteE[0]] ++;
	
				
			if(!m_bSelEthLimit_HandleInput) {			
				if((byteE[0] > m_iEnergeSpectEth_Low[y_No*8 + x_No])&&(byteE[0] < m_iEnergeSpectEth_Hight[y_No*8 + x_No])) {
					m_isdot_datafile[byteY[0]][byteX[0]] ++;				
				}			
			}
		}
	}		
	sourcFile.Close();
//--end of ������������ 		
	
	


	//������������
	filehead[0] = 0x01;
	savefile.Seek(FILEHEAD_ENERGESPECTRUM_ATTR_ADDR,CFile::begin );	
	savefile.Write(filehead,1);
	savefile.Seek(DATA_ESPECT_ADDR+ BLOCK_ESPECT_SIZE*fileoper_iblocksel,CFile::begin);

	unsigned char tempE[4];
	for(i=0;i<64;i++) {
		for(int j=0;j<64;j++) {	
			tempE[0] = (unsigned char)m_iEth_datafile[i][j];
			tempE[1] = (unsigned char)(m_iEth_datafile[i][j]>>8);
			tempE[2] = (unsigned char)(m_iEth_datafile[i][j]>>16);
			tempE[3] = (unsigned char)(m_iEth_datafile[i][j]>>24);
			savefile.Write(tempE,4);
		}
	}


	savefile.Close();	
	CString strtemp;
	strtemp = "ɢ�㡢�������� ����ɹ�!\n" + FilePathName;
	SetDlgItemText(IDC_STATIC_FILE_NAME,strtemp);


/*	
	for(int i=0;i<100;i++) filehead[i] = 0xC7+0;//ɢ��ͼ���� 0xC7��0xC8;					
	savefile.Write(filehead,100);				//���Ƶ��ļ�ͷ	

	for(i=0;i<9;i++) {
		type.intdata = m_iProgCtrlData[i];
		filehead[i] = type.uchdata[0];
	}
	savefile.Write(filehead,9);//m_iProgCtrlData[] ����̿����ݣ�����

	for(i=0;i<19;i++) filehead[i] = 0x88;//0x55;					
	savefile.Write(filehead,19);				//���Ƶ��ļ�ͷ	

	unsigned char temp[4]={0,0,0,0};
	for(i=0;i<64;i++) {
		for(int j=0;j<64;j++) {	
			temp[0] = (unsigned char)m_sdot[i][j];
			temp[1] = (unsigned char)(m_sdot[i][j]>>8);
			temp[2] = (unsigned char)(m_sdot[i][j]>>16);
			temp[3] = (unsigned char)(m_sdot[i][j]>>24);
			savefile.Write(temp,4);
		}
	}

	SetDlgItemText(IDC_STATIC_FILE_NAME,"ɢ�����ݱ���ɹ�");
	savefile.Close();

	//-----------------���澧�������ļ�----------//
	CString FilePathNameE; 
	CFileDialog dlgE(FALSE,"dat","*.dat");///TRUEΪOPEN�Ի���FALSEΪSAVE AS�Ի��� 
	dlgE.m_ofn.lpstrFilter = ("�������� ""*.dat");//() 
	if(dlgE.DoModal()==IDOK) FilePathNameE=dlgE.GetPathName(); 
	else return;

	CFile savefileE;
	if( !savefileE.Open( FilePathNameE, CFile::modeCreate | CFile::modeWrite) ) {			
	}
	unsigned char fileheadE[128];
	for(i=0;i<100;i++) fileheadE[i] = 0xC5+i;//�������� 0xC5��0xC6					
	savefileE.Write(fileheadE,100);				//���Ƶ��ļ�ͷ	

	for(i=0;i<9;i++) {
		type.intdata = m_iProgCtrlData[i];
		fileheadE[i] = type.uchdata[0];
	}
	savefileE.Write(fileheadE,9);//m_iProgCtrlData[] ����̿����ݣ�����

	for(i=0;i<19;i++) fileheadE[i] = 0x00;//0x55;					
	savefileE.Write(fileheadE,19);				//���Ƶ��ļ�ͷ	
//////


	unsigned char tempE[4]={0,0,0,0};
	for(i=0;i<64;i++) {
		for(int j=0;j<64;j++) {	
			tempE[0] = (unsigned char)m_iEth_datafile[i][j];
			tempE[1] = (unsigned char)(m_iEth_datafile[i][j]>>8);
			tempE[2] = (unsigned char)(m_iEth_datafile[i][j]>>16);
			tempE[3] = (unsigned char)(m_iEth_datafile[i][j]>>24);
			savefileE.Write(tempE,4);
		}
	}
	SetDlgItemText(IDC_STATIC_FILE_NAME,"ɢ�㡢�������ݱ���ɹ�");
	savefileE.Close();
*/
	
}


void CDynShowDlg::OnStartSample() 
{
	memset(m_arrTimeFuheData,0,256*4);

	CString FilePathName; 
	CFileDialog dlg(TRUE,"dat","*.dat");///TRUEΪOPEN�Ի���FALSEΪSAVE AS�Ի��� 

	dlg.m_ofn.lpstrFilter = ("[ʵʱ����] ""*.*");//() 
	if(dlg.DoModal()==IDOK) FilePathName=dlg.GetPathName(); 
	else return;

	m_CurFileName = FilePathName;


	if (g_ComPortNo == USB_PORT) {  //usb �˿�
		hUsb = OpenUsbDevice();
		if(hUsb == NULL ){
			AfxMessageBox("��USB�豸����!");
			return ;
		}
	}
	else {  //ѡ���� ����
		if(!OpenShmMaint232(g_ComPortNo)) {
			MessageBox("Communication Port Open Error!");
			return;
		}
	}

	m_pThread_Comm = AfxBeginThread(ReadDataProc,
							(LPVOID)this,
							THREAD_PRIORITY_BELOW_NORMAL,
							0,
							CREATE_SUSPENDED);						

	//m_pThread_Comm->m_bAutoDelete = FALSE;
	//m_pThread_Comm->ResumeThread();

	m_Check_Midfilter.SetCheck(0);
	m_bEnMidFilter = false;

	if( !m_RealTimeFile.Open( FilePathName, CFile::modeCreate | CFile::modeWrite) ) {
			
	}
	Sleep(200);

	unsigned char chrtemp[128];
	memset(chrtemp,0x55,4);				// �����ļ�ͷ��
	m_RealTimeFile.Write(chrtemp,100);

	for(int i=0;i<9;i++){			
		type.intdata = m_iProgCtrlData[i];
		chrtemp[i] = type.uchdata[0];
	}
	m_RealTimeFile.Write(chrtemp,9);	// ����̿�����
	memset(chrtemp,0,19);
	m_RealTimeFile.Write(chrtemp,19);   //�������

	FilePathName += "\n���ڲɼ�����......";
	SetDlgItemText(IDC_STATIC_FILE_NAME,FilePathName);

	m_reccounter = 0;
	for(i=0;i<64;i++){			//���64��64 ������	
		for(int j=0;j<64;j++){
			m_sdot[i][j] = 0;	
			m_isdot_datafile[i][j] = 0;
			m_iEth_datafile[i][j] = 0; //������������
		}
		m_Energer_Spectrum[i] = 0;
	}
	for(i=0;i<128;i++) {
		m_Time_Spectrum[i] = 0;
	}

	UpdateData(true);
	UpdateData(false);
	
	ASSERT(SetTimer(2,1000,NULL)!=0);// �趨��ʱˢ��ʱ��

	m_iSampleCount = 0;
	if(!m_strSampleTime.IsEmpty()) {
		if(m_bSampleTimeMode) {
			m_iSampleTime = (int)_atoi64(m_strSampleTime);
			if(m_iSampleTime <1 ) m_iSampleTime = 1;
			m_iSampleTimeCounter = 0;
		//	
		}
		else {
			m_iSampleCount = (long)_atoi64(m_strSampleTime);
			if(m_iSampleCount <1 ) m_iSampleCount = 1;
			m_iSampleTime = 0;
			m_iSampleTimeCounter = 0;
		}
	}
	else{
		m_iSampleTime = 0x7ffffff0; //Ĭ��λΪ�ܳ�ʱ��
		m_iSampleTimeCounter = 0;
	}

	m_bCompareFinish = false;
	GetDlgItem(IDC_START_SAMPLE)->EnableWindow(false);
	GetDlgItem(IDC_STOP_SAMPLE)->EnableWindow(true);
	GetDlgItem(IDC_PROG_CONTROL)->EnableWindow(false);
	GetDlgItem(IDC_SAMPLE_MODE_SEL)->EnableWindow(false);

	unsigned char start = 0x01;

	if (g_ComPortNo == USB_PORT) {
		int nusbwrite = WriteUsbData(hUsb, &start, 1);
		if(!nusbwrite) AfxMessageBox(" Write usb error");
	}
	else {
		int nWR = WriteCommandMaint232(1,&start);
		if(nWR == false) {
			AfxMessageBox("Communication Write Error.\n\n Please check Communication Setting.");
		}		
	}

	m_pThread_Comm->m_bAutoDelete = FALSE;
	m_pThread_Comm->ResumeThread();

	Beep(1000,50);

	// �����ٶ��õ�
#ifdef TEST_USB_SPEED
	dwTime1 = GetTickCount();
#endif	

}

void CDynShowDlg::OnStopSample() 
{	
	// �����ٶ��õ�
#ifdef TEST_USB_SPEED
	dwTime2 = GetTickCount();
	g_TotalNum = m_reccounter;
#endif
	
	KillTimer(2);

	g_bRunCommThread = false;
	ASSERT(SetTimer(1,800,NULL)!=0);	

	if( WaitForSingleObject( m_pThread_Comm->m_hThread,1200) == WAIT_TIMEOUT ) {
		CancelIo(hUsb);
		TerminateThread( m_pThread_Comm->m_hThread, -1);
	}

	delete m_pThread_Comm;

	unsigned char buf=0;

	if (g_ComPortNo == USB_PORT) {		
		WriteUsbData(hUsb, &buf, 1);
		Sleep(10);
		CloseUsbDevice(hUsb);
	}
	else {
		WriteCommandMaint232(1,&buf);
		Sleep(10);
		CloseShmMaint232();	
	}

	UpdateData(true);
	UpdateData(false);

	if(m_bSampleTimeMode) {
		if(!m_strSampleTime.IsEmpty()) {
			KillTimer(2);	
		}
	}
	GetDlgItem(IDC_STOP_SAMPLE)->EnableWindow(false);
	GetDlgItem(IDOK)->EnableWindow(false);	

}


LRESULT CDynShowDlg::OnRecveive(WPARAM wParam, LPARAM lParam)
{						
	unsigned char byteX,byteY,byteE,byteT;
	CString strcounter;
	int	x_No = 3,y_No = 3;
	BOOL flagx = true;	
	BOOL flagy = true;

	m_reccounter += wParam;

	m_RealTimeFile.Write(m_RecFastData,wParam); //�ļ�����
	
	strcounter.Format("���հ���:%d",(int)(m_reccounter/4));
	SetDlgItemText(IDC_STATIC_CONTER,strcounter);

	//�ж��Ƿ�Ϊ ʱ�������������
	if (m_iSmallMap_Show_Sel == SEL_2D_TIME_FUHE) { //ʱ�������ʾ
		OnAnalyzeTimeFuheData((char*)m_RecFastData,wParam); 
        // ˢ��������
//		m_RecFastCounter += wParam;		
//		if (g_ComPortNo == USB_PORT) { // ��ʾˢ�� Ƶ��
//			if(m_RecFastCounter >= 64*10) {//64*1024*5
//				m_RecFastCounter = 0;
//				OnRedraw_2d(); //�˴�ֻˢ���� 2D ͼ��
//			}
//		}		
		return 0;
	}



	// ����Ϊ������ X��Y��E��T ���ݷֽ����
	if(!m_bSampleTimeMode) {
		if((m_iSampleCount > 0)&&(m_reccounter/4 >= m_iSampleCount)) {
			m_iSampleCount = 0xfffffff0;  //�ܴ��һ����
			unsigned char buf[1] = {0x00};

			OnStopSample();

			g_bRunCommThread = false;
			ASSERT(SetTimer(1,2500,NULL)!=0);
			UpdateData(true);
			UpdateData(false);
			GetDlgItem(IDC_STOP_SAMPLE)->EnableWindow(false);
		}
	}
	
	for(int i=0;i<(int)(wParam/4);i++) {
		byteX = m_RecFastData[i*4 + 0];
		byteY = m_RecFastData[i*4 + 1];
		byteE = m_RecFastData[i*4 + 2];
		byteT = m_RecFastData[i*4 + 3];

		byteX = (byteX >>1)&0x3f;
		byteY = (byteY >>1)&0x3f;
		byteE = (byteE >>1)&0x3f;  //
		byteT = (byteT&0x7f);

		m_Energer_Spectrum[byteE] ++;	
		if(m_Energer_Spectrum[byteE] > 0xffffff00) m_Energer_Spectrum[byteE] = 0xffffff00;

		if((byteE >= ileft_limit)&&(byteE <= iright_limit)&&(m_bSelEthLimit_HandleInput)){
			//m_sdot[byteY][byteX] ++;
			m_Time_Spectrum[byteT] ++;

			m_isdot_datafile[byteY][byteX] ++;
			if(m_sdot[byteY][byteX] > 0xffffff00) m_sdot[byteY][byteX] = 0xffffff00;			
			m_Time_Spectrum[byteT] ++;	
			if(m_Time_Spectrum[byteT] > 0xffffff00) m_Time_Spectrum[byteT] = 0xffffff00;
		}

		//------------------------���㾧��������----------//		
			x_No = 3,y_No = 3;
			flagx = true;	flagy = true;
			while((flagx)||(flagy)) {
				if(byteX < m_Position_X0[y_No][x_No]) { if((--x_No) < 0) flagx = false; }
				else if(byteX > m_Position_X1[y_No][x_No]) {  if((++x_No) > 7) flagx = false;}
				else flagx = false;
					
				if(byteY < m_Position_Y0[y_No][x_No]) { if((--y_No) < 0) flagy = false; }
				else if(byteY > m_Position_Y1[y_No][x_No]) { if((++y_No) > 7) flagy = false; }
				else flagy = false;
			} 
			m_iEth_datafile[y_No*8 + x_No][byteE] ++;

		if(!m_bSelEthLimit_HandleInput) {			
			if((byteE > m_iEnergeSpectEth_Low[y_No*8 + x_No])&&(byteE < m_iEnergeSpectEth_Hight[y_No*8 + x_No])) {
				m_isdot_datafile[byteY][byteX] ++;				
			}			
		}//----- end ���㾧�������� - ----------------------------//
		

	} // end for(int i=0;i<(int)(wParam/4);i++) {

	m_RecFastCounter += wParam;

	
	return 0L;

}

void CDynShowDlg::OnMouseMove(UINT nFlags, CPoint point) 
{
	CDialog::OnMouseMove(nFlags, point);

	CDC* pDC=GetDC(); //����ά ֱ��
	CPen pShow2(PS_SOLID,1,RGB(255,10,,0));//RGB(0,210,0)
	CPen* pOldPen=NULL;
	pOldPen=pDC->SelectObject(&pShow2);

	pDC->SetROP2(R2_NOT);
	CString strtext;

	//----------��ƽ���������±߽硿--------//
	if(((point.x <650)||(point.x> 906))&&((m_bMove_LeftLimit)||(m_bMove_RightLimit)))  {
			ReleaseDC(pDC);
			return;
	}
	if((m_bMove_LeftLimit)||(m_bMove_RightLimit)) {
		pDC->MoveTo(m_Oldpoint.x, 265);
		pDC->LineTo(m_Oldpoint.x, 265 - 190); //akang  80
		pDC->MoveTo(point.x, 265);
		pDC->LineTo(point.x, 265 - 190); //akang  80
		m_Oldpoint = point;
		if(m_bMove_LeftLimit) {
			strtext.Format("%d",(point.x-650)/4);
			SetDlgItemText(IDC_ENERGE_LEFT_LIMIT,strtext);		
		}
		else if(m_bMove_RightLimit) {
			strtext.Format("%d",(point.x-650)/4);
			SetDlgItemText(IDC_ENERGE_RIGHT_LIMIT,strtext);		
		}
	}
//	ReleaseDC(pDC);
	//---------����ȡɢ��ԭʼ���ݡ�------//
	if((point.x<75)||((point.x>588))) {
		strtext.Format("������ʾ��");
		SetDlgItemText(IDC_STATIC_STATUS,strtext);
		ReleaseDC(pDC);
		return;
	}
	if((point.y>588)||(point.y<75)) {
		strtext.Format("������ʾ��");
		SetDlgItemText(IDC_STATIC_STATUS,strtext);
		ReleaseDC(pDC);
		return;
	}
	int px,py;
	int datavalue;
	px = (point.x - g_draw_startpoint_y+4)/8 ;
	py = (point.y - g_draw_startpoint_x+4)/8 ;
	if(px == -1) px = 0;
	if(py == -1) py = 0;
	datavalue = m_sdot[py][px];	

	strtext.Format("ͼ������: (%d,  %d) ������ʾ�� %d ",px,py,datavalue);
	SetDlgItemText(IDC_STATIC_STATUS,strtext);

	//---------��ɢ���λ�ñ߽�ƽ�ơ�------//

/*	CDC* pDC=GetDC(); //����ά ֱ��
	CPen pShow2(PS_SOLID,2,RGB(255,10,,0));//RGB(0,210,0)
	CPen* pOldPen=NULL;
	pOldPen=pDC->SelectObject(&pShow2);

	pDC->SetROP2(R2_NOT);
//	CString strtext;
*/
	if(m_bSameDriction_X) { //ˮƽ����һ��
		if(m_bButtonDown) {
			if(m_bEnMoveBreakLine) {  //����ϵ�ˮƽ����
				if((m_sdotOldpoint.y < g_draw_startpoint_x + m_Position_Y0[m_iMoveCurLine_Y-1][m_iMoveCurLine_X]*8 +1)
					||(m_sdotOldpoint.y > g_draw_startpoint_x + m_Position_Y1[m_iMoveCurLine_Y][m_iMoveCurLine_X]*8 -1))  {
										
					m_bthisMoveInValid = true;
					m_sdotOldpoint = point;
					ReleaseDC(pDC);
					return;
				}
				if(m_iMoveCurLine_X <7 ) {
					pDC->MoveTo(g_draw_startpoint_y + m_Position_X0[m_iMoveCurLine_Y][m_iMoveCurLine_X]*8,m_sdotOldpoint.y);
					pDC->LineTo(g_draw_startpoint_y + (m_Position_X1[m_iMoveCurLine_Y][m_iMoveCurLine_X]+1)*8,m_sdotOldpoint.y);
					pDC->MoveTo(g_draw_startpoint_y + m_Position_X0[m_iMoveCurLine_Y][m_iMoveCurLine_X]*8,point.y);
					pDC->LineTo(g_draw_startpoint_y + (m_Position_X1[m_iMoveCurLine_Y][m_iMoveCurLine_X]+1)*8,point.y);					
				}
				else {
					pDC->MoveTo(g_draw_startpoint_y + m_Position_X0[m_iMoveCurLine_Y][m_iMoveCurLine_X]*8,m_sdotOldpoint.y);
					pDC->LineTo(g_draw_startpoint_y + (m_Position_X1[m_iMoveCurLine_Y][m_iMoveCurLine_X])*8,m_sdotOldpoint.y);
					pDC->MoveTo(g_draw_startpoint_y + m_Position_X0[m_iMoveCurLine_Y][m_iMoveCurLine_X]*8,point.y);
					pDC->LineTo(g_draw_startpoint_y + (m_Position_X1[m_iMoveCurLine_Y][m_iMoveCurLine_X])*8,point.y);

				}

				m_sdotOldpoint = point;
				ReleaseDC(pDC);
				return;
			}

			if(m_bMoveLongLine) {//������
				if((m_sdotOldpoint.y < g_draw_startpoint_x + m_Position_Y0[m_Long_Moving_Tab -1][1]*8 +1)
					||(m_sdotOldpoint.y > g_draw_startpoint_x + m_Position_Y1[m_Long_Moving_Tab ][1]*8 -1))  {
					m_bthisMoveInValid = true;
					m_sdotOldpoint = point;
					ReleaseDC(pDC);
					return;
				}
				pDC->MoveTo(g_draw_startpoint_y + m_Position_X0[m_Long_Moving_Tab][1]*8,m_sdotOldpoint.y);
				pDC->LineTo(g_draw_startpoint_y + m_Position_X0[m_Long_Moving_Tab][7]*8,m_sdotOldpoint.y);
				pDC->MoveTo(g_draw_startpoint_y + m_Position_X0[m_Long_Moving_Tab][1]*8,point.y);
				pDC->LineTo(g_draw_startpoint_y + m_Position_X0[m_Long_Moving_Tab][7]*8,point.y);					
									
				m_sdotOldpoint = point;
				ReleaseDC(pDC);
				return;
			}
			else { //��ˮƽ����
				if(m_bShort_Line_Valid) { //ˮƽ����Ĵ�ֱ������Ч
					if((m_sdotOldpoint.x < g_draw_startpoint_y + m_Position_X0[m_Moving_Tab_Ver][m_Moving_Tab_Hor -1]*8 +1)
						||(m_sdotOldpoint.x > g_draw_startpoint_y + m_Position_X1[m_Moving_Tab_Ver][m_Moving_Tab_Hor]*8 -1))  { //�ж�Խ��
						m_sdotOldpoint = point;
						m_bthisMoveInValid = true;
						ReleaseDC(pDC);
						return;
					}

					pDC->MoveTo(m_sdotOldpoint.x,g_draw_startpoint_x + m_Position_Y0[m_Moving_Tab_Ver][m_Moving_Tab_Hor]*8);
					if(m_Moving_Tab_Ver == 7) pDC->LineTo(m_sdotOldpoint.x,g_draw_startpoint_x + m_Position_Y1[m_Moving_Tab_Ver][m_Moving_Tab_Hor]*8);
					else pDC->LineTo(m_sdotOldpoint.x,g_draw_startpoint_x + (m_Position_Y1[m_Moving_Tab_Ver][m_Moving_Tab_Hor]+1)*8);
					pDC->MoveTo(point.x,g_draw_startpoint_x + m_Position_Y0[m_Moving_Tab_Ver][m_Moving_Tab_Hor]*8);
					if(m_Moving_Tab_Ver == 7) pDC->LineTo(point.x,g_draw_startpoint_x + m_Position_Y1[m_Moving_Tab_Ver][m_Moving_Tab_Hor]*8);
					else pDC->LineTo(point.x,g_draw_startpoint_x + (m_Position_Y1[m_Moving_Tab_Ver][m_Moving_Tab_Hor]+1)*8);

					m_sdotOldpoint = point;
					ReleaseDC(pDC);
					return;
				}
				if(m_Moving_Tab_Hor == 0) {// ˮƽǰ����
					if((m_sdotOldpoint.y < g_draw_startpoint_x + m_Position_Y0[m_Moving_Tab_Ver -1][0]*8 +4)
						||(m_sdotOldpoint.y > g_draw_startpoint_x + m_Position_Y1[m_Moving_Tab_Ver][0]*8 -1))  {//�ж�Խ��
						m_sdotOldpoint = point;
						m_bthisMoveInValid = true;
						ReleaseDC(pDC);
						return;
					}
					pDC->MoveTo(g_draw_startpoint_y + m_Position_X0[m_Moving_Tab_Ver][0]*8,m_sdotOldpoint.y);
					pDC->LineTo(g_draw_startpoint_y + (m_Position_X1[m_Moving_Tab_Ver][0]+1)*8,m_sdotOldpoint.y);
					pDC->MoveTo(g_draw_startpoint_y + m_Position_X0[m_Moving_Tab_Ver][0]*8,point.y);
					pDC->LineTo(g_draw_startpoint_y + (m_Position_X1[m_Moving_Tab_Ver][0]+1)*8,point.y);					
										
					m_sdotOldpoint = point;
					ReleaseDC(pDC);
					return;
				}
				else {//ˮƽ�����
					if((m_sdotOldpoint.y < g_draw_startpoint_x + m_Position_Y0[m_Moving_Tab_Ver -1][7]*8 +4)
						||(m_sdotOldpoint.y > g_draw_startpoint_x + m_Position_Y1[m_Moving_Tab_Ver][7]*8 -1))  { //�ж�Խ��
						m_sdotOldpoint = point;
						m_bthisMoveInValid = true;
						ReleaseDC(pDC);
						return;
					}
					pDC->MoveTo(g_draw_startpoint_y + m_Position_X0[m_Moving_Tab_Ver][7]*8,m_sdotOldpoint.y);
					pDC->LineTo(g_draw_startpoint_y + m_Position_X1[m_Moving_Tab_Ver][7]*8,m_sdotOldpoint.y);
					pDC->MoveTo(g_draw_startpoint_y + m_Position_X0[m_Moving_Tab_Ver][7]*8,point.y);
					pDC->LineTo(g_draw_startpoint_y + m_Position_X1[m_Moving_Tab_Ver][7]*8,point.y);					
										
					m_sdotOldpoint = point;
					ReleaseDC(pDC);
					return;
				}
			}

		}

	}
	else { //��ֱ����
		if(m_bButtonDown) {
			if(m_bEnMoveBreakLine) {  //����ϵ�ˮƽ����
				if((m_sdotOldpoint.x < g_draw_startpoint_y + m_Position_X0[m_iMoveCurLine_Y][m_iMoveCurLine_X-1]*8 +1)
					||(m_sdotOldpoint.x > g_draw_startpoint_y + m_Position_X1[m_iMoveCurLine_Y][m_iMoveCurLine_X]*8 -1))  {
										
					m_bthisMoveInValid = true;
					m_sdotOldpoint = point;
					ReleaseDC(pDC);
					return;
				}
			 
				if(m_iMoveCurLine_Y <7) {
					pDC->MoveTo(m_sdotOldpoint.x,g_draw_startpoint_x + m_Position_Y0[m_iMoveCurLine_Y][m_iMoveCurLine_X]*8);
					pDC->LineTo(m_sdotOldpoint.x,g_draw_startpoint_x + (m_Position_Y1[m_iMoveCurLine_Y][m_iMoveCurLine_X]+1)*8);
					pDC->MoveTo(point.x,g_draw_startpoint_x + m_Position_Y0[m_iMoveCurLine_Y][m_iMoveCurLine_X]*8);
					pDC->LineTo(point.x,g_draw_startpoint_x + (m_Position_Y1[m_iMoveCurLine_Y][m_iMoveCurLine_X]+1)*8);					
				}
				else {
					pDC->MoveTo(m_sdotOldpoint.x,g_draw_startpoint_x + m_Position_Y0[m_iMoveCurLine_Y][m_iMoveCurLine_X]*8);
					pDC->LineTo(m_sdotOldpoint.x,g_draw_startpoint_x + (m_Position_Y1[m_iMoveCurLine_Y][m_iMoveCurLine_X])*8);
					pDC->MoveTo(point.x,g_draw_startpoint_x + m_Position_Y0[m_iMoveCurLine_Y][m_iMoveCurLine_X]*8);
					pDC->LineTo(point.x,g_draw_startpoint_x + (m_Position_Y1[m_iMoveCurLine_Y][m_iMoveCurLine_X])*8);	

				}

				m_sdotOldpoint = point;
				ReleaseDC(pDC);
				return;
			}

			if(m_bMoveLongLine) {//������
				if((m_sdotOldpoint.x < g_draw_startpoint_y + m_Position_X0[1][m_Long_Moving_Tab -1]*8 +4)
					||(m_sdotOldpoint.x > g_draw_startpoint_y + m_Position_X1[1][m_Long_Moving_Tab]*8 -1))  { //�ж�Խ��
					m_bthisMoveInValid = true;
					m_sdotOldpoint = point;
					ReleaseDC(pDC);
					return;
				}
				pDC->MoveTo(m_sdotOldpoint.x,g_draw_startpoint_x + m_Position_Y0[1][m_Long_Moving_Tab]*8);
				pDC->LineTo(m_sdotOldpoint.x,g_draw_startpoint_x + m_Position_Y0[7][m_Long_Moving_Tab]*8);
				pDC->MoveTo(point.x,g_draw_startpoint_x + m_Position_Y0[1][m_Long_Moving_Tab]*8);
				pDC->LineTo(point.x,g_draw_startpoint_x + m_Position_Y0[7][m_Long_Moving_Tab]*8);					
									
				m_sdotOldpoint = point;
				ReleaseDC(pDC);
				return;
			}
			else{// ����ֱ����
				if(m_bShort_Line_Valid) { //��ֱ�����ˮƽ������Ч
					if((m_sdotOldpoint.y < g_draw_startpoint_x + m_Position_Y0[m_Moving_Tab_Ver-1][m_Moving_Tab_Hor]*8 +4)
						||(m_sdotOldpoint.y > g_draw_startpoint_x + m_Position_Y1[m_Moving_Tab_Ver][m_Moving_Tab_Hor]*8 -1))  { //�ж�Խ��
						m_sdotOldpoint = point;
						m_bthisMoveInValid = true;
						ReleaseDC(pDC);
						return;
					}

					pDC->MoveTo(g_draw_startpoint_y + m_Position_X0[m_Moving_Tab_Ver][m_Moving_Tab_Hor]*8,m_sdotOldpoint.y);
					if(m_Moving_Tab_Hor == 7) pDC->LineTo(g_draw_startpoint_y + m_Position_X1[m_Moving_Tab_Ver][m_Moving_Tab_Hor]*8,m_sdotOldpoint.y);
					else pDC->LineTo(g_draw_startpoint_y + (m_Position_X1[m_Moving_Tab_Ver][m_Moving_Tab_Hor]+1)*8,m_sdotOldpoint.y);
					pDC->MoveTo(g_draw_startpoint_y + m_Position_X0[m_Moving_Tab_Ver][m_Moving_Tab_Hor]*8,point.y);
					if(m_Moving_Tab_Hor == 7) pDC->LineTo(g_draw_startpoint_y + m_Position_X1[m_Moving_Tab_Ver][m_Moving_Tab_Hor]*8,point.y);
					else pDC->LineTo(g_draw_startpoint_y + (m_Position_X1[m_Moving_Tab_Ver][m_Moving_Tab_Hor]+1)*8,point.y);

					m_sdotOldpoint = point;
					ReleaseDC(pDC);
					return;
				}


				if(m_Moving_Tab_Ver == 0) {// ��ֱǰ����
					if((m_sdotOldpoint.x < g_draw_startpoint_y + m_Position_X0[0][m_Moving_Tab_Hor -1]*8 +4)
						||(m_sdotOldpoint.x > g_draw_startpoint_y + m_Position_X1[0][m_Moving_Tab_Hor]*8 -1))  { //�ж�Խ��
						m_sdotOldpoint = point;
						m_bthisMoveInValid = true;
						return;
					}
					pDC->MoveTo(m_sdotOldpoint.x,g_draw_startpoint_x + m_Position_Y0[0][m_Moving_Tab_Hor]*8);
					pDC->LineTo(m_sdotOldpoint.x,g_draw_startpoint_x + (m_Position_Y1[0][m_Moving_Tab_Hor]+1)*8);
					pDC->MoveTo(point.x,g_draw_startpoint_x + m_Position_Y0[0][m_Moving_Tab_Hor]*8);
					pDC->LineTo(point.x,g_draw_startpoint_x + (m_Position_Y1[0][m_Moving_Tab_Hor]+1)*8);					
										
					m_sdotOldpoint = point;
					ReleaseDC(pDC);
					return;
				}
				else { //��ֱ�����
					if((m_sdotOldpoint.x < g_draw_startpoint_y + m_Position_X0[7][m_Moving_Tab_Hor -1]*8 +4)
						||(m_sdotOldpoint.x > g_draw_startpoint_y + m_Position_X1[7][m_Moving_Tab_Hor]*8 -1))  { //�ж�Խ��
						m_sdotOldpoint = point;
						m_bthisMoveInValid = true;
						return;
					}
					pDC->MoveTo(m_sdotOldpoint.x,g_draw_startpoint_x + m_Position_Y0[7][m_Moving_Tab_Hor]*8);
					pDC->LineTo(m_sdotOldpoint.x,g_draw_startpoint_x + m_Position_Y1[7][m_Moving_Tab_Hor]*8);
					pDC->MoveTo(point.x,g_draw_startpoint_x + m_Position_Y0[7][m_Moving_Tab_Hor]*8);
					pDC->LineTo(point.x,g_draw_startpoint_x + m_Position_Y1[7][m_Moving_Tab_Hor]*8);					
										
					m_sdotOldpoint = point;
					ReleaseDC(pDC);
					return;
				}
			}
		}

	}

	ReleaseDC(pDC);

}

void CDynShowDlg::OnLButtonDown(UINT nFlags, CPoint point) 
{
	CDialog::OnLButtonDown(nFlags, point);
	m_Oldpoint = point;
	
	//----------��ƽ���������±߽硿--------//
	if((point.y<265)&&(point.y>188)) {
		if((point.x >= (650+ileft_limit*4 - 2))&&(point.x <= (650+ileft_limit*4 + 2))) {
			m_bMove_LeftLimit = true;
			m_bMove_RightLimit = false;
		}
		else if((point.x >= (650+iright_limit*4 - 2))&&(point.x <= (650+iright_limit*4 + 2))) {
			m_bMove_LeftLimit = false;
			m_bMove_RightLimit = true;
		}
	}

	//-------��ȱȲ���-----//
	if(m_bEnShowPeakValeRatio) { // ���ײ��� ѡ��
		int selnum;
		bool flag = false;
		for(int i=0;i<8;i++) { //ˮƽ
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



	//---------��ɢ���λ�ñ߽�ƽ�ơ�------//
//	SetCapture();	
	CRect rect;
	GetClientRect(&rect);
	ClientToScreen(&rect);

	CDC* pDC=GetDC();
	CPen pShow(PS_SOLID,2,RGB(15, 105,185));//RGB(0,210,0)
	CPen* pOldPen=NULL;
	pOldPen=pDC->SelectObject(&pShow); 
	pDC->SetROP2(R2_NOT);

	if(m_bSameDriction_X) { //ˮƽ����һ��
		if(!m_bMoveWholeLine) { //��ˮƽ��ϵĶ��ߣ�
			for(int i=1;i<8;i++) {			
				for(int no=1;no<=7;no++) {  //
					if((point.x <= g_draw_startpoint_y + m_Position_X1[i][no]*8)&&(point.x >= g_draw_startpoint_y + m_Position_X0[i][no]*8)) {
						if((point.y <= g_draw_startpoint_x + m_Position_Y0[i][no]*8 +3)&&(point.y >= g_draw_startpoint_x + m_Position_Y0[i][no]*8 -3)) {
							if((m_Position_X1[i-1][no] == m_Position_X1[i][no])&&(m_Position_X0[i-1][no] == m_Position_X0[i][no])){  //�ж�ǰ���Ƿ�ƽ��
								
									m_iMoveCurLine_X = no;
									m_iMoveCurLine_Y = i;	
									m_bMovingVerDirection = false;
									m_bEnMoveBreakLine = true;
									m_bButtonDown = true;

									m_sdotOldpoint = point;
									if(m_iMoveCurLine_X < 7) {
										pDC->MoveTo(g_draw_startpoint_y + m_Position_X0[m_iMoveCurLine_Y][m_iMoveCurLine_X]*8,g_draw_startpoint_x + m_Position_Y0[m_iMoveCurLine_Y][m_iMoveCurLine_X]*8);
										pDC->LineTo(g_draw_startpoint_y + (m_Position_X1[m_iMoveCurLine_Y][m_iMoveCurLine_X]+1)*8,g_draw_startpoint_x + m_Position_Y0[m_iMoveCurLine_Y][m_iMoveCurLine_X]*8);
										pDC->MoveTo(g_draw_startpoint_y + m_Position_X0[m_iMoveCurLine_Y][m_iMoveCurLine_X]*8,m_sdotOldpoint.y);
										pDC->LineTo(g_draw_startpoint_y + (m_Position_X1[m_iMoveCurLine_Y][m_iMoveCurLine_X]+1)*8,m_sdotOldpoint.y);	
									}
									else {
										pDC->MoveTo(g_draw_startpoint_y + m_Position_X0[m_iMoveCurLine_Y][m_iMoveCurLine_X]*8,g_draw_startpoint_x + m_Position_Y0[m_iMoveCurLine_Y][m_iMoveCurLine_X]*8);
										pDC->LineTo(g_draw_startpoint_y + (m_Position_X1[m_iMoveCurLine_Y][m_iMoveCurLine_X])*8,g_draw_startpoint_x + m_Position_Y0[m_iMoveCurLine_Y][m_iMoveCurLine_X]*8);
										pDC->MoveTo(g_draw_startpoint_y + m_Position_X0[m_iMoveCurLine_Y][m_iMoveCurLine_X]*8,m_sdotOldpoint.y);
										pDC->LineTo(g_draw_startpoint_y + (m_Position_X1[m_iMoveCurLine_Y][m_iMoveCurLine_X])*8,m_sdotOldpoint.y);	
									
									}
									
									ReleaseDC(pDC);
									return;								
							}
						}
					}
				}				
			}  // end of for
		}

		for(int i=1;i<8;i++) { //��ˮƽ��
			//��ˮƽ����			
			for(int no=1;no<7;no++) {
					if((point.x <= g_draw_startpoint_y + m_Position_X1[i][no]*8)&&(point.x >= g_draw_startpoint_y + m_Position_X0[i][no]*8)) {
						if((point.y <= g_draw_startpoint_x + m_Position_Y0[i][no]*8 +3)&&(point.y >= g_draw_startpoint_x + m_Position_Y0[i][no]*8 -3)) {
							if((m_Position_X1[i-1][0] == m_Position_X1[i][0])&&(m_Position_X0[i-1][7] == m_Position_X0[i][7])){  //�ж�ǰ���Ƿ�ƽ��	
								if(!m_bMoveWholeLine) { //���ж� �� ˮƽ��ϵĶ���
									ReleaseDC(pDC);
									return;
								}
									m_bMoveLongLine = true;						
									m_Long_Moving_Tab = i;	//��Ч��Χ0��7									
									m_bButtonDown = true;
									m_Moving_Tab_Hor = 0;
									m_Moving_Tab_Ver = 0;

									m_sdotOldpoint = point;
									pDC->MoveTo(g_draw_startpoint_y + m_Position_X0[i][1]*8,g_draw_startpoint_x + m_Position_Y0[i][no]*8);
									pDC->LineTo(g_draw_startpoint_y + m_Position_X0[i][7]*8,g_draw_startpoint_x + m_Position_Y0[i][no]*8);
									pDC->MoveTo(g_draw_startpoint_y + m_Position_X0[i][1]*8,m_sdotOldpoint.y);
									pDC->LineTo(g_draw_startpoint_y + m_Position_X0[i][7]*8,m_sdotOldpoint.y);	
									ReleaseDC(pDC);
									return;								
							}
					}
				}
			}
			//��ˮƽǰ����		
			if((point.y <= g_draw_startpoint_x + m_Position_Y0[i][0]*8 +3)&&(point.y >= g_draw_startpoint_x + m_Position_Y0[i][0]*8 -3)) {						
				if((point.x < g_draw_startpoint_y + m_Position_X0[i][1]*8)&&(point.x > g_draw_startpoint_y + m_Position_X0[i][0]*8)) { //ˮƽǰһ����
					if(m_Position_X1[i-1][0] == m_Position_X1[i][0]){  //�ж�ǰ��/�����Ƿ�ƽ��
						m_bMoveLongLine = false;
						m_Long_Moving_Tab = 0;	//��Ч��Χ0��7
						m_bButtonDown = true;
						m_Moving_Tab_Hor = 0;
						m_Moving_Tab_Ver = i;

						m_sdotOldpoint = point;
						pDC->MoveTo(g_draw_startpoint_y + m_Position_X0[i][0]*8,g_draw_startpoint_x + m_Position_Y0[i][0]*8);
						pDC->LineTo(g_draw_startpoint_y + (m_Position_X1[i][0]+1)*8,g_draw_startpoint_x + m_Position_Y0[i][0]*8);
						pDC->MoveTo(g_draw_startpoint_y + m_Position_X0[i][0]*8,m_sdotOldpoint.y);
						pDC->LineTo(g_draw_startpoint_y + (m_Position_X1[i][0]+1)*8,m_sdotOldpoint.y);	
						ReleaseDC(pDC);
						return;
					}
				}				
			}
			//��ˮƽ�����
			if((point.y <= g_draw_startpoint_x + m_Position_Y0[i][7]*8 +3)&&(point.y >= g_draw_startpoint_x + m_Position_Y0[i][7]*8 -3)) {
				if((point.x < g_draw_startpoint_y + m_Position_X1[i][7]*8)&&(point.x > g_draw_startpoint_y + m_Position_X0[i][7]*8)) { //ˮƽ��һ����
					if(m_Position_X0[i-1][7] == m_Position_X0[i][7]){  //�ж�ǰ��/�����Ƿ�ƽ��
						m_bMoveLongLine = false;
						m_Long_Moving_Tab = 0;	//��Ч��Χ0��7
						m_bButtonDown = true;
						m_Moving_Tab_Hor = 7;
						m_Moving_Tab_Ver = i;

						m_sdotOldpoint = point;
						pDC->MoveTo(g_draw_startpoint_y + m_Position_X0[i][7]*8,g_draw_startpoint_x + m_Position_Y0[i][7]*8);
						pDC->LineTo(g_draw_startpoint_y + m_Position_X1[i][7]*8,g_draw_startpoint_x + m_Position_Y0[i][7]*8);
						pDC->MoveTo(g_draw_startpoint_y + m_Position_X0[i][7]*8,m_sdotOldpoint.y);
						pDC->LineTo(g_draw_startpoint_y + m_Position_X1[i][7]*8,m_sdotOldpoint.y);	
						ReleaseDC(pDC);
						return;
					}
				}
			}
		}
		//-----��ˮƽ����Ĵ�ֱ���ߡ�------//		
			for(i=0;i<8;i++) {				
				for(int j=1;j<8;j++) {
					if((point.x < g_draw_startpoint_y + m_Position_X0[i][j]*8+5)&&(point.x > g_draw_startpoint_y + m_Position_X0[i][j]*8-5)) {
						if((point.y < g_draw_startpoint_x + m_Position_Y1[i][j]*8)&&(point.y > g_draw_startpoint_x + m_Position_Y0[i][j]*8)) {
							if((m_Position_Y0[i][j-1] == m_Position_Y0[i][j])&&(m_Position_Y1[i][j-1] == m_Position_Y1[i][j])){  //�ж�ǰ��/�����Ƿ�ƽ��
								m_bMoveLongLine = false;
								m_Long_Moving_Tab = 0;	//��Ч��Χ0��7
								m_bShort_Line_Valid = true;
								m_bButtonDown = true;
								m_Moving_Tab_Hor = j;
								m_Moving_Tab_Ver = i;

								m_sdotOldpoint = point;
								pDC->MoveTo(g_draw_startpoint_y + m_Position_X0[i][j]*8,g_draw_startpoint_x + m_Position_Y0[i][j]*8);
								if(i==7) pDC->LineTo(g_draw_startpoint_y + m_Position_X0[i][j]*8,g_draw_startpoint_x + m_Position_Y1[i][j]*8);
								else pDC->LineTo(g_draw_startpoint_y + m_Position_X0[i][j]*8,g_draw_startpoint_x + (m_Position_Y1[i][j]+1)*8);
								pDC->MoveTo(m_sdotOldpoint.x,g_draw_startpoint_x + m_Position_Y0[i][j]*8);
								if(i==7) pDC->LineTo(m_sdotOldpoint.x,g_draw_startpoint_x + m_Position_Y1[i][j]*8);	
								else pDC->LineTo(m_sdotOldpoint.x,g_draw_startpoint_x + (m_Position_Y1[i][j]+1)*8);	
								ReleaseDC(pDC);
								return;
							}
						}
					}
				}
			}

		

	}
	else {   //��ֱ����һ��

		if(!m_bMoveWholeLine) { //����ֱ��ϵĶ��ߣ�
			for(int i=1;i<8;i++) {			
				for(int no=1;no<=7;no++) {
					if((point.y <= g_draw_startpoint_x + m_Position_Y1[i][no]*8)&&(point.y >= g_draw_startpoint_x + m_Position_Y0[i][no]*8)) {
						if((point.x <= g_draw_startpoint_y + m_Position_X0[i][no]*8 +3)&&(point.x >= g_draw_startpoint_y + m_Position_X0[i][no]*8 -3)) {
							if((m_Position_Y1[i][no-1] == m_Position_Y1[i][no])&&(m_Position_Y0[i][no-1] == m_Position_Y0[i][no])){  //�ж�ǰ���Ƿ�ƽ��
								
									m_iMoveCurLine_X = no;
									m_iMoveCurLine_Y = i;	

									m_bMovingVerDirection = true;
									m_bEnMoveBreakLine = true;
									m_bButtonDown = true;						 

									m_sdotOldpoint = point;

									if(m_iMoveCurLine_Y <7) {
										pDC->MoveTo(g_draw_startpoint_y + m_Position_X0[m_iMoveCurLine_Y][m_iMoveCurLine_X]*8,g_draw_startpoint_x + m_Position_Y0[m_iMoveCurLine_Y][m_iMoveCurLine_X]*8);
										pDC->LineTo(g_draw_startpoint_y + m_Position_X0[m_iMoveCurLine_Y][m_iMoveCurLine_X]*8,g_draw_startpoint_x + (m_Position_Y1[m_iMoveCurLine_Y][m_iMoveCurLine_X]+1)*8);
										pDC->MoveTo(m_sdotOldpoint.x,g_draw_startpoint_x + m_Position_Y0[m_iMoveCurLine_Y][m_iMoveCurLine_X]*8);
										pDC->LineTo(m_sdotOldpoint.x,g_draw_startpoint_x + (m_Position_Y1[m_iMoveCurLine_Y][m_iMoveCurLine_X]+1)*8);	
									}
									else {
										pDC->MoveTo(g_draw_startpoint_y + m_Position_X0[m_iMoveCurLine_Y][m_iMoveCurLine_X]*8,g_draw_startpoint_x + m_Position_Y0[m_iMoveCurLine_Y][m_iMoveCurLine_X]*8);
										pDC->LineTo(g_draw_startpoint_y + m_Position_X0[m_iMoveCurLine_Y][m_iMoveCurLine_X]*8,g_draw_startpoint_x + (m_Position_Y1[m_iMoveCurLine_Y][m_iMoveCurLine_X])*8);
										pDC->MoveTo(m_sdotOldpoint.x,g_draw_startpoint_x + m_Position_Y0[m_iMoveCurLine_Y][m_iMoveCurLine_X]*8);
										pDC->LineTo(m_sdotOldpoint.x,g_draw_startpoint_x + (m_Position_Y1[m_iMoveCurLine_Y][m_iMoveCurLine_X])*8);

									}

									ReleaseDC(pDC);
									return;

							}


						}
					}
				}				
			}  // end of for
		}

		for(int i=1;i<8;i++) { //����ֱ����
			for(int no=1;no<=7;no++) {
				if((point.y <= g_draw_startpoint_x + m_Position_Y1[i][no]*8)&&(point.y >= g_draw_startpoint_x + m_Position_Y0[i][no]*8)) {
					if((point.x <= g_draw_startpoint_y + m_Position_X0[i][no]*8 +3)&&(point.x >= g_draw_startpoint_y + m_Position_X0[i][no]*8 -3)) {
						if((m_Position_Y1[0][no-1] == m_Position_Y1[0][no])&&(m_Position_Y0[7][no-1] == m_Position_Y0[7][no])){  //�ж�ǰ���Ƿ�ƽ��	
								
							if(!m_bMoveWholeLine) { //���ж� �� ��ֱ��ϵĶ���
								ReleaseDC(pDC);
								return;
							}	
							if(i < 7) {
								m_bMoveLongLine = true;
								m_Long_Moving_Tab = no;	//��Ч��Χ0��7  
								m_bButtonDown = true;

								m_sdotOldpoint = point;
								pDC->MoveTo(g_draw_startpoint_y + m_Position_X0[i][no]*8,g_draw_startpoint_x + m_Position_Y0[1][no]*8);
								pDC->LineTo(g_draw_startpoint_y + m_Position_X0[i][no]*8,g_draw_startpoint_x + m_Position_Y0[7][no]*8);
								pDC->MoveTo(m_sdotOldpoint.x,g_draw_startpoint_x + m_Position_Y0[1][no]*8);
								pDC->LineTo(m_sdotOldpoint.x,g_draw_startpoint_x + m_Position_Y0[7][no]*8);	
								ReleaseDC(pDC);
								return;
							}
						}
					}
				}
			}
			//����ֱǰ����		
			if((point.x <= g_draw_startpoint_y + m_Position_X0[0][i]*8 +3)&&(point.x >= g_draw_startpoint_y + m_Position_X0[0][i]*8 -3)) {						
				if((point.y < g_draw_startpoint_x + m_Position_Y1[0][i]*8)&&(point.y > g_draw_startpoint_x + m_Position_Y0[0][i]*8)) { //��ֱǰһ����
					if(m_Position_Y1[0][i-1] == m_Position_Y1[0][i]){  //�ж�ǰ��/�����Ƿ�ƽ��
						m_bMoveLongLine = false;
						m_Long_Moving_Tab = 0;	//��Ч��Χ0��7
						m_bButtonDown = true;
						m_Moving_Tab_Hor = i;
						m_Moving_Tab_Ver = 0;

						m_sdotOldpoint = point;
						pDC->MoveTo(g_draw_startpoint_y + m_Position_X0[0][i]*8,g_draw_startpoint_x + m_Position_Y0[0][i]*8);
						pDC->LineTo(g_draw_startpoint_y + m_Position_X0[0][i]*8,g_draw_startpoint_x + (m_Position_Y1[0][i]+1)*8);
						pDC->MoveTo(m_sdotOldpoint.x,g_draw_startpoint_x + m_Position_Y0[0][i]*8);
						pDC->LineTo(m_sdotOldpoint.x,g_draw_startpoint_x + (m_Position_Y1[0][i]+1)*8);	
						ReleaseDC(pDC);
						return;
					}
				}				
			}
			//����ֱ��һ����
			if((point.x <= g_draw_startpoint_y + m_Position_X0[7][i]*8 +3)&&(point.x >= g_draw_startpoint_y + m_Position_X0[7][i]*8 -3)) {						
				if((point.y < g_draw_startpoint_x + m_Position_Y1[7][i]*8)&&(point.y > g_draw_startpoint_x + m_Position_Y0[7][i]*8)) { //��ֱ��һ����
					if(m_Position_Y0[7][i-1] == m_Position_Y0[7][i]){  //�ж�ǰ��/�����Ƿ�ƽ��
						m_bMoveLongLine = false;
						m_Long_Moving_Tab = 0;	//��Ч��Χ0��7
						m_bButtonDown = true;
						m_Moving_Tab_Hor = i;
						m_Moving_Tab_Ver = 7;

						m_sdotOldpoint = point;
						pDC->MoveTo(g_draw_startpoint_y + m_Position_X0[7][i]*8,g_draw_startpoint_x + m_Position_Y0[7][i]*8);
						pDC->LineTo(g_draw_startpoint_y + m_Position_X0[7][i]*8,g_draw_startpoint_x + m_Position_Y1[7][i]*8);
						pDC->MoveTo(m_sdotOldpoint.x,g_draw_startpoint_x + m_Position_Y0[7][i]*8);
						pDC->LineTo(m_sdotOldpoint.x,g_draw_startpoint_x + m_Position_Y1[7][i]*8);	
						ReleaseDC(pDC);
						return;
					}
				}				
			}
		}
		//-----����ֱ�����ˮƽ���ߡ�------//
		{
			for(int i=0;i<8;i++) { //ˮƽ����				
				for(int j=1;j<8;j++) { //��ֱ����
					if((point.y < g_draw_startpoint_x + m_Position_Y0[j][i]*8+5)&&(point.y > g_draw_startpoint_x + m_Position_Y0[j][i]*8-5)) {
						if((point.x < g_draw_startpoint_y + m_Position_X1[j][i]*8)&&(point.x > g_draw_startpoint_y + m_Position_X0[j][i]*8)) {
							if((m_Position_X0[j-1][i] == m_Position_X0[j][i])&&(m_Position_X1[j-1][i] == m_Position_X1[j][i])){  //�ж�ǰ��/�����Ƿ�ƽ��
								m_bMoveLongLine = false;
								m_Long_Moving_Tab = 0;	//��Ч��Χ0��7
								m_bShort_Line_Valid = true;
								m_bButtonDown = true;
								m_Moving_Tab_Hor = i;
								m_Moving_Tab_Ver = j;

								m_sdotOldpoint = point;
								pDC->MoveTo(g_draw_startpoint_y + m_Position_X0[j][i]*8,g_draw_startpoint_x + m_Position_Y0[j][i]*8);
								if( i==7) pDC->LineTo(g_draw_startpoint_y + m_Position_X1[j][i]*8,g_draw_startpoint_x + m_Position_Y0[j][i]*8);
								else pDC->LineTo(g_draw_startpoint_y + (m_Position_X1[j][i]+1)*8,g_draw_startpoint_x + m_Position_Y0[j][i]*8);
								pDC->MoveTo(g_draw_startpoint_y + m_Position_X0[j][i]*8,m_sdotOldpoint.y);
								if(i==7) pDC->LineTo(g_draw_startpoint_y + m_Position_X1[j][i]*8,m_sdotOldpoint.y);	
								else pDC->LineTo(g_draw_startpoint_y + (m_Position_X1[j][i]+1)*8,m_sdotOldpoint.y);	
								ReleaseDC(pDC);
								return;
							}
						 
						}
					}
				}
			}
		} //end of ����ֱ�����ˮƽ���ߡ�

	}

	ReleaseDC(pDC);

}

void CDynShowDlg::OnLButtonUp(UINT nFlags, CPoint point) 
{
	ReleaseCapture();
	ClipCursor(NULL);
	CDialog::OnLButtonUp(nFlags, point);

	//----------��ƽ���������±߽硿--------//
	if((m_bMove_LeftLimit)||(m_bMove_RightLimit)) {
		m_bMove_LeftLimit = false;
		m_bMove_RightLimit = false;
		OnRedraw_2d();
	}
	//---------��ɢ���λ�ñ߽�ƽ�ơ�------//
	if(!m_bButtonDown) return;

	if((m_bEnMoveBreakLine)) {  //����ƽ��
		m_bEnMoveBreakLine = false;
		m_bButtonDown = false;	
		if(m_bthisMoveInValid) {  //�ƶ��Ƿ���Ч
			m_bthisMoveInValid = false;
			return;
		}
		if(!m_bMovingVerDirection) {  //ˮƽ����
			int pointvalue = (point.y-g_draw_startpoint_x +4)/8;
			if((pointvalue > 63)||(pointvalue <0)) return;
			m_Position_Y0[m_iMoveCurLine_Y][m_iMoveCurLine_X] = pointvalue;
			m_Position_Y1[m_iMoveCurLine_Y-1][m_iMoveCurLine_X] = pointvalue-1;
		}
		if(m_bMovingVerDirection) {  //��ֱ����
			int pointvalue = (point.x-g_draw_startpoint_y +4)/8;
			if((pointvalue > 63)||(pointvalue <0)) return;
			m_Position_X0[m_iMoveCurLine_Y][m_iMoveCurLine_X] = pointvalue;
			m_Position_X1[m_iMoveCurLine_Y][m_iMoveCurLine_X-1] = pointvalue-1;
		}	
		RedrawLimitFile(); ///akang
		return;
	}

	if(m_bMoveLongLine) { //����
		if(m_bSameDriction_X) {//ˮƽ����
			int pointvalue = (point.y-g_draw_startpoint_x +4)/8;
			if((pointvalue > 63)||(pointvalue <0)) return;
			for(int i=1;i<7;i++) {
				if(!m_bthisMoveInValid) {
					m_Position_Y0[m_Long_Moving_Tab][i] = pointvalue;
					m_Position_Y1[m_Long_Moving_Tab-1][i] = pointvalue-1;
				}
			}			
		}
		else { //��ֱ����
			int pointvalue = (point.x-g_draw_startpoint_y +4)/8;
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
	else { //����
		if(m_bSameDriction_X) { //ˮƽ����
			int pointvalue = (point.y-g_draw_startpoint_x +4)/8;
			if((pointvalue > 63)||(pointvalue <0)) return;

			if(m_bShort_Line_Valid) {  //ˮƽ����һ�µ� ��ֱ������Ч
				pointvalue = (point.x-g_draw_startpoint_y +4)/8;
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
			if(m_Moving_Tab_Hor == 0) { //ǰһ��ˮƽ����
				if(!m_bthisMoveInValid) {
					m_Position_Y0[m_Moving_Tab_Ver][0] = pointvalue;
					m_Position_Y1[m_Moving_Tab_Ver-1][0] = pointvalue-1;					
				}
			}
			else{//��һ��ˮƽ����
				if(!m_bthisMoveInValid) {
					m_Position_Y0[m_Moving_Tab_Ver][7] = pointvalue;
					m_Position_Y1[m_Moving_Tab_Ver-1][7] = pointvalue-1;					
				}
			}		

		}
		else { // ��ֱ����
			int pointvalue = (point.x-g_draw_startpoint_y +4)/8;
			if((pointvalue > 63)||(pointvalue <0)) return;

			if(m_bShort_Line_Valid) {  //��ֱ����һ�µ� ˮƽ������Ч
				pointvalue = (point.y-g_draw_startpoint_x +4)/8;
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

			if(m_Moving_Tab_Ver == 0) { //ǰһ�δ�ֱ����
				if(!m_bthisMoveInValid) {
					m_Position_X0[0][m_Moving_Tab_Hor] = pointvalue;
					m_Position_X1[0][m_Moving_Tab_Hor-1] = pointvalue-1;					
				}
			}
			else{//��һ�δ�ֱ����]
				if(!m_bthisMoveInValid) {
					m_Position_X0[7][m_Moving_Tab_Hor] = pointvalue;
					m_Position_X1[7][m_Moving_Tab_Hor-1] = pointvalue-1;					
				}
			}
		}
		m_bthisMoveInValid = false;
		m_bMoveLongLine = false;
		m_bButtonDown = false;	
		
		RedrawLimitFile(); ///akang
	}

}

void CDynShowDlg::OnEnergerSpectrum_Sel() 
{
	// TODO: Add your control notification handler code here
	m_iSmallMap_Show_Sel = 1;
	SetDlgItemText(IDC_SMALL_MAP_TITILE,"������ʾͼ ");

	OnRedraw_2d();
}

void CDynShowDlg::OnXDirection_Sel() 
{
	// TODO: Add your control notification handler code here
	m_iSmallMap_Show_Sel = 2;
	SetDlgItemText(IDC_SMALL_MAP_TITILE,"Y ����ͶӰͼ ");

	OnRedraw_2d();
}

void CDynShowDlg::OnYDirection_Sel() 
{
	// TODO: Add your control notification handler code here
	m_iSmallMap_Show_Sel = 3;
	SetDlgItemText(IDC_SMALL_MAP_TITILE,"X ����ͶӰͼ ");

	OnRedraw_2d();
}

void CDynShowDlg::OnTimeSpectrumSel() 
{
	// TODO: Add your control notification handler code here
	m_iSmallMap_Show_Sel = 4;
	SetDlgItemText(IDC_SMALL_MAP_TITILE," ʱ��ֱ��ͼ ");

	OnRedraw_2d();
}
void CDynShowDlg::On2d_SynEnergeSpectrumSel() 
{
	// TODO: Add your control notification handler code here
	m_iSmallMap_Show_Sel = 5;
	SetDlgItemText(IDC_SMALL_MAP_TITILE," ������������ʾ ");

	OnRedraw_2d();
	
}
void CDynShowDlg::OnTimeFuheSel() 
{
	m_iSmallMap_Show_Sel = SEL_2D_TIME_FUHE;
	SetDlgItemText(IDC_SMALL_MAP_TITILE," ʱ�������ʾ ");

	OnRedraw_2d();
}


void CDynShowDlg::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	int nusbwrite;
	switch(nIDEvent) {
		case 1:	{
				KillTimer(1);
				g_bRunCommThread = false;
				m_iSampleCount = 0;
				m_RealTimeFile.Close();	
				SetDlgItemText(IDC_STATIC_FILE_NAME,"������ݲɼ�");
				OnRedraw_2d();
				OnRedraw_3d();
				GetDlgItem(IDC_START_SAMPLE)->EnableWindow(true);	
				GetDlgItem(IDC_PROG_CONTROL)->EnableWindow(true);
				GetDlgItem(IDC_SAMPLE_MODE_SEL)->EnableWindow(true);
				GetDlgItem(IDOK)->EnableWindow(true);	
				
				//delete m_pThread_Comm;
				break; };
		case 2:	 {				
				if (m_bSampleTimeMode) {				
					m_iSampleTimeCounter ++;
					if(m_iSampleTimeCounter >= m_iSampleTime) {
						KillTimer(2);						
						OnStopSample();						
						g_bRunCommThread = false;
						ASSERT(SetTimer(1,2500,NULL)!=0);				

						GetDlgItem(IDC_STOP_SAMPLE)->EnableWindow(false);
					}
				}
				OnRedraw_2d();
				OnRedraw_3d();

				break;}
		default: break;
	};	

	CDialog::OnTimer(nIDEvent);
}

void CDynShowDlg::OnCompareStat() 
{
	// TODO: Add your control notification handler code here
	AfxMessageBox("�˹�����ʱ��������");
	return;

	int X0[8][8],Y0[8][8],X1[8][8],Y1[8][8];

	CString FilePathName; 
	long datacounter;
	CFile file;
	DWORD filelenth;

	CFileDialog dlg3(TRUE,"dat","*.dat");///TRUEΪOPEN�Ի���FALSEΪSAVE AS�Ի��� 
	dlg3.m_ofn.lpstrFilter = ("��λ�ñ߽��ļ���""*.dat");//
	if(dlg3.DoModal()==IDOK) FilePathName=dlg3.GetPathName(); 
	else return;

	file.Open(FilePathName,CFile::modeRead);
	filelenth = file.GetLength();
	datacounter = filelenth/4;

	if(datacounter < 32+ 8*8) {
		AfxMessageBox("���ļ���С���ԣ����ʵ");
		return;
	}

	unsigned char filehead[4];
	file.Read(filehead,4);

	if((filehead[0]==0xBB)&&(filehead[1]==0xBB)) {  //�ļ�ͷ  �ж�
		unsigned char chartemp[4];
		for(int i=0;i<124;i++) {   // 128 
			file.Read(chartemp,1);
		}
		for(i=0;i<8;i++) {
			for(int j=0;j<8;j++) {
				file.Read(chartemp,4);			
				X0[i][j] = chartemp[0];
				Y0[i][j] = chartemp[1];
				X1[i][j] = chartemp[2];
				Y1[i][j] = chartemp[3];

				m_Position_X0[i][j] = (int)chartemp[0];
				m_Position_Y0[i][j] = (int)chartemp[1];
				m_Position_X1[i][j] = (int)chartemp[2];
				m_Position_Y1[i][j] = (int)chartemp[3];
			}
		}
		file.Close();
		m_bSameDriction_X = true;
		for(i=1;i<7;i++) {
			for(int j=1;j<6;j++) {
				if(m_Position_Y0[i][j] != m_Position_Y0[i][j+1]) {
					m_bSameDriction_X = false; 
				}
			}
		}

	}

//-------------------------------------------------------------------------------
	CFileDialog dlg1(TRUE,"dat","*.dat");///TRUEΪOPEN�Ի���FALSEΪSAVE AS�Ի��� 
	dlg1.m_ofn.lpstrFilter = ("��ɢ���ļ�1��""*.dat");//
	if(dlg1.DoModal()==IDOK) FilePathName=dlg1.GetPathName(); 
	else return;

	file.Open(FilePathName,CFile::modeRead);
	filelenth = file.GetLength();
	unsigned char byteX[1],byteY[1],byteE[1],byteT[1];
	datacounter = filelenth/4;
	
	m_FileName1 = FilePathName;
	SetDlgItemText(IDC_STATIC_FILE_NAME,FilePathName);

	if(datacounter < 32+ 64*64) {
		AfxMessageBox("���ļ���С���ԣ����ʵ");
		return;
	}
	for(int i=0;i<64;i++){			//���64��64 ������	
		for(int j=0;j<64;j++){
			m_sdot[i][j] = 0;
			m_isdot_datafile[i][j] = 0;
			m_iEth_datafile[i][j] = 0; //������������
		}
		m_Energer_Spectrum[i] = 0;		
	}
	for(i=0;i<128;i++) {
		m_Time_Spectrum[i] = 0;
	}



	file.Read(byteX,1);
	file.Read(byteY,1);
	file.Read(byteE,1);
	file.Read(byteT,1);
	if((byteX[0]==0xc7)&&(byteY[0]==0xc8)) {  // ���������ɢ������
		unsigned char chartemp[4];
		for(i=0;i<124;i++) {   // 128 �ļ�ͷ
			file.Read(chartemp,1);
		}
		for(i=0;i<64;i++) {
			for(int j=0;j<64;j++) {
				file.Read(chartemp,4);			
				m_sdot[i][j] = chartemp[3];
				m_sdot[i][j] = (m_sdot[i][j] << 8);
				m_sdot[i][j] += chartemp[2];
				m_sdot[i][j] = (m_sdot[i][j] << 8);
				m_sdot[i][j] += chartemp[1];
				m_sdot[i][j] = (m_sdot[i][j] << 8);
				m_sdot[i][j] += chartemp[0];

				m_sdot_file1[i][j] = m_sdot[i][j];
				m_isdot_datafile[i][j] = m_sdot[i][j];
			}
		}
		file.Close();
	} 
	else AfxMessageBox("�򿪵��ļ�1 ���Ͳ���");
//-------------------------------------------

	for(i=0;i<8;i++) {
		for(int j=0;j<8;j++) {
			m_Limit_Stat[i][j] = 0;
		}
	}	

	for(i=0;i<8;i++) { //������
		for(int j=0;j<8;j++) { //������
			for(int k=X0[i][j];k<=X1[i][j];k++) {
				for(int m=Y0[i][j];m<=Y1[i][j];m++) {
					m_Limit_Stat[i][j] +=  m_sdot[m][k];					
				}
			}
		}
	}

	int max_stat= m_Limit_Stat[0][0];
	for(i=0;i<8;i++) {
		for(int j=0;j<8;j++){
			if(m_Limit_Stat[i][j] > max_stat) 
				max_stat= m_Limit_Stat[i][j];
		}
	}
	if(max_stat < 1) max_stat=1;
	for(i=0;i<8;i++) {
		for(int j=0;j<8;j++){
			m_igoto_one_first[i][j] = 100* m_Limit_Stat[i][j]/max_stat ;
		}
	}

	CFileDialog dlg2(TRUE,"dat","*.dat");///TRUEΪOPEN�Ի���FALSEΪSAVE AS�Ի��� 
	dlg2.m_ofn.lpstrFilter = ("��ɢ���ļ�2��""*.dat");//
	if(dlg2.DoModal()==IDOK) FilePathName=dlg2.GetPathName(); 
	else return;

	file.Open(FilePathName,CFile::modeRead);
	filelenth = file.GetLength();
	datacounter = filelenth/4;

	m_FileName2 = FilePathName;
	SetDlgItemText(IDC_STATIC_FILE_NAME,FilePathName);

	if(datacounter < 32+ 64*64) {
		AfxMessageBox("���ļ���С���ԣ����ʵ");
		return;
	}

	for(i=0;i<64;i++){			//���64��64 ������	
		for(int j=0;j<64;j++){
			m_sdot[i][j] = 0;	
			m_isdot_datafile[i][j] = 0;
			m_iEth_datafile[i][j] = 0; //������������
		}
	}

	file.Read(byteX,1);
	file.Read(byteY,1);
	file.Read(byteE,1);
	file.Read(byteT,1);
	if((byteX[0]==0xc7)&&(byteY[0]==0xc8)) {  // ���������ɢ������
		unsigned char chartemp[4];
		for(i=0;i<124;i++) {   // 128 �ļ�ͷ
			file.Read(chartemp,1);
		}
		for(i=0;i<64;i++) {
			for(int j=0;j<64;j++) {
				file.Read(chartemp,4);			
				m_sdot[i][j] = chartemp[3];
				m_sdot[i][j] = (m_sdot[i][j] << 8);
				m_sdot[i][j] += chartemp[2];
				m_sdot[i][j] = (m_sdot[i][j] << 8);
				m_sdot[i][j] += chartemp[1];
				m_sdot[i][j] = (m_sdot[i][j] << 8);
				m_sdot[i][j] += chartemp[0];

				m_sdot_file2[i][j] = m_sdot[i][j];
				m_isdot_datafile[i][j] = m_sdot[i][j];
			}
		}
		file.Close();
	}
	else AfxMessageBox("�򿪵��ļ�2 ���Ͳ���");

	for(i=0;i<8;i++) {
		for(int j=0;j<8;j++) {
			m_Limit_Stat_Second[i][j] = 0;
		}
	}	

	for(i=0;i<8;i++) { //������
		for(int j=0;j<8;j++) { //������
			for(int k=X0[i][j];k<=X1[i][j];k++) {
				for(int m=Y0[i][j];m<=Y1[i][j];m++) {
					m_Limit_Stat_Second[i][j] +=  m_sdot[m][k];					
				}
			}
		}
	}
	
	max_stat = m_Limit_Stat_Second[0][0];
	for(i=0;i<8;i++) {
		for(int j=0;j<8;j++){
			if(m_Limit_Stat_Second[i][j] > max_stat) 
				max_stat= m_Limit_Stat_Second[i][j];
		}
	}
	if(max_stat < 1) max_stat=1;
	for(i=0;i<8;i++) {
		for(int j=0;j<8;j++){
			m_igoto_one_second[i][j] = 100* m_Limit_Stat_Second[i][j]/max_stat ;
		}
	}
//--------------------------------------------------------------------------
//	GetDlgItem(IDC_STAT_SHOW_MAP)->EnableWindow(true);

}

void CDynShowDlg::OnCompareValueShow()
{
	int compare_value[8][8];

	for(int i=0;i<8;i++) {
		for(int j=0;j<8;j++) {
			if(m_igoto_one_first[i][j] <= 0.001) m_igoto_one_first[i][j] = 1;
			compare_value[i][j] = 100*(m_igoto_one_second[i][j] - m_igoto_one_first[i][j]) /m_igoto_one_first[i][j];
		}
	}

	int max_stat= compare_value[0][0];
	for(i=0;i<8;i++) {
		for(int j=0;j<8;j++){
			if(compare_value[i][j] > max_stat) 
				max_stat= compare_value[i][j];
		}
	}
	int igoto_one[8][8];
	for(i=0;i<8;i++) {
		for(int j=0;j<8;j++){
			igoto_one[i][j] = compare_value[i][j];
		}
	}

	CDC *pDC=GetDC();
	CString strtext;
	pDC->SetTextColor(RGB(105,15,205));

	if(max_stat <= 1) max_stat = 1;

	   CDC* pDshow=GetDC();	    
	   int y = g_draw_startpoint_y + 27;	 
	   for(int j=0;j<8;j++) {
		    int x = g_draw_startpoint_x+27;
		   	for(int k=0;k<8;k++) {
					CBrush bShow;			
					bShow.CreateSolidBrush(
						RGB(
						GetBValue(ColorSet(m_Limit_Stat[j][k],max_stat)), 
						GetGValue(ColorSet(m_Limit_Stat[j][k],max_stat)),
						GetRValue(ColorSet(m_Limit_Stat[j][k],max_stat))
						)
						);// 
					
					CBrush* pOldBrush=pDshow->SelectObject(&bShow);
					pDshow->Rectangle(x-32,y-32,x+32,y+32);
					
					pDshow->SelectObject(pOldBrush);//
					bShow.DeleteObject();
					strtext.Format("%d",igoto_one[j][k]);				
					pDC->TextOut(x-5,y-5,strtext);			
					
					x += g_draw_size*8;
			}
			y += g_draw_size*8;
	   }
	   ReleaseDC(pDshow);
}


void CDynShowDlg::OnProgControl() 
{
	unsigned char sendaddr[10] = {0,1,2,4,8,16,32,64,128,255};
	unsigned char senddata[3]={0x80,0x00,0x00};

	unsigned char  chrworkmode;

	CProgContrl  progcontrl;
	memcpy(progcontrl.iProgCtrlData,m_iProgCtrlData,10*4);
	progcontrl.m_iWorkMode = m_iWorkMode;

	if(IDOK != progcontrl.DoModal()) {
		return;
	}
	memcpy(m_iProgCtrlData,progcontrl.iProgCtrlData,10*4);
	chrworkmode = (unsigned char)progcontrl.m_iWorkMode;
	m_iWorkMode = (unsigned char)progcontrl.m_iWorkMode;


	if (g_ComPortNo == USB_PORT) {
		hUsb = OpenUsbDevice();
		if (hUsb == INVALID_HANDLE_VALUE) {
			MessageBox("USB Open Error!");
			return;
		}	
	}
	else {
		if(!OpenShmMaint232(g_ComPortNo)) {
			MessageBox("Communication Port Open Error!");
			return;
		}
	}
	
	Sleep(100);
	
	if (g_ComPortNo == USB_PORT) {
		int nusbwrite = WriteUsbData(hUsb, &chrworkmode, 1);
		if(!nusbwrite) AfxMessageBox(" Write usb error");
		
		Sleep(50);
		if ((chrworkmode == 0xdd)||(chrworkmode == 0xEE) ){ // AB · ʱ�����
			if (g_ComPortNo == USB_PORT) {
				CloseUsbDevice(hUsb);
			}
			else {
				CloseShmMaint232();		
			}
			return;
		}	
	}

	for(int i=0;i<9;i++) {
		senddata[1] = sendaddr[i];
		senddata[2] = m_iProgCtrlData[i];

		if (g_ComPortNo == USB_PORT) {
			int nusbwrite = WriteUsbData(hUsb, senddata, 3);
			if(!nusbwrite) AfxMessageBox(" Write usb error");
		}
		else {
			int nWR = WriteCommandMaint232(3,senddata);
			if(nWR == false) {
				AfxMessageBox("Communication Write Error.\n\n Please check Communication Setting.");
			}			
		}
		Sleep(10);   
	}

	if (g_ComPortNo == USB_PORT) {
		CloseUsbDevice(hUsb);
	}
	else {
		CloseShmMaint232();		
	}
	Beep(2000,50);

}

void CDynShowDlg::OnRButtonDown(UINT nFlags, CPoint point) 
{
//	CDialog::OnRButtonDown(nFlags, point);

	CPoint ScreenPoint;   //������������Ļ�ϵ�λ��,������λ����ָ�û������λ��
    ScreenPoint = point;
    ClientToScreen(&ScreenPoint);

	UINT CommandId = 0;
    CMenu MyContextMenu;
    MyContextMenu.CreatePopupMenu();
    MyContextMenu.AppendMenu(MF_STRING, ++CommandId, "ˢ��λ�ñ߽�");
    MyContextMenu.AppendMenu(MF_STRING, ++CommandId, "����λ�ñ߽��ļ�");
    MyContextMenu.AppendMenu(MF_STRING, ++CommandId, "����λ�ñ߽��ļ�");
    MyContextMenu.AppendMenu(MF_STRING, ID_SEPARATOR); //to get separator line
    MyContextMenu.AppendMenu(MF_STRING, ++CommandId, "...");
	MyContextMenu.AppendMenu(MF_STRING, ++CommandId, "�������ױ߽��ļ�");
	MyContextMenu.AppendMenu(MF_STRING, ++CommandId, "��ʾɢ�� ��ͼ");
	MyContextMenu.AppendMenu(MF_STRING, ++CommandId, "��ʾ���� ��ͼ");
	MyContextMenu.AppendMenu(MF_STRING, ID_SEPARATOR); //to get separator line
	MyContextMenu.AppendMenu(MF_STRING, ++CommandId, "�Ŵ�������");



	//... (Append as many commands as you like)
    //ָ���˵�������.
    UINT Command = (UINT)MyContextMenu.TrackPopupMenu(TPM_LEFTALIGN|TPM_NONOTIFY | TPM_RETURNCMD,
		ScreenPoint.x, ScreenPoint.y, this);
    //ʹ��TPM_NONOTIFY | TPM_RETURNCMD���ر�ѡ��Ĳ˵�����,0����û��(����
    //����˵�����)
    //����㲻��������������,��ĸ����ڽ������ѡ���������յ�WM_COMMAND��Ϣ

	MyContextMenu.DestroyMenu();

    //switch is only necessary if you use TPM_NONOTIFY | TPM_RETURNCMD !
    //switchֻ����ʹ����������ʱ���б�Ҫ.

    switch(Command)
    {
		case 0:		
			//û������,���Բ������κ�����.			
			break;
		case 1:			
			RedrawLimitFile();
			break;
		case 2:	
			OnLoadLimitFile();
			break;
		case 3:
			OnSaveLimitFile();
			break;
		case 4:
			//do whatever	AfxMessageBox("You Select No 4.");	
			break;
		case 5:
			OnLoadEnergeSpectrumLimitFile();
			break;
		case 6:	
			m_bSelShowEnergeSpectrum = false;
			SetDlgItemText(IDC_STATIC_3DSHOW,"ɢ�� ֱ��ͼ");
			break;
		case 7:
			m_bSelShowEnergeSpectrum = true;
			SetDlgItemText(IDC_STATIC_3DSHOW,"���� ֱ��ͼ");
			break;
		case 8:	
			OnAmplifierCorrect();	
			break;

		default:		
			//do default	AfxMessageBox("Invalide Selected");			
				break;
	}

}



void CDynShowDlg::RedrawLimitFile()
{
	//�Ȼ���ɢ��ͼ���ٻ���λ�ñ߽�

	m_bEnShowPeakValeRatio = false;	
	long temp;
	CEdit* pThreshvalue=(CEdit*)GetDlgItem(IDC_3D_SHOW_THREVALUE);	
	if(m_bSelShowEnergeSpectrum) { memcpy(m_sdot,m_iEth_datafile,64*64*4); }
	else memcpy(m_sdot,m_isdot_datafile,64*64*4);

	CDC* pDC=GetDC();			
	CRect clientrect;
	GetClientRect(clientrect);			
	CDC MemDC; //���ȶ���һ����ʾ�豸����
	CBitmap MemBitmap;//����һ��λͼ����	
	MemDC.CreateCompatibleDC(NULL);
	MemBitmap.CreateCompatibleBitmap(pDC,clientrect.Width(),clientrect.Height()); //�Զ���һ��ͼ��Ĵ�С
	CBitmap *pOldBit=MemDC.SelectObject(&MemBitmap);	
	
	CPen pShow5(PS_SOLID,2,RGB(219,206,198));//
	CPen* pOldPen=NULL;
	pOldPen=MemDC.SelectObject(&pShow5);
	
	CBrush bShow;			
	bShow.CreateSolidBrush(RGB(219,206,198));// 			 					  
	CBrush* pOldBrush=MemDC.SelectObject(&bShow);// 
	MemDC.Rectangle(45,45,615,615);// ˢ��С���ڵĴ�С45,45,557,557
	MemDC.SelectObject(pOldBrush);//++++
	bShow.DeleteObject();

	MemDC.SetBkColor(RGB(219,206,198));	

    MemDC.SetTextColor(RGB(0,128,0));
	MemDC.TextOut(130,56,"[ D ]");
	MemDC.TextOut(480,56,"[ B ]");
	MemDC.TextOut(130,587,"[ C ]");
	MemDC.TextOut(480,587,"[ A ]");

	MemDC.SetTextColor(RGB(0,0,0));
	CPen pShow6(PS_SOLID,1,RGB(0,0,0));//���ó� ��ɫ������
	pOldPen=MemDC.SelectObject(&pShow6);
	
	MemDC.SetBkColor(RGB(219,206,198));
	
	
	MemDC.TextOut(170,48," --------------------   ˮƽ (X) ����   -------------------> ");
	MemDC.MoveTo(55,105);
	MemDC.LineTo(55,190);
	MemDC.TextOut(50,200,"��");
	MemDC.TextOut(50,220,"ֱ");
	MemDC.TextOut(50,240,"��");
	MemDC.TextOut(50,260,"��");
	MemDC.TextOut(52,360,"V");
	MemDC.MoveTo(55,280);
	MemDC.LineTo(55,365);
	
	
	CString m_strEditThresh;
	pThreshvalue->GetWindowText(m_strEditThresh);
	if(!m_strEditThresh.IsEmpty()) {		
		   temp=atol(m_strEditThresh); 	        
	}
	else {	//�����ֵ
	   temp = m_sdot[0][0];  //�ҵ� 0 ������
	   for(int j=0;j<64;j++) {
		   for(int k=0;k<64;k++) {
			   if(m_sdot[j][k] > temp)  temp = m_sdot[j][k];
		   }
	   }
	}
	if(temp <= 63) temp =63;	    
    
	int y = g_draw_startpoint_x;
	int x = g_draw_startpoint_y;
	CBrush bShowkkkk;	 
	CBrush* pOldBrushkkk;
	for(int j=0;j<64;j++) {
	   x = g_draw_startpoint_y;
	   for(int k=0;k<64;k++) {		  			
			bShowkkkk.CreateSolidBrush(RGB(GetBValue(ColorSet(m_sdot[j][k],temp)), 
				   GetGValue(ColorSet(m_sdot[j][k],temp)),
				   GetRValue(ColorSet(m_sdot[j][k],temp))));// 			 					  
			pOldBrushkkk=MemDC.SelectObject(&bShowkkkk);// 
			MemDC.Rectangle(x-g_draw_frame,y-g_draw_frame,x+g_draw_frame,y+g_draw_frame);// 
 			MemDC.SelectObject(pOldBrushkkk);//++++	
			bShowkkkk.DeleteObject();					   
			x += g_draw_size;
		}
	    y += g_draw_size;
	 }	 

 
	// ���»���λ�ñ߽�
    CPen pShow(PS_SOLID,2,RGB(15, 105,185));
    pOldPen=MemDC.SelectObject(&pShow); 
	MemDC.SetROP2(R2_NOT);
	
	for(int i=0;i<8;i++) {
		for(int j=0;j<8;j++) {
			MemDC.MoveTo(g_draw_startpoint_y  + m_Position_X0[i][j]*8,g_draw_startpoint_x  + (m_Position_Y0[i][j])*8); 
			if(j<7) MemDC.LineTo(g_draw_startpoint_y  + (m_Position_X1[i][j]+1)*8,g_draw_startpoint_x  + m_Position_Y0[i][j]*8);
			else MemDC.LineTo(g_draw_startpoint_y  + m_Position_X1[i][j]*8,g_draw_startpoint_x  + m_Position_Y0[i][j]*8);
			
			MemDC.MoveTo(g_draw_startpoint_y  + m_Position_X0[i][j]*8,g_draw_startpoint_x  + (m_Position_Y0[i][j])*8);
			if(i<7) MemDC.LineTo(g_draw_startpoint_y  + m_Position_X0[i][j]*8,g_draw_startpoint_x  + (m_Position_Y1[i][j]+1)*8);
			else  MemDC.LineTo(g_draw_startpoint_y  + m_Position_X0[i][j]*8,g_draw_startpoint_x  + m_Position_Y1[i][j]*8); 
		}
	}
	
	MemDC.MoveTo(g_draw_startpoint_y  + m_Position_X1[0][7]*8,g_draw_startpoint_x  + (m_Position_Y0[0][7])*8); 
	MemDC.LineTo(g_draw_startpoint_y  + m_Position_X1[0][7]*8,g_draw_startpoint_x  + (m_Position_Y1[7][7])*8);
	
	MemDC.MoveTo(g_draw_startpoint_y  + m_Position_X0[7][0]*8,g_draw_startpoint_x  + (m_Position_Y1[7][0])*8); 
	MemDC.LineTo(g_draw_startpoint_y  + m_Position_X1[7][7]*8,g_draw_startpoint_x  + (m_Position_Y1[7][0])*8);
	

	pDC->BitBlt(45,45,560,560,&MemDC,45,45,SRCCOPY);	
	MemBitmap.DeleteObject();
	MemDC.DeleteDC();
	   
	ReleaseDC(pDC);

}

void CDynShowDlg::OnLoadLimitFile()
{
	CFileSave filesave;
	filesave.bNewfilepackage = false;
	filesave.iBlockSel = fileoper_iblocksel;
	filesave.bEnReadOperate = true;
	if( IDOK != filesave.DoModal()) {
		return;
	}
	filesave.bEnReadOperate = false;
	fileoper_iblocksel = filesave.iBlockSel;

	CString FilePathName; 
	CFileDialog dlg(TRUE,"dat","*.dat");///TRUEΪOPEN�Ի���FALSEΪSAVE AS�Ի��� 

	dlg.m_ofn.lpstrFilter = ("λ�ñ߽絼��.dat");//
	if(dlg.DoModal()==IDOK) FilePathName=dlg.GetPathName(); 
	else return;
	CFile file;
	if(!file.Open(FilePathName,CFile::modeRead)) {
		AfxMessageBox("�ļ�������");
		return;
	}
	int filelenth = file.GetLength();
//	if(filelenth != (128*4 +4)*1024 ) {
//		AfxMessageBox("�ļ����Ȳ��ԣ�");
//		return;
//	}
	unsigned char chartemp[128];
//	file.Seek(FILEHEAD_PLIMIT_ATTR_ADDR,CFile::begin );	
//	file.Read(chartemp,1);
//	if(chartemp[0] != 0x01) {
//		AfxMessageBox("�ļ���λ�ñ߽�������Ч");
//		return;
//	}

	file.Seek(PARA_PLIMIT_DATA_ADDR + fileoper_iblocksel * BLOCK_PLIMIT_DATA_SIZE,CFile::begin );	

	for(int i=0;i<8;i++) {
			for(int j=0;j<8;j++) {
				file.Read(chartemp,4);			
				m_Position_X0[i][j] = (int)chartemp[0];
				m_Position_Y0[i][j] = (int)chartemp[1];
				m_Position_X1[i][j] = (int)chartemp[2];
				m_Position_Y1[i][j] = (int)chartemp[3];
			}
	}

	file.Close();

	m_bSameDriction_X = true;
	for(i=1;i<7;i++) {
		for(int j=1;j<6;j++) {
			if(m_Position_Y0[i][j] != m_Position_Y0[i][j+1]) {
				m_bSameDriction_X = false; 
			}
		}
	}

	CString strtemp;
	strtemp = "λ�ñ߽絼��ɹ�!\n" + FilePathName;
	SetDlgItemText(IDC_STATIC_FILE_NAME,strtemp);

	RedrawLimitFile();	
	
}

void CDynShowDlg::OnSaveLimitFile()
{
	filesave.bNewfilepackage = fileoper_Newfile;
	filesave.iBlockSel = fileoper_iblocksel;
	if( IDOK != filesave.DoModal()) {
		return;
	}
	fileoper_Newfile = filesave.bNewfilepackage;
	fileoper_iblocksel = filesave.iBlockSel;

	CString FilePathName; 
	CFile savefile;
	if(fileoper_Newfile) {
		CFileDialog dlg(FALSE,"dat","*.dat");///TRUEΪOPEN�Ի���FALSEΪSAVE AS�Ի��� 
		dlg.m_ofn.lpstrFilter = ("����λ�ñ߽��ļ�LimitFile ""*.dat");
		if(dlg.DoModal()==IDOK) FilePathName=dlg.GetPathName(); 
		else return;
		if( !savefile.Open( FilePathName, CFile::modeCreate | CFile::modeWrite) ) {			
		}
		savefile.SetLength( (128*4 + 4)*1024 );		
	} 
	else {
		CFileDialog dlg(TRUE,"dat","*.dat");///TRUEΪOPEN�Ի���FALSEΪSAVE AS�Ի���
		dlg.m_ofn.lpstrFilter = ("��λ�ñ߽��ļ�LimitFile ""*.dat");
		if(dlg.DoModal()==IDOK) FilePathName=dlg.GetPathName(); 
		else return;
		CFile tempfile;
		if(!tempfile.Open(FilePathName,CFile::modeRead)) {
			AfxMessageBox("���ܽ��б༭���� (û���ҵ����ļ�) \n������ѡ�񱣴��ļ��Ĳ�������!");
			CString strtemp;
			strtemp = "�ļ�����ʧ��!(û���ҵ�������ļ�)\n" + FilePathName;
			SetDlgItemText(IDC_STATIC_FILE_NAME,strtemp);
			return;
		}
		tempfile.Close();
		if( !savefile.Open( FilePathName, CFile::modeReadWrite) ) {			
		}
	}

	unsigned char filehead[128];
	savefile.SeekToBegin();

//	int filelenth = savefile.GetLength();
//	if(filelenth != (128*4 +4)*1024 ) {
//		AfxMessageBox("�ļ����Ȳ��ԣ����治�ɹ���");
//		return;
//	}

	filehead[0] = 0x01;
	savefile.Seek(FILEHEAD_PLIMIT_ATTR_ADDR,CFile::begin );	
	savefile.Write(filehead,1);

	savefile.Seek(PARA_PLIMIT_DATA_ADDR+ BLOCK_PLIMIT_DATA_SIZE*fileoper_iblocksel,CFile::begin);
	unsigned char temp[4]={0,0,0,0};
	
	for(int i=0;i<8;i++) {
		for(int j=0;j<8;j++) {	
			temp[0] = m_Position_X0[i][j];
			temp[1] = m_Position_Y0[i][j];
			temp[2] = m_Position_X1[i][j];
			temp[3] = m_Position_Y1[i][j];
			savefile.Write(temp,4);
		}
	}	
	savefile.Close();
	
	CString strtemp;
	strtemp = "λ�ñ߽��ļ��ɹ�����!\n" + FilePathName;
	SetDlgItemText(IDC_STATIC_FILE_NAME,strtemp);


}

void CDynShowDlg::OnSampleModeSel() 
{
	m_bSampleTimeMode = (!m_bSampleTimeMode);
	if(m_bSampleTimeMode) {
		m_strSampleShowMode = "ʱ��(s):";
	}
	else {
		m_strSampleShowMode = "����(b):";
	}
	
	UpdateData(false);
}

void CDynShowDlg::OnTimeSpect_redraw_2D()
{
		int tplace;
		int x=650;
		long longtdata[128];
		long temp;
		CEdit* pLeftLimit=(CEdit*)GetDlgItem(IDC_ENERGE_LEFT_LIMIT);
		CEdit* pRightLimit=(CEdit*)GetDlgItem(IDC_ENERGE_RIGHT_LIMIT);

		memcpy(longtdata,m_Time_Spectrum,128*4);
		char chtext[30];
		int  position=0;
		temp = longtdata[0];		
		for(int n=0;n<128;n++) {  //�����ֵ
			if(longtdata[n] > temp) {
				temp = longtdata[n];
				position = n;
			}
		} 
		sprintf(chtext,"val:%d  pos:%d",temp,position);
		temp = (long)(1.5*temp);


		CDC* pDC=GetDC();			
		CRect clientrect;
		GetClientRect(clientrect);			
		CDC MemDC; //���ȶ���һ����ʾ�豸����
		CBitmap MemBitmap;//����һ��λͼ����	
		MemDC.CreateCompatibleDC(NULL);
		MemBitmap.CreateCompatibleBitmap(pDC,clientrect.Width(),clientrect.Height()); //�Զ���һ��ͼ��Ĵ�С
		CBitmap *pOldBit=MemDC.SelectObject(&MemBitmap);			
		
		CPen pShow5(PS_SOLID,1,RGB(212,208,200));//RGB(0,210,0)
	    CPen* pOldPen=NULL;
	    pOldPen=MemDC.SelectObject(&pShow5);

		CBrush bShow;			
		bShow.CreateSolidBrush(RGB(219,206,198));// 30003000				 					  
		CBrush* pOldBrush=MemDC.SelectObject(&bShow);// 3000 
	    MemDC.Rectangle(635,50,920,285);// ˢ��С���ڵĴ�С
					
		MemDC.SelectObject(pOldBrush);//++++
		bShow.DeleteObject();	


		MemDC.SetBkColor(RGB(219,206,198));
        CPen pShow(PS_SOLID,1,RGB(15, 105,185));//RGB(0,210,0)
	    pOldPen=NULL;
	    pOldPen=MemDC.SelectObject(&pShow);

		MemDC.MoveTo(570+g_draw_startpoint_x,65); 
		MemDC.LineTo(570+g_draw_startpoint_x,265);
		MemDC.MoveTo(570+g_draw_startpoint_x,65);
		MemDC.LineTo(570+g_draw_startpoint_x-3,65+5);

	    MemDC.MoveTo(570+g_draw_startpoint_x,265);
		MemDC.LineTo(570+g_draw_startpoint_x +4*64 ,265);
	    MemDC.MoveTo(570+g_draw_startpoint_x +4*64,265);
        MemDC.LineTo(570+g_draw_startpoint_x +4*64 - 5,265+3);

		MemDC.TextOut(638,47,chtext);
	    MemDC.TextOut(635,268,"(0,0)");
		MemDC.TextOut(898,268,"127");  
		// ԭ������Ϊ ��650��265��  ÿ��ļ��Ϊ 4 mil    650 + 64*4 = 906; 
	
	//	CDC* pDshow=GetDC();
		if(temp <= 1) temp = 1;
		for(int i=0;i<128;i++) {			
			tplace = 265 - (int)(2*128*longtdata[i]/temp);
			if(tplace < 60) tplace = 60;
			if(tplace > 265) tplace = 265;
			CBrush bShow;				
			bShow.CreateSolidBrush(RGB(
					   GetBValue(ColorSet(longtdata[i],temp)), 
					   GetGValue(ColorSet(longtdata[i],temp)),
					   GetRValue(ColorSet(longtdata[i],temp))));			 					  
			CBrush* pOldBrush=MemDC.SelectObject(&bShow);// 
			MemDC.Rectangle(x-1,tplace-1,x+1,tplace+1);// 
						
			MemDC.SelectObject(pOldBrush);//++++
			bShow.DeleteObject();
			x += 2;//x += 4;
		}

		pOldPen=MemDC.SelectObject(&pShow);
		if(temp < 1) temp = 1;
		x = 650 ; 
		tplace = 265- (int)(2*128*longtdata[0]/temp);
		for(int kk=1;kk<128;kk++) {																				
			MemDC.MoveTo(x,tplace);
			x += 2;//x += 4;
			tplace = 265 - (int)(2*128*longtdata[kk]/temp);			
			if(tplace < 60) tplace = 60;
			if(tplace > 265) tplace = 265;
			MemDC.LineTo(x,tplace);
		}

		//����8nsΪ���ڵ�16��ƽ��ˮƽ��
		CPen pShowGreen(PS_SOLID,1,RGB(159,64,146)); //��ɫ
		MemDC.SelectObject(&pShowGreen);
		int arrage = 0;
		for(i=0;i<8;i++) {
			arrage += longtdata[i];
		}
		arrage = (float)arrage/8;
		x = 650 ;
		for(i=0;i<16;i++) { //
			arrage = 0;	
			for(int kkk=i*8;kkk<(i+1)*8;kkk++) {
				arrage += longtdata[kkk];
			}
			arrage = (float)arrage/8; 
			tplace = 265 - (int)(2*128*arrage/temp);
			if(tplace < 60) tplace = 60;
			if(tplace > 265) tplace = 265;
			MemDC.MoveTo(x,tplace);
			x += 16;					
			MemDC.LineTo(x,tplace);			
		}
		

		CString strLeftlimit;
		CString strRightlimit;
		pLeftLimit->GetWindowText(strLeftlimit);
		pRightLimit->GetWindowText(strRightlimit);

		if(!strLeftlimit.IsEmpty()) {		
				ileft_limit =atol(strLeftlimit);        
		}
		else{  ileft_limit = 0; }

		if(!strRightlimit.IsEmpty()) {		
				iright_limit =atol(strRightlimit);        
		}
		else{ 	iright_limit = 63;}
		// ���߽�
		CPen pShow2(PS_SOLID,1,RGB(255,10,,0));//RGB(0,210,0)
	    pOldPen=MemDC.SelectObject(&pShow2);
		MemDC.MoveTo(650 + iright_limit*4 , 265);
		MemDC.LineTo(650 + iright_limit*4 , 265 - 190);
		MemDC.MoveTo(650 + ileft_limit*4 , 265);
		MemDC.LineTo(650 + ileft_limit*4 , 265 - 190);




		
		pDC->BitBlt(635,50,285,235,&MemDC,635,50,SRCCOPY);	
		MemBitmap.DeleteObject();
		MemDC.DeleteDC();
		
		ReleaseDC(pDC);

}



//void CDynShowDlg::OnSelchangeComboProgContrlSel() 
//{
//	// TODO: Add your control notification handler code here
//	CEdit* pProgContrlData=(CEdit*)GetDlgItem(IDC_PROG_CTRL_DATA);
//	CString strProgCtrlDataValue;
//	pProgContrlData->GetWindowText(strProgCtrlDataValue);
//
//	int nPrctlindex=m_ProgCtrl_Sel.GetCurSel(); 
//	CString strprog;
//	strprog.Format("%d",m_iProgCtrlData[nPrctlindex]);
//	SetDlgItemText(IDC_PROG_CTRL_DATA,strprog);
//
/////	if(!strProgCtrlDataValue.IsEmpty())  {		
////            m_iProgCtrlData[nPrctlindex]=(int)atol(strProgCtrlDataValue);        
////	}
//	


//void CDynShowDlg::OnDropdownComboProgContrlSel() 
//{
//	// TODO: Add your control notification handler code here
//
//	CEdit* pProgContrlData=(CEdit*)GetDlgItem(IDC_PROG_CTRL_DATA);
//	CString strProgCtrlDataValue;
//	pProgContrlData->GetWindowText(strProgCtrlDataValue);
//
//	int nPrctlindex=m_ProgCtrl_Sel.GetCurSel(); 
//
//	if(!strProgCtrlDataValue.IsEmpty())  {	        
//		m_iProgCtrlData[nPrctlindex] = (int)atol(strProgCtrlDataValue);  
//	}
//	else {
//		CString strprog;
//		strprog.Format("%d",m_iProgCtrlData[nPrctlindex]);
//		SetDlgItemText(IDC_PROG_CTRL_DATA,strprog);
//	}



void CDynShowDlg::OnLoadEnergeSpectrumLimitFile()
{

	CFileSave filesave;
	filesave.bNewfilepackage = false;
	filesave.iBlockSel = fileoper_iblocksel;
	filesave.bEnReadOperate = true;
	if( IDOK != filesave.DoModal()) {
		return;
	}
	filesave.bEnReadOperate = false;
	fileoper_iblocksel = filesave.iBlockSel;

	CString FilePathName; 
	CFileDialog dlg(TRUE,"dat","*.dat");///TRUEΪOPEN�Ի���FALSEΪSAVE AS�Ի��� 

	dlg.m_ofn.lpstrFilter = ("���ױ߽絼��.dat");//
	if(dlg.DoModal()==IDOK) FilePathName=dlg.GetPathName(); 
	else return;
	CFile file;
	if(!file.Open(FilePathName,CFile::modeRead)) {
		AfxMessageBox("�ļ�������");
		return;
	}
	int filelenth = file.GetLength();
	if(filelenth != (128*4 +4)*1024 ) {
		AfxMessageBox("�ļ����Ȳ��ԣ�");
		return;
	}
	unsigned char chartemp[128];
	file.Seek(FILEHEAD_ELIMIT_ATTR_ADDR,CFile::begin );	
	file.Read(chartemp,1);
	if(chartemp[0] != 0x01) {
		AfxMessageBox("�ļ������ױ߽�������Ч");
		return;
	}

	file.Seek(PARA_ETHLIMIT_DATA_ADDR + fileoper_iblocksel * BLOCK_ETHLIMIT_DATA_SIZE,CFile::begin );	

	for(int i=0;i<64;i++) {			
				file.Read(chartemp,2);			
				m_iEnergeSpectEth_Low[i] = (int)chartemp[0];
				m_iEnergeSpectEth_Hight[i] = (int)chartemp[1];
	}

	file.Close();

	m_bSelEthLimit_HandleInput = false;
	GetDlgItem(IDC_ENERGE_LEFT_LIMIT)->EnableWindow(false);
	GetDlgItem(IDC_ENERGE_RIGHT_LIMIT)->EnableWindow(false);

	AfxMessageBox("���ױ߽絼��ɹ�");	
	CString strtemp;
	strtemp = "���ױ߽絼��ɹ�!\n" + FilePathName;
	SetDlgItemText(IDC_STATIC_FILE_NAME,strtemp);	

}

void CDynShowDlg::OnAddSynEnergSpectrumNo() 
{
	// TODO: Add your control notification handler code here
	CEdit* pSynEnerSpectValue=(CEdit*)GetDlgItem(IDC_SYN_ENERSPECT_VALUE);

	int temp;
	CString strSynEnerSpectValue;
	pSynEnerSpectValue->GetWindowText(strSynEnerSpectValue);
	if(!strSynEnerSpectValue.IsEmpty())	{		
			temp=atol(strSynEnerSpectValue); 
			temp ++;			
			if((temp>63)||(temp<0)) {
				temp = 0;
			}
			strSynEnerSpectValue.Format(" %d",temp);
			SetDlgItemText(IDC_SYN_ENERSPECT_VALUE,strSynEnerSpectValue);
	}
	else {  
		temp = 0; 
		SetDlgItemText(IDC_SYN_ENERSPECT_VALUE," 0");
	}

	OnRedraw_2d();

}

void CDynShowDlg::OnEthReset() 
{
	// TODO: Add your control notification handler code here

	for(int i=0;i<64;i++){			//���64��64 ������	
		for(int j=0;j<64;j++){
			m_sdot[i][j] = 0;	//ɢ������
			m_isdot_datafile[i][j] = 0;
			m_iEth_datafile[i][j] = 0; //������������
		}
		m_Energer_Spectrum[i] = 0;
	}
	for(i=0;i<128;i++) {    //ʱ����
		m_Time_Spectrum[i] = 0;
	}

	for(i=0;i<64;i++) {
		m_iEnergeSpectEth_Low[i] = 0;
		m_iEnergeSpectEth_Hight[i] = 63;
	}

	//-------��ɢ��߽紦��----------//
	m_bButtonDown = false;
	m_bMoveLongLine = false;
	m_bSameDriction_X = true;
	m_bShort_Line_Valid = false;
	m_bthisMoveInValid = false;

	for(i=0;i<8;i++) {
		for(int j=0;j<8;j++) {
			m_Position_X0[i][j] = j*8; 
			m_Position_Y0[i][j] = i*8;
			m_Position_X1[i][j] = (j+1)*8 -1;
			m_Position_Y1[i][j] = (i+1)*8 -1;
		}
	}

	SetDlgItemText(IDC_ENERGE_LEFT_LIMIT,"00");
	SetDlgItemText(IDC_ENERGE_RIGHT_LIMIT,"63");


	m_bSelEthLimit_HandleInput = true;
	GetDlgItem(IDC_ENERGE_LEFT_LIMIT)->EnableWindow(true);
	GetDlgItem(IDC_ENERGE_RIGHT_LIMIT)->EnableWindow(true);
	
}

void CDynShowDlg::OnAmplifierCorrect()
{
	int A_Counter = 0;
	int B_Counter = 0;
	int C_Counter = 0;
	int D_Counter = 0;
	int Avr_Counter;

	int x,y;
	for(y=0;y<32;y++){
		for(x=0;x<32;x++){
			D_Counter += m_isdot_datafile[y][x];
		}
	}

	for(y=0;y<32;y++){
		for(x=32;x<64;x++){
			B_Counter += m_isdot_datafile[y][x];
		}
	}

	for(y=32;y<64;y++){
		for(x=0;x<32;x++){
			C_Counter += m_isdot_datafile[y][x];
		}
	}

	for(y=32;y<64;y++){
		for(x=32;x<64;x++){
			A_Counter += m_isdot_datafile[y][x];
		}
	}

	Avr_Counter = (A_Counter + B_Counter + C_Counter + D_Counter)/4;
	if(Avr_Counter == 0) Avr_Counter = 1; 

	if(A_Counter > Avr_Counter) { // A · �Ŵ�ϵ��
		m_iProgCtrlData[1] = m_iProgCtrlData[1] - m_iProgCtrlData[1]*(A_Counter - Avr_Counter)/Avr_Counter;
	}
	else {
		m_iProgCtrlData[1] = m_iProgCtrlData[1] + m_iProgCtrlData[1]*(Avr_Counter - A_Counter)/Avr_Counter;
	}

	if(B_Counter > Avr_Counter) { // B · �Ŵ�ϵ��
		m_iProgCtrlData[2] = m_iProgCtrlData[2] - m_iProgCtrlData[2]*(B_Counter - Avr_Counter)/Avr_Counter;
	}
	else {
		m_iProgCtrlData[2] = m_iProgCtrlData[2] + m_iProgCtrlData[2]*(Avr_Counter - B_Counter)/Avr_Counter;
	}

	if(C_Counter > Avr_Counter) { // C · �Ŵ�ϵ��
		m_iProgCtrlData[3] = m_iProgCtrlData[3] - m_iProgCtrlData[3]*(C_Counter - Avr_Counter)/Avr_Counter;
	}
	else {
		m_iProgCtrlData[3] = m_iProgCtrlData[3] + m_iProgCtrlData[3]*(Avr_Counter - C_Counter)/Avr_Counter;
	}

	if(D_Counter > Avr_Counter) { // B · �Ŵ�ϵ��
		m_iProgCtrlData[4] = m_iProgCtrlData[4] - m_iProgCtrlData[4]*(D_Counter - Avr_Counter)/Avr_Counter;
	}
	else {
		m_iProgCtrlData[4] = m_iProgCtrlData[4] + m_iProgCtrlData[4]*(Avr_Counter - D_Counter)/Avr_Counter;
	}


//	m_ProgCtrl_Sel.SetCurSel(1); // ; 
//	CString strprog;
//	strprog.Format("%d",m_iProgCtrlData[1]);
//	SetDlgItemText(IDC_PROG_CTRL_DATA,strprog);


}

void CDynShowDlg::OnEnLimitPositionWholeMove() 
{
	m_bMoveWholeLine = (!m_bMoveWholeLine);

}

void CDynShowDlg::OnPeakValeStat() 
{
	CEdit* pThPeakVale=(CEdit*)GetDlgItem(IDC_PEAKVALE_ETH);

	float iPeakValue;  //akang
	CString strPeakvalue;
	pThPeakVale->GetWindowText(strPeakvalue);
	if(!strPeakvalue.IsEmpty()) {		
            iPeakValue=atof(strPeakvalue);        
	        pThPeakVale->Invalidate();
	}
	else iPeakValue = 0;

	//���㣻

	memcpy(peakvaleratio.iSdot,m_sdot,64*64*4);

	memcpy(peakvaleratio.Position_X0,m_Position_X0,8*8*4);
	memcpy(peakvaleratio.Position_Y0,m_Position_Y0,8*8*4);
	memcpy(peakvaleratio.Position_X1,m_Position_X1,8*8*4);
	memcpy(peakvaleratio.Position_Y1,m_Position_Y1,8*8*4);

	peakvaleratio.CalPeakValeRatio();

	//-----------------����ֵͳ��
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
						RGB(
						GetBValue(ColorSet(showstatnum[j][k]*10,100)), //��ɫ��ʾ
						GetGValue(ColorSet(showstatnum[j][k]*10,100)),
						GetRValue(ColorSet(showstatnum[j][k]*10,100))
						)
						);					
					CBrush* pOldBrush=pDshow->SelectObject(&bShow);
					pDshow->Rectangle(x-32,y-32,x+32,y+32);
					
					pDshow->SelectObject(pOldBrush);
					bShow.DeleteObject();

					strtext.Format("%d",showstatnum[j][k]);	//��ֵ��ʾ		
					pDC->TextOut(x-5,y-5,strtext);			
					
					x += g_draw_size*8;
			}
			y += g_draw_size*8;
	   }


	CPen pShow5(PS_SOLID,1,RGB(219,206,198));//  ��ߵ���ɫ
	CPen* pOldPen=NULL;
	pOldPen=pDC->SelectObject(&pShow5);

	CBrush bShow;			
	bShow.CreateSolidBrush(RGB(219,206,198));//  �����				 					  
	CBrush* pOldBrush=pDC->SelectObject(&bShow);
	pDC->Rectangle(635,42,918,287);// 
	pDC->SelectObject(pOldBrush);//++++
	bShow.DeleteObject();

	ReleaseDC(pDC);

	OnShowPeakValeRatio(0);

	m_bEnShowPeakValeRatio = true;
	
}

void CDynShowDlg::OnShowPeakValeRatio(int no)
{
	CDC* pDC=GetDC();
	int iPeakValue = no;
	CString strtextout;

	CPen pShow5(PS_SOLID,1,RGB(219,206,198));//  ��ߵ���ɫ
	CPen* pOldPen=NULL;
	pOldPen=pDC->SelectObject(&pShow5);

	CBrush bShow;			
	bShow.CreateSolidBrush(RGB(219,206,198));//  �����				 					  
	CBrush* pOldBrush=pDC->SelectObject(&bShow);
	pDC->Rectangle(635,42,918,287);// 
	pDC->SelectObject(pOldBrush);//++++
	bShow.DeleteObject();

	pDC->SetBkColor(RGB(72,183,152));
	pDC->SetTextColor(RGB(30,43,31));  //RGB(236,28,33)

	if(peakvaleratio.fPeakVale_Ratio[iPeakValue/8][iPeakValue%8][0] != -1) {
		strtextout.Format("X=%d��Y=%d�� ���Ͻǣ�%5.3f  ",iPeakValue%8, iPeakValue/8,peakvaleratio.fPeakVale_Ratio[iPeakValue/8][iPeakValue%8][0]);
	} else strtextout.Format("X=%d��Y=%d�� ���Ͻǣ�**** ",iPeakValue%8, iPeakValue/8);

	pDC->TextOut(650,50,strtextout);
	if(peakvaleratio.fPeakVale_Ratio[iPeakValue/8][iPeakValue%8][1] != -1) {
		strtextout.Format("X=%d��Y=%d�� �Ϸ���%5.3f ",iPeakValue%8, iPeakValue/8,peakvaleratio.fPeakVale_Ratio[iPeakValue/8][iPeakValue%8][1]);
	} else strtextout.Format("X=%d��Y=%d�� �Ϸ���**** ",iPeakValue%8, iPeakValue/8); 
	pDC->TextOut(650,65,strtextout);
	if(peakvaleratio.fPeakVale_Ratio[iPeakValue/8][iPeakValue%8][2] != -1) {
		strtextout.Format("X=%d��Y=%d�� ���Ͻǣ�%.3f ",iPeakValue%8, iPeakValue/8,peakvaleratio.fPeakVale_Ratio[iPeakValue/8][iPeakValue%8][2]);
	} else strtextout.Format("X=%d��Y=%d�� ���Ͻǣ�**** ",iPeakValue%8, iPeakValue/8); 
	pDC->TextOut(650,80,strtextout);
	if(peakvaleratio.fPeakVale_Ratio[iPeakValue/8][iPeakValue%8][3] != -1) {
		strtextout.Format("X=%d��Y=%d�� �ҷ���%5.3f ",iPeakValue%8, iPeakValue/8,peakvaleratio.fPeakVale_Ratio[iPeakValue/8][iPeakValue%8][3]);
	} else strtextout.Format("X=%d��Y=%d�� �ҷ���**** ",iPeakValue%8, iPeakValue/8); 

	pDC->TextOut(650,95,strtextout);
	if(peakvaleratio.fPeakVale_Ratio[iPeakValue/8][iPeakValue%8][4] != -1) {
		strtextout.Format("X=%d��Y=%d�� ���½ǣ�%5.3f ",iPeakValue%8, iPeakValue/8,peakvaleratio.fPeakVale_Ratio[iPeakValue/8][iPeakValue%8][4]);
	} else strtextout.Format("X=%d��Y=%d�� ���½ǣ�**** ",iPeakValue%8, iPeakValue/8); 
	pDC->TextOut(650,110,strtextout);

	if(peakvaleratio.fPeakVale_Ratio[iPeakValue/8][iPeakValue%8][5] != -1) {
		strtextout.Format("X=%d��Y=%d�� �·���%5.3f ",iPeakValue%8, iPeakValue/8,peakvaleratio.fPeakVale_Ratio[iPeakValue/8][iPeakValue%8][5]);
	} else strtextout.Format("X=%d��Y=%d�� �·���****  ",iPeakValue%8, iPeakValue/8); 
	pDC->TextOut(650,125,strtextout);
	if(peakvaleratio.fPeakVale_Ratio[iPeakValue/8][iPeakValue%8][6] != -1) {
		strtextout.Format("X=%d��Y=%d�� ���½ǣ�%5.3f ",iPeakValue%8, iPeakValue/8,peakvaleratio.fPeakVale_Ratio[iPeakValue/8][iPeakValue%8][6]);
	} else strtextout.Format("X=%d��Y=%d�� ���½ǣ�**** ",iPeakValue%8, iPeakValue/8); 
	pDC->TextOut(650,140,strtextout);
	if(peakvaleratio.fPeakVale_Ratio[iPeakValue/8][iPeakValue%8][7] != -1) {
		strtextout.Format("X=%d��Y=%d�� �󷽣�%5.3f ",iPeakValue%8, iPeakValue/8,peakvaleratio.fPeakVale_Ratio[iPeakValue/8][iPeakValue%8][7]);
	} else strtextout.Format("X=%d��Y=%d�� �󷽣�**** ",iPeakValue%8, iPeakValue/8); 
	pDC->TextOut(650,155,strtextout);

	//show text;

	strtextout.Format("�������ţ� %d  ,X=%d��Y=%d��",iPeakValue,iPeakValue%8, iPeakValue/8); 
	pDC->TextOut(650,180,strtextout);
	

	ReleaseDC(pDC);
}

void CDynShowDlg::OnLimitStat() 
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

	for( i=0;i<8;i++) {
		for(int j=0;j<8;j++) {
			limit_stat[i][j] = 0;

		}
	}	

	for(i=0;i<8;i++) { //������
		for(int j=0;j<8;j++) { //������
			for(int k=X0[i][j];k<=X1[i][j];k++) {
				for(int m=Y0[i][j];m<=Y1[i][j];m++) {
					limit_stat[i][j] +=  m_sdot[m][k];					
				}
			}
		}
	}

	int max_stat= m_Limit_Stat[0][0];
	for(i=0;i<8;i++) {
		for(int j=0;j<8;j++){
			if(limit_stat[i][j] > max_stat) 
				max_stat= limit_stat[i][j];
		}
	}
	if(max_stat < 1) max_stat = 1;
	for(i=0;i<8;i++) {
		for(int j=0;j<8;j++){
			igoto_one[i][j] = 100* limit_stat[i][j]/max_stat ;
		}
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
						RGB(
						GetBValue(ColorSet(limit_stat[j][k],max_stat)), 
						GetGValue(ColorSet(limit_stat[j][k],max_stat)),
						GetRValue(ColorSet(limit_stat[j][k],max_stat))
						)
						);// 30003000					
					CBrush* pOldBrush=pDshow->SelectObject(&bShow);// 3000 
					pDshow->Rectangle(x-32,y-32,x+32,y+32);// 
					
					pDshow->SelectObject(pOldBrush);//++++
					bShow.DeleteObject();

					strtext.Format("%d",igoto_one[j][k]);				
					pDC->TextOut(x-5,y-5,strtext);			
					
					x += g_draw_size*8;
			}
			y += g_draw_size*8;
	   }

	   ReleaseDC(pDshow);	
	
}

void CDynShowDlg::OnEnerspect64eth() 
{
	//������������
	unsigned char chartemp[128];
	if (m_CurFileName == "") {
		AfxMessageBox("���ȡ��ԭʼ�ļ�������");
		return;
	}
	CFile sourcFile;
	if( !sourcFile.Open( m_CurFileName, CFile::modeRead) ) {
		AfxMessageBox("ԭʼ�ļ� �򿪴���");
		return;			
	}
	int filelenth = sourcFile.GetLength();
	unsigned char byteX[1],byteY[1],byteE[1],byteT[1];
	int datacounter = filelenth/4;
	unsigned char manydata[1024];
	
	memset(m_sdot,0,64*64*4);
	memset(m_isdot_datafile,0,64*64*4);
	memset(m_iEth_datafile,0,64*64*4);
	memset(m_Energer_Spectrum,0,64*4);
	memset(m_Time_Spectrum,0,128*4);
    int i;
/*	for(i=0;i<64;i++) {
		m_iEnergeSpectEth_Low[i] = 0;
		m_iEnergeSpectEth_Hight[i] = 63;
	}
*/
	
	sourcFile.Seek(0,CFile::begin );	
	sourcFile.Read(byteX,1);
	sourcFile.Read(byteY,1);
	sourcFile.Read(byteE,1);
	sourcFile.Read(byteT,1);
	
    // ���ļ�ͷ��ԭʼ����
	sourcFile.Read(chartemp,124);
	sourcFile.Seek(128,CFile::begin );
	i=0;
	int x_No = 3,y_No = 3;
	BOOL flagx = true;
	BOOL flagy = true;			
	int num =0;
	int realcounter;
	while (num < filelenth-128) {
		realcounter = filelenth-128-num;
		if( realcounter >= 1024*1) {
			sourcFile.Read(manydata,1024*1);
			num += 1024*1;
			realcounter = 1024*1;
		}
		else {  
			sourcFile.Read(manydata,filelenth-128-num);
			num += filelenth-128-num;
			realcounter = filelenth-128-num;
		}
		
		for(int i=0;i<realcounter/4;i++) {
			byteX[0] = (manydata[4*i] >>1)&0x3f;
			byteY[0] = (manydata[4*i+1] >>1)&0x3f;
			byteE[0] = (manydata[4*i+2] >>1)&0x3f;
			byteT[0] = (manydata[4*i+3]&0x7f);
			//	byteT[0] = (manydata[4*i]&0x7f);// tests akang

			//ͳ��������
			if((byteE[0] >= ileft_limit)&&(byteE[0] <= iright_limit)) {
				m_isdot_datafile[byteY[0]][byteX[0]] ++;
				m_Time_Spectrum[byteT[0]] ++;
			}
			m_Energer_Spectrum[byteE[0]] ++;
			
			//ͳ��64����������----------//
			x_No = 3,y_No = 3;
			flagx = true;	flagy = true;
			while((flagx)||(flagy)) {
				if(byteX[0] < m_Position_X0[y_No][x_No]) { if((--x_No) < 0) flagx = false; }
				else if(byteX[0] > m_Position_X1[y_No][x_No]) {  if((++x_No) > 7) flagx = false;}
				else flagx = false;
				
				if(byteY[0] < m_Position_Y0[y_No][x_No]) { if((--y_No) < 0) flagy = false; }
				else if(byteY[0] > m_Position_Y1[y_No][x_No]) { if((++y_No) > 7) flagy = false; }
				else flagy = false;
			} 
			if((byteE[0] > m_iEnergeSpectEth_Low[y_No*8 + x_No])&&(byteE[0] < m_iEnergeSpectEth_Hight[y_No*8 + x_No]))
				m_iEth_datafile[y_No*8 + x_No][byteE[0]] ++;
			
			
			if(!m_bSelEthLimit_HandleInput) {			
				if((byteE[0] > m_iEnergeSpectEth_Low[y_No*8 + x_No])&&(byteE[0] < m_iEnergeSpectEth_Hight[y_No*8 + x_No])) {
					m_isdot_datafile[byteY[0]][byteX[0]] ++;				
				}			
			}
		}
	}	
	sourcFile.Close();
	//--end of ������������ 	


	//���ݿ���
	memcpy(energeSpectEth.m_iEnergeSpectrum64,m_iEth_datafile, 64*64*4);
	memcpy(energeSpectEth.m_iEnergeSpectEth_Low,m_iEnergeSpectEth_Low,64*4);
	memcpy(energeSpectEth.m_iEnergeSpectEth_Hight,m_iEnergeSpectEth_Hight,64*4);
	
	energeSpectEth.fileoper_Newfile = fileoper_Newfile;
	energeSpectEth.fileoper_iblocksel = fileoper_iblocksel; 

	energeSpectEth.DoModal();

	memcpy(m_iEnergeSpectEth_Low,energeSpectEth.m_iEnergeSpectEth_Low,64*4);
	memcpy(m_iEnergeSpectEth_Hight,energeSpectEth.m_iEnergeSpectEth_Hight,64*4);

	fileoper_Newfile = energeSpectEth.fileoper_Newfile;
	fileoper_iblocksel = energeSpectEth.fileoper_iblocksel; 
	
}

void CDynShowDlg::OnCheck1() 
{
	// TODO: Add your control notification handler code here
	m_bSameDriction_X = (!m_bSameDriction_X);
	
}

void CDynShowDlg::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	// TODO: Add your message handler code here and/or call default
	
	CDialog::OnKeyUp(nChar, nRepCnt, nFlags);
}

void CDynShowDlg::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	// TODO: Add your message handler code here and/or call default
/*	if(nChar == VK_F5)  OnRedraw_3d();               //3Dˢ��
	if(nChar == VK_F6)	OnRedraw_2d();				// Сͼˢ��
	if(nChar == VK_F8)	RedrawLimitFile();				//λ�ñ߽�ˢ��
*/
	
	CDialog::OnKeyDown(nChar, nRepCnt, nFlags);
}


void CDynShowDlg::OnCheckMidfilter() 
{

	m_bEnMidFilter = (!m_bEnMidFilter);

	if(m_bEnMidFilter) { //������ֵ�˲�
		memcpy(m_Backup_iMidfilter_sdot,m_sdot,64*64*4);
		OnMidfilter();
		memcpy(m_sdot,m_iMidfilter_sdot,64*64*4);

		memcpy(m_isdot_datafile,m_sdot,64*64*4);
	}
	else {
		memcpy(m_sdot,m_Backup_iMidfilter_sdot,64*64*4);

		memcpy(m_isdot_datafile,m_Backup_iMidfilter_sdot,64*64*4);
	}
	
	OnRedraw_3d();

}

void CDynShowDlg::OnMidfilter()
{
	float avrage;
	memcpy(m_iMidfilter_sdot,m_sdot,64*64*4);
	for(int i=1;i<63;i++) {
		for(int j=1;j<63;j++) {
			avrage = 0;
			avrage += m_sdot[i-1][j-1];
			avrage += m_sdot[i-1][j];
			avrage += m_sdot[i-1][j+1];
			avrage += m_sdot[i][j+1];
			avrage += m_sdot[i+1][j+1];
			avrage += m_sdot[i+1][j];
			avrage += m_sdot[i+1][j-1];
			avrage += m_sdot[i][j-1];
			avrage += m_sdot[i][j];

			avrage = avrage/9;

			m_iMidfilter_sdot[i][j] = avrage;
		}
	}

	for(i=1;i<63;i++) {
		avrage = 0;
		avrage += m_sdot[0][i-1];
		avrage += m_sdot[0][i];
		avrage += m_sdot[0][i+1];
		avrage = avrage/3;
		m_iMidfilter_sdot[0][i] = avrage;
	}
	for(i=1;i<63;i++) {
		avrage = 0;
		avrage += m_sdot[63][i-1];
		avrage += m_sdot[63][i];
		avrage += m_sdot[63][i+1];
		avrage = avrage/3;
		m_iMidfilter_sdot[63][i] = avrage;
	}


	for(i=1;i<63;i++) {
		avrage = 0;
		avrage += m_sdot[i-1][0];
		avrage += m_sdot[i][0];
		avrage += m_sdot[i+1][0];
		avrage = avrage/3;
		m_iMidfilter_sdot[i][0] = avrage;
	}
	for(i=1;i<63;i++) {
		avrage = 0;
		avrage += m_sdot[i-1][63];
		avrage += m_sdot[i][63];
		avrage += m_sdot[i+1][63];
		avrage = avrage/3;
		m_iMidfilter_sdot[i][63] = avrage;
	}

	avrage = 0;
	avrage += m_sdot[0][0];
	avrage += m_sdot[0][1];
	avrage += m_sdot[1][0];
	avrage = avrage/3;
	m_iMidfilter_sdot[0][0] = avrage;

	avrage = 0;
	avrage += m_sdot[0][62];
	avrage += m_sdot[0][63];
	avrage += m_sdot[1][63];
	avrage = avrage/3;
	m_iMidfilter_sdot[0][63] = avrage;

	avrage = 0;
	avrage += m_sdot[62][0];
	avrage += m_sdot[63][0];
	avrage += m_sdot[63][1];
	avrage = avrage/3;
	m_iMidfilter_sdot[63][0] = avrage;

	avrage = 0;
	avrage += m_sdot[62][63];
	avrage += m_sdot[63][63];
	avrage += m_sdot[63][62];
	avrage = avrage/3;
	m_iMidfilter_sdot[63][63] = avrage;

}

void CDynShowDlg::OnClearData() 
{
	memset(m_Energer_Spectrum,0,64*4);
	memset(m_isdot_datafile,0,64*64*4);
	memset(m_isdot_datafile,0,64*64*4);
	memset(m_iEth_datafile,0,64*64*4);
	memset(m_Time_Spectrum,0,128*4);

	memset(m_arrTimeFuheData,0,256*4);
	
	OnRedraw_2d();	
	OnRedraw_3d();
}



void CDynShowDlg::OnShowTimeFuheMap_2D()
{
	UINT arrGo2One[256];
	char chrValue[30];
	memcpy(arrGo2One,m_arrTimeFuheData,256*4);
    int position = 0;
	UINT maxvalue = 1;
	for(int i=0;i<256;i++) {
		if (arrGo2One[i] > maxvalue) {
			maxvalue = arrGo2One[i];
			position = i;
		}
	}

	sprintf(chrValue,"val:%d  pos:%d",maxvalue,position);
	maxvalue = 1.5 * (float)maxvalue;  //�����ֵ������1.5��

    // �������ߵ���ز���
	CDC* pDC=GetDC();			
	CRect clientrect;
	GetClientRect(clientrect);			
	CDC MemDC; //���ȶ���һ����ʾ�豸����
	CBitmap MemBitmap;//����һ��λͼ����	
	MemDC.CreateCompatibleDC(NULL);
	MemBitmap.CreateCompatibleBitmap(pDC,clientrect.Width(),clientrect.Height()); //�Զ���һ��ͼ��Ĵ�С
	CBitmap *pOldBit=MemDC.SelectObject(&MemBitmap);			
	
	CPen pShow5(PS_SOLID,2,RGB(212,208,200));//RGB(0,210,0)
	CPen* pOldPen=NULL;
	pOldPen=MemDC.SelectObject(&pShow5);
	
	CBrush bShow;			
	bShow.CreateSolidBrush(RGB(219,206,198));// 30003000				 					  
	CBrush* pOldBrush=MemDC.SelectObject(&bShow);// 3000 
	MemDC.Rectangle(635,50,920,285);// ˢ��С���ڵĴ�С
	
	MemDC.SelectObject(pOldBrush);//++++
	bShow.DeleteObject();	
	
	MemDC.SetBkColor(RGB(219,206,198));
	CPen pShow(PS_SOLID,1,RGB(15, 105,185));//RGB(0,210,0)
	pOldPen=NULL;
	pOldPen=MemDC.SelectObject(&pShow);

	//��������ϵͳ
	MemDC.MoveTo(650,65); 
	MemDC.LineTo(650,265);
	MemDC.MoveTo(650,65);
	MemDC.LineTo(650-3,65+5);
	
	MemDC.MoveTo(650,265);
	MemDC.LineTo(650 +4*64 ,265);
	MemDC.MoveTo(650 +4*64,265);
	MemDC.LineTo(650 +4*64 - 5,265+3);

	MemDC.MoveTo(650 +1*128,265);
	MemDC.LineTo(650 +1*128,265+3);

	MemDC.MoveTo(650 +1*64,265);
	MemDC.LineTo(650 +1*64,265+3);

	MemDC.MoveTo(650 +1*192,265);
	MemDC.LineTo(650 +1*192,265+3);
		
	MemDC.TextOut(650 +126,268,"0");
	MemDC.TextOut(650 +63,268,"-64");
	MemDC.TextOut(650 +190,268,"64");

	MemDC.TextOut(638,48,chrValue);
	MemDC.TextOut(635,268,"-127");
	MemDC.TextOut(897,268,"127");    
	
	//��������
	int x = 650 ; 
	if(maxvalue < 1) maxvalue = 1;
	int tplace = 265- (int)(2*128*(float)arrGo2One[0]/(float)maxvalue);
	for(int kk=1;kk<255;kk++) {																				
		MemDC.MoveTo(x,tplace);
		x += 1;//������һ�����صķֱ���
		tplace = 265 - (int)(2*128*(float)arrGo2One[kk]/(float)maxvalue);		
		if(tplace < 60) tplace = 60;
		if(tplace > 265) tplace = 265;
		MemDC.LineTo(x,tplace);
	}

	pDC->BitBlt(635,50,285,235,&MemDC,635,50,SRCCOPY);	
	MemBitmap.DeleteObject();
	MemDC.DeleteDC();	
	ReleaseDC(pDC);
}

// ʱ����ϵ����ݷֽ⣺
// Դ��������:  pData , datasize;
// Ŀ�������:  m_arrTimeFuheData[]
BOOL CDynShowDlg::OnAnalyzeTimeFuheData(char* pData,int datasize)
{
	if (datasize%4 != 0) {
		return FALSE;
	}
	//data0 : in std_logic;		 -- A · ʱ��ֵ
	//data1 : in std_logic;		 -- A · ���� ����2λ����01����
	//data2 : in std_logic;		 -- B · ʱ��ֵ
	//data3 : in std_logic;		 -- B · ���� ����2λ����10����	
	UINT addr = 0;
	UINT addrL = 0;
	UINT addrh = 0;
	int i=0;
	UINT kk = 0;
	while(i < datasize) { // ÿ�����Ĳ�
		addrh = (unsigned char)pData[i] & 0x7f;   
		addrL = (unsigned char)pData[i+2] & 0x7f;
		addr = addrh - addrL + 128; // A - B ·�������ݵ�ʱ��
		if( (addr >= 0) && (addr <= 255) ) {
			m_arrTimeFuheData[addr] += 1;
		}
	/*
		if(pData[i] == 128) {
			kk = 128;
		}
		else {
			kk = pData[i] & 0x7f;
		}
        
		m_arrTimeFuheData[kk] += 1; //ֻ��ͳ��A·�ź� ��ʱ��ֲ�
     */
		i += 4;
	}

	return TRUE;
}
