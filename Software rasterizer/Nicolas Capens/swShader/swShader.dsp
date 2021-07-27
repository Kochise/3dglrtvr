# Microsoft Developer Studio Project File - Name="swShader" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=swShader - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "swShader.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "swShader.mak" CFG="swShader - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "swShader - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "swShader - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "swShader - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /W3 /GX /O2 /Ob2 /I "Shader" /I "Main" /I "Renderer" /I "Common" /I "SoftWire" /I "libjpeg" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x413 /d "NDEBUG"
# ADD RSC /l 0x413 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib dxguid.lib ddraw.lib /nologo /subsystem:windows /machine:I386

!ELSEIF  "$(CFG)" == "swShader - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "swShader___Win32_Debug"
# PROP BASE Intermediate_Dir "swShader___Win32_Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /W3 /Gm /GX /ZI /Od /I "Shader" /I "Main" /I "Renderer" /I "Common" /I "SoftWire" /I "libjpeg" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /FR /YX /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x413 /d "_DEBUG"
# ADD RSC /l 0x413 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib dxguid.lib ddraw.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept

!ENDIF 

# Begin Target

# Name "swShader - Win32 Release"
# Name "swShader - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Group "Shader"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Shader\Instruction.cpp
# End Source File
# Begin Source File

SOURCE=.\Shader\ps20_parser.cpp
# End Source File
# Begin Source File

SOURCE=.\Shader\ps20_scanner.cpp
# End Source File
# Begin Source File

SOURCE=.\Shader\ps20_token.cpp
# End Source File
# Begin Source File

SOURCE=.\Shader\PS20Assembler.cpp
# End Source File
# Begin Source File

SOURCE=.\Shader\PS20Shader.cpp
# End Source File
# End Group
# Begin Group "Main"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Main\Application.cpp
# End Source File
# Begin Source File

SOURCE=.\Main\FrameBuffer.cpp
# End Source File
# Begin Source File

SOURCE=.\Main\Main.cpp
# End Source File
# Begin Source File

SOURCE=.\Main\Model3DS.cpp
# End Source File
# Begin Source File

SOURCE=.\Main\Window.cpp
# End Source File
# End Group
# Begin Group "Renderer"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Renderer\Bitmap.cpp
# End Source File
# Begin Source File

SOURCE=.\Renderer\Clipper.cpp
# End Source File
# Begin Source File

SOURCE=.\Renderer\Color.cpp
# End Source File
# Begin Source File

SOURCE=.\Renderer\ColorBuffer.cpp
# End Source File
# Begin Source File

SOURCE=.\Renderer\Context.cpp
# End Source File
# Begin Source File

SOURCE=.\Renderer\IndexBuffer.cpp
# End Source File
# Begin Source File

SOURCE=.\Renderer\Matrix.cpp
# End Source File
# Begin Source File

SOURCE=.\Renderer\Point.cpp
# End Source File
# Begin Source File

SOURCE=.\Renderer\Rasterizer.cpp
# End Source File
# Begin Source File

SOURCE=.\Renderer\Renderer.cpp
# End Source File
# Begin Source File

SOURCE=.\Renderer\RenderTarget.cpp
# End Source File
# Begin Source File

SOURCE=.\Renderer\Sampler.cpp
# End Source File
# Begin Source File

SOURCE=.\Renderer\State.cpp
# End Source File
# Begin Source File

SOURCE=.\Renderer\Texture.cpp
# End Source File
# Begin Source File

SOURCE=.\Renderer\Vector.cpp
# End Source File
# Begin Source File

SOURCE=.\Renderer\VertexBuffer.cpp
# End Source File
# Begin Source File

SOURCE=.\Renderer\Viewport.cpp
# End Source File
# End Group
# Begin Group "Common"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Common\Error.cpp
# End Source File
# Begin Source File

SOURCE=.\Common\File.cpp
# End Source File
# Begin Source File

SOURCE=.\Common\Lockable.cpp
# End Source File
# Begin Source File

SOURCE=.\Common\Math.cpp
# End Source File
# Begin Source File

SOURCE=.\Common\String.cpp
# End Source File
# Begin Source File

SOURCE=.\Common\Timer.cpp
# End Source File
# End Group
# Begin Group "libjpeg"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\libjpeg\jcapimin.c
# End Source File
# Begin Source File

SOURCE=.\libjpeg\jcapistd.c
# End Source File
# Begin Source File

SOURCE=.\libjpeg\jccoefct.c
# End Source File
# Begin Source File

SOURCE=.\libjpeg\jccolor.c
# End Source File
# Begin Source File

SOURCE=.\libjpeg\jcdctmgr.c
# End Source File
# Begin Source File

SOURCE=.\libjpeg\jchuff.c
# End Source File
# Begin Source File

SOURCE=.\libjpeg\jcinit.c
# End Source File
# Begin Source File

