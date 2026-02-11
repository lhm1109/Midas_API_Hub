// SpfcItemGrid.cpp : implementation file
//
// 2003. 10. 06   by TAE
// Starting Day / Story Key / Story Name
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMStageStdgSubGrid.h"

#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_base\wg_base_CompFunc.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define EPSILON 1.e-12
#define DATA_FMT _T("%.15g")
#define D_STORY_NONE  _LS(IDS_WG_CMD__ADDD__None)

/////////////////////////////////////////////////////////////////////////////
// CCMStageStdgSubGrid
CString CCMStageStdgSubGrid::m_aCurDefVal[] = {_T("0.0"), _T("")};
CCMStageStdgSubGrid::CCMStageStdgSubGrid()
{
	m_pData = 0;

	AddColInfo(_LS(IDS_CMD_ESQW_Starting_Day),  t_real,    0);
	AddColInfo(_LS(IDS_CMD_ESQW_Story),         t_string , 0);

	m_aPrimaryKey.Add(0);

	CString aCurDefVal[] = {_T("0.0"), D_STORY_NONE};
	for(int i=0; i<sizeof(aCurDefVal)/sizeof(CString); i++)
		m_aCurDefVal[i] = aCurDefVal[i];
}

CCMStageStdgSubGrid::~CCMStageStdgSubGrid()
{
}


BEGIN_MESSAGE_MAP(CCMStageStdgSubGrid, CTBCommon)
	//{{AFX_MSG_MAP(CCMStageStdgSubGrid)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CCMStageStdgSubGrid message handlers

void CCMStageStdgSubGrid::Initialize(T_ESQW_ADD_LOAD *pData)
{
	ASSERT(pData);
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
			.SetAllowEnter(FALSE));
	ChangeColHeaderStyle(CGXStyle().SetFont(CGXFont_GC().SetBold(FALSE)));
	ChangeRowHeaderStyle(CGXStyle()
					.SetFont(CGXFont_GC().SetBold(FALSE))
					.SetHorizontalAlignment(DT_RIGHT)
					.SetControl(GX_IDS_CTRL_HEADER));
	//GetParam()->SetNumberedRowHeaders(FLASE);

	// 컬럼 스타일 변경
	SetStyleRange(CGXRange().SetCols(1), CGXStyle()
		.SetValueType(GX_VT_NUMERIC)
		.SetFormat(GX_FMT_FIXED)
		.SetPlaces(0));

	CString csChoiceList(_T(""));
	T_STOR_D StorD;
	CArray<T_STOR_K, T_STOR_K> aStorK;
	pDoc->m_pAttrCtrl->GetStorKeyList(aStorK);
	for (int i=0; i<aStorK.GetSize(); i++)
	{
		pDoc->m_pAttrCtrl->GetStor(aStorK[i], StorD);
		csChoiceList += StorD.StoryName;
		csChoiceList += _T("\n");
	}  
	csChoiceList += D_STORY_NONE; csChoiceList += _T("\n");
	SetStyleRange(CGXRange().SetCols(2), CGXStyle()
		.SetControl(GX_IDS_CTRL_TABBED_COMBOBOX)
		.SetChoiceList(csChoiceList)
		.SetUserAttribute(GX_IDS_UA_TABLIST_KEYCOL, _T("0"))
		.SetUserAttribute(GX_IDS_UA_TABLIST_TEXTCOL, _T("0"))
		.SetUserAttribute(GX_IDS_UA_TABLIST_SHOWALLCOLS, _T("1"))
		.SetValueType(GX_VT_STRING)
		.SetHorizontalAlignment(DT_LEFT)
		.SetValue(_T(""))
		);

	// 현재 셀 설정
	SetRightValidCell(1, 0);
	GetParam()->SetHideCurrentCell(GX_HIDE_NEVER);

	// always show vertical scrollbar
	SetScrollBarMode(SB_VERT, gxnEnabled | gxnEnhanced);  

	// Sorting 정보 초기화
	m_aSortInfo.SetSize(1);
	m_aSortInfo[0].nRC = 0;
	m_aSortInfo[0].sortType = CGXSortInfo::numeric;
	m_aSortInfo[0].sortOrder = CGXSortInfo::ascending;

	MakeItemEx();
	LockUpdate(FALSE);
}

