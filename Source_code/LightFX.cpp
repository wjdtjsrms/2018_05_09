#include "stdafx.h"
#include "../Header_file/LightFX.h"



LightFxClass::LightFxClass()
{

	m_fxShader = 0;
	m_layout = 0;
	mTech = 0;
	mfxWorldViewProj = 0;



}

LightFxClass::LightFxClass(const LightFxClass&)
{
}

LightFxClass::~LightFxClass()
{
}

void LightFxClass::Shutdown()
{
	// Shutdown the vertex and pixel shaders as well as the related objects.
	ShutdownShader();

	return;
}

void LightFxClass::ShutdownShader()
{
	// Release the light constant buffer.


	// Release the layout.
	if (m_layout)
	{
		m_layout->Release();
		m_layout = 0;
	}

	if (m_fxShader) {
		m_fxShader->Release();
		m_fxShader = 0;
	}



	return;
}

void LightFxClass::OutputShaderErrorMessage(ID3DBlob* errorMessage, HWND hwnd, const WCHAR* shaderFilename)
{
	char* compileErrors;
	unsigned long bufferSize, i;
	ofstream fout;
	

	// Get a pointer to the error message text buffer.
	compileErrors = (char*)(errorMessage->GetBufferPointer());

	// Get the length of the message.
	bufferSize = errorMessage->GetBufferSize();

	// Open a file to write the error message to.
	fout.open("shader-error.txt");

	// Write out the error message.
	for (i = 0; i < bufferSize; i++)
	{
		fout << compileErrors[i];
	}

	// Close the file.
	fout.close();

	// Release the error message.
	errorMessage->Release();
	errorMessage = 0;

	// Pop a message up on the screen to notify the user to check the text file for compile errors.
	MessageBox(hwnd, L"Error compiling shader.  Check shader-error.txt for message.", shaderFilename, MB_OK);

	return;
}




bool LightFxClass::Initialize(ID3D11Device* device, HWND hwnd)
{
	bool result;
	

	result = InitializeShader(device, hwnd, L"Shader_code/color.fx");
	if (!result)
	{
		return false;
	}
	return true;
}


bool LightFxClass::Render(ID3D11DeviceContext* deviceContext, int indexCount, XMMATRIX& worldMatrix, XMMATRIX& viewMatrix,
	XMMATRIX& projectionMatrix, XMVECTOR& lightDirection, XMVECTOR& diffuseColor, XMVECTOR& ambientColor, XMVECTOR& cameraPosition, XMVECTOR& specularColor, float specularPower, ID3D11ShaderResourceView** textureArray)
{
	bool result;



	// Set the shader parameters that it will use for rendering.
	result = SetFXParameters(deviceContext, worldMatrix, viewMatrix, projectionMatrix, lightDirection, diffuseColor, ambientColor, cameraPosition, specularColor, specularPower, textureArray);
	if (!result)
	{
		return false;
	}


	// Now render the prepared buffers with the shader.
	RenderShader(deviceContext, indexCount);

	return true;
}



