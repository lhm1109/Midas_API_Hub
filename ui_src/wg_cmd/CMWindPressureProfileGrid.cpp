// ThfcItemGrid.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMWindPressureProfileGrid.h"
#include "CMWindPressureProfileDlg.h"
#include "CMWindPressureProfileKBC2016Dlg.h"

#include "..\wg_base\wg_base_CompFunc.h"
#include "..\wg_db\wg_db_UnitCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define EPSILON  1.e-12
#define DATA_FMT _T("%.15g")

enum EN_WINDP_PROFILE_GRIDCOL
{
	EN_WINDP_PROFILE_GRIDCOL_01_HEIGHT,
	EN_WINDP_PROFILE_GRIDCOL_02_WIDTH,
	EN_WINDP_PROFILE_GRIDCOL_03_THICK,
	EN_WINDP_PROFILE_GRIDCOL_04_PRESSURE,
	EN_WINDP_PROFILE_GRIDCOL_05_ADDTIONAL,
};

/////////////////////////////////////////////////////////////////////////////
// CCMWindPressureProfileGrid

CCMWindPressureProfileGrid::CCMWindPressureProfileGrid()
{
	m_pData = NULL;
	m_pParent = NULL;
	m_pParentKBC2016 = NULL;
	AddColInfo(_LS(IDS_WG_CMD__ADDD__Height),                  t_real, D_UNITSYS_BASE_LENGTH);
	AddColInfo(_LS(IDS_CMD_WINDP_PROFILE_WIND_WIDTH),          t_real, D_UNITSYS_BASE_LENGTH);
	AddColInfo(_LS(IDS_CMD_WINDP_PROFILE_WIND_THICK),          t_real, D_UNITSYS_BASE_LENGTH);
	AddColInfo(_LS(IDS_CMD_WINDP_PROFILE_WIND_PRESSURE),       t_real, D_UNITSYS_BASE_PRESSURE);
	AddColInfo(_LS(IDS_CMD_WINDP_PROFILE_ADDITIONAL_PRESSURE), t_real, D_UNITSYS_BASE_PRESSURE);

	m_aCurDefVal[0] = _T("0.0");
	m_aCurDefVal[1] = _T("0.0");
	m_aCurDefVal[2] = _T("0.0");
	m_aCurDefVal[3] = _T("0.0");
	m_aCurDefVal[4] = _T("0.0");

	m_bShowThick = TRUE; //보이는걸 Default.

	m_aPrimaryKey.Add(0);
}

CCMWindPressureProfileGrid::~CCMWindPressureProfileGrid()
{
}


BEGIN_MESSAGE_MAP(CCMWindPressureProfileGrid, CTBCommon)
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CCMWindPressureProfileGrid message handlers

void CCMWindPressureProfileGrid::Initialize(T_WDPR_D* pData, CCMWindPressureProfileDlg* pParent)
{
	m_pParent = pParent;
	Initialize_sub(pData);
}
void CCMWindPressureProfileGrid::Initialize(T_WDPR_D *pData, CCMWindPressureProfileKBC2016Dlg* pParent)
{
	m_pParentKBC2016 = pParent;
	Initialize_sub(pData);
}

