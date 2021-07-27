// TreeMapTest.cpp : main source file for TreeMapTest.exe
//

#include "stdafx.h"

#include "resource.h"

#include "MainFrm.h"

CMyAppModule _Module;


int Run(LPTSTR /*lpstrCmdLine*/ = NULL, int nCmdShow = SW_SHOWDEFAULT)
{
   CMessageLoop theLoop;
   _Module.AddMessageLoop(&theLoop);

   CMainFrame wndMain;
   if( wndMain.CreateEx() == NULL ) {
      ATLTRACE(_T("Main window creation failed!\n"));
      return 0;
   }
   wndMain.ShowWindow(nCmdShow);

   int nRet = theLoop.Run();

   _Module.RemoveMessageLoop();
   return nRet;
}


int WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE /*hPrevInstance*/, LPTSTR lpstrCmdLine, int nCmdShow)
{
#ifdef _DEBUG
   _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF); 
#endif // _DEBUG

   HRESULT hRes = ::CoInitialize(NULL);
   ATLASSERT(SUCCEEDED(hRes));

   ::DefWindowProc(NULL, 0, 0, 0L);

   AtlInitCommonControls(ICC_COOL_CLASSES | ICC_BAR_CLASSES);

   hRes = _Module.Init(NULL, hInstance);
   _Module.m_Allocator.Init();

   int nRet = Run(lpstrCmdLine, nCmdShow);

   _Module.m_Allocator.Term();
   _Module.Term();
   ::CoUninitialize();

#ifdef _DEBUG
   _CrtDumpMemoryLeaks();
#endif // _DEBUG

   return nRet;
}

