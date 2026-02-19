// DgnToolMPhi_SeismicDraw.cpp: implementation of the CDgnToolMPhi_SeismicDraw class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DgnToolMPhi_SeismicDraw.h"

#include <io.h>
#include "..\wg_db\wg_db_DBDoc.h" // for Message
#include "..\wg_base\SpecialChar.h"
#include "..\wg_base\I_GENModelBase.h"

#include "DgnDrawSection.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDgnToolMPhi_SeismicDraw::CDgnToolMPhi_SeismicDraw()
{

}

CDgnToolMPhi_SeismicDraw::~CDgnToolMPhi_SeismicDraw()
{

}

BOOL CDgnToolMPhi_SeismicDraw::MakeLateralCapaFig(_DGN_RESERVE_LATERAL_CAPA_RES& ResData)
{
	CDBDoc* pDoc  = CDBDoc::GetDocPoint();	
	CString strFilePathDir  = pDoc->GetProgramPath() + _T("Temp\\");
	if(::GetFileAttributes(strFilePathDir) == -1)
	{
		CreateDirectory(strFilePathDir, NULL);
	}  
	
	// ¸ðµ¨ »ðµµ
	I_GENModelBase* pGenModel = I_GENModelBase::GetCurMySelfST();
	if(pGenModel ==  NULL) return FALSE;
		
	ResData.str2DimFileName.Format(_T("%s2DimFig.bmp"), strFilePathDir);
	ResData.str3DimFileName.Format(_T("%s3DimFig.bmp"), strFilePathDir);

	pGenModel->SetViewPointMy();
	pGenModel->ExportBitmap4AMR(ResData.str2DimFileName);
	
	pGenModel->SetViewPointISO();
	pGenModel->ExportBitmap4AMR(ResData.str3DimFileName);

	// ´Ü¸é ¹× M-Phi, P-Delta »ðµµ	
	int HSize = 6000;
	int BSize = 8000;
	TCHAR szDesc[] = _T("MFS Enhanced MetaFile Application");
	CRect rcSect(0, 0, BSize, HSize);
	CMetaFileDC metaDC;

	CDgnDrawSection  DrawSect;
	CDgn_DrawManager DrawManager;
	_DGN_DRAW_SECT_OPTION optionD;  optionD.Initialize();
	optionD.CanvasType = 1;
	optionD.Canvas = rcSect;
	optionD.DimSize = (min(BSize, HSize)/10);
	optionD.RebarSize = (min(BSize, HSize)/50);
	optionD.bRebar = TRUE;

	HSize = 4000;
	BSize = 5000;
	CRect rcChart(0, 0, BSize, HSize);
	DGNDRAW_MPHI_CHART_DATA   MPhiChart;            MPhiChart.Initialize();
	DGNDRAW_PDELTA_CHART_DATA PDeltaChart;          PDeltaChart.Initialize();
	DGNDRAW_MPHI_PDELTA_CHART_OPTION ChartOption;   ChartOption.Initialize();			
	ChartOption.CanvasType = 1;
	ChartOption.Canvas = rcChart;
	ChartOption.nFontSize = (min(BSize, HSize)/17);
	ChartOption.nForceUnit_Data  = ResData.DataUnit.nBase_Force;
	ChartOption.nLengthUnit_Data = ResData.DataUnit.nBase_Length;
	ChartOption.nForceUnit_Draw  = D_UNITSYS_FORCE_INDEX_KN;
	ChartOption.nLengthUnit_Draw = D_UNITSYS_LENGTH_INDEX_M;
	ChartOption.nMomDecimal      = 0;
	ChartOption.nForDecimal      = 0;
	ChartOption.nCurDecimal      = 1;
	ChartOption.nDisDecimal      = 2;
	CString  strMoment; strMoment.Format(_T("%s M (%s%s%s)"), _LS(IDS_DGN_RLC_BEND_MOMENT), D_UNITSYS_FORCE_NAME_KN, D_CH_MULSIGN, D_UNITSYS_LENGTH_NAME_M); // ÈÚ¸ð¸àÆ® M (kN.M)
	CStringW strWMoment(strMoment);
	ChartOption.strMoment = strWMoment;
	CString strCurvature; strCurvature.Format(_T("%s %sx10^-3$ (1/%s)"), _LS(IDS_DGN_RLC_CURVATURE), _LS(IDS_DGN_CHAR_PHI), D_UNITSYS_LENGTH_NAME_M);// °îÀ² ¥õx10^-3 (1/M)
	CStringW strWCurvature(strCurvature);
	ChartOption.strCurvature = strWCurvature;
	CString strForce; strForce.Format(_T("%s P (%s)"), _LS(IDS_DGN_RLC_HORI_FORCE), D_UNITSYS_FORCE_NAME_KN); // ¼öÆò·Â P (kN)
	CStringW strWForce(strForce);  
	ChartOption.strForce = strWForce;
	CString strDisplacement; strDisplacement.Format(_T("%s %s (%s)"), _LS(IDS_DGN_RLC_HORI_DIST), _LS(IDS_DGN_CHAR_DELTA), D_UNITSYS_LENGTH_NAME_M); // ¼öÆòº¯À§ ¥ä (M)
	CStringW strWDisplacement(strDisplacement);
	ChartOption.strDisplacement = strWDisplacement;
	ChartOption.dTopMargin       = 0.05;
	ChartOption.dBottomMargin    = 0.20;
	ChartOption.dLeftMargin      = 0.25;
	ChartOption.dRightMargin     = 0.05;
	
	int nSize = ResData.List.GetSize();
	CString strFileName_MPhi, strFileName_PDelta;
	_DGN_RESERVE_LATERAL_CAPA_MEMB_UNIT_RES MembUnitRes;
	for(int i=0 ; i<nSize ; i++)
	{
		if(!ResData.List[i].bIsRes)
		{ ResData.List[i].strFilePath_ShapeFig = _T("");  continue; }
		ResData.List[i].strFilePath_ShapeFig = strFilePathDir + ResData.List[i].strName + _T("_Shape.emf");
		metaDC.CreateEnhanced(NULL, ResData.List[i].strFilePath_ShapeFig , rcSect, szDesc);
		metaDC.SetMapMode(MM_HIMETRIC);	
		DrawSect.Draw_Section_RcColumn_For_ElemK(&metaDC, ResData.List[i].ElemK_Bottom, ResData.List[i].iPosi_Bottom, optionD);
		HENHMETAFILE hMetaF = metaDC.CloseEnhanced();
		::DeleteEnhMetaFile(hMetaF);		

		for(int j=0 ; j<4 ; j++)
		{
			if(j==0) 
			{				
				if(!ResData.bEarthquakeTypeI || !ResData.List[i].bTraverse)				
				{
					ResData.List[i].ResType1Trav.strFilePath_MPhiFig = ResData.List[i].ResType1Trav.strFilePath_PDeltaFig = _T("");
					continue;
				}
				ResData.List[i].ResType1Trav.strFilePath_MPhiFig   = strFilePathDir + ResData.List[i].strName + _T("_1T_MPhi.emf");
				ResData.List[i].ResType1Trav.strFilePath_PDeltaFig = strFilePathDir + ResData.List[i].strName + _T("_1T_PDelta.emf");
				MembUnitRes = ResData.List[i].ResType1Trav;
			}
			else if(j==1) 
			{		
				if(!ResData.bEarthquakeTypeI || !ResData.List[i].bLongitude)				
				{
					ResData.List[i].ResType1Long.strFilePath_MPhiFig = ResData.List[i].ResType1Long.strFilePath_PDeltaFig = _T("");
					continue;
				}
				ResData.List[i].ResType1Long.strFilePath_MPhiFig   = strFilePathDir + ResData.List[i].strName + _T("_1L_MPhi.emf");
				ResData.List[i].ResType1Long.strFilePath_PDeltaFig = strFilePathDir + ResData.List[i].strName + _T("_1L_PDelta.emf");
				MembUnitRes = ResData.List[i].ResType1Long;
			}
			else if(j==2) 
			{	
				if(!ResData.bEarthquakeTypeII || !ResData.List[i].bTraverse)				
				{
					ResData.List[i].ResType2Trav.strFilePath_MPhiFig = ResData.List[i].ResType2Trav.strFilePath_PDeltaFig = _T("");
					continue;
				}
				ResData.List[i].ResType2Trav.strFilePath_MPhiFig   = strFilePathDir + ResData.List[i].strName + _T("_2T_MPhi.emf");
				ResData.List[i].ResType2Trav.strFilePath_PDeltaFig = strFilePathDir + ResData.List[i].strName + _T("_2T_PDelta.emf");
				MembUnitRes = ResData.List[i].ResType2Trav;
			}
			else if(j==3) 
			{	
				if(!ResData.bEarthquakeTypeII || !ResData.List[i].bLongitude)				
				{
					ResData.List[i].ResType2Long.strFilePath_MPhiFig = ResData.List[i].ResType2Long.strFilePath_PDeltaFig = _T("");
					continue;
				}
				ResData.List[i].ResType2Long.strFilePath_MPhiFig   = strFilePathDir + ResData.List[i].strName + _T("_2L_MPhi.emf");
				ResData.List[i].ResType2Long.strFilePath_PDeltaFig = strFilePathDir + ResData.List[i].strName + _T("_2L_PDelta.emf");
				MembUnitRes = ResData.List[i].ResType2Long;
			}
			
			// M-Phi
			metaDC.CreateEnhanced(NULL, MembUnitRes.strFilePath_MPhiFig , rcChart, szDesc);
			metaDC.SetMapMode(MM_HIMETRIC);	
			MPhiChart.bDuplex = FALSE;
			MPhiChart.dM_P[0] = MembUnitRes.dMc;   MPhiChart.dPhi_P[0] = MembUnitRes.dPhic *1000.0;
			MPhiChart.dM_P[1] = MembUnitRes.dMy0;  MPhiChart.dPhi_P[1] = MembUnitRes.dPhiy0*1000.0;
			MPhiChart.dM_P[2] = MembUnitRes.dMy;   MPhiChart.dPhi_P[2] = MembUnitRes.dPhiy *1000.0;
			MPhiChart.dM_P[3] = MembUnitRes.dMu;   MPhiChart.dPhi_P[3] = MembUnitRes.dPhiu *1000.0;
			DrawManager.Draw_Chart_MPhi(&metaDC, MPhiChart,  ChartOption);
			hMetaF = metaDC.CloseEnhanced();
			::DeleteEnhMetaFile(hMetaF);					

			// P-Delta
			metaDC.CreateEnhanced(NULL, MembUnitRes.strFilePath_PDeltaFig , rcChart, szDesc);
			metaDC.SetMapMode(MM_HIMETRIC);					
			PDeltaChart.bDuplex = FALSE;
			PDeltaChart.dP_P[0] = MembUnitRes.dPc;   PDeltaChart.dDelta_P[0] = MembUnitRes.ddeltac;
			PDeltaChart.dP_P[1] = MembUnitRes.dPy0;  PDeltaChart.dDelta_P[1] = MembUnitRes.ddeltay0;
			PDeltaChart.dP_P[2] = MembUnitRes.dPy;   PDeltaChart.dDelta_P[2] = MembUnitRes.ddeltay;
			PDeltaChart.dP_P[3] = MembUnitRes.dPu;   PDeltaChart.dDelta_P[3] = MembUnitRes.ddeltau;
			DrawManager.Draw_Chart_PDelta(&metaDC, PDeltaChart,  ChartOption);
			hMetaF = metaDC.CloseEnhanced();
			::DeleteEnhMetaFile(hMetaF);		
		}
	}
	return TRUE;
}

