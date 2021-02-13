#define MAX_AD_NUMBER1 28
#define u12   unsigned short
typedef union
{ unsigned char uBytesSensor[20];
  struct 
  {	
	    unsigned int FL:16;
		unsigned int FR:16;
		unsigned int RL:16;
		unsigned int RR:16;
	    unsigned int SN1:8;
		unsigned int SN2:8;
		unsigned int SN3:8;
		unsigned int SN4:8;
	    unsigned int SN5:8;
		unsigned int SN6:8;
		unsigned int SN7:8;
		unsigned int SN8:8;
		unsigned int HOLD:8;
		unsigned int RLIM1:1;  //FL
		unsigned int RLIM2:1;  //FR
		unsigned int RLIM3:1;  //RL
		unsigned int RLIM4:1;  //RR
		
	  	unsigned int IR1:1;  //IR1
		unsigned int IR2:1;  //IR2
		unsigned int IR3:1;  //IR3
		unsigned int IR4:1;  //IR4
		unsigned int IR5:1;  //IR5
		unsigned int IR6:1;  //IR6
		unsigned int IR7:1;  //IR7
		unsigned int IR8:1;  //IR8
		unsigned int IR9:1;  //IR9
		unsigned int IR10:1;  //IR10
		unsigned int IR11:1;  //IR11
		unsigned int IR12:1;  //IR12
	    unsigned int CRC:8;  //ºó
   }SENSORBITS;
} CTLSENSORDATA; 

class SensorData 
{
private:
	unsigned char cal_CRC(unsigned char *ptr, unsigned char len);
	bool	CheckCRC(bool Check);//check(false) before get,check(true) after set		
public:	
    bool	SetSenData(unsigned int *AD_Data,int AD_Num);//to send
	int GetPackedCtlData(unsigned char *DataPacked);//to send     
	bool SetByBytes(unsigned char *pBytes);//to receive
	bool	GetSenData(unsigned int *AD_Data,int AD_Num);//to receive   
   	CTLSENSORDATA CtlData1;	
};