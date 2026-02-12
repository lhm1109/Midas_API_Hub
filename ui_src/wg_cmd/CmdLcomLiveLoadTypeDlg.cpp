// CmdLcomLiveLoadTypeDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "CmdLcomLiveLoadTypeDlg.h"

#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl2.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_EditData.h"
#include "..\wg_db\wg_db_UndoCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCmdLcomLiveLoadTypeDlg dialog

CCmdLcomLiveLoadTypeDlg::CCmdLcomLiveLoadTypeDlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CCmdLcomLiveLoadTypeDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCmdLcomLiveLoadTypeDlg)
	//}}AFX_DATA_INIT
	m_pLcomLiveLoadTypeGrid = new CCmdLcomLiveLoadTypeGrid;
	m_pDoc = NULL;
	m_mLcomLiveLoadType.RemoveAll();
}

CCmdLcomLiveLoadTypeDlg::~CCmdLcomLiveLoadTypeDlg()
{
	if (m_pLcomLiveLoadTypeGrid) delete m_pLcomLiveLoadTypeGrid;
}

void CCmdLcomLiveLoadTypeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCmdLcomLiveLoadTypeDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CCmdLcomLiveLoadTypeDlg, CDialogMove)
	//{{AFX_MSG_MAP(CCmdLcomLiveLoadTypeDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCmdLcomLiveLoadTypeDlg message handlers

BOOL CCmdLcomLiveLoadTypeDlg::OnInitDialog()
{
	m_pDoc = CDBDoc::GetDocPoint();

	CDialogMove::OnInitDialog();

	m_mLcomLiveLoadType.RemoveAll();
	CArray<T_LLLT_K, T_LLLT_K> aLlltK;
	
	int nSize = m_pDoc->m_pAttrCtrl->GetLlltKeyList(aLlltK);
	int nIndex = 1;
	for (int i=0; i<nSize;i++)
	{
		T_LLLT_D rData;
		if (!m_pDoc->m_pAttrCtrl->GetLllt(aLlltK[i], rData)) { ASSERT(0); continue; }

		T_STLD_D StldD;
		if (!m_pDoc->m_pAttrCtrl->GetStld(aLlltK[i], StldD)) { ASSERT(0); continue; }
		
		T_LLLT_Data LlltData; LlltData.Initialize();
		LlltData.LoadCaseName = StldD.LoadCaseName;
		LlltData.nIndex = rData.nIndex;
		LlltData.LLType = rData.LLType;
		m_mLcomLiveLoadType.SetAt(aLlltK[i], LlltData);
		nIndex++;
	}

	m_pLcomLiveLoadTypeGrid->SubclassDlgItem(IDC_CMD_LCOM_LIVE_LOAD_TYPE_GRID, this);

	if (m_pLcomLiveLoadTypeGrid != NULL)
	{
		m_pLcomLiveLoadTypeGrid->SetData(m_mLcomLiveLoadType);
	}

	T_UNIT_SYSTEM UnitSystem;
	m_pLcomLiveLoadTypeGrid->Initialize(m_pDoc, UnitSystem);

	return TRUE;
}

void CCmdLcomLiveLoadTypeDlg::OnOK()
{
	m_pLcomLiveLoadTypeGrid->OnTerminate(D_TB_SAVE_ALWAYS);
	m_pLcomLiveLoadTypeGrid->GetData(m_mLcomLiveLoadType);

	if (!m_pDoc->m_pUndoCtrl->StartEditDB(CString(_LS(IDS_DB_DATACTRL_Modify_LLLT)), CMDTYPE_FREE)) return ;

	if (!m_pDoc->m_pEditData->DelAllLllt()) { m_pDoc->m_pUndoCtrl->CloseEditDB(FALSE); return; }
	POSITION pos = m_mLcomLiveLoadType.GetStartPosition();
	while (pos)
	{
		T_LLLT_K Key;
		T_LLLT_Data LlltData;
		m_mLcomLiveLoadType.GetNextAssoc(pos, Key, LlltData);

		T_STLD_D StldD;
		if (!m_pDoc->m_pAttrCtrl->GetStld(Key, StldD)) continue;

		T_LLLT_D LlltD;
		LlltD.nIndex = LlltData.nIndex;
		LlltD.LLType = LlltData.LLType;
		if (!m_pDoc->m_pEditData->AddLllt(Key, LlltD)) { ASSERT(0);  m_pDoc->m_pUndoCtrl->CloseEditDB(FALSE); return; }
	}

	m_pDoc->m_pUndoCtrl->CloseEditDB(TRUE);

	CDialogMove::OnOK();
}

