// MeshRatioList.cpp : implementation file
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "wg_treemenu.h"
#include "BndrMconGrid.h"
#include "BndrMconDlg.h"

#include "..\wg_db\UnitCtrl.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define EPSILON 1.e-10
#define DATA_FMT _T("%.7f")

//unsigned int CBndrMconGrid::WM_SEEDGRID_CHANGED = RegisterWindowMessage(_T("WM_SEEDRATIO_GRID_CHANGED"));

/////////////////////////////////////////////////////////////////////////////
// CBndrMconGrid
CString CBndrMconGrid::m_aCurDefVal[] = { _T("-1") ,_T("DX"), _T("0.0"), _T("0.0") };
CBndrMconGrid::CBndrMconGrid()
{
	m_pData = 0;
	m_nMasterDOF = -1;
	m_pParent = 0;

	AddColInfo(_LS(IDS_TM_MCON_KEY),      t_integer, 0);
	AddColInfo(_LS(IDS_TM_MCON_NODE),     t_integer, 0); //NODE
	AddColInfo(_LS(IDS_TM_MCON_DOF),      t_string,  0);  
	AddColInfo(_LS(IDS_TM_MCON_COEFFICIENT),      t_real,    0);
	AddColInfo(_LS(IDS_TM_MCON_WEIGHT),           t_real,    0);

	m_aPrimaryKey.Add(1);
	m_aPrimaryKey.Add(2);
}

CBndrMconGrid::CBndrMconGrid(CBndrMconDlg* pParent)
{
	m_pData = 0;
	m_nMasterDOF = -1;
	m_pParent = pParent;

	AddColInfo(_LS(IDS_TM_MCON_KEY),      t_integer, 0);
	AddColInfo(_LS(IDS_TM_MCON_NODE),     t_integer, 0); //NODE
	AddColInfo(_LS(IDS_TM_MCON_DOF),      t_string,  0);  
	AddColInfo(_LS(IDS_TM_MCON_COEFFICIENT),      t_real,    0);
	AddColInfo(_LS(IDS_TM_MCON_WEIGHT),           t_real,    0);

	m_aPrimaryKey.Add(1);
	m_aPrimaryKey.Add(2);
}

CBndrMconGrid::~CBndrMconGrid()
{
}


BEGIN_MESSAGE_MAP(CBndrMconGrid, CTBCommon)
	//{{AFX_MSG_MAP(CBndrMconGrid)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CBndrMconGrid message handlers

