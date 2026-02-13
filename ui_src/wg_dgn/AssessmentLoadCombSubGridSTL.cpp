// AssessmentLoadCombSubGridSTL.cpp : implementation file
//

#include "stdafx.h"

#include "wg_dgn.h"
#include "AssessmentLoadCombSubGridSTL.h"

#include "..\wg_db\wg_db_AttrCtrl.h"

#include "AssessmentLoadCombMainGridSTL.h"
#include "..\wg_base\wg_base_MsgDll.h"

#include "..\wg_db\AttrCtrl2.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAssessmentLoadCombSubGridSTL
CAssessmentLoadCombSubGridSTL::CAssessmentLoadCombSubGridSTL()
{
	m_pMainGrid = 0;
	m_sKey = _LS(IDS_WG_CMD__ADDD__None);
	m_pDoc = 0;
	m_nLcomType = D_LCOMTYPE_GENERAL;
	m_nLCtype = D_LCOM_CB_GENERAL;
	m_csPostfix = _T("CB");

	m_aTitle.RemoveAll();

	m_aTitle.Add(_LS(IDS_CMD_STATIC_LOAD_CASES));
	m_aTitle.Add(_LS(IDS_WG_CMD__ADDD__Factor));
}

CAssessmentLoadCombSubGridSTL::~CAssessmentLoadCombSubGridSTL()
{
}

BEGIN_MESSAGE_MAP(CAssessmentLoadCombSubGridSTL, CTBBrowserWnd)
	//{{AFX_MSG_MAP(CAssessmentLoadCombSubGridSTL)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CAssessmentLoadCombSubGridSTL message handlers
void CAssessmentLoadCombSubGridSTL::Initialize(CDBDoc* pDoc)
{
	CTBBrowserWnd::Initialize();
	m_pDoc = pDoc;

	LockUpdate(TRUE);

	// 컬럼 갯수 설정
	SetColCount(m_aTitle.GetSize());
	SetHeaderTitle();

	// 표준 스타일 변경
	ChangeStandardStyle(CGXStyle()
		.SetFont(CGXFont_GC().SetSize(10))
		.SetVerticalAlignment(DT_BOTTOM)
		.SetAllowEnter(FALSE));

	// 컬럼 스타일 변경

	// Loadcase list

	SetLoadCaseColumnStyle(D_COL_RAC_LCASE);

	SetStyleRange(CGXRange().SetCols(D_COL_RAC_FACTOR), CGXStyle()
		.SetHorizontalAlignment(DT_RIGHT)
		.SetValueType(GX_VT_NUMERIC)
		.SetFormat(GX_FMT_FIXED)
		.SetPlaces(4));

	// 현재 셀의 border 설정 변경
	CGXProperties* pProp = GetParam()->GetProperties();
	pProp->SetUserProperty(GX_IDS_OUTLINECURRENTCELL,
		(CGXStyle)pProp->sInvertNoBorder);

	// 현재 셀 설정
	SetRightValidCell(1, 0);
	GetParam()->SetHideCurrentCell(GX_HIDE_NEVER);

	SetScrollBarMode(SB_VERT, gxnAutomatic);
	SetScrollBarMode(SB_HORZ, gxnAutomatic);

	// column size 설정
#if defined(_US) || defined(_RUS)
	double scale = 1.154;
#else
	double scale = 1.0;
#endif

	auto L_SetColWidth = [this] (ROWCOL nCol, int nWidth)
	{
		SetColWidth(nCol, nCol, globalUtils.ScaleByDPI(nWidth));
	};

	L_SetColWidth(D_COL_RAC_LCASE, 200 / scale);
	L_SetColWidth(D_COL_RAC_FACTOR, 80 / scale);

	GetParam()->EnableTrackColWidth(TRUE);
	GetParam()->EnableMoveCols(FALSE);
	GetParam()->EnableTrackRowHeight(FALSE);

	// 데이타 설정
	//MakeItemEx();
	LockUpdate(FALSE);
}