void CCMWindPressureProfileGrid::Initialize_sub(T_WDPR_D * pData)
{
	m_nDir = 0;

	ASSERT(pData);
	m_pData = pData;

	T_UNIT_SYSTEM us;
	CDBDoc *pDoc = CDBDoc::GetDocPoint();
	CTBCommon::Initialize(pDoc, us);

	GetParam()->EnableUndo(FALSE);
	//EnableScrollTips();
	//EnableHints();
	GetParam()->EnableTrackRowHeight(FALSE);
	GetParam()->EnableMoveCols(FALSE);        // columns can not be dragged

	CGXProperties* pProp = GetParam()->GetProperties();
	pProp->SetMarkColHeader(FALSE); // Turn off pressed button effect for column headers
	pProp->SetMarkRowHeader(FALSE);
	pProp->SetUserProperty(GX_IDS_OUTLINECURRENTCELL, // 현재 셀의 border 설정 변경
		(CGXStyle)pProp->sInvertNoBorder);

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
	ChangeColHeaderStyle(CGXStyle().SetFont(CGXFont_GC().SetBold(FALSE)));
	ChangeRowHeaderStyle(CGXStyle()
		.SetFont(CGXFont_GC().SetBold(FALSE))
		.SetHorizontalAlignment(DT_RIGHT)
		.SetControl(GX_IDS_CTRL_HEADER));
	GetParam()->SetNumberedRowHeaders(TRUE);

	// 컬럼 스타일 변경
	SetStyleRange(CGXRange().SetCols(1, m_nColCount), CGXStyle()
		.SetHorizontalAlignment(DT_RIGHT)
		.SetValueType(GX_VT_NUMERIC)
		.SetFormat(GX_FMT_FIXED)
		.SetPlaces(4));

	//Pressure는 수정불가로...
	SetStyleRange(CGXRange().SetCols(EN_WINDP_PROFILE_GRIDCOL_04_PRESSURE + 1, EN_WINDP_PROFILE_GRIDCOL_04_PRESSURE + 1), CGXStyle()
		.SetControl(GX_IDS_CTRL_STATIC)
		.SetEnabled(FALSE)
		.SetPlaces(4));

	SetRowHeightByDPI(0, 0, 52);

	//   SetStyleRange(CGXRange().SetCols(2), CGXStyle()
	//       .SetPlaces(4));

	// 현재 셀 설정
	SetRightValidCell(1, 0);
	GetParam()->SetHideCurrentCell(GX_HIDE_NEVER);

	if (!m_bShowThick)
	{
		HideColsByIntend(EN_WINDP_PROFILE_GRIDCOL_03_THICK+1, EN_WINDP_PROFILE_GRIDCOL_03_THICK+1, TRUE);
	}

	// Sorting 정보 초기화
	m_aSortInfo.SetSize(1);
	m_aSortInfo[0].nRC = 0;
	m_aSortInfo[0].sortType = CGXSortInfo::numeric;
	m_aSortInfo[0].sortOrder = CGXSortInfo::ascending;

	// Table Name 및 그리드 간격 등 설정
	//m_csTableName = D_TB_TABLE_NODE;
	//ApplyPreference();

	LockUpdate(FALSE);
}

BOOL CCMWindPressureProfileGrid::InsertRecordByKey(const double &key, const T_WIND_PRESSURE_PROFILE& data)
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

BOOL CCMWindPressureProfileGrid::DeleteRecordByKey(const double &key, const T_WIND_PRESSURE_PROFILE& data)
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
// CCMWindPressureProfileGrid Overridable Functions
void CCMWindPressureProfileGrid::OnChangedSelectionPublic(long *keys, int nSize)
{
	return;
}

long CCMWindPressureProfileGrid::GetSelectedKeys(CArray<long, long> &caKey)
{
	return 0;
}

BOOL CCMWindPressureProfileGrid::ValidateField(CString value, int nColID)
{
	if (nColID >= GetColCount())
	{
		SetWarningText(_LS(IDS_WG_CMD__ADDD___CCMStorGrid__ValidateField__Erro));
		return FALSE;
	}

	int nErrCode;
	if (TRUE) //모든 경우가 숫자다.
	{
		if (!IsValidDoubleNumber(value, nErrCode))
		{
			switch(nErrCode)
			{
			case 1: SetWarningText(_LS(IDS_WG_CMD__ADDD__Invalid_formula_)); break;
			case 2: SetWarningText(_LS(IDS_WG_CMD__ADDD__Charater_exist_in_double_value)); break;
			case 3: SetWarningText(_LS(IDS_WG_CMD__ADDD__Value_is_too_big_)); break;
			}
			return FALSE;
		}
		//level은 음수가 될수 있다.
//     if (nColID == 0 && _tstof(value) < 0.0)
//     {
//       SetWarningText(_LS(IDS_WG_CMD__ADDD__Negative_value_is_not_allowed_));
//       return FALSE;
//     }
		return TRUE;
	}
	else return FALSE;

	return TRUE;
}

