// CmdAutoLoadCombImperfectionLoadGrid.cpp : implementation file
//

#include "stdafx.h" 
#include "wg_cmd.h"
#include "CmdAutoLoadCombImperfectionLoadGrid.h"

#include "..\wg_common\wg_common_TBCommon.h"
#include "..\wg_common\wg_common_TBGrid.h"
#include "..\wg_db\DBDoc.h"
#include "..\wg_db\AttrCtrl.h"

#define D_COL_NO     	1
#define D_COL_LCASE1 	2
#define D_COL_LCASE2 	3
#define D_COL_DIR 	  4
#define D_COL_RAT 	  5
#define D_COL_NUM 	  5


CCmdAutoLoadCombImperfectionLoadGrid::CCmdAutoLoadCombImperfectionLoadGrid()
{
	m_pDoc = CDBDoc::GetDocPoint();

	AddColInfo(_LS(IDS_WG_CMD__ADDD__No), t_string, 0);
	AddColInfo(_LS(IDS_CMD_IMP_L1_IMP),   t_string, 0); 
	AddColInfo(_LS(IDS_CMD_IMP_L2),       t_string, 0);
	AddColInfo(_LS(IDS_CMD_IMP_DIR),      t_string, 0);
	AddColInfo(_LS(IDS_TB_WINDP_COEFF),      t_string, 0);

	m_curCode = _T("");
}

CCmdAutoLoadCombImperfectionLoadGrid::~CCmdAutoLoadCombImperfectionLoadGrid()
{
}

///////////////////////////////////////////////////////////////////////////////
// User Defined Function
void CCmdAutoLoadCombImperfectionLoadGrid::SetData(CArray<_LCOM_IMPF, _LCOM_IMPF&> &aData)
{
	BOOL bOldLock = LockUpdate(TRUE);

	int nSize = aData.GetSize();

	if(nSize < 0) return;

	CMap<T_SGLD_K, T_SGLD_K, int, int> mapErect;
	m_pDoc->m_pAttrCtrl->GetErectionSelected(mapErect);

	SetRowCount(nSize + 1);
	CString strValue, strNo;
	for(int i=0; i<nSize; ++i)
	{

#if defined(_CIVIL)
		if (aData[i].nAnalType == D_LCOM_STAGE)
		{
			// Erection Load 및 CS로 정의된 것만 보여주기(Tendon 제외).
			if (aData[i].LCase1K >= D_SGLD_ER1_CVL && aData[i].LCase1K < D_SGLD_SUM_CVL)
			{
				int nTemp = 0;
				if (!mapErect.Lookup(aData[i].LCase1K, nTemp)) continue;
			}
			if (aData[i].LCase2K >= D_SGLD_ER1_CVL && aData[i].LCase2K < D_SGLD_SUM_CVL)
			{
				int nTemp = 0;
				if (!mapErect.Lookup(aData[i].LCase1K, nTemp)) continue;
			}
		}
#else
		if (aData[i].nAnalType == D_LCOM_STAGE)
		{
			// Erection Load 및 CS로 정의된 것만 보여주기(Tendon 제외).
			if (aData[i].LCase1K >= D_SGLD_ER1_GEN && aData[i].LCase1K < D_SGLD_SUM_GEN)
			{
				int nTemp = 0;
				if (!mapErect.Lookup(aData[i].LCase1K, nTemp)) continue;
			}
			if (aData[i].LCase2K >= D_SGLD_ER1_GEN && aData[i].LCase2K < D_SGLD_SUM_GEN)
			{
				int nTemp = 0;
				if (!mapErect.Lookup(aData[i].LCase1K, nTemp)) continue;
			}
		}
#endif

		CStringArray aValue;
		aValue.SetSize(D_COL_NUM);

		strNo.Format(_T("%d"), i+1);
		aValue[D_COL_NO     - 1] = strNo;
		aValue[D_COL_LCASE1 - 1] = GetLCase1Str(aData[i].LCase1K);

		int nRangeStart = D_STLD_MAXNUM*aData[i].nAnalType;
		strValue.Format(_T("%d"), nRangeStart+aData[i].LCase2K);
		aValue[D_COL_LCASE2 - 1] = strValue; // Key부분을 Setting하면 됨..
		aValue[D_COL_DIR    - 1] = GetDirectionStr(aData[i].nDir);

		CString strRat;
		strRat.Format(_T("%.4f"), aData[i].dMutiRatio_CH);
		aValue[D_COL_RAT - 1] = strRat;

		SetDataToRow(i+1, aValue);
	}

	SetRowHeightByDPI(1, nSize, 18);

	LockUpdate(bOldLock);
	if(!bOldLock) Redraw();
} 

BOOL CCmdAutoLoadCombImperfectionLoadGrid::GetData(CArray<_LCOM_IMPF, _LCOM_IMPF&> &aData)
{
	aData.RemoveAll();

	int nCount = GetRowCount();
	nCount--;
	if(nCount==0) return TRUE;

	aData.SetSize(nCount);

	CStringArray aValue;
	aValue.SetSize(D_COL_NUM);

	int nAnalType;
	T_STLD_K nKey;

	CString strLCaseName;
	for(int i=0; i<nCount; ++i)
	{
		if(!GetRowToData(i+1, aValue)) return FALSE;

		strLCaseName = aValue[D_COL_LCASE1 - 1];
		strLCaseName = strLCaseName.Left(strLCaseName.ReverseFind('('));
		aData[i].LCase1K  = m_pDoc->m_pAttrCtrl->GetStldKey(strLCaseName);

		CString strDebug = GetValueRowCol(i+1, D_COL_LCASE2);
		nKey = _ttol(GetValueRowCol(i+1, D_COL_LCASE2));
		nAnalType = nKey / D_STLD_MAXNUM;
		aData[i].LCase2K = nKey % D_STLD_MAXNUM;
		aData[i].nAnalType = nAnalType;    
	aData[i].nDir = GetDirectionData(aValue[D_COL_DIR - 1]);
		aData[i].dMutiRatio_CH = _tstof(GetValueRowCol(i+1, D_COL_RAT));
	}
	return TRUE;
}

void CCmdAutoLoadCombImperfectionLoadGrid::SetDataToRow(ROWCOL nRow, CStringArray& raValue)
{
	int nColCount = GetColCount();
	ASSERT(nColCount == raValue.GetSize());

	for(int i=0; i<nColCount; ++i)
		SetValueRange(CGXRange(nRow, i+1), raValue[i]);
}

BOOL CCmdAutoLoadCombImperfectionLoadGrid::GetRowToData(ROWCOL nRow, CStringArray& raValue)
{
	int nColCount = GetColCount();
	ASSERT(nColCount == raValue.GetSize());

	for(int i=0; i<nColCount; ++i)
		raValue[i] = GetEditingValue(nRow, i+1);

	return TRUE;
}

CString CCmdAutoLoadCombImperfectionLoadGrid::GetEditingValue(ROWCOL nRow, ROWCOL nCol)
{
	CString strNum(_T(""));
	BOOL bGet = FALSE;
	ROWCOL ncRow, ncCol;
	if(GetCurrentCell(ncRow, ncCol))
	{
		if(ncRow==nRow && ncCol==nCol)
		{
			CGXControl* pControl = GetControl(nRow, nCol);
			if(pControl && pControl->IsActive())
				bGet = pControl->GetValue(strNum);
		}
	}
	if(!bGet) strNum = GetValueRowCol(nRow, nCol);

	return strNum;
}

