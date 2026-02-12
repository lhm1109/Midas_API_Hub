// CMStageSpreadGrid.cpp : implementation file
//
// 2003. 10. 09   by TAE
// Key | Step | Type | Material | Story(to) | Load Start | Load End | Stage Duration | Member Age
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMStageSpreadGrid.h"
#include "CMLoadSTDGWizardDlgNew.h"

#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_base\wg_base_CompFunc.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define EPSILON 1.e-12
#define DATA_FMT _T("%.15g")

#define D_STORY_NONE        _LS(IDS_WG_CMD__ADDD__None)
#define D_MODEL_MATL_TYPE   _LS(IDS_CMD_ESQW_Material_Type)
#define D_MODEL_MATL_NAME   _LS(IDS_CMD_ESQW_Material_Name)
#define D_MODEL_MATL_ALL    _LS(IDS_CMD_ESQW_Material_All)
#define D_MODEL_MATL_STEEL  _LS(IDS_CMD_ESQW_Material_Steel)
#define D_MODEL_MATL_CONC   _LS(IDS_CMD_ESQW_Material_Concrete)
#define D_MODEL_MATL_SRC    _LS(IDS_CMD_ESQW_Material_SRC)
#define D_MODEL_MATL_USER   _LS(IDS_CMD_ESQW_Material_User)

/////////////////////////////////////////////////////////////////////////////
// CCMStageSpreadGrid
CString CCMStageSpreadGrid::m_aCurDefVal[] = {_T(""), _T(""), _T(""), _T(""), _T(""), _T(""), _T("0"), _T("0")};
CCMStageSpreadGrid::CCMStageSpreadGrid()
{
	m_CurKey = 1;
	m_KeyMapper.RemoveAll();
	m_SubKeyMapper.RemoveAll();
	m_aStepNum.RemoveAll();
	m_aData.RemoveAll();

	AddColInfo(_LS(IDS_CMD_ESQW_Key),         t_integer, 0);
	AddColInfo(_LS(IDS_CMD_ESQW_Step),        t_integer, 0);
	AddColInfo(_LS(IDS_CMD_ESQW_Model_Type),  t_string , 0);
	AddColInfo(_LS(IDS_CMD_ESQW_Model_Matl),  t_string , 0);
	AddColInfo(_LS(IDS_CMD_ESQW_Model_Stor),  t_string , 0);
	AddColInfo(_LS(IDS_CMD_ESQW_Load_Start),  t_string , 0);
	AddColInfo(_LS(IDS_CMD_ESQW_Load_End),    t_string , 0);
	AddColInfo(_LS(IDS_CMD_ESQW_Stag_Durt),   t_real   , 0);
	AddColInfo(_LS(IDS_CMD_ESQW_Memb_Age),    t_real   , 0);

	m_aPrimaryKey.Add(0);

	CString aCurDefVal[] = {_T(""), _T(""), _T(""), _T(""), _T(""), D_STORY_NONE, _T("0"), _T("0")};
	for(int i=0; i<sizeof(aCurDefVal)/sizeof(CString); i++)
		m_aCurDefVal[i] = aCurDefVal[i];
}

CCMStageSpreadGrid::~CCMStageSpreadGrid()
{
}


BEGIN_MESSAGE_MAP(CCMStageSpreadGrid, CTBCommon)
	//{{AFX_MSG_MAP(CCMStageSpreadGrid)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CCMStageSpreadGrid message handlers

void CCMStageSpreadGrid::Initialize()
{
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
	SetRowHeightByDPI(0, 0, 36);
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
	//GetParam()->SetNumberedRowHeaders(FLASE);

	// key, step
	SetStyleRange(CGXRange().SetCols(1, 2), CGXStyle()
		.SetValueType(GX_VT_NUMERIC)
		.SetFormat(GX_FMT_FIXED)
		.SetPlaces(0));
	HideColsByIntend(1, 1, TRUE);

	// model type
	CString csChoiceList(_T(""));
	csChoiceList.Format(_T("%s\n%s\n"), D_MODEL_MATL_TYPE, D_MODEL_MATL_NAME);
	SetStyleRange(CGXRange().SetCols(3), CGXStyle()
		.SetControl(GX_IDS_CTRL_TABBED_COMBOBOX)
		.SetChoiceList(csChoiceList)
		.SetUserAttribute(GX_IDS_UA_TABLIST_KEYCOL, _T("0"))
		.SetUserAttribute(GX_IDS_UA_TABLIST_TEXTCOL, _T("0"))
		.SetUserAttribute(GX_IDS_UA_TABLIST_SHOWALLCOLS, _T("1"))
		.SetValueType(GX_VT_STRING)
		.SetHorizontalAlignment(DT_LEFT)
		.SetValue(_T(""))
		);

	// model material
	SetStyleRange(CGXRange().SetCols(4), CGXStyle()
		.SetControl(GX_IDS_CTRL_TABBED_COMBOBOX)
		.SetUserAttribute(GX_IDS_UA_TABLIST_KEYCOL, _T("0"))
		.SetUserAttribute(GX_IDS_UA_TABLIST_TEXTCOL, _T("0"))
		.SetUserAttribute(GX_IDS_UA_TABLIST_SHOWALLCOLS, _T("1"))
		.SetValueType(GX_VT_STRING)
		.SetHorizontalAlignment(DT_LEFT)
		.SetValue(_T(""))
		);
	//SetModelMaterialColumn(4, 1);

	// model story
	SetModelStoryColumn(5, FALSE);

	// load start
	SetStyleRange(CGXRange().SetCols(6), CGXStyle()
		.SetHorizontalAlignment(DT_LEFT)
		.SetValueType(GX_VT_STRING)
		.SetEnabled(FALSE));

	// load end
	SetModelStoryColumn(7, TRUE);

	// stage duration, Member Age
	SetStyleRange(CGXRange().SetCols(8, 9), CGXStyle()
		.SetValueType(GX_VT_NUMERIC)
		.SetFormat(GX_FMT_FIXED)
		.SetPlaces(2));

	// 현재 셀 설정
	SetRightValidCell(1, 0);
	GetParam()->SetHideCurrentCell(GX_HIDE_NEVER);

	// always show vertical scrollbar
	SetScrollBarMode(SB_VERT, gxnEnabled | gxnEnhanced);  

	// Sorting 정보 초기화
	m_aSortInfo.SetSize(2);
	m_aSortInfo[0].nRC = 1;
	m_aSortInfo[0].sortType = CGXSortInfo::numeric;
	m_aSortInfo[0].sortOrder = CGXSortInfo::ascending;
	m_aSortInfo[1].nRC = 0;
	m_aSortInfo[1].sortType = CGXSortInfo::numeric;
	m_aSortInfo[1].sortOrder = CGXSortInfo::ascending;

	LockUpdate(FALSE);
}

void CCMStageSpreadGrid::SetModelMaterialColumn(ROWCOL nRow, ROWCOL nCol, int nType)
{
	// nType : (1) Material Type (2) Material Name
	ASSERT(nType == 1 || nType == 2);

	CString csChoiceList(_T(""));
	if (nType == 1)
	{
		csChoiceList.Format(_T("%s\n%s\n%s\n%s\n%s\n"), 
			D_MODEL_MATL_ALL, D_MODEL_MATL_STEEL, D_MODEL_MATL_CONC,
			D_MODEL_MATL_SRC, D_MODEL_MATL_USER);
	}
	else if (nType == 2)
	{
		CString csMatl;
		T_MATL_D MatlD;
		CArray<T_MATL_K, T_MATL_K> aMatlK;
		m_pDoc->m_pAttrCtrl->GetMatlKeyList(aMatlK);
		int nNum = aMatlK.GetSize();
		for (int i=0; i<nNum; i++)
		{
			if (!m_pDoc->m_pAttrCtrl->GetMatl(aMatlK[i], MatlD)) continue;
			csMatl.Format(_T("%d: %s\n"), aMatlK[i], MatlD.Name);
			csChoiceList += csMatl;
		}
	}
	SetStyleRange(CGXRange(nRow, nCol), CGXStyle().
		SetChoiceList(csChoiceList)
		.SetValue(_T("")));
}

void CCMStageSpreadGrid::SetModelStoryColumn(ROWCOL nCol, BOOL bNone)
{
	CString csChoiceList(_T(""));

	T_STOR_D StorD;
	CArray<T_STOR_K, T_STOR_K> aStorK;
	m_pDoc->m_pAttrCtrl->GetStorKeyList(aStorK);
	if (bNone) 
	{ 
		csChoiceList += D_STORY_NONE; csChoiceList += _T("\n"); 
	}
	for (int i=1; i<aStorK.GetSize(); i++)
	{
		m_pDoc->m_pAttrCtrl->GetStor(aStorK[i], StorD);
		csChoiceList += StorD.StoryName;
		csChoiceList += _T("\n");
	}  
	SetStyleRange(CGXRange().SetCols(nCol), CGXStyle()
		.SetControl(GX_IDS_CTRL_TABBED_COMBOBOX)
		.SetChoiceList(csChoiceList)
		.SetUserAttribute(GX_IDS_UA_TABLIST_KEYCOL, _T("0"))
		.SetUserAttribute(GX_IDS_UA_TABLIST_TEXTCOL, _T("0"))
		.SetUserAttribute(GX_IDS_UA_TABLIST_SHOWALLCOLS, _T("1"))
		.SetValueType(GX_VT_STRING)
		.SetHorizontalAlignment(DT_LEFT)
		.SetValue(_T(""))
		);
}

