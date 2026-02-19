// DgnCalcCutlLine.cpp: implementation of the CDgnCalcCutlLine class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DgnCalcCutlLine.h"

#include "..\wg_db\wg_db_MathFunc.h"
#include "..\wg_db\wg_db_MathLib.h"

#include "..\..\dgnengine\idesign\DGN_lib\DGNCompare.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDgnCalcCutlLine::CDgnCalcCutlLine()
{
    m_dLenTol = 1.0e-07;
}

CDgnCalcCutlLine::~CDgnCalcCutlLine()
{

}

// PMS:4020-Seungjun-20090711 EC2:04 Slab Design.
// This class transferd from SDS.

void  CDgnCalcCutlLine::Initialize()
{
	m_arPoint.RemoveAll();
	m_arPolygon.RemoveAll();
    m_dLenTol = 1.0e-07;
}

void  CDgnCalcCutlLine::AddData(double x,double y,double z,double value)
{
	T_CALC_CUT_DATA Data;
	Data.Pt.dX = x;
	Data.Pt.dY = y;
	Data.Pt.dZ = z;
	Data.dVal  = value;
	m_arPolygon.Add(Data);
}

void  CDgnCalcCutlLine::AddLine(double x,double y,double z)
{
	T_CALC_CUT_PONT Data; 
	Data.dX = x; 
	Data.dY = y; 
	Data.dZ = z; 
	m_arPoint.Add(Data); 
}


BOOL  CDgnCalcCutlLine::CalcCultLine(double dX[2],double dY[2],double dZ[2],double dVal[2])
{
	// Data에 Error가 있다면 
	if(IsErrorData()) return FALSE; 

	// Line이 Polygon을 Cutting하는 경우 
	if(IsPolygonCut(dX,dY,dZ,dVal))
	{
		if(IsSame3DPoint(dX,dY,dZ))return FALSE; 
		MakeIdenticalDirection(dX,dY,dZ,dVal);
		return TRUE; 
	}

	// Line이 Polygon과 어떤 관계인지 상태를 얻어옴 
	BOOL bInclude[2];
	int nState=IsStateLineAndPoly(bInclude);

	switch(nState)
	{
	// Line이 Polygon안에 포함되지 않음
	case LNPL_STATE_NOT_INCLUDE:
		return FALSE; 
		break; 

	case LNPL_STATE_ONE_POINT:// Line의하나의 점만이 Polygon에 포함 
	case LNPL_STATE_TWO_POINT:// Line의 두점모두 Polygon에 포함 
		if(CalcCutlDataExtendedLine(dX,dY,dZ,dVal))
		{
			if(IsSame3DPoint(dX,dY,dZ))return FALSE; 
			MakeIdenticalDirection(dX,dY,dZ,dVal);
			return TRUE;
		}
		else 
			return FALSE; 
		break;

	default:
		ASSERT(FALSE);
		return FALSE; 
		break; 
	}
	return TRUE; 
}

BOOL  CDgnCalcCutlLine::IsErrorData()
{
	if(!m_arPoint.GetSize())return TRUE; 
	if(!m_arPolygon.GetSize())return TRUE; 
	return FALSE; 
}

