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

{Unit for Direct Draw 7 Managing Functions -
                                                                      - Made by Dime Vangelovski
                                                                                     25.08.2005}
Unit GDDraw7M;

Interface

Procedure Init_DirectDraw7(XResolution, YResolution, BPPixel, ScrRefresh: Dword);
Procedure Set_Mode(XResolution, YResolution, BPPixel, ScrRefresh: Dword);
Procedure ShutDown_DirectDraw7;
procedure RestoreSurfaces;
procedure SurfaceLock;
procedure SurfaceUnlock;
procedure FlipSurfaces;
Procedure BlitSurface;
Procedure GetDCHandle;
Procedure ReleaseDCHandle;
Procedure WriteTxt32bpp(Text:String; x, y:Integer; R, G, B:Byte);
Procedure WriteNo32bpp(Number:Single; c, d, x, y:Integer; R, G, B:Byte);

Function Get_DDraw_Error (Error_Code: HResult): String;

Implementation

Uses
     Globals,
     Windows,
     DDraw,
     GWinMnge,
     GOut2Log;

Const
      DDRAW_ERROR_TITLE = 'Direct Draw 7 Error Message';
      Display_Mode : array[0..4] of String = ('320x200',
                                              '512x384',
                                              '640x480',
                                              '1024x768',
                                              '1280x960');

Var
    g_DD :          IDirectDraw7 := NIL;          // DirectDraw Object
    g_DDSPrimary :  IDirectDrawSurface7 := NIL;   // DirectDraw Primary surface
    g_DDSBack :     IDirectDrawSurface7 := NIL;   // DirectDraw Back surface
    g_DDSOffScr :   IDirectDrawSurface7 := NIL;   // DirectDraw Back surface
    g_DDSDesc :     TDDSurfaceDesc2;
    g_DDSCaps :     TDDSCaps2;
    DDResult :      HResult;
    g_DC :          HDC;


Procedure SetResolutionConstants(XRes, YRes: DWord);
begin
  Res_Xend := XRes - 1;
  Res_Xend_p1 := XRes;
  Res_Xend_m1 := Res_Xend - 1;

  Res_Yend := YRes - 1;
  Res_Yend_p1 := YRes;
  Res_Yend_m1 := Res_Yend - 1;

  Res_PixNum := XRes * YRes;
  Xcenter := Xres shr 1;
  Ycenter := Yres shr 1;

  if (XRes = 320) then
    begin
      Res_Shl1 := 8;
      Res_Shl2 := 6;
      Res_Mask := $FFFFFFFF;
      Vid_YMove_Factor := 4;
      Vid_Mode := 0;
    end
  else
    if (XRes = 512) then
      begin
        Res_Shl1 := 9;
        Res_Shl2 := 0;
        Res_Mask := 0;
        Vid_YMove_Factor := 8;
        Vid_Mode := 1;
      end
    else
      if (XRes = 640) then
        begin
          Res_Shl1 := 9;
          Res_Shl2 := 7;
          Res_Mask := $FFFFFFFF;
          Vid_YMove_Factor := 8;
          Vid_Mode := 2;
        end
      else
        if (XRes = 1024) then
          begin
            Res_Shl1 := 10;
            Res_Shl2 := 0;
            Res_Mask := 0;
            Vid_YMove_Factor := 8;
            Vid_Mode := 3;
          end
        else
          if (XRes = 1280) then
            begin
              Res_Shl1 := 10;
              Res_Shl2 := 8;
              Res_Mask := $FFFFFFFF;
              Vid_YMove_Factor := 8;
              Vid_Mode := 4;
            end;
end;

