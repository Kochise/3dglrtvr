/*
 * Input control header
 * 
 * This file is part of the "SoftPixel Engine" (Copyright (c) 2008 by Lukas Hermanns)
 * See "SoftPixelEngine.hpp" for license information.
 */

#ifndef __SP_INPUTOUTPUT_CONTROL_H__
#define __SP_INPUTOUTPUT_CONTROL_H__


#include "Base/spStandard.hpp"
#include "Base/spDimensionPoint2D.hpp"
#include "Base/spDimensionVector3D.hpp"
#include "Base/spInputOutputLog.hpp"

#include <math.h>

#if defined(SP_PLATFORM_LINUX)
#   include <X11/X.h>
#   include <X11/Xlib.h>
#   include <X11/Xutil.h>
#elif defined(SP_PLATFORM_ANDROID)
#   include <android/input.h>
#   include "Platform/Android/android_native_app_glue.h"
#endif


namespace sp
{

class SoftPixelDevice;

#if defined(SP_PLATFORM_LINUX)
class SoftPixelDeviceLinux;
#elif defined(SP_PLATFORM_ANDROID)
class SoftPixelDeviceAndroid;
#elif defined(SP_PLATFORM_IOS)
class SoftPixelDeviceIOS;
#endif

namespace io
{


#if defined(SP_PLATFORM_WINDOWS) || defined(SP_PLATFORM_LINUX)

/*
 * Macros
 */

#define __SP_WORDSIZE   32767.5f // 65536

#define __SP_IGNORE_CURSORPOS   10000


/*
 * Enumerations
 */

enum EMouseKeyCodes
{
    MOUSE_LEFT = 0,
    MOUSE_MIDDLE,
    MOUSE_RIGHT,
};

enum EKeyCodes
{
    KEY_LBUTTON     = 0x01, // Left mouse button
    KEY_RBUTTON     = 0x02, // Right mouse button
    KEY_CANCEL      = 0x03, // Control-break processing
    KEY_MBUTTON     = 0x04, // Middle mouse button (three-button mouse)
    KEY_XBUTTON1    = 0x05, // Windows 2000/XP: X1 mouse button
    KEY_XBUTTON2    = 0x06, // Windows 2000/XP: X2 mouse button
    KEY_BACK        = 0x08, // BACKSPACE key
    KEY_TAB         = 0x09, // TAB key
    KEY_CLEAR       = 0x0C, // CLEAR key
    KEY_RETURN      = 0x0D, // ENTER key
    KEY_SHIFT       = 0x10, // SHIFT key
    KEY_CONTROL     = 0x11, // CTRL key
    KEY_MENU        = 0x12, // ALT key
    KEY_PAUSE       = 0x13, // PAUSE key
    KEY_CAPITAL     = 0x14, // CAPS LOCK key
    KEY_KANA        = 0x15, // IME Kana mode
    KEY_HANGUEL     = 0x15, // IME Hanguel mode
    KEY_HANGUL      = 0x15, // IME Hangul mode
    KEY_JUNJA       = 0x17, // IME Junja mode
    KEY_FINAL       = 0x18, // IME final mode
    KEY_HANJA       = 0x19, // IME Hanja mode
    KEY_KANJI       = 0x19, // IME Kanji mode
    
    KEY_ESCAPE      = 0x1B,
    KEY_SPACE       = 0x20,
    KEY_PAGEUP      = 0x21,
    KEY_PAGEDOWN    = 0x22,
    KEY_END         = 0x23,
    KEY_HOME        = 0x24,
    KEY_LEFT        = 0x25, // left arrow key
    KEY_UP          = 0x26, // up arrow key
    KEY_RIGHT       = 0x27, // right arrow key
    KEY_DOWN        = 0x28, // down arrow key
    KEY_SELECT      = 0x29,
    KEY_EXE         = 0x2B, // execute key
    KEY_SNAPSHOT    = 0x2C,
    KEY_INSERT      = 0x2D,
    KEY_DELETE      = 0x2E,
    KEY_HELP        = 0x2F,
    
