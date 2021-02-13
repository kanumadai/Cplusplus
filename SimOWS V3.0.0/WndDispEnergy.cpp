// WndDispSD.cpp : implementation file
//

#include "stdafx.h"
#include "SimOWS.h"
#include "WndDispEnergy.h"
#include "BuckFile.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CWndDispEnergy

CWndDispEnergy::CWndDispEnergy()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	for(int i=0; i<8; i++)
	{
		m_pEnergyBuf[i] = (long*)GlobalAlloc(GMEM_FIXED, 64*64*sizeof(long));
		ASSERT(m_pEnergyBuf != NULL);
		memset(m_pEnergyBuf[i], 0, 64*64*sizeof(long));

		m_pDispBuf[i] = (BYTE*)GlobalAlloc(GMEM_FIXED, 64*64);
		ASSERT(m_pDispBuf != NULL);
		memset(m_pDispBuf[i], 0, 64*64);
	}

	m_bDispAll = TRUE;
	m_currBlock = 0;
	m_currStrip = 0;

	m_nMaxCnt = 64;
	m_nMaxCntEnergy = 10;
	for(int blk=0; blk<8; blk++)
		for(int energy=0; energy<64; energy++){
			m_nCntEnergy[blk][energy] = 0;
		}
/*	m_nMaxCntStrip = 0;
	for(int blk=0; blk<8; blk++)
		for(int strip=0; strip<64; strip++){
			m_nCntStrip[blk][strip] = 0;
		}
*/
	m_DispMode = mode_8blk_2d;
}

CWndDispEnergy::~CWndDispEnergy()
{
	for(int i=0; i<8; i++){
		GlobalFree( m_pEnergyBuf[i] );
		GlobalFree( m_pDispBuf[i] );
	}
}


BEGIN_MESSAGE_MAP(CWndDispEnergy, CWndDisp)
	//{{AFX_MSG_MAP(CWndDispEnergy)
	ON_WM_CREATE()
	ON_WM_PAINT()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_MOUSEMOVE()
	ON_COMMAND(IDM_2D, OnDisp2d)
	ON_COMMAND(IDM_1D, OnDisp1d)
	ON_COMMAND(IDM_PAL2_DEFAULT, OnPalDefault)
	ON_COMMAND(IDM_PAL2_256, OnPal256)
	ON_COMMAND(IDM_PAL2_FLOW, OnPalFlow)
	ON_COMMAND(IDM_PAL2_HOT, OnPalHot)
	ON_COMMAND(IDM_PAL2_MAP, OnPalMap)
	ON_COMMAND(IDM_PAL2_NIH, OnPalNih)
	ON_COMMAND(IDM_SAVE2, OnSave)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CWndDispEnergy message handlers
void CWndDispEnergy::CreateWnd()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	m_hBkBrush = CreateSolidBrush(RGB(255,255,255));
	CreateEx(WS_EX_TOOLWINDOW|WS_EX_OVERLAPPEDWINDOW, 
		     AfxRegisterWndClass(CS_VREDRAW|CS_HREDRAW|CS_DBLCLKS, 0, m_hBkBrush, 0),
			 "能谱图",
			 WS_CAPTION|WS_BORDER | WS_POPUP,// |WS_VISIBLE | WS_HSCROLL,
			 0,0,512,512,
			 GetDesktopWindow()->m_hWnd,
			 NULL);
}

void CWndDispEnergy::AdjWndSize()
{
	RECT wndRect,clientRect;

#define X_8BLK  4
#define H_8BLK  256

#define X_64STRIP 2
#define H_64STRIP 64

#define X_1STRIP  8
#define H_1STRIP  512

	int WND_WIDTH,WND_HIGH;
	switch(m_DispMode){
	default:
	case mode_8blk_2d:
	case mode_8blk_1d:
		 WND_WIDTH = (((256+10)*4)+10);	  //=1074
		 WND_HIGH  = (((256+10)*2)+10);	  //=542
		 break;
	case mode_1blk_64strip:
		 WND_WIDTH = (((128+10)*8)+10);	  //=1114
		 WND_HIGH  = (((64+10)*8)+10);	  //=602
		 break;
	case mode_1blk_1strip:
 		 WND_WIDTH = 1024+20;
		 WND_HIGH  = 512+20;
		 break;
	}

	GetWindowRect(&wndRect);
	GetClientRect(&clientRect);
	wndRect.right = wndRect.right + (WND_WIDTH-(clientRect.right-clientRect.left));
	wndRect.bottom = wndRect.bottom + (WND_HIGH-(clientRect.bottom-clientRect.top));
	MoveWindow(&wndRect);
}

