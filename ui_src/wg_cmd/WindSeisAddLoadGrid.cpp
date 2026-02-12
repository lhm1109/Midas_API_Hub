// WindSeisAddLoadGrid.cpp: implementation of the CWindSeisAddLoadGrid class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "wg_cmd.h"
#include "WindSeisAddLoadGrid.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_DBLib.h"
#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_common\wg_common_TBGrid.h"
#include "..\wg_base\wg_base_CompFunc.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#define D_MAX_COL 7
enum ColumnIndex
{ COLINDEX_KEY		=  0,
	COLINDEX_STORY	= 1, 
	COLINDEX_ADD_X	= 2, COLINDEX_ADD_Y = 3, COLINDEX_ADD_X2 = 4,
	COLINDEX_ADD_Y2	= 5, COLINDEX_ADD_R = 6, COLINDEX_ADD_R2 = 7,
};

CString CWindSeisAddLoadGrid::m_aCurDefVal[] = {_T(""), _T("0.0"), _T("0.0"), _T("0.0"), _T("0.0"), _T("0.0"), _T("0.0")};
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CWindSeisAddLoadGrid::CWindSeisAddLoadGrid()
{
	m_nWndWidth = 0;

	m_pDoc = CDBDoc::GetDocPoint();
	m_pUnitCtrl = m_pDoc->m_pUnitCtrl;

	AddColInfo(_LS(IDS_WG_CMD__ADDD__Key),		t_integer, 0);
	AddColInfo(_LS(IDS_WG_CMD__ADDD__Story),  t_string, 0);
	AddColInfo(_LS(IDS_WG_CMD__ADD2__Add__X), t_real, CUnitCtrl::m_SEIS_UNIT.AddX);
	AddColInfo(_LS(IDS_WG_CMD__ADD2__Add__Y), t_real, CUnitCtrl::m_SEIS_UNIT.AddY);
	AddColInfo(_LS(IDS_WG_CMD__ADD2__Across_Add__X), t_real, CUnitCtrl::m_SEIS_UNIT.AddX);
	AddColInfo(_LS(IDS_WG_CMD__ADD2__Across_Add__Y), t_real, CUnitCtrl::m_SEIS_UNIT.AddY);
	AddColInfo(_LS(IDS_WG_CMD__ADD2__Add__R), t_real, CUnitCtrl::m_SEIS_UNIT.AddR);
	AddColInfo(_LS(IDS_WG_CMD__ADD2__Torsional_Add__RZY), t_real, CUnitCtrl::m_SEIS_UNIT.AddR);

	m_aPrimaryKey.Add(0);

	m_bWindKBC2015 = FALSE;
	m_bShowRz = TRUE;
}

CWindSeisAddLoadGrid::~CWindSeisAddLoadGrid()
{

}

