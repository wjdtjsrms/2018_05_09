
#include "stdafx.h"
#include "../Header_file/Textclass.h"


TextClass::TextClass(){
	m_Font = 0;
	m_FontShader = 0;

	m_sentence1 = 0;
	m_sentence2 = 0;
	m_sentence3 = 0;
	m_sentence4 = 0;
}

TextClass::TextClass(const TextClass& other){

}

TextClass::~TextClass(){

}


bool TextClass::Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext,HWND hwnd, int screenWidth,int screenHight,XMMATRIX& baseViewMatrix){
	bool result;

	m_screenWidth = screenWidth;
	m_screenHeight = screenHight;

	XMStoreFloat4x4(&m_baseViewMatrix, baseViewMatrix); // m_Baseviewmatrix=baseViewmatrix

	m_Font = new FontClass;
	if (!m_Font){
		return false;
	}

	result = m_Font->Initialize(device, L"Resource/Font.txt", L"Resource/Font.dds");
	if (!result){
		MessageBox(hwnd, L"Could not initialize the font object.", L"Error", MB_OK); 
		return false;
	}

	m_FontShader = new Fontshaderclass;

	if (!m_FontShader){
		return false;
	}

	result = m_FontShader->Initialize(device, hwnd);

	if (!result){
		MessageBox(hwnd, L"Could not initialize the shader object.", L"Error", MB_OK);
		return false;
	}


	// first Sentence
	result = InitializeSentence(&m_sentence1, 16, device);
	if (!result){
		return false;
	}


	// second Sentence

	result = InitializeSentence(&m_sentence2, 16, device);
	if (!result){
		return false;
	}


	result = InitializeSentence(&m_sentence3, 16, device);
	if (!result){
		return false;
	}



	result = InitializeSentence(&m_sentence4, 16, device);
	if (!result){
		return false;
	}




	return true;
}

void TextClass::Shutdown(){

	ReleaseSentence(&m_sentence1);
	ReleaseSentence(&m_sentence2);
	ReleaseSentence(&m_sentence3);
	ReleaseSentence(&m_sentence4);

	if (m_FontShader){
		m_FontShader->Shutdown();
		delete m_FontShader;
		m_FontShader = 0;
	}
	if (m_Font){
		m_Font->Shutdown();
		delete m_Font;
		m_Font = 0;
	}

	return;
}

//for(size(m_setence)) setenceType[] 아하?
bool TextClass::Render(ID3D11DeviceContext* deivceContext, XMMATRIX& worldMatrix, XMMATRIX& orthoMatrix){
	bool result;

	result = RenderSentence(deivceContext, m_sentence1, worldMatrix, orthoMatrix);

	if (!result){
		return false;
	}

	result = RenderSentence(deivceContext, m_sentence2, worldMatrix, orthoMatrix);

	if (!result){
		return false;
	}

	result = RenderSentence(deivceContext, m_sentence3, worldMatrix, orthoMatrix);

	if (!result){
		return false;
	}

	result = RenderSentence(deivceContext, m_sentence4, worldMatrix, orthoMatrix);

	if (!result){
		return false;
	}

	return true;

}


