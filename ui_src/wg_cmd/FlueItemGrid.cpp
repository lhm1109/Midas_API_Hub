#include "stdafx.h"
#include "wg_cmd.h"
#include "FlueItemGrid.h"

#include "..\wg_base\wg_base_CompFunc.h"
#include "..\wg_db\wg_db_UnitCtrl.h"

#include "FluegasTempItemDlg.h"
#include "SteelTimeTempItemDlg.h"
#include "ETC_GDS_UserPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define EPSILON 1.e-12
#define DATA_FMT _T("%.15g")
#define DATA_INT _T("%d")

/////////////////////////////////////////////////////////////////////////////
// CFlueItemGrid
CString CFlueItemGrid::m_aCurDefVal[] = {_T("0.0"), _T("0.0")};
CFlueItemGrid::CFlueItemGrid(BOOL bSteel/* =FALSE */)
{
	m_pData = 0;
	m_bSteel = bSteel;
	AddColInfo(_LS(IDS_WG_CMD__ADDD__FLUE_sec_),        t_real, 0);
	if(bSteel)
		AddColInfo(_LS(IDS_WG_CMD__ADDD__FLUE_STEEL_TemPerature_), t_real, 0);
	else
		AddColInfo(_LS(IDS_WG_CMD__ADDD__FLUE_TemPerature_), t_real, 0);

	m_aPrimaryKey.Add(0);
}

CFlueItemGrid::~CFlueItemGrid()
{
}


BEGIN_MESSAGE_MAP(CFlueItemGrid, CTBCommon)
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CFlueItemGrid message handlers

void CFlueItemGrid::Initialize(CArray<T_FLUE_BASE, T_FLUE_BASE &>* pData)
{
	ASSERT(pData);
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
	pProp->SetUserProperty(GX_IDS_OUTLINECURRENTCELL,
												(CGXStyle) pProp->sInvertNoBorder);

	LockUpdate(TRUE);

	int m_nColCount = GetInitialColCount();
	SetColCount(m_nColCount);
	SetHeaderTitle(us, TRUE);

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

	SetStyleRange(CGXRange().SetCols(1, m_nColCount), CGXStyle()
			.SetHorizontalAlignment(DT_RIGHT)
			.SetValueType(GX_VT_NUMERIC)
			.SetFormat(GX_FMT_FIXED)
			.SetPlaces(4));
	
	SetRowHeightByDPI(0, 0, 36);

	SetStyleRange(CGXRange().SetCols(2), CGXStyle()
			.SetPlaces(4));

	SetRightValidCell(1, 0);
	GetParam()->SetHideCurrentCell(GX_HIDE_NEVER);

	m_aSortInfo.SetSize(1);
	m_aSortInfo[0].nRC = 0;
	m_aSortInfo[0].sortType = CGXSortInfo::numeric;
	m_aSortInfo[0].sortOrder = CGXSortInfo::ascending;


	//if(bShowReadOnly==TRUE)
	//{
	//	SetStyleRange(CGXRange().SetCols(1, GetColCount()), CGXStyle()
	//		.SetReadOnly(TRUE));
	//}

	LockUpdate(FALSE);
}

BOOL CFlueItemGrid::InsertRecordByKey(const double &key, const double &data)
{
	ROWCOL nRow;

	CStringArray aKey;
	MakeSearchKey(key, data, aKey);
	FindRecordByKey(aKey, nRow);

	VERIFY(InsertRecord(nRow));
	return SetValue(nRow, key, data);
}

BOOL CFlueItemGrid::DeleteRecordByKey(const double &key, const double &data)
{
	ROWCOL nRow;

	CStringArray aKey;
	MakeSearchKey(key, data, aKey);
	if (!FindRecordByKey(aKey, nRow)) return FALSE;

	return DeleteRecord(nRow);
}

/////////////////////////////////////////////////////////////////////////////
// CFlueItemGrid Overridable Functions
void CFlueItemGrid::OnChangedSelectionPublic(long *keys, int nSize)
{
	return;
}

long CFlueItemGrid::GetSelectedKeys(CArray<long, long> &caKey)
{
	return 0;
}

