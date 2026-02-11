#include "Stdafx.h"
#include "wg_cmd.h"

#include "../wg_db/MathLib.h"
#include "../wg_db/MathFunc.h"

#include "DrawArc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDrawArc::CDrawArc()
{
	m_DataR.Init();
	m_Data.Init();
}

CDrawArc::~CDrawArc()
{
}

//-------------------------------------------
int CDrawArc::GetClassType()
{
	return EN_DRCLASS_TYPE_ARC;
}

int CDrawArc::GetFormatType()
{
	return EN_DRFORMAT_TYPE_DRAW;
}

void CDrawArc::Draw(HDC DC, CDrawFunc* pDrawFunc, void* pDrawFormat)
{
	T_DRAW_FORMAT DrawFormat = *((T_DRAW_FORMAT*)pDrawFormat);
	pDrawFunc->DrawArc(DC, DrawFormat, m_Data.List); 
}

void CDrawArc::Convert(BOOL bCDC, CDrawFunc* pDrawFunc, T_PAPER_FORMAT* pPaperFormat, void* pDrawFormat)
{
	if(bCDC)
	{
		// 실제크기의 Data를 HDC용 Data로 변환
		m_Data.Init();
		pDrawFunc->ConvertToArArc(*pPaperFormat, m_DataR.List, m_Data.List);					
	}
	else
	{
		// HDC용 Data를 실제크기의 Data로 변환
		m_DataR.Init();
		pDrawFunc->ConvertToArArcR(*pPaperFormat, m_Data.List, m_DataR.List);
	}
}