/*-------------------------------------------------------------------------------------------
* DATE : 2003.1.7. by KYE-HONG
* FUNC : Polygon을 Cutting하는 Line인가?
-------------------------------------------------------------------------------------------*/
BOOL  CDgnCalcCutlLine::IsPolygonCut(double dX[2],double dY[2],double dZ[2],
																	double dCutVals[2],BOOL bExtendedLine/*=FALSE*/)
{
	int i,j;
	BOOL   bCutResult;
	double dCutVal,dVal[2];
	double dInDist[2];
	double dTmpDist,dTtlDist;
	double dPoint1[3],dPoint2[3],dPoint3[3];
	double dCutLinePoint1[3],dCutLinePoint2[3];
	T_CALC_CUT_PONT CutlP;

	CArray<double,double> aCutValues;
	CArray<T_CALC_CUT_PONT,T_CALC_CUT_PONT> aCutPoints;

	if(m_arPoint.GetSize()!=2)
	{
		ASSERT(FALSE);
		return FALSE; 
	}
	dCutLinePoint1[0]=m_arPoint[0].dX;
	dCutLinePoint1[1]=m_arPoint[0].dY;
	dCutLinePoint1[2]=m_arPoint[0].dZ;
	dCutLinePoint2[0]=m_arPoint[1].dX;
	dCutLinePoint2[1]=m_arPoint[1].dY;
	dCutLinePoint2[2]=m_arPoint[1].dZ;
	
	for(i=0; i<m_arPolygon.GetSize(); i++)
	{
		j = (i==m_arPolygon.GetSize()-1)? 0:(i+1);
				
		dPoint1[0] = m_arPolygon[i].Pt.dX;
		dPoint1[1] = m_arPolygon[i].Pt.dY;
		dPoint1[2] = m_arPolygon[i].Pt.dZ;

		dPoint2[0] = m_arPolygon[j].Pt.dX;
		dPoint2[1] = m_arPolygon[j].Pt.dY;
		dPoint2[2] = m_arPolygon[j].Pt.dZ;

		if(bExtendedLine)
			bCutResult = CMathFunc::mathIntersectLine(dCutLinePoint1,dCutLinePoint2,dPoint1,dPoint2,1e-7,dTmpDist,dPoint3);
        else
        {
            bCutResult = CMathFunc::mathIntersectLine2(dCutLinePoint1, dCutLinePoint2, dPoint1, dPoint2, 1e-7, dTmpDist, dPoint3);
            if (!bCutResult)
            {
                const auto dDist1 = CMathFunc::mathDistanceToLine(dCutLinePoint1, dCutLinePoint2, dPoint1);
                const auto dDist2 = CMathFunc::mathDistanceToLine(dCutLinePoint1, dCutLinePoint2, dPoint2);
                if (dgn::EQ0(dDist1, m_dLenTol) && dgn::UQ0(dDist2, m_dLenTol))
                {
                    bCutResult = true;
                    memcpy(dPoint3, dPoint1, sizeof(double) * 3);
                }
                else if (dgn::UQ0(dDist1, m_dLenTol) && dgn::EQ0(dDist2, m_dLenTol))
                {
                    bCutResult = true;
                    memcpy(dPoint3, dPoint2, sizeof(double) * 3);
                }
            }
        }

		if(bCutResult)
		{
			
			dVal[0]=m_arPolygon[i].dVal;
			dVal[1]=m_arPolygon[j].dVal;
			dInDist[0] = CMathFunc::mathLength(dPoint3[0],dPoint3[1],dPoint3[2],
						dPoint1[0],dPoint1[1],dPoint1[2]);
			dInDist[1] = CMathFunc::mathLength(dPoint3[0],dPoint3[1],dPoint3[2],
						dPoint2[0],dPoint2[1],dPoint2[2]);

			dTtlDist = dInDist[0]+dInDist[1];
			if(dTtlDist==0.0)continue; 
				
			dCutVal = dVal[0] + (dVal[1]-dVal[0])*dInDist[0]/(dTtlDist);
			CutlP.dX = dPoint3[0];
			CutlP.dY = dPoint3[1];
			CutlP.dZ = dPoint3[2];

			aCutPoints.Add(CutlP);
			aCutValues.Add(dCutVal);
		}
	}

	// 같은 점이 있을 경우 없앤다.
	if(aCutPoints.GetSize()>1)
	for( i =0; i<aCutPoints.GetSize()-1; i++)
	for( j=i+1; j<aCutPoints.GetSize(); j++)
	{
		if(IsSame3DPoint(aCutPoints[i],aCutPoints[j]))
		{
			aCutPoints.RemoveAt(j);
			aCutValues.RemoveAt(j);
			j--;
		}
	}	
	else return FALSE; 

	if(aCutPoints.GetSize()==2 && aCutValues.GetSize()==2)
	{
		for(i=0; i<2; i++)
		{
			dX[i]=aCutPoints[i].dX;
			dY[i]=aCutPoints[i].dY;
			dZ[i]=aCutPoints[i].dZ;
			dCutVals[i]=aCutValues[i];
		}
		return TRUE; 
	}
	return FALSE; 
}