int CWndDispEnergy::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWndDisp::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	m_menu.LoadMenu(IDR_ENERGY);
	m_DispMode = mode_8blk_2d;
	m_menu.CheckMenuItem(IDM_2D, MF_CHECKED);
	m_menu.CheckMenuItem(IDM_1D, MF_UNCHECKED);
	m_bDefaultPAL = TRUE;
	m_menu.CheckMenuItem(IDM_PAL2_DEFAULT, MF_CHECKED);

	//set client rect to with=64*4*4+5*10, high=64*4*2+3*10
	AdjWndSize();

	CDC* pClientDC = GetDC();
	
	RECT clientRect;
	clientRect.left = 0; clientRect.top = 0;
	clientRect.right = (((128+10)*8)+10);
	clientRect.bottom = (((64+10)*8)+10);

	//GetClientRect(&clientRect);
	m_DCClient.CreateCompatibleDC(pClientDC);
	m_bmpClient.CreateCompatibleBitmap(pClientDC, clientRect.right-clientRect.left, clientRect.bottom-clientRect.top);
	m_DCClient.SelectObject(&m_bmpClient);

	m_DCBlk.CreateCompatibleDC(pClientDC);
	m_bmpBlk.CreateCompatibleBitmap(pClientDC, 64, 64);
	m_DCBlk.SelectObject(&m_bmpBlk);

	m_GridPen.CreatePen(PS_SOLID, 1, RGB(128,128,128) );
    m_LinePen.CreatePen(PS_SOLID, 1, RGB(0,255,0) );

	m_currBlock = 0;
	m_currStrip = 0;
	
	return 0;
}

void CWndDispEnergy::ResetDat()
{
	int blk, energy, pos;
	for(blk=0; blk<8; blk++){
		for(pos=0; pos<64*64; pos++){
			m_pEnergyBuf[blk][pos] = 0;
			m_pDispBuf[blk][pos] = 0;
		}
	}

	m_bDispAll = TRUE;
	m_currBlock = 0;
	m_currStrip = 0;

	m_nMaxCnt = 64;
	m_nMaxCntEnergy = 10;
	for(blk=0; blk<8; blk++)
		for(energy=0; energy<64; energy++){
			m_nCntEnergy[blk][energy] = 0;
		}
/*	m_nMaxCntStrip = 0;
	for(blk=0; blk<8; blk++)
		for(strip=0; strip<64; strip++){
			m_nCntStrip[blk][strip] = 0;
		}
*/
	Draw(GetDC());
}

void CWndDispEnergy::ShowDat(void *pBuf, int len)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
   	int pos,block,strip,energy;
	ENERGY_ROWDAT* pDat= (ENERGY_ROWDAT*)pBuf;

    for(int i=0; i<len/4; i++){
		if( !pDat[i].valid ) continue;
	    
		block = pDat[i].block;
		strip = pDat[i].strip;
		energy = pDat[i].energy;
	    pos = strip*64 + pDat[i].energy;
	    
		if( m_pEnergyBuf[block][pos] < 16*1024*1024 ) 
	 	    m_pEnergyBuf[block][pos]++;
		if( m_pEnergyBuf[block][pos]>m_nMaxCnt )
			m_nMaxCnt = m_pEnergyBuf[block][pos];

		if( m_nCntEnergy[block][energy] < 16*1024*1024 )
			m_nCntEnergy[block][energy]++;
		if( m_nCntEnergy[block][energy] > m_nMaxCntEnergy)
			m_nMaxCntEnergy = m_nCntEnergy[block][energy];

/*		if( m_nCntStrip[block][strip] < 16*1024*1024 )
			m_nCntStrip[block][strip]++;
		if( m_nCntStrip[block][strip] > m_nMaxCntStrip)
			m_nMaxCntStrip = m_nCntStrip[block][strip];
*/
	}

	DatFmt();
	Draw(GetDC());

	char szTemp[128];
	sprintf(szTemp, "能谱图 -- MaxValue=%d", m_nMaxCnt);
	SetWindowText(szTemp);
}

void CWndDispEnergy::DatFmt()
{
	int index;
	for(int block=0; block<8; block++){
		for(int i=0; i<64*64; i++){
			index = ColorTrans(m_pEnergyBuf[block][i], m_nMaxCnt);
			if( m_bDefaultPAL )
				ASSERT(index>=0 && index<64);
			else
				ASSERT(index>=0 && index<256);
			m_pDispBuf[block][i] = index;
		}
	}
}

