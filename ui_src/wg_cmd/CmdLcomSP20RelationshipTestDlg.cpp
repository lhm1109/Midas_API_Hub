#include "stdafx.h"
#include "wg_cmd.h"
#include "CmdLcomSP20RelationshipTestDlg.h"
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


CCmdLcomSP20TestPathDlg::CCmdLcomSP20TestPathDlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CCmdLcomSP20TestPathDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCmdLcomSP20TestPathDlg)
	//}}AFX_DATA_INIT
	m_pTestGrid = new CCmdLcomSP20TestGrid;
}

CCmdLcomSP20TestPathDlg::~CCmdLcomSP20TestPathDlg()
{
	if (m_pTestGrid) delete m_pTestGrid;
}

void CCmdLcomSP20TestPathDlg::SetData(const CArray<T_SP20_LINK_D, T_SP20_LINK_D&>& aLcaseSP20RcfiFactor)
{
	m_nTolRow = aLcaseSP20RcfiFactor.GetSize();
	m_aLcaseSP20RcfiFactor.Copy(aLcaseSP20RcfiFactor);
	CLoadCombSP20Ctrl::Instance()->GetIncidenceMatrixTable(aLcaseSP20RcfiFactor, m_aGraphIndex);
}

void CCmdLcomSP20TestPathDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCmdLcomSP20TestPathDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CCmdLcomSP20TestPathDlg, CDialogMove)
	//{{AFX_MSG_MAP(CCmdLcomSP20TestPathDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// CCmdLcomSP20TestPathDlg message handlers

BOOL CCmdLcomSP20TestPathDlg::OnInitDialog() 
{
	CDialogMove::OnInitDialog();

	CRect rect;
	GetWindowRect(&rect);
	int nAugmentWidth = 0;
	int nAugmentHeight = 400;
	if (m_aLcaseSP20RcfiFactor.GetSize() > 10)
	{
		nAugmentWidth = (m_aLcaseSP20RcfiFactor.GetSize() - 10) * 20;
	}

	int newWidth = rect.Width() + nAugmentWidth;
	int newHeight = rect.Height() + nAugmentHeight;
	SetWindowPos(NULL, 0, 0, newWidth, newHeight, SWP_NOMOVE | SWP_NOZORDER);

	GetDlgItem(IDC_CMD_LCOM_PSI_FACTOR_GRID)->GetWindowRect(&rect);
	newWidth = rect.Width() + nAugmentWidth;
	newHeight = rect.Height() + nAugmentHeight;
	GetDlgItem(IDC_CMD_LCOM_PSI_FACTOR_GRID)->SetWindowPos(NULL, 0, 0, newWidth, newHeight, SWP_NOMOVE | SWP_NOZORDER);

	GetDlgItem(IDC_WG_CMD_STATIC0)->GetWindowRect(&rect);
	newWidth = rect.Width() + nAugmentWidth;
	newHeight = rect.Height() + nAugmentHeight;
	GetDlgItem(IDC_WG_CMD_STATIC0)->SetWindowPos(NULL, 0, 0, newWidth, newHeight, SWP_NOMOVE | SWP_NOZORDER);

	CArray<UINT, UINT> aMoveCtrl;
	aMoveCtrl.Add(IDOK);
	aMoveCtrl.Add(IDCANCEL);
	CDlgUtil::CtrlMoveDistXY(this, aMoveCtrl, nAugmentWidth, nAugmentHeight);

	m_pTestGrid->SubclassDlgItem(IDC_CMD_LCOM_PSI_FACTOR_GRID, this);
	T_UNIT_SYSTEM UnitSystem;
	m_pTestGrid->SetData(m_aLcaseSP20RcfiFactor ,&m_aGraphIndex);
	m_pTestGrid->Initialize(UnitSystem);

	return TRUE;
}

void CCmdLcomSP20TestPathDlg::OnOK() 
{
	m_pTestGrid->OnTerminate(D_TB_SAVE_ALWAYS);

	CDialogMove::OnOK();
}

void CCmdLcomSP20TestPathDlg::OnCancel() 
{
	CDialogMove::OnCancel();
}


#define NAME_COL_ID         2
#define LCOM_KEY_FMT        _T("%d")


CCmdLcomSP20TestGrid::CCmdLcomSP20TestGrid()
{
}

CCmdLcomSP20TestGrid::~CCmdLcomSP20TestGrid()
{
}

