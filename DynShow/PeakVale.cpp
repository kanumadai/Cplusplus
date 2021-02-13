#include "StdAfx.h"
#include "PeakVale.h"




CPEAKVALE::CPEAKVALE()
{
	memset(iSdot,0,64*64*4);
	memset(fPeakVale_Ratio,0,8*8*4);



}
CPEAKVALE::~CPEAKVALE()
{
}

//input: noy,nox: 晶体条的坐标号；
//output: outy,outx: 最大值象素的 坐标；
BOOL CPEAKVALE::GetPeakCoordinate(int noy,int nox,int *position_yx)
{
	if((noy >= 8)||(nox >= 8)) {
		return false;
	}
	int max = 0;
	for(int i=Position_X0[noy][nox];i<=Position_X1[noy][nox];i++) {
		for(int j=Position_Y0[noy][nox];j<=Position_Y1[noy][nox];j++) {
			if(iSdot[j][i] >= max) {
				max =  iSdot[j][i];
				position_yx[0] = j;
				position_yx[1] = i;
			}
		}
	}

	return true;

}

//intput: x0,y0;x1,y1;  第一点， 第二点坐标
//output: 返回矩形框内的最小值
int CPEAKVALE::GetMinValue(int x0,int y0,int x1,int y1)
{
	int minx,maxx;
	int miny,maxy;
	if(x0 < x1) {	minx = x0;	maxx = x1;	}
	else {	minx = x1;	maxx = x0;	}
	if(y0 < y1) { 	miny = y0;	maxy = y1;	}
	else {	miny = y1;	maxy = y0;	}
	
	int min;
/*
	int min = iSdot[miny][minx];
	for(int i=miny;i<=maxy;i++) {
		for(int j=minx;j<=maxx;j++) {
			if(iSdot[i][j] < min) {	min =  iSdot[j][i]; }
		}
	}
*/
	if(x0 == x1) { 
		min = iSdot[y0][x0];
		for(int i=miny;i<=maxy;i++) {
			if(iSdot[i][x0] < min) { min = iSdot[i][x0];}
		}
		return min;
	}
	if(y0 == y1) {
		min = iSdot[y0][x0];
		for(int i=minx;i<=maxx;i++) {
			if(iSdot[y0][i] < min) { min = iSdot[y0][i];}
		}
		return min;
	}

	float k = (float)(y1-y0)/(x1-x0);
	float b = (float)y1 - k*(float)x1;

	int lenthx,lenthy;
	if(x1>x0) { lenthx = x1-x0; }
	else lenthx = x0-x1;
	if(y1>y0) { lenthy = y1 - y0;}
	else lenthy = y0 - y1;


	int intkk;
	min = iSdot[y0][x0];
	if(lenthy > lenthx) {		
		for(int i=miny;i<miny+lenthy;i++) {
			intkk = (float)((float)i - b)/k;
			if(iSdot[i][intkk] < min)  min = iSdot[i][intkk];
		}
	}
	else {
		for(int i=minx;i<minx+lenthx;i++) {
			intkk = (float)i*k + b;
			if(iSdot[intkk][i] < min)  min = iSdot[intkk][i];
		}
	}

	return min;
}