void CWndDispEnergy::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	RECT rect;
	GetClientRect(&rect);
	dc.BitBlt(0,0, rect.right-rect.left, rect.bottom-rect.top,
		        &m_DCClient, 0,0, SRCCOPY);
}

void CWndDispEnergy::GetBlockRect(int nBlock, int nStrip, RECT *pRect)
{
	int x, y;
	switch(m_DispMode)
	{
	case mode_8blk_2d:  //w=64*4 h=64*4
	case mode_8blk_1d:
		 x = nBlock%4; y=nBlock/4;
		 pRect->left = x*64*4 + (x+1)*10;
		 pRect->right = pRect->left+64*4;
		 pRect->top = y*64*4 + (y+1)*10;
		 pRect->bottom = pRect->top+64*4;
		 break;

	case mode_1blk_64strip:  //w=128 h=64
		 x = nStrip%8;  y = nStrip/8;
		 pRect->left = x*128 + (x+1)*10;
		 pRect->right = pRect->left+128;
		 pRect->top = y*64 + (y+1)*10;
		 pRect->bottom = pRect->top+64;
		 break;

	case mode_1blk_1strip:  //w=h=64*8
		 pRect->left = 10;
		 pRect->right = pRect->left+64*8;
		 pRect->top = 10;
		 pRect->bottom = pRect->top+64*8;
		 break;

	default:
		 ASSERT(FALSE);
		 return;
	}
}

void CWndDispEnergy::Draw(CDC *pDC)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	RECT rect;
	char szTemp[32];
	int x,y; double fy;

	ASSERT(m_nMaxCnt>0);

	COLORREF pixColor = RGB(0,0,255);
	
	GetClientRect(&rect);
	::FillRect(m_DCClient.m_hDC, &rect, m_hBkBrush);

	CPen* pOldPen = m_DCClient.SelectObject(&m_LinePen);

	int blk=0,strip=0,pos=0;
	if( m_DispMode==mode_8blk_2d ){
		for(blk=0; blk<8; blk++){
			GetBlockRect(blk,0,&rect);

			StretchDIBits(m_DCBlk.m_hDC, 
						  0,0,64,64,
						  0,0,64,64,
						  m_pDispBuf[blk],
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

			sprintf(szTemp, "Blk %d", blk);
			m_DCClient.TextOut(rect.left,rect.top, szTemp, strlen(szTemp)); 
		}
	}else if( m_DispMode==mode_8blk_1d ){
		CPen markPen(PS_SOLID, 1, RGB(0,0,255));
		CPen* pOldPen;
		for(blk=0; blk<8; blk++){
			if( m_nMaxCntEnergy == 0) continue;

			GetBlockRect(blk,0,&rect);

			pOldPen = m_DCClient.SelectObject(&markPen);
			x = rect.left+40*X_8BLK;
			m_DCClient.MoveTo(x,rect.top);
			m_DCClient.LineTo(x,rect.bottom);

			x = rect.left+44*X_8BLK;
			m_DCClient.MoveTo(x,rect.top);
			m_DCClient.LineTo(x,rect.bottom);
			m_DCClient.SelectObject(pOldPen);

			fy = ((double)m_nCntEnergy[blk][0])/m_nMaxCntEnergy*H_8BLK;
			x = rect.left;
			y = rect.bottom - int(fy);
			m_DCClient.MoveTo(x,y);
			for(int i=1; i<64; i++){
				x += X_8BLK;
				fy = ((double)m_nCntEnergy[blk][i])/m_nMaxCntEnergy * H_8BLK;
				y = rect.bottom - int(fy);
				m_DCClient.LineTo(x, y);

				MarkPoint(&m_DCClient, x,y, X_8BLK);
			}

			DrawGrid(&m_DCClient, &rect, 4);

			sprintf(szTemp, "Block %d", blk);
			m_DCClient.TextOut(rect.left,rect.top, szTemp, strlen(szTemp)); 
		}
	}else if( m_DispMode==mode_1blk_64strip) {
		for(strip=0; strip<64; strip++){
			if( m_nMaxCnt == 0) continue;

			GetBlockRect(0,strip,&rect);

			pos = strip*64;
			fy = ((double)m_pEnergyBuf[m_currBlock][pos])/m_nMaxCnt*H_64STRIP;
			x = rect.left;
			y = rect.bottom - int(fy);
			m_DCClient.MoveTo(x,y);
			for(int i=1; i<64; i++){
				x += X_64STRIP;
				fy = ((double)m_pEnergyBuf[m_currBlock][pos+i])/m_nMaxCnt * H_64STRIP;
				y = rect.bottom - int(fy);
				m_DCClient.LineTo(x, y);
			}

			DrawGrid(&m_DCClient, &rect, 4);

			sprintf(szTemp, "Strip %d", strip);
			m_DCClient.TextOut(rect.left,rect.top, szTemp, strlen(szTemp)); 
		}
	}else if( m_DispMode==mode_1blk_1strip){
		GetBlockRect(0,0,&rect);

		pos = m_currStrip*64;
		fy = ((double)m_pEnergyBuf[m_currBlock][pos])/m_nMaxCnt*H_1STRIP;
		x = rect.left;
		y = rect.bottom - int(fy);
		m_DCClient.MoveTo(x,y);
		for(int i=1; i<64; i++){
			x += X_1STRIP;
			fy = ((double)m_pEnergyBuf[m_currBlock][pos+i])/m_nMaxCnt * H_1STRIP;
			y = rect.bottom - int(fy);
			m_DCClient.LineTo(x, y);

			MarkPoint(&m_DCClient, x,y, X_1STRIP);
		}

		DrawGrid(&m_DCClient, &rect, 4);

		sprintf(szTemp, "Block%d Strip%d", m_currBlock, m_currStrip);
		m_DCClient.TextOut(rect.left,rect.top, szTemp, strlen(szTemp)); 
	}else{
		ASSERT(FALSE);
	}
	m_DCClient.SelectObject(pOldPen);

	GetClientRect(&rect);
	pDC->BitBlt(0,0, rect.right-rect.left, rect.bottom-rect.top,
		        &m_DCClient, 0,0, SRCCOPY);
}

