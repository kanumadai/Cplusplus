// CHORoad.cpp : implementation file
//

#include "stdafx.h"
#include "robot.h"
#include "CHORoad.h"
#include "RobotDlg.h"
#include "robotDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCHORoad dialog
CCHORoad::CCHORoad(CWnd* pParent /*=NULL*/)
	: CDialog(CCHORoad::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCHORoad)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
//	ChoStation();
//	Chooseflag     = 0;
}


void CCHORoad::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCHORoad)
		// NOTE: the ClassWizard will add DDX and DDV calls here
//	DDX_Text(pDX, IDC_DeviceNub, CTPETMRI);
//	DDX_Text(pDX, IDC_StepNub, m_Turnstate);
//	DDX_Text(pDX, IDC_HoldUp, EHoldUppostion);
//	DDX_Text(pDX, IDC_HoldDown, EHoldDownpostion);
//	CTPETMRI=10;
	//}}AFX_DATA_MAP 
}


BEGIN_MESSAGE_MAP(CCHORoad, CDialog)
	//{{AFX_MSG_MAP(CCHORoad)
	ON_BN_CLICKED(IDC_0_CT, On0_CT)
	ON_BN_CLICKED(IDC_0_PET, On0_PET)
	ON_BN_CLICKED(IDC_CT_0, OnCT_0)
	ON_BN_CLICKED(IDC_CT_PET, OnCT_PET)
	ON_BN_CLICKED(IDC_MRI_PET, OnMRI_PET)
	ON_BN_CLICKED(IDC_PET_0, OnPET_0)
	ON_BN_CLICKED(IDC_PET_CT, OnPET_CT)
	ON_BN_CLICKED(IDC_PET_MRI, OnPET_MRI)
	ON_BN_CLICKED(IDC_CANCELLIM, OnCancellim)
	ON_BN_CLICKED(IDC_PREVIEW, OnPreview)
	ON_BN_CLICKED(IDC_NEXT, OnNext)
	ON_BN_CLICKED(IDC_ReSetState, OnReSetState)
	ON_BN_CLICKED(IDC_DirectHold, OnDirectHold)
	ON_BN_CLICKED(IDC_DirectMove, OnDirectMove)
	ON_BN_CLICKED(IDC_DirectMove2, OnDirectMove2)
//	ON_BN_CLICKED(IDC_DirectHold2, OnDirectHold2)
//	ON_BN_CLICKED(IDC_DirectMove3, OnDirectMove3)
//	ON_BN_CLICKED(IDC_DirectMove4, OnDirectMove4)
//	ON_BN_CLICKED(IDC_MRITurning90, OnMRITurning90)
	ON_BN_CLICKED(IDC_DirectHold3, OnDirectHoldCT)
	ON_BN_CLICKED(IDC_DirectMove5, OnDirectMoveCT)
	ON_BN_CLICKED(IDC_DirectMove6, OnDirectMove6CT)
//	ON_BN_CLICKED(IDC_DirectMove7, OnDirectMovePETMRI)
//	ON_BN_CLICKED(IDC_MRITurning91, OnPETTurning91)
//	ON_BN_CLICKED(IDC_DirectHold4, OnDirectHoldPETMRI1)
//	ON_BN_CLICKED(IDC_DirectMove10, OnDirectMovePETMRI4)
//	ON_BN_CLICKED(IDC_DirectHold5, OnDirectHoldMRIPET)
//	ON_BN_CLICKED(IDC_DirectMove11, OnDirectMoveMRIPET2)
//	ON_BN_CLICKED(IDC_DirectMove8, OnDirectMoveMRIPET4)
	ON_COMMAND(IDC_ReadHold, OnReadHold)
	ON_BN_CLICKED(IDC_NextLimited, OnNextLimited)
	ON_BN_CLICKED(IDC_ProLimited, OnProLimited)
