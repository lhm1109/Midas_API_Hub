#include "Stdafx.h"
#include "wg_cmd.h"

#include "../wg_db/MathFunc.h"
#include "../wg_db/MathLib.h"
// #include "../DgnBase/Profiler.h"

#include "DrawDimAngle.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDrawDimAngle::CDrawDimAngle()
{
	m_DataR.Init();
	m_Data.Init();
}

CDrawDimAngle::~CDrawDimAngle()
{
}

//-------------------------------------------
int CDrawDimAngle::GetClassType()
{
	return EN_DRCLASS_TYPE_DIMANGLE;
}

int CDrawDimAngle::GetFormatType()
{
	return EN_DRFORMAT_TYPE_DIM;
}

void CDrawDimAngle::Draw(HDC DC, CDrawFunc* pDrawFunc, void* pDrawFormat)
{
	T_DIM_FORMAT DrawFormat = *((T_DIM_FORMAT*)pDrawFormat);
	pDrawFunc->DrawDimAngle(DC, DrawFormat, m_Data.List); 
}

void CDrawDimAngle::Convert(BOOL bCDC, CDrawFunc* pDrawFunc, T_PAPER_FORMAT* pPaperFormat, void* pDrawFormat)
{
	if(bCDC)
	{
		// 실제크기의 Data를 HDC용 Data로 변환
		m_Data.Init();
		pDrawFunc->ConvertToArAngle(*pPaperFormat, m_DataR.List, m_Data.List);		
	}
	else
	{
		// HDC용 Data를 실제크기의 Data로 변환
		m_DataR.Init();
		pDrawFunc->ConvertToArAngleR(*pPaperFormat, m_Data.List, m_DataR.List);
	}
}

void CDrawDimAngle::GetCalcBoundaryR(double Boundary[4])
{
	Boundary[0] = Boundary[1] = Boundary[2] = Boundary[3] = 0.0;

	double dXmax, dXmin, dYmax, dYmin;
	double dXmaxU, dXminU, dYmaxU, dYminU;

	if(m_DataR.List.GetSize() <= 0) return;

	GetPossessionRegionR(m_DataR.List[0], dXmax, dXmin, dYmax, dYmin);
	
	int i;
	//for(auto itr = m_DataR.List.Data.begin() + 1; itr != m_DataR.List.Data.end(); itr++) 
	for( i = 1; i < m_DataR.List.GetSize(); i++)
	{
		GetPossessionRegionR(m_DataR.List[i], dXmaxU, dXminU, dYmaxU, dYminU);

		dXmax = max(dXmax, dXmaxU);
		dXmin = min(dXmin, dXminU);
		dYmax = max(dYmax, dYmaxU);
		dYmin = min(dYmin, dYminU);		
	}	

	Boundary[0] = dXmin;	
	Boundary[1] = dYmin;
	Boundary[2] = dXmax;	
	Boundary[3] = dYmax;
}

void CDrawDimAngle::GetCalcBoundary(int Boundary[4])
{
	Boundary[0] = Boundary[1] = Boundary[2] = Boundary[3] = 0;

	int iXmax, iXmin, iYmax, iYmin;
	int iXmaxU, iXminU, iYmaxU, iYminU;
	
	if(m_Data.List.GetSize() <= 0) return;
	GetPossessionRegion(m_Data.List[0], iXmax, iXmin, iYmax, iYmin);
	
	int i;
	//for(auto itr = m_Data.List.Data.begin() + 1; itr != m_Data.List.Data.end(); itr++) 
	for( i = 1 ; i < m_Data.List.GetSize() ;i++)
	{
		GetPossessionRegion(m_Data.List[i], iXmaxU, iXminU, iYmaxU, iYminU);

		iXmax = max(iXmax, iXmaxU);
		iXmin = min(iXmin, iXminU);
		iYmax = max(iYmax, iYmaxU);
		iYmin = min(iYmin, iYminU);		
	}	

	Boundary[0] = iXmin;
	Boundary[1] = iYmin;
	Boundary[2] = iXmax;	
	Boundary[3] = iYmax;
}

void CDrawDimAngle::Move(BOOL bCDC, double Vector[2], T_PAPER_FORMAT* pPaperFormat)
{
	double disVectorR[2];
	int    disVector[2];
	int    i;

	GetMoveVector(bCDC, Vector, pPaperFormat, disVectorR, disVector);

	if(disVectorR[0] != 0.0 || disVectorR[1] != 0.0)
	{
		//for(auto itr = m_DataR.List.Data.begin(); itr != m_DataR.List.Data.end(); itr++) 
		for( i = 0 ; i < m_DataR.List.GetSize() ;i++)
		{
			MoveDrawUintR(disVectorR[0], disVectorR[1], m_DataR.List[i]);
		}		
	}

	if(disVector[0] != 0 || disVector[1] != 0)
	{
		//for(auto itr = m_Data.List.Data.begin(); itr != m_Data.List.Data.end(); itr++) 
		for( i = 0 ; i < m_Data.List.GetSize(); i++)
		{
			MoveDrawUint(disVector[0], disVector[1], m_Data.List[i]);
		}
	}

	m_Boundary[0] += disVector[0];
	m_Boundary[1] += disVector[1];
	m_Boundary[2] += disVector[0];
	m_Boundary[3] += disVector[1];	
}

BOOL CDrawDimAngle::IsSelectCheck(POINT sPoint, BOOL bFill, int iTol)
{
	if(!m_bIsSelect) return FALSE;

	double dTol = (double)(iTol)+m_dZero;
	double dDis;
	int	   i;

	//for(auto itr = m_Data.List.Data.begin(); itr != m_Data.List.Data.end(); itr++) 
	for( i = 0 ; i < m_Data.List.GetSize() ;i++)
	{		
		// 실수 정수를 가려서 코딩합시다. 
		T_ANGLE* itr = &m_Data.List[i];
		dDis = CMathFunc::mathLength(abs(itr->CenterP.x - sPoint.x), abs(itr->CenterP.y - sPoint.y));
		
		if(dDis <= dTol) return TRUE;
	}		
	return FALSE;
}

BOOL CDrawDimAngle::IsSnapCheck(POINT currPoint, POINT& snapPoint, T_POINT& snapPointR)
{
	snapPoint.x = snapPoint.y = 0;
	snapPointR.Init();
	
	int  nListSize = m_Data.List.GetSize();
	BOOL bIsRData = (nListSize == m_DataR.List.GetSize());
	
	if(m_SnapType == 1)
	{
		for(int i = 0; i < nListSize ; i++)
		{
			if(IsInSnapArea(m_Data.List[i].CenterP, currPoint))
			{
				snapPoint = m_Data.List[i].CenterP;
				if(bIsRData) snapPointR = m_DataR.List[i].CenterP;
				
				return TRUE;
			}
		}
	}
	return FALSE;
}

BOOL CDrawDimAngle::GetCenterPoint(int nID, POINT& CenterP)
{
	if(nID >= 0 && nID < m_Data.List.GetSize())
	{
		int iXmax, iXmin, iYmax, iYmin;
		
		GetPossessionRegion(m_Data.List[nID], iXmax, iXmin, iYmax, iYmin);
		
		CenterP.x = (iXmax + iXmin) / 2;
		CenterP.y = (iYmax + iYmin) / 2;
		return TRUE;
	}
	CenterP.x = CenterP.y = 0;
	return FALSE;
}
