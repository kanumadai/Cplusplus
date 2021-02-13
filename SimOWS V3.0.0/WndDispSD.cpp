// WndDispSD.cpp : implementation file
//

#include "stdafx.h"
#include "SimOWS.h"
#include "WndDispSD.h"
#include "BuckFile.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CWndDispSD

CWndDispSD::CWndDispSD()
{
	for(int i=0; i<8; i++)
	{
		m_pSDBuf[i] = (LONG*)GlobalAlloc(GMEM_FIXED, 64*64*4);
		ASSERT(m_pSDBuf != NULL);
		memset(m_pSDBuf[i], 0, 4*64*64);

		m_pDispBuf[i] = (BYTE*)GlobalAlloc(GMEM_FIXED, 64*64);
		ASSERT(m_pDispBuf != NULL);
		memset(m_pDispBuf[i], 0, 64*64);
	}
	m_pDispBuf[0][64*64-1] = 63;

	m_currBlock = 0;   
	m_nMaxCnt = 0;
	m_bDefaultPAL = TRUE;
}

CWndDispSD::~CWndDispSD()
{
	for(int i=0; i<8; i++){
		GlobalFree( m_pSDBuf[i] );
		GlobalFree( m_pDispBuf[i] );
	}
}


BEGIN_MESSAGE_MAP(CWndDispSD, CWndDisp)
	//{{AFX_MSG_MAP(CWndDispSD)
	ON_WM_CREATE()
	ON_WM_PAINT()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_MOUSEMOVE()
	ON_COMMAND(IDM_PAL1_DEFAULT, OnPalDefault)
	ON_COMMAND(IDM_PAL1_FLOW, OnPalFlow)
	ON_COMMAND(IDM_PAL1_HOT, OnPalHot)
	ON_COMMAND(IDM_PAL1_MAP, OnPalMap)
	ON_COMMAND(IDM_PAL1_NIH, OnPalNih)
	ON_COMMAND(IDM_PAL1_256, OnPal256)
	ON_COMMAND(IDM_SAVE1, OnSave)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CWndDispSD message handlers
void CWndDispSD::CreateWnd()
{
	m_hBkBrush = CreateSolidBrush(RGB(255,255,255));
	CreateEx(WS_EX_TOOLWINDOW|WS_EX_OVERLAPPEDWINDOW, 
		     AfxRegisterWndClass(CS_VREDRAW|CS_HREDRAW|CS_DBLCLKS, 0, m_hBkBrush, 0),
			 "散点图",
			 WS_CAPTION|WS_BORDER | WS_POPUP,// |WS_VISIBLE | WS_HSCROLL,
			 0,0,512,512,
			 GetDesktopWindow()->m_hWnd,
			 NULL);

}

int CWndDispSD::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWndDisp::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	//set client rect to with=64*4*4+5*10, high=64*4*2+3*10
	m_menu.LoadMenu(IDR_SD);
	m_menu.CheckMenuItem(IDM_PAL1_DEFAULT,MF_CHECKED);
	m_bDefaultPAL = TRUE;

#define WND_WIDTH (64*4*4+5*10)
#define WND_HIGH  (64*4*2+3*10)
	RECT wndRect,clientRect;

	GetWindowRect(&wndRect);
	GetClientRect(&clientRect);
	wndRect.right = wndRect.right + (WND_WIDTH-(clientRect.right-clientRect.left));
	wndRect.bottom = wndRect.bottom + (WND_HIGH-(clientRect.bottom-clientRect.top));
	MoveWindow(&wndRect);
	GetClientRect(&clientRect);

	CDC* pClientDC = GetDC();
	
	m_DCClient.CreateCompatibleDC(pClientDC);
	m_bmpClient.CreateCompatibleBitmap(pClientDC, clientRect.right-clientRect.left, clientRect.bottom-clientRect.top);
	m_DCClient.SelectObject(&m_bmpClient);

	m_DCBlk.CreateCompatibleDC(pClientDC);
	m_bmpBlk.CreateCompatibleBitmap(pClientDC, 64, 64);
	m_DCBlk.SelectObject(&m_bmpBlk);

	m_GridPen.CreatePen(PS_SOLID, 1, RGB(128,128,128) );

	m_currBlock = 0;
	CDC* pDC = GetDC();
	Draw(pDC);

	return 0;
}

