# Microsoft Developer Studio Project File - Name="VirtualGridDemo" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=VirtualGridDemo - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "VirtualGridDemo.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "VirtualGridDemo.mak" CFG="VirtualGridDemo - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "VirtualGridDemo - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "VirtualGridDemo - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "VirtualGridDemo - Win32 Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x422 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x422 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 /nologo /subsystem:windows /machine:I386

!ELSEIF  "$(CFG)" == "VirtualGridDemo - Win32 Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x422 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x422 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept

!ENDIF 

# Begin Target

# Name "VirtualGridDemo - Win32 Release"
# Name "VirtualGridDemo - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\CellSettingsDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\CodeGenPage.cpp
# End Source File
# Begin Source File

SOURCE=.\ColumnsPage.cpp
# End Source File
# Begin Source File

SOURCE=.\DropListEditDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\FixedPageDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\GeneralPageDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\GridButton.cpp
# End Source File
# Begin Source File

SOURCE=.\GridEdit.cpp
# End Source File
# Begin Source File

SOURCE=.\GridListBox.cpp
# End Source File
# Begin Source File

SOURCE=.\GridPropertiesSheet.cpp
# End Source File
# Begin Source File

SOURCE=.\HeaderPageDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\HeadersPage.cpp
# End Source File
# Begin Source File

SOURCE=.\MyColorButton.cpp
# End Source File
# Begin Source File

SOURCE=.\RollupCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\RowsPageDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\StylesPageDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\TitleTip.cpp
# End Source File
# Begin Source File

SOURCE=.\VirtualGridCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\VirtualGridDemo.cpp
# End Source File
# Begin Source File

SOURCE=.\VirtualGridDemo.rc
# End Source File
# Begin Source File

SOURCE=.\VirtualGridDemoDlg.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\CellSettingsDlg.h
# End Source File
# Begin Source File

SOURCE=.\CodeGenPage.h
# End Source File
# Begin Source File

SOURCE=.\ColumnsPage.h
# End Source File
# Begin Source File

SOURCE=.\DropListEditDlg.h
# End Source File
# Begin Source File

SOURCE=.\FixedPageDlg.h
# End Source File
# Begin Source File

SOURCE=.\GeneralPageDlg.h
# End Source File
# Begin Source File

SOURCE=.\GridButton.h
# End Source File
# Begin Source File

SOURCE=.\GridEdit.h
# End Source File
# Begin Source File

SOURCE=.\GridListBox.h
# End Source File
# Begin Source File

SOURCE=.\GridPropertiesSheet.h
# End Source File
# Begin Source File

SOURCE=.\HeaderPageDlg.h
# End Source File
# Begin Source File

SOURCE=.\HeadersPage.h
# End Source File
# Begin Source File

SOURCE=.\MemDC.h
# End Source File
# Begin Source File

SOURCE=.\MyColorButton.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\RollupCtrl.h
# End Source File
# Begin Source File

SOURCE=.\RowsPageDlg.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\StylesPageDlg.h
# End Source File
# Begin Source File

SOURCE=.\TitleTip.h
# End Source File
# Begin Source File

SOURCE=.\VirtualGridCtrl.h
# End Source File
# Begin Source File

SOURCE=.\VirtualGridDemo.h
# End Source File
# Begin Source File

SOURCE=.\VirtualGridDemoDlg.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\PAINT.BMP
# End Source File
# Begin Source File

SOURCE=.\res\VirtualGridDemo.ico
# End Source File
# Begin Source File

SOURCE=.\res\VirtualGridDemo.rc2
# End Source File
# End Group
# End Target
# End Project
