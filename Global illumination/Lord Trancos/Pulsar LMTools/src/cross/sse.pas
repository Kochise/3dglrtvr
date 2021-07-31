// Simple Script Engine Unit
// by Lord Trancos.

// This unit is for creating very simple script languages and for running
// scripts written with these languages.

unit sse;

// ---------------------------------------------------------------------------

interface

uses SysUtils;

const
  SCR_OBJECT_NAME_MAXLEN   = 32;
  SCR_PROPERTY_NAME_MAXLEN = 32;

type
  TScrValueType = (scrVT_Single, scrVT_UINT32, scrVT_Bool);

  TScrCall = procedure; stdcall;

  PScrValue    = ^TScrValue;
  PScrProperty = ^TScrProperty;
  PScrObject   = ^TScrObject;

  TScrValue = record
    Prop  : PScrProperty;
    ID    : byte;
    VType : TScrValueType;
    Addr  : pointer;
    Next  : PScrValue;
  end;

  TScrProperty = record
    Obj    : PScrObject;
    ID     : longint;
    Name   : array[0..SCR_PROPERTY_NAME_MAXLEN] of char;
    Values : PScrValue;
    Call   : TScrCall;
    Next   : PScrProperty;
  end;

  TScrObject = record
    ID    : longint;
    Name  : array[0..SCR_OBJECT_NAME_MAXLEN] of char;
    Props : PScrProperty;
    Next  : PScrObject;
  end;

  TScrLanguage = record
    CChar : char;
    Objs  : PScrObject;
  end;

// functions for defining script language
function  scr_NewLanguage(_comment: char): TScrLanguage;
function  scr_AddObject(var _lang: TScrLanguage;
                            _name: string): pointer;
function  scr_AddProperty(_obj: PScrObject;
                          _name: string;
                          _call: TScrCall): pointer;
function  scr_AddValue(_prop: PScrProperty;
                       _type: TScrValueType;
                       _addr: pointer): pointer;
procedure scr_FreeLanguage(var _lang: TScrLanguage);

// functions for running a script
function scr_Run(_lang: TScrLanguage;
                 _s: string): string;

// bonus functions
function scr_FindObject(_lang: TScrLanguage;
                        _name: string): PScrObject;
function scr_FindProperty(_obj: PScrObject;
                          _name: string): pointer;

// ---------------------------------------------------------------------------

implementation

const
  SCR_ERR01 = 'Object does not exists: ';
  SCR_ERR02 = 'Property|Method not specified.';
  SCR_ERR03 = 'Property|Method does not exists: ';
  SCR_ERR04 = ' is not a valid floating point value.';
  SCR_ERR05 = ' is not a valid 32-bit integer value.';
  SCT_ERR06 = ' is not a valid boolean value.';

// ---------------------------------------------------------------------------

function scr_NewLanguage(_comment: char): TScrLanguage;

begin
  Result.CChar := _comment;
  Result.Objs  := NIL;
end;

// ---------------------------------------------------------------------------

function scr_AddObject(var _lang: TScrLanguage;
                           _name: string): pointer;

// returns a pointer to the object, or NIL if any error

var
  _p : ^PScrObject;
  _n : array[0..SCR_OBJECT_NAME_MAXLEN] of char;
  _id : longint;

begin
  Result := NIL;

  // convert name to uppercase
  _name := Trim(UpperCase(_name));
  if (_name = '') or (length(_name) > SCR_OBJECT_NAME_MAXLEN) then exit;

  // save name
  Fillchar(_n, SizeOf(_n), 0);
  Move(_name[1], _n[0], length(_name));

  // find last object
  _id := 0;
  _p := @_lang.Objs;
  while _p^ <> NIL do
  begin
    // already exists ?
    if CompareMem(@_n[0], @_p^.Name[0], SCR_OBJECT_NAME_MAXLEN) then exit;
    // next
    _p := @_p^.Next;
    inc(_id);
  end;

  // build new object
  try
    GetMem(_p^, SizeOf(TScrObject));
    _p^.ID    := _id;
    Move(_n[0], _p^.Name[0], SizeOf(_n));
    _p^.Props := NIL;
    _p^.Next  := NIL;
  except
    exit;
  end;

  // done!
  Result := _p^;
end;

// ---------------------------------------------------------------------------

function scr_AddProperty(_obj: PScrObject;
                         _name: string;
                         _call: TScrCall): pointer;

var
  _p  : ^PScrProperty;
  _n  : array[0..SCR_PROPERTY_NAME_MAXLEN] of char;
  _id : longint;

