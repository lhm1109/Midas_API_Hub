// DgnSeisAnchorEvalEditBtn.cpp: implementation of the CDgnEditBtn_TndnTemplate class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "wg_dgn.h"

#include "DgnSeisAnchorEvalEditBtn.h"

#include "..\wg_db\DBDoc.h"
#include "..\wg_db\UnitCtrl.h"
#include "..\wg_db\SectUtil.h"
#include "..\wg_db\MathFunc.h"
#include "..\wg_db\Db_DllManager_Base.h"
#include "..\wg_db\InitCtrl.h"
#include "..\wg_db\MathLib.h"
#include "..\wg_db\AttrCtrl2.h"

#include <utility>
#include "..\wg_base\TestEnvMgr.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define LINKKEY_ORIGIN 9999

/////////////////////////////////////////////////////////////////////////////
// CDgnEditBtn_TndnTemplate dialog
CDgnSeisAnchorEvalEditBtn::CDgnSeisAnchorEvalEditBtn()
	: CDrawEditBtn()
{
	// 	Reset_Variables(FALSE);
	SetDrawFormat();

	m_DrawCtrl.m_SelectDrawType = 4;
	m_DrawCtrl.m_SelectDrawFormat.Width = 3;
	m_DrawCtrl.m_SelectDrawFormat.lineColor = RGB(250, 115, 75);
	m_DrawCtrl.m_SelectDrawFormat.pointType = EN_POINT_TYPE_CIRCLE;

	// 	m_nOverlapType = EN_DGN_OVERLAP_OBJECT;
	m_bUseMove = FALSE; //일단 이게 있어야 선택 사각형을 그린다.....
	m_bMoveOrigin = FALSE;
	m_DrawCtrl.SetSelectDis(2);

	//m_BkGroudColor = RGB(227, 227, 227);
	m_BkGroudColor = RGB(255, 255, 255);
	m_ColorCopying = RGB(219, 219, 219);
	m_ColorBearing = RGB(254, 159, 107);
	m_ColorBearingLine = RGB(209, 109, 55);
	m_ColorAnchor = RGB(180, 208, 221);
	m_ColorDimensionLine = RGB(79, 79, 79);
	m_ColorConcFailProjLine = RGB(245, 245, 248);
	m_ColorPryoutProjLine = RGB(79, 79, 79);
	m_ColorPryoutProjLineRep = RGB(255, 0, 0);
	m_ColorRefLine = RGB(0, 0, 255);
	m_ColorRefLine2 = RGB(255, 0, 0);
	m_ColorConcFailFill = RGB(248, 68, 16); // ConcFail 영역 강조선과 아랫쪽 사각형 체워지는 색
	m_ColorConcFailLine2 = RGB(251, 158, 4); // ConcFail 영역 강조선 중 2번째 이후의 색
	m_ColorPryoutXProjLineIn = RGB(255, 215, 193);
	m_ColorPryoutXProjLineOut = RGB(245, 245, 248);

	m_nLastFormatID = -1;

	m_dFitMargin = 0.06;
	m_dDimGap = 0.0;
	m_nDrawType = 0;
	m_bDrawOK = TRUE;

	m_bUser = FALSE;
	m_Data.Initialize();

	m_bTrans = FALSE;
	m_nDir = CDgnSeisAnchorShapeMgr::DIR_LONGI;

	m_dHa = 0.0;

	m_dCopingCenterX = 0.0;
	m_dCopingCenterY = 0.0;

	m_nDownUp = 1;
	m_nLeftRight = 1;
	m_nDownUpPry = 1;
	m_nLeftRightPry = 1;

	m_dDimOverlap = 0.0;

	m_bDrawDir = FALSE;
}

CDgnSeisAnchorEvalEditBtn::~CDgnSeisAnchorEvalEditBtn()
{
}

void CDgnSeisAnchorEvalEditBtn::DoDataExchange(CDataExchange* pDX)
{
	CDrawEditBtn::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDgnSeisAnchorEvalEditBtn, CDrawEditBtn)
END_MESSAGE_MAP()

void CDgnSeisAnchorEvalEditBtn::MakeDrawUnit(int nRatType[4], BOOL bFit /*= TRUE*/)
{
// 	m_nDownUp = 1;
// 	m_nLeftRight = 1;
// 	m_nDownUpPry = 1;
// 	m_nLeftRightPry = 1;

	m_nDownUp       = nRatType[0];
	m_nLeftRight    = nRatType[1];
	m_nDownUpPry    = nRatType[2];
	m_nLeftRightPry = nRatType[3];
	m_DrawCtrl.RemoveDrawData(TRUE, TRUE, FALSE); // Format들을 삭제 함
	//m_aDeviantLine.Init();

	m_nLastFormatID = -1;

	m_ShapeMgr.Init();
	m_ShapeMgr.SetData(m_aArBearingData, m_Data);
	m_ShapeMgr.SetDULR(m_nDownUp, m_nLeftRight);
	m_ShapeMgr.SetPryoutDULR(m_nDownUpPry, m_nLeftRightPry);
	if (m_ShapeMgr.m_dCopyingHeight == 0.0 || m_ShapeMgr.m_dCopyingWidth == 0.0)
		return;

	m_bDrawOK = TRUE;
	if (!m_ShapeMgr.CheckData())
	{
		Redraw(bFit);
		m_bDrawOK = FALSE;
		return;
	}

	if (m_nDrawType == 1)
	{
		SetRepresentConcFailBc();
	}

	if (m_nDrawType == 1)
	{
		m_ShapeMgr.UpdateCa1Auto();
		Setha();
	}
	else if (m_nDrawType == 2)
	{
		m_ShapeMgr.UpdatehefAuto();
		Setha();
	}

	if (m_nDrawType != 3) 
	{
		if (!MakeDrawCopying(bFit)) return;
	}
	m_dCopingSideGap = ConvWndPosToCoord(50);
	BOOL bFixed = IsFixAnchorExist();
	if ((m_nDrawType == 1 || m_nDrawType == 2) && bFixed)	MakeDrawCopyingSide(TRUE, bFit);

 	SetDimensionMembVal(); // 이 함수는 Dimension 을 그리기 전에 불러야 하며, 외각 영역이 그려져서 zoom 이 대략적으로 결정 된 후에 불러야 함	
	if (m_nDrawType != 3) MakeDrawBearing(TRUE);
	if (bFixed)
	{
		if (m_nDrawType == 1)	MakeDrawConcFailProjLine();
		if (m_nDrawType == 2)	MakeDrawPryoutProjXLine();
	}
	if (m_nDrawType != 3)	MakeDrawAnchor();
	if (m_nDrawType == 3)	MakeDrawBrgPreview(bFit);
	if (m_nDrawType == 0)	MakeDrawRefLine();
	if (m_nDrawType == 0)	MakeDrawDimensionLine();
	if (bFixed)
	{
		if (m_nDrawType == 2)	MakeDrawPryoutProjLine();
		if (m_nDrawType == 1 || m_nDrawType == 2)	MakeDrawProjDimensionLine();
	}
	
	if (m_bUser)
	{
		ConvDrawToGray(); // 흑백으로 바꿈
	}

	FullyRedraw(bFit);
}

void CDgnSeisAnchorEvalEditBtn::SetRepresentConcFailBc()
{
	std::vector<TConcFailBcCalc> aConcFailBc;
	m_ShapeMgr.GetConcFailBc(CDgnSeisAnchorShapeMgr::DIR_LONGI, aConcFailBc);
	m_ShapeMgr.GetMinConcFailBc(CDgnSeisAnchorShapeMgr::DIR_LONGI, aConcFailBc, m_LongiConcFailBc);

	m_ShapeMgr.GetConcFailBc(CDgnSeisAnchorShapeMgr::DIR_TRANS, aConcFailBc);
	m_ShapeMgr.GetMinConcFailBc(CDgnSeisAnchorShapeMgr::DIR_TRANS, aConcFailBc, m_TransConcFailBc);
}

BOOL CDgnSeisAnchorEvalEditBtn::IsFixAnchorExist()
{
	// Ca1,2 들을 계산 하는 위치에 고정된 Anchor 가 없으면 값을 계산할 방법이 없으므로 Error
	std::vector<TAnchorPosIdx> aCa1Idx, aCa1PIdx, aCa2Idx, aCa2PIdx;
	TConcFailBcCalc* pConcFailBc = &(m_nDir == CDgnSeisAnchorShapeMgr::DIR_LONGI ? m_LongiConcFailBc : m_TransConcFailBc);

	if (m_nDrawType == 1)
	{
		m_ShapeMgr.GetConcFailCaIdx(m_nDir, aCa1Idx, aCa1PIdx, aCa2Idx, aCa2PIdx, pConcFailBc->aBearingIdx);
	}
	else if (m_nDrawType == 2)
	{
		std::vector<std::pair<int, int>> aRepBearingIdx;
		m_ShapeMgr.GetPryoutRepresentGrpBearing(m_nDir, aRepBearingIdx);
		m_ShapeMgr.GetPryoutCaIdx(m_nDir, aCa1Idx, aCa1PIdx, aCa2Idx, aCa2PIdx, aRepBearingIdx);
	}
	else
	{
		return TRUE;
	}

	return aCa1Idx.size() != 0 && aCa1PIdx.size() != 0 && aCa2Idx.size() != 0 && aCa2PIdx.size() != 0;
}

void CDgnSeisAnchorEvalEditBtn::FullyRedraw(BOOL bFit)
{
	if (bFit)
	{
		m_DrawCtrl.AutoPaperFormat(m_Canvas, m_dFitMargin, m_dFitMargin, m_dFitMargin, m_dFitMargin);
	}
	m_DrawCtrl.ConvertDrawData(TRUE);

	OverlapChecking();
	Redraw(bFit);
}

BOOL CDgnSeisAnchorEvalEditBtn::SetData(std::vector<std::vector<T_BRPR_D>>& aArBearingData, T_ANEV_D& Data, BOOL bUser, BOOL bFit)
{
	//m_pTemplateCtrl->GetLengthCtrl()->GetHeightWidthByElem(tElemKey, m_dTotalHeight, m_dTotalWidth, m_dGapY, m_dGapZ, dPos);
	//m_pTemplateCtrl->GetPosByElem(tElemKey, dPos, m_dPos);
//  m_dTotalHeight = 10;
//  m_dTotalWidth = 10;

	m_Data = Data;

	//////////////////////////////////////////////////////////////////////////
	// 비어있는 곳 채움.
	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	pDoc->m_pAttrCtrl2->GetModifiedAnevD(m_Data);
	//////////////////////////////////////////////////////////////////////////

	m_aArBearingData = aArBearingData;
	m_bUser = bUser;

	pDoc->m_pAttrCtrl2->GetMaxSaxData(Data, m_aMaxSax);

	/*
	m_dBearingWidth = m_Data.dSx * 2.0;
	int nSize;
	nSize = m_Data.aS1.GetSize();
	for (int i = 0; i < nSize ; i++)
	{
		m_dBearingWidth += m_Data.aS1[i];
	}

	m_dBearingHeight = m_Data.dSy * 2.0;
	nSize = m_Data.aS2.GetSize();
	for (int i = 0; i < nSize ; i++)
	{
		m_dBearingHeight += m_Data.aS2[i];
	}
	*/

	int nRatType[4] = {m_nDownUp, m_nLeftRight, m_nDownUpPry ,m_nLeftRightPry};

	ResetSnapPos();
	MakeDrawUnit(nRatType, bFit);

	return TRUE;
}

double CDgnSeisAnchorEvalEditBtn::ConvWndPosToCoord(int nWndPos)
{
	double dZeroX, dZeroY;
	double dX, dY;
	GetGenuineCoord(CPoint(0, 0), dZeroX, dZeroY);
	GetGenuineCoord(CPoint(nWndPos, nWndPos), dX, dY);

	return dX - dZeroX;
}

double CDgnSeisAnchorEvalEditBtn::SetFloor(double dVal)
{
	CDBDoc* pDoc = CDBDoc::GetDocPoint();

	double dCeilVal = 1.0;

	T_UNIT_INDEX CurIndex;
	pDoc->m_pUnitCtrl->GetUnitIndexCurrent(CurIndex);
	CString strUnitLen;
	if (CurIndex.nBase_Length == D_UNITSYS_LENGTH_INDEX_MM)
	{
		dCeilVal = 1.0;
	}
	else if (CurIndex.nBase_Length == D_UNITSYS_LENGTH_INDEX_CM)
	{
		dCeilVal = 10.0;
	}
	else if (CurIndex.nBase_Length == D_UNITSYS_LENGTH_INDEX_M)
	{
		dCeilVal = 1000.0;
	}
	else if (CurIndex.nBase_Length == D_UNITSYS_LENGTH_INDEX_IN)
	{
		dCeilVal = 100.0;
	}
	else if (CurIndex.nBase_Length == D_UNITSYS_LENGTH_INDEX_FT)
	{
		dCeilVal = 1000.0;
	}
	else
	{
		ASSERT(0);
		return dVal;
	}

	dVal *= dCeilVal;
	dVal = floor(dVal);
	dVal /= dCeilVal;
	return dVal;
}

void CDgnSeisAnchorEvalEditBtn::Setha()
{
	TConcFailBcCalc* pConcFailBc = &(m_nDir == CDgnSeisAnchorShapeMgr::DIR_LONGI ? m_LongiConcFailBc : m_TransConcFailBc);

	if (m_nDrawType == 1)
	{
		double dLp, dCa1, dCa1P, dCa2, dCa2P, dCa1Temp;
		double dEachCa1, dEachCa1P, dEachCa2, dEachCa2P;
		int nBc;
		std::vector<std::pair<int, int>> aRepBearingIdx;
		if (m_ShapeMgr.GetConcFailRepresentVal(m_nDir, dLp, nBc, dCa1, dCa1P, dCa2, dCa2P))
		{
			m_ShapeMgr.GetConcfailRepresentGrpBearing(m_nDir, aRepBearingIdx);
			m_dHa = 0;
			for (int i = 0; i < aRepBearingIdx.size(); i++)
			{
				if (!m_ShapeMgr.GetReCalcCa1(m_nDir, aRepBearingIdx[i].first, aRepBearingIdx[i].second, dCa1Temp))
				{
					m_ShapeMgr.GetConcFailBearingCa(m_nDir, aRepBearingIdx[i].first, aRepBearingIdx[i].second, dEachCa1, dEachCa1P, dEachCa2, dEachCa2P);
					dCa1Temp = dEachCa1;
				}
				m_dHa += min(m_Data.dHcop, 1.5 * dCa1Temp);
			}
			if (aRepBearingIdx.size() > 0)
				m_dHa /= aRepBearingIdx.size();
		}
		else
		{
			m_dHa = m_Data.dHcop;
			ASSERT(0);
		}
	}
	else if (m_nDrawType == 2)
	{
		m_dHa = m_Data.dHcop;
	}
	else
	{
		ASSERT(0);
		return;
	}
}

BOOL CDgnSeisAnchorEvalEditBtn::MakeDrawCopying(BOOL bFit)
{
	m_dBoundBox_Bottom = 0.0;
	m_dBoundBox_Top = 0.0;
	m_dBoundBox_Left = 0.0;
	m_dBoundBox_Right = 0.0;

	//double dOnePixel;	
	//dOnePixel = ConvWndPosToCoord(2);
	//dOnePixel *= 0.70;

	T_POINT tPnt;
	T_POLYLINE_R tPolygon;

	T_POLYGON_R_GROUP tPolygonGroup;
	tPolygonGroup.List.SetSize(1);
	tPolygon.arPoint.RemoveAll();

	CArray<double, double> aX, aY, aModX, aModY;
	m_ShapeMgr.GetCopyingShape(aX, aY);
	ASSERT(aX.GetSize() == aY.GetSize());
	if (aX.GetSize() == 1 && aY.GetSize() == 1)    // layer가 0인 경우 고려 - 데이터는 1개 세팅
	{
		tPnt.x = aX[0];
		tPnt.y = aY[0];
		tPolygon.arPoint.Add(tPnt);

		tPolygonGroup.List[0] = tPolygon;
		m_DrawCtrl.AddDrawUnit(m_nLastFormatID, tPolygonGroup);

		FullyRedraw(bFit);

		return FALSE;
	}

	T_DRAW_FORMAT	DrawFormat;
	DrawFormat.bLine = TRUE;
	DrawFormat.bFill = TRUE;
	DrawFormat.lineColor = RGB(0, 0, 0);
	DrawFormat.fillColor = m_ColorCopying;
	DrawFormat.Width = 1;
	m_DrawCtrl.m_arDrawFormat.SetAt(++m_nLastFormatID, DrawFormat);

	int nSize;
	nSize = aX.GetSize();

	if (m_bTrans)
	{
		for (int i = 0; i < aX.GetSize(); i++)
		{
			m_dCopingCenterX += aX[i];
			m_dCopingCenterY += aY[i];
		}

		m_dCopingCenterX /= aX.GetSize();
		m_dCopingCenterY /= aX.GetSize();

		ConvtoTransScale(aX, aY, m_dCopingCenterX, m_dCopingCenterY, aModX, aModY);

		for (int i = 0; i < nSize; i++)
		{
			tPnt.x = aModX[i];
			tPnt.y = aModY[i];
			tPolygon.arPoint.Add(tPnt);

			CalcBoundingBox(tPnt.x, tPnt.y);
		}
	}
	else
	{
		for (int i = 0; i < nSize; i++)
		{
			tPnt.x = aX[i];
			tPnt.y = aY[i];
			tPolygon.arPoint.Add(tPnt);

			CalcBoundingBox(tPnt.x, tPnt.y);
		}
	}

	tPolygonGroup.List[0] = tPolygon;
	m_DrawCtrl.AddDrawUnit(m_nLastFormatID, tPolygonGroup);

	FullyRedraw(bFit); // 고정Size 수치들을 세팅 하기 전에 Redraw를 해줘야 함

	return TRUE;
}

