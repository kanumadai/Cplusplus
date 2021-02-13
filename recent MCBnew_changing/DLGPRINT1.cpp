// DLGPRINT1.cpp : implementation file
//

#include "stdafx.h"
#include "MCB.h"
#include "DlgPrint1.h"
#include "MCBView.h"
#include "math.h"
extern HARDWARE_PARE g_HardWare_Pare;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// DLGPRINT dialog


DLGPRINT::DLGPRINT(CWnd* pParent /*=NULL*/)
	: CDialog(DLGPRINT::IDD, pParent)
{
	//{{AFX_DATA_INIT(DLGPRINT)
	m_min = 0;
	m_max = 0;
	m_auto = _T("");
	//}}AFX_DATA_INIT
	m_printmode=1;
	m_range=0;
	m_rangemode=0;
	m_1=0;
	m_2=0;
	m_3=0;
	m_4=0;
	m_5=0;
}


void DLGPRINT::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(DLGPRINT)
	DDX_Text(pDX, IDC_EDIT1, m_min);
	DDV_MinMaxInt(pDX, m_min, 0, 8191);
	DDX_Text(pDX, IDC_EDIT2, m_max);
	DDV_MinMaxInt(pDX, m_max, 0, 8191);
	DDX_Text(pDX, IDC_EDIT3, m_auto);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(DLGPRINT, CDialog)
	//{{AFX_MSG_MAP(DLGPRINT)
	ON_BN_CLICKED(IDC_RADIO_ALL, OnAreaAll)
	ON_BN_CLICKED(IDC_RADIO_SELF, OnAreaSelf)
	ON_EN_CHANGE(IDC_EDIT1, OnChangeMIN)
	ON_EN_CHANGE(IDC_EDIT2, OnChangeMAX)
	ON_EN_CHANGE(IDC_EDIT3, OnChangeAuto)
	ON_EN_SETFOCUS(IDC_EDIT1, OnSetfocusMin)
	ON_EN_SETFOCUS(IDC_EDIT2, OnMax)
	ON_BN_CLICKED(IDC_RADIO_SELFSELECT, OnRadioSelfselect)
	ON_BN_CLICKED(IDC_RADIO_AUTOSELECT, OnRadioAutoselect)
	ON_EN_SETFOCUS(IDC_EDIT3, OnSetfocusEdit3)
	ON_CBN_SELCHANGE(IDC_COMBO1, OnSelchangeCombo)
	ON_BN_CLICKED(IDC_RADIO_INTEREST, OnAreaInterest)
	ON_BN_CLICKED(IDC_CHECK1, OnCheckFile)
	ON_BN_CLICKED(IDC_CHECK3, OnCheckRealTime)

	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// DLGPRINT message handlers

void DLGPRINT::OnAreaAll() 
{
	// TODO: Add your control notification handler code here
    CFrameWnd *pwnd=GetParentFrame();
    CMCBView *view=(CMCBView *)pwnd->GetActiveView();
	if(view->m_bMCBModeSel) m_max = view->m_iFullChannelWidth-1;
	else m_max = view->m_iBuffFullChannelWidth-1;

	m_min = 0;
    m_printmode=1;
	
	CButton *pButton=(CButton*)GetDlgItem(IDC_RADIO_SELF);
    pButton->SetCheck(FALSE);
	pButton=(CButton*)GetDlgItem(IDC_RADIO_INTEREST);
    pButton->SetCheck(FALSE);

    RangeInit();
	UpdateData(FALSE);
}


void DLGPRINT::OnAreaSelf() 
{
	// TODO: Add your control notification handler code here
	
	
    m_max=0;
	m_min=0;

	CButton *pButton=(CButton*)GetDlgItem(IDC_RADIO_ALL);
    pButton->SetCheck(FALSE);
	pButton=(CButton*)GetDlgItem(IDC_RADIO_INTEREST);
    pButton->SetCheck(FALSE);
    
    m_printmode=2;
    RangeInit();
    UpdateData(FALSE);

}

void DLGPRINT::OnChangeMIN() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	// TODO: Add your control notification handler code here