SOURCE=.\libjpeg\jcmainct.c
# End Source File
# Begin Source File

SOURCE=.\libjpeg\jcmarker.c
# End Source File
# Begin Source File

SOURCE=.\libjpeg\jcmaster.c
# End Source File
# Begin Source File

SOURCE=.\libjpeg\jcomapi.c
# End Source File
# Begin Source File

SOURCE=.\libjpeg\jcparam.c
# End Source File
# Begin Source File

SOURCE=.\libjpeg\jcphuff.c
# End Source File
# Begin Source File

SOURCE=.\libjpeg\jcprepct.c
# End Source File
# Begin Source File

SOURCE=.\libjpeg\jcsample.c
# End Source File
# Begin Source File

SOURCE=.\libjpeg\jctrans.c
# End Source File
# Begin Source File

SOURCE=.\libjpeg\jdapimin.c
# End Source File
# Begin Source File

SOURCE=.\libjpeg\jdapistd.c
# End Source File
# Begin Source File

SOURCE=.\libjpeg\jdatadst.c
# End Source File
# Begin Source File

SOURCE=.\libjpeg\jdatasrc.c
# End Source File
# Begin Source File

SOURCE=.\libjpeg\jdcoefct.c
# End Source File
# Begin Source File

SOURCE=.\libjpeg\jdcolor.c
# End Source File
# Begin Source File

SOURCE=.\libjpeg\jddctmgr.c
# End Source File
# Begin Source File

SOURCE=.\libjpeg\jdhuff.c
# End Source File
# Begin Source File

SOURCE=.\libjpeg\jdinput.c
# End Source File
# Begin Source File

SOURCE=.\libjpeg\jdmainct.c
# End Source File
# Begin Source File

SOURCE=.\libjpeg\jdmarker.c
# End Source File
# Begin Source File

SOURCE=.\libjpeg\jdmaster.c
# End Source File
# Begin Source File

SOURCE=.\libjpeg\jdmerge.c
# End Source File
# Begin Source File

SOURCE=.\libjpeg\jdphuff.c
# End Source File
# Begin Source File

SOURCE=.\libjpeg\jdpostct.c
# End Source File
# Begin Source File

SOURCE=.\libjpeg\jdsample.c
# End Source File
# Begin Source File

SOURCE=.\libjpeg\jdtrans.c
# End Source File
# Begin Source File

SOURCE=.\libjpeg\jerror.c
# End Source File
# Begin Source File

SOURCE=.\libjpeg\jfdctflt.c
# End Source File
# Begin Source File

SOURCE=.\libjpeg\jfdctfst.c
# End Source File
# Begin Source File

SOURCE=.\libjpeg\jfdctint.c
# End Source File
# Begin Source File

SOURCE=.\libjpeg\jidctflt.c
# End Source File
# Begin Source File

SOURCE=.\libjpeg\jidctfst.c
# End Source File
# Begin Source File

SOURCE=.\libjpeg\jidctint.c
# End Source File
# Begin Source File

SOURCE=.\libjpeg\jidctred.c
# End Source File
# Begin Source File

SOURCE=.\libjpeg\jmemmgr.c
# End Source File
# Begin Source File

SOURCE=.\libjpeg\jmemnobs.c
# End Source File
# Begin Source File

SOURCE=.\libjpeg\jquant1.c
# End Source File
# Begin Source File

SOURCE=.\libjpeg\jquant2.c
# End Source File
# Begin Source File

SOURCE=.\libjpeg\jutils.c
# End Source File
# End Group
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Group "Shader."

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Shader\Instruction.hpp
# End Source File
# Begin Source File

SOURCE=.\Shader\Mnemonic.hpp
# End Source File
# Begin Source File

SOURCE=.\Shader\Operand.hpp
# End Source File
# Begin Source File

SOURCE=.\Shader\ps20_parser.hpp
# End Source File
# Begin Source File

SOURCE=.\Shader\ps20_scanner.hpp
# End Source File
# Begin Source File

SOURCE=.\Shader\ps20_token.hpp
# End Source File
# Begin Source File

SOURCE=.\Shader\PS20Assembler.hpp
# End Source File
# Begin Source File

SOURCE=.\Shader\PS20Shader.hpp
# End Source File
# Begin Source File

SOURCE=.\Shader\X86Shader.hpp
# End Source File
# End Group
# Begin Group "Main."

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Main\Application.hpp
# End Source File
# Begin Source File

SOURCE=.\Main\FrameBuffer.hpp
# End Source File
# Begin Source File

SOURCE=.\Main\Model3DS.hpp
# End Source File
# Begin Source File

SOURCE=.\Main\Window.hpp
# End Source File
# End Group
# Begin Group "Renderer."

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Renderer\Bitmap.hpp
# End Source File
# Begin Source File

SOURCE=.\Renderer\Clipper.hpp
# End Source File
# Begin Source File

