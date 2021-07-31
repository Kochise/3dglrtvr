(*

  DESC:

   OCTREE - Unit for building an octree.
   Copyright © 2002 Lord Trancos / Pulsar Studio.
   <lordtrancos@softhome.net>

  API:

   NONE

*)

unit Octree;

// ------------------------------------------------------------------

interface

uses aabb_tri, vTypes;

const
  // values for TOctItem.flags
  C_OCT_IF_INSIDE_NODE = $0001;

type
  // Callback called each time one triangle is needed - this slows 
  // allows to use this unit with any Gfx API (DirectX, OpenGL, SDL...)
  // and with indexed or non indexed meshes.
  TOctGetTri = procedure(_index: longint; var _tri: T_Tri); stdcall;

  // AABB - Axis Aligned Bounding Box
  POctAABB = ^TOctAABB;
  TOctAABB = record
               min, max : T_Vec;
             end;

  // Each terminal node have an array of "items" (itemData) that tell
  // us wich triangles are inside and if those triangles are full or
  // partially inside (flags).
  POctItem = ^TOctItem;
  TOctItem = record
               triIdx : longint; // triangle index
               flags  : word;    // tell us if it's full inside the node or not
             end;

  // Each node of the octree
  POctNode = ^TOctNode;
  TOctNode = record
               father   : POctNode; // node's father
               sonID    : byte;     // child index
               aabb     : TOctAABB; // node's aabb
               center   : T_Vec;    // node's center
               size     : single;   // node's X,Y,Z size (it's a cube!)
               itemData : pointer;  // array of TOctItem
               itemCnt  : longint;  // number of items on itemData
               child    : array[0..7] of POctNode; // child nodes
               splited  : boolean;  // node has child nodes
               prev,
               next     : POctNode; // for sequential access to the nodes
               // Next ones will allow you to store your custom data.
               // octFree, frees also memory stored in these buffers.
               usrTagA  : longint;  // User Tag A
               usrDataA : pointer;  // User Buffer A
               usrSizeA : longint;  // User Buffer A Size (in bytes)
               usrTagB  : longint;  // Uset Tag B
               usrDataB : pointer;  // User Buffer B
               usrSizeB : longint;  // User Buffer B Size (in bytes)
             end;

  // Octree
  POctree = ^TOctree;
  TOctree = record
              nTri   : longint;  // total number of triangles
              nNodes : longint;  // total number of nodes
              nTerm  : longint;  // total number of terminal nodes
              root   : TOctNode; // root node
              ok     : boolean;  // octree created successfully ?
            end;

// main functions
procedure octReset(var _oct: TOctree);
function  octBuild(_nTri, _minNodeTri: longint; _minNodeSize: single;
                   _maxLevels: byte; _getTriCB: TOctGetTri): TOctree;
procedure octFree(var _oct: TOctree);

// ------------------------------------------------------------------

implementation

type
  // used for building the octree
  POctBuild = ^TOctBuild;
  TOctBuild = record
                nTri        : longint;
                minNodeTri  : longint;
                minNodeSize : single;
                maxLevels   : longint;
                getTri      : TOctGetTri;
                workBuf     : pointer;    // work buffer
                prevNode    : POctNode;
              end;

// ------------------------------------------------------------------

function octVec(_x, _y, _z: single): T_Vec;

begin
  Result[X] := _x;
  Result[Y] := _y;
  Result[Z] := _z;
end;

// ------------------------------------------------------------------

function octVecAddS(_v: T_Vec; _s: single): T_Vec;

begin
  Result[X] := _v[X] + _s;
  Result[Y] := _v[Y] + _s;
  Result[Z] := _v[Z] + _s;
end;

// ------------------------------------------------------------------

function octVertexInAABB(_v: P_Vec; _aabb: POctAABB): boolean;

begin
  Result := not ((_v^[X] < _aabb^.min[X]) or (_v^[Y] < _aabb^.min[Y]) or
                 (_v^[Z] < _aabb^.min[Z]) or (_v^[X] > _aabb^.max[X]) or
                 (_v^[Y] > _aabb^.max[Y]) or (_v^[Z] > _aabb^.max[Z]));
end;

// ------------------------------------------------------------------

function octTriInsideAABB(_t: P_Tri; _aabb: POctAABB): longint;

