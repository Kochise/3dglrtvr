// ---------------------------------------------------------------------------
// Pulsar LMTools v1.1
// Copyright (C) 2002, 2003 Lord Trancos. All rights reserved.
// ---------------------------------------------------------------------------
// 
// This file is part of Pulsar LMTools
// 
// Pulsar LMTools is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either
// version 2 of the License, or any later version.
// 
// Pulsar LMTools is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details (license.txt).
// 
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
// 
// You should also be able to get a copy of the license here:
//   
//   http://www.gnu.org/licenses/gpl.txt
// 
// ---------------------------------------------------------------------------
//
// Developed by Lord Trancos <lordtrancos@softhome.net> 
//
// ---------------------------------------------------------------------------
//
// DXLab - Visit us at: http://www.dxlab.tk or http://dxlab.host.sk
//
// ---------------------------------------------------------------------------

unit xNaviU1;

// ------------------------------------------------------------------

interface

uses
  Windows, Messages, SysUtils, Classes, Graphics, Controls, Forms,
  Dialogs, Menus, StdCtrls, ComCtrls, DirectXGraphics, D3DX8,
  DX8GFX, DX8XMESH, DX8TEXT, DX8DI, Common, xcomdlg, timing,
  ExtCtrls, dx8lmts, lmts, xexport, filez_ex, ImgList;

type
  TForm1 = class(TForm)
    MainMenu1: TMainMenu;
    Menu1: TMenuItem;
    Menu3: TMenuItem;
    N1: TMenuItem;
    Menu2: TMenuItem;
    StatusBar1: TStatusBar;
    Menu4: TMenuItem;
    Menu5: TMenuItem;
    Menu6: TMenuItem;
    Menu7: TMenuItem;
    Menu8: TMenuItem;
    Menu9: TMenuItem;
    Menu10: TMenuItem;
    Menu11: TMenuItem;
    N2: TMenuItem;
    Menu12: TMenuItem;
    N3: TMenuItem;
    Menu13: TMenuItem;
    N4: TMenuItem;
    Menu14: TMenuItem;
    N5: TMenuItem;
    Menu15: TMenuItem;
    Menu16: TMenuItem;
    Panel1: TPanel;
    Menu30: TMenuItem;
    Menu31: TMenuItem;
    Menu32: TMenuItem;
    Menu33: TMenuItem;
    Menu34: TMenuItem;
    Menu17: TMenuItem;
    Menu40: TMenuItem;
    Panel2: TPanel;
    imgFov: TImage;
    imgPlanes: TImage;
    imgMisc: TImageList;
    N6: TMenuItem;
    Menu35: TMenuItem;
    imgFog: TImage;
    procedure hHalt(_errMsg: string);
    procedure UpdateStatusBar;
    procedure SetMatrices;
    procedure SetFog;
    procedure GetUserInput;
    procedure Animate(_time: longint; _z: single);
    procedure RenderFrame;
    procedure AppOnIdle(Sender: TObject; var Done: Boolean);
    procedure AppOnActivate(Sender: TObject);
    procedure AppOnDeactivate(Sender: TObject);
    procedure OpenDialog;
    procedure LoadXMesh(_fn: string);
    procedure FormActivate(Sender: TObject);
    procedure FormDestroy(Sender: TObject);
    procedure FormKeyUp(Sender: TObject; var Key: Word;
                        Shift: TShiftState);
    procedure Menu3Click(Sender: TObject);
    procedure Menu2Click(Sender: TObject);
    procedure MenuFillMode(Sender: TObject);
    procedure MenuShadeMode(Sender: TObject);
    procedure Menu12Click(Sender: TObject);
    procedure Menu13Click(Sender: TObject);
    procedure Menu14Click(Sender: TObject);
    procedure Menu15Click(Sender: TObject);
    procedure FormKeyPress(Sender: TObject; var Key: Char);
    procedure Menu16Click(Sender: TObject);
    procedure MenuRenderMode(Sender: TObject);
    procedure Menu17Click(Sender: TObject);
    procedure FormCreate(Sender: TObject);
    procedure Menu40Click(Sender: TObject);
    procedure imgCamMouseDown(Sender: TObject; Button: TMouseButton;
                              Shift: TShiftState; X, Y: Integer);
    procedure imgCamMouseUp(Sender: TObject; Button: TMouseButton;
                            Shift: TShiftState; X, Y: Integer);
    procedure imgCamMouseMove(Sender: TObject; Shift: TShiftState; X,
                              Y: Integer);
    procedure Menu35Click(Sender: TObject);
  private
    procedure FreeMeshes;
    procedure LoadLMTSMesh(_fn: string);
    { Private declarations }
  public
    { Public declarations }
  end;

