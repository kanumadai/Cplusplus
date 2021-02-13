#include "StdAfx.h"
#include "OWSSio.h"
#include "Resource.h"
#include "SimOWSDlg.h"

#include "DbgPrt.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern HWND g_hWndOWS;

////////////////////////////////////////////////////////////////////
//              visiable in global 
CRITICAL_SECTION g_CLockMCtlInfo;
MCTL_INFO   g_MCtlInfo;             //机电控制电路相关信息

//
////////////////////////////////////////////////////////////////////

void UpdateDisplay();

void UpdateDisplay()
{
	char szTemp[512];

	switch( g_MCtlInfo.Rod_stat ){
	case unknow:
		 sprintf( szTemp, "棒源状态:未知" );      break;
    case out:	    //伸出到位
		 sprintf( szTemp, "棒源状态:伸出到位" );  break;
    case in:		//缩回到位
		 sprintf( szTemp, "棒源状态:缩回到位" );  break;
    case outing:	//伸出中
		 sprintf( szTemp, "棒源状态:伸出过程中" );  break;
    case ining:		//缩回中
		 sprintf( szTemp, "棒源状态:缩回过程中" );  break;
    case rolling:   //正在旋转
		 sprintf( szTemp, "棒源状态:正在旋转" );  break;
    case stop:		//停止
		 sprintf( szTemp, "棒源状态:停止" );  break;
	default:
		 sprintf( szTemp, "棒源状态:XXX"); break;
	}

	switch( g_MCtlInfo.septa_stat ){
	case unknow:
		 sprintf( szTemp, "%s    Septa状态:未知", szTemp );      break;
    case out:	    //伸出到位
		 sprintf( szTemp, "%s    Septa状态:伸出到位", szTemp );  break;
    case in:		//缩回到位
		 sprintf( szTemp, "%s    Septa状态:缩回到位", szTemp );  break;
    case outing:	//伸出中
		 sprintf( szTemp, "%s    Septa状态:伸出过程中", szTemp );  break;
    case ining:		//缩回中
		 sprintf( szTemp, "%s    Septa状态:缩回过程中", szTemp );  break;
    case rolling:   //正在旋转
		 sprintf( szTemp, "%s    Septa状态:正在旋转", szTemp );  break;
    case stop:		//停止
		 sprintf( szTemp, "%s    Septa状态:停止", szTemp );  break;
	default:
		 sprintf( szTemp, "%s    Septa状态:XXX"); break;
	}

	AfxGetMainWnd()->SetDlgItemText(IDC_TIPS_MC, szTemp);
}


