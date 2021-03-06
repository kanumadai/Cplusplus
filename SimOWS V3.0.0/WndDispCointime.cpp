// WndDispCointime.cpp : implementation file
//

#include "stdafx.h"
#include "SimOWS.h"
#include "WndDispCointime.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CWndDispCointime

CWndDispCointime::CWndDispCointime()
{
	
	for(int sel_buck=0; sel_buck<32; sel_buck++)
	{
		for( int fov=0; fov<15; fov++)
		{
			m_4BtoFOV_Table[sel_buck][fov] = (sel_buck+9+fov)&0x0f;
			m_bDataValid[fov] = FALSE;
		}
	}
}

CWndDispCointime::~CWndDispCointime()
{
}


BEGIN_MESSAGE_MAP(CWndDispCointime, CWndDisp)
	//{{AFX_MSG_MAP(CWndDispCointime)
	ON_WM_CREATE()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_PAINT()
	ON_COMMAND(IDM_BUCKETALL, OnBucketAll)
	ON_COMMAND(IDM_BUCKET0, OnBucket0)
	ON_COMMAND(IDM_BUCKET1, OnBucket1)
	ON_COMMAND(IDM_BUCKET2, OnBucket2)
	ON_COMMAND(IDM_BUCKET3, OnBucket3)
	ON_COMMAND(IDM_BUCKET4, OnBucket4)
	ON_COMMAND(IDM_BUCKET5, OnBucket5)
	ON_COMMAND(IDM_BUCKET6, OnBucket6)
	ON_COMMAND(IDM_BUCKET7, OnBucket7)
	ON_COMMAND(IDM_BUCKET8, OnBucket8)
	ON_COMMAND(IDM_BUCKET9, OnBucket9)
	ON_COMMAND(IDM_BUCKET10, OnBucket10)
	ON_COMMAND(IDM_BUCKET11, OnBucket11)
	ON_COMMAND(IDM_BUCKET12, OnBucket12)
	ON_COMMAND(IDM_BUCKET13, OnBucket13)
	ON_COMMAND(IDM_BUCKET14, OnBucket14)
	ON_WM_RBUTTONUP()
	ON_WM_MOUSEWHEEL()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CWndDispCointime message handlers

void CWndDispCointime::CreateWnd()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	m_hBkBrush = CreateSolidBrush(RGB(255,255,255));
	CreateEx(WS_EX_TOOLWINDOW|WS_EX_OVERLAPPEDWINDOW, 
		     AfxRegisterWndClass(CS_VREDRAW|CS_HREDRAW|CS_DBLCLKS, 0, m_hBkBrush, 0),
			 "����ʱ����",
			 WS_CAPTION|WS_BORDER | WS_POPUP,// |WS_VISIBLE | WS_HSCROLL,
			 0,0,512,512,
			 GetDesktopWindow()->m_hWnd,
			 NULL);
}

int CWndDispCointime::Bucket_4BtoFOV(int sel_buck, int buck4b)
{
	 ASSERT(sel_buck>=0 && sel_buck<32);
	 ASSERT(buck4b>=0 && buck4b<16);

	 int findFov = -1;
	 for( int fov=0; fov<15; fov++){
		 if( m_4BtoFOV_Table[sel_buck][fov] == buck4b){
			 findFov = fov;
			 break;
		 }
	 }
	 //ASSERT( findFov != -1);
	 return findFov;
}