BOOL CCMStageStdgSubGrid::InsertRecordByKey(const double &key, const long &data)
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

BOOL CCMStageStdgSubGrid::DeleteRecordByKey(const double &key, const long &data)
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
// CCMStageStdgSubGrid Overridable Functions
void CCMStageStdgSubGrid::OnChangedSelectionPublic(long *keys, int nSize)
{
	return;
}

long CCMStageStdgSubGrid::GetSelectedKeys(CArray<long, long> &caKey)
{
	return 0;
}

BOOL CCMStageStdgSubGrid::ValidateField(CString value, int nColID)
{
	if (nColID >= GetColCount())
	{
		SetWarningText(_LS(IDS_WG_CMD__ADDD___CSpfcItemGrid__ValidateField__Er));
		return FALSE;
	}

	int nErrCode;
	if (nColID == 0)
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
	else if (nColID == 1) return TRUE;
	else return FALSE;

	return TRUE;
}

// 삽입이나 수정전에 Record의 Validation을 검사한다.
BOOL CCMStageStdgSubGrid::ValidateRecord(CStringArray &value, CRowColArray &aCols)
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
BOOL CCMStageStdgSubGrid::ValidateAndMakeRecord(CStringArray &value, CRowColArray &aCols, double &key, long &data)
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

BOOL CCMStageStdgSubGrid::AddToDB(ROWCOL nRow)
{
	// Record를 만들어서 DB에 입력한다.
	double key;
	long data;
 
	GetValue(nRow, key, data);

	return AddData(key, data);
}

BOOL CCMStageStdgSubGrid::AddToDB(CArray<void*, void*>& raKey, CArray<void*, void*>& raData)
{
	ASSERT(raKey.GetSize() == raData.GetSize());
	double tKey;
	CArray<double, double> aKey;
	CArray<long, long> aData;
	for (int i = 0; i < raKey.GetSize(); i++)
	{
		tKey = *((double*)raKey.GetAt(i));
		aKey.Add(tKey);
		aData.Add(*((long*)raData.GetAt(i)));
	}
	return AddData(aKey, aData);
}


BOOL CCMStageStdgSubGrid::DeleteFromDB(ROWCOL nRow)
{
	double key;
	long data;
 
	GetValue(nRow, key, data);
	return DelData(key);
}

BOOL CCMStageStdgSubGrid::DeleteFromDB(CRowColArray& awRows)
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

BOOL CCMStageStdgSubGrid::ModifyToDB(ROWCOL nRow)
{
	ROWCOL nCol;
	VERIFY(GetColumnNoFor(0, nCol));

	double key, oldKey;
	long data;
	
	oldKey = _tstof(GetValueRowColBeforeEdit(nRow, nCol));
	GetValue(nRow, key, data);

	// 수정한다. 실패하면 에러 리턴
	return ModifyData(oldKey, key, data); 
}

BOOL CCMStageStdgSubGrid::ModifyToDB(CStringArray& raOldKey, 
												 CArray<void*, void*>& raKey, 
												 CArray<void*, void*>& raData)
{
	ASSERT(raKey.GetSize() == raData.GetSize());
	ASSERT(raKey.GetSize() == raOldKey.GetSize());
	CArray<double, double> aOldKey, aKey;
	CArray<long, long> aData;
	double Key;
	int nSize = raKey.GetSize();
	for (int i = 0; i < nSize; i++)
	{
		Key = _tstof(raOldKey.GetAt(i));
		aOldKey.Add(Key);
		aKey.Add(*((double*)raKey.GetAt(i)));
		aData.Add(*((long*)raData.GetAt(i)));
	}
	return ModifyData(aOldKey, aKey, aData);
}

CString CCMStageStdgSubGrid::GetNewKey()
{
	CString value(_T("0"));
	return value;
}

CString CCMStageStdgSubGrid::GetDefValue(int nIndex)
{
	return m_aCurDefVal[nIndex];
}

CString* CCMStageStdgSubGrid::GetDefValueArray()
{
	return m_aCurDefVal;
}

BOOL CCMStageStdgSubGrid::MakeKeyAndDataArray(CStringArray& value, CRowColArray& awCols, 
																	CArray<void*, void*>& aKey, CArray<void*, void*>& aData)
{
	double* pKey;
	long* pData;
	pKey = new double;
	pData = new long;
	aKey.Add((void*)pKey);
	aData.Add((void*)pData);
	if (!ValidateAndMakeRecord(value, awCols, *pKey, *pData)) return FALSE;
	return TRUE;
}

