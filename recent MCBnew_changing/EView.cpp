// EView.cpp : implementation file
//

#include "stdafx.h"
#include "MCB.h"
#include "EView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEView

IMPLEMENT_DYNCREATE(CEView, CEditView)

CEView::CEView()
{
}

CEView::~CEView()
{
}


BEGIN_MESSAGE_MAP(CEView, CEditView)
	//{{AFX_MSG_MAP(CEView)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEView drawing

void CEView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: add draw code here
}

/////////////////////////////////////////////////////////////////////////////
// CEView diagnostics

#ifdef _DEBUG
void CEView::AssertValid() const
{
	CEditView::AssertValid();
}

void CEView::Dump(CDumpContext& dc) const
{
	CEditView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CEView message handlers
