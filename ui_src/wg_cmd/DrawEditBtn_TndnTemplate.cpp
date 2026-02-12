// DrawEditBtn_TndnTemplate.cpp: implementation of the CDgnEditBtn_TndnTemplate class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "wg_cmd.h"

#include "DrawEditBtn_TndnTemplate.h"
#include "DgnTendonTemplateCtrl.h"
#include "DgnTendonElemLengthCtrl.h"
#include "CMDTendonTemplateDlg.h"

#include "..\wg_db\DBDoc.h"
#include "..\wg_db\UnitCtrl.h"
#include "..\wg_db\MatlDB.h"
#include "..\wg_db\SectUtil.h"
#include "..\wg_db\MathFunc.h"
#include "..\wg_db\Db_DllManager_Base.h"

#include <utility>
#include "..\dgnengine\idesign\DGN_link\ITmpDgnDrawLib.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define LINKKEY_ORIGIN 9999

/////////////////////////////////////////////////////////////////////////////
// CDgnEditBtn_TndnTemplate dialog


CDrawEditBtn_TndnTemplate::CDrawEditBtn_TndnTemplate(CDgnTendonTemplateCtrl* pTemplateCtrl)
	: CDrawEditBtn()
{
	m_pTemplateCtrl = pTemplateCtrl;
// 	Reset_Variables(FALSE);
 	SetDrawFormat();

 	m_DrawCtrl.m_SelectDrawType = 4;
	m_DrawCtrl.m_SelectDrawFormat.Width = 3;
	m_DrawCtrl.m_SelectDrawFormat.lineColor = RGB(250, 115, 75);
	m_DrawCtrl.m_SelectDrawFormat.pointType = EN_POINT_TYPE_CIRCLE;


	m_nAxisMargin = 20;
// 	m_nOverlapType = EN_DGN_OVERLAP_OBJECT;
	m_bUseMove = FALSE; //일단 이게 있어야 선택 사각형을 그린다.....
	m_bMoveOrigin = FALSE;
 	m_DrawCtrl.SetSelectDis(2);

	m_bShowElemLine	= FALSE;
	m_bShowPreview	= FALSE;
	m_bHasList      = FALSE;
	m_bUseOrigin    = FALSE;

	m_pTndnData = NULL;

	m_dMarginFactor[0] = 2.5; //Section
	m_dMarginFactor[1] = 4.5; //Add/Mod 대화상자.
	m_dMarginFactor[2] = 3.0; //Template 대화상자.

	m_dTendonZoomFactor = 0.01;

	m_uiOrigin = 0;
	m_pUnitCtrl = CDBDoc::GetDocPoint()->m_pUnitCtrl;
}

CDrawEditBtn_TndnTemplate::CDrawEditBtn_TndnTemplate()
	: CDrawEditBtn()
{
	// 	Reset_Variables(FALSE);
	SetDrawFormat();

	m_DrawCtrl.m_SelectDrawType = 4;
	m_DrawCtrl.m_SelectDrawFormat.Width = 3;
	m_DrawCtrl.m_SelectDrawFormat.lineColor = RGB(250, 115, 75);
	m_DrawCtrl.m_SelectDrawFormat.pointType = EN_POINT_TYPE_CIRCLE;


	m_nAxisMargin = 20;
	// 	m_nOverlapType = EN_DGN_OVERLAP_OBJECT;
	m_bUseMove = FALSE; //일단 이게 있어야 선택 사각형을 그린다.....
	m_bMoveOrigin = FALSE;
	m_DrawCtrl.SetSelectDis(2);

	m_bShowElemLine = FALSE;
	m_bShowPreview = FALSE;
	m_bHasList = FALSE;
	m_bUseOrigin = FALSE;

	m_pTndnData = NULL;

	m_dMarginFactor[0] = 2.5; //Section
	m_dMarginFactor[1] = 4.5; //Add/Mod 대화상자.
	m_dMarginFactor[2] = 3.0; //Template 대화상자.

	m_dTendonZoomFactor = 0.01;

	m_uiOrigin = 0;
	m_pUnitCtrl = CDBDoc::GetDocPoint()->m_pUnitCtrl;
}

CDrawEditBtn_TndnTemplate::~CDrawEditBtn_TndnTemplate()
{
}

void CDrawEditBtn_TndnTemplate::DoDataExchange(CDataExchange* pDX)
{
	CDrawEditBtn::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDrawEditBtn_TndnTemplate, CDrawEditBtn)
END_MESSAGE_MAP()

void CDrawEditBtn_TndnTemplate::MakeDrawUnit( BOOL bFit /*= TRUE*/ )
{
	m_DrawCtrl.RemoveDrawData(FALSE, TRUE, FALSE); //기존 Format을 남겨둠

	if(m_nDrawType == DRAW_EDIT_BTN_TYPE_YZ)// yz단면 그릴때.
	{
		MakeDrawSectUnit_YZ(); 

		MakeDrawTndn_YZ();
		MakeDrawDim_YZ();
		MakeDrawPreviewData();
		MakeDrawOrigin();
	}
	else if(m_nDrawType == DRAW_EDIT_BTN_TYPE_XY || m_nDrawType == DRAW_EDIT_BTN_TYPE_XZ)
 	{
 		MakeDrawSectUnit_XZXY();
		MakeDrawTndn_XZXY();
		MakeDrawPreviewData();
	}
	MakeDrawSubClass();

	if(bFit)
	{
		int nIdx;

		if(m_nDrawType == DRAW_EDIT_BTN_TYPE_YZ)  nIdx = 0; //Section
		else if(m_pTndnData)                      nIdx = 1; //Add/Mod 대화상자.
		else                                      nIdx = 2; //Template 대화상자.

		m_DrawCtrl.AutoPaperFormat(m_Canvas, m_dFitMargin * m_dMarginFactor[nIdx], m_dFitMargin, m_dFitMargin, m_dFitMargin);
	}
	m_DrawCtrl.ConvertDrawData(TRUE);
	
	OverlapChecking();
	Redraw(bFit);
}

void CDrawEditBtn_TndnTemplate::ResetVariable_Sect()
{		
	m_SectD.Initialize();   // 단면정보
	m_arConcOutPoly.RemoveAll();
	m_arConcInPoly.RemoveAll();
	m_arSteelOutPoly.RemoveAll();
	m_arSteelInPoly.RemoveAll();
	m_arSteelLine.RemoveAll();
	m_arSteelVertexLine.RemoveAll();
	m_arSteelLinePoly.RemoveAll();
	memset(m_dOutPos, 0, sizeof(m_dOutPos)); 
}

BOOL CDrawEditBtn_TndnTemplate::SetSectData( T_SECT_D& SectD, T_ELEM_K tElemKey, double dPos, BOOL bAfter, BOOL bCenter, BOOL bFit /*= TRUE*/)
{
 	ResetVariable_Sect();

	m_pTemplateCtrl->GetLengthCtrl()->GetHeightWidthByElem(tElemKey, m_dTotalHeight, m_dTotalWidth, m_dGapY, m_dGapZ, dPos);
	m_pTemplateCtrl->GetPosByElem(tElemKey, dPos, m_dPos);
 
 	CDb_DllManager_Base DllMgr;
 	if(!DllMgr.ConvertToDLL_SectData_Pos(SectD, dPos, m_SectD, TRUE, bAfter)) return FALSE;
 	
 	if(!dgn::link::ITmpDgnDrawLib::GetInstance()->Get_SectionPolygonData(m_SectD, m_arConcOutPoly, m_arConcInPoly,
 		m_arSteelOutPoly, m_arSteelInPoly, m_arSteelLine, m_arSteelVertexLine, m_arSteelLinePoly,
 		m_dOutPos, -m_SectD.dYBar, -m_SectD.dZBar, TRUE))
	{
		return FALSE;
	}
 	
 	m_bCenter = bCenter;
	if(!SetOriginPt_Init(SectD))
	{
		ASSERT(0);
	}

	ResetSnapPos();
 	MakeDrawUnit(bFit);
	return TRUE;
}

BOOL CDrawEditBtn_TndnTemplate::SetTdnaData(TndnTemplate* pData /*= NULL*/, BOOL bFit /*= TRUE*/)
{
	if(pData)
	{
		m_pTndnData = pData;
	}
	
	int nSize, i;
	BOOL bXY;
	vecTndnKey keyList;
	TndnDrawStruct drawStruct;
	
	std::vector<double>*	dPosX;
	std::vector<double>*	dPosYZ;
	std::vector<BOOL>*		bInput;  
	
	//축 그리는 데이터 만들다.
	int nWidth;
	if(m_pTndnData) { nWidth = 15; m_nAxisMargin = 15; } //Addmod 대화상자는 조금 작게.
	else            { nWidth = 20; m_nAxisMargin = 20; } //그외
	nSize = nWidth * 2;
	if(m_nDrawType == DRAW_EDIT_BTN_TYPE_XY)
		SetAxisMode(EN_DRAW_AXISMODE_PANTALINE_XY, nSize, nWidth);
	else if(m_nDrawType == DRAW_EDIT_BTN_TYPE_XZ)
		SetAxisMode(EN_DRAW_AXISMODE_PANTALINE_XZ, nSize, nWidth);
	else if(m_nDrawType == DRAW_EDIT_BTN_TYPE_YZ)
		SetAxisMode(EN_DRAW_AXISMODE_PANTALINE_YZ, nSize, nWidth);
	
	// 데이터 세팅.
	if(m_pTndnData) //Add,Mod대화상자.
	{		
		if(m_nDrawType == DRAW_EDIT_BTN_TYPE_XY)		  bXY		= TRUE;
		else if(m_nDrawType == DRAW_EDIT_BTN_TYPE_XZ)	bXY		= FALSE;
		else											                    return FALSE;

		if(bXY)
		{
			dPosX	= &(drawStruct.dVecPosX_XY);
			dPosYZ	= &(drawStruct.dVecPosY);
			bInput	= &(drawStruct.bVecInputPt_XY);
		}
		else
		{
			dPosX	= &(drawStruct.dVecPosX_XZ);
			dPosYZ	= &(drawStruct.dVecPosZ);
			bInput	= &(drawStruct.bVecInputPt_XZ);
		}

		drawStruct.Init();
		if(m_pTemplateCtrl->GetProfileDrawInfo(*m_pTndnData, bXY, *dPosX, *dPosYZ, *bInput))
		{
			m_mapDrawTendonData.clear();
			m_mapDrawTendonData.insert(std::pair<int, TndnDrawStruct>(0, drawStruct));
		}
	}
	else //Template 대화상자.
	{
		m_mapDrawTendonData.clear();
		m_pTemplateCtrl->GetKeyList(keyList);
		nSize = (int)keyList.size();

		BOOL bSuccess;

		
		for(i = 0 ; i < nSize; i++)
		{
			drawStruct.Init();
			TndnTemplate tndnTemplate;
			if(!m_pTemplateCtrl->GetTemplateNU(keyList[i], tndnTemplate))
			{
				ASSERT(0);
				continue;
			}	

			bSuccess = m_pTemplateCtrl->GetProfileDrawInfo(tndnTemplate, TRUE, drawStruct.dVecPosX_XY, drawStruct.dVecPosY, drawStruct.bVecInputPt_XY);
			if(!bSuccess)
			{
				continue;
			}
			bSuccess = m_pTemplateCtrl->GetProfileDrawInfo(tndnTemplate, FALSE, drawStruct.dVecPosX_XZ, drawStruct.dVecPosZ, drawStruct.bVecInputPt_XZ);
			if(bSuccess)
			{        
				drawStruct.nLinkKey = keyList[i] + 1;
				drawStruct.bPreInter    = tndnTemplate.bPreInter;
				drawStruct.dOffsetFront = tndnTemplate.dOffsetFront;
				drawStruct.dOffsetEnd   = tndnTemplate.dOffsetEnd;
				m_mapDrawTendonData.insert(std::pair<int, TndnDrawStruct>(keyList[i], drawStruct));
			}
		}
	}
	
	MakeDrawUnit(bFit);	

	return TRUE;
}