void CDgnSeisAnchorEvalEditBtn::MakeDrawCopyingSide(BOOL bFill, BOOL bFit)
{
	if (m_nDrawType == 0) return;

	CArray<double, double> aX, aY;
	m_ShapeMgr.GetCopyingShape(aX, aY);

	if (aX.GetSize() != 4)
	{
		return;
	}

	T_DRAW_FORMAT	DrawFormat;
	DrawFormat.bLine = TRUE;
	DrawFormat.bFill = bFill;
	DrawFormat.lineColor = RGB(0, 0, 0);
	DrawFormat.fillColor = m_ColorCopying;
	DrawFormat.Width = 1;
	m_DrawCtrl.m_arDrawFormat.SetAt(++m_nLastFormatID, DrawFormat);

	T_POINT tPnt, tModPnt;
	T_POLYLINE_R tPolygon;

	T_POLYGON_R_GROUP tPolygonGroup;
	tPolygonGroup.List.SetSize(1);
	tPolygon.arPoint.RemoveAll();

	//T_POLYLINE_R DeviantLine;

	double dYStart;
	if (m_nDrawType == 1 || (m_nDrawType == 2 && m_nDir == CDgnSeisAnchorShapeMgr::DIR_TRANS))
	{
		// 아래 coping		
		// 0
		if (m_nDownUp == 0)
		{
			dYStart = aY[0] - m_dCopingSideGap;
			tPnt.x = aX[0];
			tPnt.y = dYStart - m_dHa;
		}
		else if (m_nDownUp == 1)
		{
			dYStart = aY[2] + m_dCopingSideGap;
			tPnt.x = aX[2];
			tPnt.y = dYStart + m_dHa;
		}


		if (m_bTrans)
		{
			ConvtoTransScale(tPnt.x, tPnt.y, m_dCopingCenterX, m_dCopingCenterY, tModPnt.x, tModPnt.y);

			tPnt = tModPnt;
		}

		tPolygon.arPoint.Add(tPnt);
		CalcBoundingBox(tPnt.x, tPnt.y);

		// 1

		if (m_nDownUp == 0)
		{
			tPnt.x = aX[0];
			tPnt.y = dYStart;
		}
		else if (m_nDownUp == 1)
		{
			tPnt.x = aX[2];
			tPnt.y = dYStart;
		}

		m_BotCopingStPos.x = tPnt.x;
		m_BotCopingStPos.y = tPnt.y;

		m_SideCopingLineForLeft[0] = m_BotCopingStPos;

		if (m_bTrans)
		{
			ConvtoTransScale(tPnt.x, tPnt.y, m_dCopingCenterX, m_dCopingCenterY, tModPnt.x, tModPnt.y);

			tPnt = tModPnt;
		}

		tPolygon.arPoint.Add(tPnt);
		CalcBoundingBox(tPnt.x, tPnt.y);

		// 2

		if (m_nDownUp == 0)
		{
			tPnt.x = aX[3];
			tPnt.y = dYStart;
		}
		else if (m_nDownUp == 1)
		{
			tPnt.x = aX[1];
			tPnt.y = dYStart;
		}

		m_SideCopingLineForLeft[1].x = tPnt.x;
		m_SideCopingLineForLeft[1].y = tPnt.y;

		if (m_bTrans)
		{
			ConvtoTransScale(tPnt.x, tPnt.y, m_dCopingCenterX, m_dCopingCenterY, tModPnt.x, tModPnt.y);

			tPnt = tModPnt;
		}

		tPolygon.arPoint.Add(tPnt);
		CalcBoundingBox(tPnt.x, tPnt.y);

		// 3

		if (m_nDownUp == 0)
		{
			tPnt.x = aX[3];
			tPnt.y = dYStart - m_dHa;
		}
		else if (m_nDownUp == 1)
		{
			tPnt.x = aX[1];
			tPnt.y = dYStart + m_dHa;
		}

		if (m_bTrans)
		{
			ConvtoTransScale(tPnt.x, tPnt.y, m_dCopingCenterX, m_dCopingCenterY, tModPnt.x, tModPnt.y);

			tPnt = tModPnt;
		}

		tPolygon.arPoint.Add(tPnt);
		CalcBoundingBox(tPnt.x, tPnt.y);

		tPolygonGroup.List[0] = tPolygon;
		m_DrawCtrl.AddDrawUnit(m_nLastFormatID, tPolygonGroup);
	}

	if (m_nDrawType == 1 || (m_nDrawType == 2 && m_nDir == CDgnSeisAnchorShapeMgr::DIR_LONGI))
	{
		// 좌측 Coping
		CArray<double, double> aLeftX, aLeftY;
		double Dir[2], dTmp;
		
		if (m_nLeftRight == CDgnSeisAnchorShapeMgr::DIR_POS)
		{
			m_ShapeMgr.GetDir(aX[0], aY[0], aX[1], aY[1], Dir[0], Dir[1]);
			// 방향을 -90도 돌림
			dTmp = Dir[0];
			Dir[0] = -Dir[1];
			Dir[1] = dTmp;

			aLeftX.Add(aX[0] - m_dCopingSideGap);
			aLeftY.Add(aY[0]);

			aLeftX.Add(aX[0] - m_dCopingSideGap);
			aLeftY.Add(aY[0]);
			m_ShapeMgr.PntMove(Dir[0], Dir[1], m_dHa, aLeftX[1], aLeftY[1]);

			aLeftX.Add(aX[1] - m_dCopingSideGap);
			aLeftY.Add(aY[1]);
			m_ShapeMgr.PntMove(Dir[0], Dir[1], m_dHa, aLeftX[2], aLeftY[2]);

			aLeftX.Add(aX[1] - m_dCopingSideGap);
			aLeftY.Add(aY[1]);
		}
		
		else if (m_nLeftRight == CDgnSeisAnchorShapeMgr::DIR_NEG)
		{

			m_ShapeMgr.GetDir(aX[2], aY[2], aX[3], aY[3], Dir[0], Dir[1]);
			// 방향을 -90도 돌림
			dTmp = Dir[0];
			Dir[0] = -Dir[1];
			Dir[1] = dTmp;

			aLeftX.Add(aX[2] + m_dCopingSideGap);
			aLeftY.Add(aY[2]);

			aLeftX.Add(aX[2] + m_dCopingSideGap);
			aLeftY.Add(aY[2]);
			m_ShapeMgr.PntMove(Dir[0], Dir[1], m_dHa, aLeftX[1], aLeftY[1]);

			aLeftX.Add(aX[3] + m_dCopingSideGap);
			aLeftY.Add(aY[3]);
			m_ShapeMgr.PntMove(Dir[0], Dir[1], m_dHa, aLeftX[2], aLeftY[2]);

			aLeftX.Add(aX[3] + m_dCopingSideGap);
			aLeftY.Add(aY[3]);
		}

		int nSize = aLeftX.GetSize();
		tPolygon.arPoint.RemoveAll();
		for (int i = 0; i < nSize; i++)
		{
			tPnt.x = aLeftX[i];
			tPnt.y = aLeftY[i];

			if (i == 0)
			{
				m_LeftCopingStPos.x = tPnt.x;
				m_LeftCopingStPos.y = tPnt.y;

				m_SideCopingLineForBot[0] = m_LeftCopingStPos;
			}
			else if (i == 3)
			{
				m_SideCopingLineForBot[1].x = tPnt.x;
				m_SideCopingLineForBot[1].y = tPnt.y;
			}

			if (m_bTrans)
			{
				ConvtoTransScale(tPnt.x, tPnt.y, m_dCopingCenterX, m_dCopingCenterY, tModPnt.x, tModPnt.y);

				tPnt = tModPnt;
			}

			tPolygon.arPoint.Add(tPnt);
			CalcBoundingBox(tPnt.x, tPnt.y);
		}

		tPolygonGroup.List[0] = tPolygon;
		m_DrawCtrl.AddDrawUnit(m_nLastFormatID, tPolygonGroup);



		/* 배치 치수선 따라 Gap이 조절 되는 코드임
		/// 좌측 coping
		dXStart = aX[0] - fabs(aX[1] - aX[0]) - dSideGap;
		dYStart = aY[0];
		// TopView Coping 을 0,0 에서 시작하는 좌표로 수정
		int nSize;
		nSize = aX.GetSize();

		double dFirstX, dFirstY;
		dFirstX = aX[0];
		dFirstY = aY[0];

		for (int i = 0; i < nSize ; i++)
		{
			aX[i] -= dFirstX;
			aY[i] -= dFirstY;
		}

		// 좌측 Coping 의 좌표값 세팅
		CArray<double, double> aLeftX, aLeftY;
		double dir[2];
		aLeftX.Add(aX[0]);
		aLeftY.Add(aY[0]);
		m_ShapeMgr.GetDir(aX[0], aY[0], -aY[1], aX[1], dir[0], dir[1]);
		aLeftX.Add(aX[0]);
		aLeftY.Add(aY[0]);
		m_ShapeMgr.PntMove(dir[0], dir[1], m_dha, aLeftX[1], aLeftY[1]);
		aLeftX.Add(aX[1]);
		aLeftY.Add(aY[1]);
		m_ShapeMgr.PntMove(dir[0], dir[1], m_dha, aLeftX[2], aLeftY[2]);
		aLeftX.Add(aX[1]);
		aLeftY.Add(aY[1]);
		nSize = aLeftX.GetSize();
		for (int i = 0; i < nSize ; i++)
		{
			aLeftX[i] += dXStart;
			aLeftY[i] += dYStart;
		}
		m_LeftCopingStPos.x = -fabs(aX[1] - aX[0]) - dSideGap;
		m_LeftCopingStPos.y = 0.0;

		// AddDraw
		nSize = aLeftX.GetSize();
		tPolygon.arPoint.RemoveAll();
		for (int i = 0; i < nSize ; i++)
		{
			tPnt.x = aLeftX[i];
			tPnt.y = aLeftY[i];
			tPolygon.arPoint.Add(tPnt);
			CalcBoundingBox(tPnt.x, tPnt.y);
		}
		tPolygonGroup.List[0] = tPolygon;
		m_DrawCtrl.AddDrawUnit(m_nLastFormatID, tPolygonGroup);
		*/
	}

	//DeviantLine = tPolygon;
	//m_aDeviantLine.List.Add(DeviantLine);

	FullyRedraw(bFit); // 고정Size 수치들을 세팅 하기 전에 Redraw를 해줘야 함
}

void CDgnSeisAnchorEvalEditBtn::ConvtoTransScale(CArray<double, double>& aX, CArray<double, double>& aY, double dCopingCenterX, double dCopingCenterY, CArray<double, double>& aModX, CArray<double, double>& aModY)
{
	aModX.RemoveAll();
	aModY.RemoveAll();
	aModX.SetSize(aX.GetSize());
	aModY.SetSize(aY.GetSize());

	for (int i = 0; i < aX.GetSize(); i++)
	{
		ConvtoTransScale(aX[i], aY[i], dCopingCenterX, dCopingCenterY, aModX[i], aModY[i]);
	}
}

void CDgnSeisAnchorEvalEditBtn::ConvtoTransScale(double dX, double dY, double dCopingCenterX, double dCopingCenterY, double& dModX, double& dModY)
{
	dModX = dX;
	dModY = dY;

	// Rotate
	dModX -= dCopingCenterX;
	dModY -= dCopingCenterY;
	math_lib::rotate_2d(dModX, dModY, 90 - m_Data.dCopingAngle); // Corping Angle 은 빼야함
	dModX += dCopingCenterX;
	dModY += dCopingCenterY;
}

void CDgnSeisAnchorEvalEditBtn::ConvtoPreviewScale(CArray<double, double>& aX, CArray<double, double>& aY, double dCopingCenterX, double dCopingCenterY, double dPreviewScale, T_POINT3D& MovePos, CArray<double, double>& aMiniX, CArray<double, double>& aMiniY)
{
	aMiniX.RemoveAll();
	aMiniY.RemoveAll();
	aMiniX.SetSize(aX.GetSize());
	aMiniY.SetSize(aY.GetSize());

	for (int i = 0; i < aX.GetSize(); i++)
	{
		ConvtoPreviewScale(aX[i], aY[i], dCopingCenterX, dCopingCenterY, dPreviewScale, MovePos, aMiniX[i], aMiniY[i]);
	}
}

void CDgnSeisAnchorEvalEditBtn::ConvtoPreviewScale(double dX, double dY, double dCopingCenterX, double dCopingCenterY, double dPreviewScale, T_POINT3D& MovePos, double& dMiniX, double& dMiniY)
{
	dMiniX = dX;
	dMiniY = dY;

	// Rotate
	dMiniX -= dCopingCenterX;
	dMiniY -= dCopingCenterY;
	math_lib::rotate_2d(dMiniX, dMiniY, m_Data.dRelAngle + 90);
	dMiniX += dCopingCenterX;
	dMiniY += dCopingCenterY;

	// Scale
	dMiniX *= dPreviewScale;
	dMiniY *= dPreviewScale;

	// Move
	dMiniX += MovePos.x;
	dMiniY += MovePos.y;
}

COLORREF CDgnSeisAnchorEvalEditBtn::SetColorBright(COLORREF Color, COLORREF val)
{
	COLORREF R = min(GetRValue(Color) + val, 255);
	COLORREF G = min(GetGValue(Color) + val, 255);
	COLORREF B = min(GetBValue(Color) + val, 255);

	return RGB(R, G, B);
}

void CDgnSeisAnchorEvalEditBtn::MakeDrawBrgPreview(BOOL bFit)
{
	//if(fabs(m_Data.dRelAngle) < m_dZero)
	//{
	//	return;
	//}

	CArray<double, double> aCopingX, aCopingY, aMiniCopingX, aMiniCopingY, aMiniBaeringX, aMiniBaeringY;
	m_ShapeMgr.GetCopyingShape(aCopingX, aCopingY);
	ASSERT(aCopingX.GetSize() == aCopingY.GetSize());


	T_POINT3D MovePos;
	//MovePos.x = aCopingX[2] - dMiniBrgLineLen * 0.5 - m_ShapeMgr.m_dCopyingWidth * 0.5 * dPreviewScale;
	//MovePos.x = aCopingX[3] - m_ShapeMgr.m_dCopyingWidth * 0.5 * dPreviewScale - dMiniBrgLineLen * 0.3;
	//MovePos.y = aCopingY[3] - m_ShapeMgr.m_dCopyingHeight * 1/8.0 - dMiniBrgLineLen * 0.5;

	double dAng = m_Data.dRelAngle + 90;
	double dPreviewScale = 1.0 / 4.5;
	double dCopyingLen = max(m_ShapeMgr.m_dCopyingWidth, m_ShapeMgr.m_dCopyingHeight);

	// Guide 대화상자를 만들기 전엔 위치 조절이 필요했었는데.. Guide 대화상자가 생기면서 필요 없게됨... 간단하게 movepos 와 scale 만 초기값으로 세팅해서 기존 코드 돌림.. 나중에 어떻게 수정할지 모르니..
	MovePos.x = 0;
	MovePos.y = 0;
	dPreviewScale = 1.0;

	double dMiniBrgLineLen = dCopyingLen * dPreviewScale * 1.6;

	double dCopingCenterX, dCopingCenterY;
	dCopingCenterX = dCopingCenterY = 0.0;
	for (int i = 0; i < aCopingX.GetSize(); i++)
	{
		dCopingCenterX += aCopingX[i];
		dCopingCenterY += aCopingY[i];
	}
	dCopingCenterX /= aCopingX.GetSize();
	dCopingCenterY /= aCopingX.GetSize();

	// Set Copying
	ConvtoPreviewScale(aCopingX, aCopingY, dCopingCenterX, dCopingCenterY, dPreviewScale, MovePos, aMiniCopingX, aMiniCopingY);

	// Set Bridge Line
	double dMiniCopingYMin, dMiniCopingYMax; // 0:min, 1:max
	dMiniCopingYMin = DBL_MAX;
	dMiniCopingYMax = -DBL_MAX;
	for (int i = 0; i < aMiniCopingY.GetSize(); i++)
	{
		if (aMiniCopingY[i] < dMiniCopingYMin)
		{
			dMiniCopingYMin = aMiniCopingY[i];
		}
		if (aMiniCopingY[i] > dMiniCopingYMax)
		{
			dMiniCopingYMax = aMiniCopingY[i];
		}
	}
	double dMiniCenterX, dMiniCenterY;
	ConvtoPreviewScale(dCopingCenterX, dCopingCenterY, dCopingCenterX, dCopingCenterY, dPreviewScale, MovePos, dMiniCenterX, dMiniCenterY);

	double dBrgLineLeft, dBrgLineRight, dBrgLineTop, dBrgLineBot;
	dBrgLineLeft = aCopingX[3] - dMiniBrgLineLen * 0.8;
	dBrgLineRight = aCopingX[3] + dMiniBrgLineLen * 0.2;
	dBrgLineTop = dMiniCopingYMax - dMiniBrgLineLen * 0.1;
	dBrgLineBot = dMiniCopingYMin + dMiniBrgLineLen * 0.1;

	// Draw Coping,Brg
	T_DRAW_FORMAT	DrawFormat;

	DrawFormat.bLine = TRUE;
	DrawFormat.lineColor = SetColorBright(m_ColorBearingLine, 100);
	DrawFormat.Width = 2;
	m_DrawCtrl.m_arDrawFormat.SetAt(++m_nLastFormatID, DrawFormat);
	T_LINE_R_GROUP tArrowGroup;
	tArrowGroup.List.SetSize(2);
	tArrowGroup.List[0].P1.x = dBrgLineLeft;
	tArrowGroup.List[0].P1.y = dBrgLineTop;
	tArrowGroup.List[0].P2.x = dBrgLineRight;
	tArrowGroup.List[0].P2.y = dBrgLineTop;
	tArrowGroup.List[1].P1.x = dBrgLineLeft;
	tArrowGroup.List[1].P1.y = dBrgLineBot;
	tArrowGroup.List[1].P2.x = dBrgLineRight;
	tArrowGroup.List[1].P2.y = dBrgLineBot;
	m_DrawCtrl.AddDrawUnit(m_nLastFormatID, tArrowGroup);

	double dBrgBoxTop = (dBrgLineTop + dBrgLineBot) * 0.5 + dMiniBrgLineLen * 0.5;

	T_POINT tPnt;
	T_POLYLINE_R tPolygon;
	T_POLYGON_R_GROUP tPolygonGroup;
	DrawFormat.bLine = TRUE;
	DrawFormat.bFill = FALSE;
	DrawFormat.lineColor = m_ColorCopying;
	DrawFormat.Width = 1;
	m_DrawCtrl.m_arDrawFormat.SetAt(++m_nLastFormatID, DrawFormat);
	tPolygonGroup.List.SetSize(1);
	tPolygon.arPoint.RemoveAll();
	tPnt.x = dBrgLineLeft;
	tPnt.y = dBrgBoxTop;
	tPolygon.arPoint.Add(tPnt);
	tPnt.x = dBrgLineRight;
	tPnt.y = dBrgBoxTop;
	tPolygon.arPoint.Add(tPnt);
	tPnt.x = dBrgLineRight;
	tPnt.y = dBrgBoxTop - dMiniBrgLineLen;
	tPolygon.arPoint.Add(tPnt);
	tPnt.x = dBrgLineLeft;
	tPnt.y = dBrgBoxTop - dMiniBrgLineLen;
	tPolygon.arPoint.Add(tPnt);
	tPolygonGroup.List[0] = tPolygon;
	m_DrawCtrl.AddDrawUnit(m_nLastFormatID, tPolygonGroup);
	CalcBoundingBox(tPnt.x, tPnt.y);

	DrawFormat.bLine = TRUE;
	DrawFormat.bFill = TRUE;
	DrawFormat.lineColor = SetColorBright(RGB(0, 0, 0), 100);
	DrawFormat.fillColor = SetColorBright(m_ColorCopying, 5);
	DrawFormat.Width = 1;
	m_DrawCtrl.m_arDrawFormat.SetAt(++m_nLastFormatID, DrawFormat);
	tPolygonGroup.List.SetSize(1);
	tPolygon.arPoint.RemoveAll();
	int nSize;
	nSize = aMiniCopingX.GetSize();
	for (int i = 0; i < nSize; i++)
	{
		tPnt.x = aMiniCopingX[i];
		tPnt.y = aMiniCopingY[i];
		tPolygon.arPoint.Add(tPnt);

		CalcBoundingBox(tPnt.x, tPnt.y);
	}
	tPolygonGroup.List[0] = tPolygon;
	m_DrawCtrl.AddDrawUnit(m_nLastFormatID, tPolygonGroup);

	// 큰 Coping 이 중앙에 그려지게 하기 위한 처리
	T_POINT_R_GROUP tPointGroup;
	tPointGroup.List.RemoveAll();
	DrawFormat.lineColor = RGB(255, 255, 255);;
	m_DrawCtrl.m_arDrawFormat.SetAt(++m_nLastFormatID, DrawFormat);
	T_POINT EachPntDraw;
	EachPntDraw.x = aCopingX[1];// - (dMiniBrgLineLen * 0.5) * 1.3;
	EachPntDraw.y = aCopingY[1] + (dMiniBrgLineLen) * 0.4;
	tPointGroup.List.Add(EachPntDraw);
	m_DrawCtrl.AddDrawUnit(m_nLastFormatID, tPointGroup);
	CalcBoundingBox(EachPntDraw.x, EachPntDraw.y);

	// Draw Bearing, Anchor
	DrawFormat.bLine = TRUE;
	DrawFormat.bFill = TRUE;
	DrawFormat.lineColor = SetColorBright(m_ColorBearingLine, 20);
	DrawFormat.fillColor = SetColorBright(m_ColorBearing, 20);
	DrawFormat.Width = 1;
	m_DrawCtrl.m_arDrawFormat.SetAt(++m_nLastFormatID, DrawFormat);
	CArray<double, double> aX, aY;
	tPolygonGroup.List.SetSize(1);

	int nBearingNumI;
	CArray<int, int> aBearingNumJ;
	m_ShapeMgr.GetBearingNumber(nBearingNumI, aBearingNumJ);

	for (int nIdxBearingI = 0; nIdxBearingI < nBearingNumI; nIdxBearingI++)
	{
		for (int nIdxBearingJ = 0; nIdxBearingJ < aBearingNumJ[nIdxBearingI]; nIdxBearingJ++)
		{
			tPolygon.arPoint.RemoveAll();

			m_ShapeMgr.GetBearingShape(nIdxBearingI, nIdxBearingJ, aX, aY);
			ASSERT(aX.GetSize() == aY.GetSize());

			ConvtoPreviewScale(aX, aY, dCopingCenterX, dCopingCenterY, dPreviewScale, MovePos, aMiniBaeringX, aMiniBaeringY);

			nSize = aMiniBaeringX.GetSize();
			for (int i = 0; i < nSize; i++)
			{
				tPnt.x = aMiniBaeringX[i];
				tPnt.y = aMiniBaeringY[i];
				tPolygon.arPoint.Add(tPnt);
			}
			tPolygonGroup.List[0] = tPolygon;
			m_DrawCtrl.AddDrawUnit(m_nLastFormatID, tPolygonGroup);
		}
	}

	// Anchor  	
	tPointGroup.List.RemoveAll();
	DrawFormat.lineColor = SetColorBright(RGB(0, 0, 0), 100);
	m_DrawCtrl.m_arDrawFormat.SetAt(++m_nLastFormatID, DrawFormat);
	double dMiniPntX, dMiniPntY;
	int nAnchorNumI, nAnchorNumJ;
	double dMinDia;
	dMinDia = ConvWndPosToCoord(1);
	for (int i = 0; i < nBearingNumI; i++)
	{
		for (int j = 0; j < aBearingNumJ[i]; j++)
		{
			m_ShapeMgr.GetAnchorNumber(i, j, nAnchorNumI, nAnchorNumJ);

			EachPntDraw.Init();

			for (int ii = 0; ii < nAnchorNumI; ii++)
			{
				for (int jj = 0; jj < nAnchorNumJ; jj++)
				{
					m_ShapeMgr.CalcAnchorPos(i, j, ii, jj, EachPntDraw.x, EachPntDraw.y);
					ConvtoPreviewScale(EachPntDraw.x, EachPntDraw.y, dCopingCenterX, dCopingCenterY, dPreviewScale, MovePos, dMiniPntX, dMiniPntY);
					EachPntDraw.x = dMiniPntX;
					EachPntDraw.y = dMiniPntY;

					tPointGroup.List.Add(EachPntDraw);
					tPointGroup.List.Add(EachPntDraw);
				}
			}
		}
	}
	m_DrawCtrl.AddDrawUnit(m_nLastFormatID, tPointGroup);

	FullyRedraw(bFit);
}