void CBndrMconGrid::Initialize(T_MCON_DT* pData)
{
	ASSERT(pData);
	m_pData    = pData;
	
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
	GetParam()->SetNumberedRowHeaders(TRUE);

	// 컬럼 스타일 변경
	SetStyleRange(CGXRange().SetCols(1, m_nColCount), CGXStyle()     // 전체 컬럼의 스타일 변경 
			.SetHorizontalAlignment(DT_CENTER)
			.SetValueType(GX_VT_NUMERIC)
			.SetFormat(GX_FMT_FIXED)
			.SetPlaces(7));

	SetStyleRange(CGXRange().SetCols(1), CGXStyle()         // 1번째 컬럼 Key 
			.SetPlaces(0)
			.SetEnabled(FALSE));
	SetStyleRange(CGXRange().SetCols(2), CGXStyle()         // 2번째 컬럼 Node
			.SetPlaces(0));

	
	CString szChoiceList;

	szChoiceList += CString(_LS(IDS_TM_MCON_DX)) + _T("\n");             // 3번쨰 컬럼 DOF      
	szChoiceList += CString(_LS(IDS_TM_MCON_DY)) + _T("\n");
	szChoiceList += CString(_LS(IDS_TM_MCON_DZ)) + _T("\n");
	szChoiceList += CString(_LS(IDS_TM_MCON_RX)) + _T("\n");
	szChoiceList += CString(_LS(IDS_TM_MCON_RY)) + _T("\n");
	szChoiceList += CString(_LS(IDS_TM_MCON_RZ)) + _T("\n");
	
	SetStyleRange(CGXRange().SetCols(3), CGXStyle()
			.SetControl(GX_IDS_CTRL_TABBED_COMBOBOX)
			.SetChoiceList(szChoiceList)
			.SetUserAttribute(GX_IDS_UA_TABLIST_KEYCOL, _T("0"))
			.SetUserAttribute(GX_IDS_UA_TABLIST_TEXTCOL, _T("0"))
			.SetUserAttribute(GX_IDS_UA_TABLIST_SHOWALLCOLS, _T("1"))
			.SetValueType(GX_VT_STRING)
			.SetHorizontalAlignment(DT_LEFT)
			.SetValue(_T(""))
			);

	SetStyleRange(CGXRange().SetCols(4, 5), CGXStyle()      // 4,5 번째 컬럼 Coeff. or Weight
		.SetFont(CGXFont_GC().SetSize(9))
		.SetValueType(GX_VT_NUMERIC)
		.SetHorizontalAlignment(DT_CENTER)
		.SetVerticalAlignment(DT_VCENTER)
		.SetEnabled(TRUE)
		);

	HideColsByIntend(1, 1, TRUE);

	SetScrollBarMode(SB_VERT, gxnAutomatic | gxnEnhanced);			

	// 현재 셀 설정
	SetRightValidCell(1, 0);
	GetParam()->SetHideCurrentCell(GX_HIDE_NEVER);
	GetParam()->SetEnterKeyAction(GX_DOWN);
	EnableInsertRowByPressingKey(FALSE);  // Disable inserting row by key

	// Sorting 정보 초기화
	m_aSortInfo.SetSize(2);
	m_aSortInfo[0].nRC = 1;
	m_aSortInfo[0].sortType = CGXSortInfo::numeric;
	m_aSortInfo[0].sortOrder = CGXSortInfo::ascending;
	m_aSortInfo[1].nRC = 2;
	m_aSortInfo[1].sortType = CGXSortInfo::alphanumeric;
	m_aSortInfo[1].sortOrder = CGXSortInfo::ascending;

	LockUpdate(FALSE);
}

BOOL CBndrMconGrid::InsertRecordByKey(const int &key, const T_MCON_BASE &data)
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

BOOL CBndrMconGrid::DeleteRecordByKey(const int &key, const T_MCON_BASE &data)
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
// CBndrMconGrid Overridable Functions
void CBndrMconGrid::OnChangedSelectionPublic(long *keys, int nSize)
{
	return;
}

long CBndrMconGrid::GetSelectedKeys(CArray<long, long> &caKey)
{
	return 0;
}

BOOL CBndrMconGrid::ValidateField(CString value, int nColID)
{
	
	if (nColID >= GetColCount())
	{
		SetWarningText(_LS(IDS_LBC_ERR_OUT_OF_RANGE));
		return FALSE;
	}

	int nErrCode;

	if (nColID == 0) return TRUE;
	else if( nColID == 1) {
		if (!IsValidLongNumber(value, nErrCode)) return InvalidLongNumber(nErrCode);
	}
	else if(nColID == 2) return TRUE;
	else if( nColID==3 || nColID == 4) {
		if (!IsValidDoubleNumber(value, nErrCode)) return InvalidDoubleNumber(nErrCode);
	}

	return TRUE;
}

// 삽입이나 수정전에 Record의 Validation을 검사한다.
BOOL CBndrMconGrid::ValidateRecord(CStringArray &value, CRowColArray &aCols)
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
BOOL CBndrMconGrid::ValidateAndMakeRecord(CStringArray &value, CRowColArray& aCols, int &key, T_MCON_BASE &data)
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

BOOL CBndrMconGrid::AddToDB(ROWCOL nRow)
{
	// Record를 만들어서 DB에 입력한다.
	int key;
	T_MCON_BASE data;
 
	GetValue(nRow, key, data);

	return AddData(key, data);
}

BOOL CBndrMconGrid::AddToDB(CArray<void*, void*>& raKey, CArray<void*, void*>& raData)
{
	ASSERT(raKey.GetSize() == raData.GetSize());
	int tKey;
	CArray<int, int> aKey;
	CArray<T_MCON_BASE, T_MCON_BASE&> aData;
	for (int i = 0; i < raKey.GetSize(); i++)
	{
		tKey = *((int*)raKey.GetAt(i));
		aKey.Add(tKey);
		aData.Add(*((T_MCON_BASE*)raData.GetAt(i)));
	}
	return AddData(aKey, aData);
}