BOOL Process_GantryDat(SIO_FRAME12* pFrame)
{
#if DBG_LEVEL != 0 
	char szTemp[512];
#endif
	int i;
	BYTE* pBuf = (BYTE*)pFrame;

#if DBG_LEVEL != 0
	sprintf(szTemp, "Recv<==");
	for(i=0; i<sizeof(SIO_FRAME12); i++){
	   sprintf(szTemp, "%s%2.2x ",szTemp, pBuf[i]);
	}
	sprintf(szTemp,"%s\r\n",szTemp);
	DbgPrt(szTemp, g_hWndOWS);
#endif

	//verify eof flag
	if(pFrame->eof != 0x88 ){
		DbgPrt("GANTRY EOF FLAG ERROR!\r\n", g_hWndOWS);
		return FALSE;
	}

	//verify checksum
	BYTE checksum=0;
	pBuf = (BYTE*)pFrame;
	for(i=0; i< (sizeof(SIO_FRAME12)-2); i++)
		checksum ^= pBuf[i];


	if(checksum != pFrame->CheckSum){
		DbgPrt("CheckSum Error!\r\n", g_hWndOWS);
		return FALSE;
	}

	switch(pFrame->CmdMain){
		case 0xA0:  // GANTRY握手应答
			 DbgPrt("GANTRY HandShake Answer\r\n", g_hWndOWS);
			 break;

		case 0xA5:  //设置数据有效应答
			 DbgPrt("Set Data Valid\r\n", g_hWndOWS);
			 break;

		case 0xE0:  // 数据重发请求
			 DbgPrt("Require to Send Again\r\n", g_hWndOWS);
			 break;

		case 0xB3:  //回传温度（GANTRY、ROD、YE12、SEPTA、BED）
			 EnterCriticalSection(&g_CLockMCtlInfo);
			    switch( pFrame->CmdSub )
				{
				case ADDR_GANTRY:
				     //Gantry温度
			 		 g_MCtlInfo.wT_GANTRY = MAKEWORD(pFrame->Dat[0], pFrame->Dat[1]);
					 #if DBG_LEVEL != 0
						sprintf(szTemp,"Upload GANTRY T=%4.4x\r\n", g_MCtlInfo.wT_GANTRY);
					 #endif
					 break;

				case ADDR_ROD:
					 //ROD温度
					 g_MCtlInfo.wT_Rod = MAKEWORD(pFrame->Dat[0], pFrame->Dat[1]);
					 #if DBG_LEVEL != 0
						sprintf(szTemp,"Upload ROD T=%4.4x\r\n", g_MCtlInfo.wT_Rod);
					 #endif
					 break;

				case ADDR_BED:
					 //BED温度
					 g_MCtlInfo.wT_Bed = MAKEWORD(pFrame->Dat[0], pFrame->Dat[1]);
					 #if DBG_LEVEL != 0
						sprintf(szTemp,"Upload BED T=%4.4x\r\n", g_MCtlInfo.wT_Bed);
					 #endif
					 break;

				case ADDR_SEPTA:
					 //SEPTA温度
					 g_MCtlInfo.wT_Septa = MAKEWORD(pFrame->Dat[0], pFrame->Dat[1]);
					 #if DBG_LEVEL != 0
						sprintf(szTemp,"Upload Septa T=%4.4x\r\n", g_MCtlInfo.wT_Septa);
					 #endif
					 break;

				default:
					 #if DBG_LEVEL != 0
						sprintf(szTemp,"Upload %2.2x T=%4.4x\r\n", pFrame->CmdSub, g_MCtlInfo.wT_GANTRY);
					 #endif
					 break;
				}

				DbgPrt(szTemp, g_hWndOWS);
			     
			 LeaveCriticalSection(&g_CLockMCtlInfo);

			 break;

		case 0xC0:	// 回传WATERCOOL温度
			 EnterCriticalSection(&g_CLockMCtlInfo);
				if(pFrame->CmdSub==0x20)
				{   //WaterCool温度
					g_MCtlInfo.wT_WATERCOOL = MAKEWORD(pFrame->Dat[0], pFrame->Dat[1]);
					#if DBG_LEVEL != 0
						sprintf(szTemp,"Upload WaterCool T=%4.4x\r\n", g_MCtlInfo.wT_WATERCOOL);
					#endif
				}
				DbgPrt(szTemp, g_hWndOWS);
			 LeaveCriticalSection(&g_CLockMCtlInfo);

			 break;

		case 0xC1:  // 回传高压、电流值
			 EnterCriticalSection(&g_CLockMCtlInfo);
			     
			     g_MCtlInfo.HVStatus = pFrame->CmdSub;

				 g_MCtlInfo.wV_1 = MAKEWORD( pFrame->Dat[0], pFrame->Dat[1] );
				 g_MCtlInfo.wI_1 = MAKEWORD( pFrame->Dat[2], pFrame->Dat[3] );
					 #if DBG_LEVEL != 0
					    sprintf(szTemp,"Upload HV Status=%d, HV=%d，HVI=%dx\r\n", g_MCtlInfo.HVStatus, g_MCtlInfo.wV_1,g_MCtlInfo.wI_1);
					 #endif
				 DbgPrt(szTemp, g_hWndOWS);

			 LeaveCriticalSection(&g_CLockMCtlInfo);

			 break;

		/*prev protocol
		case 0xC1:  // 回传高压、电流值
			 EnterCriticalSection(&g_CLockMCtlInfo);
			     
			     ASSERT( pFrame->CmdSub==0x10 || pFrame->CmdSub==0x20 );

				 if(pFrame->CmdSub == 0x10){       //两路高压
					 g_MCtlInfo.wV_1 = MAKEWORD( pFrame->Dat[0], pFrame->Dat[1] );
					 g_MCtlInfo.wV_2 = MAKEWORD( pFrame->Dat[2], pFrame->Dat[3] );
					 #if DBG_LEVEL != 0
					    sprintf(szTemp,"Upload Two HV=%4.4x，%4.4x\r\n", g_MCtlInfo.wV_1,g_MCtlInfo.wV_2);
					 #endif
				 }else if(pFrame->CmdSub == 0x20){  //两路电流
					 g_MCtlInfo.wI_1 = MAKEWORD( pFrame->Dat[0], pFrame->Dat[1] );
					 g_MCtlInfo.wI_2 = MAKEWORD( pFrame->Dat[2], pFrame->Dat[3] );
					 #if DBG_LEVEL != 0
					    sprintf(szTemp,"Upload Two HV I=%4.4x，%4.4x\r\n", g_MCtlInfo.wI_1,g_MCtlInfo.wI_2);
					 #endif
				 }
				 DbgPrt(szTemp, g_hWndOWS);

			 LeaveCriticalSection(&g_CLockMCtlInfo);

			 break;
		*/

		case 0xC2:	//高压控制应答
			 EnterCriticalSection(&g_CLockMCtlInfo);

				 ASSERT(pFrame->CmdSub==0x65 || pFrame->CmdSub==0x32);
				 if( pFrame->CmdSub == 0x65 )
				 {
					 g_MCtlInfo.bHVOn = TRUE;
					 DbgPrt("HV ON/Off Control Answer:ON\n", g_hWndOWS);
				 }
				 else
				 {
					 g_MCtlInfo.bHVOn = FALSE;
					 DbgPrt("HV ON/Off Control Answer:OFF\n", g_hWndOWS);
				 }

			 LeaveCriticalSection(&g_CLockMCtlInfo);

			 break;

		case 0xC3:  //激光灯控制应答
			 EnterCriticalSection(&g_CLockMCtlInfo);

				 ASSERT(pFrame->CmdSub==0x65 || pFrame->CmdSub==0x32);
				 if( pFrame->CmdSub == 0x65 )
				 {
					 g_MCtlInfo.bLaserOn = TRUE;
					 DbgPrt("LaserLed On/Off Control Answer:ON\n", g_hWndOWS);
				 }
				 else
				 {
					 g_MCtlInfo.bLaserOn = FALSE;
					 DbgPrt("LaserLed On/Off Control Answer:Off\n", g_hWndOWS);
				 }

			 LeaveCriticalSection(&g_CLockMCtlInfo);

			 break;

		case 0xC4:  //前端主电源控制应答
			 EnterCriticalSection(&g_CLockMCtlInfo);

				 ASSERT(pFrame->CmdSub==0x65 || pFrame->CmdSub==0x32);
				 if( pFrame->CmdSub == 0x65 )
				 {
					 g_MCtlInfo.bMainPowerOn = TRUE;
					 DbgPrt("Main Power On/Off Answer:On\n", g_hWndOWS);
				 }
				 else
				 {
					 g_MCtlInfo.bMainPowerOn = FALSE;
					 DbgPrt("Main Power On/Off Answer:Off\n", g_hWndOWS);
				 }

			 LeaveCriticalSection(&g_CLockMCtlInfo);

			 break;

		case 0xC5:  // 回传电压边界
			 EnterCriticalSection(&g_CLockMCtlInfo);
			     
				 ASSERT(pFrame->CmdSub == 0x10);

				 g_MCtlInfo.wTH_V_H = MAKEWORD( pFrame->Dat[0], pFrame->Dat[1] );//高压上限
				 g_MCtlInfo.wTH_V_L = MAKEWORD( pFrame->Dat[2], pFrame->Dat[3] );//高压下限
				 #if DBG_LEVEL != 0
				    sprintf(szTemp,"Upload HV Threshold=%4.4x，%4.4x\r\n", g_MCtlInfo.wTH_V_H,g_MCtlInfo.wTH_V_L);
				 #endif
				 DbgPrt(szTemp, g_hWndOWS);

			 LeaveCriticalSection(&g_CLockMCtlInfo);

			 break;

		case 0xC6:  // 回传电流边界
			 EnterCriticalSection(&g_CLockMCtlInfo);
			     
				g_MCtlInfo.wTH_I_H = MAKEWORD( pFrame->Dat[0], pFrame->Dat[1] );//电流上限
				g_MCtlInfo.wTH_I_L = MAKEWORD( pFrame->Dat[2], pFrame->Dat[3] );//电流下限
				#if DBG_LEVEL != 0
				    sprintf(szTemp,"Upload HV I Threshold=%4.4x，%4.4x\r\n", g_MCtlInfo.wTH_I_H,g_MCtlInfo.wTH_I_L);
				#endif
				DbgPrt(szTemp, g_hWndOWS);

			 LeaveCriticalSection(&g_CLockMCtlInfo);

			 break;

		case 0xC7:  // 回传WATERCOOL边界
			 EnterCriticalSection(&g_CLockMCtlInfo);

				g_MCtlInfo.wTH_WATERCOOLT_H = MAKEWORD( pFrame->Dat[0], pFrame->Dat[1] );//上限
				g_MCtlInfo.wTH_WATERCOOLT_L = MAKEWORD( pFrame->Dat[2], pFrame->Dat[3] );//下限
				#if DBG_LEVEL != 0
				    sprintf(szTemp,"Upload WaterCool T Threshold=%4.4x，%4.4x\r\n", g_MCtlInfo.wTH_WATERCOOLT_H,g_MCtlInfo.wTH_WATERCOOLT_L);
				#endif
				DbgPrt(szTemp, g_hWndOWS);

			 LeaveCriticalSection(&g_CLockMCtlInfo);

			 break;

		case 0xC8:  // 回传GANTRY温度边界
			 EnterCriticalSection(&g_CLockMCtlInfo);

				g_MCtlInfo.wTH_GANTRYT_H = MAKEWORD( pFrame->Dat[0], pFrame->Dat[1] );//上限
				g_MCtlInfo.wTH_GANTRYT_L = MAKEWORD( pFrame->Dat[2], pFrame->Dat[3] );//下限
				#if DBG_LEVEL != 0
				    sprintf(szTemp,"Upload GANTRY T Threshold=%4.4x，%4.4x\r\n", g_MCtlInfo.wTH_GANTRYT_H,g_MCtlInfo.wTH_GANTRYT_L);
				#endif
				DbgPrt(szTemp, g_hWndOWS);

			 LeaveCriticalSection(&g_CLockMCtlInfo);

			 break;

		case 0x04:  // 棒源消息
			 EnterCriticalSection(&g_CLockMCtlInfo);
				
				if(pFrame->CmdSub == 0x00){      //棒源正在伸出

					g_MCtlInfo.Rod_stat = outing;
					DbgPrt("棒源正在伸出\r\n", g_hWndOWS);

				}else if(pFrame->CmdSub == 0x01){ //棒源伸出到位

					g_MCtlInfo.Rod_stat = out;
					DbgPrt("棒源伸出到位\r\n", g_hWndOWS);

				}else if(pFrame->CmdSub == 0x02){ //棒源正在收回或旋转
					
					if( pFrame->Dat[0] == 0x00 ){ //正在收回
						g_MCtlInfo.Rod_stat = ining;
						DbgPrt("棒源正在收回\r\n", g_hWndOWS);
					}else if( pFrame->Dat[0] == 0x01){ //正在旋转
						g_MCtlInfo.Rod_stat = rolling;
						DbgPrt("棒源开始旋转\r\n", g_hWndOWS);
					}else
						ASSERT(FALSE);

				}else if(pFrame->CmdSub == 0x03){ //棒源收回到位

					g_MCtlInfo.Rod_stat = in;
					DbgPrt("棒源收回到位\r\n", g_hWndOWS);

				}else if(pFrame->CmdSub == 0x04){ //棒源停止
					g_MCtlInfo.Rod_stat = stop;
					DbgPrt("棒源停止\r\n", g_hWndOWS);

				}else{							  //错误命令
					ASSERT(FALSE);
				}
			 LeaveCriticalSection(&g_CLockMCtlInfo);
			 
			 break;

		case 0x0A:  // 回传棒源转速
			 EnterCriticalSection(&g_CLockMCtlInfo);
				 
				 g_MCtlInfo.wRod_rate = MAKEWORD( pFrame->Dat[0], pFrame->Dat[1] );
				 #if DBG_LEVEL != 0
					sprintf(szTemp, "Upload Rod Rate=%4.4x\r\n", g_MCtlInfo.wRod_rate);
				 #endif
				 DbgPrt(szTemp, g_hWndOWS);

			 LeaveCriticalSection(&g_CLockMCtlInfo);

			 break;

		case 0xE1:    //上传BED SPETA ROD开关量设置
			 EnterCriticalSection(&g_CLockMCtlInfo);
				
			    switch(pFrame->CmdSub)
				{
				case ADDR_ROD:
					 g_MCtlInfo.wRod_SwitchSet = MAKEWORD( pFrame->Dat[0], pFrame->Dat[1] );
					 #if DBG_LEVEL != 0
						sprintf(szTemp, "Upload ROD SwitchSet=%4.4x\r\n", g_MCtlInfo.wRod_SwitchSet);
					 #endif
					 break;

				case ADDR_BED:
					 g_MCtlInfo.wBed_SwitchSet = MAKEWORD( pFrame->Dat[0], pFrame->Dat[1] );
					 #if DBG_LEVEL != 0
						sprintf(szTemp, "Upload BED SwitchSet=%4.4x\r\n", g_MCtlInfo.wBed_SwitchSet);
					 #endif
					 break;

				case ADDR_SEPTA:
					 g_MCtlInfo.wSepta_SwitchSet = MAKEWORD( pFrame->Dat[0], pFrame->Dat[1] );
					 #if DBG_LEVEL != 0
						sprintf(szTemp, "Upload SEPTA SwitchSet=%4.4x\r\n", g_MCtlInfo.wSepta_SwitchSet);
					 #endif
					 break;

				default:
					 #if DBG_LEVEL != 0
						sprintf(szTemp, "Upload %2.2x SwitchSet=%4.4x\r\n", pFrame->CmdSub, g_MCtlInfo.wSepta_SwitchSet);
					 #endif
					 break;
				}
				DbgPrt(szTemp, g_hWndOWS);
			
			 LeaveCriticalSection(&g_CLockMCtlInfo);

			 break;

		case 0x0C:  // 回传床坐标
			 EnterCriticalSection(&g_CLockMCtlInfo);

			     g_MCtlInfo.wBed_posx = MAKEWORD(pFrame->Dat[0], pFrame->Dat[1]);
			     g_MCtlInfo.wBed_posy = MAKEWORD(pFrame->Dat[2], pFrame->Dat[3]);
				 #if DBG_LEVEL != 0
					sprintf(szTemp, "Upload Bed Pos，x=%4.4x(%d), y=%4.4x(%d)\r\n", g_MCtlInfo.wBed_posx,g_MCtlInfo.wBed_posx,g_MCtlInfo.wBed_posy,g_MCtlInfo.wBed_posy);
				 #endif
				 DbgPrt(szTemp, g_hWndOWS);

			 LeaveCriticalSection(&g_CLockMCtlInfo);
			 
			 break;

		case 0x14:  //回床床运动状态
			 EnterCriticalSection(&g_CLockMCtlInfo);
				
				if( pFrame->CmdSub == 0 )
				{
					g_MCtlInfo.Bed_stat = still;
					DbgPrt("Upload Bed Status:stop\r\n", g_hWndOWS);
				}
				else
				{
					g_MCtlInfo.Bed_stat = move;
					DbgPrt("Upload Bed Status:move\r\n", g_hWndOWS);
				}

			 LeaveCriticalSection(&g_CLockMCtlInfo);

			 break;

		case 0x0D:  // 回传床步长
			 EnterCriticalSection(&g_CLockMCtlInfo);
			     g_MCtlInfo.wBed_step = MAKEWORD(pFrame->Dat[0], pFrame->Dat[1]);
				 #if DBG_LEVEL!=0
				    sprintf(szTemp, "Upload Bed Step Len：0x%4.4x\r\n", g_MCtlInfo.wBed_step);
					DbgPrt(szTemp, g_hWndOWS);
				 #endif
			 LeaveCriticalSection(&g_CLockMCtlInfo);
			 
			 break;

		case 0x0F:  // 床伸出/回缩到极限通告
			 EnterCriticalSection(&g_CLockMCtlInfo);

				if( pFrame->CmdSub == 0x0 ){        //伸出到极限
					 g_MCtlInfo.Bed_stat = out_stop;
					 DbgPrt("Bed Out Limited\r\n", g_hWndOWS);
				}else{/*pFrame->CmdSub == 0x01*/     //收回到极限
					 g_MCtlInfo.Bed_stat = in_stop;
					 DbgPrt("Bed In Limited\r\n", g_hWndOWS);
				}

			 LeaveCriticalSection(&g_CLockMCtlInfo);

			 break;

		case 0xA1:    //上传床开关量设置
			 EnterCriticalSection(&g_CLockMCtlInfo);

				g_MCtlInfo.wBed_SwitchSet = MAKEWORD(pFrame->Dat[0], pFrame->Dat[1]);
				DbgPrt("Upload Bed SwitchSet\r\n", g_hWndOWS);

			 LeaveCriticalSection(&g_CLockMCtlInfo);

			 break;

		case 0xE7:   //床开始找零
			 EnterCriticalSection(&g_CLockMCtlInfo);

				g_MCtlInfo.Bed_stat = move;
				DbgPrt("Bed Begin Adjust To Zero\r\n", g_hWndOWS);

			 LeaveCriticalSection(&g_CLockMCtlInfo);
			 break;

		case 0xA8:   //床运动开始
			 EnterCriticalSection(&g_CLockMCtlInfo);

			 	g_MCtlInfo.Bed_stat = move;
				DbgPrt("Bed Moveing\r\n", g_hWndOWS);

			 LeaveCriticalSection(&g_CLockMCtlInfo);

			 break;

		case 0xA7:	 //床运动结束
			 EnterCriticalSection(&g_CLockMCtlInfo);

				g_MCtlInfo.Bed_stat = still;
				g_MCtlInfo.wBed_posx = MAKEWORD(pFrame->Dat[0], pFrame->Dat[1]);
				g_MCtlInfo.wBed_posy = MAKEWORD(pFrame->Dat[2], pFrame->Dat[3]);
			    #if DBG_LEVEL != 0
				    sprintf(szTemp, "Bed Moving Stop,x=%d, y=%d\r\n", g_MCtlInfo.wBed_posx, g_MCtlInfo.wBed_posy);
		        #endif
				DbgPrt(szTemp, g_hWndOWS);

			 LeaveCriticalSection(&g_CLockMCtlInfo);

			 break;

		case 0x05:  //设置床坐标系统应答
			 EnterCriticalSection(&g_CLockMCtlInfo);
				ASSERT(pFrame->CmdSub==0x00 || pFrame->CmdSub==0x01);
				if(pFrame->CmdSub == 0x00)
				{
					g_MCtlInfo.wBed_bRelateCoord = FALSE;
					#if DBG_LEVEL != 0
						sprintf(szTemp, "Bed Pos System:Absulte\r\n");
					#endif
				}
				else
				{
					g_MCtlInfo.wBed_bRelateCoord = TRUE;
					#if DBG_LEVEL != 0
						sprintf(szTemp, "Bed Pos system: Relate\r\n");
					#endif
				}
				DbgPrt(szTemp, g_hWndOWS);
			 LeaveCriticalSection(&g_CLockMCtlInfo);

			 break;

		case 0x06:  //设置床步长应答
			 EnterCriticalSection(&g_CLockMCtlInfo);
				g_MCtlInfo.wBed_step = pFrame->Dat[0];
				#if DBG_LEVEL != 0
					sprintf(szTemp, "Bed Step Len：%2.2x\r\n", g_MCtlInfo.wBed_step);
				#endif
				DbgPrt(szTemp, g_hWndOWS);
			 LeaveCriticalSection(&g_CLockMCtlInfo);

			 break;

		case 0x0E:  // 床电机状态通告
			 DbgPrt("Upload Bed Meshine StatueXXXXXXXXXX\r\n", g_hWndOWS);
			 break;

		case 0x01:  // septa消息
			 EnterCriticalSection(&g_CLockMCtlInfo);
				if( pFrame->CmdSub == 0x00 ){      //正在伸出

					 g_MCtlInfo.septa_stat = outing;
					 DbgPrt("SEPTA 正在伸出\r\n", g_hWndOWS);

				}else if( pFrame->CmdSub == 0x01 ){ //伸出到位

					 g_MCtlInfo.septa_stat = out;
					 DbgPrt("SEPTA out on Position伸出到位\r\n", g_hWndOWS);

				}else if( pFrame->CmdSub == 0x02 ){ //正在缩回

					 g_MCtlInfo.septa_stat = ining;
					 DbgPrt("SEPTA 正在缩回\r\n", g_hWndOWS);

				}else if( pFrame->CmdSub == 0x03 ){ //缩回到位

					 g_MCtlInfo.septa_stat = in;
					 DbgPrt("SEPTA in On Position\r\n", g_hWndOWS);

				}else if( pFrame->CmdSub == 0x04 ){ //正在停止

					 g_MCtlInfo.septa_stat = stop;
					 DbgPrt("SEPTA stop\r\n", g_hWndOWS);

				}else{							   //错误命令
					 ASSERT(FALSE);               
				}
			 LeaveCriticalSection(&g_CLockMCtlInfo);

			 break;

		case 0x71:    //上传SEPTA开关量设置
			 EnterCriticalSection(&g_CLockMCtlInfo);
				
				g_MCtlInfo.wSepta_SwitchSet = MAKEWORD(pFrame->Dat[0], pFrame->Dat[1]);
				DbgPrt("Upload SEPTA SwitchSet\r\n", g_hWndOWS);

			 LeaveCriticalSection(&g_CLockMCtlInfo);

			 break;

		case 0xB4:  // 板级查询超时报警
			 EnterCriticalSection(&g_CLockMCtlInfo);
				switch(pFrame->CmdSub)
				{
				case 0:
					 DbgPrt("板级查询正常\r\n", g_hWndOWS);
					 break;
				case ADDR_GANTRY:
					 DbgPrt("GANTRY板级查询错误\r\n", g_hWndOWS);
					 break;
				case ADDR_BED:
					 DbgPrt("BED板级查询错误\r\n", g_hWndOWS);
					 break;
				case ADDR_ROD:
					 DbgPrt("ROD板级查询错误\r\n", g_hWndOWS);
					 break;
				case ADDR_SEPTA:
					 DbgPrt("SEPTA板级查询错误\r\n", g_hWndOWS);
					 break;
				default:
					 DbgPrt("板级查询错误,板地址未知\r\n", g_hWndOWS);
					 break;					 
				}
			 LeaveCriticalSection(&g_CLockMCtlInfo);
			 break;

		case 0xEE:  // 紧急停止通告
			 EnterCriticalSection(&g_CLockMCtlInfo);
				 if(pFrame->CmdSub == 0x01)
				 {
					 g_MCtlInfo.alarm_ESTOP = alarm;
					 DbgPrt("紧急停止\r\n", g_hWndOWS);
				 }
				 else
				 {
					 g_MCtlInfo.alarm_ESTOP = normal;
					 DbgPrt("紧急停止复位\r\n", g_hWndOWS);
				 }

			 LeaveCriticalSection(&g_CLockMCtlInfo);

			 break;

		case 0xB1:    //报警
			 EnterCriticalSection(&g_CLockMCtlInfo);
				switch(pFrame->CmdSub)
				{
				case ADDR_GANTRY:
					 g_MCtlInfo.alarm_Gantry = alarm;
					 g_MCtlInfo.ac_Gantry.code8 = pFrame->Dat[0];
					 #if DBG_LEVEL != 0
						sprintf(szTemp, "GANTRY报警：%2.2X\r\n", pFrame->Dat[0]);
					 #endif
					 break;
				case ADDR_ROD:
					 g_MCtlInfo.alarm_Rod = alarm;
					 g_MCtlInfo.ac_Rod.code8 = pFrame->Dat[0];
					 #if DBG_LEVEL != 0
						sprintf(szTemp, "ROD报警：%2.2X\r\n", pFrame->Dat[0]);
					 #endif
					 break;
				case ADDR_BED:
					 g_MCtlInfo.alarm_Bed = alarm;
					 g_MCtlInfo.ac_Bed.code8 = pFrame->Dat[0];
					 #if DBG_LEVEL != 0
						sprintf(szTemp, "Bed报警：%2.2X\r\n", pFrame->Dat[0]);
					 #endif
					 break;
				case ADDR_SEPTA:
					 g_MCtlInfo.alarm_Septa = alarm;
					 g_MCtlInfo.ac_Septa.code8 = pFrame->Dat[0];
					 #if DBG_LEVEL != 0
						sprintf(szTemp, "SEPTA报警：%2.2X\r\n", pFrame->Dat[0]);
					 #endif
					 break;
				default:
					 #if DBG_LEVEL != 0
						sprintf(szTemp, "%2.2X报警：%2.2X\r\n", pFrame->CmdSub, pFrame->Dat[0]);
					 #endif
					 break;
				}

				DbgPrt(szTemp, g_hWndOWS);

			 LeaveCriticalSection(&g_CLockMCtlInfo);
			 break;

		default:
			 TRACE("Receive UNKONW SIO Dat\r\n");
			 break;
	}//   end switch pFrame->CmdMain

	UpdateDisplay();

	return TRUE;
}

