#if !defined(AFX_FILESAVE_H__9BA76822_1DD1_42BD_9598_81CECEF22875__INCLUDED_)
#define AFX_FILESAVE_H__9BA76822_1DD1_42BD_9598_81CECEF22875__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FileSave.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CFileSave dialog
//-----------------ÎÄ¼þ²Ù×÷ºê-------------------------------//
#define		FILEHEAD_LENTH_ADDR				0
#define		FILEHEAD_TYPE_ADDR				4
#define		FILEHEAD_PLIMIT_ATTR_ADDR		6
#define		FILEHEAD_ELIMIT_ATTR_ADDR		7
#define		FILEHEAD_ANOLPARA_ATTR_ADDR		8

#define		FILEHEAD_SDOT_ATTR_ADDR				20
#define		FILEHEAD_ENERGESPECTRUM_ATTR_ADDR	21
#define		FILEHEAD_TIMEMODE_ATTR_ADDR			22
#define		FILEHEAD_FUHEMODE_ATTR_ADDR			23

#define		PARA_PLIMIT_DATA_ADDR			128
#define		PARA_ETHLIMIT_DATA_ADDR			2176	//128+256*8
#define		PARA_ANOLPARA_DATA_ADDR			3200	//128+256*8 + 128*8
#define		BLOCK_PLIMIT_DATA_SIZE			256	
#define		BLOCK_ETHLIMIT_DATA_SIZE		128
#define		BLOCK_ANOLPARA_DATA_SIZE		8

#define		DATA_SDOT_ADDR					4*1024
#define		DATA_ESPECT_ADDR				(4+128)*1024
#define		DATA_TIMEMODE_ADDR				(4+128+128)*1024
#define		DATA_FUHEMODE_ADDR				(4+128+128+128)*1024
#define		BLOCK_SDOT_SIZE					16*1024
#define		BLOCK_ESPECT_SIZE				16*1024	
#define		BLOCK_TIMEMODE_SIZE				16*1024
#define		BLOCK_FUHEMODE_SIZE				16*1024	



class CFileSave : public CDialog
{
// Construction
public:
	CFileSave(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CFileSave)
	enum { IDD = IDD_FILE_SAVE };
	CComboBox	m_BlockSel;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFileSave)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
public:
	BOOL  bNewfilepackage;
	int	  iBlockSel;
	BOOL  bEnReadOperate;

protected:

	// Generated message map functions
	//{{AFX_MSG(CFileSave)
	afx_msg void OnEditFilepack();
	afx_msg void OnNewFilepack();
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FILESAVE_H__9BA76822_1DD1_42BD_9598_81CECEF22875__INCLUDED_)
