// WndDispSD.cpp : implementation file
//

#include "stdafx.h"
#include "SimOWS.h"
#include "WndDispTime.h"
#include "BuckFile.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CWndDispTime

CWndDispTime::CWndDispTime()
{
	for(int i=0; i<8; i++)
	{
		m_pTimeBuf[i] = (long*)GlobalAlloc(GMEM_FIXED, 64*128*sizeof(long));
		ASSERT(m_pTimeBuf != NULL);
		memset(m_pTimeBuf[i], 0, 64*128*sizeof(long));

		m_pDispBuf[i] = (BYTE*)GlobalAlloc(GMEM_FIXED, 64*128);
		ASSERT(m_pDispBuf != NULL);
		memset(m_pDispBuf[i], 0, 64*128);
	}

	m_bDispAll = TRUE;
	m_currBlock = 0;
	m_currStrip = 0;

	m_nMaxCnt = 64;
	m_nMaxCntTime = 10;
	for(int blk=0; blk<8; blk++)
		for(int time=0; time<128; time++){
			m_nCntTime[blk][time] = 0;
		}
/*	m_nMaxCntStrip = 0;
	for(int blk=0; blk<8; blk++)
		for(int strip=0; strip<64; strip++){
			m_nCntStrip[blk][strip] = 0;
		}
*/
	m_DispMode = mode_8blk_2d;
}

CWndDispTime::~CWndDispTime()
{
	for(int i=0; i<8; i++){
		GlobalFree( m_pTimeBuf[i] );
		GlobalFree( m_pDispBuf[i] );
	}
}


BEGIN_MESSAGE_MAP(CWndDispTime, CWndDisp)
	//{{AFX_MSG_MAP(CWndDispTime)
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
// CWndDispTime message handlers
void CWndDispTime::CreateWnd()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	m_hBkBrush = CreateSolidBrush(RGB(255,255,255));
	CreateEx(WS_EX_TOOLWINDOW|WS_EX_OVERLAPPEDWINDOW, 
		     AfxRegisterWndClass(CS_VREDRAW|CS_HREDRAW|CS_DBLCLKS, 0, m_hBkBrush, 0),
			 "时间直方图",
			 WS_CAPTION|WS_BORDER | WS_POPUP,// |WS_VISIBLE | WS_HSCROLL,
			 0,0,512,512,
			 GetDesktopWindow()->m_hWnd,
			 NULL);
}

void CWndDispTime::AdjWndSize()
{
	RECT wndRect,clientRect;

#define X_8BLK  2
#define H_8BLK  256

#define X_64STRIP 1
#define H_64STRIP 64

#define X_1STRIP  4
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

int CWndDispTime::OnCreate(LPCREATESTRUCT lpCreateStruct) 
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
	m_bmpBlk.CreateCompatibleBitmap(pClientDC, 128, 64);
	m_DCBlk.SelectObject(&m_bmpBlk);

	m_pBmpInfo->bmiHeader.biWidth = 128;
	m_pBmpInfo->bmiHeader.biHeight = -64;

	m_GridPen.CreatePen(PS_SOLID, 1, RGB(128,128,128) );
    m_LinePen.CreatePen(PS_SOLID, 1, RGB(0,255,0) );

	m_currBlock = 0;
	m_currStrip = 0;
	
	return 0;
}

void CWndDispTime::ResetDat()
{
	int blk, time, pos;
	for(blk=0; blk<8; blk++){
		for(pos=0; pos<64*128; pos++){
			m_pTimeBuf[blk][pos] = 0;
			m_pDispBuf[blk][pos] = 0;
		}
	}

	m_bDispAll = TRUE;
	m_currBlock = 0;
	m_currStrip = 0;

	m_nMaxCnt = 64;
	m_nMaxCntTime = 10;
	for(blk=0; blk<8; blk++)
		for(time=0; time<128; time++){
			m_nCntTime[blk][time] = 0;
		}
/*	m_nMaxCntStrip = 0;
	for(blk=0; blk<8; blk++)
		for(strip=0; strip<64; strip++){
			m_nCntStrip[blk][strip] = 0;
		}
*/
	Draw(GetDC());
}

