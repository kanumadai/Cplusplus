#include "stdafx.h"
#include "SensorData.h"

bool SensorData::GetSenData(unsigned int *data,int AD_Num)
{
	bool bResult=false;
	
	if (AD_Num>=0 && AD_Num<MAX_AD_NUMBER1)
	{
		switch(AD_Num)
		{	
		case 0:{*data=CtlData1.SENSORBITS.FL;break;}
		case 1:{*data=CtlData1.SENSORBITS.FR;break;}
		case 2:{*data=CtlData1.SENSORBITS.RL;break;}
		case 3:{*data=CtlData1.SENSORBITS.RR;break;}
		case 4:{*data=CtlData1.SENSORBITS.SN1;break;}
		case 5:{*data=CtlData1.SENSORBITS.SN2;break;}
		case 6:{*data=CtlData1.SENSORBITS.SN3;break;}
		case 7:{*data=CtlData1.SENSORBITS.SN4;break;}
		case 8:{*data=CtlData1.SENSORBITS.SN5;break;}
		case 9:{*data=CtlData1.SENSORBITS.SN6;break;}
		case 10:{*data=CtlData1.SENSORBITS.SN7;break;}
		case 11:{*data=CtlData1.SENSORBITS.SN8;break;}
		case 12:{*data=CtlData1.SENSORBITS.HOLD;break;}
		case 13:{*data=CtlData1.SENSORBITS.RLIM1;break;}
		case 14:{*data=CtlData1.SENSORBITS.RLIM2;break;}
		case 15:{*data=CtlData1.SENSORBITS.RLIM3;break;}
		case 16:{*data=CtlData1.SENSORBITS.RLIM4;break;}
		case 17:{*data=CtlData1.SENSORBITS.IR1;break;}
		case 18:{*data=CtlData1.SENSORBITS.IR2;break;}
		case 19:{*data=CtlData1.SENSORBITS.IR3;break;}
		case 20:{*data=CtlData1.SENSORBITS.IR4;break;}
		case 21:{*data=CtlData1.SENSORBITS.IR5;break;}
		case 22:{*data=CtlData1.SENSORBITS.IR6;break;}
		case 23:{*data=CtlData1.SENSORBITS.IR7;break;}
		case 24:{*data=CtlData1.SENSORBITS.IR8;break;}
		case 25:{*data=CtlData1.SENSORBITS.IR9;break;}
		case 26:{*data=CtlData1.SENSORBITS.IR10;break;}
	//	case 27:{*data=CtlData1.SENSORBITS.IR11;break;}
	//	case 28:{*data=CtlData1.SENSORBITS.IR12;break;}
		default:break;
		}
		
		bResult=true;
	}
	return bResult; 
}


bool SensorData::SetSenData(unsigned int *AD_Data,int AD_Num)
{
    bool bResult=false;
	if(AD_Num>=0 && AD_Num<MAX_AD_NUMBER1)
	{
		switch(AD_Num)
		{			
		case 0: {CtlData1.SENSORBITS.FL=*AD_Data;break;}
		case 1: {CtlData1.SENSORBITS.FR=*AD_Data;break;}
		case 2: {CtlData1.SENSORBITS.RL=*AD_Data;break;}
		case 3: {CtlData1.SENSORBITS.RR=*AD_Data;break;}
		case 4: {CtlData1.SENSORBITS.SN1=*AD_Data;break;}
		case 5: {CtlData1.SENSORBITS.SN2=*AD_Data;break;}
		case 6: {CtlData1.SENSORBITS.SN3=*AD_Data;break;}
		case 7: {CtlData1.SENSORBITS.SN4=*AD_Data;break;}
		case 8: {CtlData1.SENSORBITS.SN5=*AD_Data;break;}
		case 9: {CtlData1.SENSORBITS.SN6=*AD_Data;break;}
		case 10:{CtlData1.SENSORBITS.SN7=*AD_Data;break;}
		case 11:{CtlData1.SENSORBITS.SN8=*AD_Data;break;}
		case 12: {CtlData1.SENSORBITS.HOLD=*AD_Data;break;}
		case 13: {CtlData1.SENSORBITS.RLIM1=*AD_Data;break;}
		case 14: {CtlData1.SENSORBITS.RLIM2=*AD_Data;break;}
		case 15: {CtlData1.SENSORBITS.RLIM3=*AD_Data;break;}
		case 16: {CtlData1.SENSORBITS.RLIM4=*AD_Data;break;}
		case 17: {CtlData1.SENSORBITS.IR1=*AD_Data;break;}
		case 18: {CtlData1.SENSORBITS.IR2=*AD_Data;break;}
		case 19: {CtlData1.SENSORBITS.IR3=*AD_Data;break;}	
		case 20: {CtlData1.SENSORBITS.IR4=*AD_Data;break;}
		case 21: {CtlData1.SENSORBITS.IR5=*AD_Data;break;}
		case 22: {CtlData1.SENSORBITS.IR6=*AD_Data;break;}	
		case 23: {CtlData1.SENSORBITS.IR7=*AD_Data;break;}
		case 24: {CtlData1.SENSORBITS.IR8=*AD_Data;break;}		
		case 25: {CtlData1.SENSORBITS.IR9=*AD_Data;break;}
		case 26: {CtlData1.SENSORBITS.IR10=*AD_Data;break;}	
	//	case 27: {CtlData1.SENSORBITS.IR11=*AD_Data;break;}
	//	case 28: {CtlData1.SENSORBITS.IR12=*AD_Data;break;}				
		default:break;
		}
		bResult=true;
	}
	return bResult;
}

int SensorData::GetPackedCtlData(unsigned char *DataPacked)
{
	unsigned char *p=DataPacked;
	int TotalByets=0;
	
	 CtlData1.SENSORBITS.CRC=0;
	CheckCRC(true);//calculate check CRC
//	TotalByets=(MAX_AD_NUMBER1/2)+2;
	for(int i=0;i<20;i++)
	*(p+i)=CtlData1.uBytesSensor[i];
	return TotalByets;
}
bool SensorData::SetByBytes(unsigned char *pBytesData)
{
	unsigned char *p=pBytesData;
	bool bResult=false;
   	for(int i=0;i<20;i++)
	CtlData1.uBytesSensor[i]=*(p+i);
	if (CheckCRC(false))
		{
			bResult=true;
		}	
	return bResult;
}

bool SensorData::CheckCRC(bool Check)
{       	 
	    bool bResult=false;

		if (Check==true)//cal
		{
			CtlData1.SENSORBITS.CRC = cal_CRC(&CtlData1.uBytesSensor[0], 19);//2007.7.11
			bResult=true;
		}
        else//check
		{			
				CString cs;
				if(cal_CRC(&CtlData1.uBytesSensor[0], 20)==0)//2007.7.11
				{
					bResult=true; 
				}
		}
		return bResult;
}
unsigned char SensorData::cal_CRC(unsigned char *ptr, unsigned char len)
{
      unsigned char i;
		unsigned char j = 0;
		unsigned char CRC=0;
	
		while (len--!=0)
		{
			for (i=0x80;i!=0;i/=2)
			{
				if ((CRC&0x80)!=0)//100/0000
				{
					CRC*=2;
					CRC^=0x8D;//x8+x7+x3+x2+1
				}
				else
					CRC*=2;
				if ((*ptr&i)!=0)
					CRC^=0x8D;
			}
			ptr++;
		}
		return(CRC);
}
