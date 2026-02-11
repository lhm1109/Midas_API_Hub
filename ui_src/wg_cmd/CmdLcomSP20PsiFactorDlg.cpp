#include "stdafx.h"
#include "wg_cmd.h"
#include "CmdLcomSP20PsiFactorDlg.h"
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


CCmdLcomSP20PsiFactorDlg::CCmdLcomSP20PsiFactorDlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CCmdLcomSP20PsiFactorDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCmdLcomSP20PsiFactorDlg)
	//}}AFX_DATA_INIT
	m_pLcomPsiFactorGrid = new CCmdLcomSP20PsiFactorGrid;
	m_pDoc = NULL;
}

CCmdLcomSP20PsiFactorDlg::~CCmdLcomSP20PsiFactorDlg()
{
	if (m_pLcomPsiFactorGrid) delete m_pLcomPsiFactorGrid;
}

void CCmdLcomSP20PsiFactorDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCmdLcomSP20PsiFactorDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CCmdLcomSP20PsiFactorDlg, CDialogMove)
	//{{AFX_MSG_MAP(CCmdLcomSP20PsiFactorDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// CCmdLcomSP20PsiFactorDlg message handlers

BOOL CCmdLcomSP20PsiFactorDlg::OnInitDialog() 
{
	m_pDoc = CDBDoc::GetDocPoint();

	CDialogMove::OnInitDialog();

	CRect rect;
	GetWindowRect(&rect);
	int newWidth = rect.Width() + globalUtils.ScaleByDPI(300);
	SetWindowPos(NULL, 0, 0, newWidth, rect.Height(), SWP_NOMOVE | SWP_NOZORDER);

	GetDlgItem(IDC_CMD_LCOM_PSI_FACTOR_GRID)->GetWindowRect(&rect);
	newWidth = rect.Width() + globalUtils.ScaleByDPI(300);
	GetDlgItem(IDC_CMD_LCOM_PSI_FACTOR_GRID)->SetWindowPos(NULL, 0, 0, newWidth, rect.Height(), SWP_NOMOVE | SWP_NOZORDER);

	GetDlgItem(IDC_WG_CMD_STATIC0)->GetWindowRect(&rect);
	newWidth = rect.Width() + globalUtils.ScaleByDPI(300);
	GetDlgItem(IDC_WG_CMD_STATIC0)->SetWindowPos(NULL, 0, 0, newWidth, rect.Height(), SWP_NOMOVE | SWP_NOZORDER);

	CArray<UINT, UINT> aMoveCtrl;
	aMoveCtrl.Add(IDOK);
	aMoveCtrl.Add(IDCANCEL);
	CDlgUtil::CtrlMoveDistX(this, aMoveCtrl, globalUtils.ScaleByDPI(300));

	m_pLcomPsiFactorGrid->SubclassDlgItem(IDC_CMD_LCOM_PSI_FACTOR_GRID, this);
	CLoadCombSP20Ctrl::Instance()->GeneratePsiParam(&m_aLcaseSP20PsiFactor);
	m_pLcomPsiFactorGrid->SetData(&m_aLcaseSP20PsiFactor);
	T_UNIT_SYSTEM UnitSystem;
	m_pLcomPsiFactorGrid->Initialize(m_pDoc, UnitSystem);

	return TRUE;
}

void CCmdLcomSP20PsiFactorDlg::OnOK() 
{
	m_pLcomPsiFactorGrid->OnTerminate(D_TB_SAVE_ALWAYS);
	CLoadCombSP20Ctrl::Instance()->SetPsiParam(m_aLcaseSP20PsiFactor);
	CDialogMove::OnOK();
}

void CCmdLcomSP20PsiFactorDlg::OnCancel() 
{
	CDialogMove::OnCancel();
}


#define NAME_COL_ID         2
#define LCOM_KEY_FMT        _T("%d")

CString CCmdLcomSP20PsiFactorGrid::m_aCurDefVal[] = {_T(""), _T(""), _T(""), _T(""), _T(""), _T(""), _T(""), _T(""), _T(""), _T("") };

