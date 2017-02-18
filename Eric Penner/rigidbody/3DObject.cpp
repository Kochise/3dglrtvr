/***************************************************************************
                          C3DObject.cpp  -  description
                             -------------------
    begin                : Tue Oct 14 2003
    copyright            : (C) 2003 by Eric Penner
    email                : pennere@cpsc.ucalgary.ca
 ***************************************************************************/


#include "3DObject.h"
#include <gl/glut.h>

#include <fstream>
#include <sstream>
#include <stdio.h>

#include <math.h>

#include "Transform.h"


namespace Geometry
{

C3DObject::C3DObject()
{
}

C3DObject::~C3DObject()
{
}

CFace C3DObject::GetFace(int nIndex)
{
	return m_Faces[nIndex];
}

CPoint3f C3DObject::GetVertex(int nIndex)
{
	return m_Vertexes[nIndex];
}

void C3DObject::Render()
{
	RenderSmoothFaces();
}


void C3DObject::RenderSmoothFaces()
{
	for(int i = 0; i < (int)m_Faces.size(); i++)
	{
		glBegin(GL_POLYGON);
		for(int j = 0; j < (int)m_Faces[i].size(); j++)
		{
			glNormal3fv(m_VertexNormals[m_Faces[i][j].m_nNormalIndex]);
			glVertex3fv(m_Vertexes     [m_Faces[i][j].m_nVertexIndex]);
			
//			glNormal3fv( TransformationMatrix::ModelViewMatrix.TransformVector(m_VertexNormals[m_Faces[i][j].m_nNormalIndex]) );
//			glVertex3fv( TransformationMatrix::ProjectionMatrix.TransformPoint (TransformationMatrix::ModelViewMatrix.TransformPoint (m_Vertexes     [m_Faces[i][j].m_nVertexIndex])) );
		}
		glEnd();
	}
}

void C3DObject::RenderFlatFaces()
{
	for(int i = 0; i < (int)m_Faces.size(); i++)
	{
		glBegin(GL_POLYGON);
		glNormal3fv(m_FaceNormals[i]);
//		glNormal3fv(TransformationMatrix::ModelViewMatrix.TransformVector(m_FaceNormals[i]));
		
		for(int j = 0; j < (int)m_Faces[i].size(); j++)
		{
			glVertex3fv(m_Vertexes[m_Faces[i][j].m_nVertexIndex]);
//			glVertex3fv(TransformationMatrix::ProjectionMatrix.TransformPoint (TransformationMatrix::ModelViewMatrix.TransformPoint (m_Vertexes[m_Faces[i][j].m_nVertexIndex])));
		}
		glEnd();
	}
}



void C3DObject::RenderVertexNormals()
{
	for(int i = 0; i < (int)m_Faces.size(); i++)
	{
		for(int j = 0; j < (int)m_Faces[i].size(); j++)
		{
			glBegin(GL_LINES);
				
				glVertex3fv(m_Vertexes[m_Faces[i][j].m_nVertexIndex]);
				
				CPoint3f test = m_Vertexes[m_Faces[i][j].m_nVertexIndex] + m_VertexNormals[m_Faces[i][j].m_nNormalIndex]/10;
				
				glNormal3fv(m_FaceNormals[i]);
				glVertex3fv(test);
				
			glEnd();
			
		}
	}
}

void C3DObject::RenderFaceNormals()
{
	for(int i = 0; i < (int)m_Faces.size(); i++)
	{
		for(int j = 0; j < (int)m_Faces[i].size(); j++)
		{
			glBegin(GL_LINES);
			
				glVertex3fv(m_Vertexes[m_Faces[i][j].m_nVertexIndex]);
			
				CPoint3f test = m_Vertexes[m_Faces[i][j].m_nVertexIndex] + m_FaceNormals[i]/10;
			
				glNormal3fv(m_FaceNormals[i]);
				glVertex3fv(test);
			
			glEnd();
		
		}
	}
}

void C3DObject::RenderWireframe()
{
	glDisable(GL_LIGHTING);
	glDisable(GL_TEXTURE_2D);

	glColor3f(1,1,1);

	for(int i = 0; i < (int)m_Faces.size(); i++)
	{
		glBegin(GL_LINE_LOOP);
		
		for(int j = 0; j < (int)m_Faces[i].size(); j++)
		{
			glVertex3fv(m_Vertexes[m_Faces[i][j].m_nVertexIndex]);
			//glVertex3fv(TransformationMatrix::ProjectionMatrix.TransformPoint (TransformationMatrix::ModelViewMatrix.TransformPoint (m_Vertexes[m_Faces[i][j].m_nVertexIndex])));
		}
		
		glEnd();
	}
	
	glEnable(GL_LIGHTING);
	glEnable(GL_TEXTURE_2D);
}    



void C3DObject::CenterOnOrigin()
{
	double dMinX, dMinY, dMinZ, dMaxX, dMaxY, dMaxZ;
	
	dMinX = dMinY = dMinZ =  1000000;
	dMaxX = dMaxY = dMaxZ = -1000000;
	
	for(int i = 0; i < (int)m_Vertexes.size(); i++)
	{
		if(m_Vertexes[i].GetX() < dMinX) dMinX = m_Vertexes[i].GetX();
		if(m_Vertexes[i].GetY() < dMinY) dMinY = m_Vertexes[i].GetY();
		if(m_Vertexes[i].GetZ() < dMinZ) dMinZ = m_Vertexes[i].GetZ();
		
		if(m_Vertexes[i].GetX() > dMaxX) dMaxX = m_Vertexes[i].GetX();
		if(m_Vertexes[i].GetY() > dMaxY) dMaxY = m_Vertexes[i].GetY();
		if(m_Vertexes[i].GetZ() > dMaxZ) dMaxZ = m_Vertexes[i].GetZ();
	}
	
	CPoint3f CenterPoint((dMaxX+dMinX)/2,(dMaxY+dMinY)/2,(dMaxZ+dMinZ)/2);
	
	for(int i = 0; i < (int)m_Vertexes.size(); i++)
	{
		m_Vertexes[i] = m_Vertexes[i] - CenterPoint;
	}
}

void C3DObject::Normalize(float fRadius)
{
	float fMaxSize = 0;
	
	for(int i = 0; i < (int)m_Vertexes.size(); i++)
	{
		double dX = m_Vertexes[i].GetX();
		double dY = m_Vertexes[i].GetY();
		double dZ = m_Vertexes[i].GetZ();
		
		dX = (dX < 0)? -dX : dX;
		dY = (dY < 0)? -dY : dY;
		dZ = (dZ < 0)? -dZ : dZ;
		
		if(dX > fMaxSize) fMaxSize = dX;
		if(dY > fMaxSize) fMaxSize = dY;
		if(dZ > fMaxSize) fMaxSize = dZ;
	}

	for(int i = 0; i < (int)m_Vertexes.size(); i++)
	{
		m_Vertexes[i] = (m_Vertexes[i] / fMaxSize) * fRadius;
	}
}



void C3DObject::CalculateFaceNormals()
{
	m_FaceNormals.clear();
	m_FaceNormals.reserve(m_Faces.size());

	for(int i = 0; i < (int)m_Faces.size(); i++)
	{
		CFace Face = m_Faces[i];
		
		CVector3f V1 = m_Vertexes[Face[2].m_nVertexIndex] - m_Vertexes[Face[1].m_nVertexIndex];
		CVector3f V2 = m_Vertexes[Face[0].m_nVertexIndex] - m_Vertexes[Face[1].m_nVertexIndex];
	
		m_FaceNormals.push_back(CVector3f::Normalize(CVector3f::Cross(V1,V2)));
	}
}


void C3DObject::CalculateVertexNormals()
{
	m_VertexNormals.clear();
	m_VertexNormals.reserve(m_Vertexes.size());
	m_VertexNormals.insert(m_VertexNormals.begin(),m_Vertexes.size(),CPoint3f(0,0,0));
	
	for(int i = 0; i < (int)m_Faces.size(); i++)
	{
		for(int j = 0; j < (int)m_Faces[i].size(); j++)
		{
			 m_VertexNormals[m_Faces[i][j].m_nVertexIndex] = m_VertexNormals[m_Faces[i][j].m_nVertexIndex] +
			                                                 m_FaceNormals[i];
		}
	}
	
	for(int i = 0; i < (int)m_VertexNormals.size(); i++)
	{
		m_VertexNormals[i] = CPoint3f::Normalize(m_VertexNormals[i]);
	}
	
	//Since each vertex now has a single normal vector, we assign the normal index
	//to be the same as the vertex index for each face vertex.
	for(int i = 0; i < (int)m_Faces.size(); i++)
	{
		for(int j = 0; j < (int)m_Faces[i].size(); j++)
		{
			m_Faces[i][j].m_nNormalIndex = m_Faces[i][j].m_nVertexIndex;
		}
	}
}




bool C3DObject::SaveToObjFile(const char* szFilename)
{
	ofstream outputStream(szFilename);
	
	if(!outputStream)
	{
		return false;
	}
	
	for(int i = 0; i < (int)m_Vertexes.size(); i++)
	{
		outputStream << "v ";
		outputStream << m_Vertexes[i].GetX() << " ";
		outputStream << m_Vertexes[i].GetY() << " ";
		outputStream << m_Vertexes[i].GetZ() << "\n";
	}
	
	for(int i = 0; i < (int)m_Faces.size(); i++)
	{
		outputStream << "f ";
		
		for(int j = 0; j < (int)m_Faces[i].size(); j++)
		{
			outputStream << m_Faces[i][j].m_nVertexIndex + 1 << " ";
		}
		
		outputStream << "\n";
	}
	
	outputStream.close();

	return true;
}

bool C3DObject::LoadFromObjFile(const char* szFilename)
{
	ifstream inputStream(szFilename);
	
	if(!inputStream)
	{
		return false;
	}
	
	m_Vertexes.clear();
	m_VertexNormals.clear();
	m_Faces.clear();
	
	char szLine[1024];
	
	while(!inputStream.eof())
	{
		string strLineType;
		
		inputStream.getline(szLine,1024);
		stringstream lineStream(szLine,stringstream::in);
		
		lineStream >> strLineType;
			
		if(strLineType == "v")
		{
			float dX,dY,dZ;
			
			lineStream >> dX >> dY >> dZ;
			
			m_Vertexes.push_back(CPoint3f(dX,dY,dZ));
		}

		else if(strLineType == "vn")
		{
			float dX,dY,dZ;
			
			lineStream >> dX >> dY >> dZ;
			
			m_VertexNormals.push_back(CPoint3f(dX,dY,dZ));
		}
		
		else if(strLineType == "f")
		{
			CFace Face;
			
			while(!lineStream.eof())
			{
				string strVertex;
				lineStream >> strVertex;
				
				if(strVertex == "")
				{
					continue;
				}
				
				int nSlash = -1;
				
				if( (nSlash = (int)strVertex.find("//")) >= 0)
				{
					strVertex.replace(nSlash,2,"/-1/");
				}
				
				while( (nSlash = (int)strVertex.find("/")) >= 0)
				{
					strVertex[nSlash] = ' ';
				}
				
				stringstream VertexStream(strVertex,stringstream::in);
				CFaceVertex FaceVertex;
				
				if(!VertexStream.eof()) {VertexStream >> FaceVertex.m_nVertexIndex;  FaceVertex.m_nVertexIndex--; }
				if(!VertexStream.eof()) {VertexStream >> FaceVertex.m_nTextureIndex; FaceVertex.m_nTextureIndex--;}
				if(!VertexStream.eof()) {VertexStream >> FaceVertex.m_nNormalIndex;  FaceVertex.m_nNormalIndex--; }
				
				Face.push_back(FaceVertex);
			}
			
			m_Faces.push_back(Face);
		}
		
	}
	
	CalculateFaceNormals();
	
	if(m_VertexNormals.size() == 0)
	{
		CalculateVertexNormals();
	}
	
	/*
	for(int i = 0; i < m_Faces.size() ; i++)
	{
		printf("Face: ");
		for(int j = 0; j < m_Faces[i].size(); j++)
		{
			printf (" %d ", m_Faces[i][j]);
		}
		printf("\n");
	}
	*/

	return true;
}


void C3DObject::ApplyMatrixTransform(CTransformf& M)
{
	for(int i = 0; i < (int)m_Vertexes.size(); i++)
		m_Vertexes[i] =  M.TransformPoint(m_Vertexes[i]);
	
	CalculateFaceNormals();
	CalculateVertexNormals();
}






