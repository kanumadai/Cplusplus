#ifndef	_G_FUCTION_H
#define _G_FUCTION_H

#include "StdAfx.h"




bool UCRC( unsigned char* buff);

bool ClearCommRecBuff(void);

int WriteCommandMaintainMod(int lenth, BYTE* pdata);//дά������

int ReadResultMaintainMod(BYTE* rddata); //��ά�����

#endif