Procedure Init_DirectDraw7(XResolution, YResolution, BPPixel, ScrRefresh: Dword);
begin
  Write2Log('///////////////////////////////////////////////////////////////////', true);
  Write2Log('Initializing Direct Draw 7 ...  ', true);

  Write2Log('Creating Direct Draw 7 Object ...  ', false);
  if FAILED(DirectDrawCreateEx(NIL, g_DD, IID_IDirectDraw7, NIL)) then
    begin
      Describe_Last_Error(true);
      DisplayMessage('Error initializing Direct Draw !!!', DDRAW_ERROR_TITLE, MSG_ERROR);
      Exit;
    end
  else
    Write2Log('OK', true);

  Write2Log('Setting cooperative level ...  ', false);
  if FAILED(g_DD.SetCooperativeLevel(Wind, DDSCL_EXCLUSIVE or DDSCL_FULLSCREEN)) then
    begin
      Describe_Last_Error(true);
      DisplayMessage('Error setting cooperative level !!!', DDRAW_ERROR_TITLE, MSG_ERROR);
      Exit;
    end
  else
    Write2Log('OK', true);

  Write2Log('Setting display mode ' + Display_Mode[Vid_Mode] + ' ...  ', false);
  if FAILED(g_DD.SetDisplayMode(XResolution, YResolution, BPPixel, ScrRefresh, 0)) then
    begin
      Write2Log('Error setting display mode ' + Display_Mode[Vid_Mode], true);
      Write2Log('Trying to set the same mode with 60 Hz refresh ...  ', false);
      if FAILED(g_DD.SetDisplayMode(XResolution, YResolution, BPPixel, 60, 0)) then
        begin
          Write2Log('Error setting display mode ' + Display_Mode[Vid_Mode], true);
          Write2Log('Exiting Gens 2 ...  ', true);
          Describe_Last_Error(true);
          DisplayMessage('Error setting display mode !!!', DDRAW_ERROR_TITLE, MSG_ERROR);
          Exit;
        end
      else
        Write2Log('OK', true);
    end
  else
    Write2Log('OK', true);

  Write2Log('Creating Direct Draw 7 Surfaces ...  ', true);
  // Create surfaces
  ZeroMemory(@g_DDSDesc, sizeof(g_DDSDesc));
  g_DDSDesc.dwSize := sizeof(g_DDSDesc);
  g_DDSDesc.dwFlags := DDSD_CAPS or DDSD_BACKBUFFERCOUNT;
  g_DDSDesc.ddsCaps.dwCaps := DDSCAPS_PRIMARYSURFACE or DDSCAPS_FLIP or DDSCAPS_COMPLEX or DDSCAPS_SYSTEMMEMORY;
  g_DDSDesc.dwBackBufferCount := 2;

  Write2Log('Creating front surface ...  ', false);
  // Create front surface
  if FAILED(g_DD.CreateSurface(g_DDSDesc, g_DDSPrimary, NIL)) then
    begin
      Describe_Last_Error(true);
      DisplayMessage('Error creating primary surface !!!', DDRAW_ERROR_TITLE, MSG_ERROR);
      Exit;
    end
  else
    Write2Log('OK', true);

  Write2Log('Creating back surface ...  ', false);
  // Create back surface
  ZeroMemory(@g_DDSCaps, sizeof(g_DDSCaps));
  g_DDSCaps.dwCaps := DDSCAPS_BACKBUFFER or DDSCAPS_SYSTEMMEMORY;
  if FAILED(g_DDSPrimary.GetAttachedSurface(g_DDSCaps, g_DDSBack)) then
    begin
      Describe_Last_Error(true);
      DisplayMessage('Error creating back surface !!!', DDRAW_ERROR_TITLE, MSG_ERROR);
      Exit;
    end
  else
    Write2Log('OK', true);

  Write2Log('Creating offscreen surface ...  ', false);
  // Create offscreen surface
  g_DDSDesc.dwFlags := DDSD_CAPS or DDSD_HEIGHT or DDSD_WIDTH;
  g_DDSDesc.ddsCaps.dwCaps := DDSCAPS_OFFSCREENPLAIN or DDSCAPS_SYSTEMMEMORY;
  g_DDSDesc.dwHeight := Yresolution;
  g_DDSDesc.dwWidth := Xresolution;
  if FAILED(g_DD.CreateSurface(g_DDSDesc, g_DDSOffScr, NIL)) then
    begin
      Describe_Last_Error(true);
      DisplayMessage('Error creating offscreen surface !!!', DDRAW_ERROR_TITLE, MSG_ERROR);
      Exit;
    end
  else
    Write2Log('OK', true);

  Write2Log('Direct Draw 7 Successfully Initialized.', true);
  WriteSumTime;
  Write2Log('///////////////////////////////////////////////////////////////////', true);
  SetResolutionConstants(Xresolution, Yresolution);
end;

