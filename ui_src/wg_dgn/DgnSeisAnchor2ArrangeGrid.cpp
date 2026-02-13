// DgnSeisAnchor2ArrangeGrid.cpp : implementation file
//

#include "stdafx.h"
#include "wg_dgn.h"
#include "DgnSeisAnchor2ArrangeGrid.h"
#include "DgnSeisAnchorEvalItem2NewSubArrangeDlg.h"

#include "..\wg_base\wg_base_CompFunc.h"
#include "..\wg_db\wg_db_UnitCtrl.h"

#include "..\wg_base\wg_base_StrParser.h"
#include "..\wg_common\wg_common_TBCommon.h"
#include "..\wg_common\wg_common_TBGrid.h"
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_AttrCtrl2.h"
#include "..\wg_db\wg_db_UnitCtrl.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define D_OPT_NONE  _LS(IDS_CMD_TDNA_GRID_NONE)
#define D_OPT_LEFT  _LS(IDS_CMD_TDNA_GRID_LEFT)
#define D_OPT_RIGHT _LS(IDS_CMD_TDNA_GRID_RIGHT)

#define EPSILON 1.e-12
#define KEY_FMT  _T("%.15g")
#define DATA_FMT _T("%.15g")

#define D_COL_KEY		1
#define D_COL_LAYER		2
#define D_COL_D1		3
#define D_COL_SAX	    4
#define D_COL_SAY    	5
#define D_COL_Dp1   	6
#define D_COL_D2 		7
#define D_COL_Dp2 		8
//#define D_COL_THETA 	9
#define D_COL_THETA2 	9

/////////////////////////////////////////////////////////////////////////////
// CDgnSeisAnchor2ArrangeGrid
CDgnSeisAnchor2ArrangeGrid::CDgnSeisAnchor2ArrangeGrid(CWnd* pParent/* = NULL*/)
{
	m_pParent = (CDgnSeisAnchorEvalItem2NewSubArrangeDlg*)pParent;

	m_pData = 0;
	m_bDblClick = FALSE;
	//m_aCurDefVal[9];
	m_aCurDefVal[D_COL_KEY   - 1] = _T("");
	m_aCurDefVal[D_COL_LAYER - 1] = _T("");
	m_aCurDefVal[D_COL_D1    - 1] = _T("");
	m_aCurDefVal[D_COL_SAX   - 1] = _T("");
	m_aCurDefVal[D_COL_SAY   - 1] = _T("0.0");
	m_aCurDefVal[D_COL_Dp1   - 1] = _T("");
	m_aCurDefVal[D_COL_D2    - 1] = _T("");
	m_aCurDefVal[D_COL_Dp2   - 1] = _T("");
	//m_aCurDefVal[D_COL_THETA - 1] = _T("0.0");
	m_aCurDefVal[D_COL_THETA2- 1] = _T("0.0");

	AddColInfo(_ULS(Key), t_integer, 0);
	AddColInfo(_T("Layer"), t_string, 0);
	AddColInfo(_T("D1"), t_real, D_UNITSYS_BASE_LENGTH);
	AddColInfo(_T("Sax"), t_string, D_UNITSYS_BASE_LENGTH);
	AddColInfo(_T("Say"), t_real, D_UNITSYS_BASE_LENGTH);
	AddColInfo(_T("D'1"), t_real, D_UNITSYS_BASE_LENGTH);
	AddColInfo(_T("D2"), t_real, D_UNITSYS_BASE_LENGTH);
	AddColInfo(_T("D'2"), t_real, D_UNITSYS_BASE_LENGTH);
	//AddColInfo(_T("Θ'"), t_real, D_UNITSYS_BASE_DEGREE);
	AddColInfo(_T("Θ'"), t_real, D_UNITSYS_BASE_DEGREE); // Θ' 표현이지만 dTheta_q2 로 저장

	m_aPrimaryKey.Add(0);
}

CDgnSeisAnchor2ArrangeGrid::~CDgnSeisAnchor2ArrangeGrid()
{
}


BEGIN_MESSAGE_MAP(CDgnSeisAnchor2ArrangeGrid, CTBCommon)
	//{{AFX_MSG_MAP(CDgnSeisAnchor2ArrangeGrid)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONDBLCLK()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CDgnSeisAnchor2ArrangeGrid message handlers

void CDgnSeisAnchor2ArrangeGrid::Initialize(T_ANEV_D *pData)
{
	ASSERT(pData);
	m_pData = pData;
	m_AnveD = *pData;

	T_UNIT_SYSTEM us;
	CDBDoc *pDoc = CDBDoc::GetDocPoint();
	CTBCommon::Initialize(pDoc, us);

	// 	GetParam()->EnableUndo(FALSE);
	// 	GetParam()->EnableTrackRowHeight(FALSE);
	// 	GetParam()->EnableMoveCols(FALSE);        // columns can not be dragged
	// 
	// 	CGXProperties* pProp = GetParam()->GetProperties();
	// 	pProp->SetMarkColHeader(FALSE); // Turn off pressed button effect for column headers
	// 	pProp->SetMarkRowHeader(FALSE);
	// 	pProp->SetUserProperty(GX_IDS_OUTLINECURRENTCELL, // 현재 셀의 border 설정 변경
	// 		(CGXStyle)pProp->sInvertNoBorder);

	LockUpdate(TRUE);

	// 컬럼 갯수 설정
	int m_nColCount = GetInitialColCount();
	SetColCount(m_nColCount);
	SetHeaderTitle(us, TRUE);

	// 표준 스타일 변경
	ChangeStandardStyle(CGXStyle()
		.SetFont(CGXFont_GC().SetSize(9))
		.SetVerticalAlignment(DT_BOTTOM)
		.SetAllowEnter(FALSE));

	// 컬럼 스타일 변경
	SetStyleRange(CGXRange().SetCols(D_COL_KEY), CGXStyle()
		.SetHorizontalAlignment(DT_RIGHT)
		.SetValueType(GX_VT_NUMERIC)
		.SetFormat(GX_FMT_FIXED)
		.SetPlaces(0));

	SetStyleRange(CGXRange().SetCols(D_COL_LAYER), CGXStyle() // Serial No column의 스타일 변경
		.SetHorizontalAlignment(DT_RIGHT)
		.SetValueType(GX_VT_STRING)
		.SetEnabled(FALSE));

	SetStyleRange(CGXRange().SetCols(D_COL_D1, m_nColCount), CGXStyle()
		.SetVerticalAlignment(DT_VCENTER)
		.SetHorizontalAlignment(DT_RIGHT)
		.SetValueType(GX_VT_NUMERIC)
		.SetFormat(GX_FMT_FIXED)
		.SetPlaces(4));

	SetStyleRange(CGXRange().SetCols(D_COL_SAX), CGXStyle()
		.SetValueType(GX_VT_STRING)
		.SetVerticalAlignment(DT_VCENTER)
		.SetHorizontalAlignment(DT_CENTER)
		.SetValue(_T("")));

	ChangeColHeaderStyle(CGXStyle().SetFont(CGXFont_GC().SetBold(FALSE)));
	SetRowHeightByDPI(0, 0, 36);

	SetFrozenCols(0, 0);
	HideColsByIntend(D_COL_KEY, D_COL_KEY, TRUE);

	// always show vertical scrollbar
	SetScrollBarMode(SB_VERT, gxnEnabled | gxnEnhanced);
	//SetScrollBarMode(SB_HORZ, gxnEnabled | gxnEnhanced);

	// 현재 셀 설정
	SetRightValidCell(1, 0);
	GetParam()->SetHideCurrentCell(GX_HIDE_NEVER);

	// Sorting 정보 초기화
	m_aSortInfo.SetSize(1);
	m_aSortInfo[0].nRC = 0; // D_COL_SAY - 1;
	m_aSortInfo[0].sortType = CGXSortInfo::numeric;
	m_aSortInfo[0].sortOrder = CGXSortInfo::ascending;

	LockUpdate(FALSE);
}

