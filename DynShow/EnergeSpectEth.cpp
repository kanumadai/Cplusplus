// EnergeSpectEth.cpp : implementation file
//

#include "stdafx.h"
#include "DynShow.h"
#include "EnergeSpectEth.h"
#include "FileSave.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern int g_draw_size;
extern int g_draw_frame;
extern int g_draw_startpoint_x;
extern int g_draw_startpoint_y;

/////////////////////////////////////////////////////////////////////////////
// CEnergeSpectEth dialog


CEnergeSpectEth::CEnergeSpectEth(CWnd* pParent /*=NULL*/)
	: CDialog(CEnergeSpectEth::IDD, pParent)
{
	//{{AFX_DATA_INIT(CEnergeSpectEth)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CEnergeSpectEth::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CEnergeSpectEth)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CEnergeSpectEth, CDialog)
	//{{AFX_MSG_MAP(CEnergeSpectEth)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_PAINT()
	ON_WM_RBUTTONDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEnergeSpectEth message handlers

BOOL CEnergeSpectEth::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here

	//-----���ײ���----//
	m_bEnergeSpectOperate_En = true;
	m_iEnergeSpectShowNum = 0;
	m_bEnergeSpectZoomInMode = false;
	m_bEnergeSpectSelEn = false;

	m_iEnergeSpectCurBlockNo = 0;
	m_iEnergeSpectCurSynNo = 0; //��ǰ�����

	m_bEnergeSpectMove_Eth_Low = false;
	m_bEnergeSpectMove_Eth_Hight = false;

/*	for(int i=0;i<8;i++){	//�������ݳ�ʼ��
		for(int j=0;j<64;j++)
				for(int k=0;k<64;k++) {
					m_iEnergeSpectrum64[i][j][k] = 0;
				}
	}

	for(i=0;i<8;i++){	//������ֵ ��ʼ��
		for(int j=0;j<64;j++) {
			m_iEnergeSpectEth_Low[i][j] = 0;
			m_iEnergeSpectEth_Hight[i][j] = 63;
		}
	}
*/

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CEnergeSpectEth::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default

	m_Oldpoint = point;
	if(m_bEnergeSpectZoomInMode) { //���ײ�����ʽ
			if((point.y < g_draw_size*64+g_draw_startpoint_x)&&(point.y>400)) {
				if((point.x >= (g_draw_startpoint_y +g_draw_size*m_iEnergeSpectEth_Low[m_iEnergeSpectCurBlockNo][m_iEnergeSpectCurSynNo] -3))
					&&(point.x <= (g_draw_startpoint_y +g_draw_size*m_iEnergeSpectEth_Low[m_iEnergeSpectCurBlockNo][m_iEnergeSpectCurSynNo] +3))) {
					m_bEnergeSpectMove_Eth_Low = true;
					m_bEnergeSpectMove_Eth_Hight = false;
				}
				else if((point.x >= (g_draw_startpoint_y +g_draw_size*m_iEnergeSpectEth_Hight[m_iEnergeSpectCurBlockNo][m_iEnergeSpectCurSynNo] -3))
					&&(point.x <= (g_draw_startpoint_y +g_draw_size*m_iEnergeSpectEth_Hight[m_iEnergeSpectCurBlockNo][m_iEnergeSpectCurSynNo] +3))) {
					m_bEnergeSpectMove_Eth_Low = false;
					m_bEnergeSpectMove_Eth_Hight = true;
				}
			}
			char textout[50];
			CEdit* pShowEnergeOption=(CEdit*)GetDlgItem(IDC_SHOWENERGE_OPTION);
			CEdit* pShowEnergeEthLimit = (CEdit*)GetDlgItem(IDC_SHOWENTERGE_ETHLIMIT);
			sprintf(textout,"���׷Ŵ���ʾ: ������� %d    �������� %d",m_iEnergeSpectCurBlockNo,m_iEnergeSpectCurSynNo);
			pShowEnergeOption->SetWindowText(textout);
			sprintf(textout,"״̬��ʾ�� ��������ֵ��%d    ����ֵ��%d",m_iEnergeSpectEth_Low[m_iEnergeSpectCurBlockNo][m_iEnergeSpectCurSynNo],
					m_iEnergeSpectEth_Hight[m_iEnergeSpectCurBlockNo][m_iEnergeSpectCurSynNo]);
			pShowEnergeEthLimit->SetWindowText(textout);

		return;
	}
//	int nBlindex= pBlockindex->GetCurSel();
//	if(nBlindex == -1) nBlindex = 0;
	int nBlindex = 0;

	if(m_bEnergeSpectSelEn) { // ���ײ��� ѡ��
		int barsize_x = 70;
		int barsize_y = 55;
		int barstart_x = 85;
		int barstart_y = 50;
		int selnum;
		bool flag = false;
		for(int i=0;i<8;i++) { //ˮƽ
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
			m_iEnergeSpectCurSynNo = selnum; //��ǰ�����
			OnDrawZoomInEnergeSpect(nBlindex,selnum);
			return;
		}			
		m_iEnergeSpectCurBlockNo = nBlindex;
	}


	
	CDialog::OnLButtonDown(nFlags, point);
}