//	RangeInit();

    UpdateData(TRUE);
	
	CFrameWnd *pwnd=GetParentFrame();
    CMCBView *view=(CMCBView *)pwnd->GetActiveView();
	if (view->m_bDrawLogData_Sel)
    {

        CEdit *pedit=(CEdit *)GetDlgItem(IDC_EDIT3);
		m_auto.Format("%s","Log");
		UpdateData(FALSE);
        m_rangemode=1;
	    m_range=(int)CaculateRange();
	}
	else
	{
		
       m_rangemode=1;
	   int kkk=(int)CaculateRange();
	   m_range=kkk;

	   if (kkk>1024)
	   {
			kkk=kkk/1024;
			m_auto.Format("%i%s",kkk,"k");

	   }
		else
		m_auto.Format("%i",kkk);

		UpdateData(FALSE);
	}
	
}

void DLGPRINT::OnChangeMAX() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	
//	RangeInit();

	UpdateData(TRUE);
    CFrameWnd *pwnd=GetParentFrame();
    CMCBView *view=(CMCBView *)pwnd->GetActiveView();
	if (view->m_bDrawLogData_Sel)
    {

        CEdit *pedit=(CEdit *)GetDlgItem(IDC_EDIT3);
		m_auto.Format("%s","Log");
		UpdateData(FALSE);
        m_rangemode=1;
	    m_range=(int)CaculateRange();
	}
	else
	{
		
       m_rangemode=1;
	   int kkk=(int)CaculateRange();
	   m_range=kkk;

	   if (kkk>1024)
	   {
			kkk=kkk/1024;
			m_auto.Format("%i%s",kkk,"k");

	   }
		else
		m_auto.Format("%i",kkk);

		UpdateData(FALSE);
	}
	
    
}

void DLGPRINT::OnChangeAuto() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	
}

BOOL DLGPRINT::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	
    CButton *pButton=(CButton*)GetDlgItem(IDC_RADIO_ALL);
	pButton->SetCheck(TRUE);
	pButton=(CButton*)GetDlgItem(IDC_RADIO_SELF);
	pButton->SetCheck(FALSE);

	CFrameWnd *pwnd=GetParentFrame();
    CMCBView *view=(CMCBView *)pwnd->GetActiveView();
	if(view->m_bMCBModeSel) m_max = view->m_iFullChannelWidth-1;
	else m_max = view->m_iBuffFullChannelWidth-1;
	m_min = 0;

	CComboBox *pCombox=(CComboBox *)GetDlgItem(IDC_COMBO1);
	pCombox->ResetContent();	
	pCombox->AddString("0"); 
	pCombox->SetCurSel(0);

    
	UpdateData(FALSE);

  if(view->m_bMCBModeSel)
  {
    int i=view->m_iSmallWinCenPoint;
	if (view->m_arrExternalRam[i] < 0x80000000)
	{
        CButton *pButton=(CButton*)GetDlgItem(IDC_RADIO_INTEREST);
		pButton->EnableWindow(FALSE);

	}
  }
  else
  {
    int i=view->m_iBuffSmallWinCenPoint;
	if (view->m_arrExternalRam[i] < 0x80000000)
	{
        CButton *pButton=(CButton*)GetDlgItem(IDC_RADIO_INTEREST);
		pButton->EnableWindow(FALSE);

	}
  }
	
    pButton=(CButton*)GetDlgItem(IDC_RADIO_AUTOSELECT);
	pButton->SetCheck(TRUE);
	pButton=(CButton*)GetDlgItem(IDC_RADIO_SELFSELECT);
	pButton->SetCheck(FALSE);
    
	m_rangemode=1;
	if (view->m_bDrawLogData_Sel)
	{
		m_auto.Format("%s","Log");
		m_range=(int)CaculateRange();
        UpdateData(FALSE);
		
		pButton=(CButton *)GetDlgItem(IDC_RADIO_SELFSELECT);
		pButton->EnableWindow(FALSE);
	}
	else
	{
        int kkk=(int)CaculateRange();
		 m_range=kkk;

		if (kkk>1024)
		{
			kkk=kkk/1024;
			m_auto.Format("%i%s",kkk,"k");

		}
		else
		m_auto.Format("%i",kkk);
	    UpdateData(FALSE);
	}
	return TRUE;    // return TRUE unless you set the focus to a control
	                // EXCEPTION: OCX Property Pages should return FALSE
}

void DLGPRINT::OnSetfocusMin() 
{
	// TODO: Add your control notification handler code here
	CButton *pButton=(CButton *)GetDlgItem(IDC_RADIO_ALL);
	pButton->SetCheck(FALSE);
    pButton=(CButton *)GetDlgItem(IDC_RADIO_SELF);
	pButton->SetCheck(TRUE);

    if ((m_printmode==1)||(m_printmode==3))
    {
        m_max=0;
		m_min=0;
	UpdateData(FALSE);
    }
	
	m_printmode=2;
    
}

