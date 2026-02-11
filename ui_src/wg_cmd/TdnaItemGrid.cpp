// TdnaItemGrid.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "TdnaItemGrid.h"

#include "..\wg_base\wg_base_CompFunc.h"
#include "..\wg_db\wg_db_UnitCtrl.h"

#include "CMTendonProfileDlg.h"



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

#define CURVTY_SPLIN 0
#define CURVTY_ROUND 1
#define CURVTY_PARAB 2

#define PARAB_LOW_COUNT 4

/////////////////////////////////////////////////////////////////////////////
// CTdnaItemGrid
CString CTdnaItemGrid::m_aCurDefVal[] = {_T("0.0"), _T("0.0"), _T("0"), _T("0.0"), _T("0.0"), _T("0.0"), _T(""), _T("0.0"), _T("0.0"), _T("0.0"), _T("0")};
//                                                      bfix                                                      bBOT

CTdnaItemGrid::CTdnaItemGrid()
{
	m_pData = 0;
	m_nCurveType = CURVTY_SPLIN;
	m_nInputType = 1;

	AddColInfo(_T("x"), t_real, D_UNITSYS_BASE_LENGTH);
	AddColInfo(_T("y"), t_real, D_UNITSYS_BASE_LENGTH);
	AddColInfo(_T("z"), t_real, D_UNITSYS_BASE_LENGTH);
	AddColInfo(_LS(IDS_CMD0417__fix), t_integer, 0);
	AddColInfo(_T("Ry"), t_real, D_UNITSYS_BASE_DEGREE);
	AddColInfo(_T("Rz"), t_real, D_UNITSYS_BASE_DEGREE);
	//AddColInfo(_T("Radius"), t_real, D_UNITSYS_BASE_LENGTH);
	AddColInfo(_T("R"), t_real, D_UNITSYS_BASE_LENGTH);
	//AddColInfo(_LS(IDS_CMD_TDNA_GRID_ADD)_T("Add"), t_integer, 0);
	AddColInfo(_LS(IDS_CMD_TDNA_GRID_ADD), t_integer, 0);
	AddColInfo(_T("A"), t_real, D_UNITSYS_BASE_DEGREE);
	AddColInfo(_T("h"), t_real, D_UNITSYS_BASE_LENGTH);
	AddColInfo(_T("r"), t_real, D_UNITSYS_BASE_LENGTH);
	AddColInfo(_T("BOT"), t_integer, 0);

	m_aPrimaryKey.Add(0);

	CString aCurDefVal[] = {_T("0.0"), _T("0.0"), _T("0"), _T("0.0"), _T("0.0"), _T("0.0"), D_OPT_NONE, _T("0.0"), _T("0.0"), _T("0.0"), _T("0")};
	for(int i=0; i<sizeof(aCurDefVal)/sizeof(CString); i++)
		m_aCurDefVal[i] = aCurDefVal[i];
}

CTdnaItemGrid::~CTdnaItemGrid()
{
}


BEGIN_MESSAGE_MAP(CTdnaItemGrid, CTBCommon)
	//{{AFX_MSG_MAP(CTdnaItemGrid)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CTdnaItemGrid message handlers

void CTdnaItemGrid::Initialize(int nType, T_TDNA_D *pData)
{
	ASSERT(pData);
	ASSERT(nType == 1 || nType == 2 || nType == 3);
	m_nInputType = nType;
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
												(CGXStyle) pProp->sInvertNoBorder);

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
			.SetVerticalAlignment(DT_VCENTER)
			.SetHorizontalAlignment(DT_RIGHT)
			.SetValueType(GX_VT_NUMERIC)
			.SetFormat(GX_FMT_FIXED)
			.SetPlaces(4));
	SetRowHeightByDPI(0, 0, 24);

	SetStyleRange(CGXRange().SetCols(4), CGXStyle()
			.SetControl(GX_IDS_CTRL_CHECKBOX3D)
			.SetHorizontalAlignment(DT_CENTER)
			.SetPlaces(0)
			.SetValue(_T("0")));
	SetStyleRange(CGXRange().SetCols(5, 6), CGXStyle()
			.SetPlaces(2)
			.SetEnabled(FALSE));
	SetStyleRange(CGXRange().SetCols(7), CGXStyle()
			.SetPlaces(4)
			.SetEnabled(FALSE));
	SetStyleRange(CGXRange().SetCols(12), CGXStyle()
			.SetControl(GX_IDS_CTRL_CHECKBOX3D)
			.SetHorizontalAlignment(DT_CENTER)
			.SetPlaces(0)
			.SetValue(_T("0")));

	CString csChoiceList;
	csChoiceList.Format(_T("%s\n%s\n%s\n"), D_OPT_NONE, D_OPT_LEFT, D_OPT_RIGHT);
	SetStyleRange(CGXRange().SetCols(8), CGXStyle()
			.SetControl(GX_IDS_CTRL_TABBED_COMBOBOX)
			.SetChoiceList(csChoiceList)
			.SetUserAttribute(GX_IDS_UA_TABLIST_KEYCOL, _T("0"))
			.SetUserAttribute(GX_IDS_UA_TABLIST_TEXTCOL, _T("0"))
			.SetUserAttribute(GX_IDS_UA_TABLIST_SHOWALLCOLS, _T("1"))
			.SetValueType(GX_VT_STRING)
			.SetHorizontalAlignment(DT_LEFT)
			.SetValue(_T(""))
			);

	// always show vertical scrollbar
	SetScrollBarMode(SB_VERT, gxnEnabled | gxnEnhanced);

	// 현재 셀 설정
	SetRightValidCell(1, 0);
	GetParam()->SetHideCurrentCell(GX_HIDE_NEVER);

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

BOOL CTdnaItemGrid::InsertRecordByKey(const double &key, const T_TDNA_BASE_GRID &data)
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

