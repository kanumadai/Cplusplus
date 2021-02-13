// WndDisp.cpp : implementation file
//

#include "stdafx.h"
#include "SimOWS.h"
#include "WndDisp.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

const long int CWndDisp::colorNumber[64] = {
		 0x00000000,0x000c0000,0x00180000,0x00240000,
	     0x00300000,0x003c0000,0x00480000,0x00540000,
		 0x00600000,0x006c0000,0x00780000,0x00840000,
		 0x00900000,0x009c0000,0x00a80000,0x00b40000,
		 0x00c00000,0x00cc0000,0x00d80000,0x00e40000,
		 0x00f00000,0x00fc0000,0x00ff0000,0x00ff0c00,
		 0x00ff1800,0x00ff2400,0x00ff3000,0x00ff3c00,
		 0x00ff4800,0x00ff5400,0x00ff6000,0x00ff6c00,
		 0x00ff7800,0x00ff8400,0x00ff9000,0x00ff9c00,
		 0x00ffa800,0x00ffb400,0x00ffc000,0x00ffcc00,
		 0x00ffd800,0x00ffe400,0x00fff000,0x00fffc00,
		 0x00ffff00,0x00ffff0c,0x00ffff18,0x00ffff24,
		 0x00ffff30,0x00ffff3c,0x00ffff48,0x00ffff54,
		 0x00ffff60,0x00ffff6c,0x00ffff78,0x00ffff84,
		 0x00ffffc0,0x00ffffcc,0x00ffffd8,0x00ffffe4,
		 0x00fffff0,0x00fffff6,0x00fffffc,0x00ffffff}; 
/*		 0x00000000,0x000000a0,0x000000b0,0x000000c0,
		 0x000000d0,0x000000e0,0x000000f0,0x000000ff,
		 0x000010ff,0x000020ff,0x000030ff,0x000040ff,
		 0x000050ff,0x000060ff,0x000070ff,0x000080ff,
		 0x000090ff,0x0000a0ff,0x0000b0ff,0x0000c0ff,
		 0x0000d0ff,0x0000e0ff,0x0000f0ff,0x0000ffff,
		 0x0010ffff,0x0020fff0,0x0030ffe0,0x0040ffd0,
		 0x0050ffc0,0x0060ffb0,0x0070ffa0,0x0080ff90,
		 0x0090ff80,0x00a0ff70,0x00b0ff60,0x00c0ff50,
		 0x00d0ff40,0x00e0ff30,0x00f0ff20,0x00ffff10,
		 0x00ffff00,0x00fff000,0x00ffe000,0x00ffd000,
		 0x00ffc000,0x00ffb000,0x00ffa000,0x00ff9000,
		 0x00ff8000,0x00ff7000,0x00ff6000,0x00ff5000,
		 0x00ff4000,0x00ff3000,0x00ff2000,0x00ff1000,
		 0x00ff0000,0x00ff2424,0x00ff4949,0x00ff6d6d,
		 0x00ff9292,0x00ffb6b6,0x00ffdbdb,0x00ffffff */

/////////////////////////////////////////////////////////////////////////////
// CWndDisp

CWndDisp::CWndDisp()
{
	m_bDispAll = TRUE;
	m_currBlock = 0;  m_currStrip = 0;
	m_DispMode = mode_8blk_2d;

	m_MarkPen.CreatePen(PS_SOLID, 2, RGB(0,0,255));

	m_bDefaultPAL = TRUE;
	int len = sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD) * 256;
	m_pBmpInfo = (BITMAPINFO*)new BYTE[len];
	m_pBmpInfo->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	m_pBmpInfo->bmiHeader.biWidth = 64;
	m_pBmpInfo->bmiHeader.biHeight = -64;
	m_pBmpInfo->bmiHeader.biPlanes = 1;
	m_pBmpInfo->bmiHeader.biBitCount = 8;
	m_pBmpInfo->bmiHeader.biCompression = BI_RGB;
	m_pBmpInfo->bmiHeader.biSizeImage = 0;
	m_pBmpInfo->bmiHeader.biXPelsPerMeter = 0;
	m_pBmpInfo->bmiHeader.biYPelsPerMeter = 0;
	m_pBmpInfo->bmiHeader.biClrUsed = 256;
	m_pBmpInfo->bmiHeader.biClrImportant = 256;
	for(int i=0; i<64; i++)
	{
		m_pBmpInfo->bmiColors[i].rgbRed = GetBValue( colorNumber[i] );
		m_pBmpInfo->bmiColors[i].rgbGreen = GetGValue( colorNumber[i] );
		m_pBmpInfo->bmiColors[i].rgbBlue = GetRValue( colorNumber[i] );
		m_pBmpInfo->bmiColors[i].rgbReserved = 0;;
	}
}

CWndDisp::~CWndDisp()
{
	delete [] m_pBmpInfo;
}


BEGIN_MESSAGE_MAP(CWndDisp, CWnd)
	//{{AFX_MSG_MAP(CWndDisp)
	ON_COMMAND(IDM_MAINWND, OnShowMainwnd)
	ON_WM_RBUTTONUP()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CWndDisp message handlers

void CWndDisp::OnShowMainwnd() 
{
	// TODO: Add your command handler code here
	AfxGetMainWnd()->BringWindowToTop();
}

void CWndDisp::OnRButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	ClientToScreen(&point);
	CMenu* menu = m_menu.GetSubMenu(0);
	menu->TrackPopupMenu(TPM_LEFTALIGN, point.x, point.y, this);
}

