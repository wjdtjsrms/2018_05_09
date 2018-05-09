////////////////////////////////////////////////////////////////////////////////
// Filename: colorshaderclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _LIGHTSHADERCLASS_H_
#define _LIGHTSHADERCLASS_H_



//////////////
// INCLUDES //
//////////////w

#include "pch.h"

using namespace DirectX;
using namespace std;


////////////////////////////////////////////////////////////////////////////////
// Class name: ColorShaderClass
////////////////////////////////////////////////////////////////////////////////
class LightShaderClass
{
private:
	struct MatrixBufferType
	{
		XMFLOAT4X4 world;
		XMFLOAT4X4 view;
		XMFLOAT4X4 projection;
	};

	struct LightBufferType
	{
		XMFLOAT4 ambientColor;
		XMFLOAT4 diffuseColor;
		XMFLOAT3 lightDirection;
		float  specularPower;
		XMFLOAT4 specularColor;
	};
	struct  CameraBufferType
	{
		XMFLOAT4 cameraPosition;
		
	};
	
public:
	LightShaderClass();
	LightShaderClass(const LightShaderClass&);
	~LightShaderClass();

	bool Initialize(ID3D11Device*, HWND);
	void Shutdown();
	bool Render(ID3D11DeviceContext*, int, XMMATRIX&, XMMATRIX&, XMMATRIX&, XMVECTOR&, XMVECTOR&, XMVECTOR&, XMVECTOR&, XMVECTOR&, float, ID3D11ShaderResourceView**);

private:
	bool InitializeShader(ID3D11Device*, HWND, const WCHAR*, const WCHAR*);
	bool initializeDesc(ID3D11Device*);

	void ShutdownShader();
	void OutputShaderErrorMessage(ID3D10Blob*, HWND, const WCHAR*);

	bool SetShaderParameters(ID3D11DeviceContext*, XMMATRIX&, XMMATRIX&, XMMATRIX&, XMVECTOR&, XMVECTOR&, XMVECTOR&, XMVECTOR&, XMVECTOR&, float, ID3D11ShaderResourceView**);
	void RenderShader(ID3D11DeviceContext*, int);

private:
	ID3D11VertexShader* m_vertexShader;
	ID3D11PixelShader* m_pixelShader;
	ID3D11InputLayout* m_layout;
	ID3D11Buffer* m_matrixBuffer;
	ID3D11SamplerState* m_sampleState;
	ID3D11Buffer* m_lightBuffer;
	ID3D11Buffer* m_cameraBuffer;

};

#endif