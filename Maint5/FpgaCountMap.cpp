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
	sprintf(chtext,"Bucket:%d block:%d  �ܼ���:%d, ��Ч����:%d ����:%.2f��",bucket,block,m_arr32BuckData[bucket][block][0],m_arr32BuckData[bucket][block][1], 100*(float)m_arr32BuckData[bucket][block][1]/(float)counter);
	
	GetDlgItem(IDC_STATUS)->SetWindowText(chtext);


}

void CFpgaCountMap::OnRButtonDown(UINT nFlags, CPoint point) 
{
//	CDialog::OnRButtonDown(nFlags, point);
	CPoint ScreenPoint;   //������������Ļ�ϵ�λ��,������λ����ָ�û������λ��
    ScreenPoint = point;
    ClientToScreen(&ScreenPoint);
	
	UINT CommandId = 0;
    CMenu MyContextMenu;
    MyContextMenu.CreatePopupMenu();
    MyContextMenu.AppendMenu(MF_STRING, ++CommandId, "���ļ�");
    MyContextMenu.AppendMenu(MF_STRING, ++CommandId, "�����ļ�");
	MyContextMenu.AppendMenu(MF_STRING, ++CommandId, "������ʾ");
    MyContextMenu.AppendMenu(MF_STRING, ++CommandId, "Ч����ʾ");
	
	MyContextMenu.AppendMenu(MF_STRING, ID_SEPARATOR); 
    MyContextMenu.AppendMenu(MF_STRING, ++CommandId, "����...");
	
    UINT Command = (UINT)MyContextMenu.TrackPopupMenu(TPM_LEFTALIGN|TPM_NONOTIFY | TPM_RETURNCMD,
		ScreenPoint.x, ScreenPoint.y, this);
    //ʹ��TPM_NONOTIFY | TPM_RETURNCMD���ر�ѡ��Ĳ˵�����,0����û��(����
    //����˵�����)
    //����㲻��������������,��ĸ����ڽ������ѡ���������յ�WM_COMMAND��Ϣ
	
	MyContextMenu.DestroyMenu();
	switch(Command)
    {
	case 0:	//û������,���Բ������κ�����.			
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
	COLORREF rf=RGB(20,19,16);//����ɫ 
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
	pdc->TextOut(410,22,"���� :�ܼ���");

	pdc->SetTextColor(RGB(255,0,255));
	pdc->TextOut(410,38,"���� :��Ч����");
	
	ReleaseDC(pdc);
	

}

void CFpgaCountMap::OnOpenFile()
{
	CString FilePathName; 
	CFileDialog dlg(TRUE,"dat","*.dat");///TRUEΪOPEN�Ի���FALSEΪSAVE AS�Ի��� 
	
	dlg.m_ofn.lpstrFilter = ("FPGA�����ļ�.dat");//
	if(dlg.DoModal()==IDOK) FilePathName=dlg.GetPathName(); 
	else return;
	CFile file;
	if(!file.Open(FilePathName,CFile::modeRead)) {
		AfxMessageBox("�ļ�������");
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
	CFileDialog dlg(FALSE,"dat","*.dat");///TRUEΪOPEN�Ի���FALSEΪSAVE AS�Ի��� 
	
	dlg.m_ofn.lpstrFilter = ("FPGA�����ļ�.dat");//
	if(dlg.DoModal()==IDOK) FilePathName=dlg.GetPathName(); 
	else return;
	CFile file;
	if(!file.Open(FilePathName,CFile::modeCreate | CFile::modeWrite)) {
		AfxMessageBox("�ļ�������");
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
	COLORREF rf=RGB(20,19,16);//����ɫ 
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
	pdc->TextOut(410,22,"���� :Ч��ͳ��");	

	ReleaseDC(pdc);
}
