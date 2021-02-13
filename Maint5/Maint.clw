; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CSFlash
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "Maint.h"
LastPage=0

ClassCount=30
Class1=CMaintApp
Class2=CMaintDoc
Class3=CMaintView
Class4=CMainFrame

ResourceCount=25
Resource1=IDD_FILESAVE
Resource2=IDR_MENU_SHOWENERGE
Class5=CAboutDlg
Resource3=IDD_FPGA_COUNT_MAP
Class6=CCommSet
Resource4=IDD_SET64ENERGELIMIT_PARA
Class7=CDataSample
Resource5=IDD_ANOL_PARA_MANUL_MODIFY
Class8=CEnvirmentParam
Resource6=IDD_ENVIRMENTPARAM
Class9=CBoardLocal
Resource7=IDD_ABOUT
Class10=CBoardEnerge
Resource8=IDD_FPGA_BOARDENER
Class11=CTimeModfy
Resource9=IDD_FPGA_COUNTER
Class12=CCysNum
Resource10=IDD_SHOWBAR
Class13=CTseries
Resource11=IDD_COMMSET
Class14=CAnol
Resource12=IDD_SFLASH
Class15=Csadf
Resource13=IDD_FPGA_BOARDLOCAL
Class16=CLogIn
Resource14=IDD_MODELESS
Class17=CSFlash
Resource15=IDD_ANLOG
Resource16=IDD_FPGA_CYSNUM
Class18=CFpgaCounter
Class19=CFileSave
Class20=CLoad32BK
Class21=CModeless
Resource17=IDD_ABOUTBOX
Class22=CNModeless
Resource18=IDD_AUTOSAMPLE_OPTION
Class23=CEffectStat
Resource19=IDD_FPGA_TSERIES
Class24=CManulModifyAnolPara
Resource20=IDD_FPGA_TIMEMODFY
Class25=CAutoSampleOption
Resource21=IDR_MAINFRAME
Class26=CFpgaCountMap
Class27=CFastOpen32File
Resource22=IDD_LOGIN
Class28=CAbout
Resource23=IDD_EFFECT_STAT
Class29=CSet64EnergeLimit
Resource24=IDD_DATA_SAMPLE
Class30=CEnvirmentLimit
Resource25=IDD_ENVIRMENT_LIMIT

[CLS:CMaintApp]
Type=0
HeaderFile=Maint.h
ImplementationFile=Maint.cpp
Filter=N
LastObject=IDC_MENU_ABOUT
BaseClass=CWinApp
VirtualFilter=AC

[CLS:CMaintDoc]
Type=0
HeaderFile=MaintDoc.h
ImplementationFile=MaintDoc.cpp
Filter=N
BaseClass=CDocument
VirtualFilter=DC
LastObject=IDC_FILE_OPEN

[CLS:CMaintView]
Type=0
HeaderFile=MaintView.h
ImplementationFile=MaintView.cpp
Filter=C
LastObject=ID_FILE_OPEN
BaseClass=CView
VirtualFilter=VWC


[CLS:CMainFrame]
Type=0
HeaderFile=MainFrm.h
ImplementationFile=MainFrm.cpp
Filter=T
LastObject=ID_MENU_DOWNLOAD_PROG_PARA
BaseClass=CFrameWnd
VirtualFilter=fWC




[CLS:CAboutDlg]
Type=0
HeaderFile=Maint.cpp
ImplementationFile=Maint.cpp
Filter=D
LastObject=CAboutDlg

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=8
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342177291
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889
Control5=IDC_STATIC,static,1342308352
Control6=IDC_STATIC,static,1342308352
Control7=IDC_STATIC,static,1342308352
Control8=IDC_COMMENT,edit,1352730756

[MNU:IDR_MAINFRAME]
Type=1
Class=CMainFrame
CommandCount=0

[ACL:IDR_MAINFRAME]
Type=1
Class=CMainFrame
Command1=ID_EDIT_COPY
Command2=ID_FILE_NEW
Command3=ID_FILE_OPEN
Command4=ID_FILE_PRINT
Command5=IDC_FILE_SAVE_ENERGE_LIMIT
Command6=ID_EDIT_PASTE
Command7=ID_EDIT_UNDO
Command8=ID_EDIT_CUT
Command9=ID_NEXT_PANE
Command10=ID_PREV_PANE
Command11=ID_EDIT_COPY
Command12=ID_EDIT_PASTE
Command13=ID_FASTOPEN32FILE_LAST
Command14=ID_FASTOPEN32FILE_NEXT
Command15=ID_EDIT_CUT
Command16=ID_EDIT_UNDO
CommandCount=16