void CWndDispCointime::ShowDat(char *szFile)
{
	CFile file;
	if( !file.Open(szFile, CFile::modeRead) ){
		AfxMessageBox("Open File Error!");
		return;
	}

	int minBuck, maxBuck;
	DWORD dwDat;
	file.Read(&dwDat, 4);
	if( ( (dwDat&0xC0000000)!=0xC0000000 ) || (dwDat&0x3fffffff)>31 ){
		AfxMessageBox( "Invalid Data File Format!");
		file.Close();
		return;
	}
	m_nSelectedBucket = dwDat&0x1f;
	minBuck = m_nSelectedBucket + 9;
	maxBuck = minBuck + 14;
	
	FH_TIME* pBuf;
	int readed;
	pBuf = new FH_TIME[4*1024*1024];

	int buck_4b, block, time;
	int buck_5b, buf_order;
	int valid=0, inValid=0;
	do{
		readed = file.Read(pBuf, sizeof(FH_TIME)*4*1024*1024 );
		for(int i=0; i<readed/4; i++){
			if( pBuf[i].coin_flag != 0 ) continue;

			buck_4b = pBuf[i].a_bucket;
			block   = pBuf[i].a_block;
			if( pBuf[i].time_flag == 0) 
				time = pBuf[i].time;
			else
				time = -1*pBuf[i].time;
			time += 127;

			//buck 4b->fov(buf order)
			buf_order = Bucket_4BtoFOV(m_nSelectedBucket, buck_4b);
			if(buf_order<0 || buf_order>14){
				inValid++;
				//TRACE("%d\r\n", i);
				continue;
			}else{
				valid++;
			}

			m_nDispData[buf_order][block][time]++;
			m_bDataValid[buf_order] = TRUE;
			if( m_nDispData[buf_order][block][time] > m_nMaxCnt)
				m_nMaxCnt = m_nDispData[buf_order][block][time];

		}
	}while(readed>0);

	delete [] pBuf;
	file.Close();

	TRACE("Valid=%d, Invalid=%d\r\n", valid, inValid);

	Draw(GetDC()); 

	//buf_order -> bucket5b
	char szTemp[128];
	for(buf_order=0; buf_order<15; buf_order++){
		buck_5b = buf_order + m_nSelectedBucket + 9;
		buck_5b %= 32;
		m_Bucket5B[buf_order] = buck_5b;
		if( buf_order == 7)
			sprintf(szTemp,"Bucket(%2.2d<->%2.2d)***", m_nSelectedBucket, buck_5b);
		else
			sprintf(szTemp,"Bucket(%2.2d<->%2.2d)", m_nSelectedBucket, buck_5b);
		m_menu.ModifyMenu( IDM_BUCKET0+buf_order,
			               MF_STRING,
						   IDM_BUCKET0+buf_order,
						   szTemp
						  );
		
	}
}

void CWndDispCointime::ResetDat()
{
	for( int buck=0; buck<15; buck++){
		m_bDataValid[buck] = FALSE;
		m_Bucket5B[buck] = 0;
	   for( int block=0; block<8; block++)
	      for( int time=0; time<256; time++ )
			  m_nDispData[buck][block][time] = 0;
	}
	m_nMaxCnt = 0;

	m_nCurrDispBucket = 0;
	m_DispMode = mode_allbucket;

	Draw(GetDC());
}

int CWndDispCointime::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWndDisp::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	m_menu.LoadMenu(IDR_COINTIME);
	m_DispMode = mode_allbucket;

	//set client rect to with=64*4*4+5*10, high=64*4*2+3*10
	AdjWndSize();

	CDC* pClientDC = GetDC();
	
	RECT clientRect;
	clientRect.left = 0; clientRect.top = 0;
	clientRect.right = (((256+10)*4)+10);
	clientRect.bottom = (((256+10)*8)+10);

	m_DCClient.CreateCompatibleDC(pClientDC);
	m_bmpClient.CreateCompatibleBitmap(pClientDC, clientRect.right-clientRect.left, clientRect.bottom-clientRect.top);
	m_DCClient.SelectObject(&m_bmpClient);

	m_DCBlk.CreateCompatibleDC(pClientDC);
	m_bmpBlk.CreateCompatibleBitmap(pClientDC, 1024, 512);
	m_DCBlk.SelectObject(&m_bmpBlk);

	m_pBmpInfo->bmiHeader.biWidth = 128;
	m_pBmpInfo->bmiHeader.biHeight = -64;

	m_GridPen.CreatePen(PS_SOLID, 1, RGB(128,128,128) );
    m_LinePen.CreatePen(PS_SOLID, 1, RGB(0,255,0) );

	m_nCurrDispBucket = 0;
	
	return 0;
}