BOOL CTdnaItemGrid::DeleteRecordByKey(const double &key, const T_TDNA_BASE_GRID &data)
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
// CTdnaItemGrid Overridable Functions
void CTdnaItemGrid::OnChangedSelectionPublic(long *keys, int nSize)
{
	return;
}

long CTdnaItemGrid::GetSelectedKeys(CArray<long, long> &caKey)
{
	return 0;
}

BOOL CTdnaItemGrid::ValidateField(CString value, int nColID)
{
	int nErrCode;

	if (m_nInputType == 1)  // 3D
	{
		if (nColID == 0 || nColID == 1 || nColID == 2 || nColID == 4 || nColID == 5)
		{
			if (!IsValidDoubleNumber(value, nErrCode))
				return InvalidDoubleNumber(nErrCode);
		}
	}
	else if (m_nInputType == 2) // 2D-XY
	{
		if (nColID == 0 || nColID == 1 || nColID == 4)
		{
			if (!IsValidDoubleNumber(value, nErrCode))
				return InvalidDoubleNumber(nErrCode);
		}
	}
	else if (m_nInputType == 3) // 2D-XZ
	{
		if (nColID == 0 || nColID == 2 || nColID == 5)
		{
			if (!IsValidDoubleNumber(value, nErrCode))
				return InvalidDoubleNumber(nErrCode);
		}
	}
	else ASSERT(0);

	// Curve Type : Round
	if (m_nCurveType == CURVTY_ROUND)
	{
		if (nColID == 6 || nColID == 8 || nColID == 9 || nColID == 10)
		{
			if (!IsValidDoubleNumber(value, nErrCode))
				return InvalidDoubleNumber(nErrCode);
		}
	}
	return TRUE;
}

// 삽입이나 수정전에 Record의 Validation을 검사한다.
BOOL CTdnaItemGrid::ValidateRecord(CStringArray &value, CRowColArray &aCols)
{
	CString msg = _T("");
	int nPos;

	for (int i = 0; i < GetColCount(); i++)
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
BOOL CTdnaItemGrid::ValidateAndMakeRecord(CStringArray &value, CRowColArray& aCols, double &key, T_TDNA_BASE_GRID& data)
{
	// 레코드에 대해 Validation을 검사한다.
	if (!ValidateRecord(value, aCols))
		return FALSE;

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

BOOL CTdnaItemGrid::AddToDB(ROWCOL nRow)
{
	// Record를 만들어서 DB에 입력한다.
	double key;
	T_TDNA_BASE_GRID data;
 
	GetValue(nRow, key, data);

	return AddData(key, data);
}

BOOL CTdnaItemGrid::AddToDB(CArray<void*, void*>& raKey, CArray<void*, void*>& raData)
{
	ASSERT(raKey.GetSize() == raData.GetSize());
	double tKey;
	CArray<double, double> aKey;
	CArray<T_TDNA_BASE_GRID, T_TDNA_BASE_GRID&> aData;
	for (int i = 0; i < raKey.GetSize(); i++)
	{
		tKey = *((double*)raKey.GetAt(i));
		aKey.Add(tKey);
		aData.Add(*((T_TDNA_BASE_GRID*)raData.GetAt(i)));
	}
	return AddData(aKey, aData);
}


BOOL CTdnaItemGrid::DeleteFromDB(ROWCOL nRow)
{
	double key;
	T_TDNA_BASE_GRID data;
 
	GetValue(nRow, key, data);
	return DelData(key);
}

BOOL CTdnaItemGrid::DeleteFromDB(CRowColArray& awRows)
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

BOOL CTdnaItemGrid::ModifyToDB(ROWCOL nRow)
{
	ROWCOL nCol;
	VERIFY(GetColumnNoFor(0, nCol));

	double key, oldKey;
	T_TDNA_BASE_GRID data;
	
	oldKey = _tstof(GetValueRowColBeforeEdit(nRow, nCol));
	GetValue(nRow, key, data);

	// 수정한다. 실패하면 에러 리턴
	return ModifyData(oldKey, key, data); 
}

BOOL CTdnaItemGrid::ModifyToDB(CStringArray& raOldKey, 
												 CArray<void*, void*>& raKey, 
												 CArray<void*, void*>& raData)
{
	ASSERT(raKey.GetSize() == raData.GetSize());
	ASSERT(raKey.GetSize() == raOldKey.GetSize());
	CArray<double, double> aOldKey, aKey;
	CArray<T_TDNA_BASE_GRID, T_TDNA_BASE_GRID&> aData;
	double Key;
	int nSize = raKey.GetSize();
	for (int i = 0; i < nSize; i++)
	{
		Key = _tstof(raOldKey.GetAt(i));
		aOldKey.Add(Key);
		aKey.Add(*((double*)raKey.GetAt(i)));
		aData.Add(*((T_TDNA_BASE_GRID*)raData.GetAt(i)));
	}
	return ModifyData(aOldKey, aKey, aData);
}


CString CTdnaItemGrid::GetNewKey()
{
	CString value(_T("0"));
	return value;
}

CString CTdnaItemGrid::GetDefValue(int nIndex)
{
	return m_aCurDefVal[nIndex];
}

CString* CTdnaItemGrid::GetDefValueArray()
{
	return m_aCurDefVal;
}

BOOL CTdnaItemGrid::MakeKeyAndDataArray(CStringArray& value, CRowColArray& awCols, 
																	CArray<void*, void*>& aKey, CArray<void*, void*>& aData)
{
	double* pKey;
	T_TDNA_BASE_GRID* pData;
	pKey = new double;
	pData = new T_TDNA_BASE_GRID;
	aKey.Add((void*)pKey);
	aData.Add((void*)pData);
	if (!ValidateAndMakeRecord(value, awCols, *pKey, *pData)) return FALSE;
	return TRUE;
}

void CTdnaItemGrid::DeleteKeyAndDataArray(CArray<void*, void*>& aKey, CArray<void*, void*>& aData)
{
	double* pKey;
	T_TDNA_BASE_GRID* pData;
	ASSERT(aKey.GetSize() == aData.GetSize());
	for (int i = 0; i < aKey.GetSize(); i++)
	{
		pKey = (double*)aKey.GetAt(i);
		pData = (T_TDNA_BASE_GRID*)aData.GetAt(i);
		delete pKey;
		delete pData;
	}
}

long CTdnaItemGrid::GetCountData()
{
	int nCount;
	if (m_nInputType == 1) nCount = m_pData->aProfile.GetSize();
	else if (m_nInputType == 2) nCount = m_pData->aProfileY.GetSize();
	else if (m_nInputType == 3) nCount = m_pData->aProfileZ.GetSize();
	else ASSERT(0);
	return nCount; 
}

void CTdnaItemGrid::MakeItemFiltered(long nFilteredNum, long* aFilteredKeys, CRowColArray& awCols)
{
	ASSERT(0); // no filtering
}

void CTdnaItemGrid::MakeItemNormal(long nNumData, CRowColArray& awCols)
{
	double Key;
	long nCount;
	if (m_nInputType == 1)
	{
		T_TDNA_BASE Data;
		T_TDNA_BASE_GRID GridData;
		for(nCount=0 ; nCount < nNumData; nCount++)
		{
			Key = m_pData->aProfile[nCount].x;
			Data = m_pData->aProfile[nCount];

			// Grid Data Setting
			Base2GridBase(Data, GridData);
			SetValue(GetParam()->GetData(), nCount+1, Key, GridData, awCols);
		} // end of loop nCount
	}
	else
	{
		T_TDNA_BASE_GRID Data;
		T_TDNA_BASE_2D Data2D;
		for(nCount=0 ; nCount < nNumData; nCount++)
		{
			Key = (m_nInputType == 2) ? m_pData->aProfileY[nCount].x : m_pData->aProfileZ[nCount].x;
			Data2D = (m_nInputType == 2) ? m_pData->aProfileY[nCount] : m_pData->aProfileZ[nCount]; 

			Base2GridBase(Data2D, Data);
			SetValue(GetParam()->GetData(), nCount+1, Key, Data, awCols);
		} // end of loop nCount
	}
}

void CTdnaItemGrid::MakeItemEditingRecord(ROWCOL ncRow, CStringArray& EditingRecord)
{
	CDBDoc* pDoc = m_pDoc;
	double Key;
	T_TDNA_BASE_GRID Data;
	for (int i = 0; i < GetColCount(); i++)
	{
		if (i == 0) Key = _tstof(EditingRecord[i]);
		else ConvStrToData(i, EditingRecord[i], Data);
	}
	// 현재 값을 설정한다.
	SetValue(ncRow, Key, Data);
}

/////////////////////////////////////////////////////////////////////////////
// CTdnaItemGrid Implementation
BOOL CTdnaItemGrid::GetValue(ROWCOL nRow, double &key, T_TDNA_BASE_GRID &data)
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

		if(i==0) key = data.x = _tstof(str);
		else ConvStrToData(i, str, data);
	}
	return TRUE;
}

