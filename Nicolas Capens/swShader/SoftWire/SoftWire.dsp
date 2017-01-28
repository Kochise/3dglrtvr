# Microsoft Developer Studio Project File - Name="SoftWire" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=SoftWire - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "SoftWire.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "SoftWire.mak" CFG="SoftWire - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "SoftWire - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "SoftWire - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "SoftWire - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "SoftWire___Win32_Release"
# PROP BASE Intermediate_Dir "SoftWire___Win32_Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD BASE RSC /l 0x813 /d "NDEBUG"
# ADD RSC /l 0x813 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "SoftWire - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "SoftWire___Win32_Debug"
# PROP BASE Intermediate_Dir "SoftWire___Win32_Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ  /c
# ADD CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ  /c
# ADD BASE RSC /l 0x813 /d "_DEBUG"
# ADD RSC /l 0x813 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ENDIF 

# Begin Target

# Name "SoftWire - Win32 Release"
# Name "SoftWire - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\Assembler.cpp
# End Source File
# Begin Source File

SOURCE=.\Encoding.cpp
# End Source File
# Begin Source File

SOURCE=.\Error.cpp
# End Source File
# Begin Source File

SOURCE=.\Instruction.cpp
# End Source File
# Begin Source File

SOURCE=.\InstructionSet.cpp
# End Source File
# Begin Source File

SOURCE=.\Linker.cpp
# End Source File
# Begin Source File

SOURCE=.\Loader.cpp
# End Source File
# Begin Source File

SOURCE=.\Macro.cpp
# End Source File
# Begin Source File

SOURCE=.\Operand.cpp
# End Source File
# Begin Source File

SOURCE=.\Parser.cpp
# End Source File
# Begin Source File

SOURCE=.\Scanner.cpp
# End Source File
# Begin Source File

SOURCE=.\Synthesizer.cpp
# End Source File
# Begin Source File

SOURCE=.\Token.cpp
# End Source File
# Begin Source File

SOURCE=.\TokenList.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\Assembler.hpp
# End Source File
# Begin Source File

SOURCE=.\CharType.hpp
# End Source File
# Begin Source File

SOURCE=.\Encoding.hpp
# End Source File
# Begin Source File

SOURCE=.\Error.hpp
# End Source File
# Begin Source File

SOURCE=.\File.hpp
# End Source File
# Begin Source File

SOURCE=.\Instruction.hpp
# End Source File
# Begin Source File

SOURCE=.\InstructionSet.hpp
# End Source File
# Begin Source File

SOURCE=.\Intrinsics.hpp
# End Source File
# Begin Source File

SOURCE=.\Link.hpp
# End Source File
# Begin Source File

SOURCE=.\Linker.hpp
# End Source File
# Begin Source File

SOURCE=.\Loader.hpp
# End Source File
# Begin Source File

SOURCE=.\Macro.hpp
# End Source File
# Begin Source File

SOURCE=.\Operand.hpp
# End Source File
# Begin Source File

SOURCE=.\Parser.hpp
# End Source File
# Begin Source File

SOURCE=.\Scanner.hpp
# End Source File
# Begin Source File

SOURCE=.\String.hpp
# End Source File
# Begin Source File

SOURCE=.\Synthesizer.hpp
# End Source File
# Begin Source File

SOURCE=.\Token.hpp
# End Source File
# Begin Source File

SOURCE=.\TokenList.hpp
# End Source File
# End Group
# End Target
# End Project
