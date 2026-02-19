// DgnDraw_SectViewPannel.cpp: implementation of the CDgnDraw_SectViewPannel class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "wg_dgn.h"
#include "DgnDraw_SectViewPannel.h"

#include "..\wg_db\MathFunc.h"
#include "..\DgnLib\DgnDrawBaseLib\DgnDrawBase_Tool.h"

#include "..\wg_db\DBDoc.h"
#include "..\wg_db\UnitCtrl.h"
#include "..\wg_db\MatlDB.h"
#include "..\wg_db\SectUtil.h"

#include "Dgn_DllManager_Base.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDgnDraw_SectViewPannel dialog


CDgnDraw_SectViewPannel::CDgnDraw_SectViewPannel(CWnd* pParent /*=NULL*/)
	: CDgnDrawBase_DrawView_Impl(CDgnDraw_SectViewPannel::IDD, pParent)
{
	m_bSideRebar = TRUE;
	m_dRebarDia  = 0.0;
	m_bNum       = TRUE;
	m_nRebarNum  = 0;
	m_dRebarCTC  = 0.0;
	m_nRebarPosiType = 0;

	m_bGideLine  = FALSE;
	m_dGideDis   = 0.0;	
	Set_DrawFormat();

	m_DrawCtrl.m_SelectDrawType = 4;
	m_DrawCtrl.m_SelectDrawFormat.Width = 2;

	m_nOverlapType = EN_DGN_OVERLAP_OBJECT;
}


void CDgnDraw_SectViewPannel::DoDataExchange(CDataExchange* pDX)
{
	CDgnDrawBase_DrawView_Impl::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDgnDraw_SectViewPannel)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDgnDraw_SectViewPannel, CDgnDrawBase_DrawView_Impl)
	//{{AFX_MSG_MAP(CDgnDraw_SectViewPannel)
	  //
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDgnDraw_SectViewPannel message handlers






//////////////////////////////////////////////////////

BOOL CDgnDraw_SectViewPannel::SetSectData(T_SECT_D& SectD, int nPos, BOOL bAfter, BOOL bCenter, BOOL bFit)
{
	CDgn_DllManager_Base DllMgr;
	if(!DllMgr.ConvertToDLL_SectData_Pos(SectD, nPos, m_SectD, TRUE, bAfter)) return FALSE;

	DGN_GSEC_POLYGON_LIST arOutPolyData_Matl1; arOutPolyData_Matl1.RemoveAll();
	DGN_GSEC_POLYGON_LIST arInPolyData_Matl1;  arInPolyData_Matl1.RemoveAll(); 
	DGN_GSEC_POLYGON_LIST arOutPolyData_Matl2; arOutPolyData_Matl2.RemoveAll();
	DGN_GSEC_POLYGON_LIST arInPolyData_Matl2;  arInPolyData_Matl2.RemoveAll();

	double dPos[6];
	if(!CDgnDrawBase_Tool::Get_SectionPolygonData(m_SectD, arOutPolyData_Matl1, arInPolyData_Matl1, arOutPolyData_Matl2, arInPolyData_Matl2, m_arSteelLine, dPos)) return FALSE;

	if(m_SectD.nType == DGN_SECT_TYPE_COMPO_CI     || m_SectD.nType == DGN_SECT_TYPE_COMPO_CT     || m_SectD.nType == DGN_SECT_TYPE_COMPO_PC  || m_SectD.nType == DGN_SECT_TYPE_COMPO_G || 
		m_SectD.nType == DGN_SECT_TYPE_COMPO_B      || m_SectD.nType == DGN_SECT_TYPE_COMPO_I      || m_SectD.nType == DGN_SECT_TYPE_COMPO_TUB ||     		 
		m_SectD.nType == DGN_SECT_TYPE_COMPO_STLG_I || m_SectD.nType == DGN_SECT_TYPE_COMPO_STLG_B || m_SectD.nType == DGN_SECT_TYPE_COMPO_STLG_TUB) 
	{
		m_arConcOutPoly.Copy(arOutPolyData_Matl2);
		m_arConcInPoly.Copy(arInPolyData_Matl2);
		m_arSteelOutPoly.Copy(arOutPolyData_Matl1);
		m_arSteelInPoly.Copy(arInPolyData_Matl1);
	}
	else
	{
		m_arConcOutPoly.Copy(arOutPolyData_Matl1);
		m_arConcInPoly.Copy(arInPolyData_Matl1);
		m_arSteelOutPoly.Copy(arOutPolyData_Matl2);
		m_arSteelInPoly.Copy(arInPolyData_Matl2);
	}

	
	m_bCenter = bCenter;
	Make_DrawUnit(bFit);
	RedrawWindow();
	return TRUE;
}

void CDgnDraw_SectViewPannel::SetGideLine(BOOL bGideLine, double dGideDis)
{
	m_bGideLine = bGideLine;
	m_dGideDis  = dGideDis;

	Make_DrawGideLine();	
	RedrawWindow();
}
void CDgnDraw_SectViewPannel::SetRbarParameter(BOOL bNum, int nRebarNum, double dRebarCTC, double dRebarDia, BOOL bSideRebar, int nRebarPosiType)
{
	m_bNum           = bNum;
	m_nRebarNum      = nRebarNum ;
	m_dRebarCTC      = dRebarCTC ;
	m_dRebarDia      = dRebarDia ;
	m_bSideRebar     = bSideRebar;
	m_nRebarPosiType = nRebarPosiType;
}