begin
  Result := NIL;

  // convert name to uppercase
  _name := Trim(UpperCase(_name));
  if (_name = '') or (length(_name) > SCR_PROPERTY_NAME_MAXLEN) then exit;

  // save name
  Fillchar(_n, SizeOf(_n), 0);
  Move(_name[1], _n[0], length(_name));

  // find last property
  _id := 0;
  _p  := @_obj^.Props;
  while _p^ <> NIL do
  begin
    // already exists ?
    if CompareMem(@_n[0], @_p^.Name[0], SCR_OBJECT_NAME_MAXLEN) then exit;
    // next
    _p := @_p^.Next;
    inc(_id);
  end;

  // build new property
  try
    GetMem(_p^, SizeOf(TScrProperty));
    _p^.ID     := _id;
    _p^.Obj    := _obj;
    Move(_n[0], _p^.Name[0], SizeOf(_n));
    _p^.Values := NIL;
    _p^.Call   := _call;
    _p^.Next   := NIL;
  except
    exit;
  end;

  // done!
  Result := _p^;
end;

// ---------------------------------------------------------------------------

function scr_AddValue(_prop: PScrProperty;
                      _type: TScrValueType;
                      _addr: pointer): pointer;

var
  _p  : ^PScrValue;
  _id : longint;

begin
  Result := NIL;

  // find last value
  _id := 0;
  _p  := @_prop^.Values;
  while _p^ <> NIL do
  begin
    // next
    _p := @_p^.Next;
    inc(_id);
  end;
  if (_id > 255) then exit;

  // build new value
  try
    GetMem(_p^, SizeOf(TScrValue));
    _p^.ID    := _id;
    _p^.Prop  := _prop;
    _p^.VType := _type;
    _p^.Addr  := _addr;
    _p^.Next  := NIL;
  except
    exit;
  end;

  // done!
  Result := _p^;
end;

// ---------------------------------------------------------------------------

function scr_FindObject(_lang: TScrLanguage;
                        _name: string): PScrObject;

var
  _p : ^PScrObject;
  _n : array[0..SCR_OBJECT_NAME_MAXLEN] of char;

begin
  Result := NIL;

  // convert name to uppercase
  _name := Trim(UpperCase(_name));
  if (_name = '') or (length(_name) > SCR_OBJECT_NAME_MAXLEN) then exit;

  // save name
  Fillchar(_n, SizeOf(_n), 0);
  Move(_name[1], _n[0], length(_name));

  // find object
  _p := @_lang.Objs;
  while _p^ <> NIL do
  begin
    // found ?
    if CompareMem(@_n[0], @_p^.Name[0], SCR_OBJECT_NAME_MAXLEN) then
    begin
      Result := _p^;
      exit;
    end;
    // next
    _p := @_p^.Next;
  end;
end;

// ---------------------------------------------------------------------------

function scr_FindProperty(_obj: PScrObject;
                          _name: string): pointer;

var
  _p : ^PScrProperty;
  _n : array[0..SCR_PROPERTY_NAME_MAXLEN] of char;

begin
  Result := NIL;

  // convert name to uppercase
  _name := Trim(UpperCase(_name));
  if (_name = '') or (length(_name) > SCR_PROPERTY_NAME_MAXLEN) then exit;

  // save name
  Fillchar(_n, SizeOf(_n), 0);
  Move(_name[1], _n[0], length(_name));

  // find property
  _p  := @_obj^.Props;
  while _p^ <> NIL do
  begin
    // found ?
    if CompareMem(@_n[0], @_p^.Name[0], SCR_OBJECT_NAME_MAXLEN) then
    begin
      Result := _p^;
      exit;
    end;
    // next
    _p := @_p^.Next;
  end;
end;

// ---------------------------------------------------------------------------


procedure scr_FreeValue(_val: PScrValue);

begin
  try
    FreeMem(_val, SizeOf(TScrValue));
  except
  end;
end;

// ---------------------------------------------------------------------------

procedure scr_FreeProperty(_prop: PScrProperty);

var
  _v1, _v2 : PScrValue;

begin
  // free values of this property
  _v1 := _prop^.Values;
  while _v1 <> NIL do
  begin
    _v2 := _v1^.next;
    scr_FreeValue(_v1);
    _v1 := _v2;
  end;

  // free this property
  try
    FreeMem(_prop, SizeOf(TScrProperty));
  except
  end;
end;

// ---------------------------------------------------------------------------

procedure scr_FreeObject(_obj: PScrObject);

var
  _p1, _p2 : PScrProperty;

begin
  // free properties of this object
  _p1 := _obj^.Props;
  while _p1 <> NIL do
  begin
    _p2 := _p1^.next;
    scr_FreeProperty(_p1);
    _p1 := _p2;
  end;

  // free this object
  try
    FreeMem(_obj, SizeOf(TScrObject));
  except
  end;
end;

// ---------------------------------------------------------------------------

procedure scr_FreeLanguage(var _lang: TScrLanguage);

var
  _o1, _o2 : PScrObject;

begin
  _o1 := _lang.Objs;
  while _o1 <> NIL do
  begin
    _o2 := _o1^.next;
    scr_FreeObject(_o1);
    _o1 := _o2;
  end;

  _lang.Objs := NIL;