#define CWnd CTBCommon
BEGIN_MESSAGE_MAP(CCmdLcomSP20TestGrid, CWnd)
#undef CWnd
	//{{AFX_MSG_MAP(CCmdLcomSP20TestGrid)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


// CCmdLcomSP20TestGrid message handlers
void CCmdLcomSP20TestGrid::Initialize(T_UNIT_SYSTEM &UnitSystem)
{
	CTBCommon::Initialize(CDBDoc::GetDocPoint(), UnitSystem);

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
			.SetHorizontalAlignment(DT_CENTER)
			.SetValueType(GX_VT_NUMERIC)
			.SetFormat(GX_FMT_FIXED)
			.SetPlaces(0)
			.SetEnabled(FALSE));


	SetStyleRange(CGXRange().SetCols(3,GetColCount()), CGXStyle().SetHorizontalAlignment(DT_CENTER).SetEnabled(FALSE));

	CGXProperties* pProp = GetParam()->GetProperties();
			pProp->SetUserProperty(GX_IDS_OUTLINECURRENTCELL,
		      (CGXStyle) pProp->sInvertNoBorder);

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
	SetColWidth(3, GetColCount(), 25);
	
	MakeItemEx();
	LockUpdate(FALSE);
}

void CCmdLcomSP20TestGrid::SetData(const CArray<T_SP20_LINK_D, T_SP20_LINK_D &>& aLinkData, CArrayEx<CArrayEx<T_KEY, T_KEY>, CArrayEx<T_KEY, T_KEY> &>* pGraphIndex)
{
	m_pGraphIndex = pGraphIndex;

	AddColInfo(_ULS(Key), t_integer, 0);
	AddColInfo(_LS(IDS_WG_CMD__ADDD__No), t_integer, 0);
	CString str;
	for (int i = 0; i < aLinkData.GetSize(); i++)
	{
		str.Format(_T("%d"), i + 1);
		AddColInfo(str, t_string, 0);
	}
	m_aPrimaryKey.Add(0);
}

// CCmdLcomSP20TestGrid Overridable Functions
void CCmdLcomSP20TestGrid::OnChangedSelectionPublic(long *keys, int nSize)
{
	//if (nSize <= 0)
	//{
	//	m_aSelectedKeys.RemoveAll();
	//}
	//else
	//{
	//	m_aSelectedKeys.RemoveAll();
	//	m_aSelectedKeys.SetSize(nSize);
	//	for (int i = 0; i < nSize; i++)
	//	{
	//		m_aSelectedKeys[i].keymap = keys[i];
	//	}
	//}
}

long CCmdLcomSP20TestGrid::GetSelectedKeys(CArray<long, long> &caKey)
{
	// override this function to return selected record count
	long nKeyCount;
	CArray<DGN_LCASE_K, DGN_LCASE_K > aSelKey;

	GetAllSelectedLcom(aSelKey);
	nKeyCount = aSelKey.GetSize();
	caKey.SetSize(nKeyCount);
	for (long i = 0; i < nKeyCount; i++)
		caKey[i] = (long)aSelKey[i].keymap;
	
	return nKeyCount;
}

BOOL CCmdLcomSP20TestGrid::ValidateField(CString value, int nColID)
{
	if (nColID >= GetColCount())
	{
		SetWarningText(_LS(IDS_WG_CMD__ADDD___CCMLoadCombMainGrid__ValidateFie));
		return FALSE;
	}

	return TRUE;
}

BOOL CCmdLcomSP20TestGrid::ValidateRecord(CStringArray &value, CRowColArray &aCols)
{
	CString msg = _T("");
	int nPos;

	// serial no, Description, Active, Elastic 검사 안한다.
	for (int i = 2; i < GetColCount()-3; i++)
	{
		nPos = aCols[i]-1;
		if (value[nPos] == _T(""))
			msg += ((msg != _T("")) ? _T("\n") : _T("")) + m_aTitle[i] + _LS(IDS_WG_CMD__ADDD___has_not_been_entered_);
	}
	if (msg == _T("")) return TRUE;
	else SetWarningText(msg);
	return FALSE;
}

BOOL CCmdLcomSP20TestGrid::ValidateAndMakeRecord(CStringArray &value, CRowColArray &aCols, DGN_LCASE_K& key, T_SP20_LPSI_D &data)
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

	key = data.LCaseKey;

	return TRUE;
}

