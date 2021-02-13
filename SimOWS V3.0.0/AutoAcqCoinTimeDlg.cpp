// AcqCoinTimeDlg.cpp : implementation file
//

#include "stdafx.h"
#include "simows.h"
#include "AutoAcqCoinTimeDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAutoAcqCoinTimeDlg dialog


CAutoAcqCoinTimeDlg::CAutoAcqCoinTimeDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAutoAcqCoinTimeDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAutoAcqCoinTimeDlg)
	m_BucketBegin = 0;
	m_BucketEnd = 31;
	m_strPath = _T("");
	m_dwEventCnt = 1000000;
	m_dwStopTime = 60;
	m_bStopByTime = 1;
	m_bRodOut = 0;
	m_bSeptaOut = 1;
	//}}AFX_DATA_INIT
}


void CAutoAcqCoinTimeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAutoAcqCoinTimeDlg)
	DDX_Text(pDX, IDC_BUCKET_BEGIN, m_BucketBegin);
	DDV_MinMaxInt(pDX, m_BucketBegin, 0, 31);
	DDX_Text(pDX, IDC_BUCKET_END, m_BucketEnd);
	DDV_MinMaxInt(pDX, m_BucketEnd, 0, 31);
	DDX_Text(pDX, IDC_SAVEPATH, m_strPath);
	DDX_Text(pDX, IDC_STOP_EVENTCNT, m_dwEventCnt);
	DDV_MinMaxUInt(pDX, m_dwEventCnt, 1000, 1000000000);
	DDX_Text(pDX, IDC_STOP_TIME, m_dwStopTime);
	DDV_MinMaxUInt(pDX, m_dwStopTime, 1, 1000000000);
	DDX_Radio(pDX, IDC_STOPONCNT, m_bStopByTime);
	DDX_Radio(pDX, IDC_ROD_IN, m_bRodOut);
	DDX_Radio(pDX, IDC_SEPTA_IN, m_bSeptaOut);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAutoAcqCoinTimeDlg, CDialog)
	//{{AFX_MSG_MAP(CAutoAcqCoinTimeDlg)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAutoAcqCoinTimeDlg message handlers
