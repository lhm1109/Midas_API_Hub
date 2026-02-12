// TdnaItemGrid.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"

#include "CMDTendonTemplateGrid.h"
#include "CMDTendonTemplateAddModDlg.h"

#include "..\wg_base\wg_base_CompFunc.h"
#include "..\wg_db\wg_db_UnitCtrl.h"

#include "..\wg_base\wg_base_MsgDll.h"



#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define D_OPT_NONE  _LS(IDS_CMD_TDNA_GRID_NONE)
#define D_OPT_LEFT  _LS(IDS_CMD_TDNA_GRID_LEFT)
#define D_OPT_RIGHT _LS(IDS_CMD_TDNA_GRID_RIGHT)

#define EPSILON 1.e-12
#define KEY_FMT  _T("%d")
#define DATA_FMT _T("%.15g")

#define CURVTY_SPLIN 0
#define CURVTY_ROUND 1
#define CURVTY_PARAB 2

#define PARAB_LOW_COUNT 4

#define D_TDNCOL_KEY    1   // key
#define D_TDNCOL_SPAN   2   // span
#define D_TDNCOL_BT     3   // begin    t
#define D_TDNCOL_BB     4   // begin    b   : curved 2 (TNDN_TEMPLATE_TYPE_CURVE_SIMPLE_1)
#define D_TDNCOL_MH     5   // middle   h   : harped 1 (TNDN_TEMPLATE_TYPE_STRAIGHT_BENT_1)
#define D_TDNCOL_MB     6   // middle   b   : harped 1 (TNDN_TEMPLATE_TYPE_STRAIGHT_BENT_1)
#define D_TDNCOL_MH1    7   // middle   h1
#define D_TDNCOL_MB1    8   // middle   b1
#define D_TDNCOL_MR1    9   // middle   r1  : harped 2 (TNDN_TEMPLATE_TYPE_STRAIGHT_BENT_2)
#define D_TDNCOL_MH2   10   // middle   h2  : harped 2 (TNDN_TEMPLATE_TYPE_STRAIGHT_BENT_2)
#define D_TDNCOL_MB2   11   // middle   b2  : harped 2 (TNDN_TEMPLATE_TYPE_STRAIGHT_BENT_2)
#define D_TDNCOL_MR2   12   // middle   r2  : harped 2 (TNDN_TEMPLATE_TYPE_STRAIGHT_BENT_2)
#define D_TDNCOL_ET    13   // end      t'
#define D_TDNCOL_EB    14   // end      b   : curved 4 (TNDN_TEMPLATE_TYPE_CURVE_BENT_1)

/////////////////////////////////////////////////////////////////////////////
// CCMDTendonTemplateGrid
CString CCMDTendonTemplateGrid::m_aCurDefVal[] = {\
				_T("0"),_T("0.0"),\
				_T("0.0"), _T("0.0"), _T("0.0"), _T("0.0"), _T("0.0"), _T("0.0"), _T("0.0"), _T("0.0"),\
				_T("0.0"), _T("0.0"),\
				_T("0.0"), _T("0.0")};
//CString CCMDTendonTemplateGrid::m_aHeader[] = {\
//				_T(""), _T(""),\
//				_LS(IDS_CMD_TDNA_GRID_Begin), _LS(IDS_CMD_TDNA_GRID_Begin), \
//				_LS(IDS_CMD_TDNA_GRID_Middle), _LS(IDS_CMD_TDNA_GRID_Middle), _LS(IDS_CMD_TDNA_GRID_Middle), _LS(IDS_CMD_TDNA_GRID_Middle), _LS(IDS_CMD_TDNA_GRID_Middle), _LS(IDS_CMD_TDNA_GRID_Middle), _LS(IDS_CMD_TDNA_GRID_Middle), _LS(IDS_CMD_TDNA_GRID_Middle),\
//				_LS(IDS_CMD_TDNA_GRID_End), _LS(IDS_CMD_TDNA_GRID_End) };
//CString CCMDTendonTemplateGrid::m_aHeaderPK[] = {\
//				_T("key"), _LS(IDS_CMD_TDNA_GRID_span),\
//				_T("begin_dt"), _T("begin_db"),\
//				_T("mid_dh"), _T("mid_db"), _T("mid_dh1"), _T("mid_db1"), _T("mid_r1"), _T("mid_dh2"), _T("mid_db2"), _T("mid_r2"),\
//				_T("end_dt"), _T("end_db")};
//CString CCMDTendonTemplateGrid::m_aHeaderReal[] = {\
//				_T(""), _LS(IDS_CMD_TDNA_GRID_span),\
//				_T("t"), _T("b"),\
//				_T("h"), _T("b"), _T("h1"), _T("b1"), _T("r1"), _T("h2"), _T("b2"), _T("r2"),\
//				_T("t\'"), _T("b\'")};

std::vector<CString> CCMDTendonTemplateGrid::m_aHeader;
std::vector<CString> CCMDTendonTemplateGrid::m_aHeaderPK;
std::vector<CString> CCMDTendonTemplateGrid::m_aHeaderReal;

std::vector<CString>& CCMDTendonTemplateGrid::Get_HeaderAr()
{
	if (m_aHeader.size() == 0)
	{
		m_aHeader.push_back(_T(""));
		m_aHeader.push_back(_T(""));
		m_aHeader.push_back(_LS(IDS_CMD_TDNA_GRID_Begin));
		m_aHeader.push_back(_LS(IDS_CMD_TDNA_GRID_Begin));
		m_aHeader.push_back(_LS(IDS_CMD_TDNA_GRID_Middle));
		m_aHeader.push_back(_LS(IDS_CMD_TDNA_GRID_Middle));
		m_aHeader.push_back(_LS(IDS_CMD_TDNA_GRID_Middle));
		m_aHeader.push_back(_LS(IDS_CMD_TDNA_GRID_Middle));
		m_aHeader.push_back(_LS(IDS_CMD_TDNA_GRID_Middle));
		m_aHeader.push_back(_LS(IDS_CMD_TDNA_GRID_Middle));
		m_aHeader.push_back(_LS(IDS_CMD_TDNA_GRID_Middle));
		m_aHeader.push_back(_LS(IDS_CMD_TDNA_GRID_Middle));
		m_aHeader.push_back(_LS(IDS_CMD_TDNA_GRID_End));
		m_aHeader.push_back(_LS(IDS_CMD_TDNA_GRID_End));
	}
	return m_aHeader;
}

std::vector<CString>& CCMDTendonTemplateGrid::Get_HeaderPKAr()
{
	if (m_aHeaderPK.size() == 0)
	{
		m_aHeaderPK.push_back(_T("key"));
		m_aHeaderPK.push_back(_LS(IDS_CMD_TDNA_GRID_span));
		m_aHeaderPK.push_back(_T("begin_dt"));
		m_aHeaderPK.push_back(_T("begin_db"));
		m_aHeaderPK.push_back(_T("mid_dh"));
		m_aHeaderPK.push_back(_T("mid_db"));
		m_aHeaderPK.push_back(_T("mid_dh1"));
		m_aHeaderPK.push_back(_T("mid_db1"));
		m_aHeaderPK.push_back(_T("mid_r1"));
		m_aHeaderPK.push_back(_T("mid_dh2"));
		m_aHeaderPK.push_back(_T("mid_db2"));
		m_aHeaderPK.push_back(_T("mid_r2"));
		m_aHeaderPK.push_back(_T("end_dt"));
		m_aHeaderPK.push_back(_T("end_db"));
	}
	return m_aHeaderPK;
}

