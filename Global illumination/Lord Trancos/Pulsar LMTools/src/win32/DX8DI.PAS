(*

  DESC:

   DX8DI - Pulsar Studio DirectInput 8 unit.
   Copyright © 2001-2002 Lord Trancos / Pulsar Studio.
   <lordtrancos@softhome.net>

  API:

   Win32, DirectX 8.0

*)

unit dx8di;

// *******************************************************************

interface

uses Windows, DirectInput8;

type
  TTimedInput = record
                  Time : dWord;
                  Used : boolean;
                end;

var
  DI8  : IDirectInput8 = NIL;       // DirectInput Interface.
  DIK8 : IDirectInputDevice8 = NIL; // Keyboard Interface.
  DIM8 : IDirectInputDevice8 = NIL; // Mouse Interface.
  DIJ8 : IDirectInputDevice8 = NIL; // Joystick Interface.

  // Mouse.
  DIMEvent : THandle;  // Mouse event.
  DIMou0Clicked  : boolean = false; // Mouse button 0 is pressed ?
  DIMou1Clicked  : boolean = false; // Mouse button 1 is pressed ?

  // Joystick.
  DIJ8Caps : TDIDevCaps;

// DirectInput main functions.
function  DIInit: boolean;
procedure DIClose;

// Timed inputs.
procedure TimedInputReset(var _ti: TTimedInput);
function  TimedInputRefresh(var _ti: TTimedInput;
                            _time, _timeout: dWord;
                            _condition: boolean): longint;

// Mouse.
function  DIInitMouse(_hWnd: hWnd): boolean;
function  DIMouseControl(_acquire: boolean): boolean;
function  DIGetMouseState(var _x, _y: longint; var _0u, _0d, _0z,
                          _1u, _1d, _1z: longint): boolean;

// Joystick.
function  DIInitJoystick(_hWnd: hWnd): boolean;
function  DIJoystickControl(_acquire: boolean): boolean;
function  DIGetJoystickState(var _data: TDIJoyState2): boolean;

// Keyboard
function  DIInitKeyboard(_hWnd: hWnd): boolean;
function  DIKeyboardControl(_acquire: boolean): boolean;
function  DIGetKeyboardState: boolean;
function  DIKeyDown(_key: byte): boolean;

// *******************************************************************

implementation

const
  // Mouse.
  DIMBufSize = 16;   // #Events stored in mouse's buffer.
  DIMTimeOut = 250;  // Timeout for double-click.

  // Joystick.
  DIJoyRange = 32768;

var
  // Mouse.
  DIMButSwapped : boolean = false; // Mouse buttons swapped ?
  DIM0Released  : dWord = 0; // Last time that button 0 was released.
  DIM1Released  : dWord = 0; // Last time that button 1 was released.

  // Keyboard.
  DIKeyBuffer   : array[0..255] of byte;  // Keyboard buffer.

// *******************************************************************
// Name : DIClose.
// Desc : Closes keyboard interface and then DirectInput.
// *******************************************************************

procedure DIClose;

begin
  if Assigned(DI8) then
    begin
      if Assigned(DIK8) then // Close Keyboard interface.
        begin
          DIK8.Unacquire;
          DIK8 := NIL;
        end;
      if Assigned(DIM8) then // Close Mouse interface.
        begin
          DIM8.Unacquire;
          DIM8 := NIL;
        end;
      if Assigned(DIJ8) then // Close Joystick interface.
        begin
          DIJ8.Unacquire;
          DIJ8 := NIL;
        end;
      DI8 := NIL; // Close DirectInput interface.
    end;
end;

// *******************************************************************
// Name : DIInit.
// Desc : Creates a DirectInput 8 Interface.
// *******************************************************************

function DIInit: boolean;

begin
  // Close DirectSound (if was already initialized).
  DIClose;

  // Create DirectInput Interface.
  Result := not failed(DirectInput8Create(hInstance,
                       DIRECTINPUT_VERSION,
                       IID_IDirectInput8, DI8, NIL));
end;

// *******************************************************************
// Name : TimedInputReset.
// Desc : Resets a TTimedInput var.
// *******************************************************************

procedure TimedInputReset(var _ti: TTimedInput);

