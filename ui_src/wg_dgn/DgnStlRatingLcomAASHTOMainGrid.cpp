// DgnStlRatingLcomAASHTOMainGrid.cpp: implementation of the CDgnStlRatingLcomAASHTOMainGrid class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "wg_dgn.h"
#include "DgnStlRatingLcomAASHTOMainGrid.h"

#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_LoadDB.h"
#include "..\wg_db\wg_db_LoadDgnCodeCheck.h"

#include "DgnStlRatingLcomAASHTODlg.h"
#include "DgnStlRatingLcomAASHTOSubGrid.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#define NAME_COL_ID   2
#define LCOM_KEY_FMT  _T("%d")

#define D_TYPE_DEF  D_LCOM_CT_ADD
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CString CDgnStlRatingLcomAASHTOMainGrid::m_aCurDefVal[] = {_T(""), _T(""), _T(""), _T(""), _T("")};


CDgnStlRatingLcomAASHTOMainGrid::CDgnStlRatingLcomAASHTOMainGrid()
{
	m_pDoc = CDBDoc::GetDocPoint();

	m_nLcomType = D_LCOMTYPE_GENERAL;
	m_pSubGrid = 0;

	m_aCurDefVal[4] = _LS(IDS_CMD_LCOM_ACTIVE);

	m_nColCount = 3;
	m_nColHeader = 0;
	m_nCurrentRow = 1;

	AddColInfo(_ULS(Key),                      t_integer, 0);
	AddColInfo(_LS(IDS_DGN_RATING_LOAD_TYPE),  t_string,  0);
	AddColInfo(_LS(IDS_DGN_RATING_MAX),        t_string,  0);
	AddColInfo(_LS(IDS_DGN_RATING_MIN),        t_integer, 0);
	
	m_aPrimaryKey.Add(0);
}

CDgnStlRatingLcomAASHTOMainGrid::~CDgnStlRatingLcomAASHTOMainGrid()
{
}

BEGIN_MESSAGE_MAP(CDgnStlRatingLcomAASHTOMainGrid, CTBCommon)
	//{{AFX_MSG_MAP(CDgnStlRatingLcomAASHTOMainGrid)
	ON_WM_RBUTTONDBLCLK()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONDBLCLK()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CDgnStlRatingLcomAASHTOMainGrid message handlers
void CDgnStlRatingLcomAASHTOMainGrid::Initialize(T_RTLD_D RateData[8], CDgnStlRatingLcomAASHTODlg* pParent)
{
	T_UNIT_SYSTEM UnitSystem;	
	CTBCommon::Initialize(m_pDoc, UnitSystem);

	m_pParent = pParent;

	for(int i=0 ; i<8 ; i++)
	{
	  RateLoad[i] = RateData[i];
	}

	LockUpdate(TRUE);

	// 컬럼 갯수 설정
	SetColCount(GetInitialColCount());
	SetHeaderTitle(UnitSystem, TRUE);

	//Set_Title();

	// 표준 스타일 변경
	ChangeStandardStyle(CGXStyle()
			.SetFont(CGXFont_GC().SetSize(8))
			.SetVerticalAlignment(DT_BOTTOM)
			.SetAllowEnter(FALSE));

	// 컬럼 스타일 변경
	// Load Type 의 스타일 변경
	//SetStyleRange(CGXRange(2,2,2,2), CGXStyle()
	SetStyleRange(CGXRange().SetCols(2), CGXStyle()
			.SetHorizontalAlignment(DT_RIGHT)
			.SetValueType(GX_VT_STRING)
			.SetFormat(GX_FMT_FIXED)
			.SetReadOnly(TRUE));
			//.SetEnabled(FALSE));

 // 컬럼 스타일 변경
	//max
	SetStyleRange(CGXRange().SetCols(3), CGXStyle()
			.SetVerticalAlignment(DT_VCENTER)
			.SetHorizontalAlignment(DT_RIGHT)
			.SetValueType(GX_VT_NUMERIC)
			.SetFormat(GX_FMT_FIXED)
			.SetPlaces(2)
			//.SetValue(_T("0"))
			.SetReadOnly(FALSE));
	//min
	SetStyleRange(CGXRange().SetCols(4), CGXStyle()
			.SetVerticalAlignment(DT_VCENTER)
			.SetHorizontalAlignment(DT_RIGHT)
			.SetValueType(GX_VT_NUMERIC)
			.SetFormat(GX_FMT_FIXED)
			.SetPlaces(2)
			//.SetValue(_T("0"))
			.SetReadOnly(FALSE));

	// 현재 셀의 border 설정 변경
	CGXProperties* pProp = GetParam()->GetProperties();
			pProp->SetUserProperty(GX_IDS_OUTLINECURRENTCELL,
		      (CGXStyle) pProp->sInvertNoBorder);

	// 현재 셀 설정
	SetRightValidCell(1, 0);
	GetParam()->SetHideCurrentCell(GX_HIDE_NEVER);

	// Row 이동을 가능하게 한다.
	GetParam()->EnableMoveRows(TRUE);

	// column의 이동을 금지시킨다.
	GetParam()->EnableMoveCols(FALSE);
	// row resize 금지
	GetParam()->EnableTrackRowHeight(FALSE); 
	
	// Insert key에 의한 삽입 금지
	EnableInsertRowByPressingKey(FALSE);

	// Column 크기 설정
	auto L_SetColWidth = [this] (ROWCOL nColSt, ROWCOL nColEd, int nWidth)
	{
		SetColWidth(nColSt, nColEd, globalUtils.ScaleByDPI(nWidth));
	};

	L_SetColWidth(1, 1, 0);
	L_SetColWidth(2, 2, 80);
	L_SetColWidth(3, 3, 35);
	L_SetColWidth(4, 4, 35);

	MakeItemEx();

	LockUpdate(FALSE);

	SetCellStyle();
}