BOOL CFlueItemGrid::ValidateField(CString value, int nColID)
{
	if (nColID >= GetColCount())
	{
		SetWarningText(_T("Flue Error"));
		return FALSE;
	}

	int nErrCode;
	if (nColID == 0 || nColID == 1)
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
		return TRUE;
	}
	else return FALSE;

	return TRUE;
}

BOOL CFlueItemGrid::ValidateRecord(CStringArray &value, CRowColArray &aCols)
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

BOOL CFlueItemGrid::ValidateAndMakeRecord(CStringArray &value, CRowColArray& aCols, double &key, double &data)
{
	if (!ValidateRecord(value, aCols))
	{
		//SetWarningText(_T(""));
		return FALSE;
	}

	int nPos;
	for (int i = 0; i < GetColCount(); i++)
	{
		nPos = aCols[i]-1;
		if(i==0) key = _tstof(value[nPos]);
		else ConvStrToData(i, value[nPos], data);
	}
	return TRUE;
}

BOOL CFlueItemGrid::AddToDB(ROWCOL nRow)
{
	double key;
	double data;
 
	GetValue(nRow, key, data);

	return AddData(key, data);
}

BOOL CFlueItemGrid::AddToDB(CArray<void*, void*>& raKey, CArray<void*, void*>& raData)
{
	ASSERT(raKey.GetSize() == raData.GetSize());
	double tKey;
	CArray<double, double> aKey;
	CArray<double, double> aData;
	for (int i = 0; i < raKey.GetSize(); i++)
	{
		tKey = *((double*)raKey.GetAt(i));
		aKey.Add(tKey);
		aData.Add(*((double*)raData.GetAt(i)));
	}
	return AddData(aKey, aData);
}


BOOL CFlueItemGrid::DeleteFromDB(ROWCOL nRow)
{
	double key;
	double data;
 
	GetValue(nRow, key, data);
	return DelData(key);
}

BOOL CFlueItemGrid::DeleteFromDB(CRowColArray& awRows)
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

BOOL CFlueItemGrid::ModifyToDB(ROWCOL nRow)
{
	ROWCOL nCol;
	VERIFY(GetColumnNoFor(0, nCol));

	double key, oldKey;
	double data;
	
	oldKey = _tstof(GetValueRowColBeforeEdit(nRow, nCol));
	GetValue(nRow, key, data);

	return ModifyData(oldKey, key, data); 
}

BOOL CFlueItemGrid::ModifyToDB(CStringArray& raOldKey, 
												 CArray<void*, void*>& raKey, 
												 CArray<void*, void*>& raData)
{
	ASSERT(raKey.GetSize() == raData.GetSize());
	ASSERT(raKey.GetSize() == raOldKey.GetSize());
	CArray<double, double> aOldKey, aKey, aData;
	double Key;
	int nSize = raKey.GetSize();
	for (int i = 0; i < nSize; i++)
	{
		Key = _tstof(raOldKey.GetAt(i));
		aOldKey.Add(Key);
		aKey.Add(*((double*)raKey.GetAt(i)));
		aData.Add(*((double*)raData.GetAt(i)));
	}
	return ModifyData(aOldKey, aKey, aData);
}


CString CFlueItemGrid::GetNewKey()
{
	CString value(_T("0"));
	return value;
}

CString CFlueItemGrid::GetDefValue(int nIndex)
{
	return m_aCurDefVal[nIndex];
}

CString* CFlueItemGrid::GetDefValueArray()
{
	return m_aCurDefVal;
}

BOOL CFlueItemGrid::MakeKeyAndDataArray(CStringArray& value, CRowColArray& awCols, 
																	CArray<void*, void*>& aKey, CArray<void*, void*>& aData)
{
	double* pKey;
	double* pData;
	pKey = new double;
	pData = new double;
	aKey.Add((void*)pKey);
	aData.Add((void*)pData);
	if (!ValidateAndMakeRecord(value, awCols, *pKey, *pData)) return FALSE;
	return TRUE;
}

