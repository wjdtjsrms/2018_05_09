////////////////////////////////////////////////////////////////////////////////
// Filename: modelclass.cpp
////////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "../Header_file/modelclass.h"

using namespace std;
using namespace DirectX;

ModelClass::ModelClass()
	:m_vertexBuffer(0),
	m_indexBuffer(0),
	m_Texture(0),
	m_model(0),
	m_parser(0)
{}
// 명시해주는 이유는 기본 복사 생성자가 생성되지 말라고
ModelClass::ModelClass(const ModelClass& other)
{
}


ModelClass::~ModelClass()
{
}


bool ModelClass::Initialize(ID3D11Device* device, const WCHAR* objFileName, const WCHAR* modelFilename, const WCHAR* textureFilename1, const WCHAR* textureFilename2)
{
	bool result;

	result = LoadParser(objFileName);
	if (!result)
	{
		return false;
	}

	//Load model data
	result = LoadModel(modelFilename);
	if (!result)
	{
		return false;
	}

	// Initialize the vertex and index buffers.
	//result = InitializeBuffers(device);
	//if(!result)
	//{
	//	return false;
	//}

	result = BuildGeometryBuffers(device);
	if (!result)
	{
		return false;
	}

	result = LoadTexture(device, textureFilename1, textureFilename2);
	if (!result)
	{
		return false;
	}

	return true;
}


void ModelClass::Shutdown()
{
	// Release the model texture.
	ReleaseTexture();

	// Shutdown the vertex and index buffers.
	ShutdownBuffers();

	// Release the model data.
	ReleaseModel();


	ReleaseParser();
	

	return;
}


void ModelClass::Render(ID3D11DeviceContext* deviceContext)
{
	// Put the vertex and index buffers on the graphics pipeline to prepare them for drawing.
	RenderBuffers(deviceContext);

	return;
}

//여기가 중요 포인트
//여기에요 떡대 분들
int ModelClass::GetIndexCount()
{
	return m_indexCount;
}

int ModelClass::GetGridIndexCount()
{
	return m_GridData.IndexCount;
}

ID3D11ShaderResourceView** ModelClass::GetTexture(){

	return m_Texture->GetTexture();
}


bool ModelClass::InitializeBuffers(ID3D11Device* device)
{
	VertexType* vertices;
	unsigned long* indices;
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
    D3D11_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result;
	int i;

	// Create the vertex array.
	vertices = new VertexType[m_vertexCount];
	if(!vertices)
	{
		return false;
	}

	// Create the index array.
	indices = new unsigned long[m_indexCount];
	if(!indices)
	{
		return false;
	}

	// Load the vertex array and index array with data.
	for (i = 0; i<m_vertexCount; i++)
	{
		vertices[i].position = XMFLOAT3(m_model[i].x, m_model[i].y, m_model[i].z);
		vertices[i].texture = XMFLOAT2(m_model[i].tu, m_model[i].tv);
		vertices[i].normal = XMFLOAT3(m_model[i].nx, m_model[i].ny, m_model[i].nz);


		vertices[i].Color = XMFLOAT4(0.48f, 0.77f, 0.46f, 1.0f);;

		indices[i] = i;
	}
	

	// Set up the description of the static vertex buffer.
    vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
    vertexBufferDesc.ByteWidth = sizeof(VertexType) * m_vertexCount;
    vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    vertexBufferDesc.CPUAccessFlags = 0;
    vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the vertex data.
    vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	// Now create the vertex buffer.
    result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &m_vertexBuffer);
	if(FAILED(result))
	{
		return false;
	}

	// Set up the description of the static index buffer.
    indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
    indexBufferDesc.ByteWidth = sizeof(unsigned long) * m_indexCount;
    indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
    indexBufferDesc.CPUAccessFlags = 0;
    indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the index data.
    indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	// Create the index buffer.
	result = device->CreateBuffer(&indexBufferDesc, &indexData, &m_indexBuffer);
	if(FAILED(result))
	{
		return false;
	}

	// Release the arrays now that the vertex and index buffers have been created and loaded.
	delete [] vertices;
	vertices = 0;

	delete [] indices;
	indices = 0;

	return true;
}


