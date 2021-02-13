// DlgPrint.cpp : implementation file
//

#include "stdafx.h"
#include "MCB.h"
#include "DlgPrint.h"
#include "MCBView.h"
#include "MCBIO.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
extern HARDWARE_PARE g_HardWare_Pare;  // 硬件参数结构
extern int 	g_iSetInterestLimitNo1;
extern int g_iSetInterestLimitNo2;
#include "DlgPrint.h"
/////////////////////////////////////////////////////////////////////////////
// CDlgPrint dialog


CDlgPrint::CDlgPrint(CWnd* pParent /*=NULL*/): CDialog(CDlgPrint::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgPrint)
	m_iMaxChannelWidth = 0;
	m_iMinChannelWidth = 0;
	//}}AFX_DATA_INIT
}


void CDlgPrint::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgPrint)
	DDX_Text(pDX, IDC_EDIT_MAX, m_iMaxChannelWidth);
	DDX_Text(pDX, IDC_EDIT_MIN, m_iMinChannelWidth);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgPrint, CDialog)
	//{{AFX_MSG_MAP(CDlgPrint)
	ON_BN_CLICKED(IDC_BUTTON_PRE, OnButtonPre)
	ON_EN_SETFOCUS(IDC_EDIT_MIN, OnSetfocusEditMin)
	ON_EN_SETFOCUS(IDC_EDIT_MAX, OnSetfocusEditMax)
	ON_BN_CLICKED(IDC_RADIO_ALL, OnRadioAll)
	ON_BN_CLICKED(IDC_RADIO_PART, OnRadioPart)
	ON_BN_CLICKED(IDC_RADIO_SELF, OnRadioSelf)
	ON_BN_CLICKED(IDC_CHECK1, OnCheck1)
	ON_BN_CLICKED(IDC_CHECK2, OnCheck2)
	ON_BN_CLICKED(IDC_CHECK3, OnCheck3)
	ON_BN_CLICKED(IDC_CHECK5, OnCheck5)
	ON_BN_CLICKED(IDC_CHECK6, OnCheck6)
	ON_BN_CLICKED(IDC_CHECK7, OnCheck7)
	ON_BN_CLICKED(IDC_CHECK9, OnCheck9)
	ON_BN_CLICKED(IDC_BUTTON_CANCEL, OnButtonCancel)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgPrint message handlers


void CDlgPrint::OnButtonPre() 
{
	// TODO: Add your control notification handler code here
	
	UpdateData(TRUE);

}

BOOL CDlgPrint::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	CButton* pButton = (CButton*)GetDlgItem(IDC_RADIO_ALL);
	pButton->SetCheck(TRUE);
	m_iMaxChannelWidth = g_HardWare_Pare.iChannelWidth-1;
	m_iMinChannelWidth = 0;
	UpdateData(FALSE);




///////////////////////////////////////////////////changing
	m_1=0;
	m_2=0;
	m_3=0;
	m_4=0;
	m_5=0;
    m_smallwin=0;
    m_centerline=0;

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgPrint::OnSetfocusEditMin() 
{
	// TODO: Add your control notification handler code here
	CButton* pButton = (CButton*)GetDlgItem(IDC_RADIO_ALL);
	pButton->SetCheck(FALSE);
	pButton = (CButton*)GetDlgItem(IDC_RADIO_PART);
	pButton->SetCheck(FALSE);
	pButton = (CButton*)GetDlgItem(IDC_RADIO_SELF);
	pButton->SetCheck(TRUE);
//	SetDlgItemText(IDC_EDIT_MIN,"");
}

void CDlgPrint::OnSetfocusEditMax() 
{
	// TODO: Add your control notification handler code here
	CButton* pButton = (CButton*)GetDlgItem(IDC_RADIO_ALL);
	pButton->SetCheck(FALSE);
	pButton = (CButton*)GetDlgItem(IDC_RADIO_PART);
	pButton->SetCheck(FALSE);
	pButton = (CButton*)GetDlgItem(IDC_RADIO_SELF);
	pButton->SetCheck(TRUE);
//	SetDlgItemText(IDC_EDIT_MAX,"");
}

void CDlgPrint::OnRadioAll() 
{
	// TODO: Add your control notification handler code here
	m_iMaxChannelWidth = g_HardWare_Pare.iChannelWidth;
	m_iMinChannelWidth = 0;
	UpdateData(FALSE);
}

void CDlgPrint::OnRadioPart() 
{
	// TODO: Add your control notification handler code here
	if (g_iSetInterestLimitNo1>= g_iSetInterestLimitNo2)
	{
		m_iMaxChannelWidth = g_iSetInterestLimitNo1;
		m_iMinChannelWidth = g_iSetInterestLimitNo2;	
	}
	else
	{
		m_iMaxChannelWidth = g_iSetInterestLimitNo2;
		m_iMinChannelWidth = g_iSetInterestLimitNo1;
	}
	UpdateData(FALSE);
}

void CDlgPrint::OnRadioSelf() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
}

BOOL CDlgPrint::DestroyWindow() 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CDialog::DestroyWindow();
}

void CDlgPrint::OnCheck1() 
{
	// TODO: Add your control notification handler code here
	m_smallwin=1;
}

void CDlgPrint::OnCheck2() 
{
	// TODO: Add your control notification handler code here
	m_centerline=1;
}

void CDlgPrint::OnCheck3() 
{
	// TODO: Add your control notification handler code here
	m_2=1;
}

void CDlgPrint::OnCheck5() 
{
	// TODO: Add your control notification handler code here
	m_1=1;
}

void CDlgPrint::OnCheck6() 
{
	// TODO: Add your control notification handler code here
	m_4=1;
}

void CDlgPrint::OnCheck7() 
{
	// TODO: Add your control notification handler code here
	m_5=1;
}

void CDlgPrint::OnCheck9() 
{
	// TODO: Add your control notification handler code here
	m_3=1;
}

BOOL CDlgPrint::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext) 
{
	// TODO: Add your specialized code here and/or call the base class

	return CDialog::Create(IDD, pParentWnd);
}

BOOL CDlgPrint::PreCreateWindow(CREATESTRUCT& cs) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CDialog::PreCreateWindow(cs);
}

void CDlgPrint::OnButtonCancel() 
{
	// TODO: Add your control notification handler code here
	m_1=0;
	m_2=0;
	m_3=0;
	m_4=0;
	m_5=0;
	m_smallwin=0;
    m_centerline=0;
}