void CCmdLcomLiveLoadTypeDlg::OnCancel()
{
	CDialogMove::OnCancel();
}

/////////////////////////////////////////////////////////////////////////////

#define NAME_COL_ID         2
#define LCOM_KEY_FMT        _T("%d")

const CString csLLMax = _T("LLmax");
const CString csLLins = _T("LLins");
const CString csLLmed = _T("LLmed");
CString CCmdLcomLiveLoadTypeGrid::m_aCurDefVal[] = {"", "", ""};

CCmdLcomLiveLoadTypeGrid::CCmdLcomLiveLoadTypeGrid()
{
	AddColInfo(_ULS(Key), t_integer, 0);
	AddColInfo(_LS(IDS_WG_CMD__ADDD__No),             t_integer, 0);
	AddColInfo(_LS(IDS_CMD_ENVL_loadcase),            t_string,  0);
	AddColInfo(_LS(IDS_WG_CMD__ADDD__Type),           t_string,  0);

	m_aPrimaryKey.Add(0);
}

CCmdLcomLiveLoadTypeGrid::~CCmdLcomLiveLoadTypeGrid()
{
}

#define CWnd CTBCommon
BEGIN_MESSAGE_MAP(CCmdLcomLiveLoadTypeGrid, CWnd)
#undef CWnd
	//{{AFX_MSG_MAP(CCmdLcomLiveLoadTypeGrid)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CCmdLcomLiveLoadTypeGrid message handlers
