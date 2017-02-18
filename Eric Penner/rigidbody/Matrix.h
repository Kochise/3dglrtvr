/***************************************************************************
                         Matrix.h  -  description
                             -------------------
    begin                : Tue Sep 23 2003
    copyright            : (C) 2003 by pennere
    email                : pennere@cpsc.ucalgary.ca

 The ultimate (waste of time) Matrix class.
   - Templated for any internal type.
   - Appropriate operators are overloaded.

 ***************************************************************************/

#ifndef MATRIX_H
#define MATRIX_H

#include <stdio.h>

namespace Geometry
{


template<typename InternalType,int nRows,int nCols>
class CMatrix
{
public:
	
	CMatrix();
	CMatrix(const InternalType* pData) { SetMatrix(pData);}
	virtual ~CMatrix(){}
	
	InternalType GetValue(int nRow, int nCol) const              { return m_ppInternalMatrix[nRow][nCol]; }
	void         SetValue(int nRow, int nCol, InternalType tNew) { m_ppInternalMatrix[nRow][nCol] = tNew; }
	
	void SetMatrix(const InternalType* pData);
	const CMatrix<InternalType,nRows,nCols> operator=(const InternalType* pData) { SetMatrix(pData); return *this;}
	
	CMatrix<InternalType,nCols,nRows> Transpose();
	
	//Cast operator to return an array of the internal type. Mainly for OpenGL. Keep in mind we need to
	//transpose first for OpenGL, since items are stored in column-major order
	operator InternalType* const()
	{
		return (InternalType*)m_ppInternalMatrix;
	}

	void print()
	{
		for(int i = 0; i < nRows; i++)
		{
			for(int j = 0; j < nCols; j++)
				printf(" %f ", (double) m_ppInternalMatrix[i][j]);
			
			printf("\n");
		}
		
		printf("\n");
	}
	
	
//protected:
	
	static const int m_nRows    = nRows;
	static const int m_nCols    = nCols;
	static const int m_nEntries = nRows * nCols;
	
	InternalType  m_ppInternalMatrix[nRows][nCols];
};


template<typename InternalType,int nRows, int nCols>
CMatrix<InternalType,nRows,nCols>::CMatrix()
{	
	for(int i = 0; i < m_nRows; i++)
	{
		for(int j = 0; j < m_nCols; j++)
		{
			if(i == j) m_ppInternalMatrix[i][j] = 1;
			else       m_ppInternalMatrix[i][j] = 0;
		}
	}
}

template<typename InternalType,int nRows, int nCols>
void CMatrix<InternalType,nRows,nCols>::SetMatrix(const InternalType* pData)
{
	for(int i = 0, n = nRows*nCols; i < n; i++)
	{
		((InternalType*)m_ppInternalMatrix)[i] = pData[i];
	}
}

template<typename InternalType,int nRows, int nCols>
CMatrix<InternalType,nCols,nRows> CMatrix<InternalType,nRows,nCols>::Transpose()
{	
	CMatrix<InternalType,nCols,nRows> NewMatrix;

	for(int i = 0; i < nRows; i++)
		for(int j = 0; j < nCols; j++)
			NewMatrix.m_ppInternalMatrix[j][i] = m_ppInternalMatrix[i][j];

	return NewMatrix;
}

template<typename InternalType,int nRows, int nCols>
const CMatrix<InternalType,nRows,nCols> operator+(const CMatrix<InternalType,nRows,nCols>& M1,
                                                  const CMatrix<InternalType,nRows,nCols>& M2)
{
	CMatrix<InternalType,nRows,nCols> NewMatrix;
	
	for(int i = 0; i < nRows; i++)
	{
		for(int j = 0; j < nCols; j++)
		{
			NewMatrix.SetValue(i,j, M1.GetValue(i,j) + M2.GetValue(i,j));
		}
	}
	
	return NewMatrix;
}


template<typename InternalType,int nM1Rows,int nM1Cols_M2Rows,int nM2Cols>
const CMatrix<InternalType,nM1Rows,nM2Cols> operator*(const CMatrix<InternalType,nM1Rows,nM1Cols_M2Rows>& M1,
                                                      const CMatrix<InternalType,nM1Cols_M2Rows,nM2Cols>& M2)
{
	CMatrix<InternalType,nM1Rows,nM2Cols> NewMatrix;
	
	for(int i = 0; i < nM1Rows; i++)
	{
		for(int j = 0; j < nM2Cols; j++)
		{
			InternalType tValue = 0;
			
			for(int k = 0; k < nM1Cols_M2Rows; k++)
				tValue += M1.GetValue(i,k) * M2.GetValue(k,j);
			
			NewMatrix.SetValue(i,j,tValue);
		}
	}
	
	return NewMatrix;
}


}
//namespace Geometry


#endif