[TB:IDR_MAINFRAME]
Type=1
Class=CMainFrame
Command1=IDC_FILE_OPEN
Command2=ID_FILE_SAVE_AS
Command3=ID_MENU_ANLOG
Command4=ID_MENU_DATA_SAMPLE
Command5=ID_MENU_ENVIRMENTPARAM
Command6=ID_MENU_BOARDENERG
Command7=ID_MENU_BOARDLOCAL
Command8=ID_MENU_CYSNUM
Command9=ID_MENU_TIMEMODFY
Command10=ID_MENU_TSERIES
Command11=ID_MENU_COMMSET
Command12=ID_MENU_FASTOPEN
Command13=IDC_MENU_ABOUTME
CommandCount=13

[DLG:IDD_SHOWBAR]
Type=1
Class=CMaintView
ControlCount=31
Control1=IDC_REDRAW,button,1342242817
Control2=IDC_STATIC,button,1342177287
Control3=IDC_SHOWBAR_3D_RADIO,button,1342308361
Control4=IDC_SHOWBAR_2D_RADIO,button,1342177289
Control5=IDC_STATIC,button,1342177287
Control6=IDC_2D_SHOW_X,button,1342308361
Control7=IDC_2D_SHOW_Y,button,1342177289
Control8=IDC_STATIC,static,1342308352
Control9=IDC_2D_SHOW_OFFSET1,edit,1350631552
Control10=IDC_STATIC,static,1342308352
Control11=IDC_3D_SHOW_THREVALUE,edit,1350631552
Control12=IDC_STATIC,static,1342177297
Control13=IDC_2D_SHOW_OFFSET2,edit,1350631552
Control14=IDC_STATIC,static,1342308352
Control15=IDC_SHOW_BLOCK_SEL,combobox,1344339970
Control16=IDC_STATIC,static,1342308352
Control17=IDC_STATIC,static,1342177296
Control18=IDC_SAVE_CUR_LIMIT,button,1342242816
Control19=IDC_CALCULATE_LIMIT,button,1342242816
Control20=IDC_REDRAW_LIMIT,button,1342242816
Control21=IDC_LIMIT_STAT,button,1342242816
Control22=IDC_OPEN_LIMITTAB,button,1342242816
Control23=IDC_OFFSET1_ADD,button,1342242816
Control24=IDC_OFFSET1_DEC,button,1342242816
Control25=IDC_EN_MOVEWHOLELINE,button,1342242819
Control26=IDC_STATIC,static,1342308352
Control27=IDC_PEAKVALE_ETH,edit,1350631552
Control28=IDC_PEAK_VALE_STAT,button,1342242816
Control29=IDC_CHECK_VER_HOR,button,1342242819
Control30=IDC_CHECK_MIDFILTER,button,1342242819
Control31=IDC_EFFECENT_EVALUAT,button,1342242816

[DLG:IDD_COMMSET]
Type=1
Class=CCommSet
ControlCount=19
Control1=IDOKe,button,1342242817
Control2=IDC_STATIC,button,1342177287
Control3=IDC_STATIC,static,1342308352
Control4=IDC_STATIC,static,1342308352
Control5=IDC_COMBO_BAUDSEL,combobox,1344339970
Control6=IDC_COMBO_DATABIT,combobox,1344339970
Control7=IDC_COMBO_STOPBIT,combobox,1344340226
Control8=IDC_STATIC,static,1342308352
Control9=IDC_STATIC,static,1342308352
Control10=IDC_COMBO_PORTSET,combobox,1344339970
Control11=IDC_CANCEL,button,1342242816
Control12=IDC_MSCOMM,{648A5600-2C6E-101B-82B6-000000000014},1342242816
Control13=IDC_POWERON_CHECK,button,1342242816
Control14=IDC_BUCKET_SEL,combobox,1344339970
Control15=IDC_COMSET_MANUL_CHECK,button,1342242819
Control16=IDC_STATIC_POWERCHECK,static,1342308352
Control17=IDC_STATIC,static,1342308352
Control18=IDC_COMBO_SELOBJECT,combobox,1344339970
Control19=IDC_MAINT_MODE_SET,button,1342242816

[CLS:CCommSet]
Type=0
HeaderFile=CommSet.h
ImplementationFile=CommSet.cpp
BaseClass=CDialog
Filter=D
LastObject=CCommSet
VirtualFilter=dWC