    KEY_0           = 0x30,
    KEY_1           = 0x31,
    KEY_2           = 0x32,
    KEY_3           = 0x33,
    KEY_4           = 0x34,
    KEY_5           = 0x35,
    KEY_6           = 0x36,
    KEY_7           = 0x37,
    KEY_8           = 0x38,
    KEY_9           = 0x39,
    
    KEY_A           = 0x41,
    KEY_B           = 0x42,
    KEY_C           = 0x43,
    KEY_D           = 0x44,
    KEY_E           = 0x45,
    KEY_F           = 0x46,
    KEY_G           = 0x47,
    KEY_H           = 0x48,
    KEY_I           = 0x49,
    KEY_J           = 0x4A,
    KEY_K           = 0x4B,
    KEY_L           = 0x4C,
    KEY_M           = 0x4D,
    KEY_N           = 0x4E,
    KEY_O           = 0x4F,
    KEY_P           = 0x50,
    KEY_Q           = 0x51,
    KEY_R           = 0x52,
    KEY_S           = 0x53,
    KEY_T           = 0x54,
    KEY_U           = 0x55,
    KEY_V           = 0x56,
    KEY_W           = 0x57,
    KEY_X           = 0x58,
    KEY_Y           = 0x59,
    KEY_Z           = 0x5A,
    
    KEY_WINLEFT     = 0x5B,
    KEY_WINRIGHT    = 0x5C,
    KEY_APPS        = 0x5D,
    
    KEY_NUMPAD0     = 0x60,
    KEY_NUMPAD1     = 0x61,
    KEY_NUMPAD2     = 0x62,
    KEY_NUMPAD3     = 0x63,
    KEY_NUMPAD4     = 0x64,
    KEY_NUMPAD5     = 0x65,
    KEY_NUMPAD6     = 0x66,
    KEY_NUMPAD7     = 0x67,
    KEY_NUMPAD8     = 0x68,
    KEY_NUMPAD9     = 0x69,
    
    KEY_MULTIPLY    = 0x6A,
    KEY_ADD         = 0x6B,
    KEY_SEPARATOR   = 0x6C,
    KEY_SUBTRACT    = 0x6D,
    KEY_DECIMAL     = 0x6E,
    KEY_DIVIDE      = 0x6F,
    
    KEY_F1          = 0x70,
    KEY_F2          = 0x71,
    KEY_F3          = 0x72,
    KEY_F4          = 0x73,
    KEY_F5          = 0x74,
    KEY_F6          = 0x75,
    KEY_F7          = 0x76,
    KEY_F8          = 0x77,
    KEY_F9          = 0x78,
    KEY_F10         = 0x79,
    KEY_F11         = 0x7A,
    KEY_F12         = 0x7B,
    KEY_F13         = 0x7C,
    KEY_F14         = 0x7D,
    KEY_F15         = 0x7E,
    KEY_F16         = 0x7F,
    KEY_F17         = 0x80,
    KEY_F18         = 0x81,
    KEY_F19         = 0x82,
    KEY_F20         = 0x83,
    KEY_F21         = 0x84,
    KEY_F22         = 0x85,
    KEY_F23         = 0x86,
    KEY_F24         = 0x87,
    
    KEY_NUMLOCK     = 0x90,
    KEY_SCROLL      = 0x91,
    
    KEY_LSHIFT      = 0xA0,
    KEY_RSHIFT      = 0xA1,
    KEY_LCONTROL    = 0xA2,
    KEY_RCONTROL    = 0xA3,
    KEY_LMENU       = 0xA4,
    KEY_RMENU       = 0xA5,
    
    KEY_PLUS        = 0xBB, // '+'
    KEY_COMMA       = 0xBC, // ','
    KEY_MINUS       = 0xBD, // '-'
    KEY_PERIOD      = 0xBE, // '.'
    
    KEY_EXPONENT    = 0xDC, // '^'
    
