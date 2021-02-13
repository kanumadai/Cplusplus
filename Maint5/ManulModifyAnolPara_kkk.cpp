// ManulModifyAnolPara.cpp : implementation file
//

#include "stdafx.h"
#include "Maint.h"
#include "ManulModifyAnolPara.h"
#include "MaintDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern BOOL g_bOperating_32bkPeakStat;
extern TYPE type;

/////////////////////////////////////////////////////////////////////////////
// CManulModifyAnolPara dialog


CManulModifyAnolPara::CManulModifyAnolPara(CWnd* pParent /*=NULL*/)
	: CDialog(CManulModifyAnolPara::IDD, pParent)
{
	//{{AFX_DATA_INIT(CManulModifyAnolPara)
	m_strStartFileNo = _T("");
	m_strEndFileNo = _T("");
	m_strABCDModify = _T("");
	//}}AFX_DATA_INIT
	
	memset(m_ReadBucketIndex,0,32*4);
	m_iStatCounter = 0;

}


void CManulModifyAnolPara::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CManulModifyAnolPara)
	DDX_Control(pDX, IDC_MANULMODITY_EDIT, m_ManulModify_Edit);
	DDX_Control(pDX, IDC_MSFLEXGRID1, m_Grid);
	DDX_Text(pDX, IDC_EDIT_START_FILE, m_strStartFileNo);
	DDX_Text(pDX, IDC_EDIT_END_FILE, m_strEndFileNo);
	DDX_Text(pDX, IDC_EDIT_ABCD_MODIFY, m_strABCDModify);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CManulModifyAnolPara, CDialog)
	//{{AFX_MSG_MAP(CManulModifyAnolPara)
	ON_BN_CLICKED(IDC_READ_DATA, OnReadData)
	ON_BN_CLICKED(IDC_SAVE_DATA, OnSaveData)
	ON_EN_KILLFOCUS(IDC_MANULMODITY_EDIT, OnKillfocusManulmodityEdit)
	ON_BN_CLICKED(IDC_LOAD_FILENAME, OnLoadFilename)
	ON_BN_CLICKED(IDC_ABCD_MODIFY, OnAbcdModify)
	ON_BN_CLICKED(IDC_OFFSET_MODIFY, OnOffsetModify)
	ON_BN_CLICKED(IDC_MANUAL_MOD_PRE, OnManualModPre)
	ON_WM_LBUTTONDOWN()
	ON_BN_CLICKED(IDC_MANUAL_MOD, OnManualMod)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CManulModifyAnolPara message handlers