BEGIN_MESSAGE_MAP(CCmdAutoLoadCombImperfectionLoadGrid, CTBCommon)
	//{{AFX_MSG_MAP(CCmdAutoLoadCombImperfectionLoadGrid)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCmdAutoLoadCombImperfectionLoadGrid message handlers

void CCmdAutoLoadCombImperfectionLoadGrid::Initialize()
{
	T_UNIT_SYSTEM UnitSystem;
	for (int i = 0; i < D_COL_NUM; i++)
	{
		UnitSystem.Index.Add(0);
	}

	CTBCommon::Initialize(m_pDoc, UnitSystem);

	LockUpdate(TRUE);

	SetColCount(D_COL_NUM);

	// 표준 스타일 변경
	ChangeStandardStyle(CGXStyle()
		.SetFont(CGXFont_GC().SetSize(10))
		.SetVerticalAlignment(DT_BOTTOM)
		.SetAllowEnter(FALSE));

	// Header
	SetFrozenCols(1, 1);

	// column의 스타일 변경
	// 1.No 
	SetStyleRange(CGXRange().SetCols(D_COL_NO), CGXStyle()
		.SetHorizontalAlignment(DT_RIGHT)
		.SetValueType(GX_VT_NUMERIC)
		.SetFormat(GX_FMT_FIXED)
		.SetPlaces(0)
		.SetEnabled(FALSE));

	// 2.LCASE1
	CString strCbxItem = _T("");
	GetLCase1Lst(strCbxItem);  
	SetStyleRange(CGXRange().SetCols(D_COL_LCASE1), CGXStyle()
		.SetControl(GX_IDS_CTRL_TABBED_COMBOBOX)  // GX_IDS_CTRL_CBS_TABBED_DROPDOWNLIST
		.SetChoiceList(strCbxItem)
		.SetUserAttribute(GX_IDS_UA_TABLIST_KEYCOL, _T("0"))
		.SetUserAttribute(GX_IDS_UA_TABLIST_TEXTCOL, _T("0"))
		.SetUserAttribute(GX_IDS_UA_TABLIST_SHOWALLCOLS, _T("1"))
		.SetValueType(GX_VT_STRING)
		.SetHorizontalAlignment(DT_LEFT)
		.SetValue(_T(""))
		);

	// 3.LCASE2
	int nNameLen, nDescLen;
	GetLCase2Lst(strCbxItem, nNameLen, nDescLen);    
	CString csSize;
	csSize.Format(_T("0,0,%d,0"), 130);
	SetStyleRange(CGXRange().SetCols(D_COL_LCASE2), CGXStyle()
		.SetControl(GX_IDS_CTRL_TABBED_COMBOBOX)
		.SetChoiceList(strCbxItem)
		.SetUserAttribute(GX_IDS_UA_TABLIST_KEYCOL, _T("0"))
		.SetUserAttribute(GX_IDS_UA_TABLIST_TEXTCOL, _T("2"))
		.SetUserAttribute(GX_IDS_UA_TABLIST_SHOWALLCOLS, _T("0"))    
		.SetUserAttribute(GX_IDS_UA_TABLIST_COLWIDTHS, csSize)
		.SetUserAttribute(GX_IDS_UA_TABLIST_SORTCOL, _T("1"))
		.SetValueType(GX_VT_STRING)
		.SetHorizontalAlignment(DT_LEFT)
		.SetValue(_T(""))
		);

	// 4.DIR
	strCbxItem.Format(_T("%s\n%s\n"), _T("Global-X"), _T("Global-Y"));
	SetStyleRange(CGXRange().SetCols(D_COL_DIR), CGXStyle()
		.SetControl(GX_IDS_CTRL_TABBED_COMBOBOX)  // GX_IDS_CTRL_CBS_TABBED_DROPDOWNLIST
		.SetChoiceList(strCbxItem)
		.SetUserAttribute(GX_IDS_UA_TABLIST_KEYCOL, _T("0"))
		.SetUserAttribute(GX_IDS_UA_TABLIST_TEXTCOL, _T("0"))
		.SetUserAttribute(GX_IDS_UA_TABLIST_SHOWALLCOLS, _T("1"))
		.SetValueType(GX_VT_STRING)
		.SetHorizontalAlignment(DT_LEFT)
		.SetValue(_T(""))
		);
	 //5 Rat
	SetStyleRange(CGXRange().SetCols(D_COL_RAT), CGXStyle()
			.SetHorizontalAlignment(DT_RIGHT)
			.SetValueType(GX_VT_NUMERIC)
			.SetFormat(GX_FMT_FIXED)
			.SetPlaces(4));


	SetRowHeightByDPI(0, 0, 40);

	// undo를 금지시킨다.
	GetParam()->EnableUndo(FALSE);  

	// cell의 width와 height 크기 변경을 금지한다.
	GetParam()->EnableTrackRowHeight(FALSE);
	//GetParam()->EnableTrackColWidth(FALSE);

	// Row의 이동을 금지시킨다.
	GetParam()->EnableMoveRows(FALSE);
	GetParam()->EnableMoveCols(FALSE);

	// double click시 셀 편집이 되도록 한다.
	GetParam()->SetActivateCellFlags(GX_CAFOCUS_DBLCLICKONCELL);

	// MemoryDC를 사용한다.
	SetDrawingTechnique(gxDrawUsingMemDC);

	// 헤더의 Current cell을 막는다.
	ColHeaderStyle().SetEnabled(FALSE);
	RowHeaderStyle().SetEnabled(FALSE);

	// always show vertical scrollbar
	SetScrollBarMode(SB_VERT, gxnEnabled | gxnEnhanced);

	CGXProperties* pProp = GetParam()->GetProperties();
	pProp->SetMarkColHeader(FALSE); // Turn off pressed button effect for column headers
	pProp->SetMarkRowHeader(FALSE);
	pProp->SetUserProperty(GX_IDS_OUTLINECURRENTCELL, // 현재 셀의 border 설정 변경
		(CGXStyle) pProp->sInvertNoBorder);

	// 표준 스타일 변경
	#if defined(_JP) || defined(_CH)
	ChangeColHeaderStyle(CGXStyle().SetFont(CGXFont_GC().SetBold(FALSE)));
	#endif

	CArray<int,int> aWidth;
	aWidth.SetSize(D_COL_NUM);	
	aWidth[D_COL_NO    -1] =  30;
	aWidth[D_COL_LCASE1-1] = 115;
	aWidth[D_COL_LCASE2-1] = 115;
	aWidth[D_COL_DIR   -1] =  73;
	aWidth[D_COL_RAT   -1] =  73;

	SetColWidthByDPI(0, 0, 20);
	for (int i = 0; i < D_COL_NUM; i++)
	{
		SetColWidthByDPI(i + 1, i + 1, aWidth[i]);
	}

	SetValueRange(CGXRange(0, D_COL_NO    ), _LS(IDS_WG_CMD__ADDD__No)); 
	SetValueRange(CGXRange(0, D_COL_LCASE1), _LS(IDS_CMD_IMP_L1_IMP));
	SetValueRange(CGXRange(0, D_COL_LCASE2), _LS(IDS_CMD_IMP_L2));
	SetValueRange(CGXRange(0, D_COL_DIR   ), _LS(IDS_CMD_IMP_DIR));
	SetValueRange(CGXRange(0, D_COL_RAT   ), _LS(IDS_TB_WINDP_COEFF));

	SetRowCount(1);

	EnableDeleteRowByPressingKey(TRUE);

	if(m_curCode == _T("GB50017-17"))
			HideColsByIntend(D_COL_DIR, D_COL_DIR, TRUE);
	else
			HideColsByIntend(D_COL_RAT, D_COL_RAT, TRUE);
	LockUpdate(FALSE);
}