// Column별 헤더 이름을 설정한다.
void CAssessmentLoadCombSubGridSTL::SetHeaderTitle()
{
	CString title;
	int i;
	ROWCOL nCol;

	for (i = 0; i < GetColCount(); i++)
	{
		title = m_aTitle[i];
		nCol = i + 1;

		// 컬럼 헤더 이름 및 ID를 설정
		SetStyleRange(CGXRange(0, nCol), CGXStyle()
			.SetValue(title)
			.SetUserAttribute(ID_USER_ATTR_COL_ID, m_aTitle[i]));
	}
}

// LoadCase Column의 Style을 지정한다.
// LoadCase 추가, 삭제, 수정시 Update를 콤보 박스에 반영하기 위해 사용하면 된다.
void CAssessmentLoadCombSubGridSTL::SetLoadCaseColumnStyle(ROWCOL nCol)
{
	CString szLoadCaseList = _T("");

	POSITION pos;
	CString item;
	int nRangeStart;

	int nNameLen = 0, nDescLen = 0;
	CSize nTempLen;
	CDC* pDC = GetDC();
	ASSERT(pDC);

	if (nCol == D_COL_RAC_LCASE)
	{
		//D_LCOM_STATIC		1 : 정적 하중 조건
		T_STLD_K Key;
		T_STLD_D Data;
		nRangeStart = D_STLD_MAXNUM * D_LCOM_STATIC;
		pos = m_pDoc->m_pAttrCtrl->GetStartStld();
		while (pos != NULL)
		{
			m_pDoc->m_pAttrCtrl->GetNextStld(pos, Key, Data);
			if (Data.LoadCaseType == _T("CS") || Data.LoadCaseType == _T("TCS")) continue;  // 시공타입은 제외
			item.Format(_T("%d\t%d\t%s(ST)\t%s\n"),
				nRangeStart + Key, nRangeStart + Data.LoadCaseId, Data.LoadCaseName,
				Data.Description);
			szLoadCaseList += item;
			nTempLen = pDC->GetTextExtent(Data.LoadCaseName + _T("(ST)"));
			if (nTempLen.cx > nNameLen) nNameLen = nTempLen.cx;
			nTempLen = pDC->GetTextExtent(Data.Description);
			if (nTempLen.cx > nDescLen) nDescLen = nTempLen.cx;
		}

		//D_LCOM_STAGE 13: 시공단계 하중 조건
		CArray<T_SGLD_K, T_SGLD_K> aSgldKey;
		T_SGLD_K SgldKey;
		T_SGLD_D SgldData;
		nRangeStart = D_STLD_MAXNUM * D_LCOM_STAGE;
		m_pDoc->m_pAttrCtrl->GetSgldKeyList(aSgldKey);
		int nSgldNum = aSgldKey.GetSize();
		for (int n = 0; n < nSgldNum; n++)
		{
			SgldKey = aSgldKey[n];
			if (m_pDoc->m_pAttrCtrl->GetSgld(SgldKey, SgldData))
			{
				item.Format(_T("%d\t%d\t%s(CS)\t%s\n"),
					nRangeStart + SgldKey, nRangeStart + SgldData.LoadCaseId, SgldData.LoadCaseName,
					SgldData.Description);
				szLoadCaseList += item;
				nTempLen = pDC->GetTextExtent(SgldData.LoadCaseName + _T("(CS)"));
				if (nTempLen.cx > nNameLen) nNameLen = nTempLen.cx;
				nTempLen = pDC->GetTextExtent(SgldData.Description);
				if (nTempLen.cx > nDescLen) nDescLen = nTempLen.cx;
			}
		}

		CString csSize;
		csSize.Format(_T("0,0,%d,%d"), nNameLen, nDescLen);
		SetStyleRange(CGXRange().SetCols(nCol), CGXStyle()
			.SetControl(GX_IDS_CTRL_TABBED_COMBOBOX)
			.SetChoiceList(szLoadCaseList)
			.SetUserAttribute(GX_IDS_UA_TABLIST_KEYCOL, _T("0"))
			.SetUserAttribute(GX_IDS_UA_TABLIST_TEXTCOL, _T("2"))
			.SetUserAttribute(GX_IDS_UA_TABLIST_SHOWALLCOLS, _T("1"))
			.SetUserAttribute(GX_IDS_UA_TABLIST_COLWIDTHS, csSize)
			.SetUserAttribute(GX_IDS_UA_TABLIST_SORTCOL, _T("1"))
			.SetValueType(GX_VT_NUMERIC)
			.SetHorizontalAlignment(DT_LEFT)
			.SetValue(_T(""))
		);
	}

	ReleaseDC(pDC);
}

