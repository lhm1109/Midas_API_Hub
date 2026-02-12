// MeshRatioList.cpp : implementation file
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SectRForcGrid.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl2.h"
#include "..\wg_db\LdfsUtil.h"   // for CLdfsUtil

#include "..\wg_base\CompFunc.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

UINT CSectRForcGrid::WM_SECT_RFORC_GRID_VSEC_CLICKED = RegisterWindowMessage(_T("WM_SECT_RFORC_GRID_VSEC_CLICKED"));

/////////////////////////////////////////////////////////////////////////////
// CSectRForcGrid
CString CSectRForcGrid::m_aCurDefVal[] = { _T("-1") ,_T(""), _T("0.0"), _T("0.0"), _T("0.0") };
CSectRForcGrid::CSectRForcGrid()
{
	Init_Construction();
}

CSectRForcGrid::CSectRForcGrid(CWnd* pParent)
{
	Init_Construction();
}

CSectRForcGrid::~CSectRForcGrid()
{
}

void CSectRForcGrid::Init_Construction()
{
	T_VSEC_UNIT * pUnit = &(CUnitCtrl::m_VSEC_UNIT);

	AddColInfo(_T("ID"),      t_integer, 0); // Key.
	AddColInfo(_LS(IDS_TM_SECT_RFORC_Name),      t_string, D_UNITSYS_NONE); //Name
	AddColInfo(_LS(IDS_TM_SECT_RFORC_X   ),      t_real,  D_UNITSYS_NONE); // X    // 단위계 표시 안함. 단위계 표시시 단위 변환때마다 Header 표시를 강제로 바꿔야 함.
	AddColInfo(_LS(IDS_TM_SECT_RFORC_Y   ),      t_real,  D_UNITSYS_NONE); // Y    // 단위계 표시 안함. 단위계 표시시 단위 변환때마다 Header 표시를 강제로 바꿔야 함.
	AddColInfo(_LS(IDS_TM_SECT_RFORC_Z   ),      t_real,  D_UNITSYS_NONE); // Z    // 단위계 표시 안함. 단위계 표시시 단위 변환때마다 Header 표시를 강제로 바꿔야 함.

	m_aPrimaryKey.Add(0);
}

BEGIN_MESSAGE_MAP(CSectRForcGrid, CTBCommon)
	//{{AFX_MSG_MAP(CSectRForcGrid)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CSectRForcGrid message handlers