[DLG:IDD_DATA_SAMPLE]
Type=1
Class=CDataSample
ControlCount=25
Control1=IDOKe,button,1342242817
Control2=IDC_COUNTER_REDRAW,button,1342242816
Control3=IDC_STATIC,button,1342177287
Control4=IDC_RADIO_SDOT,button,1342177289
Control5=IDC_RADIO_EDOT,button,1342177289
Control6=IDC_RADIO_TDOT,button,1342177289
Control7=IDC_RADIO_FUHE0,button,1342177289
Control8=IDC_RADIO_FUHE1,button,1342177289
Control9=IDC_STATIC,static,1342308352
Control10=IDC_BUCKET_SEL,combobox,1344339970
Control11=IDC_STATIC,static,1342308352
Control12=IDC_STATIC,button,1342177287
Control13=IDC_STATIC,static,1342308352
Control14=IDC_SETCOUNT,edit,1484849280
Control15=IDC_STATIC,static,1342308352
Control16=IDC_SETTIME,edit,1350631552
Control17=IDC_STATIC,static,1342308352
Control18=IDC_REC_COUNT,edit,1350631426
Control19=IDC_STOP_SAMPLE,button,1342242816
Control20=IDC_START_SAMPLE,button,1342242816
Control21=IDC_MSCOMM,{648A5600-2C6E-101B-82B6-000000000014},1342242816
Control22=IDC_STATIC,button,1342177287
Control23=IDC_STATIC_RECSTATUS,static,1342177280
Control24=IDC_UPLOAD_DATA,button,1342242816
Control25=IDC_BLOCK_SEL,combobox,1478557698

[CLS:CDataSample]
Type=0
HeaderFile=DataSample.h
ImplementationFile=DataSample.cpp
BaseClass=CDialog
Filter=D
LastObject=IDC_LOAD32BUCKET
VirtualFilter=dWC

[DLG:IDD_ENVIRMENTPARAM]
Type=1
Class=CEnvirmentParam
ControlCount=18
Control1=IDOKe,button,1342242816
Control2=IDC_BUCKET_SEL,combobox,1344339970
Control3=IDC_STATIC,static,1342308352
Control4=IDC_THEM_STATIC,static,1342177280
Control5=IDC_MSCOMM1,{648A5600-2C6E-101B-82B6-000000000014},1342242816
Control6=IDC_BUTT_START_QUERY,button,1342242817
Control7=IDC_STATIC,static,1342177297
Control8=IDC_STATIC,static,1342177297
Control9=IDC_STATIC,static,1342308352
Control10=IDC_CHECK1,button,1342242819
Control11=IDC_ENPARA_SHOW,edit,1352728580
Control12=IDC_CLEARSHOW,button,1342242816
Control13=IDC_STOP_QUERY,button,1342242816
Control14=IDC_STATIC,button,1342177287
Control15=IDC_STATIC,static,1342308352
Control16=IDC_VOLTAGE_STATIC,static,1342308352
Control17=IDC_STATIC,static,1342308352
Control18=IDC_NEGVOLTAGE_STATIC,static,1342308352

[CLS:CEnvirmentParam]
Type=0
HeaderFile=EnvirmentParam.h
ImplementationFile=EnvirmentParam.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=CEnvirmentParam

[CLS:CBoardLocal]
Type=0
HeaderFile=BoardLocal.h
ImplementationFile=BoardLocal.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=IDC_BOARDLOCAL_EDIT

[DLG:IDD_FPGA_BOARDENER]
Type=1
Class=CBoardEnerge
ControlCount=22
Control1=IDOKe,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_STATIC,button,1342177287
Control4=IDC_BUCKET_SEL,combobox,1344339970
Control5=IDC_STATIC,button,1342177287
Control6=IDC_STATIC,button,1342177287
Control7=IDC_EEPROM_RADIO,button,1342308361
Control8=IDC_FPGA_RADIO,button,1342242825
Control9=IDC_INPUT_RADIO,button,1342308361
Control10=IDC_OUTPUT_RADIO,button,1342177289
Control11=IDC_STATIC,static,1342308352
Control12=IDC_STATIC,static,1342308352
Control13=IDC_STATIC,static,1342308352
Control14=IDC_MSFLEXGRID1,{6262D3A0-531B-11CF-91F6-C2863C385E30},1342242816
Control15=IDC_BOARDENERG_EDIT,edit,1082196096
Control16=IDC_MSCOMM1,{648A5600-2C6E-101B-82B6-000000000014},1342242816
Control17=IDC_ENER_E2_VALUE,edit,1350631552
Control18=IDC_SET_ENER_INIT,button,1342242816
Control19=IDC_ENER_E1_VALUE,edit,1350631552
Control20=IDC_LOADFILE,button,1342242816
Control21=IDC_BLOCK_SEL,combobox,1344339970
Control22=IDC_CHANNEL_CHANGE_SEL,button,1342242819

[CLS:CBoardEnerge]
Type=0
HeaderFile=BoardEnerge.h
ImplementationFile=BoardEnerge.cpp
BaseClass=CDialog
Filter=D
LastObject=IDC_BLOCK_SEL
VirtualFilter=dWC

