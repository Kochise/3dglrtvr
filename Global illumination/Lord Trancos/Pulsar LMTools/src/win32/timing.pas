(*

  DESC:

   TIMING - Timing unit.
   Copyright © 2002 Lord Trancos / Pulsar Studio.
   <lordtrancos@softhome.net>

  API:

   Win32

*)


unit timing;

// --------------------------------------------------------------------------

interface

uses Windows;

const
  C_TIM_RES = 10000;   // timElapsedTime resolution is 1/10000 sec

var
  G_TIM_FREQ : Int64;

function timGetTickCount: int64;
function timElapsedTime(_tim1, _tim2: int64): longint;

// --------------------------------------------------------------------------

implementation

// --------------------------------------------------------------------------

procedure timInit;

begin
  QueryPerformanceFrequency(G_TIM_FREQ);
end;

// --------------------------------------------------------------------------

function timGetTickCount: int64;

begin
  QueryPerformanceCounter(Result);
end;

// --------------------------------------------------------------------------

function timElapsedTime(_tim1, _tim2: int64): longint;

begin
  Result := ((_tim2 - _tim1) * C_TIM_RES) div G_TIM_FREQ;
end;

// --------------------------------------------------------------------------

initialization

timInit;

// --------------------------------------------------------------------------

finalization

// --------------------------------------------------------------------------

end.