void CCmdLcomLiveLoadTypeGrid::Initialize(CDBDoc* pDoc, T_UNIT_SYSTEM &UnitSystem)
{
	CTBCommon::Initialize(pDoc, UnitSystem);

	LockUpdate(TRUE);

	// 컬럼 갯수 설정
	SetColCount(GetInitialColCount());
	SetHeaderTitle(UnitSystem, TRUE);

	// 표준 스타일 변경
	ChangeStandardStyle(CGXStyle()
			.SetFont(CGXFont_GC().SetSize(10))
			.SetVerticalAlignment(DT_BOTTOM)
			.SetAllowEnter(FALSE));

	// 컬럼 스타일 변경
//   SetStyleRange(CGXRange().SetCols(4), CGXStyle()
//       .SetHorizontalAlignment(DT_CENTER)
//       .SetValueType(GX_VT_STRING));
	SetFrozenCols(2, 2);
	HideColsByIntend(1, 1, TRUE);

	// Serial No column의 스타일 변경
	SetStyleRange(CGXRange().SetCols(1, 2), CGXStyle()
			.SetHorizontalAlignment(DT_RIGHT)
			.SetValueType(GX_VT_NUMERIC)
			.SetFormat(GX_FMT_FIXED)
			.SetPlaces(0)
			.SetEnabled(FALSE));

	CArray<T_STLD_K, T_STLD_K> aStldKeyList;
	m_pDoc->m_pAttrCtrl2->GetStldKeyList4LcomLiveLoadType(aStldKeyList);

	CString strStldList = GetStldList();
	SetStyleRange(CGXRange().SetCols(3), CGXStyle()
			.SetControl(GX_IDS_CTRL_TABBED_COMBOBOX)
			.SetChoiceList(strStldList)
			.SetUserAttribute(GX_IDS_UA_TABLIST_KEYCOL, _T("0"))
			.SetUserAttribute(GX_IDS_UA_TABLIST_TEXTCOL, _T("0"))
			.SetUserAttribute(GX_IDS_UA_TABLIST_SHOWALLCOLS, _T("1"))
			.SetValueType(GX_VT_STRING)
			.SetHorizontalAlignment(DT_CENTER)
			.SetValue("")
			);

	CString strTypeList = GetTypeList();
	SetStyleRange(CGXRange().SetCols(4), CGXStyle()
		.SetControl(GX_IDS_CTRL_TABBED_COMBOBOX)
		.SetChoiceList(strTypeList)
		.SetUserAttribute(GX_IDS_UA_TABLIST_KEYCOL, _T("0"))
		.SetUserAttribute(GX_IDS_UA_TABLIST_TEXTCOL, _T("0"))
		.SetUserAttribute(GX_IDS_UA_TABLIST_SHOWALLCOLS, _T("1"))
		.SetValueType(GX_VT_STRING)
		.SetHorizontalAlignment(DT_CENTER)
		.SetValue("")
	);
	
	// 현재 셀의 border 설정 변경
	CGXProperties* pProp = GetParam()->GetProperties();
			pProp->SetUserProperty(GX_IDS_OUTLINECURRENTCELL,
		      (CGXStyle) pProp->sInvertNoBorder);

	// 현재 셀 설정
	SetRightValidCell(1, 0);
	GetParam()->SetHideCurrentCell(GX_HIDE_NEVER);

	// Sorting 정보 초기화
	m_aSortInfo.SetSize(2);
	m_aSortInfo[0].nRC = 1; // Serial No
	m_aSortInfo[0].sortType = CGXSortInfo::numeric;
	m_aSortInfo[0].sortOrder = CGXSortInfo::ascending;
	m_aSortInfo[1].nRC = 0; // key
	m_aSortInfo[1].sortType = CGXSortInfo::numeric;
	m_aSortInfo[1].sortOrder = CGXSortInfo::ascending;

	// always show vertical scrollbar
	SetScrollBarMode(SB_VERT, gxnEnabled | gxnEnhanced);  

	// Row 이동을 불가능하게 한다.
	GetParam()->EnableMoveRows(FALSE);

	// Insert key에 의한 삽입 금지
	EnableInsertRowByPressingKey(FALSE);

	// Column 크기 설정
	SetColWidthByDPI(1, 1, 0);
	SetColWidthByDPI(2, 2, 60);
	SetColWidthByDPI(3, 3, 80);
	
	MakeItemEx();
	LockUpdate(FALSE);
}

/////////////////////////////////////////////////////////////////////////////
// CCmdLcomLiveLoadTypeGrid Overridable Functions
void CCmdLcomLiveLoadTypeGrid::OnChangedSelectionPublic(long *keys, int nSize)
{
	if (nSize <= 0)
	{
		m_aSelectedKeys.RemoveAll();
	}
	else
	{
		m_aSelectedKeys.RemoveAll();
		for (int i = 0; i < nSize; i++) m_aSelectedKeys.Add((T_STLD_K)keys[i]);
	}
}

long CCmdLcomLiveLoadTypeGrid::GetSelectedKeys(CArray<long, long> &caKey)
{
	// override this function to return selected record count
	long nKeyCount;
	CArray<T_STLD_K, T_STLD_K> aSelKey;

	GetAllSelectedLcom(aSelKey);
	nKeyCount = aSelKey.GetSize();
	caKey.SetSize(nKeyCount);
	for (long i = 0; i < nKeyCount; i++)
		caKey[i] = (long)aSelKey[i];
	
	return nKeyCount;
}

BOOL CCmdLcomLiveLoadTypeGrid::ValidateField(CString value, int nColID)
{
	if (nColID >= GetColCount())
	{
		SetWarningText(_LS(IDS_WG_CMD__ADDD___CCMLoadCombMainGrid__ValidateFie));
		return FALSE;
	}

	return TRUE;
}

// 삽입이나 수정전에 Record의 Validation을 검사한다.
BOOL CCmdLcomLiveLoadTypeGrid::ValidateRecord(CStringArray &value, CRowColArray &aCols)
{
	CString msg = "";
	int nPos;

	// serial no, Description, Active, Elastic 검사 안한다.
	for (int i = 2; i < GetColCount()-3; i++)
	{
		nPos = aCols[i]-1;
		if (value[nPos] == "")
			msg += ((msg != "") ? "\n" : "") + m_aTitle[i] + _LS(IDS_WG_CMD__ADDD___has_not_been_entered_);
	}
	if (msg == "") return TRUE;
	else SetWarningText(msg);
	return FALSE;
}