//	ON_BN_CLICKED(IDC_MRITurning92, OnMRITurning92)
//	ON_BN_CLICKED(IDC_MRITurning93, OnMRITurning93)
//	ON_BN_CLICKED(IDC_MRITurning94, OnMRITurning94)
//	ON_BN_CLICKED(IDC_MRITurning95, OnMRITurning95)
	ON_BN_CLICKED(IDC_NextLimited2, OnNextLimited2)
	ON_BN_CLICKED(IDC_ProLimitedwwd, OnProLimited2)	
	ON_WM_TIMER()

	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
//ChoStation();
/////////////////////////////////////////////////////////////////////////////
// CCHORoad message handlers

void CCHORoad::On0_CT() 
{
	// TODO: Add your control notification handler code here
	if(IDYES==MessageBox("ȷ��Ҫ��CT��?","���ٴ�ȷ��",MB_ICONQUESTION|MB_YESNO))
	{  
       Chooseflag=1;
	   CTPETMRI = 1;
	   
	   CDialog::OnOK();    
	}
}
void CCHORoad::On0_PET() 
{
	// TODO: Add your control notification handler code here
	if(IDYES==MessageBox("ȷ��Ҫ��PEI��?","���ٴ�ȷ��",MB_ICONQUESTION|MB_YESNO))
	{  
       Chooseflag=2;
	   CTPETMRI = 2;
	   CDialog::OnOK();
	}	
}

void CCHORoad::On0_MRI() 
{
	// TODO: Add your control notification handler code here
	if(IDYES==MessageBox("ȷ��Ҫ��MRI��?","���ٴ�ȷ��",MB_ICONQUESTION|MB_YESNO))
	{  
       Chooseflag=3;
	   CTPETMRI = 3;
	   CDialog::OnOK();
	}	
}



void CCHORoad::OnCT_0() 
{
	// TODO: Add your control notification handler code here
	if(IDYES==MessageBox("ȷ��Ҫ�����?","���ٴ�ȷ��",MB_ICONQUESTION|MB_YESNO))
	{  
       Chooseflag=4;
	   CTPETMRI = 0;
	   CDialog::OnOK();

	}
}
//CT-PET
void CCHORoad::OnCT_PET() 
{
	// TODO: Add your control notification handler code here
	if(IDYES==MessageBox("ȷ��Ҫ��PET��?","���ٴ�ȷ��",MB_ICONQUESTION|MB_YESNO))
	{  
       Chooseflag=5;
	   CTPETMRI = 2;
	   CDialog::OnOK();
	}	
}
void CCHORoad::OnCT_MRI() 
{
	// TODO: Add your control notification handler code here
	if(IDYES==MessageBox("ȷ��Ҫ��MRI��?","���ٴ�ȷ��",MB_ICONQUESTION|MB_YESNO))
	{  
       Chooseflag=6;
	   CTPETMRI = 3;
	   CDialog::OnOK();
	}	
}



void CCHORoad::OnPET_0() 
{
	// TODO: Add your control notification handler code here
	if(IDYES==MessageBox("ȷ��Ҫ�����?","���ٴ�ȷ��",MB_ICONQUESTION|MB_YESNO))
	{  
       Chooseflag=7;
	   CTPETMRI = 0;
	   CDialog::OnOK();
	}
}
//PET-CT
void CCHORoad::OnPET_CT() 
{
	// TODO: Add your control notification handler code here
	if(IDYES==MessageBox("ȷ��Ҫ��CT��?","���ٴ�ȷ��",MB_ICONQUESTION|MB_YESNO))
	{  
       Chooseflag=8;
	   CTPETMRI = 1;
	   CDialog::OnOK();
	}
}

