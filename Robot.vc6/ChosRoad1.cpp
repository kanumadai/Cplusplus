// ChosRoad1.cpp : implementation file
//

#include "stdafx.h"
#include "robot.h"
#include "ChosRoad1.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CChosRoad1 dialog


CChosRoad1::CChosRoad1(CWnd* pParent /*=NULL*/)
	: CDialog(CChosRoad1::IDD, pParent)
{
	//{{AFX_DATA_INIT(CChosRoad1)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CChosRoad1::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CChosRoad1)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CChosRoad1, CDialog)
	//{{AFX_MSG_MAP(CChosRoad1)
	ON_BN_CLICKED(IDC_DireMovetoCT, OnDireMovetoCT)
	ON_BN_CLICKED(IDC_DireLeavetoCT, OnDireLeavetoCT)
	ON_BN_CLICKED(IDC_DireHoldDownCT, OnDireHoldDownCT)
	ON_BN_CLICKED(IDC_CT_PET, OnCtPet)
	ON_BN_CLICKED(IDC_PET_CT, OnPetCt)
	ON_BN_CLICKED(IDC_DireMovetoPET, OnDireMovetoPET)
	ON_BN_CLICKED(IDC_DireLeavetoPET, OnDireLeavetoPET)
	ON_BN_CLICKED(IDC_DireHoldDownPET, OnDireHoldDownPET)
	ON_BN_CLICKED(IDC_NextLimited, OnNextLimited)
	ON_BN_CLICKED(IDC_ProLimited, OnProLimited)
	ON_BN_CLICKED(IDC_NextLimited90, OnNextLimited90)
	ON_BN_CLICKED(IDC_ProLimited90, OnProLimited90)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CChosRoad1 message handlers

void CChosRoad1::OnDireMovetoCT() 
{
	if(IDYES==MessageBox("确定要直接取CT床板?","请再次确认",MB_ICONQUESTION|MB_YESNO))
	{  
       Chooseflag=20;
	   CDialog::OnOK();
	}
}

void CChosRoad1::OnDireLeavetoCT() 
{
	if(IDYES==MessageBox("确定要直接取CT床板?","请再次确认",MB_ICONQUESTION|MB_YESNO))
	{  
       Chooseflag=21;
	   CDialog::OnOK();
	}
}

void CChosRoad1::OnDireHoldDownCT() 
{
	if(IDYES==MessageBox("确定要直接取CT床板?","请再次确认",MB_ICONQUESTION|MB_YESNO))
	{  
       Chooseflag=22;
	   CDialog::OnOK();
	}	
}

void CChosRoad1::OnCtPet() 
{
	if(IDYES==MessageBox("确定要直接取CT床板?","请再次确认",MB_ICONQUESTION|MB_YESNO))
	{  
       Chooseflag=5;
	   CDialog::OnOK();
	}	
}

void CChosRoad1::OnPetCt() 
{
	if(IDYES==MessageBox("确定要直接取CT床板?","请再次确认",MB_ICONQUESTION|MB_YESNO))
	{  
       Chooseflag=8;
	   CDialog::OnOK();
	}	
}

void CChosRoad1::OnDireMovetoPET() 
{
	if(IDYES==MessageBox("确定要直接取CT床板?","请再次确认",MB_ICONQUESTION|MB_YESNO))
	{  
       Chooseflag=13;
	   CDialog::OnOK();
	}	
}

void CChosRoad1::OnDireLeavetoPET() 
{
	if(IDYES==MessageBox("确定要直接取CT床板?","请再次确认",MB_ICONQUESTION|MB_YESNO))
	{  
       Chooseflag=14;
	   CDialog::OnOK();
	}	
}

void CChosRoad1::OnDireHoldDownPET() 
{
	if(IDYES==MessageBox("确定要直接取CT床板?","请再次确认",MB_ICONQUESTION|MB_YESNO))
	{  
       Chooseflag=15;
	   CDialog::OnOK();
	}	
}

void CChosRoad1::OnNextLimited() 
{
	if(IDYES==MessageBox("确定要直接取CT床板?","请再次确认",MB_ICONQUESTION|MB_YESNO))
	{  
       Chooseflag=25;
	   CDialog::OnOK();
	}		
}

void CChosRoad1::OnProLimited() 
{
	if(IDYES==MessageBox("确定要直接取CT床板?","请再次确认",MB_ICONQUESTION|MB_YESNO))
	{  
       Chooseflag=26;
	   CDialog::OnOK();
	}		
}

void CChosRoad1::OnNextLimited90() 
{
	if(IDYES==MessageBox("确定要直接取CT床板?","请再次确认",MB_ICONQUESTION|MB_YESNO))
	{  
       Chooseflag=32;
	   CDialog::OnOK();
	}	
}
void CChosRoad1::OnProLimited90() 
{
	if(IDYES==MessageBox("确定要直接取CT床板?","请再次确认",MB_ICONQUESTION|MB_YESNO))
	{  
       Chooseflag=33;
	   CDialog::OnOK();
	}	
}
