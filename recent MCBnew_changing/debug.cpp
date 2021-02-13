
/*--------------------------------
功能：可重用显示模块：
	  主要用于Edit 的显示操作；

操作：
	  在主模块中声明 HWND g_hEdit;
	  初始化 g_hEdit =  ::GetDlgItem(m_hWnd, IDC_EDIT_SHOW);
----------------------------------*/

#include "Stdafx.h"
#include "debug.h"

extern HWND g_hEdit;
extern CEdit *pDbgText;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

void DbgPrt(TCHAR* lpszBuf)
{
    if (g_hEdit == NULL) return; 
	
    int len = ::GetWindowTextLength(g_hEdit);
	if(len >= 16*200) ::SendMessage(g_hEdit, EM_SETSEL, 0, len);
	else ::SendMessage(g_hEdit, EM_SETSEL, len, len);
    ::SendMessage(g_hEdit, EM_REPLACESEL, 0, (LPARAM) lpszBuf);

}