void CWndDispEnergy::DrawGrid(CDC *pDC, const RECT *pRect, int GridSize)
{
	if( m_DispMode==mode_8blk_2d ){

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
		pDC->SelectObject(pOldPen);
	}

	RECT rectEdge;
	rectEdge.left = pRect->left-2;
	rectEdge.right = pRect->right+2;
	rectEdge.top = pRect->top-2;
	rectEdge.bottom = pRect->bottom+2;
	pDC->DrawEdge(&rectEdge, EDGE_BUMP,BF_RECT);
}

void CWndDispEnergy::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	char szTemp[256];
	RECT rect;
	int pos,value;

	BOOL bHit;
	int blk=0,strip=0,channel=0;
	if( m_DispMode==mode_8blk_2d ){
		for(blk=0; blk<8; blk++){
			GetBlockRect(blk,0,&rect);
			bHit = PtInRect(&rect, point);
			if(bHit) break;
		}
		if(bHit){
			channel = (point.x - rect.left)/X_8BLK;	//x
			strip = (point.y - rect.top)/X_8BLK;    //y
			pos = strip*64+channel;
			value =	m_pEnergyBuf[blk][pos];
			sprintf(szTemp, "能谱图 -- 2维显示8个Block   BlockMaxValue=%d    StripMaxValue=%d    blk=%d strip=%d energy=%d value=%d", m_nMaxCnt, m_nMaxCntEnergy, blk, strip, channel, value);
		}else{
			sprintf(szTemp, "能谱图 -- 2维显示8个Block   BlockMaxValue=%d    StripMaxValue=%d", m_nMaxCnt, m_nMaxCntEnergy);
		}
	}else if( m_DispMode==mode_8blk_1d){
		for(blk=0; blk<8; blk++){
			GetBlockRect(blk,0,&rect);
			bHit = PtInRect(&rect, point);
			if(bHit) break;
		}
		if(bHit){
			strip = (point.x - rect.left)/X_8BLK;	//energy
			value = m_nCntEnergy[blk][strip];
			sprintf(szTemp, "能谱图 -- 1维显示8个Block   BlockMaxValue=%d    StripMaxValue=%d    blk=%d energy=%d value=%d", m_nMaxCnt, m_nMaxCntEnergy, blk, strip, value);
		}else{
			sprintf(szTemp, "能谱图 -- 1维显示8个Block   BlockMaxValue=%d    StripMaxValue=%d", m_nMaxCnt, m_nMaxCntEnergy);
		}
	}else if( m_DispMode==mode_1blk_64strip) {
		for(strip=0; strip<64; strip++){
			GetBlockRect(0,strip,&rect);
			bHit = PtInRect(&rect, point);
			if(bHit) break;
		}
		if(bHit){
			channel = (point.x-rect.left)/X_64STRIP;
			pos = strip*64+channel;
			value =	m_pEnergyBuf[m_currBlock][pos];
			sprintf(szTemp, "能谱图 -- 显示 Block%d 中的64个Strip   BlockMaxValue=%d   StripMaxValue=%d    strip=%d energy=%d value=%d", m_currBlock, m_nMaxCnt, m_nMaxCntEnergy, strip, channel, value);
		}else{
			sprintf(szTemp, "能谱图 -- 显示 Block%d 中的64个Strip    BlockMaxValue=%d   StripMaxValue=%d", m_currBlock, m_nMaxCnt, m_nMaxCntEnergy);
		}
	}else if( m_DispMode==mode_1blk_1strip){
		GetBlockRect(0,0,&rect);
		bHit = PtInRect(&rect, point);
		if(bHit){
			channel = (point.x-rect.left)/X_1STRIP;
			pos = m_currStrip*64+channel;
			value =	m_pEnergyBuf[m_currBlock][pos];
			sprintf(szTemp, "能谱图 -- 显示 Block%d 中的Strip%d    BlockMaxValue=%d    energy=%d value=%d", m_currBlock, m_currStrip, m_nMaxCnt, channel, value);
		}else{
			sprintf(szTemp, "能谱图 -- 显示 Block%d 中的Strip%d", m_currBlock, m_currStrip);
		}
	}else{
		ASSERT(FALSE);
		sprintf(szTemp, "能谱图 -- 显示模式:NO");
	}

	SetWindowText(szTemp);
}

