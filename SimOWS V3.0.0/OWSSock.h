#if !defined(AFX_OWSSOCK_H__CE917C86_A455_4B04_B2C2_646BDAAF66D1__INCLUDED_)
#define AFX_OWSSOCK_H__CE917C86_A455_4B04_B2C2_646BDAAF66D1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// OWSSock.h : header file
//



/////////////////////////////////////////////////////////////////////////////
// COWSSock command target

class COWSSock : public CAsyncSocket
{
// Attributes
public:

// Operations
public:
	COWSSock();
	virtual ~COWSSock();

// Overrides
public:
	void Close();
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COWSSock)
	public:
	virtual int Send(const void* lpBuf, int nBufLen, int nFlags = 0);
	virtual int Receive(void* lpBuf, int nBufLen, int nFlags = 0);
	//}}AFX_VIRTUAL

	// Generated message map functions
	//{{AFX_MSG(COWSSock)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

// Implementation
protected:
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OWSSOCK_H__CE917C86_A455_4B04_B2C2_646BDAAF66D1__INCLUDED_)
