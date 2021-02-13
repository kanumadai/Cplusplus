#include "StdAfx.h"
#include "Mcbio.h"
#include "CommApi.h"

#include "MCBView.h"
#include "Resource.h"
//#include "Fast57600.h"
#include "debug.h"

extern HANDLE	g_hComm;  
extern HANDLE	g_hEvtFinishRec;   //完成接收句柄《数据收发同步用》 
////////////////////////////////////////////////////////////////////
//              visiable in global 


CWinThread* g_pThread_Monitor;   //监控线程指针
CWinThread* g_pThread_Send;		 //发送线程指针
bool g_bRunMonitorThread;	//是否退出监控线程标志	
bool g_bRunSendThread;		//是否退出发送线程标志

UINT		g_ThreadCMD;	//当前线程的执行命令字！！！！
BOOL		g_bExec;				//通信正在执行
BOOL		g_bComOpen;				//串口打开否？
HANDLE		g_hEvtSend;			//控制发送线程的句柄
HANDLE		g_hEvtFinishRec;	//完成接收句柄《数据收发同步用》  
HANDLE		g_hExitSendThread;		//退出发送线程句柄
HANDLE		g_hExitMonitThread;		//退出监控线程句柄
 

int g_RecPackageLenth = 1;  

MCB_OPER	g_MCB_Oper;			// 多道控制的 相关操作变量
TYPE	type;
HARDWARE_PARE g_HardWare_Pare;

FILEHEADER FileHeader;

MYDATE date;


////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////
//              visiable in this file only



//
////////////////////////////////////////////////////////////////////

//-------------------------------------------------------------------//
UINT MonitorThread(LPVOID  lpData)
{
//	m_mcbview = &((CMCBView*)AfxGetMainWnd())->m_sio;
	CMCBView *ph=(CMCBView *)lpData;
	unsigned char *data;
	data = new unsigned char [5000];

	int size;

	ph->m_precbuf = ph->m_recbuff; 	
	ph->m_iworkcounter = 0;

	g_bRunMonitorThread = true;
	while (g_bRunMonitorThread)
	{
		size = ReadResultMaint232(g_hComm,data);
		if(size == 0xffff) 
		{
				AfxMessageBox("Read Error!");
				g_bRunMonitorThread = false;
		}
		if(size > 0) 
		{			
			ph->SendMessage(WM_RECSHOW,(WPARAM)data,size);
		}
		//else Sleep(5);

		if ( WAIT_OBJECT_0 == WaitForSingleObject(g_hExitMonitThread,0))
		{
			break;	
		};

	}//else size; 
	delete [] data;
	return 0;
}


UINT  SendThread(LPVOID  lpData)
{
	CMCBView *pMCbview = ((CMCBView*)AfxGetMainWnd());
	CMCBIO* m_pmcbio;
	m_pmcbio = &((CMCBView*)AfxGetMainWnd())->m_mcbio;

	CMCBView *ph=(CMCBView *)lpData;  
	g_bRunSendThread = true;

	while(g_bRunSendThread) 
	{ 
		if(!g_MCB_Oper.bQueryEn) 
		{ //不进行轮寻操作
			if(g_MCB_Oper.bStopCMDEn) 
			{  //停止命令
				SetEvent(g_hEvtSend);
				g_MCB_Oper.bStopCMDEn = false;				
				//g_ThreadCMD = IDC_CTL_STOP_SAMPLE;
				g_RecPackageLenth = ph->m_iFullChannelWidth * 3;
				g_ThreadCMD = IDC_RECEIVE_EXTERNAL_RAM;
			}
			WaitForSingleObject(g_hEvtSend, -1);
		}
		else
		{ // 系统开始进行轮寻MCB的数据，状态等
			if(g_MCB_Oper.iCurQueryNo >= MCBMAX_QUERYNUM) {
				g_MCB_Oper.iCurQueryNo = g_MCB_Oper.iStartQueryNo;				
			}
			
			if(ph->m_bExpRamLongNew_valid) { //如果刚更新了ExpRam的边界，则重新开始数据的查询
			//	ph->m_bExpRamLongNew_valid = FALSE;
				g_MCB_Oper.iCurQueryNo = 0;
			}

			g_ThreadCMD = g_MCB_Oper.QueryTable[g_MCB_Oper.iCurQueryNo];
			g_MCB_Oper.iCurQueryNo ++;				
			//Sleep(100); //等待等待
		}

		if ( WAIT_OBJECT_0 == WaitForSingleObject(g_hExitSendThread,0)) 
		{
			return 0;	
		};

		g_bExec = TRUE;
		switch( g_ThreadCMD )
		{
		case IDC_CTL_START_SAMPLE:
			m_pmcbio->Ctl_StartSample(ph);
			break;
		case IDC_CTL_STOP_SAMPLE:
			m_pmcbio->Ctl_StopSample(ph);
			break;
		case IDC_COMTEST:
			m_pmcbio->Ctl_ComTest(ph);
			break;
		case CTL_LOAD_FULL_RAM:
			m_pmcbio->Ctl_LoadFullRam(ph);
			break;
		case CTL_LOAD_EXP_RAM:
			m_pmcbio->Ctl_LoadExpRam(ph);
			break;
		case CTL_LOAD_TIME_VALUE:
			m_pmcbio->Ctl_LoadTimeValue(ph);
			break;
		case CTL_LOAD_INTERST_CHANNEL_COUNTER:
			m_pmcbio->Ctl_LoadInterstChannCounter(ph);
			break;
		case IDC_RESET_SYSTEM: 
			m_pmcbio->Ctl_ResetSystem(ph);
			break;
		case IDC_CONTINUE_SAMPLE:
			m_pmcbio->Ctl_ContinueSample(ph);
			break;
		case IDC_SEND_EXTERNAL_RAM:
			m_pmcbio->Ctl_SendExternalRam(ph);
			break;
		case IDC_QUERY_TIMEOUT:
			m_pmcbio->Ctl_QueryTimeOut(ph);
			break;
		case IDC_QUERY_SAMPLE_STATE:
			m_pmcbio->Ctl_QuerySampleState(ph);
			break;
		case IDC_RECEIVE_EXTERNAL_RAM:
			m_pmcbio->Ctl_ReceiveExternalRam(ph);
			break;
		case CTL_PROG_CONFIG_ANOLPARA:
			m_pmcbio->Ctl_Prog_ConfigAnolPara(ph);
			break;
		case CTL_HARDWARE_READ:
			m_pmcbio->Ctl_HardWare_Read(ph);
			break;
		case CTL_HARDWARE_WRITE:
			m_pmcbio->Ctl_HardWare_Write(ph);
		default:
			break;
		}
		g_ThreadCMD = NULL;
	};

	return 0;
}

