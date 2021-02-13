// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "MCB.h"
#include "MainFrm.h"
#include "MCBView.h"
#include "EView.h"

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
	ON_UPDATE_COMMAND_UI(IDC_QUERY_TIMEOUT, OnUpdateIdClearBuff)
	ON_UPDATE_COMMAND_UI(IDD_PRINT_PRE, OnUpdatePrintPre)
	ON_WM_CLOSE()
	ON_UPDATE_COMMAND_UI(AFX_ID_STARTSAMPLE, OnUpdateIdStartsample)
	ON_UPDATE_COMMAND_UI(AFX_ID_OUT_REPORT, OnUpdateIdOutReport)
	ON_UPDATE_COMMAND_UI(AFX_ID_TESTSAMPLE, OnUpdateIdTestsample)
	ON_UPDATE_COMMAND_UI(AFX_ID_SENDSAMPLE, OnUpdateIdSendsample)
	ON_UPDATE_COMMAND_UI(AFX_ID_RESETSAMPLE, OnUpdateIdResetsample)
	ON_UPDATE_COMMAND_UI(AFX_ID_RECEIVESAMPLE, OnUpdateIdReceivesample)
	ON_UPDATE_COMMAND_UI(AFX_ID_CONTINUESAMPLE, OnUpdateIdContinuesample)
	ON_UPDATE_COMMAND_UI(AFX_ID_WHOLEDATA_SUM, OnUpdateIdWholedataSum)
	ON_UPDATE_COMMAND_UI(AFX_ID_WHOLEDATA_FILTER, OnUpdateIdWholedataFilter)
	ON_UPDATE_COMMAND_UI(AFX_ID_INTEREST_SUM, OnUpdateIdInterestSum)
	ON_UPDATE_COMMAND_UI(AFX_ID_ENERGE_KEDU, OnUpdateIdEnergeKedu)
	ON_UPDATE_COMMAND_UI(AFX_ID_WIDTH_STAT, OnUpdateIdWidthStat)
	ON_UPDATE_COMMAND_UI(ID_MENU_FILE_SAVE_AS, OnUpdateMenuFileSaveAs)
	ON_UPDATE_COMMAND_UI(ID_FILE_PRINT, OnUpdateFilePrint)
    ON_UPDATE_COMMAND_UI(IDC_EXTERNALDATA_MCB2BUFF, OnUpdateEXTERNALDATA_MCB2BUFF)
	ON_WM_MOVING()
	ON_WM_SIZING()
	ON_WM_SIZE()
	ON_WM_KEYUP()
	ON_UPDATE_COMMAND_UI(AFX_ID_CLEAR_BUFF, OnUpdateIdClearBuff)
	ON_WM_HSCROLL()
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
	
	if (!m_wndToolBar.CreateEx(this,TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP//TBSTYLE_FLAT 
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||   //
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}
//////////////////////////////////////////////////////////////////////////////changing
	m_wndToolBar.GetToolBarCtrl().SetButtonSize(CSize(25,20));//50,55
//	CImageList	imageList;
//	CBitmap		bitmap;
	// 加载位图，创建位图列表，大小为
/*	bitmap.LoadBitmap(IDB_BITMAP1);//IDB_BITMAP2
	imageList.Create(24,20, ILC_COLORDDB|ILC_MASK, 15, 1);
	imageList.Add(&bitmap, RGB(255,0,255));
	m_wndToolBar.GetToolBarCtrl().SetImageList(&imageList);
	imageList.Detach();
	bitmap.Detach();
	
/*    CImageList	imageList;
	CBitmap		bitmap;

	bitmap.LoadBitmap(IDB_BITMAP4);
	imageList.Create(21, 20, ILC_COLORDDB|ILC_MASK, 15, 1);
	imageList.Add(&bitmap, RGB(255,0,255));
	m_wndToolBar.SendMessage(TB_SETIMAGELIST, 0, (LPARAM)imageList.m_hImageList);
	imageList.Detach();
	bitmap.Detach();

	bitmap.LoadBitmap(IDB_BITMAP3);
	imageList.Create(21, 20, ILC_COLORDDB|ILC_MASK, 15, 1);
	imageList.Add(&bitmap, RGB(255,0,255));
	m_wndToolBar.SendMessage(TB_SETHOTIMAGELIST, 0, (LPARAM)imageList.m_hImageList);
	imageList.Detach();
	bitmap.Detach();      */
//////////////////////////////////////////////////////////////////////////////////////////////

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

//	
//akang	
	if(!m_wndDlgBar.Create(this,IDD_DLGBAR,
		CBRS_RIGHT|CBRS_TOOLTIPS|CBRS_FLYBY, IDD_DLGBAR))
	{
		TRACE0("Failed to create mapshowbar\n");
		return -1;
	}
	
//	m_wndDlgBar.GrayCtlColor(this,this,123);
    

	m_wndDlgBar.EnableDocking(CBRS_ALIGN_RIGHT/*LEFT*/);
	DockControlBar(&m_wndDlgBar);
     
	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	int xx = ::GetSystemMetrics(SM_CXSCREEN);
	
	if (xx == 1024) {
		cs.cx=::GetSystemMetrics(SM_CXSCREEN)*500/500;
		cs.cy=::GetSystemMetrics(SM_CYSCREEN)*241/250;
	}
	if(xx == 800) {
		cs.cx=::GetSystemMetrics(SM_CXSCREEN)*500/500;//
		cs.cy=::GetSystemMetrics(SM_CYSCREEN)*239/250;//
	}
	if(xx == 1152) {
		cs.cx=::GetSystemMetrics(SM_CXSCREEN)*500/500;//
		cs.cy=::GetSystemMetrics(SM_CYSCREEN)*250/250;//
	}
	if(xx == 1280) {
		cs.cx=::GetSystemMetrics(SM_CXSCREEN)*450/500;//
		cs.cy=::GetSystemMetrics(SM_CYSCREEN)*220/250;//
	}
	if(xx == 1600) {
		cs.cx=::GetSystemMetrics(SM_CXSCREEN)*450/500;//
		cs.cy=::GetSystemMetrics(SM_CYSCREEN)*220/250;//
	}    
	cs.x = 0;
	cs.y = 0;
	
//	cs.style = WS_SYSMENU | WS_MINIMIZEBOX;	 //确定窗口是否允许改变

	

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


void CMainFrame::OnUpdateIdClearBuff(CCmdUI* pCmdUI) 
{
	CFrameWnd *pFram=(CFrameWnd *)GetActiveFrame();
	CMCBView *view=(CMCBView *)pFram->GetActiveView();
	if (view->m_startflag&&view->m_bMCBModeSel)
	{
//		pCmdUI->Enable(FALSE);
	}
	else;
     pCmdUI->Enable(TRUE);
}

void CMainFrame::OnUpdatePrintPre(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	CFrameWnd *pFram=(CFrameWnd *)GetActiveFrame();
	CMCBView *view=(CMCBView *)pFram->GetActiveView();
	if (view->m_startflag&&view->m_bMCBModeSel)
	{
		pCmdUI->Enable(FALSE);
	}
	else
        pCmdUI->Enable(TRUE);
}

void CMainFrame::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	CFrameWnd *pFram=(CFrameWnd *)GetActiveFrame();
	CMCBView *view=(CMCBView *)pFram->GetActiveView();
  if (!(view->m_bPrintMapEn))
  {	
	  view->CloseSave();
	  view->ResumeW();
  }
	CFrameWnd::OnClose();
}