BOOL CCMStageSpreadGrid::InsertRecordByKey(const T_KEYSTEP &key, const T_ESEQ_BASE &data)
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

BOOL CCMStageSpreadGrid::DeleteRecordByKey(const T_KEYSTEP &key, const T_ESEQ_BASE &data)
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
// CCMStageSpreadGrid Overridable Functions
void CCMStageSpreadGrid::OnChangedSelectionPublic(T_KEYSTEP *keys, int nSize)
{
	return;
}

long CCMStageSpreadGrid::GetSelectedKeys(CArray<T_KEYSTEP, T_KEYSTEP> &caKey)
{
	return 0;
}

BOOL CCMStageSpreadGrid::ValidateField(CString value, int nColID)
{
// Key | Step | Type | Material | Story(to) | Load Start | Load End | Stage Duration | Member Age

	if (nColID >= GetColCount())
	{
		SetWarningText(_LS(IDS_WG_CMD__ADDD___CSpfcItemGrid__ValidateField__Er));
		return FALSE;
	}

	int nErrCode;
	if (nColID == 0 || nColID == 1)
	{
		if (!IsValidLongNumber(value, nErrCode)) 
		{
			switch(nErrCode)
			{
				case 1: SetWarningText(_LS(IDS_WG_CMD__ADDD__Invalid_formula_)); break; 
				case 2: 
				case 4: SetWarningText(_LS(IDS_WG_CMD__ADDD__Integer_Is_Required)); break; 
				case 3: SetWarningText(_LS(IDS_WG_CMD__ADDD__Out_Value_Range)); break; 
			}
			return FALSE;
		}
		if (_tstof(value) < 0)
		{
			SetWarningText(_LS(IDS_WG_CMD__ADDD__Negative_value_is_not_allowed_));
			return FALSE;
		}
	}
	else if (nColID >= 2 && nColID <= 6) return TRUE;
	else if (nColID == 7 || nColID == 8)
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
		if (_tstof(value) < 0.0)
		{
			SetWarningText(_LS(IDS_WG_CMD__ADDD__Negative_value_is_not_allowed_));
			return FALSE;
		}
	}
	else return FALSE;

	return TRUE;
}

// 삽입이나 수정전에 Record의 Validation을 검사한다.
BOOL CCMStageSpreadGrid::ValidateRecord(CStringArray &value, CRowColArray &aCols)
{
	// 에러 메세지를 위한 해더 
	CString aTitle[] = { _LS(IDS_CMD_ESQW_Key), _LS(IDS_CMD_ESQW_Step), 
		_LS(IDS_CMD_ESQW_Model_Type2), _LS(IDS_CMD_ESQW_Model_Matl2), 
		_LS(IDS_CMD_ESQW_Model_Stor2), _LS(IDS_CMD_ESQW_Load_Start2), _LS(IDS_CMD_ESQW_Load_End2), 
		_LS(IDS_CMD_ESQW_Stag_Durt2), _LS(IDS_CMD_ESQW_Memb_Age2)};

	CString msg = _T("");
	int nPos;
	for (int i = 0; i < GetColCount(); i++)
	{
		if (i == 0 || i == 5) continue;
		if (i == 6 || i == 7 || i == 8)  // step당 하나만 들어가는 데이터 
		{
			nPos = aCols[1]-1;
			BOOL bExist = FALSE;
			T_KEYSTEP keystep;
			unsigned int key;
			int data;
			POSITION pos = m_KeyMapper.GetStartPosition();
			while (pos != NULL)
			{
				m_KeyMapper.GetNextAssoc(pos, key, data);
				keystep.keymap = key;
				if (keystep.key.serial == _tstol(value[nPos])) { bExist = TRUE; break; }
			}
			if (bExist) continue;
		}
		nPos = aCols[i]-1;
		if (value[nPos] == _T(""))
			msg += ((msg != _T("")) ? _T("\n") : _T("")) + aTitle[i] + _LS(IDS_WG_CMD__ADDD___has_not_been_entered_);
		else if (!ValidateField(value[nPos], i)) return FALSE;
	}
	if (msg == _T("")) return TRUE;
	else SetWarningText(msg);
	return FALSE;
}

// 이 함수는 Paste할 때 호출되므로 False를 리턴할 때는 ValidateField와 
// Validate Record에서 설정된 Warning Text를 없애주고 리턴한다.
BOOL CCMStageSpreadGrid::ValidateAndMakeRecord(CStringArray &value, CRowColArray &aCols, T_KEYSTEP &key, T_ESEQ_BASE &data)
{
	// 레코드에 대해 Validation을 검사한다.
	if (!ValidateRecord(value, aCols))
		return FALSE;

	// 레코드를 만들어서 리턴한다.
	int nPos;
	for (int i = 0; i < GetColCount(); i++)
	{
		nPos = aCols[i]-1; // col id i에 해당하는 value가 저장된 위치
		if(i==0) 
		{
			if (_tstol(value[nPos]) == 0) key.key.entity = _tstol(GetNewKey());
			else key.key.entity = _tstol(value[nPos]);
		}
		else if(i==1) key.key.serial = _tstol(value[nPos]);
		else ConvStrToData(i, value[nPos], data);
	}
	return TRUE;
}

BOOL CCMStageSpreadGrid::AddToDB(ROWCOL nRow)
{
	// Record를 만들어서 DB에 입력한다.
	T_KEYSTEP key;
	T_ESEQ_BASE data;
 
	GetValue(nRow, key, data);

	return AddData(key, data);
}

BOOL CCMStageSpreadGrid::AddToDB(CArray<void*, void*>& raKey, CArray<void*, void*>& raData)
{
	ASSERT(raKey.GetSize() == raData.GetSize());
	T_KEYSTEP tKey;
	CArray<T_KEYSTEP, T_KEYSTEP> aKey;
	CArray<T_ESEQ_BASE, T_ESEQ_BASE&> aData;
	for (int i = 0; i < raKey.GetSize(); i++)
	{
		tKey = *((T_KEYSTEP*)raKey.GetAt(i));
		aKey.Add(tKey);
		aData.Add(*((T_ESEQ_BASE*)raData.GetAt(i)));
	}
	return AddData(aKey, aData);
}

BOOL CCMStageSpreadGrid::DeleteFromDB(ROWCOL nRow)
{
	T_KEYSTEP key;
	T_ESEQ_BASE data;

	GetValue(nRow, key, data);

	return DelData(key);
}

BOOL CCMStageSpreadGrid::DeleteFromDB(CRowColArray& awRows)
{
	if (awRows.GetSize() <= 0) return TRUE;

	ROWCOL nCol1, nCol2;
	VERIFY(GetColumnNoFor(0, nCol1));
	VERIFY(GetColumnNoFor(1, nCol2));

	CString value;
	T_KEYSTEP key;
	CArray<T_KEYSTEP, T_KEYSTEP> awKeys;
 
	for (int i = 0; i < awRows.GetSize(); i++)
	{
		value = GetValueRowColBeforeEdit(awRows.GetAt(i), nCol1);
		key.key.entity = _tstol(value);
		value = GetValueRowColBeforeEdit(awRows.GetAt(i), nCol2);
		key.key.serial = _tstol(value);
		awKeys.Add(key);
	}

	return DelData(awKeys);
}

BOOL CCMStageSpreadGrid::ModifyToDB(ROWCOL nRow)
{
	ROWCOL nCol1, nCol2;
	VERIFY(GetColumnNoFor(0, nCol1));
	VERIFY(GetColumnNoFor(1, nCol2));

	T_KEYSTEP key, oldKey;
	T_ESEQ_BASE data;
	
	oldKey.key.entity = _tstol(GetValueRowColBeforeEdit(nRow, nCol1));
	oldKey.key.serial = _tstol(GetValueRowColBeforeEdit(nRow, nCol2));

	GetValue(nRow, key, data);

	// 수정한다. 실패하면 에러 리턴
	return ModifyData(oldKey, key, data); 
}

BOOL CCMStageSpreadGrid::ModifyToDB(CStringArray& raOldKey, 
												 CArray<void*, void*>& raKey, 
												 CArray<void*, void*>& raData)
{
	ASSERT(raKey.GetSize() == raData.GetSize());
	ASSERT(raKey.GetSize() == raOldKey.GetSize());
	CArray<T_KEYSTEP, T_KEYSTEP> aOldKey, aKey;
	CArray<T_ESEQ_BASE, T_ESEQ_BASE&> aData;
	T_KEYSTEP Key;
	int nSize = raKey.GetSize();
	for (int i = 0; i < nSize; i++)
	{
		Key.keymap = _tstol(raOldKey.GetAt(i)); 
		aOldKey.Add(Key);
		aKey.Add(*((T_KEYSTEP*)raKey.GetAt(i)));
		aData.Add(*((T_ESEQ_BASE*)raData.GetAt(i)));
	}
	return ModifyData(aOldKey, aKey, aData);
}