[DLG:IDD_FPGA_BOARDLOCAL]
Type=1
Class=CBoardLocal
ControlCount=19
Control1=IDOKe,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_STATIC,button,1342177287
Control4=IDC_BUCKET_SEL,combobox,1344339970
Control5=IDC_STATIC,button,1342177287
Control6=IDC_STATIC,button,1342177287
Control7=IDC_EEPROM_RADIO,button,1342308361
Control8=IDC_FPGA_RADIO,button,1342242825
Control9=IDC_INPUT_RADIO,button,1342308361
Control10=IDC_OUTPUT_RADIO,button,1342177289
Control11=IDC_STATIC,static,1342308352
Control12=IDC_STATIC,static,1342308352
Control13=IDC_STATIC,static,1342308352
Control14=IDC_MSFLEXGRID1,{6262D3A0-531B-11CF-91F6-C2863C385E30},1342242816
Control15=IDC_BOARDLOCAL_EDIT,edit,1082196096
Control16=IDC_MSCOMM1,{648A5600-2C6E-101B-82B6-000000000014},1342242816
Control17=IDC_BLOCAL_LOADDATA,button,1342242816
Control18=IDC_BLOCK_SEL,combobox,1344339970
Control19=IDC_CHANNEL_CHANGE_SEL,button,1342242819

[DLG:IDD_FPGA_TIMEMODFY]
Type=1
Class=CTimeModfy
ControlCount=19
Control1=IDOKe,button,1342242817
Control2=IDC_OUT,button,1342242816
Control3=IDC_STATIC,button,1342177287
Control4=IDC_BUCKET_SEL,combobox,1344339970
Control5=IDC_STATIC,button,1342177287
Control6=IDC_STATIC,button,1342177287
Control7=IDC_EEPROM_RADIO,button,1342308361
Control8=IDC_FPGA_RADIO,button,1342242825
Control9=IDC_INPUT_RADIO,button,1342308361
Control10=IDC_OUTPUT_RADIO,button,1342177289
Control11=IDC_STATIC,static,1342308352
Control12=IDC_STATIC,static,1342308352
Control13=IDC_STATIC,static,1342308352
Control14=IDC_MSFLEXGRID1,{6262D3A0-531B-11CF-91F6-C2863C385E30},1342242816
Control15=IDC_TIMEMODFY_EDIT,edit,1082196096
Control16=IDC_MSCOMM1,{648A5600-2C6E-101B-82B6-000000000014},1342242816
Control17=IDC_TMOD_INIT_VALUE,edit,1350631552
Control18=IDC_SET_TMOD_INIT,button,1342242816
Control19=IDC_BLOCK_SEL,combobox,1344339970

[CLS:CTimeModfy]
Type=0
HeaderFile=TimeModfy.h
ImplementationFile=TimeModfy.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=IDC_BLOCK_SEL

[DLG:IDD_FPGA_CYSNUM]
Type=1
Class=CCysNum
ControlCount=17
Control1=IDOKe,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_STATIC,button,1342177287
Control4=IDC_BUCKET_SEL,combobox,1344339970
Control5=IDC_STATIC,button,1342177287
Control6=IDC_STATIC,button,1342177287
Control7=IDC_EEPROM_RADIO,button,1342308361
Control8=IDC_FPGA_RADIO,button,1342242825
Control9=IDC_INPUT_RADIO,button,1342308361
Control10=IDC_OUTPUT_RADIO,button,1342177289
Control11=IDC_STATIC,static,1342308352
Control12=IDC_STATIC,static,1342308352
Control13=IDC_STATIC,static,1342308352
Control14=IDC_MSFLEXGRID1,{6262D3A0-531B-11CF-91F6-C2863C385E30},1342242816
Control15=IDC_CYSNUM_EDIT,edit,1082196096
Control16=IDC_MSCOMM1,{648A5600-2C6E-101B-82B6-000000000014},1342242816
Control17=IDC_BLOCK_SEL,combobox,1344339970

[CLS:CCysNum]
Type=0
HeaderFile=CysNum.h
ImplementationFile=CysNum.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=CCysNum

[CLS:CTseries]
Type=0
HeaderFile=Tseries.h
ImplementationFile=Tseries.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=CTseries

