// DgnSeisFailureModeEditBtn.cpp: implementation of the CDgnEditBtn_TndnTemplate class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "wg_dgn.h"

#include "DgnSeisFailureModeEditBtn.h"

#include "..\wg_db\DBDoc.h"
#include "..\wg_db\UnitCtrl.h"
#include "..\wg_db\MatlDB.h"
#include "..\wg_db\SectUtil.h"
#include "..\wg_db\MathFunc.h"
#include "..\wg_db\Db_DllManager_Base.h"
#include "..\wg_db\InitCtrl.h"

#include <utility>


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define LINKKEY_ORIGIN 9999

/////////////////////////////////////////////////////////////////////////////
// CDgnEditBtn_TndnTemplate dialog
CDgnSeisFailureModeEditBtn::CDgnSeisFailureModeEditBtn()
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
	m_ColorFlame = RGB(79, 79, 79);
	m_ColorGuide = RGB(200,200,200);  
	m_ColorGuideTxt = RGB(40,40,40);  
	m_ColorLimit = RGB(120,120,120);  
	m_ColorBending = RGB(246,5,5);
	m_ColorShear = RGB(71,95,156);
	m_ColorAxisValLine = RGB(215,215,215);
	
	m_nLastFormatID = -1;

	m_dValAreaWidth = 200;
	m_dValAreaHeight = 200;

	m_dMaxX = 0.0;
	m_dMaxY = 0.0;

	m_nDisplayType = 0;
	m_nSectSnapType = 1;

	m_dFitMargin = 0.06;

	m_dFlameMargineX = 20;
	m_dFlameMargineY = 20;

	m_bBending = FALSE;
	m_bShear = FALSE;

	m_nYValExpType = 0;
}

CDgnSeisFailureModeEditBtn::~CDgnSeisFailureModeEditBtn()
{
}

void CDgnSeisFailureModeEditBtn::DoDataExchange(CDataExchange* pDX)
{
	CDrawEditBtn::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDgnSeisFailureModeEditBtn, CDrawEditBtn)
END_MESSAGE_MAP()

void CDgnSeisFailureModeEditBtn::MakeDrawUnit( BOOL bFit /*= TRUE*/ )
{
	m_DrawCtrl.RemoveDrawData(TRUE, TRUE, FALSE); // Format들을 삭제 함

	m_nLastFormatID = -1;    
	
	//MakeDrawGuideLineOld();
	//MakeDrawGuideTextOld();
	MakeDrawAxisVal();  
	MakeDrawAxisValLine();
	MakeDrawValLine();    
	MakeDrawFlame();  
	MakeDrawDot();
	MakeDrawRect();
	MakeDrawGuideBox();
	//MakeDrawGuideText();
	MakeDrawAxisText();
	MakeDrawSubClass();

	if(bFit)
	{
		m_DrawCtrl.AutoPaperFormat(m_Canvas, m_dFitMargin, m_dFitMargin, m_dFitMargin, m_dFitMargin);
	}
	m_DrawCtrl.ConvertDrawData(TRUE);
	
	OverlapChecking();
	Redraw(bFit);
}

BOOL CDgnSeisFailureModeEditBtn::SetData(FailureModeDrawD& Data, BOOL bFit)
{
	//m_pTemplateCtrl->GetLengthCtrl()->GetHeightWidthByElem(tElemKey, m_dTotalHeight, m_dTotalWidth, m_dGapY, m_dGapZ, dPos);
	//m_pTemplateCtrl->GetPosByElem(tElemKey, dPos, m_dPos);
//  m_dTotalHeight = 10;
//  m_dTotalWidth = 10;
	
	m_Data = Data;

	// aX, aY, aGuideTxt 의 갯수는 무조건 같아야 합니다.
	for (int i = 0; i < m_Data.aData.size(); i++)
	{
		if(m_Data.aData[i].aX.size() != m_Data.aData[i].aY.size())
		{
			ASSERT(0);
			return FALSE;
		}

		if(m_Data.aData[i].aX.size() != 3 || m_Data.aData[i].aY.size() != 3)
			return TRUE;
	}  
	
	ResetSnapPos();
	SetAxisValAndMax();
	if(!DataChangeByLimit())
	{
		m_Data.Init();
	}
	m_bBending = (m_Data.aData.size() > 0 && m_Data.aData[0].aX.size() > 0);
	m_bShear = m_Data.ShearData.aX.size() > 0;
	if((!m_bBending && m_bShear) || (!m_bBending && !m_bShear))
	{
		m_Data.bLimit = FALSE; // Shear 만 그릴 경우 한계선을 표현 할 필요가 없음
	}

	MakeDrawData();  
	DrawDataFitToFixSize();
	MakeDrawUnit(bFit);

	return TRUE;
}

void CDgnSeisFailureModeEditBtn::SetAxisValAndMax()
{
	m_dMaxX = -DBL_MAX;
	m_dMaxY = -DBL_MAX;
	for (int i = 0; i < m_Data.aData.size(); i++)
	{
		for (int j = 0; j < m_Data.aData[i].aX.size(); j++)
		{
			if(m_dMaxX < m_Data.aData[i].aX[j])
			{
				m_dMaxX = m_Data.aData[i].aX[j];
			}
		}

		for (int j = 0; j < m_Data.aData[i].aY.size(); j++)
		{
			if(m_dMaxY < m_Data.aData[i].aY[j])
			{
				m_dMaxY = m_Data.aData[i].aY[j];
			}
		}
	}
	for (int i = 0; i < m_Data.ShearData.aX.size(); i++)
	{
		if(m_dMaxX < m_Data.ShearData.aX[i])
		{
			m_dMaxX = m_Data.ShearData.aX[i];
		}      
	}
	for (int i = 0; i < m_Data.ShearData.aY.size(); i++)
	{
		if(m_dMaxY < m_Data.ShearData.aY[i])
		{
			m_dMaxY = m_Data.ShearData.aY[i];
		}
	}

	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	double dZero = pDoc->m_pInitCtrl->GetPreferenceRegardZero();

	// 값이 0인 경우에 대한 예외 처리
	BOOL bSetDefault = FALSE;
	if(m_dMaxX < dZero || m_dMaxY < dZero)
	{
		bSetDefault = TRUE;
	}
	if(m_Data.bLimit)
	{
		if(m_Data.dLimit < dZero)
		{
			bSetDefault = TRUE;
		}
	}
	if(bSetDefault)
	{
		m_dMaxX = m_dValAreaWidth;  
		m_dMaxY = m_dValAreaHeight;
	}
	else
	{
		if(m_dMaxX == -DBL_MAX)
		{
			m_dMaxX = m_dValAreaWidth;
		}

		if(m_dMaxY == -DBL_MAX)
		{
			m_dMaxY = m_dValAreaHeight;
		}
	}

	const int nXDiv = 5;
	const int nYDiv = 5;
	/*
	MakeAxisValType1(nXDiv, m_dMaxX, m_aAxisValX);
	MakeAxisValType1(nYDiv, m_dMaxY, m_aAxisValY);
	*/
	MakeAxisValType2(nXDiv, m_dMaxX, m_aAxisValX);
	MakeAxisValType2(nYDiv, m_dMaxY, m_aAxisValY);

	if(m_aAxisValX.size() > 0)
	{
		m_dMaxX = max(m_dMaxX, m_aAxisValX[m_aAxisValX.size() - 1]);
	}
	if(m_aAxisValY.size() > 0)
	{
		m_dMaxY = max(m_dMaxY, m_aAxisValY[m_aAxisValY.size() - 1]);
	}

	if(m_Data.bLimit)
	{
		if(m_dMaxX < m_Data.dLimit || m_Data.dLimit < 0.0)
		{
			m_Data.bLimit = FALSE;
		}
	}
}

void CDgnSeisFailureModeEditBtn::MakeAxisValType2(const int nDivNum, double dMaxVal, std::vector<double>& aAxisVal)
{
	// 분할 수는 무조건 맞추고 그 대신 공백이 생길 수 있는 방식
	aAxisVal.clear();
	if(dMaxVal == 0.0 || dMaxVal == -DBL_MAX) return;
	
	double dGap;
	dGap = dMaxVal / (double)nDivNum;

	double dScaling = 0.0;
	double dDigit = 1;
	if(dGap == 1.0)
	{
		dScaling = 1.0;
		dDigit = 1.0;
	}
	else if(dGap > 1.0)
	{
		dDigit = 1.0;
		while (true)
		{
			if((dGap / dDigit) < 10)
			{
				dScaling = dGap / dDigit;
				break;
			}
			dDigit *= 10;
		}
	} 
	else
	{
		dDigit = 0.1;
		while (true)
		{
			if((dGap / dDigit) >= 1)
			{
				dScaling = dGap / dDigit;
				break;
			}
			dDigit *= 0.1;
		}
	}
	//dScaling *= 10;
	//dDigit /= 10;

	double dZero = 1.e-4;
	dScaling = floor(dScaling);  // 내림
	dScaling++;
	dGap = dScaling * dDigit;
	for (int i = 0; i < nDivNum; i++)
	{
		aAxisVal.push_back(dGap * (i + 1));
	}
}