void DLGPRINT::OnMax() 
{
	// TODO: Add your control notification handler code here
	CButton *pButton=(CButton *)GetDlgItem(IDC_RADIO_ALL);
	pButton->SetCheck(FALSE);
    pButton=(CButton *)GetDlgItem(IDC_RADIO_SELF);
	pButton->SetCheck(TRUE);
    if ((m_printmode==1)||(m_printmode==3))
    {
        m_max=0;
		m_min=0;
    	UpdateData(FALSE);
    }
	m_printmode=2;
}

void DLGPRINT::OnRadioSelfselect() 
{
	// TODO: Add your control notification handler code here	
	
	m_rangemode=2;
	
	
	CButton *pButton=(CButton *)GetDlgItem(IDC_RADIO_AUTOSELECT);
	pButton->SetCheck(FALSE);
	pButton=(CButton *)GetDlgItem(IDC_RADIO_SELFSELECT);
	pButton->SetCheck(TRUE);

	CComboBox *pCombox=(CComboBox *)GetDlgItem(IDC_COMBO1);
	pCombox->ResetContent();	
	pCombox->AddString("8192k");
	pCombox->AddString("4096k");
	pCombox->AddString("2048k");
	pCombox->AddString("1024k");
	pCombox->AddString("512k");
	pCombox->AddString("256k");
	pCombox->AddString("128k");
	pCombox->AddString("64k");
	pCombox->AddString("32k");
	pCombox->AddString("16k");
	pCombox->AddString("8k");
	pCombox->AddString("4k");
	pCombox->AddString("2k");
	pCombox->AddString("1024");
	pCombox->AddString("512");
	pCombox->AddString("256");
	pCombox->AddString("128");
	pCombox->AddString("64");

	pCombox->SetCurSel(17);
    m_auto.Format(" % i",0);
	UpdateData(FALSE);


}

void DLGPRINT::OnRadioAutoselect() 
{
	// TODO: Add your control notification handler code here
	
	m_rangemode=1;

	CButton *pButton=(CButton *)GetDlgItem(IDC_RADIO_AUTOSELECT);
	pButton->SetCheck(TRUE);
	pButton=(CButton *)GetDlgItem(IDC_RADIO_SELFSELECT);
	pButton->SetCheck(FALSE);

	CComboBox *pCombox=(CComboBox *)GetDlgItem(IDC_COMBO1);
	pCombox->ResetContent();
	pCombox->AddString("0");
	pCombox->SetCurSel(0);

	int kkk=(int)CaculateRange();
    m_range=kkk;

	if (kkk>1024)
	{
		kkk=kkk/1024;
		m_auto.Format("%i%s",kkk,"k");

	}
    else
		m_auto.Format("%i",kkk);

    CFrameWnd *pwnd=GetParentFrame();
    CMCBView *view=(CMCBView *)pwnd->GetActiveView();
	if (view->m_bDrawLogData_Sel)
	{
        m_auto.Format("%s","Log");
	}

    UpdateData(FALSE);
    
}

void DLGPRINT::OnSetfocusEdit3() 
{
	// TODO: Add your control notification handler code here
	CButton *pButton=(CButton *)GetDlgItem(IDC_RADIO_AUTOSELECT);
	pButton->SetCheck(TRUE);
	pButton=(CButton *)GetDlgItem(IDC_RADIO_SELFSELECT);
	pButton->SetCheck(FALSE);

}

void DLGPRINT::OnSelchangeCombo() 
{
	// TODO: Add your control notification handler code here
	
}

void DLGPRINT::OnCancel() 
{
	// TODO: Add extra cleanup here
	
   m_printmode=0;
    
//	CDialog::OnCancel();
    CDialog::EndDialog(0);
}

void DLGPRINT::OnOK() 
{
	// TODO: Add extra validation here
	
    CComboBox *pCombox=(CComboBox *)GetDlgItem(IDC_COMBO1);
    m_selfvalue=pCombox->GetCurSel();

    UpdateData(TRUE);

	CDialog::OnOK();
}