// 이 함수는 Paste할 때 호출되므로 False를 리턴할 때는 ValidateField와 
// Validate Record에서 설정된 Warning Text를 없애주고 리턴한다.
BOOL CCmdLcomLiveLoadTypeGrid::ValidateAndMakeRecord(CStringArray &value, CRowColArray& aCols, T_STLD_K &key, T_LLLT_Data &data)
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
	int nPos;
	for (int i = 0; i < GetColCount(); i++)
	{
		nPos = aCols[i]-1; // col id i에 해당하는 value가 저장된 위치
		ConvStrToData(i, value[nPos], data);
	}

	key = data.StldKey;

	return TRUE;
}

BOOL CCmdLcomLiveLoadTypeGrid::AddToDB(ROWCOL nRow)
{
	// Record를 만들어서 DB에 입력한다.
	T_STLD_K key;
	T_LLLT_Data data;
 
	GetValue(nRow, key, data);

	T_LLLT_Data TempD;
	if(m_mLcomLiveLoadType.Lookup(key, TempD))
	{
		AfxMessageBox(_LS(IDS_CMD_LLLT_EXIST_LOADCASE));
		return FALSE;
	}

// 	if(data.dPsi0<0. || data.dPsi1<0. || data.dPsi2<0.)
// 	{
// 		AfxMessageBox(_LS(IDS_CMD_LLLT_PSI_FACTOR_SCOPE_ERROR));
// 		return FALSE;
// 	}

	m_mLcomLiveLoadType.SetAt(key, data);
	m_mIndex.SetAt(nRow, key);
	InsertRecord(nRow);
	SetValue(nRow, key, data);

	return TRUE;
}

BOOL CCmdLcomLiveLoadTypeGrid::AddToDB(CArray<void*, void*>& raKey, CArray<void*, void*>& raData)
{
	ASSERT(0);
	return TRUE;
}

BOOL CCmdLcomLiveLoadTypeGrid::DeleteFromDB(ROWCOL nRow)
{
	T_STLD_K key;
	T_LLLT_Data data;
 
	GetValue(nRow, key, data);

	T_LLLT_Data TempD;
	if(!m_mLcomLiveLoadType.Lookup(key, TempD)) return FALSE;
	m_mLcomLiveLoadType.RemoveKey(key);
	
	T_STLD_K TempK;
	int nIndexCount = m_mIndex.GetCount();
	for(int i=1; i<=nIndexCount; i++)
	{
		if(i<=nRow) continue;

		if(!m_mIndex.Lookup(i, TempK)) {ASSERT(0); continue;}
		m_mIndex.SetAt(i-1, TempK);
	}
	m_mIndex.RemoveKey(nIndexCount);
	
	DeleteRecord(nRow);

	CString str;
	int nSize = m_mLcomLiveLoadType.GetCount();
	for(int i=0; i<nSize; i++)
	{
		str.Format(_T("%d"), i+1);
		SetExpressionRowCol(i+1, 2, str);
	}

	return TRUE;
}

BOOL CCmdLcomLiveLoadTypeGrid::DeleteFromDB(CRowColArray& awRows)
{
	if (awRows.GetSize() <= 0) return TRUE;

	// _LS(IDS_WG_CMD__ADDD__Name) 칼럼을 얻는다.
	ROWCOL nCol;
	VERIFY(GetColumnNoFor(NAME_COL_ID, nCol));

	CString value;
	CArray<CString, CString&> awKeys;
 
	for (int i = 0; i < awRows.GetSize(); i++)
	{
		if(!DeleteFromDB(awRows.GetAt(i))) return FALSE;
	}

	return TRUE;
}