void CWndDispCointime::AdjWndSize()
{
	RECT wndRect,clientRect;
	int WND_WIDTH,WND_HIGH;

	switch(m_DispMode){
	case mode_allbucket:
	case mode_1bucket:
		 WND_WIDTH = 1024+20;	  //512
		 WND_HIGH  = 512+20;	  //512
		 break;
	case mode_8blk_1d:
		 WND_WIDTH = (((256+10)*4)+10);	  //=1114
		 WND_HIGH  = (((256+10)*2)+10);	  //=602
		 break;
	default:
		 ASSERT(FALSE);
		 return;
	}

	GetWindowRect(&wndRect);
	GetClientRect(&clientRect);
	wndRect.right = wndRect.right + (WND_WIDTH-(clientRect.right-clientRect.left));
	wndRect.bottom = wndRect.bottom + (WND_HIGH-(clientRect.bottom-clientRect.top));
	MoveWindow(&wndRect);
}

void CWndDispCointime::GetBlockRect(int nBlock, RECT *pRect)
{
	int x, y;
	switch(m_DispMode)
	{
	case mode_allbucket:
	case mode_1bucket:  //w=1024 h=512
		 pRect->left = 10;
		 pRect->right = 1024+10;
		 pRect->top = 10;
		 pRect->bottom = 512+10;
		 break;

	case mode_8blk_1d:  //w=256 h=256
		 x = nBlock%4; y=nBlock/4;
		 pRect->left = x*256 + (x+1)*10;
		 pRect->right = pRect->left+256;
		 pRect->top = y*256 + (y+1)*10;
		 pRect->bottom = pRect->top+256;
		 break;

	default:
		 ASSERT(FALSE);
		 return;
	}
}

void CWndDispCointime::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	RECT rect;
	int blk,time;

	switch( m_DispMode ){
	case mode_allbucket:   
	case mode_1bucket:
		 GetBlockRect(0, &rect);
		 if( PtInRect(&rect, point) )
			 time = (point.x-rect.left)/4-127;
		 else
			 return;
		 break;
		 
	case mode_8blk_1d:
		 for( blk=0; blk<8; blk++){
			 GetBlockRect(blk, &rect);
			 if( PtInRect(&rect, point) ) return;
		 }
		 return;
		 break;

	default:
		 ASSERT(FALSE);
		 break;
	}
	
	Draw(GetDC());

	char szTemp[128];
	sprintf(szTemp, "Time=%d", time);
	GetDC()->TextOut(512+10,10, szTemp, strlen(szTemp));

}

void CWndDispCointime::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	RECT rect;
	int i;

	switch( m_DispMode ){
	case mode_allbucket:   
		 return;

	case mode_1bucket:
		 GetBlockRect(0, &rect);
		 if( PtInRect(&rect, point) )
			 m_DispMode = mode_8blk_1d;
		 else
			 return;
		 break;
		 
	case mode_8blk_1d:
		 for( i=0; i<8; i++){
			 GetBlockRect(i, &rect);
			 if( PtInRect(&rect, point) ) return;
		 }
		 m_DispMode = mode_1bucket;
		 break;

	default:
		 ASSERT(FALSE);
		 break;
	}
	
	AdjWndSize();
	Draw(GetDC());
}

