#include "StdAfx.h"
#include "BuckFile.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CBuckFile::CBuckFile()
{
	m_szFile[0] = 0;  
	memset(m_FileHead, 0, sizeof(m_FileHead));
}


CBuckFile::~CBuckFile()
{
}

BOOL CBuckFile::CheckFileValid()
{
	return FALSE;
}

BOOL CBuckFile::Open(const char *szFile, BOOL bCreate)
{
	CFile file;
	BOOL bRet;

	if( strlen(szFile)<=0 || strlen(szFile)>sizeof(m_szFile) ) return FALSE;

	if( bCreate ){
		bRet = file.Open(szFile, CFile::modeCreate|CFile::modeWrite);
		if( !bRet )	return FALSE;

		int len = FILE_LEN;
		BYTE* pBuf = new BYTE[len];
		memset(pBuf, 0 , len);
		((int*)pBuf)[0] = len;

		file.Write(pBuf, len);
		delete [] pBuf;

		memset(m_FileHead, 0, sizeof(m_FileHead));
		((int*)m_FileHead)[0] = len;
	}else{
		bRet = file.Open(szFile, CFile::modeRead);
		if( !bRet )	return FALSE;

		if( file.GetLength() != FILE_LEN ){
			file.Close();
			return FALSE;
		}
		file.Read(m_FileHead, sizeof(m_FileHead));
		if( ((int*)m_FileHead)[0] != FILE_LEN ){
			file.Close();
			return FALSE;
		}
	}

	file.Close();
	strcpy(m_szFile, szFile);

	return TRUE;
}

void CBuckFile::Close()
{
	memset(m_FileHead, 0, sizeof(m_FileHead));
	m_szFile[0] = 0;
}


BOOL CBuckFile::SaveData_SD(const void *pBuf, int len)
{
	CFile file;

	if( len!= SD_LEN ) return FALSE;
	if( !file.Open(m_szFile, CFile::modeWrite) ) return FALSE;

	m_FileHead[20] = 1;
	file.Write(m_FileHead, sizeof(m_FileHead));

	file.Seek(4096, CFile::begin);
	file.Write(pBuf, len);

	file.Close();
	return TRUE;
}

BOOL CBuckFile::SaveData_Energy(const void *pBuf, int len)
{
	CFile file;

	if( len!= ENERGY_LEN ) return FALSE;
	if( !file.Open(m_szFile, CFile::modeWrite) ) return FALSE;

	m_FileHead[21] = 1;
	file.Write(m_FileHead, sizeof(m_FileHead));

	file.Seek(4096+SD_LEN, CFile::begin);
	file.Write(pBuf, len);

	file.Close();
	return TRUE;
}

BOOL CBuckFile::SaveData_Time(const void *pBuf, int len)
{
	CFile file;

	if( len != TIME_LEN ) return FALSE;
	if( !file.Open(m_szFile, CFile::modeWrite) ) return FALSE;

	m_FileHead[22] = 1;
	file.Write(m_FileHead, sizeof(m_FileHead));

	file.Seek(4096+SD_LEN+ENERGY_LEN, CFile::begin);
	file.Write(pBuf, len);

	file.Close();
	return TRUE;
}

BOOL CBuckFile::SaveData_Coin(const void *pBuf, int len)
{
	CFile file;

	if( len != COIN_LEN ) return FALSE;
	if( !file.Open(m_szFile, CFile::modeWrite) ) return FALSE;

	m_FileHead[23] = 1;
	file.Write(m_FileHead, sizeof(m_FileHead));

	file.Seek(4096+SD_LEN+ENERGY_LEN+TIME_LEN, CFile::begin);
	file.Write(pBuf, len);

	file.Close();
	return TRUE;
}