var
  Form1: TForm1;

  g_PrevCursorPos : TPoint;
  g_PrevYPos : longint;

// ------------------------------------------------------------------

implementation

{$R *.DFM}

uses xNaviU2, xNaviU3, xNaviU4;

// ------------------------------------------------------------------

procedure FPSCheck_Start;

begin
  G_FPS_TIME_FLAG := timGetTickCount;
end;

// ------------------------------------------------------------------

procedure FPSCheck_End(_hWnd: hWnd);

begin
  while timGetTickCount - G_FPS_TIME_FLAG = 0 do Sleep(0);
  G_FPS_FRAMETIME := timGetTickCount - G_FPS_TIME_FLAG;
  G_FPS_FRMTIM_L := timElapsedTime(G_FPS_TIME_FLAG, timGetTickCount);
  inc(G_FPS_FRAMECOUNT);
  inc(G_FPS_FRAMETIMES, G_FPS_FRAMETIME);
  if (G_FPS_FRAMETIMES > G_TIM_FREQ) then
  begin
    G_FPS_LASTFPSVAL := (G_FPS_FRAMECOUNT * G_TIM_FREQ) / G_FPS_FRAMETIMES;
    if not C_DISPLAY_FULLSCREEN
    then Form1.StatusBar1.Panels[0].Text := Format('%.2f fps', [g_FPS_LASTFPSVAL]);
    G_FPS_FRAMECOUNT := 0;
    G_FPS_FRAMETIMES := 0;
//    With g_CPosition do Form1.caption := Format('%.2f  %.2f  %.2f', [x, y, z]);
  end;
end;

// ------------------------------------------------------------------

procedure MouseControl(_enabled: boolean);

begin
  g_CEnabled := _enabled;
  DIMouseControl(g_CEnabled);
end;

// -------------------------------------------------------------------

procedure ResetCameraPos;

begin
  g_XRot := Pi/2;
  g_YRot := 0;
  g_CTarget   := D3DXVector3(0.0, 0.0, 1.0);
  g_CPosition := D3DXVector3(0.0, 0.0, 0.0);
end;

// -------------------------------------------------------------------

function LoadText(_fn: pChar; _tag: longint;
                  var _t: IDirect3DTexture8): boolean; stdcall;


begin
  case _tag of
    // normal texture - load and create mipmaps
    0: Result := not failed(D3DXCreateTextureFromFileEx(D3DDEV8, _fn, 0, 0, 0, 0,
                            D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_FILTER_TRIANGLE,
                            D3DX_FILTER_TRIANGLE, 0, NIL, NIL, _t));
    // lightmap texture! - load without alpha channel and without mipmaps
    1: Result := not failed(D3DXCreateTextureFromFileEx(D3DDEV8, _fn, 0, 0, 1, 0,
                            D3DFMT_R5G6B5, D3DPOOL_MANAGED, D3DX_FILTER_TRIANGLE,
                            D3DX_FILTER_TRIANGLE, 0, NIL, NIL, _t));
    else Result := false;
  end;
  if not Result then
  begin
    if MessageBox(Form1.Handle, pChar(Format(WRN02, [_fn])), WRNXX,
                  MB_OKCANCEL or MB_ICONWARNING) = IDOK then Result := true;
  end;
end;

// -------------------------------------------------------------------

procedure TForm1.UpdateStatusBar;

begin
  StatusBar1.Panels[1].Text := ExtractFileName(g_MeshFileName);
  StatusBar1.Panels[2].Text := Format(NFO01, [g_CSpeed]);
end;

// -------------------------------------------------------------------

procedure TForm1.SetMatrices;

