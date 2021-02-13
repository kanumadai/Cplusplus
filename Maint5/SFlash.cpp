// SFlash.cpp : implementation file
//

#include "stdafx.h"
#include "Maint.h"
#include "SFlash.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSFlash dialog


CSFlash::CSFlash(CWnd* pParent /*=NULL*/)
	: CDialog(CSFlash::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSFlash)
	m_strUser = _T("");
	m_strCompany = _T("");
	//}}AFX_DATA_INIT

	VERIFY(m_bmSplash.LoadBitmap(IDB_SFLASH));
	// Initialize a null brush for text backgrounds
	VERIFY(m_brHollow.CreateStockObject(HOLLOW_BRUSH));

	// Determine how large our bitmap is for dialog sizing
	BITMAP bm;
	VERIFY(m_bmSplash.GetBitmap(&bm));
	m_sBitmap = CSize(bm.bmWidth,bm.bmHeight);


}


void CSFlash::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSFlash)
	DDX_Text(pDX, IDC_USER, m_strUser);
	DDX_Text(pDX, IDC_COMPANY, m_strCompany);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSFlash, CDialog)
	//{{AFX_MSG_MAP(CSFlash)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
	ON_WM_ERASEBKGND()   //手动添加 擦除背景消息 驱动

END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSFlash message handlers

BOOL CSFlash::DestroyWindow() 
{
	// TODO: Add your specialized code here and/or call the base class
	m_bmSplash.DeleteObject();

	return CDialog::DestroyWindow();
}

HBRUSH CSFlash::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	// TODO: Change any attributes of the DC here
	pDC->SetBkMode(TRANSPARENT);
	pDC->SetBkColor(RGB(64,138,101));
	pDC->SetTextColor(RGB(236,26,73)); // same as text within bitmap
	
	// TODO: Return a different brush if the default is not desired
	return hbr;
}

BOOL CSFlash::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	CRect rcRect;
	GetWindowRect(&rcRect);
	SetWindowPos(&wndTopMost,
					rcRect.left,
					rcRect.top,
					m_sBitmap.cx + GetSystemMetrics(SM_CXDLGFRAME),
					m_sBitmap.cy + GetSystemMetrics(SM_CYDLGFRAME),
					SWP_HIDEWINDOW);

	// Relocate and load text controls
	CSize sSize;
	CWnd* ctrl = GetDlgItem(IDC_USER);
	ctrl->GetWindowRect(&rcRect);
	sSize = rcRect.Size();
	ctrl->MoveWindow(140,180,sSize.cx,sSize.cy,FALSE);
	ctrl = GetDlgItem(IDC_COMPANY);
	ctrl->GetWindowRect(&rcRect);
	sSize = rcRect.Size();
	ctrl->MoveWindow(70,202,sSize.cx,sSize.cy,TRUE);

	// Update the strings
	UpdateData(FALSE);

	// Show window
	Invalidate(TRUE);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CSFlash::OnEraseBkgnd(CDC *pDC)
{
	CDC dcMem;

	dcMem.CreateCompatibleDC(pDC);
	CBitmap* pOldBitmap = dcMem.SelectObject(&m_bmSplash);
	CRect rcClient;
	GetClientRect(&rcClient);
	const CSize& sBitmap = m_sBitmap;
	pDC->BitBlt(0,0,sBitmap.cx,sBitmap.cy,&dcMem,0,0,SRCCOPY);
	dcMem.SelectObject(pOldBitmap);

	return true;

}