void ModelClass::ShutdownBuffers()
{
	// Release the index buffer.
	if(m_indexBuffer)
	{
		m_indexBuffer->Release();
		m_indexBuffer = 0;
	}

	// Release the vertex buffer.
	if(m_vertexBuffer)
	{
		m_vertexBuffer->Release();
		m_vertexBuffer = 0;
	}

	return;
}


void ModelClass::RenderBuffers(ID3D11DeviceContext* deviceContext)
{
	unsigned int stride;
	unsigned int offset;


	// Set vertex buffer stride and offset.
	stride = sizeof(VertexType); 
	offset = 0;
    
	// Set the vertex buffer to active in the input assembler so it can be rendered.
	deviceContext->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);

    // Set the index buffer to active in the input assembler so it can be rendered.
	deviceContext->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);

    // Set the type of primitive that should be rendered from this vertex buffer, in this case triangles.
	deviceContext->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);



	return;
}

bool ModelClass::LoadTexture(ID3D11Device* device, const WCHAR* filename1, const WCHAR* filename2){
	bool result;

	m_Texture = new TextureClass;
	if (!m_Texture){

		return false;
	}
	result = m_Texture->Initialize(device, filename1, filename2);
	if (!result)
	{
		return false;
	}

	return true;

}

void ModelClass::ReleaseTexture()
{
	// Release the texture object.
	if (m_Texture){
		m_Texture->Shutdown();
		delete m_Texture;
		m_Texture = 0;
	}

	return;
}

bool ModelClass::LoadModel(const WCHAR* filename)
{
	ifstream fin;
	char input;
	int i;

	// Open the model file.
	fin.open(filename);

	// If it could not open the file then exit.
	if (fin.fail())
	{
		return false;
	}

	// Read up to the value of vertex count.
	fin.get(input);
	while (input != ':')
	{
		fin.get(input);
	}

	fin >> m_vertexCount;

	// Set the number of indices to be the same as the vertex count.
	m_indexCount = m_vertexCount;

	// Create the model using the vertex count that was read in.
	m_model = new ModelType[m_vertexCount];
	if (!m_model)
	{
		return false;
	}

	// Read up to the beginning of the data.
	fin.get(input);
	while (input != ':')
	{
		fin.get(input);
	}
	fin.get(input);
	fin.get(input);

	// Read in the vertex data.
	for (i = 0; i<m_vertexCount; i++)
	{
		fin >> m_model[i].x >> m_model[i].y >> m_model[i].z;
		fin >> m_model[i].tu >> m_model[i].tv;
		fin >> m_model[i].nx >> m_model[i].ny >> m_model[i].nz;
	}

	fin.close();

	return true;
}

void ModelClass::ReleaseModel()
{
	if (m_model)
	{
		delete[] m_model;
		m_model = 0;
	}

	return;
}

bool ModelClass::LoadParser(const WCHAR* FileName){

	bool result;

	m_parser = new Obj_loader;

	if (!m_parser){
		return false;
	}

	result = m_parser->Initialize(FileName);

	if (!result)
	{
		return false;
	}

	result=m_parser->LoadData(FileName);

	if (!result)
	{
		return false;
	}

	return true;

}

void ModelClass::ReleaseParser()
{
	if (m_parser)
	{
		//delete[] m_parser; Deleted twice
		m_parser = 0;
	}

	return;
}


float ModelClass::GetHeight(float x, float z)const
{
	return 0.3f*(z*sinf(0.1f*x) + x * cosf(0.1f*z));
}