[DLG:IDD_FPGA_TSERIES]
Type=1
Class=CTseries
ControlCount=16
Control1=IDOKe,button,1342242817
Control2=IDC_BUCKET_SEL,combobox,1344339970
Control3=IDC_STATIC,button,1342177287
Control4=IDC_INPUT_RADIO,button,1342308361
Control5=IDC_OUTPUT_RADIO,button,1342177289
Control6=IDC_STATIC,static,1342308352
Control7=IDC_STATIC,static,1342308352
Control8=IDC_TSERIES_EDIT,edit,1082196096
Control9=IDC_MSCOMM1,{648A5600-2C6E-101B-82B6-000000000014},1342242816
Control10=IDC_MSFLEXGRID1,{6262D3A0-531B-11CF-91F6-C2863C385E30},1342242816
Control11=IDC_STATIC,static,1342308352
Control12=IDC_STATIC,button,1342177287
Control13=IDC_EEPROM_RADIO,button,1342308361
Control14=IDC_FPGA_RADIO,button,1342242825
Control15=IDC_OUT,button,1342242816
Control16=IDC_BLOCK_SEL,combobox,1344339970

[CLS:CAnol]
Type=0
HeaderFile=Anol.h
ImplementationFile=Anol.cpp
BaseClass=CDialog
Filter=D
LastObject=IDC_BUCKET_SEL
VirtualFilter=dWC

[DLG:IDD_ANLOG]
Type=1
Class=CAnol
ControlCount=36
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_BUCKET_SEL,combobox,1344339970
Control4=IDC_PLUS_A,edit,1350631554
Control5=IDC_PLUS_B,edit,1350631426
Control6=IDC_PLUS_C,edit,1350631554
Control7=IDC_OFFSET_Y,edit,1350631426
Control8=IDC_PLUS_D,edit,1350631554
Control9=IDC_OFFSET_E,edit,1350631426
Control10=IDC_OFFSET_X,edit,1350631554
Control11=IDC_ETH_VALUE,edit,1350631426
Control12=IDC_TD_VALUE,edit,1484849154
Control13=IDC_STATIC,static,1342308352
Control14=IDC_STATIC,static,1342308352
Control15=IDC_STATIC,static,1342308352
Control16=IDC_STATIC,static,1342308352
Control17=IDC_STATIC,static,1342308352
Control18=IDC_STATIC,static,1342308352
Control19=IDC_STATIC,static,1342308352
Control20=IDC_STATIC,static,1342308352
Control21=IDC_STATIC,static,1342308352
Control22=IDC_STATIC,static,1342308352
Control23=IDC_CHIP_EEPROM,button,1342308361
Control24=IDC_CHIP_ANOL,button,1342177289
Control25=IDC_CHIP_ALL,button,1342177289
Control26=IDC_IOSEL_IN,button,1342373897
Control27=IDC_IOSEL_OUT,button,1342177289
Control28=IDC_STATIC,button,1342177287
Control29=IDC_STATIC,button,1342177287
Control30=IDC_STATIC,button,1342177287
Control31=IDC_STATIC,button,1342177287
Control32=IDC_STATIC,static,1342308352
Control33=IDC_BLOCK_SEL,combobox,1344339970
Control34=IDC_LOADFILE,button,1342242816
Control35=IDC_SAVE_PARA,button,1342242816
Control36=IDC_CHANNEL_CHANGE_SEL,button,1342242819

[CLS:Csadf]
Type=0
HeaderFile=sadf.h
ImplementationFile=sadf.cpp
BaseClass=CDialog
Filter=D
LastObject=Csadf

[DLG:IDD_LOGIN]
Type=1
Class=CLogIn
ControlCount=9
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_STATIC,static,1342308352
Control4=IDC_STATIC,static,1342308352
Control5=IDC_USER,edit,1350631552
Control6=IDC_COMPANY,edit,1342244992
Control7=IDC_PASSWORD,edit,1350631584
Control8=IDC_STATIC,static,1342308352
Control9=IDC_STATIC,static,1342177283

[CLS:CLogIn]
Type=0
HeaderFile=LogIn.h
ImplementationFile=LogIn.cpp
BaseClass=CDialog
Filter=D
LastObject=IDC_USER
VirtualFilter=dWC

[DLG:IDD_SFLASH]
Type=1
Class=CSFlash
ControlCount=2
Control1=IDC_USER,static,1342177803
Control2=IDC_COMPANY,static,1342308875

[CLS:CSFlash]
Type=0
HeaderFile=SFlash.h
ImplementationFile=SFlash.cpp
BaseClass=CDialog
Filter=D
LastObject=CSFlash
VirtualFilter=dWC

[MNU:IDR_MENU_SHOWENERGE]
Type=1
Class=?
Command1=IDC_MENU_SHOW64ENERGESPECT
CommandCount=1