// Returns the number of vertices inside the aabb.
// Warning: If this function returns 0, this does not mean that the
//          triangle is outside the node.

var _cnt : longint;

begin
  Result := 0;
  for _cnt := 0 to 2 do
    if octVertexInAABB(@_t^[_cnt], _aabb) then inc(Result);
end;

// ------------------------------------------------------------------

procedure octGetRootNodeCube(_b: POctBuild;
                             var _size   : single;
                             var _center : T_Vec;
                             var _aabb   : TOctAABB);

var _i, _j, _k : longint;
    _tri : T_Tri;
    _maxLen, _xLen, _yLen, _zLen : single;

begin
  // get the aabb
  for _i := 0 to _b^.nTri - 1 do
  begin
    _b^.getTri(_i, _tri);
    for _j := 0 to 2 do
    begin
      if (_i = 0) and (_j = 0) then
      begin
        _aabb.min := _tri[0]; 
        _aabb.max := _tri[0];
      end else
      begin
        for _k := 0 to 2 do
        begin
          if _aabb.min[_k] > _tri[_j][_k] then _aabb.min[_k] := _tri[_j][_k];
          if _aabb.max[_k] < _tri[_j][_k] then _aabb.max[_k] := _tri[_j][_k];
        end;
      end;
    end;
  end;

  // Convert our AABB into a cube
  _xLen := _aabb.max[X] - _aabb.min[X];
  _yLen := _aabb.max[Y] - _aabb.min[Y];
  _zLen := _aabb.max[Z] - _aabb.min[Z];

  _maxLen := _xLen;
  if (_maxLen < _yLen) then _maxLen := _yLen;
  if (_maxLen < _zLen) then _maxLen := _zLen;

  _xLen := (_maxLen - _xLen) / 2;
  _yLen := (_maxLen - _yLen);
  _zLen := (_maxLen - _zLen) / 2;

  _aabb.min[X] := _aabb.min[X] - _xLen;
  _aabb.max[X] := _aabb.max[X] + _xLen;

  _aabb.max[Y] := _aabb.max[Y] + _yLen;

  _aabb.min[Z] := _aabb.min[Z] - _zLen;
  _aabb.max[Z] := _aabb.max[Z] + _zLen;

  // Now get cube's size and center point
  _size := _maxLen;
  _maxLen := _maxLen / 2;
  _center[X] := _aabb.min[X] + _maxLen;
  _center[Y] := _aabb.min[Y] + _maxLen;
  _center[Z] := _aabb.min[Z] + _maxLen;
end;

// ------------------------------------------------------------------

procedure octGetChildNodeCube(_parent  : POctNode;
                              _childID : longint;
                              var _size   : single;
                              var _center : T_Vec;
                              var _aabb   : TOctAABB);

begin
  // first get node's center
  _size := _parent^.size / 4; // this is the half size of the child node
  with _parent^ do
    case _childID of
      0 : _center := octVec(center[X] + _size, center[Y] + _size, center[Z] + _size);
      1 : _center := octVec(center[X] - _size, center[Y] + _size, center[Z] + _size);
      2 : _center := octVec(center[X] + _size, center[Y] + _size, center[Z] - _size);
      3 : _center := octVec(center[X] - _size, center[Y] + _size, center[Z] - _size);
      4 : _center := octVec(center[X] + _size, center[Y] - _size, center[Z] + _size);
      5 : _center := octVec(center[X] - _size, center[Y] - _size, center[Z] + _size);
      6 : _center := octVec(center[X] + _size, center[Y] - _size, center[Z] - _size);
      7 : _center := octVec(center[X] - _size, center[Y] - _size, center[Z] - _size);
    end;
  // now calculate it's aabb
  _aabb.max := octVecAddS(_center,  _size);
  _aabb.min := octVecAddS(_center, -_size);
  // now get the true size of the node
  _size := _parent^.size / 2;
end;

// ------------------------------------------------------------------

function octCreateItemBuffer(_parent: POctNode; var _child: POctNode;
                             _build: POctBuild): boolean;

var _cnt, _inside : longint;
    _tri : T_Tri;
    _src, _dst : POctItem;
    _bhs : T_Vec;
    _int : longint; // 0 - not inside, 1 - one vertex inside, 2 - two..,
                    // 3 - three ..., 4 - none but it's inside