/*-------------------------------------------------------------------------------------------
* DATE : 2003.1.8. by KYE-HONG
* FUNC : Line이 Polygon안에 포함되는 경우 Line을 확장해서 Cutting Points를 구하고..
				 이것을 보간해서 Line양단의 값을 구함       
-------------------------------------------------------------------------------------------*/
BOOL CDgnCalcCutlLine::CalcCutlDataExtendedLine(double dX[2],double dY[2],double dZ[2],double dCutVals[2])
{
	int i,j;
	BOOL   bCutResult;
	double dCutVal,dVal[2];
	double dInDist[2];
	double dTmpDist,dTtlDist;
	double dPoint1[3],dPoint2[3],dPoint3[3];
	double dCutLinePoint1[3],dCutLinePoint2[3];
	T_CALC_CUT_PONT CutlP;

	CArray<double,double> aCutValues;
	CArray<T_CALC_CUT_PONT,T_CALC_CUT_PONT> aCutPoints;

	if(m_arPoint.GetSize()!=2)
	{
		ASSERT(FALSE);
		return FALSE; 
	}
	dCutLinePoint1[0]=m_arPoint[0].dX;
	dCutLinePoint1[1]=m_arPoint[0].dY;
	dCutLinePoint1[2]=m_arPoint[0].dZ;
	dCutLinePoint2[0]=m_arPoint[1].dX;
	dCutLinePoint2[1]=m_arPoint[1].dY;
	dCutLinePoint2[2]=m_arPoint[1].dZ;
	
	for(i=0; i<m_arPolygon.GetSize(); i++)
	{
		j = (i==m_arPolygon.GetSize()-1)? 0:(i+1);
				
		dPoint1[0] = m_arPolygon[i].Pt.dX;
		dPoint1[1] = m_arPolygon[i].Pt.dY;
		dPoint1[2] = m_arPolygon[i].Pt.dZ;

		dPoint2[0] = m_arPolygon[j].Pt.dX;
		dPoint2[1] = m_arPolygon[j].Pt.dY;
		dPoint2[2] = m_arPolygon[j].Pt.dZ;

		bCutResult = CMathFunc::mathIntersectLine(dCutLinePoint1,dCutLinePoint2,dPoint1,dPoint2,1e-7,dTmpDist,dPoint3);
		
		if(bCutResult)
		{
			dTmpDist=CMathFunc::mathDistanceToLine(dPoint1, dPoint2, dPoint3);
			if(fabs(dTmpDist)>1.0E-07)continue; 

			dVal[0]=m_arPolygon[i].dVal;
			dVal[1]=m_arPolygon[j].dVal;
			dInDist[0] = CMathFunc::mathLength(dPoint3[0],dPoint3[1],dPoint3[2],
						dPoint1[0],dPoint1[1],dPoint1[2]);
			dInDist[1] = CMathFunc::mathLength(dPoint3[0],dPoint3[1],dPoint3[2],
						dPoint2[0],dPoint2[1],dPoint2[2]);

			dTtlDist = dInDist[0]+dInDist[1];
			if(dTtlDist==0.0)continue; 
				
			dCutVal = dVal[0] + (dVal[1]-dVal[0])*dInDist[0]/(dTtlDist);
			CutlP.dX = dPoint3[0];
			CutlP.dY = dPoint3[1];
			CutlP.dZ = dPoint3[2];

			aCutPoints.Add(CutlP);
			aCutValues.Add(dCutVal);
		}
	}
	
	// 같은 점이 있을 경우 없앤다.
	if(aCutPoints.GetSize()>1)
	for( i=0;   i<aCutPoints.GetSize()-1; i++)
	for( j=i+1; j<aCutPoints.GetSize(); j++)
	{
		if(IsSame3DPoint(aCutPoints[i],aCutPoints[j]))
		{
			aCutPoints.RemoveAt(j);
			aCutValues.RemoveAt(j);
			j--;
		}
	}	
	else return FALSE; 

	if(aCutPoints.GetSize()==2 && aCutValues.GetSize()==2)
	{
		BOOL bInclude[2];
		IsStateLineAndPoly(bInclude);

		for(i=0; i<2; i++)
		{
			dInDist[0] = CMathFunc::mathLength(aCutPoints[0].dX,aCutPoints[0].dY,aCutPoints[0].dZ,
						m_arPoint[i].dX,m_arPoint[i].dY,m_arPoint[i].dZ);
	    dInDist[1] = CMathFunc::mathLength(aCutPoints[1].dX,aCutPoints[1].dY,aCutPoints[1].dZ,
						m_arPoint[i].dX,m_arPoint[i].dY,m_arPoint[i].dZ);
			if(bInclude[i])
			{
				dCutVals[i] = aCutValues[0] + (aCutValues[1]-aCutValues[0])*dInDist[0]/(dInDist[0]+dInDist[1]);
			  dX[i] = m_arPoint[i].dX;
			  dY[i] = m_arPoint[i].dY;
				dZ[i] = m_arPoint[i].dZ;
			}
			else
			{
				if(dInDist[0]<=dInDist[1])
				{
					dCutVals[i]=aCutValues[0];
					dX[i] = aCutPoints[0].dX;
			    dY[i] = aCutPoints[0].dY;
					dZ[i] = aCutPoints[0].dZ;
				}
				else
				{
					dCutVals[i]=aCutValues[1];
					dX[i] = aCutPoints[1].dX;
			    dY[i] = aCutPoints[1].dY;
					dZ[i] = aCutPoints[1].dZ;
				}
			}
		}

		return TRUE; 
	}
	return FALSE; 
}