var _v : TD3DXVector3;
    _matWorld : TD3DXMatrix; // World
    _matView  : TD3DXMatrix; // View
    _matProj  : TD3DXMatrix; // Projection

begin
  // World.
  D3DXMatrixIdentity(_matWorld);
  D3DDEV8.SetTransform(D3DTS_WORLD, _matWorld);

  // View.
  _v := D3DXVector3(  0.0,  1.0,   0.0);
  D3DXMatrixLookAtLH(_matView, g_CPOSITION, g_CTARGET, _v);
  D3DDEV8.SetTransform(D3DTS_VIEW, _matView);

  // Projection.
  D3DXMatrixPerspectiveFovLH(_matProj, g_CAM_FOV, ClientWidth/ClientHeight,
                             g_CAM_NEAR, g_CAM_FAR);
  D3DDEV8.SetTransform(D3DTS_PROJECTION, _matProj);
end;

// -------------------------------------------------------------------

procedure TForm1.SetFog;

begin
  if g_FOG_Density > C_FOG_MIN_DENSITY then
  begin
    D3DDEV8.SetRenderState(D3DRS_FOGENABLE, 1);
    D3DDEV8.SetRenderState(D3DRS_FOGCOLOR, g_BackgrndColor);
    D3DDEV8.SetRenderState(D3DRS_FOGTABLEMODE, D3DFOG_EXP);
    D3DDEV8.SetRenderState(D3DRS_FOGDENSITY, PDWord(@g_FOG_Density)^);
  end else D3DDEV8.SetRenderState(D3DRS_FOGENABLE, 0);
end;

// -------------------------------------------------------------------

procedure TForm1.GetUserInput;

var _x, _y, _z : longint;

begin
  if not g_CEnabled then exit;

  if DIGetMouseState(_x, _y, _z, _z, _z, _z, _z, _z) then
    begin
      g_XRot := g_XRot - _x / 200;
      g_YRot := g_YRot - _y / 200;

      if g_YRot > C_MAXYROT then g_YRot := C_MAXYROT;
      if g_YRot < C_MINYROT then g_YRot := C_MINYROT;
      if g_XRot < -(2*Pi) then g_XRot := g_XRot+(2*Pi);
      if g_XRot > +(2*Pi) then g_XRot := g_XRot-(2*Pi);

      g_CTarget.x := g_CPosition.x + (cos(g_XRot) * cos(g_YRot));
      g_CTarget.z := g_CPosition.z + (sin(g_XRot) * cos(g_YRot));
      g_CTarget.y := g_CPosition.y + (sin(g_YRot));

      SetMatrices;
    end else if g_CEnabled then DIMouseControl(true);
end;

// -------------------------------------------------------------------

procedure TForm1.Animate(_time: longint; _z: single);

var _v : TD3DVector;
    _m : single;

begin
  if not g_CEnabled then exit;

  _m := (_time * g_CSpeed) * _z;

  _v.x := (cos(g_XRot) * cos(g_YRot)) * _m;
  _v.z := (sin(g_XRot) * cos(g_YRot)) * _m;
  _v.y := (sin(g_YRot)) * _m;

  g_CTarget.x := g_CTarget.x + _v.x;
  g_CTarget.y := g_CTarget.y + _v.y;
  g_CTarget.z := g_CTarget.z + _v.z;

  g_CPosition.x := g_CPosition.x + _v.x;
  g_CPosition.y := g_CPosition.y + _v.y;
  g_CPosition.z := g_CPosition.z + _v.z;
end;

// -------------------------------------------------------------------

procedure TForm1.RenderFrame;

var _f : cardinal;

begin
  // Clear the back buffer to a blue color.
  _f := D3DCLEAR_ZBUFFER;
  if Menu16.Checked then _f := _f or D3DCLEAR_TARGET;
  D3DDEV8.Clear(0, NIL, _f, g_BackgrndColor, 1.0, 0);

  // Begin the scene.
  D3DDEV8.BeginScene;

  // Render mesh
  if g_Mesh0.Mesh <> NIL
    then xRenderMesh(g_Mesh0) else
      if g_Mesh1.ok then dxlmts_Render(g_Mesh1, g_RenderMode);

  // End the scene.
  D3DDEV8.EndScene;

  // Display scene.
  D3DDEV8.Present(NIL, NIL, 0, NIL);
