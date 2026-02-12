#include "stdafx.h"
#include "wg_cmd.h"
#include "CmdLcomSP20ReliabilityFactorDlg.h"
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl2.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "../wg_base/DlgUtil.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCmdLcomSP20ReliabilityFactorDlg dialog

CCmdLcomSP20ReliabilityFactorDlg::CCmdLcomSP20ReliabilityFactorDlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CCmdLcomSP20ReliabilityFactorDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCmdLcomSP20ReliabilityFactorDlg)
	//}}AFX_DATA_INIT
	m_pLcomPsiFactorGrid = new CCmdLcomSP20ReliabilityGrid;
	m_pDoc = NULL;
	m_aLcomRcfiFactor.RemoveAll();
}

CCmdLcomSP20ReliabilityFactorDlg::~CCmdLcomSP20ReliabilityFactorDlg()
{
	if (m_pLcomPsiFactorGrid) delete m_pLcomPsiFactorGrid;
}

void CCmdLcomSP20ReliabilityFactorDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCmdLcomSP20ReliabilityFactorDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CCmdLcomSP20ReliabilityFactorDlg, CDialogMove)
	//{{AFX_MSG_MAP(CCmdLcomSP20ReliabilityFactorDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// CCmdLcomSP20ReliabilityFactorDlg message handlers

BOOL CCmdLcomSP20ReliabilityFactorDlg::OnInitDialog() 
{
	m_pDoc = CDBDoc::GetDocPoint();

	CDialogMove::OnInitDialog();

	CRect rect;
	GetWindowRect(&rect);
	int nAugmentWidth = 360;
	int newWidth = rect.Width() + nAugmentWidth;
	SetWindowPos(NULL, 0, 0, newWidth, rect.Height(), SWP_NOMOVE | SWP_NOZORDER);

	GetDlgItem(IDC_CMD_LCOM_PSI_FACTOR_GRID)->GetWindowRect(&rect);
	newWidth = rect.Width() + nAugmentWidth;
	GetDlgItem(IDC_CMD_LCOM_PSI_FACTOR_GRID)->SetWindowPos(NULL, 0, 0, newWidth, rect.Height(), SWP_NOMOVE | SWP_NOZORDER);

	GetDlgItem(IDC_WG_CMD_STATIC0)->GetWindowRect(&rect);
	newWidth = rect.Width() + nAugmentWidth;
	GetDlgItem(IDC_WG_CMD_STATIC0)->SetWindowPos(NULL, 0, 0, newWidth, rect.Height(), SWP_NOMOVE | SWP_NOZORDER);

	CArray<UINT, UINT> aMoveCtrl;
	aMoveCtrl.Add(IDOK);
	aMoveCtrl.Add(IDCANCEL);
	CDlgUtil::CtrlMoveDistX(this, aMoveCtrl, nAugmentWidth);

	m_pLcomPsiFactorGrid->SubclassDlgItem(IDC_CMD_LCOM_PSI_FACTOR_GRID, this);
	T_UNIT_SYSTEM UnitSystem;
	CLoadCombSP20Ctrl::Instance()->GenerateRfiParam(&m_aLcomRcfiFactor);
	m_pLcomPsiFactorGrid->SetData(&m_aLcomRcfiFactor);
	m_pLcomPsiFactorGrid->Initialize(m_pDoc, UnitSystem);


	SetWindowText(_LS(IDS_CMD_SP20_LOAD_PROPERTIES_TITLE));

	return TRUE;
}

void CCmdLcomSP20ReliabilityFactorDlg::OnOK() 
{
	m_pLcomPsiFactorGrid->OnTerminate(D_TB_SAVE_ALWAYS);
	CLoadCombSP20Ctrl::Instance()->SetRfiParam(m_aLcomRcfiFactor);
	CDialogMove::OnOK();
}

void CCmdLcomSP20ReliabilityFactorDlg::OnCancel() 
{
	CDialogMove::OnCancel();
}


#define NAME_COL_ID         2
#define LCOM_KEY_FMT        _T("%d")

CString CCmdLcomSP20ReliabilityGrid::m_aCurDefVal[] = {_T(""), _T(""), _T(""), _T(""), _T(""), _T(""), _T(""), _T(""), _T(""), _T("") };