void DLGPRINT::OnAreaInterest() 
{
	// TODO: Add your control notification handler code here

	m_printmode=3;

    CButton *pButton=(CButton*)GetDlgItem(IDC_RADIO_SELF);
    pButton->SetCheck(FALSE);
	pButton=(CButton*)GetDlgItem(IDC_RADIO_ALL);
    pButton->SetCheck(FALSE);
	pButton=(CButton*)GetDlgItem(IDC_RADIO_AUTOSELECT);
	pButton->SetCheck(TRUE);
    
    CFrameWnd *pwnd=GetParentFrame();
    CMCBView *view=(CMCBView *)pwnd->GetActiveView();
  if(view->m_bMCBModeSel)
  {
    int i=view->m_iSmallWinCenPoint;
	while (view->m_arrExternalRam[i] >= 0x80000000)
	{
        i-=1;
		if (view->m_arrExternalRam[i]<0x80000000) break;
	
	}
	m_min=i+1;

    i=view->m_iSmallWinCenPoint;
	while (view->m_arrExternalRam[i] >= 0x80000000)
	{
        i+=1;
		if (view->m_arrExternalRam[i]<0x80000000) break;
	}

    m_max=i-1;
    RangeInit();
	UpdateData(FALSE);
  }
  else
  {
    int i=view->m_iBuffSmallWinCenPoint;
	while (view->m_arrExternalRam[i] >= 0x80000000)
	{
        i-=1;
		if (view->m_arrExternalRam[i]<0x80000000) break;
	
	}
	m_min=i+1;

    i=view->m_iBuffSmallWinCenPoint;
	while (view->m_arrExternalRam[i] >= 0x80000000)
	{
        i+=1;
		if (view->m_arrExternalRam[i]<0x80000000) break;
	}

    m_max=i-1;
    RangeInit();
	UpdateData(FALSE);
  }

}

float DLGPRINT::CaculateRange()
{
    CFrameWnd *pwnd=GetParentFrame();
    CMCBView *view=(CMCBView *)pwnd->GetActiveView();


	float fmaxvalue = 0.1;
	float fminvalue = 10;

	if (view->m_bDrawLogData_Sel) 
	{
	    return 8;
	}
    else
    {
		UINT maxvalue = ((view->m_arrExternalRam[0]) & 0x7fffffff);

		for(int i=m_min;i<m_max;i++)
		{
			maxvalue = max(maxvalue,((view->m_arrExternalRam[i]) & 0x7fffffff));
		}	
	
		i=0;
		UINT kkk = 64;
		while(i<17)
		{
			if(maxvalue < kkk) break;
			i++;
			kkk *= 2;
		}
		maxvalue = kkk; 
	
		return maxvalue;
    }  
}

void DLGPRINT::RangeInit()
{
   	CButton *pButton=(CButton *)GetDlgItem(IDC_RADIO_AUTOSELECT);
	pButton->SetCheck(TRUE);
	pButton=(CButton *)GetDlgItem(IDC_RADIO_SELFSELECT);
	pButton->SetCheck(FALSE);
	
	CComboBox *pCombox=(CComboBox *)GetDlgItem(IDC_COMBO1);
	pCombox->ResetContent();
	pCombox->AddString("0");
	pCombox->SetCurSel(0);
    
    m_rangemode=1;
	int kkk=(int)CaculateRange();
    m_range=(int)CaculateRange();
    

	if (kkk>1024)
	{
		kkk=kkk/1024;
		m_auto.Format("%i%s",kkk,"k");

	}
    else
		m_auto.Format("%i",kkk);

	UpdateData(FALSE);
	
	CFrameWnd *pwnd=GetParentFrame();
    CMCBView *view=(CMCBView *)pwnd->GetActiveView();
	
	if (view->m_bDrawLogData_Sel)
    {
        CButton *pButton=(CButton*)GetDlgItem(IDC_RADIO_SELFSELECT);
		pButton->EnableWindow(FALSE);


		pButton=(CButton *)GetDlgItem(IDC_RADIO_AUTOSELECT);
		pButton->SetCheck(TRUE);

        CEdit *pedit=(CEdit *)GetDlgItem(IDC_EDIT3);
		m_auto.Format("%s","Log");

		m_range=(int)CaculateRange();
        m_rangemode=1;
		UpdateData(FALSE);
	}
}



void DLGPRINT::OnCheckFile() 
{
	// TODO: Add your control notification handler code here
	
    m_1=1;
}

void DLGPRINT::OnCheckRealTime() 
{
	// TODO: Add your control notification handler code here
	
	m_3=1;
}