BOOL CCmdLcomLiveLoadTypeGrid::ModifyToDB(ROWCOL nRow)
{
	ROWCOL nCol;
	VERIFY(GetColumnNoFor(NAME_COL_ID, nCol));  // load case name

	T_STLD_K key;
	T_LLLT_Data data;
	GetValue(nRow, key, data);

	T_STLD_K OldKey=NULL;
	m_mIndex.Lookup(nRow, OldKey);

	T_LLLT_Data TempD;
	if(OldKey!=key && m_mLcomLiveLoadType.Lookup(key, TempD))
	{
		AfxMessageBox(_LS(IDS_CMD_LLLT_EXIST_LOADCASE));
		return FALSE;
	}
	
// 	if(data.dPsi0<0. || data.dPsi1<0. || data.dPsi2<0.)
// 	{
// 		AfxMessageBox(_LS(IDS_CMD_LLLT_PSI_FACTOR_SCOPE_ERROR));
// 		return FALSE;
// 	}

	m_mLcomLiveLoadType.SetAt(key, data);
	
	if(OldKey!=NULL && OldKey!=key)
	{
		m_mLcomLiveLoadType.RemoveKey(OldKey);
	}
	m_mIndex.SetAt(nRow, key);

	////////////////////////////////////////////////////
	// 들어간 순서를 맞춰주기 위해서
	T_STLD_K TempKey=NULL;
	int nIndexCount = m_mIndex.GetCount();
	for(int i=1; i<=nIndexCount; i++)
	{
		if(i<nRow) continue;

		m_mIndex.Lookup(i, TempKey);
		m_mLcomLiveLoadType.Lookup(TempKey, TempD);

		m_mLcomLiveLoadType.RemoveKey(TempKey);
		m_mLcomLiveLoadType.SetAt(TempKey, TempD);
	}
	////////////////////////////////////////////////////

	DeleteRecord(nRow);
	InsertRecord(nRow);
	SetValue(nRow, key, data);

	return TRUE;
}

BOOL CCmdLcomLiveLoadTypeGrid::ModifyToDB(CStringArray& raOldKey,
												 CArray<void*, void*>& raKey, 
												 CArray<void*, void*>& raData)
{
	ASSERT(0);
	return TRUE;
}

CString CCmdLcomLiveLoadTypeGrid::GetNewKey()
{
	return "";
}

CString CCmdLcomLiveLoadTypeGrid::GetDefValue(int nIndex)
{
	return m_aCurDefVal[nIndex];
}

CString* CCmdLcomLiveLoadTypeGrid::GetDefValueArray()
{
	return m_aCurDefVal;
}

BOOL CCmdLcomLiveLoadTypeGrid::MakeKeyAndDataArray(CStringArray& value, CRowColArray& awCols,
																	CArray<void*, void*>& aKey, CArray<void*, void*>& aData)
{
	T_STLD_K* pKey;
	T_LLLT_Data* pData;
	pKey = new T_STLD_K;
	pData = new T_LLLT_Data;
	aKey.Add((void*)pKey);
	aData.Add((void*)pData);
	if (!ValidateAndMakeRecord(value, awCols, *pKey, *pData)) return FALSE;
	return TRUE;
}

void CCmdLcomLiveLoadTypeGrid::DeleteKeyAndDataArray(CArray<void*, void*>& aKey, CArray<void*, void*>& aData)
{
	T_STLD_K* pKey;
	T_LLLT_Data* pData;
	ASSERT(aKey.GetSize() == aData.GetSize());
	for (int i = 0; i < aKey.GetSize(); i++)
	{
		pKey = (T_STLD_K*)aKey.GetAt(i);
		pData = (T_LLLT_Data*)aData.GetAt(i);
		delete pKey;
		delete pData;
	}
}

long CCmdLcomLiveLoadTypeGrid::GetCountData()
{
	return m_mLcomLiveLoadType.GetCount();
}

void CCmdLcomLiveLoadTypeGrid::MakeItemFiltered(long nFilteredNum, long* aFilteredKeys, CRowColArray& awCols)
{
	CDBDoc* pDoc = m_pDoc;
	T_LLLT_Data Data;

	for(long nCount=0 ; nCount < nFilteredNum; nCount++)
 	{
		m_mLcomLiveLoadType.Lookup(aFilteredKeys[nCount], Data);
		SetValue(GetParam()->GetData(), nCount+1, aFilteredKeys[nCount], Data, awCols);
	}
}