CString CCMStageSpreadGrid::GetNewKey()
{
	CString value;
	value.Format(_T("%ld"), m_CurKey);
	m_CurKey++;
	return value;
}

CString CCMStageSpreadGrid::GetDefValue(int nIndex)
{
	return m_aCurDefVal[nIndex];
}

CString* CCMStageSpreadGrid::GetDefValueArray()
{
	return m_aCurDefVal;
}

BOOL CCMStageSpreadGrid::MakeKeyAndDataArray(CStringArray& value, CRowColArray& awCols, 
																	CArray<void*, void*>& aKey, CArray<void*, void*>& aData)
{
	T_KEYSTEP* pKey;
	T_ESEQ_BASE* pData;
	pKey = new T_KEYSTEP;
	pData = new T_ESEQ_BASE;
	aKey.Add((void*)pKey);
	aData.Add((void*)pData);
	if (!ValidateAndMakeRecord(value, awCols, *pKey, *pData)) return FALSE;
	return TRUE;
}

void CCMStageSpreadGrid::DeleteKeyAndDataArray(CArray<void*, void*>& aKey, CArray<void*, void*>& aData)
{
	T_KEYSTEP* pKey;
	T_ESEQ_BASE* pData;
	ASSERT(aKey.GetSize() == aData.GetSize());
	for (int i = 0; i < aKey.GetSize(); i++)
	{
		pKey = (T_KEYSTEP*)aKey.GetAt(i);
		pData = (T_ESEQ_BASE*)aData.GetAt(i);
		delete pKey;
		delete pData;
	}
}

long CCMStageSpreadGrid::GetCountData()
{
	// 총 데이터 개수 = 스테이지개수 * 스테이지별 스텝 개수 
	int nTotal=0;
	for (int i=0; i<m_aData.GetSize(); i++)
	{
		nTotal += m_aStepNum[i];
	}
	return nTotal;
}

void CCMStageSpreadGrid::MakeItemFiltered(long nFilteredNum, long* aFilteredKeys, CRowColArray& awCols)
{
	ASSERT(0); // no filtering
}

BOOL CCMStageSpreadGrid::GetDataIndex(int nCount, int &nIndex, int &nSubIndex)
{
	int n = 0;
	int i = 0;
	for (i = 0; i < m_aStepNum.GetSize(); i++)
	{
		n += m_aStepNum[i];
		if (n >= nCount) break;
	}
	nIndex = i;
	nSubIndex = m_aStepNum[i]-(n-nCount)-1;
	if (nSubIndex < 0) return FALSE;
	return TRUE;
}

BOOL CCMStageSpreadGrid::GetKeyByIndex(int nIndex, int nSubIndex, T_KEYSTEP &rKey)
{
	unsigned int key=0;
	int ix=0, subix=0;
	POSITION pos1, pos2;
	T_KEYSTEP keystep, subkeystep;

	pos1 = m_KeyMapper.GetStartPosition();
	while (pos1 != NULL)
	{
		m_KeyMapper.GetNextAssoc(pos1, key, ix);
		if (ix == nIndex)
		{
			keystep.keymap = key;
			pos2 = m_SubKeyMapper.GetStartPosition();
			while (pos2 != NULL)
			{
				m_SubKeyMapper.GetNextAssoc(pos2, key, subix);
				subkeystep.keymap = key;
				if (subkeystep.key.entity == keystep.key.entity && subix == nSubIndex)
				{
					rKey.keymap = key;
					return TRUE;
				}
			}
		}
	}
	return FALSE;
}

void CCMStageSpreadGrid::MakeItemNormal(long nNumData, CRowColArray& awCols)
{
	int ix, subix;
	T_KEYSTEP key;
	T_ESEQ_BASE data, tmpData;
	for(long nCount=0 ; nCount < nNumData; nCount++)
	{
		if (!GetDataIndex(nCount+1, ix, subix)) ASSERT(0);
		if (!GetKeyByIndex(ix, subix, key)) ASSERT(0);
		tmpData = m_aData.GetAt(ix);
		data = tmpData;
		data.EseqSel[0].nType = tmpData.EseqSel[subix].nType;
		data.EseqSel[0].KeyMatl = tmpData.EseqSel[subix].KeyMatl;
		data.EseqSel[0].KeyStorModel = tmpData.EseqSel[subix].KeyStorModel;
		SetValue(GetParam()->GetData(), nCount+1, key, data, awCols);
	} // end of loop nCount
}

void CCMStageSpreadGrid::MakeItemEditingRecord(ROWCOL ncRow, CStringArray& EditingRecord)
{
	CDBDoc* pDoc = m_pDoc;
	T_KEYSTEP Key;
	T_ESEQ_BASE Data;
	for (int i = 0; i < GetColCount(); i++)
	{
		if (i == 0) Key.key.entity = _tstol(EditingRecord[i]);
		else if (i == 1) Key.key.serial = _tstol(EditingRecord[i]);
		else ConvStrToData(i, EditingRecord[i], Data);
	}
	// 현재 값을 설정한다.
	SetValue(ncRow, Key, Data);
}

/////////////////////////////////////////////////////////////////////////////
// CCMStageSpreadGrid Implementation
BOOL CCMStageSpreadGrid::GetValue(ROWCOL nRow, T_KEYSTEP &key, T_ESEQ_BASE &data)
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

		if(i==0) key.key.entity = _tstol(str);
		else if(i==1) key.key.serial = _tstol(str);
		else ConvStrToData(i, str, data);
	}
	return TRUE;
}

BOOL CCMStageSpreadGrid::SetValue(ROWCOL nRow, const T_KEYSTEP &key, const T_ESEQ_BASE &data)
{
	//
	CRowColArray awCols;
	awCols.SetSize(GetColCount());
	for (int i = 0;i < GetColCount(); i++)
		VERIFY(GetColumnNoFor(m_aTitle[i], awCols[i]));
	return SetValue(GetParam()->GetData(), nRow, key, data, awCols);
}

BOOL CCMStageSpreadGrid::SetValue(CGXData* pData, ROWCOL nRow, const T_KEYSTEP &key, const T_ESEQ_BASE &data, CRowColArray &awCols)
{
	CString str;
	ChangeModelType(nRow, data.EseqSel[0].nType);

	for (int i = 0; i < GetColCount(); i++)
	{
		if(i==0)  // key
		{
			if (key.key.entity == 0) str = GetNewKey();
			else str.Format(_T("%ld"), key.key.entity);
		}
		else if(i==1) str.Format(_T("%ld"), key.key.serial);
		else ConvDataToStr(i, data, str);
		pData->StoreValueRowCol(nRow, awCols[i], str, gxOverride, GX_VT_NUMERIC);
	}
	ChangeModelStep(nRow, key);
	SetStartStoryName();

	//ROWCOL nStepCol;
	//GetColumnNoFor(1, nStepCol);
	//SetStyleRange(CGXRange(nRow, nStepCol), CGXStyle().SetEnabled(FALSE));

	return TRUE;
}

void CCMStageSpreadGrid::ConvStrToData(int i, CString& value, T_ESEQ_BASE& data)
{ 
	if (i==2) data.EseqSel[0].nType = GetModelTypeData(value);
	else if (i==3) data.EseqSel[0].KeyMatl = GetModelMatlData(data.EseqSel[0].nType, value);
	else if (i==4) data.EseqSel[0].KeyStorModel = GetStoryKey(value);
	else if (i==5) ; // nothing
	else if (i==6) data.KeyStorLoad = GetStoryKey(value);
	else if (i==7) data.dDuration = _tstof(value);
	else if (i==8) data.dAge = _tstof(value);
	else ASSERT(0);
}

void CCMStageSpreadGrid::ConvDataToStr(int i, const T_ESEQ_BASE& data, CString& value)
{
	if (i==2) value = GetModelTypeStr(data.EseqSel[0].nType);
	else if (i==3) value = GetModelMatlStr(data.EseqSel[0].nType, data.EseqSel[0].KeyMatl);
	else if (i==4) value = GetStoryName(data.EseqSel[0].KeyStorModel);
	else if (i==5) value = _T(""); // nothing
	else if (i==6) value = GetStoryName(data.KeyStorLoad);
	else if (i==7) value.Format(DATA_FMT, data.dDuration);
	else if (i==8) value.Format(DATA_FMT, data.dAge); 
	else ASSERT(0);
}