BOOL CDgnSeisAnchor2ArrangeGrid::InsertRecordByKey(const double &key, const T_ANEV_LAYER_D &data)
{
	// 삽입 위치를 찾는다.
	ROWCOL nRow;

	// aKey를 만드는 코드 삽입
	CStringArray aKey;
	MakeSearchKey(key, data, aKey);
	FindRecordByKey(aKey, nRow);

	VERIFY(InsertRecord(nRow));
	return SetValue(nRow, key, data);
}

BOOL CDgnSeisAnchor2ArrangeGrid::DeleteRecordByKey(const double &key, const T_ANEV_LAYER_D &data)
{
	// 레코드를 찾는다.
	ROWCOL nRow;

	// aKey를 만드는 코드 삽입
	CStringArray aKey;
	MakeSearchKey(key, data, aKey);
	if (!FindRecordByKey(aKey, nRow)) return FALSE;

	// 삭제한다.
	return DeleteRecord(nRow);
}

/////////////////////////////////////////////////////////////////////////////
// CDgnSeisAnchor2ArrangeGrid Overridable Functions
void CDgnSeisAnchor2ArrangeGrid::OnChangedSelectionPublic(long *keys, int nSize)
{
	return;
}

long CDgnSeisAnchor2ArrangeGrid::GetSelectedKeys(CArray<long, long> &caKey)
{
	return 0;
}

BOOL CDgnSeisAnchor2ArrangeGrid::ValidateField(CString value, int nColID)
{
	int nErrCode;

	if (nColID == D_COL_KEY - 1 || nColID == D_COL_LAYER - 1)
	{/*skip*/
	}
	else if (nColID == D_COL_Dp1 - 1 || nColID == D_COL_D2 - 1 || nColID == D_COL_Dp2 - 1)
	{/*skip*/  //비활성화 되는 구간이므로
	}
	else if (nColID == D_COL_SAX - 1)
	{
		BOOL bAllZero = TRUE;
		CArray<double, double> aSax;
		CStrParser::GetUEDistance(value, aSax);

		for (int i = 0; i < aSax.GetSize(); i++)
		{
			if (aSax[i] > 0.0)
			{
				bAllZero = FALSE;
			}
		}

		if (bAllZero)
		{
			//CString msg;
			//msg.Format(_LS(IDS_DGN_SEIS_ANCHOR_LINK_BERING_NUM_CHK)); // _T("Link와 받침 갯수가 일치하지 않습니다"));
			//AfxMessageBox(msg);
			//return FALSE;
			return TRUE;

			//int nErrCode = 3;
			//return InvalidDoubleNumber(nErrCode);
		}
	}
	else
	{
		if (!IsValidDoubleNumber(value, nErrCode))
			return InvalidDoubleNumber(nErrCode);
	}

	return TRUE;
}

// 삽입이나 수정전에 Record의 Validation을 검사한다.
BOOL CDgnSeisAnchor2ArrangeGrid::ValidateRecord(CStringArray &value, CRowColArray &aCols)
{
	CString msg = _T("");
	int nPos;

	for (int i = 0; i < GetColCount(); i++)
	{
		if (i == D_COL_KEY - 1 || i == D_COL_LAYER - 1) { continue; /*skip*/ }
		nPos = aCols[i] - 1;
		if (value[nPos] == _T(""))
			msg += ((msg != _T("")) ? _T("\n") : _T("")) + m_aTitle[i] + _LS(IDS_WG_CMD__ADDD___has_not_been_entered_);
		else if (!ValidateField(value[nPos], i)) return FALSE;
	}
	if (msg == _T("")) return TRUE;
	else SetWarningText(msg);
	return FALSE;
}

// 이 함수는 Paste할 때 호출되므로 False를 리턴할 때는 ValidateField와 
// Validate Record에서 설정된 Warning Text를 없애주고 리턴한다.
BOOL CDgnSeisAnchor2ArrangeGrid::ValidateAndMakeRecord(CStringArray &value, CRowColArray& aCols, double &key, T_ANEV_LAYER_D& data)
{
	// 레코드에 대해 Validation을 검사한다.
	if (!ValidateRecord(value, aCols))
		return FALSE;

	// 레코드를 만들어서 리턴한다.
	int nPos;
	for (int i = 0; i < GetColCount(); i++)
	{
		nPos = aCols[i] - 1; // col id i에 해당하는 value가 저장된 위치
		if (i == D_COL_SAY - 1) key = _ttof(value[nPos]);
		//if (i == 0) key = _ttof(value[nPos]);
		ConvStrToData(i, value[nPos], data);
	}
	return TRUE;
}

BOOL CDgnSeisAnchor2ArrangeGrid::AddToDB(ROWCOL nRow)
{
	// Record를 만들어서 DB에 입력한다.
	double key;
	T_ANEV_LAYER_D data;
	T_ANEV_BRPR_KEYS_D Prop;
	T_ANEV_REG_SUPP_LAYER_D Supp;
	T_ANEV_FAIL_D Fail;

	GetValue(nRow, key, data, Prop, Supp, Fail);

	return AddData(key, data, Prop, Supp, Fail);
}

BOOL CDgnSeisAnchor2ArrangeGrid::AddToDB(CArray<void*, void*>& raKey, CArray<void*, void*>& raData)
{
	ASSERT(raKey.GetSize() == raData.GetSize());
	double tKey;
	CArray<double, double> aKey;
	CArray<T_ANEV_LAYER_D, T_ANEV_LAYER_D&> aData;
	CArray<T_ANEV_BRPR_KEYS_D, T_ANEV_BRPR_KEYS_D&> aProp;
	CArray<T_ANEV_REG_SUPP_LAYER_D, T_ANEV_REG_SUPP_LAYER_D &> aSupp;
	for (int i = 0; i < raKey.GetSize(); i++)
	{
		tKey = *((double*)raKey.GetAt(i));
		aKey.Add(tKey);
		aData.Add(*((T_ANEV_LAYER_D*)raData.GetAt(i)));
	}
	return AddData(aKey, aData);
}


