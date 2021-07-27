/***************************************************************************
                          C3DObject.h  -  description
                             -------------------
    begin                : Tue Oct 14 2003
    copyright            : (C) 2003 by Eric Penner
    email                : pennere@cpsc.ucalgary.ca
 ***************************************************************************/


#ifndef C3DOBJECT_H
#define C3DOBJECT_H


#include <vector>
#include <string>

#include "Vector.h"
#include "Transform.h"
#include "Engine/Renderable.h"

using namespace std;

namespace Geometry
{


struct CFaceVertex
{
	CFaceVertex::CFaceVertex()
	{
		m_nVertexIndex = m_nNormalIndex = m_nTextureIndex = -1;
	}
		     
	int m_nVertexIndex;
	int m_nNormalIndex;
	int m_nTextureIndex;
};



typedef vector<CPoint3f>     CPointVector;
typedef vector<CFaceVertex>  CFace;
typedef vector<CFace>        CFaceVector;


class C3DObject :public CRenderable
{
public: 
	C3DObject();
	virtual ~C3DObject();

	void Render();
	void RenderSmoothFaces();
	void RenderFlatFaces();
	void RenderVertexNormals();
	void RenderFaceNormals();
	void RenderWireframe();    

	bool LoadFromObjFile(const char* szFilename);
	bool SaveToObjFile(const char* szFilename);

	void CalculateFaceNormals();
	void CalculateVertexNormals();
	void CenterOnOrigin();
	void Normalize(float fRadius);
	
	void ApplyMatrixTransform(CTransformf& M);
	
	CFace          GetFace(int nIndex);
	CPoint3f       GetVertex(int nIndex);
	
private:
	
	CPointVector   m_Vertexes;
	CPointVector   m_VertexNormals;
	
	CFaceVector    m_Faces;
	CPointVector   m_FaceNormals;
};

typedef SmartPtr<C3DObject> C3DObjectPtr;

} //namespace Geometry

#endif
