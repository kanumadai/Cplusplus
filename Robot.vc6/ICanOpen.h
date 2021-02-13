#ifndef __ICAN_OPEN_26245365_686897_2358_34567
#define __ICAN_OPEN_26245365_686897_2358_34567
#include <objbase.h>
#include "IDriver.h"
#include <afxtempl.h>

enum NMT_COMMAND_SPECIFIER
{
	START_REMOTE_NODE = 0x01,
	STOP_REMOTE_NODE = 0x02,
	ENTER_PRE_OPERATIONAL = 0x80,
	RESET_NODE = 0x81,
	RESET_COMMUNICATION = 0x82
};
struct PDO_MAPPING_OBJ
{
	WORD object;
	BYTE index;
	BYTE size;
};
interface ICanOpen
{
	virtual int PDO_Tx_Async(BYTE PdoNum, const BYTE *pData, int Length, DWORD TimeOut=500, BYTE NodeID=0x80)=0;
	virtual int PDO_Rx_Async(BYTE PdoNum, BYTE *pData, int *Length, DWORD TimeOut=500)=0;
	virtual int PDO_Tx_Rx_Sync(BYTE PdoNum, const BYTE *pDataTx, int LengthTx, BYTE *pDataRx, int *LengthRx, DWORD TimeOut=500)=0;
	virtual int NMT_Service(NMT_COMMAND_SPECIFIER cs, BYTE NodeID)=0;
	virtual int SYNC_Tx()=0;
	virtual int TimeStamp_Tx(DWORD pdwTimeStamp)=0;
	virtual void SetEmergencyCallback(EMERGENCY_CALLBACK pfnCANEmergencyCallback)=0;
	virtual int	SDO_Upload(WORD Index, BYTE SubIndex, CArray<BYTE,BYTE> &Data, DWORD TimeOut=500)=0;
	virtual int SDO_DownloadExpSeg(WORD Index, BYTE SubIndex, const BYTE * pData, int bufLen, DWORD TimeOut=500)=0;
	// + Node Guarding....
};

interface ICanOpenMaster
{
	static ICanOpenMaster *CreateMaster(const SCommunicationInfo &SComInfo,DRIVER_CALLBACK pfnErrorProc=NULL);
	virtual void DestroyMaster();
	virtual int PDO_Tx_Async(BYTE NodeID, BYTE PdoNum, const BYTE *pData, int Length, DWORD TimeOut=500)=0;
	virtual int PDO_Rx_Async(BYTE NodeID, BYTE PdoNum, BYTE *pData, int *Length, DWORD TimeOut=500)=0;
	virtual int PDO_Tx_Rx_Sync(BYTE NodeID, BYTE PdoNum, const BYTE *pDataTx, int LengthTx, BYTE *pDataRx, int *LengthRx, DWORD TimeOut=500)=0;
	virtual int NMT_Service(BYTE NodeID, NMT_COMMAND_SPECIFIER cs)=0;
	virtual int SYNC_Tx()=0;
	virtual int TimeStamp_Tx(DWORD pdwTimeStamp)=0;
	virtual void SetEmergencyCallback(BYTE NodeID, EMERGENCY_CALLBACK pfnCANEmergencyCallback)=0;
	virtual int	SDO_Upload(BYTE NodeID, WORD Index, BYTE SubIndex, CArray<BYTE,BYTE> &Data, DWORD TimeOut=500)=0;
	virtual int SDO_DownloadExpSeg(BYTE NodeID, WORD Index, BYTE SubIndex, const BYTE * pData, int bufLen, DWORD TimeOut=500)=0;
	virtual int Mapping_PDO_Tx(BYTE NodeID,BYTE PDO_num, const PDO_MAPPING_OBJ *pMappingObj,BYTE ObjCount,BYTE TxType,DWORD EventType)=0;
	virtual int Mapping_PDO_Rx(BYTE NodeID,BYTE PDO_num, const PDO_MAPPING_OBJ *pMappingObj,BYTE ObjCount)=0;
	virtual int OS_Command(BYTE NodeID, const char *pOutData, int OutLen, char *pInpData, int InpLen)=0;
	//virtual int NodeGuarding()=0;??????
	//virtual int HeartBit()=0;????????
};

#endif //__ICAN_OPEN_26245365_686897_2358_34567