end;

// -------------------------------------------------------------------

procedure TForm1.hHalt(_errMsg: string);

begin
  MessageBox(Handle, pChar(_errMsg), ERRXX, MB_ICONERROR);
  Close;
end;

// -------------------------------------------------------------------

procedure TForm1.AppOnIdle(Sender: TObject; var Done: Boolean);

begin
  Done := false;
  if ((not g_DXReady) or (not g_WindowReady) or (not Menu40.Checked)) then exit;
  FPSCheck_Start;
  RenderFrame;
  GetUserInput;
  if DIMou0Clicked then Animate(g_FPS_FRMTIM_L, +C_SPEED_SF);
  if DIMou1Clicked then Animate(g_FPS_FRMTIM_L, -C_SPEED_SF);
  FPSCheck_End(Handle);
end;

// ------------------------------------------------------------------

procedure TForm1.AppOnActivate(Sender: TObject);

begin
  g_WindowReady := true;
end;

// ------------------------------------------------------------------

procedure TForm1.AppOnDeactivate(Sender: TObject);

begin
  g_WindowReady := false;
end;

// ------------------------------------------------------------------

procedure TForm1.OpenDialog;

var
  _fn, _ext : string;

begin
  // Show dialog.
  _fn := Trim(dlgBrowseOpen(Handle, NIL, pChar(g_OpenDlgPath),
                            C_FLT_FILTER, NIL, 0));
  if _fn = '' then exit;

  // Get file extension and current path
  g_OpenDlgPath := ExtractFilePath(_fn);
  _ext := UpperCase(ExtractFileExt(_fn));

  // Open file.
  StatusBar1.Panels[0].Text := NFO02;
  Application.ProcessMessages;
  if (_ext = '.X') then LoadXMesh(_fn) else LoadLMTSMesh(_fn);
  StatusBar1.Panels[0].Text := '';
  UpdateStatusBar;
end;

// ------------------------------------------------------------------

procedure TForm1.FreeMeshes;

begin
  Menu17.Enabled := false;
  xFreeMesh(g_Mesh0, true);
  dxlmts_Free(g_Mesh1, true);
end;

// ------------------------------------------------------------------

procedure TForm1.LoadXMesh(_fn: string);

begin
  // free meshes
  FreeMeshes;

  // reset camera
  ResetCameraPos;
  SetMatrices;

  // load
  g_MeshFileName := '';
  g_Mesh0 := xLoadMesh(_fn, 0);
  if g_Mesh0.Mesh = NIL
    then MessageBox(Handle, WRN01, WRNXX, MB_ICONWARNING) else
    begin
      Menu17.Enabled := true;
      g_MeshFileName := _fn;
    end;
end;

// ------------------------------------------------------------------

procedure TForm1.LoadLMTSMesh(_fn: string);

begin
  // free meshes
  FreeMeshes;

  // reset camera
  ResetCameraPos;
  SetMatrices;

  // load mesh
  g_MeshFileName := '';
  g_Mesh1 := dxlmts_Load(_fn, 0, 1);
  if not g_Mesh1.ok
    then MessageBox(Handle, WRN01, WRNXX, MB_ICONWARNING)
      else g_MeshFileName := _fn;
end;

// ------------------------------------------------------------------

procedure TForm1.Menu2Click(Sender: TObject);

// Open

begin
  OpenDialog;
end;

// ------------------------------------------------------------------

procedure TForm1.Menu3Click(Sender: TObject);

// Exit

begin
  Close;
end;

// ------------------------------------------------------------------

procedure TForm1.MenuFillMode(Sender: TObject);

// Fill mode

var _value : cardinal;

begin
  TMenuItem(Sender).Checked := true;
  _value := D3DFILL_SOLID;
  if Menu6.Checked then _value := D3DFILL_POINT;
  if Menu7.Checked then _value := D3DFILL_WIREFRAME;
  D3DDEV8.SetRenderState(D3DRS_FILLMODE, _value);
end;

// ------------------------------------------------------------------

procedure TForm1.MenuShadeMode(Sender: TObject);