//////////////////////////////////////////////////////////////////////
// Interface
void CWindSeisAddLoadGrid::Initialize(CWnd* pParent, BOOL bWind, int nWidth, BOOL bWindKBC2015/*=FALSE*/, BOOL bShowRz/*=TRUE*/)
{
	m_pParent = pParent;
	m_bWind = bWind;
	m_nWndWidth = nWidth;
	m_bWindKBC2015 = bWindKBC2015;
	m_bShowRz = bShowRz;

	SetTitle();

	T_UNIT_SYSTEM us;
	CTBCommon::Initialize(m_pDoc, us);

	if(m_bWindKBC2015)
	{
		SetRowHeightByDPI(0, 0, 36);
	}
	else
	{
		SetRowHeightByDPI(0, 0, 20);
	}
	
	GetParam()->EnableUndo(FALSE);
	GetParam()->EnableTrackRowHeight(FALSE); 
	GetParam()->EnableMoveCols(FALSE);        // columns can not be dragged

	CGXProperties* pProp = GetParam()->GetProperties();
	pProp->SetMarkColHeader(FALSE); // Turn off pressed button effect for column headers
	pProp->SetMarkRowHeader(FALSE);
	pProp->SetUserProperty(GX_IDS_OUTLINECURRENTCELL, // 현재 셀의 border 설정 변경
												(CGXStyle) pProp->sInvertNoBorder);

	LockUpdate(TRUE);

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
	SetStorColumn(2);

	SetStyleRange(CGXRange().SetCols(3, 8), CGXStyle()
		.SetHorizontalAlignment(DT_RIGHT)
		.SetValueType(GX_VT_NUMERIC)
		.SetFormat(GX_FMT_FIXED)
		.SetPlaces(4));

	HideColsByIntend(5,6,m_bWindKBC2015==FALSE);
	HideColsByIntend(8,8,m_bWindKBC2015==FALSE);
	HideColsByIntend(7,7,m_bWindKBC2015==FALSE && bShowRz==FALSE);

	HideColsByIntend(1, 1, TRUE);
	
	SetScrollBarMode(SB_HORZ, m_bWindKBC2015? gxnEnabled : gxnDisabled);
	SetScrollBarMode(SB_VERT, gxnEnabled);
	
	// Sorting 정보 초기화
	m_aSortInfo.SetSize(1);
	m_aSortInfo[0].nRC = 0;
	m_aSortInfo[0].sortType = CGXSortInfo::numeric;
	m_aSortInfo[0].sortOrder = CGXSortInfo::ascending;

	LockUpdate(FALSE);
	Redraw();

	LockUpdate(FALSE);
}

void CWindSeisAddLoadGrid::SetMode(BOOL bWindKBC2015, BOOL bAcross, BOOL bTorsinal)
{
	m_bWindKBC2015 = bWindKBC2015;
	HideColsByIntend(5,6,m_bWindKBC2015==FALSE);
	HideColsByIntend(8,8,m_bWindKBC2015==FALSE);

	SetTitle();
	if(m_bWindKBC2015)
	{
		SetRowHeightByDPI(0, 0, 36);

		SetStyleRange(CGXRange().SetCols(5,6), CGXStyle().SetEnabled(bAcross));
		SetStyleRange(CGXRange().SetCols(7,8), CGXStyle().SetEnabled(bTorsinal));
	}
	else
	{
		SetRowHeightByDPI(0, 0, 20);

		SetStyleRange(CGXRange().SetCols(5,6), CGXStyle().SetEnabled(TRUE));
		SetStyleRange(CGXRange().SetCols(7,8), CGXStyle().SetEnabled(TRUE));
	}

	SetScrollBarMode(SB_HORZ, m_bWindKBC2015? gxnEnabled : gxnDisabled);
	ScrollCellInView(0, COLINDEX_STORY);
	
	T_UNIT_SYSTEM us;
	SetHeaderTitle(us,TRUE);
}

void CWindSeisAddLoadGrid::SetTitle()
{
	m_aTitle.RemoveAll();
	if(m_bWindKBC2015)
	{
		m_aTitle.Add(_LS(IDS_WG_CMD__ADDD__Key));
		m_aTitle.Add(_LS(IDS_WG_CMD__ADDD__Story));
		m_aTitle.Add(_LS(IDS_WG_CMD__ADD2__Along_Add__X));
		m_aTitle.Add(_LS(IDS_WG_CMD__ADD2__Along_Add__Y));
		m_aTitle.Add(_LS(IDS_WG_CMD__ADD2__Across_Add__X));
		m_aTitle.Add(_LS(IDS_WG_CMD__ADD2__Across_Add__Y));
		m_aTitle.Add(_LS(IDS_WG_CMD__ADD2__Torsional_Add__RZX));
		m_aTitle.Add(_LS(IDS_WG_CMD__ADD2__Torsional_Add__RZY));
	}
	else
	{
		m_aTitle.Add(_LS(IDS_WG_CMD__ADDD__Key));
		m_aTitle.Add(_LS(IDS_WG_CMD__ADDD__Story));
		m_aTitle.Add(_LS(IDS_WG_CMD__ADD2__Add__X));
		m_aTitle.Add(_LS(IDS_WG_CMD__ADD2__Add__Y));
		m_aTitle.Add(_LS(IDS_WG_CMD__ADD2__Across_Add__X));
		m_aTitle.Add(_LS(IDS_WG_CMD__ADD2__Across_Add__Y));
		m_aTitle.Add(_LS(IDS_WG_CMD__ADD2__Add__R));
		m_aTitle.Add(_LS(IDS_WG_CMD__ADD2__Torsional_Add__RZY));
	}
}

