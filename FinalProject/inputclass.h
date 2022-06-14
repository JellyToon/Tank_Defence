////////////////////////////////////////////////////////////////////////////////
// Filename: inputclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _INPUTCLASS_H_
#define _INPUTCLASS_H_

//#pragma comment(lib, "d3d11.lib")
#pragma comment (lib, "dinput8.lib")
#pragma comment (lib, "dxguid.lib")

#include <dinput.h>

////////////////////////////////////////////////////////////////////////////////
// Class name: InputClass
////////////////////////////////////////////////////////////////////////////////
class InputClass
{
public:
	InputClass();
	InputClass(const InputClass&);
	~InputClass();

	bool Initialize(HWND, HINSTANCE);

	void KeyDown(unsigned int);
	void KeyUp(unsigned int);

	bool IsKeyDown(unsigned int);

	IDirectInputDevice8* GetDIMouse() { return DIMouse; }
	DIMOUSESTATE GetMouseState() { return m_mouseState; }

	void SetMouseState(DIMOUSESTATE value) { m_mouseState = value; }


private:
	bool m_keys[256];
	
	LPDIRECTINPUT8 DirectInput;
	IDirectInputDevice8* DIMouse;
	DIMOUSESTATE m_mouseState;
	int m_mouseX, m_mouseY;


};

#endif