void CSectRForcGrid::Initialize()
{
	T_UNIT_SYSTEM us;
	CDBDoc *pDoc = CDBDoc::GetDocPoint();
	CTBCommon::Initialize(pDoc, us);

	LockUpdate(TRUE);

	SetHeaderTitle(us, TRUE);
	SetRowHeight(0, 0, globalUtils.ScaleByDPI(20));

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
	//GetParam()->SetNumberedRowHeaders(TRUE);

	// 컬럼 스타일 변경
	SetStyleRange(CGXRange().SetCols(1, GetInitialColCount()), CGXStyle()     // 전체 컬럼의 스타일 변경 
			.SetHorizontalAlignment(DT_CENTER)
			.SetValueType(GX_VT_NUMERIC)
			.SetFormat(GX_FMT_FIXED)
			.SetPlaces(7));

	SetStyleRange(CGXRange().SetCols(1), CGXStyle()         // 1번째 컬럼 Key 
			.SetPlaces(0)
			.SetEnabled(FALSE));
	//HideColsByIntend(1, 1, TRUE);

	SetStyleRange(CGXRange().SetCols(2), CGXStyle()         // 2번째 컬럼 Name
			.SetValueType(GX_VT_STRING)
			.SetHorizontalAlignment(DT_LEFT)
			.SetValue(_T(""))
			.SetEnabled(TRUE)
			);
	
	SetStyleRange(CGXRange().SetCols(3, 5), CGXStyle()      // 3,4,5 번째 컬럼 X,Y,Z
			.SetFont(CGXFont_GC().SetSize(9))
			.SetValueType(GX_VT_NUMERIC)
			.SetFormat(GX_FMT_FIXED)
			.SetPlaces(4)
			.SetEnabled(TRUE));

	SetColumnAttr();

//   // 현재 셀 설정
//   GetParam()->SetHideCurrentCell(GX_HIDE_NEVER);
	GetParam()->SetEnterKeyAction(GX_DOWN);
	EnableInsertRowByPressingKey(FALSE);  // Disable inserting row by key
	EnableAppendRow(FALSE); // append 금지

	// Row 헤더와 Column 헤더를 숨긴다.
	//HideRows(0, 0);
	HideCols(0, 0);

//   // Sorting 정보 초기화
//   m_aSortInfo.SetSize(2);
//   m_aSortInfo[0].nRC = 1;
//   m_aSortInfo[0].sortType = CGXSortInfo::numeric;
//   m_aSortInfo[0].sortOrder = CGXSortInfo::ascending;
//   m_aSortInfo[1].nRC = 2;
//   m_aSortInfo[1].sortType = CGXSortInfo::alphanumeric;
//   m_aSortInfo[1].sortOrder = CGXSortInfo::ascending;

	MakeItemEx();

	//SetScrollBarMode(SB_VERT, gxnEnabled | gxnEnhanced);
	SetScrollBarMode(SB_BOTH, gxnAutomatic | gxnEnhanced);
	this->EnableInsertRowByPressingKey(FALSE);  // Insert Key에 의한 Record 삽입 금지

	LockUpdate(FALSE);
}

BOOL CSectRForcGrid::InsertRecordByKey(const int &key, const T_VSEC_D &data)
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

BOOL CSectRForcGrid::DeleteRecordByKey(const int &key, const T_VSEC_D &data)
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
// CSectRForcGrid Overridable Functions
void CSectRForcGrid::OnChangedSelectionPublic(long *keys, int nSize)
{
	return;
}

long CSectRForcGrid::GetSelectedKeys(CArray<long, long> &caKey)
{
	return 0;
}

int CSectRForcGrid::GetSelectedVsecKList(CArray<UINT, UINT>& aVsecK)
{
	aVsecK.RemoveAll();

	// get key
	ROWCOL nkCol;
	GetColumnNoFor(GetKeyColID(), nkCol);

	CRowColArray awRows;
	ROWCOL nSel = GetSelectedRows(awRows);
	for (int i=0; i<nSel; ++i)
	{
		T_VSEC_K VsecK = _ttol(GetValueRowCol(awRows[i], nkCol));
		if (VsecK == 0)  continue;

		aVsecK.Add(VsecK);
	}

	return aVsecK.GetSize();
}

BOOL CSectRForcGrid::OnLButtonClickedRowCol(ROWCOL nRow, ROWCOL nCol, UINT nFlags, CPoint pt)
{
	BOOL bRet = CTBCommon::OnLButtonClickedRowCol(nRow, nCol, nFlags, pt);

	// get key
	ROWCOL nkCol;
	GetColumnNoFor(GetKeyColID(), nkCol);
	T_VSEC_K VsecK = _ttol(GetValueRowCol(nRow, nkCol));

	// TODO: Add your control notification handler code here
	CWnd* pParentWnd = GetParent();
	if(pParentWnd && pParentWnd->GetSafeHwnd())   
		pParentWnd->PostMessage(CSectRForcGrid::WM_SECT_RFORC_GRID_VSEC_CLICKED, (WPARAM)VsecK, (LPARAM)0);	

	return bRet;
}

BOOL CSectRForcGrid::ValidateField(CString value, int nColID)
{
	if (nColID >= GetColCount())
	{
		SetWarningText(_LS(IDS_LBC_ERR_OUT_OF_RANGE));
		return FALSE;
	}

	int nErrCode;

	if (nColID == 0) return TRUE;   // ID : error check 없음
	else if( nColID == 1)           // Name
	{
		nErrCode; // Do Nothing...
	}
	else if(nColID == 2 || nColID == 3 || nColID == 4)    // X, Y, Z
	{    
		if (!IsValidDoubleNumber(value, nErrCode)) return InvalidDoubleNumber(nErrCode);
	}

	return TRUE;
}