std::vector<CString>& CCMDTendonTemplateGrid::Get_HeaderRealAr()
{
	if (m_aHeaderReal.size() == 0)
	{
		m_aHeaderReal.push_back(_T(""));
		m_aHeaderReal.push_back(_LS(IDS_CMD_TDNA_GRID_span));
		m_aHeaderReal.push_back(_T("t"));
		m_aHeaderReal.push_back(_T("b"));
		m_aHeaderReal.push_back(_T("h"));
		m_aHeaderReal.push_back(_T("b"));
		m_aHeaderReal.push_back(_T("h1"));
		m_aHeaderReal.push_back(_T("b1"));
		m_aHeaderReal.push_back(_T("r1"));
		m_aHeaderReal.push_back(_T("h2"));
		m_aHeaderReal.push_back(_T("b2"));
		m_aHeaderReal.push_back(_T("r2"));
		m_aHeaderReal.push_back(_T("t\'"));
		m_aHeaderReal.push_back(_T("b\'"));
	}
	return m_aHeaderReal;
}

CCMDTendonTemplateGrid::CCMDTendonTemplateGrid()
{
		m_pData = 0;
		m_nInputType = TNDN_TEMPALTE_TYPE_XYXZ_XZ;

		AddColInfo(Get_HeaderPKAr()[D_TDNCOL_KEY-1],  t_integer, D_UNITSYS_NONE);
		AddColInfo(Get_HeaderPKAr()[D_TDNCOL_SPAN-1], t_integer, D_UNITSYS_NONE);

		AddColInfo(Get_HeaderPKAr()[D_TDNCOL_BT -1], t_real,  D_UNITSYS_BASE_LENGTH); // begin    t
		AddColInfo(Get_HeaderPKAr()[D_TDNCOL_BB -1], t_real,  D_UNITSYS_BASE_LENGTH); // begin    b   : curved 2 (TNDN_TEMPLATE_TYPE_CURVE_SIMPLE_1)
		AddColInfo(Get_HeaderPKAr()[D_TDNCOL_MH -1], t_real,  D_UNITSYS_BASE_LENGTH); // middle   h   : harped 1 (TNDN_TEMPLATE_TYPE_STRAIGHT_BENT_1)
		AddColInfo(Get_HeaderPKAr()[D_TDNCOL_MB -1], t_real,  D_UNITSYS_BASE_LENGTH); // middle   b   : harped 1 (TNDN_TEMPLATE_TYPE_STRAIGHT_BENT_1)
		AddColInfo(Get_HeaderPKAr()[D_TDNCOL_MH1-1], t_real,  D_UNITSYS_BASE_LENGTH); // middle   h1
		AddColInfo(Get_HeaderPKAr()[D_TDNCOL_MB1-1], t_real,  D_UNITSYS_BASE_LENGTH); // middle   b1
		AddColInfo(Get_HeaderPKAr()[D_TDNCOL_MR1-1], t_real,  D_UNITSYS_BASE_LENGTH); // middle   r1  : harped 2 (TNDN_TEMPLATE_TYPE_STRAIGHT_BENT_2)
		AddColInfo(Get_HeaderPKAr()[D_TDNCOL_MH2-1], t_real,  D_UNITSYS_BASE_LENGTH); // middle   h2  : harped 2 (TNDN_TEMPLATE_TYPE_STRAIGHT_BENT_2)
		AddColInfo(Get_HeaderPKAr()[D_TDNCOL_MB2-1], t_real,  D_UNITSYS_BASE_LENGTH); // middle   b2  : harped 2 (TNDN_TEMPLATE_TYPE_STRAIGHT_BENT_2)
		AddColInfo(Get_HeaderPKAr()[D_TDNCOL_MR2-1], t_real,  D_UNITSYS_BASE_LENGTH); // middle   r2  : harped 2 (TNDN_TEMPLATE_TYPE_STRAIGHT_BENT_2)
		AddColInfo(Get_HeaderPKAr()[D_TDNCOL_ET -1], t_real,  D_UNITSYS_BASE_LENGTH); // end      t'
		AddColInfo(Get_HeaderPKAr()[D_TDNCOL_EB -1], t_real,  D_UNITSYS_BASE_LENGTH); // end      b   : curved 4 (TNDN_TEMPLATE_TYPE_CURVE_BENT_1)

		m_aPrimaryKey.Add(0);
}

CCMDTendonTemplateGrid::~CCMDTendonTemplateGrid()
{
}


BEGIN_MESSAGE_MAP(CCMDTendonTemplateGrid, CTBCommon)
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CCMDTendonTemplateGrid message handlers

void CCMDTendonTemplateGrid::Initialize(TNDN_TEMPALTE_TYPE_XYXZ nType, TndnTemplate *pData)
{
	ASSERT( pData );
	ASSERT( nType == 0 || nType == 1 );
	m_nInputType = nType; // 0 : XZ, 1 : XY
	m_pData = pData;
	
	T_UNIT_SYSTEM us;
	CDBDoc *pDoc = CDBDoc::GetDocPoint();
	CTBCommon::Initialize(pDoc, us);
	
	GetParam()->EnableUndo(FALSE);
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
		.SetHorizontalAlignment(DT_RIGHT)
		.SetAllowEnter(FALSE)
		);

	SetStyleRange(CGXRange().SetCols(D_TDNCOL_KEY), CGXStyle()
		.SetControl(GX_IDS_CTRL_STATIC)
		.SetHorizontalAlignment(DT_CENTER)
		.SetEnabled(FALSE)
		.SetPlaces(0));

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

BOOL CCMDTendonTemplateGrid::InsertRecordByKey(const int &key, const T_TDNA_TEMPLATE_BASE_GRID &data)
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

BOOL CCMDTendonTemplateGrid::DeleteRecordByKey(const int &key, const T_TDNA_TEMPLATE_BASE_GRID &data)
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
// CCMDTendonTemplateGrid Overridable Functions
void CCMDTendonTemplateGrid::OnChangedSelectionPublic(long *keys, int nSize)
{
	return;
}

long CCMDTendonTemplateGrid::GetSelectedKeys(CArray<long, long> &caKey)
{
	caKey.RemoveAll();
	return 0;
}

BOOL CCMDTendonTemplateGrid::ValidateField(CString value, int nColID)
{
	int nErrCode;
	
	if(nColID == 0) //span
	{
		if(!this->IsValidLongNumber(value, nErrCode))
			return InvalidLongNumber(nErrCode);
	}
	else //나머지 모든 값은 이렇게.
	{
		if(!(value == _T("-")))
		{
			if (!IsValidDoubleNumber(value, nErrCode))
				return InvalidDoubleNumber(nErrCode);
		}
	}

	return TRUE;
}

// 삽입이나 수정전에 Record의 Validation을 검사한다.
BOOL CCMDTendonTemplateGrid::ValidateRecord(CStringArray &value, CRowColArray &aCols)
{
	CString msg = _T("");
	int nPos;
	
	for (int i = 0; i < GetColCount(); i++)
	{
		nPos = aCols[i] - 1;
		if (value[nPos] == _T(""))
			msg += ((msg != _T("")) ? _T("\n") : _T("")) + m_aTitle[i] + _LS(IDS_WG_CMD__ADDD___has_not_been_entered_);
		else if (!ValidateField(value[nPos], i))
			return FALSE;
	}

	if (msg == _T(""))		return TRUE;
	else				SetWarningText(msg);

	return FALSE;
}

