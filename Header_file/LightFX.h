#ifndef _LIGHTFXCLASS_H_
#define _LIGHTFXCLASS_H_

#include "pch.h"





class LightFxClass
{

private:

	struct SetFxLightValue {
		DirectX::XMFLOAT4 Ambient;
		DirectX::XMFLOAT4 Diffuse;
		DirectX::XMFLOAT4 Specular;
		DirectX::XMFLOAT3 LightDirection;
		float SpecularPower;
	};

public:
	LightFxClass();
	LightFxClass(const LightFxClass&);
	~LightFxClass();

	bool Initialize(ID3D11Device*, HWND);
	void Shutdown();
	bool Render(ID3D11DeviceContext*, int, int,int,DirectX::XMMATRIX&, DirectX::XMMATRIX&, DirectX::XMMATRIX&, DirectX::XMVECTOR&, DirectX::XMVECTOR&, DirectX::XMVECTOR&, DirectX::XMVECTOR&, DirectX::XMVECTOR&, float, ID3D11ShaderResourceView**);

private:
	bool InitializeShader(ID3D11Device*, HWND, const WCHAR*);
	void ShutdownShader();
	void OutputShaderErrorMessage(ID3D10Blob*, HWND, const WCHAR*);
	bool SetFXParameters(ID3D11DeviceContext*, DirectX::XMMATRIX&, DirectX::XMMATRIX&, DirectX::XMMATRIX&, DirectX::XMVECTOR&, DirectX::XMVECTOR&, DirectX::XMVECTOR&, DirectX::XMVECTOR&, DirectX::XMVECTOR&, float, ID3D11ShaderResourceView**);
	void RenderShader(ID3D11DeviceContext*, int,int,int);

	bool BuildGeometryBuffers(ID3D11Device*);
	bool SetShaderVariableName(HWND);
private:

	SetFxLightValue LightValue;
	ID3D11InputLayout* m_layout;

	ID3DX11Effect* m_fxShader;
	ID3DX11EffectTechnique* mTech;
	ID3DX11EffectMatrixVariable* mfxWorldViewProj;
	ID3DX11EffectMatrixVariable* mfxWorldMatrix;
	ID3DX11EffectMatrixVariable* mfxViewMatrix;
	ID3DX11EffectMatrixVariable* mfxProjectionMatrix;
	ID3DX11EffectVariable* mfxCameraPosition;
	ID3DX11EffectVariable* mfxLightValue;
	ID3DX11EffectShaderResourceVariable* mfxShaderTexture;
};
#endif  _LIGHTFXCLASS_H_