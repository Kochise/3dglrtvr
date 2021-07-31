(*

  DESC:

   BITMAP - Unit for working with bitmap type images.
   Copyright © 2002-2003 Lord Trancos / Pulsar Studio.
   <lordtrancos@softhome.net>

  API:

   NONE

*)

unit bitmap;

// ---------------------------------------------------------------------------

interface

function  bitmap_flip(_height, _linelength: longint; _data: pointer): boolean;
function  bitmap_mirror(_width, _height, _bpp: longint;
                       _data: pointer): boolean;
function  bitmap_rotateR(_srcW, _srcH: longint; _bpp: byte;
                         _srcData, _dstData: pointer): boolean;
procedure bitmap_CopyInside(_src, _dst : pointer;
                            _srcW, _srcH, _dstW, _dstH,
                            _dstX, _dstY : cardinal);
procedure bitmap_X8R8G8B8toR5G6B5(_src32bit, _dst16bit: pointer;
                                  _width, _height: longint);
procedure bitmap_X8R8G8B8toB8G8R8(_src32bit, _dst24bit: pointer;
                                  _dstBpl, _width, _height: longint);
                                  
// ---------------------------------------------------------------------------

implementation

// ---------------------------------------------------------------------------

function bitmap_flip(_height, _linelength: longint; _data: pointer): boolean;

// height - image height
// linelength - image horizontal line length in bytes

var
  _y : longint;
  _p, _q, _r  : pointer;

begin
  Result := false;

  // get memory for one line
  try
    GetMem(_p, _linelength);
  except
    exit;
  end;

  // process
  for _y := 0 to (_height shr 1)-1 do
  begin
    _q := pointer(longint(_data)+(_y*_linelength));
    Move(_q^, _p^, _linelength);
    _r := pointer(longint(_data)+((_height-_y-1)*_linelength));
    Move(_r^, _q^, _linelength);
    Move(_p^, _r^, _linelength);
  end;

  // free memory
  FreeMem(_p, _linelength);

  // all right
  Result := true;
end;

// ---------------------------------------------------------------------------

function bitmap_mirror(_width, _height, _bpp: longint;
                       _data: pointer): boolean;

// only supports 8, 15, 24 and 32 bpp

var
  _pixel : array[0..3] of byte;
  _q, _r, _s : pointer;
  _x, _y, _z, _sz : longint;

begin
  Result := false;

  // check bpp
  case _bpp of
    8  : _z := 1;
    16 : _z := 2;
    24 : _z := 3;
    32 : _z := 4;
    else exit;
  end;
  _sz := _width * _z;

  // process
  _q := _data;
  for _y := 0 to _height-1 do
  begin
    _r := _q;
    for _x := 0 to (_width shr 1)-1 do
    begin
      Move(_r^, _pixel[0], _z);
      _s := pointer(longint(_data) + ((_y+1) * _sz) - ((_width-_x)*_z));
      Move(_s^, _r^, _z);
      Move(_pixel[0], _s^, _z);
      // next pixel
      inc(longint(_r), _z);
    end;
    inc(longint(_q), _sz);
  end;

  // all right
  Result := true;
end;

// ---------------------------------------------------------------------------

function bitmap_rotateR(_srcW, _srcH: longint; _bpp: byte;
                        _srcData, _dstData: pointer): boolean;

// Rotates a bitmap 90º right.
// only supports 8, 15, 24 and 32 bpp

var
  _jmp : byte;
  _s, _d : pointer;
  _x, _y : longint;  
  _bpl : longint;

begin
  Result := false;

  // get pixel size
  case _bpp of
    8:  _jmp := 1;
    16: _jmp := 2;
    24: _jmp := 3;
    32: _jmp := 4;
    else exit;
  end;
  _bpl := _srcH * _jmp;

  // --    
  _s := _srcData;
  for _y := 0 to _srcH - 1 do
  begin
    // set dest. cursor pos
    _d := _dstData;
    inc(longint(_d), (_srcH - _y - 1) * _jmp);

    // process line
    for _x := 0 to _srcW - 1 do
    begin
      // copy pixel
      Move(_s^, _d^, _jmp);
      // next pixel
      inc(longint(_s), _jmp);
      inc(longint(_d), _bpl);  
    end;
  end;  

  // all right
  Result := true;
end;

// ---------------------------------------------------------------------------

procedure bitmap_CopyInside(_src, _dst : pointer;
                            _srcW, _srcH, _dstW, _dstH,
                            _dstX, _dstY : cardinal);

// _srcW and _dstW - are the width of the pics in BYTES!
// _dstX - destination X coordinate in BYTES!
// _srcH and _dstH - are height in pixels.
// _dstY - destination Y in pixels.
// no cliping is done!

var
  _cnt : longint;
  _sc, _dc : pointer;

begin
  if (_dstX + _srcW > _dstW) or
     (_dstY + _srcH > _dstH) then exit;

  // draw
  _sc := _src;
  _dc := _dst;
  inc(longint(_dc), (_dstY * _dstW) + _dstX);
  for _cnt := 0 to _srcH-1 do
  begin
    // draw line
    Move(_sc^, _dc^, _srcW);
    // next line
    inc(longint(_sc), _srcW);
    inc(longint(_dc), _dstW);
  end;
end;

// ---------------------------------------------------------------------------

procedure bitmap_X8R8G8B8toR5G6B5(_src32bit, _dst16bit: pointer;
                                  _width, _height: longint);

var
  _32 : ^cardinal;
  _16 : ^word;
  _x, _y : longint;

begin
  _32 := _src32bit;
  _16 := _dst16bit;
  for _y := 0 to _height - 1 do
    for _x := 0 to _width - 1 do
    begin
      _16^ := ((_32^ and $F8) shr 3)
            + ((_32^ and $F800) shr 6)
            + ((_32^ and $F80000) shr 9)
            + ((_32^ and $80000000) shr 16);
      // next pixel
      inc(_32);
      inc(_16);
    end;
end;

// ---------------------------------------------------------------------------

procedure bitmap_X8R8G8B8toB8G8R8(_src32bit, _dst24bit: pointer;
                                  _dstBpl, _width, _height: longint);

// _dstBpl - bytes per line on destination bmp

var
  _32 : ^cardinal;
  _24 : ^byte;
  _x, _y : longint;
  _xtra : longint;

begin
  _32 := _src32bit;
  _24 := _dst24bit;
  _xtra := _dstBpl - (_width * 3);

  for _y := 0 to _height - 1 do
  begin
    // convert and copy
    for _x := 0 to _width - 1 do
    begin
      _24^ := (_32^ and $FF);
      inc(_24);
      _24^ := (_32^ shr 8) and $FF;
      inc(_24);
      _24^ := (_32^ shr 16) and $FF;
      inc(_24);
      // next pixel
      inc(_32);
    end;
    // clear xtra bytes
    for _x := 0 to _xtra - 1 do
    begin
      _24^ := 0;
      inc(_24);
    end;
  end;   
end;

// ---------------------------------------------------------------------------

end.