CMCBIO::CMCBIO()
{
	g_MCB_Oper.bQueryEn = false;
	g_MCB_Oper.bStopCMDEn = false;

	g_MCB_Oper.iCurQueryNo = 0;
	g_MCB_Oper.iStartQueryNo = MCB_START_QUERYNO;
//	g_MCB_Oper.QueryTable[0] = CTL_LOAD_EXP_RAM;
//	g_MCB_Oper.QueryTable[1] = CTL_LOAD_FULL_RAM;
//	g_MCB_Oper.QueryTable[2] = CTL_LOAD_INTERST_CHANNEL_COUNTER;

	g_MCB_Oper.QueryTable[0] = CTL_LOAD_INTERST_CHANNEL_COUNTER;
	g_MCB_Oper.QueryTable[1] = CTL_LOAD_EXP_RAM;
	g_MCB_Oper.QueryTable[2] = CTL_LOAD_FULL_RAM;


	g_MCB_Oper.QueryTable[3] = CTL_LOAD_TIME_VALUE;
	g_MCB_Oper.QueryTable[4] = IDC_QUERY_SAMPLE_STATE;
	g_MCB_Oper.QueryTable[5] = IDC_QUERY_TIMEOUT;

}

CMCBIO::~CMCBIO()
{

}

//
BOOL CMCBIO::Ctl_StartSample(LPVOID  lpData)
{
	CMCBView *ph=(CMCBView *)lpData;

	memset(ph->m_recbuff,0,1024*4*8);
	memset(ph->m_secrecbuf,0,1024*4*8);
	ph->m_precbuf = ph->m_recbuff;
	ph->m_iworkcounter = 0;

	m_SendFrame[0] = 2;						//首先发【停止命令】
	m_SendFrame[1] = 9;						//【reset命令】

	ph->PostMessage(WM_SENDSHOW,(WPARAM)m_SendFrame,2);
	WriteCommandMaint232(g_hComm,2,m_SendFrame);
	Sleep(200);
	
									//【设置时间模式】
	if (g_HardWare_Pare.bSampleModeRealTime) 
	{ 
		m_SendFrame[0] = 15;		
		m_SendFrame[1] = 0x80;		// 实时  (d7 位有效 = '1')
	}
	else
	{		
		m_SendFrame[0] = 15;		
		m_SendFrame[1] = 0x00;		// 活时  (d7 位有效 = '0')
	}
	
/*									//【ADC 极性设置】
	unsigned char chbuff = 0;
	if (g_HardWare_Pare.bDT_Pulse) { chbuff |= 0x40; }; //死时间
	if (g_HardWare_Pare.bREADY_Pulse) { chbuff |= 0x20;	};//Ready
	if (g_HardWare_Pare.bANSER_Pulse) { chbuff |= 0x10;	};//Anwser
	if (g_HardWare_Pare.bENADC_Pulse) { chbuff |= 0x08; };//EnAdc
	m_SendFrame[4] = 16;	
	m_SendFrame[5] = chbuff;	
*/
									//【设置时间长度】
	type.intdata = g_HardWare_Pare.iSampleTimeValue * 100;
	m_SendFrame[2] = 8;						
	m_SendFrame[3] = type.uchdata[0];
	m_SendFrame[4] = type.uchdata[1];
	m_SendFrame[5] = type.uchdata[2];
	m_SendFrame[6] = type.uchdata[3];
									// 【设置道宽】
	m_SendFrame[7] = 5;
	type.intdata = g_HardWare_Pare.iChannelWidth; 
	m_SendFrame[8] = type.uchdata[0];		//道宽
	m_SendFrame[9] = type.uchdata[1];

	m_SendFrame[10] = 17;
	type.intdata = ph->m_iSmallWinCenPoint;	//【设置感兴趣道】
	m_SendFrame[11] = type.uchdata[0];		
	m_SendFrame[12] = type.uchdata[1];

	m_SendFrame[13] = 18;			//【设置道边界】
	type.intdata = ph->m_iSmallWinCHL;		//道左边界
	m_SendFrame[14] = type.uchdata[0];		
	m_SendFrame[15] = type.uchdata[1];
	type.intdata = ph->m_iSmallWinCHR;		//道右边界
	m_SendFrame[16] = type.uchdata[0];		
	m_SendFrame[17] = type.uchdata[1];

	m_SendFrame[18] = 3;				//【清空Ram】
	m_SendFrame[19] = 1;   //『开始命令』 显示用的，实际还没有发送“开始命令”，延时后再发

	ph->PostMessage(WM_SENDSHOW,(WPARAM)m_SendFrame,20);
	WriteCommandMaint232(g_hComm,19,m_SendFrame);
	Sleep(100);

	ph->m_ExpRamDataLong_old = (ph->m_iSmallWinCHR - ph->m_iSmallWinCHL +1)*3;
	m_SendFrame[0] = 1;  // 【开始 命令】
	g_RecPackageLenth = 0;  
	WriteCommandMaint232(g_hComm,1,m_SendFrame);

	int dwRet = WaitForSingleObject(g_hEvtFinishRec, SENDTIMEOUT); //发送超时
	switch(dwRet)
	{
	case WAIT_OBJECT_0:
			 break;				 
	default:		
		g_ThreadCMD = NULL;		//发送命令默认为空
			break;
	}

	ResetEvent(g_hEvtSend);
	g_bExec = FALSE;

	 return TRUE;
}