void CDgnDraw_SectViewPannel::SetRebarData(CArray<T_RPSC_MBAR_SSM, T_RPSC_MBAR_SSM&> arRbarD, CArray<UINT, UINT>& arRbarKey, BOOL bCenter, BOOL bFit)
{
	m_arRbarD.RemoveAll();
	m_arRbarD.Copy(arRbarD);
	m_arRbarKey.RemoveAll();
	m_arRbarKey.Copy(arRbarKey);
	m_bCenter = bCenter;

	Make_DrawUnit(bFit);
	RedrawWindow();
}
void CDgnDraw_SectViewPannel::SetStressPointData(CArray<T_POINT_SSM, T_POINT_SSM&> arStressPointD, CArray<UINT, UINT>& arStressPointKey, int nFixPointSize, BOOL bCenter, BOOL bFit)
{
	m_arStressPointD.RemoveAll();
	m_arStressPointD.Copy(arStressPointD);
	m_arStressPointKey.RemoveAll();
	m_arStressPointKey.Copy(arStressPointKey);
	m_nFixPointSize = nFixPointSize;
	m_bCenter = bCenter;

	Make_DrawUnit(bFit);
	RedrawWindow();
}

void CDgnDraw_SectViewPannel::SetTendonParameter(BOOL bPropertyName, BOOL bDuctHall)
{
	m_bPropertyName = bPropertyName;
	m_bDuctHall = bDuctHall;
}
void CDgnDraw_SectViewPannel::SetTendonData(CArray<T_GSEC_TNDN, T_GSEC_TNDN&> arTendonD, BOOL bCenter, BOOL bFit)
{
	m_arTendonD.RemoveAll();
	m_arTendonD.Copy(arTendonD);
	m_bCenter = bCenter;

	Make_DrawUnit(bFit);
	RedrawWindow();
}


//////////////////////////////////////////////////////////////////////////


void CDgnDraw_SectViewPannel::Set_DrawFormat()
{
	// Format Setting ----------------------------
	T_DGN_DRAW_FORMAT DrawFormat;
	DrawFormat.Initialize();
	// Void	
	DrawFormat.startArrowType = EN_DGN_ARROW_TYPE_OPEN;
	DrawFormat.endArrowType   = EN_DGN_ARROW_TYPE_NONE;
	DrawFormat.Width          = 0;
	DrawFormat.dRWidth        = 3;
	DrawFormat.dRHeight       = 3;	
	DrawFormat.lineColor      = RGB(220, 220, 220);
	DrawFormat.bFill          = TRUE;	
	DrawFormat.fillColor      = m_BkGroudColor;
	m_nVoidFormatKey = 11;  
	m_DrawCtrl.m_arDrawFormat.SetAt(m_nVoidFormatKey, DrawFormat);

	// Concrete	
	DrawFormat.fillColor      = RGB(220, 220, 220);
	m_nConcFormatKey = 12;  
	m_DrawCtrl.m_arDrawFormat.SetAt(m_nConcFormatKey, DrawFormat);

	// Rebar	
	DrawFormat.lineColor = RGB(  0,   0, 255);
	DrawFormat.fillColor = RGB( 82,  71, 139);
	DrawFormat.Width     = 1;
	m_nRbarFormatKey = 13;  
	m_DrawCtrl.m_arDrawFormat.SetAt(m_nRbarFormatKey, DrawFormat);

	// Gide Line
	DrawFormat.lineColor = RGB(  0,   0, 255);
	DrawFormat.fillColor = RGB(  0,   0, 255);
	DrawFormat.nPenStyle = PS_DOT;
	DrawFormat.Width     = 1;
	DrawFormat.bFill     = FALSE;	
	m_nGideFormatKey     = 14;  
	m_DrawCtrl.m_arDrawFormat.SetAt(m_nGideFormatKey, DrawFormat);
}

void CDgnDraw_SectViewPannel::Make_DrawUnit(BOOL bFit, BOOL bCreateRebarData)
{
	m_DrawCtrl.RemoveDrawData(FALSE, TRUE, FALSE); //기존 Format을 남겨둠

	if(bCreateRebarData)
	{	
		CDBDoc* pDoc = CDBDoc::GetDocPoint();
		T_UNIT_INDEX CurIndex;
		pDoc->m_pUnitCtrl->GetUnitIndexCurrent(CurIndex);
		pDoc->m_pMatlDB->Create_RebarData(CurIndex.nBase_Length, CurIndex.nBase_Force, TRUE);
	}

	// Draw Shape ----------------------------
	Make_DrawSectUnit();

	// Draw Part -----------------------------
	Make_DrawPartUnit();

	// Draw Stiffener ------------------------
	Make_DrawStiffenerUnit();
	
	// Draw Tendon ---------------------------
	Make_DrawTendonUnit();

	// Draw Gide Line ------------------------
	Make_DrawGideLine();

	// Draw Rebar ----------------------------
	Make_DrawRebarUnit();

	// Draw Stress Point ---------------------
	Make_DrawStressPointUnit();

	if(bFit) m_DrawCtrl.Auto_PaperFormat(m_Canvas, m_dFitMargin, m_dFitMargin, m_dFitMargin, m_dFitMargin);
	m_DrawCtrl.ConvertDrawData(TRUE);
	OverlapChecking();
}