Procedure Set_Mode(XResolution, YResolution, BPPixel, ScrRefresh: Dword);
begin
  Write2Log('///////////////////////////////////////////////////////////////////', true);
  Write2Log('Changing display mode ...  ', true);

  Write2Log('Releasing Direct Draw Surfaces ...  ', false);

  // Release Direct Draw Surfaces
  if (g_DDSOffScr <> NIL) then
    begin
      g_DDSOffScr.Release;
      g_DDSOffScr := NIL;
    end;
  if (g_DDSBack <> NIL) then
    begin
      g_DDSBack.Release;
      g_DDSBack := NIL;
    end;
  if (g_DDSPrimary <> NIL) then
    begin
      g_DDSPrimary.Release;
      g_DDSPrimary := NIL;
    end;
  Write2Log('OK', true);

  Write2Log('Setting cooperative level ...  ', false);
  if FAILED(g_DD.SetCooperativeLevel(Wind, DDSCL_EXCLUSIVE or DDSCL_FULLSCREEN)) then
    begin
      Describe_Last_Error(true);
      DisplayMessage('Error setting cooperative level !!!', DDRAW_ERROR_TITLE, MSG_ERROR);
      Exit;
    end
  else
    Write2Log('OK', true);

  Write2Log('Setting display mode ' + Display_Mode[Vid_Mode] + ' ...  ', false);
  if FAILED(g_DD.SetDisplayMode(XResolution, YResolution, BPPixel, ScrRefresh, 0)) then
    begin
      Describe_Last_Error(true);
      DisplayMessage('Error setting display mode !!!', DDRAW_ERROR_TITLE, MSG_ERROR);
      Exit;
    end
  else
    Write2Log('OK', true);

  Write2Log('Creating Direct Draw 7 Surfaces ...  ', true);
  // Create surfaces
  ZeroMemory(@g_DDSDesc, sizeof(g_DDSDesc));
  g_DDSDesc.dwSize := sizeof(g_DDSDesc);
  g_DDSDesc.dwFlags := DDSD_CAPS or DDSD_BACKBUFFERCOUNT;
  g_DDSDesc.ddsCaps.dwCaps := DDSCAPS_PRIMARYSURFACE or DDSCAPS_FLIP or DDSCAPS_COMPLEX or DDSCAPS_SYSTEMMEMORY;
  g_DDSDesc.dwBackBufferCount := 2;

  Write2Log('Creating front surface ...  ', false);
  // Create front surface
  if FAILED(g_DD.CreateSurface(g_DDSDesc, g_DDSPrimary, NIL)) then
    begin
      Describe_Last_Error(true);
      DisplayMessage('Error creating primary surface !!!', DDRAW_ERROR_TITLE, MSG_ERROR);
      Exit;
    end
  else
    Write2Log('OK', true);

  Write2Log('Creating back surface ...  ', false);
  // Create back surface
  ZeroMemory(@g_DDSCaps, sizeof(g_DDSCaps));
  g_DDSCaps.dwCaps := DDSCAPS_BACKBUFFER or DDSCAPS_SYSTEMMEMORY;
  if FAILED(g_DDSPrimary.GetAttachedSurface(g_DDSCaps, g_DDSBack)) then
    begin
      Describe_Last_Error(true);
      DisplayMessage('Error creating back surface !!!', DDRAW_ERROR_TITLE, MSG_ERROR);
      Exit;
    end
  else
    Write2Log('OK', true);

  Write2Log('Creating offscreen surface ...  ', false);
  // Create offscreen surface
  g_DDSDesc.dwFlags := DDSD_CAPS or DDSD_HEIGHT or DDSD_WIDTH;
  g_DDSDesc.ddsCaps.dwCaps := DDSCAPS_OFFSCREENPLAIN or DDSCAPS_SYSTEMMEMORY;
  g_DDSDesc.dwHeight := Yresolution;
  g_DDSDesc.dwWidth := Xresolution;
  if FAILED(g_DD.CreateSurface(g_DDSDesc, g_DDSOffScr, NIL)) then
    begin
      Describe_Last_Error(true);
      DisplayMessage('Error creating offscreen surface !!!', DDRAW_ERROR_TITLE, MSG_ERROR);
      Exit;
    end
  else
    Write2Log('OK', true);

  Write2Log('Display mode changed successfully.', true);
  WriteSumTime;
  Write2Log('///////////////////////////////////////////////////////////////////', true);
  SetResolutionConstants(Xresolution, Yresolution);
end;