bool TextClass::InitializeSentence(SentenceType** sentence, int maxLength, ID3D11Device* device){
	VertexType* vertices;
	unsigned long* indices;
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result;
	int i;


	// Create a new sentence object.
	*sentence = new SentenceType;
	if (!*sentence)
	{
		return false;
	}

	// Initialize the sentence buffers to null.
	(*sentence)->vertexBuffer = 0;
	(*sentence)->indexBuffer = 0;

	// Set the maximum length of the sentence.
	(*sentence)->maxLength = maxLength;

	// Set the number of vertices in the vertex array.
	(*sentence)->vertexCount = 6 * maxLength;

	// Set the number of indexes in the index array.
	(*sentence)->indexCount = (*sentence)->vertexCount;

	// Create the vertex array.
	vertices = new VertexType[(*sentence)->vertexCount];
	if (!vertices)
	{
		return false;
	}

	// Create the index array.
	indices = new unsigned long[(*sentence)->indexCount];
	if (!indices)
	{
		return false;
	}

	// Initialize vertex array to zeros at first.
	memset(vertices, 0, (sizeof(VertexType) * (*sentence)->vertexCount));

	// Initialize the index array.
	for (i = 0; i<(*sentence)->indexCount; i++)
	{
		indices[i] = i;
	}


	// Set up the description of the dynamic vertex buffer.
	vertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	vertexBufferDesc.ByteWidth = sizeof(VertexType) * (*sentence)->vertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the vertex data.
	vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	// Create the vertex buffer.
	result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &(*sentence)->vertexBuffer);
	if (FAILED(result))
	{
		return false;
	}

	// Set up the description of the static index buffer.
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long) * (*sentence)->indexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the index data.
	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	// Create the index buffer.
	result = device->CreateBuffer(&indexBufferDesc, &indexData, &(*sentence)->indexBuffer);
	if (FAILED(result))
	{
		return false;
	}

	// Release the vertex array as it is no longer needed.
	delete[] vertices;
	vertices = 0;

	// Release the index array as it is no longer needed.
	delete[] indices;
	indices = 0;

	return true;


}

bool TextClass::UpdateSentence(SentenceType* sentence, char* text, int positionX, int positionY, float red, float green, float blue, ID3D11DeviceContext* deviceContext){
	
	int numLetters;
	VertexType* vertices;
	float drawX, drawY;
	HRESULT result;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	VertexType* verticesPtr;


	sentence->red = red;
	sentence->blue = blue;
	sentence->green = green;

	numLetters = (int)strlen(text);

	if (numLetters > sentence->maxLength){
		return false;
	}

	vertices = new VertexType[sentence->vertexCount];
	if (!vertices){
		return false;
	}

	memset(vertices, 0, (sizeof(VertexType)*sentence->vertexCount));

	drawX = (float)(((m_screenWidth / 2) * -1) + positionX);
	drawY = (float)((m_screenHeight / 2) - positionY);

	m_Font->BuildVertexArray((void*)vertices, text, drawX, drawY);


	// Lock the vertex buffer so it can be written to.
	result = deviceContext->Map(sentence->vertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result))
	{
		return false;
	}

	// Get a pointer to the data in the vertex buffer.
	verticesPtr = (VertexType*)mappedResource.pData;

	// Copy the data into the vertex buffer.
	memcpy(verticesPtr, (void*)vertices, (sizeof(VertexType) * sentence->vertexCount));

	// Unlock the vertex buffer.
	deviceContext->Unmap(sentence->vertexBuffer, 0);

	// Release the vertex array as it is no longer needed.
	delete[] vertices;
	vertices = 0;

	return true;
}