void CMainFrame::OnUpdateIdStartsample(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	CFrameWnd *pFram=(CFrameWnd *)GetActiveFrame();
	CMCBView *view=(CMCBView *)pFram->GetActiveView();
	
	if (view->m_startflag||view->m_icontinueflag)	pCmdUI->Enable(FALSE);
	else  pCmdUI->Enable(TRUE);

}

void CMainFrame::OnUpdateIdOutReport(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	CFrameWnd *pFram=(CFrameWnd *)GetActiveFrame();
	CMCBView *view=(CMCBView *)pFram->GetActiveView();
	if ((view->m_startflag||view->m_continueflag)&&(view->m_bMCBModeSel))
	{
		pCmdUI->Enable(FALSE);
	}
	else
      pCmdUI->Enable(TRUE);
}

void CMainFrame::OnUpdateIdTestsample(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	CFrameWnd *pFram=(CFrameWnd *)GetActiveFrame();
	CMCBView *view=(CMCBView *)pFram->GetActiveView();
	if (view->m_startflag||view->m_continueflag)
	{
		pCmdUI->Enable(FALSE);
	}
	else
      pCmdUI->Enable(TRUE);
}

void CMainFrame::OnUpdateIdSendsample(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	CFrameWnd *pFram=(CFrameWnd *)GetActiveFrame();
	CMCBView *view=(CMCBView *)pFram->GetActiveView();
	if (view->m_startflag||view->m_icontinueflag)
	{
		pCmdUI->Enable(FALSE);
	}
	else
      pCmdUI->Enable(TRUE);
}

void CMainFrame::OnUpdateIdResetsample(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	CFrameWnd *pFram=(CFrameWnd *)GetActiveFrame();
	CMCBView *view=(CMCBView *)pFram->GetActiveView();
	if (view->m_startflag||view->m_icontinueflag)
	{
		pCmdUI->Enable(FALSE);
	}
	else
      pCmdUI->Enable(TRUE);
}

void CMainFrame::OnUpdateIdReceivesample(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	CFrameWnd *pFram=(CFrameWnd *)GetActiveFrame();
	CMCBView *view=(CMCBView *)pFram->GetActiveView();
	if (view->m_startflag||view->m_icontinueflag)
	{
		pCmdUI->Enable(FALSE);
	}
	else
      pCmdUI->Enable(TRUE);
}

