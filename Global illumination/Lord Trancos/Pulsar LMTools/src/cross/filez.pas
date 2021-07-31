unit filez;

// -------------------------------------------------------------------

interface

type TOpenBinFileMode = (obfRead, obfWrite);

function filezOpenBin(_FileName: string; _Mode: TOpenBinFileMode;
                      var _File: file): boolean;
function getBuffer(var _srcF: file;
                   var _srcM: pointer;
                       _dest: pointer;
                       _size: longint): boolean;

// -------------------------------------------------------------------

implementation

// -------------------------------------------------------------------

function filezOpenBin(_FileName: string; _Mode: TOpenBinFileMode;
                      var _File: file): boolean;

begin
  Result := false;
  if _FileName = '' then exit;

  Assign(_File, _FileName);
  {$I-}
  case _Mode of
    obfRead  : begin FileMode := 0; Reset(_File, 1); end;
    obfWrite : ReWrite(_File, 1);
  end;
  FileMode := 2;
  {$I+}
  if IOResult = 0 then Result := true;
end;

// ------------------------------------------------------------------

function getBuffer(var _srcF: file;
                   var _srcM: pointer;
                       _dest: pointer;
                       _size: longint): boolean;

begin
  Result := false;

  // Load from file ?
  if (_srcM = NIL) then
  begin
    try
      BlockRead(_srcF, _dest^, _size);
    except
      exit;
    end;
  end
  else // Load from memory
  begin
    try
      Move(_srcM^, _dest^, _size);
      inc(longint(_srcM), _size);
    except
      exit;
    end;
  end;

  // All right
  Result := true;
end;

// ------------------------------------------------------------------

end.
