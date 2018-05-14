
#include "stdafx.h"
#include "../Header_file/Textureclass.h"

using namespace DirectX;
using namespace Microsoft::WRL;

TextureClass::TextureClass()
	:m_texture{0}
{}


TextureClass::TextureClass(const TextureClass& other)
{
}


TextureClass::~TextureClass()
{
}

bool TextureClass::Initialize(ID3D11Device* device, const WCHAR* filename1, const WCHAR* filename2){

	HRESULT result;
	result = CreateDDSTextureFromFile(device, filename1, NULL, &m_texture[0], NULL, NULL);

	
	if (FAILED(result))
	{
		return false;
	}

	result = CreateDDSTextureFromFile(device, filename2, NULL, &m_texture[1], NULL, NULL);

	if (FAILED(result))
	{
		return false;
	}


	return true;

}

bool TextureClass::Initialize(ID3D11Device* device, const WCHAR* filename){

	HRESULT result;
	result = CreateDDSTextureFromFile(device, filename, NULL, &m_texture[0], NULL, NULL);

	if (FAILED(result))
	{
		return false;
	}


	return true;
}

void TextureClass::Shutdown()
{
	// Release the texture resource.
	if (m_texture[0])
	{
		m_texture[0]->Release();
		m_texture[0] = 0;
	}

	if (m_texture[1])
	{
		m_texture[1]->Release();
		m_texture[1] = 0;
	}


	return;
}

ID3D11ShaderResourceView** TextureClass::GetTexture()
{
	return m_texture;
}