void CDrawEditBtn_TndnTemplate::MakeDrawSectUnit_YZ()
{
	int nSnapType = m_nSectSnapType; //좌표점 스넵을 사용
	double dOffsetX, dOffsetY;
	int i, j, nSize, nOutSize, nInSize;//, nLineSize;

	if(m_bCenter)
	{	dOffsetX = dOffsetY = 0.0; }
	else
	{
		dOffsetX = m_SectD.dYBar;
		dOffsetY = m_SectD.dZBar;
	}
	// Draw Shape ----------------------------
	T_POLYGON_R_GROUP PolyRGroup;     
	T_POLYLINE_R      polyLineUnit;   

	double dTempConvert;

	m_dBoundBox_Bottom = 0.0;
	m_dBoundBox_Top = 0.0;
	m_dBoundBox_Left = 0.0;
	m_dBoundBox_Right = 0.0;
	
	// Concrete
	nOutSize = m_arConcOutPoly.GetSize();
	PolyRGroup.List.SetSize(nOutSize);
	for(i=0 ; i<nOutSize ; i++)
	{
		nSize = m_arConcOutPoly[i].aVertex.GetSize();
		polyLineUnit.Init();
		polyLineUnit.arPoint.SetSize(nSize);	
		for(j=0 ; j<nSize ; j++)
		{
			dTempConvert = m_arConcOutPoly[i].aVertex[j].dX + dOffsetX;
			polyLineUnit.arPoint[j].x = m_pUnitCtrl->ConvertUnitDataIn(D_UNITSYS_BASE_LENGTH, dTempConvert);
			dTempConvert = m_arConcOutPoly[i].aVertex[j].dY + dOffsetY;
			polyLineUnit.arPoint[j].y = m_pUnitCtrl->ConvertUnitDataIn(D_UNITSYS_BASE_LENGTH, dTempConvert);
			
			CalcBoundingBox(polyLineUnit.arPoint[j].x, polyLineUnit.arPoint[j].y);
		}
		PolyRGroup.List.SetAt(i, polyLineUnit);		
	}

	if(nOutSize > 0)
		m_DrawCtrl.AddDrawUnit(m_uiDrawFormatKeySection, PolyRGroup,FALSE, 0, nSnapType);
	
	PolyRGroup.Init();
	nInSize = m_arConcInPoly.GetSize();
	PolyRGroup.List.SetSize(nInSize);
	for(i=0 ; i<nInSize ; i++)
	{
		nSize = m_arConcInPoly[i].aVertex.GetSize();
		polyLineUnit.Init();
		polyLineUnit.arPoint.SetSize(nSize);	
		for(j=0 ; j<nSize ; j++)
		{
			dTempConvert = m_arConcInPoly[i].aVertex[j].dX + dOffsetX;
			polyLineUnit.arPoint[j].x = m_pUnitCtrl->ConvertUnitDataIn(D_UNITSYS_BASE_LENGTH, dTempConvert);
			dTempConvert = m_arConcInPoly[i].aVertex[j].dY + dOffsetY;
			polyLineUnit.arPoint[j].y = m_pUnitCtrl->ConvertUnitDataIn(D_UNITSYS_BASE_LENGTH, dTempConvert);
		}
		PolyRGroup.List.SetAt(i, polyLineUnit);
	}

	if(nInSize > 0)
		m_DrawCtrl.AddDrawUnit(m_uiDrawFormatKeySection_Inside, PolyRGroup, FALSE, 0, nSnapType);	
	
	// Steel (Polygon)
	nOutSize = m_arSteelOutPoly.GetSize();
	PolyRGroup.List.SetSize(nOutSize);
	for(i=0 ; i<nOutSize ; i++)
	{
		nSize = m_arSteelOutPoly[i].aVertex.GetSize();
		polyLineUnit.Init();
		polyLineUnit.arPoint.SetSize(nSize);	
		for(j=0 ; j<nSize ; j++)
		{
			dTempConvert = m_arSteelOutPoly[i].aVertex[j].dX + dOffsetX;
			polyLineUnit.arPoint[j].x = m_pUnitCtrl->ConvertUnitDataIn(D_UNITSYS_BASE_LENGTH, dTempConvert);
			dTempConvert = m_arSteelOutPoly[i].aVertex[j].dY + dOffsetY;
			polyLineUnit.arPoint[j].y = m_pUnitCtrl->ConvertUnitDataIn(D_UNITSYS_BASE_LENGTH, dTempConvert);

			CalcBoundingBox(polyLineUnit.arPoint[j].x, polyLineUnit.arPoint[j].y);
		}
		PolyRGroup.List.SetAt(i, polyLineUnit);		
	}
	int nSteelFormatKey = m_uiDrawFormatKeySection;
	if(IsCompositeSection(m_SectD.nType)) nSteelFormatKey = m_uiDrawFormatKeySection;

	if(nOutSize > 0)
		m_DrawCtrl.AddDrawUnit(m_uiDrawFormatKeySection, PolyRGroup, FALSE, 0, nSnapType);
	
	PolyRGroup.Init();
	nInSize = m_arSteelInPoly.GetSize();
	PolyRGroup.List.SetSize(nInSize);
	for(i=0 ; i<nInSize ; i++)
	{
		nSize = m_arSteelInPoly[i].aVertex.GetSize();
		polyLineUnit.Init();
		polyLineUnit.arPoint.SetSize(nSize);	
		for(j=0 ; j<nSize ; j++)
		{
			dTempConvert = m_arSteelInPoly[i].aVertex[j].dX + dOffsetX;
			polyLineUnit.arPoint[j].x = m_pUnitCtrl->ConvertUnitDataIn(D_UNITSYS_BASE_LENGTH, dTempConvert);
			dTempConvert = m_arSteelInPoly[i].aVertex[j].dY + dOffsetY;
			polyLineUnit.arPoint[j].y = m_pUnitCtrl->ConvertUnitDataIn(D_UNITSYS_BASE_LENGTH, dTempConvert);
		}
		PolyRGroup.List.SetAt(i, polyLineUnit);
	}

	if(nInSize > 0)
		m_DrawCtrl.AddDrawUnit(m_uiDrawFormatKeySection_Inside, PolyRGroup, FALSE, 0, nSnapType);	
}


void CDrawEditBtn_TndnTemplate::MakeDrawSectUnit_XZXY()
{
	CUnitCtrl* pUnitCtrl = m_pUnitCtrl;
	CDgnTendonElemLengthCtrl* pLengthCtrl = m_pTemplateCtrl->GetLengthCtrl();

	int nCountLengthInfo = pLengthCtrl->GetLengthInfoCount();

	if(nCountLengthInfo < 1)
		return;

	T_POLYGON_R_GROUP PolyRGroup;
	PolyRGroup.List.SetSize(nCountLengthInfo);

// 	T_TEXT_R_GROUP tTextRGroup;
// 	tTextRGroup.List.SetSize(nCountLengthInfo);

	T_POINT_R_GROUP PointRGroup;
	PointRGroup.List.SetSize(nCountLengthInfo + 1);
	
	T_DIMENSION_R_GROUP tDimenGroup;
	tDimenGroup.List.SetSize(nCountLengthInfo + 1);

	double dMinPos = 100000000.0;
	double dMaxPos = -100000000.0;
	int i;

	for(i = 0 ; i < nCountLengthInfo ; i++)
	{
		lengthInfoIJ lenInfoIJ;
		pLengthCtrl->GetLengthInfoByIdx(i, lenInfoIJ, FALSE);	//Elem의 오프셋을 기준으로 구한다.
		const lengthInfo& lenInfoPre = lenInfoIJ.infoEndI;
		const lengthInfo& lenInfoNext = lenInfoIJ.infoEndJ;

		//세로선 계산을 위해서...
		if(i == 0)	PointRGroup.List[0].x = lenInfoPre.dPosX;
		PointRGroup.List[i + 1].x = lenInfoNext.dPosX; //x좌표 등록.

		//요소 번호 및 길이.
		double dLengthElem = lenInfoNext.dPosX - lenInfoPre.dPosX;
		dLengthElem = pUnitCtrl->ConvertUnitDataOut(D_UNITSYS_BASE_LENGTH, dLengthElem);
		dLengthElem = GetScaledElemLen(dLengthElem, (int)pLengthCtrl->GetElemKeyByIdx(i));
		CString strUnitSystem;
		if(!pUnitCtrl->GetUnitSystem(D_UNITSYS_BASE_LENGTH, strUnitSystem))			ASSERT(0);

	//	_stprintf(tTextRGroup.List[i].szText.GetBuffer(), _T("%d (%.5g %s)"), (int)pLengthCtrl->GetElemKeyByIdx(i), dLengthElem, strUnitSystem);
		_stprintf(tDimenGroup.List[i].szText.GetBuffer(), _T("%d (%.5g %s)"), (int)pLengthCtrl->GetElemKeyByIdx(i), dLengthElem, (LPCTSTR)strUnitSystem);
		//tTextRGroup.List[i].PPosition.x = (lenInfoNext.dPosX + lenInfoPre.dPosX) * 0.5;

		PolyRGroup.List[i].arPoint.SetSize(4);

		if(m_nDrawType == DRAW_EDIT_BTN_TYPE_XZ)
		{
			PolyRGroup.List[i].arPoint[0].x = lenInfoPre.dPosX;
			PolyRGroup.List[i].arPoint[0].y = -lenInfoPre.dBot;
			PolyRGroup.List[i].arPoint[1].x = lenInfoNext.dPosX;
			PolyRGroup.List[i].arPoint[1].y = -lenInfoNext.dBot;
			PolyRGroup.List[i].arPoint[2].x = lenInfoNext.dPosX;
			PolyRGroup.List[i].arPoint[2].y = lenInfoNext.dTop_Z;
			PolyRGroup.List[i].arPoint[3].x = lenInfoPre.dPosX;
			PolyRGroup.List[i].arPoint[3].y = lenInfoPre.dTop_Z;
		}
		if(m_nDrawType == DRAW_EDIT_BTN_TYPE_XY)
		{
			PolyRGroup.List[i].arPoint[0].x = lenInfoPre.dPosX;
			PolyRGroup.List[i].arPoint[0].y = -lenInfoPre.dLeft;
			PolyRGroup.List[i].arPoint[1].x = lenInfoNext.dPosX;
			PolyRGroup.List[i].arPoint[1].y = -lenInfoNext.dLeft;
			PolyRGroup.List[i].arPoint[2].x = lenInfoNext.dPosX;
			PolyRGroup.List[i].arPoint[2].y = lenInfoNext.dRight_Y;
			PolyRGroup.List[i].arPoint[3].x = lenInfoPre.dPosX;
			PolyRGroup.List[i].arPoint[3].y = lenInfoPre.dRight_Y;
			
		}
		//치수선 좌표.
		tDimenGroup.List[i].P1.x = lenInfoPre.dPosX;
		tDimenGroup.List[i].P2.x = lenInfoNext.dPosX;

		if(dMinPos > PolyRGroup.List[i].arPoint[0].y)	dMinPos = PolyRGroup.List[i].arPoint[0].y;
		if(dMinPos > PolyRGroup.List[i].arPoint[1].y)	dMinPos = PolyRGroup.List[i].arPoint[1].y;
		if(dMaxPos < PolyRGroup.List[i].arPoint[2].y)	dMaxPos = PolyRGroup.List[i].arPoint[2].y;
		if(dMaxPos < PolyRGroup.List[i].arPoint[3].y)	dMaxPos = PolyRGroup.List[i].arPoint[3].y;
	}
	
	m_DrawCtrl.AddDrawUnit(m_uiDrawFormatKeySection_XYZ, PolyRGroup);
	
	T_LINE_R_GROUP tLineRGroup;
	tLineRGroup.List.SetSize(1);

	tLineRGroup.List[0].P1.x = 0.0;
	tLineRGroup.List[0].P1.y = 0.0;
	tLineRGroup.List[0].P2.x = pLengthCtrl->GetTotalLength();
	tLineRGroup.List[0].P2.y = 0.0;
	if(m_bShowElemLine)
	{
		m_DrawCtrl.AddDrawUnit(m_uiDrawFormatKeyElemLine, tLineRGroup); //중신선을 그린다.
	}

	for(i = 0 ; i < nCountLengthInfo; i++)
	{
		tDimenGroup.List[i].P1.y = dMaxPos;
		tDimenGroup.List[i].P2.y = dMaxPos;
	}
 	tDimenGroup.List[nCountLengthInfo].P1.x = 0.0;
 	tDimenGroup.List[nCountLengthInfo].P2.x = pLengthCtrl->GetTotalLength();
 	tDimenGroup.List[nCountLengthInfo].P1.y = dMaxPos;
 	tDimenGroup.List[nCountLengthInfo].P2.y = dMaxPos;
	m_DrawCtrl.AddDrawUnit(m_uiDrawFormatKeyDimen, tDimenGroup);

	m_dTotalHeight = dMaxPos - dMinPos; 
}