COWSSIO::COWSSIO()
{
	InitializeCriticalSection(&g_CLockMCtlInfo);

	g_MCtlInfo.alarm_ESTOP = normal;
	g_MCtlInfo.alarm_Gantry = normal;
	g_MCtlInfo.alarm_Bed    = normal;
	g_MCtlInfo.alarm_Rod    = normal;
	g_MCtlInfo.alarm_Septa  = normal;
	g_MCtlInfo.ac_Gantry.code8 = 0;
	g_MCtlInfo.ac_Bed.code8    = 0;
	g_MCtlInfo.ac_Rod.code8    = 0;
	g_MCtlInfo.ac_Septa.code8  = 0;
	
	g_MCtlInfo.Rod_stat = unknow;
	g_MCtlInfo.wRod_SwitchSet =0;
	g_MCtlInfo.wRod_rate = 0;

	g_MCtlInfo.septa_stat = unknow;
	g_MCtlInfo.wSepta_SwitchSet = 0;

	g_MCtlInfo.Bed_stat  = still;
	g_MCtlInfo.wBed_SwitchSet = 0;
	g_MCtlInfo.wBed_posx  = -1;
	g_MCtlInfo.wBed_posy  = -1;
	g_MCtlInfo.wBed_step = -1;
	g_MCtlInfo.wBed_bRelateCoord = 1;
	
	g_MCtlInfo.wT_GANTRY    = -1;
	g_MCtlInfo.wT_WATERCOOL = -1;
	g_MCtlInfo.wT_Bed       = -1;
	g_MCtlInfo.wT_Rod       = -1;
	g_MCtlInfo.wT_Septa     = -1;
	g_MCtlInfo.wTH_GANTRYT_H = -1;
	g_MCtlInfo.wTH_GANTRYT_L = -1;
	g_MCtlInfo.wTH_I_H = -1;
	g_MCtlInfo.wTH_I_L = -1;
	g_MCtlInfo.wTH_V_H = -1;
	g_MCtlInfo.wTH_V_L = -1;
	g_MCtlInfo.wTH_WATERCOOLT_H = -1;
	g_MCtlInfo.wTH_WATERCOOLT_L = -1;
	g_MCtlInfo.wV_1 = -1;
	g_MCtlInfo.wV_2 = -1;
	g_MCtlInfo.wI_1 = -1;
	g_MCtlInfo.wI_2 = -1;
	
	g_MCtlInfo.bHVOn = FALSE;	
	g_MCtlInfo.bLaserOn = FALSE;    
	g_MCtlInfo.bMainPowerOn = FALSE;
}

COWSSIO::~COWSSIO()
{
	LeaveCriticalSection(&g_CLockMCtlInfo);

	DeleteCriticalSection(&g_CLockMCtlInfo);
}

//查询GANTRY温度
BOOL COWSSIO::QueryTGantry()
{
	 m_WFrame8.CheckSum = 0;
	 m_WFrame8.tAddr   = ADDR_GANTRY;   m_WFrame8.CheckSum ^= m_WFrame8.tAddr;
	 m_WFrame8.sAddr   = ADDR_ACS;	    m_WFrame8.CheckSum ^= m_WFrame8.sAddr;
	 m_WFrame8.len     = 0x08;          m_WFrame8.CheckSum ^= m_WFrame8.len;
	 m_WFrame8.attr    = 0;
	 m_WFrame8.CmdMain = 0xB3;		    m_WFrame8.CheckSum ^= m_WFrame8.CmdMain;
	 m_WFrame8.CmdSub  = 0x00;		    m_WFrame8.CheckSum ^= m_WFrame8.CmdSub;
	 m_WFrame8.eof     = 0x7F;

	OWS_HEAD head;
	head.dwDatLen = sizeof(head) + 1 + sizeof(m_WFrame8);
	head.Cmd = PET_TESTREQ;
	head.cFlag[0] = 0x7e;
	head.cFlag[1] = 0x7e;
	head.cFlag[2] = 0x7e;

	BYTE testObj = 41;
	((CSimOWSDlg*)AfxGetMainWnd())->m_ServerSock.SendDatToClient(&head, sizeof(head));
	((CSimOWSDlg*)AfxGetMainWnd())->m_ServerSock.SendDatToClient(&testObj, 1);
	((CSimOWSDlg*)AfxGetMainWnd())->m_ServerSock.SendDatToClient(&m_WFrame8,8);

	return TRUE;
}

//查询棒源温度
BOOL COWSSIO::QueryTRod()
{
	 m_WFrame9.CheckSum = 0;
	 m_WFrame9.tAddr   = ADDR_ROD;      m_WFrame9.CheckSum ^= m_WFrame9.tAddr;
	 m_WFrame9.sAddr   = ADDR_ACS;	    m_WFrame9.CheckSum ^= m_WFrame9.sAddr;
	 m_WFrame9.len     = 0x09;          m_WFrame9.CheckSum ^= m_WFrame9.len;
	 m_WFrame9.attr    = 0;
	 m_WFrame9.CmdMain = 0xB3;		    m_WFrame9.CheckSum ^= m_WFrame9.CmdMain;
	 m_WFrame9.CmdSub  = 0x00;		    m_WFrame9.CheckSum ^= m_WFrame9.CmdSub;
	 m_WFrame9.Dat     = 0x00; 			m_WFrame9.CheckSum ^= m_WFrame9.Dat;
	 m_WFrame9.eof     = 0x7F;

	OWS_HEAD head;
	head.dwDatLen = sizeof(head) + 1 + sizeof(m_WFrame9);
	head.Cmd = PET_TESTREQ;
	head.cFlag[0] = 0x7e;
	head.cFlag[1] = 0x7e;
	head.cFlag[2] = 0x7e;

	BYTE testObj = 41;
	((CSimOWSDlg*)AfxGetMainWnd())->m_ServerSock.SendDatToClient(&head, sizeof(head));
	((CSimOWSDlg*)AfxGetMainWnd())->m_ServerSock.SendDatToClient(&testObj, 1);
	((CSimOWSDlg*)AfxGetMainWnd())->m_ServerSock.SendDatToClient(&m_WFrame9,9);

	return TRUE;
}

//查询SEPTA温度
BOOL COWSSIO::QueryTSepta()
{
	 m_WFrame9.CheckSum = 0;
	 m_WFrame9.tAddr   = ADDR_SEPTA;      m_WFrame9.CheckSum ^= m_WFrame9.tAddr;
	 m_WFrame9.sAddr   = ADDR_ACS;	    m_WFrame9.CheckSum ^= m_WFrame9.sAddr;
	 m_WFrame9.len     = 0x09;          m_WFrame9.CheckSum ^= m_WFrame9.len;
	 m_WFrame9.attr    = 0;
	 m_WFrame9.CmdMain = 0xB3;		    m_WFrame9.CheckSum ^= m_WFrame9.CmdMain;
	 m_WFrame9.CmdSub  = 0x00;		    m_WFrame9.CheckSum ^= m_WFrame9.CmdSub;
	 m_WFrame9.Dat     = 0x00; 			m_WFrame9.CheckSum ^= m_WFrame9.Dat;
	 m_WFrame9.eof     = 0x7F;

	OWS_HEAD head;
	head.dwDatLen = sizeof(head) + 1 + sizeof(m_WFrame9);
	head.Cmd = PET_TESTREQ;
	head.cFlag[0] = 0x7e;
	head.cFlag[1] = 0x7e;
	head.cFlag[2] = 0x7e;

	BYTE testObj = 41;
	((CSimOWSDlg*)AfxGetMainWnd())->m_ServerSock.SendDatToClient(&head, sizeof(head));
	((CSimOWSDlg*)AfxGetMainWnd())->m_ServerSock.SendDatToClient(&testObj, 1);
	((CSimOWSDlg*)AfxGetMainWnd())->m_ServerSock.SendDatToClient(&m_WFrame9,9);
	 return TRUE;
}

//查询床温度
BOOL COWSSIO::QueryTBed()
{
	 m_WFrame9.CheckSum = 0;
	 m_WFrame9.tAddr   = ADDR_BED;      m_WFrame9.CheckSum ^= m_WFrame9.tAddr;
	 m_WFrame9.sAddr   = ADDR_ACS;	    m_WFrame9.CheckSum ^= m_WFrame9.sAddr;
	 m_WFrame9.len     = 0x09;          m_WFrame9.CheckSum ^= m_WFrame9.len;
	 m_WFrame9.attr    = 0;
	 m_WFrame9.CmdMain = 0xB3;		    m_WFrame9.CheckSum ^= m_WFrame9.CmdMain;
	 m_WFrame9.CmdSub  = 0x00;		    m_WFrame9.CheckSum ^= m_WFrame9.CmdSub;
	 m_WFrame9.Dat     = 0x00; 			m_WFrame9.CheckSum ^= m_WFrame9.Dat;
	 m_WFrame9.eof     = 0x7F;

	OWS_HEAD head;
	head.dwDatLen = sizeof(head) + 1 + sizeof(m_WFrame9);
	head.Cmd = PET_TESTREQ;
	head.cFlag[0] = 0x7e;
	head.cFlag[1] = 0x7e;
	head.cFlag[2] = 0x7e;

	BYTE testObj = 41;
	((CSimOWSDlg*)AfxGetMainWnd())->m_ServerSock.SendDatToClient(&head, sizeof(head));
	((CSimOWSDlg*)AfxGetMainWnd())->m_ServerSock.SendDatToClient(&testObj, 1);
	((CSimOWSDlg*)AfxGetMainWnd())->m_ServerSock.SendDatToClient(&m_WFrame9,9);
	 return TRUE;
}

//查询水冷机温度
BOOL COWSSIO::QueryTWaterCool()
{
	 m_WFrame8.CheckSum = 0;
	 m_WFrame8.tAddr   = ADDR_GANTRY;   m_WFrame8.CheckSum ^= m_WFrame8.tAddr;
	 m_WFrame8.sAddr   = ADDR_ACS;	    m_WFrame8.CheckSum ^= m_WFrame8.sAddr;
	 m_WFrame8.len     = 0x08;          m_WFrame8.CheckSum ^= m_WFrame8.len;
	 m_WFrame8.attr    = 0;
	 m_WFrame8.CmdMain = 0xC0;		    m_WFrame8.CheckSum ^= m_WFrame8.CmdMain;
	 m_WFrame8.CmdSub  = 0x20;		    m_WFrame8.CheckSum ^= m_WFrame8.CmdSub;
	 m_WFrame8.eof      = 0x7F;

	OWS_HEAD head;
	head.dwDatLen = sizeof(head) + 1 + sizeof(m_WFrame8);
	head.Cmd = PET_TESTREQ;
	head.cFlag[0] = 0x7e;
	head.cFlag[1] = 0x7e;
	head.cFlag[2] = 0x7e;

	BYTE testObj = 41;
	((CSimOWSDlg*)AfxGetMainWnd())->m_ServerSock.SendDatToClient(&head, sizeof(head));
	((CSimOWSDlg*)AfxGetMainWnd())->m_ServerSock.SendDatToClient(&testObj, 1);
	((CSimOWSDlg*)AfxGetMainWnd())->m_ServerSock.SendDatToClient(&m_WFrame8,8);
	 return TRUE;
}

BOOL COWSSIO::QueryHVStatus()
{
	 m_WFrame8.CheckSum = 0;
	 m_WFrame8.tAddr   = ADDR_GANTRY;   m_WFrame8.CheckSum ^= m_WFrame8.tAddr;
	 m_WFrame8.sAddr   = ADDR_ACS;	    m_WFrame8.CheckSum ^= m_WFrame8.sAddr;
	 m_WFrame8.len     = 0x08;          m_WFrame8.CheckSum ^= m_WFrame8.len;
	 m_WFrame8.attr    = 0;
	 m_WFrame8.CmdMain = 0xC1; 		    m_WFrame8.CheckSum ^= m_WFrame8.CmdMain;
	 m_WFrame8.CmdSub  = 0x00;			m_WFrame8.CheckSum ^= m_WFrame8.CmdSub;
	 m_WFrame8.eof     = 0x7F;

	OWS_HEAD head;
	head.dwDatLen = sizeof(head) + 1 + sizeof(m_WFrame8);
	head.Cmd = PET_TESTREQ;
	head.cFlag[0] = 0x7e;
	head.cFlag[1] = 0x7e;
	head.cFlag[2] = 0x7e;

	BYTE testObj = 41;
	((CSimOWSDlg*)AfxGetMainWnd())->m_ServerSock.SendDatToClient(&head, sizeof(head));
	((CSimOWSDlg*)AfxGetMainWnd())->m_ServerSock.SendDatToClient(&testObj, 1);
	((CSimOWSDlg*)AfxGetMainWnd())->m_ServerSock.SendDatToClient(&m_WFrame8,8);
	 return TRUE;
}

//控制高压通、断
BOOL COWSSIO::Control_HVON(BOOL bON)
{
	 m_WFrame8.CheckSum = 0;
	 m_WFrame8.tAddr   = ADDR_GANTRY;   m_WFrame8.CheckSum ^= m_WFrame8.tAddr;
	 m_WFrame8.sAddr   = ADDR_ACS;	   m_WFrame8.CheckSum ^= m_WFrame8.sAddr;
	 m_WFrame8.len     = 0x08;          m_WFrame8.CheckSum ^= m_WFrame8.len;
	 m_WFrame8.attr    = 0;
	 m_WFrame8.CmdMain =	0xC2;		   m_WFrame8.CheckSum ^= m_WFrame8.CmdMain;
	 if( bON )
		 m_WFrame8.CmdSub = 0x66;		
	 else
		 m_WFrame8.CmdSub = 0x33;
	 m_WFrame8.CheckSum ^= m_WFrame8.CmdSub;
	 m_WFrame8.eof      = 0x7F;

	OWS_HEAD head;
	head.dwDatLen = sizeof(head) + 1 + sizeof(m_WFrame8);
	head.Cmd = PET_TESTREQ;
	head.cFlag[0] = 0x7e;
	head.cFlag[1] = 0x7e;
	head.cFlag[2] = 0x7e;

	BYTE testObj = 41;
	((CSimOWSDlg*)AfxGetMainWnd())->m_ServerSock.SendDatToClient(&head, sizeof(head));
	((CSimOWSDlg*)AfxGetMainWnd())->m_ServerSock.SendDatToClient(&testObj, 1);
	((CSimOWSDlg*)AfxGetMainWnd())->m_ServerSock.SendDatToClient(&m_WFrame8,8);
	 return TRUE;
}

//控制激光灯通、断
BOOL COWSSIO::Control_LaserON(BOOL bON)
{
	 m_WFrame8.CheckSum = 0;
	 m_WFrame8.tAddr   = ADDR_GANTRY;   m_WFrame8.CheckSum ^= m_WFrame8.tAddr;
	 m_WFrame8.sAddr   = ADDR_ACS;	    m_WFrame8.CheckSum ^= m_WFrame8.sAddr;
	 m_WFrame8.len     = 0x08;          m_WFrame8.CheckSum ^= m_WFrame8.len;
	 m_WFrame8.attr    = 0;
	 m_WFrame8.CmdMain = 0xC3;		    m_WFrame8.CheckSum ^= m_WFrame8.CmdMain;
	 if( bON )
		 m_WFrame8.CmdSub = 0x66;		
	 else
		 m_WFrame8.CmdSub = 0x33;
	 m_WFrame8.CheckSum ^= m_WFrame8.CmdSub;
	 m_WFrame8.eof      = 0x7F;

	OWS_HEAD head;
	head.dwDatLen = sizeof(head) + 1 + sizeof(m_WFrame8);
	head.Cmd = PET_TESTREQ;
	head.cFlag[0] = 0x7e;
	head.cFlag[1] = 0x7e;
	head.cFlag[2] = 0x7e;

	BYTE testObj = 41;
	((CSimOWSDlg*)AfxGetMainWnd())->m_ServerSock.SendDatToClient(&head, sizeof(head));
	((CSimOWSDlg*)AfxGetMainWnd())->m_ServerSock.SendDatToClient(&testObj, 1);
	((CSimOWSDlg*)AfxGetMainWnd())->m_ServerSock.SendDatToClient(&m_WFrame8,8);
	 return TRUE;
}