void CDgnSeisFailureModeEditBtn::MakeAxisValType1(const int nDivNum, double dMaxVal, std::vector<double>& aAxisVal)
{
	// 꽉차게 그리 되 분할 수는 일정하지 않게 그리는 방식

	aAxisVal.clear();
	if(dMaxVal == 0.0 || dMaxVal == -DBL_MAX) return;

	// 좌표축에 표현하는 숫자를 이쁘게 나누어 떨어지게 하는 적당한 Division 을 찾아 세팅하는 과정임
	const double aGap[] = 
	{9.0, 8.0, 7.0, 6.0, 5.0, 4.0, 3.0, 2.0, 1.0,
	0.9, 0.8, 0.7, 0.6, 0.5, 0.4, 0.3, 0.2, 0.1,  
	0.09, 0.08, 0.07, 0.06, 0.05, 0.04, 0.03, 0.02, 0.01,
	0.009, 0.008, 0.007, 0.006, 0.005, 0.004, 0.003, 0.002, 0.001
	};
	int nSize = sizeof(aGap) / sizeof(double);

	double dScaling = 0.0;
	double dDigit = 1;
	if(dMaxVal == 1.0)
	{
		dScaling = 1.0;
		dDigit = 1.0;
	}
	else if(dMaxVal > 1.0)
	{
		dDigit = 1.0;
		while (true)
		{
			if((dMaxVal / dDigit) < 10)
			{
				dScaling = dMaxVal / dDigit;
				break;
			}
			dDigit *= 10;
		}
	} 
	else
	{
		dDigit = 0.1;
		while (true)
		{
			if((dMaxVal / dDigit) >= 1)
			{
				dScaling = dMaxVal / dDigit;
				break;
			}
			dDigit *= 0.1;
		}
	}
	
	double dZero = 1.e-4;

	// 나누는 횟수가 nDivNum 에 가장 가까운 dGap 을 찾음
	double dGap = aGap[0];
	double dCeilLast = 0.0;
	for (int i = 0; i < nSize; i++)
	{
		double dCeil = ceil(dScaling / aGap[i]); // 올림
		if(dCeil - dZero > nDivNum)
		{
			if(fabs(dCeilLast - nDivNum) > fabs(dCeil - nDivNum))
			{
				dGap = aGap[i];
			}
			else
			{
				if(i == 0)
				{
					dGap = aGap[0];
				}
				else
				{
					dGap = aGap[i - 1];
				}
			}

			break;
		}
		dCeilLast = dCeil;
	}

	dGap *= dDigit;
	double dEachVal = dGap;
	while (dEachVal < dMaxVal && fabs(dEachVal - dMaxVal) > dZero)
	{
		aAxisVal.push_back(dEachVal);
		dEachVal += dGap;
	}
	aAxisVal.push_back(dEachVal);
}

void CDgnSeisFailureModeEditBtn::DrawDataFitToFixSize()
{
	for (int i = 0; i < m_aLineDraw.size(); i++)
	{
		for (int j = 0; j < m_aLineDraw[i].aX.size(); j++)
		{
			m_aLineDraw[i].aX[j] = FitToFixSize(m_dMaxX, m_aLineDraw[i].aX[j], m_dValAreaWidth);
			m_aLineDraw[i].aY[j] = FitToFixSize(m_dMaxY, m_aLineDraw[i].aY[j], m_dValAreaHeight);
		}
	}
	for (int i = 0; i < m_aDotDraw.size(); i++)
	{
		m_aDotDraw[i].dX = FitToFixSize(m_dMaxX, m_aDotDraw[i].dX, m_dValAreaWidth);
		m_aDotDraw[i].dY = FitToFixSize(m_dMaxY, m_aDotDraw[i].dY, m_dValAreaHeight);
	}
	for (int i = 0; i < m_aRectDraw.size(); i++)
	{
		m_aRectDraw[i].dX = FitToFixSize(m_dMaxX, m_aRectDraw[i].dX, m_dValAreaWidth);
		m_aRectDraw[i].dY = FitToFixSize(m_dMaxY, m_aRectDraw[i].dY, m_dValAreaHeight);
	}
	for (int i = 0; i < m_aGuideTxtDraw.size(); i++)
	{
		m_aGuideTxtDraw[i].dX = FitToFixSize(m_dMaxX, m_aGuideTxtDraw[i].dX, m_dValAreaWidth);
		m_aGuideTxtDraw[i].dY = FitToFixSize(m_dMaxY, m_aGuideTxtDraw[i].dY, m_dValAreaHeight);
	}

	m_aAxisValXDisp.resize(m_aAxisValX.size());
	for (int i = 0; i < m_aAxisValX.size(); i++)
	{
		m_aAxisValXDisp[i] = FitToFixSize(m_dMaxX, m_aAxisValX[i], m_dValAreaWidth);
	}
	
	m_aAxisValYDisp.resize(m_aAxisValY.size());
	for (int i = 0; i < m_aAxisValY.size(); i++)
	{
		m_aAxisValYDisp[i] = FitToFixSize(m_dMaxY, m_aAxisValY[i], m_dValAreaHeight);
	} 
}

FailureModeGraphTxtDraw CDgnSeisFailureModeEditBtn::GetTxtDraw(int nType, int nIdx, BOOL bIntersect)
{
	FailureModeGraphTxtDraw TxtDraw;
	// nType == 0:교차점이 아님, 1:교차점

	if(m_bBending && !m_bShear)
	{
		if(nIdx == 0)
		{
			TxtDraw.nGuideType = 1;
			TxtDraw.strX1 = _T("y");
			TxtDraw.strY1 = _T("y");
			return TxtDraw;
		}
		else if(nIdx == 1)
		{
			TxtDraw.nGuideType = 1;
			TxtDraw.strX1 = _T("u");
			TxtDraw.strY1 = _T("u");

			return TxtDraw;
		}
	}
	else
	{
		if(nType == 0)
		{
			if(nIdx == 0)
			{
				TxtDraw.nGuideType = 1;
				TxtDraw.strX1 = _T("y");
				TxtDraw.strY1 = _T("y");
				return TxtDraw;
			}
			else if(nIdx == 1 && !bIntersect)
			{
				TxtDraw.nGuideType = 2;   
				TxtDraw.strX1 = _T("c");
				TxtDraw.strX2 = _T("u");
				TxtDraw.strY1 = _T("n");      
				TxtDraw.strY2 = _T("u");

				return TxtDraw;
			}
			else if(nIdx == 1 && bIntersect)
			{
				TxtDraw.nGuideType = 1;
				TxtDraw.strX1 = _T("c");
				TxtDraw.strY1 = _T("n");

				return TxtDraw;
			}
		}
		else if(nType == 1)
		{
			TxtDraw.nGuideType = 1;
			TxtDraw.strX1 = _T("u");
			TxtDraw.strY1 = _T("u");

			return TxtDraw;
		}
	}
	
	ASSERT(0);
	return TxtDraw;
}

BOOL CDgnSeisFailureModeEditBtn::DataChangeByLimit()
{
	m_aChangeByLimit.clear();
	m_aOrgLastX.clear();
	m_aOrgLastY.clear();

	if(!m_Data.bLimit) return TRUE;

	double dZero = 1.e-4; // CMathFunc::mathIntersectLine2D 와 같은 값을 씀

	const double dBigVal = 5000000000;// 5억 정도면.. 부족하지도 않고 overflow 없이 잘 돌아 가겠지..

	double aLimitLine[2][2];
	aLimitLine[0][0] = m_Data.dLimit;
	aLimitLine[0][1] = -dBigVal;
	aLimitLine[1][0] = m_Data.dLimit;
	aLimitLine[1][1] = dBigVal;

	int nIdx;
	double dXOut, dYOut;
	int nSizeI = (int)m_Data.aData.size();
	int nSizeJ;
	for (int i = 0; i < nSizeI; i++)
	{
		nSizeJ = (int)m_Data.aData[i].aX.size();

		m_aOrgLastX.push_back(m_Data.aData[nSizeI - 1].aX[nSizeJ - 1]);
		m_aOrgLastY.push_back(m_Data.aData[nSizeI - 1].aY[nSizeJ - 1]);

		nIdx = -1;
		
		for (int j = 0; j < nSizeJ - 1; j++)
		{
			if(fabs(m_Data.aData[i].aX[j] - m_Data.dLimit) < dZero ||
				 (m_Data.aData[i].aX[j] < m_Data.dLimit && m_Data.dLimit < m_Data.aData[i].aX[j + 1]))
			{
				nIdx = j;
				break;
			}
			else if(fabs(m_Data.aData[i].aX[j+1] - m_Data.dLimit) < dZero) // j+1번째 data와 같으면 index도 j+1로... 
			{
				nIdx = j+1;
				break;
			}
		}

		if(nIdx == 0)
		{
			return FALSE; // 0~1 번 데이터 사이에 한계값이 들어오는 상황은 발생하지 않음 Draw 상에서 Error 로 보고 그리지 않음
		}

		if(nIdx == -1)
		{
			m_aChangeByLimit.push_back(FALSE);
			continue;
		}
		else
		{
			m_aChangeByLimit.push_back(TRUE);
		}

		if(CMathFunc::mathIntersectLine2D(m_Data.aData[i].aX[nIdx], m_Data.aData[i].aY[nIdx], m_Data.aData[i].aX[nIdx + 1], m_Data.aData[i].aY[nIdx + 1],
			aLimitLine[0][0], aLimitLine[0][1], aLimitLine[1][0], aLimitLine[1][1], dXOut, dYOut, FALSE))
		{  
			if(nIdx == 0)
			{
				m_Data.aData[i].aX[1] = dXOut;
				m_Data.aData[i].aY[1] = dYOut;

				m_Data.aData[i].aX[2] = dXOut + (dZero * 100);
				m_Data.aData[i].aY[2] = dYOut;
			}
			else if(nIdx == 1)
			{
				m_Data.aData[i].aX[2] = dXOut;
				m_Data.aData[i].aY[2] = dYOut;
			}
			else ASSERT(0);
		}
		else
		{
			ASSERT(0); // 바로 위에 nIdx 값 세팅 하는 부분에 버그가 있거나 CMathFunc::mathIntersectLine2D 함수 부르는 곳에 버그가 있을 가능성이 높음
		}
	}
	return TRUE;
}

