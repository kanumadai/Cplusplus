// FpgaCountMap.cpp : implementation file
//

#include "stdafx.h"
#include "Maint.h"
#include "FpgaCountMap.h"

#include "MaintDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFpgaCountMap dialog

extern TYPE type;


CFpgaCountMap::CFpgaCountMap(CWnd* pParent /*=NULL*/)
	: CDialog(CFpgaCountMap::IDD, pParent)
{
	//{{AFX_DATA_INIT(CFpgaCountMap)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CFpgaCountMap::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFpgaCountMap)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CFpgaCountMap, CDialog)
	//{{AFX_MSG_MAP(CFpgaCountMap)
	ON_WM_PAINT()
	ON_WM_MOUSEMOVE()
	ON_WM_RBUTTONDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFpgaCountMap message handlers

BOOL CFpgaCountMap::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CFpgaCountMap::OnPaint() 
{
	CPaintDC dc(this); // device context for painting

	OnDrawLine();	

}

void CFpgaCountMap::OnMouseMove(UINT nFlags, CPoint point) 
{
	CDialog::OnMouseMove(nFlags, point);

	if((point.x <= 5)||(point.x >= 512+5)) {
		GetDlgItem(IDC_STATUS)->SetWindowText("   ");
		return;
	}
	if ((point.y<20)||(point.y>320)) {
		GetDlgItem(IDC_STATUS)->SetWindowText("   ");
		return;
	}

	char chtext[100];

	int bucket = (point.x -5)/16;
	int block = ((point.x -5)/2)%8;

	int counter = m_arr32BuckData[bucket][block][0];

	if(m_arr32BuckData[bucket][block][0] <= 0) counter = 1;
	sprintf(chtext,"Bucket:%d block:%d  总计数:%d, 有效计数:%d 比率:%.2f％",bucket,block,m_arr32BuckData[bucket][block][0],m_arr32BuckData[bucket][block][1], 100*(float)m_arr32BuckData[bucket][block][1]/(float)counter);
	
	GetDlgItem(IDC_STATUS)->SetWindowText(chtext);


}

void CFpgaCountMap::OnRButtonDown(UINT nFlags, CPoint point) 
{
//	CDialog::OnRButtonDown(nFlags, point);
	CPoint ScreenPoint;   //计算在整个屏幕上的位置,否则点的位置是指用户区域的位置
    ScreenPoint = point;
    ClientToScreen(&ScreenPoint);
	
	UINT CommandId = 0;
    CMenu MyContextMenu;
    MyContextMenu.CreatePopupMenu();
    MyContextMenu.AppendMenu(MF_STRING, ++CommandId, "打开文件");
    MyContextMenu.AppendMenu(MF_STRING, ++CommandId, "保存文件");
	MyContextMenu.AppendMenu(MF_STRING, ++CommandId, "计数显示");
    MyContextMenu.AppendMenu(MF_STRING, ++CommandId, "效率显示");
	
	MyContextMenu.AppendMenu(MF_STRING, ID_SEPARATOR); 
    MyContextMenu.AppendMenu(MF_STRING, ++CommandId, "附加...");
	
    UINT Command = (UINT)MyContextMenu.TrackPopupMenu(TPM_LEFTALIGN|TPM_NONOTIFY | TPM_RETURNCMD,
		ScreenPoint.x, ScreenPoint.y, this);
    //使用TPM_NONOTIFY | TPM_RETURNCMD返回被选择的菜单命令,0代表没有(比如
    //点击菜单外面)
    //如果你不加入这两个类型,你的父窗口将会从你选的命令中收到WM_COMMAND消息
	
	MyContextMenu.DestroyMenu();
	switch(Command)
    {
	case 0:	//没有命令,所以不必做任何事情.			
		break;
	case 1:	
		OnOpenFile();
		break;
	case 2:
		OnSaveFile();
		break;
	case 3:
		OnDrawLine();
		break;
	case 4:
		OnShowEfficent();
		break;
	default:					
		break;
	}
	
}

void CFpgaCountMap::OnDrawLine()
{
	CDC *pdc = GetDC();	
	COLORREF rf=RGB(20,19,16);//背景色 
	CRect rcClient(6,10,520,321);
	pdc->FillSolidRect(&rcClient,rf);
	
	CPen pShow(PS_SOLID,1,RGB(255,255,0));//
	pdc->SelectObject(&pShow);

	int maxvalue_all = 2;
	int maxvalue_valid = 2;
	int pointx,pointy;
	for(int buck=0;buck<32;buck++) {
		for(int block=0;block<8;block++){
			if (m_arr32BuckData[buck][block][0] > maxvalue_all) {
				maxvalue_all = m_arr32BuckData[buck][block][0];
			}
			if (m_arr32BuckData[buck][block][1] > maxvalue_valid) {
				maxvalue_valid = m_arr32BuckData[buck][block][1];
			}
		}
	}

	pointx = 5;
	pointy = 320 - 250* (float)m_arr32BuckData[0][0][0]/maxvalue_all;
	pdc->MoveTo(pointx,pointy);
	for(buck=0;buck<32;buck++) {
		for(int block=0;block<8;block++){
			pointy = 320 - 250* (float)m_arr32BuckData[buck][block][0]/maxvalue_all;
			pointx ++;
			pdc->LineTo(pointx,pointy);
			pointx ++;
			pdc->LineTo(pointx,pointy);

		}
	}

	CPen pShow2(PS_SOLID,1,RGB(255,0,255));//
	pdc->SelectObject(&pShow2);	
	pointx = 5;
	pointy = 320 - 200* (float)m_arr32BuckData[0][0][1]/maxvalue_valid;
	pdc->MoveTo(pointx,pointy);
	for(buck=0;buck<32;buck++) {
		for(int block=0;block<8;block++){
			pointy = 320 - 200* (float)m_arr32BuckData[buck][block][1]/maxvalue_valid;
			pointx ++;
			pdc->LineTo(pointx,pointy);
			pointx ++;
			pdc->LineTo(pointx,pointy);
			
		}
	}
	
	pdc->SetTextColor(RGB(255,255,0));
	pdc->TextOut(410,22,"—— :总计数");

	pdc->SetTextColor(RGB(255,0,255));
	pdc->TextOut(410,38,"—— :有效计数");
	
	ReleaseDC(pdc);
	

}

void CFpgaCountMap::OnOpenFile()
{
	CString FilePathName; 
	CFileDialog dlg(TRUE,"dat","*.dat");///TRUE为OPEN对话框，FALSE为SAVE AS对话框 
	
	dlg.m_ofn.lpstrFilter = ("FPGA计数文件.dat");//
	if(dlg.DoModal()==IDOK) FilePathName=dlg.GetPathName(); 
	else return;
	CFile file;
	if(!file.Open(FilePathName,CFile::modeRead)) {
		AfxMessageBox("文件读错误");
		return;
	}

	file.Seek(128,CFile::begin);	
	
	for(int bucket=0;bucket<32;bucket++) {
		for(int block=0;block<8;block++) {			
			file.Read(&type.uchdata,4);					
			m_arr32BuckData[bucket][block][0] = type.intdata;
			file.Read(&type.uchdata,4);
			m_arr32BuckData[bucket][block][1] = type.intdata;			
		}
	}
	
	file.Close();

	OnDrawLine();
}

void CFpgaCountMap::OnSaveFile()
{
	CString FilePathName; 
	CFileDialog dlg(FALSE,"dat","*.dat");///TRUE为OPEN对话框，FALSE为SAVE AS对话框 
	
	dlg.m_ofn.lpstrFilter = ("FPGA计数文件.dat");//
	if(dlg.DoModal()==IDOK) FilePathName=dlg.GetPathName(); 
	else return;
	CFile file;
	if(!file.Open(FilePathName,CFile::modeCreate | CFile::modeWrite)) {
		AfxMessageBox("文件读错误");
		return;
	}
	file.SetLength(128+ 32*8*2*4);
	file.Seek(128,CFile::begin);	
	
	for(int bucket=0;bucket<32;bucket++) {
		for(int block=0;block<8;block++) {						
			type.intdata = m_arr32BuckData[bucket][block][0];
			file.Write(&type.uchdata,4);			
			type.intdata = m_arr32BuckData[bucket][block][1];
			file.Write(&type.uchdata,4);			
		}
	}
	
	file.Close();
	
	OnDrawLine();
}

void CFpgaCountMap::OnShowEfficent()
{
	CDC *pdc = GetDC();	
	COLORREF rf=RGB(20,19,16);//背景色 
	CRect rcClient(6,10,520,321);
	pdc->FillSolidRect(&rcClient,rf);
	
	CPen pShow(PS_SOLID,1,RGB(255,255,0));//
	pdc->SelectObject(&pShow);
	
	int maxvalue = 1;
	int pointx,pointy;
	int temp;
	float fEfficent[32][8];
	for(int buck=0;buck<32;buck++) {
		for(int block=0;block<8;block++){
			if (m_arr32BuckData[buck][block][0] <=1) {
				temp = 1;
			}
			else temp = m_arr32BuckData[buck][block][0];
			
			fEfficent[buck][block] = 100* (float)m_arr32BuckData[buck][block][1] / temp ;
			if (fEfficent[buck][block] > maxvalue) {
				maxvalue = fEfficent[buck][block];
			}
		}
	}
	
	pointx = 5;
	pointy = 320 - 2.5* fEfficent[0][0];
	if (pointy < 15) {	pointy = 15; }
	pdc->MoveTo(pointx,pointy);
	for(buck=0;buck<32;buck++) {
		for(int block=0;block<8;block++){
			pointy = 320 - 2.5* fEfficent[buck][block];
			if (pointy < 15) {
				pointy = 15;
			}
			pointx ++;
			pdc->LineTo(pointx,pointy);
			pointx ++;
			pdc->LineTo(pointx,pointy);			
		}
	}

	CPen pShow2(PS_SOLID,1,RGB(0,255,255));//
	pdc->SelectObject(&pShow2);
	pdc->MoveTo(10,70);
	pdc->LineTo(510,70);
	pdc->SetTextColor(RGB(0,255,255));
	pdc->TextOut(410,60, "100 % ");	
	
	pdc->SetTextColor(RGB(255,255,0));
	pdc->TextOut(410,22,"—— :效率统计");	

	ReleaseDC(pdc);
}