Procedure ShutDown_DirectDraw7;
begin
  Write2Log('', true);
  Write2Log('Shutting Down Direct Draw 7 ...  ', false);

  // Release Direct Draw Surfaces
  if (g_DDSOffScr <> NIL) then
    begin
      g_DDSOffScr.Release;
      g_DDSOffScr := NIL;
    end;
  if (g_DDSBack <> NIL) then
    begin
      g_DDSBack.Release;
      g_DDSBack := NIL;
    end;
  if (g_DDSPrimary <> NIL) then
    begin
      g_DDSPrimary.Release;
      g_DDSPrimary := NIL;
    end;
  // Release Direct Draw Object
  if (g_DD <> NIL) then
    begin
      g_DD.Release;
      g_DD := NIL;
    end;
  Describe_Last_Error(true);
end;

procedure RestoreSurfaces;
begin
  if FAILED(g_DDSPrimary.Restore) then
    begin
      Write2Log('Error while restoring front surface : ', false);
      Describe_Last_Error(true);
      DisplayMessage('Error restoring primary surface !!!', DDRAW_ERROR_TITLE, MSG_ERROR);
      Exit;
    end
  else
    if FAILED(g_DDSBack.Restore) then
      begin
        Write2Log('Error while restoring back surface : ', false);
        Describe_Last_Error(true);
        DisplayMessage('Error restoring rendering surface !!!', DDRAW_ERROR_TITLE, MSG_ERROR);
        Exit;
      end;
end;

procedure SurfaceLock;
begin
  ZeroMemory(@g_DDSDesc, sizeof(g_DDSDesc));
  g_DDSDesc.dwSize := sizeof(g_DDSDesc);

  DDResult := g_DDSBack.Lock(NIL, g_DDSDesc, DDLOCK_SURFACEMEMORYPTR or DDLOCK_WAIT, 0);

  if (DDResult = DDERR_SURFACELOST) then
    begin
      RestoreSurfaces;
      DDResult := g_DDSBack.Lock(NIL, g_DDSDesc, DDLOCK_SURFACEMEMORYPTR or DDLOCK_WAIT, 0);
    end;

  if FAILED(DDResult) then
    begin
      Write2Log('Error while lock surface : ', false);
      Describe_Last_Error(true);
      DisplayMessage(Get_DDraw_Error(DDResult) + ' while Lock Surface', DDRAW_ERROR_TITLE, MSG_ERROR);
      Exit;
    end;

  VirtBuffer := g_DDSDesc.lpSurface;
end;

procedure SurfaceUnlock;
begin
  DDResult := g_DDSBack.Unlock(VirtBuffer);

  if FAILED(DDResult) then
    begin
      Write2Log('Error while  unlock surface : ', false);
      Describe_Last_Error(true);
      DisplayMessage(Get_DDraw_Error(DDResult) + ' while Unlock Surface', DDRAW_ERROR_TITLE, MSG_ERROR);
      Exit;
    end;
end;

procedure FlipSurfaces;
begin
  DDResult := g_DDSPrimary.Flip(NIL, DDFLIP_WAIT);

  if (DDResult = DDERR_SURFACELOST) then
    begin
      RestoreSurfaces;
      DDResult := g_DDSPrimary.Flip(NIL, DDFLIP_WAIT);
    end;

  if FAILED(DDResult) then
    begin
      Write2Log('Error while flip surfaces : ', false);
      Describe_Last_Error(true);
      DisplayMessage(Get_DDraw_Error(DDResult) + ' while Flip', DDRAW_ERROR_TITLE, MSG_ERROR);
      Exit;
    end;
end;

Procedure BlitSurface;
begin
  DDResult := g_DDSPrimary.BltFast(0, 0, g_DDSOffScr, NIL, DDBLTFAST_WAIT);

  if (DDResult = DDERR_SURFACELOST) then
    begin
      RestoreSurfaces;
      DDResult := g_DDSPrimary.BltFast(0, 0, g_DDSOffScr, NIL, DDBLTFAST_WAIT);
    end;

  if FAILED(DDResult) then
    begin
      Write2Log('Error while BltFast : ', false);
      Describe_Last_Error(true);
      DisplayMessage(Get_DDraw_Error(DDResult) + ' while BltFast', DDRAW_ERROR_TITLE, MSG_ERROR);
      Exit;
    end;
end;