void CWndDispCointime::Draw(CDC *pDC)
{
	RECT rect;
	CPen* pOldPen;
	char szTemp[128];

	GetClientRect(&rect);
	::FillRect(m_DCClient.m_hDC, &rect, m_hBkBrush);

	int blk=0,buck=0,i,x,y,ymax, ymax_channle;
	int time[256];
	memset(time, 0, 256*4);

	if( m_DispMode==mode_allbucket ){

		ymax = 0;
		for(buck=0; buck<15; buck++){
			for( blk=0; blk<8; blk++){
				for(i=0; i<256; i++){
					time[i] += m_nDispData[buck][blk][i];
					if( time[i] > ymax){
						ymax=time[i];
						ymax_channle = i-127;
					}
				}
			}
		}

		if( ymax==0 ) ymax = 10;
		
		rect.left =0; rect.right=1024;
		rect.top=0;rect.bottom=512;
		::FillRect(m_DCBlk.m_hDC, &rect, m_hBkBrush);
		pOldPen = m_DCBlk.SelectObject(&m_LinePen);

		y = 512 - (512*time[0])/ymax;
		m_DCBlk.MoveTo(0, y);
		for(i=1; i<256; i++){
			y = 512 - (512*time[i])/ymax;
			m_DCBlk.LineTo(i*4, y);
		}

		GetBlockRect(0,&rect);
		m_DCClient.StretchBlt(rect.left, rect.top, rect.right-rect.left, rect.bottom-rect.top, 
							  &m_DCBlk,
							  0,0,1024,512,
							  SRCCOPY
							 );

		m_DCBlk.SelectObject(pOldPen);

		DrawGrid(&m_DCClient, &rect, 4);

		sprintf(szTemp, "Bucket%2.2d <-> all Bucket    Peak Channel : %d", m_nSelectedBucket, ymax_channle);
		m_DCClient.TextOut(rect.left,rect.top, szTemp, strlen(szTemp)); 
	}else if( m_DispMode==mode_1bucket ){

		ymax = 0;
		for( blk=0; blk<8; blk++){
			for(i=0; i<256; i++){
				time[i] += m_nDispData[m_nCurrDispBucket][blk][i];
				if( time[i] > ymax){
					ymax=time[i];
					ymax_channle = i-127;
				}
			}
		}

		if( ymax==0 ) ymax = 10;
		
		rect.left =0; rect.right=1024;
		rect.top=0;rect.bottom=512;
		::FillRect(m_DCBlk.m_hDC, &rect, m_hBkBrush);
		pOldPen = m_DCBlk.SelectObject(&m_LinePen);

		y = 512 - (512*time[0])/ymax;
		m_DCBlk.MoveTo(0, y);
		for(i=1; i<256; i++){
			y = 512 - (512*time[i])/ymax;
			m_DCBlk.LineTo(i*4, y);
		}

		GetBlockRect(0,&rect);
		m_DCClient.StretchBlt(rect.left, rect.top, rect.right-rect.left, rect.bottom-rect.top, 
							  &m_DCBlk,
							  0,0,1024,512,
							  SRCCOPY
							 );

		m_DCBlk.SelectObject(pOldPen);

		DrawGrid(&m_DCClient, &rect, 4);
		
		sprintf(szTemp, "Bucket%2.2d <-> Bucket%2.2d    Peak Channel : %d", m_nSelectedBucket, m_Bucket5B[m_nCurrDispBucket], ymax_channle);
		m_DCClient.TextOut(rect.left,rect.top, szTemp, strlen(szTemp)); 

	}else if( m_DispMode==mode_8blk_1d) {
		ymax = 0;
		for( blk=0; blk<8; blk++){
			for(i=0; i<256; i++){
				time[i] = m_nDispData[m_nCurrDispBucket][blk][i];
				if( time[i] > ymax) ymax=time[i];
			}
		}
		if( ymax==0 ) ymax = 10;

		for(blk=0; blk<8; blk++){
			for(i=0; i<256; i++){
				time[i] = m_nDispData[m_nCurrDispBucket][blk][i];
				if( time[i] > ymax) ymax=time[i];
			}
			
			rect.left =0; rect.right=1024;
			rect.top=0;   rect.bottom=512;
			::FillRect(m_DCBlk.m_hDC, &rect, m_hBkBrush);
			pOldPen = m_DCBlk.SelectObject(&m_LinePen);

			y = 256 - (256*time[0])/ymax;
			m_DCBlk.MoveTo(0, y);
			for(i=1; i<256; i++){
				y = 256 - (256*time[i])/ymax;
				m_DCBlk.LineTo(i, y);
			}

			GetBlockRect(blk,&rect);
			m_DCClient.StretchBlt(rect.left, rect.top, rect.right-rect.left, rect.bottom-rect.top, 
								  &m_DCBlk,
								  0,0,256,256,
								  SRCCOPY
								 );

			m_DCBlk.SelectObject(pOldPen);

			DrawGrid(&m_DCClient, &rect, 4);

			sprintf(szTemp, "Bucket%2.2d <-> Bucket%2.2d Block%2.2d)", m_nSelectedBucket, m_Bucket5B[m_nCurrDispBucket], blk);
			m_DCClient.TextOut(rect.left,rect.top, szTemp, strlen(szTemp)); 
		}
	}else{
		ASSERT(FALSE);
	}
	//m_DCClient.SelectObject(pOldPen);

	GetClientRect(&rect);
	pDC->BitBlt(0,0, rect.right-rect.left, rect.bottom-rect.top,
		        &m_DCClient, 0,0, SRCCOPY);
}