// 이 함수는 Paste할 때 호출되므로 False를 리턴할 때는 ValidateField와 
// Validate Record에서 설정된 Warning Text를 없애주고 리턴한다.
BOOL CCMDTendonTemplateGrid::ValidateAndMakeRecord(CStringArray &value, CRowColArray& aCols, int &key, T_TDNA_TEMPLATE_BASE_GRID& data)
{
	// 레코드에 대해 Validation을 검사한다.
	if (!ValidateRecord(value, aCols))
		return FALSE;
	
	// 레코드를 만들어서 리턴한다.
	int nPos;
	for (int i = 0; i < GetColCount(); i++)
	{
		nPos = aCols[i] - 1; // col id i에 해당하는 value가 저장된 위치
		if(i==0)		key = _ttoi(value[nPos]);
		else			ConvStrToData(i+1, value[nPos], data);
	}
	return TRUE;
}

BOOL CCMDTendonTemplateGrid::AddToDB(ROWCOL nRow)
{
	// Record를 만들어서 DB에 입력한다.
	int key;
	T_TDNA_TEMPLATE_BASE_GRID data;
	
	GetValue(nRow, key, data);
	
	return AddData(key, data);
}

BOOL CCMDTendonTemplateGrid::AddToDB(CArray<void*, void*>& raKey, CArray<void*, void*>& raData)
{
	ASSERT(raKey.GetSize() == raData.GetSize());

	int tKey;
	CArray<int, int> aKey;
	CArray<T_TDNA_TEMPLATE_BASE_GRID, T_TDNA_TEMPLATE_BASE_GRID&> aData;
	for (int i = 0; i < raKey.GetSize(); i++)
	{
		tKey = *((int*)raKey.GetAt(i));
		aKey.Add(tKey);
		aData.Add(*((T_TDNA_TEMPLATE_BASE_GRID*)raData.GetAt(i)));
	}
	return AddData(aKey, aData);
}


BOOL CCMDTendonTemplateGrid::DeleteFromDB(ROWCOL nRow)
{
	int key;
	T_TDNA_TEMPLATE_BASE_GRID data;
	
	GetValue(nRow, key, data);
	return DelData(key);
}

BOOL CCMDTendonTemplateGrid::DeleteFromDB(CRowColArray& awRows)
{
	if (awRows.GetSize() <= 0) return TRUE;
	
	ROWCOL nCol;
	VERIFY(GetColumnNoFor(0, nCol));
	
	CString value;
	CArray<int, int> awKeys;
	
	for (int i = 0; i < awRows.GetSize(); i++)
	{
		value = GetValueRowColBeforeEdit(awRows.GetAt(i), nCol);
		awKeys.Add( _ttoi(value) );
	}
	
	return DelData(awKeys);
}

BOOL CCMDTendonTemplateGrid::ModifyToDB(ROWCOL nRow)
{
	ROWCOL nCol;
	VERIFY(GetColumnNoFor(0, nCol));
	
	int key, oldKey;
	T_TDNA_TEMPLATE_BASE_GRID data;
	
	oldKey = _ttoi(GetValueRowColBeforeEdit(nRow, nCol));
	GetValue(nRow, key, data);
	
	// 수정한다. 실패하면 에러 리턴
	return ModifyData(oldKey, key, data); 
}

BOOL CCMDTendonTemplateGrid::ModifyToDB(CStringArray& raOldKey, 
												 CArray<void*, void*>& raKey, 
												 CArray<void*, void*>& raData)
{
	ASSERT(raKey.GetSize() == raData.GetSize());
	ASSERT(raKey.GetSize() == raOldKey.GetSize());

	CArray<int, int> aOldKey, aKey;
	CArray<T_TDNA_TEMPLATE_BASE_GRID, T_TDNA_TEMPLATE_BASE_GRID&> aData;

	int Key;
	int nSize = raKey.GetSize();
	for (int i = 0; i < nSize; i++)
	{
		Key = _ttoi(raOldKey.GetAt(i));
		aOldKey.Add(Key);
		aKey.Add( *((int*)raKey.GetAt(i)) );
		aData.Add( *((T_TDNA_TEMPLATE_BASE_GRID*)raData.GetAt(i)) );
	}
	return ModifyData(aOldKey, aKey, aData);
}


CString CCMDTendonTemplateGrid::GetNewKey()
{
	ASSERT(0);
	CString value(_T("0"));
	return value;
}

CString CCMDTendonTemplateGrid::GetDefValue(int nIndex)
{
	return m_aCurDefVal[nIndex];
}

CString* CCMDTendonTemplateGrid::GetDefValueArray()
{
	return m_aCurDefVal;
}

BOOL CCMDTendonTemplateGrid::MakeKeyAndDataArray(CStringArray& value, CRowColArray& awCols, 
																	CArray<void*, void*>& aKey, CArray<void*, void*>& aData)
{
	int* pKey;
	T_TDNA_TEMPLATE_BASE_GRID* pData;
	pKey = new int;
	pData = new T_TDNA_TEMPLATE_BASE_GRID;
	aKey.Add((void*)pKey);
	aData.Add((void*)pData);

	if (!ValidateAndMakeRecord(value, awCols, *pKey, *pData)) return FALSE;
	return TRUE;
}

void CCMDTendonTemplateGrid::DeleteKeyAndDataArray(CArray<void*, void*>& aKey, CArray<void*, void*>& aData)
{
	int* pKey;
	T_TDNA_TEMPLATE_BASE_GRID* pData;
	ASSERT(aKey.GetSize() == aData.GetSize());
	for (int i = 0; i < aKey.GetSize(); i++)
	{
		pKey = (int*)aKey.GetAt(i);
		pData = (T_TDNA_TEMPLATE_BASE_GRID*)aData.GetAt(i);
		delete pKey;
		delete pData;
	}
}

long CCMDTendonTemplateGrid::GetCountData()
{
	int nCount;
	if (m_nInputType == TNDN_TEMPALTE_TYPE_XYXZ_XZ)
		nCount = (int)m_pData->tndnSubXZ.aSpanInfo.size() + 1;
	else if (m_nInputType == TNDN_TEMPALTE_TYPE_XYXZ_XY)
		nCount = (int)m_pData->tndnSubXY.aSpanInfo.size() + 1;
	else ASSERT(0);
	return nCount; 
}

void CCMDTendonTemplateGrid::MakeItemFiltered(long nFilteredNum, long* aFilteredKeys, CRowColArray& awCols)
{
	ASSERT(0); // no filtering
}