[DLG:IDD_FPGA_COUNTER]
Type=1
Class=CFpgaCounter
ControlCount=32
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_BUCKET_SEL,combobox,1344339970
Control4=IDC_STATIC,static,1342308352
Control5=IDC_STATIC,static,1342308352
Control6=IDC_CHANNEL0_ALL,edit,1350631552
Control7=IDC_CHANNEL0_G2S,edit,1350631552
Control8=IDC_STATIC,button,1342177287
Control9=IDC_STATIC,static,1342308352
Control10=IDC_STATIC_G2S,static,1342308352
Control11=IDC_STATIC,static,1342308352
Control12=IDC_CHANNEL1_ALL,edit,1350631552
Control13=IDC_CHANNEL1_G2S,edit,1350631552
Control14=IDC_STATIC,static,1342308352
Control15=IDC_CHANNEL2_ALL,edit,1350631552
Control16=IDC_CHANNEL2_G2S,edit,1350631552
Control17=IDC_STATIC,static,1342308352
Control18=IDC_CHANNEL3_ALL,edit,1350631552
Control19=IDC_CHANNEL3_G2S,edit,1350631552
Control20=IDC_STATIC,static,1342308352
Control21=IDC_CHANNEL4_ALL,edit,1350631552
Control22=IDC_CHANNEL4_G2S,edit,1350631552
Control23=IDC_STATIC,static,1342308352
Control24=IDC_CHANNEL5_ALL,edit,1350631552
Control25=IDC_CHANNEL5_G2S,edit,1350631552
Control26=IDC_STATIC,static,1342308352
Control27=IDC_CHANNEL6_ALL,edit,1350631552
Control28=IDC_CHANNEL6_G2S,edit,1350631552
Control29=IDC_STATIC,static,1342308352
Control30=IDC_CHANNEL7_ALL,edit,1350631552
Control31=IDC_CHANNEL7_G2S,edit,1350631552
Control32=IDC_SHOWMAP,button,1342242816

[CLS:CFpgaCounter]
Type=0
HeaderFile=FpgaCounter.h
ImplementationFile=FpgaCounter.cpp
BaseClass=CDialog
Filter=D
LastObject=CFpgaCounter
VirtualFilter=dWC

[DLG:IDD_FILESAVE]
Type=1
Class=CFileSave
ControlCount=5
Control1=IDC_STATIC,button,1342177287
Control2=IDC_NEW_FILEPACK,button,1342177289
Control3=IDC_EDIT_FILEPACK,button,1342177289
Control4=IDOK,button,1342242816
Control5=IDCANCEL,button,1342242816

[CLS:CFileSave]
Type=0
HeaderFile=FileSave1.h
ImplementationFile=FileSave1.cpp
BaseClass=CDialog
Filter=D
LastObject=CFileSave
VirtualFilter=dWC

[CLS:CLoad32BK]
Type=0
HeaderFile=Load32BK.h
ImplementationFile=Load32BK.cpp
BaseClass=CDialog
Filter=D
LastObject=CLoad32BK
VirtualFilter=dWC

[CLS:CModeless]
Type=0
HeaderFile=Modeless.h
ImplementationFile=Modeless.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=IDCANCEL

[DLG:IDD_MODELESS]
Type=1
Class=CNModeless
ControlCount=24
Control1=IDOK,button,1342242816
Control2=IDCANCEL,button,1342242816
Control3=IDC_STATIC,static,1342308352
Control4=IDC_MOD_STATUS,static,1342177280
Control5=IDC_RENAME,button,1342242817
Control6=IDC_SHOW_COUNTER,static,1342308352
Control7=IDC_START_POINT,edit,1350631552
Control8=IDC_STATIC,static,1342308352
Control9=IDC_STATIC,static,1342308352
Control10=IDC_BUCKETNO,static,1342308352
Control11=IDC_END_POINT,edit,1350631552
Control12=IDC_CHECK_LOCAL,button,1342242819
Control13=IDC_CHECK_ANOL,button,1342242819
Control14=IDC_CHECK_ENERGE,button,1342242819
Control15=IDC_CHECK_TSERIES,button,1342242819
Control16=IDC_LOAD,button,1342177289
Control17=IDC_DOWN,button,1342177289
Control18=IDC_CHECK_CYSNUM,button,1342242819
Control19=IDC_CHECK_TIMEMODFY,button,1342242819
Control20=IDC_DATA_PARA,button,1342242816
Control21=IDC_SEL_TYPE,static,1342308352
Control22=IDC_STATIC_FRAME,button,1342177287
Control23=IDC_CHECK_AUTOSAMPLE,button,1342242819
Control24=IDC_BUTTON_AUTO_SAMPLE,button,1342242816

[CLS:CNModeless]
Type=0
HeaderFile=NModeless.h
ImplementationFile=NModeless.cpp
BaseClass=CDialog
Filter=D
LastObject=IDC_BUCKETNO
VirtualFilter=dWC