void CCHORoad::OnPET_MRI() 
{
	// TODO: Add your control notification handler code here
	if(IDYES==MessageBox("ȷ��Ҫ��MRI��?","���ٴ�ȷ��",MB_ICONQUESTION|MB_YESNO))
	{  
       Chooseflag=9;
	   CTPETMRI = 3;
	   CDialog::OnOK();
	}
		
}
void CCHORoad::OnMRI_0() 
{
	// TODO: Add your control notification handler code here
	if(IDYES==MessageBox("ȷ��Ҫ�����?","���ٴ�ȷ��",MB_ICONQUESTION|MB_YESNO))
	{  
       Chooseflag=10;
	   CTPETMRI = 0;
	   CDialog::OnOK();
	}	
}

void CCHORoad::OnMRI_CT() 
{
	// TODO: Add your control notification handler code here
	if(IDYES==MessageBox("ȷ��Ҫ��CT��?","���ٴ�ȷ��",MB_ICONQUESTION|MB_YESNO))
	{  
       Chooseflag=11;
	   CTPETMRI = 1;
	   CDialog::OnOK();
	}	
}

void CCHORoad::OnMRI_PET() 
{
	// TODO: Add your control notification handler code here
	if(IDYES==MessageBox("ȷ��Ҫ��PET��?","���ٴ�ȷ��",MB_ICONQUESTION|MB_YESNO))
	{  
       Chooseflag=12;
	   CTPETMRI = 2;
	   CDialog::OnOK();
	}
}