void CEnergeSpectEth::OnLButtonUp(UINT nFlags, CPoint point) 
{
	if(m_bEnergeSpectZoomInMode) { //���ײ�����ʽ
		if((m_bEnergeSpectMove_Eth_Low)||(m_bEnergeSpectMove_Eth_Hight)) {
			m_bEnergeSpectMove_Eth_Low = false;
			m_bEnergeSpectMove_Eth_Hight = false;
			//OnRedraw_2d();[m_iEnergeSpectCurBlockNo][m_iEnergeSpectCurSynNo]
			OnDrawZoomInEnergeSpect(m_iEnergeSpectCurBlockNo,m_iEnergeSpectCurSynNo);
		}
		CString strtext;
		CEdit* pShowEnergeEthLimit = (CEdit*)GetDlgItem(IDC_SHOWENTERGE_ETHLIMIT);
		strtext.Format("״̬��ʾ����ǰ���� Eth �ͣ�%d   Eth��: %d",m_iEnergeSpectEth_Low[m_iEnergeSpectCurBlockNo][m_iEnergeSpectCurSynNo],m_iEnergeSpectEth_Hight[m_iEnergeSpectCurBlockNo][m_iEnergeSpectCurSynNo]);
		pShowEnergeEthLimit->SetWindowText(strtext);
		return;
	}
	
	CDialog::OnLButtonUp(nFlags, point);
}

void CEnergeSpectEth::OnMouseMove(UINT nFlags, CPoint point) 
{

	if(m_bEnergeSpectZoomInMode) {
			CDC* pDC=GetDC(); //����ά ֱ��
			CPen pShow2(PS_SOLID,2,RGB(255,10,,0));//RGB(0,210,0)
			CPen* pOldPen=NULL;
			pOldPen=pDC->SelectObject(&pShow2);

			pDC->SetROP2(R2_NOT);
			CString strtext;

			//----------��ƽ���������±߽硿--------//
			if(((point.x <g_draw_startpoint_y-1)||(point.x> g_draw_startpoint_y +g_draw_size*63+1))&&((m_bEnergeSpectMove_Eth_Low)||(m_bEnergeSpectMove_Eth_Hight)))  {
					ReleaseDC(pDC);
					return;
			}
			if((m_bEnergeSpectMove_Eth_Low)||(m_bEnergeSpectMove_Eth_Hight)) {
				pDC->MoveTo(m_Oldpoint.x, 80);
				pDC->LineTo(m_Oldpoint.x, 50+8*64+30);
				pDC->MoveTo(point.x, 80);
				pDC->LineTo(point.x, 50+8*64+30);
				m_Oldpoint = point;

				CEdit* pShowEnergeEthLimit = (CEdit*)GetDlgItem(IDC_SHOWENTERGE_ETHLIMIT);
				if(m_bEnergeSpectMove_Eth_Low) {
					strtext.Format("���ڲ�����Eth �ͣ�%d",(point.x-g_draw_startpoint_x)/8);
					m_iEnergeSpectEth_Low[m_iEnergeSpectCurBlockNo][m_iEnergeSpectCurSynNo] = (point.x-g_draw_startpoint_x)/8;
					pShowEnergeEthLimit->SetWindowText(strtext);
				}
				else if(m_bEnergeSpectMove_Eth_Hight) {
					strtext.Format("���ڲ�����Eth �� ��%d",(point.x-g_draw_startpoint_x)/8);
					m_iEnergeSpectEth_Hight[m_iEnergeSpectCurBlockNo][m_iEnergeSpectCurSynNo] = (point.x-g_draw_startpoint_x)/8;	
					pShowEnergeEthLimit->SetWindowText(strtext);
				}
			}
			ReleaseDC(pDC);
		return;
	}
	
	CDialog::OnMouseMove(nFlags, point);
}

void CEnergeSpectEth::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	m_bEnergeSpectOperate_En = true;
	OnDraw64EnergeSpectrum();
	m_bEnergeSpectSelEn = true;
	m_bEnergeSpectZoomInMode =false;

}