// 삽입이나 수정전에 Record의 Validation을 검사한다.
BOOL CSectRForcGrid::ValidateRecord(CStringArray &value, CRowColArray &aCols)
{//?
	CString msg = _T("");
	int nPos;

	for (int i = 0; i < GetColCount(); i++)
	{
		if (i == 0) continue;
		nPos = aCols[i]-1;
 
		if (value[nPos] == _T("")) value[nPos] = _T("0."); // customized! (Y.Jee)
			
		else if (!ValidateField(value[nPos], i)) return FALSE;
	}
	if (msg == _T("")) return TRUE;
	else SetWarningText(msg);
	return FALSE;
 
	return TRUE;
}

// 이 함수는 Paste할 때 호출되므로 False를 리턴할 때는 ValidateField와 
// Validate Record에서 설정된 Warning Text를 없애주고 리턴한다.
BOOL CSectRForcGrid::ValidateAndMakeRecord(CStringArray &value, CRowColArray& aCols, int &key, T_VSEC_D &data)
{
	// 레코드에 대해 Validation을 검사한다.
	if (!ValidateRecord(value, aCols))
		return FALSE;

	// 레코드를 만들어서 리턴한다.
	int nPos;
	for (int i = 0; i < GetColCount(); i++)
	{
		nPos = aCols[i]-1; // col id i에 해당하는 value가 저장된 위치
		if(i==0) key = _ttoi(value[nPos]);
		else ConvStrToData(i, value[nPos], data);
	}
	return TRUE;
}

BOOL CSectRForcGrid::AddToDB(ROWCOL nRow)
{
	if (!CanSaveData()) return FALSE;

	// Record를 만들어서 DB에 입력한다.
	T_VSEC_K key;
	T_VSEC_D data;

	GetValue(nRow, key, data);
	return m_pDoc->m_pDataCtrl->AddVsec(data);  
}

BOOL CSectRForcGrid::AddToDB(CArray<void*, void*>& raKey, CArray<void*, void*>& raData)
{
	ASSERT(raKey.GetSize() == raData.GetSize());
	T_VSEC_K tKey;
	CArray<T_VSEC_K, T_VSEC_K> aKey;
	CArray<T_VSEC_D, T_VSEC_D&> aData;
	for (int i = 0; i < raKey.GetSize(); i++)
	{
		tKey = *((T_VSEC_K*)raKey.GetAt(i));
		aKey.Add(tKey);
		aData.Add(*((T_VSEC_D*)raData.GetAt(i)));
	}
	return m_pDoc->m_pDataCtrl->AddVsec(aData);
}

BOOL CSectRForcGrid::DeleteFromDB(ROWCOL nRow)
{
	T_VSEC_K key;
	T_VSEC_D data;

	GetValue(nRow, key, data);
	return m_pDoc->m_pDataCtrl->DelVsec(key);
}

BOOL CSectRForcGrid::DeleteFromDB(CRowColArray& awRows)
{
	if (awRows.GetSize() <= 0) return TRUE;

	T_VSEC_K key;
	T_VSEC_D data;
	CArray<T_VSEC_K, T_VSEC_K> awKeys;  awKeys.RemoveAll();

	for (int i = 0; i < awRows.GetSize(); i++)
	{
		GetValue(awRows.GetAt(i), key, data);
		awKeys.Add(key);
	}

	return m_pDoc->m_pDataCtrl->DelVsec(awKeys);
}