BOOL CAssessmentLoadCombSubGridSTL::IsMainAppendRow()
{
	return m_sKey == _LS(IDS_WG_CMD__ADDD__Append_Row);
}

BOOL CAssessmentLoadCombSubGridSTL::IsMainNewRow()
{
	return m_sKey == _LS(IDS_WG_CMD__ADDD__New_Row);
}

BOOL CAssessmentLoadCombSubGridSTL::OnAddNewRecord(ROWCOL nRow)
{
	BOOL bRetVal = CTBBrowserWnd::OnAddNewRecord(nRow);

	// MQC 6867   ////////////////////////////////////////////////////
	CString value;
	value = GetValueRowCol(nRow, D_COL_RAC_LCASE);
	value.TrimLeft();
	value.TrimRight();

	if (value.IsEmpty()) return FALSE;
	//////////////////////////////////////////////////////////////////

	//SetRecordModified();
	return bRetVal;
}

BOOL CAssessmentLoadCombSubGridSTL::OnModifyRecord(ROWCOL nRow)
{
	BOOL bRetVal = CTBBrowserWnd::OnModifyRecord(nRow);

	// MQC 6867   ////////////////////////////////////////////////////
	CString value;
	value = GetValueRowCol(nRow, D_COL_RAC_LCASE);
	value.TrimLeft();
	value.TrimRight();

	if (value.IsEmpty()) return FALSE;
	//////////////////////////////////////////////////////////////////

	//SetRecordModified();
	return bRetVal;
}

BOOL CAssessmentLoadCombSubGridSTL::OnDeleteRecord(ROWCOL nRow)
{
	BOOL bRetVal = CTBBrowserWnd::OnDeleteRecord(nRow);
	m_pMainGrid->SetModeEdit();
	return bRetVal;
}

BOOL CAssessmentLoadCombSubGridSTL::OnDeleteRecords(CRowColArray &awRows)
{
	BOOL bRetVal = CTBBrowserWnd::OnDeleteRecords(awRows);
	m_pMainGrid->SetModeEdit();
	return bRetVal;
}

BOOL CAssessmentLoadCombSubGridSTL::OnCanceledAddNew()
{
	BOOL bRetVal = CTBBrowserWnd::OnCanceledAddNew();
	return bRetVal;
}

BOOL CAssessmentLoadCombSubGridSTL::OnCanceledModify()
{
	BOOL bRetVal = CTBBrowserWnd::OnCanceledModify();
	return bRetVal;
}

BOOL CAssessmentLoadCombSubGridSTL::OnInsertEmptyRecord(ROWCOL nRow)
{
	// 여기서는 Default 값을 채워 넣는 역할을 하면 된다.
	BOOL bActive;
	ROWCOL ncRow, ncCol;
	CString value;

	// 현재 셀이 Active되어 있으면 값을 채워 넣지 않는다.
	VERIFY(GetCurrentCell(ncRow, ncCol));
	CGXControl* pControl = (CGXControl*)GetControl(ncRow, ncCol);
	if (pControl && pControl->IsActive()) bActive = TRUE;
	else bActive = FALSE;

	if (ncCol != D_COL_RAC_LCASE)
	{
		CString sKey;
		CGXStyle style;
		VERIFY(GetColStyle(D_COL_RAC_LCASE, style));
		CString szChoiceList = style.GetChoiceList();
		if (szChoiceList == _T("")) return TRUE;  // ChoiceList없으면 그냥 리턴
		int nPos = szChoiceList.Find('\t'); // 첫번째 분리자를 찾는다.(key)
		VERIFY(nPos != -1);
		sKey = szChoiceList.Left(nPos);
		SetExpressionRowCol(nRow, D_COL_RAC_LCASE, sKey); // set default Load Case value
	}

	if (ncCol != D_COL_RAC_FACTOR)
	{
		SetExpressionRowCol(nRow, D_COL_RAC_FACTOR, _T("1"));  // set  default check status
	}

	return TRUE;
}