void CManulModifyAnolPara::OnReadData() 
{
	UpdateData(true);
	if (!m_strStartFileNo.IsEmpty()) {
		m_iStartFileNo = _atoi64(m_strStartFileNo);
		if ((m_iStartFileNo >= 32)||(m_iStartFileNo < 0)) {
			SetDlgItemText(IDC_EDIT_START_FILE,"0");
			m_iStartFileNo = 0;
		}
	} 
	else{
		SetDlgItemText(IDC_EDIT_START_FILE,"0");
		m_iStartFileNo = 0;
	}
	
	if (!m_strEndFileNo.IsEmpty()) {
		m_iEndFileNo = _atoi64(m_strEndFileNo);
		if ((m_iEndFileNo >= 32)||(m_iEndFileNo < 0)) {
			SetDlgItemText(IDC_EDIT_END_FILE,"31");
			m_iEndFileNo = 31;
		}
	} 
	else{
		SetDlgItemText(IDC_EDIT_END_FILE,"0");
		m_iEndFileNo = 0;
	}
	
	
	CFile srcfile;
	CString strtemp;
	BOOL bFileOpenSuccess = false;

	int peakPosition[32][8];

	m_iStatCounter = 0;	
	memset(m_ReadBucketIndex,0,32*4);
	memset(peakPosition,0,32*8*4);

	m_Grid.SetBackColor(RGB(30,189,169));	
	m_Grid.SetBackColorFixed(RGB(145,209,215));

	for(int bk=m_iStartFileNo;bk<=m_iEndFileNo;bk++) {
		bFileOpenSuccess = false;
		strtemp.Format("%.2d.dat",bk);
		if (srcfile.Open(m_SourceFilePath+ strtemp,CFile::modeRead)) { 
			srcfile.Close();
			bFileOpenSuccess = true;  //文件都存在			
		}
		if (bFileOpenSuccess) {// 读取 源文件，源文件边界，目标文件；
			
			g_bOperating_32bkPeakStat = true;
			
			m_strOperating_32bkPeakFilePath = m_SourceFilePath + strtemp;

			OnReadAnolPara(bk,m_iStatCounter,m_strOperating_32bkPeakFilePath);	
			
			OnFastPeakStat(bk, m_strOperating_32bkPeakFilePath, &(peakPosition[m_iStatCounter][0]));

			g_bOperating_32bkPeakStat = false;
			
			m_ReadBucketIndex[m_iStatCounter] = bk;
			m_iStatCounter++;
		}
	}

	m_Grid.SetRows(m_iStatCounter* 9 + 1);

	char chtext[20];

	for(int kk=0;kk<m_iStatCounter;kk++) {
		//m_Gird.
		for(int block=0;block<8;block++) {
			m_Grid.SetRow(kk*9 + block +1);
			m_Grid.SetCol(0);			
			sprintf(chtext,"Bu%d _b%d",m_ReadBucketIndex[kk],block);
			m_Grid.SetText(chtext); 
			
			m_Grid.SetCol(1);
			m_Grid.SetCellBackColor(RGB(30,189,169));
			sprintf(chtext,"%.2d",m_AnolParaData[kk][block][0]);
			m_Grid.SetText(chtext); 
			m_Grid.SetCol(2);
			m_Grid.SetCellBackColor(RGB(30,189,169));
			sprintf(chtext,"%.2d",m_AnolParaData[kk][block][1]);
			m_Grid.SetText(chtext); 
			m_Grid.SetCol(3);
			m_Grid.SetCellBackColor(RGB(30,189,169));
			sprintf(chtext,"%.2d",m_AnolParaData[kk][block][2]);
			m_Grid.SetText(chtext); 
			m_Grid.SetCol(4);
			m_Grid.SetCellBackColor(RGB(30,189,169));
			sprintf(chtext,"%.2d",m_AnolParaData[kk][block][3]);
			m_Grid.SetText(chtext); 
			m_Grid.SetCol(5);
			m_Grid.SetCellBackColor(RGB(30,189,169));
			sprintf(chtext,"%.2d",m_AnolParaData[kk][block][4]);
			m_Grid.SetText(chtext); 
			m_Grid.SetCol(6);
			m_Grid.SetCellBackColor(RGB(30,189,169));
			sprintf(chtext,"%.2d",m_AnolParaData[kk][block][5]);
			m_Grid.SetText(chtext); 
			m_Grid.SetCol(7);
			m_Grid.SetCellBackColor(RGB(30,189,169));
			sprintf(chtext,"%.2d",m_AnolParaData[kk][block][6]);
			m_Grid.SetText(chtext); 
			m_Grid.SetCol(8);
			m_Grid.SetCellBackColor(RGB(30,189,169));
			sprintf(chtext,"%.2d",m_AnolParaData[kk][block][7]);
			m_Grid.SetText(chtext); 

			m_Grid.SetCol(9);
			
			if ((peakPosition[kk][block] > 42)||(peakPosition[kk][block] <39)) {
				m_Grid.SetCellBackColor(RGB(221,226,5));
			}else m_Grid.SetCellBackColor(RGB(214,142,217));
			
			sprintf(chtext,"%.2d",peakPosition[kk][block]);
			m_Grid.SetText(chtext);				
			
		}
		m_Grid.SetRow(kk*9);
		m_Grid.SetCol(0);
		m_Grid.SetText(CString("  ")); 	
	} // end for

	sprintf(chtext," %d 个Bucket文件读取完成",m_iStatCounter);
	AfxMessageBox(chtext);

	
}

