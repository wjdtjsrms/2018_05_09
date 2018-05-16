////////////////////////////////////////////////////////////////////////////////
// Filename: modelclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _MODELCLASS_H_
#define _MODELCLASS_H_


//////////////
// INCLUDES //
//////////////



#include "pch.h"

#include "textureclass.h"
#include "obj_parser.h"
#include "GeometryGenerator.h"




////////////////////////////////////////////////////////////////////////////////
// Class name: ModelClass
////////////////////////////////////////////////////////////////////////////////
class ModelClass
{
private:
	struct VertexType
	{
		DirectX::XMFLOAT3 position;
;		DirectX::XMFLOAT2 texture;
		DirectX::XMFLOAT3 normal;
		DirectX::XMFLOAT4 Color;

	};
	struct ModelType
	{
		float x, y, z;
		float tu, tv;
		float nx, ny, nz;
	};
public:
	struct InstanceData
	{
		InstanceData()
			:VertexOffset(0),
			IndexOffset(0),
			IndexCount(0)
		{}


		int VertexOffset;
		UINT IndexOffset;
		UINT IndexCount;
	};



public:
	ModelClass();
	ModelClass(const ModelClass&);
	~ModelClass();

	bool Initialize(ID3D11Device*, const WCHAR*, const WCHAR*, const WCHAR*, const WCHAR*);
	void Shutdown();
	
	void Render(ID3D11DeviceContext*);
	int GetIndexCount();
	int GetGridIndexCount();
	ID3D11ShaderResourceView** GetTexture();

	void GetBoxeData(InstanceData&) const;
	void GetGrideData(InstanceData&) const;
	void GetSphereeData(InstanceData&) const;
	void GetCylindereData(InstanceData&) const;

private:
	bool InitializeBuffers(ID3D11Device*);
	void ShutdownBuffers();
	void RenderBuffers(ID3D11DeviceContext*);

	bool LoadTexture(ID3D11Device*, const WCHAR*, const WCHAR*);
	void ReleaseTexture();

	bool LoadModel(const WCHAR*);
	void ReleaseModel();

	bool LoadParser(const WCHAR*);
	void ReleaseParser();
	
	bool BuildGeometryBuffers(ID3D11Device*);

	float GetHeight(float,float) const;




private:
	ID3D11Buffer *m_vertexBuffer, *m_indexBuffer;
	int m_vertexCount, m_indexCount;
	TextureClass* m_Texture;
	ModelType* m_model;
	Obj_loader* m_parser;

	InstanceData m_BoxData;
	InstanceData m_GridData;
	InstanceData m_SphereData;
	InstanceData m_CylinderData;


};

#endif