

/*--------------------------------
功能：有关计算峰谷比的模块：
	  

操作：
	 
----------------------------------*/

#ifndef	_PEAK_VALE_H
#define _PEAK_VALE_H

#include "StdAfx.h"

class CPEAKVALE
{
public:
	CPEAKVALE();
	~CPEAKVALE();

public:
	int iSdot[64][64];		//原始散点数据
	int	iPeakVale_Ratio[8][8][8];	//峰谷比： 【Y】【X】【方向】；左上角为0，顺时针转；


protected:

	
	
};





#endif