void CDgnSeisAnchorEvalEditBtn::MakeDrawRefLine()
{
	m_dDimGap = ConvWndPosToCoord(5);

	T_DRAW_FORMAT	DrawFormat;
	DrawFormat.bLine = TRUE;
	DrawFormat.lineColor = m_ColorRefLine2;
	DrawFormat.nPenStyle = BS_SOLID;
	DrawFormat.nPenSubStyle = PS_GEOMETRIC | PS_DOT;
	DrawFormat.startArrowType = EN_ARROW_TYPE_NONE;
	DrawFormat.endArrowType = EN_ARROW_TYPE_NONE;
	DrawFormat.Width =1;
	m_DrawCtrl.m_arDrawFormat.SetAt(++m_nLastFormatID, DrawFormat);

	

	CArray<double, double> aX, aY, aModX, aModY;
	int nFormatIDTxt;
	m_ShapeMgr.GetCopyingShape(aX, aY);

	int nSize = aX.GetSize();

	double dMinX, dMaxX, dMinY, dMaxY;
	dMinX = dMinY = DBL_MAX;
	dMaxX = dMaxY = -DBL_MAX;

	if (m_bTrans)
	{	
		/// CL
		T_ARROWLINE_R_GROUP tArrowGroup;
		tArrowGroup.List.SetSize(1);
		T_LINE_R tLine;
		T_LINE_R tModLine;
		double dLineGap;
		dLineGap = m_ShapeMgr.m_dCopyingWidth*0.08;
		tLine.P1.x = -dLineGap;
		tLine.P1.y = 0;
		tLine.P2.x = m_ShapeMgr.m_dCopyingWidth + dLineGap;
		tLine.P2.y = 0;

		ConvtoTransScale(tLine.P1.x, tLine.P1.y, m_dCopingCenterX, m_dCopingCenterY, tModLine.P1.x, tModLine.P1.y);
		ConvtoTransScale(tLine.P2.x, tLine.P2.y, m_dCopingCenterX, m_dCopingCenterY, tModLine.P2.x, tModLine.P2.y);

		tLine = tModLine;
		tArrowGroup.List[0] = tLine;
		m_DrawCtrl.AddDrawUnit(m_nLastFormatID, tArrowGroup);


		/// CL Text
		T_TEXT_FORMAT	TextFormat;
		T_TEXT_R_GROUP tTextGroup;
		tTextGroup.List.SetSize(1);

		TextFormat.Size = 13;
		TextFormat.fontColor = RGB(255, 0, 0);
		TextFormat.positionType = EN_POSITION_TYPE_CENTER_VCENTER;
		if (m_bTrans) TextFormat.nEscapement = 0;
		else          TextFormat.nEscapement = 900;
		m_DrawCtrl.m_arTextFormat.SetAt(++m_nLastFormatID, TextFormat);
		nFormatIDTxt = m_nLastFormatID;
		tTextGroup.List[0].PPosition.x = tLine.P2.x + (tLine.P2.x- tLine.P1.x)*0.03;
		tTextGroup.List[0].PPosition.y = tLine.P2.y + (tLine.P2.y - tLine.P1.y)*0.03;

		tTextGroup.List[0].szText = _LSX(C.L);
		m_DrawCtrl.AddDrawUnit(nFormatIDTxt, tTextGroup);


		/// CA
		ConvtoTransScale(aX, aY, m_dCopingCenterX, m_dCopingCenterY, aModX, aModY);

		for (int i = 0; i < nSize; i++)
		{
			aX[i] = aModX[i];
			aY[i] = aModY[i];
		}

		for (int i = 0; i < nSize; i++)
		{
			if (dMinX > aX[i])
			{
				dMinX = aX[i];
			}
			if (dMaxX < aX[i])
			{
				dMaxX = aX[i];
			}

			if (dMinY > aY[i])
			{
				dMinY = aY[i];
			}
			if (dMaxY < aY[i])
			{
				dMaxY = aY[i];
			}
		}

		
		DrawFormat.lineColor = m_ColorRefLine;
		m_DrawCtrl.m_arDrawFormat.SetAt(++m_nLastFormatID, DrawFormat);

		tLine.P1.x = dMinX + ((dMaxX - dMinX) / 2);
		tLine.P1.y = dMinY - m_dDimGap;
		tLine.P2.x = dMinX + ((dMaxX - dMinX) / 2);
		tLine.P2.y = dMaxY + m_dDimGap;
		tArrowGroup.List[0] = tLine;
		m_DrawCtrl.AddDrawUnit(m_nLastFormatID, tArrowGroup);

		/// CA Text
		tTextGroup.List.SetSize(1);

		TextFormat.fontColor = RGB(0, 0, 255);
		TextFormat.positionType = EN_POSITION_TYPE_CENTER_VCENTER;
		if (m_bTrans) TextFormat.nEscapement = 0;
		else          TextFormat.nEscapement = 900;
		m_DrawCtrl.m_arTextFormat.SetAt(++m_nLastFormatID, TextFormat);
		nFormatIDTxt = m_nLastFormatID;
		tTextGroup.List[0].PPosition.x = tLine.P2.x;
		tTextGroup.List[0].PPosition.y = tLine.P2.y*1.1;

		tTextGroup.List[0].szText = _LSX(C.A);
		if (m_Data.dCopingAngle!=0) m_DrawCtrl.AddDrawUnit(nFormatIDTxt, tTextGroup);
		


	}
	else
	{
		for (int i = 0; i < nSize; i++)
		{
			if (dMinX > aX[i])
			{
				dMinX = aX[i];
			}
			if (dMaxX < aX[i])
			{
				dMaxX = aX[i];
			}
		}

		T_ARROWLINE_R_GROUP tArrowGroup;
		tArrowGroup.List.SetSize(1);
		T_LINE_R tLine;
		tLine.P1.x = dMinX - m_dDimGap;
		tLine.P1.y = 0;
		tLine.P2.x = dMaxX + m_dDimGap;
		tLine.P2.y = 0;
		tArrowGroup.List[0] = tLine;
		m_DrawCtrl.AddDrawUnit(m_nLastFormatID, tArrowGroup);
	}
}

void CDgnSeisAnchorEvalEditBtn::MakeDrawAnchor()
{
	T_CIRCLE_R_GROUP tCircleGroup;
	tCircleGroup.List.RemoveAll();

	T_DRAW_FORMAT	DrawFormat;
	DrawFormat.bLine = TRUE;
	DrawFormat.bFill = TRUE;
	DrawFormat.Width = 1;
	DrawFormat.lineColor = RGB(0, 0, 0);
	DrawFormat.fillColor = m_ColorAnchor;
	m_DrawCtrl.m_arDrawFormat.SetAt(++m_nLastFormatID, DrawFormat);

	int nBearingNumI, nAnchorNumI, nAnchorNumJ;
	CArray<int, int> aBearingNumJ;

	m_DrawCtrl.AutoPaperFormat(m_Canvas, m_dFitMargin, m_dFitMargin, m_dFitMargin, m_dFitMargin);// 전체 Size가 잡힌 다음 ConvWndPosToCoord 를 부르기전에 한번은 불러 줘야 함

	double dMinDia;
	dMinDia = ConvWndPosToCoord(2);

	m_ShapeMgr.GetBearingNumber(nBearingNumI, aBearingNumJ);

	T_CIRCLE_R EachDraw;
	double dModX, dModY;

	for (int i = 0; i < nBearingNumI; i++)
	{
		for (int j = 0; j < aBearingNumJ[i]; j++)
		{
			m_ShapeMgr.GetAnchorNumber(i, j, nAnchorNumI, nAnchorNumJ);

			EachDraw.Init();
			//EachDraw.Radius = m_aArBearingData[i][j].dda < dMinDia ? dMinDia : m_aArBearingData[i][j].dda;
			EachDraw.Radius = m_ShapeMgr.GetAnchorProp(i, j).dda < dMinDia ? dMinDia : m_ShapeMgr.GetAnchorProp(i, j).dda;
			EachDraw.Radius *= 0.5;

			for (int ii = 0; ii < nAnchorNumI; ii++)
			{
				for (int jj = 0; jj < nAnchorNumJ; jj++)
				{
					m_ShapeMgr.CalcAnchorPos(i, j, ii, jj, EachDraw.CenterP.x, EachDraw.CenterP.y);

					if (m_bTrans)
					{
						ConvtoTransScale(EachDraw.CenterP.x, EachDraw.CenterP.y, m_dCopingCenterX, m_dCopingCenterY, dModX, dModY);
						EachDraw.CenterP.x = dModX;
						EachDraw.CenterP.y = dModY;
					}

					tCircleGroup.List.Add(EachDraw);
					tCircleGroup.List.Add(EachDraw);
				}
			}
		}
	}
	m_DrawCtrl.AddDrawUnit(m_nLastFormatID, tCircleGroup);
}

void CDgnSeisAnchorEvalEditBtn::MakeDrawBearing(BOOL bFill)
{
	CArray<double, double> aX, aY, aModX, aModY;

	T_DRAW_FORMAT	DrawFormat;
	DrawFormat.bLine = TRUE;
	DrawFormat.bFill = bFill;
	DrawFormat.lineColor = m_ColorBearingLine;
	DrawFormat.fillColor = m_ColorBearing;
	DrawFormat.Width = 1;
	m_DrawCtrl.m_arDrawFormat.SetAt(++m_nLastFormatID, DrawFormat);

	T_POINT tPnt;
	T_POLYLINE_R tPolygon;

	T_POLYGON_R_GROUP tPolygonGroup;
	tPolygonGroup.List.SetSize(1);

	int nSize;

	int nBearingNumI;
	CArray<int, int> aBearingNumJ;
	m_ShapeMgr.GetBearingNumber(nBearingNumI, aBearingNumJ);

	for (int nIdxBearingI = 0; nIdxBearingI < nBearingNumI; nIdxBearingI++)
	{
		for (int nIdxBearingJ = 0; nIdxBearingJ < aBearingNumJ[nIdxBearingI]; nIdxBearingJ++)
		{
			tPolygon.arPoint.RemoveAll();
			m_ShapeMgr.GetBearingShape(nIdxBearingI, nIdxBearingJ, aX, aY);
			ASSERT(aX.GetSize() == aY.GetSize());

			nSize = aX.GetSize();

			if (m_bTrans)
			{
				ConvtoTransScale(aX, aY, m_dCopingCenterX, m_dCopingCenterY, aModX, aModY);

				for (int i = 0; i < nSize; i++)
				{
					tPnt.x = aModX[i];
					tPnt.y = aModY[i];
					tPolygon.arPoint.Add(tPnt);
				}
			}
			else
			{
				for (int i = 0; i < nSize; i++)
				{
					tPnt.x = aX[i];
					tPnt.y = aY[i];
					tPolygon.arPoint.Add(tPnt);
				}
			}

			tPolygonGroup.List[0] = tPolygon;
			m_DrawCtrl.AddDrawUnit(m_nLastFormatID, tPolygonGroup);
		}
	}
}

void CDgnSeisAnchorEvalEditBtn::MakeDrawPryoutProjXLine()
{
	double dSmallVal;
	dSmallVal = ConvWndPosToCoord(1);
	dSmallVal *= 1.5;

	CArray<T_CARRAY<double, double>, T_CARRAY<double, double>&> aArProjShapeInX, aArProjShapeInY, aArProjShapeOutX, aArProjShapeOutY;
	aArProjShapeInX.RemoveAll();
	aArProjShapeInY.RemoveAll();
	m_ShapeMgr.GetPryoutBcNoUnion(m_nDir, aArProjShapeInX, aArProjShapeInY, aArProjShapeOutX, aArProjShapeOutY, dSmallVal);

	/// Bearing 내부
	T_DRAW_FORMAT	DrawFormat;
	DrawFormat.bLine = TRUE;
	DrawFormat.bFill = FALSE;
	DrawFormat.lineColor = m_ColorPryoutXProjLineIn;
	DrawFormat.startArrowType = EN_ARROW_TYPE_NONE;
	DrawFormat.endArrowType = EN_ARROW_TYPE_NONE;
	DrawFormat.Width = 1;
	m_DrawCtrl.m_arDrawFormat.SetAt(++m_nLastFormatID, DrawFormat);

	T_ARROWLINE_R_GROUP tArrowGroup;
	tArrowGroup.List.SetSize(1);
	T_LINE_R tLine, tModLine;
	int nSize;
	nSize = aArProjShapeInX.GetSize();
	for (int i = 0; i < nSize; i++)
	{
		if (aArProjShapeInX[i].aData.GetSize() != 2)
		{
			ASSERT(0);
			continue;
		}

		tLine.P1.x = aArProjShapeInX[i].aData[0];
		tLine.P1.y = aArProjShapeInY[i].aData[0];
		tLine.P2.x = aArProjShapeInX[i].aData[1];
		tLine.P2.y = aArProjShapeInY[i].aData[1];

		if (m_bTrans)
		{
			ConvtoTransScale(tLine.P1.x, tLine.P1.y, m_dCopingCenterX, m_dCopingCenterY, tModLine.P1.x, tModLine.P1.y);
			ConvtoTransScale(tLine.P2.x, tLine.P2.y, m_dCopingCenterX, m_dCopingCenterY, tModLine.P2.x, tModLine.P2.y);

			tLine = tModLine;
		}

		tArrowGroup.List[0] = tLine;
		m_DrawCtrl.AddDrawUnit(m_nLastFormatID, tArrowGroup);
	}

	// Bearing 외부
	DrawFormat.lineColor = m_ColorPryoutXProjLineOut;
	m_DrawCtrl.m_arDrawFormat.SetAt(++m_nLastFormatID, DrawFormat);

	tArrowGroup.List.SetSize(1);
	nSize = aArProjShapeOutX.GetSize();
	for (int i = 0; i < nSize; i++)
	{
		if (aArProjShapeOutX[i].aData.GetSize() != 2)
		{
			ASSERT(0);
			continue;
		}

		tLine.P1.x = aArProjShapeOutX[i].aData[0];
		tLine.P1.y = aArProjShapeOutY[i].aData[0];
		tLine.P2.x = aArProjShapeOutX[i].aData[1];
		tLine.P2.y = aArProjShapeOutY[i].aData[1];

		if (m_bTrans)
		{
			ConvtoTransScale(tLine.P1.x, tLine.P1.y, m_dCopingCenterX, m_dCopingCenterY, tModLine.P1.x, tModLine.P1.y);
			ConvtoTransScale(tLine.P2.x, tLine.P2.y, m_dCopingCenterX, m_dCopingCenterY, tModLine.P2.x, tModLine.P2.y);

			tLine = tModLine;
		}

		tArrowGroup.List[0] = tLine;
		m_DrawCtrl.AddDrawUnit(m_nLastFormatID, tArrowGroup);
	}

	// Bearing에 선을 다시 그림..
	MakeDrawBearing(FALSE);
}

void CDgnSeisAnchorEvalEditBtn::MakeDrawPryoutProjLine()
{
	CArray<T_CARRAY<double, double>, T_CARRAY<double, double>&> aArProjShapeX;
	CArray<T_CARRAY<double, double>, T_CARRAY<double, double>&> aArProjShapeY;
	std::set<int> setBcIdx;
	BOOL bSingleResist;
	m_ShapeMgr.GetPryoutBc(m_nDir, aArProjShapeX, aArProjShapeY, setBcIdx, bSingleResist);

	T_DRAW_FORMAT	DrawFormat;
	DrawFormat.bLine = TRUE;
	DrawFormat.bFill = FALSE;
	DrawFormat.lineColor = m_ColorPryoutProjLineRep;
	DrawFormat.Width = 2;
	m_DrawCtrl.m_arDrawFormat.SetAt(++m_nLastFormatID, DrawFormat);
	int nRefFormat = m_nLastFormatID;

	DrawFormat.Width = 1;
	DrawFormat.lineColor = m_ColorPryoutProjLine;
	m_DrawCtrl.m_arDrawFormat.SetAt(++m_nLastFormatID, DrawFormat);

	T_POINT tPnt, tModPnt;
	T_POLYLINE_R tPolygon;

	T_POLYGON_R_GROUP tPolygonGroup;
	tPolygonGroup.List.SetSize(1);
	int nSize, nSizeJ;
	nSize = aArProjShapeX.GetSize();
	for (int i = 0; i < nSize; i++)
	{
		tPolygon.arPoint.RemoveAll();

		nSizeJ = aArProjShapeX[i].aData.GetSize();
		for (int j = 0; j < nSizeJ; j++)
		{
			tPnt.x = aArProjShapeX[i].aData[j];
			tPnt.y = aArProjShapeY[i].aData[j];

			if (m_bTrans)
			{
				ConvtoTransScale(tPnt.x, tPnt.y, m_dCopingCenterX, m_dCopingCenterY, tModPnt.x, tModPnt.y);

				tPnt = tModPnt;
			}

			tPolygon.arPoint.Add(tPnt);
		}
		tPolygonGroup.List[0] = tPolygon;
		if (setBcIdx.find(i) != setBcIdx.end())
		{
			m_DrawCtrl.AddDrawUnit(nRefFormat, tPolygonGroup);
		}
		else
		{
			m_DrawCtrl.AddDrawUnit(m_nLastFormatID, tPolygonGroup);
		}
	}
}