void CDgnDraw_SectViewPannel::Make_DrawSectUnit()
{	
	int nSnapType = 1; //좌표점 스넵을 사용
	double dOffsetX, dOffsetY;
	int i, j, nSize, nOutSize, nInSize, nLineSize;
	
	BOOL bFillConc = FALSE;
	if(m_SectD.nType == DGN_SECT_TYPE_SRC) bFillConc = CDgnDrawBase_Tool::IsFillConcret_SRC(m_SectD.Shape);

	if(m_bCenter)
	{	dOffsetX = dOffsetY = 0.0; }
	else
	{
		dOffsetX = m_SectD.dYBar;
		dOffsetY = m_SectD.dZBar;
	}
	// Draw Shape ----------------------------
	T_DGN_POLYGON_R_GROUP  PolyRGroup;     PolyRGroup.Initialize();
	T_DGN_POLYLINE_R       polyLineUnit;   

	// Concrete
	nOutSize = m_arConcOutPoly.GetSize();
	PolyRGroup.List.SetSize(nOutSize);
	for(i=0 ; i<nOutSize ; i++)
	{
		nSize = m_arConcOutPoly[i].aVertex.GetSize();
		polyLineUnit.Initialize();
		polyLineUnit.arPoint.SetSize(nSize);	
		for(j=0 ; i<nSize ; j++)
		{
			polyLineUnit.arPoint[j].x = m_arConcOutPoly[i].aVertex[j].dX + dOffsetX;
			polyLineUnit.arPoint[j].y = m_arConcOutPoly[i].aVertex[j].dY + dOffsetY;
		}
		PolyRGroup.List.SetAt(i, polyLineUnit);		
	}
	if(nOutSize > 0) m_DrawCtrl.AddDrawUnit(m_nConcFormatKey, PolyRGroup, FALSE, 0, nSnapType);

	PolyRGroup.Initialize();
	nInSize = m_arConcInPoly.GetSize();
	PolyRGroup.List.SetSize(nInSize);
	for(i=0 ; i<nInSize ; i++)
	{
		nSize = m_arConcInPoly[i].aVertex.GetSize();
		polyLineUnit.Initialize();
		polyLineUnit.arPoint.SetSize(nSize);	
		for(j=0 ; j<nSize ; j++)
		{
			polyLineUnit.arPoint[j].x = m_arConcInPoly[i].aVertex[j].dX + dOffsetX;
			polyLineUnit.arPoint[j].y = m_arConcInPoly[i].aVertex[j].dY + dOffsetY;
		}
		PolyRGroup.List.SetAt(i, polyLineUnit);
	}
	if(nInSize > 0) m_DrawCtrl.AddDrawUnit(m_nVoidFormatKey, PolyRGroup, FALSE, 0, nSnapType);	

	// Steel (Polygon)
	nOutSize = m_arSteelOutPoly.GetSize();
	PolyRGroup.List.SetSize(nOutSize);
	for(i=0 ; i<nOutSize ; i++)
	{
		nSize = m_arSteelOutPoly[i].aVertex.GetSize();
		polyLineUnit.Initialize();
		polyLineUnit.arPoint.SetSize(nSize);	
		for(j=0 ; i<nSize ; j++)
		{
			polyLineUnit.arPoint[j].x = m_arSteelOutPoly[i].aVertex[j].dX + dOffsetX;
			polyLineUnit.arPoint[j].y = m_arSteelOutPoly[i].aVertex[j].dY + dOffsetY;
		}
		PolyRGroup.List.SetAt(i, polyLineUnit);		
	}
	if(nOutSize > 0) m_DrawCtrl.AddDrawUnit(m_nSteelFormatKey, PolyRGroup, FALSE, 0, nSnapType);

	PolyRGroup.Initialize();
	nInSize = m_arSteelInPoly.GetSize();
	PolyRGroup.List.SetSize(nInSize);
	for(i=0 ; i<nInSize ; i++)
	{
		nSize = m_arSteelInPoly[i].aVertex.GetSize();
		polyLineUnit.Initialize();
		polyLineUnit.arPoint.SetSize(nSize);	
		for(j=0 ; j<nSize ; j++)
		{
			polyLineUnit.arPoint[j].x = m_arSteelInPoly[i].aVertex[j].dX + dOffsetX;
			polyLineUnit.arPoint[j].y = m_arSteelInPoly[i].aVertex[j].dY + dOffsetY;
		}
		PolyRGroup.List.SetAt(i, polyLineUnit);
	}
	if(nInSize > 0) m_DrawCtrl.AddDrawUnit((bFillConc ? m_nConcFormatKey : m_nVoidFormatKey), PolyRGroup, FALSE, 0, nSnapType);	

	// Steel (Line)
	T_DGN_LINE_R_GROUP  LineRGroup;     LineRGroup.Initialize();
	T_DGN_LINE_R        LineUnit;   
	
	nOutSize = m_SectD.arOutPolyData.GetSize();
	nLineSize = m_arSteelLine.GetSize();
	if(nOutSize > 0 && nLineSize > 0)
	{	
		int nSize = m_SectD.arOutPolyData[0].aVertex.GetSize();

		LineRGroup.List.SetSize(nLineSize);	
		for(i=0 ; i<nOutSize ; i++)
		{
			LineUnit.Initialize();
			
			if(m_arSteelLine[i].v1 < nSize && m_arSteelLine[i].v2 < nSize)
			{
				LineUnit.P1.x = m_SectD.arOutPolyData[0].aVertex[m_arSteelLine[i].v1].dX;
				LineUnit.P1.y = m_SectD.arOutPolyData[0].aVertex[m_arSteelLine[i].v1].dY;		
				LineUnit.P2.x = m_SectD.arOutPolyData[0].aVertex[m_arSteelLine[i].v2].dX;
				LineUnit.P2.y = m_SectD.arOutPolyData[0].aVertex[m_arSteelLine[i].v2].dY;
			}
			LineRGroup.List.SetAt(i, LineUnit);		
		}
		if(nLineSize > 0) m_DrawCtrl.AddDrawUnit(m_nSteelLineFormatKey, LineRGroup, FALSE, 0, nSnapType);
	}
}
void CDgnDraw_SectViewPannel::Make_DrawGideLine()
{
	if(!m_bGideLine || m_nDrawType != 1) return;

	int nSnapType = 1; //좌표점 스넵을 사용
	double dOffsetX, dOffsetY;
	int i, j, nSize, nOutSize, nInSize;
	double (*polyLine)[2];		

	if(m_bCenter)
	{	dOffsetX = dOffsetY = 0.0; }
	else
	{
		dOffsetX = m_SectD.dYBar;
		dOffsetY = m_SectD.dZBar;
	}
	
	T_DGN_POLYGON_R_GROUP  PolyRGroup;     PolyRGroup.Initialize();
	T_DGN_POLYLINE_R       polyLineUnit;  
	
	PolyRGroup.Initialize();
	nOutSize = m_arConcOutPoly.GetSize();	
	for(i=0 ; i<nOutSize ; i++)
	{
		nSize = m_arConcOutPoly[i].aVertex.GetSize();
		polyLine = new double[nSize][2];  
		for(j=0 ; j<nSize ; j++)
		{
			polyLine[j][0] = m_arConcOutPoly[i].aVertex[j].dX + dOffsetX;
			polyLine[j][1] = m_arConcOutPoly[i].aVertex[j].dY + dOffsetY;
		}
		if(CMathFunc::mathOffsetOfPolyline(-m_dGideDis, nSize, polyLine, 0))
		{
			polyLineUnit.Initialize();
			polyLineUnit.arPoint.SetSize(nSize);	
			for(j=0 ; j<nSize ; j++)
			{
				polyLineUnit.arPoint[j].x = polyLine[j][0];
				polyLineUnit.arPoint[j].y = polyLine[j][1];
			}
			PolyRGroup.List.Add(polyLineUnit);			
		}	
		delete[] polyLine;
	}

	nInSize = m_arConcOutPoly.GetSize();	
	for(i=0 ; i<nInSize ; i++)
	{
		nSize = m_arConcOutPoly[i].aVertex.GetSize();
		polyLine = new double[nSize][2];  
		for(j=0 ; j<nSize ; j++)
		{
			polyLine[j][0] = m_arConcOutPoly[i].aVertex[j].dX + dOffsetX;
			polyLine[j][1] = m_arConcOutPoly[i].aVertex[j].dY + dOffsetY;
		}
		if(CMathFunc::mathOffsetOfPolyline(m_dGideDis, nSize, polyLine, 0))
		{
			polyLineUnit.Initialize();
			polyLineUnit.arPoint.SetSize(nSize);	
			for(j=0 ; j<nSize ; j++)
			{
				polyLineUnit.arPoint[j].x = polyLine[j][0];
				polyLineUnit.arPoint[j].y = polyLine[j][1];
			}
			PolyRGroup.List.Add(polyLineUnit);			
		}	
		delete[] polyLine;
	}

	if(PolyRGroup.List.GetSize() > 0)
		m_DrawCtrl.AddDrawUnit(m_nGideFormatKey, PolyRGroup, FALSE, 0, nSnapType);	
}
void CDgnDraw_SectViewPannel::Make_DrawRebarUnit()
{
	if(m_nDrawType != 1 && m_nDrawType != 5) return;

	BOOL bIsSelect = (m_nDrawType == 1);
	
	int nSnapType = 1; //좌표점 스넵을 사용
	double dOffsetX, dOffsetY;
	int i, j, nSize, nKeySize, nRbarSize;

	if(m_bCenter)
	{	dOffsetX = dOffsetY = 0.0; }
	else
	{
		dOffsetX = m_SectD.dYBar;
		dOffsetY = m_SectD.dZBar;
	}


	T_DGN_CIRCLE_R_GROUP  CircleRGroup;     CircleRGroup.Initialize();
	
	nSize = m_arRbarD.GetSize();
	nKeySize = m_arRbarKey.GetSize();
	
	CArray<T_GSEC_RBAR,T_GSEC_RBAR&> aRbarData;
	for(i=0 ; i<nSize ; i++)
	{
		if(!CSectUtil::Get_ConvertRpscMbarSSMToGsecRbar(m_arRbarD[i], aRbarData)) continue;
		nRbarSize = aRbarData.GetSize();
		
		CircleRGroup.Initialize();
		CircleRGroup.List.SetSize(nRbarSize);		
		for(j=0 ; j<nRbarSize ; j++)
		{
			CircleRGroup.List[j].Initialize();
			CircleRGroup.List[j].Radius    = aRbarData[j].dDia / 2.0;
			CircleRGroup.List[j].CenterP.x = aRbarData[j].dyz[0] + dOffsetX; 
			CircleRGroup.List[j].CenterP.y = aRbarData[j].dyz[1] + dOffsetY; 
		}
		if(nRbarSize>0) m_DrawCtrl.AddDrawUnit(m_nRbarFormatKey, CircleRGroup, FALSE, (i<nKeySize ? m_arRbarKey[i] : 0), nSnapType, bIsSelect, 0, 0, TRUE);	//Select기능 Test
	}	
}