CCmdLcomSP20PsiFactorGrid::CCmdLcomSP20PsiFactorGrid()
{
	AddColInfo(_ULS(Key), t_integer, 0);
	AddColInfo(_LS(IDS_WG_CMD__ADDD__No),		t_integer,	0);
	AddColInfo(_STR(_T("Load Case")),			t_string,	0);
	AddColInfo(_LS(IDS_WG_CMD__ADDD__Type),		t_string,	0);
	AddColInfo(_LSX(Psi1),						t_real,		0);
	AddColInfo(_LSX(Psi2),						t_real,		0);
	AddColInfo(_LSX(Psi3),						t_real,		0);
	AddColInfo(_LSX(PsiSe),						t_real,		0);
	AddColInfo(_LSX(PsiO1),						t_real,		0);
	AddColInfo(_LSX(PsiO2),						t_real,		0);
	AddColInfo(_LSX(PsiO3),						t_real,		0);
	m_aPrimaryKey.Add(0);
}

CCmdLcomSP20PsiFactorGrid::~CCmdLcomSP20PsiFactorGrid()
{
}

#define CWnd CTBCommon
BEGIN_MESSAGE_MAP(CCmdLcomSP20PsiFactorGrid, CWnd)
#undef CWnd
	//{{AFX_MSG_MAP(CCmdLcomSP20PsiFactorGrid)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


// CCmdLcomSP20PsiFactorGrid message handlers
void CCmdLcomSP20PsiFactorGrid::Initialize(CDBDoc* pDoc, T_UNIT_SYSTEM &UnitSystem)
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

	SetStyleRange(CGXRange().SetCols(4), CGXStyle().SetEnabled(FALSE));

	SetStyleRange(CGXRange().SetCols(5,11), CGXStyle()
			.SetHorizontalAlignment(DT_RIGHT)
			.SetValueType(GX_VT_NUMERIC)
			.SetFormat(GX_FMT_FIXED)
			.SetPlaces(4));

	CGXProperties* pProp = GetParam()->GetProperties();
			pProp->SetUserProperty(GX_IDS_OUTLINECURRENTCELL,
		      (CGXStyle) pProp->sInvertNoBorder);

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

	GetParam()->EnableMoveRows(FALSE);

	EnableInsertRowByPressingKey(FALSE);
	EnableAppendRow(FALSE);

	
	SetColWidth(1, 1, 0);
	SetColWidth(2, 2, 40);
	SetColWidth(3, 3, 100);
	SetColWidth(4, 4, 90);
	SetColWidth(5, 11, 55);
	
	MakeItemEx();
	LockUpdate(FALSE);
}

// CCmdLcomSP20PsiFactorGrid Overridable Functions
void CCmdLcomSP20PsiFactorGrid::OnChangedSelectionPublic(long *keys, int nSize)
{
	if (nSize <= 0)
	{
		m_aSelectedKeys.RemoveAll();
	}
	else
	{
		m_aSelectedKeys.RemoveAll();
		m_aSelectedKeys.SetSize(nSize);
		for (int i = 0; i < nSize; i++)
		{
			m_aSelectedKeys[i].keymap = keys[i];
		}
	}
}

long CCmdLcomSP20PsiFactorGrid::GetSelectedKeys(CArray<long, long> &caKey)
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

BOOL CCmdLcomSP20PsiFactorGrid::ValidateField(CString value, int nColID)
{
	if (nColID >= GetColCount())
	{
		SetWarningText(_LS(IDS_WG_CMD__ADDD___CCMLoadCombMainGrid__ValidateFie));
		return FALSE;
	}

	return TRUE;
}

BOOL CCmdLcomSP20PsiFactorGrid::ValidateRecord(CStringArray &value, CRowColArray &aCols)
{
	return TRUE;
}

BOOL CCmdLcomSP20PsiFactorGrid::ValidateAndMakeRecord(CStringArray &value, CRowColArray &aCols, DGN_LCASE_K& key, T_SP20_LPSI_D &data)
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