void CDgnSeisAnchorEvalEditBtn::MakeDrawConcFailProjLine()
{
	std::vector<T_LINE2D> aSideLine;
	m_ShapeMgr.GetConcFailSideLine(m_nDir, aSideLine, m_nDownUp, m_nLeftRight);

	CArray<double, double> aCopingX, aCopingY;
	m_ShapeMgr.GetCopyingShape(aCopingX, aCopingY);

	T_DRAW_FORMAT	DrawFormat;
	DrawFormat.bLine = TRUE;
	//DrawFormat.bFill  = FALSE;		  
	DrawFormat.lineColor = m_ColorConcFailProjLine;
	//DrawFormat.fillColor = RGB(118,   0,   118);
	DrawFormat.startArrowType = EN_ARROW_TYPE_NONE;
	DrawFormat.endArrowType = EN_ARROW_TYPE_NONE;
	DrawFormat.Width = 1;
	m_DrawCtrl.m_arDrawFormat.SetAt(++m_nLastFormatID, DrawFormat);

	T_ARROWLINE_R_GROUP tArrowGroup;
	tArrowGroup.List.SetSize(1);
	T_LINE_R tLine, tModLine;

	CArray<int, int> aBearingI, aBearingJ, aAnchorI, aAnchorJ;
	m_ShapeMgr.GetPrjTgtIdxConcFail(m_nDir, aBearingI, aBearingJ, aAnchorI, aAnchorJ);
	if (aBearingI.GetSize() < 1 || aBearingJ.GetSize() < 1 || aAnchorI.GetSize() < 1 || aAnchorJ.GetSize() < 1)
	{
		return;
	}

	ASSERT(aBearingI.GetSize() == aBearingJ.GetSize());
	ASSERT(aBearingI.GetSize() == aAnchorI.GetSize());
	ASSERT(aBearingI.GetSize() == aAnchorJ.GetSize());

	double AnchorPos[2], Pos1[2], Pos2[2], Pos1P[2], Pos2P[2];

	T_LINE_R SideLine;
	SideLine.P1.x = aCopingX[0];
	SideLine.P1.y = aCopingY[0];
	SideLine.P2.x = aCopingX[0];
	SideLine.P2.y = aCopingY[0];

	T_POINT Left, Right;

	int nSize;
	nSize = aBearingI.GetSize();

	double dDistMin = 1.0E7;
	if (m_nDir == 1/*DIR_TRANS*/)
	{
		double dDist = 0.0;
		for (int i = 0; i < nSize; i++)
		{
			m_ShapeMgr.CalcRecalAnchorProjPosConcFailForDist(m_nDir, aBearingI[i], aBearingJ[i], aAnchorI[i], aAnchorJ[i], dDist);
			dDistMin = min(dDistMin, dDist);
		}
	}

	for (int i = 0; i < nSize; i++)
	{
		m_ShapeMgr.CalcAnchorPos(aBearingI[i], aBearingJ[i], aAnchorI[i], aAnchorJ[i], AnchorPos[0], AnchorPos[1]);
		m_ShapeMgr.CalcAnchorProjPosConcFail(m_nDir, aBearingI[i], aBearingJ[i], aAnchorI[i], aAnchorJ[i], Pos1[0], Pos1[1], Pos2[0], Pos2[1]);
		m_ShapeMgr.CalcRecalAnchorProjPosConcFail(m_nDir, aBearingI[i], aBearingJ[i], aAnchorI[i], aAnchorJ[i], dDistMin, Pos1P[0], Pos1P[1], Pos2P[0], Pos2P[1]);

		tLine.P1.x = AnchorPos[0];
		tLine.P1.y = AnchorPos[1];
		tLine.P2.x = Pos1[0];
		tLine.P2.y = Pos1[1];

		if (m_bTrans)
		{
			ConvtoTransScale(tLine.P1.x, tLine.P1.y, m_dCopingCenterX, m_dCopingCenterY, tModLine.P1.x, tModLine.P1.y);
			ConvtoTransScale(tLine.P2.x, tLine.P2.y, m_dCopingCenterX, m_dCopingCenterY, tModLine.P2.x, tModLine.P2.y);

			tLine = tModLine;
		}

		tArrowGroup.List[0] = tLine;
		m_DrawCtrl.AddDrawUnit(m_nLastFormatID, tArrowGroup);

		tLine.P1.x = AnchorPos[0];
		tLine.P1.y = AnchorPos[1];
		tLine.P2.x = Pos2[0];
		tLine.P2.y = Pos2[1];

		if (m_bTrans)
		{
			ConvtoTransScale(tLine.P1.x, tLine.P1.y, m_dCopingCenterX, m_dCopingCenterY, tModLine.P1.x, tModLine.P1.y);
			ConvtoTransScale(tLine.P2.x, tLine.P2.y, m_dCopingCenterX, m_dCopingCenterY, tModLine.P2.x, tModLine.P2.y);

			tLine = tModLine;
		}

		tArrowGroup.List[0] = tLine;
		m_DrawCtrl.AddDrawUnit(m_nLastFormatID, tArrowGroup);
	}

	m_CaCalcPos[0].x = aSideLine[0].P1.x;
	m_CaCalcPos[0].y = aSideLine[0].P1.y;
	m_CaCalcPos[1].x = aSideLine[(int)aSideLine.size() - 1].P2.x;
	m_CaCalcPos[1].y = aSideLine[(int)aSideLine.size() - 1].P2.y;

	// Coping 아랫쪽의 강조선 표현	
	DrawFormat.bLine = TRUE;		  
	DrawFormat.lineColor = m_ColorConcFailFill;
	DrawFormat.startArrowType = EN_ARROW_TYPE_NONE;
	DrawFormat.endArrowType = EN_ARROW_TYPE_NONE;
	DrawFormat.Width = 4;
	m_DrawCtrl.m_arDrawFormat.SetAt(++m_nLastFormatID, DrawFormat);
	int nFormatIDBc = m_nLastFormatID;

	DrawFormat.Width = 3;
	DrawFormat.lineColor = m_ColorConcFailLine2;
	m_DrawCtrl.m_arDrawFormat.SetAt(++m_nLastFormatID, DrawFormat);

	double Line[2][2], dLp;
	int nBc;
	TConcFailBcCalc MinConcFailBc;
	m_ShapeMgr.GetConcFailBcRecal(m_nDir, MinConcFailBc);
	Line[0][0] = MinConcFailBc.LpLine[0][0];
	Line[0][1] = MinConcFailBc.LpLine[0][1];
	Line[1][0] = MinConcFailBc.LpLine[1][0];
	Line[1][1] = MinConcFailBc.LpLine[1][1];
	dLp = MinConcFailBc.dLp;
	nBc = MinConcFailBc.nBc;

	nSize = (int)aSideLine.size();
	if (nSize >1 && nSize != m_ShapeMgr.m_SideLineIdx.size())
	{
		ASSERT(0);
	}
	BOOL t_flag;
	if (nSize > 1)
	{
		for (int i = 0; i < nSize; i++)
		{	
			t_flag = TRUE;
			//////
			for (int j = 0; j < MinConcFailBc.aBearingIdx.size(); j++)
			{
				if (m_ShapeMgr.m_SideLineIdx[i].first == MinConcFailBc.aBearingIdx[j].first && m_ShapeMgr.m_SideLineIdx[i].second == MinConcFailBc.aBearingIdx[j].second)
				{
					t_flag = FALSE;
					break;
				}
			}
			if (t_flag == FALSE)
			{
				continue;
			}
			//////
			tLine.P1.x = aSideLine[i].P1.x;
			tLine.P1.y = aSideLine[i].P1.y;
			tLine.P2.x = aSideLine[i].P2.x;
			tLine.P2.y = aSideLine[i].P2.y;

			if (m_bTrans)
			{
				ConvtoTransScale(tLine.P1.x, tLine.P1.y, m_dCopingCenterX, m_dCopingCenterY, tModLine.P1.x, tModLine.P1.y);
				ConvtoTransScale(tLine.P2.x, tLine.P2.y, m_dCopingCenterX, m_dCopingCenterY, tModLine.P2.x, tModLine.P2.y);

				tLine = tModLine;
			}

			tArrowGroup.List[0] = tLine;

			m_DrawCtrl.AddDrawUnit(m_nLastFormatID, tArrowGroup);
		}
	}
	

	tLine.P1.x = Line[0][0];
	tLine.P1.y = Line[0][1];
	tLine.P2.x = Line[1][0];
	tLine.P2.y = Line[1][1];

	if (m_bTrans)
	{
		ConvtoTransScale(tLine.P1.x, tLine.P1.y, m_dCopingCenterX, m_dCopingCenterY, tModLine.P1.x, tModLine.P1.y);
		ConvtoTransScale(tLine.P2.x, tLine.P2.y, m_dCopingCenterX, m_dCopingCenterY, tModLine.P2.x, tModLine.P2.y);

		tLine = tModLine;
	}

	tArrowGroup.List[0] = tLine;

	m_DrawCtrl.AddDrawUnit(nFormatIDBc, tArrowGroup);

	// 아래 혹은 왼쪽 coping 에 사각형 표현	
	DrawFormat.bLine = FALSE;
	DrawFormat.bFill = TRUE;
	DrawFormat.lineColor = m_ColorConcFailFill;
	DrawFormat.fillColor = m_ColorConcFailFill;
	DrawFormat.Width = 1;
	m_DrawCtrl.m_arDrawFormat.SetAt(++m_nLastFormatID, DrawFormat);

	//T_POLYGON2D DeviantLine;

	double Dir[2];
	double dTmp;

	T_POINT tPnt;
	T_POLYLINE_R tPolygon, tModPolygon;
	T_POLYGON_R_GROUP tPolygonGroup;
	tPolygonGroup.List.SetSize(1);
	tPolygon.arPoint.SetSize(4);
	tModPolygon.arPoint.SetSize(4);
	if (m_nDir == CDgnSeisAnchorShapeMgr::DIR_LONGI)
	{
		if (m_nDownUp == 0)
		{
			tPolygon.arPoint[0].x = Line[0][0] - aCopingX[0] + m_BotCopingStPos.x;
			tPolygon.arPoint[0].y = Line[0][1] - aCopingY[0] + m_BotCopingStPos.y;
			tPolygon.arPoint[1].x = Line[1][0] - aCopingX[0] + m_BotCopingStPos.x;
			tPolygon.arPoint[1].y = Line[1][1] - aCopingY[0] + m_BotCopingStPos.y;
			tPolygon.arPoint[2].x = tPolygon.arPoint[1].x;
			tPolygon.arPoint[2].y = tPolygon.arPoint[1].y - m_dHa;
			tPolygon.arPoint[3].x = tPolygon.arPoint[0].x;
			tPolygon.arPoint[3].y = tPolygon.arPoint[0].y - m_dHa;
		}
		else if (m_nDownUp == 1)
		{
			tPolygon.arPoint[0].x = Line[0][0] - aCopingX[2] + m_BotCopingStPos.x;
			tPolygon.arPoint[0].y = Line[0][1] - aCopingY[2] + m_BotCopingStPos.y;
			tPolygon.arPoint[1].x = Line[1][0] - aCopingX[2] + m_BotCopingStPos.x;
			tPolygon.arPoint[1].y = Line[1][1] - aCopingY[2] + m_BotCopingStPos.y;
			tPolygon.arPoint[2].x = tPolygon.arPoint[1].x;
			tPolygon.arPoint[2].y = tPolygon.arPoint[1].y + m_dHa;
			tPolygon.arPoint[3].x = tPolygon.arPoint[0].x;
			tPolygon.arPoint[3].y = tPolygon.arPoint[0].y + m_dHa;
		}
		
	}
	else if (m_nDir == CDgnSeisAnchorShapeMgr::DIR_TRANS)
	{	
		if (m_nLeftRight == CDgnSeisAnchorShapeMgr::DIR_POS)
		{
			tPolygon.arPoint[0].x = Line[0][0] - aCopingX[0] + m_LeftCopingStPos.x;
			tPolygon.arPoint[0].y = Line[0][1] - aCopingY[0] + m_LeftCopingStPos.y;
			tPolygon.arPoint[1].x = Line[1][0] - aCopingX[0] + m_LeftCopingStPos.x;
			tPolygon.arPoint[1].y = Line[1][1] - aCopingY[0] + m_LeftCopingStPos.y;
			m_ShapeMgr.GetDir(tPolygon.arPoint[0].x, tPolygon.arPoint[0].y, tPolygon.arPoint[1].x, tPolygon.arPoint[1].y, Dir[0], Dir[1]);
			dTmp = Dir[0];
			Dir[0] = -Dir[1];
			Dir[1] = dTmp;
			tPolygon.arPoint[2].x = tPolygon.arPoint[1].x;
			tPolygon.arPoint[2].y = tPolygon.arPoint[1].y;
			m_ShapeMgr.PntMove(Dir[0], Dir[1], m_dHa, tPolygon.arPoint[2].x, tPolygon.arPoint[2].y);
			tPolygon.arPoint[3].x = tPolygon.arPoint[0].x;
			tPolygon.arPoint[3].y = tPolygon.arPoint[0].y;
			m_ShapeMgr.PntMove(Dir[0], Dir[1], m_dHa, tPolygon.arPoint[3].x, tPolygon.arPoint[3].y);
		}
		else if (m_nLeftRight == CDgnSeisAnchorShapeMgr::DIR_NEG)
		{
			tPolygon.arPoint[0].x = Line[0][0] - aCopingX[2] + m_LeftCopingStPos.x;
			tPolygon.arPoint[0].y = Line[0][1] - aCopingY[2] + m_LeftCopingStPos.y;
			tPolygon.arPoint[1].x = Line[1][0] - aCopingX[2] + m_LeftCopingStPos.x;
			tPolygon.arPoint[1].y = Line[1][1] - aCopingY[2] + m_LeftCopingStPos.y;
			m_ShapeMgr.GetDir(tPolygon.arPoint[0].x, tPolygon.arPoint[0].y, tPolygon.arPoint[1].x, tPolygon.arPoint[1].y, Dir[0], Dir[1]);
			dTmp = Dir[0];
			Dir[0] = -Dir[1];
			Dir[1] = dTmp;
			tPolygon.arPoint[2].x = tPolygon.arPoint[1].x;
			tPolygon.arPoint[2].y = tPolygon.arPoint[1].y;
			m_ShapeMgr.PntMove(Dir[0], Dir[1], m_dHa, tPolygon.arPoint[2].x, tPolygon.arPoint[2].y);
			tPolygon.arPoint[3].x = tPolygon.arPoint[0].x;
			tPolygon.arPoint[3].y = tPolygon.arPoint[0].y;
			m_ShapeMgr.PntMove(Dir[0], Dir[1], m_dHa, tPolygon.arPoint[3].x, tPolygon.arPoint[3].y);
		}
	}

	if (m_bTrans)
	{
		for (int i = 0; i < tPolygon.arPoint.GetSize(); i++)
		{
			ConvtoTransScale(tPolygon.arPoint[i].x, tPolygon.arPoint[i].y, m_dCopingCenterX, m_dCopingCenterY, tModPolygon.arPoint[i].x, tModPolygon.arPoint[i].y);
		}

		tPolygonGroup.List[0] = tModPolygon;
	}
	else
	{
		tPolygonGroup.List[0] = tPolygon;
	}

	m_DrawCtrl.AddDrawUnit(m_nLastFormatID, tPolygonGroup);

	//m_aDeviantLine.List.Add(tPolygon);
	MakeDrawCopyingSide(FALSE, FALSE); // Coping 선을 외각선만 다시 그린다..

	// 치수선 그리기
	int nFormatIDLine, nFormatIDArrow, nFormatIDTxt;
	DrawFormat.bLine = TRUE;
	//DrawFormat.bFill  = FALSE;		  
	DrawFormat.lineColor = m_ColorDimensionLine;
	//DrawFormat.fillColor = RGB(118,   0,   118);
	DrawFormat.startArrowType = EN_ARROW_TYPE_NONE;
	DrawFormat.endArrowType = EN_ARROW_TYPE_NONE;
	DrawFormat.Width = 1;
	m_DrawCtrl.m_arDrawFormat.SetAt(++m_nLastFormatID, DrawFormat);
	nFormatIDLine = m_nLastFormatID;

	DrawFormat.startArrowType = EN_ARROW_TYPE_ARROW;
	DrawFormat.endArrowType = EN_ARROW_TYPE_ARROW;
	DrawFormat.dRWidth = 3;
	DrawFormat.dRHeight = 2;
	m_DrawCtrl.m_arDrawFormat.SetAt(++m_nLastFormatID, DrawFormat);
	nFormatIDArrow = m_nLastFormatID;

	T_TEXT_FORMAT	TextFormat;
	T_TEXT_R_GROUP tTextGroup;
	tTextGroup.List.SetSize(1);

	TextFormat.Size = 13;
	TextFormat.fontColor = RGB(0, 0, 0);
	TextFormat.positionType = EN_POSITION_TYPE_CENTER_VCENTER;

	T_POINT2D Line2D[2];

	CString strTmp;
	double dLength;

	//
	Line2D[0].x = tPolygon.arPoint[0].x;
	Line2D[0].y = tPolygon.arPoint[0].y;
	Line2D[1].x = tPolygon.arPoint[1].x;
	Line2D[1].y = tPolygon.arPoint[1].y;

	dLength = math_lib::distance_2d(Line2D[0].x, Line2D[0].y, Line2D[1].x, Line2D[1].y);
	if (m_bUser)
	{
		strTmp = _T("Lp");
	}
	else
	{
		strTmp.Format(_T("%g"), SetFloor(CMathFunc::mathRoundOff(dLength, 7)));
	}


	BOOL bRight;
	if (m_nDir == CDgnSeisAnchorShapeMgr::DIR_LONGI)
	{
		if (m_bTrans) TextFormat.nEscapement = 900;
		else          TextFormat.nEscapement = 0;

		bRight = FALSE;
	}
	else if (m_nDir == CDgnSeisAnchorShapeMgr::DIR_TRANS)
	{
		if (m_bTrans) TextFormat.nEscapement = 900 + (int)((-m_Data.dTheta1 - 90) * 10.0);
		else          TextFormat.nEscapement = (int)((-m_Data.dTheta1 - 90) * 10.0);

		bRight = TRUE;
	}
	if (TextFormat.nEscapement == 900 || TextFormat.nEscapement == -900)
	{
		TextFormat.Size = 12;
	}
	else
	{
		TextFormat.Size = 13;
	}
	m_DrawCtrl.m_arTextFormat.SetAt(++m_nLastFormatID, TextFormat);
	nFormatIDTxt = m_nLastFormatID;

	MakeDrawBasicDim(nFormatIDLine, nFormatIDArrow, nFormatIDTxt, Line2D, m_dDimFrontGap * 0.5, strTmp, bRight);

	//
	m_ShapeMgr.GetDir(Line2D[0].x, Line2D[0].y, Line2D[1].x, Line2D[1].y, Dir[0], Dir[1]);
	if (m_nDir == CDgnSeisAnchorShapeMgr::DIR_LONGI)
	{
		// 90도 돌림
		dTmp = Dir[0];
		Dir[0] = Dir[1];
		Dir[1] = -dTmp;
	}
	else if (m_nDir == CDgnSeisAnchorShapeMgr::DIR_TRANS)
	{
		// -90도 돌림
		dTmp = Dir[0];
		Dir[0] = -Dir[1];
		Dir[1] = dTmp;
	}

	Line2D[1] = Line2D[0];
	if (m_nDir == CDgnSeisAnchorShapeMgr::DIR_LONGI)
	{
		Line2D[0].x = Line2D[1].x = m_BotCopingStPos.x;
		Line2D[0].y = Line2D[1].y = m_BotCopingStPos.y;
	}
	else
	{
		Line2D[0].x = Line2D[1].x = m_LeftCopingStPos.x;
		Line2D[0].y = Line2D[1].y = m_LeftCopingStPos.y;
	}

	m_ShapeMgr.PntMove(Dir[0], Dir[1], m_dHa, Line2D[1].x, Line2D[1].y);

	if (m_bUser)
	{
		if (m_nDrawType == 1)
		{
			strTmp = _T("Ha");
		}
		else if (m_nDrawType == 2)
		{
			strTmp = _T("Hcop");
		}
		else
		{
			ASSERT(0);
			strTmp = _T("Hcop");
		}
	}
	else
	{
		strTmp.Format(_T("%g"), SetFloor(CMathFunc::mathRoundOff(m_dHa, 7)));
	}
	if (m_nDir == CDgnSeisAnchorShapeMgr::DIR_LONGI)
	{
		if (m_bTrans) TextFormat.nEscapement = 0;
		else          TextFormat.nEscapement = 900;

		bRight = TRUE;
	}
	else if (m_nDir == CDgnSeisAnchorShapeMgr::DIR_TRANS)
	{
		if (m_bTrans) TextFormat.nEscapement = 900 + (int)((-m_Data.dTheta1) * 10.0);
		else          TextFormat.nEscapement = (int)((-m_Data.dTheta1) * 10.0);

		bRight = FALSE;
	}
	if (TextFormat.nEscapement == 900 || TextFormat.nEscapement == -900)
	{
		TextFormat.Size = 12;
	}
	else
	{
		TextFormat.Size = 13;
	}
	m_DrawCtrl.m_arTextFormat.SetAt(++m_nLastFormatID, TextFormat);
	nFormatIDTxt = m_nLastFormatID;
	MakeDrawBasicDim(nFormatIDLine, nFormatIDArrow, nFormatIDTxt, Line2D, m_dDimFrontGap * 0.5, strTmp, bRight);
}

void CDgnSeisAnchorEvalEditBtn::SetDimensionMembVal()
{
	m_dDimLenShort = ConvWndPosToCoord(20);
	m_dDimLenLong = ConvWndPosToCoord(36);
	m_dDimLenTailGap = ConvWndPosToCoord(5);
	m_GapCri = ConvWndPosToCoord(4);
	m_dDimOverlap = ConvWndPosToCoord(8);
	m_dDimLendTxtGap = ConvWndPosToCoord(7);
	m_dDimFrontGap = ConvWndPosToCoord(7);
}