void CDgnDraw_SectViewPannel::Make_DrawStressPointUnit()
{
	if(m_nDrawType != 3) return;
	
	int nSnapType = 1; //좌표점 스넵을 사용
	double dOffsetX, dOffsetY;
	int i, nPointSize, nKeySize;

	if(m_bCenter)
	{	dOffsetX = dOffsetY = 0.0; }
	else
	{
		dOffsetX = m_SectD.dYBar;
		dOffsetY = m_SectD.dZBar;
	}

	T_DGN_POINT_R_GROUP  PointRGroup;     PointRGroup.Initialize();
	
	nPointSize = m_arStressPointD.GetSize();
	nKeySize   = m_arStressPointKey.GetSize();
	
	PointRGroup.List.SetSize(1);
	for(i=0 ; i<nPointSize ; i++)
	{		
		PointRGroup.List[0].Initialize();
		PointRGroup.List[0].x = m_arStressPointD[i].dPointY + dOffsetX;
		PointRGroup.List[0].y = m_arStressPointD[i].dPointZ + dOffsetY;

		if(i<=m_nFixPointSize-1)
			m_DrawCtrl.AddDrawUnit(m_nStressFixFormatKey, PointRGroup, FALSE, (i<nKeySize ? m_arStressPointKey[i] : 0), nSnapType, FALSE, 0, 0, TRUE);
		else 
			m_DrawCtrl.AddDrawUnit(m_nStressMoveFormatKey, PointRGroup, FALSE, (i<nKeySize ? m_arStressPointKey[i] : 0), nSnapType, TRUE, 0, 0, TRUE);
	}	
}