BOOL CTdnaItemGrid::SetValue(ROWCOL nRow, const double &key, const T_TDNA_BASE_GRID &data)
{
	//
	CRowColArray awCols;
	awCols.SetSize(GetColCount());
	for (int i = 0;i < GetColCount(); i++)
		VERIFY(GetColumnNoFor(m_aTitle[i], awCols[i]));
	return SetValue(GetParam()->GetData(), nRow, key, data, awCols);
}

BOOL CTdnaItemGrid::SetValue(CGXData* pData, ROWCOL nRow, const double &key, const T_TDNA_BASE_GRID& data, CRowColArray &awCols)
{
	//
	CString str;
	for (int i = 0; i < GetColCount(); i++)
	{
		if(i==0) str.Format(KEY_FMT,key);
		else ConvDataToStr(i, data, str);
		pData->StoreValueRowCol(nRow, awCols[i], str, gxOverride, GX_VT_NUMERIC);
	}
	ChangeAngleColEnable(nRow, data.bFix);
	ChangeGridColEnable(nRow);

	return TRUE;
}

void CTdnaItemGrid::ConvStrToData(int i, CString& value, T_TDNA_BASE_GRID &data)
{
	if (i==1) data.y = _tstof(value);
	else if (i==2) data.z = _tstof(value);
	else if (i==3) data.bFix = _ttoi(value);
	else if (i==4) data.dRy = _tstof(value);
	else if (i==5) data.dRz = _tstof(value);
	else if (i==6) data.dRadius = _tstof(value);
	else if (i==7)
	{
		if (value.Compare(D_OPT_NONE) == 0)
			data.nAddOpt = 0;
		else if (value.Compare(D_OPT_LEFT) == 0)
			data.nAddOpt = 1;
		else if (value.Compare(D_OPT_RIGHT) == 0)
			data.nAddOpt = 2;
	}
	else if (i==8) data.dAngle = _tstof(value);
	else if (i==9) data.dHeight = _tstof(value);
	else if (i==10) data.dRadius2 = _tstof(value);
	else if (i==11) data.bBOT = _tstof(value);
}

void CTdnaItemGrid::ConvDataToStr(int i, const T_TDNA_BASE_GRID &data, CString &value)
{
	if (i==1) value.Format(DATA_FMT, data.y);
	else if (i==2) value.Format(DATA_FMT, data.z);
	else if (i==3) value.Format(_T("%d"), data.bFix);
	else if (i==4) value.Format(DATA_FMT, data.dRy);
	else if (i==5) value.Format(DATA_FMT, data.dRz);
	else if (i==6) value.Format(DATA_FMT, data.dRadius);
	else if (i==7) 
	{
		if (data.nAddOpt == 0)
			value.Format(D_OPT_NONE);
		else if (data.nAddOpt == 1)
			value.Format(D_OPT_LEFT);
		else if (data.nAddOpt == 2)
			value.Format(D_OPT_RIGHT);
	}
	else if (i==8) value.Format(DATA_FMT, data.dAngle);
	else if (i==9) value.Format(DATA_FMT, data.dHeight);
	else if (i==10) value.Format(DATA_FMT, data.dRadius2);
	else if (i==11) value.Format(_T("%d"), data.bBOT);
}

