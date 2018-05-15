////////////////////////////////////////////////////////////////////////////////
// Filename: graphicsclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _GRAPHICSCLASS_H_
#define _GRAPHICSCLASS_H_


///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "d3dclass.h"
#include "cameraclass.h"
#include "modelclass.h"
#include "lightshaderclass.h"
#include "LightFX.h"
#include "lightclass.h"
#include "Textclass.h"





/////////////
// GLOBALS //
/////////////
const bool FULL_SCREEN = false;
const bool VSYNC_ENABLED = true;
const float SCREEN_DEPTH = 1000.0f;
const float SCREEN_NEAR = 0.1f;


////////////////////////////////////////////////////////////////////////////////
// Class name: GraphicsClass
////////////////////////////////////////////////////////////////////////////////
class GraphicsClass
{


public:

	//안 쓰거나 그냥 VECTOR 쓰는게 더 나을지두
	typedef struct MousePosition {
		float mouseX;
		float	mouseY;

	}MouseXY;

	GraphicsClass();
	GraphicsClass(const GraphicsClass&);
	~GraphicsClass();

	bool Initialize(int, int, HWND);
	void Shutdown();
	bool Frame();
	bool SetHardWareData(MouseXY, MouseXY,int,int,float );

private:
	bool Render();
	bool setValue();
	void SetMemberVariable();

private:
	D3DClass* m_D3D;
	CameraClass* m_Camera;
	ModelClass* m_Model;
	LightShaderClass* m_LightShader;
	LightFxClass* m_LightFx;
	LightClass* m_Light;
	TextClass* m_Text;

	int m_mouseX;
	int m_mouseY;

	float m_PlayerX;
	float m_PlayerY;
};


#endif