void CDgnSeisFailureModeEditBtn::MakeDrawDataLimitLine()
{
	if(m_Data.bLimit)
	{
		// 한계치 세로선 처리( 향후에 m_aLineDraw 를 대상으로 FitToFixSize() 를 불러도 고정 Size 가 되게 하기 위한 Y 값을 세팅 함
		const double dUpperFixed = 15;
		FMEditBtnLineDraw EachLineDraw;

		EachLineDraw.Init();
		EachLineDraw.nLineType = 2;
		EachLineDraw.Color = m_ColorLimit;

		EachLineDraw.aX.push_back(m_Data.dLimit);
		EachLineDraw.aY.push_back(0.0);

		EachLineDraw.aX.push_back(m_Data.dLimit);
		EachLineDraw.aY.push_back(((m_dValAreaHeight + dUpperFixed) * m_dMaxY) / m_dValAreaHeight);

		m_aLineDraw.push_back(EachLineDraw);
	}
}

void CDgnSeisFailureModeEditBtn::MakeDrawAxisText()
{
	CString strX, strY, strUnitLen, strUnitForc;

	CDBDoc* pDoc = CDBDoc::GetDocPoint();  
	pDoc->m_pUnitCtrl->GetUnitSystemLength(strUnitLen);
	pDoc->m_pUnitCtrl->GetUnitSystemForce(strUnitForc);

	strX.Format(_T("%s (%s)"), _LSX(변위 Δ), strUnitLen);

	if(m_bBending && m_bShear)	strY.Format(_T("%s (%s)"), _LSX(단면력 P), strUnitForc);
	else if(m_bBending)			strY.Format(_T("%s (%s)"), _LSX(휨강도 F), strUnitForc);
	else if(m_bShear)			strY.Format(_T("%s (%s)"), _LSX(전단강도 Vn), strUnitForc);
	else                        strY.Format(_T("%s (%s)"), _LSX(단면력 P), strUnitForc);

	T_TEXT_FORMAT	TextFormat;    
	T_TEXT_R_GROUP tTextGroup;
	tTextGroup.List.SetSize(1);

	TextFormat.Size = 14;  
	TextFormat.fontColor = RGB(0, 0, 0);
	TextFormat.positionType = EN_POSITION_TYPE_LEFT_TOP;
	m_DrawCtrl.m_arTextFormat.SetAt(++m_nLastFormatID, TextFormat);
	
	double dXPos = (m_dValAreaWidth + m_dFlameMargineX) / 2.0 - 23;
	tTextGroup.List[0].PPosition.x = dXPos;
	tTextGroup.List[0].PPosition.y = -15;
	tTextGroup.List[0].szText = strX;
	m_DrawCtrl.AddDrawUnit(m_nLastFormatID, tTextGroup);

	TextFormat.Size = 13;  
	TextFormat.positionType = EN_POSITION_TYPE_LEFT_BOTTOM;
	TextFormat.nEscapement = 900;
	m_DrawCtrl.m_arTextFormat.SetAt(++m_nLastFormatID, TextFormat);
	
	double dPosX = 0.0;;
	if(m_nYValExpType == 0)
	{
		dPosX = -21;
	}
	else if(m_nYValExpType == 1)
	{
		dPosX = -30.5;
	}
	else if(m_nYValExpType == 2)
	{
		dPosX = -36;
	}
	else
	{
		ASSERT(0);
		dPosX = -21;
	}

	double dTest = 0.0;

	tTextGroup.List[0].PPosition.x = dPosX + dTest;
	tTextGroup.List[0].PPosition.y = (m_dValAreaHeight + m_dFlameMargineY) / 2.0 - 35;
	tTextGroup.List[0].szText = strY;
	m_DrawCtrl.AddDrawUnit(m_nLastFormatID, tTextGroup);
}

void CDgnSeisFailureModeEditBtn::MakeDrawData()
{
	m_aLineDraw.clear();
	m_aDotDraw.clear();
	m_aRectDraw.clear();
	m_aGuideTxtDraw.clear();
	
	FMEditBtnLineDraw EachLineDraw;
	FMEditBtnDotDraw EachDotDraw;
	FMEditBtnGuideTxtDraw EachGuideTxtDraw;
	FMEditBtnRectDraw EachRectDraw;

	for (int i = 0; i < m_Data.aData.size(); i++)
	{
		for (int j = 0; j < m_Data.aData[i].aX.size(); j++)
		{
			EachDotDraw.dX = m_Data.aData[i].aX[j];
			EachDotDraw.dY = m_Data.aData[i].aY[j];
			m_aDotDraw.push_back(EachDotDraw);
		}
	}

	ASSERT(m_Data.ShearData.aX.size() == m_Data.ShearData.aY.size());  
	if(m_Data.ShearData.aX.size() == 0)
	{
		// Failure 곡선이 없으면 그냥 다 직선형태로 그리고 끝
		double dXLast = 0.0;
		double dYLast = 0.0;

		for (int i = 0; i < m_Data.aData.size(); i++)
		{
			if(m_Data.aData[i].aX.size() < 2)
			{
				continue;
			}

			EachLineDraw.Init();
			EachLineDraw.nLineType = 0;
			EachLineDraw.Color = m_ColorBending;

			EachLineDraw.aX = m_Data.aData[i].aX;
			EachLineDraw.aY = m_Data.aData[i].aY;
			m_aLineDraw.push_back(EachLineDraw);
			 
			EachGuideTxtDraw.Init();
			//GuideTxtDraw.TxtColor = m_Data.aData[i].Color;
			EachGuideTxtDraw.TxtColor = m_ColorFlame;
			EachGuideTxtDraw.GuideColor = m_ColorGuide;      
			for (int j = 1; j < m_Data.aData[i].aX.size(); j++)
			{
				EachGuideTxtDraw.dX = m_Data.aData[i].aX[j];
				EachGuideTxtDraw.dY = m_Data.aData[i].aY[j];
				EachGuideTxtDraw.GuideTxt = GetTxtDraw(0, j - 1, FALSE); 
				m_aGuideTxtDraw.push_back(EachGuideTxtDraw);
			}

			dXLast = EachGuideTxtDraw.dX;
			dYLast = EachGuideTxtDraw.dY;
			
			// 한계치 이후 점선 처리
			if(m_aChangeByLimit.size() > 0 && m_aChangeByLimit[i])
			{
				ASSERT(m_Data.bLimit);
				EachLineDraw.Init();
				EachLineDraw.nLineType = 2;
				EachLineDraw.Color = m_ColorBending;

				EachLineDraw.aX.push_back(dXLast);
				EachLineDraw.aY.push_back(dYLast);

				EachLineDraw.aX.push_back(m_aOrgLastX[i]);
				EachLineDraw.aY.push_back(m_aOrgLastY[i]);

				m_aLineDraw.push_back(EachLineDraw);

				m_aGuideTxtDraw[m_aGuideTxtDraw.size() - 1].bDrawXGuideLine = FALSE;
			}
		}
		MakeDrawDataLimitLine();
		return;
	}

	EachLineDraw.Init();
	EachLineDraw.nLineType = 0;
	EachLineDraw.Color = m_ColorShear;

	EachLineDraw.aX = m_Data.ShearData.aX;
	EachLineDraw.aY = m_Data.ShearData.aY;
	m_aLineDraw.push_back(EachLineDraw);
		
	for (int i = 0; i < m_Data.ShearData.aX.size(); i++)
	{
		EachRectDraw.dX = m_Data.ShearData.aX[i];
		EachRectDraw.dY = m_Data.ShearData.aY[i];
		m_aRectDraw.push_back(EachRectDraw);
	}

	double dXIntersect, dYIntersect, dXLast, dYLast;
	BOOL bIntersect;
	int nIntersectIdx;
	for (int i = 0; i < m_Data.aData.size(); i++)
	{
		if(m_Data.aData[i].aX.size() < 2)
		{
			continue;
		}

		// Intersect 이전 부분 처리
		EachGuideTxtDraw.Init();
		//GuideTxtDraw.TxtColor = m_Data.aData[i].Color;
		EachGuideTxtDraw.TxtColor = m_ColorGuideTxt;    
		EachGuideTxtDraw.GuideColor = m_ColorGuide;    

		bIntersect = FALSE;
		EachLineDraw.Init();
		EachLineDraw.nLineType = 0;
		EachLineDraw.Color = m_ColorBending;
		for (int j = 0; j < m_Data.aData[i].aX.size() - 1; j++)
		{
			EachLineDraw.aX.push_back(m_Data.aData[i].aX[j]);
			EachLineDraw.aY.push_back(m_Data.aData[i].aY[j]);
			
			if(FindIntersect(m_Data.aData[i].aX[j], m_Data.aData[i].aY[j], m_Data.aData[i].aX[j + 1], m_Data.aData[i].aY[j + 1], m_Data.ShearData.aX, m_Data.ShearData.aY, dXIntersect, dYIntersect))
			{
				EachLineDraw.aX.push_back(dXIntersect);
				EachLineDraw.aY.push_back(dYIntersect);

				EachRectDraw.dX = dXIntersect;
				EachRectDraw.dY = dYIntersect;
				m_aRectDraw.push_back(EachRectDraw);
				
				EachGuideTxtDraw.dX = dXIntersect;
				EachGuideTxtDraw.dY = dYIntersect;
				EachGuideTxtDraw.GuideTxt = GetTxtDraw(0, j, TRUE); 
				m_aGuideTxtDraw.push_back(EachGuideTxtDraw);

				bIntersect = TRUE;
				nIntersectIdx = j;
				break;
			}
			else
			{
				EachLineDraw.aX.push_back(m_Data.aData[i].aX[j + 1]);
				EachLineDraw.aY.push_back(m_Data.aData[i].aY[j + 1]);

				EachGuideTxtDraw.dX = m_Data.aData[i].aX[j + 1];
				EachGuideTxtDraw.dY = m_Data.aData[i].aY[j + 1];
				EachGuideTxtDraw.GuideTxt = GetTxtDraw(0, j, FALSE); 
				m_aGuideTxtDraw.push_back(EachGuideTxtDraw);
			}
		}
		ASSERT(EachLineDraw.aX.size() >= 2);    
		m_aLineDraw.push_back(EachLineDraw);    

		// Intersect 지점 이후 처리
		if(bIntersect) // 원래 Intersect 지점부터 점선으로 그리는 스펙이였음.. 흐름을 바꾸지 않고 처리 하기 위해 로직은 바꾸지 않음
		{
			EachLineDraw.Init();
			EachLineDraw.nLineType = 0;
			EachLineDraw.Color = m_ColorBending;     

			for (int j = nIntersectIdx; j < m_Data.aData[i].aX.size() - 1; j++)
			{
				if(j == nIntersectIdx)
				{
					EachLineDraw.aX.push_back(dXIntersect);
					EachLineDraw.aY.push_back(dYIntersect);
				}
				else
				{
					EachLineDraw.aX.push_back(m_Data.aData[i].aX[j]);
					EachLineDraw.aY.push_back(m_Data.aData[i].aY[j]);
				}
				EachLineDraw.aX.push_back(m_Data.aData[i].aX[j + 1]);
				EachLineDraw.aY.push_back(m_Data.aData[i].aY[j + 1]);

				EachGuideTxtDraw.dX = m_Data.aData[i].aX[j + 1];
				EachGuideTxtDraw.dY = m_Data.aData[i].aY[j + 1];
				EachGuideTxtDraw.GuideTxt = GetTxtDraw(1, 0, TRUE);  
				m_aGuideTxtDraw.push_back(EachGuideTxtDraw);
			}
			dXLast = EachGuideTxtDraw.dX;
			dYLast = EachGuideTxtDraw.dY;

			ASSERT(EachLineDraw.aX.size() >= 2);
			m_aLineDraw.push_back(EachLineDraw);
		}
		else
		{
			dXLast = EachLineDraw.aX[EachLineDraw.aX.size() - 1];
			dYLast = EachLineDraw.aY[EachLineDraw.aY.size() - 1];      
		}

		// 한계치 이후 점선 처리
		if(m_aChangeByLimit.size() > 0 && m_aChangeByLimit[i])
		{
			ASSERT(m_Data.bLimit);
			EachLineDraw.Init();
			EachLineDraw.nLineType = 2;
			EachLineDraw.Color = m_ColorBending;

			EachLineDraw.aX.push_back(dXLast);
			EachLineDraw.aY.push_back(dYLast);

			EachLineDraw.aX.push_back(m_aOrgLastX[i]);
			EachLineDraw.aY.push_back(m_aOrgLastY[i]);
			
			m_aLineDraw.push_back(EachLineDraw);

			m_aGuideTxtDraw[m_aGuideTxtDraw.size() - 1].bDrawXGuideLine = FALSE;
		}
	}

	MakeDrawDataLimitLine();
}