void CDgnSeisAnchorEvalEditBtn::MakeDrawDimensionLine()
{
	CArray<double, double> aX, aY, aModX, aModY;
	m_ShapeMgr.GetCopyingShape(aX, aY);
	ASSERT(aX.GetSize() == aY.GetSize());


	std::vector<double> aXPos, aYPos;

	double dAccu = 0.0;;
	aXPos.push_back(0.0);
	dAccu += m_Data.aLayer[0].dD1;
	aXPos.push_back(dAccu);
	for (int i = 0; i < m_aMaxSax.GetSize()/*m_Data.aLayer[0].aSax.GetSize()*/; i++)
	{
		dAccu += m_aMaxSax[i]; //m_Data.aLayer[m_Data.aLayer.GetSize() - 1].aSax[i];
		aXPos.push_back(dAccu);
	}
	aXPos.push_back(m_ShapeMgr.m_dCopyingWidth);

	dAccu = aY[1];
	aYPos.push_back(0.0);
	for (int i = 0; i < m_Data.aLayer.GetSize(); i++)
	{
		dAccu =  m_Data.aLayer[i].dD2_q;		//check dAccu =  m_Data.aLayer[i].dD2_q; DB수정되고 고쳐야함.
		aYPos.push_back(dAccu);
	}

	int nFormatIDLine, nFormatIDArrow, nFormatIDTxt, nFormatIDTxtY, nFormatIDDot, nFormatIDDirec, nFormatIDTxtDirec;

	T_DRAW_FORMAT	DrawFormat;
	DrawFormat.bLine = TRUE;
	//DrawFormat.bFill  = FALSE;		  
	DrawFormat.lineColor = m_ColorDimensionLine;
	//DrawFormat.fillColor = RGB(118,   0,   118); 
	DrawFormat.startArrowType = EN_ARROW_TYPE_NONE;
	DrawFormat.endArrowType = EN_ARROW_TYPE_NONE;
	DrawFormat.Width = 1;
	m_DrawCtrl.m_arDrawFormat.SetAt(++m_nLastFormatID, DrawFormat);
	nFormatIDLine = m_nLastFormatID;

	DrawFormat.nPenStyle = BS_SOLID;
	DrawFormat.nPenSubStyle = PS_GEOMETRIC | PS_DOT;
	DrawFormat.Width = 1;
	m_DrawCtrl.m_arDrawFormat.SetAt(++m_nLastFormatID, DrawFormat);
	nFormatIDDot = m_nLastFormatID;

	DrawFormat.nPenStyle = BS_SOLID;
	DrawFormat.nPenSubStyle = -1;
	DrawFormat.Width = 1;
	DrawFormat.startArrowType = EN_ARROW_TYPE_ARROW;
	DrawFormat.endArrowType = EN_ARROW_TYPE_ARROW;
	DrawFormat.dRWidth = 3;
	DrawFormat.dRHeight = 2;
	m_DrawCtrl.m_arDrawFormat.SetAt(++m_nLastFormatID, DrawFormat);
	nFormatIDArrow = m_nLastFormatID;

	DrawFormat.Width = 1.3;
	m_DrawCtrl.m_arDrawFormat.SetAt(++m_nLastFormatID, DrawFormat);
	nFormatIDDirec = m_nLastFormatID;


	T_TEXT_FORMAT	TextFormat;
	T_TEXT_R_GROUP tTextGroup;
	T_TEXT_R_GROUP tTextModGroup;
	tTextGroup.List.SetSize(1);
	tTextModGroup.List.SetSize(1);

	TextFormat.Size = 13;
	TextFormat.fontColor = RGB(0, 0, 0);
	TextFormat.positionType = EN_POSITION_TYPE_CENTER_VCENTER;
	if (m_bTrans) TextFormat.nEscapement = 900;
	else          TextFormat.nEscapement = 0;
	m_DrawCtrl.m_arTextFormat.SetAt(++m_nLastFormatID, TextFormat);
	nFormatIDTxt = m_nLastFormatID;

	TextFormat.Size = 12;
	TextFormat.positionType = EN_POSITION_TYPE_CENTER_VCENTER;
	if (m_bTrans) TextFormat.nEscapement = 0;
	else          TextFormat.nEscapement = 900;
	m_DrawCtrl.m_arTextFormat.SetAt(++m_nLastFormatID, TextFormat);
	nFormatIDTxtY = m_nLastFormatID;

	T_ARROWLINE_R_GROUP tArrowGroup;
	tArrowGroup.List.SetSize(1);
	T_LINE_R tLine;
	T_LINE_R tModLine;

	double dBotY, dTopY;
	dTopY = aY[1];
	dBotY = aY[0];

	// 	X 방향(B, D1, Sax, D'1)	
	// B
	tLine.P1.x = 0;
	tLine.P1.y = dTopY;
	tLine.P2.x = 0;
	tLine.P2.y = dTopY + m_dDimLenLong;
	tLine.P1.y += m_dDimFrontGap;
	tLine.P2.y += m_dDimFrontGap;
	
	if (m_bTrans)
	{
		ConvtoTransScale(tLine.P1.x, tLine.P1.y, m_dCopingCenterX, m_dCopingCenterY, tModLine.P1.x, tModLine.P1.y);
		ConvtoTransScale(tLine.P2.x, tLine.P2.y, m_dCopingCenterX, m_dCopingCenterY, tModLine.P2.x, tModLine.P2.y);
	
		tLine = tModLine;
	}
	
	tArrowGroup.List[0] = tLine;
	m_DrawCtrl.AddDrawUnit(nFormatIDLine, tArrowGroup);
	
	if (fabs(m_Data.dTheta1) > m_dZero)
	{
		tLine.P1.x = 0;
		tLine.P1.y = 0;
		tLine.P2.x = 0;
		tLine.P2.y = dTopY + m_dDimFrontGap;
	
		if (m_bTrans)
		{
			ConvtoTransScale(tLine.P1.x, tLine.P1.y, m_dCopingCenterX, m_dCopingCenterY, tModLine.P1.x, tModLine.P1.y);
			ConvtoTransScale(tLine.P2.x, tLine.P2.y, m_dCopingCenterX, m_dCopingCenterY, tModLine.P2.x, tModLine.P2.y);
	
			tLine = tModLine;
		}
	
		tArrowGroup.List[0] = tLine;
		m_DrawCtrl.AddDrawUnit(nFormatIDDot, tArrowGroup);
	}
	
	tLine.P1.x = m_ShapeMgr.m_dCopyingWidth;
	tLine.P1.y = dTopY;
	tLine.P2.x = m_ShapeMgr.m_dCopyingWidth;
	tLine.P2.y = dTopY + m_dDimLenLong;
	tLine.P1.y += m_dDimFrontGap;
	tLine.P2.y += m_dDimFrontGap;
	
	if (m_bTrans)
	{
		ConvtoTransScale(tLine.P1.x, tLine.P1.y, m_dCopingCenterX, m_dCopingCenterY, tModLine.P1.x, tModLine.P1.y);
		ConvtoTransScale(tLine.P2.x, tLine.P2.y, m_dCopingCenterX, m_dCopingCenterY, tModLine.P2.x, tModLine.P2.y);
	
		tLine = tModLine;
	}
	
	tArrowGroup.List[0] = tLine;
	m_DrawCtrl.AddDrawUnit(nFormatIDLine, tArrowGroup);
	
	if (fabs(m_Data.dTheta2) > m_dZero)
	{
		tLine.P1.x = m_ShapeMgr.m_dCopyingWidth;
		tLine.P1.y = 0;
		tLine.P2.x = m_ShapeMgr.m_dCopyingWidth;
		tLine.P2.y = dTopY + m_dDimFrontGap;
	
		if (m_bTrans)
		{
			ConvtoTransScale(tLine.P1.x, tLine.P1.y, m_dCopingCenterX, m_dCopingCenterY, tModLine.P1.x, tModLine.P1.y);
			ConvtoTransScale(tLine.P2.x, tLine.P2.y, m_dCopingCenterX, m_dCopingCenterY, tModLine.P2.x, tModLine.P2.y);
	
			tLine = tModLine;
		}
	
		tArrowGroup.List[0] = tLine;
		m_DrawCtrl.AddDrawUnit(nFormatIDDot, tArrowGroup);
	}
	
	tLine.P1.x = 0;
	tLine.P1.y = dTopY + m_dDimLenLong - m_dDimLenTailGap;
	tLine.P2.x = m_ShapeMgr.m_dCopyingWidth;
	tLine.P2.y = dTopY + m_dDimLenLong - m_dDimLenTailGap;
	tLine.P1.y += m_dDimFrontGap;
	tLine.P2.y += m_dDimFrontGap;
	
	if (m_bTrans)
	{
		ConvtoTransScale(tLine.P1.x, tLine.P1.y, m_dCopingCenterX, m_dCopingCenterY, tModLine.P1.x, tModLine.P1.y);
		ConvtoTransScale(tLine.P2.x, tLine.P2.y, m_dCopingCenterX, m_dCopingCenterY, tModLine.P2.x, tModLine.P2.y);
	
		tLine = tModLine;
	
		tTextGroup.List[0].PPosition.x = (tLine.P1.x + tLine.P2.x) * 0.5 - m_dDimLendTxtGap;
		tTextGroup.List[0].PPosition.y = (tLine.P1.y + tLine.P2.y) * 0.5;
	}
	else
	{
		tTextGroup.List[0].PPosition.x = (tLine.P1.x + tLine.P2.x) * 0.5;
		tTextGroup.List[0].PPosition.y = (tLine.P1.y + tLine.P2.y) * 0.5 + m_dDimLendTxtGap;
	}
	
	tArrowGroup.List[0] = tLine;
	m_DrawCtrl.AddDrawUnit(nFormatIDArrow, tArrowGroup);
	
	tTextGroup.List[0].szText = _LSX(B);
	m_DrawCtrl.AddDrawUnit(nFormatIDTxt, tTextGroup);
	
	
	// D1
	tLine.P1.x = 0;
	tLine.P1.y = dTopY + m_dDimLenShort - m_dDimLenTailGap;
	tLine.P2.x = aXPos[1];
	tLine.P2.y = dTopY + m_dDimLenShort - m_dDimLenTailGap;
	tLine.P1.y += m_dDimFrontGap;
	tLine.P2.y += m_dDimFrontGap;
	if (m_bTrans)
	{
		ConvtoTransScale(tLine.P1.x, tLine.P1.y, m_dCopingCenterX, m_dCopingCenterY, tModLine.P1.x, tModLine.P1.y);
		ConvtoTransScale(tLine.P2.x, tLine.P2.y, m_dCopingCenterX, m_dCopingCenterY, tModLine.P2.x, tModLine.P2.y);
	
		tLine = tModLine;
	
		tTextGroup.List[0].PPosition.x = (tLine.P1.x + tLine.P2.x) * 0.5 - m_dDimLendTxtGap;
		tTextGroup.List[0].PPosition.y = (tLine.P1.y + tLine.P2.y) * 0.5;
	}
	else
	{
		tTextGroup.List[0].PPosition.x = (tLine.P1.x + tLine.P2.x) * 0.5 ;
		tTextGroup.List[0].PPosition.y = (tLine.P1.y + tLine.P2.y) * 0.5 + m_dDimLendTxtGap;
	}
	
	tArrowGroup.List[0] = tLine;
	m_DrawCtrl.AddDrawUnit(nFormatIDArrow, tArrowGroup);
	
	tTextGroup.List[0].szText = _LSX(D1);
	m_DrawCtrl.AddDrawUnit(nFormatIDTxt, tTextGroup);
	
	// Sax
	int nSize;
	nSize = m_aMaxSax.GetSize() + 1; //m_Data.aLayer[m_Data.aLayer.GetSize() - 1].aSax.GetSize() + 1;
	for (int i = 0; i < nSize; i++)
	{
		tLine.P1.x = aXPos[i + 1];
		tLine.P1.y = dTopY;
		tLine.P2.x = aXPos[i + 1];
		tLine.P2.y = dTopY + m_dDimLenShort;
		tLine.P1.y += m_dDimFrontGap;
		tLine.P2.y += m_dDimFrontGap;
	
		if (m_bTrans)
		{
			ConvtoTransScale(tLine.P1.x, tLine.P1.y, m_dCopingCenterX, m_dCopingCenterY, tModLine.P1.x, tModLine.P1.y);
			ConvtoTransScale(tLine.P2.x, tLine.P2.y, m_dCopingCenterX, m_dCopingCenterY, tModLine.P2.x, tModLine.P2.y);
	
			tLine = tModLine;
		}
	
		tArrowGroup.List[0] = tLine;
		m_DrawCtrl.AddDrawUnit(nFormatIDLine, tArrowGroup);
	
		if (i < nSize - 1)
		{
			tLine.P1.x = aXPos[i + 1];
			tLine.P1.y = dTopY + m_dDimLenShort - m_dDimLenTailGap;
			tLine.P2.x = aXPos[i + 2];
			tLine.P2.y = dTopY + m_dDimLenShort - m_dDimLenTailGap;
			tLine.P1.y += m_dDimFrontGap;
			tLine.P2.y += m_dDimFrontGap;
	
			if (m_bTrans)
			{
				ConvtoTransScale(tLine.P1.x, tLine.P1.y, m_dCopingCenterX, m_dCopingCenterY, tModLine.P1.x, tModLine.P1.y);
				ConvtoTransScale(tLine.P2.x, tLine.P2.y, m_dCopingCenterX, m_dCopingCenterY, tModLine.P2.x, tModLine.P2.y);
	
				tLine = tModLine;
	
				tTextGroup.List[0].PPosition.x = (tLine.P1.x + tLine.P2.x) * 0.5 - m_dDimLendTxtGap;
				tTextGroup.List[0].PPosition.y = (tLine.P1.y + tLine.P2.y) * 0.5 ;
			}
			else
			{
				tTextGroup.List[0].PPosition.x = (tLine.P1.x + tLine.P2.x) * 0.5;
				tTextGroup.List[0].PPosition.y = (tLine.P1.y + tLine.P2.y) * 0.5 + m_dDimLendTxtGap;
			}
	
			tArrowGroup.List[0] = tLine;
			m_DrawCtrl.AddDrawUnit(nFormatIDArrow, tArrowGroup);

			if(m_aMaxSax.GetSize() > 1)
			{
				tTextGroup.List[0].szText = _LSX(Sax);
			}
			m_DrawCtrl.AddDrawUnit(nFormatIDTxt, tTextGroup);
		}
	}
	
	// D'1
	tLine.P1.x = aXPos[aXPos.size() - 2];
	tLine.P1.y = dTopY + m_dDimLenShort - m_dDimLenTailGap;
	tLine.P2.x = m_ShapeMgr.m_dCopyingWidth;
	tLine.P2.y = dTopY + m_dDimLenShort - m_dDimLenTailGap;
	tLine.P1.y += m_dDimFrontGap;
	tLine.P2.y += m_dDimFrontGap;
	
	if (m_bTrans)
	{
		ConvtoTransScale(tLine.P1.x, tLine.P1.y, m_dCopingCenterX, m_dCopingCenterY, tModLine.P1.x, tModLine.P1.y);
		ConvtoTransScale(tLine.P2.x, tLine.P2.y, m_dCopingCenterX, m_dCopingCenterY, tModLine.P2.x, tModLine.P2.y);
	
		tLine = tModLine;
	
		tTextGroup.List[0].PPosition.x = (tLine.P1.x + tLine.P2.x) * 0.5 - m_dDimLendTxtGap;
		tTextGroup.List[0].PPosition.y = (tLine.P1.y + tLine.P2.y) * 0.5;
	}
	else
	{
		tTextGroup.List[0].PPosition.x = (tLine.P1.x + tLine.P2.x) * 0.5;
		tTextGroup.List[0].PPosition.y = (tLine.P1.y + tLine.P2.y) * 0.5 + m_dDimLendTxtGap;
	}
	
	
	tArrowGroup.List[0] = tLine;
	m_DrawCtrl.AddDrawUnit(nFormatIDArrow, tArrowGroup);
	
	tTextGroup.List[0].szText = _T("D'1");
	m_DrawCtrl.AddDrawUnit(nFormatIDTxt, tTextGroup);


	// Y 방향(L, D2, Say, D'2)
	// L	
	double dMinXConsiderSkew;
	dMinXConsiderSkew = DBL_MAX;
	
	nSize = aX.GetSize();
	for (int i = 0; i < nSize; i++)
	{
		if (dMinXConsiderSkew > aX[i])
		{
			dMinXConsiderSkew = aX[i];
		}
	}
	
	tLine.P1.x = 0.0;
	tLine.P1.y = dBotY;
	tLine.P2.x = -m_dDimLenLong;
	tLine.P2.y = dBotY;
	tLine.P1.x += -m_dDimFrontGap + dMinXConsiderSkew;
	tLine.P2.x += -m_dDimFrontGap + dMinXConsiderSkew;
	
	if (m_bTrans)
	{
		ConvtoTransScale(tLine.P1.x, tLine.P1.y, m_dCopingCenterX, m_dCopingCenterY, tModLine.P1.x, tModLine.P1.y);
		ConvtoTransScale(tLine.P2.x, tLine.P2.y, m_dCopingCenterX, m_dCopingCenterY, tModLine.P2.x, tModLine.P2.y);
		
		tLine = tModLine;			
	}
	
	tArrowGroup.List[0] = tLine;
	m_DrawCtrl.AddDrawUnit(nFormatIDLine, tArrowGroup);
	
	tLine.P1.x = 0.0;
	tLine.P1.y = dTopY;
	tLine.P2.x = -m_dDimLenLong;
	tLine.P2.y = dTopY;
	tLine.P1.x += -m_dDimFrontGap + dMinXConsiderSkew;
	tLine.P2.x += -m_dDimFrontGap + dMinXConsiderSkew;
	
	if (m_bTrans)
	{
		ConvtoTransScale(tLine.P1.x, tLine.P1.y, m_dCopingCenterX, m_dCopingCenterY, tModLine.P1.x, tModLine.P1.y);
		ConvtoTransScale(tLine.P2.x, tLine.P2.y, m_dCopingCenterX, m_dCopingCenterY, tModLine.P2.x, tModLine.P2.y);
	
		tLine = tModLine;
	}
	
	tArrowGroup.List[0] = tLine;
	m_DrawCtrl.AddDrawUnit(nFormatIDLine, tArrowGroup);
	
	tLine.P1.x = -m_dDimLenLong + m_dDimLenTailGap;
	tLine.P1.y = dBotY;
	tLine.P2.x = -m_dDimLenLong + m_dDimLenTailGap;
	tLine.P2.y = dTopY;
	tLine.P1.x += -m_dDimFrontGap + dMinXConsiderSkew;
	tLine.P2.x += -m_dDimFrontGap + dMinXConsiderSkew;
	
	if (m_bTrans)
	{
		ConvtoTransScale(tLine.P1.x, tLine.P1.y, m_dCopingCenterX, m_dCopingCenterY, tModLine.P1.x, tModLine.P1.y);
		ConvtoTransScale(tLine.P2.x, tLine.P2.y, m_dCopingCenterX, m_dCopingCenterY, tModLine.P2.x, tModLine.P2.y);
	
		tLine = tModLine;
	
		tTextGroup.List[0].PPosition.x = (tLine.P1.x + tLine.P2.x) * 0.5;
		tTextGroup.List[0].PPosition.y = (tLine.P1.y + tLine.P2.y) * 0.5 - m_dDimLendTxtGap;
	}
	else
	{
		tTextGroup.List[0].PPosition.x = (tLine.P1.x + tLine.P2.x) * 0.5 - m_dDimLendTxtGap;
		tTextGroup.List[0].PPosition.y = (tLine.P1.y + tLine.P2.y) * 0.5;
	}
	
	tArrowGroup.List[0] = tLine;
	m_DrawCtrl.AddDrawUnit(nFormatIDArrow, tArrowGroup);
	
	tTextGroup.List[0].szText = _LSX(H);
	m_DrawCtrl.AddDrawUnit(nFormatIDTxtY, tTextGroup);


	// D2
	tLine.P1.x = -m_dDimLenShort + m_dDimLenTailGap;
	tLine.P1.y = dTopY;
	tLine.P2.x = -m_dDimLenShort + m_dDimLenTailGap;
	tLine.P2.y = tLine.P1.y - aYPos[1];
	tLine.P1.x += -m_dDimFrontGap + dMinXConsiderSkew;
	tLine.P2.x += -m_dDimFrontGap + dMinXConsiderSkew;
	
	if (m_bTrans)
	{
		ConvtoTransScale(tLine.P1.x, tLine.P1.y, m_dCopingCenterX, m_dCopingCenterY, tModLine.P1.x, tModLine.P1.y);
		ConvtoTransScale(tLine.P2.x, tLine.P2.y, m_dCopingCenterX, m_dCopingCenterY, tModLine.P2.x, tModLine.P2.y);
	
		tLine = tModLine;
	
		tTextGroup.List[0].PPosition.x = (tLine.P1.x + tLine.P2.x) * 0.5 ;
		tTextGroup.List[0].PPosition.y = (tLine.P1.y + tLine.P2.y) * 0.5 - m_dDimLendTxtGap;
	}
	else
	{
		tTextGroup.List[0].PPosition.x = (tLine.P1.x + tLine.P2.x) * 0.5 - m_dDimLendTxtGap;
		tTextGroup.List[0].PPosition.y = (tLine.P1.y + tLine.P2.y) * 0.5;
	}
	
	tArrowGroup.List[0] = tLine;
	m_DrawCtrl.AddDrawUnit(nFormatIDArrow, tArrowGroup);
	
	tTextGroup.List[0].szText = _T("D'2");
	m_DrawCtrl.AddDrawUnit(nFormatIDTxtY, tTextGroup);
	
	// Say
	nSize = m_Data.aLayer.GetSize();
	for (int i = 0; i < nSize; i++)
	{	
		tLine.P1.x = 0.0;
		tLine.P1.y = dTopY - aYPos[i + 1];
		tLine.P2.x = -m_dDimLenShort;
		tLine.P2.y = tLine.P1.y;
		tLine.P1.x += -m_dDimFrontGap + dMinXConsiderSkew;
		tLine.P2.x += -m_dDimFrontGap + dMinXConsiderSkew;
	
		if (m_bTrans)
		{
			ConvtoTransScale(tLine.P1.x, tLine.P1.y, m_dCopingCenterX, m_dCopingCenterY, tModLine.P1.x, tModLine.P1.y);
			ConvtoTransScale(tLine.P2.x, tLine.P2.y, m_dCopingCenterX, m_dCopingCenterY, tModLine.P2.x, tModLine.P2.y);
	
			tLine = tModLine;
		}
	
		tArrowGroup.List[0] = tLine;
		m_DrawCtrl.AddDrawUnit(nFormatIDLine, tArrowGroup);
	
		if (i < nSize - 1)
		{
	
			tLine.P1.x = -m_dDimLenShort + m_dDimLenTailGap;
			tLine.P1.y = dTopY - aYPos[i + 1];
			tLine.P2.x = -m_dDimLenShort + m_dDimLenTailGap;
			tLine.P2.y = dTopY - aYPos[i + 2];
			tLine.P1.x += -m_dDimFrontGap + dMinXConsiderSkew;
			tLine.P2.x += -m_dDimFrontGap + dMinXConsiderSkew;
	
			if (m_bTrans)
			{
				ConvtoTransScale(tLine.P1.x, tLine.P1.y, m_dCopingCenterX, m_dCopingCenterY, tModLine.P1.x, tModLine.P1.y);
				ConvtoTransScale(tLine.P2.x, tLine.P2.y, m_dCopingCenterX, m_dCopingCenterY, tModLine.P2.x, tModLine.P2.y);
	
				tLine = tModLine;
	
				tTextGroup.List[0].PPosition.x = (tLine.P1.x + tLine.P2.x) * 0.5;
				tTextGroup.List[0].PPosition.y = (tLine.P1.y + tLine.P2.y) * 0.5 - m_dDimLendTxtGap;
			}
			else
			{
				tTextGroup.List[0].PPosition.x = (tLine.P1.x + tLine.P2.x) * 0.5 - m_dDimLendTxtGap;
				tTextGroup.List[0].PPosition.y = (tLine.P1.y + tLine.P2.y) * 0.5;
			}
	
			tArrowGroup.List[0] = tLine;
			m_DrawCtrl.AddDrawUnit(nFormatIDArrow, tArrowGroup);
	
	
			tTextGroup.List[0].szText = _LSX(Say);
			m_DrawCtrl.AddDrawUnit(nFormatIDTxtY, tTextGroup);
		}
	}
	
	// D'2
	tLine.P1.x = -m_dDimLenShort + m_dDimLenTailGap;
	tLine.P1.y = dTopY - aYPos[aYPos.size()-1];
	tLine.P2.x = -m_dDimLenShort + m_dDimLenTailGap;
	tLine.P2.y = dBotY;
	tLine.P1.x += -m_dDimFrontGap + dMinXConsiderSkew;
	tLine.P2.x += -m_dDimFrontGap + dMinXConsiderSkew;
	
	if (m_bTrans)
	{
		ConvtoTransScale(tLine.P1.x, tLine.P1.y, m_dCopingCenterX, m_dCopingCenterY, tModLine.P1.x, tModLine.P1.y);
		ConvtoTransScale(tLine.P2.x, tLine.P2.y, m_dCopingCenterX, m_dCopingCenterY, tModLine.P2.x, tModLine.P2.y);
	
		tLine = tModLine;

		tTextGroup.List[0].PPosition.x = (tLine.P1.x + tLine.P2.x) * 0.5;
		tTextGroup.List[0].PPosition.y = (tLine.P1.y + tLine.P2.y) * 0.5 - m_dDimLendTxtGap;
	}
	else
	{
		tTextGroup.List[0].PPosition.x = (tLine.P1.x + tLine.P2.x) * 0.5 - m_dDimLendTxtGap;
		tTextGroup.List[0].PPosition.y = (tLine.P1.y + tLine.P2.y) * 0.5;
				
	}
	
	tArrowGroup.List[0] = tLine;
	m_DrawCtrl.AddDrawUnit(nFormatIDArrow, tArrowGroup);
	
	tTextGroup.List[0].szText = _LSX(D2);;
	m_DrawCtrl.AddDrawUnit(nFormatIDTxtY, tTextGroup);


	// Direction Alert
	// 1. 교축직각방향
	tLine.P1.x = 0;
	tLine.P1.y = dBotY - (m_dDimLenLong - m_dDimLenTailGap);
	tLine.P2.x = m_ShapeMgr.m_dCopyingWidth*0.18;
	tLine.P2.y = dBotY - (m_dDimLenLong - m_dDimLenTailGap);

	if (m_bTrans)
	{
		tTextGroup.List[0].PPosition.x = (tLine.P1.x + tLine.P2.x) * 0.5;
		tTextGroup.List[0].PPosition.y = (tLine.P1.y + tLine.P2.y) * 0.5 + m_dDimLendTxtGap;

		ConvtoTransScale(tLine.P1.x, tLine.P1.y, m_dCopingCenterX, m_dCopingCenterY, tModLine.P1.x, tModLine.P1.y);
		ConvtoTransScale(tLine.P2.x, tLine.P2.y, m_dCopingCenterX, m_dCopingCenterY, tModLine.P2.x, tModLine.P2.y);

		tLine = tModLine;

		ConvtoTransScale(tTextGroup.List[0].PPosition.x, tTextGroup.List[0].PPosition.y, m_dCopingCenterX, m_dCopingCenterY, tTextModGroup.List[0].PPosition.x, tTextModGroup.List[0].PPosition.y);
		tTextGroup = tTextModGroup;
	}
	else
	{
		tTextGroup.List[0].PPosition.x = (tLine.P1.x + tLine.P2.x) * 0.5;
		tTextGroup.List[0].PPosition.y = (tLine.P1.y + tLine.P2.y) * 0.5 + m_dDimLendTxtGap;
	}

	tArrowGroup.List[0] = tLine;
	if (m_bDrawDir) m_DrawCtrl.AddDrawUnit(nFormatIDDirec, tArrowGroup);

	TextFormat.nEscapement = (900- m_Data.dCopingAngle * 10);
	m_DrawCtrl.m_arTextFormat.SetAt(++m_nLastFormatID, TextFormat);
	nFormatIDTxtDirec = m_nLastFormatID;
	tTextGroup.List[0].szText = _LSX(교축직각방향);
	if (m_bDrawDir) m_DrawCtrl.AddDrawUnit(nFormatIDTxtDirec, tTextGroup);

	// 2. 교축방향
	tLine.P1.x = m_ShapeMgr.m_dCopyingWidth + m_dDimLenTailGap;
	tLine.P1.y = dBotY;
	tLine.P2.x = m_ShapeMgr.m_dCopyingWidth + m_dDimLenTailGap;
	tLine.P2.y = dBotY + m_ShapeMgr.m_dCopyingHeight*0.4;
	tLine.P1.x -= -m_dDimFrontGap + dMinXConsiderSkew;
	tLine.P2.x -= -m_dDimFrontGap + dMinXConsiderSkew;

	if (m_bTrans)
	{
		tTextGroup.List[0].PPosition.x = (tLine.P1.x + tLine.P2.x) * 0.5; 
		tTextGroup.List[0].PPosition.y = tLine.P1.y - (m_dDimLendTxtGap + 100);

		ConvtoTransScale(tLine.P1.x, tLine.P1.y, m_dCopingCenterX, m_dCopingCenterY, tModLine.P1.x, tModLine.P1.y);
		ConvtoTransScale(tLine.P2.x, tLine.P2.y, m_dCopingCenterX, m_dCopingCenterY, tModLine.P2.x, tModLine.P2.y);

		tLine = tModLine;

		ConvtoTransScale(tTextGroup.List[0].PPosition.x, tTextGroup.List[0].PPosition.y, m_dCopingCenterX, m_dCopingCenterY, tTextModGroup.List[0].PPosition.x, tTextModGroup.List[0].PPosition.y);
		tTextGroup = tTextModGroup;
	}
	else
	{
		tTextGroup.List[0].PPosition.x = (tLine.P1.x + tLine.P2.x) * 0.5;
		tTextGroup.List[0].PPosition.y = tLine.P1.y - (m_dDimLendTxtGap + 100);
	}

	tArrowGroup.List[0] = tLine;
	if (m_bDrawDir) m_DrawCtrl.AddDrawUnit(nFormatIDDirec, tArrowGroup);

	tTextGroup.List[0].szText = _LSX(교축방향);
	if (m_bDrawDir) m_DrawCtrl.AddDrawUnit(nFormatIDTxtDirec, tTextGroup);
}