Procedure GetDCHandle;
begin
  DDResult := g_DDSBack.GetDC(g_DC);

  if (DDResult = DDERR_SURFACELOST) then
    begin
      RestoreSurfaces;
      DDResult := g_DDSBack.GetDC(g_DC);
    end;

  if FAILED(DDResult) then
    begin
      Write2Log('Error while GetDC : ', false);
      Describe_Last_Error(true);
      DisplayMessage(Get_DDraw_Error(DDResult) + ' while GetDC', DDRAW_ERROR_TITLE, MSG_ERROR);
      Exit;
    end;
end;

Procedure ReleaseDCHandle;
begin
  DDResult := g_DDSBack.ReleaseDC(g_DC);

  if FAILED(DDResult) then
    begin
      Write2Log('Error while ReleaseDC : ', false);
      Describe_Last_Error(true);
      DisplayMessage(Get_DDraw_Error(DDResult) + ' while ReleaseDC', DDRAW_ERROR_TITLE, MSG_ERROR);
      Exit;
    end;
end;

Procedure WriteTxt32bpp(Text:String; x, y:Integer; R, G, B:Byte);
begin
  SetBkMode (g_DC, TRANSPARENT);
  SetTextColor(g_DC, RGB(R, G, B));
  TextOut(g_DC, x, y, @Text[1], Length(Text));
end;

Procedure WriteNo32bpp(Number:Single; c, d, x, y:Integer; R, G, B:Byte);
var
    strNum: String;
begin
  str(Number:c:d, strNum);
  SetBkMode (g_DC, TRANSPARENT);
  SetTextColor(g_DC, RGB(R, G, B));
  TextOut(g_DC, x, y, @strNum[1], Length(strNum));
end;