void CCMDTendonTemplateGrid::MakeItemNormal(long nNumData, CRowColArray& awCols)
{
 	int Key;
 	long nCount;
 	if (m_nInputType == TNDN_TEMPALTE_TYPE_XYXZ_XZ)
 	{
		TndnTemplateSubInfo Data;
		T_TDNA_TEMPLATE_BASE_GRID GridData;
		for(nCount = 0 ; nCount < nNumData; nCount++)
		{
			Key = nCount;//m_pData->tndnSubXZ.aSpanInfo[]
			if(nCount == 0)
				continue;
			Data = m_pData->tndnSubXZ.aSpanInfo[nCount - 1];
			
			// Grid Data Setting
			Base2GridBase(Data, GridData);
			GridData.spanNum = nCount;
			SetValue(GetParam()->GetData(), nCount+1, Key, GridData, awCols);
		} // end of loop nCount
	}
	else if (m_nInputType == TNDN_TEMPALTE_TYPE_XYXZ_XY)
	{
		TndnTemplateSubInfo Data;
		T_TDNA_TEMPLATE_BASE_GRID GridData;
		for(nCount = 0 ; nCount < nNumData; nCount++)
		{
			Key = nCount;//m_pData->tndnSubXZ.aSpanInfo[]
			if(nCount == 0)
				continue;
			Data = m_pData->tndnSubXY.aSpanInfo[nCount - 1];
			
			// Grid Data Setting
			Base2GridBase(Data, GridData);
			GridData.spanNum = nCount;
			SetValue(GetParam()->GetData(), nCount+1, Key, GridData, awCols);
		} // end of loop nCount
	}
	HideRows(nNumData+1, nNumData+1);
	SetHeaderTitle_2();
}

void CCMDTendonTemplateGrid::MakeItemEditingRecord(ROWCOL ncRow, CStringArray& EditingRecord)
{
	int Key;
	T_TDNA_TEMPLATE_BASE_GRID Data;

	for (int i = 0; i < GetColCount(); i++)
	{
		if (i == 0) Key = _ttoi(EditingRecord[i]);
		else ConvStrToData(i+1, EditingRecord[i], Data);
	}
	// 현재 값을 설정한다.
	SetValue(ncRow, Key, Data);
}

/////////////////////////////////////////////////////////////////////////////
// CCMDTendonTemplateGrid Implementation
BOOL CCMDTendonTemplateGrid::GetValue(ROWCOL nRow, int &key, T_TDNA_TEMPLATE_BASE_GRID &data)
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
		
		if(i==0)
		{
			key = _ttoi(str);
			data.spanNum = key;
		}
		else
			ConvStrToData(i+1, str, data);
	}
	return TRUE;
}

BOOL CCMDTendonTemplateGrid::SetValue(ROWCOL nRow, const int &key, const T_TDNA_TEMPLATE_BASE_GRID &data)
{
	CRowColArray awCols;
	awCols.SetSize(GetColCount());
	for (int i = 0;i < GetColCount(); i++)
		VERIFY(GetColumnNoFor(m_aTitle[i], awCols[i]));
	return SetValue(GetParam()->GetData(), nRow, key, data, awCols);
}

BOOL CCMDTendonTemplateGrid::SetValue(CGXData* pData, ROWCOL nRow, const int &key, const T_TDNA_TEMPLATE_BASE_GRID& data, CRowColArray &awCols)
{
	CString str;
	for (int i = 0; i < GetColCount(); i++)
	{
		if(i==0)	str.Format(KEY_FMT,key);
		else		ConvDataToStr(i+1, data, str);

		pData->StoreValueRowCol(nRow, awCols[i], str, gxOverride, GX_VT_NUMERIC);
	}

	//ChangeGridColEnable(nRow);

	return TRUE;
}

void CCMDTendonTemplateGrid::ConvStrToData(int col, CString& value, T_TDNA_TEMPLATE_BASE_GRID &data)
{
	if (col == D_TDNCOL_SPAN)
	{
		data.spanNum = _ttoi(value);
	}
	else if(m_nSubType == TNDN_TEMPLATE_TYPE_STRAIGHT || m_nSubType == TNDN_TEMPLATE_TYPE_CURVE_LINE || m_nSubType == TNDN_TEMPLATE_TYPE_CURVE_ST)
	{
			switch (col)
			{
			case D_TDNCOL_BT: data.dDt  = _tstof(value); break;
			case D_TDNCOL_ET: data.dDt1 = _tstof(value); break;
			}
	}
	else if(m_nSubType == TNDN_TEMPLATE_TYPE_CURVE_BENT_1 || m_nSubType == TNDN_TEMPLATE_TYPE_STRAIGHT_BENT_1)
	{
			switch (col)
			{
			case D_TDNCOL_BT: data.dDt  = _tstof(value); break;
			case D_TDNCOL_MH: data.dDf  = _tstof(value); break;
			case D_TDNCOL_MB: data.dDb  = _tstof(value); break;
			case D_TDNCOL_ET: data.dDt1 = _tstof(value); break;
			}
	}
	else if(m_nSubType == TNDN_TEMPLATE_TYPE_STRAIGHT_BENT_2)
	{
			switch (col)
			{
			case D_TDNCOL_BT:  data.dDt = _tstof(value); break;
			case D_TDNCOL_MH1: data.dDf = _tstof(value); break;
			case D_TDNCOL_MB1: data.dDb = _tstof(value); break;
			case D_TDNCOL_MR1: data.dR1 = _tstof(value); break;
			case D_TDNCOL_MH2: data.dDe = _tstof(value); break;
			case D_TDNCOL_MB2: data.dDb1= _tstof(value); break;
			case D_TDNCOL_MR2: data.dR2 = _tstof(value); break;
			case D_TDNCOL_ET:  data.dDt1= _tstof(value); break;
			}
	}
	else if(m_nSubType == TNDN_TEMPLATE_TYPE_CURVE_SIMPLE_1)
	{
			switch (col)
			{
			case D_TDNCOL_BB:  data.dDb  = _tstof(value); break;
			case D_TDNCOL_MH1: data.dDf  = _tstof(value); break;
			case D_TDNCOL_MB1: data.dDb1 = _tstof(value); break;
			case D_TDNCOL_ET:  data.dDt1 = _tstof(value); break;
			}
		
	}
	else if(m_nSubType == TNDN_TEMPLATE_TYPE_CURVE_SIMPLE_2)
	{
			switch (col)
			{
			case D_TDNCOL_BT:  data.dDt = _tstof(value); break;
			case D_TDNCOL_MH1: data.dDf = _tstof(value); break;
			case D_TDNCOL_MB1: data.dDb = _tstof(value); break;
			case D_TDNCOL_EB:  data.dDb1= _tstof(value); break;
			}
	}
	else
	{
		ASSERT(0);
	}
}