BOOL CCmdLcomSP20TestGrid::AddToDB(ROWCOL nRow)
{
	ASSERT(FALSE);
	return TRUE;
}

BOOL CCmdLcomSP20TestGrid::AddToDB(CArray<void*, void*>& raKey, CArray<void*, void*>& raData)
{
	ASSERT(0);
	return TRUE;
}

BOOL CCmdLcomSP20TestGrid::DeleteFromDB(ROWCOL nRow)
{
	//DGN_LCASE_K  key;
	//T_SP20_LPSI_D data;

	//GetValue(nRow, key, data);

	//T_SP20_LPSI_D TempD;
	////if (!m_paLcaseSP20PsiFactor->Lookup(key.keymap, TempD)) return FALSE;
	//m_paLcaseSP20PsiFactor->RemoveKey(key.keymap);

	//int nIndexCount = m_mIndex.GetCount();
	//for (int i = 1; i <= nIndexCount; i++)
	//{
	//	if (i <= nRow) continue;

	//	if (!m_mIndex.Lookup(i, key)) { ASSERT(0); continue; }
	//	m_mIndex.SetAt(i - 1, key);
	//}
	//m_mIndex.RemoveKey(nIndexCount);

	//DeleteRecord(nRow);

	//CString str;
	//int nSize = m_paLcaseSP20PsiFactor->GetCount();
	//for (int i = 0; i < nSize; i++)
	//{
	//	str.Format(_T("%d"), i + 1);
	//	SetExpressionRowCol(i + 1, 2, str);
	//}

	return TRUE;
}

BOOL CCmdLcomSP20TestGrid::DeleteFromDB(CRowColArray& awRows)
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

BOOL CCmdLcomSP20TestGrid::ModifyToDB(ROWCOL nRow)
{
	ROWCOL nCol;
	VERIFY(GetColumnNoFor(NAME_COL_ID, nCol));  // load case name

	//DGN_LCASE_K key;
	//T_SP20_LPSI_D data;
	//GetValue(nRow, key, data);

	//DGN_LCASE_K OldKey; OldKey.keymap = 0;
	//m_mIndex.Lookup(nRow, OldKey);

	//T_SP20_LPSI_D TempD;
	//if (OldKey.keymap != key.keymap && m_paLcaseSP20PsiFactor->Lookup(key.keymap, TempD))
	//{
	//	AfxMessageBox(_LS(IDS_CMD_LPSI_EXIST_LOADCASE));
	//	return FALSE;
	//}
	//
	//if (data.dPsi1 < 0. || data.dPsi2 < 0. || data.dPsi3 < 0. || data.dPsiSe < 0. || data.dPsiO1 < 0. || data.dPsiO2 < 0. || data.dPsiO3 < 0.)
	//{
	//	AfxMessageBox(_LS(IDS_CMD_SP20_PSI_FACTOR_SCOPE_ERROR));
	//	return FALSE;
	//}

	//m_paLcaseSP20PsiFactor->SetAt(key.keymap, data);
	//
	//if (OldKey.keymap != 0 && OldKey.keymap != key.keymap)
	//{
	//	m_paLcaseSP20PsiFactor->RemoveKey(OldKey.keymap);
	//}
	//m_mIndex.SetAt(nRow, key);

	////////////////////////////////////////////////////
	//DGN_LCASE_K TempKey;
	//int nIndexCount = m_mIndex.GetCount();
	//for(int i=1; i<=nIndexCount; i++)
	//{
	//	if(i<nRow) continue;

	//	m_mIndex.Lookup(i, TempKey);
	//	m_paLcaseSP20PsiFactor->Lookup(TempKey.keymap, TempD);

	//	m_paLcaseSP20PsiFactor->RemoveKey(TempKey.keymap);
	//	m_paLcaseSP20PsiFactor->SetAt(TempKey.keymap, TempD);
	//}
	////////////////////////////////////////////////////

	//DeleteRecord(nRow);
	//InsertRecord(nRow);
	//SetValue(nRow, key, data);

	return TRUE;
}

BOOL CCmdLcomSP20TestGrid::ModifyToDB(CStringArray& raOldKey, CArray<void*, void*>& raKey, CArray<void*, void*>& raData)
{
	ASSERT(0);
	return TRUE;
}

CString CCmdLcomSP20TestGrid::GetNewKey()
{
	return _T("");
}

CString CCmdLcomSP20TestGrid::GetDefValue(int nIndex)
{
	return _T("");
}