void CWndDispSD::ResetDat()
{
	for(int i=0; i<8; i++){
		memset( m_pSDBuf[i], 0, 64*64*sizeof(long) );
		memset( m_pDispBuf[i], 0, 64*64*sizeof(BYTE) );
	}

	m_bDispAll = TRUE;
	m_currBlock = 0;
	m_nMaxCnt = 256;

	Draw(GetDC());
}

void CWndDispSD::ShowDat(void *pBuf, int len)
{
   	int pos,block;
	SD_ROWDAT* pDat= (SD_ROWDAT*)pBuf;
			 
    for(int i=0; i<len/4; i++){
		if( !pDat[i].valid ) continue;
	    block = pDat[i].block;
	    pos = pDat[i].y*64 + pDat[i].x;
	    if( m_pSDBuf[block][pos] < 16*1024*1024 ) 
	 	    m_pSDBuf[block][pos]++;
		if( m_pSDBuf[block][pos] > m_nMaxCnt) m_nMaxCnt = m_pSDBuf[block][pos]; 
	}

	DatFmt();
	Draw(GetDC());

	char szTemp[128];
	sprintf(szTemp, "散点图 -- MaxValue=%d", m_nMaxCnt);
	SetWindowText(szTemp);
}

void CWndDispSD::DatFmt()
{
	int index;
	for(int block=0; block<8; block++){
		for(int i=0; i<64*64; i++){
			index = ColorTrans(m_pSDBuf[block][i], m_nMaxCnt);
			if( m_bDefaultPAL )
				ASSERT(index>=0 && index<64);
			else
				ASSERT(index>=0 && index<256);
			m_pDispBuf[block][i] = index;
		}
	}
}

void CWndDispSD::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	//Draw(&dc);
	RECT rect;
	GetClientRect(&rect);
	dc.BitBlt(0,0, rect.right-rect.left, rect.bottom-rect.top,
		        &m_DCClient, 0,0, SRCCOPY);
}

void CWndDispSD::GetBlockRect(int nBlock, RECT *pRect)
{
	int x = nBlock%4;
	int y = nBlock/4;

	pRect->left = x*64*4 + (x+1)*10;
	pRect->right = pRect->left+64*4;

	pRect->top = y*64*4 + (y+1)*10;
	pRect->bottom = pRect->top+64*4;
}

void CWndDispSD::Draw(CDC *pDC)
{
	RECT rect;
	char szTemp[32];
	GetClientRect(&rect);
	::FillRect(m_DCClient.m_hDC, &rect, m_hBkBrush);

	if( m_bDispAll ){
		for(int i=0; i<8; i++){
			GetBlockRect(i, &rect);

			StretchDIBits(m_DCBlk.m_hDC, 
						  0,0,64,64,
						  0,0,64,64,
						  m_pDispBuf[i],
						  m_pBmpInfo,
						  DIB_RGB_COLORS,
						  SRCCOPY
						  );
			m_DCClient.StretchBlt(rect.left, rect.top, rect.right-rect.left, rect.bottom-rect.top, 
							&m_DCBlk,
							0,0,64,64,
							SRCCOPY
						  );

			DrawGrid(&m_DCClient, &rect, 4);
			sprintf(szTemp, "Block %d", i);
			m_DCClient.TextOut(rect.left,rect.top, szTemp, strlen(szTemp)); 
		}
	}else{
		rect.left = 10; rect.right = rect.left+64*8;
		rect.top = 10; rect.bottom = rect.top+64*8;

		StretchDIBits(m_DCBlk.m_hDC, 
					  0,0,64,64,
					  0,0,64,64,
					  m_pDispBuf[m_currBlock],
					  m_pBmpInfo,
					  DIB_RGB_COLORS,
					  SRCCOPY
					  );
		m_DCClient.StretchBlt(rect.left, rect.top, rect.right-rect.left, rect.bottom-rect.top, 
						&m_DCBlk,
						0,0,64,64,
						SRCCOPY
					  );

		DrawGrid(&m_DCClient, &rect, 8);
		sprintf(szTemp, "Block %d", m_currBlock);
		m_DCClient.TextOut(rect.left,rect.top, szTemp, strlen(szTemp)); 
	}

	GetClientRect(&rect);
	pDC->BitBlt(0,0, rect.right-rect.left, rect.bottom-rect.top,
		        &m_DCClient, 0,0, SRCCOPY);
}