void CCMDTendonTemplateGrid::ConvDataToStr(int col, const T_TDNA_TEMPLATE_BASE_GRID &data, CString &value)
{
	value.Format(_T("0"));

	if(col == D_TDNCOL_SPAN)
	{
		value.Format(KEY_FMT, data.spanNum);
	}
	else if(m_nSubType == TNDN_TEMPLATE_TYPE_STRAIGHT || m_nSubType == TNDN_TEMPLATE_TYPE_CURVE_LINE || m_nSubType == TNDN_TEMPLATE_TYPE_CURVE_ST)
	{
			switch (col)
			{
			case D_TDNCOL_BT: value.Format(DATA_FMT, data.dDt);  break;
			case D_TDNCOL_ET: value.Format(DATA_FMT, data.dDt1); break;
			}
		
	}
	else if(m_nSubType == TNDN_TEMPLATE_TYPE_CURVE_BENT_1 || m_nSubType == TNDN_TEMPLATE_TYPE_STRAIGHT_BENT_1)
	{
			switch (col)
			{
			case D_TDNCOL_BT: value.Format(DATA_FMT, data.dDt);  break;
			case D_TDNCOL_MH: value.Format(DATA_FMT, data.dDf);  break;
			case D_TDNCOL_MB: value.Format(DATA_FMT, data.dDb);  break;
			case D_TDNCOL_ET: value.Format(DATA_FMT, data.dDt1); break;
			}
	}
	else if(m_nSubType == TNDN_TEMPLATE_TYPE_STRAIGHT_BENT_2)
	{
			switch (col)
			{
			case D_TDNCOL_BT:  value.Format(DATA_FMT, data.dDt);  break;
			case D_TDNCOL_MH1: value.Format(DATA_FMT, data.dDf);  break;
			case D_TDNCOL_MB1: value.Format(DATA_FMT, data.dDb);  break;
			case D_TDNCOL_MR1: value.Format(DATA_FMT, data.dR1);  break;
			case D_TDNCOL_MH2: value.Format(DATA_FMT, data.dDe);  break;
			case D_TDNCOL_MB2: value.Format(DATA_FMT, data.dDb1); break;
			case D_TDNCOL_MR2: value.Format(DATA_FMT, data.dR2);  break;
			case D_TDNCOL_ET:  value.Format(DATA_FMT, data.dDt1); break;
			}
		
	}
	else if(m_nSubType == TNDN_TEMPLATE_TYPE_CURVE_SIMPLE_1)
	{
			switch (col)
			{
			case D_TDNCOL_BB:  value.Format(DATA_FMT, data.dDb);  break;
			case D_TDNCOL_MH1: value.Format(DATA_FMT, data.dDf);  break;
			case D_TDNCOL_MB1: value.Format(DATA_FMT, data.dDb1); break;
			case D_TDNCOL_ET:  value.Format(DATA_FMT, data.dDt1); break;
			}
	}
	else if(m_nSubType == TNDN_TEMPLATE_TYPE_CURVE_SIMPLE_2)
	{
			switch (col)
			{
			case D_TDNCOL_BT:  value.Format(DATA_FMT, data.dDt);  break;
			case D_TDNCOL_MH1: value.Format(DATA_FMT, data.dDf);  break;
			case D_TDNCOL_MB1: value.Format(DATA_FMT, data.dDb);  break;
			case D_TDNCOL_EB:  value.Format(DATA_FMT, data.dDb1); break;
			}
	}
	else
	{
		ASSERT(0);
	}
}

void CCMDTendonTemplateGrid::MakeSearchKey(const int &key, const T_TDNA_TEMPLATE_BASE_GRID& data, CStringArray &aKey)
{
	int nColID;
	int nSortKeyNum = m_aSortInfo.GetSize();
	
	aKey.SetSize(nSortKeyNum);
	for (int i = 0; i < nSortKeyNum; i++)
	{
		nColID = m_aSortInfo[i].nRC;

		if		(nColID == 0) aKey[i].Format(KEY_FMT, key);
		else	ConvDataToStr(nColID+1, data, aKey[i]);
	}
}

void CCMDTendonTemplateGrid::SetHeaderTitle_2()
{
	CString csUnit, csTitle;
	
	for (int i = 0; i < GetColCount(); i++)
	{
		CUnitCtrl::GetUnitSystem(m_aUnit[i], csUnit);
		
		SetStyleRange(CGXRange(1, i+1), CGXStyle()
			.SetControl(GX_IDS_CTRL_HEADER)
			.SetValue(Get_HeaderRealAr()[i]));
	} 
}

void CCMDTendonTemplateGrid::SetHeaderTitle(T_UNIT_SYSTEM &us, BOOL bInit)
{
	CString csUnit, csTemp;
	
	SetFrozenRows(1, 1);
	
	CUnitCtrl::GetUnitSystem(D_UNITSYS_BASE_LENGTH, csUnit);
	csTemp = _T("(")+csUnit+_T(")");
	Get_HeaderAr()[1] = csTemp;

	for (int i = 0; i < GetColCount(); i++)
	{
		SetStyleRange(CGXRange(0, i+1), CGXStyle()
			.SetValue(Get_HeaderAr()[i])
			.SetUserAttribute(ID_USER_ATTR_COL_ID, Get_HeaderPKAr()[i])
			);

		SetStyleRange(CGXRange(1, i+1), CGXStyle()
			.SetValue(Get_HeaderRealAr()[i])
			);

		if (bInit)
			SetColWidthByDPI(i+1, i+1, 48);
	}
	if (bInit)
	{
		SetColWidthByDPI(0, 0, 20);
		SetColWidthByDPI(D_TDNCOL_SPAN, D_TDNCOL_SPAN, 44); //스팬의 너비.
		SetColWidthByDPI(D_TDNCOL_MH1, D_TDNCOL_MB1, 42);
		SetColWidthByDPI(D_TDNCOL_MR1, D_TDNCOL_MR1, 32);
		SetColWidthByDPI(D_TDNCOL_MH2, D_TDNCOL_MB2, 42);
		SetColWidthByDPI(D_TDNCOL_MR2, D_TDNCOL_MR2, 32);
	}
	
	SetCoveredCellsRowCol(0, D_TDNCOL_BT, 0, D_TDNCOL_BB);
	SetCoveredCellsRowCol(0, D_TDNCOL_MH, 0, D_TDNCOL_MR2);
	SetCoveredCellsRowCol(0, D_TDNCOL_ET, 0, D_TDNCOL_EB);
}

BOOL CCMDTendonTemplateGrid::FindData(int key, int& nPos, int nStart)
{
	int i;
	int nSize;
	if (m_nInputType == TNDN_TEMPALTE_TYPE_XYXZ_XZ)		nSize = (int)m_pData->tndnSubXZ.aSpanInfo.size();
	else if (m_nInputType == TNDN_TEMPALTE_TYPE_XYXZ_XY) nSize = (int)m_pData->tndnSubXY.aSpanInfo.size();
	else ASSERT(0);
	
	CString csVal;
	int dblVal;

	for (i = nStart; i < nSize; i++)
	{
		if (m_nInputType == TNDN_TEMPALTE_TYPE_XYXZ_XZ)		csVal.Format(KEY_FMT, i);
		else if (m_nInputType == TNDN_TEMPALTE_TYPE_XYXZ_XY) csVal.Format(KEY_FMT, i);
		else ASSERT(0);

		dblVal = _ttoi(csVal);

		if(key - 1 == dblVal)
		{
			nPos = i;
			return TRUE;
		}
	}
	nPos = i;
	return FALSE;
}