BOOL CDgnSeisFailureModeEditBtn::FindIntersect(double dP1X, double dP1Y, double dP2X, double dP2Y, std::vector<double>& aX, std::vector<double>& aY, double &dXOut, double &dYOut )
{
	dXOut = 0.0;
	dYOut = 0.0;

	ASSERT(aX.size() == aY.size());
	for (int i = 0; i < aX.size() - 1; i++)
	{
		if(dP1X > aX[i + 1]) continue;

		if(CMathFunc::mathIntersectLine2D(dP1X, dP1Y, dP2X, dP2Y, aX[i], aY[i], aX[i + 1], aY[i + 1], dXOut, dYOut, TRUE))
		{
			return TRUE;
		}
	}
	return FALSE;
}

double CDgnSeisFailureModeEditBtn::FitToFixSize(double dMax, double dVal, double dFixSize)
{
	if(dMax == 0) return 0.0;

	return (dVal / dMax) * dFixSize;
}

/*
void CDgnSeisFailureModeEditBtn::MakeDrawValue()
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

void CDgnSeisFailureModeEditBtn::MakeDrawValLine()
{
	T_DRAW_FORMAT	DrawFormat;
	DrawFormat.bLine  = TRUE;	
	DrawFormat.bFill  = FALSE;	  

	T_LINE_R_GROUP tLineGroup;
	tLineGroup.List.SetSize(1);  
	
	for (int i = 0; i < m_aLineDraw.size(); i++)
	{
		DrawFormat.lineColor = m_aLineDraw[i].Color;
		if(m_aLineDraw[i].nLineType == 0)
		{
			DrawFormat.Width  = 2;     
			DrawFormat.nPenStyle = PS_SOLID;
		}
		else if(m_aLineDraw[i].nLineType == 1)
		{
			DrawFormat.Width  = 1;     
			DrawFormat.nPenStyle = PS_SOLID;
		}
		else if(m_aLineDraw[i].nLineType == 2)
		{
			DrawFormat.Width  = 1;     
			DrawFormat.nPenStyle = PS_DOT;
		}
		else
		{
			ASSERT(0);
		}
		m_DrawCtrl.m_arDrawFormat.SetAt(++m_nLastFormatID, DrawFormat);

		tLineGroup.List.SetSize(m_aLineDraw[i].aX.size() - 1);
		for (int j = 0; j < m_aLineDraw[i].aX.size() - 1; j++)
		{
			tLineGroup.List[j].P1.x = m_aLineDraw[i].aX[j];
			tLineGroup.List[j].P1.y = m_aLineDraw[i].aY[j];

			tLineGroup.List[j].P2.x = m_aLineDraw[i].aX[j + 1];
			tLineGroup.List[j].P2.y = m_aLineDraw[i].aY[j + 1];
		}
		m_DrawCtrl.AddDrawUnit(m_nLastFormatID, tLineGroup);
	}
}

void CDgnSeisFailureModeEditBtn::MakeDrawDot()
{
	T_DRAW_FORMAT	DrawFormat;
	DrawFormat.bLine  = TRUE;	
	DrawFormat.bFill  = TRUE;		
	DrawFormat.Width  = 1;     
	DrawFormat.fillColor = m_ColorBending;

	T_CIRCLE_R_GROUP tCircleGroup;
	tCircleGroup.List.SetSize(1);
	tCircleGroup.List[0].Radius = 3;

	for (int i = 0; i < m_aDotDraw.size(); i++)
	{    
		m_DrawCtrl.m_arDrawFormat.SetAt(++m_nLastFormatID, DrawFormat);

		tCircleGroup.List[0].CenterP.x = m_aDotDraw[i].dX;
		tCircleGroup.List[0].CenterP.y = m_aDotDraw[i].dY;    
		m_DrawCtrl.AddDrawUnit(m_nLastFormatID, tCircleGroup);
	}
}

void CDgnSeisFailureModeEditBtn::MakeDrawRect()
{
	T_DRAW_FORMAT	DrawFormat;
	DrawFormat.bLine  = TRUE;	
	DrawFormat.bFill  = TRUE;		
	DrawFormat.Width  = 1;     
	DrawFormat.lineColor = RGB(0, 0, 0);    
	DrawFormat.fillColor = m_ColorShear;

	T_RECT_R_GROUP tRectGroup;
	tRectGroup.List.SetSize(1);
	tRectGroup.List[0].dThik = 1;

	double dSize = 3.0;
	
	for (int i = 0; i < m_aRectDraw.size(); i++)
	{    
		m_DrawCtrl.m_arDrawFormat.SetAt(++m_nLastFormatID, DrawFormat);

		tRectGroup.List[0].P1.x = m_aRectDraw[i].dX - dSize;
		tRectGroup.List[0].P1.y = m_aRectDraw[i].dY - dSize;

		tRectGroup.List[0].P2.x = m_aRectDraw[i].dX + dSize;
		tRectGroup.List[0].P2.y = m_aRectDraw[i].dY + dSize;

		m_DrawCtrl.AddDrawUnit(m_nLastFormatID, tRectGroup);
	}  
}

void CDgnSeisFailureModeEditBtn::MakeDrawAxisValLine()
{
	T_DRAW_FORMAT	DrawFormat;
	DrawFormat.bLine  = TRUE;	
	DrawFormat.Width  = 1;
	DrawFormat.nPenStyle = PS_SOLID;
	DrawFormat.lineColor = m_ColorAxisValLine;
	m_DrawCtrl.m_arDrawFormat.SetAt(++m_nLastFormatID, DrawFormat);

	T_LINE_R EachLine;

	T_LINE_R_GROUP tLineGroup;  
	tLineGroup.List.RemoveAll();
	for (int i = 0; i < m_aAxisValXDisp.size(); i++)
	{ 
		EachLine.P1.x = m_aAxisValXDisp[i];
		EachLine.P1.y = 0.0;

		EachLine.P2.x = m_aAxisValXDisp[i];
		EachLine.P2.y = m_dValAreaHeight + m_dFlameMargineY;

		tLineGroup.List.Add(EachLine);      
	}
	for (int i = 0; i < m_aAxisValYDisp.size(); i++)
	{ 
		EachLine.P1.x = 0.0;
		EachLine.P1.y = m_aAxisValYDisp[i];

		EachLine.P2.x = m_dValAreaWidth + m_dFlameMargineX;
		EachLine.P2.y = m_aAxisValYDisp[i];

		tLineGroup.List.Add(EachLine);      
	}

	m_DrawCtrl.AddDrawUnit(m_nLastFormatID, tLineGroup);
}

void CDgnSeisFailureModeEditBtn::MakeDrawAxisVal()
{
	// String
	T_TEXT_FORMAT	TextFormat;    
	T_TEXT_R_GROUP tTextGroup;
	tTextGroup.List.SetSize(1);

	TextFormat.Size = 12;  
	TextFormat.fontColor = RGB(0, 0, 0);
	TextFormat.positionType = EN_POSITION_TYPE_CENTER_TOP;
	m_DrawCtrl.m_arTextFormat.SetAt(++m_nLastFormatID, TextFormat);

	CString strTmp;
	for (int i = 0; i < m_aAxisValXDisp.size(); i++)
	{
		tTextGroup.List[0].PPosition.x = m_aAxisValXDisp[i];
		tTextGroup.List[0].PPosition.y = -3.2;
		strTmp.Format(_T("%g"), m_aAxisValX[i]);
		tTextGroup.List[0].szText = strTmp;
		m_DrawCtrl.AddDrawUnit(m_nLastFormatID, tTextGroup);
	}
	
	TextFormat.positionType = EN_POSITION_TYPE_RIGHT_VCENTER;
	m_DrawCtrl.m_arTextFormat.SetAt(++m_nLastFormatID, TextFormat);
	for (int i = 0; i < m_aAxisValYDisp.size(); i++)
	{
		tTextGroup.List[0].PPosition.x = -5;
		tTextGroup.List[0].PPosition.y = m_aAxisValYDisp[i] + 9.0;
		strTmp.Format(_T("%g"), m_aAxisValY[i]);
		tTextGroup.List[0].szText = strTmp;
		m_DrawCtrl.AddDrawUnit(m_nLastFormatID, tTextGroup);
	}

	if(strTmp.GetLength() <= 3)
	{
		m_nYValExpType = 0;
	}
	else if(strTmp.GetLength() <= 5)
	{
		m_nYValExpType = 1;
	}
	else
	{
		m_nYValExpType = 2;
	}
}

void CDgnSeisFailureModeEditBtn::MakeDrawGuideTextDelta(double dX, double dY, COLORREF Color, 
																												double dLeftInc /*= 0.0*/, double dRightInc /*= 0.0*/, double dTopInc /*= 0.0*/, double dBotInc /*= 0.0*/)
{
	T_DRAW_FORMAT	DrawFormat;
	DrawFormat.bLine  = TRUE;	
	DrawFormat.lineColor = Color;
	DrawFormat.Width  = 1;     
	DrawFormat.nPenStyle = PS_SOLID;
	m_DrawCtrl.m_arDrawFormat.SetAt(++m_nLastFormatID, DrawFormat);

	T_LINE_R_GROUP tLineGroup;
	tLineGroup.List.SetSize(3);  
	
	tLineGroup.List[0].P1.x = dX + dRightInc;
	tLineGroup.List[0].P1.y = dY - dBotInc;
	tLineGroup.List[0].P2.x = dX + dRightInc;
	tLineGroup.List[0].P2.y = dY + 6 + dTopInc;
	
	tLineGroup.List[1].P1.x = tLineGroup.List[0].P2.x;
	tLineGroup.List[1].P1.y = tLineGroup.List[0].P2.y;
	tLineGroup.List[1].P2.x = dX - 5 - dLeftInc;
	tLineGroup.List[1].P2.y = dY - dBotInc;

	tLineGroup.List[2].P1.x = tLineGroup.List[1].P2.x;
	tLineGroup.List[2].P1.y = tLineGroup.List[1].P2.y;
	tLineGroup.List[2].P2.x = tLineGroup.List[0].P1.x;
	tLineGroup.List[2].P2.y = tLineGroup.List[0].P1.y;
	m_DrawCtrl.AddDrawUnit(m_nLastFormatID, tLineGroup);
}

