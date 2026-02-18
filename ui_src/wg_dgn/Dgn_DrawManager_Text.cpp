#include "stdafx.h"
#include "Dgn_DrawManager_Text.h"
#include "..\wg_db\wg_db_MathLib.h"
#include "..\wg_db\MathFunc.h"

CDgn_DrawManager_Text::CDgn_DrawManager_Text(void)
{
	m_bEMF=FALSE;
}


CDgn_DrawManager_Text::~CDgn_DrawManager_Text(void)
{
}

// MQC 10770 : Gen/Civil(멀티바이트 문자열 집합 사용)에서 생성된 CDC에 DgnEngine(유니코드)가 글씨를 쓰면, 문자열이 2칸당 1글자씩 출력되는 문제
// Moment-Curvature 관련 데이터를 Gen/Civil로 넘겨줘서 직접 출력하도록 수정
// DesignEngine의 void CDgnDrawBase_DrawFunc::Draw_Chart(CDC* pDC, T_DGN_CHART_FORMAT TCHARtFormat, T_DGN_CHART& dgnChart) 함수에서 텍스트 출력부분만 이전해옴
void CDgn_DrawManager_Text::Draw_DgnText(CDC* pDC, CArray<T_DGN_CHART_FORMAT,T_DGN_CHART_FORMAT>* paDrawFormat, CArray<T_DGN_CHART_GROUP,T_DGN_CHART_GROUP>* paDgnChartGroup)
{
	for(int i=0; i<paDrawFormat->GetSize(); i++)
	{
		T_DGN_CHART_FORMAT* pFormat = &paDrawFormat->GetAt(i);
		T_DGN_CHART_GROUP* pChtGrp = &paDgnChartGroup->GetAt(i);

		for(int j=0; j<pChtGrp->List.GetSize(); j++)
		{
			if(pChtGrp->List[j].frame.arText.GetSize() > 0)
				Draw_Text(pDC, pFormat->frame.fontColor, RGB(255,255,255), FALSE, pFormat->frame.fontSize, CString(pFormat->frame.strFont), EN_DGN_POSITION_TYPE_CENTER_VCENTER, pChtGrp->List[j].frame.arText);

			DGN_POSITION_TYPE LabelPosi = EN_DGN_POSITION_TYPE_CENTER_VCENTER;
			// - xAxis 
			if(pChtGrp->List[j].Info.xAxis.bAxis)
			{
				if(pChtGrp->List[j].xAxis.arText.GetSize() > 0)
					Draw_Text(pDC, pFormat->xAxis.fontColor, RGB(255,255,255), FALSE, pFormat->xAxis.fontSize, CString(pFormat->xAxis.strFont), LabelPosi, pChtGrp->List[j].xAxis.arText, pFormat->xAxis.nEscapement);

				if(pChtGrp->List[j].Info.xAxis.bAxisName) Draw_Text(pDC, pFormat->xAxis.fontColor_Title, RGB(255,255,255), FALSE, pFormat->xAxis.fontSize_Title, CString(pFormat->xAxis.strFont_Title), EN_DGN_POSITION_TYPE_CENTER_VCENTER, pChtGrp->List[j].xAxis.title);
			}

			// - yAxis 
			if(pChtGrp->List[j].Info.yAxis.bAxis)
			{
				if(pChtGrp->List[j].yAxis.arText.GetSize() > 0)
					Draw_Text(pDC, pFormat->yAxis.fontColor, RGB(255,255,255), FALSE, pFormat->yAxis.fontSize, CString(pFormat->yAxis.strFont), LabelPosi, pChtGrp->List[j].yAxis.arText, pFormat->yAxis.nEscapement);

				if(pChtGrp->List[j].Info.yAxis.bAxisName) Draw_Text(pDC, pFormat->yAxis.fontColor_Title, RGB(255,255,255), FALSE, pFormat->yAxis.fontSize_Title, CString(pFormat->yAxis.strFont_Title), EN_DGN_POSITION_TYPE_CENTER_VCENTER, pChtGrp->List[j].yAxis.title, 900);
			}

			// Remark
			if(pChtGrp->List[j].Info.bRemark)
			{
				if(pChtGrp->List[j].remark.arText.GetSize() > 0)
					Draw_Text(pDC, pFormat->remark.fontColor, RGB(255,255,255), FALSE, pFormat->remark.fontSize, CString(pFormat->remark.strFont), EN_DGN_POSITION_TYPE_LEFT_VCENTER, pChtGrp->List[j].remark.arText);
			}

			// Remark외에 Data Name을 표시하는 경우
			int nSize = (int)pChtGrp->List[j].arData.GetSize();
			for(int k=0 ; k<nSize ; k++)
			{
				T_DGN_TEXT DgnTextD;  DgnTextD.Initialize();
				if(pChtGrp->List[j].arData[k].Info.nNameType == EN_DGN_CHART_DATANAME_POSI_START && pChtGrp->List[j].arData[k].arPoint.GetSize() > 0)		
				{ DgnTextD.PPosition = pChtGrp->List[j].arData[k].arPoint[0]; }
				else if(pChtGrp->List[j].arData[k].Info.nNameType == EN_DGN_CHART_DATANAME_POSI_END && pChtGrp->List[j].arData[k].arPoint.GetSize() > 0)		
				{ DgnTextD.PPosition = pChtGrp->List[j].arData[k].arPoint[pChtGrp->List[j].arData[k].arPoint.GetSize()-1]; }
				else continue;

				DgnTextD.strText = pChtGrp->List[j].arData[k].Info.strName;
				Draw_Text(pDC, pFormat->remark.fontColor, RGB(255,255,255), FALSE, pFormat->remark.fontSize, CString(pFormat->remark.strFont), pChtGrp->List[j].arData[k].Info.nNamePosiType, DgnTextD);
			}
		}
	}  
}