void CDgnDraw_SectViewPannel::Make_DrawPartUnit()
{
	// Check!!! by SHIN 추가요망
}
void CDgnDraw_SectViewPannel::Make_DrawStiffenerUnit()
{	
	// Check!!! by SHIN 추가요망
}
void CDgnDraw_SectViewPannel::Make_DrawTendonUnit()
{	
	if(m_nDrawType != 1 && m_nDrawType != 5) return;

	int nSnapType = 1; //좌표점 스넵을 사용
	double dOffsetX, dOffsetY;
	int i, nSize;

	if(m_bCenter)
	{	dOffsetX = dOffsetY = 0.0; }
	else
	{
		dOffsetX = m_SectD.dYBar;
		dOffsetY = m_SectD.dZBar;
	}

	T_DGN_CIRCLE_R_GROUP  TendonRGroup;     TendonRGroup.Initialize();
	T_DGN_CIRCLE_R_GROUP  DuctRGroup;       DuctRGroup.Initialize();

	T_DGN_CIRCLE_R CircleRD;

	nSize = m_arTendonD.GetSize();
	TendonRGroup.List.SetSize(nSize);
	for(i=0 ; i<nSize ; i++)
	{
		CircleRD.Initialize();
		CircleRD.Radius = m_arTendonD[i].dDia / 2.0;
		CircleRD.CenterP.x = m_arTendonD[i].dyz[0];
		CircleRD.CenterP.y = m_arTendonD[i].dyz[1];
		TendonRGroup.List.SetAt(i, CircleRD);
		if(m_bDuctHall)
		{
			if(m_arTendonD[i].dDuctDia > 0.0)
			{
				CircleRD.Radius = m_arTendonD[i].dDuctDia / 2.0;
				DuctRGroup.List.Add(CircleRD);
			}	
		}
	}
	
	if(DuctRGroup.List.GetSize() > 0)
		m_DrawCtrl.AddDrawUnit(m_nDuctFormatKey, DuctRGroup, FALSE, 0, nSnapType, FALSE, 0, 0, TRUE);

	if(TendonRGroup.List.GetSize() > 0)
		m_DrawCtrl.AddDrawUnit(m_nTendonFormatKey, TendonRGroup, FALSE, 0, nSnapType, FALSE, 0, 0, TRUE);
}

//////////////////////////////////////////////////////////////////////////

void CDgnDraw_SectViewPannel::Make_CurrUnit_Rebar(double dStartP[2])
{	
	T_DGN_POINT PointR;      PointR.Initialize();	
	PointR.x = dStartP[0];   PointR.y = dStartP[1];	
	T_DGN_CIRCLE_R CircleR;  CircleR.Initialize();	
	CircleR.CenterP.x = dStartP[0];  CircleR.CenterP.y = dStartP[1];
	CircleR.Radius = m_dRebarDia/2.0;

	m_CurrDrawClass_Point.m_DataR.List.Add(PointR);
	m_CurrDrawClass_Circle.m_DataR.List.Add(CircleR);
}

// Overrides //////////////////////////////////////////////////////////////////////////

void CDgnDraw_SectViewPannel::Before_CurrUnit_Point(CPoint CurrP)
{
	double dCoord[2];
	Get_GenuineCoord(CurrP, dCoord[0], dCoord[1]);	
	Change_SnapPoint(CurrP, dCoord);// Snap이 적용될 경우 좌표변환
	RedrawWindow();
}
void CDgnDraw_SectViewPannel::Before_CurrUnit_Line(CPoint CurrP)
{
	double dCoord[2];
	Get_GenuineCoord(CurrP, dCoord[0], dCoord[1]);	
	Change_SnapPoint(CurrP, dCoord);// Snap이 적용될 경우 좌표변환
	RedrawWindow();
}
void CDgnDraw_SectViewPannel::Before_CurrUnit_Arc(CPoint CurrP)
{
	double dCoord[2];
	Get_GenuineCoord(CurrP, dCoord[0], dCoord[1]);
	Change_SnapPoint(CurrP, dCoord);// Snap이 적용될 경우 좌표변환	
	RedrawWindow();
}
void CDgnDraw_SectViewPannel::Before_CurrUnit_Circle(CPoint CurrP)
{
	double dCoord[2];
	Get_GenuineCoord(CurrP, dCoord[0], dCoord[1]);
	Change_SnapPoint(CurrP, dCoord);// Snap이 적용될 경우 좌표변환	
	RedrawWindow();
}