BOOL CDgnSeisAnchor2ArrangeGrid::DeleteFromDB(ROWCOL nRow)
{
	double key;
	T_ANEV_LAYER_D data;
	T_ANEV_BRPR_KEYS_D Prop;
	T_ANEV_REG_SUPP_LAYER_D Supp;
	T_ANEV_FAIL_D Fail;

	GetValue(nRow, key, data, Prop, Supp, Fail);

	return DelData(key);
}

BOOL CDgnSeisAnchor2ArrangeGrid::DeleteFromDB(CRowColArray& awRows)
{
	if (awRows.GetSize() <= 0) return TRUE;

	ROWCOL nCol;
	//VERIFY(GetColumnNoFor(D_COL_SAY-1, nCol));
	VERIFY(GetColumnNoFor(0, nCol));

	CString value;
	CArray<double, double> awKeys;

	for (int i = 0; i < awRows.GetSize(); i++)
	{
		value = GetValueRowColBeforeEdit(awRows.GetAt(i), nCol);
		awKeys.Add(_ttof(value));
	}

	return DelData(awKeys);
}

BOOL CDgnSeisAnchor2ArrangeGrid::ModifyToDB(ROWCOL nRow)
{
	ROWCOL nCol;
	//VERIFY(GetColumnNoFor(D_COL_SAY-1, nCol));  // OldKey를 위한 Col
	VERIFY(GetColumnNoFor(0, nCol));

	double key, oldKey;
	T_ANEV_LAYER_D data;
	T_ANEV_BRPR_KEYS_D Prop;
	T_ANEV_REG_SUPP_LAYER_D Supp;
	T_ANEV_FAIL_D Fail;

	oldKey = _ttof(GetValueRowColBeforeEdit(nRow, nCol));
	GetValue(nRow, key, data, Prop, Supp, Fail);

	// 수정한다. 실패하면 에러 리턴
	return ModifyData(nRow, oldKey, key, data, Prop, Supp, Fail);
}

BOOL CDgnSeisAnchor2ArrangeGrid::ModifyToDB(CStringArray& raOldKey,
	CArray<void*, void*>& raKey,
	CArray<void*, void*>& raData)
{
	ASSERT(raKey.GetSize() == raData.GetSize());
	ASSERT(raKey.GetSize() == raOldKey.GetSize());
	CArray<double, double> aOldKey, aKey;
	CArray<T_ANEV_LAYER_D, T_ANEV_LAYER_D&> aData;
	double Key;
	int nSize = raKey.GetSize();
	for (int i = 0; i < nSize; i++)
	{
		Key = _ttof(raOldKey.GetAt(i));
		aOldKey.Add(Key);
		aKey.Add(*((double*)raKey.GetAt(i)));
		aData.Add(*((T_ANEV_LAYER_D*)raData.GetAt(i)));
	}
	return ModifyData(aOldKey, aKey, aData);
}


CString CDgnSeisAnchor2ArrangeGrid::GetNewKey()
{
	CString value(_T("0"));
	return value;
}

CString CDgnSeisAnchor2ArrangeGrid::GetDefValue(int nIndex)
{
	return m_aCurDefVal[nIndex+1];
}

CString* CDgnSeisAnchor2ArrangeGrid::GetDefValueArray()
{
	return m_aCurDefVal;
}

BOOL CDgnSeisAnchor2ArrangeGrid::MakeKeyAndDataArray(CStringArray& value, CRowColArray& awCols,
	CArray<void*, void*>& aKey, CArray<void*, void*>& aData)
{
	double* pKey;
	T_ANEV_LAYER_D* pData;
	pKey = new double;
	pData = new T_ANEV_LAYER_D;
	aKey.Add((void*)pKey);
	aData.Add((void*)pData);
	if (!ValidateAndMakeRecord(value, awCols, *pKey, *pData)) return FALSE;
	return TRUE;
}

void CDgnSeisAnchor2ArrangeGrid::DeleteKeyAndDataArray(CArray<void*, void*>& aKey, CArray<void*, void*>& aData)
{
	double* pKey;
	T_ANEV_LAYER_D* pData;
	ASSERT(aKey.GetSize() == aData.GetSize());
	for (int i = 0; i < aKey.GetSize(); i++)
	{
		pKey = (double*)aKey.GetAt(i);
		pData = (T_ANEV_LAYER_D*)aData.GetAt(i);
		delete pKey;
		delete pData;
	}
}

long CDgnSeisAnchor2ArrangeGrid::GetCountData()
{
	int nCount = m_pData->aLayer.GetSize();

	return nCount;
}

void CDgnSeisAnchor2ArrangeGrid::MakeItemFiltered(long nFilteredNum, long* aFilteredKeys, CRowColArray& awCols)
{
	ASSERT(0); // no filtering
}

void CDgnSeisAnchor2ArrangeGrid::MakeItemNormal(long nNumData, CRowColArray& awCols)
{
	double Key;
	long nCount;
	T_ANEV_LAYER_D Data;
	for (nCount = 0; nCount < nNumData; nCount++)
	{
		Key = m_pData->aLayer[nCount].dSay;
		Data = m_pData->aLayer[nCount];

		// Grid Data Setting
		SetValue(GetParam()->GetData(), nCount + 1, Key, Data, awCols);
	} // end of loop nCount


	//////////////////////////////////////////////////////////////////////////
	// 추가 변경
	ChangeCellDataForDlg(nNumData);
}

