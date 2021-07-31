(*

  DESC:

   DX8NOIDX - Unit for converting DX8 indexed primitives
              to non indexed ones.
   Copyright © 2002 Lord Trancos / Pulsar Studio.
   <lordtrancos@softhome.net>

  API:

   DirectX 8.0

*)

unit DX8NOIDX;

// ------------------------------------------------------------------

interface

uses Windows, DirectXGraphics, D3DX8, DX8GFX, DX8TEXT;

type
  PNonIndex = ^TNonIndex;
  TNonIndex = record
                tris : pointer;  // triangles
                mats : pointer;  // array of materials
                txts : pointer;  // array of texture indexes
                subs : pointer;  // subset info (see TNISub)
                fvf  : cardinal; // fvf
                nTri : longint;  // # tris
                nSub : longint;  // # subsets
                vSiz : longint;  // vertex size
                ok   : boolean;
              end;

  PNISub = ^TNISub;
  TNISub = record
             offset : longint;
             count  : longint;
           end;

procedure resetNonIndexed(var _ni: TNonIndex);
procedure freeNonIndexed(var _ni: TNonIndex);
function  indexedToNonIndexed(_src: ID3DXMesh;
                              _srcMat, _srcTxt: pointer;
                              var _dst: TNonIndex): boolean;
function  niGetFaceSubset(_faceIdx: longint; _ni: TNonIndex): longint;
procedure renderNonIndexed(_ni: TNonIndex);

// ------------------------------------------------------------------

implementation

// ------------------------------------------------------------------

procedure resetNonIndexed(var _ni: TNonIndex);

begin
  FillChar(_ni, SizeOf(_ni), 0);
end;

// ------------------------------------------------------------------

procedure freeNonIndexed(var _ni: TNonIndex);

begin
  if _ni.tris <> NIL then FreeMem(_ni.tris, _ni.nTri * _ni.vSiz * 3);
  if _ni.mats <> NIL then FreeMem(_ni.mats, _ni.nSub * SizeOf(TD3DMaterial8));
  if _ni.txts <> NIL then FreeMem(_ni.txts, _ni.nSub * SizeOf(word));
  if _ni.subs <> NIL then FreeMem(_ni.subs, _ni.nSub * SizeOf(TNISub));
  resetNonIndexed(_ni);
end;

// ------------------------------------------------------------------

function indexedToNonIndexed(_src: ID3DXMesh;
                             _srcMat, _srcTxt: pointer;
                             var _dst: TNonIndex): boolean;

var
  _p1, _p2 : PByte;
  _dstV : pointer; // dest vertex
  _dstS : PNISub; // dest subset
  _idxCur : PWord;
  _attCur : PD3DXAttributeRange;
  _attTab : pointer; // attribute table
  _attTabEnt : dWord;
  _triCnt : longint;
  _i, _j : longint;

  // ----------------------------------

  procedure CopyVertex(_idx: longint);

  var _srcV : pointer;

  begin
    _srcV := pointer(_p1);
    inc(longint(_srcV), _idx * _dst.vSiz);
    Move(_srcV^, _dstV^, _dst.vSiz);
    inc(longint(_dstV), _dst.vSiz);
  end;

  // ----------------------------------