begin
  Result := false;

  // prepare box-half-size for triBoxOverlap calls
  _bhs := octVec(_child^.size / 2, _child^.size / 2, _child^.size / 2);

  // First check each triangle inside parent node against child node;
  // store which are inside in the work buffer.
  _inside := 0;
  _src := _parent^.itemData;
  _dst := _build^.workBuf;
  for _cnt := 0 to _parent^.itemCnt-1 do
  begin
    // get the triangle
    _build^.getTri(_src^.triIdx, _tri);

    // is the triangle inside child node ?
    _int := octTriInsideAABB(@_tri, @_child.aabb);
    if _int = 0 then
      if overlap_Tri_AABB(@_child.center, @_bhs, @_tri) then _int := 4;

    // it's inside (full or partially) ?
    if (_int > 0) then
    begin
      _dst^.triIdx := _src^.triIdx;
      if (_int = 3)
        then _dst^.flags := C_OCT_IF_INSIDE_NODE // full inside
          else _dst^.flags := 0; // partially inside
      // one more, move cursor
      inc(_inside);
      inc(_dst);
    end;

    // next triangle
    inc(_src);
  end;

  // now if it's needed create a item buffer for the child node and copy
  // the work buffer content on it.
  if (_inside > 0) then
  begin
    try
      // create child's node buffer and copy it's item data
      _child^.itemCnt := _inside;
      GetMem(_child^.itemData, _inside * SizeOf(TOctItem));
      Move(_build^.workBuf^, _child^.itemData^, _inside * SizeOf(TOctItem));
      // this is for sequential access
      _build.prevNode^.next := _child;
      _child^.prev := _build.prevNode;
      _build.prevNode := _child;
    except
      exit;
    end;
  end else
  begin
    // void node! free it
    try
      FreeMem(_child, SizeOf(TOctNode));
      _child := NIL;
    except
    end;
  end;

  // All right
  Result := true;
end;

// ------------------------------------------------------------------

function octBuildChilds(_parent: POctNode; _build: POctBuild;
                        _level: longint): boolean;

var _cnt : longint;

begin
  Result := false;

  // check if we should split this node
  if ((_parent^.itemCnt < _build^.minNodeTri) and (_build^.minNodeTri > 0)) or
     (((_parent^.size / 2) < _build^.minNodeSize) and (_build^.minNodeSize > 0)) or
     ((_level >= _build^.maxLevels) and (_build^.maxLevels > 0)) then
  begin
    Result := true;
    exit;
  end;

  // split
  for _cnt := 0 to 7 do
  begin
    // get memory for the new node
    try
      GetMem(_parent^.child[_cnt], SizeOf(TOctNode));
      FillChar(_parent^.child[_cnt]^, SizeOf(TOctNode), 0);
      _parent^.child[_cnt]^.father := _parent;
      _parent^.child[_cnt]^.sonID := _cnt;
    except
      exit;
    end;

    // get child aabb
    with _parent^.child[_cnt]^
      do octGetChildNodeCube(_parent, _cnt, size, center, aabb);

    // create and fill child's item buffer
    Result := octCreateItemBuffer(_parent, _parent^.child[_cnt], _build);
    if (not Result) then exit; // wops! error! ¿not enough memory?

    // any triangle inside child node ?
    if (_parent^.child[_cnt] <> NIL) then
    begin
      // build childs for this child... xD (recursion... recursion!)
      Result := octBuildChilds(_parent^.child[_cnt], _build, _level + 1);
      if (not Result) then exit; // wops! error! ¿not enough memory?
    end;
  end;

  // free parent's item buffer (since we only need item buffers
  // on terminal nodes)
  try
    FreeMem(_parent^.itemData, _parent^.itemCnt * SizeOf(TOctItem));
    _parent^.itemData := NIL;
    _parent^.itemCnt  := 0;
    _parent^.splited  := true;
  except
  end;

  // All right
end;

// ------------------------------------------------------------------

procedure octFreeNodeBuffers(_n : POctNode);

