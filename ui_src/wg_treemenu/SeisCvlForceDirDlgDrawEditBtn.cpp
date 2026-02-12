// SeisCvlForceDirDlgDrawEditBtn.cpp: implementation of the CDgnEditBtn_TndnTemplate class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#include "SeisCvlForceDirDlgDrawEditBtn.h"

#include "..\wg_db\MatlDB.h"
#include "..\wg_db\Db_DllManager_Base.h"

#include "SeisCvl1DElemContourUtil.h"
#include "..\wg_base\TestEnvMgr.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define LINKKEY_ORIGIN 9999

/////////////////////////////////////////////////////////////////////////////
// CDgnEditBtn_TndnTemplate dialog


CSeisCvlForceDirDlgDrawEditBtn::CSeisCvlForceDirDlgDrawEditBtn()
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

	m_nLastFormatID = -1;

	m_dContourWidth = 70;
	m_dContourHeight = 200;

	m_dWidthX = 0.0;
	m_dHeightY = 0.0;

	if(CTestEnvMgr::GetTestEnvST(_LSX(SeisDrawFill)) == _LSX(yes))
	{
		m_nDisplayType = 1;
	}
	else
	{
		m_nDisplayType = 0;
	}
	m_nSectSnapType = 1;
}

CSeisCvlForceDirDlgDrawEditBtn::~CSeisCvlForceDirDlgDrawEditBtn()
{
}

void CSeisCvlForceDirDlgDrawEditBtn::DoDataExchange(CDataExchange* pDX)
{
	CDrawEditBtn::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CSeisCvlForceDirDlgDrawEditBtn, CDrawEditBtn)
END_MESSAGE_MAP()

void CSeisCvlForceDirDlgDrawEditBtn::MakeDrawUnit( BOOL bFit /*= TRUE*/ )
{
	if(m_aX.size() <= 2) return;

	m_DrawCtrl.RemoveDrawData(TRUE, TRUE, FALSE); // Format들을 삭제 함
	
	m_nLastFormatID = -1;
	SetMaxVal(m_aX, m_aY);
	MakeDrawContour(); 
	MakeDrawElem();
	MakeDrawValue();
	MakeDrawSubClass();

	if(bFit)
	{
		Fit();
	}
	m_DrawCtrl.ConvertDrawData(TRUE);
	
	OverlapChecking();
	Redraw(bFit);
}

void CSeisCvlForceDirDlgDrawEditBtn::Fit()
{
	m_DrawCtrl.AutoPaperFormat(m_Canvas, m_dFitMargin, m_dFitMargin + 0.048, m_dFitMargin, m_dFitMargin);
}

BOOL CSeisCvlForceDirDlgDrawEditBtn::SetData(std::vector<double>& aX, std::vector<double>& aY, BOOL bFit)
{
	//m_pTemplateCtrl->GetLengthCtrl()->GetHeightWidthByElem(tElemKey, m_dTotalHeight, m_dTotalWidth, m_dGapY, m_dGapZ, dPos);
	//m_pTemplateCtrl->GetPosByElem(tElemKey, dPos, m_dPos);
	m_dTotalHeight = 10;
	m_dTotalWidth = 10;
	
	m_aX.clear();
	for (int i = 0; i < aX.size(); i++)
	{
		if(i == 0)
		{
			m_aX.push_back(0.0);
		}
		else
		{
			m_aX.push_back(aX[i] - aX[0]); // 개발편의를 위해 0부터 시작하게 만들어 둠
		}
	}
	m_aY.clear();
	for (int i = 0; i < aY.size(); i++)
	{
		m_aY.push_back(aY[i]);
	}

	ResetSnapPos();
	MakeDrawUnit(bFit);

	return TRUE;
}

double CSeisCvlForceDirDlgDrawEditBtn::FitToFixSize(double dMax, double dVal, double dFixSize)
{
	return (dVal / dMax) * dFixSize;
}

