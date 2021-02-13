// SonorSetting.cpp : implementation file
//

#include "stdafx.h"
#include "robot.h"
#include "SonorSetting.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSonorSetting dialog


CSonorSetting::CSonorSetting(CWnd* pParent /*=NULL*/)
	: CDialog(CSonorSetting::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSonorSetting)
		// NOTE: the ClassWizard will add member initialization here
	m_SonorLimtedL1 = 60;
	m_SonorLimtedL2 = 60;
	m_SonorLimtedL3 = 60;
	m_SonorLimtedL4 = 60;
	m_SonorLimtedR1 = 60;
	m_SonorLimtedR2 = 60;
	m_SonorLimtedR3 = 60;
	m_SonorLimtedR4 = 60;
	m_IRLimtedL = 20;
	m_IRLimtedR = 20;
	//}}AFX_DATA_INIT
}


void CSonorSetting::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSonorSetting)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	DDX_Text(pDX, IDC_SONORLIMTEDL1, m_SonorLimtedL1);
	DDX_Text(pDX, IDC_SONORLIMTEDL2, m_SonorLimtedL2);
	DDX_Text(pDX, IDC_SONORLIMTEDL3, m_SonorLimtedL3);
	DDX_Text(pDX, IDC_SONORLIMTEDL4, m_SonorLimtedL4);
	DDX_Text(pDX, IDC_SONORLIMTEDR1, m_SonorLimtedR1);
	DDX_Text(pDX, IDC_SONORLIMTEDR2, m_SonorLimtedR2);
	DDX_Text(pDX, IDC_SONORLIMTEDR3, m_SonorLimtedR3);
	DDX_Text(pDX, IDC_SONORLIMTEDR4, m_SonorLimtedR4);
	DDX_Text(pDX, IDC_IRLIMTEDL, m_IRLimtedL);
	DDX_Text(pDX, IDC_IRLIMTEDR, m_IRLimtedR);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSonorSetting, CDialog)
	//{{AFX_MSG_MAP(CSonorSetting)
	ON_EN_CHANGE(IDC_SONORLIMTEDL1, OnChangeSonorlimtedl1)
	ON_EN_CHANGE(IDC_SONORLIMTEDL2, OnChangeSonorlimtedl2)
	ON_EN_CHANGE(IDC_IRLIMTEDL, OnChangeIrlimtedl)
	ON_EN_CHANGE(IDC_IRLIMTEDR, OnChangeIrlimtedr)
	ON_EN_CHANGE(IDC_SONORLIMTEDL3, OnChangeSonorlimtedl3)
	ON_EN_CHANGE(IDC_SONORLIMTEDL4, OnChangeSonorlimtedl4)
	ON_EN_CHANGE(IDC_SONORLIMTEDR1, OnChangeSonorlimtedr1)
	ON_EN_CHANGE(IDC_SONORLIMTEDR2, OnChangeSonorlimtedr2)
	ON_EN_CHANGE(IDC_SONORLIMTEDR3, OnChangeSonorlimtedr3)
	ON_EN_CHANGE(IDC_SONORLIMTEDR4, OnChangeSonorlimtedr4)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSonorSetting message handlers

void CSonorSetting::OnChangeSonorlimtedl1() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
    UpdateData(TRUE);
	if (m_SonorLimtedL1 > 300 )
	{
		MessageBox("超出设定范围45~300CM");
	}
	// TODO: Add your control notification handler code here
	
}

void CSonorSetting::OnChangeSonorlimtedl2() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	if (m_SonorLimtedL2 > 300 )
	{
		MessageBox("超出设定范围45~300CM");
	}	
	// TODO: Add your control notification handler code here
	
}




void CSonorSetting::OnChangeSonorlimtedl3() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	if (m_SonorLimtedL3 > 300 )
	{
		MessageBox("超出设定范围45~300CM");
	}	
	// TODO: Add your control notification handler code here
	
}

void CSonorSetting::OnChangeSonorlimtedl4() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	if (m_SonorLimtedL4 > 300 )
	{
		MessageBox("超出设定范围45~300CM");
	}	
	// TODO: Add your control notification handler code here
	
}

void CSonorSetting::OnChangeSonorlimtedr1() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	if (m_SonorLimtedR1 > 300 )
	{
		MessageBox("超出设定范围45~300CM");
	}	
	// TODO: Add your control notification handler code here
	
}

void CSonorSetting::OnChangeSonorlimtedr2() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	if (m_SonorLimtedR2 > 300 )
	{
		MessageBox("超出设定范围45~300CM");
	}	
	// TODO: Add your control notification handler code here
	
}

void CSonorSetting::OnChangeSonorlimtedr3() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	if (m_SonorLimtedR3 > 300 )
	{
		MessageBox("超出设定范围45~300CM");
	}	
	// TODO: Add your control notification handler code here
	
}

void CSonorSetting::OnChangeSonorlimtedr4() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	if (m_SonorLimtedR4 > 300 )
	{
		MessageBox("超出设定范围45~300CM");
	}	
	// TODO: Add your control notification handler code here
	
}
void CSonorSetting::OnChangeIrlimtedl() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	if (m_IRLimtedL > 50 )
	{
		MessageBox("超出设定范围0~50CM");
	}	
	// TODO: Add your control notification handler code here
	
}

void CSonorSetting::OnChangeIrlimtedr() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	if (m_IRLimtedR > 50 )
	{
		MessageBox("超出设定范围0~50CM");
	}	
	// TODO: Add your control notification handler code here
	
}