void CEnergeSpectEth::OnRButtonDown(UINT nFlags, CPoint point) 
{
	if(!m_bEnergeSpectOperate_En) return;

	CPoint ScreenPoint;   //������������Ļ�ϵ�λ��,������λ����ָ�û������λ��
    ScreenPoint = point;
    ClientToScreen(&ScreenPoint);

	UINT CommandId = 0;
    CMenu MyContextMenu;
    MyContextMenu.CreatePopupMenu();
    MyContextMenu.AppendMenu(MF_STRING, ++CommandId, "��ʾ64������");
    MyContextMenu.AppendMenu(MF_STRING, ++CommandId, "����3D��ʾ");
	MyContextMenu.AppendMenu(MF_STRING, ++CommandId, "�������ױ߽�");
    MyContextMenu.AppendMenu(MF_STRING, ++CommandId, "�������ױ߽�");
	MyContextMenu.AppendMenu(MF_STRING, ++CommandId, "���ټ������ױ߽�");
    MyContextMenu.AppendMenu(MF_STRING, ID_SEPARATOR); //to get separator line
    MyContextMenu.AppendMenu(MF_STRING, ++CommandId, "����...");




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
//	CMaintDoc*  pDoc = GetDocument();  
	switch(Command)
    {
		case 0:		
			//û������,���Բ������κ�����.			
			break;
		case 1:	//��ʾ64������
			m_bEnergeSpectOperate_En = true;
//			memcpy(m_iEnergeSpectrum64,pDoc->BuffDot3w,8*64*64*4);
			OnDraw64EnergeSpectrum();
			m_bEnergeSpectSelEn = true;
			m_bEnergeSpectZoomInMode =false;
			break;
		case 2:	//������ʾ	
		//	OnRedraw();
		//	m_bEnergeSpectSelEn = false;
		//	m_bEnergeSpectZoomInMode =false;
//			OnDrawZoomInEnergeSpect(0,5);
			break;
		case 3:		
			OnLoadEthFile();
			OnDraw64EnergeSpectrum();
			m_bEnergeSpectOperate_En = true;
			m_bEnergeSpectSelEn = true;
			m_bEnergeSpectZoomInMode =false;
			break;
		case 4:			
			OnSaveEthFile();	
			break;
		case 5:
			OnFastCalcEthLowHighLimit();
			break;
/**/
		case 6:
			AfxMessageBox(" XX��д���Ժ���˵�ɣ�");
			break;
		default:		
			//do default	AfxMessageBox("Invalide Selected");			
			break;
	}
	
	
	CDialog::OnRButtonDown(nFlags, point);
}

void CEnergeSpectEth::OnDraw64EnergeSpectrum()
{

		CDC* pDC=GetDC();
		CPen pShow5(PS_SOLID,2,RGB(219,206,198));//���屳��ɫ
	    CPen* pOldPen=NULL;
	    pOldPen=pDC->SelectObject(&pShow5);

		CBrush bShow;			
		bShow.CreateSolidBrush(RGB(219,206,198));// 	
		CBrush* pOldBrush=pDC->SelectObject(&bShow);//
		pDC->Rectangle(40,32,910,692);// 
					
		pDC->SelectObject(pOldBrush);//++++
		bShow.DeleteObject();

		CBrush smallwinbkcolor;
		smallwinbkcolor.CreateSolidBrush(RGB(153,185,165));


		int barsize_x = 70;
		int barsize_y = 55;
		int barstart_x = 85;
		int barstart_y = 50;
		pDC->SetBkColor(RGB(219,206,198));
		for(int i=0;i<8;i++) {
			barstart_x = 85;
			for (int j=0;j<8;j++) {	
				pDC->TextOut(barstart_x-5,barstart_y+barsize_y,"0");
				pDC->TextOut(barstart_x+barsize_x-15,barstart_y+barsize_y,"63");
				pDC->SelectObject(smallwinbkcolor);
				pDC->Rectangle(barstart_x,barstart_y,barstart_x+barsize_x,barstart_y+barsize_y);//
				barstart_x += 100;	
				smallwinbkcolor.DeleteObject();				
			}
			barstart_y += 80;	
		}
		barsize_x = 70;
		barsize_y = 55;
		barstart_x = 85;
		barstart_y = 50;


		CPen pShow(PS_SOLID,1,RGB(85,110,165));

		CPen pShowLine(PS_SOLID,1,RGB(251, 36,36));//
		pOldPen=NULL;
		pOldPen=pDC->SelectObject(&pShow);

		int gotooneEnergeSpectrum[64];
		int maxEnergeSpectrum=0;
		int Num=0;
		char textout[10];

		m_iEnergeSpectCurBlockNo = 0;  //ֻ�ǲ�����һ��Block

		barstart_y = 50;
		for(int Num_y=0;Num_y<8;Num_y++) {
			barstart_x = 85;
			for(int Num_x=0;Num_x<8;Num_x++ ) {
				maxEnergeSpectrum=0;
				for(i=0;i<63;i++) { //64
					if(m_iEnergeSpectrum64[0][Num][i] > maxEnergeSpectrum)  maxEnergeSpectrum = m_iEnergeSpectrum64[0][Num][i];
				}
				if (maxEnergeSpectrum<1) maxEnergeSpectrum = 1;		
				for(i=0;i<64;i++) {
					gotooneEnergeSpectrum[i] =  (50 * (float)m_iEnergeSpectrum64[0][Num][i]) / maxEnergeSpectrum;
					if(gotooneEnergeSpectrum[i] > 54) gotooneEnergeSpectrum[i] = 54;
				}
				
				pDC->SetBkColor(RGB(219,206,198));
				sprintf(textout,"%.3d",maxEnergeSpectrum);
				pDC->TextOut(barstart_x-25,barstart_y,textout);

				pDC->SetBkColor(RGB(225,121,113));///////akang
				sprintf(textout,"(%.2d)",Num);
				pDC->TextOut(barstart_x+25,barstart_y-17,textout);
				for(i=0;i<63;i++) {
					pDC->MoveTo(barstart_x+i,  barstart_y+54 - gotooneEnergeSpectrum[i]);  //55
					pDC->LineTo(barstart_x+1+i,barstart_y+54 - gotooneEnergeSpectrum[i+1]);	 //55				
				}
				
			//�� �����߽�
				pOldPen=pDC->SelectObject(&pShowLine);
				pDC->MoveTo(barstart_x+m_iEnergeSpectEth_Low[m_iEnergeSpectCurBlockNo][Num], barstart_y+54 );
				pDC->LineTo(barstart_x+m_iEnergeSpectEth_Low[m_iEnergeSpectCurBlockNo][Num],barstart_y+54 - 30);
				
				pDC->MoveTo(barstart_x+m_iEnergeSpectEth_Hight[m_iEnergeSpectCurBlockNo][Num], barstart_y+54 );
				pDC->LineTo(barstart_x+m_iEnergeSpectEth_Hight[m_iEnergeSpectCurBlockNo][Num],barstart_y+54 - 30);

				pOldPen=pDC->SelectObject(&pShow);
				//

				barstart_x += 100;
				Num++;


			}
			barstart_y += 80;	
		}

		ReleaseDC(pDC);	
}