	/*

int C3DObject::RemoveLargestFace()
{
	double dLargestIndex = -1;
	double dLargestValue = -1;

	for(int i = 0; i < m_Faces.size(); i++)
	{
		double dLength = 0;
		dLength += (m_Vertexes[m_Faces[i][0].m_nVertexIndex] - m_Vertexes[m_Faces[i][1].m_nVertexIndex]).Magnitude();
		dLength += (m_Vertexes[m_Faces[i][1].m_nVertexIndex] - m_Vertexes[m_Faces[i][2].m_nVertexIndex]).Magnitude();
		dLength += (m_Vertexes[m_Faces[i][2].m_nVertexIndex] - m_Vertexes[m_Faces[i][0].m_nVertexIndex]).Magnitude();
		
		if(dLength > dLargestValue)
		{
			dLargestValue = dLength;
			dLargestIndex = i;
		}
	}

	m_Faces[dLargestIndex] = m_Faces[0];
	m_FaceNormals[dLargestIndex] = m_FaceNormals[0];

	return dLargestIndex;
}




	ofstream outputStream("C:\\faces.txt");
	outputStream << "Face 1:  ";

	for(int i = 0; i < m_Faces[648].size(); i++)
	{
		outputStream << m_Faces[648][i].m_nVertexIndex << " , " ;	
	}

	outputStream << "Face 2:  ";
	
	for(int i = 0; i < m_Faces[649].size(); i++)
	{
		outputStream << m_Faces[649][i].m_nVertexIndex << " , " ;	
	}


	outputStream.close();



	
	m_Faces[648] = m_Faces[0];
	m_FaceNormals[648] = m_FaceNormals[0];

	m_Faces[649] = m_Faces[0];
	m_FaceNormals[649] = m_FaceNormals[0];




	this->RemoveLargestFace();
	this->RemoveLargestFace();
	this->RemoveLargestFace();
	this->RemoveLargestFace();
	this->RemoveLargestFace();

	int n1 = this->RemoveLargestFace();

	this->RemoveLargestFace();
	
	int n2 = this->RemoveLargestFace();

	ofstream outputStream("C:\\faces.txt");
	outputStream << "Face 1: " << n1 << "  ";
	outputStream << "Face 2: " << n2 << "  ";
	outputStream.close();

	*/

} //namespace Geometry