BOOL CDgnToolMPhi_SeismicDraw::DeleteLateralCapaFig(_DGN_RESERVE_LATERAL_CAPA_RES& ResData)
{
	if(access(CW2A(ResData.str2DimFileName), 0) == 0)
	{
		remove(CW2A(ResData.str2DimFileName));
	}
	if(access(CW2A(ResData.str3DimFileName), 0) == 0)
	{
		remove(CW2A(ResData.str3DimFileName));
	}


	int nSize = ResData.List.GetSize();
	CString strFileName_MPhiFig, strFileName_PDeltaFig;
	for(int i=0 ; i<nSize ; i++)
	{
		if(!ResData.List[i].bIsRes) continue;
		if(access(CW2A(ResData.List[i].strFilePath_ShapeFig), 0) == 0)
		{
			remove(CW2A(ResData.List[i].strFilePath_ShapeFig));
		}

		for(int j=0 ; j<4 ; j++)
		{		
			if(j==0) 
			{
				strFileName_MPhiFig   = ResData.List[i].ResType1Trav.strFilePath_MPhiFig;
				strFileName_PDeltaFig = ResData.List[i].ResType1Trav.strFilePath_PDeltaFig;
			}
			else if(j==1) 
			{
				strFileName_MPhiFig   = ResData.List[i].ResType1Long.strFilePath_MPhiFig;
				strFileName_PDeltaFig = ResData.List[i].ResType1Long.strFilePath_PDeltaFig;
			}
			else if(j==2) 
			{
				strFileName_MPhiFig   = ResData.List[i].ResType2Trav.strFilePath_MPhiFig;
				strFileName_PDeltaFig = ResData.List[i].ResType2Trav.strFilePath_PDeltaFig;
			}
			else if(j==3) 
			{
				strFileName_MPhiFig   = ResData.List[i].ResType2Long.strFilePath_MPhiFig;
				strFileName_PDeltaFig = ResData.List[i].ResType2Long.strFilePath_PDeltaFig;
			}

			if(strFileName_MPhiFig != _T(""))
			{		
				if(access(CW2A(strFileName_MPhiFig), 0) == 0)
				{ remove(CW2A(strFileName_MPhiFig)); }
			}		
			if(strFileName_PDeltaFig != _T(""))
			{		
				if(access(CW2A(strFileName_PDeltaFig), 0) == 0)
				{ remove(CW2A(strFileName_PDeltaFig)); }
			}
		}
	}
	return TRUE;
}