CString* CCmdLcomSP20TestGrid::GetDefValueArray()
{
	return NULL;
}

BOOL CCmdLcomSP20TestGrid::MakeKeyAndDataArray(CStringArray& value, CRowColArray& awCols, CArray<void*, void*>& aKey, CArray<void*, void*>& aData)
{
	//T_STLD_K* pKey;
	//T_SP20_LPSI_D* pData;
	//pKey = new T_STLD_K;
	//pData = new T_SP20_LPSI_D;
	//aKey.Add((void*)pKey);
	//aData.Add((void*)pData);
	//if (!ValidateAndMakeRecord(value, awCols, *pKey, *pData)) return FALSE;
	ASSERT(FALSE);
	return TRUE;
}

void CCmdLcomSP20TestGrid::DeleteKeyAndDataArray(CArray<void*, void*>& aKey, CArray<void*, void*>& aData)
{
	T_STLD_K* pKey;
	T_SP20_LPSI_D* pData;
	ASSERT(aKey.GetSize() == aData.GetSize());
	for (int i = 0; i < aKey.GetSize(); i++)
	{
		pKey = (T_STLD_K*)aKey.GetAt(i);
		pData = (T_SP20_LPSI_D*)aData.GetAt(i);
		delete pKey;
		delete pData;
	}
}

long CCmdLcomSP20TestGrid::GetCountData()
{
	return m_pGraphIndex->GetSize();
}

void CCmdLcomSP20TestGrid::MakeItemFiltered(long nFilteredNum, long* aFilteredKeys, CRowColArray& awCols)
{
	//CDBDoc* pDoc = m_pDoc;
	//T_SP20_LPSI_D Data;
	//DGN_LCASE_K key;
	//for(long nCount=0 ; nCount < nFilteredNum; nCount++)
 //	{
	//	m_paLcaseSP20PsiFactor->Lookup(aFilteredKeys[nCount], Data);
	//	key.keymap = aFilteredKeys[nCount];
	//	SetValue(GetParam()->GetData(), nCount + 1, key, Data, awCols);
	//}
}

BOOL ContainsIndex(IN int index, IN const CArrayEx<T_KEY, T_KEY>& aIndex)
{
	for (int i = 0; i < aIndex.GetSize(); i++)
	{
		if (aIndex[i] == index)
			return TRUE;
	}
	return FALSE;
}

void CCmdLcomSP20TestGrid::MakeItemNormal(long nNumData, CRowColArray& awCols)
{
	CString str;
	for (long nCount = 0; nCount < nNumData; nCount++)
	{
		const CArrayEx<T_KEY, T_KEY>& path = m_pGraphIndex->GetAt(nCount);
		str.Format(_T("%d"), nCount + 1);
		GetParam()->GetData()->StoreValueRowCol(nCount + 1, awCols[1], str, gxOverride);
		for (int i = 2; i < GetColCount(); i++)
		{
			str = _T("0");
			if (ContainsIndex(i - 1, path))
				str = _T("1");
			GetParam()->GetData()->StoreValueRowCol(nCount + 1, awCols[i], str, gxOverride);
		}
	}
}

void CCmdLcomSP20TestGrid::MakeItemEditingRecord(ROWCOL ncRow, CStringArray& EditingRecord)
{
	ASSERT(FALSE);
	//CDBDoc* pDoc = m_pDoc;
	//DGN_LCASE_K key;
	//T_SP20_LPSI_D Data;
	//for (int i = 0; i < GetColCount(); i++)
	//{
	//	if (i == 0) key.keymap = _tstol(EditingRecord[i]);
	//	else ConvStrToData(i, EditingRecord[i], Data);
	//}
	//SetValue(ncRow, key, Data);
}


void CCmdLcomSP20TestGrid::ConvStrToData(int i, CString& value, T_SP20_LPSI_D& data)
{
	if (i==2)
	{
		//data.StldKey = m_pDoc->m_pAttrCtrl->GetStldKey(value);
		data.LoadCaseName = value; // name
	}
	else if (i==3) data.LoadCaseType = value; // type
	else if (i==4) data.dPsi1 = _tstof(value);
	else if (i==5) data.dPsi2 = _tstof(value);
	else if (i==6) data.dPsi3 = _tstof(value);
	else if (i == 7) data.dPsiSe = _tstof(value);
	else if (i == 8) data.dPsiO1 = _tstof(value);
	else if (i == 9) data.dPsiO2 = _tstof(value);
	else if (i == 10) data.dPsiO3 = _tstof(value);
}