//
BOOL CMCBIO::Ctl_StopSample(LPVOID  lpData)
{
	CMCBView *ph=(CMCBView *)lpData;

	memset(ph->m_recbuff,0,1024*4*8);
	memset(ph->m_secrecbuf,0,1024*4*8);
	ph->m_precbuf = ph->m_recbuff;
	ph->m_iworkcounter = 0;

	g_RecPackageLenth = ph->m_iFullChannelWidth * 3;

	m_SendFrame[0] = 2;
	m_SendFrame[1] = 7;

	ph->PostMessage(WM_SENDSHOW,(WPARAM)m_SendFrame,2);

	int nWR = WriteCommandMaint232(g_hComm,2,m_SendFrame);
	if(nWR == false) {
		AfxMessageBox("Communication Write Error.\n\n Please check Communication Setting.");
	}


//	int dwRet = WaitForSingleObject(g_hEvtFinishRec, SENDTIMEOUT*5); //发送超时
//	switch(dwRet){
//	case WAIT_OBJECT_0:
//		break;				 
//	default:		
//		g_ThreadCMD = NULL;		//发送命令默认为空
//		break;
//	}


	ResetEvent(g_hEvtSend);
	g_bExec = FALSE;
	
	 return TRUE;
}

void CMCBIO::Ctl_ComTest(LPVOID lpData)
{
	CMCBView *ph=(CMCBView *)lpData;
    CString s;
	memset(ph->m_recbuff,0,1024*4*8);
	memset(ph->m_secrecbuf,0,1024*4*8);
	ph->m_precbuf = ph->m_recbuff;
	ph->m_iworkcounter = 0;

	g_RecPackageLenth = 256;
	m_SendFrame[0] = 19; 
	
	int nWR = WriteCommandMaint232(g_hComm,1,m_SendFrame);
	if(nWR == false) {
		AfxMessageBox("Communication Write Error.\n\n Please check Communication Setting.");
	}

	ph->PostMessage(WM_SENDSHOW,(WPARAM)m_SendFrame,1);

	int dwRet = WaitForSingleObject(g_hEvtFinishRec, SENDTIMEOUT); //发送超时SENDTIMEOUT
	switch(dwRet){
	case WAIT_OBJECT_0:
			break;		
	case WAIT_TIMEOUT:
			AfxMessageBox("测试发送超时了");
//          s.Format("%s","测试发送超时了.");
//		    ph->TextScroll(s);
			g_ThreadCMD = NULL;		//发送命令默认为空
			break;
	default:			
			break;
	}
	ResetEvent(g_hEvtSend);
	g_bExec = FALSE;

}