CCmdLcomSP20ReliabilityGrid::CCmdLcomSP20ReliabilityGrid()
{
	AddColInfo(_ULS(Key), t_integer, 0);
	AddColInfo(_LS(IDS_WG_CMD__ADDD__No),		t_integer,		0);
	AddColInfo(_STR(_T("Load Case")),			t_string,		0);
	AddColInfo(_LS(IDS_WG_CMD__ADDD__Type),		t_string,		0);
	AddColInfo(_LSX(gF),						t_real,			0);
	AddColInfo(_LSX(gfa),						t_real,			0);
	AddColInfo(_LSX(gN),						t_real,			0);
	AddColInfo(_LSX(gFo),						t_real,			0);
	AddColInfo(_LSX(dL),						t_real,			0);
	AddColInfo(_LSX(Dominace_1),				t_integer,		0);
	AddColInfo(_LSX(Dominace_2),				t_integer,		0);
	m_aPrimaryKey.Add(0);
}


CCmdLcomSP20ReliabilityGrid::~CCmdLcomSP20ReliabilityGrid()
{
}

#define CWnd CTBCommon
BEGIN_MESSAGE_MAP(CCmdLcomSP20ReliabilityGrid, CWnd)
#undef CWnd
	//{{AFX_MSG_MAP(CCmdLcomSP20ReliabilityGrid)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


// CCmdLcomSP20ReliabilityGrid message handlers
void CCmdLcomSP20ReliabilityGrid::Initialize(CDBDoc* pDoc, T_UNIT_SYSTEM &UnitSystem)
{
	CTBCommon::Initialize(pDoc, UnitSystem);

	LockUpdate(TRUE);

	SetColCount(GetInitialColCount());
	SetHeaderTitle(UnitSystem, TRUE);

	ChangeStandardStyle(CGXStyle()
			.SetFont(CGXFont_GC().SetSize(10))
			.SetVerticalAlignment(DT_BOTTOM)
			.SetAllowEnter(FALSE));

	SetFrozenCols(2, 2);
	HideColsByIntend(1, 1, TRUE);

	SetStyleRange(CGXRange().SetCols(1, 2), CGXStyle()
			.SetHorizontalAlignment(DT_RIGHT)
			.SetValueType(GX_VT_NUMERIC)
			.SetFormat(GX_FMT_FIXED)
			.SetPlaces(0)
			.SetEnabled(FALSE));

	CString strStldList = GetLoadNameList();
	SetStyleRange(CGXRange().SetCols(3), CGXStyle()
			.SetControl(GX_IDS_CTRL_TABBED_COMBOBOX)
			.SetChoiceList(strStldList)
			.SetUserAttribute(GX_IDS_UA_TABLIST_KEYCOL, _T("0"))
			.SetUserAttribute(GX_IDS_UA_TABLIST_TEXTCOL, _T("0"))
			.SetUserAttribute(GX_IDS_UA_TABLIST_SHOWALLCOLS, _T("1"))
			.SetValueType(GX_VT_STRING)
			.SetHorizontalAlignment(DT_LEFT)
			.SetValue(_T(""))
			.SetEnabled(FALSE));

	CString strTypeList = GetLoadTypeList();
	SetStyleRange(CGXRange().SetCols(4), CGXStyle()
				  .SetControl(GX_IDS_CTRL_TABBED_COMBOBOX)
				  .SetChoiceList(strTypeList)
				  .SetUserAttribute(GX_IDS_UA_TABLIST_KEYCOL, _T("0"))
				  .SetUserAttribute(GX_IDS_UA_TABLIST_TEXTCOL, _T("0"))
				  .SetUserAttribute(GX_IDS_UA_TABLIST_SHOWALLCOLS, _T("1"))
				  .SetValueType(GX_VT_STRING)
				  .SetHorizontalAlignment(DT_LEFT)
				  .SetValue(_T("")));

	SetStyleRange(CGXRange().SetCols(5,9), CGXStyle()
			.SetHorizontalAlignment(DT_RIGHT)
			.SetValueType(GX_VT_NUMERIC)
			.SetFormat(GX_FMT_FIXED)
			.SetPlaces(4));

	SetStyleRange(CGXRange().SetCols(10,11), CGXStyle()
				  .SetControl(GX_IDS_CTRL_CHECKBOX3D)
				  .SetValueType(GX_VT_NUMERIC)
				  .SetHorizontalAlignment(DT_CENTER)
				  .SetVerticalAlignment(DT_VCENTER)
				  .SetValue(_T("")));

	CGXProperties* pProp = GetParam()->GetProperties();
	pProp->SetUserProperty(GX_IDS_OUTLINECURRENTCELL,(CGXStyle)pProp->sInvertNoBorder);

	SetRightValidCell(1, 0);
	GetParam()->SetHideCurrentCell(GX_HIDE_NEVER);

	m_aSortInfo.SetSize(2);
	m_aSortInfo[0].nRC = 1; // Serial No
	m_aSortInfo[0].sortType = CGXSortInfo::numeric;
	m_aSortInfo[0].sortOrder = CGXSortInfo::ascending;
	m_aSortInfo[1].nRC = 0; // key
	m_aSortInfo[1].sortType = CGXSortInfo::numeric;
	m_aSortInfo[1].sortOrder = CGXSortInfo::ascending;

	// always show vertical scrollbar
	SetScrollBarMode(SB_VERT, gxnEnabled | gxnEnhanced);  

	GetParam()->EnableMoveRows(FALSE);
	EnableInsertRowByPressingKey(FALSE);
	EnableAppendRow(FALSE);

	SetColWidth(1, 1, 0);
	SetColWidth(2, 2, 40);
	SetColWidth(3, 3, 100);
	SetColWidth(4, 4, 90);
	SetColWidth(5, 9, 55);
	SetColWidth(10, 11, 85);
	
	MakeItemEx();
	LockUpdate(FALSE);
}