[DLG:IDD_EFFECT_STAT]
Type=1
Class=CEffectStat
ControlCount=16
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_STATIC_SOURCE,static,1342308352
Control4=IDC_EFFECT_RESOURCE_FILE,button,1342242816
Control5=IDC_EFFECT_DESTINATION_FILE,button,1342242816
Control6=IDC_STATIC_DESTINATION,static,1342308352
Control7=IDC_EFFECT_OUTTEXT_FILE,button,1342242816
Control8=IDC_STATIC_OUTTEXT,static,1342308352
Control9=IDC_STATIC,static,1342308352
Control10=IDC_EDIT_START_FILE,edit,1350631552
Control11=IDC_STATIC,static,1342308352
Control12=IDC_EDIT_END_FILE,edit,1350631552
Control13=IDC_CHECK_OUTRELATIVE_VALUE,button,1342242819
Control14=IDC_CHECK_PEAK_ANOLPARA_SEL,button,1342242819
Control15=IDC_STATIC_PEAK,static,1342308352
Control16=IDC_EDIT_PEAK,edit,1350631552

[CLS:CEffectStat]
Type=0
HeaderFile=EffectStat.h
ImplementationFile=EffectStat.cpp
BaseClass=CDialog
Filter=D
LastObject=CEffectStat
VirtualFilter=dWC

[DLG:IDD_ANOL_PARA_MANUL_MODIFY]
Type=1
Class=CManulModifyAnolPara
ControlCount=18
Control1=IDOK,button,1342242816
Control2=IDC_STATIC_PATH,static,1342308352
Control3=IDC_LOAD_FILENAME,button,1342242816
Control4=IDC_STATIC,static,1342308352
Control5=IDC_EDIT_START_FILE,edit,1350631552
Control6=IDC_STATIC,static,1342308352
Control7=IDC_EDIT_END_FILE,edit,1350631552
Control8=IDC_READ_DATA,button,1342242816
Control9=IDC_SAVE_DATA,button,1342242816
Control10=IDC_STATIC,static,1342177296
Control11=IDC_MSFLEXGRID1,{6262D3A0-531B-11CF-91F6-C2863C385E30},1342242816
Control12=IDC_MANULMODITY_EDIT,edit,1082196096
Control13=IDC_ABCD_MODIFY,button,1342242816
Control14=IDC_STATIC,static,1342308352
Control15=IDC_EDIT_ABCD_MODIFY,edit,1350631552
Control16=IDC_OFFSET_MODIFY,button,1342242816
Control17=IDC_MANUAL_MOD_PRE,button,1342242816
Control18=IDC_MANUAL_MOD,button,1342242816

[CLS:CManulModifyAnolPara]
Type=0
HeaderFile=ManulModifyAnolPara.h
ImplementationFile=ManulModifyAnolPara.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=CManulModifyAnolPara

[DLG:IDD_AUTOSAMPLE_OPTION]
Type=1
Class=CAutoSampleOption
ControlCount=19
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_STATIC,static,1342308352
Control4=IDC_EDIT_SAMPL_TIME,edit,1350631552
Control5=IDC_STATIC,static,1342308352
Control6=IDC_EDIT_REPEAT_NUM,edit,1350631552
Control7=IDC_CHECK_SDOT,button,1342242819
Control8=IDC_CHECK_ENERGE,button,1342242819
Control9=IDC_CHECK_TIME,button,1342242819
Control10=IDC_CHECK_FUHE1,button,1342242819
Control11=IDC_STATIC,button,1342177287
Control12=IDC_CHECK_FUHE2,button,1208025091
Control13=IDC_STATIC,button,1342177287
Control14=IDC_PARA_POSITION_LIMIT,button,1342242819
Control15=IDC_PARA_ENERGE_LIMIT,button,1342242819
Control16=IDC_PARA_ANOL_PARA,button,1342242819
Control17=IDC_PARA_TIMESERIER,button,1342242819
Control18=IDC_PARA_CYSNUM,button,1342242819
Control19=IDC_PARA_TIMEMODFY,button,1342242819

[CLS:CAutoSampleOption]
Type=0
HeaderFile=AutoSampleOption.h
ImplementationFile=AutoSampleOption.cpp
BaseClass=CDialog
Filter=D
LastObject=CAutoSampleOption
VirtualFilter=dWC

[DLG:IDD_FPGA_COUNT_MAP]
Type=1
Class=CFpgaCountMap
ControlCount=2
Control1=IDC_STATIC,button,1342177287
Control2=IDC_STATUS,static,1342308352

[CLS:CFpgaCountMap]
Type=0
HeaderFile=FpgaCountMap.h
ImplementationFile=FpgaCountMap.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=CFpgaCountMap