void CDgnStlRatingLcomAASHTOMainGrid::SetCellStyle()
{
	LockUpdate(TRUE);

	// 표준 스타일 변경
	ChangeStandardStyle(CGXStyle()
			.SetFont(CGXFont_GC().SetSize(8))
			.SetVerticalAlignment(DT_BOTTOM)
			.SetAllowEnter(FALSE));

	// 컬럼 스타일 변경

	CGXStyle style;

	CString title = _T("");
	style.SetValue(title);
	SetCoveredCellsRowCol(4,3,4,4);
	SetStyleRange(CGXRange(4,3,4,4),style);

	style.SetValue(title);
	SetCoveredCellsRowCol(5,3,5,4);
	SetStyleRange(CGXRange(5,3,5,4),style);

	style.SetValue(title);
	SetCoveredCellsRowCol(6,3,6,4);
	SetStyleRange(CGXRange(6,3,6,4),style);

	style.SetValue(title);
	SetCoveredCellsRowCol(7,3,7,4);
	SetStyleRange(CGXRange(7,3,7,4),style);

	style.SetValue(title);
	SetCoveredCellsRowCol(8,3,8,4);
	SetStyleRange(CGXRange(8,3,8,4),style);

	style.SetValue(title);
	SetCoveredCellsRowCol(9,3,9,4);
	SetStyleRange(CGXRange(9,3,9,4), CGXStyle()
			.SetVerticalAlignment(DT_VCENTER)
			.SetHorizontalAlignment(DT_RIGHT)
			.SetValueType(GX_VT_NUMERIC)
			.SetFormat(GX_FMT_FIXED)
			.SetPlaces(2)
			.SetReadOnly(TRUE));

	LockUpdate(FALSE);
}

void CDgnStlRatingLcomAASHTOMainGrid::Set_Title()
{
	CString title=_T("");
	
	auto L_SetColWidth = [this] (ROWCOL nColSt, ROWCOL nColEd, int nWidth)
	{
		SetColWidth(nColSt, nColEd, globalUtils.ScaleByDPI(nWidth));
	};
	//////////////////////////////////////////////////////////////////////////////////////
	//	ResultView의 Title를 입력하는 곳												//

	CGXStyle style;
	style.SetControl(GX_IDS_CTRL_STATIC);
	style.SetHorizontalAlignment(DT_CENTER);
	style.SetVerticalAlignment(DT_VCENTER);
	style.SetDraw3dFrame(gxFrameRaised);
	style.SetFont(CGXFont_GC().SetBold(FALSE).SetSize(8));
	//style.SetInterior(dwColor1);	// RGB(192,192,192) -> GetSysColor(...).

	title = _LS(IDS_DGN_RATING_LOAD_TYPE);
	style.SetValue(title);
	SetCoveredCellsRowCol(0,2,1,2);
	SetStyleRange(CGXRange(0,2,1,2),style);  
	L_SetColWidth(2,2,95);
	title = _LS(IDS_DGN_RATING_FACTOR);
	style.SetValue(title);
	SetCoveredCellsRowCol(0,3,0,4);
	SetStyleRange(CGXRange(0,3,0,4),style);	
	title = _LS(IDS_DGN_RATING_MAX);
	style.SetValue(title);
	SetStyleRange(CGXRange(1,3),style);   
	L_SetColWidth(3,3,35);
	title = _LS(IDS_DGN_RATING_MIN);
	style.SetValue(title);
	SetStyleRange(CGXRange(1,4),style);  
	L_SetColWidth(4,4,35);
}

void CDgnStlRatingLcomAASHTOMainGrid::LoadDlgData()
{
	SetValue(1, 0, RateLoad[0]); // DC Before
	SetValue(2, 1, RateLoad[1]); // DC After
	SetValue(3, 2, RateLoad[2]); // DW
	SetValue(4, 3, RateLoad[3]); // Temp.Gradient
	SetValue(5, 4, RateLoad[4]); // TG
	SetValue(6, 5, RateLoad[5]); // Secondary
	SetValue(7, 6, RateLoad[6]); // Permanent
	SetValue(8, 7, RateLoad[7]); // User
	
	Redraw();
}

