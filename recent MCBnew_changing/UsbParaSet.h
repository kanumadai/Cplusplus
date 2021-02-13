#if !defined(AFX_USBPARASET_H__15094533_A2E4_4294_89D9_543D72682797__INCLUDED_)
#define AFX_USBPARASET_H__15094533_A2E4_4294_89D9_543D72682797__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// UsbParaSet.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CUsbParaSet dialog

class CUsbParaSet : public CDialog
{
// Construction
public:
	CUsbParaSet(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CUsbParaSet)
	enum { IDD = IDD_PARA_SET_USB };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CUsbParaSet)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CUsbParaSet)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_USBPARASET_H__15094533_A2E4_4294_89D9_543D72682797__INCLUDED_)