void CDrawEditBtn_TndnTemplate::SetDrawFormat()
{
	int formatIdx = 0;

	T_DRAW_FORMAT	DrawFormat;
	T_TEXT_FORMAT	TextFormat;
	T_DIM_FORMAT	DimFormat;

	DimFormat.Init();
	DimFormat.fontColor = RGB(0,0,0);
	DimFormat.bMainLine = TRUE;
	DimFormat.dRTextSpace = 0.05;
	DimFormat.dRDimSpace = 0.1;
	
	DimFormat.fontSize = 12;
	DimFormat.dRGuideIn = 1.1;
	DimFormat.dRGuideOut = 0.3;
	DimFormat.dimType = EN_DIM_TYPE_BOTTOM;
	m_uiDrawFormatKeyDimen_Vert = formatIdx++;
	m_DrawCtrl.m_arDimFormat.SetAt(m_uiDrawFormatKeyDimen_Vert, DimFormat);
	
	DimFormat.fontSize = 13;
	DimFormat.dRGuideIn = 0.4;
	DimFormat.dRGuideOut = 0.7;
	DimFormat.dimType = EN_DIM_TYPE_TOP;
	m_uiDrawFormatKeyDimen = formatIdx++;
	m_DrawCtrl.m_arDimFormat.SetAt(m_uiDrawFormatKeyDimen, DimFormat);
	
	DimFormat.fontSize = 14;
	DimFormat.dRGuideIn = 0.4;
	DimFormat.dRGuideOut = 0.7;
	DimFormat.dimType = EN_DIM_TYPE_TOP;
	m_uiDrawFormatKeyDimen_Horz = formatIdx++;
	m_DrawCtrl.m_arDimFormat.SetAt(m_uiDrawFormatKeyDimen_Horz, DimFormat);

	TextFormat.fontColor = RGB(0,0,0);
	TextFormat.Size = 13;
	TextFormat.positionType = EN_POSITION_TYPE_LEFT_BOTTOM;
	m_uiDrawFormatKeyText = formatIdx++;
	m_DrawCtrl.m_arTextFormat.SetAt(m_uiDrawFormatKeyText, TextFormat);

	DrawFormat.bLine          = TRUE;
	DrawFormat.bFill          = FALSE;
	DrawFormat.fillColor      = RGB(0,0,0);
	DrawFormat.lineColor      = RGB(240,   50,   50);
	m_uiDrawFormatKeyElemLine = formatIdx++;
	m_DrawCtrl.m_arDrawFormat.SetAt(m_uiDrawFormatKeyElemLine, DrawFormat);

	DrawFormat.Width          = 1;
	DrawFormat.nPenStyle      = PS_DOT;
	DrawFormat.bLine          = TRUE;
	DrawFormat.bFill          = TRUE;
	DrawFormat.fillColor      = RGB(220, 220, 220);
	DrawFormat.lineColor      = RGB(220, 220, 220);
	m_uiDrawFormatKeyTndn_Debonded = formatIdx++;   // Tendon Debnded
	m_DrawCtrl.m_arDrawFormat.SetAt(m_uiDrawFormatKeyTndn_Debonded, DrawFormat);

	DrawFormat.Width          = 1;
	DrawFormat.nPenStyle      = PS_SOLID;
	DrawFormat.bLine          = TRUE;	
	DrawFormat.bFill          = TRUE;		
	DrawFormat.fillColor      = RGB(220, 220, 220);
	DrawFormat.lineColor      = RGB(0,   0,   0); 
	m_uiDrawFormatKeySection_Line = formatIdx++;
	m_DrawCtrl.m_arDrawFormat.SetAt(m_uiDrawFormatKeySection_Line, DrawFormat);	
	
	DrawFormat.bLine          = TRUE;	
	DrawFormat.bFill          = TRUE;		
	DrawFormat.fillColor      = RGB(220, 220, 220);
	DrawFormat.lineColor      = RGB(0,   0,   0); 
	m_uiDrawFormatKeySection = formatIdx++;
	m_DrawCtrl.m_arDrawFormat.SetAt(m_uiDrawFormatKeySection, DrawFormat);
	
	DrawFormat.bLine          = TRUE;	
	DrawFormat.bFill          = TRUE;		
	DrawFormat.fillColor      = RGB(255, 255, 255);
	DrawFormat.lineColor      = RGB(0,   0,   0); 
	m_uiDrawFormatKeySection_Inside = formatIdx++;
	m_DrawCtrl.m_arDrawFormat.SetAt(m_uiDrawFormatKeySection_Inside, DrawFormat);

	DrawFormat.bLine          = TRUE;	
	DrawFormat.bFill          = TRUE;		
	DrawFormat.fillColor      = RGB(220, 220, 220);
	DrawFormat.lineColor      = RGB(0,   0,   0); 
	m_uiDrawFormatKeySection_XYZ = formatIdx++;
	m_DrawCtrl.m_arDrawFormat.SetAt(m_uiDrawFormatKeySection_XYZ, DrawFormat);
	
	DrawFormat.bLine          = TRUE;
	DrawFormat.bFill          = TRUE;
	DrawFormat.fillColor      = RGB(245, 230, 230);
	DrawFormat.lineColor      = RGB(230,  50,  50);
	m_uiDrawFormatKeyTndn = formatIdx++;
	m_DrawCtrl.m_arDrawFormat.SetAt(m_uiDrawFormatKeyTndn, DrawFormat);

	DrawFormat.bLine          = TRUE;
	DrawFormat.bFill          = FALSE;
	DrawFormat.lineColor      = RGB(20, 20, 20);
	m_uiDrawFormatKeyTndn_Preview = formatIdx++;
	m_DrawCtrl.m_arDrawFormat.SetAt(m_uiDrawFormatKeyTndn_Preview, DrawFormat);


	DrawFormat.bLine          = TRUE;
	DrawFormat.bFill          = TRUE;
	DrawFormat.fillColor      = RGB(250, 20, 20);
	DrawFormat.lineColor      = RGB(250, 20, 20);
	DrawFormat.Width          = 8;
	DrawFormat.pointType      = EN_POINT_TYPE_CIRCLE;
	m_uiDrawFormatKeyOrigin = formatIdx++;
	m_DrawCtrl.m_arDrawFormat.SetAt(m_uiDrawFormatKeyOrigin, DrawFormat); 

	m_CurrDrawFormat.nPenStyle = PS_DOT;
}

void CDrawEditBtn_TndnTemplate::ResetVariable( BOOL bWithSect )
{
	if(bWithSect)
	{
		ResetVariable_Sect();
	}

	m_nDrawType = DRAW_EDIT_BTN_TYPE_YZ;
	m_bCenter = TRUE;
	m_nSectSnapType = 1;
	m_bLineDetail = TRUE;
	m_bIsOverlapChk = TRUE;	
}


BOOL CDrawEditBtn_TndnTemplate::IsCompositeSection(int nShape)
{
	return (nShape == DGN_SECT_TYPE_COMPO_B  || nShape == DGN_SECT_TYPE_COMPO_I  || 
		      nShape == DGN_SECT_TYPE_COMPO_CI || nShape == DGN_SECT_TYPE_COMPO_CT || 
		      nShape == DGN_SECT_TYPE_COMPO_PC || nShape == DGN_SECT_TYPE_COMPO_G); 	
}

void CDrawEditBtn_TndnTemplate::SetDrawType( DRAW_EDIT_BTN_TYPE nType, BOOL bRemake /*= FALSE*/ )
{
	m_nDrawType = nType;
	
	int nWidth;
	if(m_pTndnData) { nWidth = 15; m_nAxisMargin = 15; } //Addmod 대화상자는 조금 작게.
	else            { nWidth = 20; m_nAxisMargin = 20; } //그외
	int nSize = nWidth * 2;

	if(m_nDrawType == DRAW_EDIT_BTN_TYPE_XY)
		SetAxisMode(EN_DRAW_AXISMODE_PANTALINE_XY, nSize, nWidth);
	else if(m_nDrawType == DRAW_EDIT_BTN_TYPE_XZ)
		SetAxisMode(EN_DRAW_AXISMODE_PANTALINE_XZ, nSize, nWidth);
	else if(m_nDrawType == DRAW_EDIT_BTN_TYPE_YZ)
		SetAxisMode(EN_DRAW_AXISMODE_PANTALINE_YZ, nSize, nWidth);

	if(bRemake)
	{
		MakeDrawUnit(TRUE);
	}
}

void CDrawEditBtn_TndnTemplate::Redraw( BOOL bFit )
{

	if(bFit)
	{
		int nIdx;

		if(m_nDrawType == DRAW_EDIT_BTN_TYPE_YZ)  nIdx = 0; //Section
		else if(m_pTndnData)                      nIdx = 1; //Add/Mod 대화상자.
		else                                      nIdx = 2; //Template 대화상자.

		m_DrawCtrl.AutoPaperFormat(m_Canvas, m_dFitMargin * m_dMarginFactor[nIdx], m_dFitMargin, m_dFitMargin, m_dFitMargin);
	}

	if(this)
	{
		if(IsWindow(this->m_hWnd))
		{
			RedrawWindow();
		}
	}	
}

void CDrawEditBtn_TndnTemplate::MakeDrawTndn_YZ()
{
	vecTndnKey keyList;
	int nSize, i;
	
	if(m_nDrawType == DRAW_EDIT_BTN_TYPE_XY
		|| m_nDrawType == DRAW_EDIT_BTN_TYPE_XZ)
		return;
	
	if(m_pTndnData)
	{
		return;
	}
	else
	{
		m_pTemplateCtrl->GetKeyList(keyList);
	}
	
	nSize = (int)keyList.size();
	
	double dPosY;
	double dPosZ;

	double dVertPos=0.;
	m_mapVertDim.clear();
	for(i = 0 ; i < nSize ; i++)
	{
		TndnDrawStruct* pDrawStruct = &(m_mapDrawTendonData[keyList[i]]);

		if(!pDrawStruct->GetPosYZByPosX(m_dPos, TRUE, dPosY))
			continue;
		if(!pDrawStruct->GetPosYZByPosX(m_dPos, FALSE, dPosZ))
			continue;

		T_CIRCLE_R_GROUP tCircleGroup;
		tCircleGroup.List.SetSize(1);
		tCircleGroup.List[0].CenterP.x = dPosY - m_dGapY;
		tCircleGroup.List[0].CenterP.y = dPosZ - m_dGapZ;

		dVertPos -= m_dBoundBox_Bottom;
		dVertPos = floor( 1000.0 * tCircleGroup.List[0].CenterP.y + 0.5 ) / 1000.0;

		if(m_mapVertDim.find(dVertPos) == m_mapVertDim.end())
			m_mapVertDim[dVertPos] = 1;
		else
			m_mapVertDim[dVertPos]++;


		if(pDrawStruct->dTendonDia <= 1E-5)
		{
			tCircleGroup.List[0].Radius = m_dTotalHeight * m_dTendonZoomFactor;
		}
		else
		{
			tCircleGroup.List[0].Radius = pDrawStruct->dTendonDia;
		}

		m_DrawCtrl.AddDrawUnit(m_uiDrawFormatKeyTndn, tCircleGroup, FALSE, pDrawStruct->nLinkKey, 0, TRUE, 0.0, 0.0, FALSE, 0);
	}
}

void CDrawEditBtn_TndnTemplate::MakeDrawTndn_XZXY_Debonded(TndnDrawStruct* pDrawStruct, T_LINE_R_GROUP& tLineGroup, T_LINE_R_GROUP& tLineGroupF, T_LINE_R_GROUP& tLineGroupE)
{
	double dTol = 1.0e-10;
	T_LINE_R lineUnit;

	int nPointSize = tLineGroup.List.GetSize();

	if(pDrawStruct->dOffsetFront>0.0)
	{
		double dlSum=0.0;
		for(int j = 0 ; j < nPointSize; j++)
		{
			dlSum +=CMathFunc::mathLength(tLineGroup.List[j].P2.x-tLineGroup.List[j].P1.x, tLineGroup.List[j].P2.y-tLineGroup.List[j].P1.y, 0.0);
			lineUnit.P1.x = tLineGroup.List[j].P1.x;
			lineUnit.P1.y = tLineGroup.List[j].P1.y;
			lineUnit.P2.x = tLineGroup.List[j].P2.x; 
			lineUnit.P2.y = tLineGroup.List[j].P2.y;


			if(dlSum == pDrawStruct->dOffsetFront)
			{
				tLineGroupF.List.Add(lineUnit);
				 break;
			}
			else if(dlSum < pDrawStruct->dOffsetFront)
			{
				tLineGroupF.List.Add(lineUnit);
			}
			else if(dlSum > pDrawStruct->dOffsetFront)
			{
				double dl = fabs(tLineGroup.List[j].P1.x-tLineGroup.List[j].P2.x);
				if(dl < dTol) break;
				double dLeft = 1.0-(dlSum - pDrawStruct->dOffsetFront)/dl;

				double dXtemp = CMathFunc::mathInterpolate(tLineGroup.List[j].P1.x, tLineGroup.List[j].P2.x, dLeft);
				double dYtemp = CMathFunc::mathInterpolate(tLineGroup.List[j].P1.y, tLineGroup.List[j].P2.y, dLeft);

				lineUnit.P2.x = dXtemp;
				lineUnit.P2.y = dYtemp;
				tLineGroupF.List.Add(lineUnit);
				break;
			}
		}
	}

	if(pDrawStruct->dOffsetEnd>0.0)
	{
		double dlSum=0.0;
		for(int j = nPointSize-1; j > 0; j--)
		{
			dlSum +=CMathFunc::mathLength(tLineGroup.List[j].P2.x-tLineGroup.List[j].P1.x, tLineGroup.List[j].P2.y-tLineGroup.List[j].P1.y, 0.0);
			lineUnit.P1.x = tLineGroup.List[j].P1.x;
			lineUnit.P1.y = tLineGroup.List[j].P1.y;
			lineUnit.P2.x = tLineGroup.List[j].P2.x; 
			lineUnit.P2.y = tLineGroup.List[j].P2.y;

			if(dlSum == pDrawStruct->dOffsetEnd) 
			{
				tLineGroupE.List.Add(lineUnit);
				break;
			}
			else if(dlSum < pDrawStruct->dOffsetEnd)
			{
				tLineGroupE.List.Add(lineUnit);
			}
			else if(dlSum > pDrawStruct->dOffsetEnd)
			{
				double dl = fabs(tLineGroup.List[j].P1.x-tLineGroup.List[j].P2.x);
				if(dl < dTol) break;
				double dLeft = 1.0-(dlSum - pDrawStruct->dOffsetEnd)/dl;

				double dXtemp = CMathFunc::mathInterpolate(tLineGroup.List[j].P2.x, tLineGroup.List[j].P1.x, dLeft);
				double dYtemp = CMathFunc::mathInterpolate(tLineGroup.List[j].P2.y, tLineGroup.List[j].P1.y, dLeft);

				lineUnit.P1.x = dXtemp;
				lineUnit.P1.y = dYtemp;
				tLineGroupE.List.Add(lineUnit);
				break;
			}
		}
	}

}