//控制前端主电源通、断
BOOL COWSSIO::Control_MainPowerON(BOOL bON)
{
	 m_WFrame8.CheckSum = 0;
	 m_WFrame8.tAddr   = ADDR_GANTRY;   m_WFrame8.CheckSum ^= m_WFrame8.tAddr;
	 m_WFrame8.sAddr   = ADDR_ACS;	    m_WFrame8.CheckSum ^= m_WFrame8.sAddr;
	 m_WFrame8.len     = 0x08;          m_WFrame8.CheckSum ^= m_WFrame8.len;
	 m_WFrame8.attr    = 0;
	 m_WFrame8.CmdMain =	0xC4;		m_WFrame8.CheckSum ^= m_WFrame8.CmdMain;
	 if( bON )
		 m_WFrame8.CmdSub = 0x66;		
	 else
		 m_WFrame8.CmdSub = 0x33;
	 m_WFrame8.CheckSum ^= m_WFrame8.CmdSub;
	 m_WFrame8.eof      = 0x7F;

	OWS_HEAD head;
	head.dwDatLen = sizeof(head) + 1 + sizeof(m_WFrame8);
	head.Cmd = PET_TESTREQ;
	head.cFlag[0] = 0x7e;
	head.cFlag[1] = 0x7e;
	head.cFlag[2] = 0x7e;

	BYTE testObj = 41;
	((CSimOWSDlg*)AfxGetMainWnd())->m_ServerSock.SendDatToClient(&head, sizeof(head));
	((CSimOWSDlg*)AfxGetMainWnd())->m_ServerSock.SendDatToClient(&testObj, 1);
	((CSimOWSDlg*)AfxGetMainWnd())->m_ServerSock.SendDatToClient(&m_WFrame8,8);
	 return TRUE;
}

//查询高压上下限
BOOL COWSSIO::QueryThreshold_HV()
{
	 m_WFrame8.CheckSum = 0;
	 m_WFrame8.tAddr   = ADDR_GANTRY;   m_WFrame8.CheckSum ^= m_WFrame8.tAddr;
	 m_WFrame8.sAddr   = ADDR_ACS;	   m_WFrame8.CheckSum ^= m_WFrame8.sAddr;
	 m_WFrame8.len     = 0x08;          m_WFrame8.CheckSum ^= m_WFrame8.len;
	 m_WFrame8.attr    = 0;
	 m_WFrame8.CmdMain = 0xC5;		   m_WFrame8.CheckSum ^= m_WFrame8.CmdMain;
	 m_WFrame8.CmdSub  = 0x10;		   m_WFrame8.CheckSum ^= m_WFrame8.CmdSub;
	 m_WFrame8.eof     = 0x7F;

	OWS_HEAD head;
	head.dwDatLen = sizeof(head) + 1 + sizeof(m_WFrame8);
	head.Cmd = PET_TESTREQ;
	head.cFlag[0] = 0x7e;
	head.cFlag[1] = 0x7e;
	head.cFlag[2] = 0x7e;

	BYTE testObj = 41;
	((CSimOWSDlg*)AfxGetMainWnd())->m_ServerSock.SendDatToClient(&head, sizeof(head));
	((CSimOWSDlg*)AfxGetMainWnd())->m_ServerSock.SendDatToClient(&testObj, 1);
	((CSimOWSDlg*)AfxGetMainWnd())->m_ServerSock.SendDatToClient(&m_WFrame8,8);
	 return TRUE;
}

//设置高压上下限
BOOL COWSSIO::SetThreshold_HV(WORD upper, WORD lower)
{
	 m_WFrame12.CheckSum = 0;
	 m_WFrame12.tAddr   = ADDR_GANTRY;   m_WFrame12.CheckSum ^= m_WFrame12.tAddr;
	 m_WFrame12.sAddr   = ADDR_ACS;	     m_WFrame12.CheckSum ^= m_WFrame12.sAddr;
	 m_WFrame12.len     = 0x0c;          m_WFrame12.CheckSum ^= m_WFrame12.len;
	 m_WFrame12.attr    = 0;
	 m_WFrame12.CmdMain = 0xC5;		     m_WFrame12.CheckSum ^= m_WFrame12.CmdMain;
	 m_WFrame12.CmdSub  = 0x20;	         m_WFrame12.CheckSum ^= m_WFrame12.CmdSub;
	 m_WFrame12.Dat[0]  = LOBYTE(upper); m_WFrame12.CheckSum ^= m_WFrame12.Dat[0];
	 m_WFrame12.Dat[1]  = HIBYTE(upper); m_WFrame12.CheckSum ^= m_WFrame12.Dat[1];
	 m_WFrame12.Dat[2]  = LOBYTE(lower); m_WFrame12.CheckSum ^= m_WFrame12.Dat[2];
	 m_WFrame12.Dat[3]  = HIBYTE(lower); m_WFrame12.CheckSum ^= m_WFrame12.Dat[3];
	 m_WFrame12.eof      = 0x7F;

	OWS_HEAD head;
	head.dwDatLen = sizeof(head) + 1 + sizeof(m_WFrame12);
	head.Cmd = PET_TESTREQ;
	head.cFlag[0] = 0x7e;
	head.cFlag[1] = 0x7e;
	head.cFlag[2] = 0x7e;

	BYTE testObj = 41;
	((CSimOWSDlg*)AfxGetMainWnd())->m_ServerSock.SendDatToClient(&head, sizeof(head));
	((CSimOWSDlg*)AfxGetMainWnd())->m_ServerSock.SendDatToClient(&testObj, 1);
	((CSimOWSDlg*)AfxGetMainWnd())->m_ServerSock.SendDatToClient(&m_WFrame12,12);
	 return TRUE;
}

//查询电流上限
BOOL COWSSIO::QueryThreshold_I()
{
	 m_WFrame8.CheckSum = 0;
	 m_WFrame8.tAddr   = ADDR_GANTRY;   m_WFrame8.CheckSum ^= m_WFrame8.tAddr;
	 m_WFrame8.sAddr   = ADDR_ACS;	    m_WFrame8.CheckSum ^= m_WFrame8.sAddr;
	 m_WFrame8.len     = 0x08;          m_WFrame8.CheckSum ^= m_WFrame8.len;
	 m_WFrame8.attr    = 0;
	 m_WFrame8.CmdMain = 0xC6;		    m_WFrame8.CheckSum ^= m_WFrame8.CmdMain;
	 m_WFrame8.CmdSub  = 0x10;		    m_WFrame8.CheckSum ^= m_WFrame8.CmdSub;
	 m_WFrame8.eof     = 0x7F;

	OWS_HEAD head;
	head.dwDatLen = sizeof(head) + 1 + sizeof(m_WFrame8);
	head.Cmd = PET_TESTREQ;
	head.cFlag[0] = 0x7e;
	head.cFlag[1] = 0x7e;
	head.cFlag[2] = 0x7e;

	BYTE testObj = 41;
	((CSimOWSDlg*)AfxGetMainWnd())->m_ServerSock.SendDatToClient(&head, sizeof(head));
	((CSimOWSDlg*)AfxGetMainWnd())->m_ServerSock.SendDatToClient(&testObj, 1);
	((CSimOWSDlg*)AfxGetMainWnd())->m_ServerSock.SendDatToClient(&m_WFrame8,8);
	 return TRUE;
}

//设置电流上下限
BOOL COWSSIO::SetThreshold_I(WORD upper, WORD lower)
{
	 m_WFrame12.CheckSum = 0;
	 m_WFrame12.tAddr   = ADDR_GANTRY;   m_WFrame12.CheckSum ^= m_WFrame12.tAddr;
	 m_WFrame12.sAddr   = ADDR_ACS;	     m_WFrame12.CheckSum ^= m_WFrame12.sAddr;
	 m_WFrame12.len     = 0x0c;          m_WFrame12.CheckSum ^= m_WFrame12.len;
	 m_WFrame12.attr    = 0;
	 m_WFrame12.CmdMain = 0xC6;		     m_WFrame12.CheckSum ^= m_WFrame12.CmdMain;
	 m_WFrame12.CmdSub  = 0x20;	         m_WFrame12.CheckSum ^= m_WFrame12.CmdSub;
	 m_WFrame12.Dat[0]  = LOBYTE(upper); m_WFrame12.CheckSum ^= m_WFrame12.Dat[0];
	 m_WFrame12.Dat[1]  = HIBYTE(lower); m_WFrame12.CheckSum ^= m_WFrame12.Dat[1];
	 m_WFrame12.Dat[2]  = LOBYTE(upper); m_WFrame12.CheckSum ^= m_WFrame12.Dat[2];
	 m_WFrame12.Dat[3]  = HIBYTE(lower); m_WFrame12.CheckSum ^= m_WFrame12.Dat[3];
	 m_WFrame12.eof      = 0x7F;

	OWS_HEAD head;
	head.dwDatLen = sizeof(head) + 1 + sizeof(m_WFrame12);
	head.Cmd = PET_TESTREQ;
	head.cFlag[0] = 0x7e;
	head.cFlag[1] = 0x7e;
	head.cFlag[2] = 0x7e;

	BYTE testObj = 41;
	((CSimOWSDlg*)AfxGetMainWnd())->m_ServerSock.SendDatToClient(&head, sizeof(head));
	((CSimOWSDlg*)AfxGetMainWnd())->m_ServerSock.SendDatToClient(&testObj, 1);
	((CSimOWSDlg*)AfxGetMainWnd())->m_ServerSock.SendDatToClient(&m_WFrame12,12);
	 return TRUE;
}

//查询水冷机温度上下限
BOOL COWSSIO::QueryThreshold_TW()
{
	 m_WFrame8.CheckSum = 0;
	 m_WFrame8.tAddr   = ADDR_GANTRY;  m_WFrame8.CheckSum ^= m_WFrame8.tAddr;
	 m_WFrame8.sAddr   = ADDR_ACS;	   m_WFrame8.CheckSum ^= m_WFrame8.sAddr;
	 m_WFrame8.len     = 0x08;         m_WFrame8.CheckSum ^= m_WFrame8.len;
	 m_WFrame8.attr    = 0;
	 m_WFrame8.CmdMain = 0xC7;		   m_WFrame8.CheckSum ^= m_WFrame8.CmdMain;
	 m_WFrame8.CmdSub  = 0x10;		   m_WFrame8.CheckSum ^= m_WFrame8.CmdSub;
	 m_WFrame8.eof     = 0x7F;

	OWS_HEAD head;
	head.dwDatLen = sizeof(head) + 1 + sizeof(m_WFrame8);
	head.Cmd = PET_TESTREQ;
	head.cFlag[0] = 0x7e;
	head.cFlag[1] = 0x7e;
	head.cFlag[2] = 0x7e;

	BYTE testObj = 41;
	((CSimOWSDlg*)AfxGetMainWnd())->m_ServerSock.SendDatToClient(&head, sizeof(head));
	((CSimOWSDlg*)AfxGetMainWnd())->m_ServerSock.SendDatToClient(&testObj, 1);
	((CSimOWSDlg*)AfxGetMainWnd())->m_ServerSock.SendDatToClient(&m_WFrame8,8);
	 return TRUE;
}

//设置水冷机温度上下限
BOOL COWSSIO::SetThreshold_TW(WORD upper, WORD lower)
{
	 m_WFrame12.CheckSum = 0;
	 m_WFrame12.tAddr   = ADDR_GANTRY;   m_WFrame12.CheckSum ^= m_WFrame12.tAddr;
	 m_WFrame12.sAddr   = ADDR_ACS;	     m_WFrame12.CheckSum ^= m_WFrame12.sAddr;
	 m_WFrame12.len     = 0x0c;          m_WFrame12.CheckSum ^= m_WFrame12.len;
	 m_WFrame12.attr    = 0;
	 m_WFrame12.CmdMain = 0xC7;		     m_WFrame12.CheckSum ^= m_WFrame12.CmdMain;
	 m_WFrame12.CmdSub  = 0x20;	         m_WFrame12.CheckSum ^= m_WFrame12.CmdSub;
	 m_WFrame12.Dat[0]  = LOBYTE(upper); m_WFrame12.CheckSum ^= m_WFrame12.Dat[0];
	 m_WFrame12.Dat[1]  = HIBYTE(upper); m_WFrame12.CheckSum ^= m_WFrame12.Dat[1];
	 m_WFrame12.Dat[2]  = LOBYTE(lower); m_WFrame12.CheckSum ^= m_WFrame12.Dat[2];
	 m_WFrame12.Dat[3]  = HIBYTE(lower); m_WFrame12.CheckSum ^= m_WFrame12.Dat[3];
	 m_WFrame12.eof      = 0x7F;

	OWS_HEAD head;
	head.dwDatLen = sizeof(head) + 1 + sizeof(m_WFrame12);
	head.Cmd = PET_TESTREQ;
	head.cFlag[0] = 0x7e;
	head.cFlag[1] = 0x7e;
	head.cFlag[2] = 0x7e;

	BYTE testObj = 41;
	((CSimOWSDlg*)AfxGetMainWnd())->m_ServerSock.SendDatToClient(&head, sizeof(head));
	((CSimOWSDlg*)AfxGetMainWnd())->m_ServerSock.SendDatToClient(&testObj, 1);
	((CSimOWSDlg*)AfxGetMainWnd())->m_ServerSock.SendDatToClient(&m_WFrame12,12);
	 return TRUE;
}

//查询GANTRY温度上下限
BOOL COWSSIO::QueryThreshold_TG()
{
	 m_WFrame8.CheckSum = 0;
	 m_WFrame8.tAddr   = ADDR_GANTRY;  m_WFrame8.CheckSum ^= m_WFrame8.tAddr;
	 m_WFrame8.sAddr   = ADDR_ACS;	   m_WFrame8.CheckSum ^= m_WFrame8.sAddr;
	 m_WFrame8.len     = 0x08;         m_WFrame8.CheckSum ^= m_WFrame8.len;
	 m_WFrame8.attr    = 0;
	 m_WFrame8.CmdMain = 0xC8;		   m_WFrame8.CheckSum ^= m_WFrame8.CmdMain;
	 m_WFrame8.CmdSub  = 0x10;		   m_WFrame8.CheckSum ^= m_WFrame8.CmdSub;
	 m_WFrame8.eof      = 0x7F;

	OWS_HEAD head;
	head.dwDatLen = sizeof(head) + 1 + sizeof(m_WFrame8);
	head.Cmd = PET_TESTREQ;
	head.cFlag[0] = 0x7e;
	head.cFlag[1] = 0x7e;
	head.cFlag[2] = 0x7e;

	BYTE testObj = 41;
	((CSimOWSDlg*)AfxGetMainWnd())->m_ServerSock.SendDatToClient(&head, sizeof(head));
	((CSimOWSDlg*)AfxGetMainWnd())->m_ServerSock.SendDatToClient(&testObj, 1);
	((CSimOWSDlg*)AfxGetMainWnd())->m_ServerSock.SendDatToClient(&m_WFrame8,8);
	 return TRUE;
}

