#define MAX_AD_NUMBER 4
#define MAX_IO_NUMBER 8
#define MAX_B_NUMBER 6

typedef union
{ unsigned char uBytes[6];
  unsigned int iData;
  struct 
  {	
		unsigned int ad0:8;            //�ֱ�1yǰ��
		unsigned int ad1:8;            //�ֱ�1xת��
		unsigned int ad2:8;            //�ֱ�2y�����½�

		unsigned int ad3:8;            //�ֱ�2x


		unsigned int io0:1;	           //������DI2[6]	
		unsigned int io1:1;            //������DI1[6]
		unsigned int io2:1;            //��̨������DI2[0]
		unsigned int io3:1;            //��̨������DI2[2]
		unsigned int io4:1;  //5���ֽ�   //��̨��ת��DI1[0]
		unsigned int io5:1;            //��̨��ת��DI1[2]
		unsigned int io6:1;            //����DI1[1]		
		unsigned int io7:1;            //�ٶ�ģʽDI1[7]


		unsigned int CRC:8;//6���ֽ�
		
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