BOOL CSectRForcGrid::ModifyToDB(ROWCOL nRow)
{
	if (!CanSaveData()) return FALSE;

	T_VSEC_K key;
	T_VSEC_D data;
	GetValue(nRow, key, data);

	data.nCentCalcType = 1;   // 도심 계산 방법 (=0: Auto, =1: User). 수정시 도심 계산방법을 user로 변경.

	return m_pDoc->m_pDataCtrl->ModifyVsec(key, data);
}

BOOL CSectRForcGrid::ModifyToDB(CStringArray& raOldKey, 
												 CArray<void*, void*>& raKey, 
												 CArray<void*, void*>& raData)
{
	ASSERT(raKey.GetSize() == raData.GetSize());
	ASSERT(raKey.GetSize() == raOldKey.GetSize());

	CArray<T_VSEC_K,T_VSEC_K> aVsecK_old;   aVsecK_old.RemoveAll();
	T_VSEC_K key;
	T_VSEC_D data;
	CArray<T_VSEC_D, T_VSEC_D&> aData;
	for (int i = 0; i < raKey.GetSize(); i++)
	{
		key = _ttol(raOldKey.GetAt(i));
		m_pDoc->m_pAttrCtrl2->GetVsec(key, data);
		aVsecK_old.Add(key);

		T_VSEC_D& TempVsec = *((T_VSEC_D*)raData.GetAt(i));
		data.strName = TempVsec.strName;
		data.dCentX  = TempVsec.dCentX;
		data.dCentY  = TempVsec.dCentY;
		data.dCentZ  = TempVsec.dCentZ;
		data.nCentCalcType = 1;    // 도심 계산 방법 (=0: Auto, =1: User). 수정시 도심 계산방법을 user로 변경.

		aData.Add(data);
	}

	return m_pDoc->m_pDataCtrl->ModifyVsec(aVsecK_old, aData);
}

CString CSectRForcGrid::GetNewKey()
{
	CString value = _T("");
	T_VSEC_K key = m_pDoc->m_pAttrCtrl2->GetStartNumVsec();
	value.Format(_T("%d"), key);
	return value;
}

CString CSectRForcGrid::GetDefValue(int nIndex)
{
	return m_aCurDefVal[nIndex];
}

CString* CSectRForcGrid::GetDefValueArray()
{
	return m_aCurDefVal;
}

BOOL CSectRForcGrid::MakeKeyAndDataArray(CStringArray& value, CRowColArray& awCols, 
																	CArray<void*, void*>& aKey, CArray<void*, void*>& aData)
{
	int* pKey;
	T_VSEC_D* pData;
	pKey = new int;
	pData = new T_VSEC_D;
	aKey.Add((int*)pKey);
	aData.Add((T_VSEC_D*)pData);
	if (!ValidateAndMakeRecord(value, awCols, *pKey, *pData)) return FALSE;
	return TRUE;
}

void CSectRForcGrid::DeleteKeyAndDataArray(CArray<void*, void*>& aKey, CArray<void*, void*>& aData)
{
	int* pKey;
	T_VSEC_D* pData;
	ASSERT(aKey.GetSize() == aData.GetSize());
	for (int i = 0; i < aKey.GetSize(); i++)
	{
		pKey = (int*)aKey.GetAt(i);
		pData = (T_VSEC_D*)aData.GetAt(i);
		delete pKey;
		delete pData;
	}
}

long CSectRForcGrid::GetCountData()
{
	CDBDoc* pDoc = m_pDoc;
	return pDoc->m_pAttrCtrl2->GetCountVsec();
}

void CSectRForcGrid::MakeItemFiltered(long nFilteredNum, long* aFilteredKeys, CRowColArray& awCols)
{
	CDBDoc* pDoc = m_pDoc;
	T_VSEC_D Data;

	for(long nCount=0 ; nCount < nFilteredNum; nCount++)
	{
		if (pDoc->m_pAttrCtrl2->GetVsec(aFilteredKeys[nCount], Data))
		{
			if (Data.nCentCalcType == 0)    // 도심 자동계산
			{
				CLdfsUtil utilLdfs;
				T_COMPSECT_INFO infoSect;   infoSect.Initialize();
				if (utilLdfs.GetCompSectInforFromVsec(Data, infoSect))  { Data = infoSect.VsecD; }
			}
		}
		SetValue(GetParam()->GetData(), nCount+1, aFilteredKeys[nCount], Data, awCols);
	}
}

