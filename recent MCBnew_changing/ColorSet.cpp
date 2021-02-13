// ColorSet.cpp : implementation file
//

#include "stdafx.h"
#include "MCB.h"
#include "ColorSet.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CColorSet dialog


CColorSet::CColorSet(CWnd* pParent /*=NULL*/)
	: CDialog(CColorSet::IDD, pParent)
{
	//{{AFX_DATA_INIT(CColorSet)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CColorSet::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CColorSet)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CColorSet, CDialog)
	//{{AFX_MSG_MAP(CColorSet)
	ON_BN_CLICKED(IDC_FULL_COLOR, OnFullColor)
	ON_BN_CLICKED(IDC_EXP_COLOR, OnExpColor)
	ON_BN_CLICKED(IDC_INTEREST_COLOR, OnInterestColor)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_SMALL_WINDOW_COLOR, OnSmallWindowColor)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CColorSet message handlers

BOOL CColorSet::OnInitDialog() 
{
	CDialog::OnInitDialog();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CColorSet::OnOK() 
{
	
	CDialog::OnOK();
}

void CColorSet::OnFullColor() 
{
	CColorDialog  FullColor(m_Full_Color,0,NULL);
	if(IDOK == FullColor.DoModal() ) {
		m_Full_Color = FullColor.GetColor();
	}
	OnPaint();
	
}

void CColorSet::OnExpColor() 
{
	CColorDialog  ExpColor(m_Exp_Color,0,NULL);
	if(IDOK == ExpColor.DoModal() ) {
		m_Exp_Color = ExpColor.GetColor();
	}
	OnPaint();
	
}

void CColorSet::OnInterestColor() 
{
	CColorDialog  InterestColor(m_Interest_Color,0,NULL);
	if(IDOK == InterestColor.DoModal() ) {
		m_Interest_Color = InterestColor.GetColor();
	}
	OnPaint();
	
}

void CColorSet::OnSmallWindowColor() 
{
	CColorDialog  SmallWinColor(m_Interest_Color,0,NULL);
	if(IDOK == SmallWinColor.DoModal() ) {
		m_SmallWin_Color = SmallWinColor.GetColor();
	}
	OnPaint();	
	
}


void CColorSet::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	CDC *pDC = GetDC();
	
	CBrush bShowFull;			
	bShowFull.CreateSolidBrush(m_Full_Color);//  (RGB(27,146,180));大窗口
	pDC->SelectObject(bShowFull);
	pDC->Rectangle(180,20,230,40);// 
	pDC->SelectObject(bShowFull);
	bShowFull.DeleteObject();

	CBrush bShowExp;			
	bShowExp.CreateSolidBrush(m_Exp_Color);//  (RGB(27,146,180));大窗口
	pDC->SelectObject(bShowExp);
	pDC->Rectangle(180,50,230,70);// 
	pDC->SelectObject(bShowExp);
	bShowExp.DeleteObject();

	CBrush bShowInterest;			
	bShowInterest.CreateSolidBrush(m_Interest_Color);//  (RGB(27,146,180));大窗口
	pDC->SelectObject(bShowInterest);
	pDC->Rectangle(180,80,230,100);// 
	pDC->SelectObject(bShowInterest);
	bShowInterest.DeleteObject();

	CBrush bShowSmallWin;			
	bShowSmallWin.CreateSolidBrush(m_SmallWin_Color);//  (RGB(27,146,180));大窗口
	pDC->SelectObject(bShowSmallWin);
	pDC->Rectangle(180,110,230,130);// 
	pDC->SelectObject(bShowSmallWin);
	bShowInterest.DeleteObject();

	ReleaseDC(pDC);	
}

