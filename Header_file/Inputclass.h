////////////////////////////////////////////////////////////////////////////////
// Filename: inputclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _INPUTCLASS_H_
#define _INPUTCLASS_H_

#define DIRECTINPUT_VERSION 0x0800



#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")

static const int MOUSE_LEFT = 0;
static const int MOUSE_RIGHT = 1;

#include <dinput.h>
#include <iostream>


////////////////////////////////////////////////////////////////////////////////
// Class name: InputClass
////////////////////////////////////////////////////////////////////////////////
class InputClass
{
public:
	InputClass();
	InputClass(const InputClass&);
	~InputClass();

	bool Initialize(HINSTANCE, HWND, int, int);
	void Shutdown();
	bool Frame();

	bool IsEscapePressed(); 
	void GetMouseLocation(int&, int&);
	int mouseKeyIsDown(unsigned char);
	void GetPlayerLocation(float& ,float&);


private:
	bool ReadKeyboard();
	bool ReadMouse(); 
	void ProcessInput();



private:
	//interface variable
	IDirectInput8* m_directInput;
	IDirectInputDevice8* m_keyboard;
	IDirectInputDevice8* m_mouse;

	unsigned char m_keyboardState[256];
	DIMOUSESTATE m_mouseState;

	int m_screenWidth, m_screenHeight;
	int m_mouseX, m_mouseY;

	float m_playerX, m_playerY;

	



};

#endif