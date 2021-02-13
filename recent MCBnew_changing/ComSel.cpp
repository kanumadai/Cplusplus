// ComSel.cpp : implementation file
//

#include "stdafx.h"
#include "MCB.h"
#include "ComSel.h"
#include "EzusbApi.h"
#include "ini.h"
#include "MCBView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern DWORD g_ComPortSel;
extern DWORD g_CommBaud;
extern HANDLE g_hUsb;

/////////////////////////////////////////////////////////////////////////////
// CComSel dialog


CComSel::CComSel(CWnd* pParent /*=NULL*/)
	: CDialog(CComSel::IDD, pParent)
{
	//{{AFX_DATA_INIT(CComSel)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CComSel::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CComSel)
//	DDX_Control(pDX, IDOK, m_ok);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CComSel, CDialog)
	//{{AFX_MSG_MAP(CComSel)
	ON_CBN_SELCHANGE(IDC_COMSEL, OnSelchangeComsel)
	ON_BN_CLICKED(IDC_CANCEL, OnCancel)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CComSel message handlers

void CComSel::OnOK() 
{
	CComboBox * pComSel = (CComboBox *)GetDlgItem(IDC_COMSEL);	

	int portsel = pComSel->GetCurSel();

	if(portsel == 0) g_ComPortSel = COM1_PORT;
	if(portsel == 1) g_ComPortSel = COM2_PORT;
	if(portsel == 2) g_ComPortSel = USB_PORT;
/*
	CIni inifile;
	if(inifile.Open("MCBini_05_1028.ini")){ 
		inifile.Write("MCB ShowPara", "ComPort",g_ComPortSel);
		inifile.Save();
		inifile.Close();
	}
*/
	if (g_ComPortSel == USB_PORT) {
		g_hUsb = OpenUsbDevice();
		if (g_hUsb == INVALID_HANDLE_VALUE) {
			AfxMessageBox("USB 端口打开错误\r\n请检查硬件是否连接.",MB_ICONINFORMATION);
			return;
		}

		CDialog::OnOK();
		return;
	}
		
	m_bComOpen = !m_bComOpen;
	
	CDialog::OnOK();
}

void CComSel::OnSelchangeComsel() 
{
	CComboBox * pComSel = (CComboBox *)GetDlgItem(IDC_COMSEL);	
	CButton  *pbut = (CButton *)GetDlgItem(IDC_PORT_STATUS_SHOW);
	
	int portsel = pComSel->GetCurSel();

	if(portsel == 0) g_ComPortSel = COM1_PORT;
	if(portsel == 1) g_ComPortSel = COM2_PORT;
	if(portsel == 2) g_ComPortSel = USB_PORT;


	if (g_ComPortSel == USB_PORT) {
		pbut->SetWindowText("采用USB端口进行通讯");
	}
	else{
		pbut->SetWindowText("串口通信速率: 57600 bps");
	}

	
}

BOOL CComSel::OnInitDialog() 
{
	CDialog::OnInitDialog();

	CComboBox * pComSel = (CComboBox *)GetDlgItem(IDC_COMSEL);	
	CButton  *pbut = (CButton *)GetDlgItem(IDC_PORT_STATUS_SHOW);

	pComSel->SetCurSel(g_ComPortSel);

	if (g_ComPortSel == USB_PORT) 
	{
		pbut->SetWindowText("采用USB端口进行通讯");
	}
	else
	{
		pbut->SetWindowText("串口通信速率: 57600 bps");
	}

    CFrameWnd *pFramwnd=GetParentFrame();
	CMCBView *view=(CMCBView *)pFramwnd->GetActiveView();
	if (view->m_startflag||view->m_continueflag)
	{
		CButton *pButton=(CButton *)GetDlgItem(IDOK);
		pButton->EnableWindow(FALSE);
	}
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CComSel::OnCancel() 
{

	CDialog::OnCancel();	
}