bool TextClass::RenderSentence(ID3D11DeviceContext* deviceContext, SentenceType* sentence, XMMATRIX& worldMatrix,
	XMMATRIX& orthoMatrix)
{
	unsigned int stride, offset;
	XMVECTOR pixelColor;
	XMMATRIX baseViewMatrix;
	bool result;

	baseViewMatrix = XMLoadFloat4x4(&m_baseViewMatrix);

	// Set vertex buffer stride and offset.
	stride = sizeof(VertexType);
	offset = 0;

	// Set the vertex buffer to active in the input assembler so it can be rendered.
	deviceContext->IASetVertexBuffers(0, 1, &sentence->vertexBuffer, &stride, &offset);

	// Set the index buffer to active in the input assembler so it can be rendered.
	deviceContext->IASetIndexBuffer(sentence->indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	// Set the type of primitive that should be rendered from this vertex buffer, in this case triangles.
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// Create a pixel color vector with the input sentence color.
	pixelColor = XMVectorSet(sentence->red, sentence->green, sentence->blue, 1.0f);
	

	// Render the text using the font shader.
	result = m_FontShader->Render(deviceContext, sentence->indexCount, worldMatrix, baseViewMatrix, orthoMatrix, m_Font->GetTexture(), pixelColor);
	if (!result)
	{
		false;
	}

	return true;
}

void TextClass::ReleaseSentence(SentenceType** sentence)
{
	if (*sentence)
	{
		// Release the sentence vertex buffer.
		if ((*sentence)->vertexBuffer)
		{
			(*sentence)->vertexBuffer->Release();
			(*sentence)->vertexBuffer = 0;
		}

		// Release the sentence index buffer.
		if ((*sentence)->indexBuffer)
		{
			(*sentence)->indexBuffer->Release();
			(*sentence)->indexBuffer = 0;
		}

		// Release the sentence.
		delete *sentence;
		*sentence = 0;
	}

	return;
}

//int[] 출력 char[] 출력 등등 형식에 따라 나누는게 더 좋을듯 templete은 느리겠지?
bool TextClass::SetMousePosition(int mouseX, int mouseY, ID3D11DeviceContext* deviceContext){
	char tempString[16];
	char mouseString[16];

	bool result;

	//convert the mouseX integer to string format
	_itoa_s(mouseX, tempString, 10);

	//Setup the mouseX string
	
	strcpy_s(mouseString, "Mouse X: ");
	strcat_s(mouseString, tempString);

	//Update the sentence vertex buffer with the new string information;
	result = UpdateSentence(m_sentence1, mouseString, 20, 20, 1.0f, 1.0f, 1.0f, deviceContext);

	if (!result){
		return false;
	}

	//convert the mouseX integer to string format
	_itoa_s(mouseY, tempString, 10);

	//Setup the mouseX string
	strcpy_s(mouseString, "Mouse Y: ");
	strcat_s(mouseString, tempString);

	//Update the sentence vertex buffer with the new string information;
	result = UpdateSentence(m_sentence2, mouseString, 20, 40, 1.0f, 1.0f, 1.0f, deviceContext);

	if (!result){
		return false;
	}

	return true;

}

bool TextClass::setFps(int fps, ID3D11DeviceContext* deviceContext){

	char tempString[16];
	char fpsString[16];
	float red, green, blue;
	bool result;

	if (fps > 9999){
		fps = 9999;
	}

	_itoa_s(fps, tempString, 10);

	strcpy_s(fpsString, "Fps: ");
	strcat_s(fpsString, tempString);

	if (fps >= 60){
		red = 0.0f;
		green = 1.0f;
		blue = 0.0f;
	}

	if (fps < 60){
		red = 1.0f;
		green = 1.0f;
		blue = 0.0f;
	}

	if (fps < 30){
		red = 1.0f;
		green = 0.0f;
		blue = 0.0f;
	}

	result = UpdateSentence(m_sentence3, fpsString, 20, 60, red, green, blue, deviceContext);

	if (!result){
		return false;
	}
	return true;
}

bool TextClass::SetCpu(int cpu, ID3D11DeviceContext* deviceContext){


	char tempString[16];
	char cpuString[16];

	bool result;

	_itoa_s(cpu, tempString, 10);

	strcpy_s(cpuString, "Cpu: ");
	strcat_s(cpuString, tempString);
	strcat_s(cpuString, "%");


	result = UpdateSentence(m_sentence4, cpuString, 20, 80, 0.0f, 1.0f, 0.0f, deviceContext);

	if (!result){
		return false;
	}

	return true;
}

bool TextClass::SetRenderCount(int renderCount, ID3D11DeviceContext* deviceContext){


	char tempString[16];
	char RenderString[16];

	bool result;

	_itoa_s(renderCount, tempString, 10);

	strcpy_s(RenderString, "renderCount: ");
	strcat_s(RenderString, tempString);


	result = UpdateSentence(m_sentence1, RenderString, 20, 80, 0.0f, 1.0f, 0.0f, deviceContext);

	if (!result){
		return false;
	}

	return true;
}