void CDgnStlRatingLcomAASHTOMainGrid::MakeItemEx()
{
	CWaitCursor wait;

	SetModeDisplayOnly(FALSE);

	BOOL bOldLock = LockUpdate(TRUE);

	int nRecCount = 8;
	SetRowCount(m_nColHeader);
	SetRecordCount(m_nColHeader+nRecCount);

	LockUpdate(bOldLock);
	if (!bOldLock) Redraw();  
}

BOOL CDgnStlRatingLcomAASHTOMainGrid::InsertRecordByKey(int &key, T_RTLD_D &data)
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

BOOL CDgnStlRatingLcomAASHTOMainGrid::DeleteRecordByKey(int &key, T_RTLD_D &data)
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

void CDgnStlRatingLcomAASHTOMainGrid::UnitChanged(CDBDoc* pDoc)
{
	// unit을 사용하는 것이 없기 때문에 그냥 return 한다.
	return;
}


/////////////////////////////////////////////////////////////////////////////
// CDgnStlRatingLcomAASHTOMainGrid Overridable Functions
void CDgnStlRatingLcomAASHTOMainGrid::OnChangedSelectionPublic(long *keys, int nSize)
{
	if (nSize <= 0)
	{
		m_aSelectedKeys.RemoveAll();
	}
	else
	{
		m_aSelectedKeys.RemoveAll();
		for (int i = 0; i < nSize; i++) m_aSelectedKeys.Add((int)keys[i]);
	}

	//m_pParent->Data2Dlg();
}

long CDgnStlRatingLcomAASHTOMainGrid::GetSelectedKeys(CArray<long, long> &caKey)
{
	// override this function to return selected record count

	/* 현재 Selected키 관리가 안되고 있다.
		 (UpdateBuffer에서 Add, Del, Modify 처리시 Selected key 관리를 
			안하고 있기 때문)
	*/
	long nKeyCount;
	CArray<int, int> aSelKey;

	GetAllSelectedStld(aSelKey);
	nKeyCount = aSelKey.GetSize();
	caKey.SetSize(nKeyCount);
	for (long i = 0; i < nKeyCount; i++)
		caKey[i] = (long)aSelKey[i];
	
	return nKeyCount;
}

BOOL CDgnStlRatingLcomAASHTOMainGrid::ValidateField(CString value, int nColID)
{
	if (nColID >= GetColCount())
	{
		SetWarningText(_LS(IDS_WG_CMD__ADDD___CCMLoadCaseGrid__ValidateField__));
		return FALSE;
	}

	return TRUE;
}

// 삽입이나 수정전에 Record의 Validation을 검사한다.
BOOL CDgnStlRatingLcomAASHTOMainGrid::ValidateRecord(CStringArray &value, CRowColArray &aCols)
{
	CString msg = _T("");
	int nPos;

	for (int i = 2; i < GetColCount()-1; i++)
	{
		nPos = aCols[i]-1;
		if (value[nPos] == _T(""))
			msg += ((msg != _T("")) ? _T("\n") : _T("")) + m_aTitle[i] + _LS(IDS_WG_CMD__ADDD___has_not_been_entered_);
	}
	if (msg == _T("")) return TRUE;
	else SetWarningText(msg);
	return FALSE;
}

// 이 함수는 Paste할 때 호출되므로 False를 리턴할 때는 ValidateField와 
// Validate Record에서 설정된 Warning Text를 없애주고 리턴한다.
BOOL CDgnStlRatingLcomAASHTOMainGrid::ValidateAndMakeRecord(CStringArray &value, CRowColArray& aCols, int &key, T_RTLD_D &data)
{
	// 각 필드에 대해 Validation을 검사한다.
	for (int i = 0; i < value.GetSize(); i++)
	{
		if (!ValidateField(value[aCols[i]-1], i)) 
		{
			SetWarningText(_T(""));
			return FALSE;
		}
	}
	// 레코드에 대해 Validation을 검사한다.
	if (!ValidateRecord(value, aCols))
	{
		SetWarningText(_T(""));
		return FALSE;
	}

	// 레코드를 만들어서 리턴한다.
	for (int i = 0; i < GetColCount(); i++)
	{
		int nPos = aCols[i]-1; // col id i에 해당하는 value가 저장된 위치
		if(i==0) 
		{
			if (value[nPos] == _T("")) key = 0;
			else key = _ttol(value[nPos]);
		}
		else ConvStrToData(i, value[nPos], data);
	}
	return TRUE;
}

BOOL CDgnStlRatingLcomAASHTOMainGrid::AddToDB(ROWCOL nRow)
{
	// Record를 만들어서 DB에 입력한다.
	int key;
	T_RTLD_D data;
 
	GetValue(nRow, key, data);

	//return m_pDoc->m_pDataCtrl->AddStld(data);
	return true;
}

BOOL CDgnStlRatingLcomAASHTOMainGrid::AddToDB(CArray<void*, void*>& raKey, CArray<void*, void*>& raData)
{
	ASSERT(raKey.GetSize() == raData.GetSize());
	int tKey;
	CArray<int, int> aKey;
	CArray<T_RTLD_D, T_RTLD_D&> aData;
	for (int i = 0; i < raKey.GetSize(); i++)
	{
		tKey = *((int*)raKey.GetAt(i));
		aKey.Add(tKey);
		aData.Add(*((T_RTLD_D*)raData.GetAt(i)));
	}
//  return m_pDoc->m_pDataCtrl->AddStld(aData);
	return true;
}