void CDrawEditBtn_TndnTemplate::MakeDrawTndn_XZXY()
{
 	vecTndnKey keyList;
	int nSize, i, j;
 	BOOL bXY;

	if(m_nDrawType == DRAW_EDIT_BTN_TYPE_XY)		bXY		= TRUE;
	else if(m_nDrawType == DRAW_EDIT_BTN_TYPE_XZ)	bXY		= FALSE;
	else											return;

	std::vector<double>*	dPosX;
	std::vector<double>*	dPosYZ;
	std::vector<BOOL>*		bInput;

 	if(m_pTndnData)
	{
		keyList.clear();
		keyList.push_back(0);
	}
	else
	{
		m_pTemplateCtrl->GetKeyList(keyList);
	}

	nSize = (int)keyList.size();

	for(i = 0 ; i < nSize ; i++)
	{
		TndnDrawStruct* pDrawStruct = &(m_mapDrawTendonData[keyList[i]]);

		if(bXY)
		{
			dPosX = &(pDrawStruct->dVecPosX_XY);
			dPosYZ = &(pDrawStruct->dVecPosY);
			bInput = &(pDrawStruct->bVecInputPt_XY);
		}
		else
		{
			dPosX = &(pDrawStruct->dVecPosX_XZ);
			dPosYZ = &(pDrawStruct->dVecPosZ);
			bInput = &(pDrawStruct->bVecInputPt_XZ);
		}

		int nPointSize = (int)dPosX->size();
		
		if(nPointSize < 2) return;

		T_LINE_R_GROUP tLineGroup;
		tLineGroup.List.SetSize(nPointSize - 1);

		for(j = 0 ; j < nPointSize - 1; j++)
		{
			tLineGroup.List[j].P1.x = (*dPosX)[j];
			tLineGroup.List[j].P1.y = (*dPosYZ)[j];
			tLineGroup.List[j].P2.x = (*dPosX)[j + 1];
			tLineGroup.List[j].P2.y = (*dPosYZ)[j + 1];
		}

		T_LINE_R_GROUP tLineGroupF, tLineGroupE;
		if(pDrawStruct->bPreInter)
		{
			MakeDrawTndn_XZXY_Debonded(pDrawStruct, tLineGroup, tLineGroupF, tLineGroupE);
		}

		if(pDrawStruct->nLinkKey != 0 )
		{
			m_DrawCtrl.AddDrawUnit(m_uiDrawFormatKeyElemLine, tLineGroup, FALSE, pDrawStruct->nLinkKey, 0, TRUE, 0.0, 0.0, FALSE, 0);

			if(tLineGroupF.List.GetSize()>0)
				m_DrawCtrl.AddDrawUnit(m_uiDrawFormatKeyTndn_Debonded, tLineGroupF, FALSE, pDrawStruct->nLinkKey, 0, TRUE, 0.0, 0.0, FALSE, 0);

			if(tLineGroupE.List.GetSize()>0)
				m_DrawCtrl.AddDrawUnit(m_uiDrawFormatKeyTndn_Debonded, tLineGroupE, FALSE, pDrawStruct->nLinkKey, 0, TRUE, 0.0, 0.0, FALSE, 0);
		}
		else
			m_DrawCtrl.AddDrawUnit(m_uiDrawFormatKeyElemLine, tLineGroup);
	}
}

void CDrawEditBtn_TndnTemplate::MakeDrawPreviewData()
{
	if(m_pTndnData)			return; //여기서(Add/Mod대화상자)는 일단 프리뷰를 사용하지 않는다.
	if(!m_bShowPreview)		return;	//프리뷰 데이터를 만들지 않는다...

	int i, j;
	int nSize;

	double dPosY;
	double dPosZ;

	nSize = (int)m_vecDrawTendonData_Preview.size();

	if(m_nDrawType == DRAW_EDIT_BTN_TYPE_YZ)// yz단면 그릴때.
	{
		for(i = 0 ; i < nSize; i++)
		{
			TndnDrawStruct* pDrawStruct = &( m_vecDrawTendonData_Preview[i] );
			
			if(!pDrawStruct->GetPosYZByPosX(m_dPos, TRUE, dPosY))
				continue;
			if(!pDrawStruct->GetPosYZByPosX(m_dPos, FALSE, dPosZ))
				continue;
			
			T_CIRCLE_R_GROUP tCircleGroup;
			tCircleGroup.List.SetSize(1);
			tCircleGroup.List[0].CenterP.x = dPosY - m_dGapY;
			tCircleGroup.List[0].CenterP.y = dPosZ - m_dGapZ;
			if(pDrawStruct->dTendonDia <= 1E-5)
			{
				tCircleGroup.List[0].Radius = m_dTotalHeight * m_dTendonZoomFactor;
			}
			else
			{
				tCircleGroup.List[0].Radius = pDrawStruct->dTendonDia;
			}
			
			m_DrawCtrl.AddDrawUnit(m_uiDrawFormatKeyTndn_Preview, tCircleGroup);
		}
	}
	else if(m_nDrawType == DRAW_EDIT_BTN_TYPE_XY || m_nDrawType == DRAW_EDIT_BTN_TYPE_XZ)
 	{
		BOOL bXY;
		
		if(m_nDrawType == DRAW_EDIT_BTN_TYPE_XY)		bXY		= TRUE;
		else if(m_nDrawType == DRAW_EDIT_BTN_TYPE_XZ)	bXY		= FALSE;
		else											return;
		
		std::vector<double>*	dPosX;
		std::vector<double>*	dPosYZ;
		std::vector<BOOL>*		bInput;
		
		for(i = 0 ; i < nSize ; i++)
		{
			TndnDrawStruct* pDrawStruct = &(m_vecDrawTendonData_Preview[i]);
			
			if(bXY)
			{
				dPosX = &(pDrawStruct->dVecPosX_XY);
				dPosYZ = &(pDrawStruct->dVecPosY);
				bInput = &(pDrawStruct->bVecInputPt_XY);
			}
			else
			{
				dPosX = &(pDrawStruct->dVecPosX_XZ);
				dPosYZ = &(pDrawStruct->dVecPosZ);
				bInput = &(pDrawStruct->bVecInputPt_XZ);
			}
			
			int nPointSize = (int)dPosX->size();
			
			if(nPointSize < 2)
			return;
			
			T_LINE_R_GROUP tLineGroup;
			tLineGroup.List.SetSize(nPointSize - 1);
			
			for(j = 0 ; j < nPointSize - 1; j++)
			{
				tLineGroup.List[j].P1.x = (*dPosX)[j];
				tLineGroup.List[j].P1.y = (*dPosYZ)[j];
				tLineGroup.List[j].P2.x = (*dPosX)[j + 1];
				tLineGroup.List[j].P2.y = (*dPosYZ)[j + 1];
			}

			if(pDrawStruct->nLinkKey > 0 && pDrawStruct->nLinkKey <= nSize)
				m_DrawCtrl.AddDrawUnit(m_uiDrawFormatKeyTndn_Preview, tLineGroup, FALSE, pDrawStruct->nLinkKey, 0, TRUE, 0.0, 0.0, FALSE, 0);
			else
				m_DrawCtrl.AddDrawUnit(m_uiDrawFormatKeyTndn_Preview, tLineGroup);
		}
	}	
}

BOOL CDrawEditBtn_TndnTemplate::SetPreviewData( const vecTndnTemp& vecTndnTempInfo, BOOL bFit )
{
	int nSize, i;
	BOOL bSuccess;
	TndnDrawStruct drawStruct;
	
	nSize = (int)vecTndnTempInfo.size();
	
	m_vecDrawTendonData_Preview.clear();
	m_vecDrawTendonData_Preview.resize(nSize);

	for(i = 0 ; i < nSize; i++)
	{
		drawStruct.Init();
		const TndnTemplate& tndnTemplate = vecTndnTempInfo[i];
		
		bSuccess = m_pTemplateCtrl->GetProfileDrawInfo(tndnTemplate, TRUE, drawStruct.dVecPosX_XY, drawStruct.dVecPosY, drawStruct.bVecInputPt_XY);
		if(!bSuccess)
		{
			ASSERT(0);
			continue;
		}
		bSuccess = m_pTemplateCtrl->GetProfileDrawInfo(tndnTemplate, FALSE, drawStruct.dVecPosX_XZ, drawStruct.dVecPosZ, drawStruct.bVecInputPt_XZ);
		if(bSuccess)
		{
			drawStruct.nLinkKey = 0;
			m_vecDrawTendonData_Preview[i] = drawStruct;
		}
		else
		{
			ASSERT(0);
		}
	}
	
	MakeDrawUnit(bFit);	
	
	return TRUE;
}

void CDrawEditBtn_TndnTemplate::SetSelectedLinkKeyListByTndnKey( const vecTndnKey& keyList, BOOL bRedraw /*= TRUE*/ )
{	
	nrUINT nrUint;
	int nSize, i; 
	
	nSize = (int)keyList.size();
	
	nrUint.SetSize(nSize);
	for(i = 0 ; i < nSize; i++)
	{
		nrUint[i] = keyList[i] + 1;
	}
	
	SetSelectedLinkKeyList(nrUint, bRedraw);	
}