void CDgnSeisAnchorEvalEditBtn::MakeDrawBasicDim(int nFormatIDLine, int nFormatIDArrow, int nFormatIDTxt, T_POINT2D Line[2], double dGap, CString& strVal, BOOL bRight)
{
	double Dir[2], Dir90[2];
	m_ShapeMgr.GetDir(Line[0].x, Line[0].y, Line[1].x, Line[1].y, Dir[0], Dir[1]);
	if (bRight)
	{
		// 방향을 90도 돌림	
		Dir90[0] = Dir[1];
		Dir90[1] = -Dir[0];
	}
	else
	{
		// 방향을 -90도 돌림	
		Dir90[0] = -Dir[1];
		Dir90[1] = Dir[0];
	}


	T_ARROWLINE_R_GROUP tArrowGroup;
	tArrowGroup.List.SetSize(1);
	T_LINE_R tLine, tModLine;
	T_POINT tPnt, tModPnt;

	T_POINT2D GapLine[2];
	GapLine[0] = Line[0];
	GapLine[1] = Line[1];

	m_ShapeMgr.PntMove(Dir90[0], Dir90[1], dGap, GapLine[0].x, GapLine[0].y);
	m_ShapeMgr.PntMove(Dir90[0], Dir90[1], dGap, GapLine[1].x, GapLine[1].y);

	CString strTmp;
	tLine.P1.x = GapLine[0].x;
	tLine.P1.y = GapLine[0].y;
	tLine.P2.x = GapLine[0].x;
	tLine.P2.y = GapLine[0].y;
	m_ShapeMgr.PntMove(Dir90[0], Dir90[1], m_dDimLenShort + m_dDimOverlap, tLine.P2.x, tLine.P2.y);

	if (m_bTrans)
	{
		ConvtoTransScale(tLine.P1.x, tLine.P1.y, m_dCopingCenterX, m_dCopingCenterY, tModLine.P1.x, tModLine.P1.y);
		ConvtoTransScale(tLine.P2.x, tLine.P2.y, m_dCopingCenterX, m_dCopingCenterY, tModLine.P2.x, tModLine.P2.y);

		tArrowGroup.List[0] = tModLine;
	}
	else
	{
		tArrowGroup.List[0] = tLine;
	}

	m_DrawCtrl.AddDrawUnit(nFormatIDLine, tArrowGroup);

	tLine.P1.x = GapLine[1].x;
	tLine.P1.y = GapLine[1].y;
	tLine.P2.x = GapLine[1].x;
	tLine.P2.y = GapLine[1].y;
	m_ShapeMgr.PntMove(Dir90[0], Dir90[1], m_dDimLenShort+m_dDimOverlap, tLine.P2.x, tLine.P2.y);

	if (m_bTrans)
	{
		ConvtoTransScale(tLine.P1.x, tLine.P1.y, m_dCopingCenterX, m_dCopingCenterY, tModLine.P1.x, tModLine.P1.y);
		ConvtoTransScale(tLine.P2.x, tLine.P2.y, m_dCopingCenterX, m_dCopingCenterY, tModLine.P2.x, tModLine.P2.y);

		tArrowGroup.List[0] = tModLine;
	}
	else
	{
		tArrowGroup.List[0] = tLine;
	}

	m_DrawCtrl.AddDrawUnit(nFormatIDLine, tArrowGroup);

	tLine.P1.x = GapLine[0].x;
	tLine.P1.y = GapLine[0].y;
	tLine.P2.x = GapLine[1].x;
	tLine.P2.y = GapLine[1].y;
	m_ShapeMgr.PntMove(Dir90[0], Dir90[1], m_dDimLenShort - m_dDimLenTailGap + m_dDimOverlap, tLine.P1.x, tLine.P1.y);
	m_ShapeMgr.PntMove(Dir90[0], Dir90[1], m_dDimLenShort - m_dDimLenTailGap + m_dDimOverlap, tLine.P2.x, tLine.P2.y);

	if (m_bTrans)
	{
		ConvtoTransScale(tLine.P1.x, tLine.P1.y, m_dCopingCenterX, m_dCopingCenterY, tModLine.P1.x, tModLine.P1.y);
		ConvtoTransScale(tLine.P2.x, tLine.P2.y, m_dCopingCenterX, m_dCopingCenterY, tModLine.P2.x, tModLine.P2.y);

		tArrowGroup.List[0] = tModLine;
	}
	else
	{
		tArrowGroup.List[0] = tLine;
	}

	m_DrawCtrl.AddDrawUnit(nFormatIDArrow, tArrowGroup);

	T_TEXT_R_GROUP tTextGroup;
	tTextGroup.List.SetSize(1);
	tPnt.x = (tLine.P1.x + tLine.P2.x) * 0.5;
	tPnt.y = (tLine.P1.y + tLine.P2.y) * 0.5;
	m_ShapeMgr.PntMove(Dir90[0], Dir90[1], m_dDimLendTxtGap, tPnt.x, tPnt.y);

	if (m_bTrans)
	{
		ConvtoTransScale(tPnt.x, tPnt.y, m_dCopingCenterX, m_dCopingCenterY, tModPnt.x, tModPnt.y);

		tTextGroup.List[0].PPosition = tModPnt;
	}
	else
	{
		tTextGroup.List[0].PPosition = tPnt;
	}

	tTextGroup.List[0].szText = strVal;
	m_DrawCtrl.AddDrawUnit(nFormatIDTxt, tTextGroup);
	/*
	// 테스트용 선
	T_DRAW_FORMAT	DrawFormat;
	DrawFormat.bLine  = TRUE;
	//DrawFormat.bFill  = FALSE;
	DrawFormat.lineColor = RGB(255, 0, 0);
	//DrawFormat.fillColor = RGB(118,   0,   118);
	DrawFormat.startArrowType = EN_ARROW_TYPE_NONE;
	DrawFormat.endArrowType = EN_ARROW_TYPE_NONE;
	DrawFormat.Width  = 1;
	m_DrawCtrl.m_arDrawFormat.SetAt(++m_nLastFormatID, DrawFormat);
	tLine.P1.x = Line[0].x;
	tLine.P1.y = Line[0].y;
	tLine.P2.x = Line[1].x;
	tLine.P2.y = Line[1].y;
	tArrowGroup.List[0] = tLine;
	m_DrawCtrl.AddDrawUnit(m_nLastFormatID, tArrowGroup);
	*/
}

BOOL CDgnSeisAnchorEvalEditBtn::IsCa1DimCopingSize()
{
	if (m_nDir == CDgnSeisAnchorShapeMgr::DIR_LONGI)
	{
		return fabs(m_Data.dTheta1 - m_Data.dTheta_q2) < m_dZero;
	}
	else if (m_nDir == CDgnSeisAnchorShapeMgr::DIR_TRANS)
	{
		return FALSE;
		//return fabs(m_Data.dTheta_q2) < m_dZero;
	}
	else
	{
		ASSERT(0);
		return TRUE;
	}
}

void CDgnSeisAnchorEvalEditBtn::MakeDrawSideAnchorAndDim(int nCa1BearingIIdx, int nCa1BearingJIdx, int nCa1AnchorIIdx, int nCa1AnchorJIdx, double dCa1Recal)
{
	double PosCa1Dim[2], PosCa1DimB[2], PosCa1DimL[2], Dir[2];

	double dBearingThickness = ConvWndPosToCoord(5);

	MakeDrawSideAnchor(CDgnSeisAnchorShapeMgr::DIR_LONGI, nCa1BearingIIdx, nCa1BearingJIdx, nCa1AnchorIIdx, nCa1AnchorJIdx, dBearingThickness, PosCa1DimB);
	MakeDrawSideAnchor(CDgnSeisAnchorShapeMgr::DIR_TRANS, nCa1BearingIIdx, nCa1BearingJIdx, nCa1AnchorIIdx, nCa1AnchorJIdx, dBearingThickness, PosCa1DimL);
	if (m_nDir == CDgnSeisAnchorShapeMgr::DIR_LONGI)
	{
		PosCa1Dim[0] = PosCa1DimB[0];
		PosCa1Dim[1] = PosCa1DimB[1];
	}
	else
	{
		PosCa1Dim[0] = PosCa1DimL[0];
		PosCa1Dim[1] = PosCa1DimL[1];
	}

	// 치수선 그리기
	int nFormatIDLine, nFormatIDArrow, nFormatIDTxt;

	T_DRAW_FORMAT	DrawFormat;
	DrawFormat.bLine = TRUE;
	//DrawFormat.bFill  = FALSE;		  
	DrawFormat.lineColor = m_ColorDimensionLine;
	//DrawFormat.fillColor = RGB(118,   0,   118);
	DrawFormat.startArrowType = EN_ARROW_TYPE_NONE;
	DrawFormat.endArrowType = EN_ARROW_TYPE_NONE;
	DrawFormat.Width = 1;
	m_DrawCtrl.m_arDrawFormat.SetAt(++m_nLastFormatID, DrawFormat);
	nFormatIDLine = m_nLastFormatID;

	DrawFormat.startArrowType = EN_ARROW_TYPE_ARROW;
	DrawFormat.endArrowType = EN_ARROW_TYPE_ARROW;
	DrawFormat.dRWidth = 3;
	DrawFormat.dRHeight = 2;
	m_DrawCtrl.m_arDrawFormat.SetAt(++m_nLastFormatID, DrawFormat);
	nFormatIDArrow = m_nLastFormatID;

	T_TEXT_FORMAT	TextFormat;
	T_TEXT_R_GROUP tTextGroup;
	tTextGroup.List.SetSize(1);

	TextFormat.Size = 13;
	TextFormat.fontColor = RGB(0, 0, 0);
	TextFormat.positionType = EN_POSITION_TYPE_CENTER_VCENTER;

	T_POINT2D Line[2];


	CString strTmp;
	double dLength;
	BOOL bRight;

	if (m_nDir == CDgnSeisAnchorShapeMgr::DIR_LONGI)
	{
		if (m_nDrawType == 1) //Con파괴, Ca1
		{
			if (m_nDownUp == 0 && m_nLeftRight == 0)
				Line[0] = m_SideCopingLineForBot[0];
			else if (m_nDownUp == 1 && m_nLeftRight == 0)
				Line[0] = m_SideCopingLineForBot[1];
			else if (m_nDownUp == 0 && m_nLeftRight == 1)
				Line[0] = m_SideCopingLineForBot[1];
			else if (m_nDownUp == 1 && m_nLeftRight == 1)
				Line[0] = m_SideCopingLineForBot[0];
			else
			{
				Line[0] = m_SideCopingLineForBot[0];
				ASSERT(0);
			}
		}
		else
		{
			Line[0] = m_SideCopingLineForBot[0];
		}

	}
	else
	{
		Line[0] = m_SideCopingLineForLeft[0];
	}
	Line[1].x = PosCa1Dim[0];
	Line[1].y = PosCa1Dim[1];
	if (IsCa1DimCopingSize())
	{
		dLength = math_lib::distance_2d(Line[0].x, Line[0].y, Line[1].x, Line[1].y);
		if (m_bUser)
		{
			strTmp.Format(_T("Ca1"));
		}
		else
		{
			if(m_nDrawType == 1)
				strTmp.Format(_T("%g"), SetFloor(CMathFunc::mathRoundOff(dCa1Recal, 7)));
			else
				strTmp.Format(_T("%g"), SetFloor(CMathFunc::mathRoundOff(dLength, 7)));
		}	
		if(m_nDir == CDgnSeisAnchorShapeMgr::DIR_LONGI)
		{
			if (m_bTrans) TextFormat.nEscapement = 900 + (int)((-m_Data.dTheta1 - 90) * 10.0);
			else          TextFormat.nEscapement = (int)((-m_Data.dTheta1 - 90) * 10.0);

			bRight = TRUE;
		}
		else if (m_nDir == CDgnSeisAnchorShapeMgr::DIR_TRANS)
		{
			if (m_bTrans) TextFormat.nEscapement = -900;
			else          TextFormat.nEscapement = 0;

			bRight = FALSE;
		}
		if (TextFormat.nEscapement == 900 || TextFormat.nEscapement == -900)
		{
			TextFormat.Size = 12;
		}
		else
		{
			TextFormat.Size = 13;
		}
		m_DrawCtrl.m_arTextFormat.SetAt(++m_nLastFormatID, TextFormat);
		nFormatIDTxt = m_nLastFormatID;
		MakeDrawBasicDim(nFormatIDLine, nFormatIDArrow, nFormatIDTxt, Line, m_dDimFrontGap * 0.5 + dBearingThickness, strTmp, bRight);
	}

	double dTmp;
	m_ShapeMgr.GetDir(Line[0].x, Line[0].y, Line[1].x, Line[1].y, Dir[0], Dir[1]);
	if (m_nDir == CDgnSeisAnchorShapeMgr::DIR_LONGI)
	{	
		if (m_nLeftRight == CDgnSeisAnchorShapeMgr::DIR_POS)
		{
			if (Dir[1] >= 0)
			{
				// -90도 돌림
				dTmp = Dir[0];
				Dir[0] = -Dir[1];
				Dir[1] = dTmp;
			}
			else if (Dir[1] < 0)
			{
				// 90도 돌림
				dTmp = Dir[0];
				Dir[0] = Dir[1];
				Dir[1] = -dTmp;
			}
			else
			{
				ASSERT(0);
			}
		}
		else if (m_nLeftRight == CDgnSeisAnchorShapeMgr::DIR_NEG)
		{
			if (Dir[1] >= 0)
			{
				// 90도 돌림
				dTmp = Dir[0];
				Dir[0] = Dir[1];
				Dir[1] = -dTmp;				
			}
			else if (Dir[1] < 0)
			{
				// -90도 돌림
				dTmp = Dir[0];
				Dir[0] = -Dir[1];
				Dir[1] = dTmp;
			}
			else
			{
				ASSERT(0);
			}
		}
	}
	else if (m_nDir == CDgnSeisAnchorShapeMgr::DIR_TRANS)
	{
		// 90도 돌림
		dTmp = Dir[0];
		Dir[0] = Dir[1];
		Dir[1] = -dTmp;
	}

	TConcFailBcCalc* pConcFailBc = &(m_nDir == CDgnSeisAnchorShapeMgr::DIR_LONGI ? m_LongiConcFailBc : m_TransConcFailBc);
	double dCa1, dCa1P, dCa2, dCa2P;
	T_POINT2D Ca1Pos[2], Ca1PPos[2], Ca2Pos[2], Ca2PPos[2];
	if (m_nDrawType == 1)
	{
		m_ShapeMgr.GetConcFailCa(m_nDir, dCa1, Ca1Pos[0].x, Ca1Pos[0].y, Ca1Pos[1].x, Ca1Pos[1].y,
			nCa1BearingIIdx, nCa1BearingJIdx, nCa1AnchorIIdx, nCa1AnchorJIdx,
			dCa1P, Ca1PPos[0].x, Ca1PPos[0].y, Ca1PPos[1].x, Ca1PPos[1].y,
			dCa2, Ca2Pos[0].x, Ca2Pos[0].y, Ca2Pos[1].x, Ca2Pos[1].y,
			dCa2P, Ca2PPos[0].x, Ca2PPos[0].y, Ca2PPos[1].x, Ca2PPos[1].y,
			pConcFailBc->aBearingIdx);
	}
	else if (m_nDrawType == 2)
	{
		m_ShapeMgr.GetPryoutCa(m_nDir, dCa1, Ca1Pos[0].x, Ca1Pos[0].y, Ca1Pos[1].x, Ca1Pos[1].y,
			nCa1BearingIIdx, nCa1BearingJIdx, nCa1AnchorIIdx, nCa1AnchorJIdx,
			dCa1P, Ca1PPos[0].x, Ca1PPos[0].y, Ca1PPos[1].x, Ca1PPos[1].y,
			dCa2, Ca2Pos[0].x, Ca2Pos[0].y, Ca2Pos[1].x, Ca2Pos[1].y,
			dCa2P, Ca2PPos[0].x, Ca2PPos[0].y, Ca2PPos[1].x, Ca2PPos[1].y);
	}
	else
	{
		ASSERT(0);
		return;
	}

	Line[1] = Line[0];
	m_ShapeMgr.PntMove(Dir[0], Dir[1], m_dHa, Line[1].x, Line[1].y);
	if (m_bUser)
	{
		if (m_nDrawType == 1)
		{
			strTmp = _T("Ha");
		}
		else if (m_nDrawType == 2)
		{
			strTmp = _T("Hcop");
		}
		else
		{
			ASSERT(0);
			strTmp = _T("Hcop");
		}
	}
	else
	{
		strTmp.Format(_T("%g"), SetFloor(CMathFunc::mathRoundOff(m_dHa, 7)));
	}

	if (m_nDir == CDgnSeisAnchorShapeMgr::DIR_LONGI)
	{
		if (m_bTrans) TextFormat.nEscapement = 900 + (int)((-m_Data.dTheta1) * 10.0);
		else          TextFormat.nEscapement = (int)((-m_Data.dTheta1) * 10.0);

		if (m_nLeftRight == 0)
		{
			bRight = (m_nDownUp == 0) ? FALSE : TRUE;
		}
		else if (m_nLeftRight == 1)
		{
			bRight = (m_nDownUp == 1) ? FALSE : TRUE;
		}
		
	}
	else if (m_nDir == CDgnSeisAnchorShapeMgr::DIR_TRANS)
	{
		if (m_bTrans) TextFormat.nEscapement = 0;
		else          TextFormat.nEscapement = 900;
		bRight = TRUE;
	}
	if (TextFormat.nEscapement == 900 || TextFormat.nEscapement == -900)
	{
		TextFormat.Size = 12;
	}
	else
	{
		TextFormat.Size = 13;
	}
	m_DrawCtrl.m_arTextFormat.SetAt(++m_nLastFormatID, TextFormat);
	nFormatIDTxt = m_nLastFormatID;
	MakeDrawBasicDim(nFormatIDLine, nFormatIDArrow, nFormatIDTxt, Line, m_dDimFrontGap * 0.5, strTmp, bRight);
}