// CCmdLcomSP20ReliabilityGrid Overridable Functions
void CCmdLcomSP20ReliabilityGrid::OnChangedSelectionPublic(long *keys, int nSize)
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

long CCmdLcomSP20ReliabilityGrid::GetSelectedKeys(CArray<long, long> &caKey)
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

BOOL CCmdLcomSP20ReliabilityGrid::ValidateField(CString value, int nColID)
{
	if (nColID >= GetColCount())
	{
		SetWarningText(_LS(IDS_WG_CMD__ADDD___CCMLoadCombMainGrid__ValidateFie));
		return FALSE;
	}

	return TRUE;
}

BOOL CCmdLcomSP20ReliabilityGrid::ValidateRecord(CStringArray &value, CRowColArray &aCols)
{
	return TRUE;
	//CString msg = _T("");
	//int nPos;

	//// serial no, Description, Active, Elastic 검사 안한다.
	//for (int i = 2; i < GetColCount()-3; i++)
	//{
	//	nPos = aCols[i]-1;
	//	if (value[nPos] == _T(""))
	//		msg += ((msg != _T("")) ? _T("\n") : _T("")) + m_aTitle[i] + _LS(IDS_WG_CMD__ADDD___has_not_been_entered_);
	//}
	//if (msg == _T("")) return TRUE;
	//else SetWarningText(msg);
	//return FALSE;
}

BOOL CCmdLcomSP20ReliabilityGrid::ValidateAndMakeRecord(CStringArray &value, CRowColArray& aCols, T_STLD_K &key, T_SP20_RCFI_D &data)
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

	//key = data.StldKey;

	return TRUE;
}

BOOL CCmdLcomSP20ReliabilityGrid::AddToDB(ROWCOL nRow)
{
	ASSERT(0);
	return TRUE;
}

BOOL CCmdLcomSP20ReliabilityGrid::AddToDB(CArray<void*, void*>& raKey, CArray<void*, void*>& raData)
{
	ASSERT(0);
	return TRUE;
}