BOOL CCMDTendonTemplateGrid::ExistAllKey(CArray<int, int>& aKey, int& errorKey)
{
	CArray<int, int> aTempKey;
	
	int nSize = aKey.GetSize();
	aTempKey.Copy(aKey);
	BOOL bSame;
	int nPos = 0;
	int nStart;
	
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

BOOL CCMDTendonTemplateGrid::AddData(int key, TndnTemplateSubInfo data)
{
	return TRUE;
	T_TDNA_TEMPLATE_BASE_GRID target;
	Base2GridBase(data, target);
	return AddData(key, target);
}

BOOL CCMDTendonTemplateGrid::AddData(int key, T_TDNA_TEMPLATE_BASE_GRID data)
{
	return TRUE;
	CArray<int, int> aOldKey, aKey;
	CArray<T_TDNA_TEMPLATE_BASE_GRID, T_TDNA_TEMPLATE_BASE_GRID&> aData;
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
	
	TndnTemplateSubInfo item;
	GridBase2Base(data, item);

	if (m_nInputType == TNDN_TEMPALTE_TYPE_XYXZ_XZ)  // xz
	{
		m_pData->tndnSubXZ.aSpanInfo[key] = item;
	}
	else if( m_nInputType == TNDN_TEMPALTE_TYPE_XYXZ_XY ) // xy
	{
		m_pData->tndnSubXY.aSpanInfo[key] = item;
	}
	else
	{
		ASSERT(0);
	}
	
	UpdateBuffer(0, aOldKey, aKey, aData);
	DBChanged();
	return TRUE;
}

BOOL CCMDTendonTemplateGrid::AddData(CArray<int, int> &aKey, CArray<T_TDNA_TEMPLATE_BASE_GRID, T_TDNA_TEMPLATE_BASE_GRID&> &aData, BOOL bNotify)
{
	return TRUE;
	CArray<int, int> aOldKey;
	
	// 동일한 key값 중복 검토
	int nSize = aKey.GetSize();
	aOldKey.Copy(aKey);

	qsort((void*)aOldKey.GetData(), nSize, sizeof(int), CCompFunc::intAsc);
	
	BOOL bSame;
	int nPos = 0;
	int nStart;
	
	for (int i = 0; i < nSize; i++)
	{
		nStart = nPos;
		bSame = FindData(aOldKey[i], nPos, nStart);
		if (!bSame && i > 0)
			bSame = aOldKey[i] == aOldKey[i-1];
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
		
		if (m_nInputType == TNDN_TEMPALTE_TYPE_XYXZ_XZ) 
		{
			TndnTemplateSubInfo item;
			GridBase2Base(aData[i], item);
			m_pData->tndnSubXZ.aSpanInfo[aKey[i]] = item;
		}
		else if(m_nInputType == TNDN_TEMPALTE_TYPE_XYXZ_XY)
		{
			TndnTemplateSubInfo item;
			GridBase2Base(aData[i], item);
			m_pData->tndnSubXY.aSpanInfo[aKey[i]] = item;
		}
		else
		{
			ASSERT(0);
		}
	}
	UpdateBuffer(0, aOldKey, aKey, aData);
	
	if (bNotify) DBChanged();
	return TRUE;
}

BOOL CCMDTendonTemplateGrid::DelData(int key)
{
	return TRUE;
	ASSERT(0);
// 	CArray<int, int> aOldKey, aKey;
// 	CArray<T_TDNA_TEMPLATE_BASE_GRID, T_TDNA_TEMPLATE_BASE_GRID&> aData;
// 	T_TDNA_TEMPLATE_BASE_GRID data;
// 
// 	aKey.Add(key);
// 	aData.Add(data);
// 	
// 	int nPos;
// 	if (!FindData(key, nPos))
// 	{
// 		CString msg;
// 		msg.Format(_LS(IDS_CMD0417__x__g_does_not_exist), key);
// 		AfxMessageBox(msg);
// 		return FALSE;
// 	}
// 
// 	if (m_nInputType == 0)		m_pData->tndnSubXZ.aSpanInfo. aProfile.RemoveAt(nPos);
// 	else if (m_nInputType == 2) m_pData->aProfileY.RemoveAt(nPos);
// 	else if (m_nInputType == 3) m_pData->aProfileZ.RemoveAt(nPos);
// 	else ASSERT(0);
// 	UpdateBuffer(1, aOldKey, aKey, aData);
// 	DBChanged();
//	return TRUE;
}

BOOL CCMDTendonTemplateGrid::DelData(CArray<int, int> &aKey, BOOL bNotify)
{
	return TRUE;
	ASSERT(0);
//   CArray<double, double> aOldKey;
//   CArray<T_TDNA_TEMPLATE_BASE_GRID, T_TDNA_TEMPLATE_BASE_GRID&> aData;
//   T_TDNA_TEMPLATE_BASE_GRID data;
//   int nSize = aKey.GetSize();
//   for (int i = 0; i < nSize; i++) aData.Add(data);
// 
//   aOldKey.Copy(aKey);
//   qsort((void*)aOldKey.GetData(), nSize, sizeof(double), CCompFunc::doubleAsc);
//   // 동일한 key값 중복 검토
//   double errKey;
//   if (!ExistAllKey(aOldKey, errKey))
//   {
//     CString msg;
//     msg.Format(_LS(IDS_CMD0417__x__g_does_not_exist), errKey);
//     AfxMessageBox(msg);
//     return FALSE;
//   }
// 
//   int nSize2;
//   if (m_nInputType == 1) nSize2 = m_pData->aProfile.GetSize();
//   else if (m_nInputType == 2) nSize2 = m_pData->aProfileY.GetSize();
//   else if (m_nInputType == 3) nSize2 = m_pData->aProfileZ.GetSize();
//   else ASSERT(0);
//   int j = nSize2-1;
//   double dblDiff;
//   int nDeleted = 0;
//   for (i = nSize-1; i >= 0; i--)
//   {
//     for (; j >= 0; j--)
//     {
//       if (m_nInputType == 1) dblDiff = m_pData->aProfile[j].x;
//       else if (m_nInputType == 2) dblDiff = m_pData->aProfileY[j].x;
//       else if (m_nInputType == 3) dblDiff = m_pData->aProfileZ[j].x;
//       else ASSERT(0);
//       if (CCompFunc::CompReal(aOldKey[i], dblDiff) == 0)
//       {
//         if (m_nInputType == 1) m_pData->aProfile.RemoveAt(j);
//         else if (m_nInputType == 2) m_pData->aProfileY.RemoveAt(j);
//         else if (m_nInputType == 3) m_pData->aProfileZ.RemoveAt(j);
//         else ASSERT(0);
//         j--;
//         nDeleted++;
//         break;
//       }
//     }
//   }
// 
//   UpdateBuffer(1, aOldKey, aKey, aData);
// 
//   if (bNotify) DBChanged();
	return TRUE;
}

BOOL CCMDTendonTemplateGrid::ModifyData(int oldKey, int key, T_TDNA_TEMPLATE_BASE_GRID data)
{
	CArray<int, int> aOldKey, aKey;
	CArray<T_TDNA_TEMPLATE_BASE_GRID, T_TDNA_TEMPLATE_BASE_GRID&> aData;
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
		if (m_nInputType == TNDN_TEMPALTE_TYPE_XYXZ_XZ)
		{
			TndnTemplateSubInfo item;
			GridBase2Base(data, item);
			m_pData->tndnSubXZ.aSpanInfo[key-1] = item;
		}
		else if (m_nInputType == TNDN_TEMPALTE_TYPE_XYXZ_XY)
		{
			TndnTemplateSubInfo item;
			GridBase2Base(data, item);
			m_pData->tndnSubXY.aSpanInfo[key-1] = item;
		}
		else 
		{
			ASSERT(0);
		}
	}
	else
	{
		ASSERT(0);
		//key값.. 스팬 위치 정보는 바꿀수 없도록 한다.
	}
	
	UpdateBuffer(2, aOldKey, aKey, aData);
	
	DBChanged();
	return TRUE;
}