// 작업완료 /////////////////////////////////
void CDgnDraw_SectViewPannel::Complete_CurrUnit_Point(CPoint CurrP)
{	
	double dCoord[2];
	Get_GenuineCoord(CurrP, dCoord[0], dCoord[1]);
	Change_SnapPoint(CurrP, dCoord);// Snap이 적용될 경우 좌표변환
	
	CString strCoord = Get_CoordText(dCoord[0], dCoord[1]);
	if(m_pPointEdit) m_pPointEdit->SetWindowText(strCoord);	
		
	// 객체를 추가하지 않고 CurrUnit를 남겨둠
	Curr_RemoveDrawData();
	Make_CurrUnit_Point(dCoord);
	Curr_ConvertDrawData(TRUE);
	m_nButtonCount = 0;
	RedrawWindow();
}
void CDgnDraw_SectViewPannel::Complete_CurrUnit_Line(CPoint CurrP)
{	
	double dCoord[2];
	Get_GenuineCoord(CurrP, dCoord[0], dCoord[1]);
	Change_SnapPoint(CurrP, dCoord);// Snap이 적용될 경우 좌표변환

	double dStartP[2], dEndP[2];
	dStartP[0] = m_StartPointR[0];  dStartP[1] = m_StartPointR[1];
	dEndP[0]   = dCoord[0]       ;  dEndP[1]   = dCoord[1]       ;
	
	CString strCoord = Get_CoordText(dEndP[0], dEndP[1]);	
	if(m_pLineEndPEdit)   m_pLineEndPEdit->SetWindowText(strCoord);	
	
	// 객체를 추가하지 않고 CurrUnit를 남겨둠
	Curr_RemoveDrawData();
	Make_CurrUnit_Line(dStartP, dEndP);	
	Curr_ConvertDrawData(TRUE);
	m_nButtonCount = 0;
	RedrawWindow();
}
void CDgnDraw_SectViewPannel::Complete_CurrUnit_Arc(CPoint CurrP)
{
	double dCoord[2];
	Get_GenuineCoord(CurrP, dCoord[0], dCoord[1]);
	Change_SnapPoint(CurrP, dCoord);// Snap이 적용될 경우 좌표변환

	double dStartP[2], dPassP[2], dEndP[2];
	dStartP[0] = m_StartPointR[0];  dStartP[1] = m_StartPointR[1];
	dPassP[0]  = m_InterPointR[0];  dPassP[1]  = m_InterPointR[1];
	dEndP[0]   = dCoord[0]       ;  dEndP[1]   = dCoord[1]       ;
	
	CString strCoord = Get_CoordText(dEndP[0], dEndP[1]);
	if(m_pArcEndPEdit)   m_pArcEndPEdit->SetWindowText(strCoord);
		
	// 객체를 추가하지 않고 CurrUnit를 남겨둠
	Curr_RemoveDrawData();
	Make_CurrUnit_Arc(dStartP, dPassP, dEndP);
	Curr_ConvertDrawData(TRUE);
	m_nButtonCount = 0;
	RedrawWindow();
}
void CDgnDraw_SectViewPannel::Complete_CurrUnit_Circle(CPoint CurrP)
{
	double dCoord[2];
	Get_GenuineCoord(CurrP, dCoord[0], dCoord[1]);
	Change_SnapPoint(CurrP, dCoord);// Snap이 적용될 경우 좌표변환

	double dStartP[2], dEndP[2];
	dStartP[0] = m_StartPointR[0];  dStartP[1] = m_StartPointR[1];
	dEndP[0]   = dCoord[0]       ;  dEndP[1]   = dCoord[1]       ;
	
	CString strCoord = Get_CoordText(dEndP[0], dEndP[1]);	
	if(m_pCircleEndPEdit)   m_pCircleEndPEdit->SetWindowText(strCoord);		
	strCoord.Format("%g", sqrt((dEndP[0]-dStartP[0])*(dEndP[0]-dStartP[0]) + (dEndP[1]-dStartP[1])*(dEndP[1]-dStartP[1])));
	if(m_pCircleRadiusEdit) m_pCircleRadiusEdit->SetWindowText(strCoord);	
	
	// 객체를 추가하지 않고 CurrUnit를 남겨둠
	Curr_RemoveDrawData();
	Make_CurrUnit_Circle(dStartP, dEndP);	
	Curr_ConvertDrawData(TRUE);
	m_nButtonCount = 0;
	RedrawWindow();
}
//
void CDgnDraw_SectViewPannel::Make_CurrUnit_Point(double dStartP[2])
{
	Make_CurrUnit_Rebar(dStartP);
}
void CDgnDraw_SectViewPannel::Make_CurrUnit_Line(double dStartP[2], double dEndP[2])
{
	double dLength = CMathFunc::mathLength(dStartP[0], dStartP[1], dEndP[0], dEndP[1]);
	if(dLength < m_dZero)
	{
		Make_CurrUnit_Rebar(dStartP);
		return;
	}

	//선표시
	m_CurrDrawClass_Line.m_DataR.List.SetSize(1);
	T_DGN_LINE_R LineR;
	LineR.P1.x = dStartP[0];   LineR.P1.y = dStartP[1];
	LineR.P2.x = dEndP[0];     LineR.P2.y = dEndP[1];
	m_CurrDrawClass_Line.m_DataR.List.SetAt(0, LineR);
	//철근표시
	CArray<T_GSEC_RBAR,T_GSEC_RBAR&> aRbarData;
	T_RPSC_MBAR_SSM RbarD;  RbarD.Initialize();

	RbarD.nType = 1;// 0:Point 1:Line 2:Arc 3:Circle 4:Poly Line
	RbarD.Start.dPointY = dStartP[0];
	RbarD.Start.dPointZ = dStartP[1];
	RbarD.End.dPointY = dEndP[0];
	RbarD.End.dPointZ = dEndP[1];
	RbarD.nNumorCTC = m_bNum ? 0 : 1;
	RbarD.nNum = m_nRebarNum;
	RbarD.dCTC = m_dRebarCTC;
	RbarD.nStandRef = m_nRebarPosiType;	
	RbarD.bEdgeBar = m_bSideRebar;	
	RbarD.strBar = "";	

	if(!CSectUtil::Get_ConvertRpscMbarSSMToGsecRbar(RbarD, aRbarData)) return;

	int nSize = aRbarData.GetSize();
	double RbarPoint[2];
	for(int i=0 ; i<nSize ; i++)
	{
		RbarPoint[0] = aRbarData[i].dyz[0];  RbarPoint[1] = aRbarData[i].dyz[1];
		Make_CurrUnit_Rebar(RbarPoint);
	}	
}
void CDgnDraw_SectViewPannel::Make_CurrUnit_Arc(double dStartP[2], double dPassP[2], double dEndP[2])
{
	double CenterP[2];
	double dRadius, dStartAngle, dSweepAngle;

	//선표시
	int nChk = CMathFunc::mathArcForm3Point(dStartP, dPassP, dEndP, CenterP, dRadius, dStartAngle, dSweepAngle);
	
	if(nChk == 0)
	{
		if(fabs(dStartP[0]-dEndP[0]) < m_dZero && fabs(dStartP[1]-dEndP[1]) < m_dZero)
		{
			Make_CurrUnit_Rebar(dStartP);
			return;
		}
		m_CurrDrawClass_Line.m_DataR.List.SetSize(1);
		T_DGN_LINE_R LineR;
		LineR.P1.x = dStartP[0];   LineR.P1.y = dStartP[1];
		LineR.P2.x = dEndP[0];     LineR.P2.y = dEndP[1];
		m_CurrDrawClass_Line.m_DataR.List.SetAt(0, LineR);		
	}
	else if(nChk == 1)
	{
		if(dRadius == m_dZero || dSweepAngle == m_dZero)
		{
			Make_CurrUnit_Rebar(dStartP);
			return;
		}
		m_CurrDrawClass_Arc.m_DataR.List.SetSize(1);
		T_DGN_ARC_R ArcR;
		ArcR.CenterP.x = CenterP[0];   ArcR.CenterP.y = CenterP[1];
		ArcR.Radius = dRadius;
		ArcR.StartAngle = dStartAngle;
		ArcR.SweepAngle = dSweepAngle;
		m_CurrDrawClass_Arc.m_DataR.List.SetAt(0, ArcR);		
	}
	m_CurrDrawClass_Point.m_DataR.List.SetSize(3);
	T_DGN_POINT PointR;
	PointR.x = dStartP[0];   PointR.y = dStartP[1];
	m_CurrDrawClass_Point.m_DataR.List.SetAt(0, PointR);
	PointR.x = dPassP[0];    PointR.y = dPassP[1];
	m_CurrDrawClass_Point.m_DataR.List.SetAt(1, PointR);
	PointR.x = dEndP[0];     PointR.y = dEndP[1];
	m_CurrDrawClass_Point.m_DataR.List.SetAt(2, PointR);
	//철근표시
	CArray<T_GSEC_RBAR,T_GSEC_RBAR&> aRbarData;
	T_RPSC_MBAR_SSM RbarD;  RbarD.Initialize();

	RbarD.nType = 2;// 0:Point 1:Line 2:Arc 3:Circle 4:Poly Line
	RbarD.Start.dPointY = dStartP[0];
	RbarD.Start.dPointZ = dStartP[1];
	RbarD.aPassage.SetSize(1);
	RbarD.aPassage[0].dPointY = dPassP[0];
	RbarD.aPassage[1].dPointZ = dPassP[1];
	RbarD.End.dPointY = dEndP[0];
	RbarD.End.dPointZ = dEndP[1];
	RbarD.nNumorCTC = m_bNum ? 0 : 1;
	RbarD.nNum = m_nRebarNum;
	RbarD.dCTC = m_dRebarCTC;
	RbarD.nStandRef = m_nRebarPosiType;	
	RbarD.bEdgeBar = m_bSideRebar;	
	RbarD.strBar = "";	

	if(!CSectUtil::Get_ConvertRpscMbarSSMToGsecRbar(RbarD, aRbarData)) return;

	int nSize = aRbarData.GetSize();
	double RbarPoint[2];
	for(int i=0 ; i<nSize ; i++)
	{
		RbarPoint[0] = aRbarData[i].dyz[0];  RbarPoint[1] = aRbarData[i].dyz[1];
		Make_CurrUnit_Rebar(RbarPoint);
	}	
}
void CDgnDraw_SectViewPannel::Make_CurrUnit_Circle(double dStartP[2], double dEndP[2])
{
	double dR = sqrt((dStartP[0]-dEndP[0])*(dStartP[0]-dEndP[0]) + (dStartP[1]-dEndP[1])*(dStartP[1]-dEndP[1]));
	//선표시
	if(fabs(dR < m_dZero))
	{
		Make_CurrUnit_Rebar(dStartP);
		return;
	}
	m_CurrDrawClass_Circle.m_DataR.List.SetSize(1);
	T_DGN_CIRCLE_R CircleR;
	CircleR.CenterP.x = dStartP[0];   CircleR.CenterP.y = dStartP[1];
	CircleR.Radius = dR;
	m_CurrDrawClass_Circle.m_DataR.List.SetAt(0, CircleR);
	//철근표시	
	CArray<T_GSEC_RBAR,T_GSEC_RBAR&> aRbarData;
	T_RPSC_MBAR_SSM RbarD;  RbarD.Initialize();

	RbarD.nType = 3;// 0:Point 1:Line 2:Arc 3:Circle 4:Poly Line
	RbarD.Start.dPointY = dStartP[0];
	RbarD.Start.dPointZ = dStartP[1];
	RbarD.End.dPointY   = dEndP[0];
	RbarD.End.dPointZ   = dEndP[1];
	RbarD.nNumorCTC = m_bNum ? 0 : 1;
	RbarD.nNum = m_nRebarNum;
	RbarD.dCTC = m_dRebarCTC;
	RbarD.nStandRef = m_nRebarPosiType;	
	RbarD.bEdgeBar = m_bSideRebar;	
	RbarD.strBar = "";	

	if(!CSectUtil::Get_ConvertRpscMbarSSMToGsecRbar(RbarD, aRbarData)) return;

	int nSize = aRbarData.GetSize();
	double RbarPoint[2];
	for(int i=0 ; i<nSize ; i++)
	{
		RbarPoint[0] = aRbarData[i].dyz[0];  RbarPoint[1] = aRbarData[i].dyz[1];
		Make_CurrUnit_Rebar(RbarPoint);
	}	
}

