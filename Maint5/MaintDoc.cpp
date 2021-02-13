// MaintDoc.cpp : implementation of the CMaintDoc class
//

#include "stdafx.h"
#include "Maint.h"
#include "MainFrm.h"
#include "MaintDoc.h"
#include "MaintView.h"

#include "FileSave.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern int g_Blockindex;
extern int g_DataSample_Mod_Sel;
extern unsigned char g_chFileType;
CString g_strFileName = "�ļ�����";
CString g_strBuckFileName = "Bucket **";

extern BOOL g_bOperating_32bkEffectEvaluate;
extern CString	g_strOperating_32bkEffectFilePath;

//λ�ñ߽籣���־
extern BOOL g_bPLimitChangeValid;

//union TYPE 
//{
//	UINT intdata;
//	unsigned char uchdata[4];
//};

TYPE type;

extern int g_Backup_iMidfilter_sdot[64][64];  //��ֵ�˲�����


/////////////////////////////////////////////////////////////////////////////
// CMaintDoc

IMPLEMENT_DYNCREATE(CMaintDoc, CDocument)

BEGIN_MESSAGE_MAP(CMaintDoc, CDocument)
	//{{AFX_MSG_MAP(CMaintDoc)
	ON_COMMAND(IDC_FILE_OPEN, OnFileOpen)
	ON_COMMAND(IDC_MENU_SAVEAS, OnMenuSaveAs)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMaintDoc construction/destruction

CMaintDoc::CMaintDoc()
{
	// TODO: add one-time construction code here
	m_paperColor = RGB(255, 255, 255);
	
	bNewfilepackage = true;
	bFirstFileOpened = true;
	
	memset(m_chrAnolParam,0,8*10);
	memset(BuffDot3w,0,8*64*64*4);
	bFileIsSample = false;

}

CMaintDoc::~CMaintDoc()
{
}


/////////////////////////////////////////////////////////////////////////////
// CMaintDoc serialization

void CMaintDoc::Serialize(CArchive& ar)
{
	
	if (ar.IsStoring()) 
	{
		for(int t=0;t<2;t++) {
			ar<< g_chFileType;
		}
		unsigned char uchr = 0x11;
		for(t=0;t<2;t++) {
			ar<< uchr;
		}

		for(t=0;t<124;t++) {
			ar<< g_chFileType;
		}

		for(int i=0;i<8;i++)	{	//д ����
				for(int j=0;j<64;j++)
					for(int k=0;k<64;k++) {
						ar<<BuffDot3w[i][j][k];
					}
		}
	}
	else{	}
}

/////////////////////////////////////////////////////////////////////////////
// CMaintDoc diagnostics

#ifdef _DEBUG
void CMaintDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CMaintDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////

