// About.cpp : implementation file
//

#include "stdafx.h"
#include "Maint.h"
#include "About.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAbout dialog


CAbout::CAbout(CWnd* pParent /*=NULL*/)
	: CDialog(CAbout::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAbout)
	m_strComment = _T("");
	//}}AFX_DATA_INIT
}


void CAbout::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAbout)
	DDX_Text(pDX, IDC_COMMENT, m_strComment);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAbout, CDialog)
	//{{AFX_MSG_MAP(CAbout)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAbout message handlers

BOOL CAbout::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	CString strbuff;

	m_strComment.Format("程序实现: \r\n");
	strbuff.Format("0908前: 完成大部分实用功能; \r\n");
	m_strComment += strbuff;
	strbuff.Format("0912  : 添加连续打开32文件功能; \r\n");
	m_strComment += strbuff;
	strbuff.Format("          键盘的左右箭头实现文件切换; \r\n");
	m_strComment += strbuff;
	
	strbuff.Format("0913  :定制能谱边界的四周（变宽了）; \r\n");
	m_strComment += strbuff;

	strbuff.Format("0915  :修改散点位置边界;  \r\n");
	m_strComment += strbuff;		
	strbuff.Format("          将边界划分到二个晶体条的边上; \r\n");	 
	m_strComment += strbuff;

    strbuff.Format("0916  :快速计算并保存32Bucket能谱边界;\r\n");
	m_strComment += strbuff;
	strbuff.Format("0916  :打开散点数据的同时打开位置边界（32）;\r\n");
	m_strComment += strbuff;
	strbuff.Format("0916  :打开能谱数据的同时打开能谱边界（32）;\r\n");
	m_strComment += strbuff;
	strbuff.Format("0916  :增加能谱边界“宽”“窄”二个功能;\r\n");
	m_strComment += strbuff;

	strbuff.Format("-----------------------------------\r\n");
	m_strComment += strbuff;

	strbuff.Format("0922  :增加显示64能谱模式下调整边界功能;\r\n");
	m_strComment += strbuff;

	strbuff.Format("0923  :修改效率刷新的“烦琐”BUG;\r\n");
	m_strComment += strbuff;

	strbuff.Format("0923  :增加能谱边界修正参数框;\r\n");
	m_strComment += strbuff;

	strbuff.Format("0924  :增加连续打开32文件时,同时读取模拟数据;\r\n");
	m_strComment += strbuff;

	strbuff.Format("1014  :增加平均温度曲线图显示功能;\r\n");
	m_strComment += strbuff;

	strbuff.Format("1015  :增加位置边界修改后快速保存功能(ctrl＋S);\r\n");
	m_strComment += strbuff;

	strbuff.Format("1016  :修正能谱边界计算程序;\r\n");
	m_strComment += strbuff;

	strbuff.Format("1016  :增加效率低于30％的效率统计(对应文件名输出);\r\n");
	m_strComment += strbuff;


	UpdateData(false);	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