void CDgn_DrawManager_Text::Draw_Text(CDC* pDC, T_DGN_TEXT_FORMAT &textFormat, T_DGN_TEXT &dgnText)
{ 
	Draw_Text(pDC, textFormat.fontColor, textFormat.fillColor, textFormat.bFill, textFormat.Size, CString(textFormat.strFont), textFormat.positionType, dgnText, textFormat.nEscapement, textFormat.bHalfWidth); 
}

void CDgn_DrawManager_Text::Draw_Text(CDC* pDC, T_DGN_TEXT_FORMAT &textFormat, CArray<T_DGN_TEXT,T_DGN_TEXT&>& arText)
{ 
	Draw_Text(pDC, textFormat.fontColor, textFormat.fillColor, textFormat.bFill, textFormat.Size, CString(textFormat.strFont), textFormat.positionType, arText, textFormat.nEscapement, textFormat.bHalfWidth);
}

void CDgn_DrawManager_Text::Draw_Text(CDC* pDC, COLORREF fontColor, COLORREF fillColor, BOOL bFill, int fontSize, CString strFont, DGN_POSITION_TYPE enPosiType, T_DGN_TEXT &dgnText, int nEscapement, BOOL bHalfWidth)
{// 겹지정된 함수를 사용하기위한 변환함수임
	CArray<T_DGN_TEXT, T_DGN_TEXT&> arText;
	arText.SetSize(1);
	arText.SetAt(0, dgnText);
	Draw_Text(pDC, fontColor, fillColor, bFill, fontSize, CString(strFont), enPosiType, arText, nEscapement, bHalfWidth);
}