void CCmdLcomLiveLoadTypeGrid::MakeItemNormal(long nNumData, CRowColArray& awCols)
{
	T_STLD_K Key;
	T_LLLT_Data Data;

	CArray<T_STLD_K, T_STLD_K> aStldK;
	m_mLcomLiveLoadType.GetKeyList(aStldK);
	if(nNumData!=aStldK.GetSize()) {ASSERT(0); return;}

	for(long nCount=1 ; nCount <= nNumData; nCount++)
	{
		if(!m_mIndex.Lookup(nCount, Key)) {ASSERT(0); continue;}
		if(!m_mLcomLiveLoadType.Lookup(Key, Data)) {ASSERT(0); continue;}
		SetValue(GetParam()->GetData(), nCount, Key, Data, awCols);
	}
}

void CCmdLcomLiveLoadTypeGrid::MakeItemEditingRecord(ROWCOL ncRow, CStringArray& EditingRecord)
{
	CDBDoc* pDoc = m_pDoc;
	T_STLD_K Key;
	T_LLLT_Data Data;
	for (int i = 0; i < GetColCount(); i++)
	{
		if (i == 0) Key = _ttol(EditingRecord[i]);
		else ConvStrToData(i, EditingRecord[i], Data);
	}
	
	SetValue(ncRow, Key, Data);
}

/////////////////////////////////////////////////////////////////////////////
// CCmdLcomLiveLoadTypeGrid Implementation
BOOL CCmdLcomLiveLoadTypeGrid::GetValue(ROWCOL nRow, T_STLD_K &key, T_LLLT_Data &data)
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

		ConvStrToData(i, str, data);
	}

	key = data.StldKey;

	return TRUE;
}

BOOL CCmdLcomLiveLoadTypeGrid::SetValue(ROWCOL nRow, const T_STLD_K &key, const T_LLLT_Data &data)
{
	//
	CRowColArray awCols;
	awCols.SetSize(GetColCount());
	for (int i = 0;i < GetColCount(); i++)
		VERIFY(GetColumnNoFor(m_aTitle[i], awCols[i]));
	return SetValue(GetParam()->GetData(), nRow, key, data, awCols);
}

BOOL CCmdLcomLiveLoadTypeGrid::SetValue(CGXData* pData, ROWCOL nRow, const T_STLD_K &key, const T_LLLT_Data &data, CRowColArray &awCols)
{
	CString str;
	for (int i = 0; i < GetColCount(); i++)
	{
		if(i==0) 
		{
			if (key == 0) str.Empty();
			else str.Format(LCOM_KEY_FMT,key);
		}
		else if(i==1)
		{
			str.Format(_T("%d"), nRow);
		}
		else ConvDataToStr(i, data, str);
		pData->StoreValueRowCol(nRow, awCols[i], str, gxOverride);
	}

	return TRUE;
}

void CCmdLcomLiveLoadTypeGrid::ConvStrToData(int i, CString& value, T_LLLT_Data& data)
{
	if (i==2)
	{
		data.StldKey = m_pDoc->m_pAttrCtrl->GetStldKey(value);
		data.LoadCaseName = value; // name
	}
	else if (i == 3)
	{
		if	    (value == csLLMax) data.LLType = T_LLLT_D::EN_TYPE_LLMAX;
		else if (value == csLLins) data.LLType = T_LLLT_D::EN_TYPE_LLINS;
		else if (value == csLLmed) data.LLType = T_LLLT_D::EN_TYPE_LLMED;
		else { ASSERT(0); data.LLType = T_LLLT_D::EN_TYPE_LLMAX; }
	}
}

void CCmdLcomLiveLoadTypeGrid::ConvDataToStr(int i, const T_LLLT_Data& data, CString& value)
{
	if (i==2) value = data.LoadCaseName;
	else if (i == 3)
	{
		if      (data.LLType == T_LLLT_D::EN_TYPE_LLMAX) value = csLLMax;
		else if (data.LLType == T_LLLT_D::EN_TYPE_LLINS) value = csLLins;
		else if (data.LLType == T_LLLT_D::EN_TYPE_LLMED) value = csLLmed;
		else { ASSERT(0); value = csLLMax; }
	}
}

