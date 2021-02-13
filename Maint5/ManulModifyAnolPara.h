//{{AFX_INCLUDES()
#include "msflexgrid.h"
//}}AFX_INCLUDES
#if !defined(AFX_MANULMODIFYANOLPARA_H__89F41FDA_C217_444F_A0F6_F85C3875B15C__INCLUDED_)
#define AFX_MANULMODIFYANOLPARA_H__89F41FDA_C217_444F_A0F6_F85C3875B15C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ManulModifyAnolPara.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CManulModifyAnolPara dialog

class CManulModifyAnolPara : public CDialog
{
// Construction
public:
	BOOL OnFastPeakStat(int buckno,CString filename,int* pPeakPosition);
	void OnReadAnolPara(int buckno, int lineNo,CString filename);
	CManulModifyAnolPara(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CManulModifyAnolPara)
	enum { IDD = IDD_ANOL_PARA_MANUL_MODIFY };
	CEdit	m_ManulModify_Edit;
	CMSFlexGrid	m_Grid;
	CString	m_strStartFileNo;
	CString	m_strEndFileNo;
	CString	m_strABCDModify;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CManulModifyAnolPara)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

protected:
	CString strBuffInput;

	CString m_strOperating_32bkPeakFilePath;
	CString m_SourceFilePath; 
	CString m_SourceFileName;

	unsigned char m_AnolParaData[32][8][8];  //模拟数据 BucketNo, Block, [A,B,C,..]

	int m_ReadBucketIndex[32];  //读取Bucket的索引表
	int m_iStatCounter;

	int m_iStartFileNo;
	int m_iEndFileNo;

public:
	unsigned char m_arrAnolParaPreData[32][8][10];  //[bucket][block][anolpara]


// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CManulModifyAnolPara)
	afx_msg void OnReadData();
	afx_msg void OnSaveData();
	virtual BOOL OnInitDialog();
	afx_msg void OnDblClickMsflexgrid1();
	afx_msg void OnKillfocusManulmodityEdit();
	afx_msg void OnLoadFilename();
	afx_msg void OnClickMsflexgrid1();
	afx_msg void OnAbcdModify();
	afx_msg void OnScrollMsflexgrid1();
	afx_msg void OnOffsetModify();
	afx_msg void OnManualModPre();
	afx_msg void OnManualMod();
	DECLARE_EVENTSINK_MAP()
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MANULMODIFYANOLPARA_H__89F41FDA_C217_444F_A0F6_F85C3875B15C__INCLUDED_)