void CMaintDoc::OnFileOpen() 
{
	CString FilePathName; 

	if (g_bOperating_32bkEffectEvaluate) {
		FilePathName = g_strOperating_32bkEffectFilePath;
	}
	else {
		CFileDialog dlg(TRUE,"dat","*.dat");///TRUEΪOPEN�Ի���FALSEΪSAVE AS�Ի��� 
		dlg.m_ofn.lpstrFilter = ("��3D��������.dat");//
		if(dlg.DoModal()==IDOK) {
			FilePathName=dlg.GetPathName(); 
			g_strBuckFileName = dlg.GetFileName();
		}
		else return;
	}

	bFirstFileOpened = true;
	CFile file;
	if( !file.Open(FilePathName,CFile::modeRead) ) return;

	g_strFileName = FilePathName;
	unsigned char chartemp[128];

	memset(m_chrAnolParam,0,8*10);
	file.Seek(PARA_ANOLPARA_DATA_ADDR,CFile::begin);
	for(int i=0;i<8;i++) {
		file.Read(chartemp,8);
		for(int k=0;k<8;k++) {
			m_chrAnolParam[i][k+1] = chartemp[k];
		}
	}

	file.Seek(FILEHEAD_TYPE_ADDR,CFile::begin);  //��ȡ �ļ�����
	file.Read(chartemp,2);
	BOOL bSampledfile = false;
	if((chartemp[0] == 0x01)&&(chartemp[1] == 0x01)) {
		bSampledfile = true;  //
	}

	file.SeekToBegin();

	memset(BuffDot3w,0,8*64*64*4);
	switch(g_DataSample_Mod_Sel) {
		case 0xc7: //ɢ��
					file.SeekToBegin();
					file.Seek(DATA_SDOT_ADDR,CFile::begin);
					g_chFileType = 0xc7;
					break;
		case 0xc5:	//�ܷ�
					file.Seek(DATA_ESPECT_ADDR,CFile::begin);
					g_chFileType = 0xc5;
					break;
		case 0xc6:  
					//file.Seek(FILEHEAD_TIMEMODE_ATTR_ADDR,CFile::begin);
					//file.Read(chartemp,1);
					//if(chartemp[0] != 0x01) AfxMessageBox("ʱ��������Ч");			

					file.Seek(DATA_TIMEMODE_ADDR,CFile::begin);
					g_chFileType = 0xc6;
					break;
		case 0xc8:	
					//file.Seek(FILEHEAD_FUHEMODE_ATTR_ADDR,CFile::begin);
					//file.Read(chartemp,1);
					//if(chartemp[0] != 0x01) AfxMessageBox("����������Ч");

					file.Seek(DATA_FUHEMODE_ADDR,CFile::begin);
					g_chFileType = 0xc8;
					break;
		default: g_chFileType = 0x00;
					break;
	}

	if(bSampledfile) { //���������ݰ��ļ�        //������
			unsigned char chtemp[10];
			for( i=0;i<8;i++)	{	
				for(int j=0;j<64;j++) {
					for(int k=0;k<64;k++) {
						file.Read(chtemp,2);
						type.uchdata[0] = chtemp[0];
						type.uchdata[1] = chtemp[1];
						BuffDot3w[i][j][k] = type.intdata;
					}
				}
			}
			bFileIsSample = false;
	}

	if(!bSampledfile) { //��������ݰ��ļ�          //������
			for( i=0;i<8;i++)	{	
				for(int j=0;j<64;j++) {
					for(int k=0;k<64;k++) {
						file.Read(type.uchdata,4);
						BuffDot3w[i][j][k] = type.intdata;							
					}
				}
			}
	}

	file.Close();
	memcpy(g_Backup_iMidfilter_sdot,BuffDot3w[g_Blockindex],64*64*4);

	g_bPLimitChangeValid = false;
	
}