void CDgnSeisFailureModeEditBtn::MakeDrawGuideText()
{
	T_TEXT_FORMAT	TextFormat;  
	TextFormat.positionType = EN_POSITION_TYPE_LEFT_VCENTER;

	T_TEXT_R_GROUP tTextGroup;
	tTextGroup.List.SetSize(1);

	int nFormatStr;

	double dX, dY;
	double dXMove, dYMove;

	BOOL bIntesrsectExist = FALSE;
	if(m_aGuideTxtDraw.size() == 3)
	{
		bIntesrsectExist = TRUE;
	}

	dXMove = dYMove = 0.0;
	for (int i = 0; i < m_aGuideTxtDraw.size(); i++)
	{
		if(m_aGuideTxtDraw[i].GuideTxt.nGuideType == 0) continue;

		dX = m_aGuideTxtDraw[i].dX;
		dY = m_aGuideTxtDraw[i].dY;
		
		TextFormat.fontColor = m_aGuideTxtDraw[i].TxtColor;
		if(m_aGuideTxtDraw[i].GuideTxt.nGuideType == 1)
		{ 
			if(bIntesrsectExist && i == 1)
			{
				//dXMove = -7.0;
				dXMove = 0.0;
				dYMove = -10;
			}
			else
			{
				dXMove = 0.0;
				dYMove = 5;
			}

			// X
			TextFormat.Size = 10;  
			m_DrawCtrl.m_arTextFormat.SetAt(++m_nLastFormatID, TextFormat);
			tTextGroup.List[0].PPosition.x = dX - 5 + dXMove;
			tTextGroup.List[0].PPosition.y = dY + dYMove;
			tTextGroup.List[0].szText = m_aGuideTxtDraw[i].GuideTxt.strX1;
			m_DrawCtrl.AddDrawUnit(m_nLastFormatID, tTextGroup);

			MakeDrawGuideTextDelta(dX - 7 + dXMove, dY + dYMove, m_aGuideTxtDraw[i].TxtColor);

			// Y
			TextFormat.Size = 10;  
			m_DrawCtrl.m_arTextFormat.SetAt(++m_nLastFormatID, TextFormat);    
			tTextGroup.List[0].PPosition.x = dX + 7 + dXMove;
			tTextGroup.List[0].PPosition.y = dY + dYMove;
			tTextGroup.List[0].szText = m_aGuideTxtDraw[i].GuideTxt.strY1;
			m_DrawCtrl.AddDrawUnit(m_nLastFormatID, tTextGroup);

			TextFormat.Size = 14;  
			m_DrawCtrl.m_arTextFormat.SetAt(++m_nLastFormatID, TextFormat);    
			tTextGroup.List[0].PPosition.x -= 5;
			tTextGroup.List[0].PPosition.y += 3;
			tTextGroup.List[0].szText = _T("F");
			m_DrawCtrl.AddDrawUnit(m_nLastFormatID, tTextGroup);      

			tTextGroup.List[0].PPosition.x -= 3;
			tTextGroup.List[0].szText = _T(",");
			m_DrawCtrl.AddDrawUnit(m_nLastFormatID, tTextGroup);      
		}
		else if(m_aGuideTxtDraw[i].GuideTxt.nGuideType == 2)
		{
			dXMove = 0.0;
			dYMove = 5;      

			// x
			TextFormat.Size = 10;  
			TextFormat.fontColor = m_aGuideTxtDraw[i].TxtColor;
			m_DrawCtrl.m_arTextFormat.SetAt(++m_nLastFormatID, TextFormat);
			nFormatStr = m_nLastFormatID;
			tTextGroup.List[0].PPosition.x = dX - 6 + dXMove;
			tTextGroup.List[0].PPosition.y = dY + dYMove;
			tTextGroup.List[0].szText = m_aGuideTxtDraw[i].GuideTxt.strX2;
			m_DrawCtrl.AddDrawUnit(nFormatStr, tTextGroup);

			MakeDrawGuideTextDelta(dX - 8 + dXMove, dY + dYMove, m_aGuideTxtDraw[i].TxtColor);

			TextFormat.Size = 14;  
			TextFormat.fontColor = m_ColorFlame;
			m_DrawCtrl.m_arTextFormat.SetAt(++m_nLastFormatID, TextFormat);    
			tTextGroup.List[0].PPosition.x = dX - 19 + dXMove;
			tTextGroup.List[0].PPosition.y = dY + dYMove;
			tTextGroup.List[0].szText = _T("=");
			m_DrawCtrl.AddDrawUnit(m_nLastFormatID, tTextGroup);

			tTextGroup.List[0].PPosition.x = dX - 23 + dXMove;
			tTextGroup.List[0].PPosition.y = dY + dYMove;
			tTextGroup.List[0].szText = m_aGuideTxtDraw[i].GuideTxt.strX1;
			m_DrawCtrl.AddDrawUnit(nFormatStr, tTextGroup);

			MakeDrawGuideTextDelta(dX - 25 + dXMove, dY + dYMove, m_aGuideTxtDraw[i].TxtColor);

			// Y
			TextFormat.Size = 10;  
			TextFormat.fontColor = m_aGuideTxtDraw[i].TxtColor;    
			m_DrawCtrl.m_arTextFormat.SetAt(++m_nLastFormatID, TextFormat);    
			tTextGroup.List[0].PPosition.x = dX + 22 + dXMove;
			tTextGroup.List[0].PPosition.y = dY + dYMove;
			tTextGroup.List[0].szText = m_aGuideTxtDraw[i].GuideTxt.strY2;
			m_DrawCtrl.AddDrawUnit(m_nLastFormatID, tTextGroup);

			TextFormat.Size = 14;  
			m_DrawCtrl.m_arTextFormat.SetAt(++m_nLastFormatID, TextFormat);    
			tTextGroup.List[0].PPosition.x -= 5;
			tTextGroup.List[0].PPosition.y += 3;
			tTextGroup.List[0].szText = _T("F");
			m_DrawCtrl.AddDrawUnit(m_nLastFormatID, tTextGroup);

			tTextGroup.List[0].PPosition.x -= 6;
			tTextGroup.List[0].szText = _T("=");
			m_DrawCtrl.AddDrawUnit(m_nLastFormatID, tTextGroup);

			TextFormat.Size = 10;  
			m_DrawCtrl.m_arTextFormat.SetAt(++m_nLastFormatID, TextFormat);    
			tTextGroup.List[0].PPosition.x -=4;
			tTextGroup.List[0].PPosition.y = dY + dYMove;
			tTextGroup.List[0].szText = m_aGuideTxtDraw[i].GuideTxt.strY1;
			m_DrawCtrl.AddDrawUnit(m_nLastFormatID, tTextGroup);

			TextFormat.Size = 14;  
			m_DrawCtrl.m_arTextFormat.SetAt(++m_nLastFormatID, TextFormat);    
			tTextGroup.List[0].PPosition.x -= 5;
			tTextGroup.List[0].PPosition.y += 3;
			tTextGroup.List[0].szText = _T("F");
			m_DrawCtrl.AddDrawUnit(m_nLastFormatID, tTextGroup);

			tTextGroup.List[0].PPosition.x -= 3;
			tTextGroup.List[0].szText = _T(",");
			m_DrawCtrl.AddDrawUnit(m_nLastFormatID, tTextGroup);     
		}
		else
		{
			ASSERT(0);
		}    
	}

	// X축
	MakeDrawGuideTextDelta(m_dValAreaWidth + m_dFlameMargineX - 2, -11, m_ColorFlame);

	// Y축
	TextFormat.Size = 14;  
	TextFormat.fontColor = m_ColorFlame;
	m_DrawCtrl.m_arTextFormat.SetAt(++m_nLastFormatID, TextFormat);    
	tTextGroup.List[0].PPosition.x = -11;
	tTextGroup.List[0].PPosition.y = m_dValAreaHeight + m_dFlameMargineY - 5;
	tTextGroup.List[0].szText = _T("F");
	m_DrawCtrl.AddDrawUnit(m_nLastFormatID, tTextGroup);
}