void CDgnSeisAnchorEvalEditBtn::MakeDrawSideAnchor(int nDir, int nCa1BearingIIdx, int nCa1BearingJIdx, int nCa1AnchorIIdx, int nCa1AnchorJIdx, double dBearingThickness, double PosCa1Dim[2])
{
	// 이 함수는 nDir이 CDgnSeisAnchorShapeMgr::DIR_BOT 일 때 좌측에 그리고... CDgnSeisAnchorShapeMgr::DIR_LEFT 일 때 아래에 그린다..
	// 리팩토링이 필요하지만... 시간이 없어서 그만...

	int nBearingNumI, nBearingNumJ, nAnchorNumI, nAnchorNumJ;
	CArray<int, int> aBearingNumJ;
	m_ShapeMgr.GetBearingNumber(nBearingNumI, aBearingNumJ);
	nBearingNumJ = aBearingNumJ[0]; // CivilSeismicTest <- 비대칭 게산이 가능하도록 수정 필요

	std::vector<double> aAnchorPosX, aAnchorPosY, aBearingPosX, aBearingPosY;
	double Pos[2];
	double Pos1[2], Pos2[2];

	T_POINT2D SideCopingLine[2];
	if (nDir == CDgnSeisAnchorShapeMgr::DIR_LONGI)
	{
		SideCopingLine[0] = m_SideCopingLineForBot[0];
		SideCopingLine[1] = m_SideCopingLineForBot[1];
	}
	else
	{
		SideCopingLine[0] = m_SideCopingLineForLeft[0];
		SideCopingLine[1] = m_SideCopingLineForLeft[1];
	}
	int nSideAnchorIdx;
	// 베어링과 앵커 옆모습 그리기
	if (nDir == CDgnSeisAnchorShapeMgr::DIR_LONGI)
	{
		for (int i = 0; i < nBearingNumI; i++)
		{
			//m_ShapeMgr.GetAnchorNumber(i, 0, nAnchorNumI, nAnchorNumJ);
			m_ShapeMgr.GetAnchorNumber(i, nCa1BearingJIdx, nAnchorNumI, nAnchorNumJ);

			for (int j = 0; j < nAnchorNumI; j++)
			{
				nSideAnchorIdx = (nCa1AnchorJIdx == 0) ? 0 : nAnchorNumJ - 1;
				m_ShapeMgr.CalcAnchorPos(i, nCa1BearingJIdx, j, nSideAnchorIdx, Pos[0], Pos[1]);

				if (j == 0)
				{
					aBearingPosX.push_back(Pos[0]);
					aBearingPosY.push_back(Pos[1] - m_aArBearingData[i][0].dSy);
				}

				aAnchorPosX.push_back(Pos[0]);
				aAnchorPosY.push_back(Pos[1]);

				if (i == nCa1BearingIIdx && j == nCa1AnchorIIdx)
				{
					PosCa1Dim[0] = Pos[0];
					PosCa1Dim[1] = Pos[1];
				}

				if (j == nAnchorNumI - 1)
				{
					aBearingPosX.push_back(Pos[0]);
					aBearingPosY.push_back(Pos[1] + m_aArBearingData[i][0].dSy);
				}
			}
		}
	}
	else if (nDir == CDgnSeisAnchorShapeMgr::DIR_TRANS)
	{
		for (int i = 0; i < nBearingNumJ; i++)
		{
			m_ShapeMgr.GetAnchorNumber(nCa1BearingIIdx, i, nAnchorNumI, nAnchorNumJ);

			for (int j = 0; j < nAnchorNumJ; j++)
			{
				m_ShapeMgr.CalcAnchorPos(nCa1BearingIIdx, i, min(nCa1AnchorIIdx, nAnchorNumI - 1), j, Pos[0], Pos[1]);

				if (j == 0)
				{
					aBearingPosX.push_back(Pos[0] - m_aArBearingData[0][i].dSx);
					aBearingPosY.push_back(Pos[1]);
				}

				aAnchorPosX.push_back(Pos[0]);
				aAnchorPosY.push_back(Pos[1]);

				nSideAnchorIdx = (nCa1AnchorJIdx == 0) ? 0 : nAnchorNumJ - 1;
				if (i == nCa1BearingJIdx && j == nSideAnchorIdx)
				{
					PosCa1Dim[0] = Pos[0];
					PosCa1Dim[1] = Pos[1];
				}

				if (j == nAnchorNumJ - 1)
				{
					aBearingPosX.push_back(Pos[0] + m_aArBearingData[0][i].dSx);
					aBearingPosY.push_back(Pos[1]);
				}
			}
		}
	}

	double dLongDist = max(m_ShapeMgr.m_dCopyingWidth, m_ShapeMgr.m_dCopyingHeight) * 2.0;

	std::vector<double> aSidePosX, aSidePosY, aSideBrPosX, aSideBrPosY;
	int nSize;
	if (nDir == CDgnSeisAnchorShapeMgr::DIR_LONGI)
	{
		nSize = (int)aAnchorPosX.size();
		for (int i = 0; i < nSize; i++)
		{
			Pos1[0] = aAnchorPosX[i] - dLongDist;
			Pos1[1] = aAnchorPosY[i];

			Pos2[0] = aAnchorPosX[i] + dLongDist;
			Pos2[1] = aAnchorPosY[i];
			if (CMathFunc::mathIntersectLine2D(Pos1[0], Pos1[1], Pos2[0], Pos2[1], SideCopingLine[0].x, SideCopingLine[0].y, SideCopingLine[1].x, SideCopingLine[1].y, Pos[0], Pos[1], FALSE))
			{
				aSidePosX.push_back(Pos[0]);
				aSidePosY.push_back(Pos[1]);
			}
		}
		nSize = (int)aBearingPosX.size();
		for (int i = 0; i < nSize; i++)
		{
			Pos1[0] = aBearingPosX[i] - dLongDist;
			Pos1[1] = aBearingPosY[i];

			Pos2[0] = aBearingPosX[i] + dLongDist;
			Pos2[1] = aBearingPosY[i];
			if (CMathFunc::mathIntersectLine2D(Pos1[0], Pos1[1], Pos2[0], Pos2[1], SideCopingLine[0].x, SideCopingLine[0].y, SideCopingLine[1].x, SideCopingLine[1].y, Pos[0], Pos[1], FALSE))
			{
				aSideBrPosX.push_back(Pos[0]);
				aSideBrPosY.push_back(Pos[1]);
			}
		}

		Pos1[0] = PosCa1Dim[0] - dLongDist;
		Pos1[1] = PosCa1Dim[1];
		Pos2[0] = PosCa1Dim[0] + dLongDist;
		Pos2[1] = PosCa1Dim[1];
		if (CMathFunc::mathIntersectLine2D(Pos1[0], Pos1[1], Pos2[0], Pos2[1], SideCopingLine[0].x, SideCopingLine[0].y, SideCopingLine[1].x, SideCopingLine[1].y, Pos[0], Pos[1], FALSE))
		{
			PosCa1Dim[0] = Pos[0];
			PosCa1Dim[1] = Pos[1];
		}
	}
	else if (nDir == CDgnSeisAnchorShapeMgr::DIR_TRANS)
	{
		nSize = (int)aAnchorPosX.size();
		for (int i = 0; i < nSize; i++)
		{

			Pos1[0] = aAnchorPosX[i];
			Pos1[1] = aAnchorPosY[i] - dLongDist;

			Pos2[0] = aAnchorPosX[i];
			Pos2[1] = aAnchorPosY[i] + dLongDist;
			if (CMathFunc::mathIntersectLine2D(Pos1[0], Pos1[1], Pos2[0], Pos2[1], SideCopingLine[0].x, SideCopingLine[0].y, SideCopingLine[1].x, SideCopingLine[1].y, Pos[0], Pos[1], FALSE))
			{
				aSidePosX.push_back(Pos[0]);
				aSidePosY.push_back(Pos[1]);
			}
		}
		nSize = (int)aBearingPosX.size();
		for (int i = 0; i < nSize; i++)
		{

			Pos1[0] = aBearingPosX[i];
			Pos1[1] = aBearingPosY[i] - dLongDist;

			Pos2[0] = aBearingPosX[i];
			Pos2[1] = aBearingPosY[i] + dLongDist;
			if (CMathFunc::mathIntersectLine2D(Pos1[0], Pos1[1], Pos2[0], Pos2[1], SideCopingLine[0].x, SideCopingLine[0].y, SideCopingLine[1].x, SideCopingLine[1].y, Pos[0], Pos[1], FALSE))
			{
				aSideBrPosX.push_back(Pos[0]);
				aSideBrPosY.push_back(Pos[1]);
			}
		}

		Pos1[0] = PosCa1Dim[0];
		Pos1[1] = PosCa1Dim[1] - dLongDist;

		Pos2[0] = PosCa1Dim[0];
		Pos2[1] = PosCa1Dim[1] + dLongDist;
		if (CMathFunc::mathIntersectLine2D(Pos1[0], Pos1[1], Pos2[0], Pos2[1], SideCopingLine[0].x, SideCopingLine[0].y, SideCopingLine[1].x, SideCopingLine[1].y, Pos[0], Pos[1], FALSE))
		{
			PosCa1Dim[0] = Pos[0];
			PosCa1Dim[1] = Pos[1];
		}
	}

	if (aSidePosX.size() == 0)
		return;

	if (aSideBrPosX.size() % 2 != 0)
	{
		ASSERT(0);
		return;
	}

	double Dir[2], Dir90[2], DirTmp[2];
	m_ShapeMgr.GetDir(SideCopingLine[0].x, SideCopingLine[0].y, SideCopingLine[1].x, SideCopingLine[1].y, Dir[0], Dir[1]);

	// 방향을 90도 돌림	
	Dir90[0] = Dir[1];
	Dir90[1] = -Dir[0];

	// Bearing 그리기
	T_DRAW_FORMAT	DrawFormat;
	DrawFormat.bLine = TRUE;
	DrawFormat.bFill = TRUE;
	DrawFormat.lineColor = RGB(0, 0, 0);
	DrawFormat.fillColor = m_ColorBearing;
	DrawFormat.Width = 1;
	m_DrawCtrl.m_arDrawFormat.SetAt(++m_nLastFormatID, DrawFormat);

	T_POLYLINE_R tPolygon, tModPolygon;
	T_POLYGON_R_GROUP tPolygonGroup;
	tPolygonGroup.List.SetSize(1);
	tPolygon.arPoint.SetSize(4);
	tModPolygon.arPoint.SetSize(4);
	if (nDir == CDgnSeisAnchorShapeMgr::DIR_LONGI)
	{
		DirTmp[0] = Dir90[0];
		DirTmp[1] = Dir90[1];
	}
	else if (nDir == CDgnSeisAnchorShapeMgr::DIR_TRANS)
	{
		DirTmp[0] = -Dir90[0];
		DirTmp[1] = -Dir90[1];
	}
	nSize = (int)aSideBrPosX.size();
	for (int i = 0; i < nSize; i += 2)
	{
		tPolygon.arPoint[0].x = aSideBrPosX[i];
		tPolygon.arPoint[0].y = aSideBrPosY[i];
		tPolygon.arPoint[1].x = aSideBrPosX[i + 1];
		tPolygon.arPoint[1].y = aSideBrPosY[i + 1];
		tPolygon.arPoint[2].x = tPolygon.arPoint[1].x;
		tPolygon.arPoint[2].y = tPolygon.arPoint[1].y;
		m_ShapeMgr.PntMove(DirTmp[0], DirTmp[1], dBearingThickness, tPolygon.arPoint[2].x, tPolygon.arPoint[2].y);
		tPolygon.arPoint[3].x = tPolygon.arPoint[0].x;
		tPolygon.arPoint[3].y = tPolygon.arPoint[0].y;
		m_ShapeMgr.PntMove(DirTmp[0], DirTmp[1], dBearingThickness, tPolygon.arPoint[3].x, tPolygon.arPoint[3].y);

		if (m_bTrans)
		{
			for (int j = 0; j < tPolygon.arPoint.GetSize(); j++)
			{
				ConvtoTransScale(tPolygon.arPoint[j].x, tPolygon.arPoint[j].y, m_dCopingCenterX, m_dCopingCenterY, tModPolygon.arPoint[j].x, tModPolygon.arPoint[j].y);
			}

			tPolygon = tModPolygon;
		}

		tPolygonGroup.List[0] = tPolygon;

		m_DrawCtrl.AddDrawUnit(m_nLastFormatID, tPolygonGroup);
	}

	// Anchor 그리기 ( 선 )
	double dAnchorDepth = m_aArBearingData[0][0].dhef;
	DrawFormat.bLine = TRUE;
	DrawFormat.lineColor = RGB(0, 0, 0);
	DrawFormat.startArrowType = EN_ARROW_TYPE_NONE;
	DrawFormat.endArrowType = EN_ARROW_TYPE_NONE;
	DrawFormat.Width = 1;
	m_DrawCtrl.m_arDrawFormat.SetAt(++m_nLastFormatID, DrawFormat);

	T_ARROWLINE_R_GROUP tArrowGroup;
	tArrowGroup.List.SetSize(1);
	T_LINE_R tLine, tModLine;

	if (nDir == CDgnSeisAnchorShapeMgr::DIR_LONGI)
	{
		DirTmp[0] = -Dir90[0];
		DirTmp[1] = -Dir90[1];
	}
	else if (nDir == CDgnSeisAnchorShapeMgr::DIR_TRANS)
	{
		DirTmp[0] = Dir90[0];
		DirTmp[1] = Dir90[1];
	}
	nSize = (int)aSidePosX.size();
	for (int i = 0; i < nSize; i++)
	{
		tLine.P1.x = aSidePosX[i];
		tLine.P1.y = aSidePosY[i];
		tLine.P2.x = tLine.P1.x;
		tLine.P2.y = tLine.P1.y;
		m_ShapeMgr.PntMove(DirTmp[0], DirTmp[1], dAnchorDepth, tLine.P2.x, tLine.P2.y);

		if (m_bTrans)
		{
			ConvtoTransScale(tLine.P1.x, tLine.P1.y, m_dCopingCenterX, m_dCopingCenterY, tModLine.P1.x, tModLine.P1.y);
			ConvtoTransScale(tLine.P2.x, tLine.P2.y, m_dCopingCenterX, m_dCopingCenterY, tModLine.P2.x, tModLine.P2.y);

			tLine = tModLine;
		}

		tArrowGroup.List[0] = tLine;
		m_DrawCtrl.AddDrawUnit(m_nLastFormatID, tArrowGroup);
	}
}

void CDgnSeisAnchorEvalEditBtn::ConvColorToGray(COLORREF& Color)
{
	COLORREF R = GetRValue(Color);
	COLORREF G = GetGValue(Color);
	COLORREF B = GetBValue(Color);

	COLORREF Gray = (0.299 * R + 0.587 * G + 0.114 * B);
	Color = RGB(Gray, Gray, Gray);
}

void CDgnSeisAnchorEvalEditBtn::ConvDrawToGray()
{
	T_DRAW_FORMAT      DrawFormat;
	T_TEXT_FORMAT      TextFormat;

	UINT Index;
	POSITION Pos;

	Index = 0;
	Pos = m_DrawCtrl.m_arDrawFormat.GetStartPosition();
	while (Pos)
	{
		m_DrawCtrl.m_arDrawFormat.GetNextAssoc(Pos, Index, DrawFormat);
		ConvColorToGray(DrawFormat.lineColor);
		ConvColorToGray(DrawFormat.fillColor);
		m_DrawCtrl.m_arDrawFormat.SetAt(Index, DrawFormat);
	}
	Index = 0;
	Pos = m_DrawCtrl.m_arTextFormat.GetStartPosition();
	while (Pos)
	{
		m_DrawCtrl.m_arTextFormat.GetNextAssoc(Pos, Index, TextFormat);
		ConvColorToGray(TextFormat.fillColor);
		ConvColorToGray(TextFormat.fontColor);
		m_DrawCtrl.m_arTextFormat.SetAt(Index, TextFormat);
	}
}