void CWindSeisAddLoadGrid::SetHeaderTitle(T_UNIT_SYSTEM &UnitSystem, BOOL bInit)
{
	if(bInit == FALSE)    return;

	double dRatio = (double)m_nWndWidth/245.0;
	int nSizeStor   = 50 * dRatio;
	int nSizeLoad   = 56 * dRatio;
	int nSizeLoadT  = 60 * dRatio;
	int nSizeLoadT2 = 66 * dRatio;

	int aSize[8];
	
	aSize[0]=0; 
	aSize[1]=nSizeStor; 
	aSize[2]=nSizeLoad;
	aSize[3]=nSizeLoad;
	aSize[4]=nSizeLoad;
	aSize[5]=nSizeLoad;
	aSize[6]=m_bWindKBC2015 ? nSizeLoadT2 : nSizeLoadT;
	aSize[7]=m_bWindKBC2015 ? nSizeLoadT2 : nSizeLoadT;
	
	CString csTitle;
	int nColCount = GetColCount();
	for (int i = 0; i < nColCount; i++)
	{
		csTitle = m_aTitle[i];
		SetStyleRange(CGXRange(0, i+1), CGXStyle()
			.SetValue(csTitle)
			.SetUserAttribute(ID_USER_ATTR_COL_ID, m_aTitle[i]));
		if (bInit) SetColWidth(i+1, i+1, aSize[i]);
	}
	SetColWidth(0,0,0);
}

void CWindSeisAddLoadGrid::Data2Grid(CArray<T_WIND_ADDITION, T_WIND_ADDITION&>& aData)
{
	int nSize = aData.GetSize();
	SetRowCount(nSize+1);

	T_STOR_D StorD;  

	for (int i=0; i<nSize; i++)
	{
		if (!m_pDoc->m_pAttrCtrl->GetStor(aData[i].KeyStor, StorD)) continue;
		
		SetValueRange(CGXRange(i+1, COLINDEX_STORY), StorD.StoryName);
		SetValueRange(CGXRange(i+1, COLINDEX_ADD_X), (double)aData[i].AddX);
		SetValueRange(CGXRange(i+1, COLINDEX_ADD_Y), (double)aData[i].AddY);
		SetValueRange(CGXRange(i+1, COLINDEX_ADD_X2), (double)aData[i].AddX2);
		SetValueRange(CGXRange(i+1, COLINDEX_ADD_Y2), (double)aData[i].AddY2);
		SetValueRange(CGXRange(i+1, COLINDEX_ADD_R), (double)aData[i].AddR);
		SetValueRange(CGXRange(i+1, COLINDEX_ADD_R2), (double)aData[i].AddR2);
	}
}

void CWindSeisAddLoadGrid::Grid2Data(CArray<T_WIND_ADDITION, T_WIND_ADDITION&>& aData)
{
	int nSize = GetRowCount() - 1;
	aData.SetSize(nSize);

	for(int i = 0; i < nSize; i++)
	{
		CString strValue = GetValueRowCol(i+1, COLINDEX_STORY);
		aData[i].KeyStor = m_pDoc->m_pAttrCtrl->GetStorKey(strValue);
		aData[i].AddX = _tstof(GetValueRowCol(i+1, COLINDEX_ADD_X));
		aData[i].AddY = _tstof(GetValueRowCol(i+1, COLINDEX_ADD_Y));
		aData[i].AddX2 = _tstof(GetValueRowCol(i+1, COLINDEX_ADD_X2));
		aData[i].AddY2 = _tstof(GetValueRowCol(i+1, COLINDEX_ADD_Y2));
		aData[i].AddR = _tstof(GetValueRowCol(i+1, COLINDEX_ADD_R));
		aData[i].AddR2 = _tstof(GetValueRowCol(i+1, COLINDEX_ADD_R2));
	}
}