SOURCE=.\Renderer\Color.hpp
# End Source File
# Begin Source File

SOURCE=.\Renderer\ColorBuffer.hpp
# End Source File
# Begin Source File

SOURCE=.\Renderer\Context.hpp
# End Source File
# Begin Source File

SOURCE=.\Renderer\FVF.hpp
# End Source File
# Begin Source File

SOURCE=.\Renderer\IndexBuffer.hpp
# End Source File
# Begin Source File

SOURCE=.\Renderer\LTVertex.hpp
# End Source File
# Begin Source File

SOURCE=.\Renderer\LVertex.hpp
# End Source File
# Begin Source File

SOURCE=.\Renderer\Matrix.hpp
# End Source File
# Begin Source File

SOURCE=.\Renderer\Point.hpp
# End Source File
# Begin Source File

SOURCE=.\Renderer\Rasterizer.hpp
# End Source File
# Begin Source File

SOURCE=.\Renderer\Renderer.hpp
# End Source File
# Begin Source File

SOURCE=.\Renderer\RenderTarget.hpp
# End Source File
# Begin Source File

SOURCE=.\Renderer\Sampler.hpp
# End Source File
# Begin Source File

SOURCE=.\Renderer\State.hpp
# End Source File
# Begin Source File

SOURCE=.\Renderer\Texture.hpp
# End Source File
# Begin Source File

SOURCE=.\Renderer\Vector.hpp
# End Source File
# Begin Source File

SOURCE=.\Renderer\Vertex.hpp
# End Source File
# Begin Source File

SOURCE=.\Renderer\VertexBuffer.hpp
# End Source File
# Begin Source File

SOURCE=.\Renderer\Viewport.hpp
# End Source File
# Begin Source File

SOURCE=.\Renderer\XVertex.hpp
# End Source File
# End Group
# Begin Group "Common."

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Common\Error.hpp
# End Source File
# Begin Source File

SOURCE=.\Common\File.hpp
# End Source File
# Begin Source File

SOURCE=.\Common\Lockable.hpp
# End Source File
# Begin Source File

SOURCE=.\Common\Math.hpp
# End Source File
# Begin Source File

SOURCE=.\Common\MetaMacro.hpp
# End Source File
# Begin Source File

SOURCE=.\Common\String.hpp
# End Source File
# Begin Source File

SOURCE=.\Common\Timer.hpp
# End Source File
# Begin Source File

SOURCE=.\Common\Types.hpp
# End Source File
# End Group
# Begin Group "libjpeg."

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\libjpeg\cderror.h
# End Source File
# Begin Source File

SOURCE=.\libjpeg\cdjpeg.h
# End Source File
# Begin Source File

SOURCE=.\libjpeg\jchuff.h
# End Source File
# Begin Source File

SOURCE=.\libjpeg\jconfig.h
# End Source File
# Begin Source File

SOURCE=.\libjpeg\jdct.h
# End Source File
# Begin Source File

SOURCE=.\libjpeg\jdhuff.h
# End Source File
# Begin Source File

SOURCE=.\libjpeg\jerror.h
# End Source File
# Begin Source File

SOURCE=.\libjpeg\jinclude.h
# End Source File
# Begin Source File

SOURCE=.\libjpeg\jmemsys.h
# End Source File
# Begin Source File

SOURCE=.\libjpeg\jmorecfg.h
# End Source File
# Begin Source File

SOURCE=.\libjpeg\jpegint.h
# End Source File
# Begin Source File

SOURCE=.\libjpeg\jpeglib.h
# End Source File
# Begin Source File

SOURCE=.\libjpeg\jversion.h
# End Source File
# End Group
# End Group
# Begin Source File

SOURCE=.\Shader\ps20.cg

!IF  "$(CFG)" == "swShader - Win32 Release"

# Begin Custom Build - Generating ps 2.0 parser
InputPath=.\Shader\ps20.cg

BuildCmds= \
	cd Shader \
	cppcc ps20.cg \
	cd .. \
	

"Shader/ps20_parser.hpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"Shader/ps20_scanner.hpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"Shader/ps20_token.hpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"Shader/ps20_parser.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"Shader/ps20_scanner.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"Shader/ps20_token.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "swShader - Win32 Debug"

# Begin Custom Build - Generating ps 2.0 parser
InputPath=.\Shader\ps20.cg

BuildCmds= \
	cd Shader \
	cppcc ps20.cg \
	cd .. \
	

"Shader/ps20_parser.hpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"Shader/ps20_scanner.hpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"Shader/ps20_token.hpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"Shader/ps20_parser.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"Shader/ps20_scanner.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"Shader/ps20_token.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Readme.html
# End Source File
# Begin Source File

SOURCE=.\Shader\Shader.txt
# End Source File
# Begin Source File

SOURCE=.\SoftWire\SoftWire.lib
# End Source File
# End Target
# End Project