begin
  // free buffers
  try
    if (_n^.itemData <> NIL) and (_n^.itemCnt > 0) then
      FreeMem(_n^.itemData, _n^.itemCnt * SizeOf(TOctItem));
    if (_n^.usrDataA <> NIL) and (_n^.usrSizeA > 0) then
      FreeMem(_n^.usrDataA, _n^.usrSizeA);
    if (_n^.usrDataB <> NIL) and (_n^.usrSizeB > 0) then
      FreeMem(_n^.usrDataB, _n^.usrSizeB);
    _n^.itemData := NIL;
    _n^.itemCnt  := 0;
    _n^.usrDataA := NIL;
    _n^.usrSizeA := 0;
    _n^.usrDataB := NIL;
    _n^.usrSizeB := 0;
  except
  end;
end;

// ------------------------------------------------------------------

procedure octReset(var _oct: TOctree);

begin
  Fillchar(_oct, SizeOf(TOctree), 0);
end;

// ------------------------------------------------------------------

procedure octFree(var _oct: TOctree);

var _n : POctNode;
    _p : pointer;

begin
  // free child nodes
  _n := _oct.root.next;
  while (_n <> NIL) do
  begin
    octFreeNodeBuffers(_n);

    // free node
    _p := _n;
    _n := _n^.next;
    try
      FreeMem(_p, SizeOf(TOctNode));
    except
    end;
  end;

  // free root
  octFreeNodeBuffers(@_oct.root);
  octReset(_oct);
end;

// ------------------------------------------------------------------

procedure octCountNodes(var _oct: TOctree);

var _n : POctNode;

begin
  _oct.nNodes := 0;
  _oct.nTerm  := 0;

  _n := @_oct.root;
  while (_n <> NIL) do
  begin
    inc(_oct.nNodes);
    if (_n^.ItemCnt > 0) then inc(_oct.nTerm);
    _n := _n^.next;
  end;
end;


// ------------------------------------------------------------------

function octRebuild_IsFullInsideNode(_oct: POctree;
                                     _triIdx: longint): boolean;

// returns if a triangle it's stored inside a node with the flag of
// full inside.

var
  _n : POctNode;
  _i : POctItem;
  _c : longint;

begin
  Result := false;

  // search
  _n := @_oct^.root;
  while _n <> NIL do
  begin
    _i := _n^.itemData;
    if _n^.itemCnt > 0 then
      for _c := 0 to _n^.itemCnt - 1 do
      begin
        if (_i^.triIdx = _triIdx) and
           (_i^.flags and C_OCT_IF_INSIDE_NODE = C_OCT_IF_INSIDE_NODE) then
        begin
          // triangle found full inside a node!!!
          Result := true;
          exit;
        end;
        // next tri
        inc(_i);
      end;

    // next node
    _n := _n^.next;
  end;
end;

// ------------------------------------------------------------------

procedure octRemoveNode(_n: POctNode);

begin
  // unlink
  _n^.father^.child[_n^.sonID] := NIL;
  _n^.prev^.next := _n^.next;  
  if _n^.next <> NIL then _n^.next^.prev := _n^.prev;
  // free mem
  octFreeNodeBuffers(_n);    
  try
    FreeMem(_n, SizeOf(TOctNode));
  except
  end;
end;

// ------------------------------------------------------------------

function octRebuild(_oct: POctree; _build: POctBuild): boolean;

// A triangle cannot be as "full inside" on one node and as
// a "shared" triangle in another node. This can happen if a vertex
// lies in a plane that splits two nodes. This function fix this
// issue (it's a bug fix).

var  
  _n, _p : POctNode;
  _it, _wc : POctItem;
  _c : longint;
  _copy : boolean;
  _copied : longint;