void CCmdAutoLoadCombImperfectionLoadGrid::GetLCase1Lst(CString& strLCaseLst)
{
	CString strTemp;
	CArray<T_STLD_K, T_STLD_K> aStldK;
	m_pDoc->m_pAttrCtrl->GetStldKeyList(aStldK);

	T_STLD_D StldD;
	for (int i = 0; i < aStldK.GetSize(); i++)
	{
		m_pDoc->m_pAttrCtrl->GetStld(aStldK[i], StldD);
		if(StldD.LoadCaseType == _T("I"))
		{
			strTemp.Format(_T("%s(ST)\n"), StldD.LoadCaseName);
			strLCaseLst += strTemp;
		}
	}
}

void CCmdAutoLoadCombImperfectionLoadGrid::GetLCase2Lst(CString& strLCaseLst, int& nNameLen, int& nDescLen)
{
	strLCaseLst = _T("");
	nNameLen = 0;
	nDescLen = 0;

	// CMLoadCombSubGrid 에서의 List와 동일하게 함  
	CString item;
	int nRangeStart;

	
	CSize nTempLen;
	CDC* pDC = GetDC();
	ASSERT(pDC);

	//D_LCOM_STATIC		1 : 정적 하중 조건
	T_STLD_K Key;
	T_STLD_D Data;
	nRangeStart = D_STLD_MAXNUM*D_LCOM_STATIC;
	POSITION pos = m_pDoc->m_pAttrCtrl->GetStartStld();
	while(pos != NULL)
	{
	  m_pDoc->m_pAttrCtrl->GetNextStld(pos,Key,Data);
		if (Data.LoadCaseType == _T("CS") || Data.LoadCaseType == _T("TCS")) continue;  // 시공타입은 제외
		if (Data.LoadCaseType == _T("I")) continue;  // Imperfection Load는 제외
		item.Format(_T("%d\t%d\t%s(ST)\t%s\n"), 
			nRangeStart+Key, nRangeStart+Data.LoadCaseId, Data.LoadCaseName,
			Data.Description);
		strLCaseLst += item;
		nTempLen = pDC->GetTextExtent(Data.LoadCaseName+_T("(ST)"));
		if (nTempLen.cx > nNameLen) nNameLen = nTempLen.cx;
		nTempLen = pDC->GetTextExtent(Data.Description);
		if (nTempLen.cx > nDescLen) nDescLen = nTempLen.cx;    
	}

	//D_LCOM_SPECTRUM		2 : 응답 스펙트럼 하중 조건
	T_SPLC_K SplcKey;
	T_SPLC_D SplcData;
	SplcData.Initialize();
	nRangeStart = D_STLD_MAXNUM*D_LCOM_SPECTRUM;
	pos = m_pDoc->m_pAttrCtrl->GetStartSplc();
	while(pos != NULL)
	{
	  m_pDoc->m_pAttrCtrl->GetNextSplc(pos,SplcKey,SplcData);
		item.Format(_T("%d\t%d\t%s(RS)\t%s\n"), 
			nRangeStart+SplcKey, nRangeStart+SplcData.LoadCaseId, SplcData.LoadCaseName,
			SplcData.Description);
		strLCaseLst += item;
		nTempLen = pDC->GetTextExtent(SplcData.LoadCaseName+_T("(RS)"));
		if (nTempLen.cx > nNameLen) nNameLen = nTempLen.cx;
		nTempLen = pDC->GetTextExtent(SplcData.Description);
		if (nTempLen.cx > nDescLen) nDescLen = nTempLen.cx;    
	}

	//D_LCOM_HISTORY		3 : 시간 이력 하중 조건
	T_THIS_K ThisKey;
	T_THIS_D ThisData;
	nRangeStart = D_STLD_MAXNUM*D_LCOM_HISTORY;
#ifdef _MEC
	CArray<T_THIS_K, T_THIS_K> rThisKeyList;
	m_pDoc->GetThisKeyListAppendDummyInit(rThisKeyList);
	for (int i = 0; i < rThisKeyList.GetSize(); i++)
	{
		ThisKey = rThisKeyList[i];
		m_pDoc->m_pAttrCtrl->GetThis(rThisKeyList[i], ThisData);
		item.Format(_T("%d\t%d\t%s(TH)\t%s\n"),
			nRangeStart + ThisKey, nRangeStart + ThisData.LoadCaseId, ThisData.LoadCaseName,
			ThisData.Description);
		strLCaseLst += item;
		nTempLen = pDC->GetTextExtent(ThisData.LoadCaseName + _T("(TH)"));
		if (nTempLen.cx > nNameLen) nNameLen = nTempLen.cx;
		nTempLen = pDC->GetTextExtent(ThisData.Description);
		if (nTempLen.cx > nDescLen) nDescLen = nTempLen.cx;
	}
#else
	pos = m_pDoc->m_pAttrCtrl->GetStartThis();
	while(pos != NULL)
	{
	  m_pDoc->m_pAttrCtrl->GetNextThis(pos,ThisKey,ThisData);
		item.Format(_T("%d\t%d\t%s(TH)\t%s\n"), 
			nRangeStart+ThisKey, nRangeStart+ThisData.LoadCaseId, ThisData.LoadCaseName,
			ThisData.Description);
		strLCaseLst += item;
		nTempLen = pDC->GetTextExtent(ThisData.LoadCaseName+_T("(TH)"));
		if (nTempLen.cx > nNameLen) nNameLen = nTempLen.cx;
		nTempLen = pDC->GetTextExtent(ThisData.Description);
		if (nTempLen.cx > nDescLen) nDescLen = nTempLen.cx;    
	}
#endif
	//D_LCOM_MOVING		  4 : 이동 하중 조건
	// 코드에 따른 방식으로 수정 
	T_MVCD_D DataMvcd;
	if(!m_pDoc->m_pAttrCtrl->GetMvcd(DataMvcd)) DataMvcd.Initialize();
	switch (DataMvcd.nCodeType)
	{
	case D_MOVE_CODE_NONE:
		break;
	case D_MOVE_CODE_AASHTO_STAN:
	case D_MOVE_CODE_AASHTO_LRFD:
	case D_MOVE_CODE_TAIWAN:
	case D_MOVE_CODE_KOREA:
	case D_MOVE_CODE_CANADA:
	case D_MOVE_CODE_PENDOT:
	case D_MOVE_CODE_EURO_BS:
	case D_MOVE_CODE_RUSSIA:
	case D_MOVE_CODE_KOREA_LRFD_2011:
	case D_MOVE_CODE_AUSTRALIA:
	case D_MOVE_CODE_POLAND:
	case D_MOVE_CODE_SOUTH_AFRICA:
	case D_MOVE_CODE_JAPAN_RAIL:
	case D_MOVE_CODE_NEWZEALAND:
	case D_MOVE_CODE_BRAZIL:
		{
			T_MVLD_K MvldKey;
			T_MVLD_D MvldData;
			nRangeStart = D_STLD_MAXNUM*D_LCOM_MOVING;
			pos = m_pDoc->m_pAttrCtrl->GetStartMvld();
			while(pos != NULL)
			{
	      m_pDoc->m_pAttrCtrl->GetNextMvld(pos,MvldKey,MvldData);
				item.Format(_T("%d\t%d\t%s(MV)\t%s\n"), 
					nRangeStart+MvldKey, nRangeStart+MvldData.LoadCaseId, MvldData.LoadCaseName,
					MvldData.Description);
				strLCaseLst += item;
				nTempLen = pDC->GetTextExtent(MvldData.LoadCaseName+_T("(MV)"));
				if (nTempLen.cx > nNameLen) nNameLen = nTempLen.cx;
				nTempLen = pDC->GetTextExtent(MvldData.Description);
				if (nTempLen.cx > nDescLen) nDescLen = nTempLen.cx;    
			}
		}
		break;
	case D_MOVE_CODE_CHINA:
		{
			T_MVLDch_K MvldchKey;
			T_MVLDch_D MvldchData;
			nRangeStart = D_STLD_MAXNUM*D_LCOM_MOVING;
			pos = m_pDoc->m_pAttrCtrl->GetStartMvldch();
			while(pos != NULL)
			{
	      m_pDoc->m_pAttrCtrl->GetNextMvldch(pos,MvldchKey,MvldchData);
				item.Format(_T("%d\t%d\t%s(MV)\t%s\n"), 
					nRangeStart+MvldchKey, nRangeStart+MvldchData.LoadCaseId, MvldchData.LoadCaseName,
					MvldchData.Description);
				strLCaseLst += item;
				nTempLen = pDC->GetTextExtent(MvldchData.LoadCaseName+_T("(MV)"));
				if (nTempLen.cx > nNameLen) nNameLen = nTempLen.cx;
				nTempLen = pDC->GetTextExtent(MvldchData.Description);
				if (nTempLen.cx > nDescLen) nDescLen = nTempLen.cx;      
			}
		}
		break;
	case D_MOVE_CODE_JAPAN:
		{
			// japan version
			T_MVLDjp_K MvldjpKey;
			T_MVLDjp_D MvldjpData;
			nRangeStart = D_STLD_MAXNUM*D_LCOM_MOVING;
			pos = m_pDoc->m_pAttrCtrl->GetStartMvldjp();
			while(pos != NULL)
			{
	      m_pDoc->m_pAttrCtrl->GetNextMvldjp(pos,MvldjpKey,MvldjpData);
				item.Format(_T("%d\t%d\t%s(MV)\t%s\n"), 
					nRangeStart+MvldjpKey, nRangeStart+MvldjpData.LoadCaseId, MvldjpData.LoadCaseName,
					MvldjpData.Description);
				strLCaseLst += item;
				nTempLen = pDC->GetTextExtent(MvldjpData.LoadCaseName+_T("(MV)"));
				if (nTempLen.cx > nNameLen) nNameLen = nTempLen.cx;
				nTempLen = pDC->GetTextExtent(MvldjpData.Description);
				if (nTempLen.cx > nDescLen) nDescLen = nTempLen.cx;    
			}      
		}
		break;
	case D_MOVE_CODE_INDIA:
		{
			T_MVLDid_K MvldidKey;
			T_MVLDid_D MvldidData;
			nRangeStart = D_STLD_MAXNUM*D_LCOM_MOVING;
			pos = m_pDoc->m_pAttrCtrl->GetStartMvldid();
			while(pos != NULL)
			{
	      m_pDoc->m_pAttrCtrl->GetNextMvldid(pos,MvldidKey,MvldidData);
				item.Format(_T("%d\t%d\t%s(MV)\t%s\n"), 
					nRangeStart+MvldidKey, nRangeStart+MvldidData.LoadCaseId, MvldidData.LoadCaseName,
					MvldidData.Description);
				strLCaseLst += item;
				nTempLen = pDC->GetTextExtent(MvldidData.LoadCaseName+_T("(MV)"));
				if (nTempLen.cx > nNameLen) nNameLen = nTempLen.cx;
				nTempLen = pDC->GetTextExtent(MvldidData.Description);
				if (nTempLen.cx > nDescLen) nDescLen = nTempLen.cx;    
			}
		}
		break;
	case D_MOVE_CODE_BS:
		{
			T_MVLDbs_K MvldbsKey;
			T_MVLDbs_D MvldbsData;
			nRangeStart = D_STLD_MAXNUM*D_LCOM_MOVING;
			pos = m_pDoc->m_pAttrCtrl->GetStartMvldbs();
			while(pos != NULL)
			{
	      m_pDoc->m_pAttrCtrl->GetNextMvldbs(pos,MvldbsKey,MvldbsData);
				item.Format(_T("%d\t%d\t%s(MV)\t%s\n"), 
					nRangeStart+MvldbsKey, nRangeStart+MvldbsData.LoadCaseId, MvldbsData.LoadCaseName,
					MvldbsData.Description);
				strLCaseLst += item;
				nTempLen = pDC->GetTextExtent(MvldbsData.LoadCaseName+_T("(MV)"));
				if (nTempLen.cx > nNameLen) nNameLen = nTempLen.cx;
				nTempLen = pDC->GetTextExtent(MvldbsData.Description);
				if (nTempLen.cx > nDescLen) nDescLen = nTempLen.cx;    
			}
		}
		break;
	case D_MOVE_CODE_FRANCE:
		{
			T_MVLDfr_K MvldfrKey;
			T_MVLDfr_D MvldfrData;
			nRangeStart = D_STLD_MAXNUM*D_LCOM_MOVING;
			pos = m_pDoc->m_pAttrCtrl->GetStartMvldfr();
			while(pos != NULL)
			{
	      m_pDoc->m_pAttrCtrl->GetNextMvldfr(pos, MvldfrKey, MvldfrData);
				item.Format(_T("%d\t%d\t%s(MV)\t%s\n"), 
					nRangeStart+ MvldfrKey, nRangeStart+ MvldfrData.LoadCaseId, MvldfrData.LoadCaseName,
					MvldfrData.Description);
				strLCaseLst += item;
				nTempLen = pDC->GetTextExtent(MvldfrData.LoadCaseName+_T("(MV)"));
				if (nTempLen.cx > nNameLen) nNameLen = nTempLen.cx;
				nTempLen = pDC->GetTextExtent(MvldfrData.Description);
				if (nTempLen.cx > nDescLen) nDescLen = nTempLen.cx;    
			}
		}
		break;
	case D_MOVE_CODE_TRANS:
		{
			T_MVLDtr_K MvldKey;
			T_MVLDtr_D MvldData;
			nRangeStart = D_STLD_MAXNUM*D_LCOM_MOVING;
			pos = m_pDoc->m_pAttrCtrl->GetStartMvldtr();
			while(pos != NULL)
			{
	      m_pDoc->m_pAttrCtrl->GetNextMvldtr(pos,MvldKey,MvldData);
				item.Format(_T("%d\t%d\t%s(MV)\t%s\n"), 
					nRangeStart+MvldKey, nRangeStart+MvldData.LoadCaseId, MvldData.LoadCaseName,
					MvldData.Description);
				strLCaseLst += item;
				nTempLen = pDC->GetTextExtent(MvldData.LoadCaseName+_T("(MV)"));
				if (nTempLen.cx > nNameLen) nNameLen = nTempLen.cx;
				nTempLen = pDC->GetTextExtent(MvldData.Description);
				if (nTempLen.cx > nDescLen) nDescLen = nTempLen.cx;    
			}
		}
		break;
	default:
		ASSERT(0);
		break;
	}

	//D_LCOM_SETTLEMENT	5 : ?? 하중 조건
	T_SMLC_K SmlcKey;
	T_SMLC_D SmlcData;
	nRangeStart = D_STLD_MAXNUM*D_LCOM_SETTLEMENT;
	pos = m_pDoc->m_pAttrCtrl->GetStartSmlc();
	while(pos != NULL)
	{
	  m_pDoc->m_pAttrCtrl->GetNextSmlc(pos,SmlcKey,SmlcData);
		item.Format(_T("%d\t%d\t%s(SM)\t%s\n"), 
			nRangeStart+SmlcKey, nRangeStart+SmlcData.LoadCaseId, SmlcData.LoadCaseName,
			SmlcData.Description);
		strLCaseLst += item;
		nTempLen = pDC->GetTextExtent(SmlcData.LoadCaseName+_T("(SM)"));
		if (nTempLen.cx > nNameLen) nNameLen = nTempLen.cx;
		nTempLen = pDC->GetTextExtent(SmlcData.Description);
		if (nTempLen.cx > nDescLen) nDescLen = nTempLen.cx;    
	}
	//D_LCOM_STAGE 13: 시공단계 하중 조건
	CArray<T_SGLD_K, T_SGLD_K> aSgldKey;
	T_SGLD_K SgldKey;
	T_SGLD_D SgldData;
	nRangeStart = D_STLD_MAXNUM*D_LCOM_STAGE;
	m_pDoc->m_pAttrCtrl->GetSgldKeyList(aSgldKey);
	int nSgldNum = aSgldKey.GetSize();
	for (int n = 0; n < nSgldNum; n++)
	{
		SgldKey = aSgldKey[n];
		if (m_pDoc->m_pAttrCtrl->GetSgld(SgldKey, SgldData))
		{
			item.Format(_T("%d\t%d\t%s(CS)\t%s\n"),
				nRangeStart+SgldKey, nRangeStart+SgldData.LoadCaseId, SgldData.LoadCaseName,
				SgldData.Description);
			strLCaseLst += item;
			nTempLen = pDC->GetTextExtent(SgldData.LoadCaseName+_T("(CS)"));
			if (nTempLen.cx > nNameLen) nNameLen = nTempLen.cx;
			nTempLen = pDC->GetTextExtent(SgldData.Description);
			if (nTempLen.cx > nDescLen) nDescLen = nTempLen.cx;    
		}
	}
	//D_LCOM_ECCENSPEC 우발편심 Spectrum
	CArray<T_ESPC_K, T_ESPC_K> aEspcKey;
	T_ESPC_K EspcKey;
	T_ESPC_D EspcData;
	nRangeStart = D_STLD_MAXNUM*D_LCOM_ECCENSPEC;
	m_pDoc->m_pAttrCtrl->GetEspcKeyList(aEspcKey);
	int nEspcNum = aEspcKey.GetSize();
	for (int n = 0; n < nEspcNum; n++)
	{
		EspcKey = aEspcKey[n];
		if (m_pDoc->m_pAttrCtrl->GetEspc(EspcKey, EspcData))
		{
			if(!m_pDoc->m_pAttrCtrl->GetSplc(EspcKey, SplcData)) {ASSERT(0); continue;}
			if(!SplcData.bConsiderEccen) continue;

			item.Format(_T("%d\t%d\t%s(ES)\t%s\n"),
				nRangeStart+EspcKey, nRangeStart+EspcData.LoadCaseId, EspcData.LoadCaseName,
				EspcData.Description);
			strLCaseLst += item;
			nTempLen = pDC->GetTextExtent(EspcData.LoadCaseName+_T("(ES)"));
			if (nTempLen.cx > nNameLen) nNameLen = nTempLen.cx;
			nTempLen = pDC->GetTextExtent(EspcData.Description);
			if (nTempLen.cx > nDescLen) nDescLen = nTempLen.cx;    
		}
	}
	//D_LCOM_GRIDMODEL 격자모델 하중 케이스
	CArray<T_GILC_K, T_GILC_K> aGilcKey;
	T_GILC_K GilcKey;
	T_GILC_D GilcData;
	nRangeStart = D_STLD_MAXNUM*D_LCOM_GRIDMODEL;
	m_pDoc->m_pAttrCtrl->GetGilcKeyList(aGilcKey);
	int nGilcNum = aGilcKey.GetSize();
	for (int n = 0; n < nGilcNum; n++)
	{
		GilcKey = aGilcKey[n];
		if (m_pDoc->m_pAttrCtrl->GetGilc(GilcKey, GilcData))
		{
			item.Format(_T("%d\t%d\t%s(GI)\t%s\n"),
				nRangeStart+GilcKey, nRangeStart+GilcData.LoadCaseId, GilcData.LoadCaseName,
				GilcData.Description);
			strLCaseLst += item;
			nTempLen = pDC->GetTextExtent(GilcData.LoadCaseName+_T("(GI)"));
			if (nTempLen.cx > nNameLen) nNameLen = nTempLen.cx;
			nTempLen = pDC->GetTextExtent(GilcData.Description);
			if (nTempLen.cx > nDescLen) nDescLen = nTempLen.cx;    
		}
	}
	//D_LCOM_CRANE 19 : Crane 하중
	BOOL bExistCrane = m_pDoc->m_pAttrCtrl->GetCountCran();
	nRangeStart = D_STLD_MAXNUM*D_LCOM_CRANE;
	if(bExistCrane)
	{
		CString strLCName = _LS(IDS_CMD_CRANE);
		item.Format(_T("%d\t%d\t%s(CN)\t%s\n"), nRangeStart+1, nRangeStart+1, strLCName, _T(""));
		strLCaseLst += item;
		if(m_pDoc->m_pAttrCtrl->IsExistCranBraking())
		{
			strLCName = _LS(IDS_CMD_CRANE_BRK);
			item.Format(_T("%d\t%d\t%s(CN)\t%s\n"), nRangeStart+2, nRangeStart+2, strLCName, _T(""));
			strLCaseLst += item;
		}
		nTempLen = pDC->GetTextExtent(strLCName+_T("(CN)"));
		if (nTempLen.cx > nNameLen) nNameLen = nTempLen.cx;   
	}
}