void CWndDispEnergy::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	BOOL bRedraw = FALSE;
	char szTemp[128];

	BOOL bHit;
	RECT rect;
	int blk=0,strip=0;
	if( m_DispMode==mode_8blk_2d || m_DispMode==mode_8blk_1d){
		for(blk=0; blk<8; blk++){
			GetBlockRect(blk,0,&rect);
			bHit = PtInRect(&rect, point);
			if(bHit) break;
		}
		if(bHit){
			m_DispMode = mode_1blk_64strip;
			m_currBlock = blk;
			m_currStrip = 0;
		}
	}else if( m_DispMode==mode_1blk_64strip) {
		for(strip=0; strip<64; strip++){
			GetBlockRect(0,strip,&rect);
			bHit = PtInRect(&rect, point);
			if(bHit) break;
		}
		if(bHit){
			m_DispMode = mode_1blk_1strip;
			m_currStrip = strip;
		}else{
			m_DispMode = mode_8blk_2d;
			OnDisp2d();
		}
	}else if( m_DispMode==mode_1blk_1strip){
		GetBlockRect(0,0,&rect);
		bHit = PtInRect(&rect, point);
		if(!bHit){
			m_DispMode = mode_1blk_64strip;
		}
	}else{
		ASSERT(FALSE);
		sprintf(szTemp, "能谱图 -- 显示模式:NO");
	}

	AdjWndSize();
	Draw(GetDC());
}


void CWndDispEnergy::OnDisp2d() 
{
	// TODO: Add your command handler code here
	m_DispMode = mode_8blk_2d;
	m_menu.CheckMenuItem(IDM_2D, MF_CHECKED);
	m_menu.CheckMenuItem(IDM_1D, MF_UNCHECKED);

	AdjWndSize();
	Draw(GetDC());
}

void CWndDispEnergy::OnDisp1d() 
{
	// TODO: Add your command handler code here
	m_DispMode = mode_8blk_1d;
	m_menu.CheckMenuItem(IDM_2D, MF_UNCHECKED);
	m_menu.CheckMenuItem(IDM_1D, MF_CHECKED);

	AdjWndSize();
	Draw(GetDC());
}



void CWndDispEnergy::OnPalDefault() 
{
	// TODO: Add your command handler code here
	CWndDisp::OnPalDefault();

	DatFmt();
	Draw(GetDC());

	m_menu.CheckMenuItem(IDM_PAL2_DEFAULT, MF_CHECKED);
	m_menu.CheckMenuItem(IDM_PAL2_MAP, MF_UNCHECKED);
	m_menu.CheckMenuItem(IDM_PAL2_HOT, MF_UNCHECKED);
	m_menu.CheckMenuItem(IDM_PAL2_FLOW, MF_UNCHECKED);
	m_menu.CheckMenuItem(IDM_PAL2_NIH, MF_UNCHECKED);
	m_menu.CheckMenuItem(IDM_PAL2_256, MF_UNCHECKED);
}