//设置GANTRY温度上下限
BOOL COWSSIO::SetThreshold_TG(WORD upper, WORD lower)
{
	 m_WFrame12.CheckSum = 0;
	 m_WFrame12.tAddr   = ADDR_GANTRY;   m_WFrame12.CheckSum ^= m_WFrame12.tAddr;
	 m_WFrame12.sAddr   = ADDR_ACS;	     m_WFrame12.CheckSum ^= m_WFrame12.sAddr;
	 m_WFrame12.len     = 0x0c;          m_WFrame12.CheckSum ^= m_WFrame12.len;
	 m_WFrame12.attr    = 0;
	 m_WFrame12.CmdMain = 0xC8;		     m_WFrame12.CheckSum ^= m_WFrame12.CmdMain;
	 m_WFrame12.CmdSub  = 0x20;	         m_WFrame12.CheckSum ^= m_WFrame12.CmdSub;
	 m_WFrame12.Dat[0]  = LOBYTE(upper); m_WFrame12.CheckSum ^= m_WFrame12.Dat[0];
	 m_WFrame12.Dat[1]  = HIBYTE(upper); m_WFrame12.CheckSum ^= m_WFrame12.Dat[1];
	 m_WFrame12.Dat[2]  = LOBYTE(lower); m_WFrame12.CheckSum ^= m_WFrame12.Dat[2];
	 m_WFrame12.Dat[3]  = HIBYTE(lower); m_WFrame12.CheckSum ^= m_WFrame12.Dat[3];
	 m_WFrame12.eof      = 0x7F;

	OWS_HEAD head;
	head.dwDatLen = sizeof(head) + 1 + sizeof(m_WFrame12);
	head.Cmd = PET_TESTREQ;
	head.cFlag[0] = 0x7e;
	head.cFlag[1] = 0x7e;
	head.cFlag[2] = 0x7e;

	BYTE testObj = 41;
	((CSimOWSDlg*)AfxGetMainWnd())->m_ServerSock.SendDatToClient(&head, sizeof(head));
	((CSimOWSDlg*)AfxGetMainWnd())->m_ServerSock.SendDatToClient(&testObj, 1);
	((CSimOWSDlg*)AfxGetMainWnd())->m_ServerSock.SendDatToClient(&m_WFrame12,12);
	 return TRUE;
}

//设置棒源转速
BOOL COWSSIO::SetRodRate(WORD rate)
{
	 m_WFrame9.CheckSum = 0;
	 m_WFrame9.tAddr   = ADDR_ROD;      m_WFrame9.CheckSum ^= m_WFrame9.tAddr;
	 m_WFrame9.sAddr   = ADDR_ACS;	    m_WFrame9.CheckSum ^= m_WFrame9.sAddr;
	 m_WFrame9.len     = 0x09;          m_WFrame9.CheckSum ^= m_WFrame9.len;
	 m_WFrame9.attr    = 0;
	 m_WFrame9.CmdMain = 0x0A;		    m_WFrame9.CheckSum ^= m_WFrame9.CmdMain;
	 m_WFrame9.CmdSub  = LOBYTE(rate);	m_WFrame9.CheckSum ^= m_WFrame9.CmdSub;
	 m_WFrame9.Dat     = HIBYTE(rate);  m_WFrame9.CheckSum ^= m_WFrame9.Dat;
	 m_WFrame9.eof     = 0x7F;

	OWS_HEAD head;
	head.dwDatLen = sizeof(head) + 1 + sizeof(m_WFrame9);
	head.Cmd = PET_TESTREQ;
	head.cFlag[0] = 0x7e;
	head.cFlag[1] = 0x7e;
	head.cFlag[2] = 0x7e;

	BYTE testObj = 41;
	((CSimOWSDlg*)AfxGetMainWnd())->m_ServerSock.SendDatToClient(&head, sizeof(head));
	((CSimOWSDlg*)AfxGetMainWnd())->m_ServerSock.SendDatToClient(&testObj, 1);
	((CSimOWSDlg*)AfxGetMainWnd())->m_ServerSock.SendDatToClient(&m_WFrame9,9);
	 return TRUE;
}
//查询SEPTA状态
BOOL COWSSIO::QuerySeptaStatus()
{
	 m_WFrame9.CheckSum = 0;
	 m_WFrame9.tAddr   = ADDR_SEPTA;     m_WFrame9.CheckSum ^= m_WFrame9.tAddr;
	 m_WFrame9.sAddr   = ADDR_ACS;	   m_WFrame9.CheckSum ^= m_WFrame9.sAddr;
	 m_WFrame9.len     = 0x09;         m_WFrame9.CheckSum ^= m_WFrame9.len;
	 m_WFrame9.attr    = 0;
	 m_WFrame9.CmdMain = 0x14;	       m_WFrame9.CheckSum ^= m_WFrame9.CmdMain;
	 m_WFrame9.CmdSub  = 0x00;		   m_WFrame9.CheckSum ^= m_WFrame9.CmdSub;
	 m_WFrame9.Dat     = 0x00;
	 m_WFrame9.eof     = 0x7F;

	OWS_HEAD head;
	head.dwDatLen = sizeof(head) + 1 + sizeof(m_WFrame9);
	head.Cmd = PET_TESTREQ;
	head.cFlag[0] = 0x7e;
	head.cFlag[1] = 0x7e;
	head.cFlag[2] = 0x7e;

	BYTE testObj = 41;
	((CSimOWSDlg*)AfxGetMainWnd())->m_ServerSock.SendDatToClient(&head, sizeof(head));
	((CSimOWSDlg*)AfxGetMainWnd())->m_ServerSock.SendDatToClient(&testObj, 1);
	((CSimOWSDlg*)AfxGetMainWnd())->m_ServerSock.SendDatToClient(&m_WFrame9,9);
	 return TRUE;
}

//查询棒源状态
BOOL COWSSIO::QueryRodStatus()
{
	 m_WFrame9.CheckSum = 0;
	 m_WFrame9.tAddr   = ADDR_ROD;     m_WFrame9.CheckSum ^= m_WFrame9.tAddr;
	 m_WFrame9.sAddr   = ADDR_ACS;	   m_WFrame9.CheckSum ^= m_WFrame9.sAddr;
	 m_WFrame9.len     = 0x09;         m_WFrame9.CheckSum ^= m_WFrame9.len;
	 m_WFrame9.attr    = 0;
	 m_WFrame9.CmdMain = 0x14;	       m_WFrame9.CheckSum ^= m_WFrame9.CmdMain;
	 m_WFrame9.CmdSub  = 0x00;		   m_WFrame9.CheckSum ^= m_WFrame9.CmdSub;
	 m_WFrame9.Dat     = 0x00;
	 m_WFrame9.eof     = 0x7F;

	OWS_HEAD head;
	head.dwDatLen = sizeof(head) + 1 + sizeof(m_WFrame9);
	head.Cmd = PET_TESTREQ;
	head.cFlag[0] = 0x7e;
	head.cFlag[1] = 0x7e;
	head.cFlag[2] = 0x7e;

	BYTE testObj = 41;
	((CSimOWSDlg*)AfxGetMainWnd())->m_ServerSock.SendDatToClient(&head, sizeof(head));
	((CSimOWSDlg*)AfxGetMainWnd())->m_ServerSock.SendDatToClient(&testObj, 1);
	((CSimOWSDlg*)AfxGetMainWnd())->m_ServerSock.SendDatToClient(&m_WFrame9,9);
	 return TRUE;
}

//查询棒源转速
BOOL COWSSIO::QueryRodRate()
{
	 m_WFrame9.CheckSum = 0;
	 m_WFrame9.tAddr   = ADDR_ROD;     m_WFrame9.CheckSum ^= m_WFrame9.tAddr;
	 m_WFrame9.sAddr   = ADDR_ACS;	   m_WFrame9.CheckSum ^= m_WFrame9.sAddr;
	 m_WFrame9.len     = 0x09;         m_WFrame9.CheckSum ^= m_WFrame9.len;
	 m_WFrame9.attr    = 0;
	 m_WFrame9.CmdMain = 0x0B;	       m_WFrame9.CheckSum ^= m_WFrame9.CmdMain;
	 m_WFrame9.CmdSub  = 0x00;		   m_WFrame9.CheckSum ^= m_WFrame9.CmdSub;
	 m_WFrame9.Dat     = 0x00;
	 m_WFrame9.eof     = 0x7F;

	OWS_HEAD head;
	head.dwDatLen = sizeof(head) + 1 + sizeof(m_WFrame9);
	head.Cmd = PET_TESTREQ;
	head.cFlag[0] = 0x7e;
	head.cFlag[1] = 0x7e;
	head.cFlag[2] = 0x7e;

	BYTE testObj = 41;
	((CSimOWSDlg*)AfxGetMainWnd())->m_ServerSock.SendDatToClient(&head, sizeof(head));
	((CSimOWSDlg*)AfxGetMainWnd())->m_ServerSock.SendDatToClient(&testObj, 1);
	((CSimOWSDlg*)AfxGetMainWnd())->m_ServerSock.SendDatToClient(&m_WFrame9,9);
	 return TRUE;
}

//查询棒源开关量设置
BOOL COWSSIO::QueryRodSwitchSet()
{
	 m_WFrame12.CheckSum = 0;
	 m_WFrame12.tAddr   = ADDR_ROD;    m_WFrame12.CheckSum ^= m_WFrame12.tAddr;
	 m_WFrame12.sAddr   = ADDR_ACS;	   m_WFrame12.CheckSum ^= m_WFrame12.sAddr;
	 m_WFrame12.len     = 0x0C;        m_WFrame12.CheckSum ^= m_WFrame12.len;
	 m_WFrame12.attr    = 0;
	 m_WFrame12.CmdMain = 0xE1;	       m_WFrame12.CheckSum ^= m_WFrame12.CmdMain;
	 m_WFrame12.CmdSub  = ADDR_ROD;	   m_WFrame12.CheckSum ^= m_WFrame12.CmdSub;
	 m_WFrame12.Dat[0]  = 0x00;
	 m_WFrame12.Dat[1]  = 0x00;
	 m_WFrame12.Dat[2]  = 0x00;
	 m_WFrame12.Dat[3]  = 0x00;
	 m_WFrame12.eof     = 0x7F;

	OWS_HEAD head;
	head.dwDatLen = sizeof(head) + 1 + sizeof(m_WFrame12);
	head.Cmd = PET_TESTREQ;
	head.cFlag[0] = 0x7e;
	head.cFlag[1] = 0x7e;
	head.cFlag[2] = 0x7e;

	BYTE testObj = 41;
	((CSimOWSDlg*)AfxGetMainWnd())->m_ServerSock.SendDatToClient(&head, sizeof(head));
	((CSimOWSDlg*)AfxGetMainWnd())->m_ServerSock.SendDatToClient(&testObj, 1);
	((CSimOWSDlg*)AfxGetMainWnd())->m_ServerSock.SendDatToClient(&m_WFrame12,12);
	 return TRUE;
}

//控制棒源伸出并转动
BOOL COWSSIO::Control_RodOut()
{
	 m_WFrame9.CheckSum = 0;
	 m_WFrame9.tAddr   = ADDR_ROD;   m_WFrame9.CheckSum ^= m_WFrame9.tAddr;
	 m_WFrame9.sAddr   = ADDR_ACS;   m_WFrame9.CheckSum ^= m_WFrame9.sAddr;
	 m_WFrame9.len     = 0x09;       m_WFrame9.CheckSum ^= m_WFrame9.len;
	 m_WFrame9.attr    = 0;
	 m_WFrame9.CmdMain = 0x01;	     m_WFrame9.CheckSum ^= m_WFrame9.CmdMain;
	 m_WFrame9.CmdSub  = 0x01;		 m_WFrame9.CheckSum ^= m_WFrame9.CmdSub;
	 m_WFrame9.Dat     = 0x00;
	 m_WFrame9.eof     = 0x7F;

	OWS_HEAD head;
	head.dwDatLen = sizeof(head) + 1 + sizeof(m_WFrame9);
	head.Cmd = PET_TESTREQ;
	head.cFlag[0] = 0x7e;
	head.cFlag[1] = 0x7e;
	head.cFlag[2] = 0x7e;

	BYTE testObj = 41;
	((CSimOWSDlg*)AfxGetMainWnd())->m_ServerSock.SendDatToClient(&head, sizeof(head));
	((CSimOWSDlg*)AfxGetMainWnd())->m_ServerSock.SendDatToClient(&testObj, 1);
	((CSimOWSDlg*)AfxGetMainWnd())->m_ServerSock.SendDatToClient(&m_WFrame9,9);
	 return TRUE;
}

//控制棒源在已伸出的状态下开始旋转
BOOL COWSSIO::Control_RodRoll()
{
	 m_WFrame9.CheckSum = 0;
	 m_WFrame9.tAddr   = ADDR_ROD;   m_WFrame9.CheckSum ^= m_WFrame9.tAddr;
	 m_WFrame9.sAddr   = ADDR_ACS;   m_WFrame9.CheckSum ^= m_WFrame9.sAddr;
	 m_WFrame9.len     = 0x09;       m_WFrame9.CheckSum ^= m_WFrame9.len;
	 m_WFrame9.attr    = 0;
	 m_WFrame9.CmdMain = 0x03;	     m_WFrame9.CheckSum ^= m_WFrame9.CmdMain;
	 m_WFrame9.CmdSub  = 0x03;		 m_WFrame9.CheckSum ^= m_WFrame9.CmdSub;
	 m_WFrame9.Dat     = 0x01;       m_WFrame9.CheckSum ^= m_WFrame9.Dat;
	 m_WFrame9.eof     = 0x7F;

	OWS_HEAD head;
	head.dwDatLen = sizeof(head) + 1 + sizeof(m_WFrame9);
	head.Cmd = PET_TESTREQ;
	head.cFlag[0] = 0x7e;
	head.cFlag[1] = 0x7e;
	head.cFlag[2] = 0x7e;

	BYTE testObj = 41;
	((CSimOWSDlg*)AfxGetMainWnd())->m_ServerSock.SendDatToClient(&head, sizeof(head));
	((CSimOWSDlg*)AfxGetMainWnd())->m_ServerSock.SendDatToClient(&testObj, 1);
	((CSimOWSDlg*)AfxGetMainWnd())->m_ServerSock.SendDatToClient(&m_WFrame9,9);
	 return TRUE;
}

