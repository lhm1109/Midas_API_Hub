#include "Stdafx.h"
#include "wg_cmd.h"

#include "../wg_db/MathLib.h"
#include "../wg_db/MathFunc.h"
// #include "../DgnBase/Profiler.h"

#include "DrawText.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDrawText::CDrawText()
{
	m_DataR.Init();
	m_Data.Init();
}

CDrawText::~CDrawText()
{
}

//-------------------------------------------
int CDrawText::GetClassType()
{
	return EN_DRCLASS_TYPE_TEXT;
}

int CDrawText::GetFormatType()
{
	return EN_DRFORMAT_TYPE_TEXT;
}

void CDrawText::Draw(HDC DC, CDrawFunc* pDrawFunc, void* pDrawFormat)
{
	T_TEXT_FORMAT& DrawFormat = *((T_TEXT_FORMAT*)pDrawFormat);
	BOOL bCurrFormat = FALSE;
	int i;

	//for(auto itr = m_Data.List.Data.begin(); itr != m_Data.List.Data.end(); itr++)
	for(i = 0 ; i < m_Data.List.GetSize(); i++)
	{
		T_TEXT* itr = &m_Data.List[i];
		if(itr->bCurrFormat)
		{
			bCurrFormat = TRUE;  
			break; 
		}
	}
	if(!bCurrFormat)
	{
		pDrawFunc->DrawText(DC, DrawFormat, m_Data.List); 
	}
	else 
	{
		for(i = 0 ; i < m_Data.List.GetSize(); i++)
		//for(auto itr = m_Data.List.Data.begin(); itr != m_Data.List.Data.end(); itr++)
		{
			T_TEXT* itr = &m_Data.List[i];
			if(itr->bCurrFormat)
			{ 
				pDrawFunc->DrawText(DC, itr->CurrFormat, *itr); 
			}
			else 
			{ 
				pDrawFunc->DrawText(DC, DrawFormat, *itr); 
			}
		}
	}
}

void CDrawText::Convert(BOOL bCDC, CDrawFunc* pDrawFunc, T_PAPER_FORMAT* pPaperFormat, void* pDrawFormat)
{
	if(bCDC)
	{
		// 실제크기의 Data를 HDC용 Data로 변환
		m_Data.Init();
		pDrawFunc->ConvertToArText(*pPaperFormat, m_DataR.List, m_Data.List);					
	}
	else
	{
		// HDC용 Data를 실제크기의 Data로 변환
		m_DataR.Init();
		pDrawFunc->ConvertToArTextR(*pPaperFormat, m_Data.List, m_DataR.List);
	}
}

void CDrawText::GetCalcBoundaryR(double Boundary[4])
{
	Boundary[0] = Boundary[1] = Boundary[2] = Boundary[3] = 0.0;

	double dXmax, dXmin, dYmax, dYmin;
	double dXmaxU, dXminU, dYmaxU, dYminU;

	if(m_DataR.List.GetSize() <= 0) return;

	GetPossessionRegionR(m_DataR.List[0], dXmax, dXmin, dYmax, dYmin);

	int i;
	//for(auto itr = m_DataR.List.Data.begin() + 1; itr != m_DataR.List.Data.end(); itr++) 
	for(i = 0 ; i < m_DataR.List.GetSize(); i++)
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

void CDrawText::GetCalcBoundary(int Boundary[4])
{
	Boundary[0] = Boundary[1] = Boundary[2] = Boundary[3] = 0;

	int iXmax, iXmin, iYmax, iYmin;
	int iXmaxU, iXminU, iYmaxU, iYminU;

	if(m_Data.List.GetSize() <= 0) return;

	GetPossessionRegion(m_Data.List[0], iXmax, iXmin, iYmax, iYmin);
	
	int i;
	//for(auto itr = m_Data.List.Data.begin() + 1; itr != m_Data.List.Data.end(); itr++) 
	for(i = 0 ; i < m_Data.List.GetSize(); i++)
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

void CDrawText::Move(BOOL bCDC, double Vector[2], T_PAPER_FORMAT* pPaperFormat)
{
	double disVectorR[2];
	int    disVector[2];
	int		i;

	GetMoveVector(bCDC, Vector, pPaperFormat, disVectorR, disVector);

	if(disVectorR[0] != 0.0 || disVectorR[1] != 0.0)
	{
		for(i = 0 ; i < m_DataR.List.GetSize(); i++)
		{
			T_TEXT_R * itr = &m_DataR.List[i];
			MoveDrawUintR(disVectorR[0], disVectorR[1], *itr);
		}
	}

	if(disVector[0] != 0 || disVector[1] != 0)
	{
		for(i = 0 ; i < m_Data.List.GetSize(); i++)
		//for(T_TEXT_R * itr = m_Data.List.GetData(); itr != m_Data.List.Data.end(); itr++) 
		{
			T_TEXT * itr = &m_Data.List[i];
			MoveDrawUint(disVector[0], disVector[1], *itr);
		}
	}
	m_Boundary[0] += disVector[0];
	m_Boundary[1] += disVector[1];
	m_Boundary[2] += disVector[0];
	m_Boundary[3] += disVector[1];	
}

BOOL CDrawText::IsSelectCheck(POINT sPoint, BOOL bFill, int iTol)
{
	if(!m_bIsSelect) return FALSE;

	double dTol = (double)(iTol) + m_dZero;
	
	int i;
	//for(auto itr = m_Data.List.Data.begin(); itr != m_Data.List.Data.end(); itr++) 
	for( i = 0 ; i < m_Data.List.GetSize(); i++)
	{	
		T_TEXT* itr = &m_Data.List[i];
		if(abs(itr->PPosition.x - sPoint.x) <= dTol && abs(itr->PPosition.y - sPoint.y) <= dTol)
		{
			return TRUE;				
		}
	}		
	return FALSE;
}

BOOL CDrawText::IsSnapCheck(POINT currPoint, POINT& snapPoint, T_POINT& snapPointR)
{
	snapPoint.x = snapPoint.y = 0;
	snapPointR.Init();

	int  nListSize = m_Data.List.GetSize();
	BOOL bIsRData = (nListSize == m_DataR.List.GetSize());
	
	if(m_SnapType == 1)
	{
		for(int i = 0; i < nListSize; i++)
		{
			if(IsInSnapArea(m_Data.List[i].PPosition, currPoint))
			{
				snapPoint = m_Data.List[i].PPosition;
				if(bIsRData) snapPointR = m_DataR.List[i].PPosition;
				return TRUE;
			}
		}
	}
	return FALSE;
}

BOOL CDrawText::GetCenterPoint(int nID, POINT& CenterP)
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