// Shade mode

var _value : cardinal;

begin
  TMenuItem(Sender).Checked := true;
  _value := D3DSHADE_GOURAUD;
  if Menu10.Checked then _value := D3DSHADE_FLAT;
  D3DDEV8.SetRenderState(D3DRS_SHADEMODE, _value);
end;

// ------------------------------------------------------------------

procedure TForm1.MenuRenderMode(Sender: TObject);

// Render mode

begin
  TMenuItem(Sender).Checked := true;
  g_RenderMode := rm_Multitexture;
  if Menu32.Checked then g_RenderMode := rm_TwoPass;
  if Menu33.Checked then g_RenderMode := rm_OnlyLightmaps;
  if Menu34.Checked then g_RenderMode := rm_NoLightmaps;
end;

// ------------------------------------------------------------------


procedure TForm1.Menu12Click(Sender: TObject);

// Filter textures

var _value : cardinal;

begin
  Menu12.Checked := not Menu12.Checked;
  if Menu12.Checked
    then _value := D3DTEXF_LINEAR
      else _value := D3DTEXF_NONE;
  D3DDEV8.SetTextureStageState(0, D3DTSS_MAGFILTER, _value);
  D3DDEV8.SetTextureStageState(0, D3DTSS_MINFILTER, _value);
  D3DDEV8.SetTextureStageState(0, D3DTSS_MIPFILTER, _value);
  D3DDEV8.SetTextureStageState(1, D3DTSS_MAGFILTER, _value);
  D3DDEV8.SetTextureStageState(1, D3DTSS_MINFILTER, _value);
  D3DDEV8.SetTextureStageState(1, D3DTSS_MIPFILTER, _value);
end;

// ------------------------------------------------------------------

procedure TForm1.Menu35Click(Sender: TObject);

// Choose background-fog color

var _c : cardinal;

begin
  _c := SwapRedAndBlue(g_BackgrndColor);
  if dlgChooseColor(Handle, _c, @g_CustomColors) then
  begin
    g_BackgrndColor := SwapRedAndBlue(_c);
    SetFog;
  end;
end;

// ------------------------------------------------------------------

procedure TForm1.Menu13Click(Sender: TObject);

// Camera enabled

begin
  Menu13.Checked := not Menu13.Checked;
  MouseControl(Menu13.Checked);
end;

// ------------------------------------------------------------------

procedure TForm1.Menu14Click(Sender: TObject);

// File information

var
  _c, _subsets, _faces, _vertices : longint;
  _ti : TListItem;
  _dlg : TForm4;
  _tex : PLMTS_TexData;
  _sub : PLMTS_Subset;
  _s : string;

begin
  if g_Mesh0.Mesh <> NIL then
  begin
    _subsets := g_Mesh0.MatCount;
    _faces := g_Mesh0.Mesh.GetNumFaces;
    _vertices := g_Mesh0.Mesh.GetNumVertices;
    ShowMessage(Format(NFO00, [_faces, _vertices, _subsets]));
  end;
  if g_Mesh1.ok then
  begin
    _subsets := g_Mesh1.header.nSubsets;
    _faces := g_Mesh1.header.nTris;
    _vertices := _faces * 3;
    // create dialog
    _dlg := TForm4.Create(Application);
    // fill dialog
    _dlg.Memo1.Text := Format(NFO00, [_faces, _vertices, _subsets]);
    _tex := g_Mesh1.texData;
    if (g_Mesh1.header.nTexts > 0) then
      for _c := 0 to g_Mesh1.header.nTexts-1 do
      begin
        _ti := _dlg.ListView1.Items.Add;
        _ti.Caption := IntToStr(_c);
        _ti.SubItems.Add(StrPas(@_tex^.FName[0]));
        if _tex^.Flags and 1 = 1 then _ti.SubItems.Add('x');
        inc(_tex);
      end;
    _sub := g_Mesh1.subsets;
    if (g_Mesh1.header.nSubsets > 0) then
      for _c := 0 to g_Mesh1.header.nSubsets-1 do
      begin
        _ti := _dlg.ListView2.Items.Add;
        _ti.Caption := IntToStr(_c);
        _ti.SubItems.Add(IntToStr(_sub^.Offset));
        _ti.SubItems.Add(IntToStr(_sub^.Count));
        if _sub^.TextID1 = C_NO_TEXTURE
          then _s := '(none)' else _s := IntToStr(_sub^.TextID1);
        _ti.SubItems.Add(_s);
        if _sub^.TextID2 = C_NO_TEXTURE
          then _s := '(none)' else _s := IntToStr(_sub^.TextID2);
        _ti.SubItems.Add(_s);
        inc(_sub);
      end;
    // show dialog
    _dlg.ShowModal;
    // free dialog
    _dlg.Free;
  end;
