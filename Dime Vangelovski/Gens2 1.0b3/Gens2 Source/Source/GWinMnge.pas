/* **********************************************************
 * Copyright 2005 Dimitar Vangelovski
 *
 * This file is part of the Gens2 3D Engine.
 * 
 * The Gens2 3D Engine is free software; you can redistribute it and/or
 * modify it under the terms of version 2 of the GNU General Public
 * License as published by the Free Software Foundation.
 * 
 * The Gens2 3D Engine is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with the Gens2 3D Engine; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
 * 02110-1301, USA
 * **********************************************************/

{Unit for Windows Managing Functions -
                                                                      - Made by Dime Vangelovski
                                                                                     31.08.2005}
Unit GWinMnge;

Interface

Uses
     Windows,
     Messages;

Const
      WINDOW_CLASS_NAME =  'GENS 2';
      WINDOW_TITLE =       'Gens 2';
      STARTWINDOW_WIDTH =  400;
      STARTWINDOW_HEIGHT = 300;

      // Message related constants
      MSG_QUESTION =        1;
      MSG_INFO =            0;
      MSG_ERROR =           -1;
      __NEW_LINE__ =        ^m^j;
      __NULL_TERMINATOR__ = #0;

      // FPS Timer Constants
      TIMER_ID =   1;
      TIMER_RATE = 1000;

Var
    Key_Repeats:array[0..1000] of Integer;
    Mouse_Repeats:array[1..3] of Integer;
    Window_Closed: Boolean;
    g_AppActive : Boolean;
    Wind : hWnd := 0;

Function GM_CreateWindow(Instance: THandle; nCmdShow: Integer; Width, Height:Integer; ShowCaption:Boolean):Boolean;
Procedure GM_DestroyWindow(Instance: THandle);
Function DisplayMessage(MessageString:String; CaptionString:String; MessageType: Integer):Integer;
Procedure WindowText(Text: String; x, y: Integer; R, G, B: Byte);

Implementation

Uses
     Globals,
     GDDraw7M,
     GInput,
     GOut2Log;

Function CALLBACK_WindowProc conv arg_stdcall (Wnd: HWND; message: UINT; wParam: WPARAM; lParam: LPARAM): LResult;
begin
  case message of
      WM_CREATE:
      begin
        if (TIMER_ID <> SetTimer(Wnd, TIMER_ID, TIMER_RATE, NIL)) then
          begin
            DisplayMessage('Error initializing the Timer !!!', WINDOW_TITLE, MSG_ERROR);
            Write2Log('Error initializing the Timer !!!', true);
            Exit;
          end;
      end;

      WM_ACTIVATEAPP:
      begin
        g_AppActive := BOOLEAN(wParam);
        Result := 0;
        Exit;
      end;

      WM_TIMER:
      begin
        if (TIMER_ID = wParam) then
          begin
            CurrentFPS := FPS;
            FPS := 0;
          end;
      end;

      WM_LBUTTONDOWN:
      begin
        Inc(Mouse_Repeats[1]);
        Mouse_Buttons_INPUT(1, Mouse_Repeats[1]);
      end;

      WM_RBUTTONDOWN:
      begin
        Inc(Mouse_Repeats[2]);
        Mouse_Buttons_INPUT(2, Mouse_Repeats[2]);
      end;

      WM_MBUTTONDOWN:
      begin
        Inc(Mouse_Repeats[3]);
        Mouse_Buttons_INPUT(3, Mouse_Repeats[3]);
      end;

      WM_LBUTTONUP:
      begin
        Mouse_Repeats[1] := 0;
      end;

      WM_RBUTTONUP:
      begin
        Mouse_Repeats[2] := 0;
      end;

      WM_MBUTTONUP:
      begin
        Mouse_Repeats[3] := 0;
      end;

      WM_KEYDOWN:
      begin
        Inc(Key_Repeats[wParam]);

        if (wParam = VK_ESCAPE) then
          begin
            ShutDown_DirectDraw7;
            KillTimer(Wind, TIMER_ID);
            ShowCursor(TRUE);
            Write2Log('Posting quit message ... ', true);
            PostQuitMessage(0);
            Result := 0;
            Exit;
          end;

        if (wParam = VK_F9) then
          begin
            ScrShot := TRUE;
          end;

        Key_INPUT(wParam, Key_Repeats[wParam]);
      end;

      WM_KEYUP:
      begin
        Key_Repeats[wParam] := 0;
      end;

      WM_ERASEBKGND:
      begin
        if (Engine_Initialized) then
          begin
            Result := 1;
            Exit;
          end;
      end;

      WM_DESTROY:
      begin
        // Stay here, just in case i forget it exists
      end;
  end;
  Result := DefWindowProc(Wnd, message, wParam, lParam);
