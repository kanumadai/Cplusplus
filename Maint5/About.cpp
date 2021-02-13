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

	m_strComment.Format("����ʵ��: \r\n");
	strbuff.Format("0908ǰ: ��ɴ󲿷�ʵ�ù���; \r\n");
	m_strComment += strbuff;
	strbuff.Format("0912  : ���������32�ļ�����; \r\n");
	m_strComment += strbuff;
	strbuff.Format("          ���̵����Ҽ�ͷʵ���ļ��л�; \r\n");
	m_strComment += strbuff;
	
	strbuff.Format("0913  :�������ױ߽�����ܣ�����ˣ�; \r\n");
	m_strComment += strbuff;

	strbuff.Format("0915  :�޸�ɢ��λ�ñ߽�;  \r\n");
	m_strComment += strbuff;		
	strbuff.Format("          ���߽绮�ֵ������������ı���; \r\n");	 
	m_strComment += strbuff;

    strbuff.Format("0916  :���ټ��㲢����32Bucket���ױ߽�;\r\n");
	m_strComment += strbuff;
	strbuff.Format("0916  :��ɢ�����ݵ�ͬʱ��λ�ñ߽磨32��;\r\n");
	m_strComment += strbuff;
	strbuff.Format("0916  :���������ݵ�ͬʱ�����ױ߽磨32��;\r\n");
	m_strComment += strbuff;
	strbuff.Format("0916  :�������ױ߽硰����խ����������;\r\n");
	m_strComment += strbuff;

	strbuff.Format("-----------------------------------\r\n");
	m_strComment += strbuff;

	strbuff.Format("0922  :������ʾ64����ģʽ�µ����߽繦��;\r\n");
	m_strComment += strbuff;

	strbuff.Format("0923  :�޸�Ч��ˢ�µġ�������BUG;\r\n");
	m_strComment += strbuff;

	strbuff.Format("0923  :�������ױ߽�����������;\r\n");
	m_strComment += strbuff;

	strbuff.Format("0924  :����������32�ļ�ʱ,ͬʱ��ȡģ������;\r\n");
	m_strComment += strbuff;

	strbuff.Format("1014  :����ƽ���¶�����ͼ��ʾ����;\r\n");
	m_strComment += strbuff;

	strbuff.Format("1015  :����λ�ñ߽��޸ĺ���ٱ��湦��(ctrl��S);\r\n");
	m_strComment += strbuff;

	strbuff.Format("1016  :�������ױ߽�������;\r\n");
	m_strComment += strbuff;

	strbuff.Format("1016  :����Ч�ʵ���30����Ч��ͳ��(��Ӧ�ļ������);\r\n");
	m_strComment += strbuff;


	UpdateData(false);	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
