////////////////////////////////////////////////////////////////////////////////
// Filename: inputclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "../Header_file/inputclass.h"


InputClass::InputClass()
	:m_directInput(0),
	m_keyboard(0),
	m_mouse(0),
	m_playerX(0),
	m_playerY(0)
{}



InputClass::InputClass(const InputClass& other)
{
}


InputClass::~InputClass()
{
}


bool InputClass::Initialize(HINSTANCE hinstance, HWND hwnd, int screenWidth, int screenHeight)
{
	HRESULT result;
	
	// Store the screen size which will be used for positioning the mouse cursor.
	m_screenWidth = screenWidth;
	m_screenHeight = screenHeight;

	// Initialize the location of the mouse on the screen. 
	m_mouseX = 0; 
	m_mouseY = 0;

	//initialize the main direct input interface
	result = DirectInput8Create(hinstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_directInput, NULL);
	if (FAILED(result)){
		return false;
	}




	result = m_directInput->CreateDevice(GUID_SysKeyboard, &m_keyboard, NULL);
	if (FAILED(result)) {
		return false;
	}

	// Set the data format. In this case since it is a keyboard we can use the predefined data format.
	result = m_keyboard->SetDataFormat(&c_dfDIKeyboard);
	if (FAILED(result)) {
		return false;
	}

	//DISCL_BACKGROUND DISCL_NONEXCLUSIVE
	result = m_keyboard->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_EXCLUSIVE);//DISCL_NONEXCLUSIVE 비배제 상태 ,| or 비트 연산자
	if (FAILED(result)) {
		return false;
	}

	// now acquire(얻다) the keyboard
	result = m_keyboard->Acquire();
	if (FAILED(result)) {
		return false;
	}



	result = m_directInput->CreateDevice(GUID_SysMouse, &m_mouse, NULL);
	if (FAILED(result)) {
		return false;
	}

	result = m_mouse->SetDataFormat(&c_dfDIMouse);
	if (FAILED(result)) {
		return false;
	}

	result = m_mouse->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
	if (FAILED(result)) {
		return false;
	}
	result = m_mouse->Acquire();
	if (FAILED(result)) {
		return false;
	}

	return true;
}

void InputClass::Shutdown()
{
	//This process is always in Unacquire->Release

	if (m_mouse){
		m_mouse->Unacquire();
		m_mouse->Release();
		m_mouse = 0;
	}

	if (m_keyboard){
		m_keyboard->Unacquire();
		m_keyboard->Release();
		m_keyboard = 0;
	}

	if (m_directInput){
		m_directInput->Release();
		m_directInput = 0;
	}

	return;
}

bool InputClass::Frame(){
	bool result;

	//read the current state of the keyboard
	result = ReadKeyboard();
	if (FAILED(result)) {
		return false;
	}
	result = ReadMouse();
	if (FAILED(result)) {
		return false;
	}

	//process the changes in the mouse and keyboard
	ProcessInput();

	return true;
}

bool InputClass::ReadKeyboard(){

	HRESULT result;

	//read the keyboard device
	result = m_keyboard->GetDeviceState(sizeof(m_keyboardState), (LPVOID)&m_keyboardState);
	if (FAILED(result)){
		if ((result == DIERR_INPUTLOST) || (result == DIERR_NOTACQUIRED)){
			m_keyboard->Acquire();
		}
		else{
			return false;
		}

		
	}
	return true;
}

bool InputClass::ReadMouse(){
	HRESULT result;

	result = m_mouse->GetDeviceState(sizeof(DIMOUSESTATE), (LPVOID)&m_mouseState);
	if (FAILED(result)){
		if ((result == DIERR_INPUTLOST) || (result == DIERR_NOTACQUIRED)){
			m_mouse->Acquire();
		}
		else{
			return false;
		}


	}
	return true;
}



void InputClass::ProcessInput(){

	//// Update the location of the mouse cursor based on the change of the mouse location during the frame.
	if (m_mouseState.rgbButtons[MOUSE_LEFT & 0x80]){
		m_mouseX += m_mouseState.lX;
		
		m_mouseY += m_mouseState.lY;
	}

	//if (m_mouseX < 0){m_mouseX = 0;}
	//if (m_mouseY < 0) { m_mouseY = 0; } 

	//if (m_mouseX > m_screenWidth) { m_mouseX = m_screenWidth; } 
	//if (m_mouseY > m_screenHeight) { m_mouseY = m_screenHeight; }

	return;
}

bool InputClass::IsEscapePressed(){
	if (m_keyboardState[DIK_ESCAPE] & 0x80){
		return true;
	}
	if (m_keyboardState[DIK_W] & 0x80){
		m_playerY += 0.1f;
		
	}
	if (m_keyboardState[DIK_A] & 0x80){
		m_playerX -= 0.1f;
	}
	if (m_keyboardState[DIK_S] & 0x80){
		m_playerY -= 0.1f;
	}
	if (m_keyboardState[DIK_D] & 0x80){
		m_playerX += 0.1f;
	}
	return false;
}

void InputClass::GetMouseLocation(int& mouseX, int& mouseY) 
{	mouseX = m_mouseX;

	mouseY = m_mouseY; 
	return; 
}

void InputClass::GetPlayerLocation(float& player_X, float& player_Y){
	player_X = m_playerX;
	player_Y = m_playerY;
	return;
}


