

/*--------------------------------
���ܣ��йؼ����ȱȵ�ģ�飺
	  

������
	 
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
	int iSdot[64][64];		//ԭʼɢ������
	int Position_X0[8][8];	//λ�ñ߽�����ֵ
	int Position_X1[8][8];
	int Position_Y0[8][8];
	int Position_Y1[8][8];

	float	fPeakVale_Ratio[8][8][8];	//��ȱȣ� ��Y����X�������򡿣����Ͻ�Ϊ0��˳ʱ��ת��
protected:
	
	int PeakValue_X[8][8];		//8��8 ������ �ķ�ֵ����
	int PeakValue_Y[8][8];

public:
	void CalPeakValeRatio();

protected:
	BOOL	GetPeakCoordinate(int noy,int nox,int *position_yx); //������������Y��X�� �����ֵ���� Y��X��
	int		GetMinValue(int x0,int y0,int x1,int y1); //��������ֵ
	
	
};





#endif