void CDgnSeisAnchor2ArrangeGrid::ChangeCellDataForDlg(int nNumData, BOOL bEndEdit)
{
	T_ANEV_LAYER_D data;
	T_ANEV_BRPR_KEYS_D Prop;
	T_ANEV_REG_SUPP_LAYER_D Supp;
	for (int nCount = 0; nCount < nNumData; nCount++)
	{
		BOOL bOK = FALSE;


		if (nCount == 0)
		{
			int iRow = nCount;
			for (int j = nCount; j < nNumData; j++)
			{
				Supp = m_pData->SuppType.aLayer[j];
				for (int k = 0; k < Supp.aBrevType.GetSize(); k++)
				{
					if (Supp.aBrevType[k] == 0) { iRow = j; bOK = TRUE; break; } //BrevType  0:고정단, 1:양방향 가동단, 2:교축 가동단, 3:교직 가동단
				}
				if (bOK) break;
			}
			double dD2  = m_pData->aLayer[iRow].dD2;
			double dDp2 = m_pData->aLayer[iRow].dD2_q;
// 			if(bEndEdit)
// 			{
// 				dD2  = m_pData->aLayer[iRow].dD2_q;
// 				dDp2 = m_pData->aLayer[iRow].dD2;
// 
// 				m_pData->aLayer[iRow].dD2   = dD2;
// 				m_pData->aLayer[iRow].dD2_q = dDp2;
// 			}
			SetValueRange(CGXRange(nCount + 1, D_COL_Dp2), dDp2);
			if (nNumData == 1)
				SetValueRange(CGXRange(nCount + 1, D_COL_D2), dD2);
			else
				SetValueRange(CGXRange(nCount + 1, D_COL_D2), _T("-"));
		}
		else if (nCount == nNumData - 1)
		{
			int iRow = nNumData - 1;
			for (int j = nNumData - 1; j >= 0; j--)
			{
				Supp = m_pData->SuppType.aLayer[j];
				for (int k = 0; k < Supp.aBrevType.GetSize(); k++)
				{
					if (Supp.aBrevType[k] == 0) { iRow = j; bOK = TRUE; break; } //BrevType  0:고정단, 1:양방향 가동단, 2:교축 가동단, 3:교직 가동단
				}
				if (bOK) break;
			}

			double dDp2 = m_pData->aLayer[iRow].dD2_q;
			double dD2  = m_pData->aLayer[iRow].dD2;
// 			if (bEndEdit)
// 			{
// 				dDp2 = m_pData->aLayer[iRow].dD2;
// 				dD2 = m_pData->aLayer[iRow].dD2_q;
// 
// 				m_pData->aLayer[iRow].dD2   = dD2;
// 				m_pData->aLayer[iRow].dD2_q = dDp2;
// 			}

			SetValueRange(CGXRange(nCount + 1, D_COL_D2), dD2);
			SetValueRange(CGXRange(nCount + 1, D_COL_Dp2), _T("-"));
		}
		else
		{
			SetValueRange(CGXRange(nCount + 1, D_COL_Dp2), _T("-"));
			SetValueRange(CGXRange(nCount + 1, D_COL_D2), _T("-"));
		}
	} // end of loop nCount
}

void CDgnSeisAnchor2ArrangeGrid::MakeItemEditingRecord(ROWCOL ncRow, CStringArray& EditingRecord)
{
	CDBDoc* pDoc = m_pDoc;
	double Key;
	T_ANEV_LAYER_D Data;
	for (int i = 0; i < GetColCount(); i++)
	{
		if (i == D_COL_SAY - 1) Key = _ttof(EditingRecord[i]);
		//if (i == 0) Key = _ttof(EditingRecord[i]);
		ConvStrToData(i, EditingRecord[i], Data);
	}
	// 현재 값을 설정한다.
	SetValue(ncRow, Key, Data);
}

/////////////////////////////////////////////////////////////////////////////
// CDgnSeisAnchor2ArrangeGrid Implementation
BOOL CDgnSeisAnchor2ArrangeGrid::GetValue(ROWCOL nRow, double &key, T_ANEV_LAYER_D &data, T_ANEV_BRPR_KEYS_D &Prop, T_ANEV_REG_SUPP_LAYER_D &Supp, T_ANEV_FAIL_D& Fail)
{
	ROWCOL nCol;
	CString str;
	for (int i = 0; i < GetColCount(); i++)
	{
		VERIFY(GetColumnNoFor(m_aTitle[i], nCol));
		str = GetValueRowCol(nRow, nCol);
		// 수식인 경우 값으로 변환한다.
		// Validation이 OK인 Field이므로 항상 TRUE 리턴
		VERIFY(ConvertFormulaToValue(str));

		if (i == D_COL_SAY - 1) key = _ttof(str);
		ConvStrToData(i, str, data);
	}

	// set data
	data.dD2_q = (m_pData->dH / 2) + data.dSay;
	data.dD2   = m_pData->dH - data.dD2_q;

	// Bearing Property
	CArray<T_BRPR_K, T_BRPR_K> aBrprK;
	m_pDoc->m_pAttrCtrl2->GetBrprKeyList(aBrprK);
	T_BRPR_K Brprk = (aBrprK.GetSize() > 0) ? aBrprK[0] : 0;


	Fail.Initialize();
	int nPos = nRow - 1;
	if (m_pData->BearingProp.aBrprKs.GetSize() >= nRow)
	{
		Prop = m_pData->BearingProp.aBrprKs.GetAt(nPos);
		Supp = m_pData->SuppType.aLayer.GetAt(nPos);

		T_ANEV_FAIL_SEL_LAYER_D FailSelLayerCL = m_pData->Fail.ConcFailL.aFailSelLayer.GetAt(nPos);
		T_ANEV_FAIL_SEL_LAYER_D FailSelLayerCT = m_pData->Fail.ConcFailT.aFailSelLayer.GetAt(nPos);
		T_ANEV_FAIL_SEL_LAYER_D FailSelLayerPL = m_pData->Fail.PryoutL.aFailSelLayer.GetAt(nPos);
		T_ANEV_FAIL_SEL_LAYER_D FailSelLayerPT = m_pData->Fail.PryoutT.aFailSelLayer.GetAt(nPos);

		Fail.ConcFailL.aFailSelLayer.Add(FailSelLayerCL);
		Fail.ConcFailT.aFailSelLayer.Add(FailSelLayerCL);
		Fail.PryoutL.aFailSelLayer.Add(FailSelLayerPL);
		Fail.PryoutT.aFailSelLayer.Add(FailSelLayerPT);

		int BearingNumber = data.aSax.GetSize() + 1;
		int nBrprNum      = Prop.aBrprK.GetSize();
		if(BearingNumber < nBrprNum) //
		{
// 			for (int j = BearingNumber; j < nBrprNum; j++)
// 			{
// 				Prop.aBrprK[j]    = 0;
// 				Supp.aBrevType[j] = -1;
// 				Fail.ConcFailL.aFailSelLayer[0].aFailSel[j] = FALSE;
// 				Fail.ConcFailT.aFailSelLayer[0].aFailSel[j] = FALSE;
// 				Fail.PryoutL.aFailSelLayer[0].aFailSel[j] = FALSE;
// 				Fail.PryoutT.aFailSelLayer[0].aFailSel[j] = FALSE;
// 			}
		}
		else
		{
			for (int j = 0; j < nBrprNum; j++)
			{
				if(Prop.aBrprK[j]    ==  0) Prop.aBrprK[j]    = Brprk;
				if(Supp.aBrevType[j] == -1) Supp.aBrevType[j] = 0;
			}
		}
	}
	else
	{

		T_ANEV_FAIL_SEL_LAYER_D FailSelLayer;

		int BearingNumber = 0;
		if(data.aSax.GetSize()==1 && data.aSax[0]==0.0)
		{
			BearingNumber = 1;
		}
		else
		{
			BearingNumber = data.aSax.GetSize() + 1;
		}
		for (int j = 0; j < BearingNumber; j++) Prop.aBrprK.Add(Brprk);
		for (int j = 0; j < BearingNumber; j++) Supp.aBrevType.Add(0);
		for (int j = 0; j < BearingNumber; j++) FailSelLayer.aFailSel.Add(TRUE);

		Fail.ConcFailL.aFailSelLayer.Add(FailSelLayer);
		Fail.ConcFailT.aFailSelLayer.Add(FailSelLayer);
		Fail.PryoutL.aFailSelLayer.Add(FailSelLayer);
		Fail.PryoutT.aFailSelLayer.Add(FailSelLayer);
	}
	return TRUE;
}