void CMCBIO::Ctl_LoadFullRam(LPVOID lpData)
{
	CMCBView *ph=(CMCBView *)lpData;

	memset(ph->m_recbuff,0,1024*4*8);
	memset(ph->m_secrecbuf,0,1024*4*8);
	ph->m_precbuf = ph->m_recbuff;
	ph->m_iworkcounter = 0;

	g_RecPackageLenth = 512*3;

	m_SendFrame[0] = 13;
	m_SendFrame[1] = 21;

	int nWR = WriteCommandMaint232(g_hComm,1,m_SendFrame);
	if(nWR == false) {
		AfxMessageBox("Communication Write Error.\n\n Please check Communication Setting.");
	}
	Sleep(10);
	m_SendFrame[0] = 21;
	nWR = WriteCommandMaint232(g_hComm,1,m_SendFrame);
	if(nWR == false) {
		AfxMessageBox("Communication Write Error.\n\n Please check Communication Setting.");
	}

	m_SendFrame[0] = 13;
	m_SendFrame[1] = 21;

	ph->PostMessage(WM_SENDSHOW,(WPARAM)m_SendFrame,2);
	
//	int dwRet = WaitForSingleObject(g_hEvtFinishRec, SENDTIMEOUT); //发送超时
	int dwRet = WaitForSingleObject(g_hEvtFinishRec, SENDTIMEOUT*3); //发送超时
	switch(dwRet){
	case WAIT_OBJECT_0:
			break;	
	case WAIT_TIMEOUT:
//			sprintf(textout,"要求传送%d字节，实际接收到%d字节",g_RecPackageLenth,ph->m_iworkcounter);
//			AfxMessageBox(textout);
			break;
	default:	
			//AfxMessageBox("FullRam发送超时了");
			g_ThreadCMD = NULL;		//发送命令默认为空
			break;
	}

	ResetEvent(g_hEvtSend);
	g_bExec = FALSE;

}

void CMCBIO::Ctl_LoadExpRam(LPVOID lpData)
{
	CMCBView *ph=(CMCBView *)lpData;

	memset(ph->m_recbuff,0,1024*4*8);
	memset(ph->m_secrecbuf,0,1024*4*8);
	ph->m_precbuf = ph->m_recbuff;
	ph->m_iworkcounter = 0;

//	if(ph->m_bExpRamLongNew_valid) {
//		ph->m_bExpRamLongNew_valid = false;
//		ph->m_ExpRamDataLong_old = ph->m_ExpRamDataLong_new;
//	}

	g_RecPackageLenth = ph->m_ExpRamDataLong_old;

	m_SendFrame[0] = 13;
	int nWR = WriteCommandMaint232(g_hComm,1,m_SendFrame);
	if(nWR == false) {
		AfxMessageBox("Communication Write Error.\n\n Please check Communication Setting.");
	}
	Sleep(10);
	m_SendFrame[0] = 22;
	nWR = WriteCommandMaint232(g_hComm,1,m_SendFrame);
	if(nWR == false) {
		AfxMessageBox("Communication Write Error.\n\n Please check Communication Setting.");
	}
	
	m_SendFrame[0] = 13;
	m_SendFrame[1] = 22;

	ph->PostMessage(WM_SENDSHOW,(WPARAM)m_SendFrame,2);

//	int dwRet = WaitForSingleObject(g_hEvtFinishRec, SENDTIMEOUT*3); //发送超时
	int dwRet = WaitForSingleObject(g_hEvtFinishRec, SENDTIMEOUT*5); //发送超时
	switch(dwRet){
	case WAIT_OBJECT_0:
			break;			
	case WAIT_TIMEOUT:
//			sprintf(textout,"要求传送%d字节，实际接收到%d字节",g_RecPackageLenth,ph->m_iworkcounter);
//			AfxMessageBox(textout);
			break;
	default:	
			//AfxMessageBox("ExpRam发送超时了");
			g_ThreadCMD = NULL;		//发送命令默认为空
			break;
	}

	ResetEvent(g_hEvtSend);
	g_bExec = FALSE;

}

void CMCBIO::Ctl_LoadTimeValue(LPVOID lpData)
{
	CMCBView *ph=(CMCBView *)lpData;

	ph->KillTimer(1);
	if (g_MCB_Oper.bQueryEn) {		
		ph->SetTimer(1,1000,NULL);
	}	

	memset(ph->m_recbuff,0,1024*4*8);
	memset(ph->m_secrecbuf,0,1024*4*8);
	ph->m_precbuf = ph->m_recbuff;
	ph->m_iworkcounter = 0;

	g_RecPackageLenth = 8;
	m_SendFrame[0] = 04;
	int nWR = WriteCommandMaint232(g_hComm,1,m_SendFrame);
	if(nWR == false) {
		AfxMessageBox("Communication Write Error.\n\n Please check Communication Setting.");
	}

	ph->PostMessage(WM_SENDSHOW,(WPARAM)m_SendFrame,1);

	int dwRet = WaitForSingleObject(g_hEvtFinishRec, SENDTIMEOUT); //发送超时
	switch(dwRet){
	case WAIT_OBJECT_0:
			break;				 
	default:	
		//AfxMessageBox("ReadTime发送超时了");
		g_ThreadCMD = NULL;		//发送命令默认为空
			break;
	}
	ResetEvent(g_hEvtSend);
	g_bExec = FALSE;

}

