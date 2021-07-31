(*

  DESC:

   XCOMDLG - Dialogs unit.
   Copyright © 2002 Lord Trancos / Pulsar Studio.
   <lordtrancos@softhome.net>

  API:

   Win32

*)

unit xcomdlg;

////////////////////////////////////////////////////////////////////////////////

interface

uses Windows, Commdlg, CommCtrl, SysUtils;

type
  TBrowseFile = (bfOpen, bfSave);

  PCustomColors = ^TCustomColors;
  TCustomColors = array[0..15] of TColorRef;

procedure dlgMsg(_s: string);
procedure dlgMsgEx(_c, _s: string);
procedure dlgError(_s: string);
function  dlgBrowseFile(_hwnd: HWnd; _dlgTitle, _initialDir, _filter, _defExt: pChar;
                        _filterIndex: longint; _dlgType: TBrowseFile): string;
function  dlgBrowseOpen(_hwnd: HWnd; _dlgTitle, _initialDir, _filter, _defExt: pChar;
                        _filterIndex: longint): string;
function  dlgBrowseSave(_hwnd: HWnd; _dlgTitle, _initialDir, _filter, _defExt: pChar;
                        _filterIndex: longint): string;
function  dlgAreYouSure(_hwnd: HWnd; _c, _s: string): boolean;
function  dlgChooseColor(_hwnd: HWnd; var _color: cardinal;
                         _customColors: PCustomColors): boolean;

////////////////////////////////////////////////////////////////////////////////

implementation

////////////////////////////////////////////////////////////////////////////////

procedure dlgMsg(_s: string);

begin
  MessageBox(0, pChar(_s), '', 0);
end;

////////////////////////////////////////////////////////////////////////////////

procedure dlgMsgEx(_c, _s: string);

begin
  MessageBox(0, pChar(_s), pChar(_c), 0);
end;

////////////////////////////////////////////////////////////////////////////////

procedure dlgError(_s: string);

begin
  MessageBox(0, pChar(_s), 'Error', MB_ICONERROR);
end;

////////////////////////////////////////////////////////////////////////////////

function dlgBrowseFile(_hwnd: HWnd; _dlgTitle, _initialDir, _filter, _defExt: pChar;
                       _filterIndex: longint; _dlgType: TBrowseFile): string;

// _hwnd - should not be null.
// Filter must be a buffer containing pairs of null-terminated filter strings.
// The last chars in the buffer must be two NULL (0) characters.

var _ofn : TOpenFilename;
    _fn  : array[0..259] of char; // buffer for filename
    _res : boolean;

begin
  Result := '';

  // Fill _ofn
  Fillchar(_ofn, SizeOf(_ofn), 0);
  Fillchar(_fn, SizeOf(_fn), 0);
  _ofn.lStructSize  := SizeOf(_ofn);
  _ofn.hInstance    := hInstance;
  with _ofn do
  begin
    hwndOwner       := _hwnd;
    lpstrFile       := @_fn[0];
    nMaxFile        := sizeof(_fn);
    lpstrFilter     := _filter;
    nFilterIndex    := _filterIndex;
    lpstrFileTitle  := NIL;
    nMaxFileTitle   := 0;
    lpstrInitialDir := _initialDir;
    lpstrTitle      := _dlgTitle;
    lpstrDefExt     := _defExt;
    if (_dlgType = bfOpen)
      then Flags := OFN_EXPLORER or OFN_PATHMUSTEXIST or OFN_FILEMUSTEXIST;
    if (_dlgType = bfSave)
      then Flags := OFN_EXPLORER or OFN_PATHMUSTEXIST or OFN_OVERWRITEPROMPT;
  end;

  // Display the Open dialog box.
  _res := false;
  InitCommonControls;
  if (_dlgType = bfOpen) then _res := GetOpenFileName(_ofn);
  if (_dlgType = bfSave) then _res := GetSaveFileName(_ofn);
  if (_res) then Result := StrPas(@_ofn.lpstrFile[0]);
end;

////////////////////////////////////////////////////////////////////////////////

function dlgBrowseOpen(_hwnd: HWnd; _dlgTitle, _initialDir, _filter, _defExt: pChar;
                       _filterIndex: longint): string;

begin
  Result := dlgBrowseFile(_hwnd, _dlgTitle, _initialDir, _filter, _defExt,
                          _filterIndex, bfOpen);

end;

////////////////////////////////////////////////////////////////////////////////

function dlgBrowseSave(_hwnd: HWnd; _dlgTitle, _initialDir, _filter, _defExt: pChar;
                       _filterIndex: longint): string;

begin
  Result := dlgBrowseFile(_hwnd, _dlgTitle, _initialDir, _filter, _defExt,
                          _filterIndex, bfSave);

end;

////////////////////////////////////////////////////////////////////////////////

function dlgAreYouSure(_hwnd: HWnd; _c, _s: string): boolean;

begin
  Result := MessageBox(_hwnd, pChar(_s), pChar(_c),
                       MB_ICONQUESTION or MB_YESNO or MB_DEFBUTTON2) = IDYES;
end;

////////////////////////////////////////////////////////////////////////////////

function  dlgChooseColor(_hwnd: HWnd; var _color: cardinal;
                         _customColors: PCustomColors): boolean;

var
  _c : TChooseColor;
  _v : array[0..15] of TColorRef;

begin
  Fillchar(_c, SizeOf(_c), 0);

  _c.lStructSize := SizeOf(_c);
  if not assigned(_customColors) then
  begin
    fillchar(_v, SizeOf(_v), 0);
    _c.lpCustColors := @_v[0];
  end else _c.lpCustColors := pointer(_customColors);
  _c.hInstance  := hInstance;
  _c.hWndOwner := _hwnd;
  _c.rgbResult := _color;
  _c.Flags := CC_FULLOPEN or CC_RGBINIT;

  InitCommonControls;
  Result := ChooseColor(_c);
  if Result then _color := _c.rgbResult;
end;

////////////////////////////////////////////////////////////////////////////////

end.