BOOL CCMDTendonTemplateGrid::ModifyData(CArray<int, int> &aOldKey, CArray<int, int> &aKey, CArray<T_TDNA_TEMPLATE_BASE_GRID, T_TDNA_TEMPLATE_BASE_GRID&> &aData)
{
	return TRUE;
	ASSERT(0);
	CArray<TndnTemplateSubInfo, TndnTemplateSubInfo&> aOrgData;
	if(m_nInputType == TNDN_TEMPALTE_TYPE_XYXZ_XZ)
	{
		for(int i = 0 ; i < m_pData->tndnSubXZ.aSpanInfo.size(); i++)
		{
			aOrgData.Add(m_pData->tndnSubXZ.aSpanInfo[i]);
		}
	}
	if(m_nInputType == TNDN_TEMPALTE_TYPE_XYXZ_XY)
	{
		for(int i = 0 ; i < m_pData->tndnSubXY.aSpanInfo.size(); i++)
		{
			aOrgData.Add(m_pData->tndnSubXY.aSpanInfo[i]);
		}
	}
	
	if (!DelData(aOldKey, FALSE))
		return FALSE;

	if (!AddData(aKey, aData, FALSE))
	{
// 		if (m_nInputType == 0) m_pData->aProfile.Copy(aOrgData);
// 		else if (m_nInputType == 2) ;
// 		else if (m_nInputType == 3) ;
// 		else ASSERT(0);



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

void CCMDTendonTemplateGrid::UpdateBuffer(int nCmd, CArray<int, int> &aOldKey, CArray<int, int> &aKey, CArray<T_TDNA_TEMPLATE_BASE_GRID, T_TDNA_TEMPLATE_BASE_GRID&> &aData)
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
		case (0) :
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
		if (GetRowCount() < ncRow)
		{
			ncRow = GetRowCount();
		}
		BOOL bSet = SetCurrentCell(ncRow, ncCol);
		if (!bSet)
		{
			bSet = SetRightValidCell(ncRow, ncCol);
		}
		if (!bSet)
		{
			bSet = SetLeftValidCell(ncRow, ncCol);
		}
		m_nEditMode = nOldMode;
	}

	LockUpdate(bOldLock);
	if (!bOldLock) Redraw();
}

void CCMDTendonTemplateGrid::OnUnitChange()
{
	T_UNIT_SYSTEM us;
	SetHeaderTitle(us, FALSE);
	MakeItemEx(0, -2, TRUE);
}

void CCMDTendonTemplateGrid::DBChanged()
{
	CCMDTendonTemplateAddModDlg* pDlg = (CCMDTendonTemplateAddModDlg*)GetParent();

	if (!pDlg)								return;
	if (pDlg->GetSafeHwnd() == 0)			return;
	if (!IsWindow(pDlg->GetSafeHwnd()))		return;

	pDlg->RemakeGridDraw(m_nInputType == TNDN_TEMPALTE_TYPE_XYXZ_XY, m_nInputType == TNDN_TEMPALTE_TYPE_XYXZ_XZ);
}

/////////////////////////////////////////////////////////////////////////
// 여기부터 Column 값 변경이 다른 column의 상태나 값을 변경시키는 경우를
// 처리해 주기 위해서 Override 및 새로 정의되는 함수이다.
// OnStartEditing, OnCanceledEditing, ChangeColumnStatus
// 일반적으로 OnModifyCell이 사용되나 Check Box Type일 경우는 OnStartEditing이
// 사용된다.

BOOL CCMDTendonTemplateGrid::OnStartEditing(ROWCOL nRow, ROWCOL nCol)
{
	// 부모의 OnModifyCell을 항상 먼저 호출해 주어야 한다.
	BOOL bResult = CTBCommon::OnStartEditing(nRow, nCol);
	ChangeColumnStatus(nRow, nCol);
	return bResult;
}

void CCMDTendonTemplateGrid::OnCanceledEditing(ROWCOL nRow, ROWCOL nCol)
{
	// 부모의 OnCanceledEditing을 항상 먼저 호출해 주어야 한다.
	CTBCommon::OnCanceledEditing(nRow, nCol);
	if (!IsEdit()) return;
	ChangeColumnStatus(nRow, nCol);
}

void CCMDTendonTemplateGrid::ChangeColumnStatus(ROWCOL nRow, ROWCOL nCol)
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

	bFlag = _ttoi(csFlag);
}

/* 사용하지 않고 있음, 헷갈려서 주석처리 (20170410 KJH)
void CCMDTendonTemplateGrid::ChangeGridColEnable(ROWCOL nRow)
{
	ASSERT(0);
	//Use Same Info일때 이부분을 활용해 보자.
	BOOL bEnable = TRUE;
	CString strValue;
	strValue = GetValueRowCol(nRow, D_TDNCOL_MB1);//8);

	if (strValue.CompareNoCase(D_OPT_NONE) == 0)
		bEnable = FALSE;

	SetStyleRange(CGXRange(nRow, D_TDNCOL_MH2), CGXStyle().SetEnabled(bEnable)); // 9
	SetStyleRange(CGXRange(nRow, D_TDNCOL_MB2), CGXStyle().SetEnabled(bEnable)); // 10
	SetStyleRange(CGXRange(nRow, D_TDNCOL_ET),  CGXStyle().SetEnabled(bEnable)); // 11

	double dValue = _tstof(GetValueRowCol(nRow, D_TDNCOL_MH1));//7));
	bEnable = TRUE;     //임시 변경 2005. 12. 21 jkpark
	SetStyleRange(CGXRange(nRow, D_TDNCOL_EB), CGXStyle().SetEnabled(bEnable)); // 12
}
*/

void CCMDTendonTemplateGrid::OnModifyCell(ROWCOL nRow, ROWCOL nCol)
{
//	int nRowCount = GetRowCount();
//
//	if(m_nInputType == TNDN_TEMPALTE_TYPE_XYXZ_XY && m_pData->tndnSubXY.bSameSpanInfo)
//		nRowCount = 3;
//	else if(m_nInputType == TNDN_TEMPALTE_TYPE_XYXZ_XZ && m_pData->tndnSubXZ.bSameSpanInfo)
//		nRowCount = 3;
//
//	if(nRowCount > nRow)
//	{
		CTBCommon::OnModifyCell(nRow, nCol);
	//}
	//if (nCol == 7||nCol == 8) ChangeGridColEnable(nRow);
}

void CCMDTendonTemplateGrid::GridBase2Base(T_TDNA_TEMPLATE_BASE_GRID source, TndnTemplateSubInfo& target)
{
	target.dDb		= source.dDb;
	target.dDb1		= source.dDb1;
	target.dDe		= source.dDe;
	target.dDf		= source.dDf;
	target.dDRad	= source.dDRad;
	target.dDt		= source.dDt;
	target.dDt1		= source.dDt1;
		target.dR1      = source.dR1;
		target.dR2      = source.dR2;
}

void CCMDTendonTemplateGrid::Base2GridBase(TndnTemplateSubInfo source, T_TDNA_TEMPLATE_BASE_GRID& target)
{
	target.dDb		= source.dDb;
	target.dDb1		= source.dDb1;
	target.dDe		= source.dDe;
	target.dDf		= source.dDf;
	target.dDRad	= source.dDRad;
	target.dDt		= source.dDt;
	target.dDt1		= source.dDt1;
		target.dR1		= source.dR1;
		target.dR2		= source.dR2;
	target.spanNum	= 0 ;
}