begin
  _ti.Time := 0;
  _ti.Used := false;
end;

// *******************************************************************
// Name : TimedInputRefresh.
// Desc : Updates a TTimedInput and returns the last number of inputs.
// *******************************************************************

function TimedInputRefresh(var _ti: TTimedInput;
                           _time, _timeout: dWord;
                           _condition: boolean): longint;

begin
  Result := 0;
  if (_ti.Time = 0) and (_condition) then
    begin _ti.Time := _time; exit; end;
  if (_ti.Time <> 0) then
    if (_condition) then
      begin
        if (_time - _ti.Time >= _timeout) then
          begin
            _ti.Used := true;
            while (_time - _ti.Time >= _timeout) do
              begin
                inc(Result);
                inc(_ti.Time, _timeout)
              end;
          end;
      end else begin
                 if _ti.Used = false then inc(Result);
                 _ti.Used := false;
                 _ti.Time := 0;
               end;
end;

// *******************************************************************
//
//                **   **  ****  *    *  ****  *****
//                * * * * *    * *    * *      *
//                *  *  * *    * *    *  ****  ****
//                *     * *    * *    *      * *
//                *     *  ****   ****   ****  *****
//
// *******************************************************************

// *******************************************************************
// Name : DIInitMouse
// Desc : Creates a IDirectInputDevice interface for the mouse,
//        sets data format, cooperative level,...
// *******************************************************************

function  DIInitMouse(_hWnd: hWnd): boolean;

var _prop : TDIPropDWord;

begin
  Result := false;

  // Are mouse buttons swapped ?
  DIMButSwapped := GetSystemMetrics(SM_SWAPBUTTON) <> 0;

  // Create a IDirectInputDevice interface for the Mouse.
  if failed(DI8.CreateDevice(GUID_SysMouse, DIM8, NIL)) then exit;

  // Set the device's data format.
  if failed(DIM8.SetDataFormat(@c_dfDIMouse)) then exit;

  // Set the cooperative level.
  if failed(DIM8.SetCooperativeLevel(_hWnd, DISCL_FOREGROUND or
                                     DISCL_EXCLUSIVE)) then exit;

  // Create a event for the mouse.
  DIMEvent := CreateEvent(NIL, false, false, NIL);
  if DIMEvent = 0 then exit;

  // Assign event.
  if failed(DIM8.SetEventNotification(DIMEvent)) then exit;

  // Set buffer's description.
  with _prop do begin
    diph.dwSize       := SizeOf(TDIPropDWord);
    diph.dwHeaderSize := SizeOf(TDIPropHeader);
    diph.dwObj        := 0;
    diph.dwHow        := DIPH_DEVICE;
    dwData            := DIMBufSize;
  end;

  // Assign buffer.
  if failed(DIM8.SetProperty(DIPROP_BUFFERSIZE, _prop.diph)) then exit;

  // All right.
  Result := true;
end;

// *******************************************************************
// Name : DIMouseControl.
// Desc : Obtains or releases access to the mouse.
// *******************************************************************

function  DIMouseControl(_acquire: boolean): boolean;

var _hr : hResult;

begin
  if _acquire = false
    then _hr := DIM8.Unacquire
      else _hr := DIM8.Acquire;
  Result := not failed(_hr);
end;

// *******************************************************************
// Name : DIGetMouseState.
// Desc : Returns the variation in the position of the mouse (_x, _y),
//        the times that a button was pressed (d) or released (u),
//        and the number of double clicks (z). This function also
//        modifies the values of DIMouLClicked and DIMouRClicked.
// *******************************************************************

function  DIGetMouseState(var _x, _y: longint; var _0u, _0d, _0z,
                          _1u, _1d, _1z: longint): boolean;

var _hr : hResult;
    _od : TDIDeviceObjectData;
    _0, _1 : boolean;
    _time : dWord;
    _elements : dWord;