void CWndDispTime::ShowDat(void *pBuf, int len)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
   	int pos,block,strip,time;
	TIME_ROWDAT* pDat= (TIME_ROWDAT*)pBuf;

    for(int i=0; i<len/4; i++){
		if( !pDat[i].valid ) continue;
	    
		block = pDat[i].block;
		strip = pDat[i].strip;
		time = pDat[i].time;
	    pos = strip*128 + pDat[i].time;
	    
		if( m_pTimeBuf[block][pos] < 16*1024*1024 ) 
	 	    m_pTimeBuf[block][pos]++;
		if( m_pTimeBuf[block][pos]>m_nMaxCnt )
			m_nMaxCnt = m_pTimeBuf[block][pos];

		if( m_nCntTime[block][time] < 16*1024*1024 )
			m_nCntTime[block][time]++;
		if( m_nCntTime[block][time] > m_nMaxCntTime)
			m_nMaxCntTime = m_nCntTime[block][time];

/*		if( m_nCntStrip[block][strip] < 16*1024*1024 )
			m_nCntStrip[block][strip]++;
		if( m_nCntStrip[block][strip] > m_nMaxCntStrip)
			m_nMaxCntStrip = m_nCntStrip[block][strip];
*/
	}

	DatFmt();
	Draw(GetDC());

	char szTemp[128];
	sprintf(szTemp, "时间直方图 -- MaxValue=%d", m_nMaxCnt);
	SetWindowText(szTemp);
}

void CWndDispTime::DatFmt()
{
	int index;
	for(int block=0; block<8; block++){
		for(int i=0; i<64*128; i++){
			index = ColorTrans(m_pTimeBuf[block][i], m_nMaxCnt);
			if( m_bDefaultPAL )
				ASSERT(index>=0 && index<64);
			else
				ASSERT(index>=0 && index<256);
			m_pDispBuf[block][i] = index;
		}
	}
}

void CWndDispTime::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	RECT rect;
	GetClientRect(&rect);
	dc.BitBlt(0,0, rect.right-rect.left, rect.bottom-rect.top,
		        &m_DCClient, 0,0, SRCCOPY);
}

void CWndDispTime::GetBlockRect(int nBlock, int nStrip, RECT *pRect)
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