BOOL CDgnStlRatingLcomAASHTOMainGrid::DeleteFromDB(ROWCOL nRow)
{
	int key;
	T_RTLD_D data;
 
	GetValue(nRow, key, data);
//  return m_pDoc->m_pDataCtrl->DelStld(data.LoadCaseName);
	return true;
}

BOOL CDgnStlRatingLcomAASHTOMainGrid::DeleteFromDB(CRowColArray& awRows)
{
	if (awRows.GetSize() <= 0) return TRUE;

	// _LS(IDS_WG_CMD__ADDD__Name) 칼럼을 얻는다.
	ROWCOL nCol;
	VERIFY(GetColumnNoFor(NAME_COL_ID, nCol));

	CString value;
	CArray<CString, CString&> awKeys;
 
	for (int i = 0; i < awRows.GetSize(); i++)
	{
		value = GetValueRowColBeforeEdit(awRows.GetAt(i), nCol);
		awKeys.Add(value);
	}

	return TRUE;
	//return m_pDoc->m_pDataCtrl->DelStld(awKeys);
}

BOOL CDgnStlRatingLcomAASHTOMainGrid::ModifyToDB(ROWCOL nRow)
{
	ROWCOL nCol;
	VERIFY(GetColumnNoFor(2, nCol));  // load case name

	int key;
	T_RTLD_D data;
	CString pOldLoadCaseName;

	pOldLoadCaseName = GetValueRowColBeforeEdit(nRow, nCol);
	GetValue(nRow, key, data);

	// 수정한다. 실패하면 에러 리턴
	//return m_pDoc->m_pDataCtrl->ModifyStld(pOldLoadCaseName, data); 
	return true;
}

BOOL CDgnStlRatingLcomAASHTOMainGrid::ModifyToDB(CStringArray& raOldKey, 
												 CArray<void*, void*>& raKey, 
												 CArray<void*, void*>& raData)
{
	ASSERT(raKey.GetSize() == raData.GetSize());
	ASSERT(raKey.GetSize() == raOldKey.GetSize());
	CArray<CString, CString&> aOldKey;
	CArray<T_RTLD_D, T_RTLD_D&> aData;
	int Key;
	T_RTLD_D Data;
	for (int i = 0; i < raKey.GetSize(); i++)
	{
		Key = _ttol(raOldKey.GetAt(i));
		//m_pDoc->m_pAttrCtrl->GetStld(Key, Data);
		//aOldKey.Add(Data.LoadCaseName);
		aData.Add(*((T_RTLD_D*)raData.GetAt(i)));
	}
	//return m_pDoc->m_pDataCtrl->ModifyStld(aOldKey, aData);
	return true;
}


CString CDgnStlRatingLcomAASHTOMainGrid::GetNewKey()
{
	CString value;
	int key = m_pDoc->m_pAttrCtrl->GetStartNumElem();
	//int key = m_pDoc->m_pAttrCtrl->GetStartNumStld();
	value.Format(LCOM_KEY_FMT, key);
	return value;
}


CString CDgnStlRatingLcomAASHTOMainGrid::GetDefValue(int nIndex)
{
	return m_aCurDefVal[nIndex];
}

CString* CDgnStlRatingLcomAASHTOMainGrid::GetDefValueArray()
{
	return m_aCurDefVal;
}


BOOL CDgnStlRatingLcomAASHTOMainGrid::MakeKeyAndDataArray(CStringArray& value, CRowColArray& awCols, 
																	CArray<void*, void*>& aKey, CArray<void*, void*>& aData)
{
	int* pKey;
	T_RTLD_D* pData;
	pKey = new int;
	pData = new T_RTLD_D;
	aKey.Add((void*)pKey);
	aData.Add((void*)pData);
	if (!ValidateAndMakeRecord(value, awCols, *pKey, *pData)) return FALSE;
	return TRUE;
}

void CDgnStlRatingLcomAASHTOMainGrid::DeleteKeyAndDataArray(CArray<void*, void*>& aKey, CArray<void*, void*>& aData)
{
	int* pKey;
	T_RTLD_D* pData;
	ASSERT(aKey.GetSize() == aData.GetSize());
	for (int i = 0; i < aKey.GetSize(); i++)
	{
		pKey = (int*)aKey.GetAt(i);
		pData = (T_RTLD_D*)aData.GetAt(i);
		delete pKey;
		delete pData;
	}
}

long CDgnStlRatingLcomAASHTOMainGrid::GetCountData()
{
	CDBDoc* pDoc = m_pDoc;
	return pDoc->m_pAttrCtrl->GetCountElem();
	//return pDoc->m_pAttrCtrl->GetCountStld();
}

void CDgnStlRatingLcomAASHTOMainGrid::MakeItemFiltered(long nFilteredNum, long* aFilteredKeys, CRowColArray& awCols)
{
	CDBDoc* pDoc = m_pDoc;
	T_RTLD_D Data;

	for(long nCount=0 ; nCount < nFilteredNum; nCount++)
 	{
		//pDoc->m_pAttrCtrl->GetStld(aFilteredKeys[nCount], Data);
		SetValue(GetParam()->GetData(), nCount+1, aFilteredKeys[nCount], Data, awCols);
	}
}