end;

// ------------------------------------------------------------------

procedure TForm1.Menu15Click(Sender: TObject);

// About

begin
  ShowMessage(ABOUT);
end;

// ------------------------------------------------------------------

procedure TForm1.Menu16Click(Sender: TObject);

// Erase backgroud.

begin
  Menu16.Checked := not Menu16.Checked;
end;

// ------------------------------------------------------------------

procedure TForm1.Menu40Click(Sender: TObject);

// Enable/Disable display

begin
  Menu40.Checked := not Menu40.Checked;
  StatusBar1.Panels[0].Text := '';
  Panel1.Repaint;
end;

// ------------------------------------------------------------------

procedure TForm1.Menu17Click(Sender: TObject);

// Export to LMTS.

var
  _dlg : TForm2;
  _save : boolean;
  _tgas : boolean;
  _path : string;
  _fname : string;

begin
  if (g_Mesh0.Mesh = NIL) then exit;

  // check if source mesh FVF is supported
  if (g_Mesh0.FVF <> C_VALID_FVF1) and
     (g_Mesh0.FVF <> C_VALID_FVF2) then
  begin
    MessageBox(Handle, ERR05, ERRXX, MB_ICONERROR);
    exit;
  end;

  // create, show and free dialog
  _dlg := TForm2.Create(Application);
  _dlg.Edit1.Text := ExtractFilePath(g_MeshFileName);
  _dlg.ShowModal;
  _save := (_dlg.ModalResult = mrOk);
  _tgas := _dlg.CheckBox1.Checked; // save textures as TGA files
  g_Exp_PreserveTFN := _dlg.CheckBox2.Checked; // preserve texture fnames
  _path := _dlg.Edit1.Text;
  rewritePath(_path);
  _dlg.Free;

  // export lmts
  if (_save) then
  begin
    StatusBar1.Panels[0].Text := NFO03;
    Application.ProcessMessages;
    // save lmts
    _fname := ExtractFileName(g_MeshFileName);
    ExportToLMTS(Handle, _path, _fname);
    // export textures
    if (_tgas) then ExportToTGAs(Handle, _path);
    // ----------
    StatusBar1.Panels[0].Text := '';
    // done
    MessageBox(Handle, NFO04, APPTITLE, MB_ICONINFORMATION);
  end;
end;

// ------------------------------------------------------------------

procedure TForm1.FormKeyUp(Sender: TObject; var Key: Word;
                           Shift: TShiftState);
begin
  if (g_CEnabled) and
     ((Key = VK_ESCAPE) or (Key = VK_SPACE) or (Key = VK_RETURN))
    then Menu13.Click;
end;

// ------------------------------------------------------------------

procedure TForm1.FormKeyPress(Sender: TObject; var Key: Char);

begin
  case Key of
    '+' : begin inc(g_CSpeed); UpdateStatusBar; end;
    '-' : begin
            if (g_CSpeed > 1) then dec(g_CSpeed);
            UpdateStatusBar;
          end;
  end;
end;

// ------------------------------------------------------------------

procedure TForm1.FormActivate(Sender: TObject);

var
  _filt : dWord;
  _b : boolean;
  _s : string;

