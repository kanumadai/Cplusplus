#include "stdafx.h"
#include "ControlData.h"

bool control_data::GetCtlAD(unsigned int *data,int AD_Num)
{
	bool bResult=false;
	
	if (CheckCRC(false) && AD_Num>=0 && AD_Num<MAX_AD_NUMBER)
	{
		switch(AD_Num)
		{
		case 0:{*data=CtlData.BITS.ad0;break;}
		case 1:{*data=CtlData.BITS.ad1;break;}
		case 2:{*data=CtlData.BITS.ad2;break;}
		case 3:{*data=CtlData.BITS.ad3;break;}
		default:break;
		}
		
		bResult=true;
	}
	return bResult; 
}

bool control_data::GetCtlIO(unsigned int *data,int IO_Num)
{
	bool bResult=false;
	if (CheckCRC(false) && IO_Num>=0 && IO_Num<MAX_IO_NUMBER)
	{
		switch(IO_Num)
		{
		case 0:{*data=CtlData.BITS.io0;break;}
		case 1:{*data=CtlData.BITS.io1;break;}
		case 2:{*data=CtlData.BITS.io2;break;}
		case 3:{*data=CtlData.BITS.io3;break;}
		case 4:{*data=CtlData.BITS.io4;break;}
		case 5:{*data=CtlData.BITS.io5;break;}
		case 6:{*data=CtlData.BITS.io6;break;}
		case 7:{*data=CtlData.BITS.io7;break;}
		default:break;
		}
		bResult=true;
	}
	return bResult;
}

bool control_data::SetCtlAD(unsigned int *AD_Data,int AD_Num)
{
    bool bResult=false;
	if(AD_Num>=0 && AD_Num<MAX_AD_NUMBER)
	{
		switch(AD_Num)
		{
		case 0: {CtlData.BITS.ad0=*AD_Data;break;}
		case 1: {CtlData.BITS.ad1=*AD_Data;break;}
		case 2: {CtlData.BITS.ad2=*AD_Data;break;}
		case 3: {CtlData.BITS.ad3=*AD_Data;break;}
		default:break;
		}
		bResult=true;
	}
	return bResult;
}

bool control_data::SetCtlIO(unsigned int *IO_Data,int IO_Num)
{

	bool bResult=false;
	if(IO_Num>=0 && IO_Num<MAX_IO_NUMBER)
	{
		switch(IO_Num)
		{
		case 0:{CtlData.BITS.io0=*IO_Data;break;}
		case 1:{CtlData.BITS.io1=*IO_Data;break;}
		case 2:{CtlData.BITS.io2=*IO_Data;break;}
		case 3:{CtlData.BITS.io3=*IO_Data;break;}
		case 4:{CtlData.BITS.io4=*IO_Data;break;}
		case 5:{CtlData.BITS.io5=*IO_Data;break;}
		case 6:{CtlData.BITS.io6=*IO_Data;break;}
		case 7:{CtlData.BITS.io7=*IO_Data;break;}	
		default:break;
		}
		bResult=true;
	}
	return bResult;
}
int control_data::GetPackedCtlData(unsigned char *DataPacked)
{
	aa = 0;
	unsigned char *p = DataPacked;
	int TotalByets = 0;
    CtlData.BITS.CRC = 0;//2007.7.11  
	
	CheckCRC(true);//calculate check CRC
	TotalByets = (MAX_AD_NUMBER/2)+2;
	for(int i = 0;i < MAX_B_NUMBER;i++)
	*(p+i) = CtlData.uBytes[i];
	return TotalByets;

}
bool control_data::SetByBytes(unsigned char *pBytesData)//ะฃั้
{
	unsigned char *p=pBytesData;
	bool bResult=false;
   	for(int i=0;i<MAX_B_NUMBER;i++)
	CtlData.uBytes[i]=*(p+i);
	
	if (CheckCRC(false))
		{
			bResult=true;
		}
	return bResult;
}

bool control_data::CheckCRC(bool Check)
{       
	    bool bResult=false;

		if (Check==true)//cal
		{
			CtlData.BITS.CRC = cal_CRC(&CtlData.uBytes[0], 5);//2007.7.11
			bResult=true;
		}
        else//check
		{			
				CString cs;
				if(cal_CRC(&CtlData.uBytes[0], 5)==CtlData.BITS.CRC)//2007.7.11
				{
					bResult=true; 
				}
		}
		return bResult;
} 
//x8+x7+x3+x2+1
//2007.7.11
unsigned char control_data::cal_CRC(unsigned char *ptr, unsigned char len)
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
			aa1[j++] = *ptr;			
			ptr++;
		}
		return(CRC);
}
