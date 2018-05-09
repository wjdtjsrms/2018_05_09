////////////////////////////////////////////////////////////////////////////////
// Filename: colorshaderclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _FONTSHADERCLASS_H_
#define _FONTSHADERCLASS_H_


//////////////
// INCLUDES //
//////////////w

#include "pch.h"

using namespace std;
using namespace DirectX;


////////////////////////////////////////////////////////////////////////////////
// Class name: ColorShaderClass
////////////////////////////////////////////////////////////////////////////////
class Fontshaderclass
{
private:
	struct MatrixBufferType
	{
		XMFLOAT4X4 world;
		XMFLOAT4X4 view;
		XMFLOAT4X4 projection;
	};

	struct PixelBufferType
	{
		XMFLOAT4 pixelColor;

	};
	
public:
	Fontshaderclass();
	Fontshaderclass(const Fontshaderclass&);
	~Fontshaderclass();

	bool Initialize(ID3D11Device*, HWND);
	void Shutdown();
	bool Render(ID3D11DeviceContext*, int, XMMATRIX&, XMMATRIX&, XMMATRIX&, ID3D11ShaderResourceView**, XMVECTOR&);

private:
	bool InitializeShader(ID3D11Device*, HWND, const WCHAR*, const WCHAR*);
	void ShutdownShader();
	void OutputShaderErrorMessage(ID3D10Blob*, HWND, const WCHAR*);

	bool SetShaderParameters(ID3D11DeviceContext*, XMMATRIX&, XMMATRIX&, XMMATRIX&, ID3D11ShaderResourceView**,XMVECTOR&);
	void RenderShader(ID3D11DeviceContext*, int);

private:
	ID3D11VertexShader* m_vertexShader;
	ID3D11PixelShader* m_pixelShader;
	ID3D11InputLayout* m_layout;
	ID3D11Buffer* m_matrixBuffer;
	ID3D11Buffer* m_pixelBuffer;
	ID3D11SamplerState* m_sampleState;
};

#endif