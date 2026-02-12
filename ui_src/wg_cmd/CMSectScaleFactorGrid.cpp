// CMSectScaleFactorGrid.cpp : implementation file
//
// No | (SECF Key) | Name | I=J | IJ | fArea | fAsy | fAsz | fIxx | fIyy | fIzz | fWgt | Part | Group
// 0        1         2       3   4       5                                         11    12      13
//////////////////////////////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMSectScaleFactorGrid.h"

#include "CMSectScaleFactorDlgNew.h"

#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_StagCtrl.h"

#include "..\wg_base\wg_base_CompFunc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define KEY_FMT         _T("%ld")
#define DATA_FMT        _T("%.15g")
#define D_BNGR_COL_ID   13

#define D_BNDR_GROUP_DEFAULT  _LS(IDS_TB_DEFAULT_BNDR_GROUP)

/////////////////////////////////////////////////////////////////////////////
// CCMSectScaleFactorGrid
CString CCMSectScaleFactorGrid::m_aCurDefVal[] = {_T(""), _T(""), _T("O"), _T("I"), _T("1.0"), _T("1.0"), _T("1.0"), _T("1.0"), _T("1.0"), _T("1.0"), _T("1.0"), _T("Before"), _T("")};

CCMSectScaleFactorGrid::CCMSectScaleFactorGrid()
{
	AddColInfo(_LS(IDS_CMD_STLD_no)   , t_integer,  0);
	AddColInfo(_T("Keymap")               , t_integer,  0);
	AddColInfo(_LS(IDS_CMD_STLD_name) , t_string,   0);
	AddColInfo(_T("I=J")                  , t_string,   0);
	AddColInfo(_T("IJ")                   , t_string,   0);
	AddColInfo(_T("fArea")                , t_real,     0);
	AddColInfo(_T("fAsy")                 , t_real,     0);
	AddColInfo(_T("fAsz")                 , t_real,     0);
	AddColInfo(_T("fIxx")                 , t_real,     0);
	AddColInfo(_T("fIyy")                 , t_real,     0);
	AddColInfo(_T("fIzz")                 , t_real,     0);
	AddColInfo(_T("fWgt")                 , t_real,     0);
	AddColInfo(_T("Part")                 , t_string,   0);
	AddColInfo(_LS(IDS_CMD_PJST_Group), t_string,   0);
	
	m_aPrimaryKey.Add(0);
	m_aPrimaryKey.Add(1);

	m_aCurDefVal[D_BNGR_COL_ID-1] = _LS(IDS_TB_DEFAULT_BNDR_GROUP);
}

CCMSectScaleFactorGrid::~CCMSectScaleFactorGrid()
{
}


BEGIN_MESSAGE_MAP(CCMSectScaleFactorGrid, CTBCommon)
	//{{AFX_MSG_MAP(CCMSectScaleFactorGrid)
	ON_WM_RBUTTONDBLCLK()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONDBLCLK()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CCMSectScaleFactorGrid message handlers

void CCMSectScaleFactorGrid::Initialize(CCMSectScaleFactorDlgNew* pParent)
{
	ASSERT(pParent);
	m_pParent = pParent;

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
			.SetAllowEnter(FALSE));
	ChangeColHeaderStyle(CGXStyle().SetFont(CGXFont_GC().SetBold(FALSE)));
	ChangeRowHeaderStyle(CGXStyle()
					.SetFont(CGXFont_GC().SetBold(FALSE))
					.SetHorizontalAlignment(DT_RIGHT)
					.SetControl(GX_IDS_CTRL_HEADER));
	GetParam()->SetNumberedRowHeaders(FALSE);

	// 컬럼 스타일 변경
	SetStyleRange(CGXRange().SetCols(1, m_nColCount), CGXStyle()
			.SetVerticalAlignment(DT_VCENTER)
			.SetHorizontalAlignment(DT_RIGHT)
			.SetValueType(GX_VT_NUMERIC)
			.SetFormat(GX_FMT_FIXED)
			.SetPlaces(2));
	SetRowHeightByDPI(0, 0, 20);

	SetStyleRange(CGXRange().SetCols(1, 2), CGXStyle()
			.SetPlaces(0));
	HideColsByIntend(2, 2, TRUE);

	SetStyleRange(CGXRange().SetCols(3), CGXStyle() // Name Disable
			.SetVerticalAlignment(DT_VCENTER)
			.SetHorizontalAlignment(DT_LEFT)
			.SetValueType(GX_VT_STRING)
			.SetEnabled(FALSE)
			.SetWrapText(FALSE));

	CString szChoiceList = _T("O\nX\n");
	SetStyleRange(CGXRange().SetCols(4), CGXStyle() // I=J
			.SetControl(GX_IDS_CTRL_TABBED_COMBOBOX)
			.SetChoiceList(szChoiceList)
			.SetUserAttribute(GX_IDS_UA_TABLIST_KEYCOL, _T("0"))
			.SetUserAttribute(GX_IDS_UA_TABLIST_TEXTCOL, _T("0"))
			.SetUserAttribute(GX_IDS_UA_TABLIST_SHOWALLCOLS, _T("1"))
			.SetValueType(GX_VT_STRING)
			.SetHorizontalAlignment(DT_LEFT)
			.SetValue(_T(""))
			);

	SetStyleRange(CGXRange().SetCols(5), CGXStyle() // IJ 
			.SetVerticalAlignment(DT_VCENTER)
			.SetHorizontalAlignment(DT_CENTER)
			.SetValueType(GX_VT_STRING)
			.SetEnabled(FALSE));

	SetCompositeColumn(D_BNGR_COL_ID);

	SetBngrColumn(D_BNGR_COL_ID+1); // Group Column 설정

	// always show vertical scrollbar
	SetScrollBarMode(SB_VERT, gxnEnabled | gxnEnhanced);
	SetScrollBarMode(SB_HORZ, gxnAutomatic);

	// 현재 셀 설정
	SetRightValidCell(1, 0);
	GetParam()->SetHideCurrentCell(GX_HIDE_NEVER);

	// Sorting 정보 초기화
	m_aSortInfo.SetSize(2);
	m_aSortInfo[0].nRC = 0;
	m_aSortInfo[0].sortType = CGXSortInfo::numeric;
	m_aSortInfo[0].sortOrder = CGXSortInfo::ascending;
	m_aSortInfo[1].nRC = 1;
	m_aSortInfo[1].sortType = CGXSortInfo::numeric;
	m_aSortInfo[1].sortOrder = CGXSortInfo::ascending;

	//EnableAppendRow(FALSE); //추가하는 과정에서 에러 발생하여 추가 못하도록 임시 처리 
														// -> 에러 해결하여 추가 가능으로 변경

	LockUpdate(FALSE);
}