void CCMStageSpreadGrid::MakeSearchKey(const T_KEYSTEP &key, const T_ESEQ_BASE &data, CStringArray &aKey)
{
	int nColID;
	int nSortKeyNum = m_aSortInfo.GetSize();

	aKey.SetSize(nSortKeyNum);
	for (int i = 0; i < nSortKeyNum; i++)
	{
		nColID = m_aSortInfo[i].nRC;
		if (nColID == 0) aKey[i].Format(_T("%ld"), key.key.entity);
		else if (nColID == 1) aKey[i].Format(_T("%ld"), key.key.serial);
		else ConvDataToStr(nColID, data, aKey[i]);
	}
}

void CCMStageSpreadGrid::SetHeaderTitle(T_UNIT_SYSTEM &us, BOOL bInit)
{
// Key | Step | Type | Material | Story(to) | Load Start | Load End | Stage Duration | Member Age
	int aSize[] = {20, 40, 80, 90, 75, 85, 85, 60, 60};
	int nColCount = GetColCount();
	for (int i = 0; i < nColCount; i++)
	{
		SetStyleRange(CGXRange(0, i+1), CGXStyle()
			.SetValue(m_aTitle[i])
			.SetUserAttribute(ID_USER_ATTR_COL_ID, m_aTitle[i]));
		if (bInit) SetColWidthByDPI(i+1, i+1, aSize[i]);
	}
	if (bInit) SetColWidthByDPI(0, 0, 25);
}

BOOL CCMStageSpreadGrid::FindData(T_KEYSTEP key, int &nIndex, int &nSubIndex)
{
	unsigned int k; 
	T_KEYSTEP kmap;
	int data;
	int ix = 0;
	POSITION pos = m_KeyMapper.GetStartPosition();

	while (pos != NULL)
	{
		m_KeyMapper.GetNextAssoc(pos, k, data);
		kmap.keymap = k;
		if (key.key.serial > kmap.key.serial) { ix++; continue; }
		else if (key.key.serial < kmap.key.serial) ; 
		else
		{
			nIndex = data;
			nSubIndex = m_aStepNum[data];
			return TRUE;
		}
	}

	int nTotal = 0;
	int i = 0;
	for (i=0; i<m_aStepNum.GetSize(); i++)
	{
		if (nTotal >= ix) break; 
		nTotal += m_aStepNum[i];
	}
	nIndex = i;
	nSubIndex = 0;

	return FALSE;
}

BOOL CCMStageSpreadGrid::ExistAllKey(CArray<T_KEYSTEP, T_KEYSTEP>& aKey, T_KEYSTEP& errorKey)
{
	CArray<T_KEYSTEP, T_KEYSTEP> aTempKey;

	int nSize = aKey.GetSize();
	aTempKey.Copy(aKey);
	BOOL bSame;
	int nIndex, nSubIndex;
	for (int i = 0; i < nSize; i++)
	{
		bSame = FindData(aTempKey[i], nIndex, nSubIndex);
		if (!bSame)
		{
			errorKey = aTempKey[i];
			return FALSE;
		}
	}
	return TRUE;
}

/*
BOOL CCMStageSpreadGrid::CheckStoryLevel(CArray<T_KEYSTEP, T_KEYSTEP> &aOldkey, CArray<T_KEYSTEP, T_KEYSTEP> &akey, 
																				 CArray<T_ESEQ_BASE, T_ESEQ_BASE&> &aData, BOOL bModify)
{

}
*/

BOOL CCMStageSpreadGrid::CheckStoryLevel(T_KEYSTEP oldkey, T_KEYSTEP key, T_ESEQ_BASE &data, BOOL bModify)
{
	T_STOR_K StorK;
	T_STOR_D StorD;
	double dCurLevel;
	int nIndex, nSubIndex;
	int nPre, nPost;
	BOOL bStoryLevel = TRUE;

	BOOL bExist = FindData(key, nIndex, nSubIndex);
	if (!m_pDoc->m_pAttrCtrl->GetStor(data.KeyStorLoad, StorD))
	{
		// None이거나 현 스텝이 존재하는 경우 
		if (data.KeyStorLoad == -1)
		{
			AfxMessageBox(_LS(IDS_CMD_ERR_End_Story_for_DL));
			return FALSE;
		}
		else return TRUE;
		/*
		if (bExist) return TRUE;
		else  
		{
			AfxMessageBox(_LS(IDS_CMD_ERR_End_Story_for_DL));
			return FALSE;
		}
		*/
	}
	dCurLevel = StorD.dStoryLevel; 

	// 무조건 Step 순서대로 Check를 해준다.
	if (bModify)  // 수정모드 일때 
	{
		if (key.key.entity > oldkey.key.entity) { nPre = nIndex-2; nPost = nIndex; }
		else { nPre = nIndex-1; nPost = nIndex+1; }
	}
	else { nPre = nIndex-1; nPost = nIndex; }  // 추가모드 일때 

	if (nPre >= 0) 
	{
		StorK = m_aData[nPre].KeyStorLoad;
		m_pDoc->m_pAttrCtrl->GetStor(StorK, StorD);
		if (dCurLevel - StorD.dStoryLevel <= 0.0) bStoryLevel = FALSE;
	}
	if (nPost < m_aData.GetSize())
	{
		StorK = m_aData[nPost].KeyStorLoad;
		m_pDoc->m_pAttrCtrl->GetStor(StorK, StorD);
		if (dCurLevel - StorD.dStoryLevel >= 0.0) bStoryLevel = FALSE;
	}
	if (!bStoryLevel)
	{
		AfxMessageBox(_LS(IDS_CMD_ERR_End_Story_for_DL));
		return FALSE;
	}
	return TRUE;
}

void CCMStageSpreadGrid::AddKeyMapper(CMap<unsigned int, unsigned int, int, int>* pKeyMapper, unsigned int step, int ix, BOOL bSub/*=FALSE*/)
{
	ASSERT(pKeyMapper);
	unsigned int key;
	int data;
	T_KEYSTEP keystep;
	POSITION pos = pKeyMapper->GetStartPosition();
	while (pos != NULL)
	{
		pKeyMapper->GetNextAssoc(pos, key, data);
		if (bSub)
		{
			keystep.keymap = key;
			if (keystep.key.serial != step) continue;
		}
		if (data >= ix) pKeyMapper->SetAt(key, data+1);
	}
}

void CCMStageSpreadGrid::DelKeyMapper(CMap<unsigned int, unsigned int, int, int>* pKeyMapper, unsigned int step, int ix, BOOL bSub/*=FALSE*/)
{
	ASSERT(pKeyMapper);
	unsigned int key;
	int data;
	T_KEYSTEP keystep;
	POSITION pos = pKeyMapper->GetStartPosition();
	while (pos != NULL)
	{
		pKeyMapper->GetNextAssoc(pos, key, data);
		if (bSub)
		{
			keystep.keymap = key;
			if (keystep.key.serial != step) continue;
		}
		if (data > ix) pKeyMapper->SetAt(key, data-1);
	}
}

BOOL CCMStageSpreadGrid::AddData(T_KEYSTEP key, T_ESEQ_BASE &data, BOOL bCheck/*=TRUE*/)
{
	CArray<T_KEYSTEP, T_KEYSTEP> aOldKey, aKey;
	CArray<T_ESEQ_BASE, T_ESEQ_BASE&> aData;
	aKey.Add(key);
	aData.Add(data);
 
	// 동일한 key값 중복 검토
	int nIndex, nSubIndex;
	if (m_KeyMapper.Lookup(key.keymap, nIndex) || m_SubKeyMapper.Lookup(key.keymap, nSubIndex))
	{
		CString msg;
		msg.Format(_LS(IDS_CMD_Data_already_exist));
		AfxMessageBox(msg);
		return FALSE;
	}
	T_KEYSTEP oldkey;
	oldkey.keymap = 0;

	if (bCheck)
	{
		if (!CheckStoryLevel(oldkey, key, data, FALSE)) return FALSE;
	}

	T_ESEQ_BASE BaseData;
	if (FindData(key, nIndex, nSubIndex))   // 같은 스텝이 존재하는 경우 
	{
		BaseData = m_aData.GetAt(nIndex);
		BaseData.EseqSel[nSubIndex].nType = data.EseqSel[0].nType;
		BaseData.EseqSel[nSubIndex].KeyMatl = data.EseqSel[0].KeyMatl;
		BaseData.EseqSel[nSubIndex].KeyStorModel = data.EseqSel[0].KeyStorModel;
		m_aData.SetAt(nIndex,BaseData);
		m_aStepNum.SetAt(nIndex, nSubIndex+1);
		m_KeyMapper.SetAt(key.keymap, nIndex);
		AddKeyMapper(&m_SubKeyMapper, key.key.serial, nSubIndex, TRUE);
		m_SubKeyMapper.SetAt(key.keymap, nSubIndex);
	}
	else    // 새로 추가되는 스텝의 경우  
	{
		// 데이터를 추가하기 전에 Map 정리부터...
		m_aData.InsertAt(nIndex, data);
		m_aStepNum.InsertAt(nIndex, nSubIndex+1);
		AddKeyMapper(&m_KeyMapper, key.key.serial, nIndex, FALSE);
		m_KeyMapper.SetAt(key.keymap, nIndex);
		m_SubKeyMapper.SetAt(key.keymap, nSubIndex);
	}
	m_CurKey++;

	UpdateBuffer(0, aOldKey, aKey, aData);
	return TRUE;
}

