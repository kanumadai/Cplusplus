#ifndef __MYSOCKET_H__
#define __MYSOCKET_H__

class CMySocket:public CObject
{
public:
	void Attach(SOCKET s);
	BOOL Connect(LPCTSTR lpszHostAddress, UINT nHostPort );
	int Send(const void* pBuf ,int nBufLen);
	int Receive(void* pBuf,int nBufLen);
	void Close();
	SOCKET m_hSocket;

public:
	CMySocket();
	~CMySocket();

protected:
	SOCKADDR_IN m_addr;
};

#endif 
