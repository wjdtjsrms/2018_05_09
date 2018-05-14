#ifndef _CAMERACLASS_H_
#define _CAMERACLASS_H_


#include "pch.h"



class CameraClass
{
public:
	CameraClass();
	CameraClass(const CameraClass&);
	~CameraClass();

	void SetPosition(float, float, float);
	void SetRotation(float, float, float);

	DirectX::XMFLOAT3 GetPosition();
	DirectX::XMFLOAT3 GetRotation();

	void Render();
	void GetViewMatrix(DirectX::XMMATRIX&);

	DirectX::XMMATRIX& GetViewMatrix2();

private:
	float m_positionX, m_positionY, m_positionZ;
	float m_rotationX, m_rotationY, m_rotationZ;

	DirectX::XMFLOAT4X4 m_viewMatrix;
};

#endif