void CWndDispSD::DrawGrid(CDC *pDC, const RECT *pRect, int GridSize)
{
	//int oldROP = pDC->SetROP2(R2_XORPEN);
	CPen* pOldPen = (CPen*)pDC->SelectObject(&m_GridPen);

	for(int i=pRect->left; i<pRect->right; i+=GridSize){
		pDC->MoveTo(i, pRect->top);
		pDC->LineTo(i, pRect->bottom);
	}
	for(i=pRect->top; i<pRect->bottom; i+=GridSize){
		pDC->MoveTo(pRect->left, i);
		pDC->LineTo(pRect->right, i);
	}

	//pDC->SetROP2(oldROP);

	RECT rectEdge;
	rectEdge.left = pRect->left-2;
	rectEdge.right = pRect->right+2;
	rectEdge.top = pRect->top-2;
	rectEdge.bottom = pRect->bottom+2;
	pDC->DrawEdge(&rectEdge, EDGE_BUMP,BF_RECT);

	pDC->SelectObject(pOldPen);

}

void CWndDispSD::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	int x,y,value,pos;
	char szTemp[256];
	RECT rect;

	if( m_bDispAll ){
		BOOL bHit = FALSE;
		for(int i=0; i<8; i++){
			GetBlockRect(i, &rect);
			if( PtInRect(&rect, point) ){
				bHit = TRUE;
				break;
			}
		}
		if( bHit ){
			x = (point.x-rect.left)/4;
			y = (point.y-rect.top)/4;
			pos = y*64 + x;
			value = m_pSDBuf[i][pos];
			sprintf(szTemp, "散点图 -- MaxValue=%d   Block=%d   (X=%d<->Y=%d)   Value=%d", m_nMaxCnt, i, x, y, value);
		}else{
			sprintf(szTemp, "散点图 -- MaxValue=%d", m_nMaxCnt);
		}
	}else{
		rect.left = 10; rect.right = rect.left+64*8;
		rect.top = 10; rect.bottom = rect.top+64*8;
		if( !PtInRect(&rect, point) ){
			sprintf(szTemp, "散点图 -- MaxValue=%d", m_nMaxCnt);
		}else{
			x = (point.x-rect.left)/8;
			y = (point.y-rect.top)/8;
			pos = y*64+x;
			value = m_pSDBuf[m_currBlock][pos];
			sprintf(szTemp, "散点图 -- MaxValue=%d   Block=%d   (X=%d<->Y=%d)   Value=%d", m_nMaxCnt, m_currBlock, x, y, value);
		}
	}
	SetWindowText(szTemp);
}

void CWndDispSD::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	BOOL bRedraw = FALSE;
	char szTemp[128];

	if( m_bDispAll ){
		m_bDispAll = FALSE;
		RECT rect;
		for(int i=0; i<8; i++){
			GetBlockRect(i, &rect);
			if( PtInRect(&rect, point) ){
				bRedraw = TRUE;
				m_currBlock = i;
				sprintf(szTemp, "散点图 -- Block %d", i);
				SetWindowText(szTemp);
				break;
			}
		}
	}else{
		m_bDispAll = TRUE;
		SetWindowText("散点图");
		bRedraw = TRUE;
	}

	if(bRedraw){
		CDC* pDC = GetDC();
		Draw(pDC);
	}
}


void CWndDispSD::OnPalDefault() 
{
	// TODO: Add your command handler code here
	CWndDisp::OnPalDefault();

	DatFmt();
	Draw(GetDC());

	m_menu.CheckMenuItem(IDM_PAL1_DEFAULT, MF_CHECKED);
	m_menu.CheckMenuItem(IDM_PAL1_MAP, MF_UNCHECKED);
	m_menu.CheckMenuItem(IDM_PAL1_HOT, MF_UNCHECKED);
	m_menu.CheckMenuItem(IDM_PAL1_FLOW, MF_UNCHECKED);
	m_menu.CheckMenuItem(IDM_PAL1_NIH, MF_UNCHECKED);
	m_menu.CheckMenuItem(IDM_PAL1_256, MF_UNCHECKED);
}

void CWndDispSD::OnPal256() 
{
	// TODO: Add your command handler code here
	CWndDisp::OnPal256();

	DatFmt();
	Draw(GetDC());

	m_menu.CheckMenuItem(IDM_PAL1_DEFAULT, MF_UNCHECKED);
	m_menu.CheckMenuItem(IDM_PAL1_256, MF_CHECKED);
	m_menu.CheckMenuItem(IDM_PAL1_MAP, MF_UNCHECKED);
	m_menu.CheckMenuItem(IDM_PAL1_HOT, MF_UNCHECKED);
	m_menu.CheckMenuItem(IDM_PAL1_FLOW, MF_UNCHECKED);
	m_menu.CheckMenuItem(IDM_PAL1_NIH, MF_UNCHECKED);
}

