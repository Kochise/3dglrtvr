unit filez_ex;

// -------------------------------------------------------------------

interface

uses Windows, SysUtils, ShlObj;

procedure rewritePath(var _path: string);
function chooseDir(_owner: hWnd; _title: pChar): string;

// -------------------------------------------------------------------

implementation

// -------------------------------------------------------------------

procedure rewritePath(var _path: string);

var
  _len : longint;

begin
  _len := length(_path);
  if (_len = 0) then exit;
  if (_path[_len] <> ':') and (_path[_len] <> '\')
    then _path := _path + '\';
end;

// -------------------------------------------------------------------

function chooseDir(_owner: hWnd; _title: pChar): string;

var _browInfo : TBrowseInfo;
    _PIDL     : PItemIDList;
    _name     : array[0..MAX_PATH] of char;

begin
  Result := '';

  FillChar(_browInfo, sizeOf(_browInfo), 0);
  with _browInfo do
  begin
    hwndOwner      := _owner;
    pszDisplayName := @_name[0];
    lpszTitle      := _title;
    ulFlags        := BIF_RETURNONLYFSDIRS;
  end;

  _PIDL := SHBrowseForFolder(_browInfo);
  if Assigned(_PIDL) then
    if SHGetPathFromIDList(_PIDL, _name) then
    begin
      Result := StrPas(_name);
      rewritePath(Result);
    end;
end;

// ------------------------------------------------------------------

end.