BOOL CWindSeisAddLoadGrid::InsertRecordByKey(const int &key, const T_WIND_ADDITION &data)
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

BOOL CWindSeisAddLoadGrid::DeleteRecordByKey(const int &key, const T_WIND_ADDITION &data)
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

void CWindSeisAddLoadGrid::SetData(CArray<T_SEIS_ADDITION,T_SEIS_ADDITION&>& aSeidAddD)
{
	int nSize = aSeidAddD.GetSize();
	m_Data.SetSize(nSize);

	for(int i = 0; i < nSize ; i++)
	{
		m_Data[i].Initialize();
		m_Data[i].KeyStor = aSeidAddD[i].KeyStor;
		m_Data[i].AddX = aSeidAddD[i].AddX;
		m_Data[i].AddY = aSeidAddD[i].AddY;
		m_Data[i].AddR = aSeidAddD[i].AddR;
	}
}

void CWindSeisAddLoadGrid::GetData(CArray<T_SEIS_ADDITION, T_SEIS_ADDITION&>& aSeidAddD)
{
	OnTerminate(D_TB_SAVE_ALWAYS);
	int nSize = m_Data.GetSize();
	aSeidAddD.SetSize(nSize);

	for(int i = 0; i < nSize ; i++)
	{
		aSeidAddD[i].KeyStor = m_Data[i].KeyStor;
		aSeidAddD[i].AddX = m_Data[i].AddX;
		aSeidAddD[i].AddY = m_Data[i].AddY;
		aSeidAddD[i].AddR = m_Data[i].AddR;
	}
}

//////////////////////////////////////////////////////////////////////
// Operation

//////////////////////////////////////////////////////////////////////
// Override

BOOL CWindSeisAddLoadGrid::ValidateField(CString value, int nColID)
{
	if (nColID >= GetColCount())
	{
//    SetWarningText(_LS(IDS_WG_CMD__ADDD___CSpfcItemGrid__ValidateField__Er));
		return FALSE;
	}

	int nErrCode;
	if (nColID > 1)
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
//     if (_tstof(value) < 0.0)
//     {
//       SetWarningText(_LS(IDS_WG_CMD__ADDD__Negative_value_is_not_allowed_));
//       return FALSE;
//     }
		return TRUE;
	}

	return TRUE;
}

// 삽입이나 수정전에 Record의 Validation을 검사한다.
BOOL CWindSeisAddLoadGrid::ValidateRecord(CStringArray &value, CRowColArray &aCols)
{
	CString msg = _T("");
	int nPos;

	for (int i = 1; i < GetColCount(); i++)
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
BOOL CWindSeisAddLoadGrid::ValidateAndMakeRecord(CStringArray &value, CRowColArray& aCols, int &key, T_WIND_ADDITION &data)
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
		ConvStrToData(i, value[nPos], data);
	}
	key = data.KeyStor;
	if(key == 0)    
	{
		CString msg = _LS(IDS_CMD_ERR_STORY_NOT_EXIST);
		AfxMessageBox(msg);
		return FALSE;
	}

	return TRUE;
}

BOOL CWindSeisAddLoadGrid::AddToDB(ROWCOL nRow)
{
	// Record를 만들어서 DB에 입력한다.
	int key;
	T_WIND_ADDITION data;
	
	GetValue(nRow, key, data);
	
	return AddData(key, data);
}

BOOL CWindSeisAddLoadGrid::AddToDB(CArray<void*, void*>& raKey, CArray<void*, void*>& raData)
{
	ASSERT(raKey.GetSize() == raData.GetSize());
	double tKey;
	CArray<int, int> aKey;
	CArray<T_WIND_ADDITION, T_WIND_ADDITION&> aData;
	for (int i = 0; i < raKey.GetSize(); i++)
	{
		tKey = *((int*)raKey.GetAt(i));
		aKey.Add(tKey);
		aData.Add(*((T_WIND_ADDITION*)raData.GetAt(i)));
	}
	return AddData(aKey, aData);
}