void CDgnStlRatingLcomAASHTOMainGrid::MakeItemNormal(long nNumData, CRowColArray& awCols)
{
	//int Key;
	T_RTLD_D Data;
	CDBDoc* pDoc = m_pDoc;

	long* KeyBuf = new long[nNumData];
	long nCount = 0;
	//POSITION pos = pDoc->m_pAttrCtrl->GetStartStld();
	POSITION pos = pDoc->m_pAttrCtrl->GetStartElem();
	while(pos != NULL)
	{
	  //pDoc->m_pAttrCtrl->GetNextStld(pos,Key,Data);
	  //KeyBuf[nCount++] = Key;
	}
	qsort(KeyBuf, nNumData, sizeof(int), CTBBrowserWnd::CompareKey);

	for(nCount=0 ; nCount < nNumData; nCount++)
	{
		//pDoc->m_pAttrCtrl->GetStld(KeyBuf[nCount], Data);
		SetValue(GetParam()->GetData(), nCount+1, KeyBuf[nCount], Data, awCols);
	} // end of loop nCount

	delete []KeyBuf;
}

void CDgnStlRatingLcomAASHTOMainGrid::MakeItemEditingRecord(ROWCOL ncRow, CStringArray& EditingRecord)
{
	CDBDoc* pDoc = m_pDoc;
	int Key;
	T_RTLD_D Data;
	for (int i = 0; i < GetColCount(); i++)
	{
		if (i == 0) Key = _ttol(EditingRecord[i]);
		else ConvStrToData(i, EditingRecord[i], Data);
	}
	//pDoc->m_pUnitCtrl->ConvertUnitStldPrevious(Data);	// load case는 Unit과 관계없다.
	// 현재 값을 설정한다.
	SetValue(ncRow, Key, Data);
}

/////////////////////////////////////////////////////////////////////////////
// CDgnStlRatingLcomAASHTOMainGrid Implementation
BOOL CDgnStlRatingLcomAASHTOMainGrid::GetValue(ROWCOL nRow, int &key, T_RTLD_D &data)
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
			if (str == _T(""))
		key = m_pDoc->m_pAttrCtrl->GetStartNumElem();
				//key = m_pDoc->m_pAttrCtrl->GetStartNumStld();
			else
				key = _ttol(str);
		}
		else ConvStrToData(i, str, data);
	}
	return TRUE;
}

BOOL CDgnStlRatingLcomAASHTOMainGrid::SetValue(ROWCOL nRow, int key, T_RTLD_D &data)
{
	//
	CRowColArray awCols;
	awCols.SetSize(GetColCount());
	for (int i = 0;i < GetColCount(); i++)
		VERIFY(GetColumnNoFor(m_aTitle[i], awCols[i]));
	return SetValue(GetParam()->GetData(), nRow, key, data, awCols);
}

BOOL CDgnStlRatingLcomAASHTOMainGrid::SetValue(CGXData* pData, ROWCOL nRow, int key, T_RTLD_D &data, CRowColArray &awCols)
{
	//
	CString str;
	for (int i = 0; i < GetColCount(); i++)
	{
		if(i==0) 
		{
			if (key == 0) str.Empty();
			else str.Format(LCOM_KEY_FMT,key);
		}
		else ConvDataToStr(i, data, str);
		pData->StoreValueRowCol(nRow, awCols[i], str, gxOverride, GX_VT_NUMERIC);
	}

	return TRUE;
}

void CDgnStlRatingLcomAASHTOMainGrid::ConvStrToData(int i, CString& value, T_RTLD_D& data)
{
	if (i==1)  // Load Type
	{
		if(value == _LS(IDS_DGN_RATING_DC_BEFORE))
		{
			data.LoadType = D_STL_RCTYPE_DC_BEFORE;
			data.FactorType = 0;
		}
		else if(value == _LS(IDS_DGN_RATING_DC_AFTER))
		{
			data.LoadType = D_STL_RCTYPE_DC_AFTER;
			data.FactorType = 0;
		}
		else if(value == _LS(IDS_DGN_RATING_DW))
		{
			data.LoadType = D_STL_RCTYPE_DW;
			data.FactorType = 0;
		}
		else if(value == _LS(IDS_DGN_RATING_T))
		{
			data.LoadType = D_STL_RCTYPE_TEMPERATURE;
			data.FactorType = 1;
		}
		else if(value == _LS(IDS_DGN_RATING_TEMP_GRADIENT))//_T("Temp.Gradient")
		{
			data.LoadType = D_STL_RCTYPE_TG;
			data.FactorType = 1;
		}
		else if(value == _LS(IDS_DGN_RATING_SEC))
		{
			data.LoadType = D_STL_RCTYPE_SECONDARY;
			data.FactorType = 1;
		}
		else if(value == _LS(IDS_DGN_RATING_PERMANENT))
		{
			data.LoadType = D_STL_RCTYPE_PERMANENT;
			data.FactorType = 1;
		}
		else if(value == _LS(IDS_DGN_RATING_USER_DEFINE))//_T("User Defined")
		{
			data.LoadType = D_STL_RCTYPE_USER;
			data.FactorType = 1;
		}
	}
	else if (i==2) data.dFactor[0] = _tstof(value); // Max
	else if (i==3) data.dFactor[1] = _tstof(value); // Min
}