// 셀이 수정되면 EditMode로 들어간다.
// 주1:MainGrid가 AppendRow에 있을 때는 셀이 수정되면 먼저 AddNew가 호출되어야 한다.
// 이 때는 현재 Record의 위치가 변경되었으므로 OnChangeCurrentRecord가 호출되어야
// 하는데 셀이 수정된 후 SetRecordModified가 호출되면 수정된 내용이 지워지므로 
// SetRecordModified를 먼저 수행한다.
void CAssessmentLoadCombSubGridSTL::OnModifyCell(ROWCOL nRow, ROWCOL nCol)
{
	m_pMainGrid->SetModeEdit();// 먼저 호출되어야 한다. 주1 참조
	CTBBrowserWnd::OnModifyCell(nRow, nCol);
}

void CAssessmentLoadCombSubGridSTL::OnChangeStld()
{
	// 지워진 하중들은 제거한다.
	T_STLD_D stldData;
	T_SPLC_D splcData;
	T_THIS_D thisData;
	T_MVLD_D mvldData;
	T_MVLDjp_D mvldjpData;
	T_MVLDch_D mvldchData;
	T_MVLDid_D mvldidData;
	T_MVLDbs_D mvldbsData;
	T_MVLDfr_D mvldfrData;
	T_MVLDtr_D mvldtrData;
	T_SMLC_D smlcData;
	T_SGLD_D sgldData;
	T_ESPC_D espcData;
	T_GILC_D gilcData;
	T_LCOM_D lcomData;

	int nAnalType;
	T_STLD_K nKey;
	int nSize = GetRowCount() - 1;
	for (int i = nSize - 1; i >= 0; i--)
	{
		nKey = _ttol(GetValueRowCol(i + 1, 1));
		nAnalType = nKey / D_STLD_MAXNUM;
		nKey = nKey % D_STLD_MAXNUM;
		switch (nAnalType)
		{
		case D_LCOM_STATIC:
			if (!m_pDoc->m_pAttrCtrl->GetStld(nKey, stldData))
				DeleteRecord(i + 1);
			break;
		case D_LCOM_SPECTRUM:
			if (!m_pDoc->m_pAttrCtrl->GetSplc(nKey, splcData))
				DeleteRecord(i + 1);
			break;
		case D_LCOM_HISTORY:
			if (!m_pDoc->m_pAttrCtrl->GetThis(nKey, thisData))
				DeleteRecord(i + 1);
			break;
		case D_LCOM_MOVING:
		{
			// 코드에 따른 방식으로 수정 
			T_MVCD_D DataMvcd;
			if (!m_pDoc->m_pAttrCtrl->GetMvcd(DataMvcd)) DataMvcd.Initialize();
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
			case D_MOVE_CODE_NEWZEALAND:
			case D_MOVE_CODE_BRAZIL:
			{
				if (!m_pDoc->m_pAttrCtrl->GetMvld(nKey, mvldData))
					DeleteRecord(i + 1);
			}
			break;
			case D_MOVE_CODE_CHINA:
			{
				if (!m_pDoc->m_pAttrCtrl->GetMvldch(nKey, mvldchData))
					DeleteRecord(i + 1);
			}
			break;
			case D_MOVE_CODE_JAPAN:
			{
				if (!m_pDoc->m_pAttrCtrl->GetMvldjp(nKey, mvldjpData))
					DeleteRecord(i + 1);
			}
			break;
			case D_MOVE_CODE_INDIA:
			{
				if (!m_pDoc->m_pAttrCtrl->GetMvldid(nKey, mvldidData))
					DeleteRecord(i + 1);
			}
			break;
			case D_MOVE_CODE_BS:
			{
				if (!m_pDoc->m_pAttrCtrl->GetMvldbs(nKey, mvldbsData))
					DeleteRecord(i + 1);
			}
			case D_MOVE_CODE_FRANCE:
			{
				if (!m_pDoc->m_pAttrCtrl->GetMvldfr(nKey, mvldfrData))
					DeleteRecord(i + 1);
			}
			case D_MOVE_CODE_TRANS:
			{
				if (!m_pDoc->m_pAttrCtrl->GetMvldtr(nKey, mvldtrData))
					DeleteRecord(i + 1);
			}
			break;
			default: ASSERT(0);
				break;
			}
		}
		break;
		case D_LCOM_SETTLEMENT:
			if (!m_pDoc->m_pAttrCtrl->GetSmlc(nKey, smlcData))
				DeleteRecord(i + 1);
		case D_LCOM_STAGE:
			if (!m_pDoc->m_pAttrCtrl->GetSgld(nKey, sgldData))
				DeleteRecord(i + 1);
			break;
		case D_LCOM_ECCENSPEC:
			if (!m_pDoc->m_pAttrCtrl->GetEspc(nKey, espcData))
				DeleteRecord(i + 1);
			break;
		case D_LCOM_GRIDMODEL:
			if (!m_pDoc->m_pAttrCtrl->GetGilc(nKey, gilcData))
				DeleteRecord(i + 1);
			break;
		case D_LCOM_CB_GENERAL:
		case D_LCOM_CB_STEEL:
		case D_LCOM_CB_CONCRETE:
		case D_LCOM_CB_FOOTING:
		case D_LCOM_CB_SRC:
		case D_LCOM_CB_ALUMINUM:
		case D_LCOM_CB_STLCOMP:
			ASSERT(0);
			if (!m_pDoc->m_pAttrCtrl->GetLcom(m_nLcomType, nKey, lcomData))
				DeleteRecord(i + 1);
			break;
		}
	}

	SetLoadCaseColumnStyle(D_COL_RAC_LCASE);

	// 현재 셀을 다시 초기화 시킨다.
	ROWCOL ncRow, ncCol;
	if (!GetCurrentCell(ncRow, ncCol)) return;
	CGXControl* pControl = this->GetControl(ncRow, ncCol);
	if (pControl) pControl->Init(ncRow, ncCol);
}

