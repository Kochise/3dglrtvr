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

{Unit for Log file writing Functions -
                                                                      - Made by Dime Vangelovski
                                                                                     21.11.2005}
Unit GOut2Log;

Interface

Procedure Write2Log(KeyNote:String; NewLine:Boolean);
Procedure Describe_Last_Error(NewLine:Boolean);
Procedure WriteSumTime;
Procedure StartEndTime;

Implementation

Uses
     Windows;

Const
      Day : array[0..6] of String = ('Sunday',
                                     'Monday',
                                     'Tuesday',
                                     'Wednesday',
                                     'Thursday',
                                     'Friday',
                                     'Saturday');

      Month : array[1..12] of String = ('January',
                                        'February',
                                        'March',
                                        'April',
                                        'May',
                                        'June',
                                        'July',
                                        'August',
                                        'September',
                                        'October',
                                        'November',
                                        'December');
Var
    Cannot_Create_File : Boolean := false;
    StartTime, GensStartTime : DWORD;
    GensStarted : Boolean := false;
    CreateOnce : Boolean := false;
    OldNewLine : Boolean := false;
    SumTime : Integer := 0;
    CurrentTime : Integer;
    STime : TSystemTime;
    LogFile : Text;

Procedure Write2Log(KeyNote:String; NewLine:Boolean);
begin
  if (Cannot_Create_File) then
    Exit;

  if NOT(CreateOnce) then
    begin
      Assign(LogFile, 'Gens2.log');

{$I-}
      ReWrite(LogFile);

      if (IOResult <> 0) then
        begin
          MessageBox(0, 'Can''t create log file, click OK to continue.', 'Gens 2', MB_OK or MB_ICONINFORMATION);
          Cannot_Create_File := true;
          Exit;
        end;
{$I+}

      WriteLn(LogFile);
      WriteLn(LogFile, '--------------------------------------------------------------------------');
      WriteLn(LogFile, '--- Gens II 3D Engine ver 1.0 beta 3 (c) 2004-2005 by Dime Vangelovski ---');
      WriteLn(LogFile, '--------------------------------------------------------------------------');
      WriteLn(LogFile);
      WriteLn(LogFile, '--- This log file was automatically created by the Gens 2 3D Engine ---');
      WriteLn(LogFile);

      GetLocalTime(STime);

      WriteLn(LogFile, 'Log file created : ', Day[STime.wDayOfWeek], ', ', Month[STime.wMonth], ' ', STime.wDay, ', ',
              STime.wYear, ' ; ', STime.wHour, ':', STime.wMinute, ':', STime.wSecond);
      WriteLn(LogFile);
      WriteLn(LogFile);

      if (KeyNote = '') then
        WriteLn(LogFile)
      else
        if (NewLine) then
          begin
            if NOT(OldNewLine) then
              begin
                CurrentTime := GetTickCount - StartTime;
                WriteLn(LogFile, KeyNote, ' --- ', CurrentTime, ' ms.');
                SumTime +:= CurrentTime;
              end
            else
              WriteLn(LogFile, KeyNote);
          end
        else
          begin
            Write(LogFile, KeyNote);
            StartTime := GetTickCount;
          end;

      Close(LogFile);
      CreateOnce := true;
      OldNewLine := NewLine;
    end
  else
    begin
      Assign(LogFile, 'Gens2.log');
      Append(LogFile);

      if (KeyNote = '') then
        WriteLn(LogFile)
      else
        if (NewLine) then
          begin
            if NOT(OldNewLine) then
              begin
                CurrentTime := GetTickCount - StartTime;
                WriteLn(LogFile, KeyNote, ' --- ', CurrentTime, ' ms.');
                SumTime +:= CurrentTime;
              end
            else
              WriteLn(LogFile, KeyNote);
          end
        else
          begin
            Write(LogFile, KeyNote);
            StartTime := GetTickCount;
          end;

      Close(LogFile);
      OldNewLine := NewLine;
    end;
end;

Procedure WriteSumTime;
begin
  if (Cannot_Create_File) then
    Exit;

  Assign(LogFile, 'Gens2.log');
  Append(LogFile);

  WriteLn(LogFile, 'Total time for this block is : ', SumTime, ' ms.');
  SumTime := 0;

  Close(LogFile);
end;

Procedure WriteFormattedTime(Time: DWORD);
Var
    tempMin,tempSec,Hours,Minutes,Seconds:DWORD;

begin
  if (Time <= 999) then
    begin
      WriteLn(LogFile, Time, ' milliseconds.');
      Exit;
    end
  else
    begin
      tempSec := Time div 1000;
      tempMin := tempSec div 60;
      Seconds := tempSec - (tempMin * 60);
      Hours := tempMin div 60;
      Minutes := tempMin - (Hours * 60);

      if (Hours > 0) then
        WriteLn(LogFile, Hours, ' hours, ', Minutes, ' minutes, ', Seconds, ' seconds.')
      else
        if (Minutes > 0) then
          WriteLn(LogFile, Minutes, ' minutes, ', Seconds, ' seconds.')
        else
          WriteLn(LogFile, Seconds, ' seconds.')
    end;
end;

Procedure StartEndTime;
begin
  if (Cannot_Create_File) then
    Exit;

  if NOT(GensStarted) then
    begin
      GensStartTime := GetTickCount;
      GensStarted := true;
    end
  else
    begin
      Assign(LogFile, 'Gens2.log');
      Append(LogFile);

      Write(LogFile, 'Gens 2 total run time : ');
      WriteFormattedTime(GetTickCount - GensStartTime);

      Close(LogFile);
    end;
end;

Procedure Describe_Last_Error(NewLine:Boolean);
Var
    MsgBuf: array[0..255] of Char;
    ErrorMessage: String;
    MsgLength: DWORD;
begin
  if (Cannot_Create_File) then
    Exit;

  MsgLength := FormatMessage(
                      FORMAT_MESSAGE_ARGUMENT_ARRAY or FORMAT_MESSAGE_FROM_SYSTEM,
                      NIL,
                      GetLastError,
                      0,
                      @MsgBuf,
                      SizeOf(MsgBuf),
                      NIL);

  while ((MsgBuf[MsgLength - 1] in [#0..#32]) and (MsgLength > 0)) do
    Dec(MsgLength);

  SetString(ErrorMessage, MsgBuf, MsgLength);
  Write2Log(ErrorMessage, NewLine);
end;

END.