void CCmdLcomSP20TestGrid::ConvDataToStr(int i, const T_SP20_LPSI_D& data, CString& value)
{
	if (i==2) value = data.LoadCaseName;
	else if (i==3) value = data.LoadCaseType;
	else if (i==4) value.Format(_T("%f"), data.dPsi1);
	else if (i==5) value.Format(_T("%f"), data.dPsi2);
	else if (i==6) value.Format(_T("%f"), data.dPsi3);
	else if (i == 7) value.Format(_T("%f"), data.dPsiSe);
	else if (i == 8) value.Format(_T("%f"), data.dPsiO1);
	else if (i == 9) value.Format(_T("%f"), data.dPsiO2);
	else if (i == 10) value.Format(_T("%f"), data.dPsiO3);
}

void CCmdLcomSP20TestGrid::MakeSearchKey(const DGN_LCASE_K&key, const T_SP20_LPSI_D &data, CStringArray &aKey)
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

void CCmdLcomSP20TestGrid::GetAllSelectedLcom(CArray<DGN_LCASE_K, DGN_LCASE_K >& aSelKey)
{
	//for (int i = 0; i < m_aSelectedKeys.GetSize(); i++)
	//	aSelKey.Add(m_aSelectedKeys[i]);
}

CString CCmdLcomSP20TestGrid::GetKeyForRow(ROWCOL ncRow, ROWCOL nkCol)
{
	CString sKey;

	if (IsAppendRow(ncRow)) sKey = _LS(IDS_WG_CMD__ADDD__Append_Row);
	else if (m_nEditMode == addnew) sKey = _LS(IDS_WG_CMD__ADDD__New_Row);
	else if (m_nEditMode == edit) sKey = GetValueRowColBeforeEdit(ncRow, nkCol);
	else sKey = GetValueRowCol(ncRow, nkCol);

	return sKey;
}

BOOL CCmdLcomSP20TestGrid::IsActiveCol(ROWCOL& nCol)
{
	ROWCOL nRow, nACol;

	GetCurrentCell(nRow, nCol);
	VERIFY(GetColumnNoFor(5, nACol));

	return nCol==nACol;
}

void CCmdLcomSP20TestGrid::SetHeaderTitle(T_UNIT_SYSTEM &UnitSystem, BOOL bInit)
{
	SetHeaderTitle2((int*)m_aUnit.GetData(), bInit, TRUE);
}

BOOL CCmdLcomSP20TestGrid::GetStyleRowCol(ROWCOL nRow, ROWCOL nCol, CGXStyle& style, GXModifyType mt, int nType)
{
	CTBCommon::GetStyleRowCol(nRow, nCol, style, mt, nType);

	if(IsAppendRow(nRow))
	{
		if(nCol > 4)
		{
			style.SetEnabled(FALSE).SetInterior(m_Color.DisabledBg);
		}
	}

	return TRUE;
}

BOOL CCmdLcomSP20TestGrid::OnPasteFromClipboard(const CGXRange &range)
{
	AfxMessageBox(_LS(IDS_CMD_DONT_SUPPORT_PASTE));
	return FALSE;
}

BOOL CCmdLcomSP20TestGrid::OnInsertEmptyRecord(ROWCOL nRow)
{
	BOOL bResult = CTBCommon::OnInsertEmptyRecord(nRow);

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
		SetExpressionRowCol(nRow, 8, _T("0.3"));
		SetExpressionRowCol(nRow, 9, _T("0.3"));
		SetExpressionRowCol(nRow, 10, _T("0.3"));
		SetExpressionRowCol(nRow, 11, _T("0.3"));
	}

	return bResult;
}

void CCmdLcomSP20TestGrid::OnModifyCell(ROWCOL nRow, ROWCOL nCol)
{
	CTBCommon::OnModifyCell(nRow, nCol);

	if(nCol == 3)
	{
		CGXControl* pControl = (CGXControl*)GetControl(nRow, nCol);
		CString strLoadCaseName=_T("");
		pControl->GetValue(strLoadCaseName);
		T_STLD_D StldD;
		m_pDoc->m_pAttrCtrl->GetStld(strLoadCaseName, StldD);
		SetExpressionRowCol(nRow, 4, StldD.LoadCaseType);
	}
}