void CCHORoad::ChoStation()
{
	UpdateData(true);
	if (m_Turnstate ==10)
	{
		switch(CTPETMRI)
		 {
			case 0:
				SetDlgItemText(IDC_SStaion,_T("���������"));
				GetDlgItem(IDC_CT_0)->EnableWindow(false);
				GetDlgItem(IDC_PET_0)->EnableWindow(false);
				GetDlgItem(IDC_MRI_0)->EnableWindow(false);
				GetDlgItem(IDC_CT_PET)->EnableWindow(false);
				GetDlgItem(IDC_CT_MRI)->EnableWindow(false);
				GetDlgItem(IDC_PET_MRI)->EnableWindow(false);
				GetDlgItem(IDC_PET_CT)->EnableWindow(false);
				GetDlgItem(IDC_MRI_CT)->EnableWindow(false);
				GetDlgItem(IDC_MRI_PET)->EnableWindow(false);			
				break;
			case 1:
				SetDlgItemText(IDC_SStaion,_T("������CT"));
				GetDlgItem(IDC_0_CT)->EnableWindow(false);
				GetDlgItem(IDC_0_PET)->EnableWindow(false);
				GetDlgItem(IDC_0_MRI)->EnableWindow(false);
				GetDlgItem(IDC_PET_0)->EnableWindow(false);
				GetDlgItem(IDC_MRI_0)->EnableWindow(false);
				GetDlgItem(IDC_PET_MRI)->EnableWindow(false);
				GetDlgItem(IDC_PET_CT)->EnableWindow(false);
				GetDlgItem(IDC_MRI_CT)->EnableWindow(false);
				GetDlgItem(IDC_MRI_PET)->EnableWindow(false);
				break;
			case 2:
				SetDlgItemText(IDC_SStaion,_T("������PET"));
				GetDlgItem(IDC_0_CT)->EnableWindow(false);
				GetDlgItem(IDC_0_PET)->EnableWindow(false);
				GetDlgItem(IDC_0_MRI)->EnableWindow(false);
				GetDlgItem(IDC_CT_0)->EnableWindow(false);
				GetDlgItem(IDC_MRI_0)->EnableWindow(false);
				GetDlgItem(IDC_CT_PET)->EnableWindow(false);
				GetDlgItem(IDC_CT_MRI)->EnableWindow(false);
				GetDlgItem(IDC_MRI_CT)->EnableWindow(false);
				GetDlgItem(IDC_MRI_PET)->EnableWindow(false);
				break;
			case 3:
				SetDlgItemText(IDC_SStaion,_T("������������MRI"));
				GetDlgItem(IDC_0_CT)->EnableWindow(false);
				GetDlgItem(IDC_0_PET)->EnableWindow(false);
				GetDlgItem(IDC_0_MRI)->EnableWindow(false);
				GetDlgItem(IDC_CT_0)->EnableWindow(false);
				GetDlgItem(IDC_PET_0)->EnableWindow(false);
				GetDlgItem(IDC_CT_PET)->EnableWindow(false);
				GetDlgItem(IDC_CT_MRI)->EnableWindow(false);
				GetDlgItem(IDC_PET_MRI)->EnableWindow(false);
				GetDlgItem(IDC_PET_CT)->EnableWindow(false);
				break;
			default:
				break;
		}
	}
	else
	{	
		switch(CTPETMRI)
		 {
			case 0:
				SetDlgItemText(IDC_SStaion,_T("���������������....."));		
				break;
			case 1:
				SetDlgItemText(IDC_SStaion,_T("������������CT....."));
		
				break;
			case 2:
				SetDlgItemText(IDC_SStaion,_T("������������PET....."));

				break;
			case 3:
				SetDlgItemText(IDC_SStaion,_T("������������MRI....."));

				break;
			default:
				break;
		}
		GetDlgItem(IDC_0_CT)->EnableWindow(false);
		GetDlgItem(IDC_0_PET)->EnableWindow(false);
		GetDlgItem(IDC_0_MRI)->EnableWindow(false);
		GetDlgItem(IDC_CT_0)->EnableWindow(false);
		GetDlgItem(IDC_PET_0)->EnableWindow(false);
		GetDlgItem(IDC_MRI_0)->EnableWindow(false);
		GetDlgItem(IDC_CT_PET)->EnableWindow(false);
		GetDlgItem(IDC_CT_MRI)->EnableWindow(false);
		GetDlgItem(IDC_PET_MRI)->EnableWindow(false);
		GetDlgItem(IDC_PET_CT)->EnableWindow(false);	
		GetDlgItem(IDC_MRI_CT)->EnableWindow(false);
		GetDlgItem(IDC_MRI_PET)->EnableWindow(false);
		
	}
	if (CTPETMRI==5)
	{
			GetDlgItem(IDC_0_CT)->EnableWindow(true);
			GetDlgItem(IDC_0_PET)->EnableWindow(true);
			GetDlgItem(IDC_0_MRI)->EnableWindow(true);
			GetDlgItem(IDC_CT_0)->EnableWindow(false);
			GetDlgItem(IDC_PET_0)->EnableWindow(false);
			GetDlgItem(IDC_MRI_0)->EnableWindow(false);
			GetDlgItem(IDC_CT_PET)->EnableWindow(false);
			GetDlgItem(IDC_CT_MRI)->EnableWindow(false);
			GetDlgItem(IDC_PET_MRI)->EnableWindow(false);
			GetDlgItem(IDC_PET_CT)->EnableWindow(false);	
			GetDlgItem(IDC_MRI_CT)->EnableWindow(false);
			GetDlgItem(IDC_MRI_PET)->EnableWindow(false);	
	}
}

void CCHORoad::OnCancellim() 
{
	// TODO: Add your control notification handler code here
		GetDlgItem(IDC_0_CT)->EnableWindow(true);
		GetDlgItem(IDC_0_PET)->EnableWindow(true);
		GetDlgItem(IDC_0_MRI)->EnableWindow(true);
		GetDlgItem(IDC_CT_0)->EnableWindow(true);
		GetDlgItem(IDC_PET_0)->EnableWindow(true);
		GetDlgItem(IDC_MRI_0)->EnableWindow(true);
		GetDlgItem(IDC_CT_PET)->EnableWindow(true);
		GetDlgItem(IDC_CT_MRI)->EnableWindow(true);
		GetDlgItem(IDC_PET_MRI)->EnableWindow(true);
		GetDlgItem(IDC_PET_CT)->EnableWindow(true);	
		GetDlgItem(IDC_MRI_CT)->EnableWindow(true);
		GetDlgItem(IDC_MRI_PET)->EnableWindow(true);	
}

