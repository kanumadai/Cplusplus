#ifndef __BUCKFILE_H__
#define __BUCKFILE_H__

/*
#pragma pack(push)
#pragma pack(1)

typedef struct _buckfile_head{
	DWORD dwFileLen;
	WORD  wFileType;
	BYTE  Reserved1[9];
	//BYTE  
	BYTE  Reserved[122];
}BUCK_FH;

typedef struct _buckfile_param{
	BYTE  Reserved[4096-128];
}BUCKFILE_PARAM;

#pragma pack(pop)
*/

#define SD_LEN      (8*64*64*4)
#define ENERGY_LEN  (8*64*64*4)
#define TIME_LEN    (8*64*128*4)
#define COIN_LEN    (8*64*64*4)
#define FILE_LEN   (4096 + SD_LEN + ENERGY_LEN + TIME_LEN + COIN_LEN)

class CBuckFile
{
public:
	void Close();
	BOOL Open(const char* szFile, BOOL bCreate);
	virtual BOOL SaveData_SD(const void* pBuf, int len);
	virtual BOOL SaveData_Time(const void* pBuf, int len);
	virtual BOOL SaveData_Energy(const void* pBuf, int len);
	virtual BOOL SaveData_Coin(const void* pBuf, int len);
	virtual BOOL CheckFileValid();

	CBuckFile();
	~CBuckFile();

protected:
	char  m_szFile[512];
	BYTE  m_FileHead[128];
};

#endif  //__BUCKFILE_H__