BOOL CDgnSeisAnchor2ArrangeGrid::SetValue(ROWCOL nRow, const double &key, const T_ANEV_LAYER_D &data)
{
	//
	CRowColArray awCols;
	awCols.SetSize(GetColCount());
	for (int i = 0; i < GetColCount(); i++)
		VERIFY(GetColumnNoFor(m_aTitle[i], awCols[i]));
	return SetValue(GetParam()->GetData(), nRow, key, data, awCols);
}

BOOL CDgnSeisAnchor2ArrangeGrid::SetValue(CGXData* pData, ROWCOL nRow, const double &key, const T_ANEV_LAYER_D& data, CRowColArray &awCols)
{
	//
	CString str;
	for (int i = 0; i < GetColCount(); i++)
	{
		//if (i == D_COL_SAY-1) str.Format(KEY_FMT, key);
		if (i == 0) str.Format(KEY_FMT, key);
		else ConvDataToStr(nRow, i, data, str);
		pData->StoreValueRowCol(nRow, awCols[i], str, gxOverride, GX_VT_NUMERIC);
	}

	T_ANEV_LAYER_D LayerTemp;
	ChangeGridColDataSet(nRow, key, data, LayerTemp);

	ChangeGridColEnable(nRow);

	return TRUE;
}

void CDgnSeisAnchor2ArrangeGrid::ConvStrToData(int i, CString& value, T_ANEV_LAYER_D &data)
{
	if (i == D_COL_LAYER - 1) {/*skip*/ }
	else if (i == D_COL_D1 - 1) data.dD1 = _ttof(value);
	else if (i == D_COL_SAX - 1)
	{
		CArray<double, double> aSax;
		CStrParser::GetUEDistance(value, aSax);
		data.aSax.Copy(aSax);
	}
	else if (i == D_COL_SAY - 1) data.dSay = _ttof(value);
	else if (i == D_COL_Dp1 - 1) data.dD1_q = _ttof(value); // 비활성
	else if (i == D_COL_D2 - 1) data.dD2 = _ttof(value); // 비활성
	else if (i == D_COL_Dp2 - 1) data.dD2_q = _ttof(value); // 비활성
	//else if (i == D_COL_THETA - 1)data.dTheta_q1 = _ttof(value);
	else if (i == D_COL_THETA2 - 1)data.dTheta_q2 = _ttof(value);
	//else ASSERT(0);
}

void CDgnSeisAnchor2ArrangeGrid::ConvDataToStr(ROWCOL nRow, int i, const T_ANEV_LAYER_D &data, CString &value)
{
	if (i == D_COL_LAYER - 1) value.Format(_T("%d"), (int)nRow); //_LS(IDS_DGN_SEIS_ANCHOR_SUPPPOT_LAYER)
	else if (i == D_COL_D1 - 1)   value.Format(DATA_FMT, data.dD1);
	else if (i == D_COL_SAX - 1)
	{
		CArray<double, double> aSax;
		aSax.Copy(data.aSax);
		value = CStrParser::ConvtDistArr2DistStr(aSax);
	}
	else if (i == D_COL_SAY - 1) value.Format(DATA_FMT, data.dSay);
	else if (i == D_COL_Dp1 - 1) value.Format(DATA_FMT, data.dD1_q); // 비활성
	else if (i == D_COL_D2 - 1) value.Format(DATA_FMT, data.dD2);   // 비활성
	else if (i == D_COL_Dp2 - 1) value.Format(DATA_FMT, data.dD2_q); // 비활성
	//else if (i == D_COL_THETA - 1)value.Format(DATA_FMT, data.dTheta_q1);
	else if (i == D_COL_THETA2 - 1)value.Format(DATA_FMT, data.dTheta_q2);
	//else ASSERT(0);
}

void CDgnSeisAnchor2ArrangeGrid::MakeSearchKey(const double &key, const T_ANEV_LAYER_D& data, CStringArray &aKey)
{
	int nColID;
	int nSortKeyNum = m_aSortInfo.GetSize();

	aKey.SetSize(nSortKeyNum);
	for (int i = 0; i < nSortKeyNum; i++)
	{
		nColID = m_aSortInfo[i].nRC;
		//if (nColID == D_COL_SAY-1) aKey[i].Format(KEY_FMT, key);
		if (nColID == 0) aKey[i].Format(KEY_FMT, key);
		else ConvDataToStr(1, nColID, data, aKey[i]);
	}
}

void CDgnSeisAnchor2ArrangeGrid::SetHeaderTitle(T_UNIT_SYSTEM &us, BOOL bInit)
{
	// Set Title
	CArray<int, int> aSize;
	CString csUnit, csTitle;
	int nColCount = GetColCount();
	aSize.SetSize(nColCount);
	for (int i = 0; i < nColCount; i++)
	{
		if (i == D_COL_KEY - 1)  aSize[i] = 0;
		else if (i == D_COL_LAYER-1)  aSize[i] = 40;
		else if (i == D_COL_SAX  -1)  aSize[i] =114;	
		else                          aSize[i] = 55;
		CUnitCtrl::GetUnitSystem(m_aUnit[i], csUnit);
		if (csUnit.IsEmpty()) csTitle = m_aTitle[i];
		else                  csTitle = m_aTitle[i] + _T("\n(") + csUnit + _T(")");

		SetStyleRange(CGXRange(0, i + 1), CGXStyle()
			.SetValue(csTitle)
			.SetUserAttribute(ID_USER_ATTR_COL_ID, m_aTitle[i]));
		if (bInit) SetColWidthByDPI(i + 1, i + 1, aSize[i]);
	}
	if (bInit) SetColWidthByDPI(0, 0, 20);
}