void CSectRForcGrid::MakeItemNormal(long nNumData, CRowColArray& awCols)
{
	T_VSEC_K Key;
	T_VSEC_D Data;
	CDBDoc* pDoc = m_pDoc;

	long* KeyBuf = new long[nNumData];
	long nCount = 0;
	POSITION pos = pDoc->m_pAttrCtrl2->GetStartVsec();
	while(pos != NULL)
	{
		pDoc->m_pAttrCtrl2->GetNextVsec(pos,Key,Data);
		KeyBuf[nCount++] = Key;
	}
	qsort(KeyBuf, nNumData, sizeof(T_VSEC_K), CCompFunc::UINTAsc);

	for(nCount=0 ; nCount < nNumData; nCount++)
	{
		if (pDoc->m_pAttrCtrl2->GetVsec(KeyBuf[nCount], Data))
		{
			if (Data.nCentCalcType == 0)    // 도심 자동계산
			{
				CLdfsUtil utilLdfs;
				T_COMPSECT_INFO infoSect;   infoSect.Initialize();
				if (utilLdfs.GetCompSectInforFromVsec(Data, infoSect))  { Data = infoSect.VsecD; }
			}
		}
		SetValue(GetParam()->GetData(), nCount+1, KeyBuf[nCount], Data, awCols);
	} // end of loop nCount

	delete []KeyBuf;
}

void CSectRForcGrid::MakeItemEditingRecord(ROWCOL ncRow, CStringArray& EditingRecord)
{
	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	int Key;
	T_VSEC_D Data;
	for (int i = 0; i < GetColCount(); i++)
	{
		if (i == 0) Key = _ttoi(EditingRecord[i]);
		else ConvStrToData(i, EditingRecord[i], Data);
	}
	pDoc->m_pUnitCtrl->ConvertUnitVsecPrevious(Data);
	// 현재 값을 설정한다.
	SetValue(ncRow, Key, Data);
}

/////////////////////////////////////////////////////////////////////////////
// CSectRForcGrid Implementation
BOOL CSectRForcGrid::GetValue(ROWCOL nRow, UINT &key, T_VSEC_D &data)
{
	if (nRow < 1) return FALSE;
	if (IsAppendRow(nRow) || nRow > GetRowCount()) return FALSE;

	ROWCOL nCol;
	CString str;
	data.Initialize();
	for (int i = 0; i < GetColCount(); i++)
	{
		VERIFY(GetColumnNoFor(m_aTitle[i], nCol));
		str = GetValueRowCol(nRow, nCol);
		// 수식인 경우 값으로 변환한다.
		// Validation이 OK인 Field이므로 항상 TRUE 리턴
		VERIFY(ConvertFormulaToValue(str)); 

		if(i==0) 
		{
			key = _ttol(str);
			if (!m_pDoc->m_pAttrCtrl2->GetVsec(key, data))   data.Initialize();
			else
			{
				if (data.nCentCalcType == 0)    // 도심 자동계산
				{
					CLdfsUtil utilLdfs;
					T_COMPSECT_INFO infoSect;   infoSect.Initialize();
					if (utilLdfs.GetCompSectInforFromVsec(data, infoSect))  { data = infoSect.VsecD; }
				}
			}
		}
		else ConvStrToData(i, str, data);
	}
	return TRUE;
}

BOOL CSectRForcGrid::SetValue(ROWCOL nRow, const UINT &key, const T_VSEC_D &data)
{
	//
	CRowColArray awCols;
	awCols.SetSize(GetColCount());
	for (int i = 0;i < GetColCount(); i++)
		VERIFY(GetColumnNoFor(m_aTitle[i], awCols[i]));
	return SetValue(GetParam()->GetData(), nRow, key, data, awCols);
}