void CBndrMconGrid::Clear()
{
	m_pData->RemoveAll();
	MakeItemEx();
}

/*BOOL CBndrMconGrid::GetAllDisplayedValue(CArray<double, double>& allData)
{
	allData.RemoveAll();
	int key;
	T_MCON_BASE data;
	for(int i=0; i<m_pData->GetSize(); ++i)
	{
		if(!GetValue(i+1, key, data)) return FALSE;  
		allData.Add(data.dCoeff);
	}

	return TRUE;  
}			*/


BOOL CBndrMconGrid::DeleteFromDB(ROWCOL nRow)
{
	int key;
	T_MCON_BASE data;
 
	GetValue(nRow, key, data);
	return DelData(key);
}

BOOL CBndrMconGrid::DeleteFromDB(CRowColArray& awRows)
{
	if (awRows.GetSize() <= 0) return TRUE;

	ROWCOL nCol;
	VERIFY(GetColumnNoFor(0, nCol));

	CString value;
	CArray<int, int> awKeys;
 
	for (int i = 0; i < awRows.GetSize(); i++)
	{
		value = GetValueRowColBeforeEdit(awRows.GetAt(i), nCol);
		awKeys.Add(_ttoi(value));
	}

	return DelData(awKeys);
}

BOOL CBndrMconGrid::ModifyToDB(ROWCOL nRow)
{
	ROWCOL nCol;
	VERIFY(GetColumnNoFor(0, nCol));

	int key, oldKey;
	T_MCON_BASE data;
	
	oldKey = _ttoi(GetValueRowColBeforeEdit(nRow, nCol));
	GetValue(nRow, key, data);

	DeleteRecord(nRow);
		
	// 수정한다. 실패하면 에러 리턴
	return ModifyData(oldKey, key, data); 
}

BOOL CBndrMconGrid::ModifyToDB(CStringArray& raOldKey, 
												 CArray<void*, void*>& raKey, 
												 CArray<void*, void*>& raData)
{
	ASSERT(raKey.GetSize() == raData.GetSize());
	ASSERT(raKey.GetSize() == raOldKey.GetSize());
	CArray<int, int> aOldKey, aKey;
	CArray<T_MCON_BASE, T_MCON_BASE&> aData;
	int Key;
	int nSize = raKey.GetSize();
	for (int i = 0; i < nSize; i++)
	{
		Key = _ttoi(raOldKey.GetAt(i));
		aOldKey.Add(Key);
		aKey.Add(*((int*)raKey.GetAt(i)));
		aData.Add(*((T_MCON_BASE*)raData.GetAt(i)));
	}
	return ModifyData(aOldKey, aKey, aData);
}


CString CBndrMconGrid::GetNewKey()
{
	CString value(_T(""));
	value.Format(_T("%d"), m_pData->GetSize()+1);
	return value;
}

CString CBndrMconGrid::GetDefValue(int nIndex)
{
	return m_aCurDefVal[nIndex];
}

CString* CBndrMconGrid::GetDefValueArray()
{
	return m_aCurDefVal;
}

BOOL CBndrMconGrid::MakeKeyAndDataArray(CStringArray& value, CRowColArray& awCols, 
																	CArray<void*, void*>& aKey, CArray<void*, void*>& aData)
{
	int* pKey;
	T_MCON_BASE* pData;
	pKey = new int;
	pData = new T_MCON_BASE;
	aKey.Add((int*)pKey);
	aData.Add((T_MCON_BASE*)pData);
	if (!ValidateAndMakeRecord(value, awCols, *pKey, *pData)) return FALSE;
	return TRUE;
}

void CBndrMconGrid::DeleteKeyAndDataArray(CArray<void*, void*>& aKey, CArray<void*, void*>& aData)
{
	int* pKey;
	T_MCON_BASE* pData;
	ASSERT(aKey.GetSize() == aData.GetSize());
	for (int i = 0; i < aKey.GetSize(); i++)
	{
		pKey = (int*)aKey.GetAt(i);
		pData = (T_MCON_BASE*)aData.GetAt(i);
		delete pKey;
		delete pData;
	}
}

