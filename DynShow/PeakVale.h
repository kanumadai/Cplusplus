

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
	int Position_X0[8][8];	//位置边界坐标值
	int Position_X1[8][8];
	int Position_Y0[8][8];
	int Position_Y1[8][8];

	float	fPeakVale_Ratio[8][8][8];	//峰谷比： 【Y】【X】【方向】；左上角为0，顺时针转；
protected:
	
	int PeakValue_X[8][8];		//8×8 晶体条 的峰值坐标
	int PeakValue_Y[8][8];

public:
	void CalPeakValeRatio();

protected:
	BOOL	GetPeakCoordinate(int noy,int nox,int *position_yx); //晶体条号坐标Y，X， 输出峰值坐标 Y，X；
	int		GetMinValue(int x0,int y0,int x1,int y1); //输入坐标值
	
	
};





#endif