BOOL CCMStageSpreadGrid::AddData(CArray<T_KEYSTEP, T_KEYSTEP> &aKey, CArray<T_ESEQ_BASE, T_ESEQ_BASE&> &aData, BOOL bNotify)
{
	// 동일한 key값 중복 검토
	T_KEYSTEP oldkey;
	oldkey.keymap = 0;
	int nSize = aKey.GetSize();
	for (int i = 0; i < nSize; i++)
	{
		if (!CheckStoryLevel(oldkey, aKey[i], aData[i], FALSE)) return FALSE;
	}
	
	/*
	CArray<T_KEYSTEP, T_KEYSTEP> aOldKey;
	aOldKey.Copy(aKey);
	if (!CheckStoryLevel(aOldKey, aKey, aData, FALSE)) return FALSE;
	*/
	
	int nIndex, nSubIndex;
	for (int i = 0; i < nSize; i++)
	{
		if (m_KeyMapper.Lookup(aKey[i].keymap, nIndex) || m_SubKeyMapper.Lookup(aKey[i].keymap, nSubIndex))
		{
			CString msg;
			msg.Format(_LS(IDS_CMD_Data_already_exist));
			AfxMessageBox(msg);
			return FALSE;
		}
	}
	 
	T_ESEQ_BASE BaseData;
	for (int i = 0; i < nSize; i++)
	{
		if (FindData(aKey[i], nIndex, nSubIndex))   // 같은 스텝이 존재하는 경우 
		{
			BaseData = m_aData.GetAt(nIndex);
			BaseData.EseqSel[nSubIndex].nType = aData[i].EseqSel[0].nType;
			BaseData.EseqSel[nSubIndex].KeyMatl = aData[i].EseqSel[0].KeyMatl;
			BaseData.EseqSel[nSubIndex].KeyStorModel = aData[i].EseqSel[0].KeyStorModel;
			m_aData.SetAt(nIndex, BaseData);
			m_aStepNum.SetAt(nIndex, nSubIndex+1);
			m_KeyMapper.SetAt(aKey[i].keymap, nIndex);
			AddKeyMapper(&m_SubKeyMapper, aKey[i].key.serial, nSubIndex, TRUE);
			m_SubKeyMapper.SetAt(aKey[i].keymap, nSubIndex);
		}
		else    // 새로 추가되는 스텝의 경우  
		{
			m_aData.InsertAt(nIndex, aData[i]);
			m_aStepNum.InsertAt(nIndex, nSubIndex+1);
			AddKeyMapper(&m_KeyMapper, aKey[i].key.serial, nIndex, FALSE);
			m_KeyMapper.SetAt(aKey[i].keymap, nIndex);
			m_SubKeyMapper.SetAt(aKey[i].keymap, nSubIndex);
		}
		m_CurKey++;
	}

	UpdateBuffer(0, aKey, aKey, aData);

	return TRUE;
}

BOOL CCMStageSpreadGrid::DelData(T_KEYSTEP key)
{
	CArray<T_KEYSTEP, T_KEYSTEP> aOldKey, aKey;
	CArray<T_ESEQ_BASE, T_ESEQ_BASE&> aData;
	aKey.Add(key);
	T_ESEQ_BASE tmpData; 
	tmpData.Initialize();
	aData.Add(tmpData);

	int ix, subix;
	if (!m_KeyMapper.Lookup(key.keymap, ix) || !m_SubKeyMapper.Lookup(key.keymap, subix))
	{
		CString msg;
		msg.Format(_LS(IDS_CMD_Data_does_not_exist));
		AfxMessageBox(msg);
		return FALSE;
	}
	
	// 서브 인덱스의 개수가 1개(마지막)면 
	if (m_aStepNum[ix] == 1)
	{
		m_aData.RemoveAt(ix);
		m_aStepNum.RemoveAt(ix);
		m_KeyMapper.RemoveKey(key.keymap);
		DelKeyMapper(&m_KeyMapper, key.key.serial, ix, FALSE);
		m_SubKeyMapper.RemoveKey(key.keymap);
	}
	else 
	{
		int nNum = m_aStepNum[ix];
		T_ESEQ_BASE BaseData = m_aData.GetAt(ix);
		for (int i=subix+1; i < nNum; i++)
		{
			BaseData.EseqSel[i-1].nType = BaseData.EseqSel[i].nType;
			BaseData.EseqSel[i-1].KeyMatl = BaseData.EseqSel[i].KeyMatl;
			BaseData.EseqSel[i-1].KeyStorModel = BaseData.EseqSel[i].KeyStorModel;
		}
		m_aData.SetAt(ix, BaseData);
		m_aStepNum.SetAt(ix, nNum-1);
		m_KeyMapper.RemoveKey(key.keymap);
		m_SubKeyMapper.RemoveKey(key.keymap);
		DelKeyMapper(&m_SubKeyMapper, key.key.serial, subix, TRUE);
	}

	UpdateBuffer(1, aOldKey, aKey, aData);

	return TRUE;
}

BOOL CCMStageSpreadGrid::DelData(CArray<T_KEYSTEP, T_KEYSTEP> &aKey, BOOL bNotify)
{
	CArray<T_KEYSTEP, T_KEYSTEP> aOldKey;
	CArray<T_ESEQ_BASE, T_ESEQ_BASE&> aData;
	aOldKey.Copy(aKey);
	T_ESEQ_BASE tempData;
	tempData.Initialize();
	int nSize = aKey.GetSize();
	int i = 0;
	for (i = 0; i < nSize; i++) aData.Add(tempData);

	int ix, subix;
	for (i=0; i<nSize; i++)
	{
		if (!m_KeyMapper.Lookup(aKey[i].keymap, ix) || !m_SubKeyMapper.Lookup(aKey[i].keymap, subix))
		{
			CString msg;
			msg.Format(_LS(IDS_CMD_Data_does_not_exist));
			AfxMessageBox(msg);
			return FALSE;
		}
	}

	CArray<int, int> aDelIndex;
	CArray<int, int> aDeli;
	for (i=0; i<nSize; i++)
	{
		m_KeyMapper.Lookup(aKey[i].keymap, ix);
		m_SubKeyMapper.Lookup(aKey[i].keymap, subix);
		if (m_aStepNum[ix] == 1)
		{
			aDelIndex.Add(ix);
			aDeli.Add(i);
		}
		else 
		{
			int nNum = m_aStepNum[ix];
			T_ESEQ_BASE BaseData = m_aData.GetAt(ix);
			for (int k=subix+1; k < nNum; k++)
			{
				BaseData.EseqSel[k-1].nType = BaseData.EseqSel[k].nType;
				BaseData.EseqSel[k-1].KeyMatl = BaseData.EseqSel[k].KeyMatl;
				BaseData.EseqSel[k-1].KeyStorModel = BaseData.EseqSel[k].KeyStorModel;
			}
			m_aData.SetAt(ix, BaseData);
			m_aStepNum.SetAt(ix, nNum-1);
			m_KeyMapper.RemoveKey(aKey[i].keymap);
			m_SubKeyMapper.RemoveKey(aKey[i].keymap);
			DelKeyMapper(&m_SubKeyMapper, aKey[i].key.serial, subix, TRUE);
		}
	}

	nSize = aDelIndex.GetSize();
	for (i=nSize-1; i>=0; i--)
	{
		m_aData.RemoveAt(aDelIndex[i]);
		m_aStepNum.RemoveAt(aDelIndex[i]);
		
		m_KeyMapper.RemoveKey(aKey[aDeli[i]].keymap);
		DelKeyMapper(&m_KeyMapper, aKey[aDeli[i]].key.serial, aDelIndex[i], FALSE);
		m_SubKeyMapper.RemoveKey(aKey[aDeli[i]].keymap);
	}

	UpdateBuffer(1, aOldKey, aKey, aData);

	return TRUE;
}