BOOL CCmdLcomSP20PsiFactorGrid::AddToDB(ROWCOL nRow)
{
	DGN_LCASE_K  key;
	T_SP20_LPSI_D data;
 
	GetValue(nRow, key, data);

	//T_SP20_LPSI_D TempD;
	//if (m_paLcaseSP20PsiFactor->Lookup(key.keymap, TempD))
	//{
	//	AfxMessageBox(_LS(IDS_CMD_LPSI_EXIST_LOADCASE));
	//	return FALSE;
	//}

	if (data.dPsi1 < 0. || data.dPsi2 < 0. || data.dPsi3 < 0. || data.dPsiSe < 0. || data.dPsiO1 < 0. || data.dPsiO2 < 0. || data.dPsiO3 < 0.)
	{
		AfxMessageBox(_LS(IDS_CMD_SP20_PSI_FACTOR_SCOPE_ERROR));
		return FALSE;
	}

	//m_paLcaseSP20PsiFactor ->SetAt(key.keymap, data);
	//m_mIndex.SetAt(nRow, key);
	InsertRecord(nRow);
	SetValue(nRow, key, data);

	return TRUE;
}

BOOL CCmdLcomSP20PsiFactorGrid::AddToDB(CArray<void*, void*>& raKey, CArray<void*, void*>& raData)
{
	ASSERT(0);
	return TRUE;
}

BOOL CCmdLcomSP20PsiFactorGrid::DeleteFromDB(ROWCOL nRow)
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

BOOL CCmdLcomSP20PsiFactorGrid::DeleteFromDB(CRowColArray& awRows)
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

BOOL CCmdLcomSP20PsiFactorGrid::ModifyToDB(ROWCOL nRow)
{
	ROWCOL nCol;
	VERIFY(GetColumnNoFor(NAME_COL_ID, nCol));  // load case name

	DGN_LCASE_K key;
	T_SP20_LPSI_D data;
	data = m_paLcaseSP20PsiFactor->GetAt(nRow - 1);
	GetValue(nRow, key, data);
	
	//if (m_paLcaseSP20PsiFactor->exi
	//{
	//	AfxMessageBox(_LS(IDS_CMD_LPSI_EXIST_LOADCASE));
	//	return FALSE;
	//}
	bool error = false;
	switch (data.eLoadCaseType)
	{
	case e_SP20_Constant:
		if (data.dPsi1 < 0. || data.dPsiSe < 0. || data.dPsiO1 < 0. || data.dPsiO2 < 0. || data.dPsiO3 < 0.)
			error = true;
		break;
	case e_SP20_Long_term:
	case e_SP20_Short_term:
		if (data.dPsi1 < 0. || data.dPsi2 < 0. || data.dPsi3 < 0. || data.dPsiSe < 0. || data.dPsiO1 < 0. || data.dPsiO2 < 0. || data.dPsiO3 < 0.)
			error = true;
		break;
	case e_SP20_Seismic:
		if (data.dPsiSe < 0.)
			error = true;
		break;
	case e_SP20_Crane_brake:
	case e_SP20_Crane_vertical:
		if (data.dPsi1 < 0. || data.dPsi2 < 0. || data.dPsi3 < 0.)
			error = true;
		break;
	case e_SP20_Special:
		if (data.dPsiO1 < 0.)
			error = true;
		break;
	default:
		break;
	}
	if (error)
	{
		AfxMessageBox(_LS(IDS_CMD_SP20_PSI_FACTOR_SCOPE_ERROR));
		return FALSE;
	}

	m_paLcaseSP20PsiFactor->SetAt(nRow - 1, data);

	return TRUE;
}

BOOL CCmdLcomSP20PsiFactorGrid::ModifyToDB(CStringArray& raOldKey, CArray<void*, void*>& raKey, CArray<void*, void*>& raData)
{
	ASSERT(0);
	return TRUE;
}

CString CCmdLcomSP20PsiFactorGrid::GetNewKey()
{
	return _T("");
}

CString CCmdLcomSP20PsiFactorGrid::GetDefValue(int nIndex)
{
	return m_aCurDefVal[nIndex];
}

CString* CCmdLcomSP20PsiFactorGrid::GetDefValueArray()
{
	return m_aCurDefVal;
}