BOOL CWindSeisAddLoadGrid::DeleteFromDB(ROWCOL nRow)
{
	int key;
	T_WIND_ADDITION data;
	
	GetValue(nRow, key, data);
	return DelData(key);
}

BOOL CWindSeisAddLoadGrid::DeleteFromDB(CRowColArray& awRows)
{
	if (awRows.GetSize() <= 0) return TRUE;
	
	ROWCOL nCol;
	VERIFY(GetColumnNoFor(0, nCol));
	
	CString value;
	CArray<int, int> awKeys;
	
	for (int i = 0; i < awRows.GetSize(); i++)
	{
		value = GetValueRowColBeforeEdit(awRows.GetAt(i), nCol);
		awKeys.Add(_tstof(value));
	}
	
	return DelData(awKeys);
}

BOOL CWindSeisAddLoadGrid::ModifyToDB(ROWCOL nRow)
{
	ROWCOL nCol;
	VERIFY(GetColumnNoFor(0, nCol));
	
	int key, oldKey;
	T_WIND_ADDITION data;
	
	oldKey = _ttoi(GetValueRowColBeforeEdit(nRow, nCol));
	GetValue(nRow, key, data);
	
	// 수정한다. 실패하면 에러 리턴
	return ModifyData(oldKey, key, data); 
}

BOOL CWindSeisAddLoadGrid::ModifyToDB(CStringArray& raOldKey, 
															 CArray<void*, void*>& raKey, 
															 CArray<void*, void*>& raData)
{
	ASSERT(raKey.GetSize() == raData.GetSize());
	ASSERT(raKey.GetSize() == raOldKey.GetSize());
	CArray<int, int> aOldKey, aKey;
	CArray<T_WIND_ADDITION,T_WIND_ADDITION&> aData;
	double Key;
	int nSize = raKey.GetSize();
	for (int i = 0; i < nSize; i++)
	{
		Key = _tstof(raOldKey.GetAt(i));
		aOldKey.Add(Key);
		aKey.Add(*((int*)raKey.GetAt(i)));
		aData.Add(*((T_WIND_ADDITION*)raData.GetAt(i)));
	}
	return ModifyData(aOldKey, aKey, aData);
}

CString CWindSeisAddLoadGrid::GetNewKey()
{
	CString value(_T("0"));
	return value;
}

CString CWindSeisAddLoadGrid::GetDefValue(int nIndex)
{
	return m_aCurDefVal[nIndex];
}

CString* CWindSeisAddLoadGrid::GetDefValueArray()
{
	return m_aCurDefVal;
}

BOOL CWindSeisAddLoadGrid::MakeKeyAndDataArray(CStringArray& value, CRowColArray& awCols, 
																				CArray<void*, void*>& aKey, CArray<void*, void*>& aData)
{
	int* pKey;
	T_WIND_ADDITION* pData;
	pKey = new int;
	pData = new T_WIND_ADDITION;
	aKey.Add((void*)pKey);
	aData.Add((void*)pData);
	if (!ValidateAndMakeRecord(value, awCols, *pKey, *pData)) return FALSE;
	return TRUE;
}

void CWindSeisAddLoadGrid::DeleteKeyAndDataArray(CArray<void*, void*>& aKey, CArray<void*, void*>& aData)
{
	double* pKey;
	T_WIND_ADDITION* pData;
	ASSERT(aKey.GetSize() == aData.GetSize());
	for (int i = 0; i < aKey.GetSize(); i++)
	{
		pKey = (double*)aKey.GetAt(i);
		pData = (T_WIND_ADDITION*)aData.GetAt(i);
		delete pKey;
		delete pData;
	}
}

long CWindSeisAddLoadGrid::GetCountData()
{
	return m_Data.GetSize();
}

void CWindSeisAddLoadGrid::MakeItemFiltered(long nFilteredNum, long* aFilteredKeys, CRowColArray& awCols)
{
	ASSERT(0); // no filtering
}