void CWndDispCointime::DrawGrid(CDC *pDC, const RECT *pRect, int GridSize)
{
	
	CPen* pOldPen = (CPen*)pDC->SelectObject(&m_GridPen);
	if( m_DispMode==mode_allbucket || m_DispMode==mode_1bucket ){
/*		for(int i=pRect->left; i<pRect->right; i+=GridSize){
			pDC->MoveTo(i, pRect->top);
			pDC->LineTo(i, pRect->bottom);
		}
		for(i=pRect->top; i<pRect->bottom; i+=GridSize){
			pDC->MoveTo(pRect->left, i);
			pDC->LineTo(pRect->right, i);
		} 
*/
		//time zero
		pDC->MoveTo(pRect->left-1+(pRect->right-pRect->left)/2, pRect->top);
		pDC->LineTo(pRect->left-1+(pRect->right-pRect->left)/2, pRect->bottom);
	}else if( m_DispMode==mode_8blk_1d){
		pDC->MoveTo(pRect->left-1+(pRect->right-pRect->left)/2, pRect->top);
		pDC->LineTo(pRect->left-1+(pRect->right-pRect->left)/2, pRect->bottom);
	}else{
		ASSERT(FALSE);
	}

	pDC->SelectObject(pOldPen);

	RECT rectEdge;
	rectEdge.left = pRect->left-2;
	rectEdge.right = pRect->right+2;
	rectEdge.top = pRect->top-2;
	rectEdge.bottom = pRect->bottom+2;
	pDC->DrawEdge(&rectEdge, EDGE_BUMP,BF_RECT);
}

void CWndDispCointime::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	RECT rect;
	GetClientRect(&rect);
	dc.BitBlt(0,0, rect.right-rect.left, rect.bottom-rect.top,
		        &m_DCClient, 0,0, SRCCOPY);
}

void CWndDispCointime::OnBucketAll() 
{
	// TODO: Add your command handler code here
	m_DispMode = mode_allbucket;

	Draw(GetDC());
}

void CWndDispCointime::OnBucket0() 
{
	// TODO: Add your command handler code here
	m_DispMode = mode_1bucket;
	m_nCurrDispBucket = 0;

	Draw(GetDC());
}

void CWndDispCointime::OnBucket1() 
{
	// TODO: Add your command handler code here
	m_DispMode = mode_1bucket;
	m_nCurrDispBucket = 1;

	Draw(GetDC());
}

void CWndDispCointime::OnBucket2() 
{
	// TODO: Add your command handler code here
	m_DispMode = mode_1bucket;
	m_nCurrDispBucket = 2;

	Draw(GetDC());
}

void CWndDispCointime::OnBucket3() 
{
	// TODO: Add your command handler code here
	m_DispMode = mode_1bucket;
	m_nCurrDispBucket = 3;

	Draw(GetDC());
}

void CWndDispCointime::OnBucket4() 
{
	// TODO: Add your command handler code here
	m_DispMode = mode_1bucket;
	m_nCurrDispBucket = 4;

	Draw(GetDC());
}

void CWndDispCointime::OnBucket5() 
{
	// TODO: Add your command handler code here
	m_DispMode = mode_1bucket;
	m_nCurrDispBucket = 5;

	Draw(GetDC());
	
}

void CWndDispCointime::OnBucket6() 
{
	// TODO: Add your command handler code here
	m_DispMode = mode_1bucket;
	m_nCurrDispBucket = 6;

	Draw(GetDC());
	
}