BOOL CCmdAutoLoadCombImperfectionLoadGrid::GetStyleRowCol(ROWCOL nRow, ROWCOL nCol, CGXStyle& style, GXModifyType mt, int nType)
{
	BOOL bCurrentRow;
	BOOL bCurrentCell;
	BOOL bEnabled;
	BOOL bHeaderCol;
	BOOL bHeaderRow;

	ROWCOL ncRow, ncCol;    // 현재 Row, Col
	CTBCommon::GetStyleRowCol(nRow, nCol, style, mt, nType);

	// 현재 셀의 그리드 좌표(Row, Col 번호)를 얻어온다.
	if (!GetCurrentCell(ncRow, ncCol)) 
		ncRow = GetRowCount()+1; // 현재 셀이 없는 경우 : ncRow를 범위 밖으로 설정

	// 현재 셀이 이동되면 새로 그려준다.
	m_bRefreshOnSetCurrentCell = TRUE;

	// Row나 Col이 1보다 작으면 리턴한다.(Standard Row or Column Header)
	if (nRow < 1 || nCol < 1) return TRUE;

	// Get Current Table and Cell State
	bCurrentRow = (nRow == ncRow);
	bCurrentCell = (nRow == ncRow && nCol == ncCol);

	// Enable, ReadOnly와 같은 Cell의 상태를 조사한다.
	// ComposeStyleRowCol은 호출해서는 안된다.
	// 그래서 여기서는 직접 조사한다.
	// 단 Cell Style, Column Style만 조사한다.
	if (style.GetIncludeEnabled())  // 셀에 Enable정보 포함 여부 알아냄
	{
		bEnabled = style.GetEnabled();
	}
	else // Column & Row style에서 enable 정보 알아냄
	{
		CGXStyle styleEx;
		GetColStyle(nCol, styleEx);
		//bEnabled = (!styleEx.GetIncludeEnabled() || styleEx.GetEnabled());
		GetRowStyle(nRow, styleEx);
		bEnabled = (!styleEx.GetIncludeEnabled() || styleEx.GetEnabled());
	}

	bHeaderCol = IsFrozenCol(nCol);
	bHeaderRow = IsFrozenRow(nRow);

	// Printing 모드이면 헤더 Column이 프린트 되기 위해 다음과 같이 리턴한다.
	if (IsPrinting())
	{
		if (bHeaderCol) style.SetControl(GX_IDS_CTRL_HEADER);
		return TRUE;
	}
	/*
	// 일반 셀이 가장 많으므로 빨리 검사하고 바로 리턴할 수 있게 코드를 앞으로 뺀다.
	if (bEnabled && !bCurrentCell && !bCurrentRow && !bHeaderCol)
	{
		style
			.SetInterior(COLORREF(RGB(228, 236, 247)))
			.SetTextColor(COLORREF(RGB(0, 0, 0)));

		return TRUE;
	}
	*/
	// Disable되었을 때
	if (!bEnabled)  
	{
		if (bHeaderCol || bHeaderRow) style           // Row or Column Header
			.SetControl(GX_IDS_CTRL_HEADER)
			.SetInterior(::GetSysColor(COLOR_BTNFACE))
			.SetTextColor(COLORREF(RGB(0, 0, 0)));
		else if (bCurrentRow) style     // Disabled Current Row
			.SetInterior(COLORREF(RGB(192, 192, 192)))
			.SetTextColor(COLORREF(RGB(0, 0, 0)));
		else style                      // Disabled Cell
			.SetInterior(COLORREF(RGB(192, 192, 192)))
			.SetTextColor(COLORREF(RGB(0, 0, 0)));

		return TRUE;
	}
	/*
	if (bCurrentCell) style
		.SetInterior(COLORREF(RGB(255, 251, 240)))
		.SetTextColor(COLORREF(RGB(0, 0, 0)));
	else if (bCurrentRow) style
		.SetInterior(COLORREF(RGB(228, 236, 247)))
		.SetTextColor(COLORREF(RGB(0, 0, 0)));
	else if (bHeaderCol || bHeaderRow) style    // Enabled Header Col(Only Frozen)
		.SetInterior(COLORREF(RGB(228, 236, 247)))
		.SetTextColor(COLORREF(RGB(0, 0, 0)));
		*/
	return TRUE;
}