void CMainFrame::OnUpdateIdContinuesample(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	CFrameWnd *pFram=(CFrameWnd *)GetActiveFrame();
	CMCBView *view=(CMCBView *)pFram->GetActiveView();
	if (view->m_startflag||view->m_icontinueflag)
	{
		pCmdUI->Enable(FALSE);
	}
	else
      pCmdUI->Enable(TRUE);
}

void CMainFrame::OnUpdateIdWholedataSum(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	CFrameWnd *pFram=(CFrameWnd *)GetActiveFrame();
	CMCBView *view=(CMCBView *)pFram->GetActiveView();
	
	if((view->m_startflag||view->m_continueflag)&&(view->m_bMCBModeSel))pCmdUI->Enable(FALSE);
	else  pCmdUI->Enable(TRUE);
}

void CMainFrame::OnUpdateIdWholedataFilter(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	CFrameWnd *pFram=(CFrameWnd *)GetActiveFrame();
	CMCBView *view=(CMCBView *)pFram->GetActiveView();
	
	if((view->m_startflag||view->m_continueflag)&&(view->m_bMCBModeSel))	pCmdUI->Enable(FALSE);
	else  pCmdUI->Enable(TRUE);
}

void CMainFrame::OnUpdateIdInterestSum(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	CFrameWnd *pFram=(CFrameWnd *)GetActiveFrame();
	CMCBView *view=(CMCBView *)pFram->GetActiveView();
	
	if((view->m_startflag||view->m_continueflag)&&(view->m_bMCBModeSel))	pCmdUI->Enable(FALSE);
	else  pCmdUI->Enable(TRUE);
}

void CMainFrame::OnUpdateIdEnergeKedu(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	CFrameWnd *pFram=(CFrameWnd *)GetActiveFrame();
	CMCBView *view=(CMCBView *)pFram->GetActiveView();
	if((view->m_startflag||view->m_continueflag)&&(view->m_bMCBModeSel))	pCmdUI->Enable(FALSE);
	else  pCmdUI->Enable(TRUE);
}

void CMainFrame::OnUpdateIdWidthStat(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	CFrameWnd *pFram=(CFrameWnd *)GetActiveFrame();
	CMCBView *view=(CMCBView *)pFram->GetActiveView();
	
	if ((view->m_startflag||view->m_continueflag)&&(view->m_bMCBModeSel))	pCmdUI->Enable(FALSE);
	else  pCmdUI->Enable(TRUE);
}

void CMainFrame::OnUpdateMenuFileSaveAs(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	CFrameWnd *pFram=(CFrameWnd *)GetActiveFrame();
	CMCBView *view=(CMCBView *)pFram->GetActiveView();
	
	if (view->m_bMCBModeSel)	pCmdUI->Enable(FALSE);
	else  pCmdUI->Enable(TRUE);
}

void CMainFrame::OnUpdateFilePrint(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	CFrameWnd *pFram=(CFrameWnd *)GetActiveFrame();
	CMCBView *view=(CMCBView *)pFram->GetActiveView();
	
	if((view->m_startflag||view->m_continueflag)&&(view->m_bMCBModeSel))	pCmdUI->Enable(FALSE);
	else  pCmdUI->Enable(TRUE);
}

BOOL CMainFrame::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext) 
{
	// TODO: Add your specialized code here and/or call the base class
	return CFrameWnd::OnCreateClient(lpcs, pContext);
}

void CMainFrame::OnMoving(UINT fwSide, LPRECT pRect) 
{
	CFrameWnd::OnMoving(fwSide, pRect);
	
	// TODO: Add your message handler code here
	CFrameWnd *pFram=(CFrameWnd *)GetActiveFrame();
	CMCBView *view=(CMCBView *)pFram->GetActiveView();
	if(view->m_scrollflag)
	{
		view->m_ScrollBar.ShowScrollBar(FALSE);
		view->m_ScrollBar.ShowScrollBar(TRUE);
	}

}

void CMainFrame::OnSizing(UINT fwSide, LPRECT pRect) 
{
	CFrameWnd::OnSizing(fwSide, pRect);
	
	// TODO: Add your message handler code here

}

void CMainFrame::OnSize(UINT nType, int cx, int cy) 
{
	CFrameWnd::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	
}

void CMainFrame::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	// TODO: Add your message handler code here and/or call default

	CFrameWnd::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CMainFrame::OnUpdateEXTERNALDATA_MCB2BUFF(CCmdUI* pCmdUI)
{	
	CFrameWnd *pFram=(CFrameWnd *)GetActiveFrame();
	CMCBView *view=(CMCBView *)pFram->GetActiveView();

	if (view->m_startflag||view->m_continueflag)	pCmdUI->Enable(FALSE);
	else  pCmdUI->Enable(TRUE);
}