bool ModelClass::BuildGeometryBuffers(ID3D11Device* device) {

	GeometryGenerator::MeshData box;
	GeometryGenerator::MeshData sphere;
	GeometryGenerator::MeshData grid;
	GeometryGenerator::MeshData cylinder;

	GeometryGenerator geoGen;
	
	geoGen.CreateBox(1.0f, 1.0f, 1.0f, box);
	geoGen.CreateGrid(20.0f, 5.0f, 30, 20, grid);
	geoGen.CreateSphere(0.5f, 20, 20, sphere);
	geoGen.CreateCylinder(0.5f, 0.3f, 3.0f, 20, 20, cylinder);

	VertexType* vertices;
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result;
	unsigned int i;


	m_BoxData.VertexOffset = 0;
	m_GridData.VertexOffset = box.Vertices.size();
	m_SphereData.VertexOffset = m_GridData.VertexOffset + grid.Vertices.size();
	m_CylinderData.VertexOffset = m_SphereData.VertexOffset + sphere.Vertices.size();

	m_BoxData.IndexCount = box.Indices.size();
	m_GridData.IndexCount = grid.Indices.size();
	m_SphereData.IndexCount = sphere.Indices.size();
	m_CylinderData.IndexCount = cylinder.Indices.size();

	m_BoxData.IndexOffset = 0;
	m_GridData.IndexOffset = m_BoxData.IndexCount;
	m_SphereData.IndexOffset = m_GridData.IndexOffset + m_GridData.IndexCount;
	m_CylinderData.IndexOffset = m_SphereData.IndexOffset + m_SphereData.IndexCount;

	UINT totalVertexCount =
		box.Vertices.size() +
		grid.Vertices.size() +
		sphere.Vertices.size() +
		cylinder.Vertices.size();

	UINT totalIndexCount =
		m_BoxData.IndexCount +
		m_GridData.IndexCount +
		m_SphereData.IndexCount +
		m_CylinderData.IndexCount;



	// Create the vertex array.
	vertices = new VertexType[totalVertexCount];
	if (!vertices)
	{
		return false;
	}

	unsigned int k = 0;

	// Load the vertex array and index array with data.

	for (i = 0; i<box.Vertices.size(); i++, k++)
	{
		vertices[k].position = box.Vertices[i].Position;
		vertices[k].texture = box.Vertices[i].TexC;
		vertices[k].normal = box.Vertices[i].Normal;
		vertices[k].Color = XMFLOAT4(0.48f, 0.77f, 0.46f, 1.0f);
	}

	for (i = 0; i<grid.Vertices.size(); i++, k++)
	{
		vertices[k].position = grid.Vertices[i].Position;
		vertices[k].texture = grid.Vertices[i].TexC;
		vertices[k].normal = grid.Vertices[i].Normal;
		vertices[k].Color = XMFLOAT4(0.48f, 0.77f, 0.46f, 1.0f);
	}

	for (i = 0; i<sphere.Vertices.size(); i++, k++)
	{
		vertices[k].position = sphere.Vertices[i].Position;
		vertices[k].texture = sphere.Vertices[i].TexC;
		vertices[k].normal = sphere.Vertices[i].Normal;
		vertices[k].Color = XMFLOAT4(0.48f, 0.77f, 0.46f, 1.0f);
	}

	for (i = 0; i<cylinder.Vertices.size(); i++, k++)
	{
		vertices[k].position = cylinder.Vertices[i].Position;
		vertices[k].texture = cylinder.Vertices[i].TexC;
		vertices[k].normal = cylinder.Vertices[i].Normal;
		vertices[k].Color = XMFLOAT4(0.48f, 0.77f, 0.46f, 1.0f);
	}
	
	// Set up the description of the static vertex buffer.
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(VertexType) * totalVertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the vertex data.
	vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	// Now create the vertex buffer.
	result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &m_vertexBuffer);
	if (FAILED(result))
	{
		return false;
	}

	std::vector<UINT> indices;
	indices.insert(indices.end(), box.Indices.begin(), box.Indices.end());
	indices.insert(indices.end(), grid.Indices.begin(), grid.Indices.end());
	indices.insert(indices.end(), sphere.Indices.begin(), sphere.Indices.end());
	indices.insert(indices.end(), cylinder.Indices.begin(), cylinder.Indices.end());


	// Set up the description of the static index buffer.
	indexBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
	indexBufferDesc.ByteWidth = sizeof(UINT) * totalIndexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the index data.
	indexData.pSysMem = &indices[0];
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	// Create the index buffer.
	result = device->CreateBuffer(&indexBufferDesc, &indexData, &m_indexBuffer);
	if (FAILED(result))
	{
		return false;
	}

	// Release the arrays now that the vertex and index buffers have been created and loaded.
	delete[] vertices;
	vertices = 0;


	return true;
}

void ModelClass::GetBoxeData(InstanceData& data) const{

	data = m_BoxData;

	return;
}

void ModelClass::GetGrideData(InstanceData& data) const {

	data = m_GridData;

	return;
}

void ModelClass::GetSphereeData(InstanceData& data) const {

	data = m_SphereData;

	return;
}

void ModelClass::GetCylindereData(InstanceData& data) const {

	data = m_CylinderData;

	return;
}