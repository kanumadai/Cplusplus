; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CSysInitDlg
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "ACQCenter.h"

ClassCount=4
Class1=CACQCenterApp
Class2=CACQCenterDlg
Class3=CAboutDlg

ResourceCount=4
Resource1=IDD_ACQCENTER_DIALOG (English (U.S.))
Resource2=IDR_MAINFRAME
Resource3=IDD_SYSINIT_OPTION (English (U.S.))
Class4=CSysInitDlg
Resource4=IDD_ABOUTBOX (English (U.S.))

[CLS:CACQCenterApp]
Type=0
HeaderFile=ACQCenter.h
ImplementationFile=ACQCenter.cpp
Filter=N

[CLS:CACQCenterDlg]
Type=0
HeaderFile=ACQCenterDlg.h
ImplementationFile=ACQCenterDlg.cpp
Filter=D
BaseClass=CDialog
VirtualFilter=dWC
LastObject=CACQCenterDlg

[CLS:CAboutDlg]
Type=0
HeaderFile=ACQCenterDlg.h
ImplementationFile=ACQCenterDlg.cpp
Filter=D
LastObject=CAboutDlg

[CLS:CSysInitDlg]
Type=0
HeaderFile=SysInitDlg.h
ImplementationFile=SysInitDlg.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=CSysInitDlg

[DLG:IDD_ABOUTBOX (English (U.S.))]
Type=1
Class=CAboutDlg
ControlCount=3
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308352
Control3=IDOK,button,1342373889

[DLG:IDD_ACQCENTER_DIALOG (English (U.S.))]
Type=1
Class=CACQCenterDlg
ControlCount=24
Control1=IDC_TIME_LJ,edit,1350639744
Control2=IDC_TIME_MIN,edit,1350639744
Control3=IDC_TIME_MAX,edit,1350639744
Control4=IDC_OUTFMT,combobox,1478557699
Control5=IDC_EN_DBGBUCK,button,1342245891
Control6=IDC_EN_SIOEMC,button,1342242819
Control7=IDOK,button,1342242817
Control8=IDC_EDIT_OWS,edit,1352728580
Control9=IDC_EDIT_PROTOCOL,edit,1352728580
Control10=IDC_EDIT_FH,edit,1352728580
Control11=IDC_EDIT_SIO,edit,1352728580
Control12=IDC_STATIC,button,1342177287
Control13=IDC_STATIC,button,1342177287
Control14=IDC_STATIC,button,1342177287
Control15=IDC_STATIC,button,1342177287
Control16=IDC_STATIC,static,1342308352
Control17=IDC_STATIC,static,1342308352
Control18=IDC_STATIC,static,1342308352
Control19=IDC_STATIC,button,1342177287
Control20=IDC_PAUSE_DISP_COININFO,button,1342242819
Control21=IDC_PAUSE_DISP_SIOINFO,button,1342242819
Control22=IDC_TIPS_BED,static,1342308864
Control23=IDC_TIPS_ROD,static,1342308864
Control24=IDC_TIPS_TH_GANTRY,static,1342308864

[DLG:IDD_SYSINIT_OPTION (English (U.S.))]
Type=1
Class=CSysInitDlg
ControlCount=8
Control1=IDC_RADIO_ENDBG,button,1342308361
Control2=IDC_RADIO_DISDBG,button,1342177289
Control3=IDC_STATIC,button,1342177287
Control4=IDC_STATIC_TIMEOUT,static,1342308353
Control5=IDC_STATIC,static,1342308865
Control6=IDC_STATIC,static,1342177283
Control7=IDC_EN_SIOEMC,button,1342242819
Control8=IDC_EN_DBGBUCKET,button,1342242819