void CCHORoad::OnPreview() 
{
	UpdateData(false);
	// TODO: Add your control notification handler code here

	if (m_Turnstate<=0||m_Turnstate>=11)
	{
         
	}
	else
	{

		m_Turnstate--;
        UpdateData(false);
		return;
	}
		SetDlgItemText(IDC_SStaion,_T("�������в���....."));	
}

void CCHORoad::OnNext() 
{
	UpdateData(false);
	// TODO: Add your control notification handler code here
	if (m_Turnstate<0||m_Turnstate>=10)
	{
         
	}
	else
	{
		m_Turnstate++;	
		UpdateData(false);
		return;
	}	
    SetDlgItemText(IDC_SStaion,_T("�������в���....."));

}


void CCHORoad::OnReSetState() 
{
	// TODO: Add your control notification handler code here
	ChoStation();
}
void CCHORoad::OnDirectHold2() 
{
	// TODO: Add your control notification handler code here
	if(IDYES==MessageBox("ȷ��Ҫֱ��ȡMRI����?","���ٴ�ȷ��",MB_ICONQUESTION|MB_YESNO))
	{  
       Chooseflag=16;
	 //  CTPETMRI = 3;
	   CDialog::OnOK();
	}		
}

void CCHORoad::OnDirectMove3() 
{
	// TODO: Add your control notification handler code here
	if(IDYES==MessageBox("ȷ��Ҫֱ���뿪MRI?","���ٴ�ȷ��",MB_ICONQUESTION|MB_YESNO))
	{  
       Chooseflag=17;
	  //CTPETMRI = 3;
	   CDialog::OnOK();
	}		
}

void CCHORoad::OnDirectMove4() 
{
	// TODO: Add your control notification handler code here
	if(IDYES==MessageBox("ȷ��Ҫֱ�ӷ�MRI����?","���ٴ�ȷ��",MB_ICONQUESTION|MB_YESNO))
	{  
       Chooseflag=18;
	//   CTPETMRI = 3;
	   CDialog::OnOK();
	}		
}




//CTץȡ
void CCHORoad::OnDirectHoldCT() 
{
	// TODO: Add your control notification handler code here
	if(IDYES==MessageBox("ȷ��Ҫֱ��ȡCT����?","���ٴ�ȷ��",MB_ICONQUESTION|MB_YESNO))
	{  
       Chooseflag=20;
	  // CTPETMRI = 3;
	   CDialog::OnOK();
	}		
}
//CT�뿪
void CCHORoad::OnDirectMoveCT() //�뿪CT
{
	// TODO: Add your control notification handler code here
	if(IDYES==MessageBox("ȷ��Ҫֱ��ȡCT����?","���ٴ�ȷ��",MB_ICONQUESTION|MB_YESNO))
	{  
       Chooseflag=21;
	   //CTPETMRI = 3;
	   CDialog::OnOK();
	}		
}
//����CT����
void CCHORoad::OnDirectMove6CT() //����CT����
{
	// TODO: Add your control notification handler code here
	if(IDYES==MessageBox("ȷ��Ҫֱ��ȡCT����?","���ٴ�ȷ��",MB_ICONQUESTION|MB_YESNO))
	{  
       Chooseflag=22;
	  // CTPETMRI = 3;
	   CDialog::OnOK();
	}		
}
//ץȡPET����
void CCHORoad::OnDirectHold() 
{
	if(IDYES==MessageBox("ȷ��Ҫֱ��ȡ����?","���ٴ�ȷ��",MB_ICONQUESTION|MB_YESNO))
	{  
       Chooseflag=13;
	   CTPETMRI = 3;
	   CDialog::OnOK();
	}	
}
//�뿪PET
void CCHORoad::OnDirectMove() 
{
	// TODO: Add your control notification handler code here
	if(IDYES==MessageBox("ȷ��Ҫֱ���뿪?","���ٴ�ȷ��",MB_ICONQUESTION|MB_YESNO))
	{  
       Chooseflag=14;
	   CTPETMRI = 3;
	   CDialog::OnOK();
	}	
}
//����PET����
void CCHORoad::OnDirectMove2() 
{
	// TODO: Add your control notification handler code here
	if(IDYES==MessageBox("ȷ��Ҫֱ�ӷŴ���?","���ٴ�ȷ��",MB_ICONQUESTION|MB_YESNO))
	{  
       Chooseflag=15;
	//   CTPETMRI = 3;
	   CDialog::OnOK();
	}	
}





