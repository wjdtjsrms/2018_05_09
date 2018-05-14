#ifndef _OBJ_PARSER_H_
#define _OBJ_PARSER_H_

#include "pch.h"



typedef struct
{
	float x, y, z;
}VertexType;

typedef struct
{
	int vIndex1, vIndex2, vIndex3;
	int tIndex1, tIndex2, tIndex3;
	int nIndex1, nIndex2, nIndex3;
}FaceType;

class  Obj_loader
{
public:
	 Obj_loader();
	 Obj_loader(const Obj_loader&);
	~ Obj_loader();

	bool Initialize(const WCHAR*);
	bool GetModel(const WCHAR*);
	bool ReadFile(const WCHAR*);
	bool LoadData(const WCHAR*);


private:
	bool GetModelFilename(const WCHAR*);
	bool ReadFileCounts(const WCHAR*, int&, int&, int&, int&);
	bool LoadDataStructures(const WCHAR*,  int, int, int, int);



private:
	VertexType *vertices, *texcoords, *normals;
	FaceType *faces;
	int vertexCount, textureCount, normalCount, faceCount;


};

#endif