(*

  DESC:

   DX8XMESH - Simple unit for loading and rendering X files.
   Copyright © 2001-2002 Lord Trancos.
   <lordtrancos@softhome.net>

  API:

   DirectX 8.0

*)

unit DX8XMESH;

// -------------------------------------------------------------------

interface

uses
  Windows, SysUtils, Classes, DirectXGraphics, D3DX8, DX8GFX, DX8TEXT;

type
  TMeshSource = record
                  Mesh  : ID3DXMesh;  // Mesh data.
                  FVF   : cardinal;   // FVF
                  VSize : cardinal;   // Vertex size
                  Mats  : pointer;    // Material buffer.
                  Txts  : pointer;    // Texture indexes buffer.
                  MatCount : dWord;   // Number of materials.
                end;

// Main functions.
function  xLoadMesh(_fn: string; _textTag: longint): TMeshSource;
procedure xFreeMesh(var _mesh: TMeshSource;
                        _freeTextures: boolean);
procedure xResetMesh(var _mesh: TMeshSource);
procedure xRenderMesh(_mesh: TMeshSource);
procedure xRenderMeshB(_mesh: TMeshSource);

// -------------------------------------------------------------------

implementation

// -------------------------------------------------------------------

procedure xResetMesh(var _mesh: TMeshSource);

begin
  FillChar(_mesh, sizeOf(_mesh), 0);
end;

// -------------------------------------------------------------------

procedure xFreeMesh(var _mesh: TMeshSource;
                        _freeTextures: boolean);

var _i : longint;
    _p : ^word;

begin
  if (_mesh.MatCount > 0) then
    try
      if (_mesh.Mats <> NIL)
        then FreeMem(_mesh.Mats, SizeOf(TD3DMaterial8) * _mesh.MatCount);
      if (_mesh.Txts <> NIL) then
      begin
        _p := _mesh.Txts;
        if (_freeTextures) then
          for _i := 0 to _mesh.MatCount - 1 do
          begin
            if (_p^ <> C_NO_TEXTURE) then TE_FreeText(_p^);
            inc(_p);
          end;
        FreeMem(_mesh.Txts, SizeOf(word) * _mesh.MatCount);
      end;
      if (_mesh.Mesh <> NIL) then _mesh.Mesh := NIL;
    except
    end;
  xResetMesh(_mesh);
end;

// -------------------------------------------------------------------

function xLoadMesh(_fn: string; _textTag: longint): TMeshSource;

var
  _pAdjacency, _pD3DXMtrlBuffer : ID3DXBuffer;
  _d3dxMaterials : PD3DXMaterial;
  _matCursor : PD3DMaterial8;
  _texCursor : PWord;
  _path : string;
  _texFN : string; // Texture filename
  _idxl : longint;
  _idx : word; // Texture index (0 = none)
  _i : dWord;
  _faceRemap : ID3DXBuffer;

begin
  xResetMesh(Result);

  // Check if texture engine was initialized.
  if (not G_TE_Initialized) then exit;

  // Load mesh.
  _pAdjacency := NIL;
  _pD3DXMtrlBuffer := NIL;
  if failed(D3DXLoadMeshFromX(pChar(_fn), D3DXMESH_SYSTEMMEM,
                              D3DDEV8, _pAdjacency, _pD3DXMtrlBuffer,
                              Result.MatCount, Result.Mesh)) then exit;
  _pAdjacency := NIL;

  // This will speed up the rendering of the mesh...
  _faceRemap := NIL;
  if failed(Result.Mesh.OptimizeInplace(D3DXMESHOPT_ATTRSORT,
                                        NIL, NIL, NIL, _faceRemap)) then
  begin
    xFreeMesh(Result, true);
    exit;
  end;
  _faceRemap := NIL;

  // Get FVF and Vertex Size
  Result.FVF := Result.Mesh.GetFVF;
  Result.VSize := D3DXGetFVFVertexSize(Result.FVF);

  // After loading the mesh object and material information,
  // we need to extract the material properties and texture
  // names from the material buffer.
  _d3dxMaterials := _pD3DXMtrlBuffer.GetBufferPointer;

  // Any material ?
  if (Result.MatCount = 0) or (_d3dxMaterials = NIL) then
    begin
      _pD3DXMtrlBuffer := NIL;
      xFreeMesh(Result, true);
      exit;
    end;

  // Get memory for material and texture buffers.
  try
    GetMem(Result.Mats, SizeOf(TD3DMaterial8) * Result.MatCount);
    GetMem(Result.Txts, SizeOf(word) * Result.MatCount);
    FillChar(Result.Txts^, SizeOf(word) * Result.MatCount, 0);
  except
    _pD3DXMtrlBuffer := NIL;
    xFreeMesh(Result, true);
    exit;
  end;

  // Get path to the textures.
  _path := ExtractFilePath(_fn);

  // Load materials and textures.
  _matCursor := Result.Mats;
  _texCursor := Result.Txts;
  for _i := 0 to Result.MatCount - 1 do
    begin
      // Set the ambient color for the material (D3DX does not do this)
      _d3dxMaterials^.MatD3D.Ambient := _d3dxMaterials^.MatD3D.Diffuse;

      // Copy the material
      _matCursor^ := _d3dxMaterials^.MatD3D;

      // Any texture ?
      _idx := C_NO_TEXTURE;
      if (_d3dxMaterials^.pTextureFilename <> NIL) then
        begin
          // Get texture filename.
          _texFN := _path + StrPas(_d3dxMaterials^.pTextureFilename);

          // Load texture
          _idxl := TE_LoadText(_texFN, _textTag);
          if _idxl < 0 then
          begin
            _pD3DXMtrlBuffer := NIL;
            xFreeMesh(Result, true);
            exit;
          end;
          _idx := _idxl;
        end;

      // Set texture index.
      _texCursor^ := _idx;

      // Next
      inc(_d3dxMaterials);
      inc(_matCursor);
      inc(_texCursor);
    end;

  // Done with the material buffer
  _pD3DXMtrlBuffer := NIL;

  // All right
end;

// -------------------------------------------------------------------

procedure xRenderMesh(_mesh: TMeshSource);

var
  _matCursor : PD3DMaterial8;
  _texCursor : PWord;
  _i : longint;

begin
  _matCursor := _mesh.Mats;
  _texCursor := _mesh.Txts;
  for _i := 0 to _mesh.MatCount - 1 do
    begin
      // Set the material for the subset.
      D3DDEV8.SetMaterial(_matCursor^);
      // Set the texture for the subset.
      if _texCursor^ <> C_NO_TEXTURE
        then D3DDEV8.SetTexture(0, G_TE_Textures[_texCursor^].Text)
          else D3DDEV8.SetTexture(0, NIL);
      // Draw subset.
      _mesh.Mesh.DrawSubset(_i);
      // Next
      inc(_matCursor);
      inc(_texCursor);
    end;
end;

// -------------------------------------------------------------------

procedure xRenderMeshB(_mesh: TMeshSource);

var _i : longint;

begin
  for _i := 0 to _mesh.MatCount - 1 do _mesh.Mesh.DrawSubset(_i);
end;

// -------------------------------------------------------------------

end.