//PET/MRI(1)ץȡPET����
void CCHORoad::OnDirectHoldPETMRI1() //OnDirectHold() 
{
	// TODO: Add your control notification handler code here
	if(IDYES==MessageBox("PET-MRI?ȷ��ҪȡPET����?","���ٴ�ȷ��",MB_ICONQUESTION|MB_YESNO))
	{  
       Chooseflag=31;
	   CTPETMRI = 3;
	   CDialog::OnOK();
	}	
	
}
//PET/MRI(2)�뿪PET�����е�MRI
void CCHORoad::OnDirectMovePETMRI() 
{
	// TODO: Add your control notification handler code here
	if(IDYES==MessageBox("PET-MRI?ȷ��Ҫ�뿪PET��MRI?","���ٴ�ȷ��",MB_ICONQUESTION|MB_YESNO))
	{  
		if(IDYES==MessageBox("���ٴ�ȷ������ǰ��λ��?","���ٴ�ȷ��",MB_ICONQUESTION|MB_YESNO))
		{  
			   Chooseflag=23;
			  // CTPETMRI = 3;
			   CDialog::OnOK();
		}
	}	
}
//PET/MRI(3)��ͷ
 void CCHORoad::OnPETTurning91() 
{
	// TODO: Add your control notification handler code here
	if(IDYES==MessageBox("PET-MRI?ȷ��Ҫ��ת180��ͷ?","���ٴ�ȷ��",MB_ICONQUESTION|MB_YESNO))
	{  
       Chooseflag=24;
	  // CTPETMRI = 3;
	   CDialog::OnOK();
	}		
}
//PET/MRI(4)������ŵ�MRI��
void CCHORoad::OnDirectMovePETMRI4()// OnDirectMove4() 
{
	// TODO: Add your control notification handler code here
	if(IDYES==MessageBox("PET-MRI?ȷ������������?ȷ��Ҫ��MRI�Ŵ���?","���ٴ�ȷ��",MB_ICONQUESTION|MB_YESNO))
	{  
		if(IDYES==MessageBox("PET-MRI?ȷ������������??","���ٴ�ȷ��",MB_ICONQUESTION|MB_YESNO))
		{  
			   Chooseflag=18;
			//   CTPETMRI = 3;
			   CDialog::OnOK();
		}
	}	
}
//MRI/PET(1)ץȡMRI����
void CCHORoad::OnDirectHoldMRIPET() //OnDirectHold2() 
{
	// TODO: Add your control notification handler code here
	if(IDYES==MessageBox("MRI-PET?ȷ��ҪȡMRI����?","���ٴ�ȷ��",MB_ICONQUESTION|MB_YESNO))
	{  
       Chooseflag=16;
	 //  CTPETMRI = 3;
	   CDialog::OnOK();
	}	
}
//MRI/PET(2)�뿪MRI
void CCHORoad::OnDirectMoveMRIPET2() //OnDirectMove3() 
{
	// TODO: Add your control notification handler code here
	if(IDYES==MessageBox("MRI-PET?ȷ��Ҫֱ���뿪MRI?","���ٴ�ȷ��",MB_ICONQUESTION|MB_YESNO))
	{  
       Chooseflag=17;
	  //CTPETMRI = 3;
	   CDialog::OnOK();
	}	
}
//MRI/PET(3)תͷ
void CCHORoad::OnMRITurning90() 
{
	// TODO: Add your control notification handler code here
	if(IDYES==MessageBox("MRI-PET?ȷ��Ҫ��ת180��ͷ?","���ٴ�ȷ��",MB_ICONQUESTION|MB_YESNO))
	{  
       Chooseflag=19;
	//   CTPETMRI = 3;
	   CDialog::OnOK();
	}	
}
//MRI/PET(4)���������PET��
void CCHORoad::OnDirectMoveMRIPET4() //OnDirectMove2() 
{
	// TODO: Add your control notification handler code here
	if(IDYES==MessageBox("MRI-PET?ȷ������������?ȷ��Ҫ��PET�Ŵ���?","���ٴ�ȷ��",MB_ICONQUESTION|MB_YESNO))
	{  
		if(IDYES==MessageBox("MRI-PET?ȷ������������??","���ٴ�ȷ��",MB_ICONQUESTION|MB_YESNO))
		{  
		   Chooseflag=15;
		//   CTPETMRI = 3;
		   CDialog::OnOK();
		}
	}		
}

