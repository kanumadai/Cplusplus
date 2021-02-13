// InitACS.cpp : implementation file
//

#include "stdafx.h"
#include "simows.h"
#include "InitACSDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CInitACSDlg dialog


CInitACSDlg::CInitACSDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CInitACSDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CInitACSDlg)
	m_nCoinFmt = 0;
	m_nEcgPreAmp = 2;
	m_nEcgFreq = 4;
	m_bEnDbgBucket = TRUE;
	m_bEnEmc = TRUE;
	m_nTimeInstant = 12;
	m_nTimeMax = 60;
	m_nTimeMin = 48;
	m_nRodRate1 = 8;
	m_nRodRate2 = 12;
	m_nRodRate3 = 18;
	m_nAcqMode = 3;
	m_nAcqTime = 2000;
	m_bEnBucketSIO = FALSE;
	//}}AFX_DATA_INIT
}


void CInitACSDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CInitACSDlg)
	DDX_Radio(pDX, IDC_COIN_FMT_POS, m_nCoinFmt);
	DDX_Text(pDX, IDC_ECG_PREAMP, m_nEcgPreAmp);
	DDV_MinMaxInt(pDX, m_nEcgPreAmp, 0, 7);
	DDX_CBIndex(pDX, IDC_ECG_SAMPLEFREQ, m_nEcgFreq);
	DDX_Check(pDX, IDC_EN_DBGBUCKET, m_bEnDbgBucket);
	DDX_Check(pDX, IDC_EN_EMC, m_bEnEmc);
	DDX_Text(pDX, IDC_TIME_INSTANT, m_nTimeInstant);
	DDV_MinMaxInt(pDX, m_nTimeInstant, 4, 100);
	DDX_Text(pDX, IDC_TIME_MAX, m_nTimeMax);
	DDV_MinMaxInt(pDX, m_nTimeMax, 4, 100);
	DDX_Text(pDX, IDC_TIME_MIN, m_nTimeMin);
	DDV_MinMaxInt(pDX, m_nTimeMin, 4, 100);
	DDX_Text(pDX, IDC_RODRATE1, m_nRodRate1);
	DDV_MinMaxInt(pDX, m_nRodRate1, 6, 20);
	DDX_Text(pDX, IDC_RODRATE2, m_nRodRate2);
	DDV_MinMaxInt(pDX, m_nRodRate2, 6, 20);
	DDX_Text(pDX, IDC_RODRATE3, m_nRodRate3);
	DDV_MinMaxInt(pDX, m_nRodRate3, 6, 20);
	DDX_CBIndex(pDX, IDC_BUCKET_MODE, m_nAcqMode);
	DDX_Text(pDX, IDC_BUCKET_TIME, m_nAcqTime);
	DDV_MinMaxInt(pDX, m_nAcqTime, 10, 200000);
	DDX_Check(pDX, IDC_EN_BUCKETSIO, m_bEnBucketSIO);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CInitACSDlg, CDialog)
	//{{AFX_MSG_MAP(CInitACSDlg)
	ON_BN_CLICKED(IDC_DEFAULT, OnDefault)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CInitACSDlg message handlers

void CInitACSDlg::OnDefault() 
{
	// TODO: Add your control notification handler code here
	m_nCoinFmt = 0;
	m_nEcgPreAmp = 2;
	m_nEcgFreq = 4;
	m_bEnDbgBucket = TRUE;
	m_bEnEmc = TRUE;
	m_nTimeInstant = 12;
	m_nTimeMax = 60;
	m_nTimeMin = 48;
	m_nRodRate1 = 8;
	m_nRodRate2 = 12;
	m_nRodRate3 = 18;

	m_nAcqMode = 3;
	m_nAcqTime = 2000;

	UpdateData( FALSE );
}