BOOL CCmdAutoLoadCombImperfectionLoadGrid::OnStartEditing(ROWCOL nRow, ROWCOL nCol)
{
	return CTBCommon::OnStartEditing(nRow, nCol);
}

void CCmdAutoLoadCombImperfectionLoadGrid::OnModifyCell(ROWCOL nRow, ROWCOL nCol)
{
	CTBCommon::OnModifyCell(nRow, nCol);
}

BOOL CCmdAutoLoadCombImperfectionLoadGrid::OnEndEditing(ROWCOL nRow, ROWCOL nCol)
{
	return CTBCommon::OnEndEditing(nRow, nCol);
}

BOOL CCmdAutoLoadCombImperfectionLoadGrid::ProcessKeys(CWnd *pSender, UINT nMessage, UINT nChar, UINT nRepCnt, UINT flags)
{
	CRowColArray awRows;
	ROWCOL nRow=1, nCol=1;

	if(!GetCurrentCell(nRow, nCol)) // if no current cell, default action
		return CTBCommon::ProcessKeys(pSender, nMessage, nChar, nRepCnt, flags);

	if(nMessage == WM_KEYDOWN)
	{
		switch(nChar)
		{
		case VK_TAB: 
			{
				BOOL bShift = GetKeyState(VK_SHIFT) & 0x8000;
				if( bShift )
				{
					if( SetLeftValidCell(nRow,  nCol)) { goto GT_PROC_RETURN; }
				}

				if(!bShift )
				{
					if( SetRightValidCell(nRow, nCol)) { goto GT_PROC_RETURN; }
				}
			}
			break;
		default: 
			break;
		}
	}

	return CTBCommon::ProcessKeys(pSender, nMessage, nChar, nRepCnt, flags); 

GT_PROC_RETURN:
	if(nMessage==WM_KEYDOWN && (nChar==VK_TAB || nChar==VK_RETURN || nChar==VK_RIGHT || nChar==VK_LEFT))
	{
		Redraw();    
	}

	return TRUE;
}