void CCMStageStdgSubGrid::DeleteKeyAndDataArray(CArray<void*, void*>& aKey, CArray<void*, void*>& aData)
{
	double* pKey;
	long* pData;
	ASSERT(aKey.GetSize() == aData.GetSize());
	for (int i = 0; i < aKey.GetSize(); i++)
	{
		pKey = (double*)aKey.GetAt(i);
		pData = (long*)aData.GetAt(i);
		delete pKey;
		delete pData;
	}
}

long CCMStageStdgSubGrid::GetCountData()
{
	return m_pData->arStory.GetSize();
}

void CCMStageStdgSubGrid::MakeItemFiltered(long nFilteredNum, long* aFilteredKeys, CRowColArray& awCols)
{
	ASSERT(0); // no filtering
}

void CCMStageStdgSubGrid::MakeItemNormal(long nNumData, CRowColArray& awCols)
{
	double Key;
	long Data;

	long nCount;
	for(nCount=0 ; nCount < nNumData; nCount++)
	{
		Key = m_pData->arStory[nCount].dStartingDay;
		Data = m_pData->arStory[nCount].StoryKey;

		SetValue(GetParam()->GetData(), nCount+1, Key, Data, awCols);
	} // end of loop nCount
}

void CCMStageStdgSubGrid::MakeItemEditingRecord(ROWCOL ncRow, CStringArray& EditingRecord)
{
	CDBDoc* pDoc = m_pDoc;
	double Key;
	long Data;
	for (int i = 0; i < GetColCount(); i++)
	{
		if (i == 0) Key = _tstof(EditingRecord[i]);
		else ConvStrToData(i, EditingRecord[i], Data);
	}
	// 현재 값을 설정한다.
	SetValue(ncRow, Key, Data);
}

/////////////////////////////////////////////////////////////////////////////
// CCMStageStdgSubGrid Implementation
BOOL CCMStageStdgSubGrid::GetValue(ROWCOL nRow, double &key, long &data)
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

		if(i==0) key = _tstof(str);
		else ConvStrToData(i, str, data);
	}
	return TRUE;
}

BOOL CCMStageStdgSubGrid::SetValue(ROWCOL nRow, const double &key, const long &data)
{
	//
	CRowColArray awCols;
	awCols.SetSize(GetColCount());
	for (int i = 0;i < GetColCount(); i++)
		VERIFY(GetColumnNoFor(m_aTitle[i], awCols[i]));
	return SetValue(GetParam()->GetData(), nRow, key, data, awCols);
}

BOOL CCMStageStdgSubGrid::SetValue(CGXData* pData, ROWCOL nRow, const double &key, const long &data, CRowColArray &awCols)
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

T_STOR_K CCMStageStdgSubGrid::GetStorData(CString& value)
{
	if (value == D_STORY_NONE) return 0;
	return m_pDoc->m_pAttrCtrl->GetStorKey(value);
}

CString CCMStageStdgSubGrid::GetStorStr(T_STOR_K data)
{
	if (data == 0) return D_STORY_NONE;
	T_STOR_D StorD;
	m_pDoc->m_pAttrCtrl->GetStor(data, StorD);
	return StorD.StoryName;
}

void CCMStageStdgSubGrid::ConvStrToData(int i, CString& value, long& data)
{ 
	if (i==1) data = GetStorData(value);
}

void CCMStageStdgSubGrid::ConvDataToStr(int i, const long& data, CString& value)
{
	if (i==1) value = GetStorStr(data);
}

void CCMStageStdgSubGrid::MakeSearchKey(const double &key, const long &data, CStringArray &aKey)
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