void CCmdLcomLiveLoadTypeGrid::MakeSearchKey(const T_STLD_K &key, const T_LLLT_Data &data, CStringArray &aKey)
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

void CCmdLcomLiveLoadTypeGrid::GetAllSelectedLcom(CArray<T_STLD_K, T_STLD_K>& aSelKey)
{
	for (int i = 0; i < m_aSelectedKeys.GetSize(); i++)
		aSelKey.Add(m_aSelectedKeys[i]);
}

CString CCmdLcomLiveLoadTypeGrid::GetKeyForRow(ROWCOL ncRow, ROWCOL nkCol)
{
	CString sKey;

	if (IsAppendRow(ncRow)) sKey = _LS(IDS_WG_CMD__ADDD__Append_Row);
	else if (m_nEditMode == addnew) sKey = _LS(IDS_WG_CMD__ADDD__New_Row);
	else if (m_nEditMode == edit) sKey = GetValueRowColBeforeEdit(ncRow, nkCol);
	else sKey = GetValueRowCol(ncRow, nkCol);

	return sKey;
}

void CCmdLcomLiveLoadTypeGrid::SetHeaderTitle(T_UNIT_SYSTEM &UnitSystem, BOOL bInit)
{
	SetHeaderTitle2((int*)m_aUnit.GetData(), bInit, TRUE);
}

BOOL CCmdLcomLiveLoadTypeGrid::GetStyleRowCol(ROWCOL nRow, ROWCOL nCol, CGXStyle& style, GXModifyType mt, int nType)
{
	CTBCommon::GetStyleRowCol(nRow, nCol, style, mt, nType);

	return TRUE;
}

CString CCmdLcomLiveLoadTypeGrid::GetStldList()
{
	CString strStldList = _T("");

	CArray<T_STLD_K, T_STLD_K> aStldKeyList; aStldKeyList.RemoveAll();
	m_pDoc->m_pAttrCtrl2->GetStldKeyList4LcomLiveLoadType(aStldKeyList);

	T_STLD_D StldD;
	int nSize = aStldKeyList.GetSize();
	for(int i=0; i<nSize; i++)
	{
		if(!m_pDoc->m_pAttrCtrl->GetStld(aStldKeyList[i], StldD)) {ASSERT(0); continue;}
		strStldList += StldD.LoadCaseName;
		strStldList += "\n";
	}
	
	return strStldList;
}
CString CCmdLcomLiveLoadTypeGrid::GetTypeList()
{
	CString strTypeList = _T("");
	strTypeList = csLLMax + _T("\n") + csLLins + _T("\n") + csLLmed;

	return strTypeList;
}
BOOL CCmdLcomLiveLoadTypeGrid::OnPasteFromClipboard(const CGXRange &range)
{
	AfxMessageBox(_LS(IDS_CMD_DONT_SUPPORT_PASTE));
	return FALSE;
}

BOOL CCmdLcomLiveLoadTypeGrid::OnInsertEmptyRecord(ROWCOL nRow)
{
	BOOL bResult = CTBCommon::OnInsertEmptyRecord(nRow);

	// 여기서는 Default 값을 채워 넣는 역할을 하면 된다.
	ROWCOL ncRow, ncCol;
	CString value;
		
	VERIFY(GetCurrentCell(ncRow, ncCol));
	CGXControl* pControl = (CGXControl*)GetControl(ncRow, ncCol);

	if (pControl && ncCol == 3) // Load Case
	{
		CString strLoadCaseName=_T("");
		pControl->GetValue(strLoadCaseName);
		T_STLD_D StldD;
		m_pDoc->m_pAttrCtrl->GetStld(strLoadCaseName, StldD);
		SetExpressionRowCol(nRow, 4, csLLMax);
	}

	return bResult;
}

void CCmdLcomLiveLoadTypeGrid::OnModifyCell(ROWCOL nRow, ROWCOL nCol)
{
	CTBCommon::OnModifyCell(nRow, nCol);
}