void CFlueItemGrid::DeleteKeyAndDataArray(CArray<void*, void*>& aKey, CArray<void*, void*>& aData)
{
	double* pKey;
	double* pData;
	ASSERT(aKey.GetSize() == aData.GetSize());
	for (int i = 0; i < aKey.GetSize(); i++)
	{
		pKey = (double*)aKey.GetAt(i);
		pData = (double*)aData.GetAt(i);
		delete pKey;
		delete pData;
	}
}

long CFlueItemGrid::GetCountData()
{
	return m_pData->GetSize();
}

void CFlueItemGrid::MakeItemFiltered(long nFilteredNum, long* aFilteredKeys, CRowColArray& awCols)
{
	ASSERT(0); // no filtering
}

void CFlueItemGrid::MakeItemNormal(long nNumData, CRowColArray& awCols)
{
	double Key;
	double Data;

	long nCount;
	for(nCount=0 ; nCount < nNumData; nCount++)
	{
		Key = m_pData->GetAt(nCount).dblPeriod;
		Data = m_pData->GetAt(nCount).dblValue;

		SetValue(GetParam()->GetData(), nCount+1, Key, Data, awCols);
	} // end of loop nCount
}

void CFlueItemGrid::MakeItemEditingRecord(ROWCOL ncRow, CStringArray& EditingRecord)
{
	CDBDoc* pDoc = m_pDoc;
	double Key;
	double Data;
	for (int i = 0; i < GetColCount(); i++)
	{
		if (i == 0) Key = _tstof(EditingRecord[i]);
		else ConvStrToData(i, EditingRecord[i], Data);
	}
	SetValue(ncRow, Key, Data);
}

/////////////////////////////////////////////////////////////////////////////
// CFlueItemGrid Implementation
BOOL CFlueItemGrid::GetValue(ROWCOL nRow, double &key, double &data)
{
	ROWCOL nCol;
	CString str;
	for (int i = 0; i < GetColCount(); i++)
	{
		VERIFY(GetColumnNoFor(m_aTitle[i], nCol));
		str = GetValueRowCol(nRow, nCol);
		VERIFY(ConvertFormulaToValue(str)); 

		if(i==0) key = _tstof(str);
		else ConvStrToData(i, str, data);
	}
	return TRUE;
}

BOOL CFlueItemGrid::SetValue(ROWCOL nRow, const double &key, const double &data)
{
	//
	CRowColArray awCols;
	awCols.SetSize(GetColCount());
	for (int i = 0;i < GetColCount(); i++)
		VERIFY(GetColumnNoFor(m_aTitle[i], awCols[i]));
	return SetValue(GetParam()->GetData(), nRow, key, data, awCols);
}

BOOL CFlueItemGrid::SetValue(CGXData* pData, ROWCOL nRow, const double &key, const double &data, CRowColArray &awCols)
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

void CFlueItemGrid::ConvStrToData(int i, CString& value, double& data)
{
	if (i==1) data = _tstof(value);
}

void CFlueItemGrid::ConvDataToStr(int i, const double& data, CString& value)
{
	if (i==1) value.Format(DATA_FMT, data);
}

void CFlueItemGrid::MakeSearchKey(const double &key, const double &data, CStringArray &aKey)
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

void CFlueItemGrid::SetHeaderTitle(T_UNIT_SYSTEM &us, BOOL bInit)
{
	CString csTitle;
	//int aUnit[][2] = {
	//	{CUnitCtrl::m_FLUE_UNIT.dblPeriod, CUnitCtrl::m_FLUE_UNIT.dblValue[0]}, 
	//	{CUnitCtrl::m_FLUE_UNIT.dblPeriod, CUnitCtrl::m_FLUE_UNIT.dblValue[1]}, 
	//	{CUnitCtrl::m_FLUE_UNIT.dblPeriod, CUnitCtrl::m_FLUE_UNIT.dblValue[2]}, 
	//	{CUnitCtrl::m_FLUE_UNIT.dblPeriod, CUnitCtrl::m_FLUE_UNIT.dblValue[3]}, 
	//};

	int aSize[] = {60, 85};
	int nColCount = GetColCount();

	for (int i = 0; i < nColCount; i++)
	{
		//m_aUnit.SetAt(i, aUnit[nDataType][i]);
		//CUnitCtrl::GetUnitSystem(aUnit[nDataType][i], csUnit);
		//if (csUnit.IsEmpty()) csTitle = m_aTitle[i] + _T("\n(g)");
		//else csTitle = m_aTitle[i] + _T("\n(")+csUnit+_T(")");
		csTitle = m_aTitle[i];
		SetStyleRange(CGXRange(0, i+1), CGXStyle()
			.SetValue(csTitle)
			.SetUserAttribute(ID_USER_ATTR_COL_ID, m_aTitle[i]));
		if (bInit) SetColWidthByDPI(i+1, i+1, aSize[i]);
	}
	if (bInit) SetColWidthByDPI(0, 0, 35);
}