// 삽입이나 수정전에 Record의 Validation을 검사한다.
BOOL CCMWindPressureProfileGrid::ValidateRecord(CStringArray &value, CRowColArray &aCols)
{
	CString msg = _T("");
	int nPos;
	int nColCnt = GetColCount();

	for (int i = 0; i < nColCnt; i++)
	{
		nPos = aCols[i]-1;
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
BOOL CCMWindPressureProfileGrid::ValidateAndMakeRecord(CStringArray &value, CRowColArray& aCols, double &key, T_WIND_PRESSURE_PROFILE& data)
{
	/*
	// 각 필드에 대해 Validation을 검사한다.
	for (int i = 0; i < value.GetSize(); i++)
	{
		if (!ValidateField(value[aCols[i]-1], i)) 
		{
			//SetWarningText(_T(""));
			return FALSE;
		}
	}
	*/
	// 레코드에 대해 Validation을 검사한다.
	if (!ValidateRecord(value, aCols))
	{
		//SetWarningText(_T(""));
		return FALSE;
	}

	// 레코드를 만들어서 리턴한다.
	int nPos;
	for (int i = 0; i < GetColCount(); i++)
	{
		nPos = aCols[i]-1; // col id i에 해당하는 value가 저장된 위치
		if(i==0) key = _tstof(value[nPos]);
		else ConvStrToData(i, value[nPos], data);
	}
	return TRUE;
}

BOOL CCMWindPressureProfileGrid::AddToDB(ROWCOL nRow)
{
	// Record를 만들어서 DB에 입력한다.
	T_WIND_PRESSURE_PROFILE data;
 
	GetValue(nRow, data);

	return AddData(data.dLevel, data);
}

BOOL CCMWindPressureProfileGrid::AddToDB(CArray<void*, void*>& raKey, CArray<void*, void*>& raData)
{
	ASSERT(raKey.GetSize() == raData.GetSize());
	double tKey;
	CArray<double, double> aKey;
	CArray<T_WIND_PRESSURE_PROFILE, T_WIND_PRESSURE_PROFILE&> aData;
	for (int i = 0; i < raKey.GetSize(); i++)
	{
		tKey = *((double*)raKey.GetAt(i));
		aKey.Add(tKey);
		aData.Add(*((T_WIND_PRESSURE_PROFILE*)raData.GetAt(i)));
	}
	return AddData(aKey, aData);
}


BOOL CCMWindPressureProfileGrid::DeleteFromDB(ROWCOL nRow)
{
	T_WIND_PRESSURE_PROFILE data;
 
	GetValue(nRow, data);
	return DelData(data.dLevel);
}

BOOL CCMWindPressureProfileGrid::DeleteFromDB(CRowColArray& awRows)
{
	if (awRows.GetSize() <= 0) return TRUE;

	ROWCOL nCol;
	VERIFY(GetColumnNoFor(0, nCol));

	CString value;
	CArray<double, double> awKeys;
 
	for (int i = 0; i < awRows.GetSize(); i++)
	{
		value = GetValueRowColBeforeEdit(awRows.GetAt(i), nCol);
		awKeys.Add(_tstof(value));
	}

	return DelData(awKeys);
}

BOOL CCMWindPressureProfileGrid::ModifyToDB(ROWCOL nRow)
{
	ROWCOL nCol;
	VERIFY(GetColumnNoFor(0, nCol));

	double oldKey;
	T_WIND_PRESSURE_PROFILE data;
	
	oldKey = _tstof(GetValueRowColBeforeEdit(nRow, nCol));
	GetValue(nRow, data);

	// 수정한다. 실패하면 에러 리턴
	return ModifyData(oldKey, data.dLevel, data); 
}

BOOL CCMWindPressureProfileGrid::ModifyToDB(CStringArray& raOldKey, 
												 CArray<void*, void*>& raKey, 
												 CArray<void*, void*>& raData)
{
	ASSERT(raKey.GetSize() == raData.GetSize());
	ASSERT(raKey.GetSize() == raOldKey.GetSize());
	CArray<double, double> aOldKey, aKey;
	CArray<T_WIND_PRESSURE_PROFILE, T_WIND_PRESSURE_PROFILE&> aData;
	double Key;
	int nSize = raKey.GetSize();
	for (int i = 0; i < nSize; i++)
	{
		Key = _tstof(raOldKey.GetAt(i));
		aOldKey.Add(Key);
		aKey.Add(*((double*)raKey.GetAt(i)));
		aData.Add(*((T_WIND_PRESSURE_PROFILE*)raData.GetAt(i)));
	}
	return ModifyData(aOldKey, aKey, aData);
}


CString CCMWindPressureProfileGrid::GetNewKey()
{
	CString value(_T("0.0"));
	return value;
}

CString CCMWindPressureProfileGrid::GetDefValue(int nIndex)
{
	return m_aCurDefVal[nIndex];
}

CString* CCMWindPressureProfileGrid::GetDefValueArray()
{
	return &m_aCurDefVal[0];
}

BOOL CCMWindPressureProfileGrid::MakeKeyAndDataArray(CStringArray& value, CRowColArray& awCols, 
																	CArray<void*, void*>& aKey, CArray<void*, void*>& aData)
{
	double* pKey;
	T_WIND_PRESSURE_PROFILE* pData;
	pKey = new double;
	pData = new T_WIND_PRESSURE_PROFILE;
	aKey.Add((void*)pKey);
	aData.Add((void*)pData);
	if (!ValidateAndMakeRecord(value, awCols, *pKey, *pData)) return FALSE;
	return TRUE;
}

void CCMWindPressureProfileGrid::DeleteKeyAndDataArray(CArray<void*, void*>& aKey, CArray<void*, void*>& aData)
{
	double* pKey;
	T_WIND_PRESSURE_PROFILE* pData;
	ASSERT(aKey.GetSize() == aData.GetSize());
	for (int i = 0; i < aKey.GetSize(); i++)
	{
		pKey = (double*)aKey.GetAt(i);
		pData = (T_WIND_PRESSURE_PROFILE*)aData.GetAt(i);
		delete pKey;
		delete pData;
	}
}

long CCMWindPressureProfileGrid::GetCountData()
{
	return m_pData->arPresProfile.GetSize();
}

void CCMWindPressureProfileGrid::MakeItemFiltered(long nFilteredNum, long* aFilteredKeys, CRowColArray& awCols)
{
	ASSERT(0); // no filtering
}

void CCMWindPressureProfileGrid::MakeItemNormal(long nNumData, CRowColArray& awCols)
{
	T_WIND_PRESSURE_PROFILE data;

	long i;
	long nCount = 1;
	//테이블 상단이 마지막이 되도록 수정하였음.
	//for(nCount=0 ; nCount < nNumData; nCount++)
	long lSizeProfile = m_pData->arPresProfile.GetSize();
	ASSERT(lSizeProfile == nNumData);
	for(i=nNumData-1 ; i >= 0; i--)
	{
		data = m_pData->arPresProfile[i];

		SetValue(GetParam()->GetData(), nCount++, data.dLevel, data, awCols);
	}
}

void CCMWindPressureProfileGrid::MakeItemEditingRecord(ROWCOL ncRow, CStringArray& EditingRecord)
{
	CDBDoc* pDoc = m_pDoc;
	double Key;
	T_WIND_PRESSURE_PROFILE Data;
	for (int i = 0; i < GetColCount(); i++)
	{
		if (i == 0) Key = _tstof(EditingRecord[i]);
		else ConvStrToData(i, EditingRecord[i], Data);
	}
	// 현재 값을 설정한다.
	SetValue(ncRow, Key, Data);
}

/////////////////////////////////////////////////////////////////////////////
// CCMWindPressureProfileGrid Implementation
BOOL CCMWindPressureProfileGrid::GetValue(ROWCOL nRow, T_WIND_PRESSURE_PROFILE& profileItem)
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

		if(i==0) profileItem.dLevel = _tstof(str);
		else ConvStrToData(i, str, profileItem);
	}
	return TRUE;
}

