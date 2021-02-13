// Input1.cpp : implementation file
//

#include "stdafx.h"
#include "SimOWS.h"
#include "Input1.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CInput1 dialog


CInput1::CInput1(CWnd* pParent /*=NULL*/)
	: CDialog(CInput1::IDD, pParent)
{
	//{{AFX_DATA_INIT(CInput1)
	m_wValue = 100;
	//}}AFX_DATA_INIT
}


void CInput1::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CInput1)
	DDX_Text(pDX, IDC_EDIT_WORD, m_wValue);
	DDV_MinMaxInt(pDX, m_wValue, 0, 30000);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CInput1, CDialog)
	//{{AFX_MSG_MAP(CInput1)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CInput1 message handlers