void CDgnStlRatingLcomAASHTOMainGrid::ConvDataToStr(int i, T_RTLD_D& data, CString& value)
{
	if (i==1) // Load Type
	{
		if(data.LoadType == D_STL_RCTYPE_DC_BEFORE)
			value = _LS(IDS_DGN_RATING_DC_BEFORE);
		else if(data.LoadType == D_STL_RCTYPE_DC_AFTER)
			value = _LS(IDS_DGN_RATING_DC_AFTER);
		else if(data.LoadType == D_STL_RCTYPE_DW)
			value = _LS(IDS_DGN_RATING_DW);
		else if(data.LoadType == D_STL_RCTYPE_TEMPERATURE)
			value = _LS(IDS_DGN_RATING_T);
		else if(data.LoadType == D_STL_RCTYPE_TG)
			value = _LS(IDS_DGN_RATING_TEMP_GRADIENT);//_T("Temp.Gradient")
		else if(data.LoadType == D_STL_RCTYPE_SECONDARY)
			value = _LS(IDS_DGN_RATING_SEC);
		else if(data.LoadType == D_STL_RCTYPE_PERMANENT)
			value = _LS(IDS_DGN_RATING_PERMANENT);
		else if(data.LoadType == D_STL_RCTYPE_USER)
			value = _LS(IDS_DGN_RATING_USER_DEFINE);//_T("User Defined");
	}
	else if (i==2) value.Format(_T("%.2f"), data.dFactor[0]);
	else if (i==3) value.Format(_T("%.2f"), data.dFactor[1]);  
}

void CDgnStlRatingLcomAASHTOMainGrid::MakeSearchKey(int &key, T_RTLD_D &data, CStringArray &aKey)
{
	int nColID;
	int nSortKeyNum = m_aSortInfo.GetSize();

	aKey.SetSize(nSortKeyNum);
	for (int i = 0; i < nSortKeyNum; i++)
	{
		nColID = m_aSortInfo[i].nRC;
		if (nColID == 0) aKey[i].Format(LCOM_KEY_FMT, key);
		else ConvDataToStr(nColID, data, aKey[i]);
	}
}

void CDgnStlRatingLcomAASHTOMainGrid::GetAllSelectedStld(CArray<int, int>& aSelKey)
{
	for (int i = 0; i < m_aSelectedKeys.GetSize(); i++)
		aSelKey.Add(m_aSelectedKeys[i]);
}

void CDgnStlRatingLcomAASHTOMainGrid::SetHeaderTitle(T_UNIT_SYSTEM &UnitSystem, BOOL bInit)
{
	SetHeaderTitle2((int*)m_aUnit.GetData(), bInit, TRUE);
}


// Hide 된 Row들이 resize되지 않도록 한다.
// 주1 참조
BOOL CDgnStlRatingLcomAASHTOMainGrid::OnTrackColWidth(ROWCOL nCol)
{
	if (nCol == 1) return FALSE;
	return CTBCommon::OnTrackColWidth(nCol);
}

// TBBrowserWndEx::OnSelDragColsDrop 참조
// 부모를 먼저 호출하면 안 된다.
// 1, 2 모두 Frozen Cols로 설정되어 있으므로 이쪽으로 셀이 이동되거나 이 셀들이
// 이동되는 것을 금지한다.
// 주1 참조
BOOL CDgnStlRatingLcomAASHTOMainGrid::OnSelDragColsDrop(ROWCOL nFirstCol, ROWCOL nLastCol, ROWCOL nDestCol)
{
	// Frozen 되어 있는 column 위치에 이동되면 이동을 금지시킨다.
	//if (IsFrozenCol(nDestCol)) return FALSE;
	if (nDestCol == 1 || nFirstCol == 1) return FALSE;
	if (nDestCol == 2 || nFirstCol == 2) return FALSE;
	return CTBCommon::OnSelDragColsDrop(nFirstCol, nLastCol, nDestCol);
}