BOOL CCMWindPressureProfileGrid::SetValue(ROWCOL nRow, const double &key, const T_WIND_PRESSURE_PROFILE &data)
{
	//
	CRowColArray awCols;
	awCols.SetSize(GetColCount());
	for (int i = 0;i < GetColCount(); i++)
		VERIFY(GetColumnNoFor(m_aTitle[i], awCols[i]));
	return SetValue(GetParam()->GetData(), nRow, key, data, awCols);
}

BOOL CCMWindPressureProfileGrid::SetValue(CGXData* pData, ROWCOL nRow, const double &key, const T_WIND_PRESSURE_PROFILE &data, CRowColArray &awCols)
{
	//
	CString str;
	for (int i = 0; i < GetColCount(); i++)
	{
		if(i==0) str.Format(DATA_FMT,key);
		else ConvDataToStr(i, data, str);
		pData->StoreValueRowCol(nRow, awCols[i], str, gxOverride, GX_VT_NUMERIC);
	}

	return TRUE;
}

void CCMWindPressureProfileGrid::ConvStrToData(int i, CString& value, T_WIND_PRESSURE_PROFILE& data)
{
	if (i==EN_WINDP_PROFILE_GRIDCOL_02_WIDTH)         data.dWindWidth = _tstof(value);
	else if(i==EN_WINDP_PROFILE_GRIDCOL_03_THICK)     data.dWindThick = _tstof(value);
	else if(i==EN_WINDP_PROFILE_GRIDCOL_04_PRESSURE)
	{
		switch (m_nDir)
		{
		case 0:
			data.dPressure = _tstof(value);
			break;
		case 1:
			data.dPressureA = _tstof(value);
			break;
		case 2:
			data.dPressureT = _tstof(value);
			break;
		default:
			break;
		}
	}
	else if(i==EN_WINDP_PROFILE_GRIDCOL_05_ADDTIONAL)
	{
		switch (m_nDir)
		{
		case 0:
			data.dAdditionalPressure = _tstof(value);
			break;
		case 1:
			data.dAdditionalPressureA = _tstof(value);
			break;
		case 2:
			data.dAdditionalPressureT = _tstof(value);
			break;
		default:
			break;
		}
	}
}