Function Get_DDraw_Error (Error_Code: HResult): String;
begin
  case (Error_Code) of
     DD_OK:
     begin
        Result := 'DD_OK';
        Exit;
     end;
     DDERR_ALREADYINITIALIZED:
     begin
        Result := 'DDERR_ALREADYINITIALIZED';
        Exit;
     end;
     DDERR_BLTFASTCANTCLIP:
     begin
        Result := 'DDERR_BLTFASTCANTCLIP';
        Exit;
     end;
     DDERR_CANNOTATTACHSURFACE:
     begin
        Result := 'DDERR_CANNOTATTACHSURFACE';
        Exit;
     end;
     DDERR_CANNOTDETACHSURFACE:
     begin
        Result := 'DDERR_CANNOTDETACHSURFACE';
        Exit;
     end;
     DDERR_CANTCREATEDC:
     begin
        Result := 'DDERR_CANTCREATEDC';
        Exit;
     end;
     DDERR_CANTDUPLICATE:
     begin
        Result := 'DDERR_CANTDUPLICATE';
        Exit;
     end;
     DDERR_CLIPPERISUSINGHWND:
     begin
        Result := 'DDERR_CLIPPERUSINGHWND';
        Exit;
     end;
     DDERR_COLORKEYNOTSET:
     begin
        Result := 'DDERR_COLORKEYNOTSET';
        Exit;
     end;
     DDERR_CURRENTLYNOTAVAIL:
     begin
        Result := 'DDERR_CURRENTLYNOTAVAIL';
        Exit;
     end;
     DDERR_DIRECTDRAWALREADYCREATED:
     begin
        Result := 'DDERR_DIRECTDRAWALREADYCREATED';
        Exit;
     end;
     DDERR_EXCEPTION:
     begin
        Result := 'DDERR_EXCEPTION';
        Exit;
     end;
     DDERR_EXCLUSIVEMODEALREADYSET:
     begin
        Result := 'DDERR_EXCLUSIVEMODEALREADYSET';
        Exit;
     end;
     DDERR_GENERIC:
     begin
        Result := 'DDERR_GENERIC';
        Exit;
     end;
     DDERR_HEIGHTALIGN:
     begin
        Result := 'DDERR_HEIGHTALIGN';
        Exit;
     end;
     DDERR_HWNDALREADYSET:
     begin
        Result := 'DDERR_HWNDALREADYSET';
        Exit;
     end;
     DDERR_HWNDSUBCLASSED:
     begin
        Result := 'DDERR_HWNDSUBCLASSED';
        Exit;
     end;
     DDERR_IMPLICITLYCREATED:
     begin
        Result := 'DDERR_IMPLICITLYCREATED';
        Exit;
     end;
     DDERR_INCOMPATIBLEPRIMARY:
     begin
        Result := 'DDERR_INCOMPATIBLEPRIMARY';
        Exit;
     end;
     DDERR_INVALIDCAPS:
     begin
        Result := 'DDERR_INVALIDCAPS';
        Exit;
     end;
     DDERR_INVALIDCLIPLIST:
     begin
        Result := 'DDERR_INVALIDCLIPLIST';
        Exit;
     end;
     DDERR_INVALIDDIRECTDRAWGUID:
     begin
        Result := 'DDERR_INVALIDDIRECTDRAWGUID';
        Exit;
     end;
     DDERR_INVALIDMODE:
     begin
        Result := 'DDERR_INVALIDMODE';
        Exit;
     end;
     DDERR_INVALIDOBJECT:
     begin
        Result := 'DDERR_INVALIDOBJECT';
        Exit;
     end;
     DDERR_INVALIDPARAMS:
     begin
        Result := 'DDERR_INVALIDPARAMS';
        Exit;
     end;
     DDERR_INVALIDPIXELFORMAT:
     begin
        Result := 'DDERR_INVALIDPIXELFORMAT';
        Exit;
     end;
     DDERR_INVALIDPOSITION:
     begin
        Result := 'DDERR_INVALIDPOSITION';
        Exit;
     end;
     DDERR_INVALIDRECT:
     begin
        Result := 'DDERR_INVALIDRECT';
        Exit;
     end;
     DDERR_LOCKEDSURFACES:
     begin
        Result := 'DDERR_LOCKEDSURFACES';
        Exit;
     end;
     DDERR_NO3D:
     begin
        Result := 'DDERR_NO3D';
        Exit;
     end;
     DDERR_NOALPHAHW:
     begin
        Result := 'DDERR_NOALPHAHW';
        Exit;
     end;
     DDERR_NOBLTHW:
     begin
        Result := 'DDERR_NOBLTHW';
        Exit;
     end;
     DDERR_NOCLIPLIST:
     begin
        Result := 'DDERR_NOCLIPLIST';
        Exit;
     end;
     DDERR_NOCLIPPERATTACHED:
     begin
        Result := 'DDERR_NOCLIPPERATTACHED';
        Exit;
     end;
     DDERR_NOCOLORCONVHW:
     begin
        Result := 'DDERR_NOCOLORCONVHW';
        Exit;
     end;
     DDERR_NOCOLORKEY:
     begin
        Result := 'DDERR_NOCOLORKEY';
        Exit;
     end;
     DDERR_NOCOLORKEYHW:
     begin
        Result := 'DDERR_NOCOLORKEYHW';
        Exit;
     end;
     DDERR_NOCOOPERATIVELEVELSET:
     begin
        Result := 'DDERR_NOCOOPERATIVELEVELSET';
        Exit;
     end;
     DDERR_NODC:
     begin
        Result := 'DDERR_NODC';
        Exit;
     end;
     DDERR_NODDROPSHW:
     begin
        Result := 'DDERR_NODDROPSHW';
        Exit;
     end;
     DDERR_NODIRECTDRAWHW:
     begin
        Result := 'DDERR_NODIRECTDRAWHW';
        Exit;
     end;
     DDERR_NOEMULATION:
     begin
        Result := 'DDERR_NOEMULATION';
        Exit;
     end;
     DDERR_NOEXCLUSIVEMODE:
     begin
        Result := 'DDERR_NOEXCLUSIVEMODE';
        Exit;
     end;
     DDERR_NOFLIPHW:
     begin
        Result := 'DDERR_NOFLIPHW';
        Exit;
     end;
     DDERR_NOGDI:
     begin
        Result := 'DDERR_NOGDI';
        Exit;
     end;
     DDERR_NOHWND:
     begin
        Result := 'DDERR_NOHWND';
        Exit;
     end;
     DDERR_NOMIRRORHW:
     begin
        Result := 'DDERR_NOMIRRORHW';
        Exit;
     end;
     DDERR_NOOVERLAYDEST:
     begin
        Result := 'DDERR_NOOVERLAYDEST';
        Exit;
     end;
     DDERR_NOOVERLAYHW:
     begin
        Result := 'DDERR_NOOVERLAYHW';
        Exit;
     end;
     DDERR_NOPALETTEATTACHED:
     begin
        Result := 'DDERR_NOPALETTEATTACHED';
        Exit;
     end;
     DDERR_NOPALETTEHW:
     begin
        Result := 'DDERR_NOPALETTEHW';
        Exit;
     end;
     DDERR_NORASTEROPHW:
     begin
        Result := 'DDERR_NORASTEROPHW';
        Exit;
     end;
     DDERR_NOROTATIONHW:
     begin
        Result := 'DDERR_NOROTATIONHW';
        Exit;
     end;
     DDERR_NOSTRETCHHW:
     begin
        Result := 'DDERR_NOSTRETCHHW';
        Exit;
     end;
     DDERR_NOTFLIPPABLE:
     begin
        Result := 'DDERR_NOTFLIPPABLE';
        Exit;
     end;
     DDERR_NOTFOUND:
     begin
        Result := 'DDERR_NOTFOUND';
        Exit;
     end;
     DDERR_NOTLOCKED:
     begin
        Result := 'DDERR_NOTLOCKED';
        Exit;
     end;
     DDERR_NOVSYNCHW:
     begin
        Result := 'DDERR_NOVSYNCHW';
        Exit;
     end;
     DDERR_NOZBUFFERHW:
     begin
        Result := 'DDERR_NOZBUFFERHW';
        Exit;
     end;
     DDERR_OUTOFCAPS:
     begin
        Result := 'DDERR_OUTOFCAPS';
        Exit;
     end;
     DDERR_OUTOFMEMORY:
     begin
        Result := 'DDERR_OUTOFMEMORY';
        Exit;
     end;
     DDERR_OUTOFVIDEOMEMORY:
     begin
        Result := 'DDERR_OUTOFVIDEOMEMORY';
        Exit;
     end;
     DDERR_PRIMARYSURFACEALREADYEXISTS:
     begin
        Result := 'DDERR_PRIMARYSURFACEALREADYEXISTS';
        Exit;
     end;
     DDERR_REGIONTOOSMALL:
     begin
        Result := 'DDERR_REGIONTOOSMALL';
        Exit;
     end;
     DDERR_SURFACEALREADYATTACHED:
     begin
        Result := 'DDERR_SURFACEALREADYATTACHED';
        Exit;
     end;
     DDERR_SURFACEALREADYDEPENDENT:
     begin
        Result := 'DDERR_SURFACEALREADYDEPENDENT';
        Exit;
     end;
     DDERR_SURFACEBUSY:
     begin
        Result := 'DDERR_SURFACEBUSY';
        Exit;
     end;
     DDERR_SURFACEISOBSCURED:
     begin
        Result := 'DDERR_SURFACEISOBSCURED';
        Exit;
     end;
     DDERR_SURFACELOST:
     begin
        Result := 'DDERR_SURFACELOST';
        Exit;
     end;
     DDERR_SURFACENOTATTACHED:
     begin
        Result := 'DDERR_SURFACENOTATTACHED';
        Exit;
     end;
     DDERR_TOOBIGHEIGHT:
     begin
        Result := 'DDERR_TOOBIGHEIGHT';
        Exit;
     end;
     DDERR_TOOBIGSIZE:
     begin
        Result := 'DDERR_TOOBIGSIZE';
        Exit;
     end;
     DDERR_TOOBIGWIDTH:
     begin
        Result := 'DDERR_TOOBIGWIDTH';
        Exit;
     end;
     DDERR_UNSUPPORTED:
     begin
        Result := 'DDERR_UNSUPPORTED';
        Exit;
     end;
     DDERR_UNSUPPORTEDFORMAT:
     begin
        Result := 'DDERR_UNSUPPORTEDFORMAT';
        Exit;
     end;
     DDERR_UNSUPPORTEDMASK:
     begin
        Result := 'DDERR_UNSUPPORTEDMASK';
        Exit;
     end;
     DDERR_VERTICALBLANKINPROGRESS:
     begin
        Result := 'DDERR_VERTICALBLANKINPROGRESS';
        Exit;
     end;
     DDERR_WASSTILLDRAWING:
     begin
        Result := 'DDERR_WASSTILLDRAWING';
        Exit;
     end;
     DDERR_WRONGMODE:
     begin
        Result := 'DDERR_WRONGMODE';
        Exit;
     end;
     DDERR_XALIGN:
     begin
        Result := 'DDERR_XALIGN';
        Exit;
     end;
     else
       begin
         Result := 'UNKNOWN ERROR';
         Exit;
       end;
  end;
end;

END.