    KEY_ATTN        = 0xF6,
    KEY_CRSEL       = 0xF7,
    KEY_EXSEL       = 0xF8,
    KEY_EREOF       = 0xF9,
    KEY_PLAY        = 0xFA,
    KEY_ZOOM        = 0xFB,
    KEY_NONAME      = 0xFC,
    KEY_PA1         = 0xFD,
    KEY_OEMCLEAR    = 0xFE
};

enum EJoystickKeyCodes
{
    JOYSTICK_KEY1 = 1,
    JOYSTICK_KEY2,
    JOYSTICK_KEY3,
    JOYSTICK_KEY4,
    JOYSTICK_KEY5,
    JOYSTICK_KEY6,
    JOYSTICK_KEY7,
    JOYSTICK_KEY8,
    JOYSTICK_KEY9,
    JOYSTICK_KEY10,
    JOYSTICK_KEY11,
    JOYSTICK_KEY12,
    JOYSTICK_KEY13,
    JOYSTICK_KEY14,
    JOYSTICK_KEY15,
    JOYSTICK_KEY16,
};


//! User input control interface for PC operating-systems (Windows, Linux etc.). Keyboard, mouse and joystick input examination.
class SP_EXPORT InputControl
{
    
    public:
        
        InputControl();
        ~InputControl();
        
        //! Puts the cursor at the specified position.
        void setCursorPosition(const dim::point2di &Position, bool UpdateCursorSpeed = true);
        dim::point2di getCursorPosition() const;
        
        //! \return Cursor motion speed.
        dim::point2di getCursorSpeed();
        
        /**
        \return True if the specified key is pressed otherwise False. The return value is a reference, thus you
        can manipulate this value if you want the further keyboard-checks will or will not be effected
        after pressing this key.
        */
        bool& keyDown(const EKeyCodes KeyCode);     // Key pressed
        bool& keyHit(const EKeyCodes KeyCode);      // Key hit
        bool& keyReleased(const EKeyCodes KeyCode); // Key released
        
        //! Simulates a key press (down or release). This effects the whole system!
        void keyDownSimulation(const EKeyCodes KeyCode) const;
        void keyReleasedSimulation(const EKeyCodes KeyCode) const;
        
        /**
        Similiar to "keyDown" but supports extended keys like "Left Control"/ "Right Control" or
        "Left Shift"/ "Right Shift" etc.
        */
        bool keyDownEx(const EKeyCodes KeyCode) const;  // Extendet key pressed method (e.g. for LShift/ RShift)
        
        //! \return True if the specified mouse button is pressed otherwise False.
        bool& mouseDown(const EMouseKeyCodes KeyCode);      // Mouse button pressed
        bool& mouseHit(const EMouseKeyCodes KeyCode);       // Mouse button hit
        bool& mouseReleased(const EMouseKeyCodes KeyCode);  // Mouse button released
        
        void mouseDownSimulation(const EMouseKeyCodes KeyCode) const;
        void mouseReleasedSimulation(const EMouseKeyCodes KeyCode) const;
        
        //! \return Mouse wheel motion speed.
        s16 getMouseWheel() const;
        
        //! Simulates a mouse wheel motion.
        void setMouseWheel(s16 Value);
        
        /**
        Enables or disables the cursor visibility. In window mode the cursor is by default visible and in fullscreen
        mode by default invisible.
        */
        void setCursorVisible(bool Visible);
        
        //! \return True if the specified joystick button is pressed.
        bool joystickDown(const EJoystickKeyCodes KeyCode) const;
        
        //! \return 3D vector of the joystick position (X and Y coordinates are most important).
        dim::vector3df getJoystickPosition() const;
        
    private:
        
        friend class sp::SoftPixelDevice;
        
        #if defined(SP_PLATFORM_LINUX)
        friend class sp::SoftPixelDeviceLinux;
        #endif
        
        /* Members */
        