// LoadCase의 위치를 이동(Serial No가 변경)한다.
/*
BOOL CDgnStlRatingLcomAASHTOMainGrid::OnSelDragRowsDrop(ROWCOL nFirstRow, ROWCOL nLastRow, ROWCOL nDestRow)
{
	// 수정 모드이면 에러 리턴
/*
	if (IsEdit()) 
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Can_t_Move_row_while_editing_));
		return FALSE;
	}

	// Append Row의 뒤면 맨 마지막으로 이동하는 것으로 한다.
	if (nDestRow == GetRowCount()+1) nDestRow--;  

	// 이동의 효과가 없는 곳으로 이동(선택범위내로 다시 이동)하면 그냥 리턴
	if (nDestRow >= nFirstRow && nDestRow <= nLastRow+1) return FALSE;

	// 연속 선택이 아니면 이동을 금지시킨다.
	CRowColArray awRows;
	GetSelectedRows(awRows, TRUE, FALSE);
	if (awRows.GetSize() != nLastRow-nFirstRow+1) return FALSE;


	int nStart, nInc;
	nStart = (int)nDestRow;
	if (nDestRow < nFirstRow)  // 선택 영역 앞으로 이동
	{
		nInc = 1;
	}
	else  // 선택 영역 뒤로 이동
	{
		nStart--;
		nInc = 0;
	}

	CArray<CString, CString&> aLoadCaseName;
	CArray<T_RTLD_D, T_RTLD_D&> aData;
	CString LoadCaseName;
	int key;
	T_RTLD_D data;
	for (int i = nFirstRow; i <= nLastRow; i++)
	{
		GetValue(i, key, data);
		data.LoadCaseId = nStart;
		nStart += nInc;

		aLoadCaseName.Add(data.LoadCaseName);
		aData.Add(data);
	}
	m_pDoc->m_pDataCtrl->ModifyStld(aLoadCaseName, aData);
	return FALSE;
	
}
*/

// 이 함수는 No Column을 클릭했을 때 Row Header를 클릭한 것과 같은 효과를
// 내기 위해서이다. CTBBrowserWndEx::OnLButtonDown 참조
void CDgnStlRatingLcomAASHTOMainGrid::OnLButtonDown(UINT nFlags, CPoint point)
{
	ROWCOL nhRow, nhCol, nkCol;
	int ht = HitTest(point, &nhRow, &nhCol);
	if (ht != GX_NOHIT && ht != GX_VERTLINE)
	{
		GetColumnNoFor(1, nkCol);
		if (nhCol == nkCol && IsFrozenCol(nhCol)) // key column이면
		{
			// Row header의 rectangle을 얻는다.
			CRect r = CalcRectFromRowColEx(nhRow, 0, nhRow, 0);  
			point.x = (r.left+r.right)/2;
		}
	}
	CTBCommon::OnLButtonDown(nFlags, point);

	CheckCurrentRecordChanged();
}

/*
CString CDgnStlRatingLcomAASHTOMainGrid::GetLoadTypeCode(CString &csLoadTypeName)
{
	return m_pDoc->m_pLoadDB->GetLoadType(csLoadTypeName);
}

CString CDgnStlRatingLcomAASHTOMainGrid::GetLoadTypeName(CString &csLoadTypeCode)
{
	return m_pDoc->m_pLoadDB->GetLoadTypeName(csLoadTypeCode);
}
*/

//----------------------------------------------------------------------------->
// 여기서 부터 Current Record의 위치 변경을 Detect하기 위해 삽입된 코드이다.
// Current Record가 변경되었는지 검사
BOOL CDgnStlRatingLcomAASHTOMainGrid::CheckCurrentRecordChanged()
{
	ROWCOL ncRow, ncCol;
	GetCurrentCell(ncRow, ncCol);
	if (ncRow != m_nCurrentRow)
	{
		OnChangeCurrentRecord(ncRow);
		return TRUE;
	}
	return FALSE;
}

BOOL CDgnStlRatingLcomAASHTOMainGrid::ProcessKeys(CWnd *pSender, UINT nMessage, UINT nChar, UINT nRepCnt, UINT flags)
{
	BOOL bResult = CTBCommon::ProcessKeys(pSender, nMessage, nChar, nRepCnt, flags);
	CheckCurrentRecordChanged();

	/*
	ROWCOL ncRow, ncCol;
	GetCurrentCell(ncRow, ncCol);
	if(ncRow > 2 && ncCol==2)
	{
		T_RTLD_D TempData;
		int factor = _tstof(GetValueRowCol(ncRow, 2));
		TempData.dFactor[1] = factor;
		SetValue(ncRow, ncRow-1, TempData);
	}
	*/

	return bResult;
}

void CDgnStlRatingLcomAASHTOMainGrid::OnRButtonDblClk(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	ROWCOL ncRowBefore, ncRowAfter, ncCol;
	if (!GetCurrentCell(ncRowBefore, ncCol)) return;

	CTBCommon::OnRButtonDblClk(nFlags, point);

	if (!GetCurrentCell(ncRowAfter, ncCol)) return;
	if (ncRowBefore != ncRowAfter) OnChangeCurrentRecord(ncRowAfter);
}

void CDgnStlRatingLcomAASHTOMainGrid::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	CTBCommon::OnLButtonDblClk(nFlags, point);
	CheckCurrentRecordChanged();
}

// OnLButtonDown이 기존에 정의되어 있기 때문에 기존 함수에 
// CheckCurrentRecordChanged를 호출하도록 한다.
/*
void CDgnStlRatingLcomAASHTOMainGrid::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	CTBCommon::OnLButtonDown(nFlags, point);
	CheckCurrentRecordChanged();
}
*/