void CSeisCvlForceDirDlgDrawEditBtn::SetMaxVal(std::vector<double>& aX, std::vector<double>& aY)
{ 
	m_dWidthX = aX[aX.size() - 1];

	double dMin, dMax;
	dMax = -DBL_MAX;
	dMin = DBL_MAX;
	for (int i = 0; i < aY.size(); i++)
	{
		if(aY[i] > dMax)
		{
			dMax = aY[i];
		}
		if(aY[i] < dMin)
		{
			dMin = aY[i];
		}
	}

	if(dMax < 0)
	{
		dMax = 0;
	}

	if(dMin > 0)
	{
		dMin = 0;
	}

	m_dHeightY = dMax - dMin;
	
	// 값이 모두 없는 경우 요소를 가운데 그리기 위해서 임의의 값을 넣는다.
	if(m_dHeightY == 0.0) m_dHeightY = 10;	
}

void CSeisCvlForceDirDlgDrawEditBtn::MakeContourLine(std::vector<double>& aX, std::vector<double>& aY, std::vector<COLORREF>& aColorDiv, int nType, std::vector<T_POINT3D>& aPnt1, std::vector<T_POINT3D>& aPnt2, std::vector<COLORREF>& aColor)
{
	// nType == 0 : Global Div 인 경우 임. Y의 값을 표현 하는 선을 그림
	// nType == 1 : Val div 인 경우임. ValDiv 지점마다 긋는 선을 그림

	// 이코드는 draw 안에 넣어야 할 듯
	if(aX.size() <= 2) return;

	ASSERT(aX.size() == aY.size());

	SetMaxVal(aX, aY);

	T_POINT3D EachPos;

	for (int i = 0; i < aX.size(); i++)
	{
		if(nType == 0)
		{
			if(i == aX.size() - 1) break;

			EachPos.x = FitToFixSize(m_dHeightY, aY[i], m_dContourWidth) * -1;
			EachPos.y = FitToFixSize(m_dWidthX, aX[i], m_dContourHeight);
			aPnt1.push_back(EachPos);

			EachPos.x = FitToFixSize(m_dHeightY, aY[i + 1], m_dContourWidth) * -1;
			EachPos.y = FitToFixSize(m_dWidthX, aX[i + 1], m_dContourHeight);
			aPnt2.push_back(EachPos);

			aColor.push_back(aColorDiv[i]);
		}
		else if(nType == 1)
		{
			EachPos.x = 0;
			EachPos.y = FitToFixSize(m_dWidthX, aX[i], m_dContourHeight);
			aPnt1.push_back(EachPos);

			EachPos.x = FitToFixSize(m_dHeightY, aY[i], m_dContourWidth) * -1;
			EachPos.y = FitToFixSize(m_dWidthX, aX[i], m_dContourHeight);
			aPnt2.push_back(EachPos);

			aColor.push_back(aColorDiv[i]);
		}    
	}
}

void CSeisCvlForceDirDlgDrawEditBtn::MakeContourPoly(std::vector<double>& aX, std::vector<double>& aY, std::vector<COLORREF>& aColorDiv, std::vector<std::vector<T_POINT3D>>& aArPoly, std::vector<COLORREF>& aColor)
{
	// nType == 0 : Global Div 인 경우 임. Y의 값을 표현 하는 선을 그림
	// nType == 1 : Val div 인 경우임. ValDiv 지점마다 긋는 선을 그림

	// 이코드는 draw 안에 넣어야 할 듯
	if(aX.size() <= 2) return;

	ASSERT(aX.size() == aY.size());
	SetMaxVal(aX, aY);

	std::vector<T_POINT3D> aEachPoly;
	T_POINT3D EachPos;
	for (int i = 0; i < aX.size() - 1; i++)
	{
			aEachPoly.clear();

			EachPos.x = 0;
			EachPos.y = FitToFixSize(m_dWidthX, aX[i], m_dContourHeight);
			aEachPoly.push_back(EachPos);

			EachPos.x = FitToFixSize(m_dHeightY, aY[i], m_dContourWidth) * -1;
			EachPos.y = FitToFixSize(m_dWidthX, aX[i], m_dContourHeight);
			aEachPoly.push_back(EachPos);

			EachPos.x = FitToFixSize(m_dHeightY, aY[i + 1], m_dContourWidth) * -1;
			EachPos.y = FitToFixSize(m_dWidthX, aX[i + 1], m_dContourHeight);
			aEachPoly.push_back(EachPos);

			EachPos.x = 0;
			EachPos.y = FitToFixSize(m_dWidthX, aX[i + 1], m_dContourHeight);
			aEachPoly.push_back(EachPos);

			aArPoly.push_back(aEachPoly);
			aColor.push_back(aColorDiv[i]);
	}
}

