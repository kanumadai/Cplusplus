#define MAX_AD_NUMBER 4
#define MAX_IO_NUMBER 8
#define MAX_B_NUMBER 6

typedef union
{ unsigned char uBytes[6];
  unsigned int iData;
  struct 
  {	
		unsigned int ad0:8;            //手柄1y前进
		unsigned int ad1:8;            //手柄1x转弯
		unsigned int ad2:8;            //手柄2y上升下降

		unsigned int ad3:8;            //手柄2x


		unsigned int io0:1;	           //调焦上DI2[6]	
		unsigned int io1:1;            //调焦下DI1[6]
		unsigned int io2:1;            //云台俯仰上DI2[0]
		unsigned int io3:1;            //云台俯仰下DI2[2]
		unsigned int io4:1;  //5个字节   //云台回转上DI1[0]
		unsigned int io5:1;            //云台回转下DI1[2]
		unsigned int io6:1;            //车灯DI1[1]		
		unsigned int io7:1;            //速度模式DI1[7]


		unsigned int CRC:8;//6个字节
		
   }BITS;
} CTLDATA; 

class control_data 
{
private:   	

	bool   CheckCRC(bool Check);//check(false) before get,check(true) after set
		
public:

	unsigned int aa;
	unsigned int aa1[5];

	CTLDATA CtlData;
	unsigned char cal_CRC(unsigned char *ptr, unsigned char len);
	
    bool	SetCtlAD(unsigned int *AD_Data,int AD_Num);//to send
    bool	SetCtlIO(unsigned int *IO_Data,int IO_Num);//to send
	int     GetPackedCtlData(unsigned char *DataPacked);//to send 
    
	bool    SetByBytes(unsigned char *pBytes);//to receive
	bool	GetCtlAD(unsigned int *AD_Data,int AD_Num);//to receive
    bool	GetCtlIO(unsigned int *IO_Data,int IO_Num);//to receive    
};