void CDgnSeisFailureModeEditBtn::MakeDrawGuideTextOld()
{
	ASSERT(0); // 이제 안씀 스펙이 어떻게 바뀔지 모르니 기존 방식 코드 남겨둠

	T_TEXT_FORMAT	TextFormat;  
	TextFormat.positionType = EN_POSITION_TYPE_LEFT_VCENTER;

	T_TEXT_R_GROUP tTextGroup;
	tTextGroup.List.SetSize(1);

	int nFormatStr;

	for (int i = 0; i < m_aGuideTxtDraw.size(); i++)
	{
		if(m_aGuideTxtDraw[i].GuideTxt.nGuideType == 0) continue;

		if(m_aGuideTxtDraw[i].GuideTxt.nGuideType == 1)
		{
			// X축
			TextFormat.Size = 10;  
			TextFormat.fontColor = m_aGuideTxtDraw[i].TxtColor;
			m_DrawCtrl.m_arTextFormat.SetAt(++m_nLastFormatID, TextFormat);
			tTextGroup.List[0].PPosition.x = m_aGuideTxtDraw[i].dX + 2;
			tTextGroup.List[0].PPosition.y = -11;
			tTextGroup.List[0].szText = m_aGuideTxtDraw[i].GuideTxt.strX1;
			m_DrawCtrl.AddDrawUnit(m_nLastFormatID, tTextGroup);

			MakeDrawGuideTextDelta(m_aGuideTxtDraw[i].dX, -11, m_aGuideTxtDraw[i].TxtColor);

			// Y축
			TextFormat.Size = 10;  
			TextFormat.fontColor = m_aGuideTxtDraw[i].TxtColor;    
			m_DrawCtrl.m_arTextFormat.SetAt(++m_nLastFormatID, TextFormat);    
			tTextGroup.List[0].PPosition.x = -6;
			tTextGroup.List[0].PPosition.y = m_aGuideTxtDraw[i].dY - 2;
			tTextGroup.List[0].szText = m_aGuideTxtDraw[i].GuideTxt.strY1;
			m_DrawCtrl.AddDrawUnit(m_nLastFormatID, tTextGroup);

			TextFormat.Size = 14;  
			m_DrawCtrl.m_arTextFormat.SetAt(++m_nLastFormatID, TextFormat);    
			tTextGroup.List[0].PPosition.x -= 5;
			tTextGroup.List[0].PPosition.y += 3;
			tTextGroup.List[0].szText = _T("F");
			m_DrawCtrl.AddDrawUnit(m_nLastFormatID, tTextGroup);
		}
		else if(m_aGuideTxtDraw[i].GuideTxt.nGuideType == 2)
		{
			// X축
			TextFormat.Size = 10;  
			TextFormat.fontColor = m_aGuideTxtDraw[i].TxtColor;
			m_DrawCtrl.m_arTextFormat.SetAt(++m_nLastFormatID, TextFormat);
			nFormatStr = m_nLastFormatID;
			tTextGroup.List[0].PPosition.x = m_aGuideTxtDraw[i].dX + 11;
			tTextGroup.List[0].PPosition.y = -11;
			tTextGroup.List[0].szText = m_aGuideTxtDraw[i].GuideTxt.strX2;
			m_DrawCtrl.AddDrawUnit(nFormatStr, tTextGroup);

			MakeDrawGuideTextDelta(m_aGuideTxtDraw[i].dX + 9, -11, m_aGuideTxtDraw[i].TxtColor);

			TextFormat.Size = 14;  
			TextFormat.fontColor = m_ColorFlame;
			m_DrawCtrl.m_arTextFormat.SetAt(++m_nLastFormatID, TextFormat);    
			tTextGroup.List[0].PPosition.x = m_aGuideTxtDraw[i].dX - 2;
			tTextGroup.List[0].PPosition.y = -11;
			tTextGroup.List[0].szText = _T("=");
			m_DrawCtrl.AddDrawUnit(m_nLastFormatID, tTextGroup);

			tTextGroup.List[0].PPosition.x = m_aGuideTxtDraw[i].dX - 6;
			tTextGroup.List[0].PPosition.y = -11;
			tTextGroup.List[0].szText = m_aGuideTxtDraw[i].GuideTxt.strX1;
			m_DrawCtrl.AddDrawUnit(nFormatStr, tTextGroup);

			MakeDrawGuideTextDelta(m_aGuideTxtDraw[i].dX - 8, -11, m_aGuideTxtDraw[i].TxtColor);

			// Y축
			TextFormat.Size = 10;  
			TextFormat.fontColor = m_aGuideTxtDraw[i].TxtColor;    
			m_DrawCtrl.m_arTextFormat.SetAt(++m_nLastFormatID, TextFormat);    
			tTextGroup.List[0].PPosition.x = -6;
			tTextGroup.List[0].PPosition.y = m_aGuideTxtDraw[i].dY - 2;
			tTextGroup.List[0].szText = m_aGuideTxtDraw[i].GuideTxt.strY2;
			m_DrawCtrl.AddDrawUnit(m_nLastFormatID, tTextGroup);

			TextFormat.Size = 14;  
			m_DrawCtrl.m_arTextFormat.SetAt(++m_nLastFormatID, TextFormat);    
			tTextGroup.List[0].PPosition.x -= 5;
			tTextGroup.List[0].PPosition.y += 3;
			tTextGroup.List[0].szText = _T("F");
			m_DrawCtrl.AddDrawUnit(m_nLastFormatID, tTextGroup);

			tTextGroup.List[0].PPosition.x -= 6;
			tTextGroup.List[0].szText = _T("=");
			m_DrawCtrl.AddDrawUnit(m_nLastFormatID, tTextGroup);

			TextFormat.Size = 10;  
			m_DrawCtrl.m_arTextFormat.SetAt(++m_nLastFormatID, TextFormat);    
			tTextGroup.List[0].PPosition.x -=4;
			tTextGroup.List[0].PPosition.y = m_aGuideTxtDraw[i].dY - 2;
			tTextGroup.List[0].szText = m_aGuideTxtDraw[i].GuideTxt.strY1;
			m_DrawCtrl.AddDrawUnit(m_nLastFormatID, tTextGroup);

			TextFormat.Size = 14;  
			m_DrawCtrl.m_arTextFormat.SetAt(++m_nLastFormatID, TextFormat);    
			tTextGroup.List[0].PPosition.x -= 5;
			tTextGroup.List[0].PPosition.y += 3;
			tTextGroup.List[0].szText = _T("F");
			m_DrawCtrl.AddDrawUnit(m_nLastFormatID, tTextGroup);
		}
		else
		{
			ASSERT(0);
		}    
	}

	// X축
	MakeDrawGuideTextDelta(m_dValAreaWidth + m_dFlameMargineX - 2, -11, m_ColorFlame);

	// Y축
	TextFormat.Size = 14;  
	TextFormat.fontColor = m_ColorFlame;
	m_DrawCtrl.m_arTextFormat.SetAt(++m_nLastFormatID, TextFormat);    
	tTextGroup.List[0].PPosition.x = -11;
	tTextGroup.List[0].PPosition.y = m_dValAreaHeight + m_dFlameMargineY - 5;
	tTextGroup.List[0].szText = _T("F");
	m_DrawCtrl.AddDrawUnit(m_nLastFormatID, tTextGroup);
}