BOOL CDgnSeisAnchor2ArrangeGrid::CheckData(T_ANEV_LAYER_D Layer, BOOL bMsg/* = TRUE*/)
{
	double dSaxSum = 0.0;
	for (int k = 0; k < Layer.aSax.GetSize(); k++)
	{
		dSaxSum += Layer.aSax[k];
	}
	if (Layer.aSax.GetSize()>1 && dSaxSum == 0.0) return FALSE;

	CString msg;
	if(Layer.dD1 < 0.0 || Layer.dD1_q < 0.0)
	{
		if(bMsg)
		{
			msg.Format(_LS(IDS_DB_DT_ANEV_ERROR_COPING_SHAPE_DATA_CHECK));
			//msg.Format(_T("Layer Input값의 합산 길이가 B를 초과합니다."));
			AfxMessageBox(msg);
		}
		return FALSE;
	}
	if (Layer.dD2 < 0.0 || Layer.dD2_q < 0.0)
	{
		if (bMsg)
		{
			msg.Format(_T("Layer Input값의 합산 길이가 H를 초과합니다."));
			AfxMessageBox(msg);
		}
		return FALSE;
	}

	double dD1_q = m_pData->dB - (Layer.dD1 + dSaxSum);
	if (dD1_q < 0.0)
	{
		if (bMsg)
		{
			msg.Format(_LS(IDS_DB_DT_ANEV_ERROR_COPING_SHAPE_DATA_CHECK));
			//msg.Format(_T("Layer Input값의 합산 길이가 B를 초과합니다."));
			AfxMessageBox(msg);
		}
		return FALSE;
	}

	double dY = m_pData->dH/2.0 - fabs(Layer.dSay);
	if (dY < 0.0 )
	{
		if(bMsg)
		{
			msg.Format(_T("Say값이 영역을 벗어났습니다."));
			AfxMessageBox(msg);
		}
		return FALSE;
	}

	return TRUE;
}

BOOL CDgnSeisAnchor2ArrangeGrid::FindData(double key, int& nPos, int nStart)
{
	int i;
	int nSize = m_pData->aLayer.GetSize();

	CString csVal;
	double dblVal;
	int nCompResult;
	for (i = nStart; i < nSize; i++)
	{
		csVal.Format(KEY_FMT, m_pData->aLayer[i].dSay);
		dblVal = _ttof(csVal);
		nCompResult = CCompFunc::FastCompDouble(key, dblVal);
		if (nCompResult > 0) continue;    // key > value
		else if (nCompResult < 0) break;  // key < value
		else                              // key == value
		{
			nPos = i;
			return TRUE;;
		}
	}
	nPos = i;
	return FALSE;
}

BOOL CDgnSeisAnchor2ArrangeGrid::ExistAllKey(CArray<double, double>& aKey, double& errorKey)
{
	CArray<double, double> aTempKey;

	int nSize = aKey.GetSize();
	aTempKey.Copy(aKey);
	BOOL bSame;
	int nPos = 0, nStart;
	for (int i = 0; i < nSize; i++)
	{
		nStart = nPos;
		bSame = FindData(aTempKey[i], nPos, nStart);
		if (!bSame)
		{
			errorKey = aTempKey[i];
			return FALSE;
		}
		nPos = nStart;
	}
	return TRUE;
}
void CDgnSeisAnchor2ArrangeGrid::SetViewData()
{
	m_pParent->SetViewData();
}

// aLayer와 연관된 다른 데이터도 변경
void CDgnSeisAnchor2ArrangeGrid::RemoveAt(int nPos)
{
	m_pData->aLayer.RemoveAt(nPos);

	m_pData->BearingProp.aBrprKs.RemoveAt(nPos);
	m_pData->SuppType.aLayer.RemoveAt(nPos);

	//////////////////////////////////////////////////////////////////////////
	// 추가
	m_pData->Fail.ConcFailL.aFailSelLayer.RemoveAt(nPos);
	m_pData->Fail.ConcFailT.aFailSelLayer.RemoveAt(nPos);
	m_pData->Fail.PryoutL.aFailSelLayer.RemoveAt(nPos);
	m_pData->Fail.PryoutT.aFailSelLayer.RemoveAt(nPos);
}

void CDgnSeisAnchor2ArrangeGrid::InsertAt(int nPos, T_ANEV_LAYER_D item, T_ANEV_BRPR_KEYS_D Prop, T_ANEV_REG_SUPP_LAYER_D Supp, T_ANEV_FAIL_D Fail)
{
	m_pData->aLayer.InsertAt(nPos, item);

	m_pData->BearingProp.aBrprKs.InsertAt(nPos, Prop);
	m_pData->SuppType.aLayer.InsertAt(nPos, Supp);

	//////////////////////////////////////////////////////////////////////////
	// 추가
	m_pData->Fail.ConcFailL.aFailSelLayer.InsertAt(nPos, Fail.ConcFailL.aFailSelLayer[0]);
	m_pData->Fail.ConcFailT.aFailSelLayer.InsertAt(nPos, Fail.ConcFailT.aFailSelLayer[0]);
	m_pData->Fail.PryoutL.aFailSelLayer.InsertAt(nPos, Fail.PryoutL.aFailSelLayer[0]);
	m_pData->Fail.PryoutT.aFailSelLayer.InsertAt(nPos, Fail.PryoutT.aFailSelLayer[0]);
}

BOOL CDgnSeisAnchor2ArrangeGrid::AddData(double key, T_ANEV_LAYER_D data, T_ANEV_BRPR_KEYS_D Prop, T_ANEV_REG_SUPP_LAYER_D Supp, T_ANEV_FAIL_D Fail)
{
	CArray<double, double> aOldKey, aKey;
	CArray<T_ANEV_LAYER_D, T_ANEV_LAYER_D&> aData;
	aKey.Add(key);
	aData.Add(data);

	// 동일한 key값 중복 검토
	int nPos;
	if (FindData(key, nPos, 0))
	{
		CString msg;
		msg.Format(_LS(IDS_CMD0417__Say_already_exists), key);
		AfxMessageBox(msg);
		return FALSE;
	}

	T_ANEV_LAYER_D item;
	item = data;
	InsertAt(nPos, item, Prop, Supp, Fail);//m_pData->aLayer.InsertAt(nPos, item);

	UpdateBuffer(0, aOldKey, aKey, aData);
	DBChanged();
	return TRUE;
}