BOOL CSectRForcGrid::SetValue(CGXData* pData, ROWCOL nRow, const UINT &key, const T_VSEC_D &data, CRowColArray &awCols)
{
	//
	CString str;
	int nValueType;
	for (int i = 0; i < GetColCount(); i++)
	{
		if(i==0) 
		{
			if (key == 0) str.Empty();
			else str.Format(_T("%d"), key);
		}
		else ConvDataToStr(i, data, str, key);

		if (m_aType[i] == t_string) nValueType = GX_VT_STRING;
		else nValueType = GX_VT_NUMERIC;
		pData->StoreValueRowCol(nRow, awCols[i], str, gxOverride, nValueType);
	}

	return TRUE;
}

void CSectRForcGrid::ConvStrToData(int i, CString& value, T_VSEC_D& data)
{
	if (i == 1) data.strName = value;    // Name
	else if (i == 2) data.dCentX = _tstof(value);  // X
	else if (i == 3) data.dCentY = _tstof(value);  // Y
	else if (i == 4) data.dCentZ = _tstof(value);  // Z
}

void CSectRForcGrid::ConvDataToStr(int i, const T_VSEC_D& data, CString& value, const T_VSEC_K VsecK)
{
	if (i == 1) value = data.strName;    // Name
	else if (i == 2) value.Format(_T("%g"), data.dCentX);    // X
	else if (i == 3) value.Format(_T("%g"), data.dCentY);    // Y
	else if (i == 4) value.Format(_T("%g"), data.dCentZ);    // Z
	else ASSERT(0);
}

void CSectRForcGrid::MakeSearchKey(const int &key, const T_VSEC_D &data, CStringArray &aKey)
{
	int nColID;
	int nSortKeyNum = m_aSortInfo.GetSize();

	aKey.SetSize(nSortKeyNum);
	for (int i = 0; i < nSortKeyNum; i++)
	{
		nColID = m_aSortInfo[i].nRC;
		if (nColID == 0)
		{
			if (key == 0) aKey[i] = GetNewKey();
			aKey[i].Format(_T("%d"), key);
		}
		else ConvDataToStr(nColID, data, aKey[i], key);
	}
}

void CSectRForcGrid::SetHeaderTitle(T_UNIT_SYSTEM &us, BOOL bInit)
{
	SetHeaderTitle2((int*)m_aUnit.GetData(), bInit, TRUE);
}

// Column 폭을 지정한다.
// Hide 시켜야 할 칼럼을 지정한다.
void CSectRForcGrid::SetColumnAttr()
{
	int anColWidth[] = {30, 55, 30, 30, 30};

	for (int i = 0; i < sizeof(anColWidth)/sizeof(int); i++)    SetColWidthByDPI(i+1, i+1, anColWidth[i]);
}

void CSectRForcGrid::UpdateBuffer()
{
	MakeItemEx();
}

void CSectRForcGrid::ConvertUnit()
{
	MakeItemEx();
}

// MNET:XXXX 20080117 mylee - Unit 변경시 회전 자유도를 길이처럼 변경시켜 주기 위해
void CSectRForcGrid::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
{
	HWND hWnd;
	if ((hWnd = GetSafeHwnd()) == 0 || !IsWindow(hWnd)) return;

	switch(lHint)
	{
	case D_UPDATE_DBALL:
	case D_UPDATE_DEFAULT:
		UpdateBuffer();
		break;
	case D_UPDATE_BUFFER_BEFORE:
		break;
	case D_UPDATE_BUFFER_AFTER:
		UpdateBuffer();
		break;
	case D_UPDATE_UNIT:
		ConvertUnit();
		break;
	case D_UPDATE_SEL_ADD: case D_UPDATE_SEL_DEL:
		break;
	case D_UPDATE_EXE_START: case D_UPDATE_EXE_END:
		break;
	default:
		//ASSERT(FALSE);
		break;
	}
}
