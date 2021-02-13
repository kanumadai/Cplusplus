#if !defined(AFX_EVIEW_H__6E9D9956_C145_476F_8EE3_7130F7A3C2E3__INCLUDED_)
#define AFX_EVIEW_H__6E9D9956_C145_476F_8EE3_7130F7A3C2E3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EView.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CEView view

class CEView : public CEditView
{
protected:
	CEView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CEView)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEView)
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CEView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(CEView)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EVIEW_H__6E9D9956_C145_476F_8EE3_7130F7A3C2E3__INCLUDED_)