void CDrawEditBtn_TndnTemplate::CalcBoundingBox( double dX, double dY, double* dLeft, double* dRight, double* dTop, double* dBottom )
{
	if(dRight  == NULL) dRight  = &m_dBoundBox_Right;
	if(dLeft   == NULL) dLeft   = &m_dBoundBox_Left;
	if(dTop    == NULL) dTop    = &m_dBoundBox_Top;
	if(dBottom == NULL) dBottom = &m_dBoundBox_Bottom;

	if(dX > *dRight)  *dRight  = dX;
	if(dX < *dLeft)   *dLeft   = dX;
	if(dY > *dTop)    *dTop    = dY;
	if(dY < *dBottom) *dBottom = dY;
}
void CDrawEditBtn_TndnTemplate::MakeDrawDim_YZ()
{
	std::map<double, int>::iterator itrMapDim;
	CString strLenSys;
	//CString strTemp;
	CString strFormat;
	double dFactor = m_pUnitCtrl->GetConvertFactorCurrent(D_UNITSYS_BASE_LENGTH);
	m_pUnitCtrl->GetUnitSystemLength(strLenSys);
	
	if(dFactor <= 0.001 + 1E-5)
		strFormat = _T("%3d @ %8.0f %s");
	else if( dFactor <= 0.01 + 1E-5)
		strFormat = _T("%3d @ %8.1f %s");
	else if( dFactor <= 0.1 + 1E-5)
		strFormat = _T("%3d @ %8.2f %s");
	else
		strFormat = _T("%3d @ %8.3f %s");
	
	T_LINE_R_GROUP tLineGroup;
	T_TEXT_R_GROUP tTextGroup;
	
	double dPosVert;
	double dPosTemp = m_dBoundBox_Left - fabs((m_dTotalWidth + m_dTotalHeight) * 0.35);
	tLineGroup.List.SetSize(m_mapVertDim.size());
	tTextGroup.List.SetSize(m_mapVertDim.size());
	int idx = 0;
	for( itrMapDim= m_mapVertDim.begin();itrMapDim != m_mapVertDim.end(); itrMapDim++)
	{
		//strTemp.Format(strFormat, itrMapDim->second, (itrMapDim->first / dFactor), strLenSys);
		dPosVert = itrMapDim->first;
		tLineGroup.List[idx].P1.x = m_dBoundBox_Left;
		tLineGroup.List[idx].P1.y = dPosVert;
		tLineGroup.List[idx].P2.x = dPosTemp;
		tLineGroup.List[idx].P2.y = dPosVert;
		tTextGroup.List[idx].PPosition.x = dPosTemp;
		tTextGroup.List[idx].PPosition.y = dPosVert;
		_stprintf(tTextGroup.List[idx].szText.GetBuffer(), strFormat, itrMapDim->second,
			(( itrMapDim->first - m_dBoundBox_Bottom ) / dFactor), strLenSys);

		idx++;
	}

	m_DrawCtrl.AddDrawUnit(m_uiDrawFormatKeySection, tLineGroup);
	m_DrawCtrl.AddDrawUnit(m_uiDrawFormatKeyText, tTextGroup);
	
	if(dFactor <= 0.001 + 1E-5)
		strFormat = _T("%.0f %s");
	else if( dFactor <= 0.01 + 1E-5)
		strFormat = _T("%.1f %s");
	else if( dFactor <= 0.1 + 1E-5)
		strFormat = _T("%.2f %s");
	else
		strFormat = _T("%.3f %s");

	T_DIMENSION_R_GROUP tDimGroup;
	tDimGroup.List.SetSize(1);
	tDimGroup.List[0].P1.x = m_dBoundBox_Left;
	tDimGroup.List[0].P1.y = m_dBoundBox_Top;
	tDimGroup.List[0].P2.x = m_dBoundBox_Right;
	tDimGroup.List[0].P2.y = m_dBoundBox_Top;
	_stprintf(tDimGroup.List[0].szText.GetBuffer(), strFormat, ( m_dTotalWidth / dFactor ), strLenSys);
	m_DrawCtrl.AddDrawUnit(m_uiDrawFormatKeyDimen_Horz, tDimGroup);

	tDimGroup.List[0].P1.x = m_dBoundBox_Right;
	tDimGroup.List[0].P1.y = m_dBoundBox_Top;
	tDimGroup.List[0].P2.x = m_dBoundBox_Right;
	tDimGroup.List[0].P2.y = m_dBoundBox_Bottom;
	_stprintf(tDimGroup.List[0].szText.GetBuffer(), strFormat, ( m_dTotalHeight / dFactor ), strLenSys);
	m_DrawCtrl.AddDrawUnit(m_uiDrawFormatKeyDimen_Vert, tDimGroup);
}