begin
  Result := false;

  _x  := 0; _y  := 0;
  _0u := 0; _0d := 0;
  _1u := 0; _1d := 0;
  _0z := 0; _1z := 0;

  // Read event by event.
  repeat
    _elements := 1;
    _hr := DIM8.GetDeviceData(SizeOf(TDIDeviceObjectData),
                              @_od, _elements, 0);
    if _hr = DIERR_INPUTLOST then
      begin
        _hr := DIM8.Acquire;
        if not failed(_hr)
          then _hr := DIM8.GetDeviceData(SizeOf(TDIDeviceObjectData),
                                         @_od, _elements, 0);
      end;
    if (failed(_hr)) then exit;
    Result := true;
    if (_elements = 0) then exit;

    // Analize event data.
    _0 := false;
    _1 := false;
    case _od.dwOfs of
      DIMOFS_X : _x := _x + longint(_od.dwData);
      DIMOFS_Y : _y := _y + longint(_od.dwData);
      DIMOFS_BUTTON0 : if DIMButSwapped
                         then _1 := true else _0 := true;
      DIMOFS_BUTTON1 : if DIMButSwapped
                         then _0 := true else _1 := true;
    end;

    // Button 0 clicked or released ?
    if _0 = true then
      begin
        DIMou0Clicked := (_od.dwData and $80 = $80);
        if not DIMou0Clicked then
          begin
            inc(_0u);
            // Double-click check
            _time := GetTickCount;
            if (_time - DIM0Released < DIMTimeOut) then
              begin
                DIM0Released := 0;
                inc(_0z);
              end else DIM0Released := _time;
          end else inc(_0d);
      end;

    // Button 1 clicked or released ?
    if _1 = true then
      begin
        DIMou1Clicked := (_od.dwData and $80 = $80);
        if not DIMou1Clicked then
          begin
            inc(_1u);
            // Double-click check
            _time := GetTickCount;
            if (_time - DIM1Released < DIMTimeOut) then
              begin
                DIM1Released := 0;
                inc(_1z);
              end else DIM1Released := _time;
          end else inc(_1d);
      end;
  until _elements = 0;
end;

// *******************************************************************
//
//               *  ****  *   *  **** ***** ***  **** *  *
//               * *    *  * *  *       *    *  *     * *
//               * *    *   *    ****   *    *  *     **
//          *    * *    *   *        *  *    *  *     * *
//           ****   ****    *    ****   *   ***  **** *  *
//
// *******************************************************************

// *******************************************************************
//  Callbacks for DIInitJoystick function.
// *******************************************************************

function EnumJoysticksCallback(_didi: PDIDeviceInstance;
                               _ref: pointer): integer; stdcall;

begin
  Result := DIENUM_CONTINUE;
  if failed(DI8.CreateDevice(_didi.guidInstance, DIJ8, NIL)) then exit;
  Result := DIENUM_STOP;
end;

// ********************************

function EnumAxesCallback(var _didoi : TDIDeviceObjectInstance;
                          _ref: pointer): integer; stdcall;

var _diprg : TDIPropRange;

begin
  Result := DIENUM_CONTINUE;

  _diprg.diph.dwSize       := sizeof(TDIPropRange);
  _diprg.diph.dwHeaderSize := sizeof(TDIPropHeader);
  _diprg.diph.dwHow        := DIPH_BYID;
  _diprg.diph.dwObj        := _didoi.dwType;
  _diprg.lMin              := -DIJoyRange;
  _diprg.lMax              := DIJoyRange;

  if failed(DIJ8.SetProperty(DIPROP_RANGE, _diprg.diph))
    then Result := DIENUM_STOP;
end;

// *******************************************************************
// Name : DIInitJoystick
// Desc : Creates a IDirectInputDevice interface for the first game
//        control found, sets data format and cooperative level,...
// *******************************************************************

function  DIInitJoystick(_hWnd: hWnd): boolean;

begin
  Result := false;

  // Find joystick and create the device.
  DI8.EnumDevices(DI8DEVCLASS_GAMECTRL, @EnumJoysticksCallback,
                  NIL, DIEDFL_ATTACHEDONLY);
  if DIJ8 = NIL then exit;

  // Set joystick Data Format.
  if failed(DIJ8.SetDataFormat(@c_dfDIJoystick2)) then
    begin DIJ8 := NIL; exit; end;

  // Set joystick cooperative level.
  if failed(DIJ8.SetCooperativeLevel(_hWnd,
            DISCL_EXCLUSIVE or DISCL_FOREGROUND)) then
    begin DIJ8 := NIL; exit; end;

  // Get joystick capabilities.
  DIJ8CAPS.dwSize := sizeof(TDIDevCaps);
  if failed(DIJ8.GetCapabilities(DIJ8CAPS)) then
    begin DIJ8 := NIL; exit; end;

  // Customize the device properties.
  if failed(DIJ8.EnumObjects(EnumAxesCallback,
                             NIL, DIDFT_AXIS)) then
    begin DIJ8 := NIL; exit; end;

  // All right.
  Result := true;