void CDgnStlRatingLcomAASHTOMainGrid::OnChangeCurrentRecord(ROWCOL nRow)
{
	if(nRow>8)
		return;

	m_pSubGrid->GetCombItem(RateLoad[GetDataIndexfromRow(m_nCurrentRow)]);
	m_pSubGrid->SetData(RateLoad[GetDataIndexfromRow(nRow)]);
	m_pSubGrid->EnableDisableGrid(nRow);
	m_nCurrentRow = nRow;

	m_pParent->EnableDisableSubGrid(nRow);
}

void CDgnStlRatingLcomAASHTOMainGrid::SaveCurrentGrid()
{
	m_pSubGrid->GetCombItem(RateLoad[GetDataIndexfromRow(m_nCurrentRow)]);

	// DC Before
	RateLoad[0].dFactor[0] = _tstof(GetValueRowCol(1, 3)); 
	RateLoad[0].dFactor[1] = _tstof(GetValueRowCol(1, 4));

	// DC After
	RateLoad[1].dFactor[0] = _tstof(GetValueRowCol(2, 3)); 
	RateLoad[1].dFactor[1] = _tstof(GetValueRowCol(2, 4));

	// DW
	RateLoad[2].dFactor[0] = _tstof(GetValueRowCol(3, 3));
	RateLoad[2].dFactor[1] = _tstof(GetValueRowCol(3, 4));

	// Temp
	RateLoad[3].dFactor[0] = _tstof(GetValueRowCol(4, 3));
	RateLoad[3].dFactor[1] = _tstof(GetValueRowCol(4, 4));

	// Temp.Gradient
	RateLoad[4].dFactor[0] = _tstof(GetValueRowCol(5, 3));
	RateLoad[4].dFactor[1] = _tstof(GetValueRowCol(5, 4));

	// SEC
	RateLoad[5].dFactor[0] = _tstof(GetValueRowCol(6, 3));
	RateLoad[5].dFactor[1] = _tstof(GetValueRowCol(6, 4));

	// Permanent
	RateLoad[6].dFactor[0] = _tstof(GetValueRowCol(7, 3));
	RateLoad[6].dFactor[1] = _tstof(GetValueRowCol(7, 4));

	// User
	RateLoad[7].dFactor[0] = _tstof(GetValueRowCol(8, 3));
	RateLoad[7].dFactor[1] = _tstof(GetValueRowCol(8, 4));	
}

int CDgnStlRatingLcomAASHTOMainGrid::GetDataIndexfromRow(int nRow)
{
	int nIndex = 0;

	switch(nRow)
	{
		case 1 :
			nIndex = 0; 
			break;
		case 2 :
			nIndex = 1;
			break;
		case 3 :
			nIndex = 2;
			break;
		case 4 :
			nIndex = 3;
			break;
		case 5 :
			nIndex = 4;
			break;
		case 6 :
			nIndex = 5;
			break;
		case 7 :
			nIndex = 6;
			break;
		case 8 :
			nIndex = 7;
			break;
	}

	return nIndex;
}

CString CDgnStlRatingLcomAASHTOMainGrid::GetKeyForRow(ROWCOL ncRow, ROWCOL nkCol)
{
	CString sKey;

	if (IsAppendRow(ncRow)) sKey = _LS(IDS_WG_CMD__ADDD__Append_Row);
	else if (m_nEditMode == addnew) sKey = _LS(IDS_WG_CMD__ADDD__New_Row);
	else if (m_nEditMode == edit) sKey = GetValueRowColBeforeEdit(ncRow, nkCol);
	else sKey = GetValueRowCol(ncRow, nkCol);

	return sKey;
}

BOOL CDgnStlRatingLcomAASHTOMainGrid::GetCurrentRcbe(int& nKey)
{
	ROWCOL ncRow, ncCol;
	if (!GetCurrentCell(ncRow, ncCol)) return FALSE;
	if (ncRow < 1) return FALSE;
	if (IsAppendRow(ncRow)) return FALSE;

	if (!GetColumnNoFor(GetKeyColID(), ncCol)) return FALSE;
	nKey = _ttoi(GetValueRowCol(ncRow, ncCol));
	return TRUE;
}

BOOL CDgnStlRatingLcomAASHTOMainGrid::SetCurrentRcbe(int key, T_RTLD_D& data)
{
	ROWCOL nRow, nCol;

	if (!GetCurrentCell(nRow, nCol)) return FALSE;
	
	CStringArray aKey;
	MakeSearchKey(key, data, aKey);
	if (!FindRecordByKey(aKey, nRow)) return FALSE;
	SetCurrentCell(nRow, nCol);
	CheckCurrentRecordChanged();
	return TRUE;
}

BOOL CDgnStlRatingLcomAASHTOMainGrid::LockUpdateGrid(BOOL bUpdate)
{
	return LockUpdate(bUpdate);
}

void CDgnStlRatingLcomAASHTOMainGrid::SetRtldData(T_RTLD_D RateData[8])
{
	for(int i=0 ; i<8 ; i++)
	{
	  RateLoad[i] = RateData[i];
	}

	LoadDlgData();

	ROWCOL ncRow, ncCol;
	GetCurrentCell(ncRow, ncCol);

	m_pSubGrid->SetData(RateLoad[GetDataIndexfromRow(ncRow)]);
}