BOOL CCmdLcomSP20ReliabilityGrid::DeleteFromDB(ROWCOL nRow)
{
	//T_STLD_K key;
	//T_SP20_RCFI_D data;
 //
	//GetValue(nRow, key, data);

	//T_SP20_RCFI_D TempD;
	//if(!m_mLcomPsiFactor.Lookup(key, TempD)) return FALSE;
	//m_mLcomPsiFactor.RemoveKey(key);
	//
	//T_STLD_K TempK;
	//int nIndexCount = m_mIndex.GetCount();
	//for(int i=1; i<=nIndexCount; i++)
	//{
	//	if(i<=nRow) continue;

	//	if(!m_mIndex.Lookup(i, TempK)) {ASSERT(0); continue;}
	//	m_mIndex.SetAt(i-1, TempK);
	//}
	//m_mIndex.RemoveKey(nIndexCount);
	//
	//DeleteRecord(nRow);

	//CString str;
	//int nSize = m_mLcomPsiFactor.GetCount();
	//for(int i=0; i<nSize; i++)
	//{
	//	str.Format(_T("%d"), i+1);
	//	SetExpressionRowCol(i+1, 2, str);
	//}

	return TRUE;
}

BOOL CCmdLcomSP20ReliabilityGrid::DeleteFromDB(CRowColArray& awRows)
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

BOOL CCmdLcomSP20ReliabilityGrid::ModifyToDB(ROWCOL nRow)
{
	ROWCOL nCol;
	VERIFY(GetColumnNoFor(NAME_COL_ID, nCol));  // load case name

	DGN_LCASE_K key;
	T_SP20_RCFI_D data = m_paLcomRcfiFactor->GetAt(nRow - 1);
	GetValue(nRow, key, data);

	const T_SP20_RCFI_D& PreData = m_paLcomRcfiFactor->GetAt(nRow - 1);
	if(PreData.LoadCaseType != data.LoadCaseType)
		CLoadCombSP20Ctrl::Instance()->GetDefaultRfiParam(data);

	if(data.dgF < 0. || data.dgN < 0. || data.dgFo < 0.0 || data.ddL < 0.0)
	{
		AfxMessageBox(_LS(IDS_CMD_SP20_RFI_FACTOR_SCOPE_ERROR));
		return FALSE;
	}
	if(data.eLoadCaseType == e_SP20_Constant && data.dgfa < 0.0)
	{
		AfxMessageBox(_LS(IDS_CMD_SP20_RFI_FACTOR_SCOPE_ERROR));
		return FALSE;
	}

	//DeleteRecord(nRow);
	//InsertRecord(nRow);
	SetValue(nRow, key, data);
	m_paLcomRcfiFactor->SetAt(nRow - 1, data);
	return TRUE;
}

BOOL CCmdLcomSP20ReliabilityGrid::ModifyToDB(CStringArray& raOldKey, CArray<void*, void*>& raKey, CArray<void*, void*>& raData)
{
	ASSERT(0);
	return TRUE;
}

CString CCmdLcomSP20ReliabilityGrid::GetNewKey()
{
	return _T("");
}

CString CCmdLcomSP20ReliabilityGrid::GetDefValue(int nIndex)
{
	return m_aCurDefVal[nIndex];
}

CString* CCmdLcomSP20ReliabilityGrid::GetDefValueArray()
{
	return m_aCurDefVal;
}

BOOL CCmdLcomSP20ReliabilityGrid::MakeKeyAndDataArray(CStringArray& value, CRowColArray& awCols, CArray<void*, void*>& aKey, CArray<void*, void*>& aData)
{
	T_STLD_K* pKey;
	T_SP20_RCFI_D* pData;
	pKey = new T_STLD_K;
	pData = new T_SP20_RCFI_D;
	aKey.Add((void*)pKey);
	aData.Add((void*)pData);
	if (!ValidateAndMakeRecord(value, awCols, *pKey, *pData)) return FALSE;
	return TRUE;
}

void CCmdLcomSP20ReliabilityGrid::DeleteKeyAndDataArray(CArray<void*, void*>& aKey, CArray<void*, void*>& aData)
{
	T_STLD_K* pKey;
	T_SP20_RCFI_D* pData;
	ASSERT(aKey.GetSize() == aData.GetSize());
	for (int i = 0; i < aKey.GetSize(); i++)
	{
		pKey = (T_STLD_K*)aKey.GetAt(i);
		pData = (T_SP20_RCFI_D*)aData.GetAt(i);
		delete pKey;
		delete pData;
	}
}

long CCmdLcomSP20ReliabilityGrid::GetCountData()
{
	return m_paLcomRcfiFactor->GetCount();
}