void CWndDispTime::Draw(CDC *pDC)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	RECT rect;
	char szTemp[32];
	int x,y; double fy;

	ASSERT(m_nMaxCnt>0);

	GetClientRect(&rect);
	::FillRect(m_DCClient.m_hDC, &rect, m_hBkBrush);

	CPen* pOldPen = m_DCClient.SelectObject(&m_LinePen);

	int blk=0,strip=0,pos=0;
	if( m_DispMode==mode_8blk_2d ){
		for(blk=0; blk<8; blk++){
			GetBlockRect(blk,0,&rect);

			StretchDIBits(m_DCBlk.m_hDC, 
						  0,0,128,64,
						  0,0,128,64,
						  m_pDispBuf[blk],
						  m_pBmpInfo,
						  DIB_RGB_COLORS,
						  SRCCOPY
						  );
			m_DCClient.StretchBlt(rect.left, rect.top, rect.right-rect.left, rect.bottom-rect.top, 
								  &m_DCBlk,
								  0,0,128,64,
								  SRCCOPY
							     );

			DrawGrid(&m_DCClient, &rect, 4);

			sprintf(szTemp, "Blk %d", blk);
			m_DCClient.TextOut(rect.left,rect.top, szTemp, strlen(szTemp)); 
		}
	}else if( m_DispMode==mode_8blk_1d ){
		for(blk=0; blk<8; blk++){
			if( m_nMaxCntTime == 0) continue;

			GetBlockRect(blk,0,&rect);

			fy = ((double)m_nCntTime[blk][0])/m_nMaxCntTime*H_8BLK;
			x = rect.left;
			y = rect.bottom - int(fy);
			m_DCClient.MoveTo(x,y);
			for(int i=1; i<128; i++){
				x += X_8BLK;
				fy = ((double)m_nCntTime[blk][i])/m_nMaxCntTime * H_8BLK;
				y = rect.bottom - int(fy);
				m_DCClient.LineTo(x, y);

				MarkPoint(&m_DCClient, x, y, 8);
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
			fy = ((double)m_pTimeBuf[m_currBlock][pos])/m_nMaxCnt*H_64STRIP;
			x = rect.left;
			y = rect.bottom - int(fy);
			m_DCClient.MoveTo(x,y);
			for(int i=1; i<128; i++){
				x += X_64STRIP;
				fy = ((double)m_pTimeBuf[m_currBlock][pos+i])/m_nMaxCnt * H_64STRIP;
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
		fy = ((double)m_pTimeBuf[m_currBlock][pos])/m_nMaxCnt*H_1STRIP;
		x = rect.left;
		y = rect.bottom - int(fy);
		m_DCClient.MoveTo(x,y);
		for(int i=1; i<128; i++){
			x += X_1STRIP;
			fy = ((double)m_pTimeBuf[m_currBlock][pos+i])/m_nMaxCnt * H_1STRIP;
			y = rect.bottom - int(fy);
			m_DCClient.LineTo(x, y);

			MarkPoint(&m_DCClient, x, y, 8);
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

void CWndDispTime::DrawGrid(CDC *pDC, const RECT *pRect, int GridSize)
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

void CWndDispTime::OnMouseMove(UINT nFlags, CPoint point) 
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
			strip = (point.y - rect.top)/4;    //y
			pos = strip*128+channel;
			value =	m_pTimeBuf[blk][pos];
			sprintf(szTemp, "时间直方图 -- 2维显示8个Block   BlockMaxValue=%d    StripMaxValue=%d    blk=%d strip=%d time=%d value=%d", m_nMaxCnt, m_nMaxCntTime, blk, strip, channel, value);
		}else{
			sprintf(szTemp, "时间直方图 -- 2维显示8个Block   BlockMaxValue=%d    StripMaxValue=%d", m_nMaxCnt, m_nMaxCntTime);
		}
	}else if( m_DispMode==mode_8blk_1d){
		for(blk=0; blk<8; blk++){
			GetBlockRect(blk,0,&rect);
			bHit = PtInRect(&rect, point);
			if(bHit) break;
		}
		if(bHit){
			strip = (point.x - rect.left)/X_8BLK;	//time
			value = m_nCntTime[blk][strip];
			sprintf(szTemp, "时间直方图 -- 1维显示8个Block   BlockMaxValue=%d    StripMaxValue=%d    blk=%d time=%d value=%d", m_nMaxCnt, m_nMaxCntTime, blk, strip, value);
		}else{
			sprintf(szTemp, "时间直方图 -- 1维显示8个Block   BlockMaxValue=%d    StripMaxValue=%d", m_nMaxCnt, m_nMaxCntTime);
		}
	}else if( m_DispMode==mode_1blk_64strip) {
		for(strip=0; strip<64; strip++){
			GetBlockRect(0,strip,&rect);
			bHit = PtInRect(&rect, point);
			if(bHit) break;
		}
		if(bHit){
			channel = (point.x-rect.left)/X_64STRIP;
			pos = strip*128+channel;
			value =	m_pTimeBuf[m_currBlock][pos];
			sprintf(szTemp, "时间直方图 -- 显示 Block%d 中的64个Strip   BlockMaxValue=%d   StripMaxValue=%d    strip=%d time=%d value=%d", m_currBlock, m_nMaxCnt, m_nMaxCntTime, strip, channel, value);
		}else{
			sprintf(szTemp, "时间直方图 -- 显示 Block%d 中的64个Strip    BlockMaxValue=%d   StripMaxValue=%d", m_currBlock, m_nMaxCnt, m_nMaxCntTime);
		}
	}else if( m_DispMode==mode_1blk_1strip){
		GetBlockRect(0,0,&rect);
		bHit = PtInRect(&rect, point);
		if(bHit){
			channel = (point.x-rect.left)/X_1STRIP;
			pos = m_currStrip*128+channel;
			value =	m_pTimeBuf[m_currBlock][pos];
			sprintf(szTemp, "时间直方图 -- 显示 Block%d 中的Strip%d    BlockMaxValue=%d    time=%d value=%d", m_currBlock, m_currStrip, m_nMaxCnt, channel, value);
		}else{
			sprintf(szTemp, "时间直方图 -- 显示 Block%d 中的Strip%d", m_currBlock, m_currStrip);
		}
	}else{
		ASSERT(FALSE);
		sprintf(szTemp, "时间直方图 -- 显示模式:NO");
	}

	SetWindowText(szTemp);
}

void CWndDispTime::OnLButtonDblClk(UINT nFlags, CPoint point) 
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
		sprintf(szTemp, "时间直方图 -- 显示模式:NO");
	}

	AdjWndSize();
	Draw(GetDC());
}


void CWndDispTime::OnDisp2d() 
{
	// TODO: Add your command handler code here
	m_DispMode = mode_8blk_2d;
	m_menu.CheckMenuItem(IDM_2D, MF_CHECKED);
	m_menu.CheckMenuItem(IDM_1D, MF_UNCHECKED);

	AdjWndSize();
	Draw(GetDC());
}

void CWndDispTime::OnDisp1d() 
{
	// TODO: Add your command handler code here
	m_DispMode = mode_8blk_1d;
	m_menu.CheckMenuItem(IDM_2D, MF_UNCHECKED);
	m_menu.CheckMenuItem(IDM_1D, MF_CHECKED);

	AdjWndSize();
	Draw(GetDC());
}



void CWndDispTime::OnPalDefault() 
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

void CWndDispTime::OnPal256() 
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

void CWndDispTime::OnPalFlow() 
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

void CWndDispTime::OnPalHot() 
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

void CWndDispTime::OnPalMap() 
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

void CWndDispTime::OnPalNih() 
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

void CWndDispTime::OnSave() 
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

	int len = 8*64*128*4;
	BYTE* pBuf = new BYTE[len];
	for(int blk=0; blk<8; blk++){
		memcpy(pBuf+blk*64*128*4, m_pTimeBuf[blk], 64*128*4);
	}
	sfile.SaveData_Time(pBuf, len);

	sfile.Close();
	delete [] pBuf;
}