void CTdnaItemGrid::MakeSearchKey(const double &key, const T_TDNA_BASE_GRID& data, CStringArray &aKey)
{
	int nColID;
	int nSortKeyNum = m_aSortInfo.GetSize();

	aKey.SetSize(nSortKeyNum);
	for (int i = 0; i < nSortKeyNum; i++)
	{
		nColID = m_aSortInfo[i].nRC;
		if (nColID == 0) aKey[i].Format(KEY_FMT, key);
		else ConvDataToStr(nColID, data, aKey[i]);
	}
}

void CTdnaItemGrid::SetHeaderTitle(T_UNIT_SYSTEM &us, BOOL bInit)
{
	// Set Title
	int aSize[12];
	if (m_nInputType == 1)
	{
		aSize[0]=52; aSize[1]=52; aSize[2]=52; aSize[3]=20; aSize[4]=54; aSize[5]=54; aSize[6]=120; 
		aSize[7]=0; aSize[8]=0; aSize[9]=0; aSize[10]=0; aSize[11]=0;
	}
	else if (m_nInputType == 2)
	{
		aSize[0]=38; aSize[1]=38; aSize[2]= 0; aSize[3]=20; aSize[4]= 0; aSize[5]=60; aSize[6]=40; 
		aSize[7]=50; aSize[8]=46; aSize[9]=37; aSize[10]=37; aSize[11]=0;
	}
	else if (m_nInputType == 3)
	{
		aSize[0]=38; aSize[1]= 0; aSize[2]=38; aSize[3]=20; aSize[4]=60; aSize[5]= 0; aSize[6]=40; 
		aSize[7]=50; aSize[8]=46; aSize[9]=37; aSize[10]=37; aSize[11]=30;
	}
	else ASSERT(0);

	CString csUnit, csTitle;
	int nColCount = GetColCount();
	for (int i = 0; i < nColCount; i++)
	{
		CUnitCtrl::GetUnitSystem(m_aUnit[i], csUnit);
		if (csUnit.IsEmpty()) csTitle = m_aTitle[i];
		else if (i==4 || i==5 || i==8) csTitle = m_aTitle[i] + csUnit;
		else csTitle = m_aTitle[i] + _T("(")+csUnit+_T(")");

		SetStyleRange(CGXRange(0, i+1), CGXStyle()
			.SetValue(csTitle)
			.SetUserAttribute(ID_USER_ATTR_COL_ID, m_aTitle[i]));
		if (bInit) SetColWidth(i+1, i+1, globalUtils.ScaleByDPI(aSize[i]));
	}
	if (bInit) SetColWidth(0, 0, globalUtils.ScaleByDPI(20));
}

BOOL CTdnaItemGrid::FindData(double key, int& nPos, int nStart)
{
	int i;
	int nSize;
	if (m_nInputType == 1) nSize = m_pData->aProfile.GetSize();
	else if (m_nInputType == 2) nSize = m_pData->aProfileY.GetSize();
	else if (m_nInputType == 3) nSize = m_pData->aProfileZ.GetSize();
	else ASSERT(0);
	
	CString csVal;
	double dblVal;
	int nCompResult;
	for (i = nStart; i < nSize; i++)
	{
		if (m_nInputType == 1) csVal.Format(KEY_FMT, m_pData->aProfile[i].x);
		else if (m_nInputType == 2) csVal.Format(KEY_FMT, m_pData->aProfileY[i].x);
		else if (m_nInputType == 3) csVal.Format(KEY_FMT, m_pData->aProfileZ[i].x);
		else ASSERT(0);
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

BOOL CTdnaItemGrid::ExistAllKey(CArray<double, double>& aKey, double& errorKey)
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

BOOL CTdnaItemGrid::AddData(double key, T_TDNA_BASE data)
{
	T_TDNA_BASE_GRID target;
	Base2GridBase(data, target);
	return AddData(key, target);
}

BOOL CTdnaItemGrid::AddData(double key, T_TDNA_BASE data, BOOL bBot) //2D의 XZ용
{
	T_TDNA_BASE_GRID target;
	target.bBOT = bBot;
	Base2GridBase(data, target);
	return AddData(key, target);
}

BOOL CTdnaItemGrid::AddData(double key, T_TDNA_BASE_GRID data)
{
	CArray<double, double> aOldKey, aKey;
	CArray<T_TDNA_BASE_GRID, T_TDNA_BASE_GRID&> aData;
	aKey.Add(key);
	aData.Add(data);
	
	// 동일한 key값 중복 검토
	int nPos;
	if (FindData(key, nPos, 0))
	{
		CString msg;
		msg.Format(_LS(IDS_CMD0417__x__g_already_exists), key);
		AfxMessageBox(msg);
		return FALSE;
	}

	if (m_nInputType == 1) 
	{
		T_TDNA_BASE item;
		GridBase2Base(data, item);
		item.x = key;
		m_pData->aProfile.InsertAt(nPos, item);
	}
	else
	{
		T_TDNA_BASE_2D item2;
		GridBase2Base(data, item2);
		item2.x = key;
		
		if (m_nInputType == 2) 
		{
			m_pData->aProfileY.InsertAt(nPos, item2);
			aData[0].z = 0.0; aData[0].dRy = 0.0;
		}
		else if (m_nInputType == 3) 
		{
			m_pData->aProfileZ.InsertAt(nPos, item2);
			aData[0].y = 0.0; aData[0].dRz = 0.0;
		}
		else ASSERT(0);
	}

	UpdateBuffer(0, aOldKey, aKey, aData);
	DBChanged();
	return TRUE;
}

BOOL CTdnaItemGrid::AddData(CArray<double, double> &aKey, CArray<T_TDNA_BASE_GRID, T_TDNA_BASE_GRID&> &aData, BOOL bNotify)
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
			bSame = (aOldKey[i]-aOldKey[i-1] <= EPSILON);
		if (bSame)
		{
			CString msg;
			msg.Format(_LS(IDS_CMD0417__x__g_already_exists), aOldKey[i]);
			AfxMessageBox(msg);
			return FALSE;
		}
		nPos = nStart;
	}
	for (int i = 0; i < nSize; i++)
	{
		bSame = FindData(aKey[i], nPos, 0);
		VERIFY(!bSame);

		if (m_nInputType == 1) 
		{
			T_TDNA_BASE item;
			GridBase2Base(aData[i], item);
			item.x = aKey[i];
			m_pData->aProfile.InsertAt(nPos, item);
		}
		else
		{
			T_TDNA_BASE_2D item2;
			GridBase2Base(aData[i], item2);
			item2.x = aKey[i];
			
			if (m_nInputType == 2) 
			{
				m_pData->aProfileY.InsertAt(nPos, item2);
				aData[i].z = 0.0; aData[i].dRy = 0.0;
			}
			else if (m_nInputType == 3) 
			{
				m_pData->aProfileZ.InsertAt(nPos, item2);
				aData[i].y = 0.0; aData[i].dRz = 0.0;
			}
			else ASSERT(0);
		}
	}
	UpdateBuffer(0, aOldKey, aKey, aData);

	if (bNotify) DBChanged();
	return TRUE;
}