BOOL CAssessmentLoadCombSubGridSTL::OnValidateField(ROWCOL nRow, ROWCOL nCol)
{
	// Factor가 실수(정수포함)로 입력되어 있는지 검사한다.
	if (nCol == D_COL_RAC_LCASE)
	{
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
		T_LCOM_D lcomData;

		int nAnalType;
		T_STLD_K nKey;

		nKey = _ttol(GetValueRowCol(nRow, nCol));
		nAnalType = nKey / D_STLD_MAXNUM;
		nKey = nKey % D_STLD_MAXNUM;

		BOOL bError = FALSE;

		switch (nAnalType)
		{
		case D_LCOM_STATIC:
			if (!m_pDoc->m_pAttrCtrl->GetStld(nKey, stldData))
				bError = TRUE;
			break;
		case D_LCOM_SPECTRUM:
			if (!m_pDoc->m_pAttrCtrl->GetSplc(nKey, splcData))
				bError = TRUE;
			break;
		case D_LCOM_HISTORY:
			if (!m_pDoc->m_pAttrCtrl->GetThis(nKey, thisData))
				bError = TRUE;
			break;
		case D_LCOM_MOVING:
		{
			// 코드에 따른 방식으로 수정 
			T_MVCD_D DataMvcd;
			if (!m_pDoc->m_pAttrCtrl->GetMvcd(DataMvcd)) DataMvcd.Initialize();
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
			case D_MOVE_CODE_NEWZEALAND:
			case D_MOVE_CODE_BRAZIL:
			{
				if (!m_pDoc->m_pAttrCtrl->GetMvld(nKey, mvldData))
					bError = TRUE;
			}
			break;
			case D_MOVE_CODE_CHINA:
			{
				if (!m_pDoc->m_pAttrCtrl->GetMvldch(nKey, mvldchData))
					bError = TRUE;
			}
			break;
			case D_MOVE_CODE_JAPAN:
			{
				if (!m_pDoc->m_pAttrCtrl->GetMvldjp(nKey, mvldjpData))
					bError = TRUE;
			}
			break;
			case D_MOVE_CODE_INDIA:
			{
				if (!m_pDoc->m_pAttrCtrl->GetMvldid(nKey, mvldidData))
					bError = TRUE;
			}
			break;
			case D_MOVE_CODE_BS:
			{
				if (!m_pDoc->m_pAttrCtrl->GetMvldbs(nKey, mvldbsData))
					bError = TRUE;
			}
			case D_MOVE_CODE_TRANS:
			{
				if (!m_pDoc->m_pAttrCtrl->GetMvldtr(nKey, mvldtrData))
					bError = TRUE;
			}
			break;
			default: ASSERT(0);
				break;
			}
		}
		break;
		case D_LCOM_SETTLEMENT:
			if (!m_pDoc->m_pAttrCtrl->GetSmlc(nKey, smlcData))
				bError = TRUE;
		case D_LCOM_STAGE:
			if (!m_pDoc->m_pAttrCtrl->GetSgld(nKey, sgldData))
				bError = TRUE;
			break;
		case D_LCOM_ECCENSPEC:
			if (!m_pDoc->m_pAttrCtrl->GetEspc(nKey, espcData))
				bError = TRUE;
			break;
		case D_LCOM_GRIDMODEL:
			if (!m_pDoc->m_pAttrCtrl->GetGilc(nKey, gilcData))
				bError = TRUE;
			break;
		case D_LCOM_CB_GENERAL:
		case D_LCOM_CB_STEEL:
		case D_LCOM_CB_CONCRETE:
		case D_LCOM_CB_FOOTING:
		case D_LCOM_CB_SRC:
		case D_LCOM_CB_ALUMINUM:
		case D_LCOM_CB_STLCOMP:
			if (!m_pDoc->m_pAttrCtrl->GetLcom(m_nLcomType, nKey, lcomData))
				bError = TRUE;
			break;
		}

		if (bError) return FALSE;

		return TRUE;
	}
	else if (nCol == D_COL_RAC_FACTOR)
	{
		return ValidataFieldDbl(nRow, nCol);
	}

	return TRUE;
}