void CDgn_DrawManager_Text::Draw_Text(CDC* pDC, COLORREF fontColor, COLORREF fillColor, BOOL bFill, int fontSize, CString strFont, DGN_POSITION_TYPE enPosiType, CArray<T_DGN_TEXT,T_DGN_TEXT&>& arTextOrg, int nEscapement, BOOL bHalfWidth)
{
	if(fontSize <= 0) return;

	int fontSize_sub = (int)(0.6*fontSize);

	CPen*    pOldPen;
	CBrush*  pOldBrh;
	CFont*   pOldFont;
	CPen     LinePen(PS_SOLID, 1, fontColor);
	CBrush   FillBrush(fillColor);
	CFont    Font;
	COLORREF oldTextColor;
	COLORREF oldBkColor;
	//if(nEscapement == 0) fontSize += m_nFontAddSize_RZero;

	//if (IsNationCodeUS())
	//  Font.CreateFont(fontSize*0.8, (bHalfWidth? fontSize*0.8/2 : 0), nEscapement*(m_bEMF? -1 : 1), 0, FW_DONTCARE, FALSE, FALSE, FALSE, HANGEUL_CHARSET, OUT_DEFAULT_PRECIS, CLIP_LH_ANGLES, DEFAULT_QUALITY, DEFAULT_PITCH, strFont);
	//else
	Font.CreateFont(fontSize, (bHalfWidth? fontSize/2 : 0), nEscapement*(m_bEMF? -1 : 1), 0, FW_DONTCARE, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_LH_ANGLES, DEFAULT_QUALITY, DEFAULT_PITCH, CString(strFont));

	oldTextColor = pDC->SetTextColor(fontColor);
	if(bFill) oldBkColor   = pDC->SetBkColor(fillColor);
	pOldPen = pDC->SelectObject(&LinePen);
	pOldFont = pDC->SelectObject(&Font); 
	if(bFill) pOldBrh = pDC->SelectObject(&FillBrush);

	CArray<T_DGN_TEXT,T_DGN_TEXT&> arTextLine, arText, arText_sub;
	ConvertNextLineText(fontSize, enPosiType, nEscapement, arTextOrg, arTextLine);
	ConvertMathText(pDC, fontSize, fontSize_sub, enPosiType, nEscapement, arTextLine, arText, arText_sub);

	INT_PTR i, nSize;
	nSize = arText.GetSize();  
	for(i=0 ; i<nSize; i++)
	{ 
		CSize textSize(0,0);
		if(arText[i].strText != _T("")) textSize = m_bEMF ? pDC->GetOutputTextExtent(CString(arText[i].strText)) : pDC->GetTextExtent(CString(arText[i].strText));

		double dFactor[2];
		dFactor[0] = cos(nEscapement/10.0 * CMathFunc::m_trrad);
		dFactor[1] = sin(nEscapement/10.0 * CMathFunc::m_trrad);
		CPoint LeftTopPoint;    
		if     (enPosiType==EN_DGN_POSITION_TYPE_LEFT_TOP      ) 
		{ LeftTopPoint.x = arText[i].PPosition.x;  
		LeftTopPoint.y = arText[i].PPosition.y; }
		else if(enPosiType==EN_DGN_POSITION_TYPE_LEFT_VCENTER  ) 
		{ LeftTopPoint.x = math_lib::round_off(arText[i].PPosition.x + (                            - textSize.cy/2.0 * dFactor[1]));  
		LeftTopPoint.y = math_lib::round_off(arText[i].PPosition.y + (                              textSize.cy/2.0 * dFactor[0])*(m_bEMF? 1 : -1)); }
		else if(enPosiType==EN_DGN_POSITION_TYPE_LEFT_BOTTOM   ) 
		{ 
			LeftTopPoint.x = math_lib::round_off(arText[i].PPosition.x + (                            - textSize.cy     * dFactor[1]));  
			LeftTopPoint.y = math_lib::round_off(arText[i].PPosition.y + (                              textSize.cy     * dFactor[0])*(m_bEMF? 1 : -1)); }
		else if(enPosiType==EN_DGN_POSITION_TYPE_CENTER_TOP    )
		{ 
			LeftTopPoint.x = math_lib::round_off(arText[i].PPosition.x + (-textSize.cx/2.0*dFactor[0]                               ));  
			LeftTopPoint.y = math_lib::round_off(arText[i].PPosition.y + (-textSize.cx/2.0*dFactor[1]                               )*(m_bEMF? 1 : -1)); }
		else if(enPosiType==EN_DGN_POSITION_TYPE_CENTER_VCENTER)
		{ 
			LeftTopPoint.x = math_lib::round_off(arText[i].PPosition.x + (-textSize.cx/2.0*dFactor[0] - textSize.cy/2.0 * dFactor[1]));  
			LeftTopPoint.y = math_lib::round_off(arText[i].PPosition.y + (-textSize.cx/2.0*dFactor[1] + textSize.cy/2.0 * dFactor[0])*(m_bEMF? 1 : -1)); }
		else if(enPosiType==EN_DGN_POSITION_TYPE_CENTER_BOTTOM )
		{ 
			LeftTopPoint.x = math_lib::round_off(arText[i].PPosition.x + (-textSize.cx/2.0*dFactor[0] - textSize.cy     * dFactor[1]));  
			LeftTopPoint.y = math_lib::round_off(arText[i].PPosition.y + (-textSize.cx/2.0*dFactor[1] + textSize.cy     * dFactor[0])*(m_bEMF? 1 : -1)); }
		else if(enPosiType==EN_DGN_POSITION_TYPE_RIGHT_TOP     )
		{ 
			LeftTopPoint.x = math_lib::round_off(arText[i].PPosition.x + (-textSize.cx*dFactor[0]                                   ));  
			LeftTopPoint.y = math_lib::round_off(arText[i].PPosition.y + (-textSize.cx*dFactor[1]                                   )*(m_bEMF? 1 : -1)); }
		else if(enPosiType==EN_DGN_POSITION_TYPE_RIGHT_VCENTER )
		{
			LeftTopPoint.x = math_lib::round_off(arText[i].PPosition.x + (-textSize.cx*dFactor[0]     - textSize.cy/2.0 * dFactor[1]));  
			LeftTopPoint.y = math_lib::round_off(arText[i].PPosition.y + (-textSize.cx*dFactor[1]     + textSize.cy/2.0 * dFactor[0])*(m_bEMF? 1 : -1)); }
		else if(enPosiType==EN_DGN_POSITION_TYPE_RIGHT_BOTTOM  )
		{ 
			LeftTopPoint.x = math_lib::round_off(arText[i].PPosition.x + (-textSize.cx*dFactor[0]     - textSize.cy     * dFactor[1]));  
			LeftTopPoint.y = math_lib::round_off(arText[i].PPosition.y + (-textSize.cx*dFactor[1]     + textSize.cy     * dFactor[0])*(m_bEMF? 1 : -1)); }
		else ASSERT(0);

		if(bFill) pDC->SetBkMode(OPAQUE);
		else      pDC->SetBkMode(TRANSPARENT);

		pDC->TextOut(LeftTopPoint.x, LeftTopPoint.y, CString(arText[i].strText));    
	}

	Font.DeleteObject();
	//if (IsNationCodeUS())
	//	Font.CreateFont((int)(fontSize_sub*0.8), (bHalfWidth? (int)(fontSize*0.8/2) : 0), nEscapement*(m_bEMF? -1 : 1), 0, FW_DONTCARE, FALSE, FALSE, FALSE, HANGEUL_CHARSET, OUT_DEFAULT_PRECIS, CLIP_LH_ANGLES, DEFAULT_QUALITY, DEFAULT_PITCH, strFont);
	//else  
	Font.CreateFont(fontSize_sub, (bHalfWidth? fontSize/2 : 0), nEscapement*(m_bEMF? -1 : 1), 0, FW_DONTCARE, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_LH_ANGLES, DEFAULT_QUALITY, DEFAULT_PITCH, strFont);

	pDC->SelectObject(&Font);

	nSize = arText_sub.GetSize();  
	for(i=0 ; i<nSize; i++)
	{ 
		CSize textSize(0,0);
		if(arText_sub[i].strText != _T("")) textSize = m_bEMF ? pDC->GetOutputTextExtent(CString(arText_sub[i].strText)) : pDC->GetTextExtent(CString(arText_sub[i].strText));
		double dFactor[2];
		dFactor[0] = cos(nEscapement/10.0 * CMathFunc::m_trrad);
		dFactor[1] = sin(nEscapement/10.0 * CMathFunc::m_trrad);
		CPoint LeftTopPoint;    
		if     (enPosiType==EN_DGN_POSITION_TYPE_LEFT_TOP      ) 
		{ LeftTopPoint.x = arText_sub[i].PPosition.x;  
		LeftTopPoint.y = arText_sub[i].PPosition.y; }
		else if(enPosiType==EN_DGN_POSITION_TYPE_LEFT_VCENTER  ) 
		{ LeftTopPoint.x = math_lib::round_off(arText_sub[i].PPosition.x + (                            - textSize.cy/2.0 * dFactor[1]));  
		LeftTopPoint.y = math_lib::round_off(arText_sub[i].PPosition.y + (                              textSize.cy/2.0 * dFactor[0])*(m_bEMF? 1 : -1)); }
		else if(enPosiType==EN_DGN_POSITION_TYPE_LEFT_BOTTOM   ) 
		{ 
			LeftTopPoint.x = math_lib::round_off(arText_sub[i].PPosition.x + (                            - textSize.cy     * dFactor[1]));  
			LeftTopPoint.y = math_lib::round_off(arText_sub[i].PPosition.y + (                              textSize.cy     * dFactor[0])*(m_bEMF? 1 : -1)); }
		else if(enPosiType==EN_DGN_POSITION_TYPE_CENTER_TOP    )
		{ 
			LeftTopPoint.x = math_lib::round_off(arText_sub[i].PPosition.x + (-textSize.cx/2.0*dFactor[0]                               ));  
			LeftTopPoint.y = math_lib::round_off(arText_sub[i].PPosition.y + (-textSize.cx/2.0*dFactor[1]                               )*(m_bEMF? 1 : -1)); }
		else if(enPosiType==EN_DGN_POSITION_TYPE_CENTER_VCENTER)
		{ 
			LeftTopPoint.x = math_lib::round_off(arText_sub[i].PPosition.x + (-textSize.cx/2.0*dFactor[0] - textSize.cy/2.0 * dFactor[1]));  
			LeftTopPoint.y = math_lib::round_off(arText_sub[i].PPosition.y + (-textSize.cx/2.0*dFactor[1] + textSize.cy/2.0 * dFactor[0])*(m_bEMF? 1 : -1)); }
		else if(enPosiType==EN_DGN_POSITION_TYPE_CENTER_BOTTOM )
		{ 
			LeftTopPoint.x = math_lib::round_off(arText_sub[i].PPosition.x + (-textSize.cx/2.0*dFactor[0] - textSize.cy     * dFactor[1]));  
			LeftTopPoint.y = math_lib::round_off(arText_sub[i].PPosition.y + (-textSize.cx/2.0*dFactor[1] + textSize.cy     * dFactor[0])*(m_bEMF? 1 : -1)); }
		else if(enPosiType==EN_DGN_POSITION_TYPE_RIGHT_TOP     )
		{ 
			LeftTopPoint.x = math_lib::round_off(arText_sub[i].PPosition.x + (-textSize.cx*dFactor[0]                                   ));  
			LeftTopPoint.y = math_lib::round_off(arText_sub[i].PPosition.y + (-textSize.cx*dFactor[1]                                   )*(m_bEMF? 1 : -1)); }
		else if(enPosiType==EN_DGN_POSITION_TYPE_RIGHT_VCENTER )
		{
			LeftTopPoint.x = math_lib::round_off(arText_sub[i].PPosition.x + (-textSize.cx*dFactor[0]     - textSize.cy/2.0 * dFactor[1]));  
			LeftTopPoint.y = math_lib::round_off(arText_sub[i].PPosition.y + (-textSize.cx*dFactor[1]     + textSize.cy/2.0 * dFactor[0])*(m_bEMF? 1 : -1)); }
		else if(enPosiType==EN_DGN_POSITION_TYPE_RIGHT_BOTTOM  )
		{ 
			LeftTopPoint.x = math_lib::round_off(arText_sub[i].PPosition.x + (-textSize.cx*dFactor[0]     - textSize.cy     * dFactor[1]));  
			LeftTopPoint.y = math_lib::round_off(arText_sub[i].PPosition.y + (-textSize.cx*dFactor[1]     + textSize.cy     * dFactor[0])*(m_bEMF? 1 : -1)); }
		else ASSERT(0);

		if(bFill) pDC->SetBkMode(OPAQUE);
		else      pDC->SetBkMode(TRANSPARENT);

		pDC->TextOut(LeftTopPoint.x, LeftTopPoint.y, CString(arText_sub[i].strText));
	}

	pDC->SetTextColor(oldTextColor);
	if(bFill) pDC->SetBkColor(oldBkColor);
	pDC->SelectObject(pOldPen);
	pDC->SelectObject(pOldFont);
	if(bFill) pDC->SelectObject(pOldBrh);

	LinePen.DeleteObject();
	FillBrush.DeleteObject();
	Font.DeleteObject();
}