void CSeisCvlForceDirDlgDrawEditBtn::MakeDrawValue()
{
	//여기다가 코드 넣어야 함
	T_TEXT_FORMAT	TextFormat;
	TextFormat.fontColor = RGB(0,0,0);
	TextFormat.Size = 13;
	TextFormat.positionType = EN_POSITION_TYPE_CENTER_TOP;
	m_DrawCtrl.m_arTextFormat.SetAt(++m_nLastFormatID, TextFormat);

	T_TEXT_R_GROUP tTextGroup;
	tTextGroup.List.SetSize(1);

	tTextGroup.List[0].PPosition.x = (FitToFixSize(m_dHeightY, m_aY[0], m_dContourWidth) + 5) * -1 + 5;
	tTextGroup.List[0].PPosition.y = -2;
	CString strTmp;
	strTmp.Format(_T("%g"), m_aY[0]);
	tTextGroup.List[0].szText = strTmp;
	m_DrawCtrl.AddDrawUnit(m_nLastFormatID, tTextGroup);

	tTextGroup.List[0].PPosition.x = (FitToFixSize(m_dHeightY, m_aY[m_aY.size() - 1], m_dContourWidth) + 5) * -1 + 4;
	tTextGroup.List[0].PPosition.y = m_dContourHeight + 11;
	strTmp.Format(_T("%g"), m_aY[m_aY.size() - 1]);
	tTextGroup.List[0].szText = strTmp;
	m_DrawCtrl.AddDrawUnit(m_nLastFormatID, tTextGroup);
	
}

void CSeisCvlForceDirDlgDrawEditBtn::MakeDrawElem()
{
	if(m_aX.size() <= 2) return;
	ASSERT(m_aX[0] == 0.0);

	m_dWidthX = m_aX[m_aX.size() - 1];

	std::vector<T_POINT3D> aElemPos;
	T_POINT3D EachPos;
	EachPos.x = 0.0;
	for (int i = 0; i < m_aX.size(); i++)
	{
		EachPos.y = FitToFixSize(m_dWidthX, m_aX[i], m_dContourHeight);
		aElemPos.push_back(EachPos);
	}

	T_DRAW_FORMAT	DrawFormat;
	DrawFormat.bLine  = TRUE;	
	DrawFormat.bFill  = TRUE;		
	DrawFormat.Width  = 1;     
	DrawFormat.lineColor = RGB(79, 79, 79);
	DrawFormat.fillColor = RGB(79, 79, 79);
	m_DrawCtrl.m_arDrawFormat.SetAt(++m_nLastFormatID, DrawFormat);

	T_LINE_R_GROUP tLineGroup;
	tLineGroup.List.SetSize(1);  

	T_CIRCLE_R_GROUP tCircleGroup;
	tCircleGroup.List.SetSize(1);
	tCircleGroup.List[0].Radius = 2;
	for (int i = 0; i < aElemPos.size() - 1; i++)
	{
		tLineGroup.List[0].P1.x = aElemPos[i].x;
		tLineGroup.List[0].P1.y = aElemPos[i].y;

		tLineGroup.List[0].P2.x = aElemPos[i + 1].x;
		tLineGroup.List[0].P2.y = aElemPos[i + 1].y;
		m_DrawCtrl.AddDrawUnit(m_nLastFormatID, tLineGroup);

		tCircleGroup.List[0].CenterP.x = aElemPos[i].x;
		tCircleGroup.List[0].CenterP.y = aElemPos[i].y;    
		m_DrawCtrl.AddDrawUnit(m_nLastFormatID, tCircleGroup);
		if(i == aElemPos.size() - 2)
		{
			tCircleGroup.List[0].CenterP.x = aElemPos[i + 1].x;
			tCircleGroup.List[0].CenterP.y = aElemPos[i + 1].y;
			m_DrawCtrl.AddDrawUnit(m_nLastFormatID, tCircleGroup);
		}
	}
}

