// EnergeKedu.cpp : implementation file
//

#include "stdafx.h"
#include "MCB.h"
#include "EnergeKedu.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEnergeKedu dialog

CEnergeKedu::CEnergeKedu(CView *pView)
{
	m_pView = pView;  //akang add
}

CEnergeKedu::CEnergeKedu(CWnd* pParent /*=NULL*/)
	: CDialog(CEnergeKedu::IDD, pParent)
{
	m_pView = NULL;

	//{{AFX_DATA_INIT(CEnergeKedu)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CEnergeKedu::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CEnergeKedu)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CEnergeKedu, CDialog)
	//{{AFX_MSG_MAP(CEnergeKedu)
	ON_WM_CLOSE()
	ON_CBN_SELCHANGE(IDC_COMBOX_EGUNIT, OnSelchangeComboxEgunit)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEnergeKedu message handlers


BOOL CEnergeKedu::Create()  //akang add
{
	return CDialog::Create(CEnergeKedu::IDD);
}


BOOL CEnergeKedu::OnInitDialog() 
{
	CDialog::OnInitDialog();

/*	switch(m_iEnergeUnit) 
	{
	case 0:
		GetDlgItem(IDC_STATIC_UNIT)->SetWindowText("нч");
		m_iEnergeUnit = 0;
		break;
	case 1:
		GetDlgItem(IDC_STATIC_UNIT)->SetWindowText("eV");
		m_iEnergeUnit = 1;
		break;
	case 2:
		GetDlgItem(IDC_STATIC_UNIT)->SetWindowText("KeV");
		m_iEnergeUnit = 2;
		break;
	case 3:
		GetDlgItem(IDC_STATIC_UNIT)->SetWindowText("MeV");
		m_iEnergeUnit = 3;
		break;
	default:
		GetDlgItem(IDC_STATIC_UNIT)->SetWindowText("нч");
		m_iEnergeUnit = 0;
		break;
	}	
*/	
    CComboBox *pEGUNIT=(CComboBox *)GetDlgItem(IDC_COMBOX_EGUNIT);
	pEGUNIT->AddString("нч");
	pEGUNIT->AddString("eV");
	pEGUNIT->AddString("keV");    
	pEGUNIT->AddString("Mev");
    
	switch(m_iEnergeUnit)
	{
		case 0: 
			pEGUNIT->SetCurSel(0);break;
		case 1:
            pEGUNIT->SetCurSel(1);break;
		case 2:
			pEGUNIT->SetCurSel(2);break;
		case 3:
			pEGUNIT->SetCurSel(3);break;
		default:
            pEGUNIT->SetCurSel(0);break;
	}
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CEnergeKedu::OnOK() 
{
	CString strvalue;
	float fvalue;
	UpdateData(TRUE);
	GetDlgItem(IDC_ENERGE_KEDU_VALUE)->GetWindowText(strvalue);

	CComboBox *pEGUNIT = (CComboBox *)GetDlgItem(IDC_COMBOX_EGUNIT);
	int index = 0;
	index = pEGUNIT->GetCurSel();
	switch(index)
	{
		case 0:
			m_iEnergeUnit = 0;break;
		case 1:
			m_iEnergeUnit = 1;break;
		case 2:
			m_iEnergeUnit = 2;break;
		case 3:
			m_iEnergeUnit = 3;break;
		default:
			m_iEnergeUnit = 0;break;
	}

	if ((strvalue.IsEmpty())||m_iEnergeUnit==0)
	{
		return;
	} 
	else 
	{
		fvalue = atof(strvalue);
	}
	fvalue += 0.00002;  

	float fbuff = fvalue * 10000.;
	int idot = (int)fbuff % 10000;

	if (m_pView != NULL) 
	{
		m_pView->PostMessage(WM_ENERGE_KEDU_IDOK,(int)fvalue,(int)idot);
	}
}

void CEnergeKedu::OnCancel() 
{
	if (m_pView != NULL) {
		m_pView->PostMessage(WM_ENERGE_KEDU_GOODBYE,IDCANCEL);
	}
	else CDialog::OnCancel();
}

void CEnergeKedu::OnSelchangeComboxEgunit() 
{
	// TODO: Add your control notification handler code here
/*	CComboBox *pEGUNIT = (CComboBox *)GetDlgItem(IDC_COMBOX_EGUNIT);
	int index = 0;
	index = pEGUNIT->GetCurSel();
	switch(index)
	{
		case 0:
			m_iEnergeUnit = 0;break;
		case 1:
			m_iEnergeUnit = 1;break;
		case 2:
			m_iEnergeUnit = 2;break;
		case 3:
			m_iEnergeUnit = 3;break;
		default:
			m_iEnergeUnit = 0;break;
	}*/
}
