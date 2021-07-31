unit lmdb;

// ---------------------------------------------------------------------------

interface

const
  C_LMDB_ID  = $42444D4C;
  C_LMDB_VER = 1;

type
  TLMDB_Header = packed record
    ID          : cardinal;
    Ver         : cardinal;
    nLightmaps  : cardinal;
    MaxLMapSize : word;
  end;

// ---------------------------------------------------------------------------

implementation

// ---------------------------------------------------------------------------

end.