void CSeisCvlForceDirDlgDrawEditBtn::MakeDrawContour()
{
	int nSnapType = m_nSectSnapType; //좌표점 스넵을 사용
	
	/*
	// Draw Shape ----------------------------
	T_POLYGON_R_GROUP PolyRGroup;     
	T_POLYLINE_R      polyLineUnit;   

	m_dBoundBox_Bottom = 0.0;
	m_dBoundBox_Top = 0.0;
	m_dBoundBox_Left = 0.0;
	m_dBoundBox_Right = 0.0;
	
	PolyRGroup.List.SetSize(1);

	for (int i = 0; i < m_arConcOutPoly.size(); i++)
	{
		polyLineUnit.Init();
		polyLineUnit.arPoint.SetSize(m_arConcOutPoly[i].size());	
		for (int j = 0; j < m_arConcOutPoly[i].size(); j++)
		{
			polyLineUnit.arPoint[j].x = m_arConcOutPoly[i][j].x + dOffsetX;
			polyLineUnit.arPoint[j].y = m_arConcOutPoly[i][j].y + dOffsetY;

			CalcBoundingBox(polyLineUnit.arPoint[j].x, polyLineUnit.arPoint[j].y);
		}
		if(m_arConcOutPoly[i].size() == 0) continue;

		PolyRGroup.List.SetAt(0, polyLineUnit);		

		m_DrawCtrl.AddDrawUnit(i, PolyRGroup, FALSE, 0, nSnapType);
	}
	*/

	if(m_aX.size() <= 2) return;
	ASSERT(m_aX[0] == 0.0);

	std::vector<T_POINT3D> aPnt1;
	std::vector<T_POINT3D> aPnt2;
	std::vector<COLORREF> aColor;

	std::vector<double> aXGlobalDiv, aYGlobalDiv, aXValDiv, aYValDiv;
	const int nGlobalDiv = 50;
	const int nValDiv = 5;
	std::vector<COLORREF> aColorGlobalDiv, aColorValDiv;
	CSeisCvl1DElemContourUtil::Get1DElemContour(m_aX, m_aY, nGlobalDiv, nValDiv, aXGlobalDiv, aYGlobalDiv, aColorGlobalDiv, aXValDiv, aYValDiv, aColorValDiv);
	
	m_dBoundBox_Bottom = 0.0;
	m_dBoundBox_Top = 0.0;
	m_dBoundBox_Left = 0.0;
	m_dBoundBox_Right = 0.0;
	if(m_nDisplayType == 0)
	{
		MakeContourLine(aXGlobalDiv, aYGlobalDiv, aColorGlobalDiv, 0, aPnt1, aPnt2, aColor);
		MakeContourLine(aXValDiv, aYValDiv, aColorValDiv, 1, aPnt1, aPnt2, aColor);

		T_DRAW_FORMAT	DrawFormat;
		DrawFormat.bLine  = TRUE;	
		DrawFormat.bFill  = FALSE;		
		DrawFormat.Width  = 1;
		for (int i = 0; i < aColor.size(); i++)
		{    
			DrawFormat.lineColor = aColor[i];
			//DrawFormat.lineColor = RGB(0,   0,   0); 
			m_DrawCtrl.m_arDrawFormat.SetAt(i, DrawFormat);
		}
		m_nLastFormatID = (int)aColor.size() - 1;

		T_LINE_R_GROUP tLineGroup;
		tLineGroup.List.SetSize(1);
		for (int i = 0; i < aPnt1.size(); i++)
		{
			tLineGroup.List[0].P1.x = aPnt1[i].x;
			tLineGroup.List[0].P1.y = aPnt1[i].y;
			CalcBoundingBox(aPnt1[i].x, aPnt1[i].y);

			tLineGroup.List[0].P2.x = aPnt2[i].x;
			tLineGroup.List[0].P2.y = aPnt2[i].y;
			CalcBoundingBox(aPnt2[i].x, aPnt2[i].y);    
			m_DrawCtrl.AddDrawUnit(i, tLineGroup, FALSE, 0, 0);
		}
	}
	else if(m_nDisplayType == 1)
	{
		std::vector<std::vector<T_POINT3D>> aArPoly;
		std::vector<COLORREF> aColor;

		MakeContourPoly(aXGlobalDiv, aYGlobalDiv, aColorGlobalDiv, aArPoly, aColor);

		T_DRAW_FORMAT	DrawFormat;
		DrawFormat.bLine  = FALSE;	
		DrawFormat.bFill  = TRUE;		
		for (int i = 0; i < aColor.size(); i++)
		{ 
			//DrawFormat.lineColor = aColor[i];
			DrawFormat.fillColor = aColor[i];
			m_DrawCtrl.m_arDrawFormat.SetAt(i, DrawFormat);
		}
		m_nLastFormatID = (int)aColor.size() - 1;

		T_POLYGON_R_GROUP PolyRGroup;     
		T_POLYLINE_R      polyLineUnit;   
		PolyRGroup.List.SetSize(1);
		for (int i = 0; i < aArPoly.size(); i++)
		{
			polyLineUnit.Init();
			polyLineUnit.arPoint.SetSize(aArPoly[i].size());	
			for (int j = 0; j < aArPoly[i].size(); j++)
			{
				polyLineUnit.arPoint[j].x = aArPoly[i][j].x;
				polyLineUnit.arPoint[j].y = aArPoly[i][j].y;

				CalcBoundingBox(polyLineUnit.arPoint[j].x, polyLineUnit.arPoint[j].y);
			}
			if(aArPoly[i].size() == 0) continue;

			PolyRGroup.List.SetAt(0, polyLineUnit);		

			m_DrawCtrl.AddDrawUnit(i, PolyRGroup, FALSE, 0, nSnapType);
		}
	}
}