void CCMWindPressureProfileGrid::ConvDataToStr(int i, const T_WIND_PRESSURE_PROFILE& data, CString& value)
{
	if (i==EN_WINDP_PROFILE_GRIDCOL_02_WIDTH)          value.Format(DATA_FMT, data.dWindWidth);
	else if (i==EN_WINDP_PROFILE_GRIDCOL_03_THICK)     value.Format(DATA_FMT, data.dWindThick);
	else if (i==EN_WINDP_PROFILE_GRIDCOL_04_PRESSURE)
	{
		switch (m_nDir)
		{
		case 0:
			value.Format(DATA_FMT, data.dPressure);
			break;
		case 1:
			value.Format(DATA_FMT, data.dPressureA);
			break;
		case 2:
			value.Format(DATA_FMT, data.dPressureT);
			break;
		default:
			break;
		}
	}
	else if (i==EN_WINDP_PROFILE_GRIDCOL_05_ADDTIONAL)
	{
		switch (m_nDir)
		{
		case 0:
			value.Format(DATA_FMT, data.dAdditionalPressure);
			break;
		case 1:
			value.Format(DATA_FMT, data.dAdditionalPressureA);
			break;
		case 2:
			value.Format(DATA_FMT, data.dAdditionalPressureT);
			break;
		default:
			break;
		}
	}
}