BOOL CCMStageSpreadGrid::ModifyData(T_KEYSTEP oldKey, T_KEYSTEP key, T_ESEQ_BASE data)
{
	CArray<T_KEYSTEP, T_KEYSTEP> aOldKey, aKey;
	CArray<T_ESEQ_BASE, T_ESEQ_BASE&> aData;
	aOldKey.Add(oldKey);
	aKey.Add(key);
	aData.Add(data);

	int ix, subix;
	if (!m_KeyMapper.Lookup(oldKey.keymap, ix) || !m_SubKeyMapper.Lookup(oldKey.keymap, subix))
	{
		CString msg;
		msg.Format(_LS(IDS_CMD_Data_does_not_exist2));
		AfxMessageBox(msg);
		return FALSE;
	}

	if (!CheckStoryLevel(oldKey, key, data, TRUE)) return FALSE;

	T_ESEQ_BASE BaseData;
	if (oldKey.keymap == key.keymap)
	{
		BaseData = m_aData.GetAt(ix);
		BaseData.KeyStorLoad = data.KeyStorLoad;
		BaseData.dDuration = data.dDuration;
		BaseData.dAge = data.dAge;
		BaseData.EseqSel[subix].nType = data.EseqSel[0].nType;
		BaseData.EseqSel[subix].KeyMatl = data.EseqSel[0].KeyMatl;
		BaseData.EseqSel[subix].KeyStorModel = data.EseqSel[0].KeyStorModel;
		m_aData.SetAt(ix, BaseData);
	}
	else
	{
		int ix2, subix2;
		if (m_KeyMapper.Lookup(key.keymap, ix2) || m_SubKeyMapper.Lookup(key.keymap, subix2))
		{
			CString msg;
			msg.Format(_LS(IDS_CMD_Data_already_exist2));
			AfxMessageBox(msg);
			return FALSE;
		}
		
		// Remove = oldkey
		// 서브 인덱스의 개수가 1개(마지막)면 
		if (m_aStepNum[ix] == 1)
		{
			m_aData.RemoveAt(ix);
			m_aStepNum.RemoveAt(ix);
			m_KeyMapper.RemoveKey(oldKey.keymap);
			DelKeyMapper(&m_KeyMapper, oldKey.key.serial, ix, FALSE);
			m_SubKeyMapper.RemoveKey(oldKey.keymap);
		}
		else 
		{
			int nNum = m_aStepNum[ix];
			BaseData = m_aData.GetAt(ix);
			for (int i=subix+1; i < nNum; i++)
			{
				BaseData.EseqSel[i-1].nType = BaseData.EseqSel[i].nType;
				BaseData.EseqSel[i-1].KeyMatl = BaseData.EseqSel[i].KeyMatl;
				BaseData.EseqSel[i-1].KeyStorModel = BaseData.EseqSel[i].KeyStorModel;
			}
			m_aData.SetAt(ix, BaseData);
			m_aStepNum.SetAt(ix, nNum-1);
			m_KeyMapper.RemoveKey(oldKey.keymap);
			m_SubKeyMapper.RemoveKey(oldKey.keymap);
			DelKeyMapper(&m_SubKeyMapper, oldKey.key.serial, subix, TRUE);
		}

		// Insert = newkey
		if (FindData(key, ix2, subix2))   // 같은 스텝이 존재하는 경우 
		{
			BaseData = m_aData.GetAt(ix2);
			BaseData.EseqSel[subix2].nType = data.EseqSel[0].nType;
			BaseData.EseqSel[subix2].KeyMatl = data.EseqSel[0].KeyMatl;
			BaseData.EseqSel[subix2].KeyStorModel = data.EseqSel[0].KeyStorModel;
			m_aData.SetAt(ix2, BaseData);
			m_aStepNum.SetAt(ix2, subix2+1);
			m_KeyMapper.SetAt(key.keymap, ix2);
			AddKeyMapper(&m_SubKeyMapper, key.key.entity, subix2, TRUE);
			m_SubKeyMapper.SetAt(key.keymap, subix2);
		}
		else    // 새로 추가되는 스텝의 경우  
		{
			m_aData.InsertAt(ix2, data);
			m_aStepNum.InsertAt(ix2, subix2+1);
			AddKeyMapper(&m_KeyMapper, key.key.serial, ix2, FALSE);
			m_KeyMapper.SetAt(key.keymap, ix2);
			m_SubKeyMapper.SetAt(key.keymap, subix2);
		}
		m_CurKey++;
	}

	UpdateBuffer(2, aOldKey, aKey, aData);

	return TRUE;
}

BOOL CCMStageSpreadGrid::ModifyData(CArray<T_KEYSTEP, T_KEYSTEP> &aOldKey, CArray<T_KEYSTEP, T_KEYSTEP> &aKey, CArray<T_ESEQ_BASE, T_ESEQ_BASE&> &aData)
{
	CArray<T_ESEQ_BASE, T_ESEQ_BASE&> aOrgData;
	aOrgData.Copy(m_aData);
	
	// 스토리 레벨 검토
	/*
	for (int i=0; i<aData.GetSize(); i++)
	{
		if (!CheckStoryLevel(aOldKey[i], aKey[i], aData[i], TRUE)) return FALSE;
	}
	*/

	if (!DelData(aOldKey, FALSE))
		return FALSE;
	if (!AddData(aKey, aData, FALSE))
	{
		//for (int i=0; i<aOrgData.GetSize(); i++)
		AddData(aOldKey, aOrgData, FALSE);

		//aOrgData.GetSize();
		//m_aData.Copy(aOrgData);
		return FALSE;
	}
	return TRUE;
}

void CCMStageSpreadGrid::UpdateBuffer(int nCmd, CArray<T_KEYSTEP, T_KEYSTEP> &aOldKey, CArray<T_KEYSTEP, T_KEYSTEP> &aKey, CArray<T_ESEQ_BASE, T_ESEQ_BASE&> &aData)
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

	// 여기서 전체 정렬 
	if (nCmd == 1) 
	{
		EnableDisableByStep(aKey);
		SetStartStoryName();
	}

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

void CCMStageSpreadGrid::SetStartStoryName()
{
	CString csStory;
	ROWCOL nKeyCol, nStepCol;
	ROWCOL nLoadStr, nLoadEnd;
	GetColumnNoFor(0, nKeyCol);
	GetColumnNoFor(1, nStepCol);
	GetColumnNoFor(5, nLoadStr);
	GetColumnNoFor(6, nLoadEnd);

	if (m_aData.GetSize() == 1)  // 첫 데이터일때 
	{
		SetStyleRange(CGXRange(1, nLoadStr), CGXStyle().SetValue(_T("")));
		return;
	}

	// 데이터가 두개 이상일때 
	T_STOR_K StorK;
	CArray<T_STOR_K, T_STOR_K> aStorK;
	m_pDoc->m_pAttrCtrl->GetStorKeyList(aStorK);
	int nStorNum = aStorK.GetSize();

	int nIndex;
	int nOldRow = 2;
	T_KEYSTEP keystep;  
	SetStyleRange(CGXRange(1, nLoadStr), CGXStyle().SetValue(_T("")));
	for (int i=2; i<GetRowCount(); i++)
	{
		keystep.key.entity = _tstol(GetValueRowCol(i, nKeyCol));
		keystep.key.serial = _tstol(GetValueRowCol(i, nStepCol));
		if (!m_SubKeyMapper.Lookup(keystep.keymap, nIndex)) continue;
		if (nIndex != 0) continue;
		
		csStory = GetValueRowCol(nOldRow-1, nLoadEnd);
		StorK = GetStoryKey(csStory);       
		if (StorK == 0) csStory = D_STORY_NONE;
		else
		{
			int k = 0;
			for (k=0; k<nStorNum; k++)
				if (aStorK[k] == StorK) break;
			if (k >= nStorNum - 1) { ASSERT(0); continue; }
			csStory = GetStoryName(aStorK[k+1]);
		}
		SetStyleRange(CGXRange(i, nLoadStr), CGXStyle().SetValue(csStory));
		nOldRow = i+1;
	}
}

void CCMStageSpreadGrid::EnableDisableByStep(CArray<T_KEYSTEP, T_KEYSTEP> &aKey)
{
	CString csStep;
	ROWCOL nStepCol, nTargetCol;
	GetColumnNoFor(1, nStepCol);

	int ix, subix;
	int nNum = aKey.GetSize();
	for (int i=0; i<nNum; i++)
	{
		if (!FindData(aKey[i], ix, subix)) continue;
		for (int nRow=1; nRow<GetRowCount(); nRow++)
		{
			csStep = GetEditingValue(nRow, nStepCol);
			if (aKey[i].key.serial != _tstol(csStep)) continue;

			T_ESEQ_BASE data = m_aData.GetAt(ix);
			GetColumnNoFor(6, nTargetCol);
			SetStyleRange(CGXRange(nRow, nTargetCol), CGXStyle().SetEnabled(TRUE)
				.SetValue(GetStoryName(data.KeyStorLoad)));
			GetColumnNoFor(7, nTargetCol);
			SetStyleRange(CGXRange(nRow, nTargetCol), CGXStyle().SetEnabled(TRUE)
				.SetValue(double(data.dDuration)));
			GetColumnNoFor(8, nTargetCol);
			SetStyleRange(CGXRange(nRow, nTargetCol), CGXStyle().SetEnabled(TRUE)
				.SetValue(double(data.dAge)));
			break;
		}
	}
}

/////////////////////////////////////////////////////////////////////////
// 여기부터 Column 값 변경이 다른 column의 상태나 값을 변경시키는 경우를
// 처리해 주기 위해서 Override 및 새로 정의되는 함수이다.

