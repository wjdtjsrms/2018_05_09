#ifndef _LIGHTCLASS_H_
#define _LIGHTCLASS_H_


#include "pch.h"

class LightClass
{
public:
	LightClass();
	LightClass(const LightClass&);
	~LightClass();

	void setSpecularColor(float, float, float, float);
	void SetDiffuseColor(float, float, float, float);
	void SetDirection(float, float, float);
	void SetAmbientColor(float, float, float, float);
	void SetSpecularPower(float);

	void GetDiffuseColor(DirectX::XMVECTOR&);
	void GetDirection(DirectX::XMVECTOR&);
	void GetAmbientColor(DirectX::XMVECTOR&);
	void GetSpecularColor(DirectX::XMVECTOR&);
	float GetSpecularPower();

private:
	DirectX::XMFLOAT4 m_ambientColor;
	DirectX::XMFLOAT4 m_diffuseColor;
	DirectX::XMFLOAT3 m_direction;
	DirectX::XMFLOAT4 m_specularColor;
	float m_specularPower;
	
};



#endif