void CWindSeisAddLoadGrid::MakeItemNormal(long nNumData, CRowColArray& awCols)
{
	double Key;
	T_WIND_ADDITION Data;
	
	long nCount;
	for(nCount=0 ; nCount < nNumData; nCount++)
	{
		Key = m_Data[nCount].KeyStor;
		Data = m_Data[nCount];
		
		SetValue(GetParam()->GetData(), nCount+1, Key, Data, awCols);
	} // end of loop nCount
}

void CWindSeisAddLoadGrid::MakeItemEditingRecord(ROWCOL ncRow, CStringArray& EditingRecord)
{
	CDBDoc* pDoc = m_pDoc;
	double Key;
	T_WIND_ADDITION Data;
	for (int i = 0; i < GetColCount(); i++)
	{
		ConvStrToData(i, EditingRecord[i], Data);
	}
	Key = Data.KeyStor;
	// 현재 값을 설정한다.
	SetValue(ncRow, Key, Data);
}

/////////////////////////////////////////////////////////////////////////////
// CLD_CTRL_WIND_ShapeFactGrid Implementation
BOOL CWindSeisAddLoadGrid::GetValue(ROWCOL nRow, int &key, T_WIND_ADDITION &data)
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
	key = data.KeyStor;
	return TRUE;
}

BOOL CWindSeisAddLoadGrid::SetValue(ROWCOL nRow, const int &key, const T_WIND_ADDITION &data)
{
	//
	CRowColArray awCols;
	awCols.SetSize(GetColCount());
	for (int i = 0;i < GetColCount(); i++)
		VERIFY(GetColumnNoFor(m_aTitle[i], awCols[i]));
	return SetValue(GetParam()->GetData(), nRow, key, data, awCols);
}

BOOL CWindSeisAddLoadGrid::SetValue(CGXData* pData, ROWCOL nRow, const int &key, const T_WIND_ADDITION &data, CRowColArray &awCols)
{
	CString str;
	for (int i = 0; i < GetColCount(); i++)
	{
		ConvDataToStr(i, data, str);
		pData->StoreValueRowCol(nRow, awCols[i], str, gxOverride, GX_VT_NUMERIC);
	}

	return TRUE;
}

void CWindSeisAddLoadGrid::ConvStrToData(int i, CString& value, T_WIND_ADDITION& data)
{
	if(i == 0)  return;
	else if(i == 1) 
	{
		data.KeyStor  = m_pDoc->m_pAttrCtrl->GetStorKey(value);
	}
	else if(i == 2) data.AddX    = _tstof(value);
	else if(i == 3) data.AddY    = _tstof(value);
	else if(i == 4) data.AddX2   = _tstof(value);
	else if(i == 5) data.AddY2   = _tstof(value);
	else if(i == 6) data.AddR    = _tstof(value);
	else if(i == 7) data.AddR2   = _tstof(value);
}

void CWindSeisAddLoadGrid::ConvDataToStr(int i, const T_WIND_ADDITION& data, CString& value)
{
	if(i == 0) value.Format(_T("%d"),data.KeyStor);
	else if (i == 1)
	{
		T_STOR_D StorD;
		if(!m_pDoc->m_pAttrCtrl->GetStor(data.KeyStor, StorD))  StorD.Initialize();
		value = StorD.StoryName;
	}
	else if(i == 2) value.Format(_T("%g"),data.AddX);
	else if(i == 3) value.Format(_T("%g"),data.AddY);
	else if(i == 4) value.Format(_T("%g"),data.AddX2);
	else if(i == 5) value.Format(_T("%g"),data.AddY2);
	else if(i == 6) value.Format(_T("%g"),data.AddR);
	else if(i == 7) value.Format(_T("%g"),data.AddR2);
}