void CCMWindPressureProfileGrid::MakeSearchKey(const double& key, const T_WIND_PRESSURE_PROFILE& data, CStringArray& aKey)
{
	int nColID;
	int nSortKeyNum = m_aSortInfo.GetSize();

	aKey.SetSize(nSortKeyNum);
	for (int i = 0; i < nSortKeyNum; i++)
	{
		nColID = m_aSortInfo[i].nRC;
		if (nColID == 0) aKey[i].Format(DATA_FMT, key);
		else ConvDataToStr(nColID, data, aKey[i]);
	}
}

void CCMWindPressureProfileGrid::SetHeaderTitle(T_UNIT_SYSTEM &UnitSystem, BOOL bInit)
{
	int nIndexWidth = 35;
	int i;
	int nColCount = GetColCount();
	CRect rectGrid;
	GetWindowRect(rectGrid);
	int nWndWidth = rectGrid.Width() - nIndexWidth - 22; //윈도우 너비에서 index와 scrollbar 사이즈 뺀다.
	int aSize[5];
	//double dWidthRatio[] = {0.20, 0.25, 0.28, 0.28, 0.28};
	double dWidthRatio[] = {0.18, 0.19, 0.19, 0.22, 0.22};
	for(i = 0 ; i < nColCount; i++)
	{
		aSize[i] = (int)((double)nWndWidth * dWidthRatio[i]);
	}

	CString csUnit;
	CString csTitle;

	for (i = 0; i < nColCount; i++)
	{
		//m_aUnit.SetAt(i, aUnit[nDataType][i]);
		CUnitCtrl::GetUnitSystem(m_aUnit[i], csUnit);

		if (csUnit.IsEmpty()) csTitle = m_aTitle[i];
		else                  csTitle = m_aTitle[i] + _T("\n(")+csUnit+_T(")");

		SetStyleRange(CGXRange(0, i+1), CGXStyle()
			.SetValue(csTitle)
			.SetUserAttribute(ID_USER_ATTR_COL_ID, m_aTitle[i]));

		if (bInit) SetColWidth(i+1, i+1, aSize[i]);
	}
	if (bInit) SetColWidthByDPI(0, 0, nIndexWidth);
}

