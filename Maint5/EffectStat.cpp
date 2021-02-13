// EffectStat.cpp : implementation file
//

#include "stdafx.h"
#include "Maint.h"
#include "EffectStat.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEffectStat dialog


CEffectStat::CEffectStat(CWnd* pParent /*=NULL*/)
	: CDialog(CEffectStat::IDD, pParent)
{

	m_Effect_SourceFilePath = " ";
	m_Effect_SourceFileName = "SrcFile";
	m_Effect_DestFilePath = " ";
	m_Effect_DestFileName = "DstFile";
	m_Effect_OutTextFilePath = " ";
	m_Effect_OutTextFileName = "OutText";

	m_iStartFileNo = 0;
	m_iEndFileNo = 31;
	m_bEncheck = true;

	m_bSel_Peak_Stat = true;

}


void CEffectStat::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CEffectStat)
	DDX_Control(pDX, IDC_CHECK_PEAK_ANOLPARA_SEL, m_checkPeak_AnolPara_sel);
	DDX_Control(pDX, IDC_CHECK_OUTRELATIVE_VALUE, m_checkRelativeValue);
	DDX_Text(pDX, IDC_EDIT_END_FILE, m_strEndFileNo);
	DDX_Text(pDX, IDC_EDIT_START_FILE, m_strStartFileNo);
	DDX_Text(pDX, IDC_EDIT_PEAK, m_strInPeakPosition);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CEffectStat, CDialog)
	//{{AFX_MSG_MAP(CEffectStat)
	ON_BN_CLICKED(IDC_EFFECT_RESOURCE_FILE, OnEffectResourceFile)
	ON_BN_CLICKED(IDC_EFFECT_DESTINATION_FILE, OnEffectDestinationFile)
	ON_BN_CLICKED(IDC_EFFECT_OUTTEXT_FILE, OnEffectOuttextFile)
	ON_BN_CLICKED(IDC_CHECK_PEAK_ANOLPARA_SEL, OnCheckPeak_AnolParaSel)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEffectStat message handlers

BOOL CEffectStat::OnInitDialog() 
{
	CDialog::OnInitDialog();	
	
	this->SetDlgItemText(IDC_EDIT_START_FILE,"0");
	this->SetDlgItemText(IDC_EDIT_END_FILE,"31");
	m_checkRelativeValue.SetCheck(1);

	this->GetDlgItem(IDC_CHECK_PEAK_ANOLPARA_SEL)->ShowWindow(false);
	
	if (!m_bEncheck) {
		if (m_bSel_Peak_Stat) { //峰位统计
			this->SetDlgItemText(IDC_EFFECT_RESOURCE_FILE,"A 文件名");
			this->SetDlgItemText(IDC_CHECK_PEAK_ANOLPARA_SEL,"另选：参数调整操作");
			m_checkRelativeValue.ShowWindow(false);
			
			this->GetDlgItem(IDC_EFFECT_DESTINATION_FILE)->ShowWindow(true);
			this->GetDlgItem(IDC_EFFECT_OUTTEXT_FILE)->ShowWindow(true);
			this->GetDlgItem(IDC_STATIC_OUTTEXT)->ShowWindow(true);
			this->GetDlgItem(IDC_STATIC_DESTINATION)->ShowWindow(true);
			this->GetDlgItem(IDC_STATIC_PEAK)->ShowWindow(false);
			this->GetDlgItem(IDC_EDIT_PEAK)->ShowWindow(false);
			
		}
		else { //模拟参数修改
			this->SetDlgItemText(IDC_EFFECT_RESOURCE_FILE,"模拟文件名");
			this->SetDlgItemText(IDC_CHECK_PEAK_ANOLPARA_SEL,"另选：峰位统计操作");
			m_checkRelativeValue.ShowWindow(false);
			
			this->GetDlgItem(IDC_EFFECT_DESTINATION_FILE)->ShowWindow(false);
			this->GetDlgItem(IDC_EFFECT_OUTTEXT_FILE)->ShowWindow(false);
			this->GetDlgItem(IDC_STATIC_OUTTEXT)->ShowWindow(false);
			this->GetDlgItem(IDC_STATIC_DESTINATION)->ShowWindow(false);
			this->SetDlgItemText(IDC_EDIT_PEAK,"42");
			m_iRefPeakPosition = 42;
		}
	}
	else {
		m_checkPeak_AnolPara_sel.ShowWindow(false);	
		this->GetDlgItem(IDC_STATIC_PEAK)->ShowWindow(false);
		this->GetDlgItem(IDC_EDIT_PEAK)->ShowWindow(false);
	}

	return TRUE;  
}


void CEffectStat::OnEffectResourceFile() 
{
	CFileDialog dlg(FALSE,"","");///TRUE为OPEN对话框，FALSE为SAVE AS对话框 	
	dlg.m_ofn.lpstrFilter = ("选择源文件名");//
	if(dlg.DoModal()==IDOK) { 
		m_Effect_SourceFilePath=dlg.GetPathName(); 
		m_Effect_SourceFileName = dlg.GetFileName();
	}  
	else return;	
	SetDlgItemText(IDC_STATIC_SOURCE,m_Effect_SourceFilePath + "*");	
}