bool LightFxClass::InitializeShader(ID3D11Device*  device, HWND hwnd, const WCHAR* fxFileName) {

	HRESULT result;

	ID3DBlob* errorMessage=0;
	ID3DBlob* fxShaderBuffer=0;

	DWORD shaderFlags = 0;
	D3D11_INPUT_ELEMENT_DESC polygonLayout[3];
	unsigned int numElements;
	
#if defined( DEBUG ) || defined( _DEBUG )
	shaderFlags |= D3D10_SHADER_DEBUG;
	shaderFlags |= D3D10_SHADER_SKIP_OPTIMIZATION;
#endif


	result = D3DCompileFromFile((LPCWSTR)fxFileName, NULL, NULL, NULL, "fx_5_0",
		shaderFlags, 0, &fxShaderBuffer, &errorMessage);

	if (FAILED(result))
	{
		// If the shader failed to compile it should have writen something to the error message.
		if (errorMessage)
		{
			OutputShaderErrorMessage(errorMessage, hwnd, fxFileName);
		}
		// If there was  nothing in the error message then it simply could not find the shader file itself.
		else
		{
			MessageBox(hwnd, fxFileName, L"Missing fx File", MB_OK);
		}

		return false;
	}

	result = D3DX11CreateEffectFromMemory(fxShaderBuffer->GetBufferPointer(), fxShaderBuffer->GetBufferSize(),
		0, device, &m_fxShader);

	polygonLayout[0].SemanticName = "POSITION";
	polygonLayout[0].SemanticIndex = 0;
	polygonLayout[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	polygonLayout[0].InputSlot = 0;
	polygonLayout[0].AlignedByteOffset = 0;
	polygonLayout[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[0].InstanceDataStepRate = 0;

	polygonLayout[1].SemanticName = "TEXCOORD";
	polygonLayout[1].SemanticIndex = 0;
	polygonLayout[1].Format = DXGI_FORMAT_R32G32_FLOAT; // x y or u v
	polygonLayout[1].InputSlot = 0;
	polygonLayout[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	polygonLayout[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[1].InstanceDataStepRate = 0;

	polygonLayout[2].SemanticName = "NORMAL";
	polygonLayout[2].SemanticIndex = 0;
	polygonLayout[2].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	polygonLayout[2].InputSlot = 0;
	polygonLayout[2].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	polygonLayout[2].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[2].InstanceDataStepRate = 0;

	// Get a count of the elements in the layout.
	numElements = sizeof(polygonLayout) / sizeof(polygonLayout[0]);

	D3DX11_PASS_DESC passDesc;



	fxShaderBuffer->Release();

	mTech = m_fxShader->GetTechniqueByName("ColorTech");

	mfxWorldViewProj = m_fxShader->GetVariableByName("gWorldViewProj")->AsMatrix();
	mfxWorldMatrix = m_fxShader->GetVariableByName("gWorldMatrix")->AsMatrix();
	mfxViewMatrix = m_fxShader->GetVariableByName("gViewMatrix")->AsMatrix();
	mfxProjectionMatrix = m_fxShader->GetVariableByName("gProjectionMatrix")->AsMatrix();

	mfxCameraPosition = m_fxShader->GetVariableByName("gCameraPostion");


	mfxLightValue = m_fxShader->GetVariableByName("gDirLight");

	mfxShaderTexture = m_fxShader->GetVariableByName("ShaderTexture")->AsShaderResource();

	mTech->GetPassByIndex(0)->GetDesc(&passDesc);

	device->CreateInputLayout(polygonLayout, numElements, passDesc.pIAInputSignature,
		passDesc.IAInputSignatureSize, &m_layout);
	if (FAILED(result))
	{
		return false;
	}


	return true;
}







bool LightFxClass::SetFXParameters(ID3D11DeviceContext* deviceContext, XMMATRIX& worldMatrix, XMMATRIX& viewMatrix,
	XMMATRIX& projectionMatrix, XMVECTOR& lightDirection, XMVECTOR& diffuseColor, XMVECTOR& ambientColor, XMVECTOR& cameraPosition, XMVECTOR& specularColor, float specularPower, ID3D11ShaderResourceView** textureArray)
{
	HRESULT result;


	XMMATRIX worldViewProj = worldMatrix * viewMatrix*projectionMatrix;
	
	XMStoreFloat4(&LightValue.Ambient, ambientColor);
	XMStoreFloat4(&LightValue.Diffuse, diffuseColor);
	XMStoreFloat4(&LightValue.Specular, specularColor);
	XMStoreFloat3(&LightValue.LightDirection, lightDirection);
	LightValue.SpecularPower = specularPower;
	
	
	result = mfxCameraPosition->SetRawValue(&cameraPosition, 0, sizeof(cameraPosition));
	if (FAILED(result)) {
		return false;
	}

	result=mfxLightValue->SetRawValue(&LightValue, 0, sizeof(LightValue));
	if (FAILED(result)) {
		return false;
	}
	
	result = result = mfxWorldViewProj->SetMatrix(reinterpret_cast<float*>(&worldViewProj));
	if (FAILED(result)) {
		return false;
	}

	result = mfxWorldMatrix->SetMatrix(reinterpret_cast<float*>(&worldMatrix));
	if (FAILED(result)) {
		return false;
	}

	result = mfxViewMatrix->SetMatrix(reinterpret_cast<float*>(&viewMatrix));
	if (FAILED(result)) {
		return false;
	}

	result = mfxProjectionMatrix->SetMatrix(reinterpret_cast<float*>(&projectionMatrix));
	if (FAILED(result)) {
		return false;
	}

	result = mfxShaderTexture->SetResource(*textureArray);
	if (FAILED(result)) {
		return false;
	}
	
	return true;
}

void LightFxClass::RenderShader(ID3D11DeviceContext* deviceContext, int indexCount)
{



	// Set the vertex input layout.
	deviceContext->IASetInputLayout(m_layout);

	D3DX11_TECHNIQUE_DESC techDesc;
	mTech->GetDesc(&techDesc);

	for (UINT p = 0; p < techDesc.Passes; p++) {

		mTech->GetPassByIndex(p)->Apply(0, deviceContext);

		deviceContext->DrawIndexed(36, 0, 0);
	}

	

	return;
}



