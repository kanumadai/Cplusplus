#include "stdafx.h"
#include "FHows.h"
#include "FHDlg.h"
#include "DbgPrt.h"

extern HWND g_hWndFH;

COWS::COWS()
{
}

COWS::~COWS()
{
}

typedef union _fh_dat
{
	DWORD dwDat;
	struct{
		DWORD ring1  : 5;
		DWORD row1   : 3;
		DWORD modul1 : 1;
		DWORD buck1  : 5;
		//
		DWORD ring2  : 5;
		DWORD row2   : 3;
		DWORD modul2 : 1;
		DWORD buck2  : 5;
		//
		DWORD e_flag : 1;  //能量符合标记:  1->其他符合；0->光电符合
		DWORD t_flag : 1;  //时间符合标记： 1->立即符合；0->延迟符合
		DWORD flag   : 2;
	};
}FH_DAT;

typedef union _fh_rowdat
{
	DWORD dwDat;
	struct{
		DWORD time : 7;
		DWORD strip : 6;
		DWORD energy : 1;
		DWORD valid : 1;
		DWORD block : 3;
		DWORD bucket : 5;
	};
}FH_ROWDAT;

BOOL COWS::SendFHDat(BYTE *pBuf, DWORD dwBufLen)
{
	char szTemp[512];
	
	FH_DAT dat;
	FH_ROWDAT rowdat;

	int i,dispCnt;
	DWORD* pDW = (DWORD*)pBuf;
	
	dispCnt=0;
	if(m_bTest)
		DbgPrt("Display First 10 Valid Event\r\n", g_hWndFH);
	else
		DbgPrt("Display First 10 FH Event\r\n", g_hWndFH);
	for(i=0; i<dwBufLen/4; i++)
	{
	   dat.dwDat = pDW[i];
	   rowdat.dwDat = dat.dwDat;

	   if(m_bTest)
	   {
		    if(rowdat.valid == 0) continue;
			sprintf(szTemp, "ROWDat %8.8x =>(time=%d,strip=%d,energy flag=%d,valid=%d,block=%d,bucket=%d\r\n",
				             rowdat.dwDat,
					      	 rowdat.time, rowdat.strip, rowdat.energy,
						     rowdat.valid, rowdat.block,rowdat.bucket 
			       );
			dispCnt++;
			if( dispCnt >= 10 ) break;
	   }
	   else
	   {
			sprintf(szTemp, "FH %8.8x =>非光电符合:%d,立即符合:%d,(ring=%d,module=%d,row=%d,bucket=%d)<=>(ring=%d,module=%d,row=%d,bucket=%d)\r\n", 
				            dat.dwDat,
					        dat.e_flag, dat.t_flag,
		                    dat.ring1, dat.modul1, dat.row1, dat.buck1,
					        dat.ring2, dat.modul2, dat.row2, dat.buck2
			       );
			dispCnt++;
			if( dispCnt >= 10 ) break;
	   }

	   DbgPrt(szTemp, g_hWndFH);

	}
	
	if(m_bTest)
	{
		int j=dwBufLen/4;
		int errCnt=0, energyCnt=0;
		for(i=0; i<j; i++)
		{
			rowdat.dwDat = pDW[i];
			if( rowdat.valid ){
				errCnt++;
				if( rowdat.energy==1 ) energyCnt++;
			}
		}
		sprintf(szTemp, "有效事件记树=%d, 光电峰记数=%d\r\n\r\n", errCnt, energyCnt);
		DbgPrt(szTemp, g_hWndFH);
	}
	else
	{
		sprintf(szTemp, "符合事例记数=%d\r\n\r\n", dwBufLen/4);
		DbgPrt(szTemp, g_hWndFH);
	}

   	//((CFHDlg*)AfxGetMainWnd())->m_DispWnd.DispFHDat(pBuf, dwBufLen);
	
	return TRUE;
}

void COWS::SetDispMode(BOOL bTest)
{
	m_bTest = bTest;
}