BOOL CDgnSeisAnchor2ArrangeGrid::AddData(CArray<double, double> &aKey, CArray<T_ANEV_LAYER_D, T_ANEV_LAYER_D&> &aData, BOOL bNotify)
{
	CArray<double, double> aOldKey;

	// 동일한 key값 중복 검토
	int nSize = aKey.GetSize();
	aOldKey.Copy(aKey);
	qsort((void*)aOldKey.GetData(), nSize, sizeof(double), CCompFunc::doubleAsc);

	T_ANEV_BRPR_KEYS_D Prop;
	T_ANEV_REG_SUPP_LAYER_D Supp;
	T_ANEV_FAIL_D Fail;
	T_ANEV_FAIL_SEL_LAYER_D FailSelLayer;

	int nNumX, nNumY;
	CDgnSeisAnchorShapeMgr ShapeMgr;
	ShapeMgr.GetBearingNumberNew2(m_AnveD, nNumX, nNumY);
	for (int i = 0; i < nNumX; i++) Supp.aBrevType.Add(0);
	for (int i = 0; i < nNumX; i++) FailSelLayer.aFailSel.Add(TRUE);

	Fail.ConcFailL.aFailSelLayer.Add(FailSelLayer);
	Fail.ConcFailT.aFailSelLayer.Add(FailSelLayer);
	Fail.PryoutL.aFailSelLayer.Add(FailSelLayer);
	Fail.PryoutT.aFailSelLayer.Add(FailSelLayer);

	BOOL bSame;
	int nPos = 0, nStart;
	for (int i = 0; i < nSize; i++)
	{
		nStart = nPos;
		bSame = FindData(aOldKey[i], nPos, nStart);
		if (!bSame && i > 0)
			bSame = (aOldKey[i] - aOldKey[i - 1] <= EPSILON);
		if (bSame)
		{
			CString msg;
			msg.Format(_LS(IDS_CMD0417__Say_already_exists), aOldKey[i]);
			AfxMessageBox(msg);
			return FALSE;
		}
		nPos = nStart;
	}
	for (int i = 0; i < nSize; i++)
	{
		bSame = FindData(aKey[i], nPos, 0);
		VERIFY(!bSame);

		T_ANEV_LAYER_D item;
		item = aData[i];

		InsertAt(nPos, item, Prop, Supp, Fail);//m_pData->aLayer.InsertAt(nPos, item);
	}
	UpdateBuffer(0, aOldKey, aKey, aData);

	if (bNotify) DBChanged();
	return TRUE;
}

BOOL CDgnSeisAnchor2ArrangeGrid::DelData(double key)
{
	CArray<double, double> aOldKey, aKey;
	CArray<T_ANEV_LAYER_D, T_ANEV_LAYER_D&> aData;
	T_ANEV_LAYER_D data; data.Initialize();
	aKey.Add(key);
	aData.Add(data);

	int nPos;
	if (!FindData(key, nPos))
	{
		CString msg;
		msg.Format(_LS(IDS_CMD0417__Say_does_not_exist), key);
		AfxMessageBox(msg);
		return FALSE;
	}
	RemoveAt(nPos); //m_pData->aLayer.RemoveAt(nPos);

	UpdateBuffer(1, aOldKey, aKey, aData);
	DBChanged();
	return TRUE;
}

BOOL CDgnSeisAnchor2ArrangeGrid::DelData(CArray<double, double> &aKey, BOOL bNotify)
{
	CArray<double, double> aOldKey;
	CArray<T_ANEV_LAYER_D, T_ANEV_LAYER_D&> aData;
	T_ANEV_LAYER_D data;
	int nSize = aKey.GetSize();
	for (int i = 0; i < nSize; i++) aData.Add(data);

	aOldKey.Copy(aKey);
	qsort((void*)aOldKey.GetData(), nSize, sizeof(double), CCompFunc::doubleAsc);
	// 동일한 key값 중복 검토
	double errKey;
	if (!ExistAllKey(aOldKey, errKey))
	{
		CString msg;
		msg.Format(_LS(IDS_CMD0417__Say_does_not_exist), errKey);
		AfxMessageBox(msg);
		return FALSE;
	}

	int nSize2 = m_pData->aLayer.GetSize();
	int j = nSize2 - 1;
	double dblDiff;
	int nDeleted = 0;
	for (int i = nSize - 1; i >= 0; i--)
	{
		for (; j >= 0; j--)
		{
			dblDiff = m_pData->aLayer[j].dSay;
			if (CCompFunc::CompReal(aOldKey[i], dblDiff) == 0)
			{
				RemoveAt(j);//m_pData->aLayer.RemoveAt(j);
				j--;
				nDeleted++;
				break;
			}
		}
	}

	UpdateBuffer(1, aOldKey, aKey, aData);

	if (bNotify) DBChanged();
	return TRUE;
}

BOOL CDgnSeisAnchor2ArrangeGrid::ModifyData(ROWCOL nRow, double oldKey, double key, T_ANEV_LAYER_D data, T_ANEV_BRPR_KEYS_D Prop, T_ANEV_REG_SUPP_LAYER_D Supp, T_ANEV_FAIL_D Fail)
{
	if (!CheckData(data)) return FALSE;

	CArray<double, double> aOldKey, aKey;
	CArray<T_ANEV_LAYER_D, T_ANEV_LAYER_D&> aData;
	aOldKey.Add(oldKey);
	aKey.Add(key);
	aData.Add(data);

	int nPos;
	if ((m_nEditMode == CTBBrowserWnd::noMode))  
	{ // 국내내진에서만 특이한 경우임. 셀을 더블클릭 후 수정한 경우
		nPos = (int)nRow - 1;
	}
	else if (!FindData(oldKey, nPos))
	{
		CString msg;
		msg.Format(_LS(IDS_CMD0417__Old_Say_does_not_exist), oldKey);
		AfxMessageBox(msg);
		return FALSE;
	}
	if (oldKey == key)
	{
		m_pData->aLayer[nPos] = data;
	}
	else
	{
		int nPos2;
		if (FindData(key, nPos2))
		{
			CString msg;
			msg.Format(_LS(IDS_CMD0417__New_Say_already_exists), key);
			AfxMessageBox(msg);
			return FALSE;
		}

		T_ANEV_LAYER_D item;
		item = data;
		RemoveAt(nPos); // m_pData->aLayer.RemoveAt(nPos);
		if (nPos2 > nPos) nPos2--;
		InsertAt(nPos2, item, Prop, Supp, Fail); // m_pData->aLayer.InsertAt(nPos2, item);
	}

	UpdateBuffer(2, aOldKey, aKey, aData);

	DBChanged();
	return TRUE;
}

BOOL CDgnSeisAnchor2ArrangeGrid::ModifyData(CArray<double, double> &aOldKey, CArray<double, double> &aKey, CArray<T_ANEV_LAYER_D, T_ANEV_LAYER_D&> &aData)
{
	CArray<T_ANEV_LAYER_D, T_ANEV_LAYER_D> aOrgData;
	aOrgData.Copy(m_pData->aLayer);

	if (!DelData(aOldKey, FALSE))
		return FALSE;
	if (!AddData(aKey, aData, FALSE))
	{
		m_pData->aLayer.Copy(aOrgData);
		return FALSE;
	}

	DBChanged();
	return TRUE;
}