BOOL CDgnDraw_SectViewPannel::Find_StartEndPoint(CArray<int, int> arLineID, CArray<DGN_GSEC_LINE, DGN_GSEC_LINE&>& arLineData_Sort)
{
	int i, j, nSize, nPolySize, nLineSize;
	if(m_SectD.arOutPolyData.GetSize() == 0) return FALSE;	
	nSize = arLineID.GetSize();
	nPolySize = m_SectD.arOutPolyData[i].aVertex.GetSize();
	nLineSize = m_SectD.arLineData.GetSize();
	if(nSize == 0 || nPolySize == 0 || nLineSize == 0) return FALSE;
	for(i=0 ; i<nSize ; i++)
	{ if(arLineID[i] >= nLineSize) return FALSE; }
	
	// v1 또는 v2 가 연결되지 않은 Line ID를 찾음	
	CArray<int, int> arSingleLineID_v1;
	CArray<int, int> arSingleLineID_v2;
	int nCount = 0;
	int vID;
	BOOL bLink;
	for(i=0 ; i<nSize ; i++)
	{
		vID = m_SectD.arLineData[arLineID[i]].v1;
		bLink = FALSE;
		for(j=i+1 ; j<nSize ; j++)
		{
			if(vID == m_SectD.arLineData[arLineID[j]].v1) { bLink = TRUE;  break; }
			if(vID == m_SectD.arLineData[arLineID[j]].v2) { bLink = TRUE;  break; }
		}
		if(!bLink)
		{
			if(nCount >= 2) return FALSE;
			arSingleLineID_v1.Add(i);  nCount++;
		}
	}
	for(i=0 ; i<nSize ; i++)
	{
		vID = m_SectD.arLineData[arLineID[i]].v2;
		bLink = FALSE;
		for(j=i+1 ; j<nSize ; j++)
		{
			if(vID == m_SectD.arLineData[arLineID[j]].v1) { bLink = TRUE;  break; }
			if(vID == m_SectD.arLineData[arLineID[j]].v2) { bLink = TRUE;  break; }
		}
		if(!bLink)
		{
			if(nCount >= 2) return FALSE;
			arSingleLineID_v2.Add(i);
			nCount++;
		}
	}

	if(nCount != 0 || nCount != 2) return FALSE; // 끝점이 2개 이거나 패합되어야 함
	
	// 시작점을 설정함
	BOOL bv2End = TRUE;
	CArray<int, int> arLineID_Sort;
	arLineData_Sort.SetSize(nSize);
	arLineID_Sort.Copy(arLineID);
	for(i=0 ; i<nSize ; i++) { arLineData_Sort[i].Initialize();  arLineID_Sort[i] = -1; }
	int nStartLineID = arLineID_Sort[0];
	if(nCount == 0)
	{// 패합일때에느 v1값이 가장 작은 기준으로 한다.
		for(i=0 ; i<nSize ; i++) 
		{
			if(i==0 || arLineData_Sort[0].v1 > m_SectD.arLineData[arLineID[i]].v1) 
			{ nStartLineID = arLineID[i];  bv2End = TRUE; }
		}
	}
	else 
	{
		if(arSingleLineID_v1.GetSize() == 1) 	
		{ nStartLineID = arSingleLineID_v1[0];  bv2End = TRUE; }
		else if(arSingleLineID_v1.GetSize() == 2) 	
		{
			if(m_SectD.arLineData[arSingleLineID_v1[0]].v1 <= m_SectD.arLineData[arSingleLineID_v1[1]].v1)
			{ nStartLineID = arSingleLineID_v1[0]; }
			else 
			{ nStartLineID = arSingleLineID_v1[1]; }
			bv2End = TRUE;
		}
		else if(arSingleLineID_v2.GetSize() == 2) 
		{
			if(m_SectD.arLineData[arSingleLineID_v2[0]].v2 <= m_SectD.arLineData[arSingleLineID_v2[1]].v2)
			{ nStartLineID = arSingleLineID_v2[0]; }
			else 
			{ nStartLineID = arSingleLineID_v2[1]; }
			bv2End = FALSE;
		}
		else 
		{ ASSERT(0);  return FALSE; }
	}
	arLineData_Sort[0] = m_SectD.arLineData[nStartLineID];
	if(!bv2End) Change_LineEndData(arLineData_Sort[0]);
	
	for(i=1 ; i<nSize ; i++)
	{
		if(nStartLineID == arLineID_Sort[i]) 
		{ Change_ArrayData(arLineID_Sort, 0, i);  break; }
	}

	for(i=1 ; i<nSize ; i++)
	{
		vID = arLineData_Sort[i-1].v2;
		if(i==nSize-1)
		{ 
			arLineData_Sort[i] = m_SectD.arLineData[arLineID_Sort[i]]; 
			if(vID == arLineData_Sort[i].v2) Change_LineEndData(arLineData_Sort[i]);
		}
		else 
		{			
			nCount = 0;
			for(j=i+1 ; j<nSize ; j++)
			{
				if(vID == m_SectD.arLineData[arLineID_Sort[j]].v1 || vID == m_SectD.arLineData[arLineID_Sort[j]].v2) 
				{
					if(nCount > 0) return FALSE;
					arLineData_Sort[i] = m_SectD.arLineData[arLineID_Sort[j]];
					Change_ArrayData(arLineID_Sort, i, j);
					if(vID == m_SectD.arLineData[arLineID_Sort[j]].v2) Change_LineEndData(arLineData_Sort[i]);
					nCount++;
				}
			}
			
			if(nCount == 1) return FALSE;		
		}
	}
	return TRUE;
}


BOOL CDgnDraw_SectViewPannel::Change_ArrayData(CArray<int, int>& List, int nID1, int nID2)
{
	int nSize = List.GetSize();
	if(nID1 < 0 || nID1 >= nSize) return FALSE;
	if(nID2 < 0 || nID2 >= nSize) return FALSE;
	if(nID1 == nID2) return TRUE;
	int nTemp = List[nID1];
	List[nID1] = List[nID2];
	List[nID2] = nTemp;
	return TRUE;
}
void CDgnDraw_SectViewPannel::Change_LineEndData(DGN_GSEC_LINE& LineD)
{
	int nTemp = LineD.v1;
	LineD.v1 = LineD.v2;
	LineD.v2 = nTemp;
}