BOOL CCmdLcomSP20PsiFactorGrid::MakeKeyAndDataArray(CStringArray& value, CRowColArray& awCols, CArray<void*, void*>& aKey, CArray<void*, void*>& aData)
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

void CCmdLcomSP20PsiFactorGrid::DeleteKeyAndDataArray(CArray<void*, void*>& aKey, CArray<void*, void*>& aData)
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

long CCmdLcomSP20PsiFactorGrid::GetCountData()
{
	return m_paLcaseSP20PsiFactor->GetCount();
}

void CCmdLcomSP20PsiFactorGrid::MakeItemFiltered(long nFilteredNum, long* aFilteredKeys, CRowColArray& awCols)
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

void CCmdLcomSP20PsiFactorGrid::MakeItemNormal(long nNumData, CRowColArray& awCols)
{
	for (long nCount = 0; nCount < nNumData; nCount++)
	{
		//if (!m_mIndex.Lookup(nCount, Key)) { ASSERT(0); continue; }
		//if (!m_paLcaseSP20PsiFactor->Lookup(Key.keymap, Data)) { ASSERT(0); continue; }
		const T_SP20_LPSI_D& Data = m_paLcaseSP20PsiFactor->GetAt(nCount);
		SetValue(GetParam()->GetData(), nCount + 1, Data.LCaseKey, Data, awCols);
	}
}

void CCmdLcomSP20PsiFactorGrid::MakeItemEditingRecord(ROWCOL ncRow, CStringArray& EditingRecord)
{
	CDBDoc* pDoc = m_pDoc;
	DGN_LCASE_K key;
	T_SP20_LPSI_D Data;
	for (int i = 0; i < GetColCount(); i++)
	{
		if (i == 0) key.keymap = _tstol(EditingRecord[i]);
		else ConvStrToData(i, EditingRecord[i], Data);
	}
	
	SetValue(ncRow, key, Data);
}

// CCmdLcomSP20PsiFactorGrid Implementation
BOOL CCmdLcomSP20PsiFactorGrid::GetValue(ROWCOL nRow, DGN_LCASE_K&key, T_SP20_LPSI_D &data)
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

	key = data.LCaseKey;

	return TRUE;
}

BOOL CCmdLcomSP20PsiFactorGrid::SetValue(ROWCOL nRow, const DGN_LCASE_K&key, const T_SP20_LPSI_D &data)
{
	//
	CRowColArray awCols;
	awCols.SetSize(GetColCount());
	for (int i = 0;i < GetColCount(); i++)
		VERIFY(GetColumnNoFor(m_aTitle[i], awCols[i]));
	return SetValue(GetParam()->GetData(), nRow, key, data, awCols);
}

BOOL CCmdLcomSP20PsiFactorGrid::SetValue(CGXData* pData, ROWCOL nRow, const DGN_LCASE_K& key, const T_SP20_LPSI_D& data, CRowColArray& awCols)
{
	CString str;
	for (int i = 0; i < GetColCount(); i++)
	{
		if(i==0) 
		{
			if (key.keymap == 0) str.Empty();
			else str.Format(LCOM_KEY_FMT, key.key.caseKey);
		}
		else if(i==1)
		{
			str.Format(_T("%d"), nRow);
		}
		else
		{
			ConvDataToStr(i, data, str);
		}
		if(i > 3)
		{
			switch(data.eLoadCaseType)
			{
			case e_SP20_Constant:
				if(i == 5 || i == 6)
					str.Empty();
				break;
			case e_SP20_Seismic:
				if((i >= 4 && i <= 6) || (i >= 8 && i <= 10))
					str.Empty();
				break;
			case e_SP20_Crane_vertical:
			case e_SP20_Crane_brake:
				if(i >= 7 && i <= 10)
					str.Empty();
				break;
			case e_SP20_Special:
				if((i >= 4 && i <= 7) || (i >= 9 && i <= 10))
					str.Empty();
				break;
			case e_SP20_Inactive:
				str.Empty();
				break;
			default:
				break;
			}
		}
		pData->StoreValueRowCol(nRow, awCols[i], str, gxOverride);
	}
	switch (data.eLoadCaseType)
	{
	case e_SP20_Constant:
		SetStyleRange(CGXRange(nRow, 6, nRow, 7), CGXStyle().SetEnabled(FALSE));
		break;
	case e_SP20_Seismic:
		SetStyleRange(CGXRange(nRow, 5, nRow, 7), CGXStyle().SetEnabled(FALSE));
		SetStyleRange(CGXRange(nRow, 9, nRow, 11), CGXStyle().SetEnabled(FALSE));
		break;
	case e_SP20_Crane_vertical:
	case e_SP20_Crane_brake:
		SetStyleRange(CGXRange(nRow, 8, nRow, 11), CGXStyle().SetEnabled(FALSE));
		break;
	case e_SP20_Special:
		SetStyleRange(CGXRange(nRow, 5, nRow, 8), CGXStyle().SetEnabled(FALSE));
		SetStyleRange(CGXRange(nRow, 10, nRow, 11), CGXStyle().SetEnabled(FALSE));
		break;
	case e_SP20_Inactive:
		SetStyleRange(CGXRange(nRow, 5, nRow, 11), CGXStyle().SetEnabled(FALSE));
	default:
		break;
	}

	return TRUE;
}

