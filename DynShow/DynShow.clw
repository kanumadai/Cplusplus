; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CProgContrl
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "DynShow.h"

ClassCount=7
Class1=CDynShowApp
Class2=CDynShowDlg
Class3=CAboutDlg

ResourceCount=7
Resource1=IDD_ABOUTBOX
Resource2=IDR_MAINFRAME
Resource3=IDD_ENERGESPECT_SHOW
Resource4=IDD_SELCOM
Class4=CComSel
Resource5=IDD_PROGCONTRL
Class5=CEnergeSpectEth
Resource6=IDD_DYNSHOW_DIALOG
Class6=CProgContrl
Class7=CFileSave
Resource7=IDD_FILE_SAVE

[CLS:CDynShowApp]
Type=0
HeaderFile=DynShow.h
ImplementationFile=DynShow.cpp
Filter=N

[CLS:CDynShowDlg]
Type=0
HeaderFile=DynShowDlg.h
ImplementationFile=DynShowDlg.cpp
Filter=D
LastObject=CDynShowDlg
BaseClass=CDialog
VirtualFilter=dWC

[CLS:CAboutDlg]
Type=0
HeaderFile=DynShowDlg.h
ImplementationFile=DynShowDlg.cpp
Filter=D

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=5
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889
Control5=IDC_STATIC,static,1342308352

[DLG:IDD_DYNSHOW_DIALOG]
Type=1
Class=CDynShowDlg
ControlCount=46
Control1=IDOK,button,1342242816
Control2=IDC_ENERGE_CHART_REDRAW,button,1342242816
Control3=IDC_ENERGE_LEFT_LIMIT,edit,1350631552
Control4=IDC_3D_SHOW_THREVALUE,edit,1350631552
Control5=IDC_STATIC,static,1342308352
Control6=IDC_ENERGE_RIGHT_LIMIT,edit,1350631552
Control7=IDC_STATIC,static,1342308352
Control8=IDC_STATIC,static,1342308352
Control9=IDC_STATIC,button,1342177287
Control10=IDC_REDRAW,button,1342242817
Control11=IDC_OPEN_FILE,button,1342242816
Control12=IDC_SAVE_FILE,button,1342242816
Control13=IDC_STATIC_3DSHOW,button,1342210055
Control14=IDC_SMALL_MAP_TITILE,button,1342210055
Control15=IDC_STATIC_CONTER,static,1342308352
Control16=IDC_STATIC,button,1342177287
Control17=IDC_START_SAMPLE,button,1342242816
Control18=IDC_STOP_SAMPLE,button,1342242816
Control19=IDC_SYN_ENERSPECT_VALUE,edit,1350631552
Control20=IDC_STATIC_2DSHOW,static,1342308352
Control21=IDC_STATIC_STATUS,static,1342308352
Control22=IDC_STATIC_FILE_NAME,static,1342308352
Control23=IDC_PROG_CONTROL,button,1342242816
Control24=IDC_SAMPING_TIME,edit,1350631552
Control25=IDC_STATIC_SAMPLE_MODE,static,1342308864
Control26=IDC_SAMPLE_MODE_SEL,button,1342242819
Control27=IDC_STATIC,button,1342177287
Control28=IDC_ADDSYN_NO,button,1342242816
Control29=IDC_ETH_RESET,button,1342242816
Control30=IDC_EN_LIMIT_POSITION_WHOLE_MOVE,button,1342242819
Control31=IDC_LIMIT_STAT,button,1342242816
Control32=IDC_PEAK_VALE_STAT,button,1342242816
Control33=IDC_PEAKVALE_ETH,edit,1350631552
Control34=IDC_STATIC,static,1342308352
Control35=IDC_ENERSPECT_64ETH,button,1342242816
Control36=IDC_CHECK1,button,1342242819
Control37=IDC_CHECK_MIDFILTER,button,1342242819
Control38=IDC_CLEAR_DATA,button,1342242816
Control39=IDC_STATIC,button,1342177287
Control40=IDC_ENERGER_SPECTRUM_SEL,button,1342177289
Control41=IDC_Y_DIRECTION_SEL,button,1342177289
Control42=IDC_X_DIRECTION_SEL,button,1342177289
Control43=IDC_TIME_SPECTRUM_SEL,button,1342177289
Control44=IDC_RADIO1,button,1342177289
Control45=IDC_TIME_FUHE,button,1342177289
Control46=IDC_STATIC_ETH_VALUE,static,1342308352