BOOL CDgnCalcCutlLine::IsSame3DPoint(double dX[2], double dY[2], double dZ[2] )
{
	const double dTolerance = 1e-8;
	if( fabs(dX[1]-dX[0]) > dTolerance) return FALSE;
	if( fabs(dY[1]-dY[0]) > dTolerance) return FALSE;
	if( fabs(dZ[1]-dZ[0]) > dTolerance) return FALSE;

	return TRUE;

}

BOOL CDgnCalcCutlLine::IsSame3DPoint(T_CALC_CUT_PONT &p1, T_CALC_CUT_PONT &p2)
{
	const double dTolerance = 1e-8;
	if( fabs(p1.dX-p2.dX) > dTolerance) return FALSE;
	if( fabs(p1.dY-p2.dY) > dTolerance) return FALSE;
	if( fabs(p1.dZ-p2.dZ) > dTolerance) return FALSE;

	return TRUE;

}
/*-------------------------------------------------------------------------------------------
* DATE : 2003.1.7. by KYE-HONG
* FUNC : Polygon을 Cutting하는 Line인가?
-------------------------------------------------------------------------------------------*/
int CDgnCalcCutlLine::IsStateLineAndPoly(BOOL bInclude[2])
{
	bInclude[0]=FALSE; 
	bInclude[1]=FALSE; 
	int   i,nVtxNum,nReturnCode;
	T_CALC_POINT2D *Poly;

	if(m_arPoint.GetSize()!=2)
	{
		ASSERT(FALSE);
		return LNPL_STATE_NOT_INCLUDE;
	}
	nVtxNum = m_arPolygon.GetSize();
	Poly = new T_CALC_POINT2D[nVtxNum];
	
	for(i=0; i<nVtxNum; i++)
	{
		Poly[i].dX = m_arPolygon[i].Pt.dX;
		Poly[i].dY = m_arPolygon[i].Pt.dY;
	}

	bInclude[0]=IsInside(nVtxNum,Poly,m_arPoint[0].dX,m_arPoint[0].dY);
	bInclude[1]=IsInside(nVtxNum,Poly,m_arPoint[1].dX,m_arPoint[1].dY);

	if(bInclude[0] && bInclude[1])
		nReturnCode=LNPL_STATE_TWO_POINT;
	else if(bInclude[0] || bInclude[1])
		nReturnCode=LNPL_STATE_ONE_POINT;
	else 
		nReturnCode=LNPL_STATE_NOT_INCLUDE;
	delete []Poly; 

	return nReturnCode;
}