        bool isCursorSpeedBlocked_;
        dim::point2di LastCursorPos_, CursorSpeed_;
        
};

#elif defined(SP_PLATFORM_ANDROID)

/*
 * Enumerations
 */

enum EKeyCodes
{
    KEY_UNKNOWN = 0,
    KEY_SOFT_LEFT,
    KEY_SOFT_RIGHT,
    KEY_HOME,
    KEY_BACK,
    KEY_CALL,
    KEY_ENDCALL,
    KEY_0,
    KEY_1,
    KEY_2,
    KEY_3,
    KEY_4,
    KEY_5,
    KEY_6,
    KEY_7,
    KEY_8,
    KEY_9,
    KEY_STAR,
    KEY_POUND,
    KEY_DPAD_UP,
    KEY_DPAD_DOWN,
    KEY_DPAD_LEFT,
    KEY_DPAD_RIGHT,
    KEY_DPAD_CENTER,
    KEY_VOLUME_UP,
    KEY_VOLUME_DOWN,
    KEY_POWER,
    KEY_CAMERA,
    KEY_CLEAR,
    KEY_A,
    KEY_B,
    KEY_C,
    KEY_D,
    KEY_E,
    KEY_F,
    KEY_G,
    KEY_H,
    KEY_I,
    KEY_J,
    KEY_K,
    KEY_L,
    KEY_M,
    KEY_N,
    KEY_O,
    KEY_P,
    KEY_Q,
    KEY_R,
    KEY_S,
    KEY_T,
    KEY_U,
    KEY_V,
    KEY_W,
    KEY_X,
    KEY_Y,
    KEY_Z,
    KEY_COMMA,
    KEY_PERIOD,
    KEY_ALT_LEFT,
    KEY_ALT_RIGHT,
    KEY_SHIFT_LEFT,
    KEY_SHIFT_RIGHT,
    KEY_TAB,
    KEY_SPACE,
    KEY_SYM,
    KEY_EXPLORER,
    KEY_ENVELOPE,
    KEY_ENTER,
    KEY_DEL,
    KEY_GRAVE,
    KEY_MINUS,
    KEY_EQUALS,
    KEY_LEFT_BRACKET,
    KEY_RIGHT_BRACKET,
    KEY_BACKSLASH,
    KEY_SEMICOLON,
    KEY_APOSTROPHE,
    KEY_SLASH,
    KEY_AT,
    KEY_NUM,
    KEY_HEADSETHOOK,
    KEY_FOCUS,
    KEY_PLUS,
    KEY_MENU,
    KEY_NOTIFICATION,
    KEY_SEARCH,
    KEY_MEDIA_PLAY_PAUSE,
    KEY_MEDIA_STOP,
    KEY_MEDIA_NEXT,
    KEY_MEDIA_PREVIOUS,
    KEY_MEDIA_REWIND,
    KEY_MEDIA_FAST_FORWARD,
    KEY_MUTE,
    KEY_PAGEUP,
    KEY_PAGEDOWN,
    KEY_PICTSYMBOLS,
    KEY_SWITCH_CHARSET,
    KEY_BUTTON_A,
    KEY_BUTTON_B,
    KEY_BUTTON_C,
    KEY_BUTTON_X,
    KEY_BUTTON_Y,
    KEY_BUTTON_Z,
    KEY_BUTTON_L1,
    KEY_BUTTON_R1,
    KEY_BUTTON_L2,
    KEY_BUTTON_R2,
    KEY_BUTTON_THUMBL,
    KEY_BUTTON_THUMBR,
    KEY_BUTTON_START,
    KEY_BUTTON_SELECT,
    KEY_BUTTON_MODE,
};


//! User input control interface for Mobile operating-systems (Android).
class SP_EXPORT InputControl
{
    
    public:
        
        InputControl();
        ~InputControl();
        
        /* === Functions === */
        
        s32 processEvent(android_app* App, AInputEvent* Event);
        
        //! Returns the position of the specified pointer in screen space.
        dim::point2df getPointerPosition(u32 Index = 0) const;
        
        //! Returns the pressure size (in the range [0.0, 1.0]) of the specified pointer.
        f32 getPointerSize(u32 Index = 0) const;
        
        dim::point2df getMotionSpeed(u32 Index = 0);
        
        //! Returns true if the specified pointer is active (i.e. touch events are performed).
        bool isPointerActive(u32 Index = 0) const;
        
        //! Returns the pinch speed. This is the speed when you pinch with two fingers. Thus the first two pointers are used.
        f32 getPinchSpeed();
        