void CMaintDoc::OnMenuSaveAs() 
{
	CString strtype;
	switch(g_DataSample_Mod_Sel) {
		case 0xc7:  strtype = "ȷ��Ҫ���� ɢ��������";
					break;
		case 0xc5:	strtype = "ȷ��Ҫ���� ����������";
					break;
		case 0xc6:  strtype = "ȷ��Ҫ���� ʱ��������";
					break;
		case 0xc8:	strtype = "ȷ��Ҫ���� ����������";
					break;
		default:    strtype = "�������ݵ����Ͳ���";				
					break;					
	}
	AfxMessageBox(strtype);

	CFileSave filesave;
	filesave.bNewfilepackage = bNewfilepackage;
	if( IDOK != filesave.DoModal()) {
		return;
	}
	bNewfilepackage = filesave.bNewfilepackage;

	CString FilePathName; 
	CFile savefile;
	if(bNewfilepackage) {
		CFileDialog dlg(FALSE,"dat","*.dat");///TRUEΪOPEN�Ի���FALSEΪSAVE AS�Ի��� 
		dlg.m_ofn.lpstrFilter = ("3D ͼ���� ""*.dat");
		if(dlg.DoModal()==IDOK) FilePathName=dlg.GetPathName(); 
		else return;
		if( !savefile.Open( FilePathName, CFile::modeCreate | CFile::modeWrite) ) {			
		}
		savefile.SetLength( (128*4 + 4)*1024 );			
		
		//��ʼ��Ĭ��λ�ñ߽�ֵ
		savefile.Seek(PARA_PLIMIT_DATA_ADDR,CFile::begin );		
		unsigned char position_X0[8][8];
		unsigned char position_Y0[8][8];
		unsigned char position_X1[8][8];
		unsigned char position_Y1[8][8];
		unsigned char chrtext[4];

		for(int block=0;block<8;block++) {
			for(int i=0;i<8;i++) {
				for(int j=0;j<8;j++) {
					position_X0[i][j] = j*8; 
					position_Y0[i][j] = i*8;
					position_X1[i][j] = (j+1)*8 -1;
					position_Y1[i][j] = (i+1)*8 -1;

					chrtext[0] = position_X0[i][j];
					chrtext[1] = position_Y0[i][j];
					chrtext[2] = position_X1[i][j];
					chrtext[3] = position_Y1[i][j];
					savefile.Write(chrtext,4);
				}
			}
		}
		savefile.Seek(0,CFile::begin);

		//��ʼ�����ױ߽�
		chrtext[0] = 0x01;
		savefile.Seek(FILEHEAD_ELIMIT_ATTR_ADDR,CFile::begin );	
		savefile.Write(chrtext,1);
		savefile.Seek(PARA_ETHLIMIT_DATA_ADDR,CFile::begin);
		chrtext[0] = 5;  
		chrtext[1] = 62;		
		for(block=0;block<8;block++) {
			for(int i=0;i<64;i++) {					
				savefile.Write(chrtext,2);				
			}	
		}
		savefile.Seek(0,CFile::begin);

	} 
	else {
		CFileDialog dlg(TRUE,"dat","*.dat");///TRUEΪOPEN�Ի���FALSEΪSAVE AS�Ի���
		dlg.m_ofn.lpstrFilter = ("3D ͼ���� ""*.dat");
		if(dlg.DoModal()==IDOK) FilePathName=dlg.GetPathName(); 
		else return;
		CFile tempfile;
		if(!tempfile.Open(FilePathName,CFile::modeRead)) {
			AfxMessageBox("���ܽ��б༭���� (û���ҵ����ļ�) \n������ѡ�񱣴��ļ��Ĳ�������!");
			return;
		}
		tempfile.Close();
		if( !savefile.Open( FilePathName, CFile::modeReadWrite) ) {			
		}
	}

//	int filelenth = savefile.GetLength();
//	if(filelenth != (128*4 +4)*1024 ) {
//		AfxMessageBox("�ļ����Ȳ��ԣ����治�ɹ���");
//		return;
//	}


	unsigned char chartemp[128];
//	chartemp[0] = 0x00;	  //2003.4.7��ǰ�������4���ֽڵ�����
//	chartemp[1] = 0x00;

	chartemp[0] = 0x01;   //2003.4.7�Ժ󣬱������2���ֽڵ�����
	chartemp[1] = 0x01;
	savefile.Seek(FILEHEAD_TYPE_ADDR,CFile::begin);  //д �ļ�����
	savefile.Write(chartemp,2);


	unsigned char filehead[128];
	filehead[0] = 0x01;
	savefile.Seek(FILEHEAD_ANOLPARA_ATTR_ADDR,CFile::begin );	
	savefile.Write(filehead,1);
	savefile.Seek(PARA_ANOLPARA_DATA_ADDR,CFile::begin);
	for(int i=0;i<8;i++) {				//����ģ�����
		for(int k=0;k<8;k++) {
			 filehead[k]= m_chrAnolParam[i][k+1];
		}
		savefile.Write(filehead,8);
	}


	switch(g_DataSample_Mod_Sel) {
		case 0xc7:  filehead[0] = 0x01;
					savefile.Seek(FILEHEAD_SDOT_ATTR_ADDR,CFile::begin );	
					savefile.Write(filehead,1);
					savefile.Seek(DATA_SDOT_ADDR,CFile::begin);
					break;
		case 0xc5:	filehead[0] = 0x01;
					savefile.Seek(FILEHEAD_ENERGESPECTRUM_ATTR_ADDR,CFile::begin );	
					savefile.Write(filehead,1);
					savefile.Seek(DATA_ESPECT_ADDR,CFile::begin);
					break;
		case 0xc6:  filehead[0] = 0x01;
					savefile.Seek(FILEHEAD_TIMEMODE_ATTR_ADDR,CFile::begin );	
					savefile.Write(filehead,1);
					savefile.Seek(DATA_TIMEMODE_ADDR,CFile::begin);
					break;
		case 0xc8:	filehead[0] = 0x01;
					savefile.Seek(FILEHEAD_FUHEMODE_ATTR_ADDR,CFile::begin );	
					savefile.Write(filehead,1);
					savefile.Seek(DATA_FUHEMODE_ADDR,CFile::begin);
					break;
		default:    AfxMessageBox("�������ݵ����Ͳ���");				
					break;
					return;
	}

	for(i=0;i<8;i++)	{	//д ����
		for(int j=0;j<64;j++) {
				for(int k=0;k<64;k++) {

					type.intdata = BuffDot3w[i][j][k];
					chartemp[0] = type.uchdata[0]; 
					chartemp[1] = type.uchdata[1]; 
			//		chartemp[2] = type.uchdata[2];  //2003.4.7��ǰ�������4���ֽڵ�����
			//		chartemp[3] = type.uchdata[3]; 
			//		savefile.Write(chartemp,4);	

					savefile.Write(chartemp,2);	//2003.4.7�Ժ󣬱������2���ֽڵ�����

				}
		}	
	}

	savefile.Close();

	
}