void CWindSeisAddLoadGrid::MakeSearchKey(const int &key, const T_WIND_ADDITION &data, CStringArray &aKey)
{
	int nColID;
	int nSortKeyNum = m_aSortInfo.GetSize();

	aKey.SetSize(nSortKeyNum);
	for (int i = 0; i < nSortKeyNum; i++)
	{
		nColID = m_aSortInfo[i].nRC;
		if (nColID == 0) aKey[i].Format(_T("%d"), key);
		else ConvDataToStr(nColID, data, aKey[i]);
	}
}

BOOL CWindSeisAddLoadGrid::FindData(int key, T_WIND_ADDITION& data, int& nPos, int nStart)
{
	int i;
	int nSize = m_Data.GetSize();
	
	for (i = nStart; i < nSize; i++)
	{
		if(key < m_Data[i].KeyStor)   break;
		if(key == m_Data[i].KeyStor)
		{ 
			nPos = i; 
			return TRUE;
		}    
	}
	
	nPos = i;
	return FALSE;
}

BOOL CWindSeisAddLoadGrid::AddData(int key, T_WIND_ADDITION& data)
{

	CArray<int, int> aOldKey, aKey;
	CArray<T_WIND_ADDITION, T_WIND_ADDITION&> aData;
	aKey.Add(key);
	aData.Add(data);
	// 동일한 key값 중복 검토
	int nPos;
	if (FindData(key, data, nPos, 0))
	{
		CString msg = _LS(IDS_CMD_ERR_STORY_ALREADY_EXIST);
		AfxMessageBox(msg);
		return FALSE;
	}
	m_Data.InsertAt(nPos, data);
	UpdateBuffer(0, aOldKey, aKey, aData);
	DBChanged();

	return TRUE;
}

BOOL CWindSeisAddLoadGrid::AddData(CArray<int, int> &aKey, CArray<T_WIND_ADDITION, T_WIND_ADDITION&> &aData, BOOL bNotify)
{
	CArray<int, int> aOldKey;

	// 동일한 key값 중복 검토
	int nSize = aKey.GetSize();
	aOldKey.Copy(aKey);
	qsort((void*)aOldKey.GetData(), nSize, sizeof(int), CCompFunc::intAsc);

	BOOL bSame;
	int nPos=0, nStart;
	for (int i = 0; i < nSize; i++)
	{
		nStart = nPos;
		bSame = FindData(aOldKey[i], aData[i], nPos, nStart);

		if (bSame)
		{
			CString msg = _LS(IDS_CMD_ERR_STORY_ALREADY_EXIST);
			AfxMessageBox(msg);
			return FALSE;
		}
		nPos = nStart;
	}
	for (int i = 0; i < nSize; i++)
	{
		bSame = FindData(aKey[i], aData[i], nPos, 0);
		VERIFY(!bSame);

		m_Data.InsertAt(nPos, aData[i]);
	}
	UpdateBuffer(0, aOldKey, aKey, aData);

	if (bNotify) DBChanged();
	
	return TRUE;
}

BOOL CWindSeisAddLoadGrid::DelData(int key)
{
	CArray<int, int> aOldKey, aKey;
	CArray<T_WIND_ADDITION,T_WIND_ADDITION&> aData;
	aKey.Add(key);

	int nSize = m_Data.GetSize();
	T_WIND_ADDITION tempData;

	int nPos;
	if (!FindData(key, tempData, nPos))
	{
		CString msg = _LS(IDS_CMD_ERR_STORY_NOT_EXIST);
		AfxMessageBox(msg);
		return FALSE;
	}

	aData.Add(m_Data[nPos]);
	m_Data.RemoveAt(nPos);
	
	UpdateBuffer(1, aOldKey, aKey, aData);
	DBChanged();
	return TRUE;
}