void CDgnSeisFailureModeEditBtn::MakeDrawGuideLineOld()
{
	ASSERT(0); // 이제 안씀 스펙이 어떻게 바뀔지 모르니 기존 방식 코드 남겨둠

	T_DRAW_FORMAT	DrawFormat;
	DrawFormat.bLine  = TRUE;	
	DrawFormat.Width  = 1;     

	T_LINE_R EachLine;

	T_LINE_R_GROUP tLineGroup;  
	for (int i = 0; i < m_aGuideTxtDraw.size(); i++)
	{
		if(m_aGuideTxtDraw[i].GuideTxt.nGuideType == 0) continue;

		DrawFormat.lineColor = m_aGuideTxtDraw[i].GuideColor;
		DrawFormat.nPenStyle = PS_DOT;
		m_DrawCtrl.m_arDrawFormat.SetAt(++m_nLastFormatID, DrawFormat);

		tLineGroup.List.RemoveAll();
		if(m_aGuideTxtDraw[i].bDrawYGuideLine)
		{
			EachLine.P1.x = 0.0;
			EachLine.P1.y = m_aGuideTxtDraw[i].dY;

			EachLine.P2.x = m_aGuideTxtDraw[i].dX;
			EachLine.P2.y = m_aGuideTxtDraw[i].dY;

			tLineGroup.List.Add(EachLine);
		}
		if(m_aGuideTxtDraw[i].bDrawXGuideLine)
		{
			EachLine.P1.x = m_aGuideTxtDraw[i].dX;
			EachLine.P1.y = m_aGuideTxtDraw[i].dY;

			EachLine.P2.x = m_aGuideTxtDraw[i].dX;
			EachLine.P2.y = 0.0;

			tLineGroup.List.Add(EachLine);
		}

		if(tLineGroup.List.GetSize() > 0)
		{
			m_DrawCtrl.AddDrawUnit(m_nLastFormatID, tLineGroup);
		}
	}
}

void CDgnSeisFailureModeEditBtn::MakeDrawFlame()
{
	T_DRAW_FORMAT	DrawFormat;
	DrawFormat.bLine  = TRUE;	
	//DrawFormat.bFill  = FALSE;		  
	DrawFormat.lineColor = m_ColorFlame;
	//DrawFormat.fillColor = RGB(118,   0,   118);
	DrawFormat.startArrowType = EN_ARROW_TYPE_NONE;
	DrawFormat.endArrowType = EN_ARROW_TYPE_ARROW;
	DrawFormat.dRWidth        = 3;
	DrawFormat.dRHeight       = 2;
	DrawFormat.Width  = 2;     
	m_DrawCtrl.m_arDrawFormat.SetAt(++m_nLastFormatID, DrawFormat);
	
	m_dBoundBox_Bottom = 0.0;
	m_dBoundBox_Top = 0.0;
	m_dBoundBox_Left = 0.0;
	m_dBoundBox_Right = 0.0;

	CalcBoundingBox(0, 0);
	CalcBoundingBox(m_dValAreaWidth + m_dFlameMargineX, m_dValAreaHeight + m_dFlameMargineY);

	T_ARROWLINE_R_GROUP tArrowGroup;
	tArrowGroup.List.SetSize(1);  

	T_LINE_R tLine;
	tLine.P1.x = 0;
	tLine.P1.y = 0;
	tLine.P2.x = m_dValAreaWidth + m_dFlameMargineX;
	tLine.P2.y = 0;
	tArrowGroup.List[0] = tLine;
	m_DrawCtrl.AddDrawUnit(m_nLastFormatID, tArrowGroup);
	
	tLine.P1.x = 0;
	tLine.P1.y = 0;
	tLine.P2.x = 0;
	tLine.P2.y = m_dValAreaHeight + m_dFlameMargineY;
	tArrowGroup.List[0] = tLine;
	m_DrawCtrl.AddDrawUnit(m_nLastFormatID, tArrowGroup);
}

void CDgnSeisFailureModeEditBtn::MakeDrawGuideBox()
{ 
	if(!m_bBending && !m_bShear) return;

	if(m_bBending && m_bShear)
	{
		MakeDrawGuideBoxAll();
	}
	else
	{
		// 그리지 말자고 해서 뺌
		//int nType = m_bBending ? 0 : 1;
		//MakeDrawGuideBoxEachOne(nType);
	}
}

void CDgnSeisFailureModeEditBtn::MakeDrawGuideBoxAll()
{
	double dChangeByTextX; // Axis Text를 넣으면서 조율한 값들임
	dChangeByTextX = 4;

	double dBoxWidth = 82 + dChangeByTextX;
	double dBoxHeight = 27;

	double dLeftBotX, dLeftBotY;
	dLeftBotX = 136 - dChangeByTextX;
	//dLeftBotY = 10;
	dLeftBotY = m_dValAreaHeight + m_dFlameMargineY - dBoxHeight -  10;

	T_DRAW_FORMAT	DrawFormat;
	DrawFormat.bLine  = TRUE;	
	DrawFormat.bFill  = TRUE;		  
	DrawFormat.lineColor = RGB(0,   0,   0);
	DrawFormat.fillColor = RGB(255, 255, 255);
	DrawFormat.Width  = 1;     
	m_DrawCtrl.m_arDrawFormat.SetAt(++m_nLastFormatID, DrawFormat);

	T_POINT tPnt, tPntDot, tPntRect;
	T_POLYLINE_R tPolygon;

	// Box
	T_POLYGON_R_GROUP tPolygonGroup;
	tPolygonGroup.List.SetSize(1);      
	tPolygon.arPoint.RemoveAll();
	tPnt.x = dLeftBotX;
	tPnt.y = dLeftBotY;
	tPolygon.arPoint.Add(tPnt);
	tPnt.x = dLeftBotX + dBoxWidth;
	tPnt.y = dLeftBotY;
	tPolygon.arPoint.Add(tPnt);
	tPnt.x = dLeftBotX + dBoxWidth;
	tPnt.y = dLeftBotY + dBoxHeight;
	tPolygon.arPoint.Add(tPnt);
	tPnt.x = dLeftBotX;
	tPnt.y = dLeftBotY + dBoxHeight;
	tPolygon.arPoint.Add(tPnt);
	tPolygonGroup.List[0] = tPolygon;
	m_DrawCtrl.AddDrawUnit(m_nLastFormatID, tPolygonGroup);

	// Line
	DrawFormat.lineColor = m_ColorShear;
	DrawFormat.Width  = 2;
	m_DrawCtrl.m_arDrawFormat.SetAt(++m_nLastFormatID, DrawFormat);
	T_POLYLINE_R_GROUP tPolyLineGroup;
	tPolyLineGroup.List.SetSize(1);   
	tPolygon.arPoint.RemoveAll();
	tPnt.x = dLeftBotX + 3;
	tPnt.y = dLeftBotY + 7;
	tPntRect.x = tPnt.x;
	tPntRect.y = tPnt.y;
	tPolygon.arPoint.Add(tPnt);
	tPnt.x = dLeftBotX + 13;
	tPnt.y = dLeftBotY + 7;
	tPntRect.x = (tPntRect.x + tPnt.x) / 2.0;
	tPntRect.y = (tPntRect.y + tPnt.y) / 2.0;
	tPolygon.arPoint.Add(tPnt);
	tPolyLineGroup.List[0] = tPolygon;
	m_DrawCtrl.AddDrawUnit(m_nLastFormatID, tPolyLineGroup);

	DrawFormat.lineColor = m_ColorBending;
	m_DrawCtrl.m_arDrawFormat.SetAt(++m_nLastFormatID, DrawFormat);

	tPolygon.arPoint.RemoveAll();
	tPnt.x = dLeftBotX + 3;
	tPnt.y = dLeftBotY + 18;
	tPntDot.x = tPnt.x;
	tPntDot.y = tPnt.y;
	tPolygon.arPoint.Add(tPnt);
	tPnt.x = dLeftBotX + 13;
	tPnt.y = dLeftBotY + 18;
	tPntDot.x = (tPntDot.x + tPnt.x) / 2.0;
	tPntDot.y = (tPntDot.y + tPnt.y) / 2.0;
	tPolygon.arPoint.Add(tPnt);
	tPolyLineGroup.List[0] = tPolygon;
	m_DrawCtrl.AddDrawUnit(m_nLastFormatID, tPolyLineGroup);

	// Rect  
	tPntRect.x++;  // 보기 좋게 하기 위한 조절
	//tPntRect.y--;
	T_RECT_R_GROUP tRectGroup;
	tRectGroup.List.SetSize(1);
	tRectGroup.List[0].dThik = 1;
	double dRectSize = 3.0;
	DrawFormat.Width  = 1;
	DrawFormat.lineColor = RGB(0, 0, 0);
	DrawFormat.fillColor = m_ColorShear;
	m_DrawCtrl.m_arDrawFormat.SetAt(++m_nLastFormatID, DrawFormat);
	tRectGroup.List[0].P1.x = tPntRect.x - dRectSize;
	tRectGroup.List[0].P1.y = tPntRect.y - dRectSize;
	tRectGroup.List[0].P2.x = tPntRect.x + dRectSize;
	tRectGroup.List[0].P2.y = tPntRect.y + dRectSize;
	m_DrawCtrl.AddDrawUnit(m_nLastFormatID, tRectGroup);

	// Dot
	tPntDot.x++;  // 보기 좋게 하기 위한 조절
	T_CIRCLE_R_GROUP tCircleGroup;
	tCircleGroup.List.SetSize(1);
	tCircleGroup.List[0].Radius = 3;
	DrawFormat.lineColor = RGB(0, 0, 0);
	DrawFormat.fillColor = m_ColorBending;
	m_DrawCtrl.m_arDrawFormat.SetAt(++m_nLastFormatID, DrawFormat);
	tCircleGroup.List[0].CenterP.x = tPntDot.x;
	tCircleGroup.List[0].CenterP.y = tPntDot.y;
	m_DrawCtrl.AddDrawUnit(m_nLastFormatID, tCircleGroup);

	// String
	T_TEXT_FORMAT	TextFormat;  
	TextFormat.positionType = EN_POSITION_TYPE_LEFT_VCENTER;
	T_TEXT_R_GROUP tTextGroup;
	tTextGroup.List.SetSize(1);

	TextFormat.Size = 14;
	TextFormat.fontColor = RGB(0, 0, 0);
	m_DrawCtrl.m_arTextFormat.SetAt(++m_nLastFormatID, TextFormat);

	// Text를 고치면 어차피 Box Size 도 조절 해야 하니깐 Message로 뽑지 않음(국가별 처리를 할려면 #define 으로 해서 다른 Size 의 Box 를 그려줘야 하지 않을까??)
	tTextGroup.List[0].PPosition.x = dLeftBotX + 16;
	tTextGroup.List[0].PPosition.y = dLeftBotY + 7;
	tTextGroup.List[0].szText = _LSX(전단 성능 곡선);
	m_DrawCtrl.AddDrawUnit(m_nLastFormatID, tTextGroup);

	tTextGroup.List[0].PPosition.x = dLeftBotX + 16;
	tTextGroup.List[0].PPosition.y = dLeftBotY + 18;
	tTextGroup.List[0].szText = _LSX(휨 성능 곡선);
	m_DrawCtrl.AddDrawUnit(m_nLastFormatID, tTextGroup);
}