BOOL CCmdAutoLoadCombImperfectionLoadGrid::SetRightValidCell(ROWCOL nRow, ROWCOL nCol)
{
	long i=nRow, j=nCol+1; // 현재 행 다음 열
	int nColCount = GetColCount();
	if(j>nColCount) { ++i; j=1; }

	BOOL bCovered;/* = FALSE;*/
	while(i<=GetRowCount())
	{
		bCovered = FALSE;

		while(j<=nColCount)
		{
			// 열이 선택될 수 있는지 조사
			CGXStyle style; style.Free();
			ComposeStyleRowCol(i, j, &style);

			// Is it a covered cell?
			CGXRange cvr;
			if(!bCovered && GetCoveredCellsRowCol(i, j, cvr))
			{
				if((cvr.top<=i && i<=cvr.bottom) && (cvr.left<=j && j<=cvr.right))
				{
					if (nRow == cvr.top) i = cvr.bottom;
					j = cvr.right+1;
					bCovered = TRUE;
					continue;
				}
			}

			if(IsRowHidden(i)) break;
			if(IsColHidden(j)) { ++j; continue; }

			BOOL bEnable = (!style.GetIncludeEnabled() || style.GetEnabled());
			BOOL bReadOnly = style.GetReadOnly();

			if(bEnable && !bReadOnly)
			{
				if(SetCurrentCell(i, j)) return TRUE;
				return FALSE;
			}
			//bCovered = TRUE;
			bCovered = FALSE;

			++j;  // 다음 열을 조사
		}
		if(j>nColCount) { ++i; j=1; }
	}
	return FALSE;
}