void CDrawEditBtn_TndnTemplate::DrawAxisUnit(CDrawCtrl* pDrawCtrl, int nAxisMode, int nAxisSize, int nAxisWidth, BOOL bCapital,
																						 T_DRAW_FORMAT AxisDrawFormat, BOOL bCDC_AxisOrgPoint, CPoint AxisOrgPoint, T_POINT AxisOrgPointR,
																						 double dAxis_Vector1[2], double dAxis_Vector2[2])
{	
	BOOL bLocalAxis = (nAxisMode == EN_DRAW_AXISMODE_PANTALINE_XY_L || nAxisMode == EN_DRAW_AXISMODE_PANTALINE_YZ_L || nAxisMode == EN_DRAW_AXISMODE_PANTALINE_XZ_L);

	if(nAxisMode == EN_DRAW_AXISMODE_PANTALINE_XY   || nAxisMode == EN_DRAW_AXISMODE_PANTALINE_YZ   || nAxisMode == EN_DRAW_AXISMODE_PANTALINE_XZ ||
	   nAxisMode == EN_DRAW_AXISMODE_PANTALINE_XY_L || nAxisMode == EN_DRAW_AXISMODE_PANTALINE_YZ_L || nAxisMode == EN_DRAW_AXISMODE_PANTALINE_XZ_L)
	{
		// 원점 & 축방향
		CPoint OrgP;            
		OrgP.x = OrgP.y = 0;
		double dAxisVector1[2]; 
		dAxisVector1[0] = dAxisVector1[1] = 0.0;
		double dAxisVector2[2];
		dAxisVector2[0] = dAxisVector2[1] = 0.0;

		if(!bLocalAxis)
		{
			OrgP = pDrawCtrl->m_PaperFormat.OrgPoint;
			if(pDrawCtrl->m_DrawFunc.IsEMF()) OrgP.y *= -1;
			dAxisVector1[0] = 1.0;  
			dAxisVector1[1] = 0.0;
			dAxisVector2[0] = 0.0;  
			dAxisVector2[1] = (pDrawCtrl->m_DrawFunc.IsEMF() ? 1.0 : -1.0);
		}
		else 
		{
			if(!bCDC_AxisOrgPoint) AxisOrgPointConvertDrawData(TRUE, pDrawCtrl, AxisOrgPoint, AxisOrgPointR); //입력받은 원점 좌표가 실제좌표계인 경우 CDC좌표기준으로 바꾸어 준다.
			OrgP.x = pDrawCtrl->m_PaperFormat.OrgPoint.x + AxisOrgPoint.x;
			OrgP.y = pDrawCtrl->m_PaperFormat.OrgPoint.y + AxisOrgPoint.y;
			dAxisVector1[0] = dAxis_Vector1[0];
			dAxisVector1[1] = (pDrawCtrl->m_DrawFunc.IsEMF() ? 1.0 : -1.0) * dAxis_Vector1[1];
			dAxisVector2[0] = dAxis_Vector2[0]; 
			dAxisVector2[1] = (pDrawCtrl->m_DrawFunc.IsEMF() ? 1.0 : -1.0) * dAxis_Vector2[1];
		}

		if(TRUE)
		{
			// 항상 좌하단에 보여지게 하기 위해서..
			CRect rectWnd;
			GetWindowRect(&rectWnd);
			int nHeight = rectWnd.Height();
			int nMargin = m_nAxisMargin;
			OrgP.x = nMargin;
			OrgP.y = nHeight - nMargin - (nMargin / 3);
		}

		if(m_DrawCtrl.m_nMirrorMode == EN_MIRRORVIEW_TYPE_HORIZONTAL) { dAxisVector1[0] *= -1.0;  dAxisVector2[0] *= -1.0; }
		if(m_DrawCtrl.m_nMirrorMode == EN_MIRRORVIEW_TYPE_VERTICAL  ) { dAxisVector1[1] *= -1.0;  dAxisVector2[1] *= -1.0; }


		int nCount = 0;

		T_LINE_GROUP lineGroup;
		T_LINE       lineUnit;
		lineGroup.List.RemoveAll();	
		//원점
		lineUnit.P1.x = OrgP.x - long(dAxisVector2[0] * (nAxisWidth / 4)); 
		lineUnit.P1.y = OrgP.y - long(dAxisVector2[1] * (nAxisWidth / 4));		
		lineUnit.P2.x = OrgP.x + long(dAxisVector2[0] * (nAxisWidth / 4)); 
		lineUnit.P2.y = OrgP.y + long(dAxisVector2[1] * (nAxisWidth / 4));
		lineGroup.List.Add(lineUnit);
		lineUnit.P1.x = OrgP.x - long(dAxisVector1[0] * (nAxisWidth / 4));  
		lineUnit.P1.y = OrgP.y - long(dAxisVector1[1] * (nAxisWidth / 4));		
		lineUnit.P2.x = OrgP.x + long(dAxisVector1[0] * (nAxisWidth / 4));  
		lineUnit.P2.y = OrgP.y + long(dAxisVector1[1] * (nAxisWidth / 4));
		lineGroup.List.Add(lineUnit);

		CPoint TextOrgP;
		double dDiagVectorTR[2]; 
		dDiagVectorTR[0] = 1.0;  
		dDiagVectorTR[1] = (pDrawCtrl->m_DrawFunc.IsEMF() ? 1.0 : -1.0);
		double dDiagVectorTL[2]; 
		dDiagVectorTL[0] = -1.0;
		dDiagVectorTL[1] = (pDrawCtrl->m_DrawFunc.IsEMF() ? 1.0 : -1.0);
		double dDownVector[2]; 
		dDownVector[0] = 0.0;
		dDownVector[1] = (pDrawCtrl->m_DrawFunc.IsEMF() ? -1.0 : 1.0);
		// X
		if(nAxisMode == EN_DRAW_AXISMODE_PANTALINE_XY   || nAxisMode == EN_DRAW_AXISMODE_PANTALINE_XZ ||
		   nAxisMode == EN_DRAW_AXISMODE_PANTALINE_XY_L || nAxisMode == EN_DRAW_AXISMODE_PANTALINE_XZ_L)
		{
			TextOrgP.x = OrgP.x + long(dAxisVector1[0] * (nAxisSize-nAxisWidth / 2 - nAxisWidth / 4));
			TextOrgP.y = OrgP.y + long(dAxisVector1[1] * (nAxisSize-nAxisWidth / 2 - nAxisWidth / 4));			

			if(bLocalAxis)
			{
				if(pDrawCtrl->m_DrawFunc.IsEMF())
				{ 
					dDiagVectorTR[0] = dAxisVector1[0]        - dAxisVector1[1]; 
					dDiagVectorTR[1] = dAxisVector1[1]        + dAxisVector1[0];
					dDiagVectorTL[0] = (-1.0)*dAxisVector1[0] - dAxisVector1[1];  
					dDiagVectorTL[1] = (-1.0)*dAxisVector1[1] + dAxisVector1[0];
				}
				else
				{ 
					dDiagVectorTR[0] = dAxisVector1[0] +        dAxisVector1[1];   
					dDiagVectorTR[1] = dAxisVector1[1]        - dAxisVector1[0];
					dDiagVectorTL[0] = (-1.0)*dAxisVector1[0] + dAxisVector1[1];  
					dDiagVectorTL[1] = (-1.0)*dAxisVector1[1] - dAxisVector1[0];
				}
			}

			int halfText = (int)((double)nAxisWidth / 4.0 * (bCapital ? 1.0 : 0.5));

			lineUnit.P1.x = TextOrgP.x + long(halfText * dDiagVectorTR[0]);    
			lineUnit.P1.y = TextOrgP.y + long(halfText * dDiagVectorTR[1]);		
			lineUnit.P2.x = TextOrgP.x - long(halfText * dDiagVectorTR[0]);  
			lineUnit.P2.y = TextOrgP.y - long(halfText * dDiagVectorTR[1]);
			lineGroup.List.Add(lineUnit);
			lineUnit.P1.x = TextOrgP.x + long(halfText * dDiagVectorTL[0]);     
			lineUnit.P1.y = TextOrgP.y + long(halfText * dDiagVectorTL[1]);		
			lineUnit.P2.x = TextOrgP.x - long(halfText * dDiagVectorTL[0]); 
			lineUnit.P2.y = TextOrgP.y - long(halfText * dDiagVectorTL[1]);
			lineGroup.List.Add(lineUnit);
		}
		// Y
		if(nAxisMode == EN_DRAW_AXISMODE_PANTALINE_XY   || nAxisMode == EN_DRAW_AXISMODE_PANTALINE_YZ   ||
			nAxisMode == EN_DRAW_AXISMODE_PANTALINE_XY_L || nAxisMode == EN_DRAW_AXISMODE_PANTALINE_YZ_L)
		{
			double dDiagVectorBR_Min[2];

			if(nAxisMode == EN_DRAW_AXISMODE_PANTALINE_YZ || nAxisMode == EN_DRAW_AXISMODE_PANTALINE_YZ_L)
			{
				TextOrgP.x = OrgP.x + long(dAxisVector1[0] * (nAxisSize - nAxisWidth / 2 - nAxisWidth / 4));
				TextOrgP.y = OrgP.y + long(dAxisVector1[1] * (nAxisSize - nAxisWidth / 2 - nAxisWidth / 4));	

				if(bLocalAxis)
				{
					if(pDrawCtrl->m_DrawFunc.IsEMF())
					{ 
						dDiagVectorTR[0] = dAxisVector1[0] - dAxisVector1[1];    
						dDiagVectorTR[1] = dAxisVector1[1] + dAxisVector1[0];
						dDiagVectorTL[0] = (-1.0) * dAxisVector1[0] - dAxisVector1[1];   
						dDiagVectorTL[1] = (-1.0) * dAxisVector1[1] + dAxisVector1[0];
						dDownVector[0] = dAxisVector1[1];                            
						dDownVector[1] = (-1.0) * dAxisVector1[0];
						dDiagVectorBR_Min[0] = (-1.0) * dAxisVector1[0] + (3.0) * dAxisVector1[1];   
						dDiagVectorBR_Min[1] = (-1.0) * dAxisVector1[1] - (3.0) * dAxisVector1[0]; 
					}
					else
					{ 
						dDiagVectorTR[0] = dAxisVector1[0] + dAxisVector1[1];  
						dDiagVectorTR[1] = dAxisVector1[1] - dAxisVector1[0];
						dDiagVectorTL[0] = (-1.0) * dAxisVector1[0] + dAxisVector1[1];  
						dDiagVectorTL[1] = (-1.0) * dAxisVector1[1] - dAxisVector1[0];
						dDownVector[0] = (-1.0) * dAxisVector1[1];                  
						dDownVector[1] = dAxisVector1[0];
						dDiagVectorBR_Min[0] = (-1.0) * dAxisVector1[0] - (3.0) * dAxisVector1[1]; 
						dDiagVectorBR_Min[1] = (-1.0) * dAxisVector1[1] + (3.0) * dAxisVector1[0]; 
					}
				}
			}
			else
			{
				TextOrgP.x = OrgP.x + long(dAxisVector2[0] * (nAxisSize - nAxisWidth / 2 - nAxisWidth / 4));
				TextOrgP.y = OrgP.y + long(dAxisVector2[1] * (nAxisSize - nAxisWidth / 2 - nAxisWidth / 4));	

				if(bLocalAxis)
				{
					if(pDrawCtrl->m_DrawFunc.IsEMF())
					{ 
						dDiagVectorTR[0] = dAxisVector2[0] + dAxisVector2[1]; 
						dDiagVectorTR[1] = dAxisVector2[1] - dAxisVector2[0]; 
						dDiagVectorTL[0] = dAxisVector2[0] - dAxisVector2[1]; 
						dDiagVectorTL[1] = dAxisVector2[1] + dAxisVector2[0];
						dDiagVectorBR_Min[0] = (-3.0)*dAxisVector2[0] - dAxisVector2[1];  
						dDiagVectorBR_Min[1] = (-3.0)*dAxisVector2[1] + dAxisVector2[0]; 
					}
					else
					{ 
						dDiagVectorTR[0] = dAxisVector2[0] - dAxisVector2[1];  
						dDiagVectorTR[1] = dAxisVector2[1] + dAxisVector2[0]; 
						dDiagVectorTL[0] = dAxisVector2[0] + dAxisVector2[1]; 
						dDiagVectorTL[1] = dAxisVector2[1] - dAxisVector2[0];
						dDiagVectorBR_Min[0] = (-3.0)*dAxisVector2[0] + dAxisVector2[1]; 
						dDiagVectorBR_Min[1] = (-3.0)*dAxisVector2[1] - dAxisVector2[0]; 
					}
					dDownVector[0] = (-1.0) * dAxisVector2[0];       
					dDownVector[1] = (-1.0) * dAxisVector2[1];
				}
			}

			int halfText = (int)((double)nAxisWidth / 4.0 * (bCapital ? 1.0 : 0.5));
			if(bCapital)
			{
				lineUnit.P1.x = TextOrgP.x + long(halfText * dDiagVectorTL[0]);    
				lineUnit.P1.y = TextOrgP.y + long(halfText * dDiagVectorTL[1]);			
				lineUnit.P2.x = TextOrgP.x;                                       
				lineUnit.P2.y = TextOrgP.y;
				lineGroup.List.Add(lineUnit);
				lineUnit.P1.x = TextOrgP.x + long(halfText * dDiagVectorTR[0]);   
				lineUnit.P1.y = TextOrgP.y + long(halfText * dDiagVectorTR[1]);
				lineUnit.P2.x = TextOrgP.x;                                      
				lineUnit.P2.y = TextOrgP.y;
				lineGroup.List.Add(lineUnit);
				lineUnit.P1.x = TextOrgP.x;                                       
				lineUnit.P1.y = TextOrgP.y;
				lineUnit.P2.x = TextOrgP.x + long(halfText * dDownVector[0]);  
				lineUnit.P2.y = TextOrgP.y + long(halfText * dDownVector[1]);
				lineGroup.List.Add(lineUnit);
			}
			else
			{
				lineUnit.P1.x = TextOrgP.x + long(halfText * dDiagVectorTL[0]);   
				lineUnit.P1.y = TextOrgP.y + long(halfText * dDiagVectorTL[1]);			
				lineUnit.P2.x = TextOrgP.x + long(halfText * dDownVector[0]);   
				lineUnit.P2.y = TextOrgP.y + long(halfText * dDownVector[1]);
				lineGroup.List.Add(lineUnit);
				lineUnit.P1.x = TextOrgP.x + long(halfText * dDiagVectorTR[0]);     
				lineUnit.P1.y = TextOrgP.y + long(halfText * dDiagVectorTR[1]);
				lineUnit.P2.x = TextOrgP.x + long(halfText * dDiagVectorBR_Min[0]); 
				lineUnit.P2.y = TextOrgP.y + long(halfText * dDiagVectorBR_Min[1]);
				lineGroup.List.Add(lineUnit);
			}
		}
		//Z
		if(nAxisMode == EN_DRAW_AXISMODE_PANTALINE_YZ   || nAxisMode == EN_DRAW_AXISMODE_PANTALINE_XZ ||
		   nAxisMode == EN_DRAW_AXISMODE_PANTALINE_YZ_L || nAxisMode == EN_DRAW_AXISMODE_PANTALINE_XZ_L)
		{
			TextOrgP.x = OrgP.x + long(dAxisVector2[0] * (nAxisSize - nAxisWidth / 2 - nAxisWidth / 4));
			TextOrgP.y = OrgP.y + long(dAxisVector2[1] * (nAxisSize - nAxisWidth / 2 - nAxisWidth / 4));	

			if(bLocalAxis)
			{
				if(pDrawCtrl->m_DrawFunc.IsEMF())
				{ 
					dDiagVectorTR[0] = dAxisVector2[0] + dAxisVector2[1]; 
					dDiagVectorTR[1] = dAxisVector2[1] - dAxisVector2[0]; 
					dDiagVectorTL[0] = dAxisVector2[0] - dAxisVector2[1]; 
					dDiagVectorTL[1] = dAxisVector2[1] + dAxisVector2[0];
				}
				else
				{ 
					dDiagVectorTR[0] = dAxisVector2[0] - dAxisVector2[1]; 
					dDiagVectorTR[1] = dAxisVector2[1] + dAxisVector2[0]; 
					dDiagVectorTL[0] = dAxisVector2[0] + dAxisVector2[1]; 
					dDiagVectorTL[1] = dAxisVector2[1] - dAxisVector2[0];
				}
			}

			int halfText = (int)((double)nAxisWidth/4.0 * (bCapital ? 1.0 : 0.5));
			lineUnit.P1.x = TextOrgP.x + long(halfText * dDiagVectorTL[0]);   
			lineUnit.P1.y = TextOrgP.y + long(halfText * dDiagVectorTL[1]);		
			lineUnit.P2.x = TextOrgP.x + long(halfText * dDiagVectorTR[0]);   
			lineUnit.P2.y = TextOrgP.y + long(halfText * dDiagVectorTR[1]);
			lineGroup.List.Add(lineUnit);		     
			lineUnit.P1.x = TextOrgP.x + long(halfText * dDiagVectorTR[0]);   
			lineUnit.P1.y = TextOrgP.y + long(halfText * dDiagVectorTR[1]);		
			lineUnit.P2.x = TextOrgP.x - long(halfText * dDiagVectorTR[0]);  
			lineUnit.P2.y = TextOrgP.y - long(halfText * dDiagVectorTR[1]);
			lineGroup.List.Add(lineUnit);		     
			lineUnit.P1.x = TextOrgP.x - long(halfText * dDiagVectorTR[0]); 
			lineUnit.P1.y = TextOrgP.y - long(halfText * dDiagVectorTR[1]);		
			lineUnit.P2.x = TextOrgP.x - long(halfText * dDiagVectorTL[0]); 
			lineUnit.P2.y = TextOrgP.y - long(halfText * dDiagVectorTL[1]);
			lineGroup.List.Add(lineUnit);		    
		}

		pDrawCtrl->DrawUnit(AxisDrawFormat, lineGroup);

		T_POLYGON_GROUP polygonGroup;
		T_POLYLINE      polygonUnit;
		CPoint          pointUnit;		
		polygonGroup.List.SetSize(2);
		// 1개축이 5각형으로 된 X,Y축 Polyline
		double dDirVector[2];
		double dCrossVector[2];
		dDirVector[0]   = dAxisVector1[0]; 
		dDirVector[1]   = dAxisVector1[1];
		dCrossVector[0] = dAxisVector1[1]; 
		dCrossVector[1] = dAxisVector1[0] * (-1.0);

		polygonUnit.arPoint.SetSize(7);
		pointUnit.x = OrgP.x - long((nAxisWidth / 2) * dDirVector[0]) - long((nAxisWidth / 2) * dCrossVector[0]);     
		pointUnit.y = OrgP.y - long((nAxisWidth / 2) * dDirVector[1]) - long((nAxisWidth / 2) * dCrossVector[1]);     
		polygonUnit.arPoint[0] = pointUnit;
		pointUnit.x = OrgP.x + long((nAxisWidth / 2) * dDirVector[0]) - long((nAxisWidth / 2) * dCrossVector[0]);     
		pointUnit.y = OrgP.y + long((nAxisWidth / 2) * dDirVector[1]) - long((nAxisWidth / 2) * dCrossVector[1]);     
		polygonUnit.arPoint[1] = pointUnit;
		pointUnit.x = OrgP.x + long((nAxisSize - nAxisWidth / 2) * dDirVector[0]) - long((nAxisWidth / 2) * dCrossVector[0]);   
		pointUnit.y = OrgP.y + long((nAxisSize - nAxisWidth / 2) * dDirVector[1]) - long((nAxisWidth / 2) * dCrossVector[1]);
		polygonUnit.arPoint[2] = pointUnit;
		pointUnit.x = OrgP.x + long(nAxisSize * dDirVector[0]);                    
		pointUnit.y = OrgP.y + long(nAxisSize * dDirVector[1]);
		polygonUnit.arPoint[3] = pointUnit;
		pointUnit.x = OrgP.x + long((nAxisSize - nAxisWidth / 2) * dDirVector[0]) + long((nAxisWidth / 2) * dCrossVector[0]);   
		pointUnit.y = OrgP.y + long((nAxisSize - nAxisWidth / 2) * dDirVector[1]) + long((nAxisWidth / 2) * dCrossVector[1]);
		polygonUnit.arPoint[4] = pointUnit;
		pointUnit.x = OrgP.x + long((nAxisWidth / 2) * dDirVector[0]) + long((nAxisWidth / 2) * dCrossVector[0]);               
		pointUnit.y = OrgP.y + long((nAxisWidth / 2) * dDirVector[1]) + long((nAxisWidth / 2) * dCrossVector[1]);
		polygonUnit.arPoint[5] = pointUnit;
		pointUnit.x = OrgP.x - long((nAxisWidth / 2) * dDirVector[0]) + long((nAxisWidth / 2) * dCrossVector[0]);               
		pointUnit.y = OrgP.y - long((nAxisWidth / 2) * dDirVector[1]) + long((nAxisWidth / 2) * dCrossVector[1]);
		polygonUnit.arPoint[6] = pointUnit;
		polygonGroup.List[0] = polygonUnit;

		dDirVector[0]   = dAxisVector2[0];  
		dDirVector[1]   = dAxisVector2[1];
		dCrossVector[0] = dAxisVector2[1]; 
		dCrossVector[1] = dAxisVector2[0] * (-1.0);

		pointUnit.x = OrgP.x - long((nAxisWidth / 2) * dDirVector[0]) - long((nAxisWidth / 2) * dCrossVector[0]);     
		pointUnit.y = OrgP.y - long((nAxisWidth / 2) * dDirVector[1]) - long((nAxisWidth / 2) * dCrossVector[1]);     
		polygonUnit.arPoint[0] = pointUnit;
		pointUnit.x = OrgP.x + long((nAxisWidth / 2) * dDirVector[0]) - long((nAxisWidth / 2) * dCrossVector[0]);     
		pointUnit.y = OrgP.y + long((nAxisWidth / 2) * dDirVector[1]) - long((nAxisWidth / 2) * dCrossVector[1]);     
		polygonUnit.arPoint[1] = pointUnit;
		pointUnit.x = OrgP.x + long((nAxisSize - nAxisWidth / 2) * dDirVector[0]) - long((nAxisWidth / 2) * dCrossVector[0]);   
		pointUnit.y = OrgP.y + long((nAxisSize - nAxisWidth / 2) * dDirVector[1]) - long((nAxisWidth / 2) * dCrossVector[1]);
		polygonUnit.arPoint[2] = pointUnit;
		pointUnit.x = OrgP.x + long(nAxisSize * dDirVector[0]);                    
		pointUnit.y = OrgP.y + long(nAxisSize * dDirVector[1]);
		polygonUnit.arPoint[3] = pointUnit;
		pointUnit.x = OrgP.x + long((nAxisSize - nAxisWidth / 2) * dDirVector[0]) + long((nAxisWidth / 2) * dCrossVector[0]);   
		pointUnit.y = OrgP.y + long((nAxisSize - nAxisWidth / 2) * dDirVector[1]) + long((nAxisWidth / 2) * dCrossVector[1]);
		polygonUnit.arPoint[4] = pointUnit;
		pointUnit.x = OrgP.x + long((nAxisWidth / 2) * dDirVector[0]) + long((nAxisWidth / 2) * dCrossVector[0]);               
		pointUnit.y = OrgP.y + long((nAxisWidth / 2) * dDirVector[1]) + long((nAxisWidth / 2) * dCrossVector[1]);
		polygonUnit.arPoint[5] = pointUnit;
		pointUnit.x = OrgP.x - long((nAxisWidth / 2) * dDirVector[0]) + long((nAxisWidth / 2) * dCrossVector[0]);               
		pointUnit.y = OrgP.y - long((nAxisWidth / 2) * dDirVector[1]) + long((nAxisWidth / 2) * dCrossVector[1]);
		polygonUnit.arPoint[6] = pointUnit;
		polygonGroup.List[1] = polygonUnit;

		pDrawCtrl->DrawUnit(AxisDrawFormat, polygonGroup);
	}
}

