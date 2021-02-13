// MaintDoc.h : interface of the CMaintDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAINTDOC_H__D7D6420B_BF45_11D6_9644_00E04C50B69B__INCLUDED_)
#define AFX_MAINTDOC_H__D7D6420B_BF45_11D6_9644_00E04C50B69B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "FileSave.h"

#define  TIMEOUT	2000

//-----------------文件操作宏-------------------------------//
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
#define		PARA_TIMEMODFY_DATA_ADDR		3300    //64*8
#define		PARA_TSERIES_DATA_ADDR			3812	//64
#define		PARA_CYSNUM_DATA_ADDR			3876    //64

#define		BLOCK_PLIMIT_DATA_SIZE			256	
#define		BLOCK_ETHLIMIT_DATA_SIZE		128
#define		BLOCK_ANOLPARA_DATA_SIZE		8
#define		BLOCK_TIMEMODFY_DATA_SIZE		64
#define		BLOCK_TSERIES_DATA_SIZE			64
#define		BLOCK_CYSNUM_DATA_SIZE			64



#define		DATA_SDOT_ADDR					4*1024
#define		DATA_ESPECT_ADDR				(4+128)*1024
#define		DATA_TIMEMODE_ADDR				(4+128+128)*1024
#define		DATA_FUHEMODE_ADDR				(4+128+128+128)*1024
#define		BLOCK_SDOT_SIZE					128*1024
#define		BLOCK_ESPECT_SIZE				128*1024	
#define		BLOCK_TIMEMODE_SIZE				128*1024
#define		BLOCK_FUHEMODE_SIZE				128*1024	


union TYPE 
{
	UINT intdata;
	unsigned char uchdata[4];
};


class CMaintDoc : public CDocument
{
protected: // create from serialization only
	CMaintDoc();
	DECLARE_DYNCREATE(CMaintDoc)

// Attributes
public:

// Operations
public:
	COLORREF GetPaperColor() const { return m_paperColor; }
	

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMaintDoc)
	public:
	virtual void Serialize(CArchive& ar);

	//}}AFX_VIRTUAL

// Implementation
public:


	//文件操作
	BOOL  bNewfilepackage;
	int	  iBlockSel;
	BOOL  bEnReadOperate;

	BOOL  bFirstFileOpened;



	long sBufDot[4096*4*2*2*4];
	long BuffDot3w[8][64][64]; //【块号】【Y坐标】【X坐标】
	int	 ibuffdot3w[8][64][64]; //【块号】【Y坐标】【X坐标】

	unsigned char m_chrAnolParam[8][10];

	long temp;
	long tFillData[32];

	bool bFileIsSample;

	unsigned int itemp[100];
	unsigned char uchartemp[300];

	virtual ~CMaintDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

	COLORREF m_paperColor;
// Generated message map function
public:
	afx_msg void OnFileOpen();
	afx_msg void OnMenuSaveAs();
protected:
	//{{AFX_MSG(CMaintDoc)
//	afx_msg void OnFileOpen();
//	afx_msg void OnMenuSaveAs();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINTDOC_H__D7D6420B_BF45_11D6_9644_00E04C50B69B__INCLUDED_)