//return the color index
long CWndDisp::ColorTrans(long nCnt, long nMaxCnt)
{
	int ColorDepath;

	int index;

	if( m_bDefaultPAL ) ColorDepath=64;
	else ColorDepath = 256;

	long span=(nMaxCnt+1)/ColorDepath;
	if( span==0 ) return 0;

	index = nCnt/span;
	if( index>=ColorDepath ) return ColorDepath-1;
	else return index;
}

void CWndDisp::OnPalDefault() 
{
	// TODO: Add your command handler code here
	m_bDefaultPAL = TRUE;

	for(int i=0; i<64; i++)
	{
		m_pBmpInfo->bmiColors[i].rgbRed = GetBValue( colorNumber[i] );
		m_pBmpInfo->bmiColors[i].rgbGreen = GetGValue( colorNumber[i] );
		m_pBmpInfo->bmiColors[i].rgbBlue = GetRValue( colorNumber[i] );
		m_pBmpInfo->bmiColors[i].rgbReserved = 0;;
	}
}

void CWndDisp::OnPal256() 
{
	// TODO: Add your command handler code here
	m_bDefaultPAL = FALSE;

	for(int i=0; i<256; i++)
	{
		m_pBmpInfo->bmiColors[i].rgbRed =   i ;
		m_pBmpInfo->bmiColors[i].rgbGreen = i;
		m_pBmpInfo->bmiColors[i].rgbBlue =  i;
		m_pBmpInfo->bmiColors[i].rgbReserved = 0;;
	}	
}

void CWndDisp::OnPalFlow() 
{
	// TODO: Add your command handler code here
	m_bDefaultPAL = FALSE;

	HRSRC hsrc;
	hsrc = FindResource(NULL, (const char*)IDR_PAL_FLOW, "PAL");
	HANDLE hRes = LoadResource(NULL,hsrc);
	if(hRes == NULL){
		AfxMessageBox("Cann't find pal resource");
		return;
	}

	BYTE* rgb = (BYTE*)LockResource(hRes);

	for(int i=0; i<256; i++)
	{
		m_pBmpInfo->bmiColors[i].rgbRed = rgb[i*3+2];
		m_pBmpInfo->bmiColors[i].rgbGreen = rgb[i*3+1];
		m_pBmpInfo->bmiColors[i].rgbBlue = rgb[i*3];
		m_pBmpInfo->bmiColors[i].rgbReserved = 0;;
	}
}

void CWndDisp::OnPalHot() 
{
	// TODO: Add your command handler code here
	m_bDefaultPAL = FALSE;

	HRSRC hsrc;
	hsrc = FindResource(NULL, (const char*)IDR_PAL_HOT, "PAL");
	HANDLE hRes = LoadResource(NULL,hsrc);
	if(hRes == NULL){
		AfxMessageBox("Cann't find pal resource");
		return;
	}

	BYTE* rgb = (BYTE*)LockResource(hRes);

	for(int i=0; i<256; i++)
	{
		m_pBmpInfo->bmiColors[i].rgbRed = rgb[i*3+2];
		m_pBmpInfo->bmiColors[i].rgbGreen = rgb[i*3+1];
		m_pBmpInfo->bmiColors[i].rgbBlue = rgb[i*3];
		m_pBmpInfo->bmiColors[i].rgbReserved = 0;;
	}
}

void CWndDisp::OnPalMap() 
{
	// TODO: Add your command handler code here
	m_bDefaultPAL = FALSE;

	HRSRC hsrc;
	hsrc = FindResource(NULL, (const char*)IDR_PAL_MAP, "PAL");
	HANDLE hRes = LoadResource(NULL,hsrc);
	if(hRes == NULL){
		AfxMessageBox("Cann't find pal resource");
		return;
	}

	BYTE* rgb = (BYTE*)LockResource(hRes);

	for(int i=0; i<256; i++)
	{
		m_pBmpInfo->bmiColors[i].rgbRed = rgb[i*3+2];
		m_pBmpInfo->bmiColors[i].rgbGreen = rgb[i*3+1];
		m_pBmpInfo->bmiColors[i].rgbBlue = rgb[i*3];
		m_pBmpInfo->bmiColors[i].rgbReserved = 0;;
	}
}

void CWndDisp::OnPalNih() 
{
	// TODO: Add your command handler code here
	m_bDefaultPAL = FALSE;

	HRSRC hsrc;
	hsrc = FindResource(NULL, (const char*)IDR_PAL_NIH, "PAL");
	HANDLE hRes = LoadResource(NULL,hsrc);
	if(hRes == NULL){
		AfxMessageBox("Cann't find pal resource");
		return;
	}

	BYTE* rgb = (BYTE*)LockResource(hRes);

	for(int i=0; i<256; i++)
	{
		m_pBmpInfo->bmiColors[i].rgbRed = rgb[i*3+2];
		m_pBmpInfo->bmiColors[i].rgbGreen = rgb[i*3+1];
		m_pBmpInfo->bmiColors[i].rgbBlue = rgb[i*3];
		m_pBmpInfo->bmiColors[i].rgbReserved = 0;;
	}
}

void CWndDisp::MarkPoint(CDC* pDC, int x, int y, int width)
{
	RECT rect;
	rect.left = x - width/2; rect.right = rect.left+width;
	rect.top = y-width/2; rect.bottom = rect.top+width;

	//CPen* pOldPen = pDC->SelectObject(&m_MarkPen);
	pDC->Ellipse(&rect);
	//pDC->SelectObject(&pOldPen);
}