long CBndrMconGrid::GetCountData()
{
	return m_pData->GetSize();
}

void CBndrMconGrid::MakeItemFiltered(long nFilteredNum, long* aFilteredKeys, CRowColArray& awCols)
{
	ASSERT(0); // no filtering
}

void CBndrMconGrid::MakeItemNormal(long nNumData, CRowColArray& awCols)
{
	int Key;
	T_MCON_BASE Data;

	long nCount;
	for(nCount=0 ; nCount < nNumData; nCount++)
	{
		Key = nCount+1;
		Data = m_pData->GetAt(nCount);

		SetValue(GetParam()->GetData(), nCount+1, Key, Data, awCols);
	} // end of loop nCount
}

void CBndrMconGrid::MakeItemEditingRecord(ROWCOL ncRow, CStringArray& EditingRecord)
{
	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	int Key;
	T_MCON_BASE Data;
	for (int i = 0; i < GetColCount(); i++)
	{
		if (i == 0) Key = _ttoi(EditingRecord[i]);
		else ConvStrToData(i, EditingRecord[i], Data);
	}
	// 현재 값을 설정한다.
	SetValue(ncRow, Key, Data);
}

/////////////////////////////////////////////////////////////////////////////
// CBndrMconGrid Implementation
BOOL CBndrMconGrid::GetValue(ROWCOL nRow, int &key, T_MCON_BASE &data)
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

		if(i==0) key = _ttoi(str);
		else ConvStrToData(i, str, data);
	}
	return TRUE;
}

BOOL CBndrMconGrid::SetValue(ROWCOL nRow, const int &key, const T_MCON_BASE &data)
{
	//
	CRowColArray awCols;
	awCols.SetSize(GetColCount());
	for (int i = 0;i < GetColCount(); i++)
		VERIFY(GetColumnNoFor(m_aTitle[i], awCols[i]));
	return SetValue(GetParam()->GetData(), nRow, key, data, awCols);
}

BOOL CBndrMconGrid::SetValue(CGXData* pData, ROWCOL nRow, const int &key, const T_MCON_BASE &data, CRowColArray &awCols)
{
	//
	CString str;
	for (int i = 0; i < GetColCount(); i++)
	{
		if(i==0) 
		{
			if (key == 0) str = GetNewKey();
			else str.Format(_T("%d"), key);
		}
		else ConvDataToStr(i, data, str);
		pData->StoreValueRowCol(nRow, awCols[i], str, gxOverride, GX_VT_NUMERIC);//GX_VT_STRING );//GX_VT_NUMERIC);
	}

	return TRUE;
}

UINT CBndrMconGrid::GetDOFKey(CString& sName)
{
	if (sName      == _LS(IDS_TM_MCON_DX)) return 0;
	else if (sName == _LS(IDS_TM_MCON_DY)) return 1;
	else if (sName == _LS(IDS_TM_MCON_DZ)) return 2;
	else if (sName == _LS(IDS_TM_MCON_RX)) return 3;
	else if (sName == _LS(IDS_TM_MCON_RY)) return 4;
	else if (sName == _LS(IDS_TM_MCON_RZ)) return 5;
	else {ASSERT(0); return 0;}
}

CString CBndrMconGrid::GetDOFName(UINT nKey)
{
	if (nKey == 0)      return _LS(IDS_TM_MCON_DX);
	else if (nKey == 1) return _LS(IDS_TM_MCON_DY);
	else if (nKey == 2) return _LS(IDS_TM_MCON_DZ);
	else if (nKey == 3) return _LS(IDS_TM_MCON_RX);
	else if (nKey == 4) return _LS(IDS_TM_MCON_RY);
	else if (nKey == 5) return _LS(IDS_TM_MCON_RZ);
	else {ASSERT(0); return _LS(IDS_TM_MCON_DX);}
}

void CBndrMconGrid::ConvStrToData(int i, CString& value, T_MCON_BASE& data)
{
	if (i == 1) data.NodeK  = _ttoi(value);
	else if (i == 2) data.nDof = GetDOFKey(value);
	else if (i == 3) data.dCoeff = _tstof(value);
	else if (i == 4) data.dWeight = _tstof(value);
	else ASSERT(0);
}