void CDrawEditBtn_TndnTemplate::MakeDrawOrigin()
{
	m_uiOrigin = 0;
	if(m_bUseOrigin)
	{
		T_POINT_R_GROUP tPointGroup;
		tPointGroup.List.SetSize(1);
		if(IsCompositeSection(m_SectD.nType) && m_InitOrigin)
		{
			tPointGroup.List[0].x = - m_dOrigin_Org[0];
			tPointGroup.List[0].y = - m_dOrigin_Org[1];
		}
		else
		{
			tPointGroup.List[0].x = m_dBoundBox_Left;
			tPointGroup.List[0].y = m_dBoundBox_Bottom;
		}
		m_uiOrigin = m_DrawCtrl.AddDrawUnit(m_uiDrawFormatKeyOrigin, tPointGroup, FALSE, LINKKEY_ORIGIN, 0, TRUE, 0.0, 0.0, FALSE);		
		double dPos[2];
		dPos[0] = tPointGroup.List[0].x - m_dBoundBox_Left; //좌하단이 0,0 되도록.
		dPos[1] = tPointGroup.List[0].y - m_dBoundBox_Bottom;

		dPos[0] = m_pUnitCtrl->ConvertUnitDataOut(D_UNITSYS_BASE_LENGTH, dPos[0]);
		dPos[1] = m_pUnitCtrl->ConvertUnitDataOut(D_UNITSYS_BASE_LENGTH, dPos[1]);

		SetParentOriginPos_AutoDlg(dPos);
	}
}

void CDrawEditBtn_TndnTemplate::StartCurrUnitSelect( CPoint CurrP )
{
	CDrawEditBtn::StartCurrUnitSelect( CurrP );
}

void CDrawEditBtn_TndnTemplate::InProgressCurrUnitSelect( CPoint CurrP )
{
	if(m_bMoveOrigin)
	{
		CurrRemoveDrawData();
		SelectRemoveDrawData();

		int nSnapX = -1;
		int nSnapY = -1;
		double dCoord[2];
		GetPosByCPoint(CurrP, dCoord[0], dCoord[1], nSnapX, nSnapY, TRUE);
		GetDisplayCoord(dCoord[0], dCoord[1], CurrP);

		//     MakeCurrUnitSelectMove(m_StartPoint, CurrP);    
		//     CurrConvertDrawData(FALSE);

		MakeCurrLinePreview(dCoord[0], dCoord[1], nSnapX, nSnapY);

		RedrawWindow();
	}
	else
	{
		CDrawEditBtn::InProgressCurrUnitSelect( CurrP );  
	}
}

void CDrawEditBtn_TndnTemplate::CompleteCurrUnitSelect( CPoint CurrP )
{
	BOOL bShift = GetAsyncKeyState(VK_LSHIFT) ? TRUE: FALSE;

	nrUINT nrUintPre;
	int nSizePre = 0;
	
	if(bShift) nSizePre = GetSelectedLinkKeyList(nrUintPre);

	CDrawEditBtn::CompleteCurrUnitSelect( CurrP );

	if(m_bHasList)
	{
		vecTndnKey keyList;
		nrUINT nrUint, nrUintTarget;
		int nSize = GetSelectedLinkKeyList(nrUint);
		int i, j;

		for(i = 0 ; i < nSize; i++)
		{
			nrUintTarget.Add(nrUint[i]);
		}

		if(bShift && nSizePre > 0)
		{
			BOOL bExist;
			for(i = 0 ; i < nSizePre; i++)
			{
				bExist = FALSE;

				for(j = 0 ; j < nSize; j++)
				{
					if(nrUintPre[i] == nrUint[j]) bExist = TRUE;
				}

				if(!bExist) nrUintTarget.Add(nrUintPre[i]);
			}
		}
		nSize = nrUintTarget.GetSize();
		keyList.resize(nSize);
		int idxOrigin = -1;
		int nIdx = 0;
		for(i = 0 ; i < nSize; i++)
		{
			if(nrUintTarget[i] == LINKKEY_ORIGIN) idxOrigin = i;
			else if(nrUintTarget[i] > 0)          keyList[nIdx++] = nrUintTarget[i] - 1;
			else                                  ASSERT(0);
		}

		keyList.resize(nIdx);
		if(idxOrigin >= 0) nrUintTarget.RemoveAt(idxOrigin);

		SetParentSelectedKey(nIdx, keyList, nrUintTarget, idxOrigin);

	}
}

void CDrawEditBtn_TndnTemplate::OnLButtonUp( UINT nFlags, CPoint point )
{
	if(m_bMoveOrigin)
	{
		m_bUseMove = FALSE;
		m_bMoveOrigin = FALSE;  

		nrUINT arKey;
		m_DrawCtrl.GetSelectedKeyList(arKey);
		UINT nKey; 
		BOOL bMovePoint = FALSE;
		if(arKey.GetSize() == 1)
		{
			CDrawPoint* pDrawPt = (CDrawPoint*)m_DrawCtrl.GetDrawClass(arKey[0]);
			if(pDrawPt->m_LinkKey == LINKKEY_ORIGIN)
			{
				bMovePoint = TRUE;
				nKey = arKey[0];
			}
		}

		//이전의 상태를 저장해놓는다.
		int nModePre = m_nMode;
		m_nMode = EN_DRAW_WINMODE_NONE;
		CDrawEditBtn::OnLButtonUp(nFlags, point);
		m_nMode = nModePre;

		//움직인 점을 이동한다.
		if(bMovePoint)
		{
			int nSnapX, nSnapY;
			double dPos[2];
			GetPosByCPoint(point, dPos[0], dPos[1], nSnapX, nSnapY, TRUE);
			dPos[0] = m_pUnitCtrl->ConvertUnitDataOut(D_UNITSYS_BASE_LENGTH, dPos[0]);
			dPos[1] = m_pUnitCtrl->ConvertUnitDataOut(D_UNITSYS_BASE_LENGTH, dPos[1]);
			SetDrawOrigin(dPos, FALSE, TRUE);
		}    
	}
	else
	{
		CDrawEditBtn::OnLButtonUp(nFlags, point);
	}
}

void CDrawEditBtn_TndnTemplate::OnLButtonDown( UINT nFlags, CPoint point )
{
	CDrawEditBtn::OnLButtonDown(nFlags, point);
	UINT nKey;
	if(m_DrawCtrl.GetSelectDrawUnitKey(point, nKey))
	{
		CDrawPoint* pDrawPt = (CDrawPoint*)m_DrawCtrl.GetDrawClass(nKey);
		if(pDrawPt->m_LinkKey == LINKKEY_ORIGIN)
		{
			m_bUseMove = TRUE;
			m_bMoveOrigin = TRUE;
		}
	}
}

void CDrawEditBtn_TndnTemplate::GetPosByCPoint( CPoint& cPt, double& dPtX, double& dPtY, int& nSnapX, int& nSnapY, BOOL bSnap )
{
	GetGenuineCoord(cPt, dPtX, dPtY);

	if(bSnap && !(GetAsyncKeyState(VK_LSHIFT)))
	{
		nSnapX = -1;
		nSnapY = -1;
		double dGap = (m_dTotalHeight + m_dTotalWidth) * 0.015;
		double dSnapLenX, dSnapLenY;
		double dTempX, dTempY;
		int i, nSize;
		nSize = m_aSnapVertex.GetSize();

		//가장 가까운 X스냅 좌표를 찾는다.
		for(i = 0 ; i < nSize; i++)
		{
			dTempX = fabs(dPtX - m_aSnapVertex[i].dX);
			dTempY = fabs(dPtY - m_aSnapVertex[i].dY);

			if(dTempX < dGap)
			{
				if(nSnapX < 0 || dSnapLenX > dTempX)
				{
					dSnapLenX = dTempX;
					dSnapLenY = dTempY;
					nSnapX = i;
				}
				else if(fabs(dSnapLenX - dTempX) <= 1E-5 && dSnapLenY > dTempY)
				{
					dSnapLenX = dTempX;
					dSnapLenY = dTempY;
					nSnapX = i;
				}
			}
		}

		//가장 가까운 Y스냅 좌표를 찾는다.
		for(i = 0 ; i < nSize; i++)
		{
			dTempX = fabs(dPtX - m_aSnapVertex[i].dX);
			dTempY = fabs(dPtY - m_aSnapVertex[i].dY);

			if(dTempY < dGap)
			{
				if(nSnapY < 0 || dSnapLenY > dTempY)
				{
					dSnapLenX = dTempX;
					dSnapLenY = dTempY;
					nSnapY = i;
				}
				else if(fabs(dSnapLenY - dTempY) <= 1E-5 && dSnapLenX > dTempX)
				{
					dSnapLenX = dTempX;
					dSnapLenY = dTempY;
					nSnapY = i;
				}
			}
		}

		if(nSnapX >= 0) dPtX = m_aSnapVertex[nSnapX].dX;
		if(nSnapY >= 0) dPtY = m_aSnapVertex[nSnapY].dY;
	}
}

BOOL CDrawEditBtn_TndnTemplate::GetDrawOrigin( double* dOrigin )
{
	CDrawPoint* pDrawPt = (CDrawPoint*)m_DrawCtrl.GetDrawClass(m_uiOrigin);
	if(pDrawPt)
	{
		if(pDrawPt->m_LinkKey == LINKKEY_ORIGIN)
		{
			if(pDrawPt->m_DataR.List.GetSize() != 1)
				return FALSE;

			dOrigin[0] = pDrawPt->m_DataR.List[0].x - m_dBoundBox_Left;
			dOrigin[1] = pDrawPt->m_DataR.List[0].y - m_dBoundBox_Bottom;
			dOrigin[0] = m_pUnitCtrl->ConvertUnitDataOut(D_UNITSYS_BASE_LENGTH, dOrigin[0]);
			dOrigin[1] = m_pUnitCtrl->ConvertUnitDataOut(D_UNITSYS_BASE_LENGTH, dOrigin[1]);
			return TRUE;
		}
	}
	return FALSE;
}

void CDrawEditBtn_TndnTemplate::SetDrawOrigin( double* dOrigin, BOOL bConsiderBound, BOOL bSetParent )
{
	dOrigin[0] = m_pUnitCtrl->ConvertUnitDataIn(D_UNITSYS_BASE_LENGTH, dOrigin[0]);
	dOrigin[1] = m_pUnitCtrl->ConvertUnitDataIn(D_UNITSYS_BASE_LENGTH, dOrigin[1]);
	if(m_bUseOrigin && m_uiOrigin > 0)
	{
		T_POINT_R_GROUP tPointGroup;
		tPointGroup.List.SetSize(1);
		tPointGroup.List[0].x = dOrigin[0];
		tPointGroup.List[0].y = dOrigin[1];
		if(bConsiderBound)
		{
			tPointGroup.List[0].x += m_dBoundBox_Left;
			tPointGroup.List[0].y += m_dBoundBox_Bottom;
		}
		m_DrawCtrl.ModifyDrawUnit(m_uiOrigin, tPointGroup);

		CurrRemoveDrawData();

		m_DrawCtrl.ConvertDrawData(TRUE);	
		CurrConvertDrawData(TRUE);
		OverlapChecking();
		RedrawWindow();

		if(bSetParent)
		{
			SetParentOriginPos(tPointGroup);
		}
	}
}