BOOL CTdnaItemGrid::DelData(double key)
{
	CArray<double, double> aOldKey, aKey;
	CArray<T_TDNA_BASE_GRID, T_TDNA_BASE_GRID&> aData;
	T_TDNA_BASE_GRID data;
	aKey.Add(key);
	aData.Add(data);

	int nPos;
	if (!FindData(key, nPos))
	{
		CString msg;
		msg.Format(_LS(IDS_CMD0417__x__g_does_not_exist), key);
		AfxMessageBox(msg);
		return FALSE;
	}
	if (m_nInputType == 1) m_pData->aProfile.RemoveAt(nPos);
	else if (m_nInputType == 2) m_pData->aProfileY.RemoveAt(nPos);
	else if (m_nInputType == 3) m_pData->aProfileZ.RemoveAt(nPos);
	else ASSERT(0);
	UpdateBuffer(1, aOldKey, aKey, aData);
	DBChanged();
	return TRUE;
}

BOOL CTdnaItemGrid::DelData(CArray<double, double> &aKey, BOOL bNotify)
{
	CArray<double, double> aOldKey;
	CArray<T_TDNA_BASE_GRID, T_TDNA_BASE_GRID&> aData;
	T_TDNA_BASE_GRID data;
	int nSize = aKey.GetSize();
	for (int i = 0; i < nSize; i++) aData.Add(data);

	aOldKey.Copy(aKey);
	qsort((void*)aOldKey.GetData(), nSize, sizeof(double), CCompFunc::doubleAsc);
	// 동일한 key값 중복 검토
	double errKey;
	if (!ExistAllKey(aOldKey, errKey))
	{
		CString msg;
		msg.Format(_LS(IDS_CMD0417__x__g_does_not_exist), errKey);
		AfxMessageBox(msg);
		return FALSE;
	}

	int nSize2;
	if (m_nInputType == 1) nSize2 = m_pData->aProfile.GetSize();
	else if (m_nInputType == 2) nSize2 = m_pData->aProfileY.GetSize();
	else if (m_nInputType == 3) nSize2 = m_pData->aProfileZ.GetSize();
	else ASSERT(0);
	int j = nSize2-1;
	double dblDiff;
	int nDeleted = 0;
	for (int i = nSize-1; i >= 0; i--)
	{
		for (; j >= 0; j--)
		{
			if (m_nInputType == 1) dblDiff = m_pData->aProfile[j].x;
			else if (m_nInputType == 2) dblDiff = m_pData->aProfileY[j].x;
			else if (m_nInputType == 3) dblDiff = m_pData->aProfileZ[j].x;
			else ASSERT(0);
			if (CCompFunc::CompReal(aOldKey[i], dblDiff) == 0)
			{
				if (m_nInputType == 1) m_pData->aProfile.RemoveAt(j);
				else if (m_nInputType == 2) m_pData->aProfileY.RemoveAt(j);
				else if (m_nInputType == 3) m_pData->aProfileZ.RemoveAt(j);
				else ASSERT(0);
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

BOOL CTdnaItemGrid::ModifyData(double oldKey, double key, T_TDNA_BASE_GRID data)
{
	CArray<double, double> aOldKey, aKey;
	CArray<T_TDNA_BASE_GRID, T_TDNA_BASE_GRID&> aData;
	aOldKey.Add(oldKey);
	aKey.Add(key);
	aData.Add(data);

	int nPos;
	if (!FindData(oldKey, nPos))
	{
		CString msg;
		msg.Format(_LS(IDS_CMD0417__Old_x__g_does_not_exist), oldKey);
		AfxMessageBox(msg);
		return FALSE;
	}
	if (oldKey == key)
	{
		if (m_nInputType == 1)
		{
			T_TDNA_BASE data2;
			GridBase2Base(data, data2);
			m_pData->aProfile[nPos] = data2;
		}
		else 
		{
			T_TDNA_BASE_2D data2;
			GridBase2Base(data, data2);
			if (m_nInputType == 2) m_pData->aProfileY[nPos] = data2;
			else if (m_nInputType == 3) m_pData->aProfileZ[nPos] = data2;
			else ASSERT(0);
		}
	}
	else
	{
		int nPos2;
		if (FindData(key, nPos2))
		{
			CString msg;
			msg.Format(_LS(IDS_CMD0417__New_x__g_already_exists), key);
			AfxMessageBox(msg);
			return FALSE;
		}

		if (m_nInputType == 1) m_pData->aProfile.RemoveAt(nPos);
		else if (m_nInputType == 2) m_pData->aProfileY.RemoveAt(nPos);
		else if (m_nInputType == 3) m_pData->aProfileZ.RemoveAt(nPos);
		else ASSERT(0);
		if (nPos2 > nPos) nPos2--;

		if (m_nInputType == 1) 
		{
			T_TDNA_BASE item;
			GridBase2Base(data, item);
			item.x = key;
			m_pData->aProfile.InsertAt(nPos2, item);
		}
		else
		{
			T_TDNA_BASE_2D item2;
			GridBase2Base(data, item2);
			item2.x = key;
			
			if (m_nInputType == 2) 
			{
				m_pData->aProfileY.InsertAt(nPos, item2);
				aData[0].z = 0.0; aData[0].dRy = 0.0;
			}
			else if (m_nInputType == 3) 
			{
				m_pData->aProfileZ.InsertAt(nPos, item2);
				aData[0].y = 0.0; aData[0].dRz = 0.0;
			}
			else ASSERT(0);
		}
	}

	UpdateBuffer(2, aOldKey, aKey, aData);

	DBChanged();
	return TRUE;
}

BOOL CTdnaItemGrid::ModifyData(CArray<double, double> &aOldKey, CArray<double, double> &aKey, CArray<T_TDNA_BASE_GRID, T_TDNA_BASE_GRID&> &aData)
{
	CArray<T_TDNA_BASE, T_TDNA_BASE&> aOrgData;
	CArray<T_TDNA_BASE_2D, T_TDNA_BASE_2D&> aOrgDataY;
	CArray<T_TDNA_BASE_2D, T_TDNA_BASE_2D&> aOrgDataZ;
	aOrgData.Copy(m_pData->aProfile);
	
	if (!DelData(aOldKey, FALSE))
		return FALSE;
	if (!AddData(aKey, aData, FALSE))
	{
		if (m_nInputType == 1) m_pData->aProfile.Copy(aOrgData);
		else if (m_nInputType == 2) ;
		else if (m_nInputType == 3) ;
		else ASSERT(0);
		/**
		{
			T_TDNA_BASE_2D OrgData;
			CArray<T_TDNA_BASE_2D, T_TDNA_BASE_2D&> aOrgData2;
			for (int i=0; i<aOrgData.GetSize(); i++)
			{
				OrgData.x = aOrgData[i].x;
				OrgData.yz = (m_nInputType == 2) ? aOrgData[i].y : aOrgData[i].z;
				OrgData.bFix = aOrgData[i].bFix;
				OrgData.dR = (m_nInputType == 2) ? aOrgData[i].dRz : aOrgData[i].dRy;
				OrgData.dRadius = aOrgData[i].dRadius;
				aOrgData2.Add(OrgData);
			}
			if (m_nInputType == 2) m_pData->aProfileY.Copy(aOrgData2);
			else if (m_nInputType == 3) m_pData->aProfileZ.Copy(aOrgData2);
			else ASSERT(0);
		}
		**/
		return FALSE;
	}

	DBChanged();
	return TRUE;
}

void CTdnaItemGrid::UpdateBuffer(int nCmd, CArray<double, double> &aOldKey, CArray<double, double> &aKey, CArray<T_TDNA_BASE_GRID, T_TDNA_BASE_GRID&> &aData)
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
}

void CTdnaItemGrid::OnUnitChange()
{
	T_UNIT_SYSTEM us;
	SetHeaderTitle(us, FALSE);
	MakeItemEx(0, -2, TRUE);
}

void CTdnaItemGrid::DBChanged()
{
	CCMTendonProfileDlg* pDlg = (CCMTendonProfileDlg*)GetParent();
	if (!pDlg) return;
	if (pDlg->GetSafeHwnd() == 0) return;
	if (!IsWindow(pDlg->GetSafeHwnd())) return;

	if (m_nInputType == 1) pDlg->OnGridDataChanged3D();
	else if (m_nInputType == 2) pDlg->OnGridDataChanged2DXY();
	else if (m_nInputType == 3) pDlg->OnGridDataChanged2DXZ();
	else ASSERT(0);
}

/////////////////////////////////////////////////////////////////////////
// 여기부터 Column 값 변경이 다른 column의 상태나 값을 변경시키는 경우를
// 처리해 주기 위해서 Override 및 새로 정의되는 함수이다.
// OnStartEditing, OnCanceledEditing, ChangeColumnStatus
// 일반적으로 OnModifyCell이 사용되나 Check Box Type일 경우는 OnStartEditing이
// 사용된다.

BOOL CTdnaItemGrid::OnStartEditing(ROWCOL nRow, ROWCOL nCol)
{
	// 부모의 OnModifyCell을 항상 먼저 호출해 주어야 한다.
	BOOL bResult = CTBCommon::OnStartEditing(nRow, nCol);
	ChangeColumnStatus(nRow, nCol);
	return bResult;
}

void CTdnaItemGrid::OnCanceledEditing(ROWCOL nRow, ROWCOL nCol)
{
	// 부모의 OnCanceledEditing을 항상 먼저 호출해 주어야 한다.
	CTBCommon::OnCanceledEditing(nRow, nCol);
	if (!IsEdit()) return;
	ChangeColumnStatus(nRow, nCol);
}

void CTdnaItemGrid::ChangeColumnStatus(ROWCOL nRow, ROWCOL nCol)
{
	ROWCOL nFlagCol;
	GetColumnNoFor(3, nFlagCol);

	if (nCol != nFlagCol) return;

	CString csFlag;
	int bFlag;

	CGXControl *pControl;
	pControl = GetControl(nRow, nCol);
	if (nCol == nFlagCol)
		pControl->GetValue(csFlag);
	else
		csFlag = GetValueRowCol(nRow, nFlagCol);
	bFlag = _tstol(csFlag);

	// Check Box는 StartEditing이므로 값이 변경되기 전이다.
	// Toggle형식이므로 반대로 설정해주면 원하는 대로 된다.
	ChangeAngleColEnable(nRow, !bFlag);
}

void CTdnaItemGrid::ChangeAngleColEnable(ROWCOL nRow, BOOL bEnable)
{
	ROWCOL nCol;
	int aColID[] = {4, 5};

	GetColumnNoFor(3, nCol);
	SetStyleRange(CGXRange(nRow, nCol), CGXStyle().SetEnabled(m_nCurveType != CURVTY_ROUND));
	if (m_nCurveType == 1) bEnable = FALSE;
	for (int i = 0; i < sizeof(aColID)/sizeof(int); i++)
	{
		GetColumnNoFor(aColID[i], nCol);
		// 변경되는 것에 따라 Enable 상태를 변경한다.
		SetStyleRange(CGXRange(nRow, nCol), CGXStyle().SetEnabled(bEnable));
		// Disable 이면 Default값으로 초기화 한다.
		if (!bEnable && m_nCurveType != CURVTY_ROUND) SetValueRange(CGXRange(nRow, nCol), double(0.0));
	}
}

void CTdnaItemGrid::SetCurveType(int nCurveType, int nShapeType)
// 2D Element를 구분하기 위해 변수 nShapeType 추가 2005. 12.23 jkpark
{

	// 여기서 Enable/Disable을 모두 해준다. -
	m_nCurveType = nCurveType;

	ROWCOL nRow, nCol;
	GetCurrentCell(nRow, nCol);
	//if (nCol >= 3)  // Angle column
	SetCurrentCell(nRow, 1);  // change current cell

	int nFix;
	CString csFix;
	ROWCOL nFixCol;
	GetColumnNoFor(3, nFixCol);

	BOOL bEnable;
	int nRowCount = GetRowCount();
	int i = 0;
	for (i = 0; i < nRowCount-1; i++)
	{
		if (nCurveType != CURVTY_SPLIN) bEnable = FALSE;
		else
		{
			csFix = GetValueRowCol(i+1, nFixCol);
			nFix = _ttoi(csFix);
			if (nFix == 1) bEnable = TRUE;
			else bEnable = FALSE;
		}
		ChangeAngleColEnable(i+1, bEnable);
	}
	SetStyleRange(CGXRange(i+1, 4, i+1, 6), CGXStyle().SetEnabled(nCurveType != CURVTY_ROUND));
	// Radius Col은 항상 Enable
	GetColumnNoFor(6, nCol);
	SetStyleRange(CGXRange().SetCols(nCol), CGXStyle().SetEnabled(TRUE));

	if (nCurveType == CURVTY_ROUND) // Curve Type == Round : Show Radis Col, Hide Angle Cols
	{
		HideColsByIntend(4, 6, TRUE); 
		HideColsByIntend(7, 7, FALSE);
	}
	else               // Curve Type == Spline : Hide Radis Col, Show Angle Cols 
	{
		HideColsByIntend(4, 6, FALSE);
		HideColsByIntend(7, 7, TRUE);
	}

	// 추가 
	HideColsByIntend(2, 2, m_nInputType == 3); // y
	HideColsByIntend(3, 3, m_nInputType == 2); // z
	HideColsByIntend(5, 5, m_nInputType == 2 || m_nCurveType != CURVTY_SPLIN); // Ry
	HideColsByIntend(6, 6, m_nInputType == 3 || m_nCurveType != CURVTY_SPLIN); // Rz

	HideColsByIntend(8, 8, m_nInputType == 1 || m_nCurveType != CURVTY_ROUND);
	HideColsByIntend(9, 9, m_nInputType == 1 || m_nCurveType != CURVTY_ROUND);
	HideColsByIntend(10, 10, m_nInputType == 1 || m_nCurveType != CURVTY_ROUND);
	HideColsByIntend(11, 11, m_nInputType == 1 || m_nCurveType != CURVTY_ROUND);

	if(CURVTY_PARAB == nCurveType)
	{
		int nRowCount = GetRowCount();
		
		while(nRowCount > PARAB_LOW_COUNT)
		{
			DeleteFromDB(PARAB_LOW_COUNT);
			nRowCount--;
		}
		
		if(nRowCount < PARAB_LOW_COUNT)
		{
			T_TDNA_BASE tdnaData;
			tdnaData.Initiaize();
			
			for(int k = nRowCount; nRowCount < PARAB_LOW_COUNT; nRowCount++)
			{
				tdnaData.x = tdnaData.x + 1.0;
				if(!AddData(tdnaData.x, tdnaData))
				{
					ASSERT(FALSE);
				}
			}
		}
		
		DBChanged();
		EnableAppendRow(FALSE);
	}
	else
	{
		EnableAppendRow(TRUE);
	}

	//Column Size 조절
	int aSize[12];
	if (m_nInputType == 1)
		return;
	else if (m_nInputType == 2)
	{
		if (nCurveType != CURVTY_ROUND)
		{
			aSize[0]=80; aSize[1]=80; aSize[2]= 0; aSize[3]=20; aSize[4]= 0; aSize[5]=80; aSize[6]=120;
			aSize[7]=0; aSize[8]=0; aSize[9]=0; aSize[10]=0; aSize[11]=0;
		}
		else
		{
//      aSize[0]=38; aSize[1]=38; aSize[2]= 0; aSize[3]=20; aSize[4]= 0; aSize[5]=60; aSize[6]=40; 
//      aSize[7]=50; aSize[8]=46; aSize[9]=37; aSize[10]=37; aSize[11]=0;
			aSize[0]=38; aSize[1]=38; aSize[2]= 0; aSize[3]=20; aSize[4]= 0; aSize[5]=60; aSize[6]=47; 
			aSize[7]=57; aSize[8]=53; aSize[9]=45; aSize[10]=45; aSize[11]=0;
		}
	}
	else if (m_nInputType == 3)
	{
		//2D Element를 구분하기 위해 추가 2005.12.23 jkpark
		int nBotSize=0;
		if(nShapeType==2)
		{
			nBotSize=30;
		}
		//추가 끝 2005.12.23 jkpark
		if (nCurveType != CURVTY_ROUND)
		{
			aSize[0]=65; aSize[1]= 0; aSize[2]=65; aSize[3]=20; aSize[4]=80; aSize[5]= 0; aSize[6]=100;
			aSize[7]=0; aSize[8]=0; aSize[9]=0; aSize[10]=0; aSize[11]=nBotSize;
		}
		else
		{
//      aSize[0]=38; aSize[1]= 0; aSize[2]=38; aSize[3]=20; aSize[4]=60; aSize[5]= 0; aSize[6]=40; 
//      aSize[7]=50; aSize[8]=46; aSize[9]=37; aSize[10]=37; aSize[11]=nBotSize;
			aSize[0]=38; aSize[1]= 0; aSize[2]=38; aSize[3]=20; aSize[4]=60; aSize[5]= 0; aSize[6]=47; 
			aSize[7]=50; aSize[8]=46; aSize[9]=45; aSize[10]=45; aSize[11]=nBotSize;
		}
	}
	else ASSERT(0);

	int nColCount = GetColCount();
	for (i = 0; i < nColCount; i++)
		SetColWidth(i+1, i+1, globalUtils.ScaleByDPI(aSize[i]));
}

void CTdnaItemGrid::ChangeGridColEnable(ROWCOL nRow)
{
	BOOL bEnable = TRUE;
	CString strValue;
	strValue = GetValueRowCol(nRow, 8);

	if (strValue.CompareNoCase(D_OPT_NONE) == 0)
		bEnable = FALSE;

	SetStyleRange(CGXRange(nRow, 9), CGXStyle().SetEnabled(bEnable));
	SetStyleRange(CGXRange(nRow,10), CGXStyle().SetEnabled(bEnable));
	SetStyleRange(CGXRange(nRow,11), CGXStyle().SetEnabled(bEnable));

	double dValue = _tstof(GetValueRowCol(nRow, 7));
/* 임시 변경 2005. 12. 21 jkpark
	if (dValue == 0)
	{
		bEnable = TRUE;
	}
	else
	{
		bEnable = FALSE;
		SetStyleRange(CGXRange(nRow, 12), CGXStyle().SetValue(_T("0")));
	}
*/
	bEnable = TRUE;     //임시 변경 2005. 12. 21 jkpark
	SetStyleRange(CGXRange(nRow, 12), CGXStyle().SetEnabled(bEnable));
}

void CTdnaItemGrid::OnModifyCell(ROWCOL nRow, ROWCOL nCol)
{
	CTBCommon::OnModifyCell(nRow, nCol);
	if (nCol == 7||nCol == 8) ChangeGridColEnable(nRow);
}

void CTdnaItemGrid::GridBase2Base(T_TDNA_BASE_GRID source, T_TDNA_BASE& target)
{
	target.Initiaize();

	target.x = source.x;
	target.y = source.y;
	target.z = source.z;
	target.bFix = ((int)source.bFix==1)? TRUE : FALSE;
	target.dRadius = source.dRadius;
	target.dRy = source.dRy;
	target.dRz = source.dRz;
}

void CTdnaItemGrid::GridBase2Base(T_TDNA_BASE_GRID source, T_TDNA_BASE_2D& target)
{
	target.Initiaize();

	target.x = source.x;
	target.yz = (m_nInputType == 2) ? source.y : source.z;
	target.bFix = ((int)source.bFix==1)? TRUE : FALSE;  // bFix에 0, 1 이외의 값이 저장되는 경우 발생함.
	target.dR = (m_nInputType == 2) ? source.dRz : source.dRy;
	target.dRadius = source.dRadius;
	
	if (m_nCurveType == CURVTY_ROUND)
	{
		target.nAddOpt = source.nAddOpt;
		target.dHeight = source.dHeight;
		target.dAngle = source.dAngle;
		target.dRadius2 = source.dRadius2;
	}
	target.bBottomZ = source.bBOT;
}

void CTdnaItemGrid::Base2GridBase(T_TDNA_BASE source, T_TDNA_BASE_GRID& target)
{
	target.Initialize();
	target.x = source.x;
	target.y = source.y;
	target.z = source.z;
	target.bFix = ((int)source.bFix==1)? TRUE : FALSE;
	target.dRadius = source.dRadius;
	target.dRy = source.dRy;
	target.dRz = source.dRz;
}

void CTdnaItemGrid::Base2GridBase(T_TDNA_BASE_2D source, T_TDNA_BASE_GRID& target)
{
	target.Initialize();
	target.x = source.x;
	target.y = (m_nInputType == 2) ? source.yz : 0.0 ;
	target.z = (m_nInputType == 2) ? 0.0 : source.yz;
	target.bFix = ((int)source.bFix==1)? TRUE : FALSE;
	target.dRy = (m_nInputType == 2) ? 0.0 : source.dR; 
	target.dRz = (m_nInputType == 2) ? source.dR : 0.0;
	target.dRadius = source.dRadius;
	if (m_nCurveType == CURVTY_ROUND)
	{
		target.nAddOpt = source.nAddOpt;
		target.dHeight = source.dHeight;
		target.dAngle = source.dAngle;
		target.dRadius2 = source.dRadius2;
	}
	target.bBOT = source.bBottomZ;
}

BOOL CTdnaItemGrid::OnInsertEmptyRecord(ROWCOL nRow)
{
	BOOL bReturn = FALSE;
	if(!(nRow > 3 && CURVTY_PARAB == m_nCurveType))
	{
		bReturn = CTBCommon::OnInsertEmptyRecord(nRow);
	}

	if (bReturn)
		ChangeGridColEnable(nRow);
	return bReturn;
}

BOOL CTdnaItemGrid::OnDeleteRecord(ROWCOL nRow)
{
	if(CURVTY_PARAB == m_nCurveType)
	{
		return FALSE;
	}
	return CTBCommon::OnDeleteRecord(nRow);
}