void CSeisCvlForceDirDlgDrawEditBtn::SetDrawFormat()
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
	m_CurrDrawFormat.nPenStyle = PS_DOT;
}

void CSeisCvlForceDirDlgDrawEditBtn::Redraw( BOOL bFit )
{
	if(bFit)
	{
		Fit();
	}

	if(this)
	{
		if(IsWindow(this->m_hWnd))
		{
			RedrawWindow();
		}
	}	
}

void CSeisCvlForceDirDlgDrawEditBtn::CalcBoundingBox( double dX, double dY, double* dLeft, double* dRight, double* dTop, double* dBottom )
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

void CSeisCvlForceDirDlgDrawEditBtn::StartCurrUnitSelect( CPoint CurrP )
{
	CDrawEditBtn::StartCurrUnitSelect( CurrP );
}

void CSeisCvlForceDirDlgDrawEditBtn::InProgressCurrUnitSelect( CPoint CurrP )
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

void CSeisCvlForceDirDlgDrawEditBtn::CompleteCurrUnitSelect( CPoint CurrP )
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

void CSeisCvlForceDirDlgDrawEditBtn::OnLButtonUp( UINT nFlags, CPoint point )
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

void CSeisCvlForceDirDlgDrawEditBtn::OnLButtonDown( UINT nFlags, CPoint point )
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

void CSeisCvlForceDirDlgDrawEditBtn::GetPosByCPoint( CPoint& cPt, double& dPtX, double& dPtY, int& nSnapX, int& nSnapY, BOOL bSnap )
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
}

void CSeisCvlForceDirDlgDrawEditBtn::ResetSnapPos()
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

void CSeisCvlForceDirDlgDrawEditBtn::MakeCurrLinePreview( double dPtX, double dPtY, int nSnapX, int nSnapY )
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
		tLine.P2.x = m_aSnapVertex[nSnapX].x;
		tLine.P2.y = m_aSnapVertex[nSnapX].y;
		m_CurrDrawClass_Line.m_DataR.List.Add(tLine);
	}
	if(nSnapY >= 0)
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
BOOL CSeisCvlForceDirDlgDrawEditBtn::IsSamePolygon( const DGN_GSEC_POLYGON_LIST& arOrg, const DGN_GSEC_POLYGON_LIST& arCmp, double& dDiffY, double& dDiffZ )
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



BOOL CSeisCvlForceDirDlgDrawEditBtn::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
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

void CSeisCvlForceDirDlgDrawEditBtn::OnMouseMove(UINT nFlags, CPoint point)
{
	// 화면 이동 막음
	return;
}

void CSeisCvlForceDirDlgDrawEditBtn::MakeDrawSubClass()
{
	// 자식 Class에서 그림
}
/*
void CSeisCvlForceDirDlgDrawEditBtn::SetParentSelectedKey( int nIdx, vecTndnKey& keyList, nrUINT& nrUintTarget, int idxOrigin )
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