BOOL CFlueItemGrid::FindData(double key, int& nPos, int nStart)
{
	int i;
	int nSize = m_pData->GetSize();
	
	CString csVal;
	double dblVal;
	int nCompResult;
	for (i = nStart; i < nSize; i++)
	{
		csVal.Format(DATA_INT, m_pData->GetAt(i).dblPeriod);
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

BOOL CFlueItemGrid::ExistAllKey(CArray<double, double>& aKey, double& errorKey)
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

BOOL CFlueItemGrid::AddData(double key, double data)
{
	CArray<double, double> aOldKey, aKey, aData;
	aKey.Add(key);
	aData.Add(data);
	T_FLUE_BASE item;
	item.dblPeriod = key;
	item.dblValue = data;

	int nPos;
	if (FindData(key, nPos, 0))
	{
		CString msg;
		msg.Format(_LS(IDS_WG_CMD__ADDD__Time__g_already_exist_), key);
		AfxMessageBox(msg);
		return FALSE;
	}
	m_pData->InsertAt(nPos, item);
	UpdateBuffer(0, aOldKey, aKey, aData);
	DBChanged();
	return TRUE;
}

BOOL CFlueItemGrid::AddData(CArray<double, double> &aKey, CArray<double, double> &aData, BOOL bNotify)
{
	CArray<double, double> aOldKey;

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
			msg.Format(_LS(IDS_CMD_TDMF_GRID_time__d__already_exist_), aOldKey[i]);
			AfxMessageBox(msg);
			return FALSE;
		}
		nPos = nStart;
	}
	for (int i = 0; i < nSize; i++)
	{
		bSame = FindData(aKey[i], nPos, 0);
		VERIFY(!bSame);

		T_FLUE_BASE item;
		item.dblPeriod = aKey[i];
		item.dblValue = aData[i];
		m_pData->InsertAt(nPos, item);
	}
	UpdateBuffer(0, aOldKey, aKey, aData);

	if (bNotify) DBChanged();
	return TRUE;
}

BOOL CFlueItemGrid::DelData(double key)
{
	CArray<double, double> aOldKey, aKey, aData;
	aKey.Add(key);
	aData.Add(0.0);

	int nPos;
	if (!FindData(key, nPos))
	{
		CString msg;
		msg.Format(_LS(IDS_CMD_TDMF_GRID_time__d__does_not_exist_), key);
		AfxMessageBox(msg);
		return FALSE;
	}
	m_pData->RemoveAt(nPos);
	UpdateBuffer(1, aOldKey, aKey, aData);
	DBChanged();
	return TRUE;
}

BOOL CFlueItemGrid::DelData(CArray<double, double> &aKey, BOOL bNotify)
{
	CArray<double, double> aOldKey, aData;
	int nSize = aKey.GetSize();
	for (int i = 0; i < nSize; i++) aData.Add(0.0);

	aOldKey.Copy(aKey);
	qsort((void*)aOldKey.GetData(), nSize, sizeof(double), CCompFunc::doubleAsc);
	double errKey;
	if (!ExistAllKey(aOldKey, errKey))
	{
		CString msg;
		msg.Format(_LS(IDS_CMD_TDMF_GRID_time__d__does_not_exist_), errKey);
		AfxMessageBox(msg);
		return FALSE;
	}
	int nSize2 = m_pData->GetSize();
	int j = nSize2-1;
	//double dblDiff;
	for (int i = nSize-1; i >= 0; i--)
	{
		j = m_pData->GetSize()-1;
		for (; j >= 0; j--)
		{
			//dblDiff = aOldKey[i] - m_pData->arFlueFunData[j].dblPeriod;
			//if (dblDiff >= -EPSILON && dblDiff <= EPSILON) 
			if (CCompFunc::SafeCompDouble(aOldKey[i], m_pData->GetAt(j).dblPeriod) == 0)
			{
				m_pData->RemoveAt(j);
				j--;
				break;
			}
		}
	}

	UpdateBuffer(1, aOldKey, aKey, aData);

	if (bNotify) DBChanged();
	return TRUE;
}