void CDgn_DrawManager_Text::ConvertNextLineText(int fontSize, DGN_POSITION_TYPE enPosiType, int nEscapement, CArray<T_DGN_TEXT,T_DGN_TEXT&>& arTextOrg, CArray<T_DGN_TEXT,T_DGN_TEXT&>& arTextCng)
{
	BOOL m_bEMF=TRUE;
	arTextCng.RemoveAll();
	int i, j, nSize, nCount, nLength;
	nSize = (int)arTextOrg.GetSize();
	CArray<int, int> arLineSize;
	arLineSize.SetSize(nSize);
	for(i=0 ; i<nSize ; i++)
	{
		nCount = 1;
		if(arTextOrg[i].strText.Find(_T('\n')) < 0)
		{ arLineSize[i] = nCount;}
		else 
		{
			nLength = arTextOrg[i].strText.GetLength();
			for(j=0 ; j<nLength ; j++)
			{ if(arTextOrg[i].strText.GetAt(j) == _T('\n')) nCount++; }
			arLineSize[i] = nCount;
		}
	}
	nCount = 0;
	for(i=0 ; i<nSize ; i++)
	{
		nCount += arLineSize[i];
	}
	arTextCng.SetSize(nCount);

	CString strText;
	int nFindID;
	nCount = 0;
	CPoint StartPoint;
	double dVactor[2];
	double dtrrad = CMathFunc::m_trrad;
	dVactor[0] = fontSize*cos(dtrrad*(nEscapement-900)/10);
	dVactor[1] = fontSize*sin(dtrrad*(nEscapement-900)/10) * (m_bEMF ? 1.0 : -1.0);

	for(i=0 ; i<nSize ; i++)
	{
		if(arLineSize[i] <= 1)
		{ arTextCng[nCount] = arTextOrg[i];  nCount++; }
		else 
		{
			if(enPosiType==EN_DGN_POSITION_TYPE_LEFT_TOP || enPosiType==EN_DGN_POSITION_TYPE_CENTER_TOP || enPosiType==EN_DGN_POSITION_TYPE_RIGHT_TOP)
			{ StartPoint = arTextOrg[i].PPosition; }
			else if(enPosiType==EN_DGN_POSITION_TYPE_LEFT_VCENTER || enPosiType==EN_DGN_POSITION_TYPE_CENTER_VCENTER || enPosiType==EN_DGN_POSITION_TYPE_RIGHT_VCENTER)
			{ 
				StartPoint.x = arTextOrg[i].PPosition.x - (int)(0.5*(arLineSize[i]-1)*dVactor[0]); 
				StartPoint.y = arTextOrg[i].PPosition.y - (int)(0.5*(arLineSize[i]-1)*dVactor[1]); 
			}
			else if(enPosiType==EN_DGN_POSITION_TYPE_LEFT_BOTTOM || enPosiType==EN_DGN_POSITION_TYPE_CENTER_BOTTOM || enPosiType==EN_DGN_POSITION_TYPE_RIGHT_BOTTOM)
			{ 
				StartPoint.x = arTextOrg[i].PPosition.x - (int)((arLineSize[i]-1)*dVactor[0]); 
				StartPoint.y = arTextOrg[i].PPosition.y - (int)((arLineSize[i]-1)*dVactor[1]); 
			}

			strText = arTextOrg[i].strText;
			for(j=0 ; j<arLineSize[i] ; j++)
			{
				if(j == arLineSize[i]-1)
					arTextCng[nCount].strText = strText;
				else 
				{
					nFindID = strText.Find(_T('\n'));
					if(nFindID >= 0)
					{
						nLength = strText.GetLength();
						arTextCng[nCount].strText = strText.Left(nFindID);
						strText = strText.Right(nLength - (nFindID+1));
					}
					else
					{
						ASSERT(0);
						arTextCng[nCount].strText = _T("");						
					}
				}
				arTextCng[nCount].PPosition.x = (int)(StartPoint.x + j*dVactor[0]); 	
				arTextCng[nCount].PPosition.y = (int)(StartPoint.y + j*dVactor[1]); 	
				nCount++;
			}
		}
	}

}

