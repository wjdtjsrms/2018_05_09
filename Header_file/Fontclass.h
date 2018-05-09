#ifndef _FONTCLASS_H_
#define _FONTCLASS_H_

#include "pch.h"
#include "Textureclass.h"


using namespace DirectX;
using namespace std;



class FontClass
{
private:
	struct FontType
	{
		float left, right;
		int size;
	};

	struct  VertextType
	{
		XMFLOAT3 position;
		XMFLOAT2 texture;

	};

public:
	FontClass();
	FontClass(const FontClass&);
	~FontClass();

	bool Initialize(ID3D11Device*, const WCHAR*, const WCHAR*);
	void Shutdown();

	ID3D11ShaderResourceView** GetTexture();
	void BuildVertexArray(void*, char*, float, float);

private:
	bool LoadFontDate(const WCHAR*);
	void ReleaseFontData();

	bool LoadTexture(ID3D11Device*, const WCHAR*);
	void ReleaseTexture();

private:
	FontType* m_Font;
	TextureClass* m_Texture;


};

#endif