void CDgnSeisAnchorEvalEditBtn::MakeDrawProjDimensionLine()
{
	CArray<double, double> aCopingX, aCopingY;
	m_ShapeMgr.GetCopyingShape(aCopingX, aCopingY);

	TConcFailBcCalc* pConcFailBc = &(m_nDir == CDgnSeisAnchorShapeMgr::DIR_LONGI ? m_LongiConcFailBc : m_TransConcFailBc);
	int nCa1BearingIIdx, nCa1BearingJIdx, nCa1AnchorIIdx, nCa1AnchorJIdx;
	double dCa1, dCa1P, dCa2, dCa2P;
	T_POINT2D Ca1Pos[2], Ca1PPos[2], Ca2Pos[2], Ca2PPos[2];	
	double dCa1Min = 1.0E6;
	if(m_nDrawType == 1)
	{
		m_ShapeMgr.GetConcFailCa(m_nDir, dCa1, Ca1Pos[0].x, Ca1Pos[0].y, Ca1Pos[1].x, Ca1Pos[1].y,
			nCa1BearingIIdx, nCa1BearingJIdx, nCa1AnchorIIdx, nCa1AnchorJIdx,
			dCa1P, Ca1PPos[0].x, Ca1PPos[0].y, Ca1PPos[1].x, Ca1PPos[1].y,
			dCa2, Ca2Pos[0].x, Ca2Pos[0].y, Ca2Pos[1].x, Ca2Pos[1].y,
			dCa2P, Ca2PPos[0].x, Ca2PPos[0].y, Ca2PPos[1].x, Ca2PPos[1].y,
			pConcFailBc->aBearingIdx);

		//////////////////////////////////////////////////////////////////////////
		// dCa1 재계산
		std::vector<std::pair<int, int>> aRepBearingIdx;
		m_ShapeMgr.GetConcfailRepresentGrpBearing(m_nDir, aRepBearingIdx);

		double dEachCa1, dEachCa1P, dEachCa2, dEachCa2P;
		double dCa1Temp, dCa1Recal;
		dCa1Recal = 0.0;
		for (int i = 0; i < aRepBearingIdx.size(); i++)
		{
			if (!m_ShapeMgr.GetReCalcCa1(m_nDir, aRepBearingIdx[i].first, aRepBearingIdx[i].second, dCa1Temp))
			{
				m_ShapeMgr.GetConcFailBearingCa(m_nDir, aRepBearingIdx[i].first, aRepBearingIdx[i].second, dEachCa1, dEachCa1P, dEachCa2, dEachCa2P);
				dCa1Temp = dEachCa1;
			}
			dCa1Min = min(dCa1Min, dCa1Temp);

			dCa1Recal += dCa1Temp;
		}
		if (aRepBearingIdx.size() > 0)
		{
			dCa1Recal /= aRepBearingIdx.size();
		}
		if (fabs(dCa1Recal) < m_dZero) dCa1Min = 0.0;

	}
	else if (m_nDrawType == 2)
	{
		m_ShapeMgr.GetPryoutCa(m_nDir, dCa1, Ca1Pos[0].x, Ca1Pos[0].y, Ca1Pos[1].x, Ca1Pos[1].y,
			nCa1BearingIIdx, nCa1BearingJIdx, nCa1AnchorIIdx, nCa1AnchorJIdx,
			dCa1P, Ca1PPos[0].x, Ca1PPos[0].y, Ca1PPos[1].x, Ca1PPos[1].y,
			dCa2, Ca2Pos[0].x, Ca2Pos[0].y, Ca2Pos[1].x, Ca2Pos[1].y,
			dCa2P, Ca2PPos[0].x, Ca2PPos[0].y, Ca2PPos[1].x, Ca2PPos[1].y);
	}
	else
	{
		ASSERT(0);
		return;
	}

	T_DRAW_FORMAT	DrawFormat;
	DrawFormat.bLine = TRUE;
	DrawFormat.lineColor = m_ColorDimensionLine;
	DrawFormat.startArrowType = EN_ARROW_TYPE_NONE;
	DrawFormat.endArrowType = EN_ARROW_TYPE_NONE;
	DrawFormat.Width = 1;
	m_DrawCtrl.m_arDrawFormat.SetAt(++m_nLastFormatID, DrawFormat);
	int nFormatIDLine = m_nLastFormatID;

	DrawFormat.nPenStyle = BS_SOLID;
	DrawFormat.nPenSubStyle = PS_GEOMETRIC | PS_DOT;
	DrawFormat.Width = 1;
	m_DrawCtrl.m_arDrawFormat.SetAt(++m_nLastFormatID, DrawFormat);
	int nFormatIDDot = m_nLastFormatID;

	DrawFormat.nPenStyle = BS_SOLID;
	DrawFormat.nPenSubStyle = -1;
	DrawFormat.startArrowType = EN_ARROW_TYPE_ARROW;
	DrawFormat.endArrowType = EN_ARROW_TYPE_ARROW;
	DrawFormat.dRWidth = 3;
	DrawFormat.dRHeight = 2;
	m_DrawCtrl.m_arDrawFormat.SetAt(++m_nLastFormatID, DrawFormat);
	int nFormatIDArrow = m_nLastFormatID;

	T_TEXT_FORMAT	TextFormat;
	T_TEXT_R_GROUP tTextGroup;
	tTextGroup.List.SetSize(1);

	TextFormat.fontColor = RGB(0, 0, 0);
	TextFormat.positionType = EN_POSITION_TYPE_CENTER_VCENTER;


	TextFormat.nEscapement = (int)(-m_Data.aLayer[nCa1BearingIIdx].dTheta_q2 * 10.0);
	

	if (m_nDir == CDgnSeisAnchorShapeMgr::DIR_LONGI)
	{
		if (m_bTrans) TextFormat.nEscapement += 900;
	}
	else if (m_nDir == CDgnSeisAnchorShapeMgr::DIR_TRANS)
	{
		if (!m_bTrans) TextFormat.nEscapement -= 900;
	}

	if (TextFormat.nEscapement == 900 || TextFormat.nEscapement == -900)
	{
		TextFormat.Size = 9;
	}
	else
	{
		TextFormat.Size = 10;
	}

	int nCa2Escapement = TextFormat.nEscapement;
	m_DrawCtrl.m_arTextFormat.SetAt(++m_nLastFormatID, TextFormat);
	int nFormatIDTxtCa2 = m_nLastFormatID;

	T_ARROWLINE_R_GROUP tArrowGroup;
	tArrowGroup.List.SetSize(1);
	T_LINE_R tLine;
	T_POINT tPnt;

	// Ca2
	CString strTmp;
	BOOL bRight;
	if (m_nDir == CDgnSeisAnchorShapeMgr::DIR_LONGI)
	{
		bRight = TRUE;
	}
	else if (m_nDir == CDgnSeisAnchorShapeMgr::DIR_TRANS)
	{
		bRight = TRUE;
	}
	if (m_bUser)
	{
		strTmp.Format(_T("Ca2"));
	}
	else
	{
		strTmp.Format(_T("%g"), SetFloor(CMathFunc::mathRoundOff(dCa2, 7)));
	}


	// Dimension Line 이격을 위한 변수 결정
	if (Ca2Pos[1].x - Ca2Pos[0].x >= 0 && Ca2Pos[1].y - Ca2Pos[0].y == 0)
	{	
		if (Ca2Pos[0].y >=0)
		{
			if (abs(aCopingY[2] - (Ca2Pos[0].y - (m_dDimLenShort - m_dDimLenTailGap))) <= m_GapCri)
			{
				m_dDimOverlap *= -1;
			}
		}
		else
		{
			if (abs(aCopingY[3] - (Ca2Pos[0].y - (m_dDimLenShort - m_dDimLenTailGap))) <= m_GapCri)
			{
				m_dDimOverlap *= 1;
			}
		}
	}
	else if (Ca2Pos[1].x - Ca2Pos[0].x < 0 && Ca2Pos[1].y - Ca2Pos[0].y == 0)
	{	
		if (Ca2Pos[0].y >= 0)
		{
			if (abs(aCopingY[1] - (Ca2Pos[0].y + (m_dDimLenShort - m_dDimLenTailGap))) <= m_GapCri)
			{
				m_dDimOverlap *= 1;
			}
		}
		else
		{
			if (abs(aCopingY[0] - (Ca2Pos[0].y + (m_dDimLenShort - m_dDimLenTailGap))) <= m_GapCri)
			{
				m_dDimOverlap *= -1;
			}
		}
	}

	MakeDrawBasicDim(nFormatIDLine, nFormatIDArrow, nFormatIDTxtCa2, Ca2Pos, 0.0, strTmp, bRight);

	// Ca2P
	if (m_nDir == CDgnSeisAnchorShapeMgr::DIR_LONGI)
	{
		bRight = FALSE;
	}
	else if (m_nDir == CDgnSeisAnchorShapeMgr::DIR_TRANS)
	{
		bRight = FALSE;
	}
	if (m_bUser)
	{
		strTmp.Format(_T("Ca'2"));
	}
	else
	{
		strTmp.Format(_T("%g"), SetFloor(CMathFunc::mathRoundOff(dCa2P, 7)));
	}
	MakeDrawBasicDim(nFormatIDLine, nFormatIDArrow, nFormatIDTxtCa2, Ca2PPos, 0.0, strTmp, bRight);
	
	m_dDimOverlap = 0;

	MakeDrawSideAnchorAndDim(nCa1BearingIIdx, nCa1BearingJIdx, nCa1AnchorIIdx, nCa1AnchorJIdx, dCa1);

	int nAnchorNumI, nAnchorNumJ;
	m_ShapeMgr.GetAnchorNumber(0, 0, nAnchorNumI, nAnchorNumJ);

	int nBearingNumI;
	CArray<int, int> aBearingNumJ;
	m_ShapeMgr.GetBearingNumber(nBearingNumI, aBearingNumJ);
	CString strTmp2;

	if (!IsCa1DimCopingSize())
	{
		// Ca1
		if (m_bUser)
		{
			strTmp.Format(_T("Ca1"));
		}
		else
		{
			if (m_nDrawType == 1)
			{
				strTmp.Format(_T("%g"), SetFloor(CMathFunc::mathRoundOff(dCa1Min, 7)));
				strTmp2.Format(_T("%g"), SetFloor(CMathFunc::mathRoundOff(dCa1 - dCa1Min, 7)));
			}
			else if (m_nDrawType == 2)
			{
				strTmp.Format(_T("%g"), SetFloor(CMathFunc::mathRoundOff(dCa1, 7)));
			}
			
		}

		bRight = FALSE;
		if (m_nDir == CDgnSeisAnchorShapeMgr::DIR_LONGI)
		{
			if (nCa1AnchorJIdx == 0)
			{
				bRight = TRUE;
			}
			else
			{
				bRight = FALSE;
			}
		}
		else if (m_nDir == CDgnSeisAnchorShapeMgr::DIR_TRANS)
		{
			if (nAnchorNumI == 1 && nBearingNumI == 1)
			{
				bRight = TRUE;
			}
			else if (nCa1AnchorIIdx == 0)
			{
				bRight = FALSE;
			}
			else
			{
				bRight = TRUE;
			}
		}

		if (m_nDir == CDgnSeisAnchorShapeMgr::DIR_LONGI)// Ca2 의 방향에서 돌림
		{
			if (bRight)
			{	
				if (m_nDownUp == 0)
				{
					TextFormat.nEscapement = nCa2Escapement - 900;
				}
				else if (m_nDownUp == 1)
				{
					TextFormat.nEscapement = nCa2Escapement - 900;
				}
			}
			else
			{
				TextFormat.nEscapement = nCa2Escapement - 900;
			}
		}
		else
		{
			if (bRight)
			{
				TextFormat.nEscapement = nCa2Escapement + 900;   //ok
			}
			else
			{
				TextFormat.nEscapement = nCa2Escapement + 900;
			}
		}
		if (TextFormat.nEscapement == 900 || TextFormat.nEscapement == -900)
		{
			TextFormat.Size = 9;
		}
		else
		{
			TextFormat.Size = 10;
		}
		m_DrawCtrl.m_arTextFormat.SetAt(++m_nLastFormatID, TextFormat);
		int nFormatIDTxtCa1 = m_nLastFormatID;

		// Dimension Line 이격을 위한 변수 결정
		if (Ca1Pos[1].x - Ca1Pos[0].x >= 0 && Ca1Pos[1].y - Ca1Pos[0].y == 0)
		{	
			if (Ca1Pos[0].y >= 0)
			{
				if (abs(aCopingY[2] - (Ca1Pos[0].y - (m_dDimLenShort - m_dDimLenTailGap))) <= m_GapCri)
				{
					m_dDimOverlap *= -1;
				}
			}
			else
			{
				if (abs(aCopingY[3] - (Ca1Pos[0].y - (m_dDimLenShort - m_dDimLenTailGap))) <= m_GapCri)
				{
					m_dDimOverlap *= 1;
				}
			}
		}
		else if (Ca1Pos[1].x - Ca1Pos[0].x < 0 && Ca1Pos[1].y - Ca1Pos[0].y == 0)
		{	
			if (Ca1Pos[0].y >= 0)
			{
				if (abs(aCopingY[1] - (Ca1Pos[0].y + (m_dDimLenShort - m_dDimLenTailGap))) <= m_GapCri)
				{
					m_dDimOverlap *= 1;
				}
			}
			else
			{
				if (abs(aCopingY[0] - (Ca1Pos[0].y - (m_dDimLenShort - m_dDimLenTailGap))) <= m_GapCri)
				{
					m_dDimOverlap *= 1;
				}
			}
		}

		if (m_nDrawType == 1)
		{
			double dDir[2], dMovDot[2], dMovDot2[2];
			dMovDot[0] = Ca1Pos[0].x;
			dMovDot[1] = Ca1Pos[0].y;
			dMovDot2[0] = Ca1Pos[1].x;
			dMovDot2[1] = Ca1Pos[1].y;
			m_ShapeMgr.GetDir(Ca1Pos[0].x, Ca1Pos[0].y, Ca1Pos[1].x, Ca1Pos[1].y, dDir[0], dDir[1]);
			m_ShapeMgr.PntMove(dDir[0], dDir[1], dCa1 - dCa1Min, dMovDot[0], dMovDot[1]);
			Ca1Pos[1].x = dMovDot[0];
			Ca1Pos[1].y = dMovDot[1];
			MakeDrawBasicDim(nFormatIDLine, nFormatIDArrow, nFormatIDTxtCa1, Ca1Pos, 0.0, strTmp2, bRight);
			Ca1Pos[0].x = dMovDot[0];
			Ca1Pos[0].y = dMovDot[1];
			Ca1Pos[1].x = dMovDot2[0];
			Ca1Pos[1].y = dMovDot2[1];
			MakeDrawBasicDim(nFormatIDLine, nFormatIDArrow, nFormatIDTxtCa1, Ca1Pos, 0.0, strTmp, bRight);
			m_dDimOverlap = 0;
		}
		else if (m_nDrawType == 2)
		{
			MakeDrawBasicDim(nFormatIDLine, nFormatIDArrow, nFormatIDTxtCa1, Ca1Pos, 0.0, strTmp, bRight);
			m_dDimOverlap = 0;
		}		
	}

	MakeDrawSideAnchorAndDim(nCa1BearingIIdx, nCa1BearingJIdx, nCa1AnchorIIdx, nCa1AnchorJIdx, dCa1);
	

	if (CTestEnvMgr::GetTestEnvST(_T("SeisCa1PDraw")) == _T("yes"))
	{
		// Ca1P Test : 테스트용.. 눈으로 확인 하려고 그려 봄
		DrawFormat.lineColor = RGB(255, 255, 0);
		m_DrawCtrl.m_arDrawFormat.SetAt(++m_nLastFormatID, DrawFormat);
		tLine.P1.x = Ca1PPos[0].x;
		tLine.P1.y = Ca1PPos[0].y;
		tLine.P2.x = Ca1PPos[1].x;
		tLine.P2.y = Ca1PPos[1].y;
		tArrowGroup.List[0] = tLine;
		m_DrawCtrl.AddDrawUnit(m_nLastFormatID, tArrowGroup);
	}	
}
/*
void CDgnSeisAnchorEvalEditBtn::MakeDrawDeviantLine(int nIdx, double dGap)
{
	ASSERT(0); 빗금 그리는 함수인데.. 넣어 보니깐.. 한 두 픽셀식 더 길게 그리는 경우가 있어서... 그냥 안쓰는 방향으로 감
	T_POLYLINE_R& EachPoly = m_aDeviantLine.List[nIdx];
	if(EachPoly.arPoint.GetSize() != 4)
	{
		ASSERT(0);
		return;
	}

	// Step1 : 바운딩 박스를 만들어서..
	T_POINT BndBox[2]; // [0]:좌측위, [1]:우측아래
	BndBox[0].x = DBL_MAX;
	BndBox[0].y = -DBL_MAX;
	BndBox[1].x = -DBL_MAX;
	BndBox[1].y = DBL_MAX;
	int nSize;
	nSize = EachPoly.arPoint.GetSize();
	for (int i = 0; i < nSize ; i++)
	{
		if(BndBox[0].x > EachPoly.arPoint[i].x)
		{
			BndBox[0].x = EachPoly.arPoint[i].x;
		}
		if(BndBox[1].x < EachPoly.arPoint[i].x)
		{
			BndBox[1].x = EachPoly.arPoint[i].x;
		}

		if(BndBox[0].y < EachPoly.arPoint[i].y)
		{
			BndBox[0].y = EachPoly.arPoint[i].y;
		}
		if(BndBox[1].y > EachPoly.arPoint[i].y)
		{
			BndBox[1].y = EachPoly.arPoint[i].y;
		}
	}

	// Step2: 바운딩 박스 좌측부터 우측까지 대각선 선을 만들어서 충돌 지점을 찾음..
	double dLongDist = max(BndBox[1].x - BndBox[0].x, BndBox[0].y - BndBox[1].y) * 2;
	ASSERT(dLongDist >= 0);

	double Dir[2];
	m_ShapeMgr.GetDir(0, 0, 1.0, 1.0, Dir[0], Dir[1]);

	std::vector<T_POINT> aIntersect;

	T_POINT DeviantLinePos, IntersectPos;
	DeviantLinePos = BndBox[0];
	T_POINT LongLine[2];

	T_ARROWLINE_R_GROUP tArrowGroup;
	tArrowGroup.List.SetSize(1);
	T_LINE_R tLine;

	while (DeviantLinePos.x < BndBox[1].x)
	{
		aIntersect.clear();

		DeviantLinePos.x += dGap;
		LongLine[0] = LongLine[1] = DeviantLinePos;
		m_ShapeMgr.PntMove(Dir[0], Dir[1], -dLongDist, LongLine[0].x, LongLine[0].y);
		m_ShapeMgr.PntMove(Dir[0], Dir[1], dLongDist, LongLine[1].x, LongLine[1].y);

		nSize = EachPoly.arPoint.GetSize();
		for (int i = 0; i < nSize ; i++)
		{
			T_POINT& Pos1 = EachPoly.arPoint[i];
			T_POINT& Pos2 = i == nSize - 1 ? EachPoly.arPoint[0] : EachPoly.arPoint[i + 1];

			if(CMathFunc::mathIntersectLine2D(LongLine[0].x, LongLine[0].y, LongLine[1].x, LongLine[1].y, Pos1.x, Pos1.y, Pos2.x, Pos2.y, IntersectPos.x, IntersectPos.y, TRUE))
			{
				aIntersect.push_back(IntersectPos);
			}
		}

		if(aIntersect.size() <= 1)
			continue;

		for (int i = 0; i < aIntersect.size() - 1 ; i++)
		{
			for (int j = i + 1; j < aIntersect.size() ; j++)
			{
				if(math_lib::distance_2d(aIntersect[i].x, aIntersect[i].y, aIntersect[j].x, aIntersect[j].y) < 0.001)
				{
					aIntersect.erase(aIntersect.begin() + j);
					j--;
				}
			}
		}

		if(aIntersect.size() == 2)
		{
			tLine.P1.x = aIntersect[0].x;
			tLine.P1.y = aIntersect[0].y;
			tLine.P2.x = aIntersect[1].x;
			tLine.P2.y = aIntersect[1].y;
			tArrowGroup.List[0] = tLine;
			m_DrawCtrl.AddDrawUnit(m_nLastFormatID, tArrowGroup);
		}
	}
}
*/
/*
void CDgnSeisAnchorEvalEditBtn::MakeDrawDeviantLine()
{
	double dGap = ConvWndPosToCoord(4);

	T_DRAW_FORMAT	DrawFormat;
	DrawFormat.bLine  = TRUE;
	//DrawFormat.bFill  = FALSE;
	DrawFormat.lineColor = m_ColorRefLine;
	DrawFormat.startArrowType = EN_ARROW_TYPE_NONE;
	DrawFormat.endArrowType = EN_ARROW_TYPE_NONE;
	DrawFormat.Width  = 1;
	m_DrawCtrl.m_arDrawFormat.SetAt(++m_nLastFormatID, DrawFormat);

	int nSize;
	nSize = m_aDeviantLine.List.GetSize();
	for (int i = 0; i < nSize ; i++)
	{
		MakeDrawDeviantLine(i, dGap);
	}
}
*/
/*
void CDgnSeisAnchorEvalEditBtn::MakeDrawValue()
{
	T_TEXT_FORMAT	TextFormat;
	TextFormat.fontColor = RGB(0,0,0);
	TextFormat.Size = 13;
	TextFormat.positionType = EN_POSITION_TYPE_RIGHT_BOTTOM;
	m_DrawCtrl.m_arTextFormat.SetAt(++m_nLastFormatID, TextFormat);

	T_TEXT_R_GROUP tTextGroup;
	tTextGroup.List.SetSize(1);

	tTextGroup.List[0].PPosition.x = (FitToFixSize(m_dMaxY, m_aY[0], m_dContourWidth) + 5) * -1;
	tTextGroup.List[0].PPosition.y = 0 + 15;
	CString strTmp;
	strTmp.Format(_T("%g"), m_aY[0]);
	tTextGroup.List[0].szText = strTmp;
	m_DrawCtrl.AddDrawUnit(m_nLastFormatID, tTextGroup);

	tTextGroup.List[0].PPosition.x = (FitToFixSize(m_dMaxY, m_aY[m_aY.size() - 1], m_dContourWidth) + 5) * -1;
	tTextGroup.List[0].PPosition.y = m_dContourHeight + 15;
	strTmp.Format(_T("%g"), m_aY[m_aY.size() - 1]);
	tTextGroup.List[0].szText = strTmp;
	m_DrawCtrl.AddDrawUnit(m_nLastFormatID, tTextGroup);
}
*/

void CDgnSeisAnchorEvalEditBtn::SetDrawFormat()
{
	/*
	T_DRAW_FORMAT	DrawFormat;
	T_TEXT_FORMAT	TextFormat;
	T_DIM_FORMAT	DimFormat;

	DimFormat.Init();
	DimFormat.fontColor = RGB(0,0,0);
	DimFormat.bMainLine = TRUE;
	DimFormat.dRTextSpace = 0.05;
	DimFormat.dRDimSpace = 0.1;

	DrawFormat.bLine          = TRUE;
	DrawFormat.bFill          = TRUE;
	//DrawFormat.fillColor      = RGB(220, 220, 220);
	DrawFormat.fillColor      = RGB(220, 0, 0);
	DrawFormat.lineColor      = RGB(0,   0,   0);
	m_DrawCtrl.m_arDrawFormat.SetAt(0, DrawFormat);
	*/
	//m_CurrDrawFormat.nPenStyle = PS_DOT;
}

void CDgnSeisAnchorEvalEditBtn::Redraw(BOOL bFit)
{
	if (bFit)
	{
		m_DrawCtrl.AutoPaperFormat(m_Canvas, m_dFitMargin, m_dFitMargin, m_dFitMargin, m_dFitMargin);
	}

	if (this)
	{
		if (IsWindow(this->m_hWnd))
		{
			RedrawWindow();
		}
	}
}

void CDgnSeisAnchorEvalEditBtn::SetDrawType(int nDrawType, int nDir, BOOL bTrans)
{
	m_nDrawType = nDrawType;
	m_nDir = nDir;
	m_bTrans = bTrans;

	//if (CTestEnvMgr::GetTestEnvST(_T("CivilSeismicTest")) == _T("yes"))
	{
		//m_bTrans = TRUE;  // 910 배치그림을 -90도 돌려서 보여줌(v940~)
	}
}

void CDgnSeisAnchorEvalEditBtn::CalcBoundingBox(double dX, double dY, double* Left, double* Right, double* dTop, double* dBottom)
{
	if (Right == NULL) Right = &m_dBoundBox_Right;
	if (Left == NULL) Left = &m_dBoundBox_Left;
	if (dTop == NULL) dTop = &m_dBoundBox_Top;
	if (dBottom == NULL) dBottom = &m_dBoundBox_Bottom;

	if (dX > *Right)  *Right = dX;
	if (dX < *Left)   *Left = dX;
	if (dY > *dTop)    *dTop = dY;
	if (dY < *dBottom) *dBottom = dY;
}

void CDgnSeisAnchorEvalEditBtn::StartCurrUnitSelect(CPoint CurrP)
{
	CDrawEditBtn::StartCurrUnitSelect(CurrP);
}

void CDgnSeisAnchorEvalEditBtn::InProgressCurrUnitSelect(CPoint CurrP)
{
	/*
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
	*/
	CDrawEditBtn::InProgressCurrUnitSelect(CurrP);
	//}
}

void CDgnSeisAnchorEvalEditBtn::CompleteCurrUnitSelect(CPoint CurrP)
{
	/*
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

		//SetParentSelectedKey(nIdx, keyList, nrUintTarget, idxOrigin);

	}
	*/
}

void CDgnSeisAnchorEvalEditBtn::OnLButtonUp(UINT nFlags, CPoint point)
{
	/*
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
			dPos[0] = dPos[0];
			dPos[1] = dPos[1];
			SetDrawOrigin(dPos, FALSE, TRUE);
		}
	}
	else
	{*/
	//CDrawEditBtn::OnLButtonUp(nFlags, point);
//}
}

void CDgnSeisAnchorEvalEditBtn::OnLButtonDown(UINT nFlags, CPoint point)
{
	//	CDrawEditBtn::OnLButtonDown(nFlags, point);
		/*
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
		*/
}

void CDgnSeisAnchorEvalEditBtn::GetPosByCPoint(CPoint& cPt, double& dPtX, double& dPtY, int& nSnapX, int& nSnapY, BOOL bSnap)
{
	/*
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
			dTempX = fabs(dPtX - m_aSnapVertex[i].x);
			dTempY = fabs(dPtY - m_aSnapVertex[i].y);

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
			dTempX = fabs(dPtX - m_aSnapVertex[i].x);
			dTempY = fabs(dPtY - m_aSnapVertex[i].y);

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

		if(nSnapX >= 0) dPtX = m_aSnapVertex[nSnapX].x;
		if(nSnapY >= 0) dPtY = m_aSnapVertex[nSnapY].y;
	}
	*/
}

void CDgnSeisAnchorEvalEditBtn::ResetSnapPos()
{
	m_aSnapVertex.RemoveAll();
	/*
	for(int i = 0 ; i < m_aPnt1.size(); i++)
	{
		m_aSnapVertex.Add(m_aPnt1[i]);
	}

	for(int i = 0 ; i < m_aPnt2.size(); i++)
	{
		m_aSnapVertex.Add(m_aPnt2[i]);
	}
	*/
}

void CDgnSeisAnchorEvalEditBtn::MakeCurrLinePreview(double dPtX, double dPtY, int nSnapX, int nSnapY)
{
	T_POINT tPt;
	tPt.x = dPtX;
	tPt.y = dPtY;
	m_CurrDrawClass_Point.m_DataR.List.Add(tPt);
	if (nSnapX >= 0)
	{
		T_LINE_R tLine;
		tLine.P1.x = dPtX;
		tLine.P1.y = dPtY;
		tLine.P2.x = m_aSnapVertex[nSnapX].x;
		tLine.P2.y = m_aSnapVertex[nSnapX].y;
		m_CurrDrawClass_Line.m_DataR.List.Add(tLine);
	}
	if (nSnapY >= 0)
	{
		T_LINE_R tLine;
		tLine.P1.x = dPtX;
		tLine.P1.y = dPtY;
		tLine.P2.x = m_aSnapVertex[nSnapY].x;
		tLine.P2.y = m_aSnapVertex[nSnapY].y;
		m_CurrDrawClass_Line.m_DataR.List.Add(tLine);
	}

	m_CurrDrawClass_Point.Convert(TRUE, &(m_DrawCtrl.m_DrawFunc), &(m_DrawCtrl.m_PaperFormat), &(m_CurrDrawFormat));
	m_CurrDrawClass_Line.Convert(TRUE, &(m_DrawCtrl.m_DrawFunc), &(m_DrawCtrl.m_PaperFormat), &(m_CurrDrawFormat));
}

/*
// Diff는 Org-Cmp
BOOL CDgnSeisAnchorEvalEditBtn::IsSamePolygon( const DGN_GSEC_POLYGON_LIST& arOrg, const DGN_GSEC_POLYGON_LIST& arCmp, double& dDiffY, double& dDiffZ )
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
				dDiffY = arOrg[i].aVertex[j].x - arCmp[i].aVertex[j].x;
				dDiffZ = arOrg[i].aVertex[j].y - arCmp[i].aVertex[j].y;
			}
			else
			{
				dDiffYTemp = arOrg[i].aVertex[j].x - arCmp[i].aVertex[j].x;
				dDiffZTemp = arOrg[i].aVertex[j].y - arCmp[i].aVertex[j].y;

				if(fabs(dDiffY - dDiffYTemp) > 1E-5)
					return FALSE;
				if(fabs(dDiffZ - dDiffZTemp) > 1E-5)
					return FALSE;
			}
		}
	}
	return TRUE;
}
*/



BOOL CDgnSeisAnchorEvalEditBtn::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// 휠 확대 축소 막음

	/*
	if(GetAsyncKeyState(VK_LSHIFT))
	{
		if(zDelta > 0)
		{
			//m_dTendonZoomFactor += 0.01;
//			if(m_dTendonZoomFactor > 0.05) m_dTendonZoomFactor = 0.05; //최대
		}
		else if(zDelta < 0)
		{
			//m_dTendonZoomFactor -= 0.01;
			//if(m_dTendonZoomFactor < 0.01) m_dTendonZoomFactor = 0.01; //최소
		}

		MakeDrawUnit(FALSE);
		return TRUE;
	}
	else
		return CDrawEditBtn::OnMouseWheel(nFlags, zDelta, pt);
		*/
	return TRUE;
}

void CDgnSeisAnchorEvalEditBtn::OnMouseMove(UINT nFlags, CPoint point)
{
	// 화면 이동 막음
	return;
}

void CDgnSeisAnchorEvalEditBtn::MakeDrawSubClass()
{
	// 자식 Class에서 그림
}
/*
void CDgnSeisAnchorEvalEditBtn::SetParentSelectedKey( int nIdx, vecTndnKey& keyList, nrUINT& nrUintTarget, int idxOrigin )
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
*/