BOOL CCMSectScaleFactorGrid::InsertRecordByKey(const T_SECF_K &key, const T_SECF_D &data)
{
	ASSERT(0);  // mylee - 처리가 곤란하여 쓰이지 않고 전부 지우고 새로 추가하는 방법 사용 
							// void CCMSectScaleFactorDlgNew::UpdateBuffer() 참조

	// 삽입 위치를 찾는다.
	ROWCOL nRow;

	// aKey를 만드는 코드 삽입
	CStringArray aKey;
	MakeSearchKey(key, data, aKey);
	FindRecordByKey(aKey, nRow);

	VERIFY(InsertRecord(nRow+1)); // J단 뒤에 2열 추가
	VERIFY(InsertRecord(nRow+1));

	if (!SetValue(nRow+1, key, data)) return FALSE;
	if (!SetValue(nRow+2, key, data)) return FALSE;
	return TRUE;
}

// Key 값과 같은 Row 를 직접 선택해서 삭제해 주도록 변경.
// Modify 시 약간 이상하지만 어차피 다시 뿌려주므로 상관 없음.
// Del 키 눌렀을 때만 처리 
// -> 현재 호출되지 않고 싹 지우는 방법으로 변경
BOOL CCMSectScaleFactorGrid::DeleteRecordByKey(const T_SECF_K &key, const T_SECF_D &data)
{
	CString str;
	ROWCOL nRow, nCol;

	LockUpdate(TRUE);
	
	int nCount = GetRowCount();
	int nFound = 0;
	VERIFY(GetColumnNoFor(m_aTitle[1], nCol));
	for (nRow=1 ; nRow<nCount ; ++nRow)
	{
		str = GetValueRowCol(nRow, nCol);
		if (str==_T("") || key.keymap == _tstol(str))
		{
			++nFound;
			if (DeleteRecord(nRow)) {--nRow; --nCount;}
			else return FALSE;
		}
	}
	if (nFound == 0) return FALSE;

	LockUpdate(FALSE);
	
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CCMSectScaleFactorGrid Overridable Functions
void CCMSectScaleFactorGrid::OnChangedSelectionPublic(long *keys, int nSize)
{
	T_SECF_K key;
	if (nSize <= 0)
	{
		m_aSelectedKeys.RemoveAll();
	}
	else
	{
		m_aSelectedKeys.RemoveAll();
		for (int i = 0; i < nSize; i++) 
		{
			key.keymap = keys[i];
			m_aSelectedKeys.Add(key);
		}
	}
}

long CCMSectScaleFactorGrid::GetSelectedKeys(CArray<long, long> &caKey)
{
	long nKeyCount;
	CArray<T_SECF_K, T_SECF_K> aKey;

	GetAllSelectedSecf(aKey);
	nKeyCount = aKey.GetSize();
	caKey.SetSize(nKeyCount);
	for (long i = 0; i < nKeyCount; i++)
		caKey[i] = (long)aKey[i].keymap;
	
	return nKeyCount;
}

BOOL CCMSectScaleFactorGrid::ValidateField(CString value, int nColID)
{
	int nErrCode;

	if (nColID == 0 || nColID == 1)
	{
		if (!IsValidLongNumber(value, nErrCode))
			return InvalidLongNumber(nErrCode);
	}
	else if (nColID >= 5 && nColID <= 11)
	{
		if (!IsValidDoubleNumber(value, nErrCode))
			return InvalidDoubleNumber(nErrCode);
	}

	return TRUE;
}

// 삽입이나 수정전에 Record의 Validation을 검사한다.
BOOL CCMSectScaleFactorGrid::ValidateRecord(CStringArray &value, CRowColArray &aCols)
{
	CString msg = _T("");
	int nPos;

	//BOOL bBlankKeymap = (value[aCols[1]-1]==_T("")); // KeyMap 이 Blank -> New Row
	//BOOL bSameIJ = !(value[aCols[3]-1]==_T("X")); // I=J
	BOOL bJ = (value[aCols[4]-1]==_T("J")); 

	for (int i = 0; i < GetColCount(); i++)
	{
		if (bJ && (i==0 || i==2 || i==3 || i==4 || i>=12)) // J단 인 경우
			continue;
		if (i == 1) continue;     // Keymap
		if (i == 2) continue;     // name
		if (i == 3) continue;     // I=J 일단  continue
		if (i == 4) continue;     // I J
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
BOOL CCMSectScaleFactorGrid::ValidateAndMakeRecord(CStringArray &value, CRowColArray& aCols, T_SECF_K &key, T_SECF_D &data)
{
	// 레코드에 대해 Validation을 검사한다.
	if (!ValidateRecord(value, aCols))
		return FALSE;

	// I J 단을 판별함
	BOOL bJ = (value[aCols[4]-1]==_T("J")); 

	// value array 가 속한 nRow 를 찾음
	ROWCOL nRow, nCol, nTargetRow;
	int nCount = GetRowCount();
	VERIFY(GetColumnNoFor(m_aTitle[1], nCol));
	for (nRow=1 ; nRow<=nCount ; ++nRow)
	{
		if (value[aCols[1]-1] == GetValueRowCol(nRow, nCol)) 
		{
			nTargetRow = nRow; break;
		}
	}

	// GetValue 함수로 data 기본 깔아줌 (2행 읽어야 하기 때문)
	if (!GetValue(nRow, key, data)) return FALSE;
	
	if (bJ) // J단인 경우는 여기만 실행후 리턴
	{
		T_SECF_D dataForJ;
		dataForJ.Initialize();
		int nPos;
		for (int i=5; i <= 11; i++)
		{
			nPos = aCols[i]-1;
			ConvStrToData(i, value[nPos], key, dataForJ);
			data.dScaleFactorJ[i-5] = dataForJ.dScaleFactor[i-5];
		}
		return TRUE;
	}

	int nPos;
	for (int i=0; i < GetColCount(); i++)
	{
		nPos = aCols[i]-1; // col id i에 해당하는 value가 저장된 위치
		if(i==0) 
		{
			if (value[nPos] == _T("")) key.key.entity = 0;
			else key.key.entity = _tstol(value[nPos]);
		}
		else if (i==1) 
		{
			if (value[nPos] != _T("")) key.keymap = _tstol(value[nPos]); // 새 행 추가시 대입하면 key.key.entity 초기화되므로
		}
		else
			ConvStrToData(i, value[nPos], key, data);
	}
	return TRUE;
}

BOOL CCMSectScaleFactorGrid::AddToDB(ROWCOL nRow)
{
	if (!CanSaveData()) return FALSE;

	// Record를 만들어서 DB에 입력한다.
	T_SECF_K key;
	T_SECF_D data; 
	GetValue(nRow, key, data);
	return m_pDoc->m_pDataCtrl->AddSecf(key.key.entity, data);
}

BOOL CCMSectScaleFactorGrid::AddToDB(CArray<void*, void*>& raKey, CArray<void*, void*>& raData)
{
	ASSERT(raKey.GetSize() == raData.GetSize());
	T_SECF_K tKey;
	CArray<T_SECF_K, T_SECF_K> aKey;
	CArray<T_SECF_D, T_SECF_D&> aData;
	for (int i = 0; i < raKey.GetSize(); i++)
	{
		tKey = *((T_SECF_K*)raKey.GetAt(i));
		aKey.Add(tKey);
		aData.Add(*((T_SECF_D*)raData.GetAt(i)));
	}
	// Data가 배열로 들어가는 Interface는 없다. 따라서 이런 상황이 발생하면
	// 안 되므로 FALSE 리턴한다.
	return m_pDoc->m_pDataCtrl->AddSecf(aKey, aData);
}

BOOL CCMSectScaleFactorGrid::DeleteFromDB(ROWCOL nRow)
{
	T_SECF_K key;
	T_SECF_D data;
 
	GetValue(nRow, key, data);
	return m_pDoc->m_pDataCtrl->DelSecf(key);
}

BOOL CCMSectScaleFactorGrid::DeleteFromDB(CRowColArray& awRows)
{
	if (awRows.GetSize() <= 0) return TRUE;

	ROWCOL nCol, nSubCol;
	VERIFY(GetColumnNoFor(GetKeyColID(), nCol));
	VERIFY(GetColumnNoFor(1, nSubCol)); // SNo column

	CString value;
	T_SECF_K key;
	CArray<T_SECF_K, T_SECF_K> awKeys;
 
	for (int i = 0; i < awRows.GetSize(); i++)
	{
		value = GetValueRowColBeforeEdit(awRows.GetAt(i), nCol);
		key.key.entity = _tstol(value);
		value = GetValueRowColBeforeEdit(awRows.GetAt(i), nSubCol);
		key.keymap = _tstol(value);
		awKeys.Add(key);
	}

	return m_pDoc->m_pDataCtrl->DelSecf(awKeys);
}

BOOL CCMSectScaleFactorGrid::ModifyToDB(ROWCOL nRow)
{
	if (!CanSaveData()) return FALSE;

	ROWCOL nCol;
	VERIFY(GetColumnNoFor(GetKeyColID(), nCol));

	CString value;
	T_SECF_K pKey, key;
	T_SECF_D data;

	value = GetValueRowColBeforeEdit(nRow, nCol);
	pKey.key.entity = _tstol(value);
	GetColumnNoFor(1, nCol);
	value = GetValueRowColBeforeEdit(nRow, nCol);
	pKey.keymap = _tstol(value);
	GetValue(nRow, key, data);

	// 수정한다. 실패하면 에러 리턴
	return m_pDoc->m_pDataCtrl->ModifySecf(pKey, key, data);
}

BOOL CCMSectScaleFactorGrid::ModifyToDB(CStringArray& raOldKey, 
												 CArray<void*, void*>& raKey, 
												 CArray<void*, void*>& raData)
{
	T_SECF_K Key;
	ASSERT(raKey.GetSize() == raData.GetSize());
	ASSERT(raKey.GetSize() == raOldKey.GetSize());
	CArray<T_SECF_K, T_SECF_K> aKey, aOldKey;
	CArray<T_SECF_D, T_SECF_D&> aData;
	for (int i = 0; i < raKey.GetSize(); i++)
	{
		Key.keymap = _tstol(raOldKey.GetAt(i)); 
		aOldKey.Add(Key);
		aKey.Add(*((T_SECF_K*)raKey.GetAt(i)));
		aData.Add(*((T_SECF_D*)raData.GetAt(i)));
	}
	return m_pDoc->m_pDataCtrl->ModifySecf(aOldKey, aKey, aData);
}


CString CCMSectScaleFactorGrid::GetNewKey()
{
	CString value;
	value = _T("");
	return value;
}

CString CCMSectScaleFactorGrid::GetDefValue(int nIndex)
{
	return m_aCurDefVal[nIndex];
}

CString* CCMSectScaleFactorGrid::GetDefValueArray()
{
	return m_aCurDefVal;
}

BOOL CCMSectScaleFactorGrid::MakeKeyAndDataArray(CStringArray& value, CRowColArray& awCols, 
																	CArray<void*, void*>& aKey, CArray<void*, void*>& aData)
{
	T_SECF_K* pKey;
	T_SECF_D* pData;
	pKey = new T_SECF_K;
	pData = new T_SECF_D;
	aKey.Add((void*)pKey);
	aData.Add((void*)pData);
	if (!ValidateAndMakeRecord(value, awCols, *pKey, *pData)) return FALSE;
	return TRUE;
}

void CCMSectScaleFactorGrid::DeleteKeyAndDataArray(CArray<void*, void*>& aKey, CArray<void*, void*>& aData)
{
	T_SECF_K* pKey;
	T_SECF_D* pData;
	ASSERT(aKey.GetSize() == aData.GetSize());
	for (int i = 0; i < aKey.GetSize(); i++)
	{
		pKey = (T_SECF_K*)aKey.GetAt(i);
		pData = (T_SECF_D*)aData.GetAt(i);
		delete pKey;
		delete pData;
	}
}

long CCMSectScaleFactorGrid::GetCountData()
{
	return m_pDoc->m_pAttrCtrl->GetCountSecf();
}

void CCMSectScaleFactorGrid::MakeItemFiltered(long nFilteredNum, long* aFilteredKeys, CRowColArray& awCols)
{
	T_SECF_K Key;
	T_SECF_D Data;

	for(long nCount=0 ; nCount < nFilteredNum; nCount++)
 	{
		Key.keymap = aFilteredKeys[nCount];
		m_pDoc->m_pAttrCtrl->GetSecf(Key, Data);
		SetValue(GetParam()->GetData(), nCount+1, Key, Data, awCols);
	}
}

void CCMSectScaleFactorGrid::MakeItemNormal(long nNumData, CRowColArray& awCols)
{
	T_SECF_K Key;
	T_SECF_D Data;

	long* KeyBuf = new long[nNumData];
	long nCount = 0;
	POSITION pos = m_pDoc->m_pAttrCtrl->GetStartSecf();
	while(pos != NULL)
	{
	  m_pDoc->m_pAttrCtrl->GetNextSecf(pos,Key,Data);
	  KeyBuf[nCount++] = Key.keymap;
	}
	qsort(KeyBuf, nNumData, sizeof(T_SECF_K), CTBBrowserWnd::CompareKey);

	SetRecordCount(nNumData*2);

	for(nCount=0 ; nCount < nNumData; nCount++)
	{
		Key.keymap = KeyBuf[nCount];
		m_pDoc->m_pAttrCtrl->GetSecf(Key, Data);
		SetValue(GetParam()->GetData(), nCount*2+1, Key, Data, awCols);
		SetValue(GetParam()->GetData(), nCount*2+2, Key, Data, awCols);
	} // end of loop nCount

	delete []KeyBuf;
}

void CCMSectScaleFactorGrid::MakeItemEditingRecord(ROWCOL ncRow, CStringArray& EditingRecord)
{
	CDBDoc* pDoc = m_pDoc;
	T_SECF_K key;
	T_SECF_D data;
	for (int i = 0; i < GetColCount(); i++)
	{
		if (i == 0) key.key.entity = _tstol(EditingRecord[i]);
		else if (i == 1) key.keymap = _tstol(EditingRecord[i]);
		else ConvStrToData(i, EditingRecord[i], key, data);
	}
	// 현재 값을 설정한다.
	SetValue(ncRow, key, data);
}

/////////////////////////////////////////////////////////////////////////////
// CCMSectScaleFactorGrid Implementation
BOOL CCMSectScaleFactorGrid::GetValue(ROWCOL nRow, T_SECF_K &key, T_SECF_D &data)
{
	ROWCOL nCol;
	CString str;
	data.Initialize();

	if (nRow%2==0) --nRow; // J 단 생기면서 추가된 내용
	CString strForJ;
	T_SECF_D dataForJ;
	dataForJ.Initialize();
	
	for (int i = 0; i < GetColCount(); i++)
	{
		VERIFY(GetColumnNoFor(m_aTitle[i], nCol));
		str = GetValueRowCol(nRow, nCol);
		// 수식인 경우 값으로 변환한다.
		// Validation이 OK인 Field이므로 항상 TRUE 리턴
		VERIFY(ConvertFormulaToValue(str)); 

		if (i>=5 && i<=11) // J 단 생기면서 추가된 내용 - 받아온 값을 J 단에 저장
		{
			strForJ = GetValueRowCol(nRow+1, nCol);
			VERIFY(ConvertFormulaToValue(strForJ));
			ConvStrToData(i, strForJ, key, dataForJ);
			data.dScaleFactorJ[i-5] = dataForJ.dScaleFactor[i-5];
		}

		if(i==0) key.key.entity = _tstol(str);
		else if (i==1) 
		{
			if (str != _T("")) key.keymap = _tstol(str); // 새 행 추가시 대입되면 key.key.enntity 초기화되므로
		}
		else ConvStrToData(i, str, key, data);
	}
	return TRUE;
}

BOOL CCMSectScaleFactorGrid::SetValue(ROWCOL nRow, const T_SECF_K &key, const T_SECF_D &data)
{
	CRowColArray awCols;
	awCols.SetSize(GetColCount());
	for (int i = 0; i < GetColCount(); i++)
		VERIFY(GetColumnNoFor(m_aTitle[i], awCols[i]));
	return SetValue(GetParam()->GetData(), nRow, key, data, awCols);
}

BOOL CCMSectScaleFactorGrid::SetValue(CGXData* pData, ROWCOL nRow, const T_SECF_K &key, const T_SECF_D &data, CRowColArray &awCols)
{
	CString str;
	int nValueType;

	BOOL bJ = (nRow%2==0);
	T_SECF_D dataJ; dataJ.Initialize();
	if (bJ)
		for (int i=0 ; i<7 ; ++i) dataJ.dScaleFactor[i] = data.dScaleFactorJ[i]; // J단 출력 위해

	for (int i = 0; i < GetColCount(); i++)
	{
		if(i==0) 
		{
			if (key.key.entity == 0) str.Empty();
			else str.Format(KEY_FMT,key.key.entity);
		}
		else if (i==1)
		{
			str.Format(KEY_FMT, key.keymap);
		}
		else if (i==4) // IJ
			str = (bJ ? _T("J") : _T("I"));
		else 
		{
			if (bJ && (i>=5 && i<=11) ) 
				ConvDataToStr(i, key, dataJ, str); // J단 ScaleFactor
			else
				ConvDataToStr(i, key, data, str);
		}

		if (m_aType[i] == t_string) nValueType = GX_VT_STRING;
		else nValueType = GX_VT_NUMERIC;

		if (bJ && (i==0 || i==2 || i==3 || i>=12)) // J 단 keymap(1), IJ(4), scaleFactor(5-11) 제외하고 blank
			str = _T("");

		pData->StoreValueRowCol(nRow, awCols[i], str, gxOverride, nValueType);
	}

	OnOffBeforeAfter(nRow);
	CoverByIJ(nRow);

	return TRUE;
}

// 1. Section의 타입에 따른 Before, After를 Enable/Disable 해 주고 기본값 세팅
// 2. Tapered 인 경우만 I=J 활성화 
// 3. I=J 가 X 가 아닌 경우는 J 단 Edit 비활성화
// 부르는 곳: SetValue 와 OnModifyCell 에서 Call
void CCMSectScaleFactorGrid::OnOffBeforeAfter(ROWCOL nRow)
{
	CGXStyle style, styleIJ; 
	style.Free();
	styleIJ.Free();

	// J 단 생기면서 추가된 내용
	ROWCOL nRowForSect = nRow;
	if (nRow%2==0) nRowForSect = nRow-1; 

	// 해당 Row의 섹션키 얻어옴
	ROWCOL nKeyCol=0;
	VERIFY(GetColumnNoFor(GetKeyColID(), nKeyCol));
	CString value = GetValueRowCol(nRowForSect, nKeyCol); // nRow->nRowForSect
	T_SECT_K SectK = _tstol(value);
	T_SECT_D SectData;
	int nStype;
	if (!m_pDoc->m_pAttrCtrl->GetSect(SectK, SectData)) 
	{ 
		ASSERT(0);
		nStype = D_SECT_TYPE_REGULAR; // 에러일때 일단 일반단면으로 처리
	}
	else
	 nStype = SectData.nStype;
	
	// 1. Before / After ( 전체 Row Disable 시 함께 Disable 되도록 먼저 처리)
	if(nStype==D_SECT_TYPE_COMPO_B || nStype==D_SECT_TYPE_COMPO_I || nStype==D_SECT_TYPE_COMPO_TUB ||
		 nStype==D_SECT_TYPE_COMPO_STLG_B || nStype==D_SECT_TYPE_COMPO_STLG_I || nStype==D_SECT_TYPE_COMPO_STLG_TUB  || IsCmpTapBoxITub(SectData)) 
	{
		style.SetEnabled(TRUE);
	}
	else if(nStype==D_SECT_TYPE_COMPO_CI || nStype==D_SECT_TYPE_COMPO_CT || nStype==D_SECT_TYPE_COMPO_G ||
					nStype==D_SECT_TYPE_COMPO_PC || IsCmpTapCICT(SectData)) 
	{
		style.SetEnabled(FALSE);
		style.SetValue(_LS(IDS_CMD_SECF_ALL));
	}
	else 
	{
		style.SetEnabled(FALSE);
		style.SetValue(_LS(IDS_CMD_SECF_BEFORE));
	}
	SetStyleRange(CGXRange(nRow, D_BNGR_COL_ID), style);

	// 2. Tapered 만 I=J, IJ 컬럼 Enable
	SetStyleRange(CGXRange(nRow, 4), CGXStyle().SetEnabled(nStype==D_SECT_TYPE_TAPERED));

	// 3. I=J 가 X 가 아닌 경우는 J ScaleFactor 입력부분 비활성화
	ROWCOL nCol;
	int nCount = GetRowCount();
	if (nRow%2==1 && nRow+1<nCount) // I 단인 경우에만 , J 단이 존재하는 경우만
	{
		VERIFY(GetColumnNoFor(m_aTitle[3], nCol));
		value = GetValueRowCol(nRow, nCol);
		SetStyleRange(CGXRange(nRow+1, 6, nRow+1, 12), CGXStyle().SetEnabled(value == _T("X")));
	}
}

void CCMSectScaleFactorGrid::CoverByIJ(ROWCOL nRow)
{
	BOOL bI = (nRow%2==1);
	int aCols[] = {1, 3, 4, 13, 14};
	if (bI)
	{
		for (int i=0 ; i<5 ; ++i)
		{
			SetCoveredCellsRowCol(nRow, aCols[i], nRow+1, aCols[i], GX_UPDATENOW, gxRedo);
		}
	}
}

void CCMSectScaleFactorGrid::ConvStrToData(int i, CString& value, T_SECF_K& key, T_SECF_D& data)
{
	double* dFactor = data.dScaleFactor; // I or J
	if (i == 2)  ; // Nothing
	else if (i == 3) 
	{
		data.bSameIJ = TRUE;
		if (value.CompareNoCase(_T("X")) == 0)  { data.bSameIJ = FALSE; }
	}
	else if (i == 4) ; // Nothing
	else if (i == 5) dFactor[0] = _tstof(value);
	else if (i == 6) dFactor[1] = _tstof(value);
	else if (i == 7) dFactor[2] = _tstof(value);
	else if (i == 8) dFactor[3] = _tstof(value);
	else if (i == 9) dFactor[4] = _tstof(value);
	else if (i == 10) dFactor[5] = _tstof(value);
	else if (i == 11) dFactor[6] = _tstof(value);
	else if (i == 12) 
	{
		data.bBefore = FALSE; data.bAfter = FALSE;
		if      (value.CompareNoCase(_LS(IDS_CMD_SECF_BEFORE)) == 0)  { data.bBefore = TRUE; }
		else if (value.CompareNoCase(_LS(IDS_CMD_SECF_AFTER)) == 0)   { data.bAfter  = TRUE; }
		else if (value.CompareNoCase(_LS(IDS_CMD_SECF_ALL)) == 0)     { data.bBefore = TRUE; data.bAfter = TRUE; }
		else return;
	}
	else if (i ==13) 
	{
		data.GroupKey = GetBngrKey(value);
		m_pDoc->m_pAttrCtrl->MakeKeySecf(key.key.entity, data.GroupKey, key);
	}
	else ASSERT(0);
}

void CCMSectScaleFactorGrid::ConvDataToStr(int i, const T_SECF_K& key, const T_SECF_D& data, CString& value)
{
	const double* dFactor = data.dScaleFactor; // I or J
	if (i == 2) 
	{
		T_SECT_D SectD;
		if (!m_pDoc->m_pAttrCtrl->GetSect(key.key.entity, SectD)) ASSERT(0);
		value = SectD.SName;
	}
	else if (i == 3) value = (data.bSameIJ ? _T("O") : _T("X"));
	else if (i == 4) ; // nothing
	else if (i == 5) value.Format(DATA_FMT, dFactor[0]);
	else if (i == 6) value.Format(DATA_FMT, dFactor[1]);
	else if (i == 7) value.Format(DATA_FMT, dFactor[2]);
	else if (i == 8) value.Format(DATA_FMT, dFactor[3]);
	else if (i == 9) value.Format(DATA_FMT, dFactor[4]);
	else if (i == 10) value.Format(DATA_FMT, dFactor[5]);
	else if (i == 11) value.Format(DATA_FMT, dFactor[6]);
	else if (i == 12) 
	{
		T_SECT_D SectD;
		if (!m_pDoc->m_pAttrCtrl->GetSect(key.key.entity, SectD)) ASSERT(0);
		value = GetComposite(data.bBefore, data.bAfter);
	}
	else if (i == 13) value = GetBngrName(data.GroupKey);
	else ASSERT(0);
}

void CCMSectScaleFactorGrid::MakeSearchKey(const T_SECF_K &key, const T_SECF_D &data, CStringArray &aKey)
{
	int nColID;
	int nSortKeyNum = m_aSortInfo.GetSize();

	aKey.SetSize(nSortKeyNum);
	for (int i = 0; i < nSortKeyNum; i++)
	{
		nColID = m_aSortInfo[i].nRC;
		if (nColID == 0) aKey[i].Format(KEY_FMT, key.key.entity);
		else if (nColID == 1) aKey[i].Format(KEY_FMT, key.keymap);
		else ConvDataToStr(nColID, key, data, aKey[i]);
	}
}

void CCMSectScaleFactorGrid::GetAllSelectedSecf(CArray<T_SECF_K, T_SECF_K>& aSelKey)
{
	for (int i = 0; i < m_aSelectedKeys.GetSize(); i++)
		aSelKey.Add(m_aSelectedKeys[i]);
}

void CCMSectScaleFactorGrid::SetHeaderTitle(T_UNIT_SYSTEM &us, BOOL bInit)
{
	int nColSize[14] = {25, 0, 55, 25, 20, 33, 33, 33, 33, 33, 33, 33, 55, 60};

	CString csTitle;
	int nColCount = GetColCount();

	for (int i = 0; i < GetColCount(); i++)
	{
		SetStyleRange(CGXRange(0, i+1), CGXStyle()
			.SetValue(m_aTitle[i])
			.SetUserAttribute(ID_USER_ATTR_COL_ID, m_aTitle[i]));
		if (bInit) SetColWidthByDPI(i+1, i+1, nColSize[i]);
	}
	if (bInit) SetColWidthByDPI(0, 0, 20);
}

//----------------------------------------------------------------------------->
// 여기서 부터 Current Record의 위치 변경을 Detect하기 위해 삽입된 코드이다.
// Current Record가 변경되었는지 검사
/*
BOOL CCMSectScaleFactorGrid::CheckCurrentRecordChanged()
{
	ROWCOL ncRow, ncCol;
	GetCurrentCell(ncRow, ncCol);
	if (ncRow != m_nCurrentRow)
	{
		OnChangeCurrentRecord(ncRow);
		return TRUE;
	}
	return FALSE;
}*/

BOOL CCMSectScaleFactorGrid::ProcessKeys(CWnd *pSender, UINT nMessage, UINT nChar, UINT nRepCnt, UINT flags)
{
	ROWCOL nRow, nCol;
	BOOL bResult = FALSE;
	if (GetCurrentCell(nRow, nCol)) // 에러 처리한 부분 - ProcessKeys 안에 GetCurrentCell 에서 자꾸 에러가 나서 TRUE 일때만 실행
		bResult = CTBCommon::ProcessKeys(pSender, nMessage, nChar, nRepCnt, flags);
	
	//CheckCurrentRecordChanged();
	return bResult;
}

/*
void CCMSectScaleFactorGrid::OnRButtonDblClk(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	ROWCOL ncRowBefore, ncRowAfter, ncCol;
	if (!GetCurrentCell(ncRowBefore, ncCol)) return;

	CTBCommon::OnRButtonDblClk(nFlags, point);

	if (!GetCurrentCell(ncRowAfter, ncCol)) return;
	if (ncRowBefore != ncRowAfter) OnChangeCurrentRecord(ncRowAfter);
}
*/

/*
void CCMSectScaleFactorGrid::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	CTBCommon::OnLButtonDblClk(nFlags, point);
	//CheckCurrentRecordChanged();
}
*/

// 이 함수는 No Column을 클릭했을 때 Row Header를 클릭한 것과 같은 효과를
// 내기 위해서이다. CTBBrowserWndEx::OnLButtonDown 참조
//
// mylee 20070401 수정. SECF J단 추가에 관한 변경 관련하여 
// 맨 밑에단 클릭하며 ADD된경우 전체 삭제되었다 호출되는 과정에서 ERR 발생하여
// 이 함수에서 모두 처리해 주도록 수정. (아래부분 IF문 에러처리 참조)
void CCMSectScaleFactorGrid::OnLButtonDown(UINT nFlags, CPoint point)
{
	// 1. 클릭한 Row, Col 을 얻어오는 부분
	ROWCOL nhRow, nhCol, nkCol;
	int ht = HitTest(point, &nhRow, &nhCol);

	if ( nhCol==1 || nhCol==4 || nhCol==13 || nhCol==14 ) // Covered 클릭된 경우 실제 클릭된 셀은 하나 위 - 수정중 클릭시 Error가 나서 추가한 코드
	{
		if ( nhRow%2 == 0 ) --nhRow; 
	}

	if (ht != GX_NOHIT && ht != GX_VERTLINE)
	{
		GetColumnNoFor(1, nkCol);
		if (nhCol == nkCol && IsFrozenCol(nhCol)) // key column이면
		{
			CRect r = CalcRectFromRowColEx(nhRow, 0, nhRow, 0);  
			point.x = (r.left+r.right)/2;
		}
	}

	// 2. 처리하는 부분  - 변경되었을 시만
	if (nhRow != m_nCurrentRow) 
	{
		m_nCurrentRow = nhRow;
		
		T_SECF_K key;
		int nCount = GetRowCount();
		if (nhRow < 1) // 아래 If 문 3개는 에러처리 위한 것
		{
			SetCurrentCell(1, nhCol);
			m_nCurrentRow = 1;
		}
		else if (nhRow >= nCount)
		{
			SetCurrentCell(nCount, nhCol);
			m_nCurrentRow = nhRow;
		}
		else if (IsAppendRow(nhRow)) ; // do nothing
		else // 실제 처리하는 부분
		{
			ROWCOL nCol, nSubCol;
			
			VERIFY(GetColumnNoFor(GetKeyColID(), nCol));
			VERIFY(GetColumnNoFor(1, nSubCol)); // SNo column
			
			SetCurrentCell(nhRow, nhCol);
			key.key.entity = _ttoi(GetValueRowCol(nhRow, nhCol));
			key.keymap = _ttoi(GetValueRowCol(nhRow, nSubCol));

			m_pParent->OnChangeCurrentSecf(key);
		}
	}

	CTBCommon::OnLButtonDown(nFlags, point);
}

/*
// LBUTTONDOWN 에서 처리하는 것으로 수정
void CCMSectScaleFactorGrid::OnChangeCurrentRecord(ROWCOL nRow)
{
	m_nCurrentRow = nRow;

	// Append Row이면 그냥 리턴한다.
	T_SECF_K key;
	int nCount = GetRowCount();
	if (nRow < 1 || nRow >= nCount || IsAppendRow(nRow))
	{
		return;
		//key.keymap = 0;
	}
	else
	{
		// Key값을 알아와서 Parent Dialog에 Row의 변동을 알린다.
		ROWCOL nCol, nSubCol;
				
		VERIFY(GetColumnNoFor(GetKeyColID(), nCol));
		VERIFY(GetColumnNoFor(1, nSubCol)); // SNo column
		
		//SetCurrentCell(nRow, nCol);
		key.key.entity = _ttoi(GetValueRowCol(nRow, nCol));
		key.keymap = _ttoi(GetValueRowCol(nRow, nSubCol));
	}
	m_pParent->OnChangeCurrentSecf(key);
}
*/

/*
void CCMSectScaleFactorGrid::OnChangeCurrentRecord()
{
	OnChangeCurrentRecord(m_nCurrentRow);
}
*/

CString CCMSectScaleFactorGrid::GetKeyString(ROWCOL nRow)
{
	// 2개 이상의 column이 key를 형성할 경우 override
	T_SECF_K key;

	ROWCOL nMain, nSub;
	GetColumnNoFor(GetKeyColID(), nMain);
	GetColumnNoFor(1, nSub);
	key.key.entity = _tstol(GetValueRowCol(nRow, nMain));
	key.keymap = _tstol(GetValueRowCol(nRow, nSub));

	CString sKey;
	sKey.Format(_T("%ld"), key);
	return sKey;
}

CString CCMSectScaleFactorGrid::GetKeyStringBeforeEdit(ROWCOL nRow)
{
	// 2개 이상의 column이 key를 형성할 경우 override
	T_SECF_K key;

	ROWCOL nMain, nSub;
	GetColumnNoFor(GetKeyColID(), nMain);
	GetColumnNoFor(1, nSub);
	key.key.entity = _tstol(GetValueRowColBeforeEdit(nRow, nMain));
	key.keymap = _tstol(GetValueRowColBeforeEdit(nRow, nSub));

	CString sKey;
	sKey.Format(_T("%ld"), key);
	return sKey;
}

//<-----------------------------------------------------------------------------
BOOL CCMSectScaleFactorGrid::GetCurrentSecf(T_SECF_K& Key)
{
	ROWCOL ncRow, ncCol, nSubCol;
	if (!GetCurrentCell(ncRow, ncCol)) return FALSE;
	if (ncRow < 1) return FALSE;
	if (IsAppendRow(ncRow)) return FALSE;

	if (!GetColumnNoFor(GetKeyColID(), ncCol)) return FALSE;
	if (!GetColumnNoFor(1, nSubCol)) return FALSE;
	Key.key.entity = _ttoi(GetValueRowCol(ncRow, ncCol));
	Key.keymap = _ttoi(GetValueRowCol(ncRow, nSubCol));
	return TRUE;
}

/*
BOOL CCMSectScaleFactorGrid::SetCurrentSecf(T_SECF_K key, T_SECF_D& data)
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
*/


/////////////////////////////////////////////////////////////////////////
// 여기부터 Column 값 변경이 다른 column의 상태나 값을 변경시키는 경우를
// 처리해 주기 위해서 Override 및 새로 정의되는 함수이다.
// OnStartEditing, OnCanceledEditing, ChangeColumnStatus
// 일반적으로 OnModifyCell이 사용되나 Check Box Type일 경우는 OnStartEditing이
// 사용된다.

BOOL CCMSectScaleFactorGrid::OnStartEditing(ROWCOL nRow, ROWCOL nCol)
{
	// 부모의 OnModifyCell을 항상 먼저 호출해 주어야 한다.
	BOOL bResult = CTBCommon::OnStartEditing(nRow, nCol);
	ChangeColumnStatus(nRow, nCol);
	return bResult;
}

void CCMSectScaleFactorGrid::OnCanceledEditing(ROWCOL nRow, ROWCOL nCol)
{
	// 부모의 OnCanceledEditing을 항상 먼저 호출해 주어야 한다.
	CTBCommon::OnCanceledEditing(nRow, nCol);
	if (!IsEdit()) return;
	ChangeColumnStatus(nRow, nCol);
}

void CCMSectScaleFactorGrid::ChangeColumnStatus(ROWCOL nRow, ROWCOL nCol)
{
	ROWCOL nFlagCol;
	GetColumnNoFor(0, nFlagCol);

	if (nCol != nFlagCol) return;

	CString csFlag;
	int nNo;

	CGXControl *pControl;
	pControl = GetControl(nRow, nCol);
	if (nCol == nFlagCol)
		pControl->GetValue(csFlag);
	else
		csFlag = GetValueRowCol(nRow, nFlagCol);
	nNo = _tstol(csFlag);

	// Check Box는 StartEditing이므로 값이 변경되기 전이다.
	// Toggle형식이므로 반대로 설정해주면 원하는 대로 된다.
	ChangeSectionID(nRow, !nNo);
}

void CCMSectScaleFactorGrid::ChangeSectionID(ROWCOL nRow, int nID)
{
	ROWCOL nCol;
	GetColumnNoFor(2, nCol);

	CString csValue(_T(""));

	T_SECT_D SectD;
	if (m_pDoc->m_pAttrCtrl->GetSect(nID, SectD)) csValue = SectD.SName;
	SetValueRange(CGXRange(nRow, nCol), csValue);
}

void CCMSectScaleFactorGrid::SetBngrColumn(ROWCOL nCol)
{
	CString szChoiceList, tmp;
	T_STAG_K CurStag = m_pDoc->m_pStagCtrl->GetCurStag();
	if (CurStag != 0) szChoiceList = _T("");
	else szChoiceList.Format(_T("%s\n"), D_BNDR_GROUP_DEFAULT);

	CArray<CString, CString&> aBngrNameList;
	m_pDoc->m_pAttrCtrl->GetBngrList(aBngrNameList);

	T_BNGR_K BngrKey;
	int nSize = aBngrNameList.GetSize();
	for (int i = 0; i < nSize; i++)
	{
		if (CurStag != 0)
		{
			BngrKey = m_pDoc->m_pAttrCtrl->GetBngrKey(aBngrNameList[i]);
			if (!m_pDoc->m_pStagCtrl->IsActiveBngrKey(BngrKey)) continue;
		}
		tmp.Format(_T("%s\n"), aBngrNameList[i]);
		szChoiceList += tmp;
	}

	SetStyleRange(CGXRange().SetCols(nCol), CGXStyle()
			.SetControl(GX_IDS_CTRL_TABBED_COMBOBOX)
			.SetChoiceList(szChoiceList)
			.SetUserAttribute(GX_IDS_UA_TABLIST_KEYCOL, _T("0"))
			.SetUserAttribute(GX_IDS_UA_TABLIST_TEXTCOL, _T("0"))
			.SetUserAttribute(GX_IDS_UA_TABLIST_SHOWALLCOLS, _T("1"))
			.SetValueType(GX_VT_STRING)
			.SetHorizontalAlignment(DT_LEFT)
			.SetValue(_T(""))
			.SetWrapText(FALSE)
			);
	ROWCOL ncRow, ncCol;
	GetCurrentCell(ncRow, ncCol);
	if (ncCol == nCol) CGXGridWnd::SetCurrentCell(ncRow, ncCol);
}

void CCMSectScaleFactorGrid::SetCompositeColumn(ROWCOL nCol)
{
	CString szChoiceList, tmp;
	szChoiceList.Format(_T("%s\n"), _LS(IDS_CMD_SECF_BEFORE));

	CString strTmp;
	CArray<CString, CString&> aCompositeNameList;
	strTmp = _LS(IDS_CMD_SECF_AFTER);   aCompositeNameList.Add(strTmp);
	strTmp = _LS(IDS_CMD_SECF_ALL);     aCompositeNameList.Add(strTmp);

	//T_BNGR_K BngrKey;
	int nSize = aCompositeNameList.GetSize();
	for (int i = 0; i < nSize; ++i)
	{
		tmp.Format(_T("%s\n"), aCompositeNameList[i]);
		szChoiceList += tmp;
	}

	SetStyleRange(CGXRange().SetCols(nCol), CGXStyle()
			.SetControl(GX_IDS_CTRL_TABBED_COMBOBOX)
			.SetChoiceList(szChoiceList)
			.SetUserAttribute(GX_IDS_UA_TABLIST_KEYCOL, _T("0"))
			.SetUserAttribute(GX_IDS_UA_TABLIST_TEXTCOL, _T("0"))
			.SetUserAttribute(GX_IDS_UA_TABLIST_SHOWALLCOLS, _T("1"))
			.SetValueType(GX_VT_STRING)
			.SetHorizontalAlignment(DT_LEFT)
			.SetValue(_T(""))
			);
	ROWCOL ncRow, ncCol;
	GetCurrentCell(ncRow, ncCol);
	if (ncCol == nCol) CGXGridWnd::SetCurrentCell(ncRow, ncCol);
}

T_BNGR_K CCMSectScaleFactorGrid::GetBngrKey(CString& BngrName)
{
	if (BngrName.CompareNoCase(D_BNDR_GROUP_DEFAULT) == 0) return 0;
	return m_pDoc->m_pAttrCtrl->GetBngrKey(BngrName);
}

CString CCMSectScaleFactorGrid::GetBngrName(T_BNGR_K BngrKey)
{
	if (BngrKey == 0) return D_BNDR_GROUP_DEFAULT;
	T_BNGR_D data;
	if (m_pDoc->m_pAttrCtrl->GetBngr(BngrKey, data))
		return data.GroupName;
	return _T("");
}

CString CCMSectScaleFactorGrid::GetComposite(BOOL bBefore, BOOL bAfter)
{
	// int nStype = SectData.nStype;
	//BOOL bIsComposite = (nStype==D_SECT_TYPE_COMPO_B || nStype==D_SECT_TYPE_COMPO_I ||
	//                     nStype==D_SECT_TYPE_COMPO_CI || nStype==D_SECT_TYPE_COMPO_CT);

	if (bBefore && !bAfter)       return CString(_LS(IDS_CMD_SECF_BEFORE));
	else if (!bBefore && bAfter)  return CString(_LS(IDS_CMD_SECF_AFTER));
	else if (bBefore && bAfter)   return CString(_LS(IDS_CMD_SECF_ALL));
	else ASSERT(0);

	return CString(_LS(IDS_CMD_PSC_GRID_None)); // Exceptional Case (!bBefore && !bAfter)
}

void CCMSectScaleFactorGrid::OnModifyCell(ROWCOL nRow, ROWCOL nCol)
{
	// 부모의 OnModifyCell을 항상 먼저 호출해 주어야 한다.
	CTBCommon::OnModifyCell(nRow, nCol);
	
	if( !(nCol==1 || nCol==4) ) return;
	
	OnOffBeforeAfter(nRow); // Before/After 항목 활성/비활성화
	
	// Section 이름 세팅
	ROWCOL nKeyCol=0;
	VERIFY(GetColumnNoFor(GetKeyColID(), nKeyCol));
	CString value = GetValueRowCol(nRow, nKeyCol);
	T_SECT_K SectK = _tstol(value);

	T_SECT_D SectD;
	CGXStyle style; 
	style.Free();
	if (m_pDoc->m_pAttrCtrl->GetSect(SectK, SectD))
	{
		style.SetValue(SectD.SName);
		SetStyleRange(CGXRange(nRow, 3), style);
	}
}

// CTBCommon Override
BOOL CCMSectScaleFactorGrid::OnPasteFromClipboard(const CGXRange &range)
{
	if (IsLockEdit()) return FALSE; // display only mode이면 바로 리턴

	// 임시로 CGXGridWnd를 하나 만들어 Paste를 한 후 작업을 한다.
	CGXGridWnd x;
	int nPasteMode;
	int nPastedCols;

	// 임시 GridWnd에 Paste 작업을 한다.
	if (!PasteFromClipboard(range, GetDefValueArray(), GetKeyColID(), 
													x, nPasteMode, nPastedCols))
	{
		x.DestroyWindow();
		DisplayWarningText();
		return FALSE;
	}
	CStringArray value;
	value.SetSize(GetColCount());
	CArray<void*, void*> aKey;
	CArray<void*, void*> aData;
	CRowColArray aCols;
	aCols.SetSize(GetColCount());
	int nColCount = GetColCount();
	for (int i = 0; i < nColCount; i++)
		VERIFY(GetColumnNoFor(i, aCols[i]));

	// DB 수정이 되지 않는 경우이므로 Copy되는 Field Validation만 수행한다.
	if (nPasteMode == TB_PASTE_MODE_ONMODIFY)
	{
		int nColID;
		for (int j = range.left; j < range.left+nPastedCols; j++)
		{
			VERIFY(GetColumnIDFor(j, nColID));
			value[j-1] = x.GetValueRowCol(1, j);
			if (!ValidateField(value[j-1], nColID))
			{
				x.DestroyWindow();
				DisplayWarningText();
				return FALSE;
			}
		}
	}
	else // DB에 추가 또는 수정이 일어나는 경우이므로 Record Validation을 한다.
	{
		int nxRowCount = x.GetRowCount();
		for (int i = 1; i <= nxRowCount; i++)
		{
			// Table에서 레코드 값을 가져온다.
			int nxColCount = x.GetColCount();
			for (int j = 1; j <= nxColCount; j++)
				value[j-1] = x.GetValueRowCol(i, j);
			// String Array에 대해 Validation 검사. (필드, 레코드 전체)
			if (!MakeKeyAndDataArray(value, aCols, aKey, aData))
			{
				DeleteKeyAndDataArray(aKey, aData);
				x.DestroyWindow();
				if (m_sWarningText.IsEmpty())
					SetWarningText(_T("Invalid data. Operation has been canceled."));
				DisplayWarningText();
				return FALSE;
			}
		}
	}
	
	// 모드에 따라 Append 또는 Modify를 한다.
	if (nPasteMode == TB_PASTE_MODE_APPEND) // DB에 추가된다.
	{
		AddToDB(aKey, aData);
	}
	else if (nPasteMode == TB_PASTE_MODE_ONMODIFY)  // DB 수정 없다.
	{
		// 직접 그리드를 수정한다.
		CArray<int, int> aColID;
		aColID.SetSize(nPastedCols);
		for (int j = 0; j < nPastedCols; j++)
			VERIFY(GetColumnIDFor(range.left+j, aColID[j]));
		qsort((void*)aColID.GetData(), aColID.GetSize(), sizeof(UINT), CCompFunc::intAsc);
		ROWCOL nColNo;
		for (int j = 0; j < nPastedCols; j++)
		{
			VERIFY(GetColumnNoFor(aColID[j], nColNo));
			SetExpressionRowCol(range.top, nColNo, value[nColNo-1]);
			OnModifyCell(range.top, nColNo);
		}
	}   
	else if (nPasteMode == TB_PASTE_MODE_MODIFY) // DB가 수정된다.
	{
		CArray<T_SECF_K, T_SECF_K> aSecfKeyList;
		m_pDoc->m_pAttrCtrl->GetSecfKeyList(aSecfKeyList);

		// 기존 key를 가져온다.
		CStringArray aOldKey;
		aOldKey.SetSize(x.GetRowCount());
		int nxRowCount = x.GetRowCount();
		for (int i = 0; i < nxRowCount; i++)
		{
			for (int j=0 ; j<aSecfKeyList.GetSize() ; ++j)
			{
				if (_tstol(GetValueRowCol(range.top+i, aCols[1])) == aSecfKeyList[j].keymap) 
				{
					aOldKey[i].Format(KEY_FMT, aSecfKeyList[j].keymap);
				}
			}
		}
		ModifyToDB(aOldKey, aKey, aData);
	}
	DeleteKeyAndDataArray(aKey, aData);

	// 디버그용. 디버그 할려면 x는 static으로 선언하고
	// 밑에 두즐을 넣고 그 아래 x.DestroyWindows는 코멘트 처리
	//x.LockUpdate(FALSE);
	//((CGXTabWnd*)GetParent())->AttachWnd(&x, _T("Paste"));
	x.DestroyWindow();
	return FALSE;   // 실제 Paste는 하지 않는다.
}