//控制棒源停止转动
BOOL COWSSIO::Control_RodStop()
{
	 m_WFrame9.CheckSum = 0;
	 m_WFrame9.tAddr   = ADDR_ROD;     m_WFrame9.CheckSum ^= m_WFrame9.tAddr;
	 m_WFrame9.sAddr   = ADDR_ACS;	   m_WFrame9.CheckSum ^= m_WFrame9.sAddr;
	 m_WFrame9.len     = 0x09;         m_WFrame9.CheckSum ^= m_WFrame9.len;
	 m_WFrame9.attr    = 0;
	 m_WFrame9.CmdMain = 0x02;		   m_WFrame9.CheckSum ^= m_WFrame9.CmdMain;
	 m_WFrame9.CmdSub  = 0x01;		   m_WFrame9.CheckSum ^= m_WFrame9.CmdSub;
	 m_WFrame9.Dat     = 0x00;
	 m_WFrame9.eof     = 0x7F;

	OWS_HEAD head;
	head.dwDatLen = sizeof(head) + 1 + sizeof(m_WFrame9);
	head.Cmd = PET_TESTREQ;
	head.cFlag[0] = 0x7e;
	head.cFlag[1] = 0x7e;
	head.cFlag[2] = 0x7e;

	BYTE testObj = 41;
	((CSimOWSDlg*)AfxGetMainWnd())->m_ServerSock.SendDatToClient(&head, sizeof(head));
	((CSimOWSDlg*)AfxGetMainWnd())->m_ServerSock.SendDatToClient(&testObj, 1);
	((CSimOWSDlg*)AfxGetMainWnd())->m_ServerSock.SendDatToClient(&m_WFrame9,9);
	 return TRUE;
}

//控制棒源收回
BOOL COWSSIO::Control_RodIn()
{
	 m_WFrame9.CheckSum = 0;
	 m_WFrame9.tAddr   = ADDR_ROD;   m_WFrame9.CheckSum ^= m_WFrame9.tAddr;
	 m_WFrame9.sAddr   = ADDR_ACS;	 m_WFrame9.CheckSum ^= m_WFrame9.sAddr;
	 m_WFrame9.len     = 0x09;       m_WFrame9.CheckSum ^= m_WFrame9.len;
	 m_WFrame9.attr    = 0;
	 m_WFrame9.CmdMain = 0x03;	     m_WFrame9.CheckSum ^= m_WFrame9.CmdMain;
	 m_WFrame9.CmdSub  = 0x03;		 m_WFrame9.CheckSum ^= m_WFrame9.CmdSub;
	 m_WFrame9.Dat     = 0x00; 
	 m_WFrame9.eof     = 0x7F;

	 OWS_HEAD head;
	head.dwDatLen = sizeof(head) + 1 + sizeof(m_WFrame9);
	head.Cmd = PET_TESTREQ;
	head.cFlag[0] = 0x7e;
	head.cFlag[1] = 0x7e;
	head.cFlag[2] = 0x7e;

	BYTE testObj = 41;
	((CSimOWSDlg*)AfxGetMainWnd())->m_ServerSock.SendDatToClient(&head, sizeof(head));
	((CSimOWSDlg*)AfxGetMainWnd())->m_ServerSock.SendDatToClient(&testObj, 1);
	((CSimOWSDlg*)AfxGetMainWnd())->m_ServerSock.SendDatToClient(&m_WFrame9,9);
	 return TRUE;
}

//控制棒源紧急停止
BOOL COWSSIO::Control_RodEStop()
{
	 m_WFrame9.CheckSum = 0;
	 m_WFrame9.tAddr   = ADDR_ROD;   m_WFrame9.CheckSum ^= m_WFrame9.tAddr;
	 m_WFrame9.sAddr   = ADDR_ACS;	 m_WFrame9.CheckSum ^= m_WFrame9.sAddr;
	 m_WFrame9.len     = 0x09;       m_WFrame9.CheckSum ^= m_WFrame9.len;
	 m_WFrame9.attr    = 0;
	 m_WFrame9.CmdMain = 0xEE;	     m_WFrame9.CheckSum ^= m_WFrame9.CmdMain;
	 m_WFrame9.CmdSub  = 0x00;		 m_WFrame9.CheckSum ^= m_WFrame9.CmdSub;
	 m_WFrame9.Dat     = 0x00;
	 m_WFrame9.eof     = 0x7F;

	OWS_HEAD head;
	head.dwDatLen = sizeof(head) + 1 + sizeof(m_WFrame9);
	head.Cmd = PET_TESTREQ;
	head.cFlag[0] = 0x7e;
	head.cFlag[1] = 0x7e;
	head.cFlag[2] = 0x7e;

	BYTE testObj = 41;
	((CSimOWSDlg*)AfxGetMainWnd())->m_ServerSock.SendDatToClient(&head, sizeof(head));
	((CSimOWSDlg*)AfxGetMainWnd())->m_ServerSock.SendDatToClient(&testObj, 1);
	((CSimOWSDlg*)AfxGetMainWnd())->m_ServerSock.SendDatToClient(&m_WFrame9,9);
	 return TRUE;
}

//控制床水平运动，绝对运动
BOOL COWSSIO::Control_BedMoveX(WORD len)
{
	 m_WFrame12.CheckSum = 0;
	 m_WFrame12.tAddr   = ADDR_BED;     m_WFrame12.CheckSum ^= m_WFrame12.tAddr;
	 m_WFrame12.sAddr   = ADDR_ACS;	    m_WFrame12.CheckSum ^= m_WFrame12.sAddr;
	 m_WFrame12.len     = 0x0C;         m_WFrame12.CheckSum ^= m_WFrame12.len;
	 m_WFrame12.attr    = 0;
	 m_WFrame12.CmdMain = 0x01;		    m_WFrame12.CheckSum ^= m_WFrame12.CmdMain;
	 m_WFrame12.CmdSub  = 0x00;         m_WFrame12.CheckSum ^= m_WFrame12.CmdSub;
	 m_WFrame12.Dat[0]  = LOBYTE(len);	m_WFrame12.CheckSum ^= m_WFrame12.Dat[0];
	 m_WFrame12.Dat[1]  = HIBYTE(len);  m_WFrame12.CheckSum ^= m_WFrame12.Dat[1];
	 m_WFrame12.Dat[2]  = 0x00;
	 m_WFrame12.Dat[3]  = 0x00;
	 m_WFrame12.eof     = 0x7F;

	OWS_HEAD head;
	head.dwDatLen = sizeof(head) + 1 + sizeof(m_WFrame12);
	head.Cmd = PET_TESTREQ;
	head.cFlag[0] = 0x7e;
	head.cFlag[1] = 0x7e;
	head.cFlag[2] = 0x7e;

	BYTE testObj = 41;
	((CSimOWSDlg*)AfxGetMainWnd())->m_ServerSock.SendDatToClient(&head, sizeof(head));
	((CSimOWSDlg*)AfxGetMainWnd())->m_ServerSock.SendDatToClient(&testObj, 1);
	((CSimOWSDlg*)AfxGetMainWnd())->m_ServerSock.SendDatToClient(&m_WFrame12,12);
	 return TRUE;
}

//控制床相对运动
BOOL COWSSIO::Control_BedMoveXRelate(WORD len, BOOL bOut)
{
	 ASSERT( g_MCtlInfo.wBed_bRelateCoord == TRUE);
	 if(g_MCtlInfo.wBed_bRelateCoord != TRUE)
	 {
		 return FALSE;
	 }

	 ASSERT( (len&0x8000) == 0);
	 if( !bOut )  len |= 0x8000;

	 m_WFrame12.CheckSum = 0;
	 m_WFrame12.tAddr   = ADDR_BED;     m_WFrame12.CheckSum ^= m_WFrame12.tAddr;
	 m_WFrame12.sAddr   = ADDR_ACS;	    m_WFrame12.CheckSum ^= m_WFrame12.sAddr;
	 m_WFrame12.len     = 0x0C;         m_WFrame12.CheckSum ^= m_WFrame12.len;
	 m_WFrame12.attr    = 0;
	 m_WFrame12.CmdMain = 0x01;		    m_WFrame12.CheckSum ^= m_WFrame12.CmdMain;
	 m_WFrame12.CmdSub  = 0x00;         m_WFrame12.CheckSum ^= m_WFrame12.CmdSub;
	 m_WFrame12.Dat[0]  = LOBYTE(len);	m_WFrame12.CheckSum ^= m_WFrame12.Dat[0];
	 m_WFrame12.Dat[1]  = HIBYTE(len);  m_WFrame12.CheckSum ^= m_WFrame12.Dat[1];
	 m_WFrame12.Dat[2]  = 0x00;
	 m_WFrame12.Dat[3]  = 0x00;
	 m_WFrame12.eof     = 0x7F;

	OWS_HEAD head;
	head.dwDatLen = sizeof(head) + 1 + sizeof(m_WFrame12);
	head.Cmd = PET_TESTREQ;
	head.cFlag[0] = 0x7e;
	head.cFlag[1] = 0x7e;
	head.cFlag[2] = 0x7e;

	BYTE testObj = 41;
	((CSimOWSDlg*)AfxGetMainWnd())->m_ServerSock.SendDatToClient(&head, sizeof(head));
	((CSimOWSDlg*)AfxGetMainWnd())->m_ServerSock.SendDatToClient(&testObj, 1);
	((CSimOWSDlg*)AfxGetMainWnd())->m_ServerSock.SendDatToClient(&m_WFrame12,12);
	 return TRUE;
}

//控制床Y绝对运动
BOOL COWSSIO::Control_BedMoveY(WORD len)
{
	 ASSERT( g_MCtlInfo.wBed_bRelateCoord == FALSE);
	 if(g_MCtlInfo.wBed_bRelateCoord != FALSE)
	 {
		 return FALSE;
	 }

	 m_WFrame12.CheckSum = 0;
	 m_WFrame12.tAddr   = ADDR_BED;     m_WFrame12.CheckSum ^= m_WFrame12.tAddr;
	 m_WFrame12.sAddr   = ADDR_ACS;	    m_WFrame12.CheckSum ^= m_WFrame12.sAddr;
	 m_WFrame12.len     = 0x0C;         m_WFrame12.CheckSum ^= m_WFrame12.len;
	 m_WFrame12.attr    = 0;
	 m_WFrame12.CmdMain = 0x02;		    m_WFrame12.CheckSum ^= m_WFrame12.CmdMain;
	 m_WFrame12.CmdSub  = 0x00;         m_WFrame12.CheckSum ^= m_WFrame12.CmdSub;
	 m_WFrame12.Dat[0]  = LOBYTE(len);  m_WFrame12.CheckSum ^= m_WFrame12.Dat[0];
	 m_WFrame12.Dat[1]  = HIBYTE(len);  m_WFrame12.CheckSum ^= m_WFrame12.Dat[1];
	 m_WFrame12.Dat[2]  = 0x00;         m_WFrame12.CheckSum ^= m_WFrame12.Dat[2];
	 m_WFrame12.Dat[3]  = 0x00;			m_WFrame12.CheckSum ^= m_WFrame12.Dat[3];
	 m_WFrame12.eof     = 0x7F;

	OWS_HEAD head;
	head.dwDatLen = sizeof(head) + 1 + sizeof(m_WFrame12);
	head.Cmd = PET_TESTREQ;
	head.cFlag[0] = 0x7e;
	head.cFlag[1] = 0x7e;
	head.cFlag[2] = 0x7e;

	BYTE testObj = 41;
	((CSimOWSDlg*)AfxGetMainWnd())->m_ServerSock.SendDatToClient(&head, sizeof(head));
	((CSimOWSDlg*)AfxGetMainWnd())->m_ServerSock.SendDatToClient(&testObj, 1);
	((CSimOWSDlg*)AfxGetMainWnd())->m_ServerSock.SendDatToClient(&m_WFrame12,12);
	 return TRUE;
}

//控制床Y相对运动
BOOL COWSSIO::Control_BedMoveYRelate(WORD len, BOOL bUp)
{
	 ASSERT( g_MCtlInfo.wBed_bRelateCoord == TRUE);
	 if(g_MCtlInfo.wBed_bRelateCoord != TRUE)
	 {
		 return FALSE;
	 }

	 ASSERT( (len&0x8000) == 0);
	 if( !bUp )  len |= 0x8000;

	 m_WFrame12.CheckSum = 0;
	 m_WFrame12.tAddr   = ADDR_BED;     m_WFrame12.CheckSum ^= m_WFrame12.tAddr;
	 m_WFrame12.sAddr   = ADDR_ACS;	    m_WFrame12.CheckSum ^= m_WFrame12.sAddr;
	 m_WFrame12.len     = 0x0C;         m_WFrame12.CheckSum ^= m_WFrame12.len;
	 m_WFrame12.attr    = 0;
	 m_WFrame12.CmdMain = 0x02;		    m_WFrame12.CheckSum ^= m_WFrame12.CmdMain;
	 m_WFrame12.CmdSub  = 0x00;         m_WFrame12.CheckSum ^= m_WFrame12.CmdSub;
	 m_WFrame12.Dat[0]  = LOBYTE(len);  m_WFrame12.CheckSum ^= m_WFrame12.Dat[0];
	 m_WFrame12.Dat[1]  = HIBYTE(len);  m_WFrame12.CheckSum ^= m_WFrame12.Dat[1];
	 m_WFrame12.Dat[2]  = 0x00;         m_WFrame12.CheckSum ^= m_WFrame12.Dat[2];
	 m_WFrame12.Dat[3]  = 0x00;			m_WFrame12.CheckSum ^= m_WFrame12.Dat[3];
	 m_WFrame12.eof     = 0x7F;

	OWS_HEAD head;
	head.dwDatLen = sizeof(head) + 1 + sizeof(m_WFrame12);
	head.Cmd = PET_TESTREQ;
	head.cFlag[0] = 0x7e;
	head.cFlag[1] = 0x7e;
	head.cFlag[2] = 0x7e;

	BYTE testObj = 41;
	((CSimOWSDlg*)AfxGetMainWnd())->m_ServerSock.SendDatToClient(&head, sizeof(head));
	((CSimOWSDlg*)AfxGetMainWnd())->m_ServerSock.SendDatToClient(&testObj, 1);
	((CSimOWSDlg*)AfxGetMainWnd())->m_ServerSock.SendDatToClient(&m_WFrame12,12);
	 return TRUE;
}

BOOL COWSSIO::Control_BedMoveXStep(BOOL bOut)
{
	 m_WFrame12.CheckSum = 0;
	 m_WFrame12.tAddr   = ADDR_BED;     m_WFrame12.CheckSum ^= m_WFrame12.tAddr;
	 m_WFrame12.sAddr   = ADDR_ACS;	    m_WFrame12.CheckSum ^= m_WFrame12.sAddr;
	 m_WFrame12.len     = 0x0C;         m_WFrame12.CheckSum ^= m_WFrame12.len;
	 m_WFrame12.attr    = 0;
	 m_WFrame12.CmdMain = 0x07;		    m_WFrame12.CheckSum ^= m_WFrame12.CmdMain;
	 if(bOut)	m_WFrame12.CmdSub  = 0;  
	 else		m_WFrame12.CmdSub  = 1;		 
									    m_WFrame12.CheckSum ^= m_WFrame12.CmdSub;
	 m_WFrame12.Dat[0]     = 0x00;      m_WFrame12.CheckSum ^= m_WFrame12.Dat[0];
	 m_WFrame12.Dat[1]     = 0x00;      m_WFrame12.CheckSum ^= m_WFrame12.Dat[1];
	 m_WFrame12.Dat[2]     = 0x00;      m_WFrame12.CheckSum ^= m_WFrame12.Dat[2];
	 m_WFrame12.Dat[3]     = 0x00;      m_WFrame12.CheckSum ^= m_WFrame12.Dat[3];
	 m_WFrame12.eof     = 0x7F;

	OWS_HEAD head;
	head.dwDatLen = sizeof(head) + 1 + sizeof(m_WFrame12);
	head.Cmd = PET_TESTREQ;
	head.cFlag[0] = 0x7e;
	head.cFlag[1] = 0x7e;
	head.cFlag[2] = 0x7e;

	BYTE testObj = 41;
	((CSimOWSDlg*)AfxGetMainWnd())->m_ServerSock.SendDatToClient(&head, sizeof(head));
	((CSimOWSDlg*)AfxGetMainWnd())->m_ServerSock.SendDatToClient(&testObj, 1);
	((CSimOWSDlg*)AfxGetMainWnd())->m_ServerSock.SendDatToClient(&m_WFrame12,12);
	 return TRUE;
}