BOOL CFlueItemGrid::ModifyData(double oldKey, double key, double data)
{
	CArray<double, double> aOldKey, aKey, aData;
	aOldKey.Add(oldKey);
	aKey.Add(key);
	aData.Add(data);

	int nPos;
	if (!FindData(oldKey, nPos))
	{
		CString msg;
		msg.Format(_LS(IDS_WG_CMD__ADDD__Old_Time__g_does_not_exist_), oldKey);
		AfxMessageBox(msg);
		return FALSE;
	}
	if (oldKey == key)
	{
		m_pData->GetAt(nPos).dblValue = data;
	}
	else
	{
		int nPos2;
		if (FindData(key, nPos2))
		{
			CString msg;
			msg.Format(_LS(IDS_WG_CMD__ADDD__Old_Time__g_does_not_exist_), key);
			AfxMessageBox(msg);
			return FALSE;
		}
		T_FLUE_BASE item;
		item.dblPeriod = key;
		item.dblValue = data;
		m_pData->RemoveAt(nPos);
		if (nPos2 > nPos) nPos2--;
		m_pData->InsertAt(nPos2, item);
	}

	UpdateBuffer(2, aOldKey, aKey, aData);

	DBChanged();
	return TRUE;
}

BOOL CFlueItemGrid::ModifyData(CArray<double, double> &aOldKey, CArray<double, double> &aKey, CArray<double, double> &aData)
{
	CArray<T_FLUE_BASE, T_FLUE_BASE&> aOrgData;
	aOrgData.Copy(*m_pData);
	
	if (!DelData(aOldKey, FALSE))
		return FALSE;
	if (!AddData(aKey, aData, FALSE))
	{
		m_pData->Copy(aOrgData);
		return FALSE;
	}
	DBChanged();
	return TRUE;
}

void CFlueItemGrid::UpdateBuffer(int nCmd, CArray<double, double> &aOldKey, CArray<double, double> &aKey, CArray<double, double> &aData)
{
	ROWCOL ncRow, ncCol;
	GetCurrentCell(ncRow, ncCol);

	BOOL bOldLock = LockUpdate(TRUE);

	int nSize = aKey.GetSize();
	for (int i = 0; i < nSize; i++)
	{
		switch(nCmd)
		{
		case(0):
				InsertRecordByKey(aKey[i], aData[i]);
				break;
		case(1):
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

	unsigned int nOldMode = m_nEditMode;
	if (nOldMode != CTBBrowserWnd::addnew)
	{
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

void CFlueItemGrid::DBChanged()
{
	if(m_bSteel)
	{
		CSteelTimeTempItemDlg* pDlg = (CSteelTimeTempItemDlg*)GetParent();
		if (!pDlg) return;
		if (pDlg->GetSafeHwnd() == 0) return;
		if (!IsWindow(pDlg->GetSafeHwnd())) return;
		pDlg->DrawGraphView(); 
	}
	else
	{
		CFlueItemDlg* pDlg = (CFlueItemDlg*)GetParent();
		if (!pDlg) return;
		if (pDlg->GetSafeHwnd() == 0) return;
		if (!IsWindow(pDlg->GetSafeHwnd())) return;

		//m_pData->nType = 1;;

		pDlg->DrawGraphView(); 
	}
}

BOOL CFlueItemGrid::OnPasteFromClipboard(const CGXRange &range)
{
	if (IsLockEdit()) return FALSE;
	return CTBCommon::OnPasteFromClipboard(range);
}

BOOL CFlueItemGrid::Copy()
{
	return CTBCommon::Copy();
}