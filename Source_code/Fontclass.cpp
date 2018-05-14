
#include "stdafx.h"
#include "../Header_file/Fontclass.h"
#include "../source_code/stdafx.h"

using namespace DirectX;
using namespace std;

FontClass::FontClass()
	:m_Font(0),
	 m_Texture(0)
{}

FontClass::FontClass(const FontClass& other){

}

FontClass::~FontClass(){

}


bool FontClass::Initialize(ID3D11Device* device, const WCHAR* fontFilename, const WCHAR* textureFilename){
	bool result;

	result = LoadFontDate(fontFilename);
	if (!result){
		return false;
	}

	result = LoadTexture(device,textureFilename);
	if (!result){
		return false;
	}
	return true;
}

void FontClass::Shutdown(){
	ReleaseTexture();
	ReleaseFontData();
	return;
}

bool FontClass::LoadFontDate(const WCHAR* filename){
	ifstream fin;
	char temp;

	m_Font = new FontType[95];
	if (!m_Font){
		return false;
	}

	fin.open(filename);
	if (fin.fail()){
		return false;
	}

	for (int i = 0; i < 95; i++){
		fin.get(temp);
		while (temp != ' '){
			fin.get(temp);
		}
		fin.get(temp);
		while (temp != ' '){
			fin.get(temp);
		}
		fin >> m_Font[i].left;
		fin >> m_Font[i].right;
		fin >> m_Font[i].size;
	}

	fin.close();

	return true;
}

void FontClass::ReleaseFontData(){
	if (m_Font){
		delete[] m_Font;
		m_Font = 0;
	}
	return;
}

bool FontClass::LoadTexture(ID3D11Device* device, const WCHAR* filename){
	bool result;

	m_Texture = new TextureClass;
	if (!m_Texture){
		return false;
	}

	result = m_Texture->Initialize(device, filename);
	if (!result){
		return false;
	}

	return true;
}

void FontClass::ReleaseTexture(){

	if (m_Texture){
		m_Texture->Shutdown();
		delete m_Texture;
		m_Texture = 0;
	}

	return;

}
ID3D11ShaderResourceView** FontClass::GetTexture(){
	return m_Texture->GetTexture();
}


void FontClass::BuildVertexArray(void* vertices, char* sentence,float drawX,float drawY){
	VertextType* vertexPtr;
	int numLetters, index, letter,i;

	// Coerce the input vertices into a VertexType structure, send data to TextClass
	vertexPtr = (VertextType*)vertices;

	numLetters = (int)strlen(sentence);
	index = 0;

	for (i = 0; i < numLetters; i++){
		letter = ((int)sentence[i]) - 32; // string[] == char 

		if (letter == 0){
			drawX = drawX + 3.0f;
		}

		else{

			// First triangle in quad.
			vertexPtr[index].position = XMFLOAT3(drawX, drawY, 0.0f);  // Top left.
			vertexPtr[index].texture = XMFLOAT2(m_Font[letter].left, 0.0f);
			index++;

			vertexPtr[index].position = XMFLOAT3((drawX + m_Font[letter].size), (drawY - 16), 0.0f);  // Bottom right.
			vertexPtr[index].texture = XMFLOAT2(m_Font[letter].right, 1.0f);
			index++;

			vertexPtr[index].position = XMFLOAT3(drawX, (drawY - 16), 0.0f);  // Bottom left.
			vertexPtr[index].texture = XMFLOAT2(m_Font[letter].left, 1.0f);
			index++;

			// Second triangle in quad.
			vertexPtr[index].position = XMFLOAT3(drawX, drawY, 0.0f);  // Top left.
			vertexPtr[index].texture = XMFLOAT2(m_Font[letter].left, 0.0f);
			index++;

			vertexPtr[index].position = XMFLOAT3(drawX + m_Font[letter].size, drawY, 0.0f);  // Top right.
			vertexPtr[index].texture = XMFLOAT2(m_Font[letter].right, 0.0f);
			index++;

			vertexPtr[index].position = XMFLOAT3((drawX + m_Font[letter].size), (drawY - 16), 0.0f);  // Bottom right.
			vertexPtr[index].texture = XMFLOAT2(m_Font[letter].right, 1.0f);
			index++;

			// Update the x location for drawing by the size of the letter and one pixel.
			drawX = drawX + m_Font[letter].size + 1.0f;
		}
		
	}

	return;
}