begin
  Result := false;

  _n := @_oct^.root;
  while _n <> NIL do
  begin
    if _n^.itemCnt > 0 then
    begin
      // copy only valid items to the work buffer
      _copied := 0;
      _it := _n^.itemData;
      _wc := _build^.workBuf;
      for _c := 0 to _n^.itemCnt - 1 do
      begin
        _copy := true;
        // if it's a shared triangle, check that it's not full
        // inside another node.
        if _it^.flags and C_OCT_IF_INSIDE_NODE = 0
          then _copy := not octRebuild_IsFullInsideNode(_oct, _it^.triIdx);
        // copy ?
        if (_copy) then
        begin
          move(_it^, _wc^, SizeOf(TOctItem));
          inc(_copied);
          // next
          inc(_wc);
        end;
        // next tri
        inc(_it);
      end;

      // node's item buffer should be rebuilded ?
      if (_copied < _n^.itemCnt) then
      begin
        // free node's item buffer
        try
          FreeMem(_n^.ItemData, _n^.ItemCnt * SizeOf(TOctItem));
          _n^.ItemData := NIL;
        except
        end;
        _n^.ItemCnt := _copied;

        // create a new one ?
        if (_copied > 0) then
        begin
          // get mem
          try
            GetMem(_n^.ItemData, _n^.ItemCnt * SizeOf(TOctItem));
          except
            exit;
          end;

          // copy data
          Move(_build^.workBuf^, _n^.ItemData^, _n^.ItemCnt * SizeOf(TOctItem));
        end else 
        begin
          // node it's empty! remove it!          
          _p := _n;
          _n := _n^.next;
          octRemoveNode(_p);        
          continue;
        end;
      end;
    end;

    // next node
    _n := _n^.next;
  end;

  Result := true;
end;

// ------------------------------------------------------------------

function octBuild(_nTri, _minNodeTri: longint; _minNodeSize: single;
                  _maxLevels: byte; _getTriCB: TOctGetTri): TOctree;

// _nTri        - numbre of triangles in source mesh.
// _minNodeTri  - min. number of triangles inside a node to allow spliting
// _minNodeSize - min. node's cube size (aabb).
// _maxLevels   - max. level of spliting.
// _getTriCB    - callback used to pass the triangles
//                to the octree builder.

// _minNodeTri, _minNodeSize and _maxLevels can be 0 (no limits)
// But not the three at the same time! At least one should be > 0.
// Be careful with these values, do not try to split too much!
// (i.e. _maxLevels = 10).

var _build : TOctBuild;
    _cnt : longint;
    _p : POctItem;

    // -----------------------------------------------------------------

    procedure FreeWorkBuffer;

    begin
      try
        FreeMem(_build.workBuf, Result.root.itemCnt * SizeOf(TOctItem));
      except
      end;
    end;

    // -----------------------------------------------------------------

begin
  Fillchar(Result, SizeOf(Result), 0);

  // check parameters
  if (_nTri < 1) or (not assigned(_getTriCB)) or 
     (_minNodeTri < 0) or (_minNodeSize < 0) or
     ((_minNodeTri = 0) and (_minNodeSize = 0) and  (_maxLevels = 0)) then exit;

  // initialize our build structure
  Fillchar(_build, SizeOf(_build), 0);
  with _build do
  begin
    nTri        := _nTri;
    minNodeTri  := _minNodeTri;
    minNodeSize := _minNodeSize;
    maxLevels   := _maxLevels;
    getTri      := _getTriCB;
  end;

  // get root's node AABB
  with Result.root do octGetRootNodeCube(@_build, size, center, aabb);

  // create buffer for root's items, and another one
  // for working (work buffer)
  try
    // root's buffer
    Result.root.itemCnt := _nTri;
    GetMem(Result.root.itemData, Result.root.itemCnt * SizeOf(TOctItem));
    // work buffer
    GetMem(_build.workBuf, Result.root.itemCnt * SizeOf(TOctItem));
  except
    octFree(Result);
    exit;
  end;

  // fill item buffer for root node
  _p := Result.root.itemData;
  for _cnt := 0 to Result.root.itemCnt-1 do
  begin
    _p^.triIdx := _cnt;
    _p^.flags  := C_OCT_IF_INSIDE_NODE; // all root's tris are full inside! :)
    inc(_p);
  end;

  // build child nodes
  _build.prevNode := @Result.root;
  Result.ok := octBuildChilds(@Result.root, @_build, 1);
  if not Result.ok then
  begin
    FreeWorkBuffer;
    octFree(Result);
    exit;
  end;

  // rebuild nodes - this is a "bug fix"
  Result.ok := octRebuild(@Result, @_build);

  // save some data
  Result.nTri := _nTri;
  octCountNodes(Result);

  // free work buffer
  FreeWorkBuffer;

  // All right ?
  if (not Result.ok) then octFree(Result);
end;

// ------------------------------------------------------------------

end.