void CWndDispEnergy::OnPal256() 
{
	// TODO: Add your command handler code here
	CWndDisp::OnPal256();

	DatFmt();
	Draw(GetDC());

	m_menu.CheckMenuItem(IDM_PAL2_DEFAULT, MF_UNCHECKED);
	m_menu.CheckMenuItem(IDM_PAL2_MAP, MF_UNCHECKED);
	m_menu.CheckMenuItem(IDM_PAL2_HOT, MF_UNCHECKED);
	m_menu.CheckMenuItem(IDM_PAL2_FLOW, MF_UNCHECKED);
	m_menu.CheckMenuItem(IDM_PAL2_NIH, MF_UNCHECKED);
	m_menu.CheckMenuItem(IDM_PAL2_256, MF_CHECKED);
}

void CWndDispEnergy::OnPalFlow() 
{
	// TODO: Add your command handler code here
	CWndDisp::OnPalFlow();

	DatFmt();
	Draw(GetDC());

	m_menu.CheckMenuItem(IDM_PAL2_DEFAULT, MF_UNCHECKED);
	m_menu.CheckMenuItem(IDM_PAL2_MAP, MF_UNCHECKED);
	m_menu.CheckMenuItem(IDM_PAL2_HOT, MF_UNCHECKED);
	m_menu.CheckMenuItem(IDM_PAL2_FLOW, MF_CHECKED);
	m_menu.CheckMenuItem(IDM_PAL2_NIH, MF_UNCHECKED);
	m_menu.CheckMenuItem(IDM_PAL2_256, MF_UNCHECKED);
}

void CWndDispEnergy::OnPalHot() 
{
	// TODO: Add your command handler code here
	CWndDisp::OnPalHot();

	DatFmt();
	Draw(GetDC());

	m_menu.CheckMenuItem(IDM_PAL2_DEFAULT, MF_UNCHECKED);
	m_menu.CheckMenuItem(IDM_PAL2_MAP, MF_UNCHECKED);
	m_menu.CheckMenuItem(IDM_PAL2_HOT, MF_CHECKED);
	m_menu.CheckMenuItem(IDM_PAL2_FLOW, MF_UNCHECKED);
	m_menu.CheckMenuItem(IDM_PAL2_NIH, MF_UNCHECKED);
	m_menu.CheckMenuItem(IDM_PAL2_256, MF_UNCHECKED);
}

void CWndDispEnergy::OnPalMap() 
{
	// TODO: Add your command handler code here
	CWndDisp::OnPalMap();

	DatFmt();
	Draw(GetDC());

	m_menu.CheckMenuItem(IDM_PAL2_DEFAULT, MF_UNCHECKED);
	m_menu.CheckMenuItem(IDM_PAL2_MAP, MF_CHECKED);
	m_menu.CheckMenuItem(IDM_PAL2_HOT, MF_UNCHECKED);
	m_menu.CheckMenuItem(IDM_PAL2_FLOW, MF_UNCHECKED);
	m_menu.CheckMenuItem(IDM_PAL2_NIH, MF_UNCHECKED);
	m_menu.CheckMenuItem(IDM_PAL2_256, MF_UNCHECKED);
}

void CWndDispEnergy::OnPalNih() 
{
	// TODO: Add your command handler code here
	CWndDisp::OnPalNih();

	DatFmt();
	Draw(GetDC());

	m_menu.CheckMenuItem(IDM_PAL2_DEFAULT, MF_UNCHECKED);
	m_menu.CheckMenuItem(IDM_PAL2_MAP, MF_UNCHECKED);
	m_menu.CheckMenuItem(IDM_PAL2_HOT, MF_UNCHECKED);
	m_menu.CheckMenuItem(IDM_PAL2_FLOW, MF_UNCHECKED);
	m_menu.CheckMenuItem(IDM_PAL2_NIH, MF_CHECKED);
	m_menu.CheckMenuItem(IDM_PAL2_256, MF_UNCHECKED);
}


void CWndDispEnergy::OnSave() 
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
		memcpy(pBuf+blk*64*64*4, m_pEnergyBuf[blk], 64*64*4);
	}
	sfile.SaveData_Energy(pBuf, len);

	sfile.Close();
	delete [] pBuf;
}