BOOL CCmdAutoLoadCombImperfectionLoadGrid::SetLeftValidCell(ROWCOL nRow, ROWCOL nCol)
{
	long i=nRow, j=nCol-1; // 현재 행 이전 열
	int nColCount = GetColCount();
	if(nCol==0 || j<1) { --i; j=nColCount; } 

	BOOL bCovered = FALSE;
	while(i>=1)
	{
		bCovered = FALSE;
		while(j>=1)
		{
			// 열이 선택될 수 있는지 조사
			CGXStyle style; style.Free();
			ComposeStyleRowCol(i, j, &style);

			// Is it a covered cell?
			CGXRange cvr;
			if(!bCovered && GetCoveredCellsRowCol(i, j, cvr))
			{
				if((cvr.top<=i && i<=cvr.bottom) && (cvr.left<=j && j<=cvr.right))
				{
					i = cvr.bottom;
					j = cvr.left-1;
					bCovered = TRUE;
					continue;
				}
			}

			if(IsRowHidden(i)) break;
			if(IsColHidden(j)) { --j; continue; }

			BOOL bEnable = !style.GetIncludeEnabled() || style.GetEnabled();
			BOOL bReadOnly = style.GetReadOnly();

			if(bEnable && !bReadOnly)
			{
				if(GetCoveredCellsRowCol(i, j, cvr))
				{
					if((cvr.top<=i && i<=cvr.bottom) && (cvr.left<=j && j<=cvr.right))
					{
						j = cvr.left;
					}
				}
				if(SetCurrentCell(i, j)) return TRUE;
				return FALSE;
			}
			bCovered = TRUE;
			--j;  // 이전 열을 조사
		}
		if(nCol==0 || j<1) { --i; j=nColCount; } 
	}
	return FALSE;
}

BOOL CCmdAutoLoadCombImperfectionLoadGrid::CopyRange(const CGXRangeList& selList)
{
	CGXRangeList mSelList;
	CGXRange* mRange;
	POSITION position=selList.GetHeadPosition();

	while(TRUE)
	{
		if(!position||position<0)
			break;

		mRange=new CGXRange();
		if(mRange->IntersectRange(CGXRange(1,1,GetRowCount(),GetColCount()),selList.GetAt(position)))
		{
			mSelList.AddTail(mRange);
		}
		else
			delete mRange;

		selList.GetNext(position);
	}

	return CTBCommon::CopyRange(mSelList);
}

// 첫번째 열에 번호 삽입 위해 추가(일반적으로는 필요없음)
BOOL CCmdAutoLoadCombImperfectionLoadGrid::AddNew(ROWCOL nRow)
{
	m_nEditMode = addnew;

	// InsertRows를 수행할 때 Current Cell이 포커스를 잃는 것을 막기 위해
	// SetActive(FALSE)를 한 후 InsertRows()를 하고 SetActive(TRUE)를 한다.
	// 이것은 InsertRows의 내부에서 TransferCurrentCell을 호출하게 되는데
	// TransferCurrentCell은 현재 컨트롤이 Active상태이면 컨트롤의 내용을
	// 저장하고 컨트롤을 Deactive시키기 때문이다.
	ROWCOL ncRow, ncCol;
	GetCurrentCell(ncRow, ncCol);
	CGXControl* pControl = GetControl(ncRow, ncCol);
	BOOL bActive = FALSE;
	if (pControl && pControl->IsActive()) bActive = TRUE;
	if (bActive) pControl->SetActive(FALSE);

	InsertRows(nRow+1, 1);

	if (bActive) pControl->SetActive(TRUE);

	RedrawRowCol(nRow, 0);
	OnInsertEmptyRecord(nRow);

	// 추가
	CString strNo;
	strNo.Format(_T("%d"), nRow);
	SetValueRange(CGXRange(nRow, D_COL_NO), strNo);
	//

	return TRUE;
}

BOOL CCmdAutoLoadCombImperfectionLoadGrid::AddToDB( ROWCOL nRow )
{
	// DB는 안쓰고 기본적인 기능만쓰고싶은데.. ADDToDB가 끝나면 행하나를 지워버리기 떄문에 미리 하나 더 복사해서 만들어 줘야함.. ㅠㅠ  
	InsertRecord(nRow);

	CString strVal;
	for (int i = 1; i <= D_COL_NUM; i++)
	{
		if(i==1)
		{
			strVal.Format(_T("%d"), nRow);
		}
		else
		{
			strVal = GetValueRowCol(nRow + 1, i);
		}
		SetValueRange(CGXRange(nRow, i), strVal);
	}

	return TRUE;
}

BOOL CCmdAutoLoadCombImperfectionLoadGrid::DeleteFromDB( ROWCOL nRow )
{
	DeleteRecord(nRow);

	CString str;
	int nSize  = GetRowCount();
	for(int i=0; i<nSize-1; i++)
	{
		str.Format(_T("%d"), i+1);
		SetExpressionRowCol(i+1, 1, str);
	}

	return TRUE;

}

BOOL CCmdAutoLoadCombImperfectionLoadGrid::DeleteFromDB( CRowColArray& awRows )
{
	for (int i = awRows.GetSize() - 1; i >= 0; i--)
	{
		DeleteRecord(awRows[i]);
	}

	CString str;
	int nSize  = GetRowCount();
	for(int i=0; i<nSize-1; i++)
	{
		str.Format(_T("%d"), i+1);
		SetExpressionRowCol(i+1, 1, str);
	}

	return TRUE;
}

BOOL CCmdAutoLoadCombImperfectionLoadGrid::OnPasteFromClipboard(const CGXRange &range)
{
	OpenClipboard(); // open clipboard
	HGLOBAL handle = GetClipboardData(CF_UNICODETEXT);
	LPTSTR psz = (LPTSTR)GlobalLock(handle);
	TCHAR* pTChar = (TCHAR*)psz;
	DWORD size = (DWORD)GlobalSize(handle);
	GlobalUnlock(handle);
	CloseClipboard(); // close the clipboard

	char	strMultibyte[1024] = {0,};
	WideCharToMultiByte(CP_ACP, 0, (LPCTSTR)psz, -1, strMultibyte, 1024 ,NULL ,NULL);

	CString  sub = _T(""), szData = strMultibyte;
	LPTSTR   buf = szData.GetBuffer(1);
	int      pos = 0, endpos, len = szData.GetLength();
	TCHAR    endchar = L' ';
	int		 col = range.left;
	int  	 row = range.top;	

	BOOL bPasteToAllRange = IsPasteToAllRange(range, szData);

	BOOL bOldLock = LockUpdate(TRUE);

	BOOL bFirstDone = FALSE;
	CString szSubFirst = _T("");

	while(pos < len)
	{
		if(buf[pos] == _T('\t') || buf[pos] == _T('\r') || buf[pos] == _T('\n'))
		{
			endchar = buf[pos];
			endpos = pos;
		}		
		else // find the end of the item then copy the item to the cell
		{
			endpos = pos + 1;
			while(endpos < len)
			{
				endchar = buf[endpos];
				if(endchar == _T('\n') || endchar == _T('\r') || endchar == _T('\t')) break;
				endpos++;
			}

			if(col <= (int)GetColCount() && row <= (int)GetRowCount())
			{	
				sub = szData.Mid(pos, endpos - pos);
				if(bFirstDone == FALSE)
				{
					szSubFirst = sub;
				}
				SetValueRange(CGXRange(row, col), sub);
			}
		}

		if(endchar == _T('\t')) col++;
		if(endchar == _T('\r') || endchar == _T('\n'))
		{
			col = range.left;
			row++;
			if(buf[endpos] == _T('\r') && buf[endpos + 1] == _T('\n')) endpos++;
		}
		pos = endpos + 1;
	}

	if(bPasteToAllRange && !szSubFirst.IsEmpty())
	{
		while(row <= (int)range.bottom)
		{
			SetValueRange(CGXRange(row, col), szSubFirst);

			row++;
		}
	}

	szData.ReleaseBuffer();

	LockUpdate(bOldLock);
	if (!bOldLock) Redraw();

	return TRUE;	
}