[CLS:CFastOpen32File]
Type=0
HeaderFile=FastOpen32File.h
ImplementationFile=FastOpen32File.cpp
BaseClass=CDialog
Filter=D
LastObject=IDC_SEL_FILENAME
VirtualFilter=dWC

[DLG:IDD_ABOUT]
Type=1
Class=CAbout
ControlCount=8
Control1=IDOK,button,1342242817
Control2=IDC_STATIC,static,1342177283
Control3=IDC_STATIC,static,1342177291
Control4=IDC_STATIC,static,1342308352
Control5=IDC_STATIC,static,1342308352
Control6=IDC_STATIC,static,1342308352
Control7=IDC_STATIC,static,1342308352
Control8=IDC_COMMENT,edit,1352730756

[CLS:CAbout]
Type=0
HeaderFile=About.h
ImplementationFile=About.cpp
BaseClass=CDialog
Filter=D
LastObject=IDC_COMMENT
VirtualFilter=dWC

[DLG:IDD_SET64ENERGELIMIT_PARA]
Type=1
Class=CSet64EnergeLimit
ControlCount=27
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_STATIC,button,1342177287
Control4=IDC_STATIC,static,1342308352
Control5=IDC_STATIC,static,1342308352
Control6=IDC_STATIC,static,1342308352
Control7=IDC_STATIC,static,1342308352
Control8=IDC_RING_1_BEF,edit,1350631552
Control9=IDC_RING_1_BACK,edit,1350631552
Control10=IDC_STATIC,static,1342308352
Control11=IDC_STATIC,static,1342308352
Control12=IDC_RING_4_BEF,edit,1350631552
Control13=IDC_RING_4_BACK,edit,1350631552
Control14=IDC_RING_3_BEF,edit,1350631552
Control15=IDC_RING_3_BACK,edit,1350631552
Control16=IDC_RING_2_BEF,edit,1350631552
Control17=IDC_RING_2_BACK,edit,1350631552
Control18=IDC_RING_1_CHECK,button,1342242819
Control19=IDC_RING_2_CHECK,button,1342242819
Control20=IDC_RING_4_CHECK,button,1342242819
Control21=IDC_RING_3_CHECK,button,1342242819
Control22=IDC_STATIC,static,1342308352
Control23=IDC_STATIC,static,1342308352
Control24=IDC_CORNER_BEF,edit,1350631552
Control25=IDC_CORNER_BACK,edit,1350631552
Control26=IDC_CORNER_CHECK,button,1342242819
Control27=IDC_PERSEND_SEL,button,1342242819

[CLS:CSet64EnergeLimit]
Type=0
HeaderFile=Set64EnergeLimit.h
ImplementationFile=Set64EnergeLimit.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=IDC_RING_1_CHECK

[DLG:IDD_ENVIRMENT_LIMIT]
Type=1
Class=CEnvirmentLimit
ControlCount=27
Control1=IDC_OPERATE,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_STATIC,static,1342308352
Control4=IDC_POS_VOLT_UPLIMIT,edit,1350631552
Control5=IDC_POS_VOLT_DOWNLIMIT,edit,1350631552
Control6=IDC_STATIC,static,1342308352
Control7=IDC_NEG_VOLT_UPLIMIT,edit,1350631552
Control8=IDC_NEG_VOLT_DOWNLIMIT,edit,1350631552
Control9=IDC_STATIC,static,1342308352
Control10=IDC_TEMPER_UPLIMIT,edit,1350631552
Control11=IDC_TEMPER_DOWNLIMIT,edit,1350631552
Control12=IDC_STATIC,button,1342177287
Control13=IDC_STATIC,static,1342308352
Control14=IDC_STATIC,static,1342308352
Control15=IDC_BKT_VER_POLL,button,1342242816
Control16=IDC_BKT_VER_SHOW,static,1342308352
Control17=IDC_STATIC,static,1342308352
Control18=IDC_BUCKET_SEL,combobox,1344339970
Control19=IDC_WR_PARA,button,1342177289
Control20=IDC_RD_PARA,button,1342177289
Control21=IDC_STATIC,button,1342177287
Control22=IDC_STATIC,button,1342177287
Control23=IDC_LOAD_FILE,button,1342242816
Control24=IDC_STATIC_FILENAME,static,1342308352
Control25=IDC_PROGRESS_DOWNLOAD,msctls_progress32,1342177281
Control26=IDC_DOWNLOAD_FILE,button,1342242816
Control27=IDC_HAND_STARTUP,button,1342242816

[CLS:CEnvirmentLimit]
Type=0
HeaderFile=EnvirmentLimit.h
ImplementationFile=EnvirmentLimit.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=CEnvirmentLimit