void CDgn_DrawManager_Text::ConvertMathText(CDC* pDC, int fontSize, int fontSize_sub, DGN_POSITION_TYPE enPosiType, int nEscapement, CArray<T_DGN_TEXT,T_DGN_TEXT&>& arTextOrg, CArray<T_DGN_TEXT,T_DGN_TEXT&>& arTextCng, CArray<T_DGN_TEXT,T_DGN_TEXT&>& arTextCng_sub)
{
	arTextCng.RemoveAll();
	arTextCng_sub.RemoveAll();

	int i, j, nSize, nUSize, nCount, nCount_non, nCount_sub, nTotal_non, nTotal_sub, nLength;
	double dSubScale = (double)(fontSize_sub)/(double)(fontSize);

	CArray<int, int> arStrSize;

	nSize = (int)arTextOrg.GetSize();
	arStrSize.SetSize(nSize);

	nTotal_non = nTotal_sub =0;
	for(i=0 ; i<nSize ; i++)
	{		
		if(arTextOrg[i].strText.Find(_T('`')) < 0 && arTextOrg[i].strText.Find('^') < 0)
		{
			nTotal_non++;
			arStrSize[i]=1;
		}
		else 
		{			
			nCount_non = nCount_sub = 0; 
			nLength = arTextOrg[i].strText.GetLength();
			BOOL bEndMark=TRUE;
			int  nStartPosi=0;
			for(j=0 ; j<nLength ; j++)
			{
				if(bEndMark && (arTextOrg[i].strText.GetAt(j) == _T('`') || arTextOrg[i].strText.GetAt(j) == _T('^')))
				{ 
					if(j-nStartPosi > 0) { nCount_non++;}
					bEndMark=FALSE;  nStartPosi=j+1;
				}
				else if(!bEndMark && arTextOrg[i].strText.GetAt(j) == _T('$'))
				{
					if(j-nStartPosi > 0) {nCount_sub++;}
					bEndMark=TRUE;  nStartPosi=j+1;
				}
				else if(j==nLength-1 && j-nStartPosi >= 0)
				{
					if(bEndMark) nCount_non++;
					else         nCount_sub++;					
				}
			}
			nTotal_non += nCount_non;
			nTotal_sub += nCount_sub;
			arStrSize[i] = nCount_non+nCount_sub;
		}
	}
	arTextCng.SetSize(nTotal_non);
	arTextCng_sub.SetSize(nTotal_sub);

	T_DGN_TEXT DgnText;
	CArray<CString, CString&> arString; //문자열
	CArray<double, double>    arWidth;  //문자열 폭
	CArray<int, int>          arSubType;//0:Non, 1:위첨자, 2:아래첨자
	nTotal_non = nTotal_sub =0;
	int nEMFSign = (m_bEMF? 1 : -1);
	for(i=0 ; i<nSize ; i++)
	{		
		if(arTextOrg[i].strText.Find(_T('`')) < 0 && arTextOrg[i].strText.Find(_T('^')) < 0)
		{
			arTextCng[nTotal_non] = arTextOrg[i];
			nTotal_non++;
		}
		else 
		{		
			nUSize = arStrSize[i];
			arString.SetSize(nUSize);
			arWidth.SetSize(nUSize);			
			arSubType.SetSize(nUSize);

			nCount = 0; 
			nLength = arTextOrg[i].strText.GetLength();
			BOOL bEndMark=TRUE;
			int  nStartPosi=0;
			int  nEndPosi=0;
			BOOL bRecNon, bRecSub, bUpdateStartPosi;
			CSize size(0,0);
			for(j=0 ; j<nLength ; j++)
			{
				bRecNon = bRecSub = bUpdateStartPosi = FALSE;
				if(bEndMark && (arTextOrg[i].strText[j] == _T('`') || arTextOrg[i].strText[j] == _T('^')))
				{ 
					if(j-nStartPosi > 0) { bRecNon = TRUE;  nEndPosi = j-1;}
					bEndMark=FALSE;  bUpdateStartPosi=TRUE;
				}
				else if(!bEndMark && arTextOrg[i].strText[j] == _T('$'))
				{
					if(j-nStartPosi > 0) { bRecSub = TRUE;  nEndPosi = j-1; }
					bEndMark=TRUE;  bUpdateStartPosi=TRUE;
				}
				else if(j==nLength-1 && j-nStartPosi >= 0)
				{
					if(bEndMark) { bRecNon = TRUE;  nEndPosi = j; }
					else         { bRecSub = TRUE;  nEndPosi = j; }				
				}

				if(bRecNon || bRecSub)
				{
					arString[nCount] = arTextOrg[i].strText.Mid(nStartPosi, (nEndPosi-nStartPosi+1));
					size = (m_bEMF ? pDC->GetOutputTextExtent(arString[nCount]) : pDC->GetTextExtent(arString[nCount]));
					arWidth[nCount]  = (bRecNon ? 1.0 : dSubScale) * size.cx;
					if(bRecNon) arSubType[nCount] = 0;
					else if(nStartPosi>0)
					{
						if     (arTextOrg[i].strText[nStartPosi-1] == _T('^')) arSubType[nCount] = 1;
						else if(arTextOrg[i].strText[nStartPosi-1] == _T('`')) arSubType[nCount] = 2;
						else { ASSERT(0);  arSubType[nCount] = 0; }
					}
					else { ASSERT(0);  arSubType[nCount] = 0; }
					nCount++;
				}

				if(bUpdateStartPosi) nStartPosi=j+1;
			}

			double dTotalWidth = 0.0;
			for(j=0 ; j<nUSize ; j++) dTotalWidth += arWidth[j];

			double dFactor[2];
			dFactor[0] = cos(nEscapement/10.0 * CMathFunc::m_trrad);
			dFactor[1] = sin(nEscapement/10.0 * CMathFunc::m_trrad);
			CSize textSize(0,0);
			if(arTextOrg[i].strText != _T("")) textSize = m_bEMF ? pDC->GetOutputTextExtent(CString(arTextOrg[i].strText)) : pDC->GetTextExtent(CString(arTextOrg[i].strText));
			CPoint LeftTopPoint;    
			if     (enPosiType==EN_DGN_POSITION_TYPE_LEFT_TOP      ) 
			{ LeftTopPoint.x = arTextOrg[i].PPosition.x;  
			LeftTopPoint.y = arTextOrg[i].PPosition.y; }
			else if(enPosiType==EN_DGN_POSITION_TYPE_LEFT_VCENTER  ) 
			{ LeftTopPoint.x = math_lib::round_off(arTextOrg[i].PPosition.x + (                            - textSize.cy/2.0 * dFactor[1]));  
			LeftTopPoint.y = math_lib::round_off(arTextOrg[i].PPosition.y + (                              textSize.cy/2.0 * dFactor[0])*nEMFSign); }
			else if(enPosiType==EN_DGN_POSITION_TYPE_LEFT_BOTTOM   ) 
			{ 
				LeftTopPoint.x = math_lib::round_off(arTextOrg[i].PPosition.x + (                            - textSize.cy     * dFactor[1]));  
				LeftTopPoint.y = math_lib::round_off(arTextOrg[i].PPosition.y + (                              textSize.cy     * dFactor[0])*nEMFSign); }
			else if(enPosiType==EN_DGN_POSITION_TYPE_CENTER_TOP    )
			{ 
				LeftTopPoint.x = math_lib::round_off(arTextOrg[i].PPosition.x + (-dTotalWidth/2.0*dFactor[0]                               ));  
				LeftTopPoint.y = math_lib::round_off(arTextOrg[i].PPosition.y + (-dTotalWidth/2.0*dFactor[1]                               )*nEMFSign); }
			else if(enPosiType==EN_DGN_POSITION_TYPE_CENTER_VCENTER)
			{ 
				LeftTopPoint.x = math_lib::round_off(arTextOrg[i].PPosition.x + (-dTotalWidth/2.0*dFactor[0] - textSize.cy/2.0 * dFactor[1]));  
				LeftTopPoint.y = math_lib::round_off(arTextOrg[i].PPosition.y + (-dTotalWidth/2.0*dFactor[1] + textSize.cy/2.0 * dFactor[0])*nEMFSign); }
			else if(enPosiType==EN_DGN_POSITION_TYPE_CENTER_BOTTOM )
			{ 
				LeftTopPoint.x = math_lib::round_off(arTextOrg[i].PPosition.x + (-dTotalWidth/2.0*dFactor[0] - textSize.cy     * dFactor[1]));  
				LeftTopPoint.y = math_lib::round_off(arTextOrg[i].PPosition.y + (-dTotalWidth/2.0*dFactor[1] + textSize.cy     * dFactor[0])*nEMFSign); }
			else if(enPosiType==EN_DGN_POSITION_TYPE_RIGHT_TOP     )
			{ 
				LeftTopPoint.x = math_lib::round_off(arTextOrg[i].PPosition.x + (-dTotalWidth*dFactor[0]                                   ));  
				LeftTopPoint.y = math_lib::round_off(arTextOrg[i].PPosition.y + (-dTotalWidth*dFactor[1]                                   )*nEMFSign); }
			else if(enPosiType==EN_DGN_POSITION_TYPE_RIGHT_VCENTER )
			{
				LeftTopPoint.x = math_lib::round_off(arTextOrg[i].PPosition.x + (-dTotalWidth*dFactor[0]     - textSize.cy/2.0 * dFactor[1]));  
				LeftTopPoint.y = math_lib::round_off(arTextOrg[i].PPosition.y + (-dTotalWidth*dFactor[1]     + textSize.cy/2.0 * dFactor[0])*nEMFSign); }
			else if(enPosiType==EN_DGN_POSITION_TYPE_RIGHT_BOTTOM  )
			{ 
				LeftTopPoint.x = math_lib::round_off(arTextOrg[i].PPosition.x + (-dTotalWidth*dFactor[0]     - textSize.cy     * dFactor[1]));  
				LeftTopPoint.y = math_lib::round_off(arTextOrg[i].PPosition.y + (-dTotalWidth*dFactor[1]     + textSize.cy     * dFactor[0])*nEMFSign); }
			else ASSERT(0);


			double dHeight = (double)size.cy;
			double dHeight_cur;
			double dWidth = 0.0;
			double dPosi[2];
			for(j=0 ; j<nUSize ; j++)
			{
				DgnText.Initialize();
				DgnText.strText = arString[j];
				//좌상단점 산출
				dPosi[0] = LeftTopPoint.x + dWidth*dFactor[0];
				dPosi[1] = LeftTopPoint.y + dWidth*dFactor[1]*nEMFSign;
				if(arSubType[j] == 2)
				{//아래첨자만 수직으로 내려줌	
					dPosi[0] = dPosi[0] + dHeight*(1.0-dSubScale)*dFactor[1];
					dPosi[1] = dPosi[1] - dHeight*(1.0-dSubScale)*dFactor[0]*nEMFSign;
				}
				dHeight_cur = (arSubType[j] == 0) ? dHeight : dHeight*dSubScale;
				//해당위치의 기준점으로 변경
				if     (enPosiType==EN_DGN_POSITION_TYPE_LEFT_TOP      ) 
				{ DgnText.PPosition.x = math_lib::round_off(dPosi[0]);  
				DgnText.PPosition.y = math_lib::round_off(dPosi[1]); }
				else if(enPosiType==EN_DGN_POSITION_TYPE_LEFT_VCENTER  ) 
				{ DgnText.PPosition.x = math_lib::round_off(dPosi[0] - (                           - dHeight_cur/2.0 * dFactor[1]));  
				DgnText.PPosition.y = math_lib::round_off(dPosi[1] - (                             dHeight_cur/2.0 * dFactor[0])*nEMFSign); }
				else if(enPosiType==EN_DGN_POSITION_TYPE_LEFT_BOTTOM   ) 
				{ 
					DgnText.PPosition.x = math_lib::round_off(dPosi[0] - (                           - dHeight_cur     * dFactor[1]));  
					DgnText.PPosition.y = math_lib::round_off(dPosi[1] - (                             dHeight_cur     * dFactor[0])*nEMFSign); }
				else if(enPosiType==EN_DGN_POSITION_TYPE_CENTER_TOP    )
				{ 
					DgnText.PPosition.x = math_lib::round_off(dPosi[0] - (-arWidth[j]/2.0*dFactor[0]                               ));  
					DgnText.PPosition.y = math_lib::round_off(dPosi[1] - (-arWidth[j]/2.0*dFactor[1]                               )*nEMFSign); }
				else if(enPosiType==EN_DGN_POSITION_TYPE_CENTER_VCENTER)
				{ 
					DgnText.PPosition.x = math_lib::round_off(dPosi[0] - (-arWidth[j]/2.0*dFactor[0] - dHeight_cur/2.0 * dFactor[1]));  
					DgnText.PPosition.y = math_lib::round_off(dPosi[1] - (-arWidth[j]/2.0*dFactor[1] + dHeight_cur/2.0 * dFactor[0])*nEMFSign); }
				else if(enPosiType==EN_DGN_POSITION_TYPE_CENTER_BOTTOM )
				{ 
					DgnText.PPosition.x = math_lib::round_off(dPosi[0] - (-arWidth[j]/2.0*dFactor[0] - dHeight_cur     * dFactor[1]));  
					DgnText.PPosition.y = math_lib::round_off(dPosi[1] - (-arWidth[j]/2.0*dFactor[1] + dHeight_cur     * dFactor[0])*nEMFSign); }
				else if(enPosiType==EN_DGN_POSITION_TYPE_RIGHT_TOP     )
				{ 
					DgnText.PPosition.x = math_lib::round_off(dPosi[0] - (-arWidth[j]*dFactor[0]                                   ));  
					DgnText.PPosition.y = math_lib::round_off(dPosi[1] - (-arWidth[j]*dFactor[1]                                   )*nEMFSign); }
				else if(enPosiType==EN_DGN_POSITION_TYPE_RIGHT_VCENTER )
				{
					DgnText.PPosition.x = math_lib::round_off(dPosi[0] - (-arWidth[j]*dFactor[0]     - dHeight_cur/2.0 * dFactor[1]));  
					DgnText.PPosition.y = math_lib::round_off(dPosi[1] - (-arWidth[j]*dFactor[1]     + dHeight_cur/2.0 * dFactor[0])*nEMFSign); }
				else if(enPosiType==EN_DGN_POSITION_TYPE_RIGHT_BOTTOM  )
				{ 
					DgnText.PPosition.x = math_lib::round_off(dPosi[0] - (-arWidth[j]*dFactor[0]     - dHeight_cur     * dFactor[1]));  
					DgnText.PPosition.y = math_lib::round_off(dPosi[1] - (-arWidth[j]*dFactor[1]     + dHeight_cur     * dFactor[0])*nEMFSign); }
				else ASSERT(0);

				if(arSubType[j] == 0)
				{
					arTextCng[nTotal_non] = DgnText;
					nTotal_non ++;
				}
				else 
				{
					arTextCng_sub[nTotal_sub] = DgnText;
					nTotal_sub ++;
				}
				dWidth += arWidth[j];
			}
		}
	}
}