void CManulModifyAnolPara::OnSaveData() 
{

	CString filename;
	CString strBuff;
	CFile saveFile;
	int savecounter = 0;

	unsigned char chartemp[10];
	for(int bk=0;bk<m_iStatCounter;bk++) {
		strBuff.Format("%.2d.dat",m_ReadBucketIndex[bk]);
		filename = m_SourceFilePath + strBuff;
		if(!saveFile.Open(filename,CFile::modeReadWrite)) {
			AfxMessageBox("需要保存的文件名不存在，请核实文件路径");
			return;
		}
		UpdateData(true);

		savecounter ++;

		saveFile.Seek(PARA_ANOLPARA_DATA_ADDR,CFile::begin );		
		for(int block=0;block<8;block++) {				
			m_Grid.SetRow(9*bk  + block + 1);
			for(int col=0;col<8;col++) {
				m_Grid.SetCol(col+1);
				m_Grid.SetCellBackColor(RGB(30,189,169));
				m_AnolParaData[bk][block][col] = (unsigned char)atoi(m_Grid.GetText());
			}			
		}
		for(block=0;block<8;block++) {	
			memcpy(chartemp,&m_AnolParaData[bk][block],8);
			//if(m_ReadBucketIndex[bk]%2 == 0) { 
			//		memcpy(chartemp,&m_AnolParaData[bk][block],8); }		
			//else {			
			//		memcpy(chartemp,&m_AnolParaData[bk][7-block],8);			
			//}
			
			saveFile.Write(chartemp,8);
		}
		saveFile.Close();
	}
	
	char chtext[20];
	sprintf(chtext," %d 个Bucket保存完成",savecounter);
	AfxMessageBox(chtext);	
	
}

BOOL CManulModifyAnolPara::OnInitDialog() 
{
	CDialog::OnInitDialog();

	m_Grid.SetCols(10);
	m_Grid.SetRows(30);
	
	m_Grid.SetRow(0);
    m_Grid.SetColWidth(0,1200);
	m_Grid.SetRow(0);
	for(int i=1;i<10;i++)
	{ 
		m_Grid.SetColWidth(i,700);
	}

	m_Grid.SetCol(1);
    m_Grid.SetText(CString("   A"));
	m_Grid.SetCol(2);
    m_Grid.SetText(CString("   B"));
	m_Grid.SetCol(3);
    m_Grid.SetText(CString("   C"));
	m_Grid.SetCol(4);
    m_Grid.SetText(CString("   D")); 
	m_Grid.SetCol(5);
    m_Grid.SetText(CString("   X")); 
	m_Grid.SetCol(6);
    m_Grid.SetText(CString("   Y")); 
	m_Grid.SetCol(7);
    m_Grid.SetText(CString("   E")); 
	m_Grid.SetCol(8);
    m_Grid.SetText(CString("  Eth")); 
	m_Grid.SetCol(9);
    m_Grid.SetText(CString("  峰位"));

	m_Grid.SetCol(0);
	for(i=1;i<20;i++) {
			m_Grid.SetRow(i);
			if (i%9 == 0) { 
				m_Grid.SetText(CString(" "));
			}
			else m_Grid.SetText(CString(" "));
	}
	m_Grid.SetBackColor(RGB(30,189,169));
	
	m_Grid.SetBackColorFixed(RGB(145,209,215));

	this->SetDlgItemText(IDC_EDIT_START_FILE,"0");
	this->SetDlgItemText(IDC_EDIT_END_FILE,"31");
	this->SetDlgItemText(IDC_EDIT_ABCD_MODIFY,"42");
	
	return TRUE;  
}

