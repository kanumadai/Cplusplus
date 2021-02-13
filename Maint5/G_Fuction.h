#ifndef	_G_FUCTION_H
#define _G_FUCTION_H

#include "StdAfx.h"




bool UCRC( unsigned char* buff);

bool ClearCommRecBuff(void);

int WriteCommandMaintainMod(int lenth, BYTE* pdata);//写维护命令

int ReadResultMaintainMod(BYTE* rddata); //读维护结果

#endif