void CCmdLcomSP20ReliabilityGrid::MakeItemFiltered(long nFilteredNum, long* aFilteredKeys, CRowColArray& awCols)
{
	//CDBDoc* pDoc = m_pDoc;
	//T_SP20_RCFI_D Data;

	//for(long nCount=0 ; nCount < nFilteredNum; nCount++)
 //	{
	//	m_mLcomPsiFactor.Lookup(aFilteredKeys[nCount], Data);
	//	SetValue(GetParam()->GetData(), nCount+1, aFilteredKeys[nCount], Data, awCols);
	//}
}

void CCmdLcomSP20ReliabilityGrid::MakeItemNormal(long nNumData, CRowColArray& awCols)
{
	for (long nCount = 0; nCount < nNumData; nCount++)
	{
		//if(!m_mIndex.Lookup(nCount, Key)) {ASSERT(0); continue;}
		const T_SP20_RCFI_D& Data = m_paLcomRcfiFactor->GetAt(nCount);
		SetValue(GetParam()->GetData(), nCount + 1, Data.LCaseKey, Data, awCols);
	}
}

void CCmdLcomSP20ReliabilityGrid::MakeItemEditingRecord(ROWCOL ncRow, CStringArray& EditingRecord)
{
	CDBDoc* pDoc = m_pDoc;
	DGN_LCASE_K Key;
	T_SP20_RCFI_D Data;
	for (int i = 0; i < GetColCount(); i++)
	{
		if (i == 0) Key.keymap = _tstol(EditingRecord[i]);
		else ConvStrToData(i, EditingRecord[i], Data);
	}
	
	SetValue(ncRow, Key, Data);
}

// CCmdLcomSP20ReliabilityGrid Implementation
BOOL CCmdLcomSP20ReliabilityGrid::GetValue(ROWCOL nRow, DGN_LCASE_K&key, T_SP20_RCFI_D &data)
{
	ROWCOL nCol;
	CString str;
	for (int i = 0; i < GetColCount(); i++)
	{
		VERIFY(GetColumnNoFor(m_aTitle[i], nCol));
		str = GetValueRowCol(nRow, nCol);
		VERIFY(ConvertFormulaToValue(str)); 

		ConvStrToData(i, str, data);
	}

	//key = data.StldKey;

	return TRUE;
}

BOOL CCmdLcomSP20ReliabilityGrid::SetValue(ROWCOL nRow, const DGN_LCASE_K&key, const T_SP20_RCFI_D &data)
{
	//
	CRowColArray awCols;
	awCols.SetSize(GetColCount());
	for (int i = 0;i < GetColCount(); i++)
		VERIFY(GetColumnNoFor(m_aTitle[i], awCols[i]));
	return SetValue(GetParam()->GetData(), nRow, key, data, awCols);
}

BOOL CCmdLcomSP20ReliabilityGrid::SetValue(CGXData* pData, ROWCOL nRow, const DGN_LCASE_K&key, const T_SP20_RCFI_D &data, CRowColArray &awCols)
{
	CString str;
	for (int i = 0; i < GetColCount(); i++)
	{
		if(i==0) 
		{
			if (key.keymap == 0) str.Empty();
			else str.Format(LCOM_KEY_FMT,key);
		}
		else if(i==1)
		{
			str.Format(_T("%d"), nRow);
		}
		else
		{
			ConvDataToStr(i, data, str);
		}
		if(i > 3 && data.eLoadCaseType == e_SP20_Inactive)
			str.Empty();
		else if(i == 5 && data.eLoadCaseType != e_SP20_Constant)
		{
			str.Empty();
		}
		pData->StoreValueRowCol(nRow, awCols[i], str, gxOverride);
	}
	SetStyleRange(CGXRange(nRow, 5, nRow, 11), CGXStyle().SetEnabled(TRUE));
	if(data.eLoadCaseType == e_SP20_Inactive)
		SetStyleRange(CGXRange(nRow, 5, nRow, 11), CGXStyle().SetEnabled(FALSE));
	else
	{
		BOOL bEnableDominance = FALSE;
		if(data.eLoadCaseType == e_SP20_Long_term || data.eLoadCaseType == e_SP20_Short_term)
		{
			bEnableDominance = TRUE;
		}
		SetStyleRange(CGXRange(nRow, 10, nRow, 11), CGXStyle().SetEnabled(bEnableDominance));
		if(data.eLoadCaseType != e_SP20_Constant)
			SetStyleRange(CGXRange(nRow, 6, nRow, 6), CGXStyle().SetEnabled(FALSE));
		else
			SetStyleRange(CGXRange(nRow, 6, nRow, 6), CGXStyle().SetEnabled(TRUE));
	}
	return TRUE;
}