void CBndrMconGrid::ConvDataToStr(int i, const T_MCON_BASE& data, CString& value)
{
	if (i == 1) value.Format(_T("%d"), data.NodeK);
	else if (i == 2) value = GetDOFName(data.nDof);
	else if (i == 3) value.Format(_T("%g"), data.dCoeff);
	else if (i == 4) value.Format(_T("%g"), data.dWeight);
	else ASSERT(0);
}

void CBndrMconGrid::MakeSearchKey(const int &key, const T_MCON_BASE &data, CStringArray &aKey)
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
		else ConvDataToStr(nColID, data, aKey[i]);
	}
}

void CBndrMconGrid::SetHeaderTitle(T_UNIT_SYSTEM &us, BOOL bInit)
{
	CString csUnit, csTitle;
	CRect   rtTable;
	int     nFrontWidth = 35;
	double dTableItemWid;
	double aSizePer[5] = { 0,25,25,35,35};
	
	int aSize[5];// = {0,45,45,60};
	
	GetWindowRect(rtTable);

	dTableItemWid = rtTable.Width() - nFrontWidth - 20; //vert scroll size(15) + margin(5)
	
	for(int i=0; i<5 ; i++)
	{
		aSize[i] = int(dTableItemWid * aSizePer[i] / 100);
	}
	
	int nColCount = GetColCount();

	for(int i=0; i < nColCount; i++)
	{
		if (csUnit.IsEmpty()) csTitle = m_aTitle[i];
		else csTitle = m_aTitle[i];
	
		SetStyleRange(CGXRange(0, i+1), CGXStyle()
			.SetValue(csTitle)
			.SetUserAttribute(ID_USER_ATTR_COL_ID, m_aTitle[i]));

		if (bInit) SetColWidth(i+1, i+1, aSize[i]);
	}
	if (bInit) SetColWidth(0, 0, nFrontWidth);			
}

BOOL CBndrMconGrid::FindData(int nID,UINT nDOF, int& nPos, int nStart)
{
	int i;
	int nSize = m_pData->GetSize();
	
	int nFindNodeID =  nID;
	int nCurrNodeID;
	UINT nCurrDOF;
	
	CString csVal;

	for (i = nStart; i < nSize; i++)
	{
		//T_MCON_BASE item;
		nCurrNodeID = (m_pData->GetAt(i)).NodeK;
		nCurrDOF    = (m_pData->GetAt(i)).nDof;
		
		if( nCurrNodeID == nFindNodeID && nCurrDOF == nDOF)
		{
			nPos = i;
			return TRUE;
		}
	}

	nPos = i;
	
	return FALSE;
}

BOOL CBndrMconGrid::AddData(int key, T_MCON_BASE& data)
{  
	if (key == 0) key = m_pData->GetSize()+1;
	
	CArray<int, int> aOldKey, aKey;
	CArray<T_MCON_BASE, T_MCON_BASE&> aData;  
	
	aKey.Add(key);
	aData.Add(data);

	int nPos;

	if (FindData( data.NodeK, data.nDof, nPos, 0))
	{
		AfxMessageBox(_LS(IDS_MESH_ERR_EXIST_VALUE));
		return FALSE;
	}
	
	m_pData->InsertAt(nPos, data);
	UpdateBuffer(0, aOldKey, aKey, aData);
	
	return TRUE;
}

BOOL CBndrMconGrid::AddData(CArray<T_MCON_BASE, T_MCON_BASE&> &aData, BOOL bNotify)
{
	CArray<int,int> aKey;

	Normalize(aData);

	aKey.SetSize(aData.GetSize());
	
	for(int i=0 ; i<aKey.GetSize() ; i++) 
		aKey[i] = 0;

	return AddData( aKey , aData ,bNotify);
}