void CCmdLcomSP20PsiFactorGrid::ConvStrToData(int i, CString& value, T_SP20_LPSI_D& data)
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

void CCmdLcomSP20PsiFactorGrid::ConvDataToStr(int i, const T_SP20_LPSI_D& data, CString& value)
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

void CCmdLcomSP20PsiFactorGrid::MakeSearchKey(const DGN_LCASE_K&key, const T_SP20_LPSI_D &data, CStringArray &aKey)
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

void CCmdLcomSP20PsiFactorGrid::GetAllSelectedLcom(CArray<DGN_LCASE_K, DGN_LCASE_K >& aSelKey)
{
	for (int i = 0; i < m_aSelectedKeys.GetSize(); i++)
		aSelKey.Add(m_aSelectedKeys[i]);
}

CString CCmdLcomSP20PsiFactorGrid::GetKeyForRow(ROWCOL ncRow, ROWCOL nkCol)
{
	CString sKey;

	if (IsAppendRow(ncRow)) sKey = _LS(IDS_WG_CMD__ADDD__Append_Row);
	else if (m_nEditMode == addnew) sKey = _LS(IDS_WG_CMD__ADDD__New_Row);
	else if (m_nEditMode == edit) sKey = GetValueRowColBeforeEdit(ncRow, nkCol);
	else sKey = GetValueRowCol(ncRow, nkCol);

	return sKey;
}

BOOL CCmdLcomSP20PsiFactorGrid::IsActiveCol(ROWCOL& nCol)
{
	ROWCOL nRow, nACol;

	GetCurrentCell(nRow, nCol);
	VERIFY(GetColumnNoFor(5, nACol));

	return nCol==nACol;
}

void CCmdLcomSP20PsiFactorGrid::SetHeaderTitle(T_UNIT_SYSTEM &UnitSystem, BOOL bInit)
{
	SetHeaderTitle2((int*)m_aUnit.GetData(), bInit, TRUE);
}

BOOL CCmdLcomSP20PsiFactorGrid::GetStyleRowCol(ROWCOL nRow, ROWCOL nCol, CGXStyle& style, GXModifyType mt, int nType)
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

CString CCmdLcomSP20PsiFactorGrid::GetLoadNameList()
{
	CString strStldList = _T("");
	for (int i = 0; i < m_paLcaseSP20PsiFactor->GetSize(); i++)
	{
		strStldList += m_paLcaseSP20PsiFactor->GetAt(i).LoadCaseName;
		strStldList += _T("\n");
	}
	
	return strStldList;
}

BOOL CCmdLcomSP20PsiFactorGrid::OnPasteFromClipboard(const CGXRange &range)
{
	AfxMessageBox(_LS(IDS_CMD_DONT_SUPPORT_PASTE));
	return FALSE;
}

BOOL CCmdLcomSP20PsiFactorGrid::OnInsertEmptyRecord(ROWCOL nRow)
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

void CCmdLcomSP20PsiFactorGrid::OnModifyCell(ROWCOL nRow, ROWCOL nCol)
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