        /**
        \return True if the specified key is pressed otherwise false. The return value is a reference, thus you
        can manipulate this value if you want the further keyboard-checks will or will not be effected
        after pressing this key.
        */
        bool& keyDown(const EKeyCodes KeyCode);
        bool& keyHit(const EKeyCodes KeyCode);
        bool& keyReleased(const EKeyCodes KeyCode);
        
        /* === Inline functions === */
        
        inline u32 getPointerCount() const
        {
            return PointerCount_;
        }
        
        inline bool isMotionEvent() const
        {
            return isMotionEvent_;
        }
        
        inline dim::vector3df getGyroscopeSensor() const
        {
            return GyroscopeSensor_;
        }
        inline dim::vector3df getAccelerometerSensor() const
        {
            return AccelerometerSensor_;
        }
        inline f32 getLightSensor() const
        {
            return LightSensor_;
        }
        
        /**
        Enables or disables the default key event handling. If disabled keys such as "volume up/down" won't occur
        a higher or lower sound volume. By default enabled.
        */
        inline void setDefaultKeyEventHandling(bool Enable)
        {
            isDefaultKeyEventHandling_ = Enable;
        }
        inline bool getDefaultKeyEventHandling() const
        {
            return isDefaultKeyEventHandling_;
        }
        
        //! Enables or disables each individual key. This has only an effect if default-key-event-handling is disabled.
        inline void setKeyEnable(const EKeyCodes KeyCode, bool Enable)
        {
            isKeyEnabled_[KeyCode] = Enable;
        }
        inline bool getKeyEnable(const EKeyCodes KeyCode) const
        {
            return isKeyEnabled_[KeyCode];
        }
        
    private:
        
        friend class sp::SoftPixelDevice;
        friend class sp::SoftPixelDeviceAndroid;
        
        /* Macros */
        
        static const s32 POINTER_COUNT = 5;
        
        /* Structures */
        
        struct STouchPointer
        {
            dim::point2df Position;
            f32 Size;
        };
        
        /* Functions */
        
        void setPointer(u32 Index, const dim::point2df &Position, f32 Size);
        void resetInputEvents();
        
        /* Members */
        
        bool isCursorSpeedBlocked_;
        bool isMotionEvent_;
        
        bool isDefaultKeyEventHandling_;
        bool isKeyEnabled_[KEY_BUTTON_MODE + 1];
        
        u32 PointerCount_;
        STouchPointer Pointer_[POINTER_COUNT];
        dim::point2df MotionSpeed_[POINTER_COUNT], MotionPointerPos_[POINTER_COUNT], LastPointerPos_[POINTER_COUNT];
        f32 DragDistance_, LastDragDistance_;
        
        dim::vector3df GyroscopeSensor_, AccelerometerSensor_;
        f32 LightSensor_;
        
};

#elif defined(SP_PLATFORM_IOS)

//! User input control interface for Apple iOS.
class SP_EXPORT InputControl
{
    
    public:
        
        InputControl();
        ~InputControl();
        
        /* === Functions === */
        
        //! Returns the position of the specified pointer in screen space.
        dim::point2di getPointerPosition(u32 Index = 0) const;
        
        //! Returns the motion speed of the specified pointer in screen space.
        dim::point2di getMotionSpeed(u32 Index = 0) const;
        
        //! Returns true if the specified pointer is active (i.e. touch events are performed).
        bool isPointerActive(u32 Index = 0) const;
        
        //! Returns the pinch speed. This is the speed when you pinch with two fingers. Thus the first two pointers are used.
        f32 getPinchSpeed();
        
        //! Returns the number of pointers.
        u32 getPointerCount() const;
        
    private:
        
        friend class sp::SoftPixelDevice;
        friend class sp::SoftPixelDeviceIOS;
        
        /* Macros */
        
        static const s32 POINTER_COUNT = 5;
        
        /* Functions */
        
        void resetInputEvents();
        
        /* Members */
        
        bool isCursorSpeedBlocked_;
        
        f32 DragDistance_, LastDragDistance_;
        
};

#endif


} // /namespace io

} // /namespace sp


#endif



// ================================================================================
