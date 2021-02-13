#if !defined(AFX_PRINTSOCKET_H__2401D294_718C_40F2_A0F3_033695A4B3F9__INCLUDED_)
#define AFX_PRINTSOCKET_H__2401D294_718C_40F2_A0F3_033695A4B3F9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PrintSocket.h : header file
//

typedef struct _dat_head
{
	BYTE soure;
	DWORD dwDatLen;
//	char datebuf[9];
	char timebuf[9];
}DAT_HEAD;

#define TEXTSIZE 1024*4
struct ALL_DATA
{
	DAT_HEAD headdat;
	char szText[TEXTSIZE];
};

#define OWS      0  // TO OWS EDIT
#define FH       1  // TO COIN EDIT
#define SIO      2  // TO EMC EDIT
#define PROTOCOL 3 // TO PROTOCOL EDIT 


/////////////////////////////////////////////////////////////////////////////
// CPrintSocket command target

class CPrintSocket : public CAsyncSocket
{
// Attributes

// Operations
public:
	CPrintSocket();
	virtual ~CPrintSocket();

// Overrides
public:
	void SendToClient(const char* szBuf, HWND hWnd);

protected:
	CAsyncSocket m_NewSocket;

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPrintSocket)
public:
	virtual void OnAccept(int nErrorCode);
	//}}AFX_VIRTUAL

	// Generated message map functions
	//{{AFX_MSG(CPrintSocket)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PRINTSOCKET_H__2401D294_718C_40F2_A0F3_033695A4B3F9__INCLUDED_)
