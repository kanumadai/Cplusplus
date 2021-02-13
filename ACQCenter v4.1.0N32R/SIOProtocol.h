////////////////////////////////////////////////////////////////////////////////
//  DEFINE THE COMMUNICATION PORTOCOL BETWEEN ACS AND OWS
//	BASED ON DOCUMENT PUBLISHED 31/05/2002
//
//  2003.10.31 zoujie
//     Add Support to Bucket SIO
////////////////////////////////////////////////////////////////////////////////

#ifndef __SIOPROTOCOL_H__
#define __SIOPROTOCOL_H__

#pragma warning(push)
#pragma warning(disable:4200)

#pragma pack(push)
#pragma pack(1)

///////////////////////////////////////////////////////////////////////////
// EMControl

typedef struct _sio_frame_head
{
	BYTE tAddr;        //target addr
	BYTE sAddr;        //source addr
	union
	{
		struct
		{
			BYTE len8;  	       //frame len, include this head
			BYTE attr;         //attribe
		};
		WORD len16;
	};
}SIO_FRAME_HEAD;

//receive frame form GC, 12byte len
typedef struct _sio_frame12
{
   BYTE tAddr;        //target addr
   BYTE sAddr;        //source addr
   BYTE len;  	      //frame len, include this head
   BYTE attr;         //attribe
   BYTE CmdMain;
   BYTE CmdSub;
   BYTE Dat[4];
   BYTE CheckSum;
   BYTE	eof;      	  //should be 0x7F	or 0x88
}SIO_FRAME12;

//Frame send to GC, 8byte len
typedef struct _sio_frame8
{
   BYTE tAddr;
   BYTE sAddr;
   BYTE len;
   BYTE attr;
   BYTE CmdMain;
   BYTE CmdSub;
   BYTE CheckSum;
   BYTE eof;
}SIO_FRAME8;

//Frame send to GC, 9byte len
typedef struct _sio_frame9
{
   BYTE tAddr;
   BYTE sAddr;
   BYTE len;
   BYTE attr;
   BYTE CmdMain;
   BYTE CmdSub;
   BYTE Dat;
   BYTE CheckSum;
   BYTE eof;
}SIO_FRAME9;

// EMControl
////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////
// Bucket Control

//ACS->BUCKET
typedef struct _buck_frame6{
   BYTE tAddr;
   BYTE sAddr;
   BYTE len8;
   BYTE CmdMain;
   BYTE checksum;
   BYTE eof;
}BUCK_FRAME6;

//ACS->BUCKET
typedef struct _buck_frame7{
   BYTE tAddr;
   BYTE sAddr;
   BYTE len8;
   BYTE CmdMain;
   BYTE CmdSub;
   BYTE checksum;
   BYTE eof;
}BUCK_FRAME7;

//ACS->BUCKET
typedef struct _buck_frame11{
   BYTE tAddr;
   BYTE sAddr;
   BYTE len8;
   BYTE CmdMain;
   BYTE CmdSub;
   union{
	   DWORD dwDat;
	   BYTE  cDat[4];
   };
   BYTE checksum;
   BYTE eof;
}BUCK_FRAME11;

//ACS->BUCKET
typedef struct _buck_frame8{
   BYTE tAddr;
   BYTE sAddr;
   BYTE len8;
   BYTE CmdMain;
   BYTE CmdSub;
   BYTE cDat;
   BYTE checksum;
   BYTE eof;
}BUCK_FRAME8;

//BUCKET->ACS
typedef struct _buck_frame_in{
   BYTE tAddr;
   BYTE sAddr;
   WORD len16;
   BYTE CmdMain;
   BYTE dat[];
}BUCK_FRAME_IN;

// Bucket Control
///////////////////////////////////////////////////////////////////////

//address define
#define ADDR_ACS         0xB0
#define ADDR_GANTRY      0xA0
#define ADDR_Y1          0x11
#define ADDR_Y2          0x12
#define ADDR_Y1Y2        0x10
#define ADDR_ROD         0x03
#define ADDR_BED         0x04
#define ADDR_SEPTA       0x05
#define ADDR_HV          0x06
#define ADDR_RODBEDSEPTA 0x08
#define ADDR_BUCKET_ALL  0x70
#define ADDR_BUCKET0     0x80
#define ADDR_BUCKET31    0x9F
#define ADDR_DEBUG       0xBB


#pragma pack(pop)
#pragma warning(pop)

#endif //__SIOPROTOCOL_H__