[DLG:IDD_SELCOM]
Type=1
Class=CComSel
ControlCount=7
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_COMSEL,combobox,1344339970
Control4=IDC_STATIC,static,1342308352
Control5=IDC_STATIC,static,1342308352
Control6=IDC_STATIC,static,1342308864
Control7=IDC_STATIC,static,1342177283

[CLS:CComSel]
Type=0
HeaderFile=ComSel.h
ImplementationFile=ComSel.cpp
BaseClass=CDialog
Filter=D
LastObject=IDC_COMSEL

[DLG:IDD_ENERGESPECT_SHOW]
Type=1
Class=CEnergeSpectEth
ControlCount=4
Control1=IDOK,button,1342242817
Control2=IDC_STATIC,button,1342177287
Control3=IDC_SHOWENERGE_OPTION,static,1342308352
Control4=IDC_SHOWENTERGE_ETHLIMIT,static,1342308352

[CLS:CEnergeSpectEth]
Type=0
HeaderFile=EnergeSpectEth.h
ImplementationFile=EnergeSpectEth.cpp
BaseClass=CDialog
Filter=D
LastObject=CEnergeSpectEth
VirtualFilter=dWC

[DLG:IDD_PROGCONTRL]
Type=1
Class=CProgContrl
ControlCount=24
Control1=IDC_SETPARA,button,1342242817
Control2=IDC_STATIC,button,1342177287
Control3=IDC_STATIC,static,1342308352
Control4=IDC_EDIT_PARA_A,edit,1350631552
Control5=IDC_STATIC,static,1342308352
Control6=IDC_EDIT_PARA_B,edit,1350631552
Control7=IDC_STATIC,static,1342308352
Control8=IDC_EDIT_PARA_C,edit,1350631552
Control9=IDC_STATIC,static,1342308352
Control10=IDC_EDIT_PARA_D,edit,1350631552
Control11=IDC_STATIC,static,1342308352
Control12=IDC_EDIT_PARA_XOFFSET,edit,1350631552
Control13=IDC_STATIC,static,1342308352
Control14=IDC_EDIT_PARA_YOFFSET,edit,1350631552
Control15=IDC_STATIC,static,1342308352
Control16=IDC_EDIT_PARA_EOFFSET,edit,1350631552
Control17=IDC_STATIC,static,1342308352
Control18=IDC_EDIT_PARA_ETH,edit,1350631552
Control19=IDC_CANCEL,button,1342242816
Control20=IDC_STATIC,button,1342177287
Control21=IDC_SEL_CHANNEL_A,button,1342177289
Control22=IDC_SEL_CHANNEL_B,button,1342177289
Control23=IDC_AB_TIMEFH,button,1342177289
Control24=IDC_BKT_FUHE,button,1342177289

[CLS:CProgContrl]
Type=0
HeaderFile=ProgContrl.h
ImplementationFile=ProgContrl.cpp
BaseClass=CDialog
Filter=D
LastObject=IDC_SETPARA
VirtualFilter=dWC

[DLG:IDD_FILE_SAVE]
Type=1
Class=CFileSave
ControlCount=7
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_STATIC,button,1342177287
Control4=IDC_STATIC,static,1342308352
Control5=IDC_BLOCK_SEL,combobox,1344339970
Control6=IDC_NEW_FILEPACK,button,1342177289
Control7=IDC_EDIT_FILEPACK,button,1342177289

[CLS:CFileSave]
Type=0
HeaderFile=FileSave.h
ImplementationFile=FileSave.cpp
BaseClass=CDialog
Filter=D
LastObject=IDC_EDIT_FILEPACK
VirtualFilter=dWC