BOOL COWSSIO::Control_BedMoveYStep(BOOL bUp)
{
	 m_WFrame12.CheckSum = 0;
	 m_WFrame12.tAddr   = ADDR_BED;     m_WFrame12.CheckSum ^= m_WFrame12.tAddr;
	 m_WFrame12.sAddr   = ADDR_ACS;	    m_WFrame12.CheckSum ^= m_WFrame12.sAddr;
	 m_WFrame12.len     = 0x0C;         m_WFrame12.CheckSum ^= m_WFrame12.len;
	 m_WFrame12.attr    = 0;
	 m_WFrame12.CmdMain = 0x08;		    m_WFrame12.CheckSum ^= m_WFrame12.CmdMain;
	 m_WFrame12.CmdSub  = 0x01;
	 if(bUp)	m_WFrame12.Dat[0]  = 0;  
	 else		m_WFrame12.Dat[0]  = 1;		 
									    m_WFrame12.CheckSum ^= m_WFrame12.Dat[0];
	 m_WFrame12.Dat[1]  = 0x00;         m_WFrame12.CheckSum ^= m_WFrame12.Dat[1];
	 m_WFrame12.Dat[2]  = 0x00;         m_WFrame12.CheckSum ^= m_WFrame12.Dat[2];
	 m_WFrame12.Dat[3]  = 0x00;         m_WFrame12.CheckSum ^= m_WFrame12.Dat[3];
	 m_WFrame12.eof     = 0x7F;

	OWS_HEAD head;
	head.dwDatLen = sizeof(head) + 1 + sizeof(m_WFrame12);
	head.Cmd = PET_TESTREQ;
	head.cFlag[0] = 0x7e;
	head.cFlag[1] = 0x7e;
	head.cFlag[2] = 0x7e;

	BYTE testObj = 41;
	((CSimOWSDlg*)AfxGetMainWnd())->m_ServerSock.SendDatToClient(&head, sizeof(head));
	((CSimOWSDlg*)AfxGetMainWnd())->m_ServerSock.SendDatToClient(&testObj, 1);
	((CSimOWSDlg*)AfxGetMainWnd())->m_ServerSock.SendDatToClient(&m_WFrame12,12);
	 return TRUE;
}

BOOL COWSSIO::Control_BedStopX()
{
	 m_WFrame9.CheckSum = 0;
	 m_WFrame9.tAddr   = ADDR_BED;     m_WFrame9.CheckSum ^= m_WFrame9.tAddr;
	 m_WFrame9.sAddr   = ADDR_ACS;	   m_WFrame9.CheckSum ^= m_WFrame9.sAddr;
	 m_WFrame9.len     = 0x09;         m_WFrame9.CheckSum ^= m_WFrame9.len;
	 m_WFrame9.attr    = 0;
	 m_WFrame9.CmdMain = 0x0B;		   m_WFrame9.CheckSum ^= m_WFrame9.CmdMain;
	 m_WFrame9.CmdSub  = 0x01; 		   m_WFrame9.CheckSum ^= m_WFrame9.CmdSub;
	 m_WFrame9.Dat     = 0x00;         m_WFrame9.CheckSum ^= m_WFrame9.Dat;
	 m_WFrame9.eof     = 0x7F;

	OWS_HEAD head;
	head.dwDatLen = sizeof(head) + 1 + sizeof(m_WFrame9);
	head.Cmd = PET_TESTREQ;
	head.cFlag[0] = 0x7e;
	head.cFlag[1] = 0x7e;
	head.cFlag[2] = 0x7e;

	BYTE testObj = 41;
	((CSimOWSDlg*)AfxGetMainWnd())->m_ServerSock.SendDatToClient(&head, sizeof(head));
	((CSimOWSDlg*)AfxGetMainWnd())->m_ServerSock.SendDatToClient(&testObj, 1);
	((CSimOWSDlg*)AfxGetMainWnd())->m_ServerSock.SendDatToClient(&m_WFrame9,9);
	 return TRUE;
}

BOOL COWSSIO::Control_BedStopY()
{
	 m_WFrame9.CheckSum = 0;
	 m_WFrame9.tAddr   = ADDR_BED;     m_WFrame9.CheckSum ^= m_WFrame9.tAddr;
	 m_WFrame9.sAddr   = ADDR_ACS;	   m_WFrame9.CheckSum ^= m_WFrame9.sAddr;
	 m_WFrame9.len     = 0x09;         m_WFrame9.CheckSum ^= m_WFrame9.len;
	 m_WFrame9.attr    = 0;
	 m_WFrame9.CmdMain = 0x0B;		   m_WFrame9.CheckSum ^= m_WFrame9.CmdMain;
	 m_WFrame9.CmdSub  = 0x02; 		   m_WFrame9.CheckSum ^= m_WFrame9.CmdSub;
	 m_WFrame9.Dat     = 0x00;         m_WFrame9.CheckSum ^= m_WFrame9.Dat;
	 m_WFrame9.eof     = 0x7F;

	OWS_HEAD head;
	head.dwDatLen = sizeof(head) + 1 + sizeof(m_WFrame9);
	head.Cmd = PET_TESTREQ;
	head.cFlag[0] = 0x7e;
	head.cFlag[1] = 0x7e;
	head.cFlag[2] = 0x7e;

	BYTE testObj = 41;
	((CSimOWSDlg*)AfxGetMainWnd())->m_ServerSock.SendDatToClient(&head, sizeof(head));
	((CSimOWSDlg*)AfxGetMainWnd())->m_ServerSock.SendDatToClient(&testObj, 1);
	((CSimOWSDlg*)AfxGetMainWnd())->m_ServerSock.SendDatToClient(&m_WFrame9,9);
	 return TRUE;
}

BOOL COWSSIO::Control_BedStopAll()
{
	 m_WFrame9.CheckSum = 0;
	 m_WFrame9.tAddr   = ADDR_BED;     m_WFrame9.CheckSum ^= m_WFrame9.tAddr;
	 m_WFrame9.sAddr   = ADDR_ACS;	   m_WFrame9.CheckSum ^= m_WFrame9.sAddr;
	 m_WFrame9.len     = 0x09;         m_WFrame9.CheckSum ^= m_WFrame9.len;
	 m_WFrame9.attr    = 0;
	 m_WFrame9.CmdMain = 0x03;		   m_WFrame9.CheckSum ^= m_WFrame9.CmdMain;
	 m_WFrame9.CmdSub  = 0x00; 		   m_WFrame9.CheckSum ^= m_WFrame9.CmdSub;
	 m_WFrame9.Dat     = 0x00;         m_WFrame9.CheckSum ^= m_WFrame9.Dat;
	 m_WFrame9.eof     = 0x7F;

	OWS_HEAD head;
	head.dwDatLen = sizeof(head) + 1 + sizeof(m_WFrame9);
	head.Cmd = PET_TESTREQ;
	head.cFlag[0] = 0x7e;
	head.cFlag[1] = 0x7e;
	head.cFlag[2] = 0x7e;

	BYTE testObj = 41;
	((CSimOWSDlg*)AfxGetMainWnd())->m_ServerSock.SendDatToClient(&head, sizeof(head));
	((CSimOWSDlg*)AfxGetMainWnd())->m_ServerSock.SendDatToClient(&testObj, 1);
	((CSimOWSDlg*)AfxGetMainWnd())->m_ServerSock.SendDatToClient(&m_WFrame9,9);
	 return TRUE;
}

//设置床坐标方式：TRUE=>相对定位，FALSE=>绝对定位
BOOL COWSSIO::SetBedCoord(BOOL bRelate)
{
	 m_WFrame9.CheckSum = 0;
	 m_WFrame9.tAddr   = ADDR_BED;     m_WFrame9.CheckSum ^= m_WFrame9.tAddr;
	 m_WFrame9.sAddr   = ADDR_ACS;	   m_WFrame9.CheckSum ^= m_WFrame9.sAddr;
	 m_WFrame9.len     = 0x09;         m_WFrame9.CheckSum ^= m_WFrame9.len;
	 m_WFrame9.attr    = 0;
	 m_WFrame9.CmdMain = 0x05;		   m_WFrame9.CheckSum ^= m_WFrame9.CmdMain;
	 if(bRelate) m_WFrame9.CmdSub  = 0x01;
	 else        m_WFrame9.CmdSub  = 0x00;  
									   m_WFrame9.CheckSum ^= m_WFrame9.CmdSub;
	 m_WFrame9.Dat     = 0x00;         m_WFrame9.CheckSum ^= m_WFrame9.Dat;
	 m_WFrame9.eof     = 0x7F;

	 g_MCtlInfo.wBed_bRelateCoord = bRelate;

	OWS_HEAD head;
	head.dwDatLen = sizeof(head) + 1 + sizeof(m_WFrame9);
	head.Cmd = PET_TESTREQ;
	head.cFlag[0] = 0x7e;
	head.cFlag[1] = 0x7e;
	head.cFlag[2] = 0x7e;

	BYTE testObj = 41;
	((CSimOWSDlg*)AfxGetMainWnd())->m_ServerSock.SendDatToClient(&head, sizeof(head));
	((CSimOWSDlg*)AfxGetMainWnd())->m_ServerSock.SendDatToClient(&testObj, 1);
	((CSimOWSDlg*)AfxGetMainWnd())->m_ServerSock.SendDatToClient(&m_WFrame9,9);
	 return TRUE;
}

//查询床开关量设置
BOOL COWSSIO::QueryBedSwitchSet()
{
	 m_WFrame12.CheckSum = 0;
	 m_WFrame12.tAddr   = ADDR_BED;    m_WFrame12.CheckSum ^= m_WFrame12.tAddr;
	 m_WFrame12.sAddr   = ADDR_ACS;	   m_WFrame12.CheckSum ^= m_WFrame12.sAddr;
	 m_WFrame12.len     = 0x0c;        m_WFrame12.CheckSum ^= m_WFrame12.len;
	 m_WFrame12.attr    = 0;
	 m_WFrame12.CmdMain = 0xE1;		   m_WFrame12.CheckSum ^= m_WFrame12.CmdMain;
	 m_WFrame12.CmdSub  = 0x00;        m_WFrame12.CheckSum ^= m_WFrame12.CmdSub;
	 m_WFrame12.Dat[0]  = 0x00;        m_WFrame12.CheckSum ^= m_WFrame12.Dat[0];
	 m_WFrame12.Dat[1]  = 0x00;
	 m_WFrame12.Dat[2]  = 0x00;
	 m_WFrame12.Dat[3]  = 0x00;
	 m_WFrame12.eof     = 0x7F;

	OWS_HEAD head;
	head.dwDatLen = sizeof(head) + 1 + sizeof(m_WFrame12);
	head.Cmd = PET_TESTREQ;
	head.cFlag[0] = 0x7e;
	head.cFlag[1] = 0x7e;
	head.cFlag[2] = 0x7e;

	BYTE testObj = 41;
	((CSimOWSDlg*)AfxGetMainWnd())->m_ServerSock.SendDatToClient(&head, sizeof(head));
	((CSimOWSDlg*)AfxGetMainWnd())->m_ServerSock.SendDatToClient(&testObj, 1);
	((CSimOWSDlg*)AfxGetMainWnd())->m_ServerSock.SendDatToClient(&m_WFrame12,12);
	 return TRUE;
}

//查询床XY坐标
BOOL COWSSIO::QueryBedPos()
{
	 m_WFrame9.CheckSum = 0;
	 m_WFrame9.tAddr   = ADDR_BED;     m_WFrame9.CheckSum ^= m_WFrame9.tAddr;
	 m_WFrame9.sAddr   = ADDR_ACS;	   m_WFrame9.CheckSum ^= m_WFrame9.sAddr;
	 m_WFrame9.len     = 0x09;         m_WFrame9.CheckSum ^= m_WFrame9.len;
	 m_WFrame9.attr    = 0;
	 m_WFrame9.CmdMain = 0x0C;		   m_WFrame9.CheckSum ^= m_WFrame9.CmdMain;
	 m_WFrame9.CmdSub  = 0x00;		   m_WFrame9.CheckSum ^= m_WFrame9.CmdSub;
	 m_WFrame9.Dat     = 0x00;         m_WFrame9.CheckSum ^= m_WFrame9.Dat;
	 m_WFrame9.eof     = 0x7F;

	OWS_HEAD head;
	head.dwDatLen = sizeof(head) + 1 + sizeof(m_WFrame9);
	head.Cmd = PET_TESTREQ;
	head.cFlag[0] = 0x7e;
	head.cFlag[1] = 0x7e;
	head.cFlag[2] = 0x7e;

	BYTE testObj = 41;
	((CSimOWSDlg*)AfxGetMainWnd())->m_ServerSock.SendDatToClient(&head, sizeof(head));
	((CSimOWSDlg*)AfxGetMainWnd())->m_ServerSock.SendDatToClient(&testObj, 1);
	((CSimOWSDlg*)AfxGetMainWnd())->m_ServerSock.SendDatToClient(&m_WFrame9,9);
	 return TRUE;
}

//查询床运动步长
BOOL COWSSIO::QueryBedStep()
{
	 m_WFrame9.CheckSum = 0;
	 m_WFrame9.tAddr   = ADDR_BED;     m_WFrame9.CheckSum ^= m_WFrame9.tAddr;
	 m_WFrame9.sAddr   = ADDR_ACS;	   m_WFrame9.CheckSum ^= m_WFrame9.sAddr;
	 m_WFrame9.len     = 0x09;         m_WFrame9.CheckSum ^= m_WFrame9.len;
	 m_WFrame9.attr    = 0;
	 m_WFrame9.CmdMain = 0x0D;		   m_WFrame9.CheckSum ^= m_WFrame9.CmdMain;
	 m_WFrame9.CmdSub  = 0x00;  	   m_WFrame9.CheckSum ^= m_WFrame9.CmdSub;
	 m_WFrame9.Dat     = 0x00;
	 m_WFrame9.eof     = 0x7F;

	OWS_HEAD head;
	head.dwDatLen = sizeof(head) + 1 + sizeof(m_WFrame9);
	head.Cmd = PET_TESTREQ;
	head.cFlag[0] = 0x7e;
	head.cFlag[1] = 0x7e;
	head.cFlag[2] = 0x7e;

	BYTE testObj = 41;
	((CSimOWSDlg*)AfxGetMainWnd())->m_ServerSock.SendDatToClient(&head, sizeof(head));
	((CSimOWSDlg*)AfxGetMainWnd())->m_ServerSock.SendDatToClient(&testObj, 1);
	((CSimOWSDlg*)AfxGetMainWnd())->m_ServerSock.SendDatToClient(&m_WFrame9,9);
	 return TRUE;
}

