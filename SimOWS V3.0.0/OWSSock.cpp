// OWSSock.cpp : implementation file
//

#include "stdafx.h"
#include "simows.h"
#include "OWSSock.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COWSSock

COWSSock::COWSSock()
{
}

COWSSock::~COWSSock()
{
}


// Do not edit the following lines, which are needed by ClassWizard.
#if 0
BEGIN_MESSAGE_MAP(COWSSock, CAsyncSocket)
	//{{AFX_MSG_MAP(COWSSock)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
#endif	// 0

/////////////////////////////////////////////////////////////////////////////
// COWSSock member functions

int COWSSock::Send(const void* lpBuf, int nBufLen, int nFlags) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CAsyncSocket::Send(lpBuf, nBufLen, nFlags);
}

int COWSSock::Receive(void* lpBuf, int nBufLen, int nFlags) 
{
	// TODO: Add your specialized code here and/or call the base class
	
//	return CAsyncSocket::Receive(lpBuf, nBufLen, nFlags);
	int recved=0;
	while(recved<nBufLen){
		int ret = recv(m_hSocket, (char*)lpBuf+recved, nBufLen-recved, nFlags);
		if( ret == 0 || ret==SOCKET_ERROR ) {
			return 0;
		}
		recved += ret;
	}

	return  recved;

}

void COWSSock::Close()
{
	closesocket(m_hSocket);
	m_hSocket = INVALID_SOCKET;
}