BOOL CBndrMconGrid::AddData(CArray<int, int> &aKey, CArray<T_MCON_BASE, T_MCON_BASE&> &aData, BOOL bNotify)
{
	int nSize = aKey.GetSize();
	int KeyIndex = m_pData->GetSize()+1;
	for (int i=0; i<nSize; i++)
	{
		if (aKey[i] != 0) continue;
		aKey[i] = KeyIndex;
		KeyIndex++;
	}
	CArray<int, int> aOldKey;
	aOldKey.Copy(aKey);

	int nPos;
	for(int i=0; i < nSize; i++)
	{
		if (FindData( aData[i].NodeK, aData[i].nDof, nPos, 0))
		{
			AfxMessageBox(_LS(IDS_MESH_ERR_EXIST_VALUE));
			return FALSE;
		}

		int pos = aKey[i]-1;
		T_MCON_BASE item = aData[i];
		m_pData->InsertAt(pos,item);
		
	}

	UpdateBuffer(0, aOldKey, aKey, aData);

	return TRUE;
}

void CBndrMconGrid::BubbleSort(CArray<int, int>& Param ) // 오름 차순 
{
	int temp ;
	int element = Param.GetSize();

	for (int  i = 0 ; i < element-1 ; i++ )
	{  
		for (int j = i+1 ; j < element ; j ++ )
		{
			if ( Param[i] > Param[j] )
			{
				temp = Param[i] ;
				Param[i] = Param[j] ;
				Param[j] = temp ;
			}
		}
	}
}


BOOL CBndrMconGrid::DelData(int key)
{
	if (key <= 0 || key > m_pData->GetSize())
	{ ASSERT(0); return FALSE; }

	CArray<int, int> aOldKey, aKey;
	CArray<T_MCON_BASE, T_MCON_BASE&> aData;
	aKey.Add(key);
	aOldKey.Add(key);
	T_MCON_BASE data;
	data.Initialize();
	aData.Add(data);

	m_pData->RemoveAt(key-1);
	UpdateBuffer(1, aOldKey, aKey, aData);
	return TRUE;
}

BOOL CBndrMconGrid::DelData(CArray<int, int> &aKey, BOOL bNotify)
{
	CArray<int, int> aOldKey;
	CArray<T_MCON_BASE, T_MCON_BASE&> aData;
	aOldKey.Copy(aKey);
	T_MCON_BASE data; 
	data.Initialize();
	int nSize = aKey.GetSize();

	BubbleSort(aKey);  // Ascending keys  by lcj

	for (int i = 0; i < nSize; i++)
	{
		if (aKey[i] <= 0 || aKey[i] > m_pData->GetSize())
		{ ASSERT(0); return FALSE; }
		aData.Add(data);
	}
	
	for (int i = nSize-1; i >= 0; i--)
		m_pData->RemoveAt(aKey[i]-1);
	UpdateBuffer(1, aOldKey, aKey, aData);

	return TRUE;
}

BOOL CBndrMconGrid::ModifyData(int oldKey, int key, T_MCON_BASE &data)
{
	if (key <= 0 || key > m_pData->GetSize())
	{ ASSERT(0); return FALSE; }

	CArray<int, int> aOldKey, aKey;
	CArray<T_MCON_BASE, T_MCON_BASE&> aData;
	aOldKey.Add(oldKey);
	aKey.Add(key);
	aData.Add(data);

	int nPos;
	if (FindData( data.NodeK, data.nDof, nPos, 0))
	{
		if( key-1 != nPos)
		{  
			AfxMessageBox(_LS(IDS_MESH_ERR_EXIST_VALUE));
			return FALSE;
		}
	}  
 
	ASSERT(oldKey == key);
	m_pData->SetAt(key-1, data);
	UpdateBuffer(2, aOldKey, aKey, aData);

	return TRUE;
}

BOOL CBndrMconGrid::ModifyData(CArray<int, int> &aOldKey, CArray<int, int> &aKey, CArray<T_MCON_BASE, T_MCON_BASE&> &aData)
{
	CArray<T_MCON_BASE, T_MCON_BASE&> aOrgData;
	aOrgData.Copy(*m_pData);

	if (!DelData(aOldKey, FALSE))
		return FALSE;
	if (!AddData(aKey, aData, FALSE))
	{
		m_pData->Copy(aOrgData);
		return FALSE;
	}
	
	return TRUE;
}

void CBndrMconGrid::UpdateBuffer(int nCmd, CArray<int, int> &aOldKey, CArray<int, int> &aKey, CArray<T_MCON_BASE, T_MCON_BASE&> &aData)
{
	MakeItemEx();
}