BEGIN_EVENTSINK_MAP(CManulModifyAnolPara, CDialog)
    //{{AFX_EVENTSINK_MAP(CManulModifyAnolPara)
	ON_EVENT(CManulModifyAnolPara, IDC_MSFLEXGRID1, -601 /* DblClick */, OnDblClickMsflexgrid1, VTS_NONE)
	ON_EVENT(CManulModifyAnolPara, IDC_MSFLEXGRID1, -600 /* Click */, OnClickMsflexgrid1, VTS_NONE)
	ON_EVENT(CManulModifyAnolPara, IDC_MSFLEXGRID1, 73 /* Scroll */, OnScrollMsflexgrid1, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

void CManulModifyAnolPara::OnDblClickMsflexgrid1() 
{
	long fpRow=m_Grid.GetRowSel();
	long fpCol=m_Grid.GetColSel();
	CDC* pDc=GetDC();
	//	ASSERT(pDc);
	int nTwipx=1440/pDc->GetDeviceCaps(LOGPIXELSX);
	int nTwipy=1440/pDc->GetDeviceCaps(LOGPIXELSY);
	ReleaseDC(pDc);
	long fpCellWidth=m_Grid.GetCellWidth()/nTwipx;
	long fpCellHeight =m_Grid.GetCellHeight()/nTwipy;
	long fpCellLeft=m_Grid.GetCellLeft()/nTwipx;
	long fpCellTop=m_Grid.GetCellTop()/nTwipy;
	m_ManulModify_Edit.SetWindowPos(&wndTop,
		fpCellLeft+12,
		fpCellTop+90,
		fpCellWidth,
		fpCellHeight+1,
		NULL);
	
//	m_Grid.SetCellBackColor(RGB(232,87,38));

    m_ManulModify_Edit.SetFocus();
    m_ManulModify_Edit.ShowWindow(SW_SHOW);// 
    SetDlgItemText(IDC_MANULMODITY_EDIT,"");
    CString fpData=m_Grid.GetText();
    m_ManulModify_Edit.ReplaceSel(fpData);

	strBuffInput = fpData;
	
}

void CManulModifyAnolPara::OnKillfocusManulmodityEdit() 
{
	CString fpInputData;
	GetDlgItemText(IDC_MANULMODITY_EDIT,fpInputData);
	if(fpInputData.IsEmpty())
	{
		
		m_Grid.SetCellBackColor(RGB(30,189,169));

		if(m_Grid.GetCol() == 9) {
			m_Grid.SetCellBackColor(RGB(214,142,217));
		}
		m_Grid.SetText("");
		return;
	}
	
	unsigned long lcolor = RGB(30,189,169);
	unsigned long reascolor;

	if (strBuffInput == fpInputData) {
		reascolor = m_Grid.GetCellBackColor();
		if(lcolor == reascolor) {
			m_Grid.SetCellBackColor(RGB(30,189,169));	
			if(m_Grid.GetCol() == 9) {
				m_Grid.SetCellBackColor(RGB(214,142,217));
			}	
		}
		
	}
	else m_Grid.SetCellBackColor(RGB(232,87,38));

//	if(m_Grid.GetCol() == 9) {
//		m_Grid.SetCellBackColor(RGB(214,142,217));
//	}
	
	m_Grid.SetText(fpInputData);
	long fpCol=m_Grid.GetColSel();
	if(fpCol>0&&fpCol<=9)
	{
		double fpValue=atol(fpInputData); 
	}
    m_ManulModify_Edit.ShowWindow(SW_HIDE);
	
}

void CManulModifyAnolPara::OnLoadFilename() 
{
	CFileDialog dlg(FALSE,"","");///TRUE为OPEN对话框，FALSE为SAVE AS对话框 	
	dlg.m_ofn.lpstrFilter = ("选择源文件名");//
	if(dlg.DoModal()==IDOK) { 
		m_SourceFilePath=dlg.GetPathName(); 
		m_SourceFileName = dlg.GetFileName();
	}  
	else return;	
	SetDlgItemText(IDC_STATIC_PATH,m_SourceFilePath + "*");	
	
}

void CManulModifyAnolPara::OnReadAnolPara(int buckno, int lineNo,CString filename)
{
	CFile datafile;

	unsigned char chartemp[10];
	if(!datafile.Open(filename,CFile::modeRead) ) {
		return;	
	};

	datafile.Seek(PARA_ANOLPARA_DATA_ADDR,CFile::begin );
	
	for(int block=0;block<8;block++) {
		datafile.Read(chartemp,8);
		memcpy(&m_AnolParaData[lineNo][block],chartemp,8);
		//if(buckno%2 == 0) { memcpy(&m_AnolParaData[lineNo][block],chartemp,8); }		
		//else {			
		//	memcpy(&m_AnolParaData[lineNo][7-block],chartemp,8);			
		//}
		
	}

	datafile.Close();		

}

void CManulModifyAnolPara::OnClickMsflexgrid1() 
{
	m_ManulModify_Edit.ShowWindow(SW_HIDE);
	
}

BOOL CManulModifyAnolPara::OnFastPeakStat(int buckno, CString filename, int *pPeakPosition)
{
		CFile dotsfile;
		unsigned char chartemp[10];	
		
		int buffdots3w[8][64][64];
		int maxpeakval[8] = {1};
		int peakval_limitL[8] = {0}; 
		int	peakval_limitR[8] = {63};
		int peakval_table[8] = {1};
		int peakdata[8][64];

		if(!dotsfile.Open(filename,CFile::modeRead)) {
			return false;
		}

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
			
		memcpy(pPeakPosition,peakval_table,8*4);

		return true;
}

void CManulModifyAnolPara::OnAbcdModify() 
{
	UpdateData(true);
	int ipeakEth = 42;
	if (!m_strABCDModify.IsEmpty()) {
		ipeakEth = _atoi64(m_strABCDModify);
		if ((ipeakEth >= 63)||(ipeakEth < 0)) {
			SetDlgItemText(IDC_EDIT_ABCD_MODIFY,"42");
			ipeakEth = 42;
		}
	} 
	else{
		SetDlgItemText(IDC_EDIT_ABCD_MODIFY,"42");
		ipeakEth = 42;
	}

	int AbcdPara[256][4];
	int PeakPositionPara[256];
	CString strtext;

	for(int bk=0; bk<m_iStatCounter; bk++) {
		for(int block=0;block<8;block++) {
			m_Grid.SetRow(9*bk  + block + 1);
			m_Grid.SetCol(9);
			PeakPositionPara[8*bk+block] = atoi(m_Grid.GetText());
			
			m_Grid.SetCol(1);
			m_Grid.SetCellBackColor(RGB(232,87,38));
			AbcdPara[8*bk+block][0] = atoi(m_Grid.GetText());
			strtext.Format("%.2d",ipeakEth-PeakPositionPara[8*bk+block] + AbcdPara[8*bk+block][0]);
			m_Grid.SetText(strtext);

			m_Grid.SetCol(2);
			m_Grid.SetCellBackColor(RGB(232,87,38));
			AbcdPara[8*bk+block][1] = atoi(m_Grid.GetText());
			strtext.Format("%.2d",ipeakEth-PeakPositionPara[8*bk+block] + AbcdPara[8*bk+block][1]);
			m_Grid.SetText(strtext);

			m_Grid.SetCol(3);
			m_Grid.SetCellBackColor(RGB(232,87,38));
			AbcdPara[8*bk+block][2] = atoi(m_Grid.GetText());
			strtext.Format("%.2d",ipeakEth-PeakPositionPara[8*bk+block] + AbcdPara[8*bk+block][2]);
			m_Grid.SetText(strtext);

			m_Grid.SetCol(4);
			m_Grid.SetCellBackColor(RGB(232,87,38));
			AbcdPara[8*bk+block][3] = atoi(m_Grid.GetText());
			strtext.Format("%.2d",ipeakEth-PeakPositionPara[8*bk+block] + AbcdPara[8*bk+block][3]);
			m_Grid.SetText(strtext);		

		}
		

	}

	UpdateData(false);
	
}

void CManulModifyAnolPara::OnScrollMsflexgrid1() 
{
	

}

void CManulModifyAnolPara::OnOffsetModify() 
{
	
}

void CManulModifyAnolPara::OnManualModPre() 
{
	
}

void CManulModifyAnolPara::OnManualMod() 
{
	// TODO: Add your control notification handler code here
	
}