//查询床运动状态
BOOL COWSSIO::QueryBedMoveStatus()
{
	 m_WFrame9.CheckSum = 0;
	 m_WFrame9.tAddr   = ADDR_BED;     m_WFrame9.CheckSum ^= m_WFrame9.tAddr;
	 m_WFrame9.sAddr   = ADDR_ACS;	   m_WFrame9.CheckSum ^= m_WFrame9.sAddr;
	 m_WFrame9.len     = 0x09;         m_WFrame9.CheckSum ^= m_WFrame9.len;
	 m_WFrame9.attr    = 0;
	 m_WFrame9.CmdMain = 0x14;		   m_WFrame9.CheckSum ^= m_WFrame9.CmdMain;
	 m_WFrame9.CmdSub  = 0x00;  	   m_WFrame9.CheckSum ^= m_WFrame9.CmdSub;
	 m_WFrame9.Dat     = 0x00;
	 m_WFrame9.eof     = 0x7F;

	OWS_HEAD head;
	head.dwDatLen = sizeof(head) + 1 + sizeof(m_WFrame9);
	head.Cmd = PET_TESTREQ;
	head.cFlag[0] = 0x7e;
	head.cFlag[1] = 0x7e;
	head.cFlag[2] = 0x7e;

	BYTE testObj = 41;
	((CSimOWSDlg*)AfxGetMainWnd())->m_ServerSock.SendDatToClient(&head, sizeof(head));
	((CSimOWSDlg*)AfxGetMainWnd())->m_ServerSock.SendDatToClient(&testObj, 1);
	((CSimOWSDlg*)AfxGetMainWnd())->m_ServerSock.SendDatToClient(&m_WFrame9,9);

	 return TRUE;
}

//设置床运动步长
BOOL COWSSIO::SetBedStep(WORD step)
{
	 m_WFrame12.tAddr   = ADDR_BED;     m_WFrame12.CheckSum ^= m_WFrame12.tAddr;
	 m_WFrame12.sAddr   = ADDR_ACS;	    m_WFrame12.CheckSum ^= m_WFrame12.sAddr;
	 m_WFrame12.len     = 0x0C;         m_WFrame12.CheckSum ^= m_WFrame12.len;
	 m_WFrame12.attr    = 0;
	 m_WFrame12.CmdMain = 0x06;		    m_WFrame12.CheckSum ^= m_WFrame12.CmdMain;
	 m_WFrame12.CmdSub  = 0x00;         m_WFrame12.CheckSum ^= m_WFrame12.CmdSub;
	 m_WFrame12.Dat[0]  = LOBYTE(step); m_WFrame12.CheckSum ^= m_WFrame12.Dat[0];
	 m_WFrame12.Dat[1]  = 0x00;
	 m_WFrame12.Dat[2]  = 0x00;
	 m_WFrame12.Dat[3]  = 0x00;
	 m_WFrame12.eof     = 0x7F;

	 g_MCtlInfo.wBed_step = step;

	OWS_HEAD head;
	head.dwDatLen = sizeof(head) + 1 + sizeof(m_WFrame12);
	head.Cmd = PET_TESTREQ;
	head.cFlag[0] = 0x7e;
	head.cFlag[1] = 0x7e;
	head.cFlag[2] = 0x7e;

	BYTE testObj = 41;
	((CSimOWSDlg*)AfxGetMainWnd())->m_ServerSock.SendDatToClient(&head, sizeof(head));
	((CSimOWSDlg*)AfxGetMainWnd())->m_ServerSock.SendDatToClient(&testObj, 1);
	((CSimOWSDlg*)AfxGetMainWnd())->m_ServerSock.SendDatToClient(&m_WFrame12,12);
	
	return TRUE;
}

//查询SEPTA开关量设置
BOOL COWSSIO::QuerySeptaSwitchSet()
{
	 m_WFrame12.CheckSum = 0;
	 m_WFrame12.tAddr   = ADDR_SEPTA;   m_WFrame12.CheckSum ^= m_WFrame12.tAddr;
	 m_WFrame12.sAddr   = ADDR_ACS;	    m_WFrame12.CheckSum ^= m_WFrame12.sAddr;
	 m_WFrame12.len     = 0x0C;         m_WFrame12.CheckSum ^= m_WFrame12.len;
	 m_WFrame12.attr    = 0;
	 m_WFrame12.CmdMain = 0xE1;		    m_WFrame12.CheckSum ^= m_WFrame12.CmdMain;
	 m_WFrame12.CmdSub  = 0x00;         m_WFrame12.CheckSum ^= m_WFrame12.CmdSub;
	 m_WFrame12.Dat[0]  = 0x00;         m_WFrame12.CheckSum ^= m_WFrame12.Dat[0];
	 m_WFrame12.Dat[1]  = 0x00;         m_WFrame12.CheckSum ^= m_WFrame12.Dat[1];
	 m_WFrame12.Dat[2]  = 0x00;         m_WFrame12.CheckSum ^= m_WFrame12.Dat[2];
	 m_WFrame12.Dat[3]  = 0x00;         m_WFrame12.CheckSum ^= m_WFrame12.Dat[3];
	 m_WFrame12.eof     = 0x7F;

	 OWS_HEAD head;
	head.dwDatLen = sizeof(head) + 1 + sizeof(m_WFrame12);
	head.Cmd = PET_TESTREQ;
	head.cFlag[0] = 0x7e;
	head.cFlag[1] = 0x7e;
	head.cFlag[2] = 0x7e;

	BYTE testObj = 41;
	((CSimOWSDlg*)AfxGetMainWnd())->m_ServerSock.SendDatToClient(&head, sizeof(head));
	((CSimOWSDlg*)AfxGetMainWnd())->m_ServerSock.SendDatToClient(&testObj, 1);
	((CSimOWSDlg*)AfxGetMainWnd())->m_ServerSock.SendDatToClient(&m_WFrame12,12);
	 return TRUE;
}

//控制SEPTA伸出
BOOL COWSSIO::Control_SeptaOut()
{
	 m_WFrame9.CheckSum = 0;
	 m_WFrame9.tAddr   = ADDR_SEPTA;   m_WFrame9.CheckSum ^= m_WFrame9.tAddr;
	 m_WFrame9.sAddr   = ADDR_ACS;	   m_WFrame9.CheckSum ^= m_WFrame9.sAddr;
	 m_WFrame9.len     = 0x09;         m_WFrame9.CheckSum ^= m_WFrame9.len;
	 m_WFrame9.attr    = 0;
	 m_WFrame9.CmdMain = 0x01;		   m_WFrame9.CheckSum ^= m_WFrame9.CmdMain;
	 m_WFrame9.CmdSub  = 0x00;         m_WFrame9.CheckSum ^= m_WFrame9.CmdSub;
	 m_WFrame9.Dat     = 0x00;         m_WFrame9.CheckSum ^= m_WFrame9.Dat;
	 m_WFrame9.eof     = 0x7F;

	OWS_HEAD head;
	head.dwDatLen = sizeof(head) + 1 + sizeof(m_WFrame9);
	head.Cmd = PET_TESTREQ;
	head.cFlag[0] = 0x7e;
	head.cFlag[1] = 0x7e;
	head.cFlag[2] = 0x7e;

	BYTE testObj = 41;
	((CSimOWSDlg*)AfxGetMainWnd())->m_ServerSock.SendDatToClient(&head, sizeof(head));
	((CSimOWSDlg*)AfxGetMainWnd())->m_ServerSock.SendDatToClient(&testObj, 1);
	((CSimOWSDlg*)AfxGetMainWnd())->m_ServerSock.SendDatToClient(&m_WFrame9,9);

	return TRUE;
}

//控制SEPTA停止
BOOL COWSSIO::Control_SeptaStop()
{
	 m_WFrame9.CheckSum = 0;
	 m_WFrame9.tAddr   = ADDR_SEPTA;   m_WFrame9.CheckSum ^= m_WFrame9.tAddr;
	 m_WFrame9.sAddr   = ADDR_ACS;	   m_WFrame9.CheckSum ^= m_WFrame9.sAddr;
	 m_WFrame9.len     = 0x09;         m_WFrame9.CheckSum ^= m_WFrame9.len;
	 m_WFrame9.attr    = 0;
	 m_WFrame9.CmdMain = 0x02;		   m_WFrame9.CheckSum ^= m_WFrame9.CmdMain;
	 m_WFrame9.CmdSub  = 0x02;         m_WFrame9.CheckSum ^= m_WFrame9.CmdSub;
	 m_WFrame9.Dat     = 0x00;         m_WFrame9.CheckSum ^= m_WFrame9.Dat;
	 m_WFrame9.eof     = 0x7F;

	OWS_HEAD head;
	head.dwDatLen = sizeof(head) + 1 + sizeof(m_WFrame9);
	head.Cmd = PET_TESTREQ;
	head.cFlag[0] = 0x7e;
	head.cFlag[1] = 0x7e;
	head.cFlag[2] = 0x7e;

	BYTE testObj = 41;
	((CSimOWSDlg*)AfxGetMainWnd())->m_ServerSock.SendDatToClient(&head, sizeof(head));
	((CSimOWSDlg*)AfxGetMainWnd())->m_ServerSock.SendDatToClient(&testObj, 1);
	((CSimOWSDlg*)AfxGetMainWnd())->m_ServerSock.SendDatToClient(&m_WFrame9,9);

	return TRUE;
}

//控制SEPTA紧急停止
BOOL COWSSIO::Control_SeptaEStop()
{
	 m_WFrame9.CheckSum = 0;
	 m_WFrame9.tAddr   = ADDR_SEPTA;   m_WFrame9.CheckSum ^= m_WFrame9.tAddr;
	 m_WFrame9.sAddr   = ADDR_ACS;	   m_WFrame9.CheckSum ^= m_WFrame9.sAddr;
	 m_WFrame9.len     = 0x09;         m_WFrame9.CheckSum ^= m_WFrame9.len;
	 m_WFrame9.attr    = 0;
	 m_WFrame9.CmdMain = 0xEE;		   m_WFrame9.CheckSum ^= m_WFrame9.CmdMain;
	 m_WFrame9.CmdSub  = 0x00;         m_WFrame9.CheckSum ^= m_WFrame9.CmdSub;
	 m_WFrame9.Dat     = 0x00;         m_WFrame9.CheckSum ^= m_WFrame9.Dat;
	 m_WFrame9.eof     = 0x7F;

	OWS_HEAD head;
	head.dwDatLen = sizeof(head) + 1 + sizeof(m_WFrame9);
	head.Cmd = PET_TESTREQ;
	head.cFlag[0] = 0x7e;
	head.cFlag[1] = 0x7e;
	head.cFlag[2] = 0x7e;

	BYTE testObj = 41;
	((CSimOWSDlg*)AfxGetMainWnd())->m_ServerSock.SendDatToClient(&head, sizeof(head));
	((CSimOWSDlg*)AfxGetMainWnd())->m_ServerSock.SendDatToClient(&testObj, 1);
	((CSimOWSDlg*)AfxGetMainWnd())->m_ServerSock.SendDatToClient(&m_WFrame9,9);

	return TRUE;
}

//控制SEPTA缩回
BOOL COWSSIO::Control_SeptaIn()
{
	 m_WFrame9.CheckSum = 0;
	 m_WFrame9.tAddr   = ADDR_SEPTA;   m_WFrame9.CheckSum ^= m_WFrame9.tAddr;
	 m_WFrame9.sAddr   = ADDR_ACS;	   m_WFrame9.CheckSum ^= m_WFrame9.sAddr;
	 m_WFrame9.len     = 0x09;         m_WFrame9.CheckSum ^= m_WFrame9.len;
	 m_WFrame9.attr    = 0;
	 m_WFrame9.CmdMain = 0x03;		   m_WFrame9.CheckSum ^= m_WFrame9.CmdMain;
	 m_WFrame9.CmdSub  = 0x03;         m_WFrame9.CheckSum ^= m_WFrame9.CmdSub;
	 m_WFrame9.Dat     = 0x00;         m_WFrame9.CheckSum ^= m_WFrame9.Dat;
	 m_WFrame9.eof     = 0x7F;
	
	 OWS_HEAD head;
	head.dwDatLen = sizeof(head) + 1 + sizeof(m_WFrame9);
	head.Cmd = PET_TESTREQ;
	head.cFlag[0] = 0x7e;
	head.cFlag[1] = 0x7e;
	head.cFlag[2] = 0x7e;

	BYTE testObj = 41;
	((CSimOWSDlg*)AfxGetMainWnd())->m_ServerSock.SendDatToClient(&head, sizeof(head));
	((CSimOWSDlg*)AfxGetMainWnd())->m_ServerSock.SendDatToClient(&testObj, 1);
	((CSimOWSDlg*)AfxGetMainWnd())->m_ServerSock.SendDatToClient(&m_WFrame9,9);

	return TRUE;
}

BOOL COWSSIO::Dbg_GCPollingEnable(BOOL bEnable)
{
     m_WFrame9.CheckSum = 0x00;
	 m_WFrame9.tAddr   = ADDR_GANTRY;  m_WFrame9.CheckSum ^= m_WFrame9.tAddr;
	 m_WFrame9.sAddr   = ADDR_ACS;	   m_WFrame9.CheckSum ^= m_WFrame9.sAddr;
	 m_WFrame9.len     = 0x09;         m_WFrame9.CheckSum ^= m_WFrame9.len;
	 m_WFrame9.attr    = 0;
	 m_WFrame9.CmdMain = 0xB2;		   m_WFrame9.CheckSum ^= m_WFrame9.CmdMain;
	 if(bEnable) m_WFrame9.CmdSub = 1;		   
	 else        m_WFrame9.CmdSub = 0;
									   m_WFrame9.CheckSum ^= m_WFrame9.CmdSub;
	 m_WFrame9.Dat      = 0x03;        m_WFrame9.CheckSum ^= m_WFrame9.Dat;
	 m_WFrame9.eof      = 0x7F;

	OWS_HEAD head;
	head.dwDatLen = sizeof(head) + 1 + sizeof(m_WFrame9);
	head.Cmd = PET_TESTREQ;
	head.cFlag[0] = 0x7e;
	head.cFlag[1] = 0x7e;
	head.cFlag[2] = 0x7e;

	BYTE testObj = 41;
	((CSimOWSDlg*)AfxGetMainWnd())->m_ServerSock.SendDatToClient(&head, sizeof(head));
	((CSimOWSDlg*)AfxGetMainWnd())->m_ServerSock.SendDatToClient(&testObj, 1);
	((CSimOWSDlg*)AfxGetMainWnd())->m_ServerSock.SendDatToClient(&m_WFrame9,9);
	 return TRUE;
}