void CEnergeSpectEth::OnDrawZoomInEnergeSpect(int nblock,int num)
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
		CPen pShow5(PS_SOLID,2,RGB(219,206,198));//212,208,200
	    CPen* pOldPen=NULL;
	    pOldPen=pDC->SelectObject(&pShow5);

		CBrush bShow;			
		bShow.CreateSolidBrush(RGB(219,206,198));// 				 					  
		CBrush* pOldBrush=pDC->SelectObject(&bShow);//
		pDC->Rectangle(40,32,910,692);// 
					
		pDC->SelectObject(pOldBrush);//++++
		bShow.DeleteObject();	


		CPen pShow(PS_SOLID,2,RGB(202,95,204));//RGB(200,200,0)
//		CPen* pOldPen=NULL;
		pOldPen=pDC->SelectObject(&pShow);

		char chtext[50];
		sprintf(chtext,"�� %d ��� %d �� ��������ͼ", nblock,num);
		pDC->TextOut(270,g_draw_size*64+g_draw_startpoint_x+5+10,chtext);

//		CEdit* pShowEnergeOption=(CEdit*)GetDlgItem(IDC_SHOWENERGE_OPTION);
//		pShowEnergeOption->SetWindowText(chtext);
		

		pDC->MoveTo(g_draw_startpoint_y,g_draw_startpoint_x); 
		pDC->LineTo(g_draw_startpoint_y,g_draw_size*64+g_draw_startpoint_x);
		pDC->MoveTo(g_draw_startpoint_y,g_draw_startpoint_x);
		pDC->LineTo(g_draw_startpoint_y-3,g_draw_startpoint_x+5);

	    pDC->MoveTo(g_draw_startpoint_y,g_draw_size*64+g_draw_startpoint_x);
		pDC->LineTo(g_draw_startpoint_y +g_draw_size*64 ,g_draw_size*64+g_draw_startpoint_x);
	    pDC->MoveTo(g_draw_startpoint_y +g_draw_size*64,g_draw_size*64+g_draw_startpoint_x);
        pDC->LineTo(g_draw_startpoint_y +g_draw_size*64 - 5,g_draw_size*64+g_draw_startpoint_x +3);

		CString strshowMax;
	    pDC->SetTextColor(RGB(58,110,165));
		pDC->SetBkColor(RGB(219,206,198));
	    pDC->TextOut(40,40,"��ֵ");
		strshowMax.Format("%d ",maxtemp);
//		pDC->SetTextColor(RGB(255,128,128));
	    pDC->TextOut(40,55,strshowMax);

	    pDC->TextOut(40+30,g_draw_size*64+g_draw_startpoint_x+5,"(0,0)");
	    pDC->TextOut(g_draw_startpoint_y +g_draw_size*64 - 10,g_draw_size*64+g_draw_startpoint_x +5,"63");


		int x = g_draw_startpoint_y;
		CPen pShow2(PS_SOLID,2,RGB(85,110,165));// 
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


		CPen pShow3(PS_SOLID,2,RGB(255,10,,0));
	    pOldPen=pDC->SelectObject(&pShow3);

		if(m_iEnergeSpectCurSynNo > 63) m_iEnergeSpectCurSynNo = 63;
		if(m_iEnergeSpectCurSynNo < 0) m_iEnergeSpectCurSynNo = 0;

		pDC->MoveTo(g_draw_startpoint_y + g_draw_size*m_iEnergeSpectEth_Low[m_iEnergeSpectCurBlockNo][m_iEnergeSpectCurSynNo], g_draw_size*64+g_draw_startpoint_x);
		pDC->LineTo(g_draw_startpoint_y + g_draw_size*m_iEnergeSpectEth_Low[m_iEnergeSpectCurBlockNo][m_iEnergeSpectCurSynNo], 400);
		pDC->MoveTo(g_draw_startpoint_y +g_draw_size*m_iEnergeSpectEth_Hight[m_iEnergeSpectCurBlockNo][m_iEnergeSpectCurSynNo], g_draw_size*64+g_draw_startpoint_x);
		pDC->LineTo(g_draw_startpoint_y +g_draw_size*m_iEnergeSpectEth_Hight[m_iEnergeSpectCurBlockNo][m_iEnergeSpectCurSynNo], 400);

		ReleaseDC(pDC);				

}