// CTBBrowserWnd::OnStartEditing 참고
// Rating에서 체크박스에 눌럿을때는 AddRow 안한다.
BOOL CAssessmentLoadCombSubGridSTL::OnStartEditing(ROWCOL nRow, ROWCOL nCol)
{
	// 이미 추가 모드이거나 편집 모드이면 무시
	if (m_nEditMode == addnew || m_nEditMode == edit)
		return CGXGridWnd::OnStartEditing(nRow, nCol);

	//   if(m_nGridType == 0 && nCol==2) 
	//     return CGXGridWnd::OnStartEditing(nRow, nCol);

	CGXControl* pControl = GetControl(nRow, nCol);
	if (!pControl) return CGXGridWnd::OnStartEditing(nRow, nCol);

	// ComboBox 종류는 무시해야 한다.
	// 문제는 이것이 콤보 박스인지 아닌지 어떻게 아느냐 하는 것이다.
	// style 정보에서 가져 오는 것 : 문제 있음.
	if (pControl->IsKindOf(RUNTIME_CLASS(CGXComboBoxWnd)))
		return CGXGridWnd::OnStartEditing(nRow, nCol);

	// edit control은 실제 고쳐질때 실행
	if (pControl->CGXControl::IsKindOf(CONTROL_CLASS(CGXEditControl)))
		return CGXGridWnd::OnStartEditing(nRow, nCol);

	// Button Type(radio , check box...)
	// 이 타입은 DECLARE_DYNAMIC이 선언 안 되어 있기 때문에 아래와 같이
	// DECLARE_CONTROL, CONTROL_CLASS를 이용한다. 
	BOOL bButtonType = FALSE;

	if (pControl->CGXControl::IsKindOf(CONTROL_CLASS(CGXCheckBox))
		|| pControl->CGXControl::IsKindOf(CONTROL_CLASS(CGXRadioButton))
		) bButtonType = TRUE;

	// check box는 무조건 실행(왜냐하면 클릭되면 값이 바뀌기 때문)
	if (!bButtonType)
		return CGXGridWnd::OnStartEditing(nRow, nCol);

	//   // Rating에서 체크박스에 눌럿을때는 AddRow 안한다.
	//   if(m_nGridType == 0 && nCol==2) return TRUE;

	// go into addnew or edit mode
	//if(m_nGridType == 0 && nCol==D_COL_RAC_FACTOR) 
	//{
	//	OnModifyCell(nRow, D_COL_RAC_FACTOR_OR_IMPACTOR);  // 체크박스 변경으로는 ModifyCell이 안되어 옆 셀 값을 주었음.
	//	return TRUE;
	//}

	//   else if (IsAppendRow(nRow)) AddNew(nRow);
	if (IsAppendRow(nRow)) AddNew(nRow);
	else Edit(nRow);
	return CGXGridWnd::OnStartEditing(nRow, nCol);
}