void CMCBIO::Ctl_LoadInterstChannCounter(LPVOID lpData)
{
	CMCBView *ph=(CMCBView *)lpData;
	memset(ph->m_recbuff,0,1024*4*8);
	memset(ph->m_secrecbuf,0,1024*4*8);
	ph->m_precbuf = ph->m_recbuff;
	ph->m_iworkcounter = 0;
	g_RecPackageLenth = 3;

	if(ph->m_bExpRamLongNew_valid) 
	{
		ph->m_bExpRamLongNew_valid = false;
		ph->m_ExpRamDataLong_old = ph->m_ExpRamDataLong_new;
	}


	m_SendFrame[0] = 17;
	type.intdata = ph->m_iSmallWinCenPoint;		//设置感兴趣道
	m_SendFrame[1] = type.uchdata[0];		
	m_SendFrame[2] = type.uchdata[1];

	m_SendFrame[3] = 18;				//【设置道边界】
	type.intdata = ph->m_iSmallWinCHL;		//道左边界
	m_SendFrame[4] = type.uchdata[0];		
	m_SendFrame[5] = type.uchdata[1];
	type.intdata = ph->m_iSmallWinCHR;		//道右边界
	m_SendFrame[6] = type.uchdata[0];		
	m_SendFrame[7] = type.uchdata[1];

	int nWR = WriteCommandMaint232(g_hComm,8,m_SendFrame);
	if(nWR == false) {
		AfxMessageBox("Communication Write Error.\n\n Please check Communication Setting.");
	}
	Sleep(10);
	m_SendFrame[0] = 12;
	nWR = WriteCommandMaint232(g_hComm,1,m_SendFrame);
	if(nWR == false) {
		AfxMessageBox("Communication Write Error.\n\n Please check Communication Setting.");
	}
	Sleep(10);
	m_SendFrame[0] = 20;
	nWR = WriteCommandMaint232(g_hComm,1,m_SendFrame);
	if(nWR == false) {
		AfxMessageBox("Communication Write Error.\n\n Please check Communication Setting.");
	}

	m_SendFrame[0] = 17;
	type.intdata = ph->m_iSmallWinCenPoint;		//设置感兴趣道
	m_SendFrame[1] = type.uchdata[0];		
	m_SendFrame[2] = type.uchdata[1];

	m_SendFrame[3] = 18;				//【设置道边界】
	type.intdata = ph->m_iSmallWinCHL;		//道左边界
	m_SendFrame[4] = type.uchdata[0];		
	m_SendFrame[5] = type.uchdata[1];
	type.intdata = ph->m_iSmallWinCHR;		//道右边界
	m_SendFrame[6] = type.uchdata[0];		
	m_SendFrame[7] = type.uchdata[1];
	m_SendFrame[8] = 12;
	m_SendFrame[9] = 20;
	ph->PostMessage(WM_SENDSHOW,(WPARAM)m_SendFrame,10);  //show send data


	int dwRet = WaitForSingleObject(g_hEvtFinishRec, SENDTIMEOUT); 
	switch(dwRet){
	case WAIT_OBJECT_0:
			break;				 
	default:			
			g_ThreadCMD = NULL;		//发送命令默认为空
			break;
	}
	ResetEvent(g_hEvtSend);
	g_bExec = FALSE;

}

void CMCBIO::Ctl_ResetSystem(LPVOID lpData)
{
	CMCBView *ph=(CMCBView *)lpData;

	memset(ph->m_recbuff,0,1024*4*8);
	memset(ph->m_secrecbuf,0,1024*4*8);
	ph->m_precbuf = ph->m_recbuff;
	ph->m_iworkcounter = 0;
	
	m_SendFrame[0] = 9;				//硬件系统复位

	g_RecPackageLenth = 8;
	int nWR = WriteCommandMaint232(g_hComm,1,m_SendFrame);
	if(nWR == false) {
		AfxMessageBox("Communication Write Error.\n\n Please check Communication Setting.");
	}


	ph->PostMessage(WM_SENDSHOW,(WPARAM)m_SendFrame,1);

	int dwRet = WaitForSingleObject(g_hEvtFinishRec, SENDTIMEOUT); //发送超时
	switch(dwRet){
	case WAIT_OBJECT_0:
			 break;				 
	default:		
		g_ThreadCMD = NULL;		//发送命令默认为空
			break;
	}

	ResetEvent(g_hEvtSend);
	g_bExec = FALSE;

}

void CMCBIO::Ctl_ContinueSample(LPVOID lpData)
{
	CMCBView *ph=(CMCBView *)lpData;

	memset(ph->m_recbuff,0,1024*4*8);
	memset(ph->m_secrecbuf,0,1024*4*8);
	ph->m_precbuf = ph->m_recbuff;
	ph->m_iworkcounter = 0;
	
	
	m_SendFrame[0] = 5;        // 【设置道宽】
	type.intdata = g_HardWare_Pare.iChannelWidth; 
	m_SendFrame[1] = type.uchdata[0];		//道宽
	m_SendFrame[2] = type.uchdata[1];

	m_SendFrame[3] = 1;				//硬件开始

	g_RecPackageLenth = 8;
	int nWR = WriteCommandMaint232(g_hComm,4,m_SendFrame);
	if(nWR == false) {
		AfxMessageBox("Communication Write Error.\n\n Please check Communication Setting.");
	}


	ph->PostMessage(WM_SENDSHOW,(WPARAM)m_SendFrame,4);

	int dwRet = WaitForSingleObject(g_hEvtFinishRec, SENDTIMEOUT); //发送超时
	switch(dwRet){
	case WAIT_OBJECT_0:
			 break;				 
	default:			
			g_ThreadCMD = NULL;		//发送命令默认为空
			break;
	}

	ResetEvent(g_hEvtSend);
	g_bExec = FALSE;
}