BOOL CWindSeisAddLoadGrid::DelData(CArray<int, int> &aKey, BOOL bNotify)
{
	CArray<int, int> aOldKey;
	int nSize = aKey.GetSize();

	CArray<T_WIND_ADDITION,T_WIND_ADDITION&> aData;

	aOldKey.Copy(aKey);
	qsort((void*)aOldKey.GetData(), nSize, sizeof(int), CCompFunc::intAsc);
 
	int nAddSize = m_Data.GetSize();
	for(int k = nAddSize-1; k >= 0; k--)
	{
		for (int i = aOldKey.GetSize()-1; i >= 0; i--)
		{
			if(m_Data[k].KeyStor == aOldKey[i])
			{
				aData.Add(m_Data[k]);
				m_Data.RemoveAt(k);
				break;
			}
		}
	}

	if(aData.GetSize() != nSize)
	{
		CString msg = _LS(IDS_CMD_ERR_STORY_NOT_EXIST);
		AfxMessageBox(msg);
		return FALSE;
	}

	UpdateBuffer(1, aOldKey, aKey, aData);

	if (bNotify) DBChanged();
 
	return TRUE;
}

BOOL CWindSeisAddLoadGrid::ModifyData(int oldKey, int key, T_WIND_ADDITION data)
{
	CArray<int, int> aOldKey, aKey;
	CArray<T_WIND_ADDITION, T_WIND_ADDITION&> aData;
	aOldKey.Add(oldKey);
	aKey.Add(key);
	aData.Add(data);

	int nPos;
	if (!FindData(oldKey,data, nPos))
	{
		CString msg = _LS(IDS_CMD_ERR_STORY_NOT_EXIST);
		AfxMessageBox(msg);
		return FALSE;
	}

	if (oldKey == key)
	{    
		m_Data[nPos] = data;
	}
	else
	{
		int nPos2;
		if (FindData(key, data, nPos2))
		{
			CString msg = _LS(IDS_CMD_ERR_STORY_ALREADY_EXIST);
			AfxMessageBox(msg);
			return FALSE;
		}
		m_Data.RemoveAt(nPos);
		if (nPos2 > nPos) nPos2--;
		m_Data.InsertAt(nPos2, data);
	}

	UpdateBuffer(2, aOldKey, aKey, aData);

	DBChanged();
	return TRUE;
}

BOOL CWindSeisAddLoadGrid::ModifyData(CArray<int, int> &aOldKey, CArray<int, int> &aKey, CArray<T_WIND_ADDITION, T_WIND_ADDITION&> &aData)
{
	
	CArray<T_WIND_ADDITION, T_WIND_ADDITION&> aOrgData;
	aOrgData.Copy(m_Data);
	
	if (!DelData(aOldKey, FALSE))
		return FALSE;
	if (!AddData(aKey, aData, FALSE))
	{
		m_Data.Copy(aOrgData);
		return FALSE;
	}

	DBChanged();
	return TRUE;
}

void CWindSeisAddLoadGrid::UpdateBuffer(int nCmd, CArray<int, int> &aOldKey, CArray<int, int> &aKey, CArray<T_WIND_ADDITION, T_WIND_ADDITION&> &aData)
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
				//DeleteRecordByKey(aKey[i], aData[i]);
				DeleteRecordByKey(aKey[i], aData[i]);
				break;
		case(2):
				//DeleteRecordByKey(aOldKey[i], aData[i]);
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

void CWindSeisAddLoadGrid::DBChanged()
{}

BOOL CWindSeisAddLoadGrid::OnPasteFromClipboard(const CGXRange &range)
{
	return CTBCommon::OnPasteFromClipboard(range);
}

BOOL CWindSeisAddLoadGrid::Copy()
{
	return CTBCommon::Copy();
}

BOOL CWindSeisAddLoadGrid::OnEndEditing(ROWCOL nRow, ROWCOL nCol)
{  
	BOOL bRet = CGXGridWnd::OnEndEditing(nRow, nCol);


	return bRet;
}

void CWindSeisAddLoadGrid::SetStorColumn(ROWCOL nCol)
{
	CString szChoiceList, tmp;
	szChoiceList = _T("");
	
	CArray<CString, CString&> aStorNameList;
	m_pDoc->m_pAttrCtrl->GetStorList(aStorNameList);
	
	int nSize = aStorNameList.GetSize();
	for (int i = 0; i < nSize; i++)
	{
		tmp.Format(_T("%s\n"), aStorNameList[i]);
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