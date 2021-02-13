# Microsoft Developer Studio Project File - Name="SimOWS" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=SimOWS - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "SimOWS.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "SimOWS.mak" CFG="SimOWS - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "SimOWS - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "SimOWS - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "SimOWS - Win32 Release"

# PROP BASE Use_MFC 5
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\Bin\SimOWS\Release"
# PROP Intermediate_Dir "..\Bin\SimOWS\Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /Zp1 /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "NDEBUG"
# ADD RSC /l 0x804 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 /nologo /subsystem:windows /machine:I386 /libpath:"..\Bin\SimOWS\Release"

!ELSEIF  "$(CFG)" == "SimOWS - Win32 Debug"

# PROP BASE Use_MFC 5
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\Bin\SimOWS\Debug"
# PROP Intermediate_Dir "..\Bin\SimOWS\Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /Zp1 /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "_DEBUG"
# ADD RSC /l 0x804 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept /libpath:"..\Bin\SimOWS\Debug"

!ENDIF 

# Begin Target

# Name "SimOWS - Win32 Release"
# Name "SimOWS - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\AutoAcqBucketDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\AutoAcqCoinTimeDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\BedCoordDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\BucketParamDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\BuckFile.cpp
# End Source File
# Begin Source File

SOURCE=.\DbgPrt.cpp
# End Source File
# Begin Source File

SOURCE=.\FHParamDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\GetThDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\InitACSDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\Input1.cpp
# End Source File
# Begin Source File

SOURCE=.\ListenSocket.cpp
# End Source File
# Begin Source File

SOURCE=.\OWSSIO.cpp
# End Source File
# Begin Source File

SOURCE=.\OWSSock.cpp
# End Source File
# Begin Source File

SOURCE=.\RelateMoveDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\SimOWS.cpp
# End Source File
# Begin Source File

SOURCE=.\SimOWS.rc
# End Source File
# Begin Source File

SOURCE=.\SimOWSDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\TestParamDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\WndDisp.cpp
# End Source File
# Begin Source File

SOURCE=.\WndDispCointime.cpp
# End Source File
# Begin Source File

SOURCE=.\WndDispEnergy.cpp
# End Source File
# Begin Source File

SOURCE=.\WndDispSD.cpp
# End Source File
# Begin Source File

SOURCE=.\WndDispTime.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\AutoAcqBucketDlg.h
# End Source File
# Begin Source File

SOURCE=.\AutoAcqCoinTimeDlg.h
# End Source File
# Begin Source File

SOURCE=.\BlkDraw\BlkApi.h
# End Source File
# Begin Source File

SOURCE=.\BucketParamDlg.h
# End Source File
# Begin Source File

SOURCE=.\BlkDraw\DatFmt.h
# End Source File
# Begin Source File

SOURCE=..\ACQCenter\DbgPrt.h
# End Source File
# Begin Source File

SOURCE=.\FHParamDlg.h
# End Source File
# Begin Source File

SOURCE=.\InitACSDlg.h
# End Source File
# Begin Source File

SOURCE=.\ListenSocket.h
# End Source File
# Begin Source File

SOURCE=.\OWSSIO.h
# End Source File
# Begin Source File

SOURCE=.\OWSSock.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\SimOWS.h
# End Source File
# Begin Source File

SOURCE=.\SimOWSDlg.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\TestParamDlg.h
# End Source File
# Begin Source File

SOURCE=.\WndDisp.h
# End Source File
# Begin Source File

SOURCE=.\WndDispCointime.h
# End Source File
# Begin Source File

SOURCE=.\WndDispEnergy.h
# End Source File
# Begin Source File

SOURCE=.\WndDispSD.h
# End Source File
# Begin Source File

SOURCE=.\WndDispTime.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\bin00001.bin
# End Source File
# Begin Source File

SOURCE=.\res\pal1.bin
# End Source File
# Begin Source File

SOURCE=.\res\pal2.bin
# End Source File
# Begin Source File

SOURCE=.\res\SimOWS.ico
# End Source File
# Begin Source File

SOURCE=.\res\SimOWS.rc2
# End Source File
# End Group
# Begin Source File

SOURCE=.\res\map.pal
# End Source File
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