end;

// ---------------------------------------------------------------------------

function scr_StrRemoveComment(_lang: TScrLanguage;
                           _s: string): string;

var _c : longint;

begin
  Result := '';

  for _c := 1 to length(_s) do
    if _s[_c] <> _lang.CChar
      then Result := Result + _s[_c] else exit;
end;

// ---------------------------------------------------------------------------

function scr_StrGetObject(_s: string): string;

var _c : longint;

begin
  Result := '';

  for _c := 1 to length(_s) do
    if _s[_c] <> '.'
      then Result := Result + UpCase(_s[_c]) else exit;
end;

// ---------------------------------------------------------------------------

function scr_StrGetProperty(_s: string): string;

var
  _c : longint;
  _b : boolean;

begin
  Result := '';

  _b := false;
  for _c := 1 to length(_s) do
  begin
    if (_b) then
    begin
      if _s[_c] <> '='
        then Result := Result + UpCase(_s[_c])
          else exit;
    end else if _s[_c] = '.' then _b := true;
  end;
end;

// ---------------------------------------------------------------------------

function scr_StrGetValue(_s: string; _num: byte): string;

var
  _c : longint;
  _b : byte;

begin
  Result := '';

  _b := 0;
  for _c := 1 to length(_s) do
  begin
    if (_b = _num) then
    begin
      if _s[_c] <> ','
        then Result := Result + UpCase(_s[_c])
          else exit;
    end else if (_s[_c] = ',') or (_s[_c] = '=') then inc(_b);
  end;
end;

// ---------------------------------------------------------------------------

procedure scr_StrRewriteFloat(var _s: string);

var
  _c : longint;

begin
  for _c := 1 to length(_s) do
    if _s[_c] = '.' then _s[_c] := DecimalSeparator;
end;

// ---------------------------------------------------------------------------

function scr_Run(_lang: TScrLanguage;
                 _s: string): string;

// Executes a valid script command line. (only one!)
// Returns an empty string if all is ok,
// otherwise returns an error string.

var
  _o, _p, _v : string;
  _obj : PScrObject;
  _prop : PScrProperty;
  _val : PScrValue;
  _vidx : longint;

  _vs : single;
  _ps : ^single;

  _vl : longint;
  _pl : ^longint;

  _vb : boolean;
  _pb : ^boolean;

begin
  Result := '';

  // -
  _vs := 0;
  _vl := 0;

  // check string
  _s := Trim(_s);
  if _s = '' then exit;

  // remove comment and check again
  _s := scr_StrRemoveComment(_lang, _s);
  if _s = '' then exit;

  // get object name
  _o := scr_StrGetObject(_s);

  // get object by name
  _obj := scr_FindObject(_lang, _o);
  if _obj = NIL then
  begin
    Result := SCR_ERR01 + _o;
    exit;
  end;

  // get property name
  _p := scr_StrGetProperty(_s);
  if _p = '' then
  begin
    Result := SCR_ERR02;
    exit;
  end;

  // get property by name
  _prop := scr_FindProperty(_obj, _p);
  if _prop = NIL then
  begin
    Result := SCR_ERR03 + _p;
    exit;
  end;

  // process values
  _val := _prop^.Values;
  _vidx := 1;
  while (_val <> NIL) do
  begin
    _v := Trim(scr_StrGetValue(_s, _vidx));

    // convert and use value
    case _val^.VType of
      // single
      scrVT_Single : begin
                       // convert
                       try
                         scr_StrRewriteFloat(_v);
                         _vs := StrToFloat(_v);
                       except
                         Result := _v + SCR_ERR04;
                         exit;
                       end;

                       // use
                       if _val^.Addr <> NIL then
                       begin
                         _ps  := _val^.Addr;
                         _ps^ := _vs;
                       end;
                     end;
      // 32 bit integer
      scrVT_UINT32 : begin
                       // convert
                       try
                         _vl := StrToInt(_v);
                       except
                         Result := _v + SCR_ERR05;
                         exit;
                       end;

                       // use
                       if _val^.Addr <> NIL then
                       begin
                         _pl  := _val^.Addr;
                         _pl^ := _vl;
                       end;
                     end;
      // boolean
      scrVT_Bool : begin
                      // convert
                      _vb := (_v = 'TRUE') or (_v = 'YES');
                      if (not _vb) then if (_v <> 'FALSE') and (_v <> 'NO') then
                      begin
                        Result := _v + SCT_ERR06;
                        exit;
                      end;

                      // use
                      if _val^.Addr <> NIL then
                      begin
                        _pb  := _val^.Addr;
                        _pb^ := _vb;
                      end;
                    end;
    end;

    // next value
    _val := _val^.Next;
    inc(_vidx);
  end;

  // use property callback
  if assigned(_prop^.Call) then _prop^.Call;
end;

// ---------------------------------------------------------------------------

end.