void CDrawArc::GetCalcBoundaryR(double Boundary[4])
{
	Boundary[0] = Boundary[1] = Boundary[2] = Boundary[3] = 0.0;

	double dXmax, dXmin, dYmax, dYmin;
	double dXmaxU, dXminU, dYmaxU, dYminU;

	if(m_DataR.List.GetSize() <= 0) return;

	GetPossessionRegionR(m_DataR.List[0], dXmax, dXmin, dYmax, dYmin);
	int i;
	for(i = 1; i < m_DataR.List.GetSize(); i++)
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

void CDrawArc::GetCalcBoundary(int Boundary[4])
{
	Boundary[0] = Boundary[1] = Boundary[2] = Boundary[3] = 0;

	int iXmax, iXmin, iYmax, iYmin;
	int iXmaxU, iXminU, iYmaxU, iYminU;

	if(m_Data.List.GetSize() <= 0) return;

	GetPossessionRegion(m_Data.List[0], iXmax, iXmin, iYmax, iYmin);
	
	int i;
	for(i = 1; i < m_Data.List.GetSize(); i++) 
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

void CDrawArc::Move(BOOL bCDC, double Vector[2], T_PAPER_FORMAT* pPaperFormat)
{
	double disVectorR[2];
	int    disVector[2];
	GetMoveVector(bCDC, Vector, pPaperFormat, disVectorR, disVector);

	if(disVectorR[0] != 0.0 || disVectorR[1] != 0.0)
	{
		int i;
		for(i = 0; i < m_DataR.List.GetSize(); i++)
		{
			MoveDrawUintR(disVectorR[0], disVectorR[1], m_DataR.List[i]);
		}
	}

	if(disVector[0] != 0 || disVector[1] != 0)
	{
		int i;
		for(i = 0; i < m_Data.List.GetSize(); i++)
		{
			MoveDrawUint(disVector[0], disVector[1], m_Data.List[i]);
		}
	}	
	m_Boundary[0] += disVector[0];
	m_Boundary[1] += disVector[1];
	m_Boundary[2] += disVector[0];
	m_Boundary[3] += disVector[1];	
}

BOOL CDrawArc::IsSelectCheck(POINT sPoint, BOOL bFill, int iTol)
{
	if(!m_bIsSelect) return FALSE;

	double dTol = (double)(iTol) + m_dZero;	
	double dDis, dAng, dxn, dyn;
	
	int i;

	//for(auto itr = m_Data.List.Data.begin(); itr != m_Data.List.Data.end(); itr++) 
	for(i = 0; i < m_Data.List.GetSize(); i++)
	{
		T_ARC* itr = &m_Data.List[i];
		dDis = CMathFunc::mathLength(abs(itr->CenterP.x - sPoint.x), abs(itr->CenterP.y - sPoint.y));
		
		if(dDis <= dTol)
		{
			if(bFill) return TRUE;
			if(itr->Radius <= dTol) return TRUE;
			continue;
		}

		CMathFunc::mathNormalize((itr->CenterP.x - sPoint.x), (itr->CenterP.y - sPoint.y), dxn, dyn);
		dAng = CMathFunc::mathCrossAngle2DSign(dxn, dyn, 1.0, 0.0);

		BOOL bAngChk = FALSE;
		double dAngDiff = dAng - itr->StartAngle;
		if(itr->SweepAngle >= 0.0)
		{
			if(dAngDiff <= itr->SweepAngle + m_dZero && dAngDiff >= -m_dZero) bAngChk = TRUE;
		}
		else
		{
			if(dAngDiff >= itr->SweepAngle - m_dZero && dAngDiff <= m_dZero) 	bAngChk = TRUE;
		}

		if(bAngChk)
		{
			if(fabs(dDis - itr->Radius) <= dTol) return TRUE;
			if(bFill && dDis <= abs(itr->Radius)) return TRUE;
		}
	}
	return FALSE;
}

BOOL CDrawArc::IsSnapCheck(POINT currPoint, POINT& snapPoint, T_POINT& snapPointR)
{	
	snapPoint.x = snapPoint.y = 0;
	snapPointR.Init();
	
	int    nListSize = m_Data.List.GetSize();
	BOOL   bIsRData = (nListSize == m_DataR.List.GetSize());	
	POINT  p1;	
	double dtrrad = 4.0 * atan(1.0) / 180.0;
	
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
			p1.x = m_Data.List[i].CenterP.x + (int)(m_Data.List[i].Radius * cos(m_Data.List[i].StartAngle * dtrrad));
			p1.y = m_Data.List[i].CenterP.y - (int)(m_Data.List[i].Radius * cos(m_Data.List[i].StartAngle * dtrrad));
			
			if(IsInSnapArea(p1, currPoint))
			{
				snapPoint = p1;
				if(bIsRData)
				{
					snapPointR.x = m_DataR.List[i].CenterP.x + (int)(m_DataR.List[i].Radius * cos(m_DataR.List[i].StartAngle * dtrrad));
					snapPointR.y = m_DataR.List[i].CenterP.y + (int)(m_DataR.List[i].Radius * cos(m_DataR.List[i].StartAngle * dtrrad));
				}
				return TRUE;
			}
			p1.x = m_Data.List[i].CenterP.x + (int)(m_Data.List[i].Radius * cos((m_Data.List[i].StartAngle + m_Data.List[i].SweepAngle) * dtrrad));
			p1.y = m_Data.List[i].CenterP.y - (int)(m_Data.List[i].Radius * cos((m_Data.List[i].StartAngle + m_Data.List[i].SweepAngle) * dtrrad));
		
			if(IsInSnapArea(p1, currPoint))
			{
				snapPoint = p1;
				if(bIsRData)
				{
					snapPointR.x = m_DataR.List[i].CenterP.x + (int)(m_DataR.List[i].Radius * cos((m_DataR.List[i].StartAngle + m_Data.List[i].SweepAngle) * dtrrad));
					snapPointR.y = m_DataR.List[i].CenterP.y + (int)(m_DataR.List[i].Radius * cos((m_DataR.List[i].StartAngle + m_Data.List[i].SweepAngle) * dtrrad));
				}
				return TRUE;
			}
		}
	}
	return FALSE;
}

BOOL CDrawArc::GetCenterPoint(int nID, POINT& CenterP)
{
	if(nID >= 0 && nID < (int)m_Data.List.GetSize())
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
