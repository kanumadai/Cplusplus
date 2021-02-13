========================================================================
       MICROSOFT FOUNDATION CLASS LIBRARY : ACQCenter
========================================================================

//////////////////////////////////////////////////////////////////////
//
// Project:      AcqCenter
//
// Purpose:
//    PET Acq Control
//
// Environment:
//    Windows NT4.0+sp5    Visual C++ 6.0
//
// Author: Zoujie
//
//  *****************  Version 3.4.0.2  *****************
//  User: zoujie          Date: 2008.10.10   Time: 	
//
//  1. ACS-GANTRY SIO protocol append a debug message, use to make sure msg form ACS to GANTRY is OK.
//     Message format :  0xBB  0XA0  0X0C  0xXX  0xXX  0xXX  0xD1 0xD2 0xD3 0xD4 0xChecksum 0x7F
//
//
//
//  *****************  Version 3.4.0.1  *****************
//  User: zoujie          Date: 2008.3.31   Time: 	
//
//  1. BUG FIX: when PET_STOP, immedate send PET_ALLPROTOCOL_OVER, cause ACS control ROD&SEPTA error
//
//
//  *****************  Version 3.4.0.0  *****************
//  User: zoujie          Date: 2008.3.5   Time: 	
//
//  1. support OWS<->ACS protocol PET_QUIT
//
//  *****************  Version 3.3.3.5  *****************
//  User: zoujie          Date: 2007.4.27   Time: 	
//
//  1.  OWS<->ACS protocol (2006-9-15) change
//      a) add support to PET_SYNTIME
//      b) value of PET_LISTMODE change from 30 to 80
//  
//
//  *****************  Version 3.3.3.4  *****************
//  User: louhuimin        Date: 2006.4.12   Time: 	
//
//  1.  BUF FIX: Bucket Sio baudrate shows on dialog error since 
//               default baudrate change from 9600 to 38400
//
//
//
//  *****************  Version 3.3.3.3  *****************
//  User: louhuimin        Date: 2004.2.24   Time: 	
//
//  1.  Support DbgPrt socket
//
//
//  *****************  Version 3.3.3.2  *****************
//  User: louhuimin        Date: 2004.2.11   Time: 16:40	
//
//  1.  Change Bucket selftest time from 5s to 20s.
//  2.  Change Bucket buad rate from 9600bps to 38400bps.
//
//
//  *****************  Version 3.3.3.1  *****************
//  User: zoujie        Date: 2004.02.16    Time: 	
//
//  1.  Start Send PET_MONITOR&PET_SYSINFO Thread When COWS::Connect
//
//
//  *****************  Version 3.3.3.0  *****************
//  User: zoujie        Date: 2004.02.11    Time: 	
//
//  1.  Support CFHSim
//
//
//  *****************  Version 3.3.2.2  *****************
//  User: zoujie        Date: 2003.11.18    Time: 	
//
//  1.  Error Code Header file change from ver2.0 to ver2.1
//      alarm code include alarm detail now.
//
//
//  *****************  Version 3.3.2.1  *****************
//  User: zoujie        Date: 2003.11.17    Time: 	
//
//  1.  Acq Protocol RingDifference As FOV; Span As RingDifference
//                    
//
//
//  *****************  Version 3.3.2.0  *****************
//  User: zoujie        Date: 2003.11.14    Time: 	
//
//  1.  When Acq Failed, Reset ROD and SEPTA;
//  2.  When Rod Rate didn't change, no need to set rod rate;
//  3.  Change the control order of SEPTA and ROD;
//  4.  If Target Bed Pos=Current Bed Pos, then skip Bed Control
//  5.  Show EMC&Bucket Status In Dialog;
//     
//
//
//  *****************  Version 3.3.1.1  *****************
//  User: zoujie        Date: 2003.11.13    Time: 	
//
//  1.  Change All Resource Language To English. 
//
//
//  *****************  Version 3.3.1.0  *****************
//  User: zoujie        Date: 2003.11.12    Time: 	
//
//  1.  Bug Fixed : When HV on, still control HV on , EMC answer with QueryHVStatus, 
//                  not the answer of ControlHVON;
//  2.  Bug Fixed : PET_TESTREQ Segment OBJ should be 1BYTE;
//  3.  Bug Fixed : When Exec Bed Control In Protocol, float to int conver error;
//  4.  Bug Fixed : CBuckSIO::ControlAcqBegin protocol document error;
//  5.  Edit CSIO, Add Target Board Address : ADDR_HV;
//      Edit struct MCTL_INFO, add member alarm_HV and ac_HV;
//      Add Error Code ERROR_HV_ALARM;
//  6.  Edit CBuckSio, Support change Baudrate Online;
//      Edit CBuckSIO::OpenSIO, SetupComm in Buffer size set to 16*1024;
//  7.  Show Protocol Detail in a seprate Edit Control.
//  
//
//  *****************  Version 3.3.0.0  *****************
//  User: zoujie        Date: 2003.11.07    Time: 	
//
//  1.  Edit CSIO, Add CBuckSIO;
//  2.  Edit PET_MONITOR;
//  3.  Add  PET_STATUS;
//  4.  Add  PET_ALLPROTOCOLOVER
//  5.  Error Code Header file change from ver1.0 to ver2.0
//
//
//  *****************  Version 3.2.1.1  *****************
//  User: zoujie        Date: 2003.10.27    Time: 	
//
//  1.  Bug: Exec Protocol, judge ROD/Septa Status Error.
//  2.  Bug: PET_ALARM, alarm code change from 4Bytes To 2Bytes.
//  3.  Add Protocol : PET_FRAMESTART
//
//
//  *****************  Version 3.2.1.0  *****************
//  User: zoujie        Date: 2003.10.24    Time: 	
//
//  1.  coin data fmt control only by acq protocol(nTotalTime Segment), 
//      neither by GUI, nor by PET_INIT protocol.
//
//
//  *****************  Version 3.2.0.4  *****************
//  User: zoujie        Date: 2003.10.13    Time: 	
//
//  1.  Bug Fix: Multi-Bed Control Protocol Exec
//
//
//  *****************  Version 3.2.0.3  *****************
//  User: zoujie        Date: 2003.08.20    Time: 	
//
//  1.  EMControl Protocol Change:
//      CSIO::QueryHV,QueryI Obsolete, should use CSIO::QueryHVStatus
//      Add Bed Protocol Adjust Zeor(CmdMain=0xe7).
//
//
//  *****************  Version 3.2.0.2  *****************
//  User: zoujie        Date: 2003.08.19    Time: 	
//
//  1.  Add PET_INITFILE Support
//  2.  Ignore Frist Coin data
//
//
//  *****************  Version 3.2.0.1  *****************
//  User: zoujie        Date: 2003.08.08    Time: 	
//
//  1.  Before X-Move Bed, make sure Bed Y-Pos great than 310mm.
//	2.  When Exec Protocol, send PET_SYSINFO.
//
//
//  *****************  Version 3.2.0.0  *****************
//  User: zoujie        Date: 2003.07.28    Time: 	
//
//  1.  When Connect to OWS Lost or g_hEvtSysError Trig
//      Acq Computer auto reboot.
//
//
//  *****************  Version 3.1.0.2  *****************
//  User: zoujie        Date: 2003.07.25    Time: 	
//
//  1.  Add CSIO QueryRodStatus
//      Add CSIO::QuerySeptaStatus
//
//
//  *****************  Version 3.1.0.1  *****************
//  User: zoujie        Date: 2003.07.22    Time: 	
//
//  1.  Change Program Init Paramter From Regist DataBase To Ini File
//      
//  2.  Add Coin Time-Windows Init Paramter Option To Ini File
//		Add Coin Data Format Init Paramter Option to Ini File
//      Add Enable/Disable Bucket Debug  Init Paramter Option to Ini File
//      Add En/Disable Protocol Control EMC Init Paramter Option to Ini File
//
//  
//  *****************  Version 3.1.0.0  *****************
//  User: zoujie        Date: 2003.07.14    Time: 15:00
//
//  1.  Updated OWSProtocol.h and COWS::ProtocolSave
//      Add RodSpeed to struct ACQ_HEAD;
//      Add GateClass and GateStop to struct ACQ_GATE_HEAD;
//
//  2.  Add Enable/disable EMControl Option when startup
//		Update CSysInitDlg;
//      Update CAcqCenterDlg::OnInitDialog CAcqCenterDlg::InitSIO;
//      Update ExecProtocol.cpp
//
//  
//  *****************  Version 3.0.0.0  *****************
//  User: zoujie        Date: 2003.06.19  Time: 
//  
//  First Release Of This Program
//
//////////////////////////////////////////////////////////////////////