begin
  Result := false;

  // Clear result
  resetNonIndexed(_dst);

  // Check parameters
  if (_src = NIL) then exit;
  if (_src.GetNumFaces = 0) then exit;

  // Get number of attribute table entries.
  _src.GetAttributeTable(NIL, _attTabEnt);
  if _attTabEnt = 0 then exit;

  // Get memory for attrib table
  try
    GetMem(_attTab, SizeOf(TD3DXAttributeRange) * _attTabEnt);
  except
    exit;
  end;

  // Copy attribute table.
  if failed(_src.GetAttributeTable(_attTab, _attTabEnt)) then
  begin
    FreeMem(_attTab, SizeOf(TD3DXAttributeRange) * _attTabEnt);
    exit;
  end;

  // Store some info
  _dst.fvf  := _src.GetFVF;
  _dst.nTri := _src.GetNumFaces;
  _dst.nSub := _attTabEnt;
  _dst.vSiz := D3DXGetFVFVertexSize(_dst.FVF);

  // Lock vertex buffer and index buffer
  _p1 := NIL;
  _p2 := NIL;
  if failed(_src.LockIndexBuffer(D3DLOCK_READONLY, _p2)) or
     failed(_src.LockVertexBuffer(D3DLOCK_READONLY, _p1)) then
  begin
    // Free attib table
    FreeMem(_attTab, SizeOf(TD3DXAttributeRange) * _attTabEnt);
    // Unlock.
    if _p2 <> NIL then _src.UnlockIndexBuffer;
    if _p1 <> NIL then _src.UnlockVertexBuffer;
    // Free and exit.
    exit;
  end;

  // Get memory for new buffers
  try
    GetMem(_dst.tris, _dst.nTri * _dst.vSiz * 3);
    GetMem(_dst.mats, _dst.nSub * SizeOf(TD3DMaterial8));
    GetMem(_dst.txts, _dst.nSub * SizeOf(word));
    GetMem(_dst.subs, _dst.nSub * SizeOf(TNISub));
    // fill mats and txts buffers
    Move(_srcMat^, _dst.mats^, _dst.nSub * SizeOf(TD3DMaterial8));
    Move(_srcTxt^, _dst.txts^, _dst.nSub * SizeOf(word));    
  except
    freeNonIndexed(_dst);
    // Free attib table
    FreeMem(_attTab, SizeOf(TD3DXAttributeRange) * _attTabEnt);
    // Unlock.
    if _p2 <> NIL then _src.UnlockIndexBuffer;
    if _p1 <> NIL then _src.UnlockVertexBuffer;
    // Free and exit.
    exit;
  end;

  _triCnt := 0;
  _dstV := _dst.tris;
  _dstS := _dst.subs;
  pointer(_attCur) := _attTab;

  for _i := 0 to _dst.nSub - 1 do
  begin
    pointer(_idxCur) := pointer(_p2);
    inc(_idxCur, _attCur^.FaceStart * 3);
    // save new subset info
    _dstS^.offset := _triCnt;
    _dstS^.count  := _attCur^.FaceCount;
    // Process faces in this subset
    for _j := 0 to _attCur^.FaceCount-1 do
    begin
      CopyVertex(_idxCur^);
      inc(_idxCur);
      CopyVertex(_idxCur^);
      inc(_idxCur);
      CopyVertex(_idxCur^);
      inc(_idxCur);
      // next tri
      inc(_triCnt);
    end;
    // next subset
    inc(_dstS);
    inc(_attCur);
  end;

  // Free attib table
  FreeMem(_attTab, SizeOf(TD3DXAttributeRange) * _attTabEnt);

  // Unlock.
  if _p2 <> NIL then _src.UnlockIndexBuffer;
  if _p1 <> NIL then _src.UnlockVertexBuffer;

  // All right
  _dst.ok := true;
  Result := true;
end;

// ------------------------------------------------------------------

function niGetFaceSubset(_faceIdx: longint; _ni: TNonIndex): longint;

var _cnt : longint;
    _cur : PNISub;
    _idx : longint;

begin
  Result := -1;

  _idx := 0;
  _cur := _ni.subs;
  for _cnt := 0 to _ni.nSub - 1 do
  begin
    if (_faceIdx >= _idx) and (_faceIdx <= _idx + _cur^.count - 1) then
    begin
      // subset found
      Result := _cnt;
      break;
    end;
    // next subset
    inc(_idx, _cur^.count);
    inc(_cur);
  end;
end;

// ------------------------------------------------------------------

procedure renderNonIndexed(_ni: TNonIndex);

var _cnt : longint;
    _sub : PNISub;
    _tri : pointer;
    _mat : PD3DMaterial8;
    _tex : PWord;

begin
  if (not _ni.ok) or (_ni.nSub = 0) then exit;

  D3DDEV8.SetVertexShader(_ni.fvf);

  _sub := _ni.subs;
  _mat := _ni.mats;
  _tex := _ni.txts;
  for _cnt := 0 to _ni.nSub - 1 do
  begin
    _tri := _ni.tris;
    inc(longint(_tri), _sub^.offset * _ni.vSiz * 3);

    // Set material
    D3DDEV8.SetMaterial(_mat^);

    // Set texture
    if _tex^ <> C_NO_TEXTURE
      then D3DDEV8.SetTexture(0, G_TE_Textures[_tex^].Text)
        else D3DDEV8.SetTexture(0, NIL);

    // Render
    D3DDEV8.DrawPrimitiveUP(D3DPT_TRIANGLELIST, _sub^.count, _tri,
                           _ni.vSiz);

    // next subset
    inc(_sub);
    inc(_mat);
    inc(_tex);    
  end;
end;

// ------------------------------------------------------------------

end.
