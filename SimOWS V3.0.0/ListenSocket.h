#if !defined(AFX_LISTENSOCKET_H__3C53B9DF_32CE_418B_BC99_C3FA5500C8F6__INCLUDED_)
#define AFX_LISTENSOCKET_H__3C53B9DF_32CE_418B_BC99_C3FA5500C8F6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ListenSocket.h : header file
//

#include "..\AcqCenter\OWSProtocol.h"
#include "OWSSock.h"

/////////////////////////////////////////////////////////////////////////////
// CListenSocket command target

class CListenSocket : public CSocket
{
// Attributes
public:

// Operations
public:
	CListenSocket();
	virtual ~CListenSocket();

// Overrides
public:
	BOOL SendDatToClient(const void* pBuf, int nLen);
	COWSSock* GetClient();
	void CloseClient();
	BOOL IsConnected();
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CListenSocket)
	public:
	virtual void OnAccept(int nErrorCode);
	//}}AFX_VIRTUAL

	// Generated message map functions
	//{{AFX_MSG(CListenSocket)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

// Implementation
protected:
	COWSSock m_nulSock;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LISTENSOCKET_H__3C53B9DF_32CE_418B_BC99_C3FA5500C8F6__INCLUDED_)
