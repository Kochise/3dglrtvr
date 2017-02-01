# Microsoft Developer Studio Project File - Name="BotanicVis" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** NICHT BEARBEITEN **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=BotanicVis - Win32 Debug Coin
!MESSAGE Dies ist kein gültiges Makefile. Zum Erstellen dieses Projekts mit NMAKE
!MESSAGE verwenden Sie den Befehl "Makefile exportieren" und führen Sie den Befehl
!MESSAGE 
!MESSAGE NMAKE /f "BotanicVis.mak".
!MESSAGE 
!MESSAGE Sie können beim Ausführen von NMAKE eine Konfiguration angeben
!MESSAGE durch Definieren des Makros CFG in der Befehlszeile. Zum Beispiel:
!MESSAGE 
!MESSAGE NMAKE /f "BotanicVis.mak" CFG="BotanicVis - Win32 Debug Coin"
!MESSAGE 
!MESSAGE Für die Konfiguration stehen zur Auswahl:
!MESSAGE 
!MESSAGE "BotanicVis - Win32 Release" (basierend auf  "Win32 (x86) Application")
!MESSAGE "BotanicVis - Win32 Debug" (basierend auf  "Win32 (x86) Application")
!MESSAGE "BotanicVis - Win32 Debug Coin" (basierend auf  "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "BotanicVis - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x407 /d "NDEBUG"
# ADD RSC /l 0x407 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386

!ELSEIF  "$(CFG)" == "BotanicVis - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "BOTANIC_VIS___Win32_Debug_Coin"
# PROP Intermediate_Dir "BOTANIC_VIS___Win32_Debug_Coin"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /W3 /GX /ZI /Od /I "$(ACEROOT)" /I "$(XERCESCROOT)/include" /I "$(STBROOT)/src" /I "$(COINROOT)\INCLUDE" /I "$(COINROOT)\INCLUDE\INVENTOR" /I "../" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x407 /d "_DEBUG"
# ADD RSC /l 0x407 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 coin2d.lib aced.lib xerces-c_2d.lib stbapid.lib /nologo /subsystem:windows /debug /machine:I386 /out:"../../../lib/BotanicVisd.dll" /pdbtype:sept /libpath:"$(COINROOT)\LIB" /libpath:"$(ACEROOT)/ace" /libpath:"$(XERCESCROOT)/lib" /libpath:"$(STBROOT)/lib"

!ELSEIF  "$(CFG)" == "BotanicVis - Win32 Debug Coin"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "BotanicVis___Win32_Debug_Coin"
# PROP BASE Intermediate_Dir "BotanicVis___Win32_Debug_Coin"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "BOTANIC_VIS___Win32_Debug_Coin"
# PROP Intermediate_Dir "BOTANIC_VIS___Win32_Debug_Coin"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /ZI /Od /I "$(ACEROOT)" /I "$(XERCESCROOT)/include" /I "$(STBROOT)/src" /I "$(COINROOT)\INCLUDE" /I "$(COINROOT)\INCLUDE\INVENTOR" /I "../" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /GX /ZI /Od /I "$(ACEROOT)" /I "$(XERCESCROOT)/include" /I "$(STBROOT)/src" /I "$(COINROOT)\INCLUDE" /I "$(COINROOT)\INCLUDE\INVENTOR" /I "../" /D "HA" /D "WIN32" /D "BOTANIC_VIS_EXPORTS" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "USE_COIN" /D "COIN_DLL" /FR /YX /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x407 /d "_DEBUG"
# ADD RSC /l 0x407 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 coin2d.lib aced.lib xerces-c_2d.lib stbapid.lib /nologo /subsystem:windows /debug /machine:I386 /out:"../../../lib/BotanicVisd.dll" /pdbtype:sept /libpath:"$(COINROOT)\LIB" /libpath:"$(ACEROOT)/ace" /libpath:"$(XERCESCROOT)/lib" /libpath:"$(STBROOT)/lib"
# ADD LINK32 coin2d.lib aced.lib xerces-c_2d.lib stbapid.lib /nologo /dll /debug /machine:I386 /out:"../../../lib/BotanicVisd.dll" /pdbtype:sept /libpath:"$(COINROOT)\LIB" /libpath:"$(ACEROOT)/ace" /libpath:"$(XERCESCROOT)/lib" /libpath:"$(STBROOT)/lib"
# SUBTRACT LINK32 /pdb:none

!ENDIF 

# Begin Target

# Name "BotanicVis - Win32 Release"
# Name "BotanicVis - Win32 Debug"
# Name "BotanicVis - Win32 Debug Coin"
# Begin Group "Quellcodedateien"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\BotanicVis.cxx
# End Source File
# Begin Source File

SOURCE=.\BotanicVis.h
# End Source File
# Begin Source File

SOURCE=.\SoBotanicalBase.cxx
# End Source File
# Begin Source File

SOURCE=.\SoBotanicalBase.h
# End Source File
# Begin Source File

SOURCE=.\SoBotanicalBranch.cxx
# End Source File
# Begin Source File

SOURCE=.\SoBotanicalBranch.h
# End Source File
# Begin Source File

SOURCE=.\SoBotanicalFruit.cxx
# End Source File
# Begin Source File

SOURCE=.\SoBotanicalFruit.h
# End Source File
# Begin Source File

SOURCE=.\SoBotanicVis.cxx
# End Source File
# Begin Source File

SOURCE=.\SoBotanicVis.h
# End Source File
# Begin Source File

SOURCE=.\SoBuildBotanicAction.cxx
# End Source File
# Begin Source File

SOURCE=.\SoBuildBotanicAction.h
# End Source File
# Begin Source File

SOURCE=.\SoTreeBase.cxx
# End Source File
# Begin Source File

SOURCE=.\SoTreeBase.h
# End Source File
# Begin Source File

SOURCE=.\SoTreeLeaf.cxx
# End Source File
# Begin Source File

SOURCE=.\SoTreeLeaf.h
# End Source File
# Begin Source File

SOURCE=.\SoTreeNode.cxx
# End Source File
# Begin Source File

SOURCE=.\SoTreeNode.h
# End Source File
# End Group
# Begin Group "Header-Dateien"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# End Group
# Begin Group "Ressourcendateien"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# End Target
# End Project