void CEffectStat::OnEffectDestinationFile() 
{
	CFileDialog dlg(FALSE,"","");///TRUE为OPEN对话框，FALSE为SAVE AS对话框 	
	dlg.m_ofn.lpstrFilter = ("选择目标名");//
	if(dlg.DoModal()==IDOK) { 
		m_Effect_DestFilePath=dlg.GetPathName(); 
		m_Effect_DestFileName = dlg.GetFileName();
		
	}  
	else return;	
	SetDlgItemText(IDC_STATIC_DESTINATION,m_Effect_DestFilePath + "*");	
}	

void CEffectStat::OnEffectOuttextFile() 
{
	CFileDialog dlg(FALSE,"","");///TRUE为OPEN对话框，FALSE为SAVE AS对话框 	
	dlg.m_ofn.lpstrFilter = ("选择目标名");//
	if(dlg.DoModal()==IDOK) { 
		m_Effect_OutTextFilePath=dlg.GetPathName(); 
		m_Effect_OutTextFileName = dlg.GetFileName();
		
	}  
	else return;
	SetDlgItemText(IDC_STATIC_OUTTEXT,m_Effect_OutTextFilePath + ".txt");

}

void CEffectStat::OnOK() 
{
	UpdateData(true);
	if (!m_strStartFileNo.IsEmpty()) {
		m_iStartFileNo = _atoi64(m_strStartFileNo);
		if ((m_iStartFileNo >= 32)||(m_iStartFileNo < 0)) {
			SetDlgItemText(IDC_EDIT_START_FILE,"0");
			m_iStartFileNo = 0;
		}
	} 
	else{
		SetDlgItemText(IDC_EDIT_START_FILE,"0");
		m_iStartFileNo = 0;
	}

	if (!m_strEndFileNo.IsEmpty()) {
		m_iEndFileNo = _atoi64(m_strEndFileNo);
		if ((m_iEndFileNo >= 32)||(m_iEndFileNo < 0)) {
			SetDlgItemText(IDC_EDIT_END_FILE,"0");
			m_iEndFileNo = 0;
		}
	} 
	else{
		SetDlgItemText(IDC_EDIT_END_FILE,"0");
		m_iEndFileNo = 0;
	}
	
	
	if( m_checkRelativeValue.GetCheck() == 1 ) {
		m_bOutRelativeValue = true;
	}
	else m_bOutRelativeValue = false;
	

	if (!m_bSel_Peak_Stat) { //模拟参数调整
		if(m_Effect_SourceFilePath == " ") {
			AfxMessageBox("需要选择修改文件的路径名");
			return;
		}
	
		m_iRefPeakPosition = _atoi64(m_strInPeakPosition);
		if ((m_iRefPeakPosition>63)||(m_iRefPeakPosition<2)) {
			m_strInPeakPosition = "42";
			m_iRefPeakPosition = 42;
			UpdateData(false);
		}

		CDialog::OnOK();	
		return;
	}

	//峰位统计
	if ((m_Effect_SourceFilePath == " ")||(m_Effect_DestFilePath == " ")
		||(m_Effect_OutTextFilePath == " ")) {
		AfxMessageBox("需要选择文件路径名");
		return;
	}
		
	CDialog::OnOK();
}



void CEffectStat::OnCheckPeak_AnolParaSel() 
{
	m_bSel_Peak_Stat = !(m_bSel_Peak_Stat);

	if (m_bSel_Peak_Stat) { //峰位统计
		this->SetDlgItemText(IDC_EFFECT_RESOURCE_FILE,"A 文件名");
		this->SetDlgItemText(IDC_CHECK_PEAK_ANOLPARA_SEL,"另选：参数调整操作");
		m_checkRelativeValue.ShowWindow(false);

		this->GetDlgItem(IDC_EFFECT_DESTINATION_FILE)->ShowWindow(true);
		this->GetDlgItem(IDC_EFFECT_OUTTEXT_FILE)->ShowWindow(true);
		this->GetDlgItem(IDC_STATIC_OUTTEXT)->ShowWindow(true);
		this->GetDlgItem(IDC_STATIC_DESTINATION)->ShowWindow(true);

	}
	else { //模拟参数修改
		this->SetDlgItemText(IDC_EFFECT_RESOURCE_FILE,"模拟文件名");
		this->SetDlgItemText(IDC_CHECK_PEAK_ANOLPARA_SEL,"另选：峰位统计操作");
		m_checkRelativeValue.ShowWindow(false);

		this->GetDlgItem(IDC_EFFECT_DESTINATION_FILE)->ShowWindow(false);
		this->GetDlgItem(IDC_EFFECT_OUTTEXT_FILE)->ShowWindow(false);
		this->GetDlgItem(IDC_STATIC_OUTTEXT)->ShowWindow(false);
		this->GetDlgItem(IDC_STATIC_DESTINATION)->ShowWindow(false);
	}


}