void CMCBIO::Ctl_SendExternalRam(LPVOID lpData)
{
	CMCBView *ph=(CMCBView *)lpData;

	memset(ph->m_recbuff,0,1024*4*8);
	memset(ph->m_secrecbuf,0,1024*4*8);
	ph->m_precbuf = ph->m_recbuff;
	ph->m_iworkcounter = 0;

	g_RecPackageLenth = 1;
	m_SendFrame[0] = 6; 
	int nWR = WriteCommandMaint232(g_hComm,1,m_SendFrame);
	if(nWR == false) {
		AfxMessageBox("Communication Write Error.\n\n Please check Communication Setting.");
	}

	Sleep(20);
	for(int kkk=0;kkk< (ph->m_iFullChannelWidth/32);kkk++) {  // 和道宽，增益 Gain 有关
		for(unsigned char i=0;i<32;i++) {
			type.intdata = ph->m_arrBUFFExternalRam[i+kkk*32];
			m_SendFrame[i*3+0] = type.uchdata[0];
			m_SendFrame[i*3+1] = type.uchdata[1];
			m_SendFrame[i*3+2] = type.uchdata[2];
		}
		nWR = WriteCommandMaint232(g_hComm,96,m_SendFrame);
		if(nWR == false) {
			AfxMessageBox("Communication Write Error.\n\n Please check Communication Setting.");
		}
		//Sleep(20);
	}

	ph->PostMessage(WM_SENDSHOW,(WPARAM)ph->m_arrBUFFExternalRam,20);

	int dwRet = WaitForSingleObject(g_hEvtFinishRec, SENDTIMEOUT); //发送超时SENDTIMEOUT
	switch(dwRet){
	case WAIT_OBJECT_0:
			break;				 
	default:			
			g_ThreadCMD = NULL;		//发送命令默认为空
			break;
	}

	ResetEvent(g_hEvtSend);
	g_bExec = FALSE;

}

void CMCBIO::Ctl_QueryTimeOut(LPVOID lpData)
{
	CMCBView *ph=(CMCBView *)lpData;

	memset(ph->m_recbuff,0,1024*4*8);
	memset(ph->m_secrecbuf,0,1024*4*8);
	ph->m_precbuf = ph->m_recbuff;
	ph->m_iworkcounter = 0;
	
	m_SendFrame[0] = 10;				//查询定时到否

	g_RecPackageLenth = 1;
	int nWR = WriteCommandMaint232(g_hComm,1,m_SendFrame);
	if(nWR == false) {
		AfxMessageBox("Communication Write Error.\n\n Please check Communication Setting.");
	}

	ph->PostMessage(WM_SENDSHOW,(WPARAM)m_SendFrame,1);

	int dwRet = WaitForSingleObject(g_hEvtFinishRec, SENDTIMEOUT); //发送超时
	switch(dwRet){
	case WAIT_OBJECT_0:
			 break;				 
	default:			
			g_ThreadCMD = NULL;		//发送命令默认为空
			break;
	}

	ResetEvent(g_hEvtSend);
	g_bExec = FALSE;
}

void CMCBIO::Ctl_QuerySampleState(LPVOID lpData)
{
	CMCBView *ph=(CMCBView *)lpData;

	ph->KillTimer(1);
	if (g_MCB_Oper.bQueryEn) {		
		ph->SetTimer(1,1000,NULL);
	}

	memset(ph->m_recbuff,0,1024*4*8);
	memset(ph->m_secrecbuf,0,1024*4*8);
	ph->m_precbuf = ph->m_recbuff;
	ph->m_iworkcounter = 0;
	
	m_SendFrame[0] = 11;				//状态查询

	g_RecPackageLenth = 1;
	int nWR = WriteCommandMaint232(g_hComm,1,m_SendFrame);
	if(nWR == false) {
		AfxMessageBox("Communication Write Error.\n\n Please check Communication Setting.");
	}
	ph->PostMessage(WM_SENDSHOW,(WPARAM)m_SendFrame,1);

	int dwRet = WaitForSingleObject(g_hEvtFinishRec, SENDTIMEOUT); //发送超时
	switch(dwRet){
	case WAIT_OBJECT_0:
			 break;				 
	default:			
			g_ThreadCMD = NULL;		//发送命令默认为空
			break;
	}

	ResetEvent(g_hEvtSend);
	g_bExec = FALSE;

}

void CMCBIO::Ctl_ReceiveExternalRam(LPVOID lpData)
{
	CMCBView *ph=(CMCBView *)lpData;

	memset(ph->m_recbuff,0,1024*4*8);
	memset(ph->m_secrecbuf,0,1024*4*8);
	ph->m_precbuf = ph->m_recbuff;
	ph->m_iworkcounter = 0;


	m_SendFrame[0] = 2;				//停止采集数据
									
	m_SendFrame[1] = 5;				// 【设置道宽】
	type.intdata = g_HardWare_Pare.iChannelWidth; 
	m_SendFrame[2] = type.uchdata[0];		//道宽
	m_SendFrame[3] = type.uchdata[1];	

	m_SendFrame[4] = 7;				//接收ExternalRam中的数据

	g_RecPackageLenth = ph->m_iFullChannelWidth * 3;
	int nWR = WriteCommandMaint232(g_hComm,5,m_SendFrame);
	if(nWR == false) {
		AfxMessageBox("Communication Write Error.\n\n Please check Communication Setting.");
	}

	ph->PostMessage(WM_SENDSHOW,(WPARAM)m_SendFrame,5);

	int dwRet = WaitForSingleObject(g_hEvtFinishRec, SENDTIMEOUT*5); //发送超时
	switch(dwRet)
	{
		case WAIT_OBJECT_0:
				break;				 
		default:			
				g_ThreadCMD = NULL;		//发送命令默认为空
				break;
	}

	ResetEvent(g_hEvtSend);
	g_bExec = FALSE;

}

