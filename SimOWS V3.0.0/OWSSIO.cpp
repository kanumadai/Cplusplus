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
MCTL_INFO   g_MCtlInfo;             //������Ƶ�·�����Ϣ

//
////////////////////////////////////////////////////////////////////

void UpdateDisplay();

void UpdateDisplay()
{
	char szTemp[512];

	switch( g_MCtlInfo.Rod_stat ){
	case unknow:
		 sprintf( szTemp, "��Դ״̬:δ֪" );      break;
    case out:	    //�����λ
		 sprintf( szTemp, "��Դ״̬:�����λ" );  break;
    case in:		//���ص�λ
		 sprintf( szTemp, "��Դ״̬:���ص�λ" );  break;
    case outing:	//�����
		 sprintf( szTemp, "��Դ״̬:���������" );  break;
    case ining:		//������
		 sprintf( szTemp, "��Դ״̬:���ع�����" );  break;
    case rolling:   //������ת
		 sprintf( szTemp, "��Դ״̬:������ת" );  break;
    case stop:		//ֹͣ
		 sprintf( szTemp, "��Դ״̬:ֹͣ" );  break;
	default:
		 sprintf( szTemp, "��Դ״̬:XXX"); break;
	}

	switch( g_MCtlInfo.septa_stat ){
	case unknow:
		 sprintf( szTemp, "%s    Septa״̬:δ֪", szTemp );      break;
    case out:	    //�����λ
		 sprintf( szTemp, "%s    Septa״̬:�����λ", szTemp );  break;
    case in:		//���ص�λ
		 sprintf( szTemp, "%s    Septa״̬:���ص�λ", szTemp );  break;
    case outing:	//�����
		 sprintf( szTemp, "%s    Septa״̬:���������", szTemp );  break;
    case ining:		//������
		 sprintf( szTemp, "%s    Septa״̬:���ع�����", szTemp );  break;
    case rolling:   //������ת
		 sprintf( szTemp, "%s    Septa״̬:������ת", szTemp );  break;
    case stop:		//ֹͣ
		 sprintf( szTemp, "%s    Septa״̬:ֹͣ", szTemp );  break;
	default:
		 sprintf( szTemp, "%s    Septa״̬:XXX"); break;
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
		case 0xA0:  // GANTRY����Ӧ��
			 DbgPrt("GANTRY HandShake Answer\r\n", g_hWndOWS);
			 break;

		case 0xA5:  //����������ЧӦ��
			 DbgPrt("Set Data Valid\r\n", g_hWndOWS);
			 break;

		case 0xE0:  // �����ط�����
			 DbgPrt("Require to Send Again\r\n", g_hWndOWS);
			 break;

		case 0xB3:  //�ش��¶ȣ�GANTRY��ROD��YE12��SEPTA��BED��
			 EnterCriticalSection(&g_CLockMCtlInfo);
			    switch( pFrame->CmdSub )
				{
				case ADDR_GANTRY:
				     //Gantry�¶�
			 		 g_MCtlInfo.wT_GANTRY = MAKEWORD(pFrame->Dat[0], pFrame->Dat[1]);
					 #if DBG_LEVEL != 0
						sprintf(szTemp,"Upload GANTRY T=%4.4x\r\n", g_MCtlInfo.wT_GANTRY);
					 #endif
					 break;

				case ADDR_ROD:
					 //ROD�¶�
					 g_MCtlInfo.wT_Rod = MAKEWORD(pFrame->Dat[0], pFrame->Dat[1]);
					 #if DBG_LEVEL != 0
						sprintf(szTemp,"Upload ROD T=%4.4x\r\n", g_MCtlInfo.wT_Rod);
					 #endif
					 break;

				case ADDR_BED:
					 //BED�¶�
					 g_MCtlInfo.wT_Bed = MAKEWORD(pFrame->Dat[0], pFrame->Dat[1]);
					 #if DBG_LEVEL != 0
						sprintf(szTemp,"Upload BED T=%4.4x\r\n", g_MCtlInfo.wT_Bed);
					 #endif
					 break;

				case ADDR_SEPTA:
					 //SEPTA�¶�
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

		case 0xC0:	// �ش�WATERCOOL�¶�
			 EnterCriticalSection(&g_CLockMCtlInfo);
				if(pFrame->CmdSub==0x20)
				{   //WaterCool�¶�
					g_MCtlInfo.wT_WATERCOOL = MAKEWORD(pFrame->Dat[0], pFrame->Dat[1]);
					#if DBG_LEVEL != 0
						sprintf(szTemp,"Upload WaterCool T=%4.4x\r\n", g_MCtlInfo.wT_WATERCOOL);
					#endif
				}
				DbgPrt(szTemp, g_hWndOWS);
			 LeaveCriticalSection(&g_CLockMCtlInfo);

			 break;

		case 0xC1:  // �ش���ѹ������ֵ
			 EnterCriticalSection(&g_CLockMCtlInfo);
			     
			     g_MCtlInfo.HVStatus = pFrame->CmdSub;

				 g_MCtlInfo.wV_1 = MAKEWORD( pFrame->Dat[0], pFrame->Dat[1] );
				 g_MCtlInfo.wI_1 = MAKEWORD( pFrame->Dat[2], pFrame->Dat[3] );
					 #if DBG_LEVEL != 0
					    sprintf(szTemp,"Upload HV Status=%d, HV=%d��HVI=%dx\r\n", g_MCtlInfo.HVStatus, g_MCtlInfo.wV_1,g_MCtlInfo.wI_1);
					 #endif
				 DbgPrt(szTemp, g_hWndOWS);

			 LeaveCriticalSection(&g_CLockMCtlInfo);

			 break;

		/*prev protocol
		case 0xC1:  // �ش���ѹ������ֵ
			 EnterCriticalSection(&g_CLockMCtlInfo);
			     
			     ASSERT( pFrame->CmdSub==0x10 || pFrame->CmdSub==0x20 );

				 if(pFrame->CmdSub == 0x10){       //��·��ѹ
					 g_MCtlInfo.wV_1 = MAKEWORD( pFrame->Dat[0], pFrame->Dat[1] );
					 g_MCtlInfo.wV_2 = MAKEWORD( pFrame->Dat[2], pFrame->Dat[3] );
					 #if DBG_LEVEL != 0
					    sprintf(szTemp,"Upload Two HV=%4.4x��%4.4x\r\n", g_MCtlInfo.wV_1,g_MCtlInfo.wV_2);
					 #endif
				 }else if(pFrame->CmdSub == 0x20){  //��·����
					 g_MCtlInfo.wI_1 = MAKEWORD( pFrame->Dat[0], pFrame->Dat[1] );
					 g_MCtlInfo.wI_2 = MAKEWORD( pFrame->Dat[2], pFrame->Dat[3] );
					 #if DBG_LEVEL != 0
					    sprintf(szTemp,"Upload Two HV I=%4.4x��%4.4x\r\n", g_MCtlInfo.wI_1,g_MCtlInfo.wI_2);
					 #endif
				 }
				 DbgPrt(szTemp, g_hWndOWS);

			 LeaveCriticalSection(&g_CLockMCtlInfo);

			 break;
		*/

		case 0xC2:	//��ѹ����Ӧ��
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

		case 0xC3:  //����ƿ���Ӧ��
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

		case 0xC4:  //ǰ������Դ����Ӧ��
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

		case 0xC5:  // �ش���ѹ�߽�
			 EnterCriticalSection(&g_CLockMCtlInfo);
			     
				 ASSERT(pFrame->CmdSub == 0x10);

				 g_MCtlInfo.wTH_V_H = MAKEWORD( pFrame->Dat[0], pFrame->Dat[1] );//��ѹ����
				 g_MCtlInfo.wTH_V_L = MAKEWORD( pFrame->Dat[2], pFrame->Dat[3] );//��ѹ����
				 #if DBG_LEVEL != 0
				    sprintf(szTemp,"Upload HV Threshold=%4.4x��%4.4x\r\n", g_MCtlInfo.wTH_V_H,g_MCtlInfo.wTH_V_L);
				 #endif
				 DbgPrt(szTemp, g_hWndOWS);

			 LeaveCriticalSection(&g_CLockMCtlInfo);

			 break;

		case 0xC6:  // �ش������߽�
			 EnterCriticalSection(&g_CLockMCtlInfo);
			     
				g_MCtlInfo.wTH_I_H = MAKEWORD( pFrame->Dat[0], pFrame->Dat[1] );//��������
				g_MCtlInfo.wTH_I_L = MAKEWORD( pFrame->Dat[2], pFrame->Dat[3] );//��������
				#if DBG_LEVEL != 0
				    sprintf(szTemp,"Upload HV I Threshold=%4.4x��%4.4x\r\n", g_MCtlInfo.wTH_I_H,g_MCtlInfo.wTH_I_L);
				#endif
				DbgPrt(szTemp, g_hWndOWS);

			 LeaveCriticalSection(&g_CLockMCtlInfo);

			 break;

		case 0xC7:  // �ش�WATERCOOL�߽�
			 EnterCriticalSection(&g_CLockMCtlInfo);

				g_MCtlInfo.wTH_WATERCOOLT_H = MAKEWORD( pFrame->Dat[0], pFrame->Dat[1] );//����
				g_MCtlInfo.wTH_WATERCOOLT_L = MAKEWORD( pFrame->Dat[2], pFrame->Dat[3] );//����
				#if DBG_LEVEL != 0
				    sprintf(szTemp,"Upload WaterCool T Threshold=%4.4x��%4.4x\r\n", g_MCtlInfo.wTH_WATERCOOLT_H,g_MCtlInfo.wTH_WATERCOOLT_L);
				#endif
				DbgPrt(szTemp, g_hWndOWS);

			 LeaveCriticalSection(&g_CLockMCtlInfo);

			 break;

		case 0xC8:  // �ش�GANTRY�¶ȱ߽�
			 EnterCriticalSection(&g_CLockMCtlInfo);

				g_MCtlInfo.wTH_GANTRYT_H = MAKEWORD( pFrame->Dat[0], pFrame->Dat[1] );//����
				g_MCtlInfo.wTH_GANTRYT_L = MAKEWORD( pFrame->Dat[2], pFrame->Dat[3] );//����
				#if DBG_LEVEL != 0
				    sprintf(szTemp,"Upload GANTRY T Threshold=%4.4x��%4.4x\r\n", g_MCtlInfo.wTH_GANTRYT_H,g_MCtlInfo.wTH_GANTRYT_L);
				#endif
				DbgPrt(szTemp, g_hWndOWS);

			 LeaveCriticalSection(&g_CLockMCtlInfo);

			 break;

		case 0x04:  // ��Դ��Ϣ
			 EnterCriticalSection(&g_CLockMCtlInfo);
				
				if(pFrame->CmdSub == 0x00){      //��Դ�������

					g_MCtlInfo.Rod_stat = outing;
					DbgPrt("��Դ�������\r\n", g_hWndOWS);

				}else if(pFrame->CmdSub == 0x01){ //��Դ�����λ

					g_MCtlInfo.Rod_stat = out;
					DbgPrt("��Դ�����λ\r\n", g_hWndOWS);

				}else if(pFrame->CmdSub == 0x02){ //��Դ�����ջػ���ת
					
					if( pFrame->Dat[0] == 0x00 ){ //�����ջ�
						g_MCtlInfo.Rod_stat = ining;
						DbgPrt("��Դ�����ջ�\r\n", g_hWndOWS);
					}else if( pFrame->Dat[0] == 0x01){ //������ת
						g_MCtlInfo.Rod_stat = rolling;
						DbgPrt("��Դ��ʼ��ת\r\n", g_hWndOWS);
					}else
						ASSERT(FALSE);

				}else if(pFrame->CmdSub == 0x03){ //��Դ�ջص�λ

					g_MCtlInfo.Rod_stat = in;
					DbgPrt("��Դ�ջص�λ\r\n", g_hWndOWS);

				}else if(pFrame->CmdSub == 0x04){ //��Դֹͣ
					g_MCtlInfo.Rod_stat = stop;
					DbgPrt("��Դֹͣ\r\n", g_hWndOWS);

				}else{							  //��������
					ASSERT(FALSE);
				}
			 LeaveCriticalSection(&g_CLockMCtlInfo);
			 
			 break;

		case 0x0A:  // �ش���Դת��
			 EnterCriticalSection(&g_CLockMCtlInfo);
				 
				 g_MCtlInfo.wRod_rate = MAKEWORD( pFrame->Dat[0], pFrame->Dat[1] );
				 #if DBG_LEVEL != 0
					sprintf(szTemp, "Upload Rod Rate=%4.4x\r\n", g_MCtlInfo.wRod_rate);
				 #endif
				 DbgPrt(szTemp, g_hWndOWS);

			 LeaveCriticalSection(&g_CLockMCtlInfo);

			 break;

		case 0xE1:    //�ϴ�BED SPETA ROD����������
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

		case 0x0C:  // �ش�������
			 EnterCriticalSection(&g_CLockMCtlInfo);

			     g_MCtlInfo.wBed_posx = MAKEWORD(pFrame->Dat[0], pFrame->Dat[1]);
			     g_MCtlInfo.wBed_posy = MAKEWORD(pFrame->Dat[2], pFrame->Dat[3]);
				 #if DBG_LEVEL != 0
					sprintf(szTemp, "Upload Bed Pos��x=%4.4x(%d), y=%4.4x(%d)\r\n", g_MCtlInfo.wBed_posx,g_MCtlInfo.wBed_posx,g_MCtlInfo.wBed_posy,g_MCtlInfo.wBed_posy);
				 #endif
				 DbgPrt(szTemp, g_hWndOWS);

			 LeaveCriticalSection(&g_CLockMCtlInfo);
			 
			 break;

		case 0x14:  //�ش����˶�״̬
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

		case 0x0D:  // �ش�������
			 EnterCriticalSection(&g_CLockMCtlInfo);
			     g_MCtlInfo.wBed_step = MAKEWORD(pFrame->Dat[0], pFrame->Dat[1]);
				 #if DBG_LEVEL!=0
				    sprintf(szTemp, "Upload Bed Step Len��0x%4.4x\r\n", g_MCtlInfo.wBed_step);
					DbgPrt(szTemp, g_hWndOWS);
				 #endif
			 LeaveCriticalSection(&g_CLockMCtlInfo);
			 
			 break;

		case 0x0F:  // �����/����������ͨ��
			 EnterCriticalSection(&g_CLockMCtlInfo);

				if( pFrame->CmdSub == 0x0 ){        //���������
					 g_MCtlInfo.Bed_stat = out_stop;
					 DbgPrt("Bed Out Limited\r\n", g_hWndOWS);
				}else{/*pFrame->CmdSub == 0x01*/     //�ջص�����
					 g_MCtlInfo.Bed_stat = in_stop;
					 DbgPrt("Bed In Limited\r\n", g_hWndOWS);
				}

			 LeaveCriticalSection(&g_CLockMCtlInfo);

			 break;

		case 0xA1:    //�ϴ�������������
			 EnterCriticalSection(&g_CLockMCtlInfo);

				g_MCtlInfo.wBed_SwitchSet = MAKEWORD(pFrame->Dat[0], pFrame->Dat[1]);
				DbgPrt("Upload Bed SwitchSet\r\n", g_hWndOWS);

			 LeaveCriticalSection(&g_CLockMCtlInfo);

			 break;

		case 0xE7:   //����ʼ����
			 EnterCriticalSection(&g_CLockMCtlInfo);

				g_MCtlInfo.Bed_stat = move;
				DbgPrt("Bed Begin Adjust To Zero\r\n", g_hWndOWS);

			 LeaveCriticalSection(&g_CLockMCtlInfo);
			 break;

		case 0xA8:   //���˶���ʼ
			 EnterCriticalSection(&g_CLockMCtlInfo);

			 	g_MCtlInfo.Bed_stat = move;
				DbgPrt("Bed Moveing\r\n", g_hWndOWS);

			 LeaveCriticalSection(&g_CLockMCtlInfo);

			 break;

		case 0xA7:	 //���˶�����
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

		case 0x05:  //���ô�����ϵͳӦ��
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

		case 0x06:  //���ô�����Ӧ��
			 EnterCriticalSection(&g_CLockMCtlInfo);
				g_MCtlInfo.wBed_step = pFrame->Dat[0];
				#if DBG_LEVEL != 0
					sprintf(szTemp, "Bed Step Len��%2.2x\r\n", g_MCtlInfo.wBed_step);
				#endif
				DbgPrt(szTemp, g_hWndOWS);
			 LeaveCriticalSection(&g_CLockMCtlInfo);

			 break;

		case 0x0E:  // �����״̬ͨ��
			 DbgPrt("Upload Bed Meshine StatueXXXXXXXXXX\r\n", g_hWndOWS);
			 break;

		case 0x01:  // septa��Ϣ
			 EnterCriticalSection(&g_CLockMCtlInfo);
				if( pFrame->CmdSub == 0x00 ){      //�������

					 g_MCtlInfo.septa_stat = outing;
					 DbgPrt("SEPTA �������\r\n", g_hWndOWS);

				}else if( pFrame->CmdSub == 0x01 ){ //�����λ

					 g_MCtlInfo.septa_stat = out;
					 DbgPrt("SEPTA out on Position�����λ\r\n", g_hWndOWS);

				}else if( pFrame->CmdSub == 0x02 ){ //��������

					 g_MCtlInfo.septa_stat = ining;
					 DbgPrt("SEPTA ��������\r\n", g_hWndOWS);

				}else if( pFrame->CmdSub == 0x03 ){ //���ص�λ

					 g_MCtlInfo.septa_stat = in;
					 DbgPrt("SEPTA in On Position\r\n", g_hWndOWS);

				}else if( pFrame->CmdSub == 0x04 ){ //����ֹͣ

					 g_MCtlInfo.septa_stat = stop;
					 DbgPrt("SEPTA stop\r\n", g_hWndOWS);

				}else{							   //��������
					 ASSERT(FALSE);               
				}
			 LeaveCriticalSection(&g_CLockMCtlInfo);

			 break;

		case 0x71:    //�ϴ�SEPTA����������
			 EnterCriticalSection(&g_CLockMCtlInfo);
				
				g_MCtlInfo.wSepta_SwitchSet = MAKEWORD(pFrame->Dat[0], pFrame->Dat[1]);
				DbgPrt("Upload SEPTA SwitchSet\r\n", g_hWndOWS);

			 LeaveCriticalSection(&g_CLockMCtlInfo);

			 break;

		case 0xB4:  // �弶��ѯ��ʱ����
			 EnterCriticalSection(&g_CLockMCtlInfo);
				switch(pFrame->CmdSub)
				{
				case 0:
					 DbgPrt("�弶��ѯ����\r\n", g_hWndOWS);
					 break;
				case ADDR_GANTRY:
					 DbgPrt("GANTRY�弶��ѯ����\r\n", g_hWndOWS);
					 break;
				case ADDR_BED:
					 DbgPrt("BED�弶��ѯ����\r\n", g_hWndOWS);
					 break;
				case ADDR_ROD:
					 DbgPrt("ROD�弶��ѯ����\r\n", g_hWndOWS);
					 break;
				case ADDR_SEPTA:
					 DbgPrt("SEPTA�弶��ѯ����\r\n", g_hWndOWS);
					 break;
				default:
					 DbgPrt("�弶��ѯ����,���ַδ֪\r\n", g_hWndOWS);
					 break;					 
				}
			 LeaveCriticalSection(&g_CLockMCtlInfo);
			 break;

		case 0xEE:  // ����ֹͣͨ��
			 EnterCriticalSection(&g_CLockMCtlInfo);
				 if(pFrame->CmdSub == 0x01)
				 {
					 g_MCtlInfo.alarm_ESTOP = alarm;
					 DbgPrt("����ֹͣ\r\n", g_hWndOWS);
				 }
				 else
				 {
					 g_MCtlInfo.alarm_ESTOP = normal;
					 DbgPrt("����ֹͣ��λ\r\n", g_hWndOWS);
				 }

			 LeaveCriticalSection(&g_CLockMCtlInfo);

			 break;

		case 0xB1:    //����
			 EnterCriticalSection(&g_CLockMCtlInfo);
				switch(pFrame->CmdSub)
				{
				case ADDR_GANTRY:
					 g_MCtlInfo.alarm_Gantry = alarm;
					 g_MCtlInfo.ac_Gantry.code8 = pFrame->Dat[0];
					 #if DBG_LEVEL != 0
						sprintf(szTemp, "GANTRY������%2.2X\r\n", pFrame->Dat[0]);
					 #endif
					 break;
				case ADDR_ROD:
					 g_MCtlInfo.alarm_Rod = alarm;
					 g_MCtlInfo.ac_Rod.code8 = pFrame->Dat[0];
					 #if DBG_LEVEL != 0
						sprintf(szTemp, "ROD������%2.2X\r\n", pFrame->Dat[0]);
					 #endif
					 break;
				case ADDR_BED:
					 g_MCtlInfo.alarm_Bed = alarm;
					 g_MCtlInfo.ac_Bed.code8 = pFrame->Dat[0];
					 #if DBG_LEVEL != 0
						sprintf(szTemp, "Bed������%2.2X\r\n", pFrame->Dat[0]);
					 #endif
					 break;
				case ADDR_SEPTA:
					 g_MCtlInfo.alarm_Septa = alarm;
					 g_MCtlInfo.ac_Septa.code8 = pFrame->Dat[0];
					 #if DBG_LEVEL != 0
						sprintf(szTemp, "SEPTA������%2.2X\r\n", pFrame->Dat[0]);
					 #endif
					 break;
				default:
					 #if DBG_LEVEL != 0
						sprintf(szTemp, "%2.2X������%2.2X\r\n", pFrame->CmdSub, pFrame->Dat[0]);
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

//��ѯGANTRY�¶�
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

//��ѯ��Դ�¶�
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

//��ѯSEPTA�¶�
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

//��ѯ���¶�
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

//��ѯˮ����¶�
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

//���Ƹ�ѹͨ����
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

//���Ƽ����ͨ����
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

//����ǰ������Դͨ����
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

//��ѯ��ѹ������
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

//���ø�ѹ������
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

//��ѯ��������
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

//���õ���������
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

//��ѯˮ����¶�������
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

//����ˮ����¶�������
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

//��ѯGANTRY�¶�������
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

//����GANTRY�¶�������
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

//���ð�Դת��
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
//��ѯSEPTA״̬
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

//��ѯ��Դ״̬
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

//��ѯ��Դת��
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

//��ѯ��Դ����������
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

//���ư�Դ�����ת��
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

//���ư�Դ���������״̬�¿�ʼ��ת
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

//���ư�Դֹͣת��
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

//���ư�Դ�ջ�
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

//���ư�Դ����ֹͣ
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

//���ƴ�ˮƽ�˶��������˶�
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

//���ƴ�����˶�
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

//���ƴ�Y�����˶�
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

//���ƴ�Y����˶�
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

//���ô����귽ʽ��TRUE=>��Զ�λ��FALSE=>���Զ�λ
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

//��ѯ������������
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

//��ѯ��XY����
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

//��ѯ���˶�����
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

//��ѯ���˶�״̬
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

//���ô��˶�����
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

//��ѯSEPTA����������
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

//����SEPTA���
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

//����SEPTAֹͣ
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

//����SEPTA����ֹͣ
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

//����SEPTA����
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