void CWndDispCointime::OnBucket7() 
{
	// TODO: Add your command handler code here
	m_DispMode = mode_1bucket;
	m_nCurrDispBucket = 7;

	Draw(GetDC());
	
}

void CWndDispCointime::OnBucket8() 
{
	// TODO: Add your command handler code here
	m_DispMode = mode_1bucket;
	m_nCurrDispBucket = 8;

	Draw(GetDC());
	
}

void CWndDispCointime::OnBucket9() 
{
	// TODO: Add your command handler code here
	m_DispMode = mode_1bucket;
	m_nCurrDispBucket = 9;

	Draw(GetDC());
	
}

void CWndDispCointime::OnBucket10() 
{
	// TODO: Add your command handler code here
	m_DispMode = mode_1bucket;
	m_nCurrDispBucket = 10;

	Draw(GetDC());
	
}

void CWndDispCointime::OnBucket11() 
{
	// TODO: Add your command handler code here
	m_DispMode = mode_1bucket;
	m_nCurrDispBucket = 11;

	Draw(GetDC());
	
}

void CWndDispCointime::OnBucket12() 
{
	// TODO: Add your command handler code here
	m_DispMode = mode_1bucket;
	m_nCurrDispBucket = 12;

	Draw(GetDC());
	
}

void CWndDispCointime::OnBucket13() 
{
	// TODO: Add your command handler code here
	m_DispMode = mode_1bucket;
	m_nCurrDispBucket = 13;

	Draw(GetDC());
	
}

void CWndDispCointime::OnBucket14() 
{
	// TODO: Add your command handler code here
	m_DispMode = mode_1bucket;
	m_nCurrDispBucket = 14;

	Draw(GetDC());
	
}


void CWndDispCointime::OnRButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	int fov;
	for(fov=0; fov<15; fov++){
		if( m_bDataValid[fov] )
			m_menu.EnableMenuItem(IDM_BUCKET0+fov, MF_ENABLED);
		else
			m_menu.EnableMenuItem(IDM_BUCKET0+fov, MF_GRAYED);
	}

	if( m_DispMode == mode_allbucket){
		m_menu.CheckMenuItem(IDM_BUCKETALL, MF_CHECKED);
		for(fov=0; fov<15; fov++)
			m_menu.CheckMenuItem(IDM_BUCKET0+fov, MF_UNCHECKED);
	}else{
		m_menu.CheckMenuItem(IDM_BUCKETALL, MF_UNCHECKED);
		for(fov=0; fov<15; fov++){
			if( fov==m_nCurrDispBucket )
				m_menu.CheckMenuItem(IDM_BUCKET0+fov, MF_CHECKED);
			else
				m_menu.CheckMenuItem(IDM_BUCKET0+fov, MF_UNCHECKED);
		}
	}

	
	CWndDisp::OnRButtonUp(nFlags, point);
}

BOOL CWndDispCointime::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt) 
{
	// TODO: Add your message handler code here and/or call default
	int offset = zDelta/WHEEL_DELTA;

	switch( m_DispMode ){
	case mode_allbucket:
		 if( offset > 0) return TRUE;
		 if( offset+IDM_BUCKETALL > IDM_BUCKET14) return TRUE;
		 m_nCurrDispBucket = -1*offset;
		 m_DispMode = mode_1bucket;
		 break;

	case mode_1bucket:
	case mode_8blk_1d:
		 offset *= -1;
		 if( offset+m_nCurrDispBucket < -1) return TRUE;
		 if( offset+m_nCurrDispBucket == -1){
			 m_nCurrDispBucket = 0;
			 m_DispMode = mode_allbucket;
			 break;
		 }
		 if( offset+m_nCurrDispBucket > 14) return TRUE;
		 m_nCurrDispBucket += offset;
		 break;

	default:
		 ASSERT(FALSE);
		 return TRUE;
		 
	}


	AdjWndSize();
	Draw(GetDC());

	return CWndDisp::OnMouseWheel(nFlags, zDelta, pt);
}