void CMCBIO::Ctl_Prog_ConfigAnolPara(LPVOID lpData)
{
	CMCBView *ph=(CMCBView *)lpData;
	memset(ph->m_recbuff,0,1024*4*8);
	memset(ph->m_secrecbuf,0,1024*4*8);
	ph->m_precbuf = ph->m_recbuff;
	ph->m_iworkcounter = 0;

	unsigned char sendaddr[10] = {0,1,2,4,8,16,32,64,128,255};	

	for(int i=0;i<9;i++) {	
		m_SendFrame[0] = 0x80;				//停止采集数据
		m_SendFrame[1] = sendaddr[i];
		m_SendFrame[2] = (unsigned char)ph->m_iProgCtrlData[i];  //

		int nWR = WriteCommandMaint232(g_hComm,3,m_SendFrame);
		if(nWR == false) {
			AfxMessageBox("Communication Write Error.\n\n Please check Communication Setting.");
		}
		ph->PostMessage(WM_SENDSHOW,(WPARAM)m_SendFrame,3);
		Sleep(20);
	}

	int dwRet = WaitForSingleObject(g_hEvtFinishRec, SENDTIMEOUT); //发送超时  10
	switch(dwRet){
	case WAIT_OBJECT_0:
			 break;				 
	default:			
			g_ThreadCMD = NULL;		//发送命令默认为空
			break;
	}

	ResetEvent(g_hEvtSend);
	g_bExec = FALSE;

}


void CMCBIO::Ctl_HardWare_Read(LPVOID lpData)
{
	CMCBView *ph=(CMCBView *)lpData;
	CString s;
	memset(ph->m_recbuff,0,1024*4*8);
	memset(ph->m_secrecbuf,0,1024*4*8);
	ph->m_precbuf = ph->m_recbuff;
	ph->m_iworkcounter = 0;

	ph->m_bExpRamLongNew_valid = FALSE;//保证这包数据被接收到

	unsigned char sendbuff[10];
	sendbuff[0] = 0x1a;
	sendbuff[1] = 0x17;
	sendbuff[2] = 0xa0;
	sendbuff[3] = 0;
	sendbuff[4] = 0;
	sendbuff[5] = 0xa1;

	g_RecPackageLenth = 23; // 数据应答长度

	int nWR = WriteCommandMaint232(g_hComm,6,sendbuff);
	if(nWR == false) {
		AfxMessageBox("Communication Write Error.\n\n Please check Communication Setting.");
	}
	ph->PostMessage(WM_SENDSHOW,(WPARAM)sendbuff,6);

	int dwRet = WaitForSingleObject(g_hEvtFinishRec, SENDTIMEOUT); //发送超时  10
	switch(dwRet){
	case WAIT_OBJECT_0:
		break;				 
	default:			
		g_ThreadCMD = NULL;		//发送命令默认为空
		//Beep(4000,400);
		AfxMessageBox("数据接收超时!\r\n1.请检查串口是否选择正确\r\n2.线路是否连接良好。");
        s.Format("%s","数据接收超时!1.请检查串口是否选择正确2.线路是否连接良好.");
//		ph->TextScroll(s);
		break;
	}	
	ResetEvent(g_hEvtSend);
	g_bExec = FALSE;

}