void CWndDispSD::OnPalFlow() 
{
	// TODO: Add your command handler code here
	CWndDisp::OnPalFlow();

	DatFmt();
	Draw(GetDC());

	m_menu.CheckMenuItem(IDM_PAL1_DEFAULT, MF_UNCHECKED);
	m_menu.CheckMenuItem(IDM_PAL1_MAP, MF_UNCHECKED);
	m_menu.CheckMenuItem(IDM_PAL1_HOT, MF_UNCHECKED);
	m_menu.CheckMenuItem(IDM_PAL1_FLOW, MF_CHECKED);
	m_menu.CheckMenuItem(IDM_PAL1_NIH, MF_UNCHECKED);
	m_menu.CheckMenuItem(IDM_PAL1_256, MF_UNCHECKED);
}

void CWndDispSD::OnPalHot() 
{
	// TODO: Add your command handler code here
	CWndDisp::OnPalHot();

	DatFmt();
	Draw(GetDC());

	m_menu.CheckMenuItem(IDM_PAL1_DEFAULT, MF_UNCHECKED);
	m_menu.CheckMenuItem(IDM_PAL1_MAP, MF_UNCHECKED);
	m_menu.CheckMenuItem(IDM_PAL1_HOT, MF_CHECKED);
	m_menu.CheckMenuItem(IDM_PAL1_FLOW, MF_UNCHECKED);
	m_menu.CheckMenuItem(IDM_PAL1_NIH, MF_UNCHECKED);
	m_menu.CheckMenuItem(IDM_PAL1_256, MF_UNCHECKED);
}

void CWndDispSD::OnPalMap() 
{
	// TODO: Add your command handler code here
	CWndDisp::OnPalMap();

	DatFmt();
	Draw(GetDC());

	m_menu.CheckMenuItem(IDM_PAL1_DEFAULT, MF_UNCHECKED);
	m_menu.CheckMenuItem(IDM_PAL1_MAP, MF_CHECKED);
	m_menu.CheckMenuItem(IDM_PAL1_HOT, MF_UNCHECKED);
	m_menu.CheckMenuItem(IDM_PAL1_FLOW, MF_UNCHECKED);
	m_menu.CheckMenuItem(IDM_PAL1_NIH, MF_UNCHECKED);
	m_menu.CheckMenuItem(IDM_PAL1_256, MF_UNCHECKED);
}

void CWndDispSD::OnPalNih() 
{
	// TODO: Add your command handler code here
	CWndDisp::OnPalNih();

	DatFmt();
	Draw(GetDC());

	m_menu.CheckMenuItem(IDM_PAL1_DEFAULT, MF_UNCHECKED);
	m_menu.CheckMenuItem(IDM_PAL1_MAP, MF_UNCHECKED);
	m_menu.CheckMenuItem(IDM_PAL1_HOT, MF_UNCHECKED);
	m_menu.CheckMenuItem(IDM_PAL1_FLOW, MF_UNCHECKED);
	m_menu.CheckMenuItem(IDM_PAL1_NIH, MF_CHECKED);
	m_menu.CheckMenuItem(IDM_PAL1_256, MF_UNCHECKED);
}

void CWndDispSD::OnSave() 
{
	// TODO: Add your command handler code here
	CFileDialog dlg(FALSE, "dat", NULL, OFN_HIDEREADONLY,"BUCKET数据文件(*.dat)|*.dat|所有文件(*.*)|*.*||");
	if( dlg.DoModal() != IDOK ) return;

	BOOL bCreate;
	if( AfxMessageBox("把数据添加到文件中吗",MB_YESNO) == IDYES )
		bCreate = FALSE;
	else
		bCreate = TRUE;

	CBuckFile sfile;
	if( !sfile.Open(dlg.GetPathName(), bCreate) ){
		AfxMessageBox("保存文件错误!");
		return;
	}

	int len = 8*64*64*4;
	BYTE* pBuf = new BYTE[len];
	for(int blk=0; blk<8; blk++){
		memcpy(pBuf+blk*64*64*4, m_pSDBuf[blk], 64*64*4);
	}
	sfile.SaveData_SD(pBuf, len);

	sfile.Close();
	delete [] pBuf;
}