end;

Function GM_CreateWindow(Instance: THandle; nCmdShow: Integer; Width, Height:Integer; ShowCaption:Boolean):Boolean;
var
    WinClass: TWndClassEx;
begin
  if (Wind = 0) then
    begin
      Write2Log('Creating main window ...  ', false);

      // Set up and register window class
      WinClass.cbSize        := sizeof(TWndClassEx);
      WinClass.style         := CS_HREDRAW or CS_VREDRAW;
      WinClass.lpfnWndProc   := @CALLBACK_WindowProc;
      WinClass.cbClsExtra    := 0;
      WinClass.cbWndExtra    := 0;
      WinClass.hInstance     := Instance;
      WinClass.hIcon         := LoadIcon(0, IDI_WINLOGO);
      WinClass.hCursor       := LoadCursor(0, IDC_ARROW);
      WinClass.hbrBackground := GetStockObject(DKGRAY_BRUSH);
      WinClass.lpszMenuName  := '';
      WinClass.lpszClassName := WINDOW_CLASS_NAME;
      WinClass.hIconSm       := LoadIcon(NULL, IDI_WINLOGO);
      RegisterClassEx(WinClass);

      // Create a window
      if (ShowCaption) then
        begin
          Wind := CreateWindowEx(
                              0,
                              WINDOW_CLASS_NAME,
                              WINDOW_TITLE,
                              WS_POPUP or WS_VISIBLE or WS_CAPTION,
                              (GetSystemMetrics(SM_CXSCREEN) - Width) div 2,
                              (GetSystemMetrics(SM_CYSCREEN) - Height) div 2,
                              Width,
                              Height,
                              0,
                              0,
                              Instance,
                              NIL);
          if (Wind = 0) then
            begin
              Write2Log('FAILED !!!', true);
              Result := FALSE;
              Exit;
            end;
          UpdateWindow(Wind);
          SetFocus(Wind);
        end
      else
        begin
          Wind := CreateWindowEx(
                              0,
                              WINDOW_CLASS_NAME,
                              WINDOW_TITLE,
                              WS_POPUP or WS_VISIBLE,
                              0,
                              0,
                              GetSystemMetrics(SM_CXSCREEN),
                              GetSystemMetrics(SM_CYSCREEN),
                              0,
                              0,
                              Instance,
                              NIL);
          if (Wind = 0) then
            begin
              Write2Log('FAILED !!!', true);
              Result := FALSE;
              Exit;
            end;
          UpdateWindow(Wind);
          SetFocus(Wind);
          ShowCursor(FALSE);
        end;

      Write2Log('OK', true);
      Window_Closed := FALSE;
    end;
  Result := TRUE;
end;

Procedure GM_DestroyWindow(Instance: THandle);
begin
  if (Wind <> 0) then
    begin
      Write2Log('Destroying main window ...  ', false);

      DestroyWindow (Wind);
      UnregisterClass (WINDOW_CLASS_NAME, Instance);
      Window_Closed := TRUE;
      Wind := 0;

      Write2Log('OK', true);
    end;
end;

Function DisplayMessage(MessageString:String; CaptionString:String; MessageType: Integer):Integer;
begin
  // Null-Terminate the Message String
  MessageString := MessageString + __NULL_TERMINATOR__;
  CaptionString := CaptionString + __NULL_TERMINATOR__;

  // Information Message
  if MessageType = MSG_INFO then
    begin
      Result := MessageBox(Wind, @MessageString[1], @CaptionString[1], MB_OK or MB_TASKMODAL);
      Exit;
    end
  else
  // Error Message
    if MessageType = MSG_ERROR then
      begin
        Result := MessageBox(Wind, @MessageString[1], @CaptionString[1], MB_OK or MB_ICONERROR or MB_TASKMODAL);
        Exit;
      end
    else
    // Question Message
      if MessageType = MSG_QUESTION then
        begin
          Result := MessageBox(Wind, @MessageString[1], @CaptionString[1], MB_YESNO or MB_ICONQUESTION or MB_TASKMODAL);
          Exit;
        end;
end;

Procedure WindowText(Text: String; x, y: Integer; R, G, B: Byte);
Var
    DC: HDC;
begin
  DC := GetDC(Wind);
  SetBkMode (DC, TRANSPARENT);
  SetTextColor(DC, RGB(R, G, B));
  TextOut(DC, x, y, @Text[1], Length(Text));
  ReleaseDC(Wind, DC);
end;

END.