//-------------------------------------------------------------
//首先设置 EEprom 各个控制参数
//再次设置 FPGA 几个相关的参数
//-------------------------------------------------------------
void CMCBIO::Ctl_HardWare_Write(LPVOID lpData)
{
	CMCBView *ph=(CMCBView *)lpData;
	memset(ph->m_recbuff,0,1024*4*8);
	memset(ph->m_secrecbuf,0,1024*4*8);
	ph->m_precbuf = ph->m_recbuff;
	ph->m_iworkcounter = 0;

	unsigned char sendbuff[30];
    //数据包头部
	sendbuff[0] = 0x1b;
	sendbuff[1] = 0x17;
	sendbuff[2] = 0xa0;
	sendbuff[3] = 0;
	sendbuff[4] = 0;
	//道宽
	sendbuff[5] = (unsigned char)g_HardWare_Pare.iChannelWidth; //先低位
	sendbuff[6] = (unsigned char)(g_HardWare_Pare.iChannelWidth>>8);//后高位
	//采样时间    采样时间单位为1/100 s;
	type.intdata = g_HardWare_Pare.iSampleTimeValue * 100;
	sendbuff[7] = type.uchdata[0];  // 先低后高
	sendbuff[8] = type.uchdata[1];
	sendbuff[9] = type.uchdata[2];
	sendbuff[10] = type.uchdata[3];
	//斜率
	sendbuff[11] = 0;  //高4字节位0
	sendbuff[12] = 0; 
	type.intdata = (int)g_HardWare_Pare.fEngerScaleSlope;							
	sendbuff[13] = type.uchdata[0];
	sendbuff[14] = type.uchdata[1];
	sendbuff[15] = type.uchdata[2];
	sendbuff[16] = type.uchdata[3]; //整数部分 4个字节
	float kkk = ((float)(g_HardWare_Pare.fEngerScaleSlope + 0.000001) - (float)type.intdata)*1000;
	
	type.intdata = (UINT)kkk;   // 小数部分 2个字节
	sendbuff[17] = type.uchdata[0];
	sendbuff[18] = type.uchdata[1];
	//偏置 
	sendbuff[19] = 0;  //高4字节位0
	sendbuff[20] = 0;
	type.intdata = (int)g_HardWare_Pare.fEngerScaleOffset;							
	sendbuff[21] = type.uchdata[0];
	sendbuff[22] = type.uchdata[1];
	sendbuff[23] = type.uchdata[2];
	sendbuff[24] = type.uchdata[3]; //整数部分 4个字节
	kkk = ((float)(g_HardWare_Pare.fEngerScaleOffset + 0.000001) - (float)type.intdata)*1000;
	
	type.intdata = (UINT)kkk;   // 小数部分 2个字节
	sendbuff[25] = type.uchdata[0];
	sendbuff[26] = type.uchdata[1];

	//other 
	unsigned char uchother = 0;
	if (g_HardWare_Pare.bSampleModeRealTime == TRUE) { //bit7  实时/活时
		uchother |= 0x80;
	}else uchother &= 0x7F;
	if (g_HardWare_Pare.bDT_Pulse == TRUE) {  //bit6  DT 死时间 高低有效
		uchother |= 0x40;	   
	}else uchother &= 0xBF;
	if (g_HardWare_Pare.bREADY_Pulse == TRUE) { //bit5 Ready 高低有效
		uchother |= 0x20;
	}else uchother &= 0xDF;
	if (g_HardWare_Pare.bANSER_Pulse == TRUE) { //bit4  anwser 高低有效
		uchother |= 0x10;
	}else uchother &= 0xef;
	if (g_HardWare_Pare.bENADC_Pulse == TRUE) { //bit3 EN ADC 高低有效
		uchother |= 0x08;
	}else uchother &= 0xF7;

	if (g_HardWare_Pare.bDataPolarity_Pulse == TRUE) { //bit2 DATA极性 高低有效
		uchother |= 0x04;
	}else uchother &= 0xFB;

	switch(g_HardWare_Pare.iEngerScaleUnit) { //bit1－bit0   单位
	case 0:
		uchother &= 0xFC;
		break;
	case 1:
		uchother &= 0xFD;
		uchother |= 0x01;
		break;
	case 2:
		uchother &= 0xFE;
		uchother |= 0x02;
		break;
	case 3:
		uchother |= 0x03;
		break;
	default:
		break;  
	}	
	sendbuff[27] = uchother;     

	g_RecPackageLenth = 0; // 数据应答长度	
	WriteCommandMaint232(g_hComm,28,sendbuff);
	ph->PostMessage(WM_SENDSHOW,(WPARAM)sendbuff,28);

	//以上为设置EEProm的参数

	Sleep(200);
	//以下设置FPGA寄存器参数
										
	if (g_HardWare_Pare.bSampleModeRealTime) { //【设置时间模式】
		sendbuff[0] = 15;		
		sendbuff[1] = 0x80;		// 实时  (d7 位有效 = '1')
	}
	else{		
		sendbuff[0] = 15;		
		sendbuff[1] = 0x00;		// 活时  (d7 位有效 = '0')
	}
	
	sendbuff[2] = 16;	//【ADC 极性设置】
	sendbuff[3] = uchother;	
		
	type.intdata = g_HardWare_Pare.iSampleTimeValue * 100; 
	sendbuff[4] = 8;		//【设置时间长度】				
	sendbuff[5] = type.uchdata[0];
	sendbuff[6] = type.uchdata[1];
	sendbuff[7] = type.uchdata[2];
	sendbuff[8] = type.uchdata[3];
	
	sendbuff[9] = 5;   // 【设置道宽】
	//type.intdata = 1024; //默认道宽为1024
	type.intdata = g_HardWare_Pare.iChannelWidth; 
	sendbuff[10] = type.uchdata[0];		//道宽
	sendbuff[11] = type.uchdata[1];

	g_RecPackageLenth = 0; // 数据应答长度	
	WriteCommandMaint232(g_hComm,12,sendbuff);
	
	ph->PostMessage(WM_SENDSHOW,(WPARAM)sendbuff,12);


	int dwRet = WaitForSingleObject(g_hEvtFinishRec, SENDTIMEOUT/3); //发送超时  10
	switch(dwRet){
	case WAIT_OBJECT_0:
		break;				 
	default:			
		g_ThreadCMD = NULL;		//发送命令默认为空
		break;
	}	
	ResetEvent(g_hEvtSend);
	g_bExec = FALSE;

	Beep(3500,200);
	
}



