begin
  if g_DXReady then exit;

  g_OpenDlgPath := ExtractFilePath(paramstr(0));
  UpdateStatusBar;
  xResetMesh(g_Mesh0);
  dxlmts_Reset(g_Mesh1);

  // Initialize application events
  with Application do
  begin
    Title := APPTITLE;
    OnIdle       := AppOnIdle;
    OnActivate   := AppOnActivate;
    OnDeactivate := AppOnDeactivate;
    OnRestore    := AppOnActivate;
    OnMinimize   := AppOnDeActivate;
  end;

  // Initialize DirectInput.
  if not DIInit then begin hHalt(ERR03); exit; end;
  if not DIInitMouse(Handle) then begin hHalt(ERR04); exit; end;

  // Init DGfx
  _s := ' (sw)';
  case g_DeviceType of
    0 : _b := InitDGfx(Panel1.Handle, C_DISPLAY_FULLSCREEN,
                       C_DISPLAY_WIDTH shr 2, C_DISPLAY_HEIGHT shr 2,
                       C_DISPLAY_BPP, C_DISPLAY_BBCOUNT, false, true, false);
    1 : _b := InitDGfx(Panel1.Handle, C_DISPLAY_FULLSCREEN,
                       C_DISPLAY_WIDTH, C_DISPLAY_HEIGHT,
                       C_DISPLAY_BPP, C_DISPLAY_BBCOUNT, true, true, false);
    2 : begin
          _b := InitDGfx(Panel1.Handle, C_DISPLAY_FULLSCREEN,
                         C_DISPLAY_WIDTH, C_DISPLAY_HEIGHT,
                         C_DISPLAY_BPP, C_DISPLAY_BBCOUNT, true, true, true);
          _s := ' (hw)';
        end;
    else _b := false;
  end;
  if (not _b) then begin hHalt(ERR00); exit; end;
  Caption := APPTITLE + ' - ' + StrPas(D3DADAP_ID.Description) + _s;

  // Initialize texture engine
  if not TE_Init(C_MAX_TEXTURES, LoadText) then
  begin hHalt(ERR01); exit; end;

  // Enable dithering
  D3DDEV8.SetRenderState(D3DRS_DITHERENABLE, 1);

  // Device supports multitexture
  Menu31.Enabled := (D3DDEVCAPS8.MaxSimultaneousTextures > 1);
  if Menu31.Enabled then MenuRenderMode(Menu31) else MenuRenderMode(Menu32);

  // Device supports bilinear filter ?
  _filt := (D3DPTFILTERCAPS_MAGFLINEAR or D3DPTFILTERCAPS_MINFLINEAR);
  Menu12.Enabled := (D3DDEVCAPS8.TextureFilterCaps and _filt = _filt);
  if Menu12.Enabled then Menu12.Click;

  // Turn off D3D lighting.
  D3DDEV8.SetRenderState(D3DRS_LIGHTING, 0);

  // Set camera pos.
  ResetCameraPos;

  // Set matrices.
  SetMatrices;

  // Set fog
  SetFog;

  // All ok
  g_DXReady := true;
end;

// ------------------------------------------------------------------

procedure TForm1.FormDestroy(Sender: TObject);

begin
  g_DXReady := false;
  // Free meshes
  FreeMeshes;
  // Close texture engine
  TE_Close;
  // Close DirectX Grapchis.
  CloseDGfx;
  // Close DirectInput
  DIClose;
end;

// ------------------------------------------------------------------

procedure TForm1.imgCamMouseDown(Sender: TObject; Button: TMouseButton;
                                 Shift: TShiftState; X, Y: Integer);

begin
  if (Button <> mbLeft) then exit;

  // change image
  imgMisc.GetBitmap(TImage(Sender).Tag+1, TImage(Sender).Picture.Bitmap);
  TImage(Sender).Refresh;
  // save cursor pos and hide it
  g_PrevCursorPos := TImage(Sender).ClientToScreen(Point(X, Y));
  ShowCursor(false);
  // save Y position
  g_PrevYPos := Y;
end;

procedure TForm1.imgCamMouseMove(Sender: TObject; Shift: TShiftState;
                                 X, Y: Integer);

var
  _p : TPoint;
  _z : longint;

