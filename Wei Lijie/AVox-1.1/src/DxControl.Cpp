#include "DxControl.H"
#include "Log.H"

HRESULT HR;

// DirectInput
DirectInput::DirectInput()
{
	_RELEASE_ (Keyboard.Device);
	_RELEASE_ (LPDI);
	
	Keyboard.Use = false;
	Keyboard.Active = false;
}

// DirectInput
DirectInput::~DirectInput()
{
}

// Create Inputÿ
int DirectInput::CreateInput(HINSTANCE hInst)
{
	if(FAILED(HR = DirectInput8Create (hInst, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&LPDI, 0))) return HR;
	return 0;
}


int DirectInput::CreateKeyboard(HWND hWnd)
{
	if(FAILED(HR = LPDI->CreateDevice (GUID_SysKeyboard, &Keyboard.Device, 0))) return HR;
	if(FAILED(HR = Keyboard.Device->SetDataFormat (&c_dfDIKeyboard))) return HR;
	if(FAILED(HR = Keyboard.Device->SetCooperativeLevel (hWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))) return HR;
	
	Keyboard.Use = true;
	Keyboard.Active = true;
	
	Log.Write("Black","DirectX8 Keyboard: Greated");
	return 0;
}

int DirectInput::CreateMouse(HWND hWnd)
{
	if(FAILED(HR = LPDI->CreateDevice (GUID_SysMouse, &Mouse.Device, 0))) return HR;
	if(FAILED(HR = Mouse.Device->SetDataFormat (&c_dfDIMouse))) return HR;
	if(FAILED(HR = Mouse.Device->SetCooperativeLevel (hWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))) return HR;

	DIPROPDWORD MProp;
	MProp.diph.dwSize = sizeof (DIPROPDWORD);
	MProp.diph.dwHeaderSize = sizeof (DIPROPHEADER);
	MProp.diph.dwObj = 0;
	MProp.diph.dwHow = DIPH_DEVICE;
	MProp.dwData = 64;
	if(FAILED(HR = Mouse.Device->SetProperty (DIPROP_BUFFERSIZE, &MProp.diph))) return HR;
		
	Mouse.Use = true;
	Mouse.Active = true;
	
	return 0;
}

// Obtains access to the input device
void DirectInput::Accure()
{
	if(Keyboard.Device) Keyboard.Device->Acquire();
	if(Mouse.Device) Mouse.Device->Acquire();
}

// Releases access to the device
void DirectInput::UnAccure()
{
	if(Keyboard.Device) Keyboard.Device->Unacquire();
	if(Mouse.Device) Mouse.Device->Unacquire();
}

void DirectInput::UpDate()
{
	if(Keyboard.Use)
	if(Keyboard.Active) Keyboard.UpDate ();

	if(Mouse.Use)
	if(Mouse.Active) Mouse.UpDate ();
}

void DirectInput::Destroy()
{
	UnAccure();
	_RELEASE_ (Mouse.Device);
	_RELEASE_ (Keyboard.Device);
	_RELEASE_ (LPDI);
	
	Mouse.Use = false;
	Mouse.Active = false;
	Keyboard.Use = false;
	Keyboard.Active = false;
}

// Keyboard Update
void DirectInput::DirectKeybord::UpDate()
{
	if(FAILED(HR = Device->GetDeviceState (sizeof(Keys), (LPVOID)&Keys)))
	if(HR == DIERR_INPUTLOST) Device->Acquire ();
}

bool DirectInput::DirectKeybord::isButton(DWORD Index)
{
	if(Keys[Index]&0x80) return true;
	return false;
}

bool DirectInput::DirectKeybord::isAnyButton()
{	
	register int Index;
	for(Index=0; Index<256; Index++) if(Keys[Index]&0x80) return true;
	return false;
}

// Mouse set data
void DirectInput::DirectMouse::SetData(int Xp, int Yp, int Zp, int xMin, int yMin, int zMin, int xMax, int yMax, int zMax, int mSpeed)
{
	Buttons[0]=Buttons[1]=Buttons[3]=0;
	
	CurPos[0] = Xp;
	CurPos[1] = Yp;
	CurPos[2] = Zp;
	
	PosRange[0] = xMin;
	PosRange[1] = yMin;
	PosRange[2] = zMin;
	PosRange[3] = xMax;
	PosRange[4] = yMax;
	PosRange[5] = zMax;
	
	Speed=mSpeed;
}

// Mouse updateøè
void DirectInput::DirectMouse::UpDate()
{
	memcpy(OldPos, CurPos, sizeof(CurPos));
	ZeroMemory(Buttons, sizeof(Buttons));
	ZeroMemory(DeltaPos, sizeof(DeltaPos));

	bool Done = FALSE;
	while(!Done)
	{
		Elem = 1;
		if(FAILED(HR = Device->GetDeviceData (sizeof(Data), &Data, &Elem, 0)))
		if(HR == DIERR_INPUTLOST) Device->Acquire ();

		if(Elem==1)
		{
			switch(Data.dwOfs)
			{
			case DIMOFS_X:
			case DIMOFS_Y:
			case DIMOFS_Z:
			{
				int Axis = Data.dwOfs >> 2;

				DeltaPos[Axis]+=Data.dwData*Speed;
				CurPos[Axis]+=DeltaPos[Axis];

				if(CurPos[Axis] < PosRange[Axis]) CurPos[Axis] = PosRange[Axis];
				else if(CurPos[Axis] > PosRange[Axis+3]) CurPos[Axis] = PosRange[Axis+3];
			}
			break;

			case DIMOFS_BUTTON0:
			case DIMOFS_BUTTON1:
			case DIMOFS_BUTTON2:
			case DIMOFS_BUTTON3:
			case DIMOFS_BUTTON4:
			case DIMOFS_BUTTON5:
			case DIMOFS_BUTTON6:
			case DIMOFS_BUTTON7:
			{
				int Butn = Data.dwOfs - DIMOFS_BUTTON0;
				if(BtnClick[Butn]) BtnClick[Butn] = false, Buttons[Butn] = 2;
				else BtnClick[Butn] = true, Buttons[Butn] = 1;
			}
			break;
			}
		}
		else if(Elem == 0) Done = true;
	}
	Move = ((DeltaPos[xAxis] != 0) || (DeltaPos[yAxis] != 0)) ? true : false;
}

// check mouse button pressøè 
bool DirectInput::DirectMouse::isAnyPressed()
{
	register int Index;
	for(Index=0; Index<8; Index++) if(BtnClick[Index]) return true;
	return false;
}

// mouse is active or not
bool DirectInput::DirectMouse::isActive (void)
{ 
	return Active; 
}
// mouse get current position
int DirectInput::DirectMouse::GetCurPos (int Axis)
{ 
	return CurPos[Axis]; 
}
// mouse get delta position
int DirectInput::DirectMouse::GetDeltaPos (int Axis)
{ 
	return DeltaPos[Axis]; 
}
// mouse is move or not			
bool DirectInput::DirectMouse::isMove (void)
{ 
	return Move; 
}

bool DirectInput::DirectMouse::isPressed (int But)
{ 
	return BtnClick[But]; 
}

bool DirectInput::DirectMouse::isJustPressed (int But)
{ 
	return Buttons[But]==1 ? true : false; 
}

bool DirectInput::DirectMouse::isJustUnPressed (int But)
{ 
	return Buttons[But]==2 ? true : false; 
}
