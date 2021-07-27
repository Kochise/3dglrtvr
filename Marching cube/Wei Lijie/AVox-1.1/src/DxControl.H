#ifndef __DX_CONTROL_H
#define __DX_CONTROL_H

#define _RELEASE_(Ptr) { if(Ptr) { (Ptr)->Release(); (Ptr)=NULL; };};
#define _DELETE_ (Ptr)  { if(Ptr) { delete (Ptr); (Ptr)=NULL; };};

#define xAxis	0
#define yAxis	1

#pragma comment(lib, "DxGuid.Lib")			// 
#pragma comment(lib, "DInput8.Lib")

#include <DInput.H>							// 

class DirectInput
{
private:
	LPDIRECTINPUT8	LPDI;					// 
		
public:                    
	DirectInput();
	~DirectInput();

	int CreateInput(HINSTANCE);
	int CreateKeyboard(HWND);
	int CreateMouse(HWND);

	void Destroy();
	void UpDate();
	void Accure();
	void UnAccure();  

	class DirectKeybord 
	{
		friend class DirectInput;
		private:
			LPDIRECTINPUTDEVICE8 Device;
			bool Active;
			bool Use;
			unsigned char Keys[256];

			void UpDate();
		public:
			bool isActive() 
			{
				return Active;
			}
			bool isButton(DWORD);
			bool isAnyButton();
	} Keyboard;

	class DirectMouse
	{
		friend class DirectInput;
		private:
			LPDIRECTINPUTDEVICE8 Device;	// 
			DIDEVICEOBJECTDATA Data;		// 
			bool Active;					// 
			bool Use;						// 
			bool Move;						// 
			DWORD Elem;						// 
			int Speed;						// 
			int Buttons[8];					// 
			bool BtnClick[8];				// 
			int CurPos[3];					// 
			int OldPos[3];					// 
			int DeltaPos[3];				// 
			int PosRange[6];				// 
			
			void UpDate();					// 
		public:
			void SetData (int, int, int, int, int, int, int, int, int, int);
			// 
			bool isActive (void); 
			int GetCurPos (int Axis);		// 
			int GetDeltaPos (int Axis);		// 
			bool isMove (void);				// 
			bool isPressed (int But);		// 
			bool isJustPressed (int But);	// 
			bool isJustUnPressed (int But);	// 
			bool isAnyPressed (void);		// 

	} Mouse;
};

#endif