begin
  if not (ssleft in Shift) then exit;

  // get screen pos
  _p := TImage(Sender).ClientToScreen(Point(X, Y));

  // get Y change
  _z := g_PrevYPos - _p.Y;
  g_PrevYPos := _p.Y;
  if _z = 0 then exit;

  // avoid screen boundaries
  if _p.Y = 0 then
  begin
    SetCursorPos(_p.X, Screen.Height - 1);
    g_PrevYPos := Screen.Height-1;
    exit;
  end else if _p.Y = Screen.Height-1 then
  begin
    SetCursorPos(_p.X, 0);
    g_PrevYPos := 0;
    exit;
  end;

  // adjust FOV changes
  if TImage(Sender) = imgFov then
  begin
    g_CAM_FOV := g_CAM_FOV + (C_CAM_FOV_SCALE * -_z);
    if g_CAM_FOV < C_CAM_FOV_MIN then g_CAM_FOV := C_CAM_FOV_MIN;
    if g_CAM_FOV > C_CAM_FOV_MAX then g_CAM_FOV := C_CAM_FOV_MAX;
    StatusBar1.Panels[2].Text := Format(NFO05, [g_CAM_FOV]);
    SetMatrices;
  end;

  // adjust Near/Far plane changes
  if TImage(Sender) = imgPlanes then
  begin
    g_CAM_FarV :=  g_CAM_FarV + (C_CAM_FAR_VSCALE * _z);
    if g_CAM_FarV < C_CAM_FAR_VMIN then g_CAM_FarV := C_CAM_FAR_VMIN;
    if g_CAM_FarV > C_CAM_FAR_VMAX then g_CAM_FarV := C_CAM_FAR_VMAX;
    g_CAM_Far  := g_CAM_FarV * g_CAM_FarV;
    g_CAM_Near := g_CAM_Far / C_CAM_NEAR_DIV;
    StatusBar1.Panels[2].Text := Format(NFO06, [g_CAM_Near, g_CAM_Far]);
    SetMatrices;
  end;

  // adjust fog
  if TImage(Sender) = imgFog then
  begin
    g_FOG_Density := g_FOG_Density + (C_FOG_SCALE  * _z);
    if g_FOG_Density < C_FOG_MIN_DENSITY then g_FOG_Density := C_FOG_MIN_DENSITY;
    if g_FOG_Density > C_FOG_MAX_DENSITY then g_FOG_Density := C_FOG_MAX_DENSITY;
    if g_FOG_Density < C_FOG_MIN_DENSITY
      then StatusBar1.Panels[2].Text := NFO07
        else StatusBar1.Panels[2].Text := Format(NFO08, [g_FOG_Density]);
    SetFog;
  end;

end;

procedure TForm1.imgCamMouseUp(Sender: TObject; Button: TMouseButton;
                               Shift: TShiftState; X, Y: Integer);
begin
  if (Button <> mbLeft) then exit;

  // restore image
  imgMisc.GetBitmap(TImage(Sender).Tag, TImage(Sender).Picture.Bitmap);
  TImage(Sender).Refresh;
  // restore cursor pos and show it
  SetCursorPos(g_PrevCursorPos.X, g_PrevCursorPos.Y);
  ShowCursor(true);
  // restore status bar
  UpdateStatusBar;
end;

// ------------------------------------------------------------------

procedure TForm1.FormCreate(Sender: TObject);

var
  _dlg : TForm3;
  _ok : boolean;

begin
  Fillchar(g_CustomColors, SizeOf(g_CustomColors), 0);

  // ask desired device type
  _dlg := TForm3.Create(Application);
  _dlg.ShowModal;
  g_DeviceType := _dlg.ComboBox1.ItemIndex;
  _ok := (_dlg.ModalResult = mrOk);
  _dlg.Free;

  // continue ?
  if (not _ok) then halt;

  // set tool bar pics
  imgFov.Tag := 0;
  imgMisc.GetBitmap(imgFov.Tag, imgFov.Picture.Bitmap);
  imgPlanes.Tag := 2;
  imgMisc.GetBitmap(imgPlanes.Tag, imgPlanes.Picture.Bitmap);
  imgFog.Tag := 4;
  imgMisc.GetBitmap(imgFog.Tag, imgFog.Picture.Bitmap);

  // set camera initial planes
  g_CAM_Far  := g_CAM_FarV * g_CAM_FarV;
  g_CAM_Near := g_CAM_Far / C_CAM_NEAR_DIV;
end;

// ------------------------------------------------------------------

end.