void CPEAKVALE::CalPeakValeRatio()
{
	memset(PeakValue_Y,0,8*8*4);
	memset(PeakValue_X,0,8*8*4);

	int position_yx[2];

	for(int i=0;i<8;i++) {
		for(int j=0;j<8;j++) {
			GetPeakCoordinate(i,j,position_yx);
			PeakValue_Y[i][j] = position_yx[0];
			PeakValue_X[i][j] = position_yx[1];
		}
	}

	int minvalue;

	for(i=0;i<8;i++) {  //Y
		for(int j=0;j<8;j++) { //x
			//左上；
			if(((i-1)<0) ||((j-1)<0)) { fPeakVale_Ratio[i][j][0] = -1;	}
			else {
				minvalue = GetMinValue(PeakValue_X[i-1][j-1],PeakValue_Y[i-1][j-1],PeakValue_X[i][j],PeakValue_Y[i][j]);
				if(minvalue<=0) minvalue =1;
				fPeakVale_Ratio[i][j][0] = (float)iSdot[ PeakValue_Y[i][j] ][ PeakValue_X[i][j]] / minvalue;
			}

			//上；
			if((i-1)<0) { fPeakVale_Ratio[i][j][1] = -1;	}
			else {
				minvalue = GetMinValue(PeakValue_X[i-1][j],PeakValue_Y[i-1][j],PeakValue_X[i][j],PeakValue_Y[i][j]);
				if(minvalue<=0) minvalue =1;
				fPeakVale_Ratio[i][j][1] = (float)iSdot[ PeakValue_Y[i][j] ][ PeakValue_X[i][j]] / minvalue;
			}
			//右上；
			if(((i-1)<0)||((j+1)>7)) {  fPeakVale_Ratio[i][j][2] = -1;	}
			else {
				minvalue = GetMinValue(PeakValue_X[i-1][j+1],PeakValue_Y[i-1][j+1],PeakValue_X[i][j],PeakValue_Y[i][j]);
				if(minvalue<=0) minvalue =1;
				fPeakVale_Ratio[i][j][2] = (float)iSdot[ PeakValue_Y[i][j] ][ PeakValue_X[i][j]] / minvalue;
			}

			//右；
			if((j+1)>7) {  fPeakVale_Ratio[i][j][3] = -1;	}
			else {
				minvalue = GetMinValue(PeakValue_X[i][j+1],PeakValue_Y[i][j+1],PeakValue_X[i][j],PeakValue_Y[i][j]);
				if(minvalue<=0) minvalue =1;
				fPeakVale_Ratio[i][j][3] = (float)iSdot[ PeakValue_Y[i][j] ][ PeakValue_X[i][j]] / minvalue;
			}

			//右下；
			if(((i+1)>7)||((j+1)>7)) {  fPeakVale_Ratio[i][j][4] = -1;	}
			else {
				minvalue = GetMinValue(PeakValue_X[i+1][j+1],PeakValue_Y[i+1][j+1],PeakValue_X[i][j],PeakValue_Y[i][j]);
				if(minvalue<=0) minvalue =1;
				fPeakVale_Ratio[i][j][4] = (float)iSdot[ PeakValue_Y[i][j] ][ PeakValue_X[i][j]] / minvalue;
			}

			//下；
			if((i+1)>7) {  fPeakVale_Ratio[i][j][5] = -1;	}
			else {
				minvalue = GetMinValue(PeakValue_X[i+1][j],PeakValue_Y[i+1][j],PeakValue_X[i][j],PeakValue_Y[i][j]);
				if(minvalue<=0) minvalue =1;
				fPeakVale_Ratio[i][j][5] = (float)iSdot[ PeakValue_Y[i][j] ][ PeakValue_X[i][j]] / minvalue;
			}

			//左下；
			if(((i+1)>7)||((j-1)<0)) {  fPeakVale_Ratio[i][j][6] = -1;	}
			else {
				minvalue = GetMinValue(PeakValue_X[i+1][j-1],PeakValue_Y[i+1][j-1],PeakValue_X[i][j],PeakValue_Y[i][j]);
				if(minvalue<=0) minvalue =1;
				fPeakVale_Ratio[i][j][6] = (float)iSdot[ PeakValue_Y[i][j] ][ PeakValue_X[i][j]] / minvalue;
			}

			//左；
			if((j-1)<0) {  fPeakVale_Ratio[i][j][7] = -1;	}
			else {
				minvalue = GetMinValue(PeakValue_X[i][j-1],PeakValue_Y[i][j-1],PeakValue_X[i][j],PeakValue_Y[i][j]);
				if(minvalue<=0) minvalue =1;
				fPeakVale_Ratio[i][j][7] = (float)iSdot[ PeakValue_Y[i][j] ][ PeakValue_X[i][j]] / minvalue;
			}
		
		} //end x
	} //end Y



}