void CCHORoad::OnReadHold() 
{
	// TODO: Add your command handler code here
	
}

void CCHORoad::OnNextLimited() 
{
	// TODO: Add your control notification handler code here
	if(IDYES==MessageBox("ȷ��Ҫ���е���һ����λ?","���ٴ�ȷ��",MB_ICONQUESTION|MB_YESNO))
	{ 
		   Chooseflag=25;
		//   CTPETMRI = 3;
		   CDialog::OnOK();
	}	
}

void CCHORoad::OnProLimited() 
{
	// TODO: Add your control notification handler code here
	if(IDYES==MessageBox("ȷ��Ҫ���е���һ����λ?","���ٴ�ȷ��",MB_ICONQUESTION|MB_YESNO))
	{ 
		   Chooseflag=26;
		//   CTPETMRI = 3;
		   CDialog::OnOK();
	}	
}

void CCHORoad::OnMRITurning92() 
{
	// TODO: Add your control notification handler code here
	if(IDYES==MessageBox("ȷ��Ҫ���õ�MRI?","���ٴ�ȷ��",MB_ICONQUESTION|MB_YESNO))
	{ 
		   Chooseflag=27;
		//   CTPETMRI = 3;
		   CDialog::OnOK();
	}	
}

void CCHORoad::OnMRITurning93() 
{
	// TODO: Add your control notification handler code here
	if(IDYES==MessageBox("ȷ��Ҫ���õ�PET?","���ٴ�ȷ��",MB_ICONQUESTION|MB_YESNO))
	{ 
		   Chooseflag=28;
		//   CTPETMRI = 3;
		   CDialog::OnOK();
	}	
}

void CCHORoad::OnMRITurning94() 
{
	// TODO: Add your control notification handler code here
	if(IDYES==MessageBox("ȷ����MRI���뿪?","���ٴ�ȷ��",MB_ICONQUESTION|MB_YESNO))
	{ 
		   Chooseflag=29;
		//   CTPETMRI = 3;
		   CDialog::OnOK();
	}	
}

void CCHORoad::OnMRITurning95() 
{
	// TODO: Add your control notification handler code here
	if(IDYES==MessageBox("ȷ����PET���뿪?","���ٴ�ȷ��",MB_ICONQUESTION|MB_YESNO))
	{ 
		   Chooseflag=30;
		//   CTPETMRI = 3;
		   CDialog::OnOK();
	}	
}

void CCHORoad::OnNextLimited2() 
{
	if(IDYES==MessageBox("ȷ��Ҫ��90������?","���ٴ�ȷ��",MB_ICONQUESTION|MB_YESNO))
	{ 
		   Chooseflag = 32;	
		   CDialog::OnOK();
	}		
}

void CCHORoad::OnProLimited2() 
{
	if(IDYES==MessageBox("ȷ��Ҫ��90������?","���ٴ�ȷ��",MB_ICONQUESTION|MB_YESNO))
	{ 
		   Chooseflag=33;
		   CDialog::OnOK();
	}	
}