BOOL CCMWindPressureProfileGrid::FindData(double key, int& nPos, int nStart)
{
	int i;
	int nSize = m_pData->arPresProfile.GetSize();
	
	CString csVal;
	double dblVal;
	int nCompResult;
	for (i = nStart; i < nSize; i++)
	{
		csVal.Format(DATA_FMT, m_pData->arPresProfile[i].dLevel);
		dblVal = _tstof(csVal);
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

BOOL CCMWindPressureProfileGrid::ExistAllKey(CArray<double, double>& aKey, double& errorKey)
{
	CArray<double, double> aTempKey;

	int nSize = aKey.GetSize();
	aTempKey.Copy(aKey);
	BOOL bSame;
	int nPos=0, nStart;
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

BOOL CCMWindPressureProfileGrid::AddData(double key, T_WIND_PRESSURE_PROFILE& data)
{
	CArray<double, double> aOldKey, aKey;
	CArray<T_WIND_PRESSURE_PROFILE, T_WIND_PRESSURE_PROFILE&> aData;
	aKey.Add(key);
	aData.Add(data);
	T_WIND_PRESSURE_PROFILE item = data;
	item.dLevel = key;

	// 동일한 key값 중복 검토
	int nPos;
	if (FindData(key, nPos, 0))
	{
		CString msg;
		msg.Format(_LS(IDS_CMD_WINDP_PROFILE_HEIGHT_ALREADY_EXIST), key);
		AfxMessageBox(msg);
		return FALSE;
	}
	m_pData->arPresProfile.InsertAt(nPos, item);
	UpdateBuffer(0, aOldKey, aKey, aData);
	DBChanged();
	return TRUE;
}

BOOL CCMWindPressureProfileGrid::AddData(CArray<double, double> &aKey,
																				 CArray<T_WIND_PRESSURE_PROFILE, T_WIND_PRESSURE_PROFILE&> &aData, BOOL bNotify)
{
	CArray<double, double> aOldKey;

	// 동일한 key값 중복 검토
	int nSize = aKey.GetSize();
	aOldKey.Copy(aKey);
	qsort((void*)aOldKey.GetData(), nSize, sizeof(double), CCompFunc::doubleAsc);

	BOOL bSame;
	int nPos=0, nStart;
	for (int i = 0; i < nSize; i++)
	{
		nStart = nPos;
		bSame = FindData(aOldKey[i], nPos, nStart);
		if (!bSame && i > 0)
			bSame = (fabs(aOldKey[i]-aOldKey[i-1]) <= EPSILON);
		if (bSame)
		{
			CString msg;
			msg.Format(_LS(IDS_CMD_WINDP_PROFILE_HEIGHT_ALREADY_EXIST), aOldKey[i]);
			AfxMessageBox(msg);
			return FALSE;
		}
		nPos = nStart;
	}
	for (int i = 0; i < nSize; i++)
	{
		bSame = FindData(aKey[i], nPos, 0);
		VERIFY(!bSame);

		T_WIND_PRESSURE_PROFILE item = aData[i];
		item.dLevel = aKey[i];
		m_pData->arPresProfile.InsertAt(nPos, item);
	}
	UpdateBuffer(0, aOldKey, aKey, aData);

	if (bNotify) DBChanged();
	return TRUE;
}

BOOL CCMWindPressureProfileGrid::DelData(double key)
{
	CArray<double, double> aOldKey, aKey;
	CArray<T_WIND_PRESSURE_PROFILE, T_WIND_PRESSURE_PROFILE&> aData;
	aKey.Add(key);
	aData.SetSize(1);

	int nPos;
	if (!FindData(key, nPos))
	{
		CString msg;
		msg.Format(_LS(IDS_WG_CMD__ADDD__Time__g_does_not_exist_), key);
		AfxMessageBox(msg);
		return FALSE;
	}
	m_pData->arPresProfile.RemoveAt(nPos);
	UpdateBuffer(1, aOldKey, aKey, aData);
	DBChanged();
	return TRUE;
}

BOOL CCMWindPressureProfileGrid::DelData(CArray<double, double> &aKey, BOOL bNotify)
{
	CArray<double, double> aOldKey;
	CArray<T_WIND_PRESSURE_PROFILE, T_WIND_PRESSURE_PROFILE&> aData;
	int nSize = aKey.GetSize();
	int i;
	aData.SetSize(nSize);

	aOldKey.Copy(aKey);
	qsort((void*)aOldKey.GetData(), nSize, sizeof(double), CCompFunc::doubleAsc);
	// 동일한 key값 중복 검토
	double errKey;
	if (!ExistAllKey(aOldKey, errKey))
	{
		CString msg;
		msg.Format(_LS(IDS_WG_CMD__ADDD__Time__g_does_not_exist_), errKey);
		AfxMessageBox(msg);
		return FALSE;
	}
	int nSize2 = m_pData->arPresProfile.GetSize();
	int j = nSize2-1;
	double dblDiff;
	for (i = nSize-1; i >= 0; i--)
	{
		for (; j >= 0; j--)
		{
			dblDiff = aOldKey[i] - m_pData->arPresProfile[j].dLevel;
			if (fabs(dblDiff) <= EPSILON) 
			{
				m_pData->arPresProfile.RemoveAt(j);
				j--;
				break;
			}
		}
	}

	UpdateBuffer(1, aOldKey, aKey, aData);

	if (bNotify) DBChanged();
	return TRUE;
}

BOOL CCMWindPressureProfileGrid::ModifyData(double oldKey, double key, T_WIND_PRESSURE_PROFILE& data)
{
	CArray<double, double> aOldKey, aKey;
	CArray<T_WIND_PRESSURE_PROFILE, T_WIND_PRESSURE_PROFILE&> aData;
	aOldKey.Add(oldKey);
	aKey.Add(key);
	aData.Add(data);

	int nPos;
	if (!FindData(oldKey, nPos))
	{
		CString msg;
		msg.Format(_LS(IDS_WG_CMD__ADDD__Old_Time__g_does_not_exist_), oldKey);
		AfxMessageBox(msg);
		return FALSE;
	}
	if (fabs(oldKey - key) <= EPSILON)
	{
		m_pData->arPresProfile[nPos] = data;
	}
	else
	{
		int nPos2;
		if (FindData(key, nPos2))
		{
			CString msg;
			msg.Format(_LS(IDS_WG_CMD__ADDD__New_Time__g_already_exists_), key);
			AfxMessageBox(msg);
			return FALSE;
		}
		T_WIND_PRESSURE_PROFILE item = data;
		item.dLevel = key;
		m_pData->arPresProfile.RemoveAt(nPos);
		if (nPos2 > nPos) nPos2--;
		m_pData->arPresProfile.InsertAt(nPos2, item);
	}

	UpdateBuffer(2, aOldKey, aKey, aData);

	DBChanged();
	return TRUE;
}

BOOL CCMWindPressureProfileGrid::ModifyData(CArray<double, double> &aOldKey, CArray<double, double> &aKey,
																						CArray<T_WIND_PRESSURE_PROFILE, T_WIND_PRESSURE_PROFILE&> &aData)
{
	CArray<T_WIND_PRESSURE_PROFILE, T_WIND_PRESSURE_PROFILE&> aOrgData;
	aOrgData.Copy(m_pData->arPresProfile);
	
	if (!DelData(aOldKey, FALSE))
		return FALSE;
	if (!AddData(aKey, aData, FALSE))
	{
		m_pData->arPresProfile.Copy(aOrgData);
		return FALSE;
	}
	DBChanged();
	return TRUE;
}

void CCMWindPressureProfileGrid::UpdateBuffer(int nCmd, CArray<double, double> &aOldKey, CArray<double, double> &aKey,
																							CArray<T_WIND_PRESSURE_PROFILE, T_WIND_PRESSURE_PROFILE&> &aData)
{
	// 현재 Row, Col을 저장 : 나중에 복구하기 위함
	ROWCOL ncRow, ncCol;
	GetCurrentCell(ncRow, ncCol);

	BOOL bOldLock = LockUpdate(TRUE);

	int nSize = aKey.GetSize();
	for (int i = 0; i < nSize; i++)
	{
		switch(nCmd)
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

	LockUpdate(bOldLock);
	if (!bOldLock) Redraw();

	if (m_pParent!= NULL)  m_pParent->UpdateProfileData();
	if (m_pParentKBC2016!= NULL)  m_pParentKBC2016->UpdateProfileData();
}

void CCMWindPressureProfileGrid::DBChanged()
{
	CCMWindPressureProfileDlg* pDlg = (CCMWindPressureProfileDlg*)GetParent();
	if (!pDlg)
		return;
	if (pDlg->GetSafeHwnd() == NULL)
		return;
	if (!IsWindow(pDlg->GetSafeHwnd()))
		return;

	pDlg->OnDataChanged();
}