end;

// *******************************************************************
// Name : DIJoystickControl.
// Desc : Obtains or releases access to the joystick.
// *******************************************************************

function DIJoystickControl(_acquire: boolean): boolean;

var _hr : hResult;

begin
  if _acquire = false
    then _hr := DIJ8.Unacquire
      else _hr := DIJ8.Acquire;
  Result := not failed(_hr);
end;

// *******************************************************************
// Name : DIGetJoystickState
// Desc : Returns joystick data.
// *******************************************************************

function DIGetJoystickState(var _data: TDIJoyState2): boolean;

begin
  Result := false;

  // Poll the device to read the current state.
  if failed(DIJ8.Poll) then
    if DIJ8.Acquire = DIERR_INPUTLOST then
      if failed(DIJ8.Acquire) then exit;

  // Retrieve data.
  Result := not failed(DIJ8.GetDeviceState(sizeof(TDIJoyState2),
                       @_data));
end;

// *******************************************************************
//
//          *  * ***** *   * ****   ****   ****  ****  ****
//          * *  *      * *  *   * *    * *    * *   * *   *
//          **   ****    *   ****  *    * ****** ****  *   *
//          * *  *       *   *   * *    * *    * *   * *   *
//          *  * *****   *   ****   ****  *    * *   * ****
//
// *******************************************************************

// *******************************************************************
// Name : DIInitKeyboard.
// Desc : Creates a IDirectInputDevice interface for the Keyboard,
//        sets data format, cooperative level,...
// *******************************************************************

function DIInitKeyboard(_hWnd: hWnd): boolean;

begin
  Result := false;

  // Create a IDirectInputDevice interface for the Keyboard.
  if failed(DI8.CreateDevice(GUID_SysKeyboard, DIK8, NIL)) then exit;

  // Set the device's data format.
  if failed(DIK8.SetDataFormat(@c_dfDIKeyboard)) then exit;

  // Set the cooperative level.
  if failed(DIK8.SetCooperativeLevel(_hWnd, DISCL_FOREGROUND or
                                     DISCL_NONEXCLUSIVE)) then exit;

  // All right.
  Result := true;
end;

// *******************************************************************
// Name : DIKeyboardControl.
// Desc : Obtains or releases access to the keyboard.
// *******************************************************************

function DIKeyboardControl(_acquire: boolean): boolean;

var _hr : hResult;

begin
  if _acquire = false
    then _hr := DIK8.Unacquire
      else _hr := DIK8.Acquire;
  Result := not failed(_hr);
end;

// *******************************************************************
// Name : DIGetKeyboardState.
// Desc : Fills the keyboard buffer with the key states.
// *******************************************************************

function DIGetKeyboardState: boolean;

var _hr : hResult;

begin
  // Get key states.
  _hr := DIK8.GetDeviceState(SizeOf(DIKeyBuffer), @DIKeyBuffer);

  // If input was lost, re-acquire.
  if _hr = DIERR_INPUTLOST then
    begin
      _hr := DIK8.Acquire;
      if not failed(_hr)
        then _hr := DIK8.GetDeviceState(SizeOf(DIKeyBuffer),
                                        @DIKeyBuffer);
    end;

  // All right ?
  Result := not failed(_hr);
end;

// *******************************************************************
// Name : DIKeyDown.
// Desc : Reads a key state on the keyboard buffer.
//        (DIGetKeyboardState must be called first.)
// *******************************************************************

function DIKeyDown(_key: byte): boolean;

begin
  Result := (DIKeyBuffer[_key] and $80 = $80);
end;

// *******************************************************************

begin
  ZeroMemory(@DIKeyBuffer, SizeOf(DIKeyBuffer));
end.