void CBndrMconGrid::ConvertCoeffUnit()
{
	UINT nUnitType = 0; 
	CString  strDof = _T("");
	double   dCoeff = 0.;
	UINT     nDof = 0;
	int      nCheck = 0;

	m_nMasterDOF = m_pParent->GetMasterDof();

	if(m_nMasterDOF < 0) return; // WD 타입일때 -1 리턴

	for(int i=0; i<m_pData->GetSize(); ++i)
	{
		dCoeff = m_pData->GetAt(i).dCoeff;
		nDof = m_pData->GetAt(i).nDof;

		if(m_nMasterDOF<3 && nDof<3) nUnitType = D_UNITSYS_NONE;
		else if(m_nMasterDOF>=3 && nDof>=3) nUnitType = D_UNITSYS_NONE;
		else if(m_nMasterDOF <3 && nDof>=3) nUnitType = D_UNITSYS_BASE_LENGTH;
		else if(m_nMasterDOF>=3 && nDof <3) nUnitType = D_UNITSYS_BASE_PERLENGTH;

		CDBDoc* pDoc = CDBDoc::GetDocPoint();       ASSERT(pDoc);
		if (!pDoc) return;
		CUnitCtrl* pUnitCtrl = pDoc->m_pUnitCtrl; ASSERT(pUnitCtrl);
		if (!pUnitCtrl) return;
	
		dCoeff =  pUnitCtrl->ConvertUnitDataPrevious(nUnitType, dCoeff);
		(*m_pData)[i].dCoeff = dCoeff;
	}

	MakeItemEx();
}

void CBndrMconGrid::SetMasterDOF(int nDOF)
{
	ASSERT(nDOF>=0 && nDOF<6);
	m_nMasterDOF = nDOF;
}

// MNET:XXXX 20080117 mylee - Unit 변경시 회전 자유도를 길이처럼 변경시켜 주기 위해
void CBndrMconGrid::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
{
	HWND hWnd;
	if ((hWnd = GetSafeHwnd()) == 0 || !IsWindow(hWnd)) return;

	switch(lHint)
	{
//   case D_UPDATE_DBALL:
//   case D_UPDATE_DEFAULT:
//    	InitTsgrData();
//     break;
//   case D_UPDATE_BUFFER_BEFORE:
//     break;
//   case D_UPDATE_BUFFER_AFTER:
//		 InitTsgrData();
//     break; 
	case D_UPDATE_UNIT:
		//if(m_nMasterDOF>=0 && m_nMasterDOF<6)
		ConvertCoeffUnit();
		break;
//   case D_UPDATE_SEL_ADD: case D_UPDATE_SEL_DEL:
//     break;
//   case D_UPDATE_EXE_START: case D_UPDATE_EXE_END:
//     break;
	default:
		//ASSERT(FALSE);
		break;
	}
}

void CBndrMconGrid::EnableTable(BOOL bEnable)
{  
	//SetStyleRange(CGXRange().SetCols(1,4) , CGXStyle().SetEnabled(bEnable));
	if(bEnable==FALSE)
		Clear();
}

void CBndrMconGrid::SetType(int nType)
{
	BOOL bOldLock = LockUpdate(TRUE);

	BOOL bExplicit = (nType==0);
	HideCols(3, 4, !bExplicit);
	HideCols(5, 5,  bExplicit);

	LockUpdate(bOldLock);
	if(!bOldLock) Redraw();
}

void CBndrMconGrid::Normalize(CArray<T_MCON_BASE, T_MCON_BASE&> &aData)
{
	int nSize = aData.GetSize();
	int nDataSize = m_pData->GetSize();
	CArray<int, int> aRemoveIndex;  aRemoveIndex.RemoveAll(); 

	for (int i=0;i<nSize; ++i)
	{
		for (int j=0; j<nDataSize; ++j)
		{
			if (m_pData->GetAt(j).NodeK == aData[i].NodeK && m_pData->GetAt(j).nDof == aData[i].nDof)
			{
				aRemoveIndex.Add(j);
			}
		} 
	}

	nSize = aRemoveIndex.GetSize();
	for (int i=nSize; i>0; --i)
	{
		m_pData->RemoveAt(aRemoveIndex[i-1]);
	}
}