void CEnergeSpectEth::OnLoadEthFile()
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
//	if(filelenth != (128*4 +4)*1024 ) {
//		AfxMessageBox("�ļ����Ȳ��ԣ�");
//		return;
//	}
	unsigned char chartemp[128];
	file.Seek(FILEHEAD_ELIMIT_ATTR_ADDR,CFile::begin );	
	file.Read(chartemp,1);
//	if(chartemp[0] != 0x01) {
//		AfxMessageBox("�ļ������ױ߽�������Ч");
//		return;
//	}

	file.Seek(PARA_ETHLIMIT_DATA_ADDR + fileoper_iblocksel * BLOCK_ETHLIMIT_DATA_SIZE,CFile::begin );	

	for(int i=0;i<64;i++) {
		file.Read(chartemp,2);
		m_iEnergeSpectEth_Low[m_iEnergeSpectCurBlockNo][i] = chartemp[0];
		m_iEnergeSpectEth_Hight[m_iEnergeSpectCurBlockNo][i] = chartemp[1];	
	}
	file.Close();

}

void CEnergeSpectEth::OnSaveEthFile()
{
	CFileSave filesave;
	filesave.bNewfilepackage = fileoper_Newfile;
	filesave.iBlockSel = fileoper_iblocksel;
	filesave.bEnReadOperate = false;
	if( IDOK != filesave.DoModal()) {
		return;
	}
	fileoper_Newfile = filesave.bNewfilepackage;
	fileoper_iblocksel = filesave.iBlockSel;

	CString FilePathName; 
	CFile savefile;
	if(fileoper_Newfile) {
		CFileDialog dlg(FALSE,"dat","*.dat");///TRUEΪOPEN�Ի���FALSEΪSAVE AS�Ի��� 
		dlg.m_ofn.lpstrFilter = ("�������ױ߽��ļ�LimitFile ""*.dat");
		if(dlg.DoModal()==IDOK) FilePathName=dlg.GetPathName(); 
		else return;
		if( !savefile.Open( FilePathName, CFile::modeCreate | CFile::modeWrite) ) {			
		}
		savefile.SetLength( (128*4 + 4)*1024 );		
	} 
	else {
		CFileDialog dlg(TRUE,"dat","*.dat");///TRUEΪOPEN�Ի���FALSEΪSAVE AS�Ի���
		dlg.m_ofn.lpstrFilter = ("�����ױ߽��ļ�LimitFile ""*.dat");
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
	int filelenth = savefile.GetLength();
//	if(filelenth != (128*4 +4)*1024 ) {
//		AfxMessageBox("�ļ����Ȳ��ԣ����治�ɹ���");
//		return;
//	}

	filehead[0] = 0x01;
	savefile.Seek(FILEHEAD_ELIMIT_ATTR_ADDR,CFile::begin );	
	savefile.Write(filehead,1);

	savefile.Seek(PARA_ETHLIMIT_DATA_ADDR+ BLOCK_ETHLIMIT_DATA_SIZE*fileoper_iblocksel,CFile::begin);

	unsigned char temp[4]={0,0,0,0};
	for(int i=0;i<64;i++) {	
		temp[0] = (unsigned char)m_iEnergeSpectEth_Low[m_iEnergeSpectCurBlockNo][i];
		temp[1] = (unsigned char)m_iEnergeSpectEth_Hight[m_iEnergeSpectCurBlockNo][i];
		savefile.Write(temp,2);
		
	}
	savefile.Close();
	
}

void CEnergeSpectEth::OnFastCalcEthLowHighLimit()
{
/*	int maxEth64 = 10;
	int maxnum = 5;
	int persent30;
	BOOL lowflag = true;

	for(int num=0;num<64;num++) {
		maxnum = 5;
		maxEth64 = 10;
		lowflag = true;
		for(int i=0;i<64;i++) { //64
			if(m_iEnergeSpectrum64[m_iEnergeSpectCurBlockNo][num][i] > maxEth64)  {
				maxEth64 = m_iEnergeSpectrum64[m_iEnergeSpectCurBlockNo][num][i];
				persent30 = (int)(maxEth64*3/10);
				maxnum = i;
			}
		}
		if((maxnum >= 12)&&(maxnum <= 50)) {
			m_iEnergeSpectEth_Low[m_iEnergeSpectCurBlockNo][num] = maxnum - 12;
			m_iEnergeSpectEth_Hight[m_iEnergeSpectCurBlockNo][num] = maxnum + 11;

			for(int kk=3;kk<64;kk++) { //64
				if((m_iEnergeSpectrum64[m_iEnergeSpectCurBlockNo][num][kk] > m_iEnergeSpectrum64[m_iEnergeSpectCurBlockNo][num][maxnum] - persent30)&&
				  (kk < maxnum-5)&&(lowflag))
				{
					lowflag = false;
					if(kk>8) {
						m_iEnergeSpectEth_Low[m_iEnergeSpectCurBlockNo][num] = kk - 8;						
					}
					else m_iEnergeSpectEth_Low[m_iEnergeSpectCurBlockNo][num] = 3;
				}
				if((m_iEnergeSpectrum64[m_iEnergeSpectCurBlockNo][num][kk] > m_iEnergeSpectrum64[m_iEnergeSpectCurBlockNo][num][maxnum] - persent30)&&
				  (kk > maxnum+7))
				{
					if(kk<52) {
						m_iEnergeSpectEth_Hight[m_iEnergeSpectCurBlockNo][num] = kk + 4;						
					}
					else m_iEnergeSpectEth_Hight[m_iEnergeSpectCurBlockNo][num] = 52;
				}
			}			
		}
		else {
			if(maxnum < 12) {
				m_iEnergeSpectEth_Low[m_iEnergeSpectCurBlockNo][num] = 5;
				m_iEnergeSpectEth_Hight[m_iEnergeSpectCurBlockNo][num] = 32;
			};
			if(maxnum > 50) { 
				m_iEnergeSpectEth_Low[m_iEnergeSpectCurBlockNo][num] = 30;
				m_iEnergeSpectEth_Hight[m_iEnergeSpectCurBlockNo][num] = 55; 
			};
		}
	}
*/  // ����Ϊ��ǰ�� ���ټ������ױ߽��㷨


	int maxEth64 = 10;
	int maxnum = 5;
	int maxpeakvalue = 10;
	int maxpeaknum = 5;

	int persent50;
	BOOL lowflag = true;
	int filter3dot[64];

	int m_i64EthPara[6][3]; //[5]:��ʾ�������ͣ�[3]: ��ֵ ���� ��
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

	m_i64EthPara[5][2] = 0;// �Ƿ񰴰ٷֱȼ���


//	for(int block=0;block<8;block++) {  
		for(int num=0;num<64;num++) {    // ���� 64 ������
			maxnum = 5;
			maxpeaknum = 5;
			maxEth64 = 10;
			lowflag = true;
			//����ƽ��
			filter3dot[0] = (m_iEnergeSpectrum64[m_iEnergeSpectCurBlockNo][num][0] + m_iEnergeSpectrum64[m_iEnergeSpectCurBlockNo][num][1])/2;
			filter3dot[63] = (m_iEnergeSpectrum64[m_iEnergeSpectCurBlockNo][num][62] + m_iEnergeSpectrum64[m_iEnergeSpectCurBlockNo][num][63])/2;
			for(int kkkk=1; kkkk<63; kkkk++) {
				filter3dot[kkkk] = (m_iEnergeSpectrum64[m_iEnergeSpectCurBlockNo][num][kkkk-1] + m_iEnergeSpectrum64[m_iEnergeSpectCurBlockNo][num][kkkk] 
					+ m_iEnergeSpectrum64[m_iEnergeSpectCurBlockNo][num][kkkk+1])/3;
			}

			// �ҷ�λ�� ���ֵ 
			maxEth64 = 10;
			for(int i=0;i<64;i++) { //64
				if(filter3dot[i] > maxEth64)  {
					maxEth64 = filter3dot[i];
					persent50 = (int)(maxEth64*5/10);
					maxnum = i;
				}
			}

			if((m_iEnergeSpectCurBlockNo == 2)&&(num == 6)) {
				i = 61;
			}
   
			//�ҷ�ֵ ��λ  ����ֵ
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


			//�� ��������Ž��� ���໷��
			int xx = num % 8,yy = num/8;
			int ring = 0;
			//һ��
			if( ((xx >= 3)&&(xx <= 4)) && ((yy<=4)&&(yy >= 3)) ) {
				ring = 0;
			}

			//2��
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

			//����
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
			
			//�Ļ�
			if((xx == 0) ||(xx == 7)){
				ring = 3;
			}
			if((yy == 0) || (yy == 7)) {
				ring = 3;
			}

			//�ı߽�
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


			// ���շ�λ �İٷֱ��� ȷ������ ���±߽� ���£�32����  �ϣ�27����
			
			if(m_i64EthPara[5][2] == 1) {	//m_i64EthPara[5][2] ���� �ٷֱȼ���
				if(maxnum >= 60) {
					m_iEnergeSpectEth_Low[m_iEnergeSpectCurBlockNo][num] = ( (float)maxnum * (1 - 0.32) );     //
					m_iEnergeSpectEth_Hight[m_iEnergeSpectCurBlockNo][num] = 62; 
				}
				if(maxnum <= 5) {
					m_iEnergeSpectEth_Low[m_iEnergeSpectCurBlockNo][num] =  5;    //
					m_iEnergeSpectEth_Hight[m_iEnergeSpectCurBlockNo][num] = ( (float)maxnum * (1 + 0.27) + 0.5); 
				}

				if((maxnum > 5)&&( maxnum <60)) {
					m_iEnergeSpectEth_Low[m_iEnergeSpectCurBlockNo][num] =  ( (float)maxnum * (1 - 0.32) ); 
					m_iEnergeSpectEth_Hight[m_iEnergeSpectCurBlockNo][num] = ( (float)maxnum * (1 + 0.27) + 0.5); 
				}
				if(m_iEnergeSpectEth_Hight[m_iEnergeSpectCurBlockNo][num] > 62) {
					m_iEnergeSpectEth_Hight[m_iEnergeSpectCurBlockNo][num] = 62;
				}
			}
			else {	//  ���� ���� �̶���� ������������������ 				 	
				if((maxnum >= m_i64EthPara[ring][0] + 5)&&(maxnum <= 61 - m_i64EthPara[ring][1])) {
					m_iEnergeSpectEth_Low[m_iEnergeSpectCurBlockNo][num] = maxnum - m_i64EthPara[ring][0];     //
					m_iEnergeSpectEth_Hight[m_iEnergeSpectCurBlockNo][num] = maxnum + m_i64EthPara[ring][1];    //
				}
				else {
					if(maxnum < m_i64EthPara[ring][0] + 5) {
						m_iEnergeSpectEth_Low[m_iEnergeSpectCurBlockNo][num] = 5;
						m_iEnergeSpectEth_Hight[m_iEnergeSpectCurBlockNo][num] = maxnum + m_i64EthPara[ring][1];
						if (m_iEnergeSpectEth_Hight[m_iEnergeSpectCurBlockNo][num] >= 62) {
							m_iEnergeSpectEth_Hight[m_iEnergeSpectCurBlockNo][num] = 62;
						}
					};
					if(maxnum > 61 - m_i64EthPara[ring][1]) { 
						m_iEnergeSpectEth_Low[m_iEnergeSpectCurBlockNo][num] = maxnum - m_i64EthPara[ring][0];
						m_iEnergeSpectEth_Hight[m_iEnergeSpectCurBlockNo][num] = 62; 

						if (m_iEnergeSpectEth_Low[m_iEnergeSpectCurBlockNo][num] <= 5) {
							m_iEnergeSpectEth_Low[m_iEnergeSpectCurBlockNo][num] = 5;
						}
					};
				}
			} // end if 

		} //64 ���������� end  

		// ����������׵� ���� �����ó� ��ֵ�� ���ж�ֵ��������
		if(m_i64EthPara[0][2] == 1) {//��һ	 ��ֵ����		
			m_iEnergeSpectEth_Low[m_iEnergeSpectCurBlockNo][27] = m_i64EthPara[0][0];
			m_iEnergeSpectEth_Hight[m_iEnergeSpectCurBlockNo][27] = m_i64EthPara[0][1]; 
			m_iEnergeSpectEth_Low[m_iEnergeSpectCurBlockNo][28] = m_i64EthPara[0][0];
			m_iEnergeSpectEth_Hight[m_iEnergeSpectCurBlockNo][28] = m_i64EthPara[0][1]; 
			m_iEnergeSpectEth_Low[m_iEnergeSpectCurBlockNo][35] = m_i64EthPara[0][0];
			m_iEnergeSpectEth_Hight[m_iEnergeSpectCurBlockNo][35] = m_i64EthPara[0][1]; 
			m_iEnergeSpectEth_Low[m_iEnergeSpectCurBlockNo][36] = m_i64EthPara[0][0];
			m_iEnergeSpectEth_Hight[m_iEnergeSpectCurBlockNo][36] = m_i64EthPara[0][1]; 
		}

		if(m_i64EthPara[1][2] == 1) {//����  ��ֵ����
			for(int ii=2;ii<=5;ii++) {
				m_iEnergeSpectEth_Low[m_iEnergeSpectCurBlockNo][2+ ii * 8] = m_i64EthPara[1][0];
				m_iEnergeSpectEth_Hight[m_iEnergeSpectCurBlockNo][2+ ii * 8] = m_i64EthPara[1][1]; 
			}

			for( ii=2;ii<=5;ii++) {
				m_iEnergeSpectEth_Low[m_iEnergeSpectCurBlockNo][5+ ii * 8] = m_i64EthPara[1][0];
				m_iEnergeSpectEth_Hight[m_iEnergeSpectCurBlockNo][5+ ii * 8] = m_i64EthPara[1][1]; 
			}

			for( ii=2;ii<=5;ii++) {
				m_iEnergeSpectEth_Low[m_iEnergeSpectCurBlockNo][ii+ 2 * 8] = m_i64EthPara[1][0];
				m_iEnergeSpectEth_Hight[m_iEnergeSpectCurBlockNo][ii+ 2 * 8] = m_i64EthPara[1][1]; 
			}

			for( ii=2;ii<=5;ii++) {
				m_iEnergeSpectEth_Low[m_iEnergeSpectCurBlockNo][ii+ 5 * 8] = m_i64EthPara[1][0];
				m_iEnergeSpectEth_Hight[m_iEnergeSpectCurBlockNo][ii+ 5 * 8] = m_i64EthPara[1][1]; 
			}
		}

		if(m_i64EthPara[2][2] == 1) {//��3  ��ֵ����
			for(int ii=1;ii<=6;ii++) {
				m_iEnergeSpectEth_Low[m_iEnergeSpectCurBlockNo][1+ ii * 8] = m_i64EthPara[2][0];
				m_iEnergeSpectEth_Hight[m_iEnergeSpectCurBlockNo][1+ ii * 8] = m_i64EthPara[2][1]; 
			}
			for( ii=1;ii<=6;ii++) {
				m_iEnergeSpectEth_Low[m_iEnergeSpectCurBlockNo][6+ ii * 8] = m_i64EthPara[2][0];
				m_iEnergeSpectEth_Hight[m_iEnergeSpectCurBlockNo][6+ ii * 8] = m_i64EthPara[2][1]; 
			}

			for( ii=1;ii<=6;ii++) {
				m_iEnergeSpectEth_Low[m_iEnergeSpectCurBlockNo][ii+ 1 * 8] = m_i64EthPara[2][0];
				m_iEnergeSpectEth_Hight[m_iEnergeSpectCurBlockNo][ii+ 1 * 8] = m_i64EthPara[2][1]; 
			}
			for( ii=1;ii<=6;ii++) {
				m_iEnergeSpectEth_Low[m_iEnergeSpectCurBlockNo][ii + 6 * 8] = m_i64EthPara[2][0];
				m_iEnergeSpectEth_Hight[m_iEnergeSpectCurBlockNo][ii+ 6 * 8] = m_i64EthPara[2][1]; 
			}
		}

		if(m_i64EthPara[3][2] == 1) {//��4  ��ֵ����
			for(int ii=1;ii<=6;ii++) {
				m_iEnergeSpectEth_Low[m_iEnergeSpectCurBlockNo][0+ ii * 8] = m_i64EthPara[3][0];
				m_iEnergeSpectEth_Hight[m_iEnergeSpectCurBlockNo][0+ ii * 8] = m_i64EthPara[3][1]; 
			}
			for( ii=1;ii<=6;ii++) {
				m_iEnergeSpectEth_Low[m_iEnergeSpectCurBlockNo][7+ ii * 8] = m_i64EthPara[3][0];
				m_iEnergeSpectEth_Hight[m_iEnergeSpectCurBlockNo][7+ ii * 8] = m_i64EthPara[3][1]; 
			}

			for( ii=1;ii<=6;ii++) {
				m_iEnergeSpectEth_Low[m_iEnergeSpectCurBlockNo][ii+ 0 * 8] = m_i64EthPara[3][0];
				m_iEnergeSpectEth_Hight[m_iEnergeSpectCurBlockNo][ii+ 0 * 8] = m_i64EthPara[3][1]; 
			}
			for( ii=1;ii<=6;ii++) {
				m_iEnergeSpectEth_Low[m_iEnergeSpectCurBlockNo][ii + 7 * 8] = m_i64EthPara[3][0];
				m_iEnergeSpectEth_Hight[m_iEnergeSpectCurBlockNo][ii+ 7 * 8] = m_i64EthPara[3][1]; 
			}
		}

		if(m_i64EthPara[4][2] == 1) {//�ı߽�  ��ֵ����
			m_iEnergeSpectEth_Low[m_iEnergeSpectCurBlockNo][0] = m_i64EthPara[4][0];
			m_iEnergeSpectEth_Hight[m_iEnergeSpectCurBlockNo][0] = m_i64EthPara[4][1]; 

			m_iEnergeSpectEth_Low[m_iEnergeSpectCurBlockNo][7] = m_i64EthPara[4][0];
			m_iEnergeSpectEth_Hight[m_iEnergeSpectCurBlockNo][7] = m_i64EthPara[4][1]; 

			m_iEnergeSpectEth_Low[m_iEnergeSpectCurBlockNo][56] = m_i64EthPara[4][0];
			m_iEnergeSpectEth_Hight[m_iEnergeSpectCurBlockNo][56] = m_i64EthPara[4][1]; 

			m_iEnergeSpectEth_Low[m_iEnergeSpectCurBlockNo][63] = m_i64EthPara[4][0];
			m_iEnergeSpectEth_Hight[m_iEnergeSpectCurBlockNo][63] = m_i64EthPara[4][1]; 
		}
	
	
//	} // ���һ�� Bucket �����ױ߽����














	//��ʾ64�����ף�����������������������������������������������������
	m_bEnergeSpectOperate_En = true;

	OnDraw64EnergeSpectrum();
	m_bEnergeSpectSelEn = true;
	m_bEnergeSpectZoomInMode =false;

}