void CCMStageStdgSubGrid::SetHeaderTitle(T_UNIT_SYSTEM &us, BOOL bInit)
{
	int aSize[] = {77, 77};
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

BOOL CCMStageStdgSubGrid::FindData(double key, int& nPos, int nStart)
{
	int i;
	int nSize = m_pData->arStory.GetSize();
	
	CString csVal;
	double dblVal;
	int nCompResult;
	for (i = nStart; i < nSize; i++)
	{
		csVal.Format(DATA_FMT, m_pData->arStory[i].dStartingDay);
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

BOOL CCMStageStdgSubGrid::ExistAllKey(CArray<double, double>& aKey, double& errorKey)
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

BOOL CCMStageStdgSubGrid::AddData(double key, long data)
{
	CArray<double, double> aOldKey, aKey;
	CArray<long, long> aData;
	aKey.Add(key);
	aData.Add(data);
	T_ESQW_ADD_STOR_BASE item;
	item.dStartingDay = key;
	item.StoryKey = data;

	// 동일한 key값 중복 검토
	int nPos;
	if (FindData(key, nPos, 0))
	{
		CString msg;
		msg.Format(_LS(IDS_CMD_Starting_Day_already_exist), key);
		AfxMessageBox(msg);
		return FALSE;
	}
	// check story level
	double oldkey = 0.;
	if (!CheckStoryLevel(oldkey, key, data, FALSE)) return FALSE;

	m_pData->arStory.InsertAt(nPos, item);
	UpdateBuffer(0, aOldKey, aKey, aData);
	return TRUE;
}

BOOL CCMStageStdgSubGrid::AddData(CArray<double, double> &aKey, CArray<long, long> &aData, BOOL bNotify)
{
	CArray<double, double> aOldKey;

	// 동일한 key값 중복 검토
	int nSize = aKey.GetSize();
	aOldKey.Copy(aKey);
	qsort((void*)aOldKey.GetData(), nSize, sizeof(double), CCompFunc::doubleAsc);

	BOOL bSame;
	double oldkey = 0;
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
			msg.Format(_LS(IDS_CMD_Starting_Day_already_exist), aOldKey[i]);
			AfxMessageBox(msg);
			return FALSE;
		}
		if (!CheckStoryLevel(oldkey, aKey[i], aData[i], FALSE)) return FALSE;
		nPos = nStart;
	}

	for (int i = 0; i < nSize; i++)
	{
		bSame = FindData(aKey[i], nPos, 0);
		VERIFY(!bSame);
		T_ESQW_ADD_STOR_BASE item;
		item.dStartingDay = aKey[i];
		item.StoryKey = aData[i];
		m_pData->arStory.InsertAt(nPos, item);
	}
	UpdateBuffer(0, aOldKey, aKey, aData);

	return TRUE;
}

BOOL CCMStageStdgSubGrid::DelData(double key)
{
	CArray<double, double> aOldKey, aKey;
	CArray<long, long> aData;
	aKey.Add(key);
	aData.Add(0);

	int nPos;
	if (!FindData(key, nPos))
	{
		CString msg;
		msg.Format(_LS(IDS_CMD_Starting_Day_not_exist), key);
		AfxMessageBox(msg);
		return FALSE;
	}
	m_pData->arStory.RemoveAt(nPos);
	UpdateBuffer(1, aOldKey, aKey, aData);
	return TRUE;
}

BOOL CCMStageStdgSubGrid::DelData(CArray<double, double> &aKey, BOOL bNotify)
{
	CArray<double, double> aOldKey;
	CArray<long, long> aData;
	int nSize = aKey.GetSize();
	int i = 0;
	for (i = 0; i < nSize; i++) aData.Add(0);

	aOldKey.Copy(aKey);
	qsort((void*)aOldKey.GetData(), nSize, sizeof(double), CCompFunc::doubleAsc);
	// 동일한 key값 중복 검토
	double errKey;
	if (!ExistAllKey(aOldKey, errKey))
	{
		CString msg;
		msg.Format(_LS(IDS_CMD_Starting_Day_not_exist), errKey);
		AfxMessageBox(msg);
		return FALSE;
	}
	int nSize2 = m_pData->arStory.GetSize();
	int j = nSize2-1;
	double dblDiff;
	for (i = nSize-1; i >= 0; i--)
	{
		for (; j >= 0; j--)
		{
			dblDiff = aOldKey[i] - m_pData->arStory[j].dStartingDay;
			if (dblDiff >= -EPSILON && dblDiff <= EPSILON) 
			{
				m_pData->arStory.RemoveAt(j);
				j--;
				break;
			}
		}
	}

	UpdateBuffer(1, aOldKey, aKey, aData);

	return TRUE;
}

BOOL CCMStageStdgSubGrid::CheckStoryLevel(double oldkey, double key, long data, BOOL bModify)
{
	T_STOR_K StorK;
	T_STOR_D StorD;
	double dCurLevel;
	int nPos, nPre, nPost;
	BOOL bStoryLevel = TRUE;

	BOOL bExist = FindData(key, nPos);
	if (!m_pDoc->m_pAttrCtrl->GetStor(data, StorD))
	{
		// None이거나 현 스텝이 존재하는 경우 
		if (data == -1)
		{
			AfxMessageBox(_LS(IDS_CMD_Story_is_not_selected_incorrectly));
			return FALSE;
		}
		else return TRUE;
	}
	dCurLevel = StorD.dStoryLevel; 

	if (bModify)  // 수정모드 일때 
	{
		if (key > oldkey) { nPre = nPos-2; nPost = nPos; }
		else { nPre = nPos-1; nPost = nPos+1; }
	}
	else { nPre = nPos-1; nPost = nPos; }  // 추가모드 일때 

	if (nPre >= 0) 
	{
		StorK = m_pData->arStory[nPre].StoryKey;
		m_pDoc->m_pAttrCtrl->GetStor(StorK, StorD);
		if (dCurLevel - StorD.dStoryLevel <= 0.0) bStoryLevel = FALSE;
	}
	if (nPost < m_pData->arStory.GetSize())
	{
		StorK = m_pData->arStory[nPost].StoryKey;
		m_pDoc->m_pAttrCtrl->GetStor(StorK, StorD);
		if (dCurLevel - StorD.dStoryLevel >= 0.0) bStoryLevel = FALSE;
	}
	if (!bStoryLevel)
	{
		AfxMessageBox(_LS(IDS_CMD_Story_is_not_selected_incorrectly));
		return FALSE;
	}
	return TRUE;
}

BOOL CCMStageStdgSubGrid::ModifyData(double oldKey, double key, long data)
{
	CArray<double, double> aOldKey, aKey;
	CArray<long, long> aData;
	aOldKey.Add(oldKey);
	aKey.Add(key);
	aData.Add(data);

	int nPos;
	if (!FindData(oldKey, nPos))
	{
		CString msg;
		msg.Format(_LS(IDS_CMD_Starting_Day_not_exist), oldKey);
		AfxMessageBox(msg);
		return FALSE;
	}

	//double oldkey = 0.;
	if (!CheckStoryLevel(oldKey, key, data, TRUE)) return FALSE;

	if (oldKey == key)
	{
		m_pData->arStory[nPos].dStartingDay = key;
		m_pData->arStory[nPos].StoryKey = data;
	}
	else
	{
		int nPos2;
		if (FindData(key, nPos2))
		{
			CString msg;
			msg.Format(_LS(IDS_CMD_Starting_Day_already_exist), key);
			AfxMessageBox(msg);
			return FALSE;
		}
		T_ESQW_ADD_STOR_BASE item;
		item.dStartingDay = key;
		item.StoryKey = data;
		m_pData->arStory.RemoveAt(nPos);
		if (nPos2 > nPos) nPos2--;
		m_pData->arStory.InsertAt(nPos2, item);
	}

	UpdateBuffer(2, aOldKey, aKey, aData);

	return TRUE;
}

BOOL CCMStageStdgSubGrid::ModifyData(CArray<double, double> &aOldKey, CArray<double, double> &aKey, CArray<long, long> &aData)
{
	CArray<T_ESQW_ADD_STOR_BASE, T_ESQW_ADD_STOR_BASE&> aOrgData;
	aOrgData.Copy(m_pData->arStory);
	
	// 스토리 레벨 검토
	for (int i=0; i<aData.GetSize(); i++)
	{
		if (!CheckStoryLevel(aOldKey[i], aKey[i], aData[i], TRUE)) return FALSE;
	}

	if (!DelData(aOldKey, FALSE))
		return FALSE;
	if (!AddData(aKey, aData, FALSE))
	{
		m_pData->arStory.Copy(aOrgData);
		return FALSE;
	}
	return TRUE;
}

void CCMStageStdgSubGrid::UpdateBuffer(int nCmd, CArray<double, double> &aOldKey, CArray<double, double> &aKey, CArray<long, long> &aData)
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