void CCmdLcomSP20ReliabilityGrid::ConvStrToData(int i, CString& value, T_SP20_RCFI_D& data)
{
	if (i==2)
	{
		//data.StldKey = m_pDoc->m_pAttrCtrl->GetStldKey(value);
		data.LoadCaseName = value; // name
	}
	else if (i == 3)
	{
		data.LoadCaseType = value; // type
		data.SetEnumLcaseType();
	}
	else if (i==4) data.dgF= _tstof(value);
	else if (i==5) data.dgfa= _tstof(value);
	else if (i==6) data.dgN= _tstof(value);
	else if (i == 7) data.dgFo= _tstof(value);
	else if (i == 8) data.ddL= _tstof(value);
	else if (i == 9) data.bDominace[0] = _tstoi(value);
	else if (i == 10) data.bDominace[1] = _tstoi(value);
}

void CCmdLcomSP20ReliabilityGrid::ConvDataToStr(int i, const T_SP20_RCFI_D& data, CString& value)
{
	if (i==2) value = data.LoadCaseName;
	else if (i==3) value = data.LoadCaseType;
	else if (i==4) value.Format(_T("%f"), data.dgF);
	else if (i==5) value.Format(_T("%f"), data.dgfa);
	else if (i==6) value.Format(_T("%f"), data.dgN);
	else if (i == 7) value.Format(_T("%f"), data.dgFo);
	else if (i == 8) value.Format(_T("%f"), data.ddL);
	else if (i == 9) value.Format(_T("%d"), data.bDominace[0]);
	else if (i == 10) value.Format(_T("%d"), data.bDominace[1]);

}

void CCmdLcomSP20ReliabilityGrid::MakeSearchKey(const T_STLD_K &key, const T_SP20_RCFI_D &data, CStringArray &aKey)
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

void CCmdLcomSP20ReliabilityGrid::GetAllSelectedLcom(CArray<T_STLD_K, T_STLD_K>& aSelKey)
{
	for (int i = 0; i < m_aSelectedKeys.GetSize(); i++)
		aSelKey.Add(m_aSelectedKeys[i]);
}

CString CCmdLcomSP20ReliabilityGrid::GetKeyForRow(ROWCOL ncRow, ROWCOL nkCol)
{
	CString sKey;

	if (IsAppendRow(ncRow)) sKey = _LS(IDS_WG_CMD__ADDD__Append_Row);
	else if (m_nEditMode == addnew) sKey = _LS(IDS_WG_CMD__ADDD__New_Row);
	else if (m_nEditMode == edit) sKey = GetValueRowColBeforeEdit(ncRow, nkCol);
	else sKey = GetValueRowCol(ncRow, nkCol);

	return sKey;
}

BOOL CCmdLcomSP20ReliabilityGrid::IsActiveCol(ROWCOL& nCol)
{
	ROWCOL nRow, nACol;

	GetCurrentCell(nRow, nCol);
	VERIFY(GetColumnNoFor(5, nACol));

	return nCol==nACol;
}

void CCmdLcomSP20ReliabilityGrid::SetHeaderTitle(T_UNIT_SYSTEM &UnitSystem, BOOL bInit)
{
	SetHeaderTitle2((int*)m_aUnit.GetData(), bInit, TRUE);
}

BOOL CCmdLcomSP20ReliabilityGrid::GetStyleRowCol(ROWCOL nRow, ROWCOL nCol, CGXStyle& style, GXModifyType mt, int nType)
{
	CTBCommon::GetStyleRowCol(nRow, nCol, style, mt, nType);

	if(IsAppendRow(nRow))
	{
		//if(nCol==5 || nCol==6 || nCol==7)
		//{
		//	style.SetEnabled(FALSE).SetInterior(m_Color.DisabledBg);
		//}
	}

	return TRUE;
}

