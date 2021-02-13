// PrintSocket.cpp : implementation file
//
#include "stdafx.h"
//#include "ACQCenter.h"
#include "PrintSocket.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


extern HWND g_hWndOWS;
extern HWND g_hWndProtocol;
extern HWND g_hWndSIO;
extern HWND g_hWndFH;

/////////////////////////////////////////////////////////////////////////////
// CPrintSocket

CPrintSocket::CPrintSocket()
{
}

CPrintSocket::~CPrintSocket()
{
	if(m_hSocket != INVALID_SOCKET)
		Close();
}


// Do not edit the following lines, which are needed by ClassWizard.
#if 0
BEGIN_MESSAGE_MAP(CPrintSocket, CAsyncSocket)
	//{{AFX_MSG_MAP(CPrintSocket)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
#endif	// 0

/////////////////////////////////////////////////////////////////////////////
// CPrintSocket member functions

void CPrintSocket::OnAccept(int nErrorCode) 
{
	// TODO: Add your specialized code here and/or call the base class
	if( nErrorCode == 0 )
	{
		//接到连接莵E丒调用Accept函数
		if( m_NewSocket.m_hSocket != INVALID_SOCKET ){
			m_NewSocket.Close();
			m_NewSocket.m_hSocket = INVALID_SOCKET;
		}
		Accept( m_NewSocket ); 
	
	}
	//CAsyncSocket::OnAccept(nErrorCode);
}

void CPrintSocket::SendToClient(const char *szBuf, HWND hWnd)
{
	if( m_hSocket == INVALID_SOCKET ) return;
	if( m_NewSocket.m_hSocket == INVALID_SOCKET ) return;
	
	ALL_DATA sendDat;

	sendDat.headdat.dwDatLen = strlen(szBuf);;
	
    _strtime( sendDat.headdat.timebuf ); 

	if( hWnd == g_hWndOWS )	 	      sendDat.headdat.soure = OWS;
	else if( hWnd == g_hWndFH )	      sendDat.headdat.soure = FH;
	else if( hWnd == g_hWndSIO )      sendDat.headdat.soure = SIO;
	else if( hWnd == g_hWndProtocol ) sendDat.headdat.soure = PROTOCOL;
	else ASSERT(FALSE);

	strcpy(sendDat.szText, szBuf);

	int datLen = sizeof(sendDat.headdat)+sendDat.headdat.dwDatLen;
	int sended = 0;

	//send head 
	while( sended < datLen ) {
		int ret = m_NewSocket.Send( &sendDat, datLen );
		if( (ret==SOCKET_ERROR) || (ret==0) ) {
			if (GetLastError() == WSAEWOULDBLOCK) break;
            else {
                TCHAR szError[256];
                wsprintf(szError, "Server Socket failed to send: %d", 
                          GetLastError());
				return;
			}

		}
		else
			sended += ret;
	}
}
