// LogIn.cpp : implementation file
//

#include "stdafx.h"
#include "Maint.h"
#include "LogIn.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLogIn dialog


CLogIn::CLogIn(CWnd* pParent /*=NULL*/)
	: CDialog(CLogIn::IDD, pParent)
{
	//{{AFX_DATA_INIT(CLogIn)
	m_strUser = _T("PETUSER");
	m_strCompany = _T("北京质子科技开发有限公司");
	m_strPassWord = _T("guest");
	//}}AFX_DATA_INIT
}


void CLogIn::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLogIn)
	DDX_Text(pDX, IDC_USER, m_strUser);
	DDX_Text(pDX, IDC_COMPANY, m_strCompany);
	DDX_Text(pDX, IDC_PASSWORD, m_strPassWord);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CLogIn, CDialog)
	//{{AFX_MSG_MAP(CLogIn)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLogIn message handlers