void CDgnSeisFailureModeEditBtn::MakeDrawGuideBoxEachOne(int nType)
{
	// nType = 0:Bending, 1:Shear 

	double dChangeByTextX; // Axis Text를 넣으면서 조율한 값들임
	dChangeByTextX = 4;

	double dBoxWidth = 82 + dChangeByTextX;
	double dBoxHeight = 15;

	double dLeftBotX, dLeftBotY;
	dLeftBotX = 136 - dChangeByTextX;
	//dLeftBotY = 10;
	dLeftBotY = m_dValAreaHeight + m_dFlameMargineY - dBoxHeight -  10;

	T_DRAW_FORMAT	DrawFormat;
	DrawFormat.bLine  = TRUE;	
	DrawFormat.bFill  = TRUE;		  
	DrawFormat.lineColor = RGB(0,   0,   0);
	DrawFormat.fillColor = RGB(255, 255, 255);
	DrawFormat.Width  = 1;     
	m_DrawCtrl.m_arDrawFormat.SetAt(++m_nLastFormatID, DrawFormat);

	T_POINT tPnt, tPntAvg;
	T_POLYLINE_R tPolygon;

	// Box  

	T_POLYGON_R_GROUP tPolygonGroup;
	tPolygonGroup.List.SetSize(1);      
	tPolygon.arPoint.RemoveAll();
	tPnt.x = dLeftBotX;
	tPnt.y = dLeftBotY;
	tPolygon.arPoint.Add(tPnt);
	tPnt.x = dLeftBotX + dBoxWidth;
	tPnt.y = dLeftBotY;
	tPolygon.arPoint.Add(tPnt);
	tPnt.x = dLeftBotX + dBoxWidth;
	tPnt.y = dLeftBotY + dBoxHeight;
	tPolygon.arPoint.Add(tPnt);
	tPnt.x = dLeftBotX;
	tPnt.y = dLeftBotY + dBoxHeight;
	tPolygon.arPoint.Add(tPnt);
	tPolygonGroup.List[0] = tPolygon;
	m_DrawCtrl.AddDrawUnit(m_nLastFormatID, tPolygonGroup);

	// Line
	if(nType == 0)
	{
		DrawFormat.lineColor = m_ColorBending;
	}
	else if(nType == 1)
	{
		DrawFormat.lineColor = m_ColorShear;
	}
	else
	{
		ASSERT(0);
	}
	
	DrawFormat.Width  = 2;
	m_DrawCtrl.m_arDrawFormat.SetAt(++m_nLastFormatID, DrawFormat);
	T_POLYLINE_R_GROUP tPolyLineGroup;
	tPolyLineGroup.List.SetSize(1);   
	tPolygon.arPoint.RemoveAll();
	tPnt.x = dLeftBotX + 3;
	tPnt.y = dLeftBotY + 7;
	tPntAvg.x = tPnt.x;
	tPntAvg.y = tPnt.y;
	tPolygon.arPoint.Add(tPnt);
	tPnt.x = dLeftBotX + 13;
	tPnt.y = dLeftBotY + 7;
	tPntAvg.x = (tPntAvg.x + tPnt.x) / 2.0;
	tPntAvg.y = (tPntAvg.y + tPnt.y) / 2.0;
	tPolygon.arPoint.Add(tPnt);
	tPolyLineGroup.List[0] = tPolygon;
	m_DrawCtrl.AddDrawUnit(m_nLastFormatID, tPolyLineGroup);
	
	DrawFormat.Width  = 1;
	DrawFormat.lineColor = RGB(0, 0, 0);
	if(nType == 0)
	{
		// Dot
		tPntAvg.x++;  // 보기 좋게 하기 위한 조절
		T_CIRCLE_R_GROUP tCircleGroup;
		tCircleGroup.List.SetSize(1);
		tCircleGroup.List[0].Radius = 3;
		DrawFormat.fillColor = m_ColorBending;
		m_DrawCtrl.m_arDrawFormat.SetAt(++m_nLastFormatID, DrawFormat);
		tCircleGroup.List[0].CenterP.x = tPntAvg.x;
		tCircleGroup.List[0].CenterP.y = tPntAvg.y;
		m_DrawCtrl.AddDrawUnit(m_nLastFormatID, tCircleGroup);
	}
	else if(nType == 1)
	{
		// Rect  
		tPntAvg.x++;  // 보기 좋게 하기 위한 조절
		//tPntAvg.y--;
		T_RECT_R_GROUP tRectGroup;
		tRectGroup.List.SetSize(1);
		tRectGroup.List[0].dThik = 1;
		double dRectSize = 3.0;        
		DrawFormat.fillColor = m_ColorShear;
		m_DrawCtrl.m_arDrawFormat.SetAt(++m_nLastFormatID, DrawFormat);
		tRectGroup.List[0].P1.x = tPntAvg.x - dRectSize;
		tRectGroup.List[0].P1.y = tPntAvg.y - dRectSize;
		tRectGroup.List[0].P2.x = tPntAvg.x + dRectSize;
		tRectGroup.List[0].P2.y = tPntAvg.y + dRectSize;
		m_DrawCtrl.AddDrawUnit(m_nLastFormatID, tRectGroup);
	}

	// String
	T_TEXT_FORMAT	TextFormat;  
	TextFormat.positionType = EN_POSITION_TYPE_LEFT_VCENTER;
	T_TEXT_R_GROUP tTextGroup;
	tTextGroup.List.SetSize(1);

	TextFormat.Size = 14;
	TextFormat.fontColor = RGB(0, 0, 0);
	m_DrawCtrl.m_arTextFormat.SetAt(++m_nLastFormatID, TextFormat);

	// Text를 고치면 어차피 Box Size 도 조절 해야 하니깐 Message로 뽑지 않음(국가별 처리를 할려면 #define 으로 해서 다른 Size 의 Box 를 그려줘야 하지 않을까??)
	tTextGroup.List[0].PPosition.x = dLeftBotX + 16;
	tTextGroup.List[0].PPosition.y = dLeftBotY + 7;
	if(nType == 0)
	{
		tTextGroup.List[0].szText = _LSX(휨 성능 곡선);    
	}
	else if(nType == 1)
	{
		tTextGroup.List[0].szText = _LSX(전단 성능 곡선);
	}
	else
	{
		ASSERT(0);
	}
	m_DrawCtrl.AddDrawUnit(m_nLastFormatID, tTextGroup);
}

void CDgnSeisFailureModeEditBtn::SetDrawFormat()
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

void CDgnSeisFailureModeEditBtn::Redraw( BOOL bFit )
{
	if(bFit)
	{
		m_DrawCtrl.AutoPaperFormat(m_Canvas, m_dFitMargin, m_dFitMargin, m_dFitMargin, m_dFitMargin);
	}

	if(this)
	{
		if(IsWindow(this->m_hWnd))
		{
			RedrawWindow();
		}
	}	
}

void CDgnSeisFailureModeEditBtn::CalcBoundingBox( double dX, double dY, double* dLeft, double* dRight, double* dTop, double* dBottom )
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

void CDgnSeisFailureModeEditBtn::StartCurrUnitSelect( CPoint CurrP )
{
	CDrawEditBtn::StartCurrUnitSelect( CurrP );
}

void CDgnSeisFailureModeEditBtn::InProgressCurrUnitSelect( CPoint CurrP )
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
		CDrawEditBtn::InProgressCurrUnitSelect( CurrP );  
	//}
}

void CDgnSeisFailureModeEditBtn::CompleteCurrUnitSelect( CPoint CurrP )
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

void CDgnSeisFailureModeEditBtn::OnLButtonUp( UINT nFlags, CPoint point )
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

void CDgnSeisFailureModeEditBtn::OnLButtonDown( UINT nFlags, CPoint point )
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

void CDgnSeisFailureModeEditBtn::GetPosByCPoint( CPoint& cPt, double& dPtX, double& dPtY, int& nSnapX, int& nSnapY, BOOL bSnap )
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

void CDgnSeisFailureModeEditBtn::ResetSnapPos()
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

void CDgnSeisFailureModeEditBtn::MakeCurrLinePreview( double dPtX, double dPtY, int nSnapX, int nSnapY )
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
BOOL CDgnSeisFailureModeEditBtn::IsSamePolygon( const DGN_GSEC_POLYGON_LIST& arOrg, const DGN_GSEC_POLYGON_LIST& arCmp, double& dDiffY, double& dDiffZ )
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



BOOL CDgnSeisFailureModeEditBtn::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
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

void CDgnSeisFailureModeEditBtn::OnMouseMove(UINT nFlags, CPoint point)
{
	// 화면 이동 막음
	return;
}

void CDgnSeisFailureModeEditBtn::MakeDrawSubClass()
{
	// 자식 Class에서 그림
}
/*
void CDgnSeisFailureModeEditBtn::SetParentSelectedKey( int nIdx, vecTndnKey& keyList, nrUINT& nrUintTarget, int idxOrigin )
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


