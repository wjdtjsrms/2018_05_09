#ifndef _TEXTURECLASS_H_
#define _TEXTURECLASS_H_


#include "pch.h"


using namespace DirectX;
using namespace Microsoft::WRL;


class TextureClass
{
public:
	TextureClass();
	TextureClass(const TextureClass&);
	~TextureClass();

	bool Initialize(ID3D11Device*, const WCHAR*, const WCHAR*); //wchar = char ? load by name
	bool Initialize(ID3D11Device*, const WCHAR*); //wchar = char ? load by name
	void Shutdown();
	ID3D11ShaderResourceView** GetTexture();

private:
	//연결 리스트로 구현해 보기
	ID3D11ShaderResourceView* m_texture[2];

};

#endif