void CDrawEditBtn_TndnTemplate::ResetSnapPos()
{
	m_aSnapVertex.RemoveAll();

	DGN_GSEC_POLYGON_LIST* pPolyList;

	int i, j;
	int nPolySize;
	for(i = 0 ; i < 4; i++)
	{
		if(i == 0)      pPolyList = &m_arConcOutPoly;
		else if(i == 1) pPolyList = &m_arConcInPoly;
		else if(i == 2) pPolyList = &m_arSteelOutPoly;
		else if(i == 3) pPolyList = &m_arSteelInPoly;
		else            break;

		nPolySize = pPolyList->GetSize();
		for(j = 0 ; j < nPolySize; j++)
		{
			m_aSnapVertex.InsertAt(m_aSnapVertex.GetSize(), &(pPolyList->GetAt(j).aVertex));
		}
	}
	int nSize = m_aSnapVertex.GetSize();
	for(i = 0 ; i < nSize; i++)
	{
		m_aSnapVertex[i].dX = m_pUnitCtrl->ConvertUnitDataIn(D_UNITSYS_BASE_LENGTH, m_aSnapVertex[i].dX);
		m_aSnapVertex[i].dY = m_pUnitCtrl->ConvertUnitDataIn(D_UNITSYS_BASE_LENGTH, m_aSnapVertex[i].dY);
	}
}

void CDrawEditBtn_TndnTemplate::MakeCurrLinePreview( double dPtX, double dPtY, int nSnapX, int nSnapY )
{
	T_POINT tPt;
	tPt.x = dPtX;
	tPt.y = dPtY;
	m_CurrDrawClass_Point.m_DataR.List.Add(tPt);
	if(nSnapX >= 0)
	{
		T_LINE_R tLine;
		tLine.P1.x = dPtX;
		tLine.P1.y = dPtY;
		tLine.P2.x = m_aSnapVertex[nSnapX].dX;
		tLine.P2.y = m_aSnapVertex[nSnapX].dY;
		m_CurrDrawClass_Line.m_DataR.List.Add(tLine);
	}
	if(nSnapY >= 0)
	{
		T_LINE_R tLine;
		tLine.P1.x = dPtX;
		tLine.P1.y = dPtY;
		tLine.P2.x = m_aSnapVertex[nSnapY].dX;
		tLine.P2.y = m_aSnapVertex[nSnapY].dY;
		m_CurrDrawClass_Line.m_DataR.List.Add(tLine);
	}

	m_CurrDrawClass_Point.Convert(TRUE, &(m_DrawCtrl.m_DrawFunc), &(m_DrawCtrl.m_PaperFormat), &(m_CurrDrawFormat));
	m_CurrDrawClass_Line.Convert(TRUE, &(m_DrawCtrl.m_DrawFunc), &(m_DrawCtrl.m_PaperFormat), &(m_CurrDrawFormat));
}

BOOL CDrawEditBtn_TndnTemplate::SetOriginPt_Init( T_SECT_D& rSectD )
{
	m_dOrigin_Org[0] = 0.0;
	m_dOrigin_Org[1] = 0.0;
	m_InitOrigin = FALSE;

	//Composite가 아닌경우는 좌하단(0.0, 0.0)으로 한다.
	if(!IsCompositeSection(rSectD.nStype))
	{
		return TRUE;
	}

	//Composite일 경우는 Origin 포인트를 Before와 After를 비교해서 구한다.
	DGN_GSEC_SHAPE_D         SectShapeD;
	DGN_GSEC_POLYGON_LIST		 arConcOutPoly;
	DGN_GSEC_POLYGON_LIST		 arConcInPoly;
	DGN_GSEC_POLYGON_LIST		 arSteelOutPoly;
	DGN_GSEC_POLYGON_LIST		 arSteelInPoly;
	DGN_GSEC_LINE_LIST			 arSteelLine;
	DGN_GSEC_VERTEXLINE_LIST arSteelVertexLine;
	DGN_GSEC_POLYGON_LIST		 arSteelLinePoly;
	double                   dOutPos[6];

	CDb_DllManager_Base DllMgr;
	if(!DllMgr.ConvertToDLL_SectData_Pos(rSectD, 0.0, SectShapeD, TRUE, FALSE)) return FALSE; //Before의 정보.

	if(!dgn::link::ITmpDgnDrawLib::GetInstance()->Get_SectionPolygonData(SectShapeD, arConcOutPoly, arConcInPoly,
		arSteelOutPoly, arSteelInPoly, arSteelLine, arSteelVertexLine, arSteelLinePoly,
		dOutPos, -SectShapeD.dYBar, -SectShapeD.dZBar, TRUE))
	{
		return FALSE;
	}

	double dOffsetY = 0.0;
	double dOffsetZ = 0.0;
	BOOL   bFind = FALSE;

	int i, j, k;
	int nSizeCmp;
	int nSizeOrg;

	DGN_GSEC_POLYGON_LIST* pListOrg;
	DGN_GSEC_POLYGON_LIST* pListCmp;
	for(k = 0 ; k < 4; k++)
	{
		if(k == 0)      { pListOrg = &m_arConcOutPoly;  pListCmp = &arConcOutPoly;  }
		else if(k == 1) { pListOrg = &m_arConcInPoly;   pListCmp = &arConcInPoly;   }
		else if(k == 2) { pListOrg = &m_arSteelOutPoly; pListCmp = &arSteelOutPoly; }
		else if(k == 3) { pListOrg = &m_arSteelInPoly;  pListCmp = &arSteelInPoly;  }
		else              continue;

		nSizeCmp = pListCmp->GetSize();
		nSizeOrg = pListOrg->GetSize();

		for(i = 0 ; i < nSizeCmp; i++)
		{
			for(j = 0 ; j < nSizeOrg; j++)
			{
				bFind = IsSamePolygon(*pListOrg, *pListCmp, dOffsetY, dOffsetZ);
				if(bFind) break;
			}
			if(bFind) break;
		}
		if(bFind) break;
	}

	if(bFind)
	{
		double dLeft;
		double dBottom;
		BOOL   bInit = TRUE;

		DGN_GSEC_POLYGON_LIST* pPolyList;    

		int nSizePoly;
		int nSizeVertex;
		for(k = 0 ; k < 4; k++)
		{ //단면의 좌하단을 찾는다.
			if(k == 0)      pPolyList = &arConcOutPoly;
			else if(k == 1) pPolyList = &arConcInPoly;
			else if(k == 2) pPolyList = &arSteelOutPoly;
			else if(k == 3) pPolyList = &arSteelInPoly;

			nSizePoly = (*pPolyList).GetSize();
			for(i = 0 ; i < nSizePoly; i++)
			{
				nSizeVertex = pPolyList->GetAt(i).aVertex.GetSize();
				for(j = 0 ; j < nSizeVertex; j++)
				{
					if(bInit)
					{
						dLeft   = pPolyList->GetAt(i).aVertex[j].dX;
						dBottom = pPolyList->GetAt(i).aVertex[j].dY;
						bInit = FALSE;
						continue;
					}
					if(dLeft   > pPolyList->GetAt(i).aVertex[j].dX) dLeft   = pPolyList->GetAt(i).aVertex[j].dX;
					if(dBottom > pPolyList->GetAt(i).aVertex[j].dY) dBottom = pPolyList->GetAt(i).aVertex[j].dY;
				}
			}
		}

		//단면의 좌하단을 오프셋만큼 움직인다.
		m_dOrigin_Org[0] = - dLeft - dOffsetY;
		m_dOrigin_Org[1] = - dBottom - dOffsetZ;
		m_dOrigin_Org[0] = m_pUnitCtrl->ConvertUnitDataIn(D_UNITSYS_BASE_LENGTH, m_dOrigin_Org[0]);
		m_dOrigin_Org[1] = m_pUnitCtrl->ConvertUnitDataIn(D_UNITSYS_BASE_LENGTH, m_dOrigin_Org[1]);
		m_InitOrigin = TRUE;
	}

	return TRUE;
}

// Diff는 Org-Cmp
BOOL CDrawEditBtn_TndnTemplate::IsSamePolygon( const DGN_GSEC_POLYGON_LIST& arOrg, const DGN_GSEC_POLYGON_LIST& arCmp, double& dDiffY, double& dDiffZ )
{
	dDiffY = 0.0;  
	dDiffZ = 0.0;

	int i, j;
	int nSizeOrg = arOrg.GetSize();
	int nSizeCmp = arCmp.GetSize();
	if(nSizeCmp != nSizeCmp)
		return FALSE;

	double dDiffYTemp;
	double dDiffZTemp;
	BOOL bDiffInit = TRUE;
	int nSizeVertexOrg;
	int nSizeVertexCmp;
	for(i = 0 ; i < nSizeOrg; i++)
	{
		nSizeVertexOrg = arOrg[i].aVertex.GetSize();
		nSizeVertexCmp = arCmp[i].aVertex.GetSize();
		if(nSizeVertexCmp != nSizeVertexOrg)
			return FALSE;

		for(j = 0; j < nSizeVertexOrg; j++)
		{
			if(bDiffInit)
			{
				dDiffY = arOrg[i].aVertex[j].dX - arCmp[i].aVertex[j].dX;
				dDiffZ = arOrg[i].aVertex[j].dY - arCmp[i].aVertex[j].dY;
			}
			else
			{
				dDiffYTemp = arOrg[i].aVertex[j].dX - arCmp[i].aVertex[j].dX;
				dDiffZTemp = arOrg[i].aVertex[j].dY - arCmp[i].aVertex[j].dY;

				if(fabs(dDiffY - dDiffYTemp) > 1E-5)
					return FALSE;
				if(fabs(dDiffZ - dDiffZTemp) > 1E-5)
					return FALSE;
			}
		}
	}
	return TRUE;
}

BOOL CDrawEditBtn_TndnTemplate::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	if(GetAsyncKeyState(VK_LSHIFT))
	{
		if(zDelta > 0)
		{
			m_dTendonZoomFactor += 0.01;
			if(m_dTendonZoomFactor > 0.05) m_dTendonZoomFactor = 0.05; //최대
		}
		else if(zDelta < 0)
		{
			m_dTendonZoomFactor -= 0.01;
			if(m_dTendonZoomFactor < 0.01) m_dTendonZoomFactor = 0.01; //최소
		}

		MakeDrawUnit(FALSE);
		return TRUE;
	}
	else
		return CDrawEditBtn::OnMouseWheel(nFlags, zDelta, pt);
}

double CDrawEditBtn_TndnTemplate::GetScaledElemLen(double dElemLen, T_ELEM_K ElemK)
{
	// 자식 Class에서 ElemLen를 다르게 하는 경우가 있어서 이렇게 처리 함
	return dElemLen;
}

void CDrawEditBtn_TndnTemplate::MakeDrawSubClass()
{
	// 자식 Class에서 그림
}

void CDrawEditBtn_TndnTemplate::SetParentOriginPos_AutoDlg(double dPos[2])
{
	CCMDTendonTemplateDlg* pDlg = (CCMDTendonTemplateDlg*)GetParent();
	pDlg->SetOriginPos_AutoDlg(dPos);
}

void CDrawEditBtn_TndnTemplate::SetParentSelectedKey( int nIdx, vecTndnKey& keyList, nrUINT& nrUintTarget, int idxOrigin )
{
	CCMDTendonTemplateDlg* pDlg = (CCMDTendonTemplateDlg*)GetParent();

	if(nIdx > 0)
	{
		pDlg->SetSelectedKeyList(keyList);
		SetSelectedLinkKeyList(nrUintTarget, TRUE);

	}
	else if(nIdx == 0 && idxOrigin < 0)
	{
		nrUintTarget.SetSize(0);
		SetSelectedKeyList(nrUintTarget, TRUE);
		pDlg->UnSelectAllKey();
	}
}

void CDrawEditBtn_TndnTemplate::SetParentOriginPos( T_POINT_R_GROUP &tPointGroup )
{
	CCMDTendonTemplateDlg* pDlg = (CCMDTendonTemplateDlg*)GetParent();
	double dPos[2];
	dPos[0] = tPointGroup.List[0].x - m_dBoundBox_Left; //좌하단이 0,0이되도록 조정.
	dPos[1] = tPointGroup.List[0].y - m_dBoundBox_Bottom;
	dPos[0] = m_pUnitCtrl->ConvertUnitDataOut(D_UNITSYS_BASE_LENGTH, dPos[0]);
	dPos[1] = m_pUnitCtrl->ConvertUnitDataOut(D_UNITSYS_BASE_LENGTH, dPos[1]);
	pDlg->SetOriginPos_AutoDlg(dPos);
}