BOOL CCMDTendonTemplateGrid::OnInsertEmptyRecord(ROWCOL nRow)
{
	return TRUE;
	BOOL bReturn = FALSE;
		bReturn = CTBCommon::OnInsertEmptyRecord(nRow);
// 
//   if(!(nRow > 3 && CURVTY_PARAB == m_nCurveType))
//   {
//     bReturn = CTBCommon::OnInsertEmptyRecord(nRow);
//   }
// 
//   if (bReturn)
//     ChangeGridColEnable(nRow);

	return bReturn;
}

BOOL CCMDTendonTemplateGrid::OnDeleteRecord(ROWCOL nRow)
{
	return CTBCommon::OnDeleteRecord(nRow);
}

void CCMDTendonTemplateGrid::SetTemplateTypeSameSpan( BOOL bIsXY, BOOL bUseSameSpanInfo )
{
	if(!m_pData)
		return;
	int nRowCount = GetRowCount();
	if(nRowCount < 4)
		return;
	
	TndnTemplateSub* pSub;

	//BOOL bOldLock = LockUpdate(TRUE);

	if(bIsXY)   pSub = &(m_pData->tndnSubXY);
	else        pSub = &(m_pData->tndnSubXZ);
	
	if(bUseSameSpanInfo)
	{
		HideRows(3, nRowCount);
	}
	else
	{
		int i;

		for(i = 3 ; i < nRowCount; i++)
		{
			CString strTemp = GetValueRowCol(i - 1, D_TDNCOL_ET);
			SetStyleRange(CGXRange(i, D_TDNCOL_BT, nRowCount, D_TDNCOL_BT), CGXStyle().SetValue(strTemp).SetEnabled(FALSE));
		}
	}

//   LockUpdate(bOldLock);
//   if (!bOldLock) Redraw();
}

void CCMDTendonTemplateGrid::SetTemplateType( TNDN_TEMPLATE_TYPE nType, BOOL bSymmetry /*= FALSE*/ )
{
		BOOL bOldLock = LockUpdate(TRUE);

		ROWCOL nColCount = GetColCount();

		HideColsByIntend(0, nColCount, FALSE);  // 일단 모두 보이게.  
		HideColsByIntend(D_TDNCOL_KEY, D_TDNCOL_KEY, TRUE);           // key 는 보이지 않게.

		m_nSubType = nType;

		switch (m_nSubType)
		{
		case TNDN_TEMPLATE_TYPE_CURVE_LINE:
		case TNDN_TEMPLATE_TYPE_STRAIGHT:
		case TNDN_TEMPLATE_TYPE_CURVE_ST:
				{
						HideColsByIntend(D_TDNCOL_BB, D_TDNCOL_MR2, TRUE);
						HideColsByIntend(D_TDNCOL_EB, nColCount, TRUE);
				}
				break;
		case TNDN_TEMPLATE_TYPE_STRAIGHT_BENT_1:
		case TNDN_TEMPLATE_TYPE_CURVE_BENT_1:
				{
						HideColsByIntend(D_TDNCOL_BB,  D_TDNCOL_BB, TRUE);
						HideColsByIntend(D_TDNCOL_MH1, D_TDNCOL_MR2, TRUE);
						HideColsByIntend(D_TDNCOL_EB,  nColCount, TRUE);
				}
				break;
		case TNDN_TEMPLATE_TYPE_STRAIGHT_BENT_2:
				{
						HideColsByIntend(D_TDNCOL_BB, D_TDNCOL_MB, TRUE);
						HideColsByIntend(D_TDNCOL_EB, nColCount, TRUE);
				}
				break;
		case TNDN_TEMPLATE_TYPE_CURVE_SIMPLE_1:
				{
						HideColsByIntend(D_TDNCOL_BT,  D_TDNCOL_BT,  TRUE);
						HideColsByIntend(D_TDNCOL_MH,  D_TDNCOL_MB,  TRUE);
						HideColsByIntend(D_TDNCOL_MR1, D_TDNCOL_MR2, TRUE);
						HideColsByIntend(D_TDNCOL_EB, nColCount, TRUE);
				}
				break;
		case TNDN_TEMPLATE_TYPE_CURVE_SIMPLE_2:
				{
						HideColsByIntend(D_TDNCOL_BB,  D_TDNCOL_MB, TRUE);
						HideColsByIntend(D_TDNCOL_MR1, D_TDNCOL_ET, TRUE);
				}
				break;
		default:
				ASSERT(0);
				break;
		}
	
		// span 열은 사용 불가.
		SetStyleRange(CGXRange().SetCols(D_TDNCOL_SPAN), CGXStyle()
				.SetEnabled(FALSE)
				.SetPlaces(0));

		LockUpdate(bOldLock);
		if (!bOldLock) Redraw();
}

void CCMDTendonTemplateGrid::OnLButtonDown( UINT nFlags, CPoint point )
{
	ROWCOL nRow;
	ROWCOL nCol;
	HitTest(point, &nRow, &nCol);
	if(nRow == 0 && nCol == 0)
		return;

	CTBCommon::OnLButtonDown(nFlags, point);
	OnTerminate(D_TB_SAVE_ALWAYS);
}

void CCMDTendonTemplateGrid::Redraw( UINT flags /*= GX_UPDATENOW*/, BOOL bCreateHint /*= TRUE*/ )
{
	CTBCommon::Redraw(flags, bCreateHint);
	
	int nRowCount = GetRowCount();
	int ncolCount = GetColCount();

	if(nRowCount >= 1 && ncolCount >= 1)
	{
//     SetStyleRange(CGXRange(1,1,nRowCount, ncolCount), CGXStyle()
//       .SetIncludeInterior(TRUE)
//       .SetIncludeNegativeStateColor(TRUE)
//       .SetIncludeTextColor(TRUE)
//       .SetNegativeStateColor(RGB(255,0,255))
//       .SetTextColor(RGB(255,255,0))
//       .SetInterior(RGB(255,0,0)), gxApplyNew
//       );
	}
}

BOOL CCMDTendonTemplateGrid::GetStyleRowCol( ROWCOL nRow, ROWCOL nCol, CGXStyle& style, GXModifyType mt /* = gxCopy */, int nType /* = 0 */ )
{
	BOOL bRetVal = CTBCommon::GetStyleRowCol(nRow, nCol, style, mt, nType);

	if(nType == 0)
	{
		if(nRow == 0 || nRow == 1)
		{
			style.SetInterior(RGB(239,241,244));
		}
	}

	return bRetVal;
}

BOOL CCMDTendonTemplateGrid::OnValidateField( ROWCOL nRow, ROWCOL nCol )
{
	int nRowCount = GetRowCount();

	//if(m_nInputType == TNDN_TEMPALTE_TYPE_XYXZ_XY && m_pData->tndnSubXY.bSameSpanInfo)
	//	nRowCount = 3;
	//else if(m_nInputType == TNDN_TEMPALTE_TYPE_XYXZ_XZ && m_pData->tndnSubXZ.bSameSpanInfo)
	//	nRowCount = 3;
	//
	//if(nRowCount > nRow)
	//{
		return CTBCommon::OnValidateField(nRow, nCol);
	//}
	//return TRUE;
}