void CDgnSeisAnchor2ArrangeGrid::UpdateBuffer(int nCmd, CArray<double, double> &aOldKey, CArray<double, double> &aKey, CArray<T_ANEV_LAYER_D, T_ANEV_LAYER_D&> &aData)
{
	// 현재 Row, Col을 저장 : 나중에 복구하기 위함
	ROWCOL ncRow, ncCol;
	GetCurrentCell(ncRow, ncCol);

	BOOL bOldLock = LockUpdate(TRUE);

	int nSize = aKey.GetSize();
	for (int i = 0; i < nSize; i++)
	{
		switch (nCmd)
		{
		case(0):
			// 삽입될 위치(nRow)를 찾아 넣는다.
			InsertRecordByKey(aKey[i], aData[i]);
			break;
		case(1):
			// 삭제될 위치(nRow)를 찾아서 지운다.
			DeleteRecordByKey(aKey[i], aData[i]);
			break;
		case(2):
			DeleteRecordByKey(aOldKey[i], aData[i]);
			InsertRecordByKey(aKey[i], aData[i]);
			break;
		default:
			break;
		}
	} // end of while

	// 원래 row, col을 복구하려고 시도
	unsigned int nOldMode = m_nEditMode;
	if (nOldMode != CTBBrowserWnd::addnew)
	{ // addnew 상태에서 복구하면 추가되는 Row가 삭제(CancelEdit에서)된다.
		m_nEditMode = CTBBrowserWnd::noMode;
		if (GetRowCount() < ncRow) ncRow = GetRowCount();
		BOOL bSet = SetCurrentCell(ncRow, ncCol);
		if (!bSet) bSet = SetRightValidCell(ncRow, ncCol);
		if (!bSet) bSet = SetLeftValidCell(ncRow, ncCol);
		m_nEditMode = nOldMode;
	}

	MakeItemEx();

	LockUpdate(bOldLock);
	if (!bOldLock) Redraw();
}

void CDgnSeisAnchor2ArrangeGrid::OnUnitChange()
{
	ASSERT(0);
	// 	T_UNIT_SYSTEM us;
	// 	SetHeaderTitle(us, FALSE);
	// 	MakeItemEx(0, -2, TRUE);
}

void CDgnSeisAnchor2ArrangeGrid::DBChanged()
{
	SetViewData();
}


void CDgnSeisAnchor2ArrangeGrid::ChangeGridColEnable(ROWCOL nRow)
{
	BOOL bEnable = FALSE;
	CString strValue;

	SetStyleRange(CGXRange(nRow, D_COL_Dp1), CGXStyle().SetEnabled(bEnable));
	SetStyleRange(CGXRange(nRow, D_COL_D2), CGXStyle().SetEnabled(bEnable));
	SetStyleRange(CGXRange(nRow, D_COL_Dp2), CGXStyle().SetEnabled(bEnable));
}

void CDgnSeisAnchor2ArrangeGrid::ChangeGridColData(ROWCOL nRow)
{
	double Key;
	T_ANEV_LAYER_D Layer;
	T_ANEV_LAYER_D LayerNew;
	T_ANEV_BRPR_KEYS_D Prop;
	T_ANEV_REG_SUPP_LAYER_D Supp;
	T_ANEV_FAIL_D Fail;

	GetValue(nRow, Key, Layer, Prop, Supp, Fail);
	ChangeGridColDataSet(nRow, Key, Layer, LayerNew);
}

void CDgnSeisAnchor2ArrangeGrid::ChangeGridColDataSet(ROWCOL nRow, double Key, T_ANEV_LAYER_D Layer, T_ANEV_LAYER_D& LayerNew)
{
	double dSaxSum = 0.0;
	for (int k = 0; k < Layer.aSax.GetSize(); k++)
	{
		dSaxSum += Layer.aSax[k];
	}

	Layer.dD1_q = m_pData->dB - Layer.dD1 - dSaxSum;

	if (Layer.dSay < 0.0)
	{
		Layer.dD2 = (m_pData->dH / 2) - fabs(Layer.dSay);
		Layer.dD2_q = m_pData->dH - Layer.dD2;
	}
	else
	{
		Layer.dD2 = (m_pData->dH / 2) + Layer.dSay;
		Layer.dD2_q = m_pData->dH - Layer.dD2;
	}

	LayerNew = Layer;

	// 현재 값을 설정한다.
	CString strRow; strRow.Format(_T("%d"), (int)nRow);
	SetValueRange(CGXRange(nRow, D_COL_LAYER), strRow);
	SetValueRange(CGXRange(nRow, D_COL_KEY), Key);
	SetValueRange(CGXRange(nRow, D_COL_Dp1), Layer.dD1_q);
	SetValueRange(CGXRange(nRow, D_COL_D2), Layer.dD2);
	SetValueRange(CGXRange(nRow, D_COL_Dp2), Layer.dD2_q);
}

void CDgnSeisAnchor2ArrangeGrid::OnModifyCell(ROWCOL nRow, ROWCOL nCol)
{
	if (m_bDblClick) return;
	CTBCommon::OnModifyCell(nRow, nCol);

	if (nCol == D_COL_D1 || nCol == D_COL_SAX || nCol == D_COL_SAY)
	{
		ChangeGridColData(nRow);
	}

	ChangeGridColEnable(nRow);
}

// OnModifyCell에서는 그리드를 더블클릭할때 문제가 있어서(두번 호출되는것이 문제가 되는 듯..)
// 더블클릭은 OnEndEditing 으로 적용함.
BOOL CDgnSeisAnchor2ArrangeGrid::OnEndEditing(ROWCOL nRow, ROWCOL nCol)
{
	// 부모의 OnEndEditing을 호출한다.
	BOOL bRet = CGXGridWnd::OnEndEditing(nRow, nCol);
	if (!m_bDblClick) return bRet;

	CString str;
	str = GetValueRowCol(nRow, nCol);

	if(bRet && !str.IsEmpty())
	{
		if (nCol == D_COL_D1 || nCol == D_COL_SAX || nCol == D_COL_SAY)
		{
			ChangeGridColData(nRow);
		}

		ChangeGridColEnable(nRow);
	}

	double Key;
	T_ANEV_LAYER_D Layer;
	T_ANEV_LAYER_D LayerNew;
	T_ANEV_BRPR_KEYS_D Prop;
	T_ANEV_REG_SUPP_LAYER_D Supp;
	T_ANEV_FAIL_D Fail;
	GetValue(nRow, Key, Layer, Prop, Supp, Fail);

	if (CheckData(Layer, FALSE))
	{
		m_bDblClick = FALSE;

		if(m_pData->aLayer.GetSize() < nRow)
			bRet = AddToDB(nRow);
		else
			bRet = ModifyToDB(nRow);
	}

	return bRet;
}

BOOL CDgnSeisAnchor2ArrangeGrid::OnInsertEmptyRecord(ROWCOL nRow)
{
	BOOL bReturn = FALSE;
	bReturn = CTBCommon::OnInsertEmptyRecord(nRow);

	if (bReturn)
		ChangeGridColEnable(nRow);
	return bReturn;
}

BOOL CDgnSeisAnchor2ArrangeGrid::OnDeleteRecord(ROWCOL nRow)
{
	return CTBCommon::OnDeleteRecord(nRow);
}

void CDgnSeisAnchor2ArrangeGrid::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	//m_bDblClick = FALSE;
	CTBCommon::OnLButtonDown(nFlags, point);
}


void CDgnSeisAnchor2ArrangeGrid::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	m_bDblClick = TRUE;
	CTBCommon::OnLButtonDblClk(nFlags, point);
}