void CCMStageSpreadGrid::OnModifyCell(ROWCOL nRow, ROWCOL nCol)
{
	// 부모의 OnModifyCell을 항상 먼저 호출해 주어야 한다.
	CTBCommon::OnModifyCell(nRow, nCol);

	ROWCOL nStep, nType, nLoadEnd;
	GetColumnNoFor(1, nStep);
	GetColumnNoFor(2, nType);
	GetColumnNoFor(6, nLoadEnd);

	if (nCol == nStep || nCol == nType || nCol == nLoadEnd)
		ChangeColumnStatus(nRow, nCol);
}

void CCMStageSpreadGrid::OnCanceledEditing(ROWCOL nRow, ROWCOL nCol)
{
	CTBCommon::OnCanceledEditing(nRow, nCol);
	if (!IsEdit()) return;

	ROWCOL nStep, nType, nLoadEnd;
	GetColumnNoFor(1, nStep);
	GetColumnNoFor(2, nType);
	GetColumnNoFor(6, nLoadEnd);

	if (nCol == nStep || nCol == nType || nCol == nLoadEnd)
		ChangeColumnStatus(nRow, nCol);
}

void CCMStageSpreadGrid::ChangeColumnStatus(ROWCOL nRow, ROWCOL nCol)
{
	ROWCOL nStepCol, nTypeCol, nLoadEndCol;
	GetColumnNoFor(1, nStepCol);
	GetColumnNoFor(2, nTypeCol);
	GetColumnNoFor(6, nLoadEndCol);

	if (nCol != nStepCol && nCol != nTypeCol && nCol != nLoadEndCol) return;

	CString csStep, csType, csLoadEnd;
	int nType;

	CGXControl *pControl;
	pControl = GetControl(nRow, nCol);
	if (pControl->IsActive() && nCol == nStepCol) pControl->GetValue(csStep);
	else csStep = GetValueRowCol(nRow, nStepCol);
	if (pControl->IsActive() && nCol == nTypeCol) pControl->GetValue(csType);
	else csType = GetValueRowCol(nRow, nTypeCol);
	if (pControl->IsActive() && nCol == nLoadEndCol) pControl->GetValue(csLoadEnd);
	else csLoadEnd = GetValueRowCol(nRow, nLoadEndCol);
	if (nCol == nStepCol && csStep != _T(""))
	{
		if (m_nEditMode == edit)
		{
			AfxMessageBox(_LS(IDS_CMD_CANNOT_MODIFY_STEP));
			CancelEdit();
			return;
		}
		T_KEYSTEP key;
		ROWCOL nKeyCol;
		GetColumnNoFor(0, nKeyCol);
		CString csKey = GetValueRowCol(nRow, nKeyCol);
		key.key.entity = _tstol(csKey);
		key.key.serial = _tstol(csStep);
		ChangeModelStep(nRow, key);
	}
	if (nCol == nTypeCol && csType != _T(""))
	{
		nType = GetModelTypeData(csType);
		ChangeModelType(nRow, nType);
	}
}

/*
BOOL CCMStageSpreadGrid::IsExistStep(T_KEYSTEP step, unsigned int &rKey)
{
	BOOL bExist = FALSE;
	unsigned int key, minkey = 0; 
	int data;
	T_KEYSTEP keystep;
	POSITION pos = m_KeyMapper.GetStartPosition();
	while (pos != NULL)
	{ 
		m_KeyMapper.GetNextAssoc(pos, key, data);
		keystep.keymap = key;
		if (keystep.key.serial == step.key.serial &&
				keystep.key.entity != step.key.entity) bExist = TRUE;
		if (keystep.key.serial == step.key.serial)
		{
			if (minkey == 0) minkey = keystep.key.entity;
			else { if (keystep.key.entity < minkey) minkey = keystep.key.entity; }
		}
	}
	rKey = minkey;
	if (bExist && minkey == step.key.entity) bExist = FALSE;
	return bExist;
}
*/

void CCMStageSpreadGrid::ChangeModelStep(ROWCOL nRow, T_KEYSTEP step)
{
	// 넘어온 스텝이 존재하는지 체크. 단, 자기자신은 아니어야 한다.
	// 그리고 같은 스텝 중 키값이 가장 작지 않아야 한다.
	BOOL bExist = FALSE;
	unsigned int key, minkey = 0; 
	int data;
	T_KEYSTEP keystep;
	POSITION pos = m_KeyMapper.GetStartPosition();
	while (pos != NULL)
	{ 
		m_KeyMapper.GetNextAssoc(pos, key, data);
		keystep.keymap = key;
		if (keystep.key.serial == step.key.serial &&
				keystep.key.entity != step.key.entity) bExist = TRUE;
		if (keystep.key.serial == step.key.serial)
		{
			if (minkey == 0) minkey = keystep.key.entity;
			else { if (keystep.key.entity < minkey) minkey = keystep.key.entity; }
		}
	}
	if (bExist && minkey == step.key.entity) bExist  = FALSE;
 
	ROWCOL nTargetCol;
	int nCol[3] = {6, 7, 8}; 
	for (int i=0; i<3; i++)
	{
		GetColumnNoFor(nCol[i], nTargetCol);
		SetStyleRange(CGXRange(nRow, nTargetCol), CGXStyle().SetEnabled(!bExist));
		if (bExist) SetStyleRange(CGXRange(nRow, nTargetCol), CGXStyle().SetValue(_T("")));
	}
	if (bExist)
	{
		GetColumnNoFor(5, nTargetCol);
		SetStyleRange(CGXRange(nRow, nTargetCol), CGXStyle().SetValue(_T("")));
	}
}

void CCMStageSpreadGrid::ChangeModelType(ROWCOL nRow, int nType)
{
	ROWCOL nMatlCol;
	GetColumnNoFor(3, nMatlCol);
	SetModelMaterialColumn(nRow, nMatlCol, nType);
}

int CCMStageSpreadGrid::GetModelTypeData(CString &str)
{
	if (str == D_MODEL_MATL_TYPE) return 1;
	if (str == D_MODEL_MATL_NAME) return 2;
	return 0;
}

CString CCMStageSpreadGrid::GetModelTypeStr(int data)
{
	if (data == 1) return D_MODEL_MATL_TYPE;
	if (data == 2) return D_MODEL_MATL_NAME;
	return _T("");
}

int CCMStageSpreadGrid::GetModelMatlData(int nType, CString &str)
{
	if (nType == 1)
	{
		if (str == D_MODEL_MATL_ALL)    return 1;
		if (str == D_MODEL_MATL_STEEL)  return 2;
		if (str == D_MODEL_MATL_CONC)   return 3;
		if (str == D_MODEL_MATL_SRC)    return 4;
		if (str == D_MODEL_MATL_USER)   return 5;
	}
	else if (nType == 2)
	{
		int n = str.Find(_T(":"));
		if (n > 0) return _tstol(str.Left(n));
	}
	return 0;
}

CString CCMStageSpreadGrid::GetModelMatlStr(int nType, int data)
{
	if (nType == 1)
	{
		if (data == 1) return D_MODEL_MATL_ALL;
		if (data == 2) return D_MODEL_MATL_STEEL;
		if (data == 3) return D_MODEL_MATL_CONC;
		if (data == 4) return D_MODEL_MATL_SRC;
		if (data == 5) return D_MODEL_MATL_USER;
	}
	else if (nType == 2)
	{
		T_MATL_D MatlD;
		if (m_pDoc->m_pAttrCtrl->GetMatl(data, MatlD))
		{
			CString csName;
			csName.Format(_T("%d: %s"), data, MatlD.Name);
			return csName;
		}
	}
	return _T("");
}

int CCMStageSpreadGrid::GetStoryKey(CString &csName)
{
	if (csName == D_STORY_NONE) return -1;
	return m_pDoc->m_pAttrCtrl->GetStorKey(csName);
}

CString CCMStageSpreadGrid::GetStoryName(int key)
{
	if (key == -1) return D_STORY_NONE;

	T_STOR_D data;
	if (!m_pDoc->m_pAttrCtrl->GetStor(key, data)) return D_STORY_NONE;
	return data.StoryName;
}



