// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "Maint.h"
#include "CommSet.h"

#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
	// TODO: add member initialization code here
	
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}

	m_wndToolBar.GetToolBarCtrl().SetButtonSize(CSize(45,53));
	CImageList	imageList;
	CBitmap		bitmap;
	// 加载位图，创建位图列表，大小为
	bitmap.LoadBitmap(IDB_BITMAP2);
	imageList.Create(38, 52, ILC_COLORDDB|ILC_MASK, 15, 1);
	imageList.Add(&bitmap, RGB(255,0,255));
	m_wndToolBar.GetToolBarCtrl().SetImageList(&imageList);
	imageList.Detach();
	bitmap.Detach();


	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}

	// TODO: Delete these three lines if you don't want the toolbar to
	//  be dockable
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndToolBar);
//akang	
	if(!m_wndShowBar.Create(this,IDD_SHOWBAR,
		CBRS_ALIGN_RIGHT,AFX_IDW_CONTROLBAR_LAST))//-2
	{
		TRACE0("Failed to create mapshowbar\n");
		return -1;
	}

	m_wndShowBar.EnableDocking(CBRS_ALIGN_RIGHT/*LEFT*/);
	DockControlBar(&m_wndShowBar);

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;



//	cs.cx=::GetSystemMetrics(SM_CXSCREEN)*450/500;//
//	cs.cy=::GetSystemMetrics(SM_CYSCREEN)*115/125;//*80/125

	int xx = ::GetSystemMetrics(SM_CXSCREEN);
	
	if (xx == 1024) {
		cs.cx=::GetSystemMetrics(SM_CXSCREEN)*450/500;//
		cs.cy=::GetSystemMetrics(SM_CYSCREEN)*115/125;//
	}
	if(xx == 800) {
		cs.cx=::GetSystemMetrics(SM_CXSCREEN)*417/500;//
		cs.cy=::GetSystemMetrics(SM_CYSCREEN)*120/125;//
	}
	if(xx == 1152) {
		cs.cx=::GetSystemMetrics(SM_CXSCREEN)*400/500;//
		cs.cy=::GetSystemMetrics(SM_CYSCREEN)*115/125;//
	}
	if(xx > 1152) {
		cs.cx=::GetSystemMetrics(SM_CXSCREEN)*350/500;//
		cs.cy=::GetSystemMetrics(SM_CYSCREEN)*105/125;//
	}


	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers

