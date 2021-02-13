#include "stdafx.h"
#include "MySocket.h"
#include "DbgPrt.h"
#include "Winsock.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

extern HWND g_hWndOWS;

CMySocket::CMySocket()
{
	m_hSocket=INVALID_SOCKET;
}

CMySocket::~CMySocket()
{
}

void CMySocket::Attach(SOCKET s)
{
	//ASSERT(m_hSocket == INVALID_SOCKET);
	m_hSocket = s;

	int ret = WSAAsyncSelect(m_hSocket, AfxGetMainWnd()->m_hWnd, 0, 0);
	//ASSERT(ret==0);
	ULONG arg=0;
	ret = ioctlsocket(m_hSocket, FIONBIO, &arg);
	//ASSERT(ret==0);
}

BOOL CMySocket::Connect(LPCTSTR szHostAddr, UINT nHostPort)
{
	if( m_hSocket )	{
		closesocket(m_hSocket);
		m_hSocket = INVALID_SOCKET ;
	}
	m_hSocket = socket(AF_INET, SOCK_STREAM,0);
	//ASSERT(m_hSocket != INVALID_SOCKET );

	m_addr.sin_family = AF_INET;
	m_addr.sin_addr.S_un.S_addr = inet_addr(szHostAddr);
	m_addr.sin_port = htons(nHostPort);

	int ret = connect(m_hSocket, (LPSOCKADDR)&m_addr, sizeof(m_addr));

	if( ret == 0){
		ret = WSAAsyncSelect(m_hSocket, AfxGetMainWnd()->m_hWnd, 0, 0);
		//ASSERT(ret==0);
		ULONG arg=0;
		ret = ioctlsocket(m_hSocket, FIONBIO, &arg);
		//ASSERT(ret==0);
		return TRUE;
	}
	else 
		return FALSE;
}

int CMySocket::Send(const void* pBuf ,int nBufLen)
{
	if(m_hSocket == INVALID_SOCKET) return 0;

	char szTemp[128];
	int sended;
	sended = send( m_hSocket, (const char*)pBuf, nBufLen, 0);
	if(sended != nBufLen){
		sprintf(szTemp, "CMySocket Send Error,Code=%d\r\n", WSAGetLastError() );
		DbgPrt(szTemp, g_hWndOWS);
		return 0;
	}		  
	return sended;
}

int CMySocket::Receive(void* pBuf,int nBufLen)
{
	memset(pBuf, 0xff, nBufLen);

	if(m_hSocket == INVALID_SOCKET) return 0;

	char szTemp[128];
	
	int recved=0;
	while(recved<nBufLen){
		int ret = recv(m_hSocket, (char*)pBuf+recved, nBufLen-recved, 0);
		if( ret == 0 || ret==SOCKET_ERROR ) {
			sprintf(szTemp, "CMySocket Receive Error,code=%d\r\n", WSAGetLastError() );
			DbgPrt(szTemp, g_hWndOWS);
			return 0;
		}
		#ifdef _DEBUG
		if(ret != nBufLen){
			//TRACE("CMySocket Recv Condition(1),len=%d\r\n", ret );
			//DbgPrt("CMySocket Recv Condition(1)\r\n", g_hWndOWS );
		}else{
			//if(nBufLen!=8) TRACE("CMySocket Recv Condition(2),len=%d\r\n", ret );
		}
		#endif
		recved += ret;
	}

	return  recved;
}

void CMySocket::Close()
{
	if(m_hSocket != INVALID_SOCKET ) {
		closesocket(m_hSocket);
		m_hSocket = INVALID_SOCKET ;
	}
}