BOOL CAssessmentLoadCombSubGridSTL::ValidataFieldDbl(ROWCOL nRow, ROWCOL nCol)
{
	CString value;
	int nErrCode;
	value = GetValueRowCol(nRow, nCol);
	value.TrimLeft();
	value.TrimRight();
	if (!IsValidDoubleNumber(value, nErrCode))
	{
		switch (nErrCode)
		{
		case 1: SetWarningText(_LS(IDS_WG_CMD__ADDD__Invalid_Formula_)); break;
		case 2: SetWarningText(_LS(IDS_WG_CMD__ADDD__Input_Number_)); break;
		case 3: SetWarningText(_LS(IDS_WG_CMD__ADDD__Out_of_range_)); break;
		}
		return FALSE;
	}
	return TRUE;
}

void CAssessmentLoadCombSubGridSTL::SetData(CString sKey)
{
	m_sKey = sKey;

	SetLoadCaseColumnStyle(D_COL_RAC_LCASE);
	if (IsMainAppendRow() || IsMainNewRow())
	{
		if (GetRowCount() == 1) return; // already cleared
		//SetRecordCount(0);
		DeleteAllRecords();
		return;
	}

	T_ALCS_K key;
	T_ALCS_D data;

	// load data and fill each row
	key = _ttol(m_sKey);
	//m_pDoc->m_pAttrCtrl->GetLcom(m_nLcomType, key, data);
	m_pDoc->m_pAttrCtrl2->GetAlcs(key, data);

	BOOL bOldLock = LockUpdate(TRUE);

	//int nCount = m_pDoc->m_pAttrCtrl->GetCountLcomCase(m_nLcomType, data.LoadCombName);
	int nCount = data.arLoadComb.GetSize();
	if (GetRowCount() == 1 && nCount == 0) return;  // already cleared

	DeleteAllRecords();
	SetRecordCount(nCount);
	CString sValue;
	int nRangeStart;

	CMap<T_SGLD_K, T_SGLD_K, int, int> mapErect;
	m_pDoc->m_pAttrCtrl->GetErectionSelected(mapErect);

	for (int i = 0; i < nCount; i++)
	{
		if (data.arLoadComb[i].AnalType == D_LCOM_STAGE)
		{
			unsigned int SgldKey = data.arLoadComb[i].LoadCaseKey;
			// Erection Load 및 CS로 정의된 것만 보여주기(Tendon 제외).
			if (SgldKey >= D_SGLD_ER1_CVL && SgldKey < D_SGLD_SUM_CVL)
			{
				int nTemp = 0;
				if (!mapErect.Lookup(SgldKey, nTemp)) continue;
			}
		}

		nRangeStart = D_STLD_MAXNUM * data.arLoadComb[i].AnalType;
		sValue.Format(_T("%d"), nRangeStart + data.arLoadComb[i].LoadCaseKey);
		SetExpressionRowCol(i + 1, D_COL_RAC_LCASE, sValue);

		sValue.Format(_T("%g"), data.arLoadComb[i].Factor);
		SetExpressionRowCol(i + 1, D_COL_RAC_FACTOR, sValue);
	}

	LockUpdate(bOldLock);
	if (!bOldLock) Redraw();
}

void CAssessmentLoadCombSubGridSTL::GetData(T_ALCS_D &data)
{
	int nAnalType;
	T_STLD_K nKey;
	data.arLoadComb.SetSize(GetRowCount() - 1);
	data.arLoadComb.SetSize(GetRowCount() - 1);
	for (int i = 0; i < GetRowCount() - 1; i++)
	{
		nKey = _ttol(GetValueRowCol(i + 1, D_COL_RAC_LCASE));
		nAnalType = nKey / D_STLD_MAXNUM;
		data.arLoadComb[i].LoadCaseKey = nKey % D_STLD_MAXNUM;

		data.arLoadComb[i].Factor = _tstof(GetValueRowCol(i + 1, D_COL_RAC_FACTOR));
		data.arLoadComb[i].AnalType = nAnalType;
	}
}