BOOL CDgnCalcCutlLine::IsInside(int number_of_vertices, T_CALC_POINT2D* vertices, double point_x, double point_y)
{
	double angle = 0.0;
	for (int i=0; i<number_of_vertices; i++)
	{
		int j=i+1;
		if(i == number_of_vertices-1)j=0;
		double vector_x1 = vertices[i].dX - point_x;
		double vector_y1 = vertices[i].dY - point_y;
		double vector_x2 = vertices[j].dX - point_x;
		double vector_y2 = vertices[j].dY - point_y;
		angle += Angle(vector_x1, vector_y1, vector_x2, vector_y2);
	}
	angle = fabs(angle);
	if (angle > 350.0) return 1;
	return 0;
}

double CDgnCalcCutlLine::Angle(double vector_x1, double vector_y1, double vector_x2, double vector_y2)
{
	const double ZERO = 1.0e-08;
		
	double vector_x3 = vector_x2 - vector_x1;
	double vector_y3 = vector_y2 - vector_y1;

	double norm1 = Norm2D(vector_x1, vector_y1);
	double norm2 = Norm2D(vector_x2, vector_y2);
	double norm3 = Norm2D(vector_x3, vector_y3);
	
		double numerator = pow(norm1, 2.0) + pow(norm2, 2.0) - pow(norm3, 2.0);
		double denominator = 2.0 * norm1 * norm2;

		if (denominator < ZERO) return 0.0;
		
		double cosine = numerator / denominator;

	if (cosine > ( 1.0 - ZERO)) return 0.0;
	if (cosine < (-1.0 + ZERO)) return 180.0;

	double angle = fabs(acos(cosine) * 180.0 / CMathFunc::m_pi);

	double cross_product = vector_x1 * vector_y2 - vector_x2 * vector_y1;
 
	if (cross_product < 0.0) return -angle;

	return angle;
} // END: Angle()

double CDgnCalcCutlLine::Norm2D(double x, double y)
{

	return sqrt(pow(x, 2.0) + pow(y, 2.0));
} // END: Norm2D()

/*-------------------------------------------------------------------------------------------
* DATE : 2003.1.14. by KYE-HONG
* FUNC : 윈래의 Line과 계산된 Cutting Line의 방향을 동일하게 만들어줌.. 
-------------------------------------------------------------------------------------------*/
BOOL CDgnCalcCutlLine::MakeIdenticalDirection(double dX[2],double dY[2],double dZ[2],double dVal[2])
{
	double dXYZ1[3],dXYZ2[3];
	dXYZ1[0] = m_arPoint[1].dX-m_arPoint[0].dX;
	dXYZ1[1] = m_arPoint[1].dY-m_arPoint[0].dY;
	dXYZ1[2] = m_arPoint[1].dZ-m_arPoint[0].dZ;
	dXYZ2[0] = dX[1]-dX[0];
	dXYZ2[1] = dY[1]-dY[0];
	dXYZ2[2] = dZ[1]-dZ[0];
	
	CMathFunc::mathNormalize(dXYZ1,dXYZ1);
	CMathFunc::mathNormalize(dXYZ2,dXYZ2);
	double dDot=CMathFunc::mathDot(dXYZ1, dXYZ2);
	
	// 같은 방향 
	if(dDot>0) // 0도에서 90도 
	{
		return TRUE; 
	}
		
	// 다른 방향 
	double dTmpXYZ[3];
	dTmpXYZ[0] = dX[0];
	dTmpXYZ[1] = dY[0];
	dTmpXYZ[2] = dZ[0];
	dX[0]=dX[1];
	dY[0]=dY[1];
	dZ[0]=dZ[1];
	dX[1]=dTmpXYZ[0];
	dY[1]=dTmpXYZ[1];
	dZ[1]=dTmpXYZ[2];

	double dTmp = dVal[0];
	dVal[0] = dVal[1];
	dVal[1] = dTmp;
	return TRUE; 
}