// 항상 FALSE를 리턴해야 한다.
// 왜냐 하면 Grid가 Paste 작업을 해서는 안 되기 때문
BOOL CCMStageSpreadGrid::OnPasteFromClipboard(const CGXRange &range)
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
		// 기존 key를 가져온다.
		T_KEYSTEP oldkey;
		CStringArray aOldKey;
		aOldKey.SetSize(x.GetRowCount());
		int nxRowCount = x.GetRowCount();
		for (int i = 0; i < nxRowCount; i++)
		{
			oldkey.key.entity = _tstol(GetValueRowCol(range.top+i, aCols[0]));
			oldkey.key.serial = _tstol(GetValueRowCol(range.top+i, aCols[1]));
			aOldKey[i].Format(_T("%ld"), oldkey.keymap);
			//aOldKey[i] = GetValueRowCol(range.top+i, aCols[0]);
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

BOOL CCMStageSpreadGrid::ProcessKeys(CWnd *pSender, UINT nMessage, UINT nChar, UINT nRepCnt, UINT flags)
{
	CRowColArray awRows;
	ROWCOL nRow=1, nCol=1;
	CGXControl* pControl;
	ROWCOL i, j;
	int nSize;

	BOOL bCtl = GetKeyState(VK_CONTROL) & 0x8000;
	BOOL bShift = GetKeyState(VK_SHIFT) & 0x8000;

	if (bCtl || bShift) 
		return CGXGridWnd::ProcessKeys(pSender, nMessage, nChar, nRepCnt, flags);

	VERIFY(GetCurrentCell(nRow, nCol));
	pControl = GetControl(nRow, nCol);
	if (nMessage == WM_KEYDOWN)
	{
		// 셀이 ACTIVE인 상태에서 좌우 화살표 키가 눌러지면 셀 이동을 하지 않는다.
		if (nChar == VK_LEFT || nChar == VK_RIGHT)
		{
			ROWCOL nRow, nCol;
			if (GetCurrentCell(nRow, nCol))
			{
				CGXControl* pControl = GetControl(nRow, nCol);
				if (pControl && pControl->IsActive()) return TRUE;
			}
		}

		switch(nChar)
		{
		case VK_ESCAPE:
			if (m_nEditMode != noMode)  // 추가모드 또는 수정모드에서
			{
				if (!pControl || !pControl->IsActive()) // 편집중인 셀이 없으면
				{
					CancelEdit(); // 레코드 편집을 취소한다.
					return TRUE;
				}
			}
			// 편집모드 아닌 상태에서 편집중인 셀이 없으면 ESC는 무시한다.
			// 이렇게 함으로써 셀이 편집되지 않았을 때 OnCancelEditing과 
			// OnCanceledEditing이 호출 되는 것을 막는다.
			else if (!pControl || !pControl->IsActive()) 
			{
				// Select된 것을 해제한다.
				SelectRange(CGXRange().SetTable(), FALSE);
				return TRUE;
			}
			break;
		case VK_RETURN: case VK_RIGHT: case VK_TAB:
			// 열의 위치에 상관 없이 동작 : 두번째 버전
			// 마지막 셀이면 그냥 리턴
			if (IsAppendRow(nRow) && nCol == GetColCount()) return TRUE;

			i = nRow; j = nCol+1; // 현재 행 다음 열
			if (j > GetColCount()) { i++; j = 1; }

			m_bValidField = m_bValidRecord = TRUE; m_sWarningText.Empty();
			while (i <= GetRowCount())
			{
				while (j <= GetColCount())
				{
					// 열이 선택될 수 있는지 조사
					if (CanSelectCurrentCell(FALSE, i, j, nRow, nCol))
					{
						if (CanSelectCurrentCell(TRUE, i, j, nRow, nCol))
						{
							SetCurrentCell(i, j); return TRUE;
							return CGXGridWnd::ProcessKeys(pSender, nMessage, nChar, nRepCnt, flags);
						}
						// 이 경우는 레코드 추가시 같은 키의 중복에 의해 레코드가 합쳐진 경우
						// CanSelectCurrentCell(FALSE...)가 불리기 전보다 Row Count가 1 작아진다.
						// CTBOffs을 참고(SetCurrentCell 함수 부분)
						// 이 경우 새로 설정하려는 row가 Row Count보다 커지는 경우가 발생
						// 이걸 막기 위해서 Append Row(마지막 Row)에 SetCurrentCell을 한다.
						if (i > GetRowCount())
						{
							SetCurrentCell(GetRowCount(), j); return TRUE;
						}
					}
					if (!m_bValidField || !m_bValidRecord)
					{
						m_bValidField = m_bValidRecord = TRUE;
						if (m_sWarningText.GetLength() > 0) DisplayWarningText();
						
						// Edit Control이면 포커스를 다시 설정한다.
						BOOL bActive = pControl->IsActive();
						pControl->SetActive(FALSE);
						pControl->Refresh();

						if (bActive)
						{
							ROWCOL ncRow, ncCol;
							if (GetCurrentCell(ncRow, ncCol))
							{
								CGXControl* pControl = GetControl(ncRow, ncCol);
								if (pControl) pControl->SetActive(TRUE);
							}
							//return 0L;
							//CWnd::PostMessage(WM_SET_CELL_ACTIVE, 0, 0);
						}
						return TRUE;
					}
					j++;  // 다음 열을 조사
				}
				i++;  // 다음 줄로 이동
				j = 1;
			}
			break;
		case VK_LEFT:
			// 열의 위치에 상관 없이 동작 : 두번째 버전
			// 첫번째 셀이면 그냥 리턴
			if (nRow == 1 && nCol == 1) return TRUE;

			i = nRow; j = nCol-1; // 현재 행 이전 열
			if (j < 1) { i--; j = GetColCount(); }

			m_bValidField = m_bValidRecord = TRUE; m_sWarningText.Empty();

			while (i >= 1)
			{
				while (j >= 1)
				{
					// 열이 선택될 수 있는지 조사
					if (CanSelectCurrentCell(FALSE, i, j, nRow, nCol))
					{
						if (CanSelectCurrentCell(TRUE, i, j, nRow, nCol))
						{
							SetCurrentCell(i, j); return TRUE;
							return CGXGridWnd::ProcessKeys(pSender, nMessage, nChar, nRepCnt, flags);
						}
					}
					if (!m_bValidField || !m_bValidRecord)
					{
						m_bValidRecord = TRUE;
						if (m_sWarningText.GetLength() > 0) DisplayWarningText();

						pControl->Refresh();
						return TRUE;
					}
					j--;  // 이전 열을 조사
				}
				i--;  // 이전 줄로 이동
				j = GetColCount();
			}
			break;
		case VK_INSERT: // 편집중인 레코드 앞에 addnew 레코드 추가
			/*
			if (!CanInsertRowByPressingKey()) return TRUE;
			if (IsLockEdit()) return TRUE;  // display only mode면 do nothing!
			if (!InsertEmptyRecord(nRow)) FALSE;
			*/
			break;
		case VK_DELETE: // 편집중인 레코드 삭제
			if (!CanDeleteRowByPressingKey()) return TRUE;
			// 먼저 범위가 선택되어 있는지 본다. 범위가 우선한다.
			if (IsLockEdit()) return TRUE;  // display only mode면 do nothing!
			m_sWarningText.Empty();
			GetSelectedRows(awRows, TRUE, FALSE);
			nSize = awRows.GetSize();
			
			if (nSize > 0)
			{
				// Append Row는 뺀다.
				if (IsAppendRow(awRows.GetAt(nSize-1))) { nSize--; awRows.RemoveAt(nSize); }
				// Title Row도 뺀다.
				if (nSize > 0 && awRows.GetAt(0) == 0) { nSize--; awRows.RemoveAt(0); }
				if (nSize == 0) return TRUE;
				if (!OnDeleteRecords(awRows))
					if (m_sWarningText.GetLength() > 0) DisplayWarningText();
			}
			else  // 이 경우는 현재 레코드 삭제이다.
			{
				if (IsAppendRow(nRow)) return TRUE;
				if (!OnDeleteRecord(nRow))
					if (m_sWarningText.GetLength() > 0) DisplayWarningText();
			}
			return TRUE;
			break;
		case VK_DOWN:
			break;
		case VK_HOME:
			SetRightValidCell(nRow, 0);
			return TRUE;
			break;
		case VK_END:
			SetLeftValidCell(nRow, GetColCount()+1);
			return TRUE;
			break;
		}
	}
	else if (nMessage == WM_CHAR)
	{
		switch(nChar)
		{
		// 셀이 Inactive 상태에서 Edit 셀에 스페이스를 눌렀을 때 편집을 활성화 시킨다.
		case VK_SPACE:
			if (pControl && !pControl->IsActive() 
				&& pControl->CGXControl::IsKindOf(CONTROL_CLASS(CGXEditControl)))
			{
				if (!pControl->IsInit()) pControl->Init(nRow, nCol);
				CString value = GetExpressionRowCol(nRow, nCol);
				pControl->SetActive(TRUE);
				pControl->Refresh(); // 반드시 호출
				if (!pControl->IsKindOf(CONTROL_CLASS(CGXComboBox)))  // combo box가 아니면
					((CGXEditControl*)pControl)->SetSel(0, value.GetLength());
				return TRUE;
			}
			break;
		}
	}
	return CGXGridWnd::ProcessKeys(pSender, nMessage, nChar, nRepCnt, flags);
}


CString CCMStageSpreadGrid::GetEditingValue(ROWCOL nRow, ROWCOL nCol)
{
	CString csNum;
	BOOL bGet = FALSE;
	ROWCOL ncRow, ncCol;
	if (GetCurrentCell(ncRow, ncCol))
	{
		if (ncRow == nRow && ncCol == nCol)
		{
			CGXControl* pControl = GetControl(nRow, nCol);
			if (pControl && pControl->IsActive())
				bGet = pControl->GetValue(csNum);
		}
	}
	if (!bGet) csNum = GetValueRowCol(nRow, nCol);
	return csNum;
}