CString CCmdLcomSP20ReliabilityGrid::GetLoadNameList()
{
	CString strStldList = _T("");
	for (int i = 0; i < m_paLcomRcfiFactor->GetSize(); i++)
	{
		strStldList += m_paLcomRcfiFactor->GetAt(i).LoadCaseName;
		strStldList += _T("\n");
	}

	return strStldList;
}

CString CCmdLcomSP20ReliabilityGrid::GetLoadTypeList()
{
	CString str[] = { _T("Constant"),_T("Long-term"),_T("Short-term"),_T("Seismic"),_T("Crane-vertical"),_T("Crane-brake"),_T("Special"),_T("Inactive") };
	CString strType;
	for (int i = 0; i < e_SP20_Inactive; i++)
	{
		strType += str[i];
		strType += _T("\n");
	}
	return strType;
}

BOOL CCmdLcomSP20ReliabilityGrid::OnPasteFromClipboard(const CGXRange &range)
{
	AfxMessageBox(_LS(IDS_CMD_DONT_SUPPORT_PASTE));
	return FALSE;
}

BOOL CCmdLcomSP20ReliabilityGrid::OnInsertEmptyRecord(ROWCOL nRow)
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
		SetExpressionRowCol(nRow, 4, StldD.LoadCaseType);

		if(StldD.LoadCaseType==_T("L") || StldD.LoadCaseType==_T("LR"))
		{
			SetExpressionRowCol(nRow, 5, _T("0.7"));
			SetExpressionRowCol(nRow, 6, _T("0.5"));
			SetExpressionRowCol(nRow, 7, _T("0.3"));
		}
		else if(StldD.LoadCaseType==_T("W"))
		{
			SetExpressionRowCol(nRow, 5, _T("0.6"));
			SetExpressionRowCol(nRow, 6, _T("0.2"));
			SetExpressionRowCol(nRow, 7, _T("0.0"));
		}
		else if(StldD.LoadCaseType==_T("S"))
		{
			SetExpressionRowCol(nRow, 5, _T("0.7"));
			SetExpressionRowCol(nRow, 6, _T("0.5"));
			SetExpressionRowCol(nRow, 7, _T("0.2"));
		}
		else if(StldD.LoadCaseType==_T("T"))
		{
			SetExpressionRowCol(nRow, 5, _T("0.6"));
			SetExpressionRowCol(nRow, 6, _T("0.5"));
			SetExpressionRowCol(nRow, 7, _T("0.0"));
		}
		else ASSERT(0);

		SetExpressionRowCol(nRow, 8, _T("0.2"));
		SetExpressionRowCol(nRow, 9, _T("0.2"));
	}

	return bResult;
}

void CCmdLcomSP20ReliabilityGrid::OnModifyCell(ROWCOL nRow, ROWCOL nCol)
{
	T_SP20_RCFI_D PreData;
	if(nCol == 4)
	{
		PreData.LoadCaseType = CGXGridWnd::GetValueRowCol(nRow, nCol);
		PreData.SetEnumLcaseType();
	}
	CTBCommon::OnModifyCell(nRow, nCol);

	if(nCol == 4)
	{
		DGN_LCASE_K key;
		T_SP20_RCFI_D data;
		GetValue(nRow, key, data);
		if(PreData.LoadCaseType != data.LoadCaseType)
		{
			CLoadCombSP20Ctrl::Instance()->GetDefaultRfiParam(data);
			//DeleteRecord(nRow);
			//InsertRecord(nRow);
			SetValue(nRow, key, data);
		}
	}
}

BOOL CCmdLcomSP20ReliabilityGrid::OnStartEditing(ROWCOL nRow, ROWCOL nCol)
{
	BOOL bRet = CTBCommon::OnStartEditing(nRow, nCol);
	if(nCol == 10)
	{
		CString str1 = GetValueRowCol(nRow, 10);
		CString str2 = GetValueRowCol(nRow, 11);
		if(str1 != str2 && str1 == _T("0"))
		{
			SetDataRowCol(nRow, 11, _T("0"));
		}
	}
	else if(nCol == 11)
	{
		CString str1 = GetValueRowCol(nRow, 10);
		CString str2 = GetValueRowCol(nRow, 11);
		if(str1 != str2 && str2 == _T("0"))
		{
			SetDataRowCol(nRow, 10, _T("0"));
		}
	}
	return bRet;
}