BOOL CCmdAutoLoadCombImperfectionLoadGrid::IsPasteToAllRange(const CGXRange &range, CString szData) // copy한 Cell이 1개 이고, 동일한 행의 cell 여러개로 붙여넣은 상황일 때 TRUE.. 이 경우엔 붙여넣을 Range의 모든 Cell에 복사한 Cell의 값을 붙여 넣어 줘야 함
{
	// 붙여넣을 Cell이 모두 동일한 행에 있는지 확인
	if(szData.Find(_T('\t')) != -1 || szData.Find(_T('\r')) != -1 || szData.Find(_T('\n')) != -1 )
	{
		return FALSE;
	}

	// 복사한 cell이 하나인지 확인
	if(range.left != range.right)
	{
		return FALSE;
	}

	return TRUE;
}

int CCmdAutoLoadCombImperfectionLoadGrid::GetDirectionData(CString &value)
{
	int nDir = 0;

	if     (value == _LS(IDS_CMD_IMP_GLOBAL_X)) nDir = 0;
	else if(value == _LS(IDS_CMD_IMP_GLOBAL_Y)) nDir = 1;
	else ASSERT(0);

	return nDir;
}

CString CCmdAutoLoadCombImperfectionLoadGrid::GetDirectionStr(int data)
{
	CString strDir;

	if     (data == 0) strDir = _LS(IDS_CMD_IMP_GLOBAL_X);
	else if(data == 1) strDir = _LS(IDS_CMD_IMP_GLOBAL_Y);
	else ASSERT(0);

	return strDir;
}

CString CCmdAutoLoadCombImperfectionLoadGrid::GetLCase1Str(T_STLD_K LCase1K)
{
	CString strStld;

	T_STLD_D StldD; StldD.Initialize();
	if(!m_pDoc->m_pAttrCtrl->GetStld(LCase1K, StldD)) return strStld;
	
	strStld = StldD.LoadCaseName + _T("(ST)");

	return strStld;
}
/*
CString CCmdAutoLoadCombImperfectionLoadGrid::GetLCase2Str(UINT LCase2K, UINT nAnalType)
{
	CString strStld;

	T_STLD_D stldData;
	T_SPLC_D splcData;
	T_THIS_D thisData;
	T_MVLD_D mvldData;
	T_MVLDjp_D mvldjpData;
	T_MVLDch_D mvldchData;
	T_MVLDid_D mvldidData;
	T_MVLDbs_D mvldbsData;
	T_MVLDtr_D mvldtrData;
	T_SMLC_D smlcData;
	T_SGLD_D sgldData;
	T_ESPC_D espcData;
	T_GILC_D gilcData;

	BOOL bError = FALSE;

	switch(nAnalType)
	{
	case D_LCOM_STATIC: 
		if(!m_pDoc->m_pAttrCtrl->GetStld(LCase2K, stldData)) strStld = _T("");
		else strStld = stldData.LoadCaseName + _T("(ST)"); 
		break;
	case D_LCOM_SPECTRUM:
		if (!m_pDoc->m_pAttrCtrl->GetSplc(LCase2K, splcData)) strStld = _T("");
		else strStld = splcData.LoadCaseName + _T("(RS)"); 
		break;
	case D_LCOM_HISTORY:
		if (!m_pDoc->m_pAttrCtrl->GetThis(LCase2K, thisData)) strStld = _T("");
		else strStld = thisData.LoadCaseName + _T("(TH)"); 
		break;
	case D_LCOM_MOVING:
		{
			// 코드에 따른 방식으로 수정 
			T_MVCD_D DataMvcd;
			if(!m_pDoc->m_pAttrCtrl->GetMvcd(DataMvcd)) DataMvcd.Initialize();
			switch (DataMvcd.nCodeType)
			{
			case D_MOVE_CODE_NONE:
				break;
			case D_MOVE_CODE_AASHTO_STAN:
			case D_MOVE_CODE_AASHTO_LRFD:
			case D_MOVE_CODE_TAIWAN:
			case D_MOVE_CODE_KOREA:
			case D_MOVE_CODE_CANADA:
			case D_MOVE_CODE_PENDOT:
			case D_MOVE_CODE_EURO_BS:
			case D_MOVE_CODE_RUSSIA:
			case D_MOVE_CODE_KOREA_LRFD_2011:
			case D_MOVE_CODE_AUSTRALIA:
			case D_MOVE_CODE_POLAND:
				{
					if (!m_pDoc->m_pAttrCtrl->GetMvld(LCase2K, mvldData)) strStld = _T("");
					else strStld = mvldData.LoadCaseName + _T("(MV)"); 
				}
				break;
			case D_MOVE_CODE_CHINA:
				{
					if (!m_pDoc->m_pAttrCtrl->GetMvldch(LCase2K, mvldchData)) strStld = _T("");
					else strStld = mvldchData.LoadCaseName + _T("(MV)"); 
				}
				break;
			case D_MOVE_CODE_JAPAN:
				{
					if (!m_pDoc->m_pAttrCtrl->GetMvldjp(LCase2K, mvldjpData)) strStld = _T("");
					else strStld = mvldjpData.LoadCaseName + _T("(MV)"); 
				}
				break;
			case D_MOVE_CODE_INDIA:
				{
					if (!m_pDoc->m_pAttrCtrl->GetMvldid(LCase2K, mvldidData)) strStld = _T("");
					else strStld = mvldidData.LoadCaseName + _T("(MV)"); 
				}
				break;
			case D_MOVE_CODE_BS:
				{
					if (!m_pDoc->m_pAttrCtrl->GetMvldbs(LCase2K, mvldbsData)) strStld = _T("");
					else strStld = mvldbsData.LoadCaseName + _T("(MV)"); 
				}
				break;
			case D_MOVE_CODE_TRANS:
				{
					if (!m_pDoc->m_pAttrCtrl->GetMvldtr(LCase2K, mvldtrData)) strStld = _T("");
					else strStld = mvldtrData.LoadCaseName + _T("(MV)"); 
				}
				break;
			default: ASSERT(0);
				break;
			}
		}
		break;
	case D_LCOM_SETTLEMENT:
		if (!m_pDoc->m_pAttrCtrl->GetSmlc(LCase2K, smlcData)) strStld = _T("");
		else strStld = smlcData.LoadCaseName + _T("(SM)"); 
		break;
	case D_LCOM_STAGE:
		if (!m_pDoc->m_pAttrCtrl->GetSgld(LCase2K, sgldData)) strStld = _T("");
		else strStld = sgldData.LoadCaseName + _T("(CS)"); 
		break;
	case D_LCOM_ECCENSPEC:
		if (!m_pDoc->m_pAttrCtrl->GetEspc(LCase2K, espcData)) strStld = _T("");
		else strStld = espcData.LoadCaseName + _T("(ES)"); 
		break;
	case D_LCOM_GRIDMODEL:
		if (!m_pDoc->m_pAttrCtrl->GetGilc(LCase2K, gilcData)) strStld = _T("");
		else strStld = espcData.LoadCaseName + _T("(GI)"); 
		break;    
	case D_LCOM_CRANE:
		if(m_pDoc->m_pAttrCtrl->IsExistCranBraking()) strStld = _T("");
		else strStld = _LS(IDS_CMD_CRANE_BRK) + _T("(CN)"); 
		break;    
	default:
		bError = TRUE;
	}
	
	return strStld;
}*/